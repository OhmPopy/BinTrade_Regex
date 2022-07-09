#include "GetCultureInfo.h"
ComCultureInfo* GetCultureInfo::s_InvariantCultureInfo = NULL;
ComCultureInfo* GetCultureInfo::s_userDefaultCulture = NULL;
ComCultureInfo* GetCultureInfo::s_userDefaultUICulture = NULL;
ComCultureInfo* GetCultureInfo::s_InstalledUICultureInfo = NULL;
ComCultureInfo* GetCultureInfo::s_DefaultThreadCurrentUICulture = NULL;
ComCultureInfo* GetCultureInfo::s_DefaultThreadCurrentCulture = NULL;
