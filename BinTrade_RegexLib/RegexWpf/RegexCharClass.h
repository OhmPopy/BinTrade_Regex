#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "CultureInfo.h"
#include "IComparer.h"
#include "objectForRegex.h"
#include "stringbuilder.h"
#include "UtilsChar.h"


// This RegexCharClass class provides the "set of Unicode chars" functionality
// used by the regexp engine.

// The main function of RegexCharClass is as a builder to turn ranges, characters and 
// Unicode categories into a single string.  This string is used as a black box 
// representation of a character class by the rest of Regex.  The format is as follows.
//
// Char index   Use
//      0       Flags - currently this only holds the "negate" flag
//      1       length of the string representing the "set" portion, eg [a-z0-9] only has a "set"
//      2       length of the string representing the "category" portion, eg [\p{Lu}] only has a "category"
//      3...m   The set.  These are a series of ranges which define the characters included in the set. 
//              To determine if a given character is in the set, we binary search over this set of ranges
//              and see where the character should go.  Based on whether the ending index is odd or even,
//              we know if the character is in the set. 
//      m+1...n The categories.  This is a list of UnicodeCategory enum values which describe categories
//              included in this class.  


namespace Regexs
{
	class REGEX_X_API RegexCharClass : public objectForRegex
	{
		class StaticConstructor : public objectForRegex
		{
		public:
			StaticConstructor();
		};

		// Lower case mapping descriptor.
		class LowerCaseMapping : public objectForRegex
		{
		public:
			LowerCaseMapping();
			LowerCaseMapping(wchar_t chMin, wchar_t chMax, int lcOp, int data);

			wchar_t _chMin = L'\0';
			wchar_t _chMax = L'\0';
			int _lcOp = 0;
			int _data = 0;
		};

		/*
		* SingleRange
		*
		* A first/last pair representing a single range of characters.
		*/
		class SingleRange final : public objectForRegex
		{
		public:
			SingleRange()
			{
			}

			SingleRange(wchar_t first, wchar_t last);

			wchar_t _first = L'\0';
			wchar_t _last = L'\0';
		};

		/*
		 * SingleRangeComparer
		 *
		 * For sorting ranges; compare based on the first char in the range.
		 */
		class SingleRangeComparer final : public IComparer<SingleRange*>
		{
		public:
			int Compare(SingleRange* x, SingleRange* y) override;
		};


		// instance data
	private:
		std::vector<SingleRange*> _rangelist;
		std::vector<PtrObjectForRegex> _rangelistPtr;
		StringBuilder* _categories;
		bool _canonical = false;
		bool _negate = false;
		RegexCharClass* _subtractor;

		// Constants
		static constexpr int FLAGS = 0;
		static constexpr int SETLENGTH = 1;
		static constexpr int CATEGORYLENGTH = 2;
		static constexpr int SETSTART = 3;

		static constexpr wchar_t Nullchar = L'\0';
		static constexpr wchar_t Lastchar = L'\uFFFF';

		static constexpr wchar_t GroupChar = static_cast<wchar_t>(0);


		static constexpr short SpaceConst = 100;
		static constexpr short NotSpaceConst = -100;

		static constexpr wchar_t ZeroWidthJoiner = L'\u200D';
		static constexpr wchar_t ZeroWidthNonJoiner = L'\u200C';


		static const std::wstring InternalRegexIgnoreCase;
		static const std::wstring Space;
		static const std::wstring NotSpace;
		static std::wstring Word;
		static std::wstring NotWord;

	public:
		static std::wstring SpaceClass;
		static std::wstring NotSpaceClass;
		static std::wstring WordClass;
		static std::wstring NotWordClass;
		static std::wstring DigitClass;
		static std::wstring NotDigitClass;

	private:
		static const std::wstring ECMASpaceSet;
		static const std::wstring NotECMASpaceSet;
		static const std::wstring ECMAWordSet;
		static const std::wstring NotECMAWordSet;
		static const std::wstring ECMADigitSet;
		static const std::wstring NotECMADigitSet;

	public:
		static const std::wstring ECMASpaceClass;
		static const std::wstring NotECMASpaceClass;
		static const std::wstring ECMAWordClass;
		static const std::wstring NotECMAWordClass;
		static const std::wstring ECMADigitClass;
		static const std::wstring NotECMADigitClass;

		static const std::wstring AnyClass;
		static const std::wstring EmptyClass;

	private:
		static std::unordered_map<std::wstring, std::wstring> _definedCategories;

		/*
		 *   The property table contains all the block definitions defined in the 
		 *   XML schema spec (http://www.w3.org/TR/2001/PR-xmlschema-2-20010316/#charcter-classes), Unicode 4.0 spec (www.unicode.org), 
		 *   and Perl 5.6 (see Programming Perl, 3rd edition page 167).   Three blocks defined by Perl (and here) may 
		 *   not be in the Unicode: IsHighPrivateUseSurrogates, IsHighSurrogates, and IsLowSurrogates.   
		 *   
		**/
		// Has to be sorted by the first column
		static const std::vector<std::vector<std::wstring>> _propTable;


		/**************************************************************************
		    Let U be the set of Unicode character values and let L be the lowercase
		    function, mapping from U to U. To perform case insensitive matching of
		    character sets, we need to be able to map an interval I in U, say

		        I = [chMin, chMax] = { ch : chMin <= ch <= chMax }

		    to a set A such that A contains L(I) and A is contained in the union of
		    I and L(I).

		    The table below partitions U into intervals on which L is non-decreasing.
		    Thus, for any interval J = [a, b] contained in one of these intervals,
		    L(J) is contained in [L(a), L(b)].

		    It is also true that for any such J, [L(a), L(b)] is contained in the
		    union of J and L(J). This does not follow from L being non-decreasing on
		    these intervals. It follows from the nature of the L on each interval.
		    On each interval, L has one of the following forms:

		        (1) L(ch) = constant            (LowercaseSet)
		        (2) L(ch) = ch + offset         (LowercaseAdd)
		        (3) L(ch) = ch | 1              (LowercaseBor)
		        (4) L(ch) = ch + (ch & 1)       (LowercaseBad)

		    It is easy to verify that for any of these forms [L(a), L(b)] is
		    contained in the union of [a, b] and L([a, b]).
		***************************************************************************/

