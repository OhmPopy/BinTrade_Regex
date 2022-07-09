#include "NumberFormatInfo.h"

NumberFormatInfo* NumberFormatInfo::_ins = NULL;
NumberFormatInfo* NumberFormatInfo::InvariantInfo = NULL;
NumberFormatInfo* NumberFormatInfo::CurrentInfo = NULL;

#define SAFE_DELETE(x) {if((x)!=NULL){ delete (x); (x) = NULL;}}

NumberFormatInfo::~NumberFormatInfo()
{
	SAFE_DELETE(NumberGroupSizes)
	SAFE_DELETE(CurrencyGroupSizes)
	SAFE_DELETE(PercentGroupSizes)

	SAFE_DELETE(PositiveSign)
	SAFE_DELETE(NegativeSign)
	SAFE_DELETE(NumberDecimalSeparator)
	SAFE_DELETE(NumberGroupSeparator)
	SAFE_DELETE(CurrencyDecimalSeparator)
	SAFE_DELETE(CurrencyGroupSeparator)
	SAFE_DELETE(CurrencySymbol)
	SAFE_DELETE(AnsiCurrencySymbol)
	SAFE_DELETE(NaNSymbol)
	SAFE_DELETE(PositiveInfinitySymbol)
	SAFE_DELETE(NegativeInfinitySymbol)
	SAFE_DELETE(PercentDecimalSeparator)
	SAFE_DELETE(PercentGroupSeparator)
	SAFE_DELETE(PercentSymbol)
	SAFE_DELETE(PerMilleSymbol)
}
