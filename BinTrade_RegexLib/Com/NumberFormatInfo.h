#pragma once
#include <vector>
#include <Windows.h>


typedef int INT32;
typedef __int64 INT64;
typedef INT64 I4;

class I4Array
{
	std::vector<I4> data;
public:
	I4Array(int size)
	{
		//data.resize(1);
	}

	int GetNumComponents() const
	{
		return data.size();
	}

	void* GetDirectConstPointerToNonObjectElements()
	{
		return data.data();
	}

	void SetIndex(int value, int index)
	{
		if ((size_t)index <= data.size() - 1 && data.size() > (size_t)0)
			data[index] = value;
		else
			data.push_back(value);
	}
};

typedef I4Array* I4ARRAYREF;


class StringObject
{
#define min(a,b) (((a) < (b)) ? (a) : (b))

	static const wchar_t* GetWC(const char* c, int& lent)
	{
		//const wchar_t* pwcsName; //LPCWSTR //slow
		//int size = MultiByteToWideChar(CP_ACP, 0, c, -1, NULL, 0);
		//pwcsName = new wchar_t[size + 1];
		//MultiByteToWideChar(CP_ACP, 0, c, -1, (LPWSTR)pwcsName, size);
		//return pwcsName;

		//const size_t cSize = strlen(c) + 1;
		//wchar_t* wc = new wchar_t[cSize];
		//mbstowcs(wc, c, cSize);
		//return wc;

		lent = strlen(c) + 1;
		wchar_t* wc = (wchar_t*)malloc(sizeof(wchar_t) * lent);

		size_t outSize;
		mbstowcs_s(&outSize, wc, lent, c, lent - 1);

		//mbstowcs(wc, c, lent);
		return wc;
	}

	const wchar_t* data;
	bool is_all;
	int length;

	static wchar_t* Clone(const wchar_t* buffer, int i)
	{
		const size_t len = min(wcslen(buffer), (size_t)i) + 1;
		wchar_t* clone = (wchar_t*)malloc(sizeof(wchar_t) * len);
		std::memcpy(clone, buffer, len * sizeof(wchar_t));
		clone[len - 1] = '\0';
		return clone;

		//const int len = min(wcslen(buffer), i) + 1;
		//wchar_t* clone = new wchar_t[len];
		//std::memcpy(clone, buffer, len * sizeof(wchar_t));
		//clone[len - 1] = '\0';
		//return clone;

		//wcscpy(clone, buffer);
		//wcscpy_s(clone, i + 1, buffer);
		//wchar_t * szPathClone = _wcsdup(buffer);
		//return szPathClone;
	}

public:
	StringObject(): data('\0'), is_all(false), length(0)
	{
	}

	StringObject(const wchar_t* _data) : data(_data), is_all(false), length(-1)
	{
	}

	StringObject(const char* _data) : is_all(true)
	{
		data = GetWC(_data, length);
	}

	StringObject(const wchar_t* buffer, int i): is_all(true)
	{
		data = Clone(buffer, i);
		length = i;
	}

	~StringObject()
	{
		if (is_all)
		{
			delete[]data;
			//free((void*)data);
		}
	}

	wchar_t* GetBuffer() const
	{
		return const_cast<wchar_t*>(data);
	}

	std::wstring ToString() const
	{
		return data;
	}

	int GetStringLength() const
	{
		if (length == -1)
			return wcslen(data);
		return length;
	}

	static StringObject* NewString(const wchar_t* buffer, int i)
	{
		return new StringObject(buffer, i);
	}
};

typedef wchar_t wchar;
typedef StringObject* STRINGREF;

class NumberFormatInfo
{
public:
	I4ARRAYREF NumberGroupSizes; // numberGroupSize
	I4ARRAYREF CurrencyGroupSizes; // currencyGroupSize
	I4ARRAYREF PercentGroupSizes; // percentGroupSize

	STRINGREF PositiveSign; // positiveSign
	STRINGREF NegativeSign; // negativeSign
	STRINGREF NumberDecimalSeparator; // numberDecimalSeparator
	STRINGREF NumberGroupSeparator; // numberGroupSeparator
	STRINGREF CurrencyDecimalSeparator; // currencyDecimalSeparator
	STRINGREF CurrencyGroupSeparator; // currencyGroupSeparator
	STRINGREF CurrencySymbol; // currencySymbol
	STRINGREF AnsiCurrencySymbol; // ansiCurrencySymbol
	STRINGREF NaNSymbol; // nanSymbol
	STRINGREF PositiveInfinitySymbol; // positiveInfinitySymbol
	STRINGREF NegativeInfinitySymbol; // negativeInfinitySymbol
	STRINGREF PercentDecimalSeparator; // percentDecimalSeparator
	STRINGREF PercentGroupSeparator; // percentGroupSeparator
	STRINGREF PercentSymbol; // percentSymbol
	STRINGREF PerMilleSymbol; // perMilleSymbol

