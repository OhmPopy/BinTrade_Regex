#ifndef _GetCultureInfo_H_
#define _GetCultureInfo_H_
#include <cassert>
#include <tchar.h>

#include "ComCultureInfo.h"
#include "Core/Exception.h"

#include <TypeDef.h>

#define NumItems(s) (sizeof(s) / sizeof(s[0]))
// Language ID for CHT (Taiwan)
#define LANGID_ZH_TW            0x0404
// Language ID for CHT (Hong-Kong)
#define LANGID_ZH_HK            0x0c04


//https://stackoverflow.com/questions/52849233/getuserpreferreduilanguages-never-returns-more-than-two-languages
//https://referencesource.microsoft.com/#mscorlib/system/globalization/cultureinfo.cs,3ba66495ea223b26
//https://github.com/CyberSys/coreclr-mono/blob/83b2cb83b32faa45b4f790237b5c5e259692294a/src/classlibnative/nls/nlsinfo.cpp
//https://github.com/CyberSys/coreclr-mono/search?q=InternalGetUserDefaultUILanguage

class REGEX_X_API  GetCultureInfo
{
public:
	const int LOCALE_TRADITIONAL_SPANISH = 0x040a;


	static ComCultureInfo* s_InvariantCultureInfo;
	static ComCultureInfo* s_userDefaultCulture;

	static ComCultureInfo* s_userDefaultUICulture;
	static ComCultureInfo* s_InstalledUICultureInfo;
	static ComCultureInfo* s_DefaultThreadCurrentUICulture;
	static ComCultureInfo* s_DefaultThreadCurrentCulture;

	static bool HaveInit()
	{
		return s_InvariantCultureInfo != NULL;
	}

	static bool Init()
	{
		if (s_InvariantCultureInfo == NULL)
		{
			s_InvariantCultureInfo = ComCultureInfo::CreateSpecificCulture(L"");
			InstalledUICulture();
		}
		// First we set it to Invariant in case someone needs it before we're done finding it.
		// For example, if we throw an exception in InitUserDefaultCulture, we will still need an valid
		// s_userDefaultCulture to be used in Thread.CurrentCulture.
		s_userDefaultCulture = s_InvariantCultureInfo;
		s_userDefaultUICulture = s_InvariantCultureInfo;

		s_userDefaultCulture = InitUserDefaultCulture();
		s_userDefaultUICulture = InitUserDefaultUICulture();

		return true;
	}

	static BOOL InternalGetDefaultLocaleName(INT32 langType, std::wstring& defaultLocaleName)
	{
		assert((langType == LOCALE_SYSTEM_DEFAULT) || (langType == LOCALE_USER_DEFAULT));

		BOOL result;

		WCHAR strName[LOCALE_NAME_MAX_LENGTH];
		int size = 0;

		if (langType == LOCALE_SYSTEM_DEFAULT)
		{
			size = GetSystemDefaultLocaleName(strName, NumItems(strName));
		}
		else
		{
			_ASSERT(langType == LOCALE_USER_DEFAULT);
			size = GetUserDefaultLocaleName(strName, NumItems(strName));
		}

		// Not found, either not longhorn (no LOCALE_SNAME) or not a valid name
		if (size == 0)
		{
			result = false;
		}
		else
		{
			defaultLocaleName = strName;
			result = true;
		}
		return result;
	}

	static std::wstring GetDefaultLocaleName(int localeType)
	{
		bool value_asset = localeType == LOCALE_USER_DEFAULT || localeType == LOCALE_SYSTEM_DEFAULT;
		if (value_asset)
			assert("[CultureInfo.GetDefaultLocaleName] localeType must be LOCALE_USER_DEFAULT or LOCALE_SYSTEM_DEFAULT");

		std::wstring localeName;
		if (InternalGetDefaultLocaleName(localeType, localeName))
		{
			return localeName;
		}
		return L"";
	}

	static ComCultureInfo* InitUserDefaultCulture()
	{
		std::wstring strDefault = GetDefaultLocaleName(LOCALE_USER_DEFAULT);
		if (strDefault == L"")
		{
			strDefault = GetDefaultLocaleName(LOCALE_SYSTEM_DEFAULT);

			if (strDefault == L"")
			{
				// If system default doesn't work, keep using the invariant
				return ComCultureInfo::InvariantCulture();
			}
		}
		ComCultureInfo* temp = GetCultureByName(strDefault, true);


		return temp;
	}

	static void ThrowLastError()
	{
	}

	static BOOL InternalGetUserDefaultUILanguage(std::wstring& userDefaultUiLanguage)
	{
		BOOL result;

		WCHAR wszBuffer[LOCALE_NAME_MAX_LENGTH];
		LPCWSTR wszLangName = NULL;

		int res = 0;
		ULONG uLangCount = 0;
		ULONG uBufLen = 0;
		res = GetUserPreferredUILanguages(MUI_LANGUAGE_NAME, &uLangCount, NULL, &uBufLen);
		if (res == 0)
			ThrowLastError();


		WCHAR* sPreferredLanguages = NULL;

		if (uBufLen > 0 && uLangCount > 0)
		{
			sPreferredLanguages = new WCHAR[uBufLen];
			res = GetUserPreferredUILanguages(MUI_LANGUAGE_NAME, &uLangCount, sPreferredLanguages, &uBufLen);

			if (res == 0)
				ThrowLastError();

			wszLangName = sPreferredLanguages;
			delete sPreferredLanguages;
			// Review size_t to int conversion (possible loss of data).
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4267)
#endif
			res = wcslen(wszLangName) + 1;
#ifdef _MSC_VER
#pragma warning(pop)
#endif
		}
		else
		{
			res = 0;
		}

