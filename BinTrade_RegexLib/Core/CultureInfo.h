#pragma once

#include <string>
#include <Com/ComCultureInfo.h>

namespace Regexs
{
 
	class REGEX_X_API TextInfo
	{
		enum Tristate : byte
		{
			NotInitialized,
			True,
			False,
		};

		static TextInfo* s_Invariant;

	public:
		std::wstring m_listSeparator;

		bool m_isReadOnly = false;

		std::wstring m_cultureName; // Name of the culture that created this text info
		ComCultureInfo* m_cultureData; // Data record for the culture that made us, not for this textinfo
		std::wstring m_textInfoName; // Name of the text info we're using (ie: m_cultureData.STEXTINFO)
		Tristate m_IsAsciiCasingSameAsInvariant = Tristate::NotInitialized;


		// Invariant text info
		static TextInfo* Invariant()
		{
			if (s_Invariant == NULL)
				s_Invariant = new TextInfo(ComCultureInfo::InvariantCulture());
			return s_Invariant;
		}

		TextInfo(ComCultureInfo* cultureData)
		{
			this->m_cultureData = cultureData;
			this->m_cultureName = this->m_cultureData->CultureName;
			this->m_textInfoName = this->m_cultureData->CultureName;
		}

		std::wstring customCultureName;


		static bool IsAscii(wchar_t c)
		{
			return c < 0x80;
		}

		static wchar_t ToLowerAsciiInvariant(wchar_t c)
		{
			if ('A' <= c && c <= 'Z')
			{
				c = (wchar_t)(c | 0x20);
			}
			return c;
		}

		static wchar_t ToUpperAsciiInvariant(wchar_t c)
		{
			if ('a' <= c && c <= 'z')
			{
				c = (wchar_t)(c & ~0x20);
			}
			return c;
		}

		static BOOL IsInvariantLocale(const std::wstring& localeName)
		{
			return localeName.length() == 0;
		}

		static wchar_t InternalChangeCaseChar(const std::wstring& localeNameUNSAFE, const wchar_t& wch, bool isToUpper)
		{
			wchar_t retVal = '\0';


			BOOL isInvariantLocale = IsInvariantLocale(localeNameUNSAFE);
			// Check for Invariant to avoid A/V in LCMapStringEx
			DWORD linguisticCasing = (isInvariantLocale) ? 0 : LCMAP_LINGUISTIC_CASING;

			int ret_LCMapStringEx = LCMapStringEx(localeNameUNSAFE.c_str(),
			                                      isToUpper ? LCMAP_UPPERCASE | linguisticCasing : LCMAP_LOWERCASE | linguisticCasing,
			                                      &wch,
			                                      1,
			                                      &retVal,
			                                      1,
			                                      NULL,
			                                      NULL,
			                                      0);

			if (0 == ret_LCMapStringEx)
			{
				// return value of 0 indicates failure and error value is supposed to be set.
				// shouldn't ever really happen
				_ASSERTE(!"catastrophic failure calling NewApis::InternalChangeCaseChar!  This could be a CultureInfo or CompareInfo bug (bad localeName string) or maybe a GCHole.");
			}

			return retVal;
		}

		bool IsAsciiCasingSameAsInvariant()
		{
			if (m_IsAsciiCasingSameAsInvariant == Tristate::NotInitialized)
			{
				int compare = std::wstring(L"abcdefghijklmnopqrstuvwxyz").compare(L"ABCDEFGHIJKLMNOPQRSTUVWXYZ");
				m_IsAsciiCasingSameAsInvariant = compare == 0
					                                 ? Tristate::True
					                                 : Tristate::False;
			}
			return m_IsAsciiCasingSameAsInvariant == Tristate::True;
		}

		wchar_t ToLower(wchar_t c);

		wchar_t ToUpper(wchar_t c);
	};


	class REGEX_X_API  UtilsCharInternal
	{
	public:
		static wchar_t ToLower(const wchar_t& input);
		static wchar_t ToLower(const wchar_t& input, ComCultureInfo* culture_info);

		static wchar_t ToUpper(wchar_t input);
		static wchar_t ToUpper(const wchar_t& input, ComCultureInfo* culture_info);
	};

	class REGEX_X_API Environment
	{
	public:
		static std::wstring NewLine;
	};
}
