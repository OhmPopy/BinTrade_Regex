#pragma once
#ifndef _UtilsChar_H_
#define _UtilsChar_H_
#include "TypeDef.h"


enum class UnicodeCategory
{
	UppercaseLetter = 0,

	LowercaseLetter = 1,

	TitlecaseLetter = 2,

	ModifierLetter = 3,

	OtherLetter = 4,

	NonSpacingMark = 5,

	SpacingCombiningMark = 6,

	EnclosingMark = 7,

	DecimalDigitNumber = 8,

	LetterNumber = 9,

	OtherNumber = 10,

	SpaceSeparator = 11,

	LineSeparator = 12,

	ParagraphSeparator = 13,

	Control = 14,

	Format = 15,

	Surrogate = 16,

	PrivateUse = 17,

	ConnectorPunctuation = 18,

	DashPunctuation = 19,

	OpenPunctuation = 20,

	ClosePunctuation = 21,

	InitialQuotePunctuation = 22,

	FinalQuotePunctuation = 23,

	OtherPunctuation = 24,

	MathSymbol = 25,

	CurrencySymbol = 26,

	ModifierSymbol = 27,

	OtherSymbol = 28,

	OtherNotAssigned = 29,
};

enum class BidiCategory
{
	LeftToRight = 0,
	LeftToRightEmbedding = 1,
	LeftToRightOverride = 2,
	RightToLeft = 3,
	RightToLeftArabic = 4,
	RightToLeftEmbedding = 5,
	RightToLeftOverride = 6,
	PopDirectionalFormat = 7,
	EuropeanNumber = 8,
	EuropeanNumberSeparator = 9,
	EuropeanNumberTerminator = 10,
	ArabicNumber = 11,
	CommonNumberSeparator = 12,
	NonSpacingMark = 13,
	BoundaryNeutral = 14,
	ParagraphSeparator = 15,
	SegmentSeparator = 16,
	Whitespace = 17,
	OtherNeutrals = 18,
	LeftToRightIsolate = 19,
	RightToLeftIsolate = 20,
	FirstStrongIsolate = 21,
	PopDirectionIsolate = 22,
};

class REGEX_X_API CharUnicodeInfo
{
	static const int UNICODE_CATEGORY_OFFSET = 0;
	static const int BIDI_CATEGORY_OFFSET = 1;

public:
	static bool IsWhiteSpace(wchar_t c)
	{
		const UnicodeCategory uc = GetUnicodeCategory(c);
		// In Unicode 3.0, U+2028 is the only character which is under the category "LineSeparator".
		// And U+2029 is th eonly character which is under the category "ParagraphSeparator".
		switch (uc)
		{
		case UnicodeCategory::SpaceSeparator:
		case UnicodeCategory::LineSeparator:
		case UnicodeCategory::ParagraphSeparator:
			return true;
		default: ;
		}
		return false;
	}

	static UnicodeCategory GetUnicodeCategory(wchar_t ch)
	{
		return InternalGetUnicodeCategory(ch);
	}

	static UnicodeCategory InternalGetUnicodeCategory(int ch);
};

class REGEX_X_API UtilsChar
{
	// Unicode category values from Unicode U+0000 ~ U+00FF. Store them in byte[] array to save space.
	static Regexs::Byte categoryForLatin1[];

public:
	static bool IsLatin1(wchar_t ch)
	{
		return ch <= 255;
	}

	static bool IsWhiteSpace(wchar_t c)
	{
		if (IsLatin1(c))
		{
			return (IsWhiteSpaceLatin1(c));
		}
		return CharUnicodeInfo::IsWhiteSpace(c);
	}

	static bool IsDigit(wchar_t c)
	{
		if (IsLatin1(c))
		{
			return c >= '0' && c <= '9';
		}
		return CharUnicodeInfo::GetUnicodeCategory(c) == UnicodeCategory::DecimalDigitNumber;
	}

	static bool IsControl(wchar_t c)
	{
		if (IsLatin1(c))
		{
			return GetLatin1UnicodeCategory(c) == UnicodeCategory::Control;
		}
		return CharUnicodeInfo::GetUnicodeCategory(c) == UnicodeCategory::Control;
	}

	static bool CheckLetterOrDigit(UnicodeCategory uc)
	{
		switch (uc)
		{
		case UnicodeCategory::UppercaseLetter:
		case UnicodeCategory::LowercaseLetter:
		case UnicodeCategory::TitlecaseLetter:
		case UnicodeCategory::ModifierLetter:
		case UnicodeCategory::OtherLetter:
		case UnicodeCategory::DecimalDigitNumber:
			return true;
		default: ;
		}
		return false;
	}


	static UnicodeCategory GetLatin1UnicodeCategory(wchar_t ch);

	static bool IsWhiteSpaceLatin1(wchar_t c)
	{
		// There are characters which belong to UnicodeCategory.Control but are considered as white spaces.
		// We use code point comparisons for these characters here as a temporary fix.

		// U+0009 = <control> HORIZONTAL TAB
		// U+000a = <control> LINE FEED
		// U+000b = <control> VERTICAL TAB
		// U+000c = <contorl> FORM FEED
		// U+000d = <control> CARRIAGE RETURN
		// U+0085 = <control> NEXT LINE
		// U+00a0 = NO-BREAK SPACE
		if ((c == ' ') || (c >= '\x0009' && c <= '\x000d') || c == '\x00a0' || c == '\x0085')
		{
			return (true);
		}
		return (false);
	}

	static bool IsLetterOrDigit(wchar_t c)
	{
		if (IsLatin1(c))
		{
			return CheckLetterOrDigit(GetLatin1UnicodeCategory(c));
		}
		return CheckLetterOrDigit(CharUnicodeInfo::GetUnicodeCategory(c));
	}
};

#endif
