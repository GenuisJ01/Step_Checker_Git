#define CINTERFACE

#include "../common.h"

/// @brief Checks if number of cores/processors is a realistic amount
/// @return Returns true if number of cores is 2 or higher, else returns false
bool check_cores() {

    const auto processor_count = std::thread::hardware_concurrency();
    
    if (processor_count < 2) {
        
        return false;
        
    } else {

        return true;

    }

}

/// @brief Checks if disk size is a realistic amount
/// @return Returns true if size of disk is 80 GB or higher, else returns false
bool check_disk_size() {

    const std::filesystem::space_info si = std::filesystem::space("/home");
    intmax_t size = static_cast<std::intmax_t>(si.capacity);
    // 80 GB Check
    if (size < 85899345920) {

        return false;

    } else {

        return true;

    }
}

/// @brief Checks if memory space is a realistic amount
/// @return Returns true if total memory space is 1GB or higher, else returns false
bool check_memory_space() {

    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);
    DWORDLONG totalMem = memInfo.ullTotalPhys;
    // 1 GB Check
    if(memInfo.ullTotalPhys < 1073741824LL) {

        return false;

    } else {

        return true;

    }

}

// Using the pafish system, check rows for model values in WMI to check rows
bool model_check_WMI_row(IWbemClassObject *row) {

    CIMTYPE type = CIM_ILLEGAL;
    VARIANT value;

    HRESULT hresult = row->lpVtbl->Get(row, L"Model", 0, &value, 0, 0);

    if (FAILED(hresult) || V_VT(&value) == VT_NULL || type != CIM_STRING) {
		return false;
	}

    return (wcsstr(V_BSTR(&value), L"VirtualBox") != NULL ||
            wcsstr(V_BSTR(&value), L"HVM domU") != NULL ||
            wcsstr(V_BSTR(&value), L"VMWare") != NULL) ? false : true;
}

/// @brief Checks models through WMI to see if there are any known VM models
/// @return Returns true if it is not using known VM models, returns false otherwise
bool check_model() {

    IWbemServices *services = NULL;
    bool result;

    if (!WMI_init(L"ROOT\\CIMV2", &services)) {

        return false;

    }

    result = !WMI_check_query(services, L"WQL", L"SELECT * FROM Win32_ComputerSystem", &model_check_WMI_row);

    WMI_cleanup(services);

    return result;

}

bool manufacturer_check_WMI_row(IWbemClassObject *row) {

    CIMTYPE type = CIM_ILLEGAL;
    VARIANT value;

    HRESULT hresult = row->lpVtbl->Get(row, L"Manufacturer", 0, &value, 0, 0);

    if (FAILED(hresult) || V_VT(&value) == VT_NULL || type != CIM_STRING) {
		return false;
	}

    return (wcsstr(V_BSTR(&value), L"VMWare") != NULL ||
            wcsstr(V_BSTR(&value), L"Xen") != NULL ||
            wcsstr(V_BSTR(&value), L"innotek GmbH") != NULL ||
            wcsstr(V_BSTR(&value), L"QEMU") != NULL) ? false : true;
}

/// @brief Checks manufacturers through WMI to see if there are any known VM manufacturers
/// @return Returns true if it is not using known VM manufacturers, returns false otherwise
bool check_manufacturer() {

    IWbemServices *services = NULL;
    bool result;

    if (!WMI_init(L"ROOT\\CIMV2", &services)) {

        return false;

    }

    result = !WMI_check_query(services, L"WQL", L"SELECT * FROM Win32_ComputerSystem", &manufacturer_check_WMI_row);

    WMI_cleanup(services);

    return result;

}

/// @brief Checks if WMI in row is empty
/// @return If empty, returns false
bool check_WMI_count(IWbemClassObject *row) {

    CIMTYPE type = CIM_ILLEGAL;
    VARIANT value;
    int count = 0;

    HRESULT hresult = row->lpVtbl->BeginEnumeration(row, WBEM_FLAG_KEYS_ONLY);
    if (FAILED(hresult)) {
		return false;
	}
    while (hresult != WBEM_S_NO_MORE_DATA) {
        hresult = row->lpVtbl->Next(row, 0, NULL, &value, NULL, NULL);
        count++;
    }

    hresult = row->lpVtbl->EndEnumeration(row);

    if (count == 0) {

        return false;

    } else {

        return true;

    }
}

/// @brief Checks certain win32 entries to see if they are empty
/// @return If they are empty, returns false, else returns true
bool check_win32_entries() {

    IWbemServices *services = NULL;
    bool result;

    if (!WMI_init(L"ROOT\\CIMV2", &services)) {

        return false;

    }

    if (WMI_check_query(services, L"WQL", L"SELECT * FROM Win32_CacheMemory", &check_WMI_count) && 
        WMI_check_query(services, L"WQL", L"SELECT * FROM Win32_PhysicalMemory", &check_WMI_count) && 
        WMI_check_query(services, L"WQL", L"SELECT * FROM Win32_MemoryDevice", &check_WMI_count) && 
        WMI_check_query(services, L"WQL", L"SELECT * FROM Win32_MemoryArray", &check_WMI_count) && 
        WMI_check_query(services, L"WQL", L"SELECT * FROM Win32_VoltageProbe", &check_WMI_count) && 
        WMI_check_query(services, L"WQL", L"SELECT * FROM Win32_PortConnector", &check_WMI_count) && 
        WMI_check_query(services, L"WQL", L"SELECT * FROM Win32_SMBIOSMemory", &check_WMI_count) && 
        WMI_check_query(services, L"WQL", L"SELECT * FROM Win32_PerfFormattedData_Counters_ThermalZoneInformation", &check_WMI_count)) {

            result = true;

        } else {

            result = false;

        }

    WMI_cleanup(services);

    return result;
}

/// @brief Checks certain CIM entries to see if they are empty
/// @return If they are empty, returns false, else returns true
bool check_CIM_entries() {

    IWbemServices *services = NULL;
    bool result;

    if (!WMI_init(L"ROOT\\CIMV2", &services)) {

        return false;

    }

    if (WMI_check_query(services, L"WQL", L"SELECT * FROM CIM_Memory", &check_WMI_count) && 
        WMI_check_query(services, L"WQL", L"SELECT * FROM CIM_NumericSensor", &check_WMI_count) && 
        WMI_check_query(services, L"WQL", L"SELECT * FROM CIM_PhysicalConnector", &check_WMI_count) && 
        WMI_check_query(services, L"WQL", L"SELECT * FROM CIM_Sensor", &check_WMI_count) && 
        WMI_check_query(services, L"WQL", L"SELECT * FROM CIM_Slot", &check_WMI_count) && 
        WMI_check_query(services, L"WQL", L"SELECT * FROM CIM_TemperatureSensor", &check_WMI_count) && 
        WMI_check_query(services, L"WQL", L"SELECT * FROM CIM_VoltageSensor", &check_WMI_count)) {

            result = true;

        } else {

            result = false;

        }

    WMI_cleanup(services);

    return result;
}