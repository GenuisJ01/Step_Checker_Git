#include "API.h"
#include <libloaderapi.h>
#include <iostream>
#include <assert.h>
#define API_COUNT (sizeof(ApiData)/sizeof(*ApiData))

// API acquired from Al Khaser to allow use of NtDelayExecution dll

API_DATA ApiData[] = {
    {API_IDENTIFIER::API_NtDelayExecution,      "ntdll.dll",        "NtDelayExecution",         API_OS_BITS::ANY,       API_OS_VERSION::WIN_XP,         API_OS_VERSION::NONE }
};

void API::Init()
{
	for (int i = 0; i < API_COUNT; i++)
	{
		ApiData[i].ExpectedAvailable = ShouldFunctionExistOnCurrentPlatform(ApiData[i].PlatformBits, ApiData[i].MinVersion, ApiData[i].RemovedInVersion);
		SetDllDirectoryA("E:/Step_Checker_Git/PROJECT");

		HMODULE hLib = LoadLibraryA(ApiData[i].Library);

		if (hLib == NULL)
		{
			ApiData[i].Available = false;
			continue;
		}
		ApiData[i].Pointer = (void *)GetProcAddress(hLib, ApiData[i].EntryName);
		if (ApiData[i].Pointer == NULL)
		{
			ApiData[i].Available = false;
			continue;
		}
		else
		{
			ApiData[i].Available = true;
		}
	}
}

bool API::ShouldFunctionExistOnCurrentPlatform(API_OS_BITS bits, API_OS_VERSION minVersion, API_OS_VERSION removedInVersion)
{
	// check if the API should exist on the OS

	// does it meet bitness requirements?
	if (bits != API_OS_BITS::ANY)
	{
#ifdef ENV64BIT
		if (bits != API_OS_BITS::X64_ONLY)
			return false;
#endif
#ifdef ENV32BIT
		if (bits != API_OS_BITS::X86_ONLY)
			return false;
#endif
	}

	// does it meet minimum version
	bool foundMinVer = false;
	bool metMinimumRequirement = false;

	for (int i = 0; i < 12; i++)
	{
		if (i == 0)
			continue;
		if (VersionFunctionMap[i].Version == minVersion)
		{
			foundMinVer = true;
			metMinimumRequirement = VersionFunctionMap[i].Function();
		}
	}
	if (!foundMinVer)
	{
		printf("ERROR: Minimum version value was invalid.\n");
		assert(false);
		return false;
	}
	if (!metMinimumRequirement)
		return false;

	// if there's no maximum OS restriction, the API should exist
	if (removedInVersion == API_OS_VERSION::NONE)
		return true;

	// we have an upper restriction. was the API removed in this version?
	bool foundRemovedVer = false;
	bool metMaximumRequirement = false;
	for (int i = 0; i < API_OS_VERSION::VERSION_MAX; i++)
	{
		if (VersionFunctionMap[i].Version == removedInVersion)
		{
			foundRemovedVer = true;
			metMaximumRequirement = !VersionFunctionMap[i].Function();
		}
	}
	if (!foundRemovedVer)
	{
		printf("ERROR: Removed version value was invalid.\n");
		assert(false);
		return false;
	}

	return metMaximumRequirement;
}

void API::PrintAvailabilityReport()
{
	int warningCount = 0;
	for (int i = 0; i < API_COUNT; i++)
	{
		if (ApiData[i].ExpectedAvailable && !ApiData[i].Available)
		{
			printf("[*] Warning: API %s!%s was expected to exist but was not found.\n", ApiData[i].Library, ApiData[i].EntryName);
			warningCount += 1;
		}
	}
	if (warningCount == 0)
	{
		printf("[*] All APIs present and accounted for.\n");
	}
}

bool API::IsAvailable(API_IDENTIFIER api)
{
	for (int i = 0; i < API_COUNT; i++)
	{
		if (ApiData[i].Identifier == api)
		{
			return ApiData[i].Available;
		}
	}
	assert(false);
	return false;
}

void* API::GetAPI(API_IDENTIFIER api)
{
	for (int i = 0; i < API_COUNT; i++)
	{
		if (ApiData[i].Identifier == api)
		{
			if (ApiData[i].Available)
			{
				return ApiData[i].Pointer;
			}
			else
			{
				return nullptr;
			}
		}
	}
	assert(false);
	return nullptr;
}