#define CINTERFACE

bool wow64;

#include "../common.h"

#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif
#define _WIN32_WINNT 0x0501

#ifdef NTDDI_VERSION
#undef NTDDI_VERSION
#endif
#define NTDDI_VERSION 0x05010000

typedef BOOL (WINAPI *wow64Process) (HANDLE, BOOL*);

DWORD processes[1024], cbNeeded;
int processesNum;

//isWow64 method acquired from example use given of IsWow64Process function: https://learn.microsoft.com/en-us/windows/win32/api/wow64apiset/nf-wow64apiset-iswow64process
void init_vmware() {

    if (!EnumProcesses(processes, sizeof(processes), &cbNeeded)) {

        std::cerr << "Could not enumerate processes!";
        std::exit(EXIT_FAILURE);

    }

    processesNum = cbNeeded / sizeof(DWORD);

    BOOL result = FALSE;
    wow64Process success = (wow64Process) GetProcAddress(GetModuleHandle(TEXT("kernel32")), "IsWow64Process");

    if(success != NULL) {

        if (!success(GetCurrentProcess(), &result)) {

            std::cerr << "Failed to check for Wow64";
            std::exit(EXIT_FAILURE);

        }

    }

    wow64 = success;

}

/// @brief Checks if number of processes running is larger than 15
/// @return If realistic number of processes running, return true
bool check_processes_running() {

    return processesNum >= 15;

}

/// @brief Checks if any of the processes are vbox or vmware processes
/// @return If none of the processes are vbox or vmware processes, return true
bool check_vm_processes() {

    TCHAR processName[MAX_PATH] = TEXT("failed");
    std::string processNameString;
    //Recommended processes acquired from Remcos attack
    std::string vbox = "vbox.exe";
    std::string vmware = "vmtoolsd";

    for (int i = 0; i < processesNum; i++) {

        if (processes[i] != 0) {

            HANDLE processIdHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, false, processes[i]);
            if (processIdHandle != NULL) {

                HMODULE hMod;
                DWORD nameBuffer;

                if (EnumProcessModules(processIdHandle, &hMod, sizeof(hMod), &nameBuffer)) {

                    GetModuleBaseName(processIdHandle, hMod, processName, sizeof(processName));

                }

                if (processName == vmware || processName == vbox) {

                    return false;

                }

            }

            CloseHandle (processIdHandle);

        }

    }

    return true;

}

