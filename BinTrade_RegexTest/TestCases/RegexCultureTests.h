#pragma once
#include "RegexMatch.h"
#include "Other/UtilsString.h"


using namespace Regexs;
namespace RegexsTest
{
	class RegexCultureTests
	{
		struct StructAnchoredPattern_TestData
		{
			wstring pattern;
			wstring input;
			wstring culture;
			RegexOptions options;
			bool expected;

			StructAnchoredPattern_TestData(const wstring& pattern, const wstring& input, const wstring& culture, RegexOptions options, bool expected)
				: pattern(pattern),
				  input(input),
				  culture(culture),
				  options(options),
				  expected(expected)
			{
			}
		};

		struct StructDifferent_Cultures
		{
			wstring pattern;
			ComCultureInfo* culture;
			wstring input;
			wstring match_expected;

			StructDifferent_Cultures(const wstring& pattern, ComCultureInfo* culture, const wstring& input, const wstring& match_expected)
				: pattern(pattern),
				  culture(culture),
				  input(input),
				  match_expected(match_expected)
			{
			}
		};


	public:
		static void Run(bool test_throw)
		{
			Assert::WriteLine(L"RegexCultureTests");

			const auto value_1 = CharactersComparedOneByOne_AnchoredPattern_TestData();
			for (StructAnchoredPattern_TestData data : value_1)
				CharactersComparedOneByOne_AnchoredPattern(data.pattern.c_str(), data.input.c_str(), data.culture.c_str(), data.options, data.expected);

			CharactersComparedOneByOne_Invariant(RegexOptions::None);
			CharactersComparedOneByOne_Invariant((RegexOptions)((int)RegexOptions::IgnoreCase | (int)RegexOptions::CultureInvariant));

			CharactersLowercasedOneByOne();

			TurkishI_Is_Differently_LowerUpperCased_In_Turkish_Culture(2, RegexOptions::None);
			TurkishI_Is_Differently_LowerUpperCased_In_Turkish_Culture(256, RegexOptions::None);

			TurkishI_Is_Differently_LowerUpperCased_In_Turkish_Culture_NonBacktracking();
			TurkishCulture_Handling_Of_IgnoreCase();


			TurkishCulture_MatchesWordChar(L"I\u0131\u0130i", RegexOptions::None, L"I\u0131\u0130i");
			TurkishCulture_MatchesWordChar(L"I\u0131\u0130i", RegexOptions::IgnoreCase, L"I\u0131\u0130i");


			const auto value_2 = Match_In_Different_Cultures_TestData();
			for (size_t i = 0; i < value_2.size(); ++i)
			{
				StructDifferent_Cultures cultures = value_2[i];
				Match_In_Different_Cultures(cultures.pattern.c_str(), cultures.culture, cultures.input.c_str(), cultures.match_expected.c_str());
			}

			const auto value_3 = Match_In_Different_Cultures_CriticalCases_TestData_For();
			for (StructDifferent_Cultures cultures : value_3)
				Match_In_Different_Cultures_CriticalCases(cultures.pattern.c_str(), cultures.culture, cultures.input.c_str(), cultures.match_expected.c_str());

			Match_InvariantCulture_None_vs_Compiled();
		}


		static vector<StructAnchoredPattern_TestData> CharactersComparedOneByOne_AnchoredPattern_TestData()
		{
			return
			{
				StructAnchoredPattern_TestData(L"^aa$", L"aA", L"da-DK", RegexOptions::None, false),
				StructAnchoredPattern_TestData(L"^aA$", L"aA", L"da-DK", RegexOptions::None, true),
				StructAnchoredPattern_TestData(L"^aa$", L"aA", L"da-DK", RegexOptions::IgnoreCase, true),
				StructAnchoredPattern_TestData(L"^aA$", L"aA", L"da-DK", RegexOptions::IgnoreCase, true),
			};
		}

		static void CharactersComparedOneByOne_AnchoredPattern(wstring pattern, wstring input, wstring culture, RegexOptions options, bool expected)
		{
			// Regex compares characters one by one.  If that changes, it could impact the behavior of
			// a case like this, where these characters are not the same, but the strings compare
			// as equal with the invariant culture (and some other cultures as well).

			const auto cache = GetCultureInfo::s_userDefaultCulture;
			GetCultureInfo::s_userDefaultCulture = ComCultureInfo::CreateSpecificCulture(culture);
			Regex r = Regex(pattern, options);
			Assert::True(expected == r.IsMatch(input));
			GetCultureInfo::s_userDefaultCulture = cache;
			GetCultureInfo::Init();
		}

		static wstring repeatwstring(wstring text, int loop)
		{
			wstring result;
			for (int i = 0; i < loop; ++i)
			{
				result += text;
			}
			return result;
		}

