#pragma once


#include <string>
#include <sstream>
#include <vector>
#include <stdexcept>

#include "UtilsChar.h"


#pragma warning( disable : 4244 )


class REGEX_X_API StringHelper
{
public:
	static std::wstring toLower(std::wstring source);

	static std::wstring toUpper(std::wstring source);

	static std::wstring trimStart(std::wstring source, const std::wstring& trimChars = L" \t\n\r\v\f")
	{
		return source.erase(0, source.find_first_not_of(trimChars));
	}

	static std::wstring trimEnd(std::wstring source, const std::wstring& trimChars = L" \t\n\r\v\f")
	{
		return source.erase(source.find_last_not_of(trimChars) + 1);
	}

	static std::wstring trim(std::wstring source, const std::wstring& trimChars = L" \t\n\r\v\f")
	{
		return trimStart(trimEnd(source, trimChars), trimChars);
	}

	static std::wstring replace(std::wstring source, const std::wstring& find, const std::wstring& replace)
	{
		std::size_t pos = 0;
		while ((pos = source.find(find, pos)) != std::wstring::npos)
		{
			source.replace(pos, find.length(), replace);
			pos += replace.length();
		}
		return source;
	}

	static bool startsWith(const std::wstring& source, const std::wstring& value)
	{
		if (source.length() < value.length())
			return false;
		else
			return source.compare(0, value.length(), value) == 0;
	}

	static bool endsWith(const std::wstring& source, const std::wstring& value)
	{
		if (source.length() < value.length())
			return false;
		else
			return source.compare(source.length() - value.length(), value.length(), value) == 0;
	}

	static std::vector<std::wstring> split(const std::wstring& source, wchar_t delimiter)
	{
		std::vector<std::wstring> output;
		std::wistringstream ss(source);
		std::wstring nextItem;

		while (std::getline(ss, nextItem, delimiter))
		{
			output.push_back(nextItem);
		}

		return output;
	}

	static bool isEmptyOrWhiteSpace(const std::wstring& source)
	{
		if (source.length() == 0)
			return true;
		else
		{
			for (std::size_t index = 0; index < source.length(); index++)
			{
				if (!UtilsChar::IsWhiteSpace(source[index]))
					return false;
			}

			return true;
		}
	}

	static std::wstring toString(const wchar_t& c)
	{
		return std::wstring(1,c);
	}
};
