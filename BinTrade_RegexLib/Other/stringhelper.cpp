#include "stringhelper.h"
#include <algorithm>
#include <cctype>


std::wstring StringHelper::toLower(std::wstring source)
{
	std::transform(source.begin(), source.end(), source.begin(), [](unsigned char c) { return std::tolower(c); });
	return source;
}

std::wstring StringHelper::toUpper(std::wstring source)
{
	std::transform(source.begin(), source.end(), source.begin(), [](unsigned char c) { return std::toupper(c); });
	return source;
}
