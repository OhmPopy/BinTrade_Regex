#pragma once

#include <unordered_set>

#include "Other/UtilsChar.h"
#include "Other/UtilsString.h"


namespace RegexsTest
{
	class RegexCharacterSetTests
	{
		class DerivedRunner : public RegexRunner
		{
		public:
			DerivedRunner()
			{
			}

			DerivedRunner(wstring* text)
			{
				runtext = text;
				runtextbeg = 0;
				runtextstart = 0;
				runtextend = text->length();
				runtextpos = 0;
			}

			bool IsBoundary(int index, int startpos, int endpos)
			{
				return RegexRunner::IsBoundary(index, startpos, endpos);
			}

			static bool CharInSet(wchar_t ch, wstring set, wstring category)
			{
				return RegexRunner::CharInSet(ch, set, category);
			}


		protected:
			void Go() override
			{
			}

			bool FindFirstChar() override
			{
				return false;
			}

			void InitTrackCount() override
			{
			}
		};


		struct StructSetInclusionsExpected
		{
			std::wstring set;
			RegexOptions options;
			vector<wchar_t> expectedIncluded;

			StructSetInclusionsExpected(const std::wstring& set, RegexOptions options, const vector<wchar_t>& expected_included)
				: set(set),
				  options(options),
				  expectedIncluded(expected_included)
			{
			}
		};

		struct StructNamedBlocksInclusionsExpected
		{
			std::wstring set;
			vector<int> ranges;

			StructNamedBlocksInclusionsExpected(const std::wstring& set, const vector<int>& ranges)
				: set(set),
				  ranges(ranges)
			{
			}
		};

		struct StructUnicodeCategory
		{
			wstring set;
			vector<UnicodeCategory> categories;

			StructUnicodeCategory(const wstring& set, const vector<UnicodeCategory>& categories)
				: set(set),
				  categories(categories)
			{
			}
		};

		struct StructInclusionsExpected
		{
			std::wstring generalCategory;
			UnicodeCategory unicodeCategory;

			StructInclusionsExpected(const std::wstring& general_category, UnicodeCategory unicode_category)
				: generalCategory(general_category),
				  unicodeCategory(unicode_category)
			{
			}
		};

		static bool Contains(unordered_set<wchar_t>* hash, wchar_t c)
		{
			for (auto itr = hash->begin(); itr != hash->end(); ++itr)
			{
				wchar_t elem = itr._Ptr->_Myval;
				if (c == elem)return true;
			}
			return false;
		}

		static int getIndex(vector<UnicodeCategory> v, UnicodeCategory K)
		{
			auto it = find(v.begin(), v.end(), K);

			// If element was found
			if (it != v.end())
			{
				// calculating the index
				// of K
				int index = it - v.begin();
				return index;
			}
			else
			{
				// If the element is not
				// present in the vector
				return -1;
			}
		}

	public:
		static void Run(bool test_throw)
		{
			Assert::WriteLine(L"RegexCharacterSetTests");

			auto value_1 = SetInclusionsExpected_MemberData();
			for (size_t i = 0; i < value_1.size(); ++i)
			{
				auto item = value_1[i];
				SetInclusionsExpected(item.set, item.options, item.expectedIncluded);
			}

			auto value_2 = SetExclusionsExpected_MemberData();
			SetExclusionsExpected(value_2.set, value_2.options, value_2.expectedIncluded);


			auto value_3 = SingleExpected_MemberData();
			for (size_t i = 0; i < value_3.size(); ++i)
			{
				auto item = value_3[i];
				SingleExpected(item);
			}

			AllEmptySets();
			AllButOneSets();
			DotInclusionsExpected();
			WhitespaceInclusionsExpected();
			DigitInclusionsExpected();

			auto value_4 = UnicodeCategoryInclusionsExpected_MemberData();
			for (size_t i = 0; i < value_4.size(); ++i)
			{
				auto item = value_4[i];
				UnicodeCategoryInclusionsExpected(item.set, item.categories);
			}

			auto value_5 = NamedBlocksInclusionsExpected_MemberData();
			for (size_t i = 0; i < value_5.size(); ++i)
			{
				auto item = value_5[i];
				NamedBlocksInclusionsExpected(item.set, item.ranges);
			}

			auto value_6 = UnicodeCategoriesInclusionsExpected_MemberData();
			for (size_t i = 0; i < value_6.size(); ++i)
			{
				auto item = value_6[i];
				UnicodeCategoriesInclusionsExpected(item.generalCategory, item.unicodeCategory);
			}
			LetterOrDigitsInclusionsExpected();


			IsBoundary_ReturnsExpectedResult(L"ab", 1, false);
			IsBoundary_ReturnsExpectedResult(L"a b", 1, true);
			IsBoundary_ReturnsExpectedResult(L"a b", 2, true);
			IsBoundary_ReturnsExpectedResult(L"\u200Da", 1, false);
			IsBoundary_ReturnsExpectedResult(L"\u200D\u200C", 1, false);
			IsBoundary_ReturnsExpectedResult(L"\u200Ca", 1, false);
			IsBoundary_ReturnsExpectedResult(L"\u200C a", 1, true);

			if (test_throw)ValidateValidateSet();
			RegexRunner_Legacy_CharInSet();
		}

