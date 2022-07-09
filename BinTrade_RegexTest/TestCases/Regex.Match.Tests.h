#pragma once

#include "random.h"
#include "RegexHelpers.h"
#include "RegexMatchTimeoutException.h" 

namespace RegexsTest
{
	class RegexMatchTests
	{
		struct StructCases
		{
			std::wstring Pattern;
			std::wstring Input;
			RegexOptions Options;
			int Beginning;
			int Length;
			bool ExpectedSuccess;
			std::wstring ExpectedValue;

			StructCases(const std::wstring& pattern, const std::wstring& input, RegexOptions options, int beginning, int length, bool expected_success, const std::wstring& expected_value)
				: Pattern(pattern),
				  Input(input),
				  Options(options),
				  Beginning(beginning),
				  Length(length),
				  ExpectedSuccess(expected_success),
				  ExpectedValue(expected_value)
			{
			}
		};

		struct StructMatch_Advanced
		{
			std::wstring pattern;
			std::wstring input;
			RegexOptions options;
			int beginning;
			int length;
			vector<CaptureData> expected;

			StructMatch_Advanced(const std::wstring& pattern, const std::wstring& input, RegexOptions options, int beginning, int length, const vector<CaptureData>& expected)
				: pattern(pattern),
				  input(input),
				  options(options),
				  beginning(beginning),
				  length(length),
				  expected(expected)
			{
			}
		};

		struct StructStartatDiffersFromBeginning
		{
			std::wstring pattern;
			std::wstring input;
			RegexOptions options;
			int startat;
			bool expectedSuccessStartAt;
			bool expectedSuccessBeginning;

			StructStartatDiffersFromBeginning(const std::wstring& pattern, const std::wstring& input, RegexOptions options, int startat, bool expected_success_start_at, bool expected_success_beginning)
				: pattern(pattern),
				  input(input),
				  options(options),
				  startat(startat),
				  expectedSuccessStartAt(expected_success_start_at),
				  expectedSuccessBeginning(expected_success_beginning)
			{
			}
		};

		struct StructMatch_Count
		{
			std::wstring pattern;
			std::wstring input;
			int expectedCount;

			StructMatch_Count(const std::wstring& pattern, const std::wstring& input, int expected_count)
				: pattern(pattern),
				  input(input),
				  expectedCount(expected_count)
			{
			}
		};

		struct StrcutAllMatchesItem
		{
			int Item1;
			int Item2;
			std::wstring Item3;

			StrcutAllMatchesItem(int value1, int value2, const std::wstring& value3)
				: Item1(value1),
				  Item2(value2),
				  Item3(value3)
			{
			}
		};

		struct StrcutAllMatches
		{
			std::wstring pattern;
			RegexOptions options;
			std::wstring input;
			std::vector<StrcutAllMatchesItem> matches;

			StrcutAllMatches(const std::wstring& pattern, RegexOptions options, const std::wstring& input, const std::vector<StrcutAllMatchesItem>& matches)
				: pattern(pattern),
				  options(options),
				  input(input),
				  matches(matches)
			{
			}
		};

		struct StructMatchAmbiguousRegexes_TestData
		{
			std::wstring pattern;
			std::wstring input;
			int Item1;
			int Item2;

			StructMatchAmbiguousRegexes_TestData(const std::wstring& pattern, const std::wstring& input, int item1, int item2)
				: pattern(pattern),
				  input(input),
				  Item1(item1),
				  Item2(item2)
			{
			}
		};

		struct StructMatchWordsInAnchoredRegexestem
		{
			int Item1;
			int Item2;

			StructMatchWordsInAnchoredRegexestem(int item1, int item2)
				: Item1(item1),
				  Item2(item2)
			{
			}
		};

		struct StructMatchWordsInAnchoredRegexes
		{
			RegexOptions options;
			wstring pattern;
			wstring input;
			vector<StructMatchWordsInAnchoredRegexestem> matches;

			StructMatchWordsInAnchoredRegexes(RegexOptions options, const wstring& pattern, const wstring& input, const vector<StructMatchWordsInAnchoredRegexestem>& matches)
				: options(options),
				  pattern(pattern),
				  input(input),
				  matches(matches)
			{
			}
		};

		static bool IsDefaultCount(wstring input, RegexOptions options, int count)
		{
			if (((int)options & (int)RegexOptions::RightToLeft) != 0)
			{
				return count == input.length() || count == -1;
			}
			return count == input.length();
		}

		static bool IsDefaultStart(wstring input, RegexOptions options, int start)
		{
			if (((int)options & (int)RegexOptions::RightToLeft) != 0)
			{
				return start == input.length();
			}
			return start == 0;
		}

	public:
		static void Run(bool test_throw)
		{
			Assert::WriteLine(L"RegexMatchTests");

			auto value_1 = Match_MemberData_Cases();
			for (size_t i = 0; i < value_1.size(); ++i)
			{
				StructCases struct_cases = value_1.at(i);
				CreateAndMatch(struct_cases.Pattern, struct_cases.Input, struct_cases.Options, struct_cases.Beginning, struct_cases.Length, struct_cases.ExpectedSuccess, struct_cases.ExpectedValue.c_str());
			}

			Match_Timeout();
			if (test_throw)Match_TestThatTimeoutHappens();
			if (test_throw)Match_Timeout_Throws();

			//Match_DefaultTimeout_Throws(); //???
			if (test_throw)Match_CachedPattern_NewTimeoutApplies(RegexOptions::None);
			if (test_throw)Match_Timeout_Repetition_Throws();

			auto value_2 = Match_Advanced_TestData();
			for (size_t i = 0; i < value_2.size(); ++i)
			{
				auto item = value_2.at(i);
				Match_Advanced(item.pattern, item.input, item.options, item.beginning, item.length, item.expected);
			}

			auto value_3 = Match_StartatDiffersFromBeginning_MemberData();
			for (size_t i = 0; i < value_3.size(); ++i)
			{
				auto item = value_3.at(i);
				Match_StartatDiffersFromBeginning(item.pattern, item.input, item.options, item.startat, item.expectedSuccessStartAt, item.expectedSuccessBeginning);
			}

			Result(L"(?<1>\\d{1,2})/(?<2>\\d{1,2})/(?<3>\\d{2,4})\\s(?<time>\\S+)", L"08/10/99 16:00", L"${time}", L"16:00");
			Result(L"(?<1>\\d{1,2})/(?<2>\\d{1,2})/(?<3>\\d{2,4})\\s(?<time>\\S+)", L"08/10/99 16:00", L"${1}", L"08");
			Result(L"(?<1>\\d{1,2})/(?<2>\\d{1,2})/(?<3>\\d{2,4})\\s(?<time>\\S+)", L"08/10/99 16:00", L"${2}", L"10");
			Result(L"(?<1>\\d{1,2})/(?<2>\\d{1,2})/(?<3>\\d{2,4})\\s(?<time>\\S+)", L"08/10/99 16:00", L"${3}", L"99");
			Result(L"abc", L"abc", L"abc", L"abc");

			if (test_throw)Result_Invalid();

			Match_SpecialUnicodeCharacters_enUS();
			Match_SpecialUnicodeCharacters_Invariant();

			if (test_throw)Match_Invalid();
			if (test_throw)IsMatch_Invalid();

			IsMatch_SucceedQuicklyDueToLoopReduction(L"(?:\\w*)+\\.", L"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", false);
			IsMatch_SucceedQuicklyDueToLoopReduction(L"(?:a+)+b", L"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", false);
			//IsMatch_SucceedQuicklyDueToLoopReduction(L"(?:x+x+)+y", L"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", false);// todo slow

			TestCharIsLowerCultureEdgeCasesAroundTurkishCharacters();

			if (test_throw)Synchronized();
			Match_Boundary();


			auto value_4 = Match_Count_TestData();
			for (size_t i = 0; i < value_4.size(); ++i)
			{
				auto item = value_4.at(i);
				Match_Count(item.pattern, item.input, item.expectedCount);
			}

			StressTestNfaMode(L"(?:a|aa|[abc]?[ab]?[abcd]).{20}$", L"aaa01234567890123456789", 23);
			StressTestNfaMode(L"(?:a|AA|BCD).{20}$", L"a01234567890123456789", 21);
			StressTestNfaMode(L"(?:a.{20}|a.{10})bc$", L"a01234567890123456789bc", 23);

			auto value_5 = AllMatches_TestData();
			for (size_t i = 0; i < value_5.size(); ++i)
			{
				auto item = value_5.at(i);
				AllMatches(item.pattern, item.options, item.input, item.matches);
			}

			StandardCharSets_SameMeaningAcrossAllEngines(L"\\w");
			StandardCharSets_SameMeaningAcrossAllEngines(L"\\s");
			StandardCharSets_SameMeaningAcrossAllEngines(L"\\d");

			Match_DisjunctionOverCounting(L"a[abc]{0,10}", L"a[abc]{0,3}", L"xxxabbbbbbbyyy", true, L"abbbbbbb");
			Match_DisjunctionOverCounting(L"a[abc]{0,10}?", L"a[abc]{0,3}?", L"xxxabbbbbbbyyy", true, L"a");

			auto value_6 = MatchAmbiguousRegexes_TestData();
			for (size_t i = 0; i < value_6.size(); ++i)
			{
				auto item = value_6.at(i);
				MatchAmbiguousRegexes(item.pattern, item.input, item.Item1, item.Item2);
			}

			auto value_7 = MatchWordsInAnchoredRegexes_TestData();
			for (size_t i = 0; i < value_7.size(); ++i)
			{
				auto item = value_7.at(i);
				MatchWordsInAnchoredRegexes(item.options, item.pattern, item.input, item.matches);
			}
		}


