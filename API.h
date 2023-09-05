#include <windows.h>
#include <versionhelpers.h>

// API acquired from Al Khaser to allow use of NtDelayExecution dll

enum API_IDENTIFIER {
    API_NtDelayExecution, 
};

enum API_OS_VERSION {
    WIN_XP, 
    NONE, 
    WIN_XP_SP1,
	WIN_XP_SP2,
	WIN_XP_SP3,
	WIN_VISTA,
	WIN_VISTA_SP1,
	WIN_VISTA_SP2,
	WIN_7,
	WIN_7_SP1,
	WIN_80,
	WIN_81,
	WIN_10,
    VERSION_MAX
};

enum API_OS_BITS {
    ANY, 
    X64_ONLY,
    X86_ONLY
};

struct VERSION_FUNCTION_MAP
{
	API_OS_VERSION Version;
	bool(*Function)();

	VERSION_FUNCTION_MAP(API_OS_VERSION version, bool(*function)())
	{
		Version = version;
		Function = function;
	}

	VERSION_FUNCTION_MAP()
	{
	}
};


struct API_DATA {
    API_IDENTIFIER Identifier;
    const char* Library;
    const char* EntryName;
    API_OS_BITS PlatformBits;
    API_OS_VERSION MinVersion;
    API_OS_VERSION RemovedInVersion;
    bool Available;
    bool ExpectedAvailable;
    void* Pointer;

    API_DATA(API_IDENTIFIER identifier, const char* lib, const char* name, API_OS_BITS bits, API_OS_VERSION minVersion, API_OS_VERSION removedInVersion) {
        Identifier = identifier;
        Library = lib;
        EntryName = name;
        PlatformBits = bits;
        MinVersion = minVersion;
        RemovedInVersion = removedInVersion;
        Available = false;
        ExpectedAvailable = false;
        Pointer = nullptr;
    }
};

const VERSION_FUNCTION_MAP VersionFunctionMap[] = {
	{ API_OS_VERSION::NONE, nullptr },
	{ API_OS_VERSION::WIN_XP, IsWindowsXPOrGreater },
    { API_OS_VERSION::WIN_XP_SP1, IsWindowsXPSP1OrGreater },
	{ API_OS_VERSION::WIN_XP_SP2, IsWindowsXPSP2OrGreater },
	{ API_OS_VERSION::WIN_XP_SP3, IsWindowsXPSP3OrGreater },
	{ API_OS_VERSION::WIN_VISTA, IsWindowsVistaOrGreater },
	{ API_OS_VERSION::WIN_VISTA_SP1, IsWindowsVistaSP1OrGreater },
	{ API_OS_VERSION::WIN_VISTA_SP2, IsWindowsVistaSP2OrGreater },
	{ API_OS_VERSION::WIN_7, IsWindows7OrGreater },
	{ API_OS_VERSION::WIN_7_SP1, IsWindows7SP1OrGreater },
	{ API_OS_VERSION::WIN_80, IsWindows8OrGreater },
	{ API_OS_VERSION::WIN_81, IsWindows8Point1OrGreater },
	{ API_OS_VERSION::WIN_10, IsWindows10OrGreater },
};

class API {
    private: 
        static bool ShouldFunctionExistOnCurrentPlatform(API_OS_BITS bits, API_OS_VERSION minVersion, API_OS_VERSION removedInVersion);
    public: 
        static void Init();
        static void PrintAvailabilityReport();
        static bool IsAvailable(API_IDENTIFIER api);
        static void* GetAPI(API_IDENTIFIER api);
};