/// @brief Checks for regkeys normally used by vbox
/// @return If no vbox regkeys are found, return true
bool check_vbox_regkeys() {

    HKEY regkey1;
    HKEY regkey2;
    HKEY regkey3;
    HKEY regkey4;
    HKEY regkey5;
    HKEY regkey6;
    HKEY regkey7;
    HKEY regkey8;
    LONG success1;
    LONG success2;
    LONG success3;
    LONG success4;
    LONG success5;
    LONG success6;
    LONG success7;
    LONG success8;
    bool success = true;
    DWORD size;
    char value[1024];

    size = sizeof(value);
    //Recommended regkeys acquired from pafish
    if (wow64) {

        success1 = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\DEVICEMAP\\Scsi\\Scsi Port 0\\Scsi Bus 0\\Target Id 0\\Logical Unit Id 0", 0, KEY_READ | KEY_WOW64_64KEY, &regkey1);
        success2 = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\Description\\System", 0, KEY_READ | KEY_WOW64_64KEY, &regkey2);
        success3 = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Oracle\\VirtualBox Guest Additions", 0, KEY_READ | KEY_WOW64_64KEY, &regkey3);
        success4 = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\Description\\System", 0, KEY_READ | KEY_WOW64_64KEY, &regkey4);
        success5 = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\ACPI\\DSDT\\VBOX__", 0, KEY_READ | KEY_WOW64_64KEY, &regkey5);
        success6 = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\ACPI\\FADT\\VBOX__", 0, KEY_READ | KEY_WOW64_64KEY, &regkey6);
        success7 = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\ACPI\\RSDT\\VBOX__", 0, KEY_READ | KEY_WOW64_64KEY, &regkey7);
        success8 = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System", 0, KEY_READ | KEY_WOW64_64KEY, &regkey8);

    } else {

        success1 = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\DEVICEMAP\\Scsi\\Scsi Port 0\\Scsi Bus 0\\Target Id 0\\Logical Unit Id 0", 0, KEY_READ, &regkey1);
        success2 = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\Description\\System", 0, KEY_READ, &regkey2);
        success3 = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Oracle\\VirtualBox Guest Additions", 0, KEY_READ, &regkey3);
        success4 = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\Description\\System", 0, KEY_READ, &regkey4);
        success5 = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\ACPI\\DSDT\\VBOX__", 0, KEY_READ, &regkey5);
        success6 = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\ACPI\\FADT\\VBOX__", 0, KEY_READ, &regkey6);
        success7 = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\ACPI\\RSDT\\VBOX__", 0, KEY_READ, &regkey7);
        success8 = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System", 0, KEY_READ, &regkey8);

    }
    if (success1 == ERROR_SUCCESS) {

        success1 = RegQueryValueEx(regkey1, "Identifier", NULL, NULL, (BYTE*)value, &size);
        RegCloseKey(regkey1);

        if (success1 == ERROR_SUCCESS) {

            size_t i;
            char lookup [5]= "VBOX";

            for (i = 0; i < strlen(value); i++) {

                value[i] = toupper(value[i]);

            }
            for (i = 0; i < 5; i++) {

                lookup[i] = toupper(lookup[i]);

            }
            if (strstr(value, lookup) != NULL) {

                success = false;

            }

        }

    }
    if (success2 == ERROR_SUCCESS) {

        success2 = RegQueryValueEx(regkey2, "SystemBiosVersion", NULL, NULL, (BYTE*)value, &size);
        RegCloseKey(regkey2);

        if (success2 == ERROR_SUCCESS) {

            size_t i;
            char lookup [5]= "VBOX";

            for (i = 0; i < strlen(value); i++) {

                value[i] = toupper(value[i]);

            }
            for (i = 0; i < 5; i++) {

                lookup[i] = toupper(lookup[i]);

            }
            if (strstr(value, lookup) != NULL) {

                success = false;

            }

        }

    }
    if (success3 == ERROR_SUCCESS) {

        success = false;
        RegCloseKey(regkey3);

    }
    if (success4 == ERROR_SUCCESS) {

        success4 = RegQueryValueEx(regkey2, "VideoBiosVersion", NULL, NULL, (BYTE*)value, &size);
        RegCloseKey(regkey4);

        if (success4 == ERROR_SUCCESS) {

            size_t i;
            char lookup [11]= "VIRTUALBOX";

            for (i = 0; i < strlen(value); i++) {

                value[i] = toupper(value[i]);

            }
            for (i = 0; i < 11; i++) {

                lookup[i] = toupper(lookup[i]);

            }
            if (strstr(value, lookup) != NULL) {

                success = false;

            }

        }

    }
    if (success5 == ERROR_SUCCESS) {

        success = false;
        RegCloseKey(regkey5);
        
    }
    if (success6 == ERROR_SUCCESS) {

        success = false;
        RegCloseKey(regkey6);
        
    }
    if (success7 == ERROR_SUCCESS) {

        success = false;
        RegCloseKey(regkey7);
        
    }
    if (success8 == ERROR_SUCCESS) {

        success8 = RegQueryValueEx(regkey2, "SystemBiosDate", NULL, NULL, (BYTE*)value, &size);
        RegCloseKey(regkey8);

        if (success8 == ERROR_SUCCESS) {

            size_t i;
            char lookup [9]= "06/23/99";

            for (i = 0; i < strlen(value); i++) {

                value[i] = toupper(value[i]);

            }
            for (i = 0; i < 9; i++) {

                lookup[i] = toupper(lookup[i]);

            }
            if (strstr(value, lookup) != NULL) {

                success = false;

            }

        }

    }

    return success;

}