		static vector<StructCases> Match_MemberData_Cases()
		{
			return
			{
				// pattern, input, options, beginning, length, expectedSuccess, expectedValue
				StructCases(L"H#", L"#H#", RegexOptions::IgnoreCase, 0, 3, true, L"H#"), // https://github.com/dotnet/runtime/issues/39390
				StructCases(L"H#", L"#H#", RegexOptions::None, 0, 3, true, L"H#"),

				// Testing octal sequence matches: "\\060(\\061)?\\061"
				// Octal \061 is ASCII 49 ('1')
				StructCases(L"\\060(\\061)?\\061", L"011", RegexOptions::None, 0, 3, true, L"011"),

				// Testing hexadecimal sequence matches: "(\\x30\\x31\\x32)"
				// Hex \x31 is ASCII 49 ('1')
				StructCases(L"(\\x30\\x31\\x32)", L"012", RegexOptions::None, 0, 3, true, L"012"),

				// Testing control character escapes???: "2", "(\u0032)"
				StructCases(L"(\u0034)", L"4", RegexOptions::None, 0, 1, true, L"4"),

				// Using long loop prefix
				StructCases(L"a{10}", wstring(10, 'a'), RegexOptions::None, 0, 10, true, wstring(10, 'a')),
				StructCases(L"a{100}", wstring(100, 'a'), RegexOptions::None, 0, 100, true, wstring(100, 'a')),

				StructCases(L"a{10}b", wstring(10, 'a') + L"bc", RegexOptions::None, 0, 12, true, wstring(10, 'a') + L"b"),
				StructCases(L"a{100}b", wstring(100, 'a') + L"bc", RegexOptions::None, 0, 102, true, wstring(100, 'a') + L"b"),

				StructCases(L"a{11}b", wstring(10, 'a') + L"bc", RegexOptions::None, 0, 12, false, L""),
				StructCases(L"a{101}b", wstring(100, 'a') + L"bc", RegexOptions::None, 0, 102, false, L""),

				StructCases(L"a{1,3}b", L"bc", RegexOptions::None, 0, 2, false, L""),
				StructCases(L"a{1,3}b", L"abc", RegexOptions::None, 0, 3, true, L"ab"),
				StructCases(L"a{1,3}b", L"aaabc", RegexOptions::None, 0, 5, true, L"aaab"),
				StructCases(L"a{1,3}b", L"aaaabc", RegexOptions::None, 0, 6, true, L"aaab"),
				StructCases(L"a{1,3}?b", L"bc", RegexOptions::None, 0, 2, false, L""),
				StructCases(L"a{1,3}?b", L"abc", RegexOptions::None, 0, 3, true, L"ab"),
				StructCases(L"a{1,3}?b", L"aaabc", RegexOptions::None, 0, 5, true, L"aaab"),
				StructCases(L"a{1,3}?b", L"aaaabc", RegexOptions::None, 0, 6, true, L"aaab"),

				StructCases(L"a{2,}b", L"abc", RegexOptions::None, 0, 3, false, L""),
				StructCases(L"a{2,}b", L"aabc", RegexOptions::None, 0, 4, true, L"aab"),
				StructCases(L"a{2,}?b", L"abc", RegexOptions::None, 0, 3, false, L""),
				StructCases(L"a{2,}?b", L"aabc", RegexOptions::None, 0, 4, true, L"aab"),

				// {,n} is treated as a literal rather than {0,n} as it should be
				StructCases(L"a{,3}b", L"a{,3}bc", RegexOptions::None, 0, 6, true, L"a{,3}b"),
				StructCases(L"a{,3}b", L"aaabc", RegexOptions::None, 0, 5, false, L""),

				// Using [a-z], \s, \w: Actual - "([a-zA-Z]+)\\s(\\w+)"
				StructCases(L"([a-zA-Z]+)\\s(\\w+)", L"David Bau", RegexOptions::None, 0, 9, true, L"David Bau"),
				StructCases(L"([a-zA-Z]+?)\\s(\\w+)", L"David Bau", RegexOptions::None, 0, 9, true, L"David Bau"),

				// \\S, \\d, \\D, \\W: Actual - "(\\S+):\\W(\\d+)\\s(\\D+)"
				StructCases(L"(\\S+):\\W(\\d+)\\s(\\D+)", L"Price: 5 dollars", RegexOptions::None, 0, 16, true, L"Price: 5 dollars"),

				// \\S, \\d, \\D, \\W: Actual - "[^0-9]+(\\d+)"
				StructCases(L"[^0-9]+(\\d+)", L"Price: 30 dollars", RegexOptions::None, 0, 17, true, L"Price: 30"),


				// Zero-width negative lookahead assertion: Actual - "abc(?!XXX)\\w+"
				StructCases(L"abc(?!XXX)\\w+", L"abcXXXdef", RegexOptions::None, 0, 9, false, L""),

				// Zero-width positive lookbehind assertion: Actual - "(\\w){6}(?<=XXX)def"
				StructCases(L"(\\w){6}(?<=XXX)def", L"abcXXXdef", RegexOptions::None, 0, 9, true, L"abcXXXdef"),
				StructCases(L"(?<=c)def", L"123abcdef", RegexOptions::None, 0, 9, true, L"def"),
				StructCases(L"(?<=abc)def", L"123abcdef", RegexOptions::None, 0, 9, true, L"def"),
				StructCases(L"(?<=a\\wc)def", L"123abcdef", RegexOptions::None, 0, 9, true, L"def"),
				StructCases(L"(?<=\\ba\\wc)def", L"123 abcdef", RegexOptions::None, 0, 10, true, L"def"),
				StructCases(L"(?<=.\\ba\\wc\\B)def", L"123 abcdef", RegexOptions::None, 0, 10, true, L"def"),
				StructCases(L"(?<=^123 abc)def", L"123 abcdef", RegexOptions::None, 0, 10, true, L"def"),
				StructCases(L"(?<=^123 abc)def", L"123 abcdef", RegexOptions::Multiline, 0, 10, true, L"def"),
				StructCases(L"(?<=123$\\nabc)def", L"123\nabcdef", RegexOptions::Multiline, 0, 10, true, L"def"),
				StructCases(L"\\w{3}(?<=^xyz|^abc)defg", L"abcdefg", RegexOptions::None, 0, 7, true, L"abcdefg"),
				StructCases(L"(abc)\\w(?<=(?(1)d|e))", L"abcdabc", RegexOptions::None, 0, 7, true, L"abcd"),
				StructCases(L"(abc)\\w(?<=(?(cd)d|e))", L"abcdabc", RegexOptions::None, 0, 7, true, L"abcd"),
				StructCases(L"\\w{3}(?<=(\\w){6,8})", L"abcdefghijklmnop", RegexOptions::None, 0, 16, true, L"def"),
				StructCases(L"\\w{3}(?<=(?:\\d\\w){4})", L"a1b2c3d4e5", RegexOptions::None, 0, 10, true, L"d4e"),
				StructCases(L"\\w{3}(?<=(\\w){6,8}?)", L"abcdefghijklmnop", RegexOptions::None, 0, 16, true, L"def"),
				StructCases(L"\\w{3}(?<=(?:\\d\\w){3,4}?\\d)", L"a1b2c3d4e5", RegexOptions::None, 0, 10, true, L"3d4"),
				StructCases(L"(\\w{3})\\w*(?<=\\1)", L"---abcdefababc123", RegexOptions::None, 0, 17, true, L"abcdefababc"),
				StructCases(L"(?<=\\w{3})\\w{4}", L"abcdefghijklmnop", RegexOptions::None, 0, 16, true, L"defg"),
				StructCases(L"(?<=\\w{3,8})\\w{4}", L"abcdefghijklmnop", RegexOptions::None, 0, 16, true, L"defg"),
				StructCases(L"(?<=\\w*)\\w{4}", L"abcdefghijklmnop", RegexOptions::None, 0, 16, true, L"abcd"),
				StructCases(L"(?<=\\w?)\\w{4}", L"abcdefghijklmnop", RegexOptions::None, 0, 16, true, L"abcd"),
				StructCases(L"(?<=\\d?)a{4}", L"123aaaaaaaaa", RegexOptions::None, 0, 12, true, L"aaaa"),
				StructCases(L"(?<=a{3,5}[ab]*)1234", L"aaaaaaa1234", RegexOptions::None, 0, 11, true, L"1234"),

				// Zero-width negative lookbehind assertion: Actual - "(\\w){6}(?<!XXX)def"
				StructCases(L"(\\w){6}(?<!XXX)def", L"XXXabcdef", RegexOptions::None, 0, 9, true, L"XXXabcdef"),
				StructCases(L"123(?<!$) abcdef", L"123 abcdef", RegexOptions::None, 0, 10, true, L"123 abcdef"),
				StructCases(L"(abc)\\w(?<!(?(1)e|d))", L"abcdabc", RegexOptions::None, 0, 7, true, L"abcd"),
				StructCases(L"(abc)\\w(?<!(?(cd)e|d))", L"abcdabc", RegexOptions::None, 0, 7, true, L"abcd"),


				// Nonbacktracking subexpression: Actual - "[^0-9]+(?>[0-9]+)3"
				// The last 3 causes the match to fail, since the non backtracking subexpression does not give up the last digit it matched
				// for it to be a success. For a correct match, remove the last character, '3' from the pattern
				StructCases(L"[^0-9]+(?>[0-9]+)3", L"abc123", RegexOptions::None, 0, 6, false, L""),
				StructCases(L"[^0-9]+(?>[0-9]+)", L"abc123", RegexOptions::None, 0, 6, true, L"abc123"),

				StructCases(L"(?!.*a)\\w*g", L"bcaefg", RegexOptions::None, 0, 6, true, L"efg"),
				StructCases(L"(?!.*a)\\w*g", L"aaaaag", RegexOptions::None, 0, 6, true, L"g"),
				StructCases(L"(?!.*a)\\w*g", L"aaaaaa", RegexOptions::None, 0, 6, false, L""),


				// More nonbacktracking expressions
				StructCases((L"(?:hi|hello|hey)hi"), L"hellohi", RegexOptions::None, 0, 7, true, L"hellohi"), // allow backtracking and it succeeds
				StructCases((L"a[^wyz] * w"), L"abczw", RegexOptions::IgnoreCase, 0, 0, false, L""),

				// Atomic greedy
				StructCases((L"(?>[0-9]+)abc"), L"abc12345abc", RegexOptions::None, 3, 8, true, L"12345abc"),
				StructCases((L"(?>(?>[0-9]+))abc"), L"abc12345abc", RegexOptions::None, 3, 8, true, L"12345abc"),
				StructCases((L"(?>[0-9]*)abc"), L"abc12345abc", RegexOptions::None, 3, 8, true, L"12345abc"),
				StructCases((L"(?>[^z]+)z"), L"zzzzxyxyxyz123", RegexOptions::None, 4, 9, true, L"xyxyxyz"),
				StructCases((L"(?>(?>[^z]+))z"), L"zzzzxyxyxyz123", RegexOptions::None, 4, 9, true, L"xyxyxyz"),
				StructCases((L"(?>[^z]*)z123"), L"zzzzxyxyxyz123", RegexOptions::None, 4, 10, true, L"xyxyxyz123"),
				StructCases((L"(?>a+)123"), L"aa1234", RegexOptions::None, 0, 5, true, L"aa123"),
				StructCases((L"(?>a*)123"), L"aa1234", RegexOptions::None, 0, 5, true, L"aa123"),
				StructCases((L"(?>(?>a*))123"), L"aa1234", RegexOptions::None, 0, 5, true, L"aa123"),
				StructCases((L"(?>a{2,})b"), L"aaab", RegexOptions::None, 0, 4, true, L"aaab"),

				// Atomic lazy
				StructCases((L"(?>[0-9]+?)abc"), L"abc12345abc", RegexOptions::None, 3, 8, true, L"5abc"),
				StructCases((L"(?>(?>[0-9]+?))abc"), L"abc12345abc", RegexOptions::None, 3, 8, true, L"5abc"),
				StructCases((L"(?>[0-9]*?)abc"), L"abc12345abc", RegexOptions::None, 3, 8, true, L"abc"),
				StructCases((L"(?>[^z]+?)z"), L"zzzzxyxyxyz123", RegexOptions::None, 4, 9, true, L"yz"),
				StructCases((L"(?>(?>[^z]+?))z"), L"zzzzxyxyxyz123", RegexOptions::None, 4, 9, true, L"yz"),
				StructCases((L"(?>[^z]*?)z123"), L"zzzzxyxyxyz123", RegexOptions::None, 4, 10, true, L"z123"),
				StructCases((L"(?>a+?)123"), L"aa1234", RegexOptions::None, 0, 5, true, L"a123"),
				StructCases((L"(?>a*?)123"), L"aa1234", RegexOptions::None, 0, 5, true, L"123"),
				StructCases((L"(?>(?>a*?))123"), L"aa1234", RegexOptions::None, 0, 5, true, L"123"),
				StructCases((L"(?>a{2,}?)b"), L"aaab", RegexOptions::None, 0, 4, true, L"aab"),

				// Alternations
				StructCases((L"(?>hi|hello|hey)hi"), L"hellohi", RegexOptions::None, 0, 0, false, L""),
				StructCases((L"(?>hi|hello|hey)hi"), L"hihi", RegexOptions::None, 0, 4, true, L"hihi"),


				StructCases((L"(?:HI|HELLO|HEY)HI"), L"hellohi", RegexOptions::IgnoreCase, 0, 7, true, L"hellohi"), // allow backtracking and it succeeds
				StructCases((L"A[^WYZ] * W"), L"abczw", RegexOptions::IgnoreCase, 0, 0, false, L""),

				// Atomic greedy
				StructCases((L"(?>[0-9]+)ABC"), L"abc12345abc", RegexOptions::IgnoreCase, 3, 8, true, L"12345abc"),
				StructCases((L"(?>(?>[0-9]+))ABC"), L"abc12345abc", RegexOptions::IgnoreCase, 3, 8, true, L"12345abc"),
				StructCases((L"(?>[0-9]*)ABC"), L"abc12345abc", RegexOptions::IgnoreCase, 3, 8, true, L"12345abc"),
				StructCases((L"(?>[^Z]+)Z"), L"zzzzxyxyxyz123", RegexOptions::IgnoreCase, 4, 9, true, L"xyxyxyz"),
				StructCases((L"(?>(?>[^Z]+))Z"), L"zzzzxyxyxyz123", RegexOptions::IgnoreCase, 4, 9, true, L"xyxyxyz"),
				StructCases((L"(?>[^Z]*)Z123"), L"zzzzxyxyxyz123", RegexOptions::IgnoreCase, 4, 10, true, L"xyxyxyz123"),
				StructCases((L"(?>A+)123"), L"aa1234", RegexOptions::IgnoreCase, 0, 5, true, L"aa123"),
				StructCases((L"(?>A*)123"), L"aa1234", RegexOptions::IgnoreCase, 0, 5, true, L"aa123"),
				StructCases((L"(?>(?>A*))123"), L"aa1234", RegexOptions::IgnoreCase, 0, 5, true, L"aa123"),
				StructCases((L"(?>A{2,})B"), L"aaab", RegexOptions::IgnoreCase, 0, 4, true, L"aaab"),

				// Atomic lazy
				StructCases((L"(?>[0-9]+?)ABC"), L"abc12345abc", RegexOptions::IgnoreCase, 3, 8, true, L"5abc"),
				StructCases((L"(?>(?>[0-9]+?))ABC"), L"abc12345abc", RegexOptions::IgnoreCase, 3, 8, true, L"5abc"),
				StructCases((L"(?>[0-9]*?)ABC"), L"abc12345abc", RegexOptions::IgnoreCase, 3, 8, true, L"abc"),
				StructCases((L"(?>[^Z]+?)Z"), L"zzzzxyxyxyz123", RegexOptions::IgnoreCase, 4, 9, true, L"yz"),
				StructCases((L"(?>(?>[^Z]+?))Z"), L"zzzzxyxyxyz123", RegexOptions::IgnoreCase, 4, 9, true, L"yz"),
				StructCases((L"(?>[^Z]*?)Z123"), L"zzzzxyxyxyz123", RegexOptions::IgnoreCase, 4, 10, true, L"z123"),
				StructCases((L"(?>A+?)123"), L"aa1234", RegexOptions::IgnoreCase, 0, 5, true, L"a123"),
				StructCases((L"(?>A*?)123"), L"aa1234", RegexOptions::IgnoreCase, 0, 5, true, L"123"),
				StructCases((L"(?>(?>A*?))123"), L"aa1234", RegexOptions::IgnoreCase, 0, 5, true, L"123"),
				StructCases((L"(?>A{2,}?)B"), L"aaab", RegexOptions::IgnoreCase, 0, 4, true, L"aab"),

				// Alternations
				StructCases((L"(?>HI|HELLO|HEY)HI"), L"hellohi", RegexOptions::IgnoreCase, 0, 0, false, L""),
				StructCases((L"(?>HI|HELLO|HEY)HI"), L"hihi", RegexOptions::IgnoreCase, 0, 4, true, L"hihi"),


				// Loops at beginning of expressions
				StructCases(L"a+", L"aaa", RegexOptions::None, 0, 3, true, L"aaa"),
				StructCases(L"a+\\d+", L"a1", RegexOptions::None, 0, 2, true, L"a1"),
				StructCases(L".+\\d+", L"a1", RegexOptions::None, 0, 2, true, L"a1"),
				StructCases(L".+\\nabc", L"a\nabc", RegexOptions::None, 0, 5, true, L"a\nabc"),
				StructCases(L"\\d+", L"abcd123efg", RegexOptions::None, 0, 10, true, L"123"),
				StructCases(L"\\d+\\d+", L"abcd123efg", RegexOptions::None, 0, 10, true, L"123"),
				StructCases(L"\\w+123\\w+", L"abcd123efg", RegexOptions::None, 0, 10, true, L"abcd123efg"),
				StructCases(L"\\d+\\w+", L"abcd123efg", RegexOptions::None, 0, 10, true, L"123efg"),
				StructCases(L"\\w+@\\w+.com", L"abc@def.com", RegexOptions::None, 0, 11, true, L"abc@def.com"),
				StructCases(L"\\w{3,}@\\w+.com", L"abc@def.com", RegexOptions::None, 0, 11, true, L"abc@def.com"),
				StructCases(L"\\w{4,}@\\w+.com", L"abc@def.com", RegexOptions::None, 0, 11, false, L""),
				StructCases(L"\\w{2,5}@\\w+.com", L"abc@def.com", RegexOptions::None, 0, 11, true, L"abc@def.com"),
				StructCases(L"\\w{3}@\\w+.com", L"abc@def.com", RegexOptions::None, 0, 11, true, L"abc@def.com"),
				StructCases(L"\\w{0,3}@\\w+.com", L"abc@def.com", RegexOptions::None, 0, 11, true, L"abc@def.com"),
				StructCases(L"\\w{0,2}c@\\w+.com", L"abc@def.com", RegexOptions::None, 0, 11, true, L"abc@def.com"),
				StructCases(L"\\w*@\\w+.com", L"abc@def.com", RegexOptions::None, 0, 11, true, L"abc@def.com"),
				StructCases(L"(\\w+)@\\w+.com", L"abc@def.com", RegexOptions::None, 0, 11, true, L"abc@def.com"),
				StructCases(L"((\\w+))@\\w+.com", L"abc@def.com", RegexOptions::None, 0, 11, true, L"abc@def.com"),
				StructCases(L"(\\w+)c@\\w+.com", L"abc@def.comabcdef", RegexOptions::None, 0, 17, true, L"abc@def.com"),
				StructCases(L"\\w+://\\w+\\.\\w+", L"test https://dot.net test", RegexOptions::None, 0, 25, true, L"https://dot.net"),
				StructCases(L"\\w+[:|$*&]//\\w+\\.\\w+", L"test https://dot.net test", RegexOptions::None, 0, 25, true, L"https://dot.net"),
				StructCases(L".+a", L"baa", RegexOptions::None, 0, 3, true, L"baa"),
				StructCases(L"[ab]+a", L"cacbaac", RegexOptions::None, 0, 7, true, L"baa"),
				StructCases(L"^(\\d{2,3}){2}$", L"1234", RegexOptions::None, 0, 4, true, L"1234"),
				StructCases(L"(\\d{2,3}){2}", L"1234", RegexOptions::None, 0, 4, true, L"1234"),
				StructCases(L"((\\d{2,3})){2}", L"1234", RegexOptions::None, 0, 4, true, L"1234"),
				StructCases(L"(abc\\d{2,3}){2}", L"abc123abc4567", RegexOptions::None, 0, 12, true, L"abc123abc456"),

				// Lazy versions of those loops
				StructCases(L"a+?", L"aaa", RegexOptions::None, 0, 3, true, L"a"),
				StructCases(L"a+?\\d+?", L"a1", RegexOptions::None, 0, 2, true, L"a1"),
				StructCases(L".+?\\d+?", L"a1", RegexOptions::None, 0, 2, true, L"a1"),
				StructCases(L".+?\\nabc", L"a\nabc", RegexOptions::None, 0, 5, true, L"a\nabc"),
				StructCases(L"\\d+?", L"abcd123efg", RegexOptions::None, 0, 10, true, L"1"),
				StructCases(L"\\d+?\\d+?", L"abcd123efg", RegexOptions::None, 0, 10, true, L"12"),
				StructCases(L"\\w+?123\\w+?", L"abcd123efg", RegexOptions::None, 0, 10, true, L"abcd123e"),
				StructCases(L"\\d+?\\w+?", L"abcd123efg", RegexOptions::None, 0, 10, true, L"12"),
				StructCases(L"\\w+?@\\w+?\\.com", L"abc@def.com", RegexOptions::None, 0, 11, true, L"abc@def.com"),
				StructCases(L"\\w{3,}?@\\w+?\\.com", L"abc@def.com", RegexOptions::None, 0, 11, true, L"abc@def.com"),
				StructCases(L"\\w{4,}?@\\w+?\\.com", L"abc@def.com", RegexOptions::None, 0, 11, false, L""),
				StructCases(L"\\w{2,5}?@\\w+?\\.com", L"abc@def.com", RegexOptions::None, 0, 11, true, L"abc@def.com"),
				StructCases(L"\\w{3}?@\\w+?\\.com", L"abc@def.com", RegexOptions::None, 0, 11, true, L"abc@def.com"),
				StructCases(L"\\w{0,3}?@\\w+?\\.com", L"abc@def.com", RegexOptions::None, 0, 11, true, L"abc@def.com"),
				StructCases(L"\\w{0,2}?c@\\w+?\\.com", L"abc@def.com", RegexOptions::None, 0, 11, true, L"abc@def.com"),
				StructCases(L"\\w*?@\\w+?\\.com", L"abc@def.com", RegexOptions::None, 0, 11, true, L"abc@def.com"),
				StructCases(L"(\\w+?)@\\w+?\\.com", L"abc@def.com", RegexOptions::None, 0, 11, true, L"abc@def.com"),
				StructCases(L"((\\w+?))@\\w+?\\.com", L"abc@def.com", RegexOptions::None, 0, 11, true, L"abc@def.com"),
				StructCases(L"(\\w+?)c@\\w+?\\.com", L"abc@def.comabcdef", RegexOptions::None, 0, 17, true, L"abc@def.com"),
				StructCases(L".+?a", L"baa", RegexOptions::None, 0, 3, true, L"ba"),
				StructCases(L"[ab]+?a", L"cacbaac", RegexOptions::None, 0, 7, true, L"ba"),
				StructCases(L"^(\\d{2,3}?){2}$", L"1234", RegexOptions::None, 0, 4, true, L"1234"),
				StructCases(L"(\\d{2,3}?){2}", L"1234", RegexOptions::None, 0, 4, true, L"1234"),
				StructCases(L"((\\d{2,3}?)){2}", L"1234", RegexOptions::None, 0, 4, true, L"1234"),
				StructCases(L"(abc\\d{2,3}?){2}", L"abc123abc4567", RegexOptions::None, 0, 12, true, L"abc123abc45"),
				StructCases(L"(b|a|aa)((?:aa)+?)+?$", L"aaaaaaaa", RegexOptions::None, 0, 8, true, L"aaaaaaaa"),
				StructCases(L"(|a|aa)(((?:aa)+?)+?|aaaaab)\\w$", L"aaaaaabc", RegexOptions::None, 0, 8, true, L"aaaaaabc"),


				// Testing selected FindOptimizations finds the right prefix
				StructCases(L"(^|a+)bc", L" aabc", RegexOptions::None, 0, 5, true, L"aabc"),
				StructCases(L"(^|($|a+))bc", L" aabc", RegexOptions::None, 0, 5, true, L"aabc"),
				StructCases(L"yz(^|a+)bc", L" yzaabc", RegexOptions::None, 0, 7, true, L"yzaabc"),
				StructCases(L"(^a|a$) bc", L"a bc", RegexOptions::None, 0, 4, true, L"a bc"),
				StructCases(L"(abcdefg|abcdef|abc|a)h", L"    ah  ", RegexOptions::None, 0, 8, true, L"ah"),
				StructCases(L"(^abcdefg|abcdef|^abc|a)h", L"    abcdefh  ", RegexOptions::None, 0, 13, true, L"abcdefh"),
				StructCases(L"(a|^abcdefg|abcdef|^abc)h", L"    abcdefh  ", RegexOptions::None, 0, 13, true, L"abcdefh"),
				StructCases(L"(abcdefg|abcdef)h", L"    abcdefghij  ", RegexOptions::None, 0, 16, true, L"abcdefgh"),

				// Back references not support with NonBacktracking

				StructCases(L"(\\w+)c@\\w+.com\\1", L"abc@def.comabcdef", RegexOptions::None, 0, 17, true, L"abc@def.comab"),
				StructCases(L"(\\w+)@def.com\\1", L"abc@def.comab", RegexOptions::None, 0, 13, false, L""),
				StructCases(L"(\\w+)@def.com\\1", L"abc@def.combc", RegexOptions::None, 0, 13, true, L"bc@def.combc"),
				StructCases(L"(\\w*)@def.com\\1", L"abc@def.com", RegexOptions::None, 0, 11, true, L"@def.com"),
				StructCases(L"\\w+(?<!a)", L"a", RegexOptions::None, 0, 1, false, L""),
				StructCases(L"\\w+(?<!a)", L"aa", RegexOptions::None, 0, 2, false, L""),
				StructCases(L"(?>\\w+)(?<!a)", L"a", RegexOptions::None, 0, 1, false, L""),
				StructCases(L"(?>\\w+)(?<!a)", L"aa", RegexOptions::None, 0, 2, false, L""),

				StructCases(L"(\\w+?)c@\\w+?.com\\1", L"abc@def.comabcdef", RegexOptions::None, 0, 17, true, L"abc@def.comab"),
				StructCases(L"(\\w+?)@def.com\\1", L"abc@def.comab", RegexOptions::None, 0, 13, false, L""),
				StructCases(L"(\\w+?)@def.com\\1", L"abc@def.combc", RegexOptions::None, 0, 13, true, L"bc@def.combc"),
				StructCases(L"(\\w*?)@def.com\\1", L"abc@def.com", RegexOptions::None, 0, 11, true, L"@def.com"),
				StructCases(L"\\w+?(?<!a)", L"a", RegexOptions::None, 0, 1, false, L""),
				StructCases(L"\\w+?(?<!a)", L"aa", RegexOptions::None, 0, 2, false, L""),
				StructCases(L"(?>\\w+?)(?<!a)", L"a", RegexOptions::None, 0, 1, false, L""),
				StructCases(L"(?>\\w+?)(?<!a)", L"aa", RegexOptions::None, 0, 2, false, L""),

				StructCases(L"(\\d{2,3})+", L"1234", RegexOptions::None, 0, 4, true, L"123"),
				StructCases(L"(\\d{2,3})*", L"123456", RegexOptions::None, 0, 4, true, L"123"),
				StructCases(L"(\\d{2,3})+?", L"1234", RegexOptions::None, 0, 4, true, L"123"),
				StructCases(L"(\\d{2,3})*?", L"123456", RegexOptions::None, 0, 4, true, L""),
				StructCases(L"(\\d{2,3}?)+", L"1234", RegexOptions::None, 0, 4, true, L"1234"),
				StructCases(L"(\\d{2,3}?)*", L"123456", RegexOptions::None, 0, 4, true, L"1234"),
				StructCases(L"(\\d{2,3}?)+?", L"1234", RegexOptions::None, 0, 4, true, L"12"),
				StructCases(L"(\\d{2,3}?)*?", L"123456", RegexOptions::None, 0, 4, true, L""),


				StructCases(L".*", L"abc", RegexOptions::None, 1, 2, true, L"bc"),
				StructCases(L".*c", L"abc", RegexOptions::None, 1, 2, true, L"bc"),
				StructCases(L"b.*", L"abc", RegexOptions::None, 1, 2, true, L"bc"),
				StructCases(L".*", L"abc", RegexOptions::None, 2, 1, true, L"c"),

				StructCases(L"a.*[bc]", L"xyza12345b6789", RegexOptions::None, 0, 14, true, L"a12345b"),
				StructCases(L"a.*[bc]", L"xyza12345c6789", RegexOptions::None, 0, 14, true, L"a12345c"),
				StructCases(L"a.*[bc]", L"xyza12345d6789", RegexOptions::None, 0, 14, false, L""),

				StructCases(L"a.*[bcd]", L"xyza12345b6789", RegexOptions::None, 0, 14, true, L"a12345b"),
				StructCases(L"a.*[bcd]", L"xyza12345c6789", RegexOptions::None, 0, 14, true, L"a12345c"),
				StructCases(L"a.*[bcd]", L"xyza12345d6789", RegexOptions::None, 0, 14, true, L"a12345d"),
				StructCases(L"a.*[bcd]", L"xyza12345e6789", RegexOptions::None, 0, 14, false, L""),

				StructCases(L"a.*[bcde]", L"xyza12345b6789", RegexOptions::None, 0, 14, true, L"a12345b"),
				StructCases(L"a.*[bcde]", L"xyza12345c6789", RegexOptions::None, 0, 14, true, L"a12345c"),
				StructCases(L"a.*[bcde]", L"xyza12345d6789", RegexOptions::None, 0, 14, true, L"a12345d"),
				StructCases(L"a.*[bcde]", L"xyza12345e6789", RegexOptions::None, 0, 14, true, L"a12345e"),
				StructCases(L"a.*[bcde]", L"xyza12345f6789", RegexOptions::None, 0, 14, false, L""),

				StructCases(L"a.*[bcdef]", L"xyza12345b6789", RegexOptions::None, 0, 14, true, L"a12345b"),
				StructCases(L"a.*[bcdef]", L"xyza12345c6789", RegexOptions::None, 0, 14, true, L"a12345c"),
				StructCases(L"a.*[bcdef]", L"xyza12345d6789", RegexOptions::None, 0, 14, true, L"a12345d"),
				StructCases(L"a.*[bcdef]", L"xyza12345e6789", RegexOptions::None, 0, 14, true, L"a12345e"),
				StructCases(L"a.*[bcdef]", L"xyza12345f6789", RegexOptions::None, 0, 14, true, L"a12345f"),
				StructCases(L"a.*[bcdef]", L"xyza12345g6789", RegexOptions::None, 0, 14, false, L""),

				StructCases(L".*?", L"abc", RegexOptions::None, 1, 2, true, L""),
				StructCases(L".*?c", L"abc", RegexOptions::None, 1, 2, true, L"bc"),
				StructCases(L"b.*?", L"abc", RegexOptions::None, 1, 2, true, L"b"),
				StructCases(L".*?", L"abc", RegexOptions::None, 2, 1, true, L""),

				StructCases(L"a.*?[bc]", L"xyza12345b6789", RegexOptions::None, 0, 14, true, L"a12345b"),
				StructCases(L"a.*?[bc]", L"xyza12345c6789", RegexOptions::None, 0, 14, true, L"a12345c"),
				StructCases(L"a.*?[bc]", L"xyza12345d6789", RegexOptions::None, 0, 14, false, L""),

				StructCases(L"a.*?[bcd]", L"xyza12345b6789", RegexOptions::None, 0, 14, true, L"a12345b"),
				StructCases(L"a.*?[bcd]", L"xyza12345c6789", RegexOptions::None, 0, 14, true, L"a12345c"),
				StructCases(L"a.*?[bcd]", L"xyza12345d6789", RegexOptions::None, 0, 14, true, L"a12345d"),
				StructCases(L"a.*?[bcd]", L"xyza12345e6789", RegexOptions::None, 0, 14, false, L""),

				StructCases(L"a.*?[bcde]", L"xyza12345b6789", RegexOptions::None, 0, 14, true, L"a12345b"),
				StructCases(L"a.*?[bcde]", L"xyza12345c6789", RegexOptions::None, 0, 14, true, L"a12345c"),
				StructCases(L"a.*?[bcde]", L"xyza12345d6789", RegexOptions::None, 0, 14, true, L"a12345d"),
				StructCases(L"a.*?[bcde]", L"xyza12345e6789", RegexOptions::None, 0, 14, true, L"a12345e"),
				StructCases(L"a.*?[bcde]", L"xyza12345f6789", RegexOptions::None, 0, 14, false, L""),

				StructCases(L"a.*?[bcdef]", L"xyza12345b6789", RegexOptions::None, 0, 14, true, L"a12345b"),
				StructCases(L"a.*?[bcdef]", L"xyza12345c6789", RegexOptions::None, 0, 14, true, L"a12345c"),
				StructCases(L"a.*?[bcdef]", L"xyza12345d6789", RegexOptions::None, 0, 14, true, L"a12345d"),
				StructCases(L"a.*?[bcdef]", L"xyza12345e6789", RegexOptions::None, 0, 14, true, L"a12345e"),
				StructCases(L"a.*?[bcdef]", L"xyza12345f6789", RegexOptions::None, 0, 14, true, L"a12345f"),
				StructCases(L"a.*?[bcdef]", L"xyza12345g6789", RegexOptions::None, 0, 14, false, L""),

				StructCases(L".*", L"abc", RegexOptions::Singleline, 1, 2, true, L"bc"),
				StructCases(L".*c", L"abc", RegexOptions::Singleline, 1, 2, true, L"bc"),
				StructCases(L"b.*", L"abc", RegexOptions::Singleline, 1, 2, true, L"bc"),
				StructCases(L".*", L"abc", RegexOptions::Singleline, 2, 1, true, L"c"),

				StructCases(L"a.*[bc]", L"xyza12345b6789", RegexOptions::Singleline, 0, 14, true, L"a12345b"),
				StructCases(L"a.*[bc]", L"xyza12345c6789", RegexOptions::Singleline, 0, 14, true, L"a12345c"),
				StructCases(L"a.*[bc]", L"xyza12345d6789", RegexOptions::Singleline, 0, 14, false, L""),

				StructCases(L"a.*[bcd]", L"xyza12345b6789", RegexOptions::Singleline, 0, 14, true, L"a12345b"),
				StructCases(L"a.*[bcd]", L"xyza12345c6789", RegexOptions::Singleline, 0, 14, true, L"a12345c"),
				StructCases(L"a.*[bcd]", L"xyza12345d6789", RegexOptions::Singleline, 0, 14, true, L"a12345d"),
				StructCases(L"a.*[bcd]", L"xyza12345e6789", RegexOptions::Singleline, 0, 14, false, L""),

				StructCases(L"a.*[bcde]", L"xyza12345b6789", RegexOptions::Singleline, 0, 14, true, L"a12345b"),
				StructCases(L"a.*[bcde]", L"xyza12345c6789", RegexOptions::Singleline, 0, 14, true, L"a12345c"),
				StructCases(L"a.*[bcde]", L"xyza12345d6789", RegexOptions::Singleline, 0, 14, true, L"a12345d"),
				StructCases(L"a.*[bcde]", L"xyza12345e6789", RegexOptions::Singleline, 0, 14, true, L"a12345e"),
				StructCases(L"a.*[bcde]", L"xyza12345f6789", RegexOptions::Singleline, 0, 14, false, L""),

				StructCases(L"a.*[bcdef]", L"xyza12345b6789", RegexOptions::Singleline, 0, 14, true, L"a12345b"),
				StructCases(L"a.*[bcdef]", L"xyza12345c6789", RegexOptions::Singleline, 0, 14, true, L"a12345c"),
				StructCases(L"a.*[bcdef]", L"xyza12345d6789", RegexOptions::Singleline, 0, 14, true, L"a12345d"),
				StructCases(L"a.*[bcdef]", L"xyza12345e6789", RegexOptions::Singleline, 0, 14, true, L"a12345e"),
				StructCases(L"a.*[bcdef]", L"xyza12345f6789", RegexOptions::Singleline, 0, 14, true, L"a12345f"),
				StructCases(L"a.*[bcdef]", L"xyza12345g6789", RegexOptions::Singleline, 0, 14, false, L""),

				StructCases(L".*?", L"abc", RegexOptions::Singleline, 1, 2, true, L""),
				StructCases(L".*?c", L"abc", RegexOptions::Singleline, 1, 2, true, L"bc"),
				StructCases(L"b.*?", L"abc", RegexOptions::Singleline, 1, 2, true, L"b"),
				StructCases(L".*?", L"abc", RegexOptions::Singleline, 2, 1, true, L""),

				StructCases(L"a.*?[bc]", L"xyza12345b6789", RegexOptions::Singleline, 0, 14, true, L"a12345b"),
				StructCases(L"a.*?[bc]", L"xyza12345c6789", RegexOptions::Singleline, 0, 14, true, L"a12345c"),
				StructCases(L"a.*?[bc]", L"xyza12345d6789", RegexOptions::Singleline, 0, 14, false, L""),

				StructCases(L"a.*?[bcd]", L"xyza12345b6789", RegexOptions::Singleline, 0, 14, true, L"a12345b"),
				StructCases(L"a.*?[bcd]", L"xyza12345c6789", RegexOptions::Singleline, 0, 14, true, L"a12345c"),
				StructCases(L"a.*?[bcd]", L"xyza12345d6789", RegexOptions::Singleline, 0, 14, true, L"a12345d"),
				StructCases(L"a.*?[bcd]", L"xyza12345e6789", RegexOptions::Singleline, 0, 14, false, L""),

				StructCases(L"a.*?[bcde]", L"xyza12345b6789", RegexOptions::Singleline, 0, 14, true, L"a12345b"),
				StructCases(L"a.*?[bcde]", L"xyza12345c6789", RegexOptions::Singleline, 0, 14, true, L"a12345c"),
				StructCases(L"a.*?[bcde]", L"xyza12345d6789", RegexOptions::Singleline, 0, 14, true, L"a12345d"),
				StructCases(L"a.*?[bcde]", L"xyza12345e6789", RegexOptions::Singleline, 0, 14, true, L"a12345e"),
				StructCases(L"a.*?[bcde]", L"xyza12345f6789", RegexOptions::Singleline, 0, 14, false, L""),

				StructCases(L"a.*?[bcdef]", L"xyza12345b6789", RegexOptions::Singleline, 0, 14, true, L"a12345b"),
				StructCases(L"a.*?[bcdef]", L"xyza12345c6789", RegexOptions::Singleline, 0, 14, true, L"a12345c"),
				StructCases(L"a.*?[bcdef]", L"xyza12345d6789", RegexOptions::Singleline, 0, 14, true, L"a12345d"),
				StructCases(L"a.*?[bcdef]", L"xyza12345e6789", RegexOptions::Singleline, 0, 14, true, L"a12345e"),
				StructCases(L"a.*?[bcdef]", L"xyza12345f6789", RegexOptions::Singleline, 0, 14, true, L"a12345f"),
				StructCases(L"a.*?[bcdef]", L"xyza12345g6789", RegexOptions::Singleline, 0, 14, false, L""),

				// Nested loops
				StructCases(L"a*(?:a[ab]*)*", L"aaaababbbbbbabababababaaabbb", RegexOptions::None, 0, 28, true, L"aaaa"),
				StructCases(L"a*(?:a[ab]*?)*?", L"aaaababbbbbbabababababaaabbb", RegexOptions::None, 0, 28, true, L"aaaa"),

				// Using beginning/end of string chars \A, \Z: Actual - "\\Aaaa\\w+zzz\\Z"
				StructCases(L"\\Aaaa\\w+zzz\\Z", L"aaaasdfajsdlfjzzz", RegexOptions::IgnoreCase, 0, 17, true, L"aaaasdfajsdlfjzzz"),
				StructCases(L"\\Aaaaaa\\w+zzz\\Z", L"aaaa", RegexOptions::IgnoreCase, 0, 4, false, L""),

				StructCases(L"\\Aaaaaa\\w+zzz\\Z", L"aaaa", RegexOptions::RightToLeft, 0, 4, false, L""),
				StructCases(L"\\Aaaaaa\\w+zzzzz\\Z", L"aaaa", RegexOptions::RightToLeft, 0, 4, false, L""),
				StructCases(L"\\Aaaaaa\\w+zzz\\Z", L"aaaa", (RegexOptions)((int)RegexOptions::RightToLeft | (int)RegexOptions::IgnoreCase), 0, 4, false, L""),

				StructCases(L"abc\\Adef", L"abcdef", RegexOptions::None, 0, 0, false, L""),
				StructCases(L"abc\\adef", L"abcdef", RegexOptions::None, 0, 0, false, L""),

				StructCases(L"abc\\Gdef", L"abcdef", RegexOptions::None, 0, 0, false, L""),

				StructCases(L"abc^def", L"abcdef", RegexOptions::None, 0, 0, false, L""),
				StructCases(L"abc\\Zef", L"abcdef", RegexOptions::None, 0, 0, false, L""),
				StructCases(L"abc\\zef", L"abcdef", RegexOptions::None, 0, 0, false, L""),

				// Using beginning/end of string chars \A, \Z: Actual - "\\Aaaa\\w+zzz\\Z"
				StructCases(L"\\Aaaa\\w+zzz\\Z", L"aaaasdfajsdlfjzzza", RegexOptions::None, 0, 18, false, L""),

				// Anchors
				StructCases(L"^abc", L"abc", RegexOptions::None, 0, 3, true, L"abc"),
				StructCases(L"^abc", L" abc", RegexOptions::None, 0, 4, false, L""),
				StructCases(L"^abc|^def", L"def", RegexOptions::None, 0, 3, true, L"def"),
				StructCases(L"^abc|^def", L" abc", RegexOptions::None, 0, 4, false, L""),
				StructCases(L"^abc|^def", L" def", RegexOptions::None, 0, 4, false, L""),
				StructCases(L"abc|^def", L" abc", RegexOptions::None, 0, 4, true, L"abc"),
				StructCases(L"abc|^def|^efg", L" abc", RegexOptions::None, 0, 4, true, L"abc"),
				StructCases(L"^abc|def|^efg", L" def", RegexOptions::None, 0, 4, true, L"def"),
				StructCases(L"^abc|def", L" def", RegexOptions::None, 0, 4, true, L"def"),
				StructCases(L"abcd$", L"1234567890abcd", RegexOptions::None, 0, 14, true, L"abcd"),
				StructCases(L"abc{1,4}d$", L"1234567890abcd", RegexOptions::None, 0, 14, true, L"abcd"),
				StructCases(L"abc{1,4}d$", L"1234567890abccccd", RegexOptions::None, 0, 17, true, L"abccccd"),


				StructCases(L"^abc", L"abc", RegexOptions::Multiline, 0, 3, true, L"abc"),
				StructCases(L"^abc", L" abc", RegexOptions::Multiline, 0, 4, false, L""),
				StructCases(L"^abc|^def", L"def", RegexOptions::Multiline, 0, 3, true, L"def"),
				StructCases(L"^abc|^def", L" abc", RegexOptions::Multiline, 0, 4, false, L""),
				StructCases(L"^abc|^def", L" def", RegexOptions::Multiline, 0, 4, false, L""),
				StructCases(L"abc|^def", L" abc", RegexOptions::Multiline, 0, 4, true, L"abc"),
				StructCases(L"abc|^def|^efg", L" abc", RegexOptions::Multiline, 0, 4, true, L"abc"),
				StructCases(L"^abc|def|^efg", L" def", RegexOptions::Multiline, 0, 4, true, L"def"),
				StructCases(L"^abc|def", L" def", RegexOptions::Multiline, 0, 4, true, L"def"),
				StructCases(L"abcd$", L"1234567890abcd", RegexOptions::Multiline, 0, 14, true, L"abcd"),
				StructCases(L"abc{1,4}d$", L"1234567890abcd", RegexOptions::Multiline, 0, 14, true, L"abcd"),
				StructCases(L"abc{1,4}d$", L"1234567890abccccd", RegexOptions::Multiline, 0, 17, true, L"abccccd"),


				StructCases(L"\\Gabc", L"abc", RegexOptions::None, 0, 3, true, L"abc"),
				StructCases(L"\\Gabc", L" abc", RegexOptions::None, 0, 4, false, L""),
				StructCases(L"\\Gabc", L" abc", RegexOptions::None, 1, 3, true, L"abc"),
				StructCases(L"\\Gabc|\\Gdef", L"def", RegexOptions::None, 0, 3, true, L"def"),
				StructCases(L"\\Gabc|\\Gdef", L" abc", RegexOptions::None, 0, 4, false, L""),
				StructCases(L"\\Gabc|\\Gdef", L" def", RegexOptions::None, 0, 4, false, L""),
				StructCases(L"\\Gabc|\\Gdef", L" abc", RegexOptions::None, 1, 3, true, L"abc"),
				StructCases(L"\\Gabc|\\Gdef", L" def", RegexOptions::None, 1, 3, true, L"def"),
				StructCases(L"abc|\\Gdef", L" abc", RegexOptions::None, 0, 4, true, L"abc"),
				StructCases(L"\\Gabc|def", L" def", RegexOptions::None, 0, 4, true, L"def"),

				// Anchors and multiline
				StructCases(L"^A$", L"A\n", RegexOptions::Multiline, 0, 2, true, L"A"),
				StructCases(L"^A$", L"ABC\n", RegexOptions::Multiline, 0, 4, false, L""),
				StructCases(L"^A$", L"123\nA", RegexOptions::Multiline, 0, 5, true, L"A"),
				StructCases(L"^A$", L"123\nA\n456", RegexOptions::Multiline, 0, 9, true, L"A"),
				StructCases(L"^A$|^B$", L"123\nB\n456", RegexOptions::Multiline, 0, 9, true, L"B"),

				// Using beginning/end of string chars \A, \Z: Actual - "\\Aaaa\\w+zzz\\Z"
				StructCases(L"\\A(line2\n)line3\\Z", L"line2\nline3\n", RegexOptions::Multiline, 0, 12, true, L"line2\nline3"),

				// Using beginning/end of string chars ^: Actual - "^b"
				StructCases(L"^b", L"ab", RegexOptions::None, 0, 2, false, L""),

				// Actual - "(?<char>\\w)\\<char>"
				StructCases(L"(?<char>\\w)\\<char>", L"aa", RegexOptions::None, 0, 2, true, L"aa"),

				// Actual - "(?<43>\\w)\\43"
				StructCases(L"(?<43>\\w)\\43", L"aa", RegexOptions::None, 0, 2, true, L"aa"),

				// Actual - "abc(?(1)111|222)"
				StructCases(L"(abbc)(?(1)111|222)", L"abbc222", RegexOptions::None, 0, 7, false, L""),

				// "x" option. Removes unescaped whitespace from the pattern: Actual - " ([^/]+) ","x"
				StructCases(L"            ((.)+) #comment     ", L"abc", RegexOptions::IgnorePatternWhitespace, 0, 3, true, L"abc"),

				// "x" option. Removes unescaped whitespace from the pattern. : Actual - "\x20([^/]+)\x20","x"
				StructCases(L"\x20([^/]+)\x20\x20\x20\x20\x20\x20\x20", L" abc       ", RegexOptions::IgnorePatternWhitespace, 0, 10, true, L" abc      "),


				StructCases(L"(?i:a)b(?i:c)d", L"aaaaAbCdddd", RegexOptions::None, 0, 11, true, L"AbCd"),
				StructCases(L"(?i:[" + UtilsString::StringInitSpe(0x0000) + L"-" + UtilsString::StringInitSpe(0x1000) + L"])[Bb]", L"aaaaAbCdddd", RegexOptions::None, 0, 11, true, L"Ab"),

				// Turning off case insensitive option in mid-pattern : Actual - "aaa(?-i:match this)bbb", L"i"
				StructCases(L"aAa(?-i:match this)bbb", L"AaAmatch thisBBb", RegexOptions::IgnoreCase, 0, 16, true, L"AaAmatch thisBBb"),

				// Turning on/off all the options at once : Actual - "aaa(?imnsx-imnsx:match this)bbb", L"i"
				StructCases(L"aaa(?imnsx-imnsx:match this)bbb", L"AaAmatcH thisBBb", RegexOptions::IgnoreCase, 0, 16, false, L""),

				// Actual - "aaa(?#ignore this completely)bbb"
				StructCases(L"aAa(?#ignore this completely)bbb", L"aAabbb", RegexOptions::None, 0, 6, true, L"aAabbb"),

				// Trying empty string: Actual "[a-z0-9]+", L""
				StructCases(L"[a-z0-9]+", L"", RegexOptions::None, 0, 0, false, L""),

				// Numbering pattern slots: "(?<1>\\d{3})(?<2>\\d{3})(?<3>\\d{4})"
				StructCases(L"(?<1>\\d{3})(?<2>\\d{3})(?<3>\\d{4})", L"8885551111", RegexOptions::None, 0, 10, true, L"8885551111"),
				StructCases(L"(?<1>\\d{3})(?<2>\\d{3})(?<3>\\d{4})", L"Invalid string", RegexOptions::None, 0, 14, false, L""),

				// Not naming pattern slots at all: "^(cat|chat)"
				StructCases(L"^(cat|chat)", L"cats are bad", RegexOptions::None, 0, 12, true, L"cat"),

				StructCases(L"abc", L"abc", RegexOptions::None, 0, 3, true, L"abc"),
				StructCases(L"abc", L"aBc", RegexOptions::None, 0, 3, false, L""),
				StructCases(L"abc", L"aBc", RegexOptions::IgnoreCase, 0, 3, true, L"aBc"),
				StructCases(L"abc.*def", L"abcghiDEF", RegexOptions::IgnoreCase, 0, 9, true, L"abcghiDEF"),

				// Using *, +, ?, {}: Actual - "a+\\.?b*\\.?c{2}"
				StructCases(L"a+\\.?b*\\.+c{2}", L"ab.cc", RegexOptions::None, 0, 5, true, L"ab.cc"),
				StructCases(L"[^a]+\\.[^z]+", L"zzzzz", RegexOptions::None, 0, 5, false, L""),

				// IgnoreCase
				StructCases(L"AAA", L"aaabbb", RegexOptions::IgnoreCase, 0, 6, true, L"aaa"),
				StructCases(L"\\p{Lu}", L"1bc", RegexOptions::IgnoreCase, 0, 3, true, L"b"),
				StructCases(L"\\p{Ll}", L"1bc", RegexOptions::IgnoreCase, 0, 3, true, L"b"),
				StructCases(L"\\p{Lt}", L"1bc", RegexOptions::IgnoreCase, 0, 3, true, L"b"),
				StructCases(L"\\p{Lo}", L"1bc", RegexOptions::IgnoreCase, 0, 3, false, L""),
				StructCases(L".[abc]", L"xYZAbC", RegexOptions::IgnoreCase, 0, 6, true, L"ZA"),
				StructCases(L".[abc]", L"xYzXyZx", RegexOptions::IgnoreCase, 0, 6, false, L""),

				// Sets containing characters that differ by a bit
				StructCases(L"123[Aa]", L"123a", RegexOptions::None, 0, 4, true, L"123a"),
				StructCases(L"123[0p]", L"123p", RegexOptions::None, 0, 4, true, L"123p"),
				StructCases(L"123[Aa@]", L"123@", RegexOptions::None, 0, 4, true, L"123@"),

				// "\D+"
				StructCases(L"\\D+", L"12321", RegexOptions::None, 0, 5, false, L""),

				// Groups
				StructCases(L"(?<first_name>\\S+)\\s(?<last_name>\\S+)", L"David Bau", RegexOptions::None, 0, 9, true, L"David Bau"),

				// "^b"
				StructCases(L"^b", L"abc", RegexOptions::None, 0, 3, false, L""),

				// Trim leading and trailing whitespace
				StructCases(L"\\s*(.*?)\\s*$", L" Hello World ", RegexOptions::None, 0, 13, true, L" Hello World "),

				// Throws NotSupported with NonBacktracking engine because of the balancing group dog-0
				StructCases(L"(?<cat>cat)\\w+(?<dog-0>dog)", L"cat_Hello_World_dog", RegexOptions::None, 0, 19, false, L""),

				// Atomic Zero-Width Assertions \A \Z \z \b \B
				StructCases(L"\\A(cat)\\s+(dog)", L"cat   \n\n\ncat     dog", RegexOptions::None, 0, 20, false, L""),
				StructCases(L"\\A(cat)\\s+(dog)", L"cat   \n\n\ncat     dog", RegexOptions::Multiline, 0, 20, false, L""),
				StructCases(L"\\A(cat)\\s+(dog)", L"cat   \n\n\ncat     dog", RegexOptions::ECMAScript, 0, 20, false, L""),

				StructCases(L"(cat)\\s+(dog)\\Z", L"cat   dog\n\n\ncat", RegexOptions::None, 0, 15, false, L""),
				StructCases(L"(cat)\\s+(dog)\\Z", L"cat   dog\n\n\ncat     ", RegexOptions::Multiline, 0, 20, false, L""),
				StructCases(L"(cat)\\s+(dog)\\Z", L"cat   dog\n\n\ncat     ", RegexOptions::ECMAScript, 0, 20, false, L""),

				StructCases(L"(cat)\\s+(dog)\\z", L"cat   dog\n\n\ncat", RegexOptions::None, 0, 15, false, L""),
				StructCases(L"(cat)\\s+(dog)\\z", L"cat   dog\n\n\ncat     ", RegexOptions::Multiline, 0, 20, false, L""),
				StructCases(L"(cat)\\s+(dog)\\z", L"cat   dog\n\n\ncat     ", RegexOptions::ECMAScript, 0, 20, false, L""),
				StructCases(L"(cat)\\s+(dog)\\z", L"cat   \n\n\n   dog\n", RegexOptions::None, 0, 16, false, L""),
				StructCases(L"(cat)\\s+(dog)\\z", L"cat   \n\n\n   dog\n", RegexOptions::Multiline, 0, 16, false, L""),
				StructCases(L"(cat)\\s+(dog)\\z", L"cat   \n\n\n   dog\n", RegexOptions::ECMAScript, 0, 16, false, L""),

				StructCases(L"\\b@cat", L"123START123;@catEND", RegexOptions::None, 0, 19, false, L""),
				StructCases(L"\\b<cat", L"123START123'<catEND", RegexOptions::None, 0, 19, false, L""),
				StructCases(L"\\b,cat", L"satwe,,,START',catEND", RegexOptions::None, 0, 21, false, L""),
				StructCases(L"\\b\\[cat", L"`12START123'[catEND", RegexOptions::None, 0, 19, false, L""),

				StructCases(L"\\B@cat", L"123START123@catEND", RegexOptions::None, 0, 18, false, L""),
				StructCases(L"\\B<cat", L"123START123<catEND", RegexOptions::None, 0, 18, false, L""),
				StructCases(L"\\B,cat", L"satwe,,,START,catEND", RegexOptions::None, 0, 20, false, L""),
				StructCases(L"\\B\\[cat", L"`12START123[catEND", RegexOptions::None, 0, 18, false, L""),

				// Lazy operator Backtracking
				StructCases(L"http://([a-zA-z0-9\\-]*\\.?)*?(:[0-9]*)??/", L"http://www.msn.com", RegexOptions::IgnoreCase, 0, 18, false, L""),

				// Grouping Constructs Invalid Regular Expressions
				StructCases(L"(?!)", L"(?!)cat", RegexOptions::None, 0, 7, false, L""),
				StructCases(L"(?<!)", L"(?<!)cat", RegexOptions::None, 0, 8, false, L""),


				StructCases(L"[^a-z0-9]etag|[^a-z0-9]digest", L"this string has .digest as a substring", RegexOptions::None, 16, 7, true, L".digest"),
				StructCases(L"(\\w+|\\d+)a+[ab]+", L"123123aa", RegexOptions::None, 0, 8, true, L"123123aa"),


				//not support

				StructCases(L"(?(dog2))", L"dog2", RegexOptions::None, 0, 4, true, L""),
				StructCases(L"(?(a:b))", L"a", RegexOptions::None, 0, 1, true, L""),
				StructCases(L"(?(a:))", L"a", RegexOptions::None, 0, 1, true, L""),
				StructCases(L"(?(cat)cat|dog)", L"cat", RegexOptions::None, 0, 3, true, L"cat"),
				StructCases(L"(?((?=cat))cat|dog)", L"cat", RegexOptions::None, 0, 3, true, L"cat"),
				StructCases(L"(?(cat)|dog)", L"cat", RegexOptions::None, 0, 3, true, L""),
				StructCases(L"(?(cat)|dog)", L"catdog", RegexOptions::None, 0, 6, true, L""),
				StructCases(L"(?(cat)|dog)", L"oof", RegexOptions::None, 0, 3, false, L""),
				StructCases(L"(?(cat)dog1|dog2)", L"catdog1", RegexOptions::None, 0, 7, false, L""),
				StructCases(L"(?(cat)dog1|dog2)", L"catdog2", RegexOptions::None, 0, 7, true, L"dog2"),
				StructCases(L"(?(cat)dog1|dog2)", L"catdog1dog2", RegexOptions::None, 0, 11, true, L"dog2"),
				StructCases(L"(?(\\w+)\\w+)dog", L"catdog", RegexOptions::None, 0, 6, true, L"catdog"),
				StructCases(L"(?(abc)\\w+|\\w{0,2})dog", L"catdog", RegexOptions::None, 0, 6, true, L"atdog"),
				StructCases(L"(?(abc)cat|\\w{0,2})dog", L"catdog", RegexOptions::None, 0, 6, true, L"atdog"),
				StructCases(L"(a|ab|abc|abcd)d", L"abcd", RegexOptions::RightToLeft, 0, 4, true, L"abcd"),
				StructCases(L"(?>(?:a|ab|abc|abcd))d", L"abcd", RegexOptions::None, 0, 4, false, L""),
				StructCases(L"(?>(?:a|ab|abc|abcd))d", L"abcd", RegexOptions::RightToLeft, 0, 4, true, L"abcd"),

				// No Negation
				StructCases(L"[abcd-[abcd]]+", L"abcxyzABCXYZ`!@#$%^&*()_-+= \t\n", RegexOptions::None, 0, 30, false, L""),
				StructCases(L"[1234-[1234]]+", L"abcxyzABCXYZ`!@#$%^&*()_-+= \t\n", RegexOptions::None, 0, 30, false, L""),

				// All Negation
				StructCases(L"[^abcd-[^abcd]]+", L"abcxyzABCXYZ`!@#$%^&*()_-+= \t\n", RegexOptions::None, 0, 30, false, L""),
				StructCases(L"[^1234-[^1234]]+", L"abcxyzABCXYZ`!@#$%^&*()_-+= \t\n", RegexOptions::None, 0, 30, false, L""),

				// No Negation
				StructCases(L"[a-z-[a-z]]+", L"abcxyzABCXYZ`!@#$%^&*()_-+= \t\n", RegexOptions::None, 0, 30, false, L""),
				StructCases(L"[0-9-[0-9]]+", L"abcxyzABCXYZ`!@#$%^&*()_-+= \t\n", RegexOptions::None, 0, 30, false, L""),

				// All Negation
				StructCases(L"[^a-z-[^a-z]]+", L"abcxyzABCXYZ`!@#$%^&*()_-+= \t\n", RegexOptions::None, 0, 30, false, L""),
				StructCases(L"[^0-9-[^0-9]]+", L"abcxyzABCXYZ`!@#$%^&*()_-+= \t\n", RegexOptions::None, 0, 30, false, L""),

				// No Negation
				StructCases(L"[\\w-[\\w]]+", L"abcxyzABCXYZ`!@#$%^&*()_-+= \t\n", RegexOptions::None, 0, 30, false, L""),
				StructCases(L"[\\W-[\\W]]+", L"abcxyzABCXYZ`!@#$%^&*()_-+= \t\n", RegexOptions::None, 0, 30, false, L""),
				StructCases(L"[\\s-[\\s]]+", L"abcxyzABCXYZ`!@#$%^&*()_-+= \t\n", RegexOptions::None, 0, 30, false, L""),
				StructCases(L"[\\S-[\\S]]+", L"abcxyzABCXYZ`!@#$%^&*()_-+= \t\n", RegexOptions::None, 0, 30, false, L""),
				StructCases(L"[\\d-[\\d]]+", L"abcxyzABCXYZ`!@#$%^&*()_-+= \t\n", RegexOptions::None, 0, 30, false, L""),
				StructCases(L"[\\D-[\\D]]+", L"abcxyzABCXYZ`!@#$%^&*()_-+= \t\n", RegexOptions::None, 0, 30, false, L""),

				// All Negation
				StructCases(L"[^\\w-[^\\w]]+", L"abcxyzABCXYZ`!@#$%^&*()_-+= \t\n", RegexOptions::None, 0, 30, false, L""),
				StructCases(L"[^\\W-[^\\W]]+", L"abcxyzABCXYZ`!@#$%^&*()_-+= \t\n", RegexOptions::None, 0, 30, false, L""),
				StructCases(L"[^\\s-[^\\s]]+", L"abcxyzABCXYZ`!@#$%^&*()_-+= \t\n", RegexOptions::None, 0, 30, false, L""),
				StructCases(L"[^\\S-[^\\S]]+", L"abcxyzABCXYZ`!@#$%^&*()_-+= \t\n", RegexOptions::None, 0, 30, false, L""),
				StructCases(L"[^\\d-[^\\d]]+", L"abcxyzABCXYZ`!@#$%^&*()_-+= \t\n", RegexOptions::None, 0, 30, false, L""),
				StructCases(L"[^\\D-[^\\D]]+", L"abcxyzABCXYZ`!@#$%^&*()_-+= \t\n", RegexOptions::None, 0, 30, false, L""),

				// MixedNegation
				StructCases(L"[^\\w-[\\W]]+", L"abcxyzABCXYZ`!@#$%^&*()_-+= \t\n", RegexOptions::None, 0, 30, false, L""),
				StructCases(L"[\\w-[^\\W]]+", L"abcxyzABCXYZ`!@#$%^&*()_-+= \t\n", RegexOptions::None, 0, 30, false, L""),
				StructCases(L"[^\\s-[\\S]]+", L"abcxyzABCXYZ`!@#$%^&*()_-+= \t\n", RegexOptions::None, 0, 30, false, L""),
				StructCases(L"[\\s-[^\\S]]+", L"abcxyzABCXYZ`!@#$%^&*()_-+= \t\n", RegexOptions::None, 0, 30, false, L""),
				StructCases(L"[^\\d-[\\D]]+", L"abcxyzABCXYZ`!@#$%^&*()_-+= \t\n", RegexOptions::None, 0, 30, false, L""),
				StructCases(L"[\\d-[^\\D]]+", L"abcxyzABCXYZ`!@#$%^&*()_-+= \t\n", RegexOptions::None, 0, 30, false, L""),

				// No Negation
				StructCases(L"[\\p{Ll}-[\\p{Ll}]]+", L"abcxyzABCXYZ`!@#$%^&*()_-+= \t\n", RegexOptions::None, 0, 30, false, L""),
				StructCases(L"[\\P{Ll}-[\\P{Ll}]]+", L"abcxyzABCXYZ`!@#$%^&*()_-+= \t\n", RegexOptions::None, 0, 30, false, L""),
				StructCases(L"[\\p{Lu}-[\\p{Lu}]]+", L"abcxyzABCXYZ`!@#$%^&*()_-+= \t\n", RegexOptions::None, 0, 30, false, L""),
				StructCases(L"[\\P{Lu}-[\\P{Lu}]]+", L"abcxyzABCXYZ`!@#$%^&*()_-+= \t\n", RegexOptions::None, 0, 30, false, L""),
				StructCases(L"[\\p{Nd}-[\\p{Nd}]]+", L"abcxyzABCXYZ`!@#$%^&*()_-+= \t\n", RegexOptions::None, 0, 30, false, L""),
				StructCases(L"[\\P{Nd}-[\\P{Nd}]]+", L"abcxyzABCXYZ`!@#$%^&*()_-+= \t\n", RegexOptions::None, 0, 30, false, L""),

				// All Negation
				StructCases(L"[^\\p{Ll}-[^\\p{Ll}]]+", L"abcxyzABCXYZ`!@#$%^&*()_-+= \t\n", RegexOptions::None, 0, 30, false, L""),
				StructCases(L"[^\\P{Ll}-[^\\P{Ll}]]+", L"abcxyzABCXYZ`!@#$%^&*()_-+= \t\n", RegexOptions::None, 0, 30, false, L""),
				StructCases(L"[^\\p{Lu}-[^\\p{Lu}]]+", L"abcxyzABCXYZ`!@#$%^&*()_-+= \t\n", RegexOptions::None, 0, 30, false, L""),
				StructCases(L"[^\\P{Lu}-[^\\P{Lu}]]+", L"abcxyzABCXYZ`!@#$%^&*()_-+= \t\n", RegexOptions::None, 0, 30, false, L""),
				StructCases(L"[^\\p{Nd}-[^\\p{Nd}]]+", L"abcxyzABCXYZ`!@#$%^&*()_-+= \t\n", RegexOptions::None, 0, 30, false, L""),
				StructCases(L"[^\\P{Nd}-[^\\P{Nd}]]+", L"abcxyzABCXYZ`!@#$%^&*()_-+= \t\n", RegexOptions::None, 0, 30, false, L""),

				// MixedNegation
				StructCases(L"[^\\p{Ll}-[\\P{Ll}]]+", L"abcxyzABCXYZ`!@#$%^&*()_-+= \t\n", RegexOptions::None, 0, 30, false, L""),
				StructCases(L"[\\p{Ll}-[^\\P{Ll}]]+", L"abcxyzABCXYZ`!@#$%^&*()_-+= \t\n", RegexOptions::None, 0, 30, false, L""),
				StructCases(L"[^\\p{Lu}-[\\P{Lu}]]+", L"abcxyzABCXYZ`!@#$%^&*()_-+= \t\n", RegexOptions::None, 0, 30, false, L""),
				StructCases(L"[\\p{Lu}-[^\\P{Lu}]]+", L"abcxyzABCXYZ`!@#$%^&*()_-+= \t\n", RegexOptions::None, 0, 30, false, L""),
				StructCases(L"[^\\p{Nd}-[\\P{Nd}]]+", L"abcxyzABCXYZ`!@#$%^&*()_-+= \t\n", RegexOptions::None, 0, 30, false, L""),
				StructCases(L"[\\p{Nd}-[^\\P{Nd}]]+", L"abcxyzABCXYZ`!@#$%^&*()_-+= \t\n", RegexOptions::None, 0, 30, false, L""),

				// Character Class Substraction
				StructCases(L"[ab\\-\\[cd-[-[]]]]", L"[]]", RegexOptions::None, 0, 3, false, L""),
				StructCases(L"[ab\\-\\[cd-[-[]]]]", L"-]]", RegexOptions::None, 0, 3, false, L""),
				StructCases(L"[ab\\-\\[cd-[-[]]]]", L"`]]", RegexOptions::None, 0, 3, false, L""),
				StructCases(L"[ab\\-\\[cd-[-[]]]]", L"e]]", RegexOptions::None, 0, 3, false, L""),
				StructCases(L"[ab\\-\\[cd-[[]]]]", L"']]", RegexOptions::None, 0, 3, false, L""),
				StructCases(L"[ab\\-\\[cd-[[]]]]", L"e]]", RegexOptions::None, 0, 3, false, L""),
				StructCases(L"[a-[a-f]]", L"abcdefghijklmnopqrstuvwxyz", RegexOptions::None, 0, 26, false, L""),

				// \c
				// not support
				//StructCases(L"(cat)(\\c[*)(dog)", L"asdlkcat\u00FFdogiwod", RegexOptions::None, 0, 15, false, L""),

				// Surrogate pairs split up into UTF-16 code units.
				StructCases(L"(\\uD82F[\\uDCA0-\\uDCA3])", UtilsString::StringInitSpe(0xD82F) + UtilsString::StringInitSpe(0xDCA2), RegexOptions::CultureInvariant, 0, 2, true, UtilsString::StringInitSpe(0xD82F) + UtilsString::StringInitSpe(0xDCA2)),

				// Unicode text
				StructCases(L"\\u05D0\\u05D1\\u05D2\\u05D3(\\u05D4\\u05D5|\\u05D6\\u05D7|\\u05D8)", L"abc\u05D0\u05D1\u05D2\u05D3\u05D4\u05D5def", RegexOptions::None, 3, 6, true, L"\u05D0\u05D1\u05D2\u05D3\u05D4\u05D5"),
				StructCases(L"\\u05D0(\\u05D4\\u05D5|\\u05D6\\u05D7|\\u05D8)", L"\u05D0\u05D8", RegexOptions::None, 0, 2, true, L"\u05D0\u05D8"),
				StructCases(L"\\u05D0(?:\\u05D1|\\u05D2|\\u05D3)", L"\u05D0\u05D2", RegexOptions::None, 0, 2, true, L"\u05D0\u05D2"),
				StructCases(L"\\u05D0(?:\\u05D1|\\u05D2|\\u05D3)", L"\u05D0\u05D4", RegexOptions::None, 0, 0, false, L""),

				StructCases(L"\\u05D0\\u05D1\\u05D2\\u05D3(\\u05D4\\u05D5|\\u05D6\\u05D7|\\u05D8)", L"abc\u05D0\u05D1\u05D2\u05D3\u05D4\u05D5def", (RegexOptions)((int)RegexOptions::IgnoreCase | (int)RegexOptions::CultureInvariant), 3, 6, true, L"\u05D0\u05D1\u05D2\u05D3\u05D4\u05D5"),
				StructCases(L"\\u05D0(\\u05D4\\u05D5|\\u05D6\\u05D7|\\u05D8)", L"\u05D0\u05D8", (RegexOptions)((int)RegexOptions::IgnoreCase | (int)RegexOptions::CultureInvariant), 0, 2, true, L"\u05D0\u05D8"),
				StructCases(L"\\u05D0(?:\\u05D1|\\u05D2|\\u05D3)", L"\u05D0\u05D2", (RegexOptions)((int)RegexOptions::IgnoreCase | (int)RegexOptions::CultureInvariant), 0, 2, true, L"\u05D0\u05D2"),
				StructCases(L"\\u05D0(?:\\u05D1|\\u05D2|\\u05D3)", L"\u05D0\u05D4", (RegexOptions)((int)RegexOptions::IgnoreCase | (int)RegexOptions::CultureInvariant), 0, 0, false, L""),


				// .* : Case sensitive
				StructCases(L".*\nfoo", L"This shouldn't match", RegexOptions::None, 0, 20, false, L""),
				StructCases(L"a.*\nfoo", L"This shouldn't match", RegexOptions::None, 0, 20, false, L""),
				StructCases(L".*\nFoo", L"\nFooThis should match", RegexOptions::None, 0, 21, true, L"\nFoo"),
				StructCases(L".*\nfoo", L"\nfooThis should match", RegexOptions::None, 4, 17, false, L""),

				StructCases(L".*?\nfoo", L"This shouldn't match", RegexOptions::None, 0, 20, false, L""),
				StructCases(L"a.*?\nfoo", L"This shouldn't match", RegexOptions::None, 0, 20, false, L""),
				StructCases(L".*?\nFoo", L"\nFooThis should match", RegexOptions::None, 0, 21, true, L"\nFoo"),
				StructCases(L".*?\nfoo", L"\nfooThis should match", RegexOptions::None, 4, 17, false, L""),

				StructCases(L".*\\dfoo", L"This shouldn't match", RegexOptions::None, 0, 20, false, L""),
				StructCases(L".*\\dFoo", L"This1Foo should match", RegexOptions::None, 0, 21, true, L"This1Foo"),
				StructCases(L".*\\dFoo", L"This1foo should 2Foo match", RegexOptions::None, 0, 26, true, L"This1foo should 2Foo"),
				StructCases(L".*\\dFoo", L"This1foo shouldn't 2foo match", RegexOptions::None, 0, 29, false, L""),
				StructCases(L".*\\dfoo", L"This1foo shouldn't 2foo match", RegexOptions::None, 24, 5, false, L""),

				StructCases(L".*?\\dfoo", L"This shouldn't match", RegexOptions::None, 0, 20, false, L""),
				StructCases(L".*?\\dFoo", L"This1Foo should match", RegexOptions::None, 0, 21, true, L"This1Foo"),
				StructCases(L".*?\\dFoo", L"This1foo should 2Foo match", RegexOptions::None, 0, 26, true, L"This1foo should 2Foo"),
				StructCases(L".*?\\dFoo", L"This1foo shouldn't 2foo match", RegexOptions::None, 0, 29, false, L""),
				StructCases(L".*?\\dfoo", L"This1foo shouldn't 2foo match", RegexOptions::None, 24, 5, false, L""),

				StructCases(L".*\\dfoo", L"1fooThis1foo should 1foo match", RegexOptions::None, 4, 9, true, L"This1foo"),
				StructCases(L".*\\dfoo", L"This shouldn't match 1foo", RegexOptions::None, 0, 20, false, L""),

				StructCases(L".*?\\dfoo", L"1fooThis1foo should 1foo match", RegexOptions::None, 4, 9, true, L"This1foo"),
				StructCases(L".*?\\dfoo", L"This shouldn't match 1foo", RegexOptions::None, 0, 20, false, L""),

				// Turkish case sensitivity
				StructCases(L"[\\u0120-\\u0130]", L"\u0130", RegexOptions::None, 0, 1, true, L"\u0130"),

				// .* : Case insensitive
				StructCases(L".*\nFoo", L"\nfooThis should match", RegexOptions::IgnoreCase, 0, 21, true, L"\nfoo"),
				StructCases(L".*\\dFoo", L"This1foo should match", RegexOptions::IgnoreCase, 0, 21, true, L"This1foo"),
				StructCases(L".*\\dFoo", L"This1foo should 2FoO match", RegexOptions::IgnoreCase, 0, 26, true, L"This1foo should 2FoO"),
				StructCases(L".*\\dFoo", L"This1Foo should 2fOo match", RegexOptions::IgnoreCase, 0, 26, true, L"This1Foo should 2fOo"),
				StructCases(L".*\\dfoo", L"1fooThis1FOO should 1foo match", RegexOptions::IgnoreCase, 4, 9, true, L"This1FOO"),

				StructCases(L".*?\nFoo", L"\nfooThis should match", RegexOptions::IgnoreCase, 0, 21, true, L"\nfoo"),
				StructCases(L".*?\\dFoo", L"This1foo should match", RegexOptions::IgnoreCase, 0, 21, true, L"This1foo"),
				StructCases(L".*?\\dFoo", L"This1foo should 2FoO match", RegexOptions::IgnoreCase, 0, 26, true, L"This1foo"),
				StructCases(L".*?\\dFoo", L"This1Foo should 2fOo match", RegexOptions::IgnoreCase, 0, 26, true, L"This1Foo"),
				StructCases(L".*?\\dFo{2}", L"This1foo should 2FoO match", RegexOptions::IgnoreCase, 0, 26, true, L"This1foo"),
				StructCases(L".*?\\dFo{2}", L"This1Foo should 2fOo match", RegexOptions::IgnoreCase, 0, 26, true, L"This1Foo"),
				StructCases(L".*?\\dfoo", L"1fooThis1FOO should 1foo match", RegexOptions::IgnoreCase, 4, 9, true, L"This1FOO"),


				//not support

				// RightToLeft
				StructCases(L"foo\\d+", L"0123456789foo4567890foo         ", RegexOptions::RightToLeft, 0, 32, true, L"foo4567890"),
				StructCases(L"foo\\d+", L"0123456789foo4567890foo         ", RegexOptions::RightToLeft, 10, 22, true, L"foo4567890"),
				StructCases(L"foo\\d+", L"0123456789foo4567890foo         ", RegexOptions::RightToLeft, 10, 4, true, L"foo4"),
				StructCases(L"foo\\d+", L"0123456789foo4567890foo         ", RegexOptions::RightToLeft, 10, 3, false, L""),
				StructCases(L"foo\\d+", L"0123456789foo4567890foo         ", RegexOptions::RightToLeft, 11, 21, false, L""),

				StructCases(L"foo\\d+?", L"0123456789foo4567890foo         ", RegexOptions::RightToLeft, 0, 32, true, L"foo4567890"),
				StructCases(L"foo\\d+?", L"0123456789foo4567890foo         ", RegexOptions::RightToLeft, 10, 22, true, L"foo4567890"),
				StructCases(L"foo\\d+?", L"0123456789foo4567890foo         ", RegexOptions::RightToLeft, 10, 4, true, L"foo4"),
				StructCases(L"foo\\d+?", L"0123456789foo4567890foo         ", RegexOptions::RightToLeft, 10, 3, false, L""),
				StructCases(L"foo\\d+?", L"0123456789foo4567890foo         ", RegexOptions::RightToLeft, 11, 21, false, L""),

				StructCases(L"\\s+\\d+", L"sdf 12sad", RegexOptions::RightToLeft, 0, 9, true, L" 12"),
				StructCases(L"\\s+\\d+", L" asdf12 ", RegexOptions::RightToLeft, 0, 6, false, L""),
				StructCases(L"aaa", L"aaabbb", RegexOptions::None, 3, 3, false, L""),
				StructCases(L"abc|def", L"123def456", (RegexOptions)((int)RegexOptions::RightToLeft | (int)RegexOptions::IgnoreCase | (int)RegexOptions::CultureInvariant), 0, 9, true, L"def"),

				// .* : RTL, Case-sensitive
				StructCases(L".*\nfoo", L"This shouldn't match", (RegexOptions)((int)RegexOptions::None | (int)RegexOptions::RightToLeft), 0, 20, false, L""),
				StructCases(L".*\nfoo", L"This should matchfoo\n", (RegexOptions)((int)RegexOptions::None | (int)RegexOptions::RightToLeft), 4, 13, false, L""),
				StructCases(L"a.*\nfoo", L"This shouldn't match", (RegexOptions)((int)RegexOptions::None | (int)RegexOptions::RightToLeft), 0, 20, false, L""),
				StructCases(L".*\nFoo", L"This should match\nFoo", (RegexOptions)((int)RegexOptions::None | (int)RegexOptions::RightToLeft), 0, 21, true, L"This should match\nFoo"),

				StructCases(L".*?\nfoo", L"This shouldn't match", (RegexOptions)((int)RegexOptions::None | (int)RegexOptions::RightToLeft), 0, 20, false, L""),
				StructCases(L".*?\nfoo", L"This should matchfoo\n", (RegexOptions)((int)RegexOptions::None | (int)RegexOptions::RightToLeft), 4, 13, false, L""),
				StructCases(L"a.*?\nfoo", L"This shouldn't match", (RegexOptions)((int)RegexOptions::None | (int)RegexOptions::RightToLeft), 0, 20, false, L""),
				StructCases(L".*?\nFoo", L"This should match\nFoo", (RegexOptions)((int)RegexOptions::None | (int)RegexOptions::RightToLeft), 0, 21, true, L"\nFoo"),

				StructCases(L".*\\dfoo", L"This shouldn't match", (RegexOptions)((int)RegexOptions::None | (int)RegexOptions::RightToLeft), 0, 20, false, L""),
				StructCases(L".*\\dFoo", L"This1Foo should match", (RegexOptions)((int)RegexOptions::None | (int)RegexOptions::RightToLeft), 0, 21, true, L"This1Foo"),
				StructCases(L".*\\dFoo", L"This1foo should 2Foo match", (RegexOptions)((int)RegexOptions::None | (int)RegexOptions::RightToLeft), 0, 26, true, L"This1foo should 2Foo"),
				StructCases(L".*\\dFoo", L"This1foo shouldn't 2foo match", (RegexOptions)((int)RegexOptions::None | (int)RegexOptions::RightToLeft), 0, 29, false, L""),
				StructCases(L".*\\dfoo", L"This1foo shouldn't 2foo match", (RegexOptions)((int)RegexOptions::None | (int)RegexOptions::RightToLeft), 19, 0, false, L""),

				StructCases(L".*?\\dfoo", L"This shouldn't match", (RegexOptions)((int)RegexOptions::None | (int)RegexOptions::RightToLeft), 0, 20, false, L""),
				StructCases(L".*?\\dFoo", L"This1Foo should match", (RegexOptions)((int)RegexOptions::None | (int)RegexOptions::RightToLeft), 0, 21, true, L"1Foo"),
				StructCases(L".*?\\dFoo", L"This1foo should 2Foo match", (RegexOptions)((int)RegexOptions::None | (int)RegexOptions::RightToLeft), 0, 26, true, L"2Foo"),
				StructCases(L".*?\\dFoo", L"This1foo shouldn't 2foo match", (RegexOptions)((int)RegexOptions::None | (int)RegexOptions::RightToLeft), 0, 29, false, L""),
				StructCases(L".*?\\dfoo", L"This1foo shouldn't 2foo match", (RegexOptions)((int)RegexOptions::None | (int)RegexOptions::RightToLeft), 19, 0, false, L""),

				StructCases(L".*\\dfoo", L"1fooThis2foo should 1foo match", (RegexOptions)((int)RegexOptions::None | (int)RegexOptions::RightToLeft), 8, 4, true, L"2foo"),
				StructCases(L".*\\dfoo", L"This shouldn't match 1foo", (RegexOptions)((int)RegexOptions::None | (int)RegexOptions::RightToLeft), 0, 20, false, L""),

				StructCases(L".*?\\dfoo", L"1fooThis2foo should 1foo match", (RegexOptions)((int)RegexOptions::None | (int)RegexOptions::RightToLeft), 8, 4, true, L"2foo"),
				StructCases(L".*?\\dfoo", L"This shouldn't match 1foo", (RegexOptions)((int)RegexOptions::None | (int)RegexOptions::RightToLeft), 0, 20, false, L""),

				// .* : RTL, case insensitive
				StructCases(L".*\nFoo", L"\nfooThis should match", (RegexOptions)((int)RegexOptions::IgnoreCase | (int)RegexOptions::RightToLeft), 0, 21, true, L"\nfoo"),
				StructCases(L".*\\dFoo", L"This1foo should match", (RegexOptions)((int)RegexOptions::IgnoreCase | (int)RegexOptions::RightToLeft), 0, 21, true, L"This1foo"),
				StructCases(L".*\\dFoo", L"This1foo should 2FoO match", (RegexOptions)((int)RegexOptions::IgnoreCase | (int)RegexOptions::RightToLeft), 0, 26, true, L"This1foo should 2FoO"),
				StructCases(L".*\\dFoo", L"This1Foo should 2fOo match", (RegexOptions)((int)RegexOptions::IgnoreCase | (int)RegexOptions::RightToLeft), 0, 26, true, L"This1Foo should 2fOo"),
				StructCases(L".*\\dfoo", L"1fooThis2FOO should 1foo match", (RegexOptions)((int)RegexOptions::IgnoreCase | (int)RegexOptions::RightToLeft), 8, 4, true, L"2FOO"),
				StructCases(L"[\\w\\s].*", L"1fooThis2FOO should 1foo match", (RegexOptions)((int)RegexOptions::IgnoreCase | (int)RegexOptions::RightToLeft), 0, 30, true, L"1fooThis2FOO should 1foo match"),
				StructCases(L"i.*", L"1fooThis2FOO should 1foo match", (RegexOptions)((int)RegexOptions::IgnoreCase | (int)RegexOptions::RightToLeft), 0, 30, true, L"is2FOO should 1foo match"),

				StructCases(L".*?\nFoo", L"\nfooThis should match", (RegexOptions)((int)RegexOptions::IgnoreCase | (int)RegexOptions::RightToLeft), 0, 21, true, L"\nfoo"),
				StructCases(L".*?\\dFoo", L"This1foo should match", (RegexOptions)((int)RegexOptions::IgnoreCase | (int)RegexOptions::RightToLeft), 0, 21, true, L"1foo"),
				StructCases(L".*?\\dFoo", L"This1foo should 2FoO match", (RegexOptions)((int)RegexOptions::IgnoreCase | (int)RegexOptions::RightToLeft), 0, 26, true, L"2FoO"),
				StructCases(L".*?\\dFoo", L"This1Foo should 2fOo match", (RegexOptions)((int)RegexOptions::IgnoreCase | (int)RegexOptions::RightToLeft), 0, 26, true, L"2fOo"),
				StructCases(L".*?\\dfoo", L"1fooThis2FOO should 1foo match", (RegexOptions)((int)RegexOptions::IgnoreCase | (int)RegexOptions::RightToLeft), 8, 4, true, L"2FOO"),
				StructCases(L"[\\w\\s].*?", L"1fooThis2FOO should 1foo match", (RegexOptions)((int)RegexOptions::IgnoreCase | (int)RegexOptions::RightToLeft), 0, 30, true, L"h"),
				StructCases(L"i.*?", L"1fooThis2FOO should 1foo match", (RegexOptions)((int)RegexOptions::IgnoreCase | (int)RegexOptions::RightToLeft), 0, 30, true, L"is2FOO should 1foo match"),

				// [ActiveIssue("https://github.com/dotnet/runtime/issues/36149")]
				//if (PlatformDetection.IsNetCore)
				//{
				//    // Unicode symbols in character ranges. These are chars whose lowercase values cannot be found by using the offsets specified in s_lcTable.
				//    StructCases(L"^(?i:[\u00D7-\u00D8])$", '\u00F7'.ToString(), RegexOptions::IgnoreCase | RegexOptions::CultureInvariant, 0, 1, false, L""),
				//    StructCases(L"^(?i:[\u00C0-\u00DE])$", '\u00F7'.ToString(), RegexOptions::IgnoreCase | RegexOptions::CultureInvariant, 0, 1, false, L""),
				//    StructCases(L"^(?i:[\u00C0-\u00DE])$", ((char)('\u00C0' + 32)).ToString(), RegexOptions::IgnoreCase | RegexOptions::CultureInvariant, 0, 1, true, ((char)('\u00C0' + 32)).ToString());
				//    StructCases(L"^(?i:[\u00C0-\u00DE])$", ((char)('\u00DE' + 32)).ToString(), RegexOptions::IgnoreCase | RegexOptions::CultureInvariant, 0, 1, true, ((char)('\u00DE' + 32)).ToString());
				//    StructCases(L"^(?i:[\u0391-\u03AB])$", ((char)('\u03A2' + 32)).ToString(), RegexOptions::IgnoreCase | RegexOptions::CultureInvariant, 0, 1, false, L""),
				//    StructCases(L"^(?i:[\u0391-\u03AB])$", ((char)('\u0391' + 32)).ToString(), RegexOptions::IgnoreCase | RegexOptions::CultureInvariant, 0, 1, true, ((char)('\u0391' + 32)).ToString());
				//    StructCases(L"^(?i:[\u0391-\u03AB])$", ((char)('\u03AB' + 32)).ToString(), RegexOptions::IgnoreCase | RegexOptions::CultureInvariant, 0, 1, true, ((char)('\u03AB' + 32)).ToString());
				//    StructCases(L"^(?i:[\u1F18-\u1F1F])$", ((char)('\u1F1F' - 8)).ToString(), RegexOptions::IgnoreCase | RegexOptions::CultureInvariant, 0, 1, false, L""),
				//    StructCases(L"^(?i:[\u1F18-\u1F1F])$", ((char)('\u1F18' - 8)).ToString(), RegexOptions::IgnoreCase | RegexOptions::CultureInvariant, 0, 1, true, ((char)('\u1F18' - 8)).ToString());
				//    StructCases(L"^(?i:[\u10A0-\u10C5])$", ((char)('\u10A0' + 7264)).ToString(), RegexOptions::IgnoreCase | RegexOptions::CultureInvariant, 0, 1, true, ((char)('\u10A0' + 7264)).ToString());
				//    StructCases(L"^(?i:[\u10A0-\u10C5])$", ((char)('\u1F1F' + 48)).ToString(), RegexOptions::IgnoreCase | RegexOptions::CultureInvariant, 0, 1, false, L""),
				//    StructCases(L"^(?i:[\u24B6-\u24D0])$", ((char)('\u24D0' + 26)).ToString(), RegexOptions::IgnoreCase | RegexOptions::CultureInvariant, 0, 1, false, L""),
				//    StructCases(L"^(?i:[\u24B6-\u24D0])$", ((char)('\u24CF' + 26)).ToString(), RegexOptions::IgnoreCase | RegexOptions::CultureInvariant, 0, 1, true, ((char)('\u24CF' + 26)).ToString());
				//}


				StructCases(L"\\W.*?\\D", L"seq 012 of 3 digits", RegexOptions::None, 0, 19, true, L" 012 "),
				StructCases(L"\\W.+?\\D", L"seq 012 of 3 digits", RegexOptions::None, 0, 19, true, L" 012 "),
				StructCases(L"\\W.{1,7}?\\D", L"seq 012 of 3 digits", RegexOptions::None, 0, 19, true, L" 012 "),
				StructCases(L"\\W.{1,2}?\\D", L"seq 012 of 3 digits", RegexOptions::None, 0, 19, true, L" of"),
				StructCases(L"\\W.*?\\b", L"digits:0123456789", RegexOptions::None, 0, 17, true, L":"),
				StructCases(L"\\B.*?\\B", L"e.g:abc", RegexOptions::None, 0, 7, true, L""),
				StructCases(L"\\B\\W+?", L"e.g:abc", RegexOptions::None, 0, 7, false, L""),
				StructCases(L"\\B\\W*?", L"e.g:abc", RegexOptions::None, 0, 7, true, L""),

				// While not lazy loops themselves, variants of the prior case that should give same results here
				StructCases(L"\\B\\W*", L"e.g:abc", RegexOptions::None, 0, 7, true, L""),
				StructCases(L"\\B\\W?", L"e.g:abc", RegexOptions::None, 0, 7, true, L""),

				//mixed lazy and eager counting
				StructCases(L"z(a{0,5}|a{0,10}?)", L"xyzaaaaaaaaaxyz", RegexOptions::None, 0, 15, true, L"zaaaaa"),

				StructCases(L"\\W.*?\\D", L"seq 012 of 3 digits", RegexOptions::Singleline, 0, 19, true, L" 012 "),
				StructCases(L"\\W.+?\\D", L"seq 012 of 3 digits", RegexOptions::Singleline, 0, 19, true, L" 012 "),
				StructCases(L"\\W.{1,7}?\\D", L"seq 012 of 3 digits", RegexOptions::Singleline, 0, 19, true, L" 012 "),
				StructCases(L"\\W.{1,2}?\\D", L"seq 012 of 3 digits", RegexOptions::Singleline, 0, 19, true, L" of"),
				StructCases(L"\\W.*?\\b", L"digits:0123456789", RegexOptions::Singleline, 0, 17, true, L":"),
				StructCases(L"\\B.*?\\B", L"e.g:abc", RegexOptions::Singleline, 0, 7, true, L""),
				StructCases(L"\\B\\W+?", L"e.g:abc", RegexOptions::Singleline, 0, 7, false, L""),
				StructCases(L"\\B\\W*?", L"e.g:abc", RegexOptions::Singleline, 0, 7, true, L""),

				// While not lazy loops themselves, variants of the prior case that should give same results here
				StructCases(L"\\B\\W*", L"e.g:abc", RegexOptions::Singleline, 0, 7, true, L""),
				StructCases(L"\\B\\W?", L"e.g:abc", RegexOptions::Singleline, 0, 7, true, L""),

				//mixed lazy and eager counting
				StructCases(L"z(a{0,5}|a{0,10}?)", L"xyzaaaaaaaaaxyz", RegexOptions::Singleline, 0, 15, true, L"zaaaaa"),


			};
		}

