#pragma once
#include "CultureInfo.h"

namespace Regexs
{
	TextInfo* TextInfo::s_Invariant = NULL;
	std::wstring Environment::NewLine = L"\r\n";

	wchar_t TextInfo::ToLower(wchar_t c)
	{
		if (IsAscii(c) && IsAsciiCasingSameAsInvariant())
		{
			return ToLowerAsciiInvariant(c);
		}
		return (InternalChangeCaseChar(this->m_textInfoName, c, false));
	}

	wchar_t TextInfo::ToUpper(wchar_t c)
	{
		if (IsAscii(c) && IsAsciiCasingSameAsInvariant())
		{
			return ToUpperAsciiInvariant(c);
		}
		return (InternalChangeCaseChar(this->m_textInfoName, c, true));
	}

	wchar_t UtilsCharInternal::ToLower(const wchar_t& input)
	{
		return ToLower(input, ComCultureInfo::CurrentCulture());
	}

	wchar_t UtilsCharInternal::ToLower(const wchar_t& input, ComCultureInfo* culture_info)
	{
		TextInfo text_info(culture_info);
		return text_info.ToLower(input);
	}

	wchar_t UtilsCharInternal::ToUpper(wchar_t input)
	{
		return ToUpper(input, ComCultureInfo::CurrentCulture());
	}

	wchar_t UtilsCharInternal::ToUpper(const wchar_t& input, ComCultureInfo* culture_info)
	{
		TextInfo text_info(culture_info);
		return text_info.ToUpper(input);
	}
}