		if (res == 0)
		{
			res = GetUserDefaultLocaleName(wszBuffer, NumItems(wszBuffer));
			wszLangName = wszBuffer;
		}


		// If not found, either not longhorn (no LOCALE_SNAME) or not a valid name
		if (res == 0)
		{
			// Didn't find string, return an empty string.
			result = false;
		}
		else
		{
			userDefaultUiLanguage = wszLangName;
			result = true;
		}

		// Return the found language name.  LCID should be found one already.
		return result;
	}

	static std::wstring GetUserDefaultUILanguage()
	{
		std::wstring userDefaultUiLanguage;
		if (InternalGetUserDefaultUILanguage(userDefaultUiLanguage))
		{
			return userDefaultUiLanguage;
		}
		return L"";
	}

	//
	// This is the equivalence of the Win32 GetUserDefaultLCID()
	//
	static ComCultureInfo* UserDefaultCulture()
	{
		ComCultureInfo* temp = s_userDefaultCulture;
		if (temp == NULL)
		{
			//
			// setting the s_userDefaultCulture with invariant culture before intializing it is a protection
			// against recursion problem just in case if somebody called CurrentCulture from the CultureInfo
			// creation path. the recursion can happen if the current user culture is a replaced custom culture.
			//

			s_userDefaultCulture = ComCultureInfo::InvariantCulture();
			temp = InitUserDefaultCulture();
			s_userDefaultCulture = temp;
		}
		return temp;
	}

	static ComCultureInfo* InitUserDefaultUICulture()
	{
		const std::wstring strDefault = GetUserDefaultUILanguage();

		// In most of cases, UserDefaultCulture == UserDefaultUICulture, so we should use the same instance if possible.
		if (strDefault == UserDefaultCulture()->Name)
		{
			return UserDefaultCulture();
		}

		ComCultureInfo* temp = GetCultureByName(strDefault, true);

		if (temp == NULL)
		{
			return ComCultureInfo::InvariantCulture();
		}


		return temp;
	}

	// We do this to try to return the system UI language and the default user languages
	// The callers should have a fallback if this fails (like Invariant)
	static ComCultureInfo* GetCultureByName(std::wstring name, bool userOverride)
	{
		// Try to get our culture
		try
		{
			//return userOverride ? new ComCultureInfo(name) : ComCultureInfo::CreateSpecificCulture(name);
			return ComCultureInfo::CreateSpecificCulture(name);
		}
		catch (Regexs::ArgumentException)
		{
		}

		return NULL;
	}

	static BOOL IsZhTwSku()
	{
		LPCWSTR DEFAULT_REGION_NAME_0404 = _T("\x53f0\x7063");

		if (::GetSystemDefaultUILanguage() == LANGID_ZH_TW)
		{
			WCHAR wszBuffer[32];
			int result = ::GetLocaleInfoW(LANGID_ZH_TW, LOCALE_SNATIVECTRYNAME, wszBuffer, 32);
			if (result)
			{
				if (wcsncmp(wszBuffer, DEFAULT_REGION_NAME_0404, 3) != 0)
				{
					return true;
				}
			}
		}
		return false;
	}

	static BOOL InternalGetSystemDefaultUILanguage(std::wstring& systemDefaultUiLanguage)
	{
		BOOL result;

		WCHAR localeName[LOCALE_NAME_MAX_LENGTH];

		int systemDefaultUiLcid = GetSystemDefaultUILanguage();
		if (systemDefaultUiLcid == LANGID_ZH_TW)
		{
			if (!IsZhTwSku())
			{
				systemDefaultUiLcid = LANGID_ZH_HK;
			}
		}

		int length = LCIDToLocaleName(systemDefaultUiLcid, localeName, NumItems(localeName), 0);
		if (length == 0)
		{
			result = false;
		}
		else
		{
			systemDefaultUiLanguage = localeName;
			result = true;
		}

		return result;
	}

	static std::wstring ImpGetSystemDefaultUILanguage()
	{
		std::wstring systemDefaultUiLanguage;
		if (InternalGetSystemDefaultUILanguage(systemDefaultUiLanguage))
		{
			return systemDefaultUiLanguage;
		}
		return L"";
	}

	static ComCultureInfo* InstalledUICulture()
	{
		ComCultureInfo* temp = s_InstalledUICultureInfo;
		if (temp == NULL)
		{
			std::wstring strDefault = ImpGetSystemDefaultUILanguage();
			temp = GetCultureByName(strDefault, true);

			if (temp == NULL)
			{
				temp = ComCultureInfo::InvariantCulture();
			}

			s_InstalledUICultureInfo = temp;
		}
		return temp;
	}
};


#endif
