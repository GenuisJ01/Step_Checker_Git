#include <windows.h>
#include <wbemcli.h>

typedef bool (*wmi_check_row) (IWbemClassObject *);

bool WMI_init(const wchar_t * resource, IWbemServices ** loc);
bool WMI_check_query(IWbemServices *services, const wchar_t *language, const wchar_t *query,
		wmi_check_row check_row);
void WMI_cleanup(IWbemServices *services);