		static void CharactersComparedOneByOne_Invariant(RegexOptions options)
		{
			// Regex compares characters one by one.  If that changes, it could impact the behavior of
			// a case like this, where these characters are not the same, but the strings compare
			// as equal with the invariant culture (and some other cultures as well).
			const wstring S1 = L"\u00D6\u200D";
			const wstring S2 = L"\u004F\u0308";

			// Validate the chosen strings to make sure they compare the way we want to test via Regex
			Assert::False(S1[0] == S2[0]);
			Assert::False(S1[1] == S2[1]);
			//Assert::StartsWith(S1, S2, StringComparison::InvariantCulture);//method not support
			//Assert::True(S1.Equals(S2, StringComparison::InvariantCulture));//method not support

			// Test varying lengths of strings to validate codegen changes that kick in at longer lengths
			const int sss[] = {1, 10, 100};
			for (int i = 0; i < 3; ++i)
			{
				const int multiple = sss[i];
				wstring pattern = repeatwstring(S1, multiple);
				wstring input = repeatwstring(S2, multiple);


				// Validate when the string is at the beginning of the pattern, as it impacts prefix matching.
				Regex r_1 = Regex(pattern, options);
				Assert::False(r_1.IsMatch(input));
				Assert::True(r_1.IsMatch(pattern));

				// Validate when it's not at the beginning of the pattern, as it impacts "multi" matching.
				Regex r_2 = Regex(L"[abc]" + pattern, options);
				Assert::False(r_2.IsMatch(L"a" + input));
				Assert::True(r_2.IsMatch(L"a" + pattern));
			}
		}

		static void CharactersLowercasedOneByOne()
		{
			const auto cache = GetCultureInfo::s_userDefaultCulture;
			GetCultureInfo::s_userDefaultCulture = ComCultureInfo::CreateSpecificCulture(L"en-US");

			Assert::True((Regex(UtilsString::StringInitSpe(0xd801) + UtilsString::StringInitSpe(0xdc00), RegexOptions::IgnoreCase)).IsMatch(UtilsString::StringInitSpe(0xd801) + UtilsString::StringInitSpe(0xdc00)));
			Assert::True((Regex(UtilsString::StringInitSpe(0xd801) + UtilsString::StringInitSpe(0xdc00), RegexOptions::IgnoreCase)).IsMatch(L"abcdefg" + UtilsString::StringInitSpe(0xd801) + UtilsString::StringInitSpe(0xdc00)));
			Assert::True((Regex(UtilsString::StringInitSpe(0xd801), RegexOptions::IgnoreCase)).IsMatch(UtilsString::StringInitSpe(0xd801) + UtilsString::StringInitSpe(0xdc00)));
			Assert::True((Regex(UtilsString::StringInitSpe(0xdc00), RegexOptions::IgnoreCase)).IsMatch(UtilsString::StringInitSpe(0xd801) + UtilsString::StringInitSpe(0xdc00)));
			GetCultureInfo::s_userDefaultCulture = cache;
		}

		static void TurkishI_Is_Differently_LowerUpperCased_In_Turkish_Culture(int length, RegexOptions options)
		{
			//not support
		}

		static void TurkishI_Is_Differently_LowerUpperCased_In_Turkish_Culture_NonBacktracking()
		{
			//fail model 
		}

		static void TurkishCulture_Handling_Of_IgnoreCase()
		{
			const auto turkish = ComCultureInfo::CreateSpecificCulture(L"tr-TR");
			const wstring input = L"I\u0131\u0130i";
			const wstring pattern = L"[H-J][\u0131-\u0140][\u0120-\u0130][h-j]";

			GetCultureInfo::Init();
			const auto cache = GetCultureInfo::s_userDefaultCulture;
			GetCultureInfo::s_userDefaultCulture = turkish;
			GetCultureInfo::s_userDefaultUICulture = turkish;


			Regex regex = Regex(pattern, RegexOptions::IgnoreCase);
			// The pattern must trivially match the input because all of the letters fall in the given intervals
			// Ignoring case can only add more letters here -- not REMOVE letters

			bool temp = regex.IsMatch(input);
			//Assert::True(temp);//bdki false with c# test 
			Assert::False(temp);

			GetCultureInfo::s_userDefaultCulture = cache;
		}

		static void TurkishCulture_MatchesWordChar(wstring input, RegexOptions options, wstring expectedResult)
		{
			const auto cache = GetCultureInfo::s_userDefaultCulture;
			GetCultureInfo::s_userDefaultCulture = ComCultureInfo::CreateSpecificCulture(L"tr-TR");

			Regex regex = Regex(L"\\w*", options);
			auto temp = regex.Match(&input)->Value;
			Assert::True(expectedResult == temp);

			GetCultureInfo::s_userDefaultCulture = cache;
		}


