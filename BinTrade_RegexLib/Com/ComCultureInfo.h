#ifndef _ComCultureInfo_H_
#define _ComCultureInfo_H_

#include <map>
#include "NumberFormatInfo.h"

#include <TypeDef.h>

class REGEX_X_API ComCultureInfo
{
	static std::map<int, ComCultureInfo*> _array;
	static std::map<std::wstring, int> _address;
	NumberFormatInfo* _number_format_info;
	static void Init();
	static void InitPri();
	static bool _is_init;


public:
	ComCultureInfo();

	ComCultureInfo(std::wstring name);

	ComCultureInfo(std::wstring name, bool useUserOverride);

	~ComCultureInfo()
	{
		delete _number_format_info;
	}

	std::wstring DisplayName;
	std::wstring EnglishName;
	std::wstring IetfLanguageTag;
	int KeyboardLayoutId;
	int LCID;
	std::wstring Name;
	std::wstring NativeName;
	std::wstring ThreeLetterISOLanguageName;
	std::wstring ThreeLetterWindowsLanguageName;
	std::wstring TwoLetterISOLanguageName;

	int ANSICodePage;
	std::wstring CultureName;
	int MacCodePage;
	int OEMCodePage;

	NumberFormatInfo* NumberFormat() const;
	static ComCultureInfo* InvariantCulture();
	static ComCultureInfo* CurrentCulture();
	static ComCultureInfo* CreateSpecificCulture(std::wstring name);

	std::wstring ToString() const;
	static std::vector<std::wstring> GetAllCultureName();
};


#endif
