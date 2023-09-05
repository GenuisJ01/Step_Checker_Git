#include "../common.h"

/// @brief Uses debug api to check for a remote debugger
/// @return True if not being debugged, false if being debugged
bool remoteDebugger() {

    BOOL result;
    CheckRemoteDebuggerPresent(GetCurrentProcess(), &result);
    return !result;

}

/// @brief Checks for hardware breakpoints, something specific to intel processors, as a method of anti debugging
/// @return True if not being debugged, false if being debugged
bool hardwareBreakpoints() {

    PCONTEXT ctx = PCONTEXT(VirtualAlloc(NULL, sizeof(CONTEXT), MEM_COMMIT, PAGE_READWRITE));

    if (ctx) {

        SecureZeroMemory(ctx, sizeof(CONTEXT));

        ctx->ContextFlags = CONTEXT_DEBUG_REGISTERS;

        if (GetThreadContext(GetCurrentThread(), ctx)) {

            if (ctx->Dr0 !=0 || ctx->Dr1 !=0 || ctx->Dr2 !=0 || ctx->Dr3 !=0) {

                return false;

            }

        }

        VirtualFree(ctx, 0, MEM_RELEASE);

    }

    return true;

}

/// @brief When being debugged, a process doesn't have an LFH, so this checks. Does not check after Windows 10 due to offset constraints
/// @return Returns 1 if not being debugged, 0 if being debugged, -1 if Windows 10
int lfhCheck() {

    PINT_PTR FEH = NULL;
    HANDLE heap = GetProcessHeap();

    /// FEH offset values given by http://rce4fun.blogspot.com/2014/02/anti-debugging-trick-checking-for-low.html
    if (IsWindowsVistaOrGreater() && !IsWindows8OrGreater()) {

        if (wow64) {
            FEH = (PINT_PTR)((CHAR*)heap + 0x178);
        } else {
            FEH = (PINT_PTR)((CHAR*)heap + 0xd4);
        }

    }

    if (IsWindows8OrGreater() && !IsWindows10OrGreater()) {

        if (wow64) {
            FEH = (PINT_PTR)((CHAR*)heap + 0x170);
        } else {
            FEH = (PINT_PTR)((CHAR*)heap + 0xd0);
        }

    }

    CloseHandle(heap);

    if (IsWindows10OrGreater()) {

        return -1;

    }

    if (FEH && *FEH == 0) {

        return 0;

    }

    return 1;

}

/// @brief A check only for earlier versions of windows, if a string causes an error in this case, that means there is no debugger
/// @return True if no debugger present, false if debugger present
bool outputString() {

	DWORD error = 0x29A;

	if ((IsWindowsXPOrGreater() && !IsWindows7OrGreater()) || !IsWindowsXPOrGreater()) {

		SetLastError(error);
		OutputDebugString(_T("abc"));

		if (GetLastError() == error)
			return false;
        
	}

	return true;

}

bool getPID(LPCTSTR process) {

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) {

        std::cerr << "Error in finding Process IDs!\n";
        return true;

    }
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(snapshot, &entry) == true) {

        while (Process32Next(snapshot, &entry) == true) {

            if (stricmp(process, entry.szExeFile) == 0) {

                return true;
                break;

            }

        }

    }

    CloseHandle(snapshot);

    return false;

}

/// @brief Checks for a list of analysis tools given by Al-khaser
/// @return If any of the processes are found, return false, else return true
bool tools() {

    const TCHAR *szProcesses[] = {
		_T("ollydbg.exe"),			// OllyDebug debugger
		_T("ProcessHacker.exe"),	// Process Hacker
		_T("tcpview.exe"),			// Part of Sysinternals Suite
		_T("autoruns.exe"),			// Part of Sysinternals Suite
		_T("autorunsc.exe"),		// Part of Sysinternals Suite
		_T("filemon.exe"),			// Part of Sysinternals Suite
		_T("procmon.exe"),			// Part of Sysinternals Suite
		_T("regmon.exe"),			// Part of Sysinternals Suite
		_T("procexp.exe"),			// Part of Sysinternals Suite
		_T("idaq.exe"),				// IDA Pro Interactive Disassembler
		_T("idaq64.exe"),			// IDA Pro Interactive Disassembler
		_T("ImmunityDebugger.exe"), // ImmunityDebugger
		_T("Wireshark.exe"),		// Wireshark packet sniffer
		_T("dumpcap.exe"),			// Network traffic dump tool
		_T("HookExplorer.exe"),		// Find various types of runtime hooks
		_T("ImportREC.exe"),		// Import Reconstructor
		_T("PETools.exe"),			// PE Tool
		_T("LordPE.exe"),			// LordPE
		_T("SysInspector.exe"),		// ESET SysInspector
		_T("proc_analyzer.exe"),	// Part of SysAnalyzer iDefense
		_T("sysAnalyzer.exe"),		// Part of SysAnalyzer iDefense
		_T("sniff_hit.exe"),		// Part of SysAnalyzer iDefense
		_T("windbg.exe"),			// Microsoft WinDbg
		_T("joeboxcontrol.exe"),	// Part of Joe Sandbox
		_T("joeboxserver.exe"),		// Part of Joe Sandbox
		_T("joeboxserver.exe"),		// Part of Joe Sandbox
		_T("ResourceHacker.exe"),	// Resource Hacker
		_T("x32dbg.exe"),			// x32dbg
		_T("x64dbg.exe"),			// x64dbg
		_T("Fiddler.exe"),			// Fiddler
		_T("httpdebugger.exe"),		// Http Debugger
	};

    int loops = sizeof(szProcesses) / sizeof(szProcesses[0]);

    for (int i = 0; i < loops; i++) {

        if (getPID(szProcesses[i])) {

            return false;

        }

    }

    return true;

}

/// @brief Erases the PE header for antidumping measures
/// @return Always returns true as long as it successfully erases the header
bool eraseHeader() {

    DWORD protect = 0;

    char *address = (char*)GetModuleHandle(NULL);

    VirtualProtect(address, 4096, PAGE_READWRITE, &protect);

    SecureZeroMemory(address, 4096);

    return true;
}

/// @brief Increases the header image size to a giant value, making it difficult to dump
/// @return Always returns true as long as it successfully increases the header image size
bool headerImageSize() {

    PPEB peb = (PPEB)__readgsqword(0x60);

    PCHAR moduleList = (PCHAR)peb->Ldr->Reserved2[1];
    PLDR_DATA_TABLE_ENTRY tableEntry = CONTAINING_RECORD(moduleList, LDR_DATA_TABLE_ENTRY, Reserved1[0]);
    PULONG imageSize = (PULONG)&tableEntry->Reserved3[1];
    *imageSize = (ULONG)((INT_PTR)tableEntry->DllBase + 0x100000);

    return true;

}