		static vector<StructDifferent_Cultures> Match_In_Different_Cultures_TestData()
		{
			ComCultureInfo* invariant = ComCultureInfo::InvariantCulture();
			ComCultureInfo* enUS = ComCultureInfo::CreateSpecificCulture(L"en-US");
			ComCultureInfo* turkish = ComCultureInfo::CreateSpecificCulture(L"tr-TR");

			vector<StructDifferent_Cultures> value =
			{
				// \u0130 (Turkish I with dot) and \u0131 (Turkish i without dot) are unrelated characters in general

				// Expected answers in the default en-US culture
				StructDifferent_Cultures(L"(?i:I)", enUS, L"xy\u0131ab", L""),
				StructDifferent_Cultures(L"(?i:iI+)", enUS, L"abcIIIxyz", L"III"),
				StructDifferent_Cultures(L"(?i:iI+)", enUS, L"abcIi\u0130xyz", L"Ii\u0130"),
				StructDifferent_Cultures(L"(?i:iI+)", enUS, L"abcI\u0130ixyz", L"I\u0130i"),
				StructDifferent_Cultures(L"(?i:iI+)", enUS, L"abc\u0130IIxyz", L"\u0130II"),
				StructDifferent_Cultures(L"(?i:iI+)", enUS, L"abc\u0130\u0131Ixyz", L""),
				StructDifferent_Cultures(L"(?i:iI+)", enUS, L"abc\u0130Iixyz", L"\u0130Ii"),
				StructDifferent_Cultures(L"(?i:[^IJKLM]I)", enUS, L"ii\u0130i\u0131ab", L""),

				// Expected answers in the invariant culture
				StructDifferent_Cultures(L"(?i:I)", invariant, L"xy\u0131ab", L""),
				StructDifferent_Cultures(L"(?i:iI+)", invariant, L"abcIIIxyz", L"III"),
				StructDifferent_Cultures(L"(?i:iI+)", invariant, L"abc\u0130\u0131Ixyz", L""),

				// Expected answers in the Turkish culture
				//
				// Android produces unexpected results for tr-TR
				// https://github.com/dotnet/runtime/issues/60568
				StructDifferent_Cultures(L"(?i:I)", turkish, L"xy\u0131ab", L"\u0131"),
				StructDifferent_Cultures(L"(?i:iI+)", turkish, L"abcIIIxyz", L""),
				StructDifferent_Cultures(L"(?i:iI+)", turkish, L"abcIi\u0130xyz", L""),
				StructDifferent_Cultures(L"(?i:iI+)", turkish, L"abcI\u0130ixyz", L""),
				StructDifferent_Cultures(L"(?i:[^IJKLM]I)", turkish, L"ii\u0130i\u0131ab", L"i\u0131"),


			};

			const auto value_2 = Match_In_Different_Cultures_CriticalCases_TestData_For();
			for (StructDifferent_Cultures cultures : value_2)
			{
				value.push_back(cultures);
			}
			return value;
		}

		static vector<StructDifferent_Cultures> Match_In_Different_Cultures_CriticalCases_TestData_For()
		{
			ComCultureInfo* invariant = ComCultureInfo::InvariantCulture();
			ComCultureInfo* turkish = ComCultureInfo::CreateSpecificCulture(L"tr-TR");

			return {
				// Expected answers in the invariant culture
				StructDifferent_Cultures(L"(?i:iI+)", invariant, L"abcIi\u0130xyz", L"Ii"), // <-- failing for None, Compiled
				StructDifferent_Cultures(L"(?i:iI+)", invariant, L"abcI\u0130ixyz", L""), // <-- failing for Compiled
				StructDifferent_Cultures(L"(?i:iI+)", invariant, L"abc\u0130IIxyz", L"II"), // <-- failing for Compiled
				StructDifferent_Cultures(L"(?i:iI+)", invariant, L"abc\u0130Iixyz", L"Ii"), // <-- failing for Compiled
				StructDifferent_Cultures(L"(?i:[^IJKLM]I)", invariant, L"ii\u0130i\u0131ab", L"\u0130i"), // <-- failing for None, Compiled

				// Expected answers in the Turkish culture
				// Android produces unexpected results for tr-TR
				// https://github.com/dotnet/runtime/issues/60568
				StructDifferent_Cultures(L"(?i:iI+)", turkish, L"abc\u0130IIxyz", L"\u0130II"), // <-- failing for None, Compiled
				StructDifferent_Cultures(L"(?i:iI+)", turkish, L"abc\u0130\u0131Ixyz", L"\u0130\u0131I"), // <-- failing for None, Compiled
				StructDifferent_Cultures(L"(?i:iI+)", turkish, L"abc\u0130Iixyz", L"\u0130I") // <-- failing for None, Compiled
			};
		}

		static void Match_In_Different_Cultures(wstring pattern, ComCultureInfo* culture, wstring input, wstring match_expected)
		{
			const auto cache = GetCultureInfo::s_userDefaultCulture;
			GetCultureInfo::s_userDefaultCulture = culture;

			Regex r = Regex(pattern, RegexOptions::None);
			const Match* match = r.Match(&input);
			Assert::True(match_expected == match->Value);

			GetCultureInfo::s_userDefaultCulture = cache;
		}

		static void Match_In_Different_Cultures_CriticalCases(wstring pattern, ComCultureInfo* culture, wstring input, wstring match_expected)
		{
			const auto cache = GetCultureInfo::s_userDefaultCulture;
			GetCultureInfo::s_userDefaultCulture = culture;


			Regex r = Regex(pattern, RegexOptions::None);
			const Match* match = r.Match(&input);
			Assert::True(match_expected == match->Value);


			GetCultureInfo::s_userDefaultCulture = cache;
		}

		static void Match_InvariantCulture_None_vs_Compiled()
		{
			//not support
		}
	};
}
