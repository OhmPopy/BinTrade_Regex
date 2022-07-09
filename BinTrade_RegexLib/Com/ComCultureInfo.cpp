#include "ComCultureInfo.h"

#include "GetCultureInfo.h"

std::map<int, ComCultureInfo*> ComCultureInfo::_array;
std::map<std::wstring, int> ComCultureInfo::_address;
bool ComCultureInfo::_is_init = false;

void ComCultureInfo::Init()
{
	if (!_is_init)
	{
		InitPri();
		_is_init = true;
	}
}

NumberFormatInfo* ComCultureInfo::NumberFormat() const
{
	return _number_format_info;
}

ComCultureInfo* ComCultureInfo::InvariantCulture()
{
	Init();
	return _array[127];
}

ComCultureInfo::ComCultureInfo(): _number_format_info(new NumberFormatInfo()), KeyboardLayoutId(0), LCID(0), ANSICodePage(0), MacCodePage(0), OEMCodePage(0)
{
}

ComCultureInfo::ComCultureInfo(std::wstring name): ComCultureInfo(name, true)
{
}

ComCultureInfo::ComCultureInfo(std::wstring name, bool useUserOverride): _number_format_info(NULL), KeyboardLayoutId(0), LCID(0), ANSICodePage(0), MacCodePage(0), OEMCodePage(0)
{
	ComCultureInfo* temp = CreateSpecificCulture(name);

	*this = *temp;
}

ComCultureInfo* ComCultureInfo::CurrentCulture()
{
	Init();
	if (!GetCultureInfo::HaveInit())
		GetCultureInfo::Init();
	return GetCultureInfo::s_userDefaultCulture;
}

std::wstring ComCultureInfo::ToString() const
{
	return Name;
}

std::vector<std::wstring> ComCultureInfo::GetAllCultureName()
{
	std::vector<std::wstring> result;
	for (auto& kvpair : _address)
	{
		result.push_back(kvpair.first);
	}
	return result;
}

ComCultureInfo* ComCultureInfo::CreateSpecificCulture(std::wstring name)
{
	Init();

	if (_address.find(name) == _address.end())
	{
		return InvariantCulture();
	}
	else
	{
		return _array[_address[name]];
	}
}