		static void VerifyMatch(Match* match, bool expectedSuccess, wstring expectedValue)
		{
			bool value_result = match->Success;
			Assert::True(expectedSuccess == value_result);
			Assert::True(expectedValue == match->ToString());

			// Groups can never be empty
			Assert::True(match->Groups->Count >= 1);
			Assert::True(expectedSuccess == match->Groups->operator[](0)->Success);
			Assert::True(expectedValue == match->Groups->operator[](0)->ToString());
		}

		static void Match(wstring pattern, wstring input, RegexOptions options, Regex* r, int beginning, int length, bool expectedSuccess, wstring expectedValue)
		{
			bool isDefaultStart = IsDefaultStart(input, options, beginning);
			bool isDefaultCount = IsDefaultCount(input, options, length);

			// Test instance method overloads
			if (isDefaultStart && isDefaultCount)
			{
				VerifyMatch(r->Match(&input), expectedSuccess, expectedValue);
				VerifyIsMatch(r, input, expectedSuccess, Regex::InfiniteMatchTimeout);
			}
			if (beginning + length == input.length() && ((int)options & (int)RegexOptions::RightToLeft) == 0)
			{
				VerifyMatch(r->Match(&input, beginning), expectedSuccess, expectedValue);
			}
			VerifyMatch(r->Match(&input, beginning, length), expectedSuccess, expectedValue);

			// Test static method overloads
			if (isDefaultStart && isDefaultCount)
			{
				auto dasds = Regex::Match(&input, pattern, options);
				VerifyMatch(dasds, expectedSuccess, expectedValue);
				VerifyIsMatch(NULL, input, expectedSuccess, Regex::InfiniteMatchTimeout, &pattern, options);
				dasds->Release();
			}
		}

