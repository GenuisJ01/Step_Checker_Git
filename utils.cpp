#define CINTERFACE

#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <wbemidl.h>
#include <wbemcli.h>
#include <iphlpapi.h>

// WMI INITIALIZE function from pafish
bool WMI_init(const wchar_t * resource, IWbemServices ** loc) {

    BSTR name;
    IWbemLocator * locator = NULL;
    bool success;

    HRESULT hresult = CoInitializeEx(0, COINIT_MULTITHREADED);

	if (FAILED(hresult)) {
		return FALSE;
	}

	hresult = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT,
		RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);

	if (FAILED(hresult)) {
		CoUninitialize();

		return FALSE;
	}

	hresult = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER,
		IID_IWbemLocator, (LPVOID *) & locator);

	if (FAILED(hresult)) {
		CoUninitialize();

		return FALSE;
	}

	name = SysAllocString(resource);

	// Connect to the namespace with the current user and obtain pointer
	// services to make IWbemServices calls.
	hresult = locator->lpVtbl->ConnectServer(locator, name, NULL, NULL, NULL, 0,
		NULL, NULL, loc);

	success = FAILED(hresult) ? FALSE : TRUE;

	SysFreeString(name);
	locator->lpVtbl->Release(locator);

	return success;
}

typedef bool (*wmi_check_row) (IWbemClassObject *);

// WMI CHECK QUERY function from pafish
bool WMI_check_query(IWbemServices *services, const wchar_t *language, const wchar_t *query,
		wmi_check_row check_row) {
	bool status = FALSE;
	IEnumWbemClassObject *queryrows = NULL;
	BSTR wmilang = SysAllocString(language);
	BSTR wmiquery = SysAllocString(query);

	// Execute the query.
	HRESULT result = services->lpVtbl->ExecQuery(
		services, wmilang, wmiquery, WBEM_FLAG_BIDIRECTIONAL, NULL, &queryrows);

	if (!FAILED(result) && (queryrows != NULL)) {
		IWbemClassObject * batchrows[10];
		ULONG index, count = 0;
		result = WBEM_S_NO_ERROR;

		while (WBEM_S_NO_ERROR == result && status == FALSE) {
			// Retrieve 10 rows (instances) each time.
			// WBEM_INFINITE = 0xFFFFFFFF
			result = queryrows->lpVtbl->Next(queryrows, 0xFFFFFFFF, 10,
				batchrows, &count);

			if (!SUCCEEDED(result)) {
				continue;
			}

			for (index = 0; index < count && status == FALSE; index++) {
				status = check_row(batchrows[index]);

				batchrows[index]->lpVtbl->Release(batchrows[index]);
			}
		}

		queryrows->lpVtbl->Release(queryrows);
	}

	SysFreeString(wmiquery);
	SysFreeString(wmilang);

	return status;
}

// WMI CLEANUP function from pafish
void WMI_cleanup(IWbemServices *services) {
	if (services != NULL) {
		services->lpVtbl->Release(services);
	}

	CoUninitialize();
}