/// @brief Checks sysfiles for vbox sysfiles
/// @return If none of these are found, return true
bool check_vbox_sysfiles() {

    LPCSTR name[18];
    int i = 0;

    //Recommended sysfiles acquired from pafish
    name[0] = "C:\\WINDOWS\\system32\\drivers\\VBoxMouse.sys";
    name[1] = "C:\\WINDOWS\\system32\\drivers\\VBoxGuest.sys";
    name[2] = "C:\\WINDOWS\\system32\\drivers\\VBoxSF.sys";
	name[3] = "C:\\WINDOWS\\system32\\drivers\\VBoxVideo.sys";
    name[4] = "C:\\WINDOWS\\system32\\vboxdisp.dll";
	name[5] = "C:\\WINDOWS\\system32\\vboxhook.dll";
	name[6] = "C:\\WINDOWS\\system32\\vboxmrxnp.dll";
	name[7] = "C:\\WINDOWS\\system32\\vboxogl.dll";
	name[8] = "C:\\WINDOWS\\system32\\vboxoglarrayspu.dll";
	name[9] = "C:\\WINDOWS\\system32\\vboxoglcrutil.dll";
	name[10] = "C:\\WINDOWS\\system32\\vboxoglerrorspu.dll";
	name[11] = "C:\\WINDOWS\\system32\\vboxoglfeedbackspu.dll";
	name[12] = "C:\\WINDOWS\\system32\\vboxoglpackspu.dll";
	name[13] = "C:\\WINDOWS\\system32\\vboxoglpassthroughspu.dll";
	name[14] = "C:\\WINDOWS\\system32\\vboxservice.exe";
	name[15] = "C:\\WINDOWS\\system32\\vboxtray.exe";
	name[16] = "C:\\WINDOWS\\system32\\VBoxControl.exe";
	name[17] = "C:\\program files\\oracle\\virtualbox guest additions\\";

    if (wow64) {

        for (i = 0; i < 18; i++) {

            PVOID redirect = NULL;

            if (Wow64DisableWow64FsRedirection(&redirect) != 0) {

                if (GetFileAttributes(name[i]) != INVALID_FILE_ATTRIBUTES) {

                    Wow64RevertWow64FsRedirection(redirect);
                    return false;

                }

            }

        }

    } else {

        for (i = 0; i < 18; i++) {

            if (GetFileAttributes(name[i]) != INVALID_FILE_ATTRIBUTES) {

                return false;

            }

        }

    }

    return true;

}

/// @brief Checks ip adapter addresses for any vbox mac
/// @return If none of these are found, return true
bool check_vbox_mac() {

    PULONG size = 0;
    ULONG success;

    success = GetAdaptersAddresses(AF_UNSPEC, 0, NULL, NULL, size);

    if (success == ERROR_BUFFER_OVERFLOW) {

        IP_ADAPTER_ADDRESSES * address = (IP_ADAPTER_ADDRESSES *)LocalAlloc(LMEM_ZEROINIT, (SIZE_T) size);
        
        if (address) {

            GetAdaptersAddresses(AF_UNSPEC, 0, 0, address, size);
            while (address) {

                if (address->PhysicalAddressLength==6) {
                    //Recommended mac starts given by pafish
                    if (!memcmp("\x08\x00\x27", address->PhysicalAddress, 3)) {

                        return false;

                    }
                    address = address->Next;

                }

            }
            LocalFree(address);

        }

    }

    return true;

}

// WMI CHECK ROW and DEVICES functions from pafish
bool vbox_WMI_check_row(IWbemClassObject *row) {

    CIMTYPE type = CIM_ILLEGAL;
	VARIANT value;

	HRESULT hresult = row->lpVtbl->Get(row, L"DeviceId", 0, &value, &type, 0);

	if (FAILED(hresult) || V_VT(&value) == VT_NULL || type != CIM_STRING) {
		return false;
	}

	return (wcsstr(V_BSTR(&value), L"PCI\\VEN_80EE&DEV_CAFE") != NULL) ? false : true;

}