		static int LowercaseSet; // Set to arg.
		static int LowercaseAdd; // Add arg.
		static int LowercaseBor; // Bitwise or with 1.
		static int LowercaseBad; // Bitwise and with 1 and add original.

		static std::vector<LowerCaseMapping> _lcTable;

		static std::vector<wchar_t> Hex;
		static std::vector<std::wstring> Categories;

	public:
		virtual ~RegexCharClass();


	private:
		static RegexCharClass::StaticConstructor staticConstructor;

	public:
		/*
		 * RegexCharClass()
		 *
		 * Creates an empty character class.
		 */
		RegexCharClass();

	private:
		RegexCharClass(bool negate, std::vector<SingleRange>& ranges, StringBuilder* categories, RegexCharClass* subtraction);

	public:
		bool getCanMerge() const;

		void setNegate(bool value);

		void AddChar(wchar_t c);

		/*
		 * AddCharClass()
		 *
		 * Adds a regex char class
		 */
		void AddCharClass(RegexCharClass* cc);

	private:
		/*
		 * AddSet()
		 *
		 * Adds a set (specified by its string represenation) to the class.
		 */
		void AddSet(const std::wstring& set);

	public:
		void AddSubtraction(RegexCharClass* sub);

		/*
		 * AddRange()
		 *
		 * Adds a single range of characters to the class.
		 */
		void AddRange(wchar_t first, wchar_t last);

		void AddCategoryFromName(const std::wstring& categoryName, bool invert, bool caseInsensitive, const std::wstring& pattern);

	private:
		void AddCategory(const std::wstring& category);

	public:
		/*
		 * AddLowerCase()
		 *
		 * Adds to the class any lowercase versions of characters already
		 * in the class. Used for case-insensitivity.
		 */
		void AddLowercase(ComCultureInfo* culture);

	private:
		/*
		 * AddLowercaseRange()
		 *
		 * For a single range that's in the set, adds any additional ranges
		 * necessary to ensure that lowercase equivalents are also included.
		 */
		void AddLowercaseRange(wchar_t chMin, wchar_t chMax, ComCultureInfo* culture);

	public:
		void AddWord(bool ecma, bool negate);

		void AddSpace(bool ecma, bool negate);

		void AddDigit(bool ecma, bool negate, const std::wstring& pattern);

		static std::wstring ConvertOldStringsToClass(const std::wstring& set, const std::wstring& category);

		/*
		 * SingletonChar()
		 *
		 * Returns the char
		 */
		static wchar_t SingletonChar(const std::wstring& set);

		static bool IsMergeable(const std::wstring& charClass);

		static bool IsEmpty(const std::wstring& charClass);

		/*
		 * IsSingleton()
		 *
		 * True if the set contains a single character only
		 */
		static bool IsSingleton(const std::wstring& set);

		static bool IsSingletonInverse(const std::wstring& set);

	private:
		static bool IsSubtraction(const std::wstring& charClass);

	public:
		static bool IsNegated(const std::wstring& set);

		static bool IsECMAWordChar(wchar_t ch);

		static bool IsWordChar(wchar_t ch);

		static bool CharInClass(wchar_t ch, const std::wstring& set);

		static bool CharInClassRecursive(wchar_t ch, const std::wstring& set, int start);

	private:
		/*
		 * CharInClass()
		 *
		 * Determines a character's membership in a character class (via the
		 * string representation of the class).
		 */
		static bool CharInClassInternal(wchar_t ch, const std::wstring& set, int start, int mySetLength, int myCategoryLength);

		static bool CharInCategory(wchar_t ch, const std::wstring& set, int start, int mySetLength, int myCategoryLength);

		/*
		*  CharInCategoryGroup
		*  This is used for categories which are composed of other categories - L, N, Z, W...
		*  These groups need special treatment when they are negated
		*/
		static bool CharInCategoryGroup(wchar_t ch, UnicodeCategory chcategory, const std::wstring& category, int& i);

		static std::wstring NegateCategory(const std::wstring& category);

	public:
		static RegexCharClass* Parse(const std::wstring& charClass);

	private:
		static RegexCharClass* ParseRecursive(const std::wstring& charClass, int start);

		/*
		 * RangeCount()
		 *
		 * The number of single ranges that have been accumulated so far.
		 */
		int RangeCount();

	public:
		/*
		 * ToString()
		 *
		 * Constructs the string representation of the class.
		 */
		std::wstring ToStringClass();

	private:
		/*
		 * GetRangeAt(int i)
		 *
		 * The ith range.
		 */

		SingleRange* GetRangeAt(int i);

		/*
		 * Canonicalize()
		 *
		 * Logic to reduce a character class to a unique, sorted form.
		 */
		void Canonicalize();

		static std::wstring SetFromProperty(const std::wstring& capname, bool invert, const std::wstring& pattern);

	public:
		/*
		* CharDescription()
		*
		* Produces a human-readable description for a single character.
		*/
		static std::wstring CharDescription(wchar_t ch);

		static std::wstring CategoryDescription(wchar_t ch);

		static std::wstring SetDescription(std::wstring set);
	};
}
