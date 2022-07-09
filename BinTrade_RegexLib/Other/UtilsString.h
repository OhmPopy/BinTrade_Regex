#pragma once

#include <string>
#include <Com/ComCultureInfo.h>

enum StringComparison
{
	CurrentCulture = 0,
	CurrentCultureIgnoreCase = 1,
	InvariantCulture = 2,
	InvariantCultureIgnoreCase = 3,
	Ordinal = 4,
	OrdinalIgnoreCase = 5
};

class REGEX_X_API UtilsString
{
public:
	static std::wstring Formats(ComCultureInfo* culture_info, const wchar_t* str, wchar_t group_char, wchar_t group, wchar_t group1, wchar_t group2, wchar_t group3)
	{
		return StringInitSpe(group_char, group, group1, group2, group3);
	}

	static int Compare(const std::wstring& strA, const std::wstring& strB, StringComparison ordinal);

	static int CompareOrdinal(const std::wstring& strA, const std::wstring& strB);

	static int CompareOrdinal(const std::wstring& wstring, int i, const std::wstring& text, int index, size_t size);

	static std::wstring wstring_convert_from_bytes(const std::vector<wchar_t>& v, int start, int leng)
	{
		std::wstring result;
		result.resize(leng);
		int j = 0;
		for (int i = start; i < start + leng; ++i)
		{
			//result.push_back(v.at(i));// \0 khong tuong thich

			auto item = v.at(i);
			result[j] = item;
			j++;
		}
		return result;
	}

	static std::wstring Cint(const int* v, int leng)
	{
		std::wstring result;
		result.resize(leng);
		int j = 0;
		for (int i = 0; i < leng; ++i)
		{
			auto item = v[i];
			result[j] = item;
			j++;
		}
		return result;
	}

	static std::wstring StringInitSpe(int i)
	{
		std::vector<int> vector;
		vector.push_back(i);
		return Cint(vector.data(), vector.size());
	}

	static std::wstring StringInitSpe(int i, int i1)
	{
		std::vector<int> vector;
		vector.push_back(i);
		vector.push_back(i1);
		return Cint(vector.data(), vector.size());
	}

	static std::wstring StringInitSpe(int i, int i1, int i2)
	{
		std::vector<int> vector;
		vector.push_back(i);
		vector.push_back(i1);
		vector.push_back(i2);
		return Cint(vector.data(), vector.size());
	}

	static std::wstring StringInitSpe(int i, int i1, int i2, int i3)
	{
		std::vector<int> vector;
		vector.push_back(i);
		vector.push_back(i1);
		vector.push_back(i2);
		vector.push_back(i3);
		return Cint(vector.data(), vector.size());
	}

	static std::wstring StringInitSpe(int i, int i1, int i2, int i3, int i4)
	{
		std::vector<int> vector;
		vector.push_back(i);
		vector.push_back(i1);
		vector.push_back(i2);
		vector.push_back(i3);
		vector.push_back(i4);
		return Cint(vector.data(), vector.size());
	}

	static std::wstring StringInitSpe(int i, int i1, int i2, int i3, int i4, int i5)
	{
		std::vector<int> vector;
		vector.push_back(i);
		vector.push_back(i1);
		vector.push_back(i2);
		vector.push_back(i3);
		vector.push_back(i4);
		vector.push_back(i5);
		return Cint(vector.data(), vector.size());
	}

	static std::wstring StringInitSpe(int i, int i1, int i2, int i3, int i4, int i5, int i6)
	{
		std::vector<int> vector;
		vector.push_back(i);
		vector.push_back(i1);
		vector.push_back(i2);
		vector.push_back(i3);
		vector.push_back(i4);
		vector.push_back(i5);
		vector.push_back(i6);
		return Cint(vector.data(), vector.size());
	}

	static std::wstring StringInitSpe(int i, int i1, int i2, int i3, int i4, int i5, int i6, int i7)
	{
		std::vector<int> vector;
		vector.push_back(i);
		vector.push_back(i1);
		vector.push_back(i2);
		vector.push_back(i3);
		vector.push_back(i4);
		vector.push_back(i5);
		vector.push_back(i6);
		vector.push_back(i7);
		return Cint(vector.data(), vector.size());
	}