/// @brief Checks the WMI queries for a certain Vbox query
/// @return Returns true if it is not found, false if it is found
bool check_vbox_wmi() {

    IWbemServices * services = NULL;

    if (!WMI_init(L"root\\cimv2", &services)) {

        return false;

    }

    bool result = WMI_check_query(services, L"WQL", L"SELECT DeviceId FROM Win32_PnPEntity", &vbox_WMI_check_row);

    WMI_cleanup(services);

    return result;

}

/// @brief Checks for regkeys normally used by vmware
/// @return If no vmware regkeys are found, return true
bool check_vmware_regkeys() {

    HKEY regkey1;
    HKEY regkey2;
    HKEY regkey3;
    HKEY regkey4;
    LONG success1;
    LONG success2;
    LONG success3;
    LONG success4;
    bool success = true;
    DWORD size;
    char value[1024];

    size = sizeof(value);
    //Recommended regkeys acquired from pafish
    if (wow64) {

        success1 = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\DEVICEMAP\\Scsi\\Scsi Port 0\\Scsi Bus 0\\Target Id 0\\Logical Unit Id 0", 0, KEY_READ | KEY_WOW64_64KEY, &regkey1);
        success2 = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\DEVICEMAP\\Scsi\\Scsi Port 1\\Scsi Bus 0\\Target Id 0\\Logical Unit Id 0", 0, KEY_READ | KEY_WOW64_64KEY, &regkey2);
        success3 = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\DEVICEMAP\\Scsi\\Scsi Port 2\\Scsi Bus 0\\Target Id 0\\Logical Unit Id 0", 0, KEY_READ | KEY_WOW64_64KEY, &regkey3);
        success4 = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\VMware, Inc.\\VMware Tools", 0, KEY_READ | KEY_WOW64_64KEY, &regkey4);

    } else {

        success1 = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\DEVICEMAP\\Scsi\\Scsi Port 0\\Scsi Bus 0\\Target Id 0\\Logical Unit Id 0", 0, KEY_READ, &regkey1);
        success2 = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\DEVICEMAP\\Scsi\\Scsi Port 1\\Scsi Bus 0\\Target Id 0\\Logical Unit Id 0", 0, KEY_READ, &regkey2);
        success3 = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\DEVICEMAP\\Scsi\\Scsi Port 2\\Scsi Bus 0\\Target Id 0\\Logical Unit Id 0", 0, KEY_READ, &regkey3);
        success4 = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\VMware, Inc.\\VMware Tools", 0, KEY_READ, &regkey4);

    }
    if (success1 == ERROR_SUCCESS) {

        success1 = RegQueryValueEx(regkey1, "Identifier", NULL, NULL, (BYTE*)value, &size);
        RegCloseKey(regkey1);

        if (success1 == ERROR_SUCCESS) {

            size_t i;
            char lookup [7]= "VMWARE";

            for (i = 0; i < strlen(value); i++) {

                value[i] = toupper(value[i]);

            }
            for (i = 0; i < 7; i++) {

                lookup[i] = toupper(lookup[i]);

            }
            if (strstr(value, lookup) != NULL) {

                success = false;

            }

        }

    }
    if (success2 == ERROR_SUCCESS) {

        success2 = RegQueryValueEx(regkey1, "Identifier", NULL, NULL, (BYTE*)value, &size);
        RegCloseKey(regkey2);

        if (success2 == ERROR_SUCCESS) {

            size_t i;
            char lookup [7]= "VMWARE";

            for (i = 0; i < strlen(value); i++) {

                value[i] = toupper(value[i]);

            }
            for (i = 0; i < 7; i++) {

                lookup[i] = toupper(lookup[i]);

            }
            if (strstr(value, lookup) != NULL) {

                success = false;

            }

        }

    }
    if (success3 == ERROR_SUCCESS) {

        success3 = RegQueryValueEx(regkey1, "Identifier", NULL, NULL, (BYTE*)value, &size);
        RegCloseKey(regkey3);

        if (success3 == ERROR_SUCCESS) {

            size_t i;
            char lookup [7]= "VMWARE";

            for (i = 0; i < strlen(value); i++) {

                value[i] = toupper(value[i]);

            }
            for (i = 0; i < 7; i++) {

                lookup[i] = toupper(lookup[i]);

            }
            if (strstr(value, lookup) != NULL) {

                success = false;

            }

        }

    }
    if (success4 == ERROR_SUCCESS) {

        success = false;
        RegCloseKey(regkey4);
        
    }

    return success;

}