		static void CreateAndMatch(wstring pattern, wstring input, RegexOptions options, int beginning, int length, bool expectedSuccess, wstring expectedValue)
		{
			Regex r = Regex(pattern, options);
			Match(pattern, input, options, &r, beginning, length, expectedSuccess, expectedValue);
		}

		static void Match_Timeout()
		{
			Regex regex = Regex(L"\\p{Lu}", RegexOptions::IgnoreCase, TimeSpan::FromHours(1));
			wstring input = L"abc";
			Regexs::Match* match = regex.Match(&input);
			Assert::True(match->Success);
			Assert::True(L"a" == match->ToString());
		}

		static void Match_TestThatTimeoutHappens()
		{
			Random rnd = Random();
			rnd.Init(42);

			vector<wchar_t> chars = vector<wchar_t>(1000000);
			for (size_t i = 0; i < chars.size(); i++)
			{
				byte b = (byte)rnd.Next(0, 256);
				chars[i] = b < 200 ? 'a' : (wchar_t)b;
			}
			wstring input = wstring(chars.data());

			Regex re = Regex(L"a.{20}^", RegexOptions::None, TimeSpan::FromMilliseconds(10));
			AssertThrows(RegexMatchTimeoutException, re.Match(&input))
		}

		static void Match_Timeout_Throws()
		{
			const wstring Pattern = L"^([0-9a-zA-Z]([-.\\w]*[0-9a-zA-Z])*@(([0-9a-zA-Z])+([-\\w]*[0-9a-zA-Z])*\\.)+[a-zA-Z]{2,9})$";
			wstring input = wstring('a', 50) + L"@a.a";

			Regex r = Regex(Pattern, RegexOptions::None, TimeSpan::FromMilliseconds(100));

			AssertThrows(RegexMatchTimeoutException, r.Match(&input))
		}