	static std::wstring StringInitSpe(int i, int i1, int i2, int i3, int i4, int i5, int i6, int i7, int i8)
	{
		std::vector<int> vector;
		vector.push_back(i);
		vector.push_back(i1);
		vector.push_back(i2);
		vector.push_back(i3);
		vector.push_back(i4);
		vector.push_back(i5);
		vector.push_back(i6);
		vector.push_back(i7);
		vector.push_back(i8);
		return Cint(vector.data(), vector.size());
	}

	static std::wstring StringInitSpe(int i, int i1, int i2, int i3, int i4, int i5, int i6, int i7, int i8, int i9)
	{
		std::vector<int> vector;
		vector.push_back(i);
		vector.push_back(i1);
		vector.push_back(i2);
		vector.push_back(i3);
		vector.push_back(i4);
		vector.push_back(i5);
		vector.push_back(i6);
		vector.push_back(i7);
		vector.push_back(i8);
		vector.push_back(i9);
		return Cint(vector.data(), vector.size());
	}

	static std::wstring StringInitSpe(int i, int i1, int i2, int i3, int i4, int i5, int i6, int i7, int i8, int i9, int i10)
	{
		std::vector<int> vector;
		vector.push_back(i);
		vector.push_back(i1);
		vector.push_back(i2);
		vector.push_back(i3);
		vector.push_back(i4);
		vector.push_back(i5);
		vector.push_back(i6);
		vector.push_back(i7);
		vector.push_back(i8);
		vector.push_back(i9);
		vector.push_back(i10);
		return Cint(vector.data(), vector.size());
	}

	static std::wstring StringInitSpe(int i, int i1, int i2, int i3, int i4, int i5, int i6, int i7, int i8, int i9, int i10, int i11)
	{
		std::vector<int> vector;
		vector.push_back(i);
		vector.push_back(i1);
		vector.push_back(i2);
		vector.push_back(i3);
		vector.push_back(i4);
		vector.push_back(i5);
		vector.push_back(i6);
		vector.push_back(i7);
		vector.push_back(i8);
		vector.push_back(i9);
		vector.push_back(i10);
		vector.push_back(i11);
		return Cint(vector.data(), vector.size());
	}

	static std::wstring StringInitSpe(int i, int i1, int i2, int i3, int i4, int i5, int i6, int i7, int i8, int i9, int i10, int i11, int i12)
	{
		std::vector<int> vector;
		vector.push_back(i);
		vector.push_back(i1);
		vector.push_back(i2);
		vector.push_back(i3);
		vector.push_back(i4);
		vector.push_back(i5);
		vector.push_back(i6);
		vector.push_back(i7);
		vector.push_back(i8);
		vector.push_back(i9);
		vector.push_back(i10);
		vector.push_back(i11);
		vector.push_back(i12);
		return Cint(vector.data(), vector.size());
	}

	static std::wstring StringInitSpe(int i, int i1, int i2, int i3, int i4, int i5, int i6, int i7, int i8, int i9, int i10, int i11, int i12, int i13)
	{
		std::vector<int> vector;
		vector.push_back(i);
		vector.push_back(i1);
		vector.push_back(i2);
		vector.push_back(i3);
		vector.push_back(i4);
		vector.push_back(i5);
		vector.push_back(i6);
		vector.push_back(i7);
		vector.push_back(i8);
		vector.push_back(i9);
		vector.push_back(i10);
		vector.push_back(i11);
		vector.push_back(i12);
		vector.push_back(i13);
		return Cint(vector.data(), vector.size());
	}

	static std::wstring StringInitSpe(int i, int i1, int i2, int i3, int i4, int i5, int i6, int i7, int i8, int i9, int i10, int i11, int i12, int i13, int i14)
	{
		std::vector<int> vector;
		vector.push_back(i);
		vector.push_back(i1);
		vector.push_back(i2);
		vector.push_back(i3);
		vector.push_back(i4);
		vector.push_back(i5);
		vector.push_back(i6);
		vector.push_back(i7);
		vector.push_back(i8);
		vector.push_back(i9);
		vector.push_back(i10);
		vector.push_back(i11);
		vector.push_back(i12);
		vector.push_back(i13);
		vector.push_back(i14);
		return Cint(vector.data(), vector.size());
	}
};


class REGEX_X_API UtilsCultureInfo
{
public:
	static std::wstring IntToString(ComCultureInfo* culture_info, int i);
	static std::wstring CharToString(ComCultureInfo* culture_info, wchar_t i);
};