/// @brief Checks sysfiles for vmware sysfiles
/// @return If none of these are found, return true
bool check_vmware_sysfiles() {

    LPCSTR name[2];
    int i = 0;

    // Recommended sysfiles acquired from pafish
    name[0] = "C:\\WINDOWS\\system32\\drivers\\vmmouse.sys";
    name[1] = "C:\\WINDOWS\\system32\\drivers\\vmhgfs.sys";

    if (wow64) {

        for (i = 0; i < 2; i++) {

            PVOID redirect = NULL;

            if (Wow64DisableWow64FsRedirection(&redirect) != 0) {

                if (GetFileAttributes(name[i]) != INVALID_FILE_ATTRIBUTES) {

                    Wow64RevertWow64FsRedirection(redirect);
                    return false;

                }

            }

        }

    } else {

        for (i = 0; i < 2; i++) {

            if (GetFileAttributes(name[i]) != INVALID_FILE_ATTRIBUTES) {

                return false;

            }

        }

    }

    return true;

}

/// @brief Checks ip adapter addresses for any vmware macs
/// @return If none of these are found, return true
bool check_vmware_mac() {

    PULONG size = 0;
    ULONG success;

    success = GetAdaptersAddresses(AF_UNSPEC, 0, NULL, NULL, size);

    if (success == ERROR_BUFFER_OVERFLOW) {

        IP_ADAPTER_ADDRESSES * address = (IP_ADAPTER_ADDRESSES *)LocalAlloc(LMEM_ZEROINIT, (SIZE_T) size);
        
        if (address) {

            GetAdaptersAddresses(AF_UNSPEC, 0, 0, address, size);
            while (address) {

                if (address->PhysicalAddressLength==6) {
                    //Recommended mac starts given by pafish
                    if (!memcmp("\x00\x0C\x29", address->PhysicalAddress, 3)) {

                        return false;

                    } else if (!memcmp("\x00\x05\x69", address->PhysicalAddress, 3)) {

                        return false;

                    } else if (!memcmp("\x00\x1C\x14", address->PhysicalAddress, 3)) {

                        return false;

                    } else if (!memcmp("\x00\x50\x56", address->PhysicalAddress, 3)) {

                        return false;

                    }
                    address = address->Next;

                }

            }
            LocalFree(address);

        }

    }

    return true;

}

// WMI CHECK ROW and SERIALS functions from pafish
bool vmware_wmi_check_row(IWbemClassObject *row) {
	CIMTYPE type = CIM_ILLEGAL;
	VARIANT value;

	HRESULT hresult = row->lpVtbl->Get(row, L"SerialNumber", 0, &value, &type, 0);

	if (FAILED(hresult) || V_VT(&value) == VT_NULL || type != CIM_STRING) {
		return FALSE;
	}

	return (wcsstr(V_BSTR(&value), L"VMware") != NULL) ? false : true;
}

/// @brief Checks the WMI queries for a certain VMware query
/// @return Returns true if it is not found, false if it is found
bool check_vmware_wmi() {

    IWbemServices *services = NULL;

	if (WMI_init(L"root\\cimv2", &services) != TRUE) {
		return false;
	}

	bool result = WMI_check_query(services, L"WQL", L"SELECT SerialNumber FROM Win32_Bios",
			&vmware_wmi_check_row);

	WMI_cleanup(services);

	return result;

}