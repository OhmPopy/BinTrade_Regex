#include "UtilsString.h"


int UtilsString::Compare(const std::wstring& strA, const std::wstring& strB, StringComparison ordinal)
{
	if (ordinal == StringComparison::Ordinal)
		return CompareOrdinal(strA, strB);


	return strA.compare(strB);
}

std::wstring UtilsCultureInfo::IntToString(ComCultureInfo* culture_info, int i)
{
	return std::to_wstring(i);
}

//PASS
std::wstring UtilsCultureInfo::CharToString(ComCultureInfo* culture_info, wchar_t i)
{
	return std::wstring(1, i);
}


#pragma region Compare 2


INT32 FastCompareStringHelper(DWORD* strAChars, INT32 countA, DWORD* strBChars, INT32 countB)
{
	INT32 count = (countA < countB) ? countA : countB;
	ptrdiff_t diff = (char*)strAChars - (char*)strBChars;

#if defined(_WIN64) || defined(ALIGN_ACCESS)
	int alignmentA = ((SIZE_T)strAChars) & (sizeof(SIZE_T) - 1);
	int alignmentB = ((SIZE_T)strBChars) & (sizeof(SIZE_T) - 1);
#endif // _WIN64 || ALIGN_ACCESS

#if defined(ALIGN_ACCESS)
	if ((!IS_ALIGNED((size_t)strAChars, sizeof(DWORD)) ||
		!IS_ALIGNED((size_t)strBChars, sizeof(DWORD))) &&
		(abs(alignmentA - alignmentB) != 4))
	{
		_ASSERTE(IS_ALIGNED((size_t)strAChars, sizeof(WCHAR)));
		_ASSERTE(IS_ALIGNED((size_t)strBChars, sizeof(WCHAR)));
		LPWSTR ptr2 = (WCHAR *)strBChars;

		while ((count -= 1) >= 0)
		{
			if ((*((WCHAR*)((char *)ptr2 + diff)) - *ptr2) != 0)
			{
				return ((int)*((WCHAR*)((char *)ptr2 + diff)) - (int)*ptr2);
			}
			++ptr2;
		}
	}
	else
#endif // ALIGN_ACCESS
	{
#if defined(_WIN64) || defined(ALIGN_ACCESS)
		if (abs(alignmentA - alignmentB) == 4)
		{
			if ((alignmentA == 2) || (alignmentB == 2))
			{
				LPWSTR ptr2 = (WCHAR *)strBChars;

				if ((*((WCHAR*)((char *)ptr2 + diff)) - *ptr2) != 0)
				{
					return ((int)*((WCHAR*)((char *)ptr2 + diff)) - (int)*ptr2);
				}
				strBChars = (DWORD*)(++ptr2);
				count -= 1;
			}
		}
#endif // WIN64 || ALIGN_ACCESS

		// Loop comparing a DWORD at a time.
		while ((count -= 2) >= 0)
		{
			if ((*((DWORD*)((char*)strBChars + diff)) - *strBChars) != 0)
			{
				LPWSTR ptr1 = (WCHAR*)((char*)strBChars + diff);
				LPWSTR ptr2 = (WCHAR*)strBChars;
				if (*ptr1 != *ptr2)
				{
					return ((int)*ptr1 - (int)*ptr2);
				}
				return ((int)*(ptr1 + 1) - (int)*(ptr2 + 1));
			}
			++strBChars;
		}

		int c;
		if (count == -1)
			if ((c = *((WCHAR*)((char*)strBChars + diff)) - *((WCHAR*)strBChars)) != 0)
				return c;
	}

	return countA - countB;
}