		static void Match_DefaultTimeout_Throws(RegexOptions options, string optionsString)
		{
			const wstring Pattern = L"^([0-9a-zA-Z]([-.\\w]*[0-9a-zA-Z])*@(([0-9a-zA-Z])+([-\\w]*[0-9a-zA-Z])*\\.)+[a-zA-Z]{2,9})$";
			wstring input = wstring('a', 50) + L"@a.a";


			AssertThrows(RegexMatchTimeoutException, Regex(Pattern).Match(&input))
			AssertThrows(RegexMatchTimeoutException, Regex(Pattern).IsMatch(input))
			AssertThrows(RegexMatchTimeoutException, Regex(Pattern).Matches(&input)->Count)
			AssertThrows(RegexMatchTimeoutException, Regex::Match(&input, Pattern))
			AssertThrows(RegexMatchTimeoutException, Regex::IsMatch(input, Pattern))
			AssertThrows(RegexMatchTimeoutException, Regex::Matches(&input, Pattern)->Count)
			AssertThrows(RegexMatchTimeoutException, Regex(Pattern, (RegexOptions)atoi(optionsString.c_str())).Match(&input))
			AssertThrows(RegexMatchTimeoutException, Regex(Pattern, (RegexOptions)atoi(optionsString.c_str())).IsMatch(input))
			AssertThrows(RegexMatchTimeoutException, Regex(Pattern, (RegexOptions)atoi(optionsString.c_str())).Matches(&input)->Count)
			AssertThrows(RegexMatchTimeoutException, Regex::Match(&input, Pattern, (RegexOptions)atoi(optionsString.c_str())))
			AssertThrows(RegexMatchTimeoutException, Regex::IsMatch(input, Pattern, (RegexOptions)atoi(optionsString.c_str())))
			AssertThrows(RegexMatchTimeoutException, Regex::Matches(&input, Pattern, (RegexOptions)atoi(optionsString.c_str()))->Count)
		}