	INT32 iDataItem; // Index into the CultureInfo Table.  Only used from managed code.
	INT32 NumberDecimalDigits; // numberDecimalDigits
	INT32 CurrencyDecimalDigits; // currencyDecimalDigits
	INT32 cPosCurrencyFormat; // positiveCurrencyFormat
	INT32 cNegCurrencyFormat; // negativeCurrencyFormat
	INT32 NumberNegativePattern; // negativeNumberFormat
	INT32 PercentPositivePattern; // positivePercentFormat
	INT32 PercentNegativePattern; // negativePercentFormat
	INT32 PercentDecimalDigits; // percentDecimalDigits

	bool IsReadOnly; // Is this NumberFormatInfo ReadOnly?
	bool bUseUserOverride; // Flag to use user override. Only used from managed code.
	bool bValidForParseAsNumber; // Are the separators set unambiguously for parsing as number?
	bool bValidForParseAsCurrency; // Are the separators set unambiguously for parsing as currency?

	static NumberFormatInfo* InvariantInfo;
	static NumberFormatInfo* CurrentInfo;

	~NumberFormatInfo();

	static NumberFormatInfo* InvariantNumberFormat()
	{
		if (_ins == NULL)
		{
			_ins = new NumberFormatInfo();
			_ins->NumberGroupSizes = new I4Array(3);
			_ins->CurrencyGroupSizes = new I4Array(3);
			_ins->PercentGroupSizes = new I4Array(3);


			_ins->PositiveSign = new StringObject(L"+");
			_ins->NegativeSign = new StringObject(L"-");
			_ins->NumberDecimalSeparator = new StringObject(L".");
			_ins->NumberGroupSeparator = new StringObject(L",");
			_ins->CurrencyDecimalSeparator = new StringObject(L",");
			_ins->CurrencyGroupSeparator = new StringObject(L".");
			_ins->CurrencySymbol = new StringObject(L"\x00a4");
			_ins->AnsiCurrencySymbol = NULL;
			_ins->NaNSymbol = new StringObject(L"NaN");
			_ins->PositiveInfinitySymbol = new StringObject(L"Infinity");
			_ins->NegativeInfinitySymbol = new StringObject(L"-Infinity");
			_ins->PercentDecimalSeparator = new StringObject(L".");
			_ins->PercentGroupSeparator = new StringObject(L",");
			_ins->PercentSymbol = new StringObject(L"%");
			_ins->PerMilleSymbol = new StringObject(L"\u2030");

			_ins->iDataItem = 0;
			_ins->NumberDecimalDigits = 2;
			_ins->CurrencyDecimalDigits = 2;
			_ins->cPosCurrencyFormat = 0;
			_ins->cNegCurrencyFormat = 0;
			_ins->NumberNegativePattern = 1;
			_ins->PercentPositivePattern = 0;
			_ins->PercentNegativePattern = 0;
			_ins->PercentDecimalDigits = 2;

			_ins->IsReadOnly = false;
			_ins->bUseUserOverride = false;
			_ins->bValidForParseAsNumber = true;
			_ins->bValidForParseAsCurrency = true;
		}


		return _ins;
	}

private:
	static NumberFormatInfo* _ins;
};

typedef NumberFormatInfo* NUMFMTREF;

#define NUMBER_MAXDIGITS 50
#define INT32_PRECISION 10
#define UINT32_PRECISION INT32_PRECISION
#define INT64_PRECISION 19
#define UINT64_PRECISION 20
#define FLOAT_PRECISION 7
#define DOUBLE_PRECISION 15
#define DECIMAL_PRECISION 29
#define LARGE_BUFFER_SIZE 600
#define MIN_BUFFER_SIZE 105
#define DECIMAL_ADD  0x00

#define SCALE_NAN 0x80000000
#define SCALE_INF 0x7FFFFFFF

#define PARSE_LEADINGWHITE  0x0001
#define PARSE_TRAILINGWHITE 0x0002
#define PARSE_LEADINGSIGN   0x0004
#define PARSE_TRAILINGSIGN  0x0008
#define PARSE_PARENS        0x0010
#define PARSE_DECIMAL       0x0020
#define PARSE_THOUSANDS     0x0040
#define PARSE_SCIENTIFIC    0x0080
#define PARSE_CURRENCY      0x0100
#define PARSE_HEX			0x0200
#define PARSE_PERCENT       0x0400

struct NUMBER
{
	int precision;
	int scale;
	int sign;
	wchar_t digits[NUMBER_MAXDIGITS + 1];

	NUMBER() : precision(0), scale(0), sign(0), digits{}
	{
	}
};


class __ThrowOK
{
public:
	static void safe_to_throw()
	{
	};
};

inline void ThrowsCOMPlusExceptionWorker()
{
}

#define DEBUG_SAFE_TO_THROW_IN_THIS_BLOCK typedef __ThrowOK __IsSafeToThrow;

#define THROWSCOMPLUSEXCEPTION()                        \
    ThrowsCOMPlusExceptionWorker();                     \
    DEBUG_SAFE_TO_THROW_IN_THIS_BLOCK