		static vector<StructSetInclusionsExpected> SetInclusionsExpected_MemberData()
		{
			return
			{
				StructSetInclusionsExpected(L"a", RegexOptions::IgnoreCase, {'a', 'A'}),
				StructSetInclusionsExpected(L"ac", RegexOptions::None, {'a', 'c'}),
				StructSetInclusionsExpected(L"ace", RegexOptions::None, {'a', 'c', 'e'}),
				StructSetInclusionsExpected(L"aceg", RegexOptions::None, {'a', 'c', 'e', 'g'}),
				StructSetInclusionsExpected(L"aceg", RegexOptions::IgnoreCase, {'a', 'A', 'c', 'C', 'e', 'E', 'g', 'G'}),
				StructSetInclusionsExpected(L"\\u00A9", RegexOptions::None, {(wchar_t)0x00a9}),
				StructSetInclusionsExpected(L"\\u00A9", RegexOptions::IgnoreCase, {(wchar_t)0x00a9}),
				StructSetInclusionsExpected(L"\\u00FD\\u00FF", RegexOptions::None, {(wchar_t)0x00fd, (wchar_t)0x00ff}),
				StructSetInclusionsExpected(L"\\u00FE\\u0080", RegexOptions::None, {(wchar_t)0x00fe, (wchar_t)0x0080}),
				StructSetInclusionsExpected(L"\\u0080\\u0082", RegexOptions::None, {(wchar_t)0x0080, (wchar_t)0x0082}),
				StructSetInclusionsExpected(L"az", (RegexOptions)((int)RegexOptions::IgnoreCase | (int)RegexOptions::CultureInvariant), {'a', 'A', 'z', 'Z'}),
				StructSetInclusionsExpected(L"azY", RegexOptions::IgnoreCase, {'a', 'A', 'z', 'Z', 'y', 'Y'}),
				StructSetInclusionsExpected(L"azY", (RegexOptions)((int)RegexOptions::IgnoreCase | (int)RegexOptions::CultureInvariant), {'a', 'A', 'z', 'Z', 'y', 'Y'}),
				StructSetInclusionsExpected(L"azY\\u00A9", RegexOptions::IgnoreCase, {'a', 'A', 'z', 'Z', 'y', 'Y', (wchar_t)0x00a9}),
				StructSetInclusionsExpected(L"azY\\u00A9", (RegexOptions)((int)RegexOptions::IgnoreCase | (int)RegexOptions::CultureInvariant), {'a', 'A', 'z', 'Z', 'y', 'Y', (wchar_t)0x00a9}),
				StructSetInclusionsExpected(L"azY\\u00A9\\u05D0", RegexOptions::IgnoreCase, {'a', 'A', 'z', 'Z', 'y', 'Y', (wchar_t)0x00a9, (wchar_t)0x05d0}),
				StructSetInclusionsExpected(L"azY\\u00A9\\u05D0", (RegexOptions)((int)RegexOptions::IgnoreCase | (int)RegexOptions::CultureInvariant), {'a', 'A', 'z', 'Z', 'y', 'Y', (wchar_t)0x00a9, (wchar_t)0x05d0}),
				StructSetInclusionsExpected(L"a ", RegexOptions::None, {'a', ' '}),
				StructSetInclusionsExpected(L"a \\t\\r", RegexOptions::None, {'a', ' ', '\t', '\r'}),
				StructSetInclusionsExpected(L"aeiou", RegexOptions::None, {'a', 'e', 'i', 'o', 'u'}),
				StructSetInclusionsExpected(L"a-a", RegexOptions::None, {'a'}),
				StructSetInclusionsExpected(L"ab", RegexOptions::None, {'a', 'b'}),
				StructSetInclusionsExpected(L"a-b", RegexOptions::None, {'a', 'b'}),
				StructSetInclusionsExpected(L"abc", RegexOptions::None, {'a', 'b', 'c'}),
				StructSetInclusionsExpected(L"1369", RegexOptions::None, {'1', '3', '6', '9'}),
				StructSetInclusionsExpected(L"ACEGIKMOQSUWY", RegexOptions::None, {'A', 'C', 'E', 'G', 'I', 'K', 'M', 'O', 'Q', 'S', 'U', 'W', 'Y'}),
				StructSetInclusionsExpected(L"abcAB", RegexOptions::None, {'A', 'B', 'a', 'b', 'c'}),
				StructSetInclusionsExpected(L"a-c", RegexOptions::None, {'a', 'b', 'c'}),
				StructSetInclusionsExpected(L"a-fA-F", RegexOptions::None, {'a', 'b', 'c', 'd', 'e', 'f', 'A', 'B', 'C', 'D', 'E', 'F'}),
				StructSetInclusionsExpected(L"a-fA-F0-9", RegexOptions::None, {'a', 'b', 'c', 'd', 'e', 'f', 'A', 'B', 'C', 'D', 'E', 'F', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'}),
				StructSetInclusionsExpected(L"X-b", RegexOptions::None, {'X', 'Y', 'Z', '[', '\\', ']', '^', '_', '`', 'a', 'b'}),
				StructSetInclusionsExpected(L"\\u0083\\u00DE-\\u00E1", RegexOptions::None, {(wchar_t)0x0083, (wchar_t)0x00DE, (wchar_t)0x00DF, (wchar_t)0x00E0, (wchar_t)0x00E1}),
				StructSetInclusionsExpected(L"\\u007A-\\u0083\\u00DE-\\u00E1", RegexOptions::None, {(wchar_t)0x007A, (wchar_t)0x007B, (wchar_t)0x007C, (wchar_t)0x007D, (wchar_t)0x007E, (wchar_t)0x007F, (wchar_t)0x0080, (wchar_t)0x0081, (wchar_t)0x0082, (wchar_t)0x0083, (wchar_t)0x00DE, (wchar_t)0x00DF, (wchar_t)0x00E0, (wchar_t)0x00E1}),
				StructSetInclusionsExpected(L"\\u05D0", RegexOptions::None, {(wchar_t)0x05D0}),
				StructSetInclusionsExpected(L"a\\u05D0", RegexOptions::None, {'a', (wchar_t)0x05D0}),
				StructSetInclusionsExpected(L"\\uFFFC-\\uFFFF", RegexOptions::None, {(wchar_t)0xFFFC, (wchar_t)0xFFFD, (wchar_t)0xFFFE, (wchar_t)0xFFFF}),
				StructSetInclusionsExpected(L"[a-z-[d-w-[m-o]]]", RegexOptions::None, {'a', 'b', 'c', 'm', 'n', 'n', 'o', 'x', 'y', 'z'}),
				StructSetInclusionsExpected(L"\\p{IsBasicLatin}-[\\x00-\\x7F]", RegexOptions::None, {}),
				StructSetInclusionsExpected(L"[0-9-[2468]]", RegexOptions::None, {'0', '1', '3', '5', '7', '9'}),
				StructSetInclusionsExpected(L"[\\u1000-\\u1001\\u3000-\\u3002\\u5000-\\u5003]", RegexOptions::None, {(wchar_t)0x1000, (wchar_t)0x1001, (wchar_t)0x3000, (wchar_t)0x3001, (wchar_t)0x3002, (wchar_t)0x5000, (wchar_t)0x5001, (wchar_t)0x5002, (wchar_t)0x5003}),
			};
		}

		static void SetInclusionsExpected(wstring set, RegexOptions options, vector<wchar_t> expectedIncluded)
		{
			bool hasBracket = std::wstring(set).find(L"[") != std::string::npos;
			if (hasBracket)
			{
				unordered_set<wchar_t> temp = unordered_set<wchar_t>(expectedIncluded.begin(), expectedIncluded.end());
				ValidateSetAsync(set, options, &temp, NULL, true);
			}
			else
			{
				unordered_set<wchar_t> temp = unordered_set<wchar_t>(expectedIncluded.begin(), expectedIncluded.end());
				ValidateSetAsync(L"[" + set + L"]", options, &temp, NULL);
				ValidateSetAsync(L"[^" + set + L"]", options, NULL, &temp);
			}
		}

		static StructSetInclusionsExpected SetExclusionsExpected_MemberData()
		{
			return StructSetInclusionsExpected(L"[^1234-[3456]]", RegexOptions::None, {'1', '2', '3', '4', '5', '6'});
		}

		static void SetExclusionsExpected(wstring set, RegexOptions options, vector<wchar_t> expectedIncluded)
		{
			unordered_set<wchar_t> temp = unordered_set<wchar_t>(expectedIncluded.begin(), expectedIncluded.end());
			ValidateSetAsync(set, options, NULL, &temp, true);
		}

		static vector<wchar_t> SingleExpected_MemberData()
		{
			return
			{
				'\0',
				(wchar_t)0x00FF,
				'a',
				'5',
				(wchar_t)0x00FF,
				(wchar_t)0x0080,
				(wchar_t)0x0100,
			};
		}

		static void SingleExpected(wchar_t c)
		{
			wstring s = wstring(1, c);
			unordered_set<wchar_t>* set = new unordered_set<wchar_t>{c};
			unordered_set<wchar_t>* set2 = new unordered_set<wchar_t>();

			// One
			ValidateSetAsync(L"" + s + L"", RegexOptions::None, set, NULL);
			ValidateSetAsync(L"[{" + s + L"]", RegexOptions::None, set, NULL);
			ValidateSetAsync(L"[^" + s + L"]", RegexOptions::None, NULL, set);

			// Positive lookahead
			ValidateSetAsync(L"(?=" + s + L")" + s + L"", RegexOptions::None, set, NULL);
			ValidateSetAsync(L"(?=[^" + s + L"])[^" + s + L"]", RegexOptions::None, NULL, set);

			// Negative lookahead
			ValidateSetAsync(L"(?![^" + s + L"])" + s + L"", RegexOptions::None, set, NULL);
			ValidateSetAsync(L"(?![" + s + L"])[^" + s + L"]", RegexOptions::None, NULL, set);

			// Concatenation
			ValidateSetAsync(L"[" + s + L"" + s + L"]", RegexOptions::None, set, NULL);
			ValidateSetAsync(L"[^" + s + L"" + s + L"" + s + L"]", RegexOptions::None, NULL, set);

			// Alternation
			ValidateSetAsync(L"" + s + L"|" + s + L"", RegexOptions::None, set, NULL);
			ValidateSetAsync(L"[^" + s + L"]|[^" + s + L"]|[^" + s + L"]", RegexOptions::None, NULL, set);
			ValidateSetAsync(L"" + s + L"|[^" + s + L"]", RegexOptions::None, NULL, set2);

			delete set;
			delete set2;
		}

		static void AllEmptySets()
		{
			unordered_set<wchar_t> set = unordered_set<wchar_t>();

			ValidateSetAsync(L"[\\u0000-\\uFFFF]", RegexOptions::None, NULL, &set);
			ValidateSetAsync(L"[\\u0000-\\uFFFFa-z]", RegexOptions::None, NULL, &set);
			ValidateSetAsync(L"[\\u0000-\\u1000\\u1001-\\u2002\\u2003-\\uFFFF]", RegexOptions::None, NULL, &set);
			ValidateSetAsync(L"[\\u0000-\\uFFFE\\u0001-\\uFFFF]", RegexOptions::None, NULL, &set, true);

			ValidateSetAsync(L"[^\\u0000-\\uFFFF]", RegexOptions::None, &set, NULL);
			ValidateSetAsync(L"[^\\u0000-\\uFFFFa-z]", RegexOptions::None, &set, NULL);
			ValidateSetAsync(L"[^\\u0000-\\uFFFE\\u0001-\\uFFFF]", RegexOptions::None, &set, NULL);
			ValidateSetAsync(L"[^\\u0000-\\u1000\\u1001-\\u2002\\u2003-\\uFFFF]", RegexOptions::None, &set, NULL, true);
		}

		static void AllButOneSets()
		{
			unordered_set<wchar_t> tempo_1 = unordered_set<wchar_t>{(wchar_t)0xFFFF};
			unordered_set<wchar_t> tempo_2 = unordered_set<wchar_t>{(wchar_t)0x0000};
			unordered_set<wchar_t> tempo_3 = unordered_set<wchar_t>{'b'};
			ValidateSetAsync(L"[\\u0000-\\uFFFE]", RegexOptions::None, NULL, &tempo_1);
			ValidateSetAsync(L"[\\u0001-\\uFFFF]", RegexOptions::None, NULL, &tempo_2);
			ValidateSetAsync(L"[\\u0000-ac-\\uFFFF]", RegexOptions::None, NULL, &tempo_3, true);
		}

		static void DotInclusionsExpected()
		{
			unordered_set<wchar_t> tempo_1 = unordered_set<wchar_t>{'\n'};
			unordered_set<wchar_t> tempo_2 = unordered_set<wchar_t>{'\n'};
			unordered_set<wchar_t> tempo_3 = unordered_set<wchar_t>{'\n'};
			unordered_set<wchar_t> tempo_4 = unordered_set<wchar_t>();

			ValidateSetAsync(L".", RegexOptions::None, NULL, &tempo_1);
			ValidateSetAsync(L".", RegexOptions::IgnoreCase, NULL, &tempo_2);
			ValidateSetAsync(L".", (RegexOptions)((int)RegexOptions::IgnoreCase | (int)RegexOptions::CultureInvariant), NULL, &tempo_3, true);

			ValidateSetAsync(L".", RegexOptions::Singleline, NULL, &tempo_4);
			ValidateSetAsync(L".", (RegexOptions)((int)RegexOptions::Singleline | (int)RegexOptions::IgnoreCase), NULL, &tempo_4);
			ValidateSetAsync(L".", (RegexOptions)((int)RegexOptions::Singleline | (int)RegexOptions::IgnoreCase | (int)RegexOptions::CultureInvariant), NULL, &tempo_4, true);
		}

		typedef std::function<bool(wchar_t value)> IncludedEvaluator;

		typedef std::function<bool(wchar_t value, vector<UnicodeCategory> categories)> IncludedEvaluator2;

		typedef std::function<bool(wchar_t value, int i, vector<int> ranges)> IncludedEvaluator3;

		static bool IncludedEvaluator_IsWhiteSpace(wchar_t value)
		{
			return UtilsChar::IsWhiteSpace(value);
		}

		static bool IncludedEvaluator_IsDigit(wchar_t value)
		{
			return UtilsChar::IsDigit(value);
		}

		static bool IncludedEvaluator_Delegate_1(wchar_t value, vector<UnicodeCategory> categories)
		{
			auto cata = CharUnicodeInfo::GetUnicodeCategory(value);
			int index = getIndex(categories, cata);
			return index >= 0;
		}

		static bool IncludedEvaluator_Delegate_2(wchar_t c, int i, vector<int> ranges)
		{
			return c >= ranges[i] && c <= ranges[i + 1];
		}

		static void WhitespaceInclusionsExpected()
		{
			unordered_set<wchar_t> whitespaceInclusions = ComputeIncludedSet(IncludedEvaluator_IsWhiteSpace);
			ValidateSetAsync(L"[\\s]", RegexOptions::None, &whitespaceInclusions, NULL);
			ValidateSetAsync(L"[^\\s]", RegexOptions::None, NULL, &whitespaceInclusions);
			ValidateSetAsync(L"[\\S]", RegexOptions::None, NULL, &whitespaceInclusions);
		}

		static void DigitInclusionsExpected()
		{
			unordered_set<wchar_t> digitInclusions = ComputeIncludedSet(IncludedEvaluator_IsDigit);
			ValidateSetAsync(L"[\\d]", RegexOptions::None, &digitInclusions, NULL);
			ValidateSetAsync(L"[^\\d]", RegexOptions::None, NULL, &digitInclusions);
			ValidateSetAsync(L"[\\D]", RegexOptions::None, NULL, &digitInclusions);
		}

		static vector<StructUnicodeCategory> UnicodeCategoryInclusionsExpected_MemberData()
		{
			return
			{
				StructUnicodeCategory(L"\\p{Lu}", {UnicodeCategory::UppercaseLetter}),
				StructUnicodeCategory(L"\\p{S}", {UnicodeCategory::CurrencySymbol, UnicodeCategory::MathSymbol, UnicodeCategory::ModifierSymbol, UnicodeCategory::OtherSymbol}),
				StructUnicodeCategory(L"\\p{Lu}\\p{Zl}", {UnicodeCategory::UppercaseLetter, UnicodeCategory::LineSeparator}),
				StructUnicodeCategory(L"\\w", {UnicodeCategory::LowercaseLetter, UnicodeCategory::UppercaseLetter, UnicodeCategory::TitlecaseLetter, UnicodeCategory::OtherLetter, UnicodeCategory::ModifierLetter, UnicodeCategory::NonSpacingMark, UnicodeCategory::DecimalDigitNumber, UnicodeCategory::ConnectorPunctuation}),
			};
		}

		static void UnicodeCategoryInclusionsExpected(wstring set, vector<UnicodeCategory> categories)
		{
			unordered_set<wchar_t> categoryInclusions;
			ComputeIncludedSet(IncludedEvaluator_Delegate_1, categoryInclusions, categories);
			ValidateSetAsync(L"[" + set + L"]", RegexOptions::None, &categoryInclusions, NULL);
			ValidateSetAsync(L"[^" + set + L"]", RegexOptions::None, NULL, &categoryInclusions);
		}

		static vector<StructNamedBlocksInclusionsExpected> NamedBlocksInclusionsExpected_MemberData()
		{
			return
			{
				StructNamedBlocksInclusionsExpected(L"\\p{IsBasicLatin}", {0x0000, 0x007F}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsLatin-1Supplement}", {0x0080, 0x00FF}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsLatinExtended-A}", {0x0100, 0x017F}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsLatinExtended-B}", {0x0180, 0x024F}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsIPAExtensions}", {0x0250, 0x02AF}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsSpacingModifierLetters}", {0x02B0, 0x02FF}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsCombiningDiacriticalMarks}", {0x0300, 0x036F}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsGreek}", {0x0370, 0x03FF}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsCyrillic}", {0x0400, 0x04FF}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsCyrillicSupplement}", {0x0500, 0x052F}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsArmenian}", {0x0530, 0x058F}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsHebrew}", {0x0590, 0x05FF}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsArabic}", {0x0600, 0x06FF}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsSyriac}", {0x0700, 0x074F}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsThaana}", {0x0780, 0x07BF}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsDevanagari}", {0x0900, 0x097F}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsBengali}", {0x0980, 0x09FF}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsGurmukhi}", {0x0A00, 0x0A7F}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsGujarati}", {0x0A80, 0x0AFF}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsOriya}", {0x0B00, 0x0B7F}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsTamil}", {0x0B80, 0x0BFF}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsTelugu}", {0x0C00, 0x0C7F}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsKannada}", {0x0C80, 0x0CFF}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsMalayalam}", {0x0D00, 0x0D7F}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsSinhala}", {0x0D80, 0x0DFF}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsThai}", {0x0E00, 0x0E7F}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsLao}", {0x0E80, 0x0EFF}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsTibetan}", {0x0F00, 0x0FFF}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsMyanmar}", {0x1000, 0x109F}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsGeorgian}", {0x10A0, 0x10FF}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsHangulJamo}", {0x1100, 0x11FF}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsEthiopic}", {0x1200, 0x137F}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsCherokee}", {0x13A0, 0x13FF}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsUnifiedCanadianAboriginalSyllabics}", {0x1400, 0x167F}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsOgham}", {0x1680, 0x169F}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsRunic}", {0x16A0, 0x16FF}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsTagalog}", {0x1700, 0x171F}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsHanunoo}", {0x1720, 0x173F}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsBuhid}", {0x1740, 0x175F}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsTagbanwa}", {0x1760, 0x177F}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsKhmer}", {0x1780, 0x17FF}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsMongolian}", {0x1800, 0x18AF}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsLimbu}", {0x1900, 0x194F}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsTaiLe}", {0x1950, 0x197F}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsKhmerSymbols}", {0x19E0, 0x19FF}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsPhoneticExtensions}", {0x1D00, 0x1D7F}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsLatinExtendedAdditional}", {0x1E00, 0x1EFF}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsGreekExtended}", {0x1F00, 0x1FFF}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsGeneralPunctuation}", {0x2000, 0x206F}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsSuperscriptsandSubscripts}", {0x2070, 0x209F}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsCurrencySymbols}", {0x20A0, 0x20CF}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsCombiningDiacriticalMarksforSymbols}", {0x20D0, 0x20FF}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsLetterlikeSymbols}", {0x2100, 0x214F}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsNumberForms}", {0x2150, 0x218F}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsArrows}", {0x2190, 0x21FF}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsMathematicalOperators}", {0x2200, 0x22FF}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsMiscellaneousTechnical}", {0x2300, 0x23FF}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsControlPictures}", {0x2400, 0x243F}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsOpticalCharacterRecognition}", {0x2440, 0x245F}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsEnclosedAlphanumerics}", {0x2460, 0x24FF}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsBoxDrawing}", {0x2500, 0x257F}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsBlockElements}", {0x2580, 0x259F}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsGeometricShapes}", {0x25A0, 0x25FF}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsMiscellaneousSymbols}", {0x2600, 0x26FF}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsDingbats}", {0x2700, 0x27BF}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsMiscellaneousMathematicalSymbols-A}", {0x27C0, 0x27EF}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsSupplementalArrows-A}", {0x27F0, 0x27FF}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsBraillePatterns}", {0x2800, 0x28FF}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsSupplementalArrows-B}", {0x2900, 0x297F}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsMiscellaneousMathematicalSymbols-B}", {0x2980, 0x29FF}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsSupplementalMathematicalOperators}", {0x2A00, 0x2AFF}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsMiscellaneousSymbolsandArrows}", {0x2B00, 0x2BFF}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsCJKRadicalsSupplement}", {0x2E80, 0x2EFF}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsKangxiRadicals}", {0x2F00, 0x2FDF}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsIdeographicDescriptionCharacters}", {0x2FF0, 0x2FFF}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsCJKSymbolsandPunctuation}", {0x3000, 0x303F}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsHiragana}", {0x3040, 0x309F}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsKatakana}", {0x30A0, 0x30FF}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsBopomofo}", {0x3100, 0x312F}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsHangulCompatibilityJamo}", {0x3130, 0x318F}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsKanbun}", {0x3190, 0x319F}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsBopomofoExtended}", {0x31A0, 0x31BF}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsKatakanaPhoneticExtensions}", {0x31F0, 0x31FF}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsEnclosedCJKLettersandMonths}", {0x3200, 0x32FF}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsCJKCompatibility}", {0x3300, 0x33FF}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsCJKUnifiedIdeographsExtensionA}", {0x3400, 0x4DBF}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsYijingHexagramSymbols}", {0x4DC0, 0x4DFF}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsCJKUnifiedIdeographs}", {0x4E00, 0x9FFF}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsYiSyllables}", {0xA000, 0xA48F}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsYiRadicals}", {0xA490, 0xA4CF}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsHangulSyllables}", {0xAC00, 0xD7AF}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsHighSurrogates}", {0xD800, 0xDB7F}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsHighPrivateUseSurrogates}", {0xDB80, 0xDBFF}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsLowSurrogates}", {0xDC00, 0xDFFF}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsPrivateUse}", {0xE000, 0xF8FF}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsCJKCompatibilityIdeographs}", {0xF900, 0xFAFF}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsAlphabeticPresentationForms}", {0xFB00, 0xFB4F}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsArabicPresentationForms-A}", {0xFB50, 0xFDFF}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsVariationSelectors}", {0xFE00, 0xFE0F}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsCombiningHalfMarks}", {0xFE20, 0xFE2F}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsCJKCompatibilityForms}", {0xFE30, 0xFE4F}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsSmallFormVariants}", {0xFE50, 0xFE6F}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsArabicPresentationForms-B}", {0xFE70, 0xFEFF}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsHalfwidthandFullwidthForms}", {0xFF00, 0xFFEF}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsSpecials}", {0xFFF0, 0xFFFF}),
				StructNamedBlocksInclusionsExpected(L"\\p{IsRunic}\\p{IsHebrew}", {0x0590, 0x05FF, 0x16A0, 0x16FF}),
				StructNamedBlocksInclusionsExpected(L"abx-z\\p{IsRunic}\\p{IsHebrew}", {0x0590, 0x05FF, 0x16A0, 0x16FF, 'a', 'a', 'b', 'b', 'x', 'x', 'y', 'z'}),
			};
		}

		static void NamedBlocksInclusionsExpected(wstring set, vector<int> ranges)
		{
			unordered_set<wchar_t> included = unordered_set<wchar_t>();
			for (size_t i = 0; i < ranges.size() - 1; i += 2)
			{
				ComputeIncludedSet(IncludedEvaluator_Delegate_2, included, i, ranges);
			}

			ValidateSetAsync(L"[" + set + L"]", RegexOptions::None, &included, NULL);
			ValidateSetAsync(L"[^" + set + L"]", RegexOptions::None, NULL, &included);
		}

		static vector<StructInclusionsExpected> UnicodeCategoriesInclusionsExpected_MemberData()
		{
			return
			{
				StructInclusionsExpected(L"Cc", UnicodeCategory::Control),
				StructInclusionsExpected(L"Cf", UnicodeCategory::Format),
				StructInclusionsExpected(L"Cn", UnicodeCategory::OtherNotAssigned),
				StructInclusionsExpected(L"Co", UnicodeCategory::PrivateUse),
				StructInclusionsExpected(L"Cs", UnicodeCategory::Surrogate),
				StructInclusionsExpected(L"Ll", UnicodeCategory::LowercaseLetter),
				StructInclusionsExpected(L"Lm", UnicodeCategory::ModifierLetter),
				StructInclusionsExpected(L"Lo", UnicodeCategory::OtherLetter),
				StructInclusionsExpected(L"Lt", UnicodeCategory::TitlecaseLetter),
				StructInclusionsExpected(L"Lu", UnicodeCategory::UppercaseLetter),
				StructInclusionsExpected(L"Mc", UnicodeCategory::SpacingCombiningMark),
				StructInclusionsExpected(L"Me", UnicodeCategory::EnclosingMark),
				StructInclusionsExpected(L"Mn", UnicodeCategory::NonSpacingMark),
				StructInclusionsExpected(L"Nd", UnicodeCategory::DecimalDigitNumber),
				StructInclusionsExpected(L"Nl", UnicodeCategory::LetterNumber),
				StructInclusionsExpected(L"No", UnicodeCategory::OtherNumber),
				StructInclusionsExpected(L"Pc", UnicodeCategory::ConnectorPunctuation),
				StructInclusionsExpected(L"Pd", UnicodeCategory::DashPunctuation),
				StructInclusionsExpected(L"Pe", UnicodeCategory::ClosePunctuation),
				StructInclusionsExpected(L"Po", UnicodeCategory::OtherPunctuation),
				StructInclusionsExpected(L"Ps", UnicodeCategory::OpenPunctuation),
				StructInclusionsExpected(L"Pf", UnicodeCategory::FinalQuotePunctuation),
				StructInclusionsExpected(L"Pi", UnicodeCategory::InitialQuotePunctuation),
				StructInclusionsExpected(L"Sc", UnicodeCategory::CurrencySymbol),
				StructInclusionsExpected(L"Sk", UnicodeCategory::ModifierSymbol),
				StructInclusionsExpected(L"Sm", UnicodeCategory::MathSymbol),
				StructInclusionsExpected(L"So", UnicodeCategory::OtherSymbol),
				StructInclusionsExpected(L"Zl", UnicodeCategory::LineSeparator),
				StructInclusionsExpected(L"Zp", UnicodeCategory::ParagraphSeparator),
				StructInclusionsExpected(L"Zs", UnicodeCategory::SpaceSeparator),


			};
		}

		static void UnicodeCategoriesInclusionsExpected(wstring generalCategory, UnicodeCategory unicodeCategory)
		{
			wstring text_1 = L"\\p{" + generalCategory + L"}";
			wstring text_2 = L"\\P{" + generalCategory + L"}";
			ValidateUnicodeCategoryInclusionExclusion(text_1, text_2, {unicodeCategory});
		}

		static void LetterOrDigitsInclusionsExpected()
		{
			ValidateUnicodeCategoryInclusionExclusion(L"[\\p{L}\\d]", L"[^\\p{L}\\d]",
			                                          {
				                                          UnicodeCategory::UppercaseLetter,
				                                          UnicodeCategory::LowercaseLetter,
				                                          UnicodeCategory::TitlecaseLetter,
				                                          UnicodeCategory::ModifierLetter,
				                                          UnicodeCategory::OtherLetter,
				                                          UnicodeCategory::DecimalDigitNumber
			                                          });
		}

		static void ValidateUnicodeCategoryInclusionExclusion(wstring inclusionPattern, wstring exclusionPattern, vector<UnicodeCategory> unicodeCategory)
		{
			int expectedInCategory = 0;
			vector<wchar_t> allChars;
			wstring total;
			for (int i = 0; i < 0xFFFF; ++i)
			{
				allChars.push_back((wchar_t)i);
				total += wstring(1, (wchar_t)i);

				if (IncludedEvaluator_Delegate_1((wchar_t)i, unicodeCategory))
					expectedInCategory++;
			}
			int expectedNotInCategory = allChars.size() - expectedInCategory;

			Regex r_1 = Regex(inclusionPattern);
			auto temp_1 = r_1.Matches(&total);
			Assert::True(expectedInCategory == temp_1->Count);

			Regex r_2 = Regex(exclusionPattern);
			auto temp_2 = r_2.Matches(&total);
			Assert::True(expectedNotInCategory == temp_2->Count);
		}

		static void IsBoundary_ReturnsExpectedResult(wstring text, int pos, bool expectedBoundary)
		{
			DerivedRunner r = DerivedRunner(&text);
			Assert::True(expectedBoundary == r.IsBoundary(pos, 0, text.length()));
		}

		static unordered_set<wchar_t> ComputeIncludedSet(IncludedEvaluator func)
		{
			unordered_set<wchar_t> included = unordered_set<wchar_t>();
			ComputeIncludedSet(func, included);
			return included;
		}

		static void ComputeIncludedSet(IncludedEvaluator func, unordered_set<wchar_t> included)
		{
			for (int i = 0; i <= 0xFFFF; i++)
			{
				if (func((wchar_t)i))
				{
					included.insert((wchar_t)i);
				}
			}
		}

		static void ComputeIncludedSet(IncludedEvaluator2 func, unordered_set<wchar_t> included, vector<UnicodeCategory> categories)
		{
			for (int i = 0; i <= 0xFFFF; i++)
			{
				if (func((wchar_t)i, categories))
				{
					included.insert((wchar_t)i);
				}
			}
		}

		static void ComputeIncludedSet(IncludedEvaluator3 func, unordered_set<wchar_t> included, int index, vector<int> ranges)
		{
			for (int i = 0; i <= 0xFFFF; i++)
			{
				if (func((wchar_t)i, index, ranges))
				{
					included.insert((wchar_t)i);
				}
			}
		}

		static void ValidateValidateSet()
		{
			unordered_set<wchar_t> tempo = unordered_set<wchar_t>{'b'};
			AssertThrows(InvalidOperationException, ValidateSetAsync( L"[a]", RegexOptions::None,&tempo, NULL))
			AssertThrows(InvalidOperationException, ValidateSetAsync(L"[a]", RegexOptions::None, &tempo, NULL, true))
			AssertThrows(InvalidOperationException, ValidateSetAsync(L"[b]", RegexOptions::None, NULL, &tempo))
			AssertThrows(InvalidOperationException, ValidateSetAsync(L"[b]", RegexOptions::None, NULL, &tempo, true))
		}

		static void RegexRunner_Legacy_CharInSet()
		{
			Assert::True(DerivedRunner::CharInSet('a', L"ab", L""));
			Assert::False(DerivedRunner::CharInSet('x', L"ab", L""));

			Assert::True(DerivedRunner::CharInSet('x', UtilsString::StringInitSpe(0x00) + UtilsString::StringInitSpe(0x00) + L"ab", L""));
			Assert::False(DerivedRunner::CharInSet('a', UtilsString::StringInitSpe(0x00) + UtilsString::StringInitSpe(0x00) + L"ab", L""));

			Assert::True(DerivedRunner::CharInSet('4', L"", L"\x0009"));
			Assert::False(DerivedRunner::CharInSet('a', L"", L"\x0009"));

			Assert::True(DerivedRunner::CharInSet('4', L"xz", L"\x0009"));
			Assert::True(DerivedRunner::CharInSet('a', L"az", L"\x0009"));
			Assert::False(DerivedRunner::CharInSet('a', L"xz", L"\x0009"));
		}

		static void Fail(int c)
		{
			throw InvalidOperationException("Set=\"{regex}\", Options=\"{options}\", {c:X4} => '{(char)c}'");
		}

		static void ValidateSetAsync(wstring regex, RegexOptions options, unordered_set<wchar_t>* included, unordered_set<wchar_t>* excluded, bool validateEveryChar = false)
		{
			Assert::True((included != NULL) ^ (excluded != NULL));

			Regex r = Regex(regex, options);

			if (validateEveryChar)
			{
				for (int i = 0; i <= 0xFFFF; i++)
				{
					wstring temos = wstring(1, (wchar_t)i);
					bool actual = r.IsMatch(temos);
					bool expected = included != NULL ? Contains(included, (wchar_t)i) : !Contains(excluded, (wchar_t)i);
					if (actual != expected)
					{
						Fail(i);
					}
				}
			}
			else if (included != NULL)
			{
				for (auto itr = included->begin(); itr != included->end(); ++itr)
				{
					wchar_t c = itr._Ptr->_Myval;
					if (!r.IsMatch(wstring(1, (wchar_t)c)))
					{
						Fail(c);
					}
				}
			}
			else
			{
				for (auto itr = excluded->begin(); itr != excluded->end(); ++itr)
				{
					wchar_t c = itr._Ptr->_Myval;
					if (r.IsMatch(wstring(1, (wchar_t)c)))
					{
						Fail(c);
					}
				}
			}
		}
	};
}