		static void Match_CachedPattern_NewTimeoutApplies(RegexOptions options)
		{
			wstring PatternLeadingToLotsOfBacktracking = L"^(\\w + \\s ? )*$";
			VerifyIsMatch(NULL, L"", true, TimeSpan::FromDays(1), &PatternLeadingToLotsOfBacktracking, options);
			//var sw = Stopwatch.StartNew();
			VerifyIsMatchThrows(NULL, L"An input string that takes a very very very very very very very very very very very long time!", TimeSpan::FromMilliseconds(1), &PatternLeadingToLotsOfBacktracking, options);
			//Assert::InRange(sw.Elapsed.TotalSeconds, 0, 10); // arbitrary upper bound that should be well above what's needed with a 1ms timeout
		}

		static void Match_Timeout_Repetition_Throws()
		{
			int repetitionCount = 800000000;
			Regex regex = Regex(L"a\\s{" + std::to_wstring(repetitionCount) + L" }", RegexOptions::None, TimeSpan::FromSeconds(1));
			wstring input = L"a" + wstring(' ', repetitionCount) + L"b";
			AssertThrows(RegexMatchTimeoutException, regex.Match(&input))
		}

		static vector<StructMatch_Advanced> Match_Advanced_TestData()
		{
			return
			{
				// \B special character escape: ".*\\B(SUCCESS)\\B.*"
				StructMatch_Advanced(
					L".*\\B(SUCCESS)\\B.*", L"adfadsfSUCCESSadsfadsf", RegexOptions::None, 0, 22,
					{
						CaptureData(L"adfadsfSUCCESSadsfadsf", 0, 22),
						CaptureData(L"SUCCESS", 7, 7)
					}
				),

				// Using |, (), ^, $, .: Actual - "^aaa(bb.+)(d|c)$"
				StructMatch_Advanced(
					L"^aaa(bb.+)(d|c)$", L"aaabb.cc", RegexOptions::None, 0, 8,
					{
						CaptureData(L"aaabb.cc", 0, 8),
						CaptureData(L"bb.c", 3, 4),
						CaptureData(L"c", 7, 1)
					}
				),

				// Using greedy quantifiers: Actual - "(a+)(b*)(c?)"
				StructMatch_Advanced(
					L"(a+)(b*)(c?)", L"aaabbbccc", RegexOptions::None, 0, 9,
					{
						CaptureData(L"aaabbbc", 0, 7),
						CaptureData(L"aaa", 0, 3),
						CaptureData(L"bbb", 3, 3),
						CaptureData(L"c", 6, 1)
					}
				),

				// Using lazy quantifiers: Actual - "(d+?)(e*?)(f??)"
				// Interesting match from this pattern and input. If needed to go to the end of the string change the ? to + in the last lazy quantifier
				StructMatch_Advanced(
					L"(d+?)(e*?)(f??)", L"dddeeefff", RegexOptions::None, 0, 9,
					{
						CaptureData(L"d", 0, 1),
						CaptureData(L"d", 0, 1),
						CaptureData(L"", 1, 0),
						CaptureData(L"", 1, 0)
					}
				),
				StructMatch_Advanced(
					L"(d+?)(e*?)(f+)", L"dddeeefff", RegexOptions::None, 0, 9,
					{
						CaptureData(L"dddeeefff", 0, 9),
						CaptureData(L"ddd", 0, 3),
						CaptureData(L"eee", 3, 3),
						CaptureData(L"fff", 6, 3),
					}
				),

				// Noncapturing group : Actual - "(a+)(?:b*)(ccc)"
				StructMatch_Advanced(
					L"(a+)(?:b*)(ccc)", L"aaabbbccc", RegexOptions::None, 0, 9,
					{
						CaptureData(L"aaabbbccc", 0, 9),
						CaptureData(L"aaa", 0, 3),
						CaptureData(L"ccc", 6, 3),
					}
				),

				// Alternation constructs: Actual - "(111|aaa)"
				StructMatch_Advanced(
					L"(111|aaa)", L"aaa", RegexOptions::None, 0, 3,
					{
						CaptureData(L"aaa", 0, 3),
						CaptureData(L"aaa", 0, 3)
					}
				),

				// Using "n" Regex option. Only explicitly named groups should be captured: Actual - "([0-9]*)\\s(?<s>[a-z_A-Z]+)", L"n"
				StructMatch_Advanced(
					L"([0-9]*)\\s(?<s>[a-z_A-Z]+)", L"200 dollars", RegexOptions::ExplicitCapture, 0, 11,
					{
						CaptureData(L"200 dollars", 0, 11),
						CaptureData(L"dollars", 4, 7)
					}
				),

				// Single line mode "s". Includes new line character: Actual - "([^/]+)","s"
				StructMatch_Advanced(
					L"(.*)", L"abc\nsfc", RegexOptions::Singleline, 0, 7,
					{
						CaptureData(L"abc\nsfc", 0, 7),
						CaptureData(L"abc\nsfc", 0, 7),
					}
				),

				// "([0-9]+(\\.[0-9]+){3})"
				StructMatch_Advanced(
					L"([0-9]+(\\.[0-9]+){3})", L"209.25.0.111", RegexOptions::None, 0, 12,
					{
						CaptureData(L"209.25.0.111", 0, 12),
						CaptureData(L"209.25.0.111", 0, 12),
						CaptureData(L".111", 8, 4,
						            {
							            CaptureData(L".25", 3, 3),
							            CaptureData(L".0", 6, 2),
							            CaptureData(L".111", 8, 4),
						            }),
					}
				),

				// Groups and captures
				StructMatch_Advanced(
					L"(?<A1>a*)(?<A2>b*)(?<A3>c*)", L"aaabbccccccccccaaaabc", RegexOptions::None, 0, 21,
					{
						CaptureData(L"aaabbcccccccccc", 0, 15),
						CaptureData(L"aaa", 0, 3),
						CaptureData(L"bb", 3, 2),
						CaptureData(L"cccccccccc", 5, 10)
					}
				),

				StructMatch_Advanced(
					L"(?<A1>A*)(?<A2>B*)(?<A3>C*)", L"aaabbccccccccccaaaabc", RegexOptions::IgnoreCase, 0, 21,
					{
						CaptureData(L"aaabbcccccccccc", 0, 15),
						CaptureData(L"aaa", 0, 3),
						CaptureData(L"bb", 3, 2),
						CaptureData(L"cccccccccc", 5, 10)
					}
				),

				// Using |, (), ^, $, .: Actual - "^aaa(bb.+)(d|c)$"
				StructMatch_Advanced(
					L"^aaa(bb.+)(d|c)$", L"aaabb.cc", RegexOptions::None, 0, 8,
					{
						CaptureData(L"aaabb.cc", 0, 8),
						CaptureData(L"bb.c", 3, 4),
						CaptureData(L"c", 7, 1)
					}
				),

				// Actual - ".*\\b(\\w+)\\b"
				StructMatch_Advanced(
					L".*\\b(\\w+)\\b", L"XSP_TEST_FAILURE SUCCESS", RegexOptions::None, 0, 24,
					{
						CaptureData(L"XSP_TEST_FAILURE SUCCESS", 0, 24),
						CaptureData(L"SUCCESS", 17, 7)
					}
				),

				// Multiline
				StructMatch_Advanced(
					L"(line2$\n)line3", L"line1\nline2\nline3\n\nline4", RegexOptions::Multiline, 0, 24,
					{
						CaptureData(L"line2\nline3", 6, 11),
						CaptureData(L"line2\n", 6, 6)
					}
				),

				// Multiline
				StructMatch_Advanced(
					L"(line2\n^)line3", L"line1\nline2\nline3\n\nline4", RegexOptions::Multiline, 0, 24,
					{
						CaptureData(L"line2\nline3", 6, 11),
						CaptureData(L"line2\n", 6, 6)
					}
				),

				// Multiline
				StructMatch_Advanced(
					L"(line3\n$\n)line4", L"line1\nline2\nline3\n\nline4", RegexOptions::Multiline, 0, 24,
					{
						CaptureData(L"line3\n\nline4", 12, 12),
						CaptureData(L"line3\n\n", 12, 7)
					}
				),

				// Multiline
				StructMatch_Advanced(
					L"(line3\n^\n)line4", L"line1\nline2\nline3\n\nline4", RegexOptions::Multiline, 0, 24,
					{
						CaptureData(L"line3\n\nline4", 12, 12),
						CaptureData(L"line3\n\n", 12, 7)
					}
				),

				// Multiline
				StructMatch_Advanced(
					L"(line2$\n^)line3", L"line1\nline2\nline3\n\nline4", RegexOptions::Multiline, 0, 24,
					{
						CaptureData(L"line2\nline3", 6, 11),
						CaptureData(L"line2\n", 6, 6)
					}
				),

				// Zero-width positive lookahead assertion: Actual - "abc(?=XXX)\\w+"
				StructMatch_Advanced(
					L"abc(?=XXX)\\w+", L"abcXXXdef", RegexOptions::None, 0, 9,
					{
						CaptureData(L"abcXXXdef", 0, 9)
					}
				),

				// Backreferences : Actual - "(\\w)\\1"
				StructMatch_Advanced(
					L"(\\w)\\1", L"aa", RegexOptions::None, 0, 2,
					{
						CaptureData(L"aa", 0, 2),
						CaptureData(L"a", 0, 1),
					}
				),

				// Actual - "(?<1>\\d+)abc(?(1)222|111)"
				StructMatch_Advanced(
					L"(?<MyDigits>\\d+)abc(?(MyDigits)222|111)", L"111abc222", RegexOptions::None, 0, 9,
					{
						CaptureData(L"111abc222", 0, 9),
						CaptureData(L"111", 0, 3)
					}
				),

				// RightToLeft
				StructMatch_Advanced(
					L"aaa", L"aaabbb", RegexOptions::RightToLeft, 3, 3,
					{
						CaptureData(L"aaa", 0, 3)
					}
				),

				// RightToLeft with anchor
				StructMatch_Advanced(
					L"^aaa", L"aaabbb", RegexOptions::RightToLeft, 3, 3,
					{
						CaptureData(L"aaa", 0, 3)
					}
				),
				StructMatch_Advanced(
					L"bbb$", L"aaabbb", RegexOptions::RightToLeft, 0, 3,
					{
						CaptureData(L"bbb", 0, 3)
					}
				),
			};
		}

