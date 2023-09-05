extern bool wow64;

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <windows.h>
#include <winuser.h>
#include <iostream>
#include <psapi.h>
#include <vector>
#include <tchar.h>
#include <string>
#include <winternl.h>
#include <iphlpapi.h>
#include <wbemidl.h>
#include <wbemcli.h>
#include <debugapi.h>
#include <processthreadsapi.h>
#include <memoryapi.h>
#include <versionhelpers.h>
#include <intrin.h>
#include <thread>
#include <filesystem>
#include <winnt.h>
#include <ntstatus.h>
#include <icmpapi.h>
#include <ipexport.h>
#include <inaddr.h>
#include <iptypes.h>
#include <threadpoollegacyapiset.h>
#include <winreg.h>
#include <tlhelp32.h>

#include "utils.hpp"

#include "analysis/analysis.hpp"
#include "hardware_detection/hardware.hpp"
#include "human_interaction/interaction.hpp"
#include "timing/timing.hpp"
#include "vmware_detection/vmware.hpp"