INT32 CompareOrdinalEx(StringObject* strA, INT32 indexA, StringObject* strB, INT32 indexB, INT32 count)
{
	// This runtime test is handled in the managed wrapper.
	_ASSERTE(strA != NULL && strB != NULL);

	//If any of our indices are negative throw an exception.
	if (count < 0)
	{
		throw L"ArgumentOutOfRange_MustBePositive";
	}
	if (indexA < 0)
	{
		throw L"ArgumentOutOfRange_MustBePositive";
	}
	if (indexB < 0)
	{
		throw L"ArgumentOutOfRange_MustBePositive";
	}

	DWORD* strAChars = (DWORD*)strA->GetBuffer();
	int strALength = strA->GetStringLength();

	DWORD* strBChars = (DWORD*)strB->GetBuffer();
	int strBLength = strB->GetStringLength();

	int countA = count;
	int countB = count;

	//Do a lot of range checking to make sure that everything is kosher and legit.
	if (count > (strALength - indexA))
	{
		countA = strALength - indexA;
		if (countA < 0)
			throw L"ArgumentOutOfRange_Index";
	}

	if (count > (strBLength - indexB))
	{
		countB = strBLength - indexB;
		if (countB < 0)
			throw L"ArgumentOutOfRange_Index";
	}

	// Set up the loop variables.
	strAChars = (DWORD*)((WCHAR*)strAChars + indexA);
	strBChars = (DWORD*)((WCHAR*)strBChars + indexB);

	INT32 result = FastCompareStringHelper(strAChars, countA, strBChars, countB);

	return result;
}

static int CompareOrdinalHelper(const std::wstring& strA, const std::wstring& strB)
{
	int length = min(strA.length(), strB.length());
	int diffOffset = -1;

	{
		const wchar_t* a = strA.c_str();
		const wchar_t* b = strB.c_str();

		// unroll the loop
		while (length >= 10)
		{
			if (*(int*)a != *(int*)b)
			{
				diffOffset = 0;
				break;
			}

			if (*(int*)(a + 2) != *(int*)(b + 2))
			{
				diffOffset = 2;
				break;
			}

			if (*(int*)(a + 4) != *(int*)(b + 4))
			{
				diffOffset = 4;
				break;
			}

			if (*(int*)(a + 6) != *(int*)(b + 6))
			{
				diffOffset = 6;
				break;
			}

			if (*(int*)(a + 8) != *(int*)(b + 8))
			{
				diffOffset = 8;
				break;
			}
			a += 10;
			b += 10;
			length -= 10;
		}

		if (diffOffset != -1)
		{
			// we already see a difference in the unrolled loop above
			a += diffOffset;
			b += diffOffset;
			int order;
			if ((order = (int)*a - (int)*b) != 0)
			{
				return order;
			}
			return ((int)*(a + 1) - (int)*(b + 1));
		}

		// now go back to slower code path and do comparison on 4 bytes one time.
		// Following code also take advantage of the fact strings will 
		// use even numbers of characters (runtime will have a extra zero at the end.)
		// so even if length is 1 here, we can still do the comparsion.  
		while (length > 0)
		{
			if (*(int*)a != *(int*)b)
			{
				break;
			}
			a += 2;
			b += 2;
			length -= 2;
		}

		if (length > 0)
		{
			int c;
			// found a different int on above loop
			if ((c = (int)*a - (int)*b) != 0)
			{
				return c;
			}
			return ((int)*(a + 1) - (int)*(b + 1));
		}

		// At this point, we have compared all the characters in at least one string.
		// The longer string will be larger.
		return strA.length() - strB.length();
	}
}

int UtilsString::CompareOrdinal(const std::wstring& strA, const std::wstring& strB)
{
	if (strA == strB)
	{
		return 0;
	}

	if (strA.length() > 0 && strB.length() > 0)
	{
		// Most common case, first character is different.
		if ((strA[0] - strB[0]) != 0)
		{
			return strA[0] - strB[0];
		}
	}

	return CompareOrdinalHelper(strA, strB);
}

int UtilsString::CompareOrdinal(const std::wstring& strA, int indexA, const std::wstring& strB, int indexB, size_t length)
{
	if (strA == L"" || strB == L"")
	{
		if (strA == strB)
		{
			//they're both null;
			return 0;
		}
		return strA == L"" ? -1 : 1; //-1 if A is null, 1 if B is null.
	}

	return CompareOrdinalEx(StringObject::NewString(strA.c_str(), strA.size()), indexA, StringObject::NewString(strB.c_str(), strB.size()), indexB, length);
}


#pragma endregion