		static void Match_Advanced(wstring pattern, wstring input, RegexOptions options, int beginning, int length, vector<CaptureData> expected)
		{
			bool isDefaultStart = IsDefaultStart(input, options, beginning);
			bool isDefaultCount = IsDefaultStart(input, options, length);

			Regex r = Regex(pattern, options);

			if (isDefaultStart && isDefaultCount)
			{
				// Use Match(string) or Match(string, string, RegexOptions)
				VerifyMatch(r.Match(&input), expected);
				VerifyMatch(Regex::Match(&input, pattern, options), expected);

				VerifyIsMatch(NULL, input, true, Regex::InfiniteMatchTimeout, &pattern, options);
			}

			if (beginning + length == input.length())
			{
				// Use Match(string, int)
				VerifyMatch(r.Match(&input, beginning), expected);
			}

			if (((int)options & (int)RegexOptions::RightToLeft) == 0)
			{
				// Use Match(string, int, int)
				VerifyMatch(r.Match(&input, beginning, length), expected);
			}
		}

		static void VerifyMatch(Regexs::Match* match, vector<CaptureData> expected)
		{
			Assert::True(match->Success);

			Assert::True(expected[0].Value == match->ToString());
			Assert::True(expected[0].Index == match->Index);
			Assert::True(expected[0].Length == match->Length);

			Assert::True(1 == match->Captures->Count);
			Assert::True(expected[0].Value == match->Captures->operator[](0)->ToString());
			Assert::True(expected[0].Index == match->Captures->operator[](0)->Index);
			Assert::True(expected[0].Length == match->Captures->operator[](0)->Length);

			Assert::True(expected.size() == match->Groups->Count);
			for (int i = 0; i < match->Groups->Count; i++)
			{
				Assert::True(match->Groups->operator[](i)->Success);

				Assert::True(expected[i].Value == match->Groups->operator[](i)->ToString());
				Assert::True(expected[i].Index == match->Groups->operator[](i)->Index);
				Assert::True(expected[i].Length == match->Groups->operator[](i)->Length);

				//if (!IsNonBacktracking(engine))
				//{
				//	Assert::True(expected[i].Captures.Length, match->Groups[i].Captures.Count);
				//	for (int j = 0; j < match->Groups[i].Captures.Count; j++)
				//	{
				//		Assert::True(expected[i].Captures[j].Value, match->Groups[i].Captures[j]);
				//		Assert::True(expected[i].Captures[j].Index, match->Groups[i].Captures[j].Index);
				//		Assert::True(expected[i].Captures[j].Length, match->Groups[i].Captures[j].Length);
				//	}
				//}
				//else
				//{
				//	// NonBacktracking does not support multiple captures
				//	Assert::True(1, match->Groups[i].Captures.Count);
				//	int lastExpected = expected[i].Captures.Length - 1;
				//	Assert::True(expected[i].Captures[lastExpected].Value, match->Groups[i].Captures[0]);
				//	Assert::True(expected[i].Captures[lastExpected].Index, match->Groups[i].Captures[0].Index);
				//	Assert::True(expected[i].Captures[lastExpected].Length, match->Groups[i].Captures[0].Length);
				//}
			}
		}

		static vector<StructStartatDiffersFromBeginning> Match_StartatDiffersFromBeginning_MemberData()
		{
			return
			{
				StructStartatDiffersFromBeginning(L"^.*", L"abc", RegexOptions::None, 0, true, true),
				StructStartatDiffersFromBeginning(L"^.*", L"abc", RegexOptions::None, 1, false, true),
				StructStartatDiffersFromBeginning(L"^.*", L"abc", RegexOptions::Singleline, 0, true, true),
				StructStartatDiffersFromBeginning(L"^.*", L"abc", RegexOptions::Singleline, 1, false, true),
				StructStartatDiffersFromBeginning(L"^.*", L"abc", RegexOptions::Multiline, 0, true, true),
				StructStartatDiffersFromBeginning(L"^.*", L"abc", RegexOptions::Multiline, 1, false, true),
				StructStartatDiffersFromBeginning(L"^.*", L"abc", (RegexOptions)((int)RegexOptions::Singleline | (int)RegexOptions::Multiline), 0, true, true),
				StructStartatDiffersFromBeginning(L"^.*", L"abc", (RegexOptions)((int)RegexOptions::Singleline | (int)RegexOptions::Multiline), 1, false, true),


				StructStartatDiffersFromBeginning(L"(?<=abc)def", L"abcdef", RegexOptions::None, 3, true, false),
				StructStartatDiffersFromBeginning(L"(?<!abc)def", L"abcdef", RegexOptions::None, 3, false, true)

			};
		}

		static void Match_StartatDiffersFromBeginning(wstring pattern, wstring input, RegexOptions options, int startat, bool expectedSuccessStartAt, bool expectedSuccessBeginning)
		{
			Regex r = Regex(pattern, options);

			Assert::True(expectedSuccessStartAt == r.IsMatch(input, startat));
			Assert::True(expectedSuccessStartAt == r.Match(&input, startat)->Success);

			wstring temp = input.substr(startat);
			Assert::True(expectedSuccessBeginning == r.Match(&temp)->Success);
			Assert::True(expectedSuccessBeginning == r.Match(&input, startat, input.length() - startat)->Success);
		}

		static void Result(wstring pattern, wstring input, wstring replacement, wstring expected)
		{
			Assert::True(expected == Regex(pattern).Match(&input)->Result(replacement));
		}

		static void Result_Invalid()
		{
			wstring input = L"foo";
			Regexs::Match* match = Regex::Match(&input, L"foo");
			//AssertThrows(ArgumentNullException, match->Result(NULL))//
		}

		static void Match_SpecialUnicodeCharacters_enUS()
		{
			auto cache = GetCultureInfo::s_userDefaultCulture;
			GetCultureInfo::s_userDefaultCulture = ComCultureInfo::CreateSpecificCulture(L"en-US");
			CreateAndMatch(L"\u0131", L"\u0049", RegexOptions::IgnoreCase, 0, 1, false, L"");
			CreateAndMatch(L"\u0131", L"\u0069", RegexOptions::IgnoreCase, 0, 1, false, L"");
			GetCultureInfo::s_userDefaultCulture = cache;
		}

		static void Match_SpecialUnicodeCharacters_Invariant()
		{
			auto cache = GetCultureInfo::s_userDefaultCulture;
			GetCultureInfo::s_userDefaultCulture = ComCultureInfo::InvariantCulture();

			CreateAndMatch(L"\u0131", L"\u0049", RegexOptions::IgnoreCase, 0, 1, false, L"");
			CreateAndMatch(L"\u0131", L"\u0069", RegexOptions::IgnoreCase, 0, 1, false, L"");
			CreateAndMatch(L"\u0130", L"\u0049", RegexOptions::IgnoreCase, 0, 1, false, L"");
			CreateAndMatch(L"\u0130", L"\u0069", RegexOptions::IgnoreCase, 0, 1, false, L"");

			GetCultureInfo::s_userDefaultCulture = cache;
		}

		static void Match_Invalid()
		{
			Regexs::Regex r = Regexs::Regex(L"pattern");

			//// Input is null
			//AssertThrows(ArgumentNullException, Regex::Match(NULL, L"pattern"))
			//AssertThrows(ArgumentNullException, Regex::Match(NULL, L"pattern", RegexOptions::None))
			//AssertThrows(ArgumentNullException, Regex::Match(NULL, L"pattern", RegexOptions::None, TimeSpan::FromSeconds(1)))
			//AssertThrows(ArgumentNullException, r.Match(NULL))
			//AssertThrows(ArgumentNullException, r.Match(NULL, 0))
			//AssertThrows(ArgumentNullException, r.Match(NULL, 0, 0))

			//// Pattern is null
			//AssertThrows(ArgumentNullException, r.Match(L"input", NULL))
			//AssertThrows(ArgumentNullException, r.Match(L"input", NULL, RegexOptions::None))
			//AssertThrows(ArgumentNullException, r.Match(L"input", NULL, RegexOptions::None, TimeSpan::FromSeconds(1)))

			wstring input = L"input";

			// Start is invalid
			AssertThrows(ArgumentOutOfRangeException, r.Match(&input, -1))
			AssertThrows(ArgumentOutOfRangeException, r.Match(&input, -1, 0))
			AssertThrows(ArgumentOutOfRangeException, r.Match(&input, 6))
			AssertThrows(ArgumentOutOfRangeException, r.Match(&input, 6, 0))

			// Length is invalid
			AssertThrows(ArgumentOutOfRangeException, r.Match(&input, 0, -1))
			AssertThrows(ArgumentOutOfRangeException, r.Match(&input, 0, 6))
		}

		static void IsMatch_Invalid()
		{
			Regexs::Regex r = Regexs::Regex(L"pattern");

			//// Input is null
			//AssertThrows(ArgumentNullException, Regex::IsMatch(NULL, L"pattern"))
			//AssertThrows(ArgumentNullException, Regex::IsMatch(NULL, L"pattern", RegexOptions::None))
			//AssertThrows(ArgumentNullException, Regex::IsMatch(NULL, L"pattern", RegexOptions::None, TimeSpan::FromSeconds(1)))


			//AssertThrows(ArgumentNullException, r.IsMatch(NULL))
			//AssertThrows(ArgumentNullException, r.IsMatch(NULL, 0))


			// Start is invalid
			AssertThrows(ArgumentOutOfRangeException, r.IsMatch(L"input", -1))
			AssertThrows(ArgumentOutOfRangeException, r.IsMatch(L"input", 6))
		}

		static void IsMatch_SucceedQuicklyDueToLoopReduction(wstring pattern, wstring input, bool expected)
		{
			Regexs::Regex r = Regexs::Regex(pattern);
			VerifyIsMatch(&r, input, expected, Regex::InfiniteMatchTimeout);
		}

		static void TestCharIsLowerCultureEdgeCasesAroundTurkishCharacters()
		{
			Regexs::Regex r1 = Regexs::Regex(L"[\u012F-\u0130]", RegexOptions::IgnoreCase);
			Regexs::Regex r2 = Regexs::Regex(L"[\u012F\u0130]", RegexOptions::IgnoreCase);
			Assert::True(r1.IsMatch(L"\u0130") == r2.IsMatch(L"\u0130"));
		}

		static void Synchronized()
		{
			wstring input = L"abc";
			auto rm = Regexs::Regex(L"abc");
			auto m = rm.Match(&input);
			Assert::True(m->Success);
			Assert::True(L"abc" == m->ToString());

			auto m2 = m->Synchronized(m);
			Assert::True(m == m2);
			Assert::True(m2->Success);
			Assert::True(L"abc" == m2->ToString());

			AssertThrows(ArgumentNullException, Match::Synchronized(NULL))
		}

		/// <summary>
		/// Tests current inconsistent treatment of \b and \w.
		/// The match fails because \u200c and \u200d do not belong to \w.
		/// At the same time \u200c and \u200d are considered as word characters for the \b and \B anchors.
		/// The test checks that the same behavior applies to all backends.
		/// </summary>
		static void Match_Boundary()
		{
			Regexs::Regex r = Regexs::Regex(L"\\b\\w + \\b");
			VerifyIsMatch(&r, L" AB\u200cCD ", false, Regex::InfiniteMatchTimeout);
			VerifyIsMatch(&r, L" AB\u200dCD ", false, Regex::InfiniteMatchTimeout);
		}

		static vector<StructMatch_Count> Match_Count_TestData()
		{
			wstring b1 = L"((?<=\\w)(?!\\w)|(?<!\\w)(?=\\w))";
			wstring b2 = L"((?<=\\w)(?=\\W)|(?<=\\W)(?=\\w))";

			return
			{
				StructMatch_Count(L"\\b\\w+\\b", L"one two three", 3),
				StructMatch_Count(L"\\b\\w+\\b", L"on\u200ce two three", 2),
				StructMatch_Count(L"\\b\\w+\\b", L"one tw\u200do three", 2),
				StructMatch_Count(L"" + b1 + L"\\w+" + b1 + L"", L"one two three", 3),
				StructMatch_Count(L"" + b1 + L"\\w+" + b1 + L"", L"on\u200ce two three", 4),
				StructMatch_Count(L"" + b2 + L"\\w+" + b2 + L"", L"one two three", 1),
				StructMatch_Count(L"" + b2 + L"\\w+" + b2 + L"", L"one two", 0),
			};
		}

		static void Match_Count(wstring pattern, wstring input, int expectedCount)
		{
			Regex r = Regex(pattern);
			Assert::True(expectedCount == r.Matches(&input)->Count);
		}

		/// <summary>
		/// Causes NonBacktracking engine to switch to NFA mode internally.
		/// NFA mode is otherwise never triggered by typical cases.
		/// </summary>
		static void StressTestNfaMode(wstring pattern, wstring input_suffix, int expected_matchlength)
		{
			Random random = Random();
			random.Init(0);

			wstring input;
			for (int i = 0; i < 50000; ++i)
			{
				if (random.NextBool())
					input += 'a';
				else
					input += 'b';
			}
			// Consider a random string of 50_000 a's and b's
			input += input_suffix;
			Regex re = Regex(pattern, RegexOptions::Singleline);
			auto m = re.Match(&input);
			Assert::True(m->Success);
			Assert::True(50000 == m->Index);
			Assert::True(expected_matchlength == m->Length);
		}

		static vector<StrcutAllMatches> AllMatches_TestData()
		{
			wstring pattern_orig = L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789< > :;&@%!";
			wstring pattern_WL = L"ａｂｃｄｅｆｇｈｉｊｋｌｍｎｏｐｑｒｓｔｕｖｗｘｙｚＡＢＣＤＥＦＧＨＩＪＫＬＭＮＯＰＱＲＳＴＵＶＷＸＹＺ０１２３４５６７８９＜＞：；＆＠％！";
			wstring pattern = L"(" + pattern_orig + L"===" + pattern_WL + L")+";
			wstring input = L"=====" + pattern_orig + L"===" + pattern_WL + pattern_orig + L"===" + pattern_WL + L"===" + pattern_orig + L"===" + pattern_orig;
			int length = 2 * (pattern_orig.length() + 3 + pattern_WL.length());

			auto ssss = input.substr(5, length);

			return
			{
				// Basic
				StrcutAllMatches(L"a+", RegexOptions::None, L"xxxxxaaaaxxxxxxxxxxaaaaaa", {StrcutAllMatchesItem(5, 4, L"aaaa"), StrcutAllMatchesItem(19, 6, L"aaaaaa")}),
				StrcutAllMatches(L"(...)+", RegexOptions::None, L"abcd\nfghijklm", {StrcutAllMatchesItem(0, 3, L"abc"), StrcutAllMatchesItem(5, 6, L"fghijk")}),
				StrcutAllMatches(L"something", RegexOptions::None, L"nothing", {}),
				StrcutAllMatches(L"(a|ba)c", RegexOptions::None, L"bac", {StrcutAllMatchesItem(0, 3, L"bac")}),
				StrcutAllMatches(L"(a|ba)c", RegexOptions::None, L"ac", {StrcutAllMatchesItem(0, 2, L"ac")}),
				StrcutAllMatches(L"(a|ba)c", RegexOptions::None, L"baacd", {StrcutAllMatchesItem(2, 2, L"ac")}),
				StrcutAllMatches(L"\n", RegexOptions::None, L"\n", {StrcutAllMatchesItem(0, 1, L"\n")}),
				StrcutAllMatches(L"[^a]", RegexOptions::None, L"\n", {StrcutAllMatchesItem(0, 1, L"\n")}),

				// In Singleline mode . includes all characters, also \n
				StrcutAllMatches(L"(...)+", (RegexOptions)((int)RegexOptions::None | (int)RegexOptions::Singleline), L"abcd\nfghijklm", {StrcutAllMatchesItem(0, 12, L"abcd\nfghijkl")}),

				// Ignoring case
				StrcutAllMatches(L"a+", (RegexOptions)((int)RegexOptions::None | (int)RegexOptions::IgnoreCase), L"xxxxxaAAaxxxxxxxxxxaaaaAa", {StrcutAllMatchesItem(5, 4, L"aAAa"), StrcutAllMatchesItem(19, 6, L"aaaaAa")}),

				// NonASCII characters
				StrcutAllMatches(L"(\\uFFFE\\uFFFF)+", RegexOptions::None, L"=====￾￿￾￿￾====",
				                 {StrcutAllMatchesItem(5, 4, UtilsString::StringInitSpe(0xFFFE) + UtilsString::StringInitSpe(0xFFFF) + UtilsString::StringInitSpe(0xFFFE) + UtilsString::StringInitSpe(0xFFFF))}),
				StrcutAllMatches(L"\\d\\s\\w+", RegexOptions::None, L"=====1\v\u212A4==========1\ta\u0130Aa",
				                 {StrcutAllMatchesItem(5, 4, L"1\v\u212A4"), StrcutAllMatchesItem(19, 6, L"1\ta\u0130Aa")}),
				StrcutAllMatches(L"\\u221E|\\u2713", RegexOptions::None, L"infinity \u221E and checkmark \u2713 are contained here",
				                 {StrcutAllMatchesItem(9, 1, L"\u221E"), StrcutAllMatchesItem(25, 1, L"\u2713")}),

				// Whitespace
				StrcutAllMatches(L"\\s+", RegexOptions::None, L"===== \n\t\v\r ====", {StrcutAllMatchesItem(5, 6, L" \n\t\v\r ")}),

				// Unicode character classes, the input string uses the first element of each character class 
				StrcutAllMatches(L"\\p{Lu}\\p{Ll}\\p{Lt}\\p{Lm}\\p{Lo}\\p{Mn}\\p{Mc}\\p{Me}\\p{Nd}\\p{Nl}", RegexOptions::None,
				                 L"=====Aa\u01C5\u02B0\u01BB\u0300\u0903\u04880\u16EE===",
				                 {StrcutAllMatchesItem(5, 10, L"Aa\u01C5\u02B0\u01BB\u0300\u0903\u04880\u16EE")}),

				StrcutAllMatches(L"\\p{No}\\p{Zs}\\p{Zl}\\p{Zp}\\p{Cc}\\p{Cf}\\p{Cs}\\p{Co}\\p{Pc}\\p{Pd}",
				                 RegexOptions::None,
				                 L"=====" + UtilsString::StringInitSpe(0x00B2) + L" " + UtilsString::StringInitSpe(0x2028) + UtilsString::StringInitSpe(0x2029) + UtilsString::StringInitSpe(0x0) + L"\u0600" + UtilsString::StringInitSpe(0xD800) + L"\uE000_\u002D===",
				                 {StrcutAllMatchesItem(5, 10, L"\u00B2 \u2028\u2029" + UtilsString::StringInitSpe(0x0) + L"\u0600" + UtilsString::StringInitSpe(0xD800) + L"\uE000_\u002D")}),

				StrcutAllMatches(L"\\p{Ps}\\p{Pe}\\p{Pi}\\p{Pf}\\p{Po}\\p{Sm}\\p{Sc}\\p{Sk}\\p{So}\\p{Cn}",
				                 RegexOptions::None,
				                 L"=====()\xAB\xBB!+$^\xA6\u0378===",
				                 {StrcutAllMatchesItem(5, 10, L"()\xAB\xBB!+$^\xA6\u0378")}),
				StrcutAllMatches(L"\\p{Lu}\\p{Ll}\\p{Lt}\\p{Lm}\\p{Lo}\\p{Mn}\\p{Mc}\\p{Me}\\p{Nd}\\p{Nl}\\p{No}\\p{Zs}\\p{Zl}\\p{Zp}\\p{Cc}\\p{Cf}\\p{Cs}\\p{Co}\\p{Pc}\\p{Pd}\\p{Ps}\\p{Pe}\\p{Pi}\\p{Pf}\\p{Po}\\p{Sm}\\p{Sc}\\p{Sk}\\p{So}\\p{Cn}",
				                 RegexOptions::None,
				                 L"=====Aa" + UtilsString::StringInitSpe(0x01C5) + UtilsString::StringInitSpe(0x02B0) + UtilsString::StringInitSpe(0x01BB) +
				                 UtilsString::StringInitSpe(0x0300) + UtilsString::StringInitSpe(0x0903) + UtilsString::StringInitSpe(0x0488) + L"0" +
				                 UtilsString::StringInitSpe(0x16EE) + L"\xB2 " + UtilsString::StringInitSpe(0x2028) + UtilsString::StringInitSpe(0x2029) + UtilsString::StringInitSpe(0x0)
				                 + UtilsString::StringInitSpe(0x0600) + UtilsString::StringInitSpe(0xD800) + UtilsString::StringInitSpe(0xE000)
				                 + L"_\x2D()\xAB\xBB!+$^\xA6\u0378===",


				                 {StrcutAllMatchesItem(5, 30, L"Aa\u01C5\u02B0\u01BB\u0300\u0903\u04880\u16EE\xB2 \u2028\u2029" + UtilsString::StringInitSpe(0x0) + L"\u0600" + UtilsString::StringInitSpe(0xD800) + L"\uE000_\x2D()\xAB\xBB!+$^\xA6\u0378")}),

				// Case insensitive cases by using ?i and some non-ASCII characters like Kelvin sign and applying ?i over negated character classes
				StrcutAllMatches(L"(?i:[a-d\u00D5]+k*)", RegexOptions::None, L"xyxaB\u00F5c\u212AKAyy", {StrcutAllMatchesItem(3, 6, L"aB\u00F5c\u212AK"), StrcutAllMatchesItem(9, 1, L"A")}),
				StrcutAllMatches(L"(?i:[a-d]+)", RegexOptions::None, L"xyxaBcyy", {StrcutAllMatchesItem(3, 3, L"aBc")}),
				StrcutAllMatches(L"(?i:[" + UtilsString::StringInitSpe(0x0000) + L"-@B-" + UtilsString::StringInitSpe(0xFFFF) + L"]+)", RegexOptions::None, L"xaAaAy", {StrcutAllMatchesItem(0, 6, L"xaAaAy")}), // this is the same as .+
				StrcutAllMatches(L"(?i:[" + UtilsString::StringInitSpe(0x0000) + L"-ac-" + UtilsString::StringInitSpe(0xFFFF) + L"])", RegexOptions::None, L"b", {StrcutAllMatchesItem(0, 1, L"b")}),
				StrcutAllMatches(L"(?i:[" + UtilsString::StringInitSpe(0x0000) + L"-PR-" + UtilsString::StringInitSpe(0xFFFF) + L"])", RegexOptions::None, L"Q", {StrcutAllMatchesItem(0, 1, L"Q")}),
				StrcutAllMatches(L"(?i:[" + UtilsString::StringInitSpe(0x0000) + L"-pr-" + UtilsString::StringInitSpe(0xFFFF) + L"])", RegexOptions::None, L"q", {StrcutAllMatchesItem(0, 1, L"q")}),
				StrcutAllMatches(L"(?i:[^a])", RegexOptions::None, L"aAaA", {}), // this correponds to not{a,A}
				StrcutAllMatches(L"(?i:[" + UtilsString::StringInitSpe(0x0000) + L"-" + UtilsString::StringInitSpe(0xFFFF) + L"-[A]])", RegexOptions::None, L"aAaA", {}), // this correponds to not{a,A}
				StrcutAllMatches(L"(?i:[^Q])", RegexOptions::None, L"q", {}),
				StrcutAllMatches(L"(?i:[^b])", RegexOptions::None, L"b", {}),

				// this correponds to not{a,A}
				StrcutAllMatches(L"(?i:[^Q])", RegexOptions::None, L"q", {}),
				StrcutAllMatches(L"(?i:[^b])", RegexOptions::None, L"b", {}),

				// Use of anchors

				StrcutAllMatches(L"\\b\\w+nn\\b", RegexOptions::None, L"both Anne and Ann are names that contain nn", {StrcutAllMatchesItem(14, 3, L"Ann")}),

				StrcutAllMatches(L"\\B x", RegexOptions::None, L" xx", {StrcutAllMatchesItem(0, 2, L" x")}),

				StrcutAllMatches(L"\\bxx\\b", RegexOptions::None, L" zxx:xx", {StrcutAllMatchesItem(5, 2, L"xx")}),

				StrcutAllMatches(L"^abc*\\B", (RegexOptions)((int)RegexOptions::None | (int)RegexOptions::Multiline), L"\nabcc \nabcccd\n", {StrcutAllMatchesItem(1, 3, L"abc"), StrcutAllMatchesItem(7, 5, L"abccc")}),

				StrcutAllMatches(L"^abc", RegexOptions::None, L"abcccc", {StrcutAllMatchesItem(0, 3, L"abc")}),

				StrcutAllMatches(L"^abc", RegexOptions::None, L"aabcccc", {}),

				StrcutAllMatches(L"abc$", RegexOptions::None, L"aabcccc", {}),

				StrcutAllMatches(L"abc\\z", RegexOptions::None, L"aabc\n", {}),

				StrcutAllMatches(L"abc\\Z", RegexOptions::None, L"aabc\n", {StrcutAllMatchesItem(1, 3, L"abc")}),

				StrcutAllMatches(L"abc$", RegexOptions::None, L"aabc\nabc", {StrcutAllMatchesItem(5, 3, L"abc")}),

				StrcutAllMatches(L"abc$", (RegexOptions)((int)RegexOptions::None | (int)RegexOptions::Multiline), L"aabc\nabc", {StrcutAllMatchesItem(1, 3, L"abc"), StrcutAllMatchesItem(5, 3, L"abc")}),

				StrcutAllMatches(L"a\\bb", RegexOptions::None, L"ab", {}),

				StrcutAllMatches(L"a\\Bb", RegexOptions::None, L"ab", {StrcutAllMatchesItem(0, 2, L"ab")}),

				StrcutAllMatches(L"(a\\Bb|a\\bb)", RegexOptions::None, L"ab", {StrcutAllMatchesItem(0, 2, L"ab")}),

				StrcutAllMatches(L"a$", (RegexOptions)((int)RegexOptions::None | (int)RegexOptions::Multiline), L"b\na", {StrcutAllMatchesItem(2, 1, L"a")}),

				// Various loop constructs

				StrcutAllMatches(L"a[bcd]{4,5}(.)", RegexOptions::None, L"acdbcdbe", {StrcutAllMatchesItem(0, 7, L"acdbcdb")}),

				StrcutAllMatches(L"a[bcd]{4,5}?(.)", RegexOptions::None, L"acdbcdbe", {StrcutAllMatchesItem(0, 6, L"acdbcd")}),

				StrcutAllMatches(L"(x{3})+", RegexOptions::None, L"abcxxxxxxxxacacaca", {StrcutAllMatchesItem(3, 6, L"xxxxxx")}),

				StrcutAllMatches(L"(x{3})+?", RegexOptions::None, L"abcxxxxxxxxacacaca", {StrcutAllMatchesItem(3, 3, L"xxx"), StrcutAllMatchesItem(6, 3, L"xxx")}),

				StrcutAllMatches(L"a[0-9]+0", RegexOptions::None, L"ababca123000xyz", {StrcutAllMatchesItem(5, 7, L"a123000")}),

				StrcutAllMatches(L"a[0-9]+?0", RegexOptions::None, L"ababca123000xyz", {StrcutAllMatchesItem(5, 5, L"a1230")}),
				// Mixed lazy/eager loop

				StrcutAllMatches(L"a[0-9]+?0|b[0-9]+0", RegexOptions::None, L"ababca123000xyzababcb123000xyz", {StrcutAllMatchesItem(5, 5, L"a1230"), StrcutAllMatchesItem(20, 7, L"b123000")}),
				// Loops around alternations

				StrcutAllMatches(L"^(?:aaa|aa)*$", RegexOptions::None, L"aaaaaaaa", {StrcutAllMatchesItem(0, 8, L"aaaaaaaa")}),

				StrcutAllMatches(L"^(?:aaa|aa)*?$", RegexOptions::None, L"aaaaaaaa", {StrcutAllMatchesItem(0, 8, L"aaaaaaaa")}),

				StrcutAllMatches(L"^(?:aaa|aa){1,5}$", RegexOptions::None, L"aaaaaaaa", {StrcutAllMatchesItem(0, 8, L"aaaaaaaa")}),

				StrcutAllMatches(L"^(?:aaa|aa){1,5}?$", RegexOptions::None, L"aaaaaaaa", {StrcutAllMatchesItem(0, 8, L"aaaaaaaa")}),

				StrcutAllMatches(L"^(?:aaa|aa){4}$", RegexOptions::None, L"aaaaaaaa", {StrcutAllMatchesItem(0, 8, L"aaaaaaaa")}),

				StrcutAllMatches(L"^(?:aaa|aa){4}?$", RegexOptions::None, L"aaaaaaaa", {StrcutAllMatchesItem(0, 8, L"aaaaaaaa")}),

				// Mostly empty matches using unusual regexes consisting mostly of anchors only

				StrcutAllMatches(L"^", RegexOptions::None, L"", {StrcutAllMatchesItem(0, 0, L"")}),

				StrcutAllMatches(L"$", RegexOptions::None, L"", {StrcutAllMatchesItem(0, 0, L"")}),

				StrcutAllMatches(L"^$", RegexOptions::None, L"", {StrcutAllMatchesItem(0, 0, L"")}),

				StrcutAllMatches(L"$^", RegexOptions::None, L"", {StrcutAllMatchesItem(0, 0, L"")}),

				StrcutAllMatches(L"$^$$^^$^$", RegexOptions::None, L"", {StrcutAllMatchesItem(0, 0, L"")}),

				StrcutAllMatches(L"a*", RegexOptions::None, L"bbb", {StrcutAllMatchesItem(0, 0, L""), StrcutAllMatchesItem(1, 0, L""), StrcutAllMatchesItem(2, 0, L""), StrcutAllMatchesItem(3, 0, L"")}),

				StrcutAllMatches(L"a*", RegexOptions::None, L"baaabb", {StrcutAllMatchesItem(0, 0, L""), StrcutAllMatchesItem(1, 3, L"aaa"), StrcutAllMatchesItem(4, 0, L""), StrcutAllMatchesItem(5, 0, L""), StrcutAllMatchesItem(6, 0, L"")}),

				StrcutAllMatches(L"\\b", RegexOptions::None, L"hello--world", {StrcutAllMatchesItem(0, 0, L""), StrcutAllMatchesItem(5, 0, L""), StrcutAllMatchesItem(7, 0, L""), StrcutAllMatchesItem(12, 0, L"")}),

				StrcutAllMatches(L"\\B", RegexOptions::None, L"hello--world", {StrcutAllMatchesItem(1, 0, L""), StrcutAllMatchesItem(2, 0, L""), StrcutAllMatchesItem(3, 0, L""), StrcutAllMatchesItem(4, 0, L""), StrcutAllMatchesItem(6, 0, L""), StrcutAllMatchesItem(8, 0, L""), StrcutAllMatchesItem(9, 0, L""), StrcutAllMatchesItem(10, 0, L""), StrcutAllMatchesItem(11, 0, L"")}
				),

				// Involving many different characters in the same regex

				StrcutAllMatches(L"(abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789<>:;@)+", RegexOptions::None,
				                 L"=====abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789<>:;@abcdefg======",
				                 {StrcutAllMatchesItem(5, 67, L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789<>:;@")}),

				//this will need a total of 2x70 + 2 parts in the partition of NonBacktracking


				StrcutAllMatches(pattern, RegexOptions::None, input, {StrcutAllMatchesItem(5, length, ssss)})
			};
		}

		static void AllMatches(wstring pattern, RegexOptions options, wstring input, vector<StrcutAllMatchesItem> matches)
		{
			Regex re = Regex(pattern, options);
			auto m = re.Match(&input);
			if (matches.size() == 0)
			{
				Assert::False(m->Success);
			}
			else
			{
				int i = 0;
				do
				{
					Assert::True(m->Success);
					Assert::True((size_t)i < matches.size());
					Assert::True(matches[i].Item1 == m->Index);
					Assert::True(matches[i].Item2 == m->Length);
					Assert::True(matches[i++].Item3 == m->Value);
					m = m->NextMatch();
				}
				while (m->Success);
				Assert::True(matches.size() == i);
			}
		}

		static void StandardCharSets_SameMeaningAcrossAllEngines(wstring singleCharPattern)
		{
			vector<Regex*> regexes;

			regexes.push_back(new Regex(singleCharPattern));

			for (int c = 0; c <= 0xFFFF; c++)
			{
				wstring s = wstring(1, (wchar_t)c);
				bool baseline = regexes[0]->IsMatch(s);
				VerifyIsMatch(regexes[0], s, baseline, Regex::InfiniteMatchTimeout);
			}

			delete regexes[0];
		}

		static void VerifyIsMatchThrows(Regex* r, wstring input, TimeSpan timeout, wstring* pattern = NULL, RegexOptions options = RegexOptions::None)
		{
			if (r == NULL)
			{
				AssertThrows(RegexMatchTimeoutException, timeout == Regex::InfiniteMatchTimeout ? Regex::IsMatch(input, *pattern, options) : Regex::IsMatch(input, *pattern, options, timeout))
			}
			else
			{
				AssertThrows(RegexMatchTimeoutException, r->IsMatch(input))
			}
		}

		static void VerifyIsMatch(Regex* r, wstring input, bool expected, TimeSpan timeout, wstring* pattern = NULL, RegexOptions options = RegexOptions::None)
		{
			if (r == NULL)
			{
				Assert::True(expected == (timeout == Regex::InfiniteMatchTimeout ? Regex::IsMatch(input, *pattern, options) : Regex::IsMatch(input, *pattern, options, timeout)));
				if (options == RegexOptions::None)
				{
					Assert::True(expected == Regex::IsMatch(input, *pattern));
				}
			}
			else
			{
				Assert::True(expected == r->IsMatch(input));
			}
		}

		static void Match_DisjunctionOverCounting(wstring disjunct1, wstring disjunct2, wstring input, bool success, wstring match)
		{
			Regex re = Regex(disjunct1 + L"|" + disjunct2);
			auto m = re.Match(&input);
			Assert::True(success == m->Success);
			Assert::True(match == m->Value);
		}

		static vector<StructMatchAmbiguousRegexes_TestData> MatchAmbiguousRegexes_TestData()
		{
			return
			{
				StructMatchAmbiguousRegexes_TestData(L"(a|ab|c|bcd){0,}d*", L"ababcd", 0, 1),
				StructMatchAmbiguousRegexes_TestData(L"(a|ab|c|bcd){0,10}d*", L"ababcd", 0, 1),
				StructMatchAmbiguousRegexes_TestData(L"(a|ab|c|bcd)*d*", L"ababcd", 0, 1),
				StructMatchAmbiguousRegexes_TestData(L"(the)\\s*([12][0-9]|3[01]|0?[1-9])", L"it is the 10:00 time", 6, 6),
				StructMatchAmbiguousRegexes_TestData(L"(ab|a|bcd|c){0,}d*", L"ababcd", 0, 6),
				StructMatchAmbiguousRegexes_TestData(L"(ab|a|bcd|c){0,10}d*", L"ababcd", 0, 6),
				StructMatchAmbiguousRegexes_TestData(L"(ab|a|bcd|c)*d*", L"ababcd", 0, 6),
				StructMatchAmbiguousRegexes_TestData(L"(the)\\s*(0?[1-9]|[12][0-9]|3[01])", L"it is the 10:00 time", 6, 5)
			};
		}

		static void MatchAmbiguousRegexes(wstring pattern, wstring input, int Item1, int Item2)
		{
			Regex r = Regex(pattern);
			auto match = r.Match(&input);
			Assert::True(Item1 == match->Index);
			Assert::True(Item2 == match->Length);
		}

		static void MatchWordsInAnchoredRegexes(RegexOptions options, wstring pattern, wstring input, vector<StructMatchWordsInAnchoredRegexestem> matches)
		{
			// The aim of these test is to test corner cases of matches involving anchors
			// For NonBacktracking these tests are meant to
			// cover most contexts in _nullabilityForContext in SymbolicRegexNode
			Regex r = Regex(pattern, options);
			MatchCollection* ms = r.Matches(&input);
			Assert::True(matches.size() == ms->Count);
			for (size_t i = 0; i < matches.size(); i++)
			{
				Assert::True(ms->operator[](i)->Index == matches.operator[](i).Item1);
				Assert::True(ms->operator[](i)->Length == matches.operator[](i).Item2);
			}
		}

		static vector<StructMatchWordsInAnchoredRegexes> MatchWordsInAnchoredRegexes_TestData()
		{
			return
			{
				StructMatchWordsInAnchoredRegexes(RegexOptions::None, L"\\b\\w{10,}\\b", L"this is a complicated word in a\nnontrivial sentence", {StructMatchWordsInAnchoredRegexestem(10, 11), StructMatchWordsInAnchoredRegexestem(32, 10)}),
				StructMatchWordsInAnchoredRegexes(RegexOptions::Multiline, L"^\\w{10,}\\b", L"this is a\ncomplicated word in a\nnontrivial sentence", {StructMatchWordsInAnchoredRegexestem(10, 11), StructMatchWordsInAnchoredRegexestem(32, 10)}),
				StructMatchWordsInAnchoredRegexes(RegexOptions::None, L"\\b\\d{1,2}\\/\\d{1,2}\\/\\d{2,4}\\b", L"date 10/12/1966 and 10/12/66 are the same", {StructMatchWordsInAnchoredRegexestem(5, 10), StructMatchWordsInAnchoredRegexestem(20, 8)}),
				StructMatchWordsInAnchoredRegexes(RegexOptions::Multiline, L"\\b\\d{1,2}\\/\\d{1,2}\\/\\d{2,4}$", L"date 10/12/1966\nand 10/12/66\nare the same", {StructMatchWordsInAnchoredRegexestem(5, 10), StructMatchWordsInAnchoredRegexestem(20, 8)})

			};
		}
	};
}
