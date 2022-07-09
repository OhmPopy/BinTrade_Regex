#pragma once

#include "UnitTestAssert.h"
#include "Other/stringhelper.h"


using namespace Regexs;
namespace RegexsTest
{
	class RegexReplaceTests
	{
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

		struct ReplaseStruct
		{
			std::wstring pattern;
			std::wstring input;
			std::wstring replacement;
			RegexOptions options;
			int count;
			int start;
			std::wstring expected;

			ReplaseStruct(const std::wstring& pattern, const std::wstring& input, const std::wstring& replacement, RegexOptions options, int count, int start, const std::wstring& expected)
				: pattern(pattern),
				  input(input),
				  replacement(replacement),
				  options(options),
				  count(count),
				  start(start),
				  expected(expected)
			{
			}
		};

		struct StructReplaceCornerCases
		{
			std::wstring pattern;
			std::wstring input;
			std::wstring replacement;
			int count;
			std::wstring expectedoutput;
			RegexOptions opt;

			StructReplaceCornerCases(const std::wstring& pattern, const std::wstring& input, const std::wstring& replacement, int count, const std::wstring& expectedoutput, RegexOptions opt)
				: pattern(pattern),
				  input(input),
				  replacement(replacement),
				  count(count),
				  expectedoutput(expectedoutput),
				  opt(opt)
			{
			}
		};

		struct StructMatchEval
		{
			std::wstring pattern;
			std::wstring input;
			MatchEvaluator evaluator;
			RegexOptions options;
			int count;
			int start;
			std::wstring expected;

			StructMatchEval(const std::wstring& pattern, const std::wstring& input, const MatchEvaluator& evaluator, RegexOptions options, int count, int start, const std::wstring& expected)
				: pattern(pattern),
				  input(input),
				  evaluator(evaluator),
				  options(options),
				  count(count),
				  start(start),
				  expected(expected)
			{
			}
		};

	public:
		static void Run(bool test_throw)
		{
			Assert::WriteLine(L"RegexReplaceTests");

			auto value_1 = Replace_String_TestData();
			for (size_t i = 0; i < value_1.size(); ++i)
			{
				ReplaseStruct replase_struct = value_1[i];
				Replace(replase_struct.pattern.c_str(), replase_struct.input.c_str(), replase_struct.replacement.c_str(), replase_struct.options, replase_struct.count, replase_struct.start, replase_struct.expected.c_str());
			}

			auto value_2 = Replace_MatchEvaluator_TestData();
			for (size_t i = 0; i < value_2.size(); ++i)
			{
				StructMatchEval struct_match_eval = value_2[i];
				Replace_MatchEvaluator_Test(struct_match_eval.pattern.c_str(), struct_match_eval.input.c_str(), struct_match_eval.evaluator, struct_match_eval.options, struct_match_eval.count, struct_match_eval.start, struct_match_eval.expected.c_str());
			}

			Replace_MatchEvaluator_Test_NonBacktracking_Matra();

			Replace_NoMatch(RegexOptions::None);

			if (test_throw)Replace_Invalid();

			auto value_3 = TestReplaceCornerCases_TestData();
			for (size_t i = 0; i < value_3.size(); ++i)
			{
				auto item = value_3.at(i);
				TestReplaceCornerCases(item.pattern, item.input, item.replacement, item.count, item.expectedoutput, item.opt);
			}

			TestReplaceWithSubstitution(L"(\\$\\d+):(\\d+)", L"it costs $500000:55 I think", L"$$???:${2}", L"it costs $???:55 I think", RegexOptions::None);
			TestReplaceWithSubstitution(L"(\\d+)([a-z]+)", L"---12345abc---", L"$2$1", L"---abc12345---", RegexOptions::None);
			TestReplaceWithToUpperMatchEvaluator(L"(\\bis\\b)", L"this is it", L"this IS it", RegexOptions::None);
		}

		static vector<ReplaseStruct> Replace_String_TestData()
		{
			return
			{

				ReplaseStruct(L"a", L"bbbb", L"c", RegexOptions::None, 4, 3, L"bbbb"),
				ReplaseStruct(L"", L"   ", L"123", RegexOptions::None, 4, 0, L"123 123 123 123"),
				ReplaseStruct(L"icrosoft", L"MiCrOsOfT", L"icrosoft", RegexOptions::IgnoreCase, 9, 0, L"Microsoft"),
				ReplaseStruct(L"dog", L"my dog has fleas", L"CAT", RegexOptions::IgnoreCase, 16, 0, L"my CAT has fleas"),
				ReplaseStruct(L"a", L"aaaaa", L"b", RegexOptions::None, 2, 0, L"bbaaa"),
				ReplaseStruct(L"a", L"aaaaa", L"b", RegexOptions::None, 2, 3, L"aaabb"),

				// Stress
				ReplaseStruct(L".", wstring(1000, 'a'), L"b", RegexOptions::None, 1000, 0, wstring(1000, 'b')),

				// Undefined groups
				ReplaseStruct(L"(?<256>cat)\\s*(?<512>dog)", L"slkfjsdcat dogkljeah", L"STARTcat$2048$1024dogEND", RegexOptions::None, 20, 0, L"slkfjsdSTARTcat$2048$1024dogENDkljeah"),
				ReplaseStruct(L"(?<cat>cat)\\s*(?<dog>dog)", L"slkfjsdcat dogkljeah", L"START${catTWO}dogcat${dogTWO}END", RegexOptions::None, 20, 0, L"slkfjsdSTART${catTWO}dogcat${dogTWO}ENDkljeah"),

				// Replace with group numbers
				ReplaseStruct(L"([a-z]([a-z]([a-z]([a-z]([a-z]([a-z]([a-z]([a-z]([a-z]([a-z]([a-z]([a-z]([a-z]([a-z]([a-z])))))))))))))))", L"abcdefghiklmnop", L"$15", RegexOptions::None, 15, 0, L"p"),
				ReplaseStruct(L"([a-z]([a-z]([a-z]([a-z]([a-z]([a-z]([a-z]([a-z]([a-z]([a-z]([a-z]([a-z]([a-z]([a-z]([a-z])))))))))))))))", L"abcdefghiklmnop", L"$3", RegexOptions::None, 15, 0, L"cdefghiklmnop"),
				ReplaseStruct(L"D\\.(.+)", L"D.Bau", L"David $1", RegexOptions::None, 5, 0, L"David Bau"),


				// Undefined group
				ReplaseStruct(L"([a_z])(.+)", L"abc", L"$3", RegexOptions::None, 3, 0, L"$3"),
				ReplaseStruct(L"(?<256>cat)\\s*(?<512>dog)", L"slkfjsdcat dogkljeah", L"STARTcat$2048$1024dogEND", RegexOptions::None, 20, 0, L"slkfjsdSTARTcat$2048$1024dogENDkljeah"),

				// Valid cases
				ReplaseStruct(L"[^ ]+\\s(?<time>)", L"08/10/99 16:00", L"${time}", RegexOptions::None, 14, 0, L"16:00"),

				ReplaseStruct(L"(?<cat>cat)\\s*(?<dog>dog)", L"cat dog", L"${cat}est ${dog}est", RegexOptions::None, 7, 0, L"catest dogest"),
				ReplaseStruct(L"(?<cat>cat)\\s*(?<dog>dog)", L"slkfjsdcat dogkljeah", L"START${cat}dogcat${dog}END", RegexOptions::None, 20, 0, L"slkfjsdSTARTcatdogcatdogENDkljeah"),
				ReplaseStruct(L"(?<512>cat)\\s*(?<256>dog)", L"slkfjsdcat dogkljeah", L"START${512}dogcat${256}END", RegexOptions::None, 20, 0, L"slkfjsdSTARTcatdogcatdogENDkljeah"),
				ReplaseStruct(L"(?<256>cat)\\s*(?<512>dog)", L"slkfjsdcat dogkljeah", L"START${256}dogcat${512}END", RegexOptions::None, 20, 0, L"slkfjsdSTARTcatdogcatdogENDkljeah"),
				ReplaseStruct(L"(?<512>cat)\\s*(?<256>dog)", L"slkfjsdcat dogkljeah", L"STARTcat$256$512dogEND", RegexOptions::None, 20, 0, L"slkfjsdSTARTcatdogcatdogENDkljeah"),
				ReplaseStruct(L"(?<256>cat)\\s*(?<512>dog)", L"slkfjsdcat dogkljeah", L"STARTcat$512$256dogEND", RegexOptions::None, 20, 0, L"slkfjsdSTARTcatdogcatdogENDkljeah"),

				ReplaseStruct(L"(hello)cat\\s+dog(world)", L"hellocat dogworld", L"$1$$$2", RegexOptions::None, 19, 0, L"hello$world"),
				ReplaseStruct(L"(cat)\\s+(dog)", L"before textcat dogafter text", L". The following should be dog and it is $+. ", RegexOptions::None, 28, 0, L"before text. The following should be dog and it is dog. after text"),

				ReplaseStruct(L"(hello)\\s+(world)", L"What the hello world goodby", L"$&, how are you?", RegexOptions::None, 27, 0, L"What the hello world, how are you? goodby"),
				ReplaseStruct(L"(hello)\\s+(world)", L"What the hello world goodby", L"$0, how are you?", RegexOptions::None, 27, 0, L"What the hello world, how are you? goodby"),
				ReplaseStruct(L"(hello)\\s+(world)", L"What the hello world goodby", L"$`cookie are you doing", RegexOptions::None, 27, 0, L"What the What the cookie are you doing goodby"),
				ReplaseStruct(L"(cat)\\s+(dog)", L"before textcat dogafter text", L". This is the $' and ", RegexOptions::None, 28, 0, L"before text. This is the after text and after text"),
				ReplaseStruct(L"(cat)\\s+(dog)", L"before textcat dogafter text", L". The following should be the entire string '$_'. ", RegexOptions::None, 28, 0, L"before text. The following should be the entire string 'before textcat dogafter text'. after text"),

				ReplaseStruct(L"(hello)\\s+(world)", L"START hello    world END", L"$2 $1 $1 $2 $3$4", RegexOptions::None, 24, 0, L"START world hello hello world $3$4 END"),
				ReplaseStruct(L"(hello)\\s+(world)", L"START hello    world END", L"$2 $1 $1 $2 $123$234", RegexOptions::None, 24, 0, L"START world hello hello world $123$234 END"),

				ReplaseStruct(L"(d)(o)(g)(\\s)(c)(a)(t)(\\s)(h)(a)(s)", L"My dog cat has fleas.", L"$01$02$03$04$05$06$07$08$09$10$11", (RegexOptions)((int)RegexOptions::CultureInvariant | (int)RegexOptions::IgnoreCase | (int)RegexOptions::Multiline), 21, 0, L"My dog cat has fleas."),
				ReplaseStruct(L"(d)(o)(g)(\\s)(c)(a)(t)(\\s)(h)(a)(s)", L"My dog cat has fleas.", L"$05$06$07$04$01$02$03$08$09$10$11", (RegexOptions)((int)RegexOptions::CultureInvariant | (int)RegexOptions::IgnoreCase | (int)RegexOptions::Multiline), 21, 0, L"My cat dog has fleas."),

				// Error cases
				ReplaseStruct(L"(?<256>cat)\\s*(?<512>dog)", L"slkfjsdcat dogkljeah", L"STARTcat$512$", RegexOptions::None, 20, 0, L"slkfjsdSTARTcatdog$kljeah"),

				// ECMAScript
				ReplaseStruct(L"(?<512>cat)\\s*(?<256>dog)", L"slkfjsdcat dogkljeah", L"STARTcat${256}${512}dogEND", RegexOptions::ECMAScript, 20, 0, L"slkfjsdSTARTcatdogcatdogENDkljeah"),
				ReplaseStruct(L"(?<256>cat)\\s*(?<512>dog)", L"slkfjsdcat dogkljeah", L"STARTcat${512}${256}dogEND", RegexOptions::ECMAScript, 20, 0, L"slkfjsdSTARTcatdogcatdogENDkljeah"),
				ReplaseStruct(L"(?<1>cat)\\s*(?<2>dog)", L"slkfjsdcat dogkljeah", L"STARTcat$2$1dogEND", RegexOptions::ECMAScript, 20, 0, L"slkfjsdSTARTcatdogcatdogENDkljeah"),
				ReplaseStruct(L"(?<2>cat)\\s*(?<1>dog)", L"slkfjsdcat dogkljeah", L"STARTcat$1$2dogEND", RegexOptions::ECMAScript, 20, 0, L"slkfjsdSTARTcatdogcatdogENDkljeah"),
				ReplaseStruct(L"(?<512>cat)\\s*(?<256>dog)", L"slkfjsdcat dogkljeah", L"STARTcat$256$512dogEND", RegexOptions::ECMAScript, 20, 0, L"slkfjsdSTARTcatdogcatdogENDkljeah"),
				ReplaseStruct(L"(?<256>cat)\\s*(?<512>dog)", L"slkfjsdcat dogkljeah", L"START${256}dogcat${512}END", RegexOptions::ECMAScript, 20, 0, L"slkfjsdSTARTcatdogcatdogENDkljeah"),

				ReplaseStruct(L"(hello)\\s+world", L"START hello    world END", L"$234 $1 $1 $234 $3$4", RegexOptions::ECMAScript, 24, 0, L"START $234 hello hello $234 $3$4 END"),
				ReplaseStruct(L"(hello)\\s+(world)", L"START hello    world END", L"$2 $1 $1 $2 $3$4", RegexOptions::ECMAScript, 24, 0, L"START world hello hello world $3$4 END"),
				ReplaseStruct(L"(hello)\\s+(world)", L"START hello    world END", L"$2 $1 $1 $2 $123$234", RegexOptions::ECMAScript, 24, 0, L"START world hello hello world hello23world34 END"),
				ReplaseStruct(L"(?<12>hello)\\s+(world)", L"START hello    world END", L"$1 $12 $12 $1 $123$134", RegexOptions::ECMAScript, 24, 0, L"START world hello hello world hello3world34 END"),
				ReplaseStruct(L"(?<123>hello)\\s+(?<23>world)", L"START hello    world END", L"$23 $123 $123 $23 $123$234", RegexOptions::ECMAScript, 24, 0, L"START world hello hello world helloworld4 END"),
				ReplaseStruct(L"(?<123>hello)\\s+(?<234>world)", L"START hello    world END", L"$234 $123 $123 $234 $123456$234567", RegexOptions::ECMAScript, 24, 0, L"START world hello hello world hello456world567 END"),

				ReplaseStruct(L"(d)(o)(g)(\\s)(c)(a)(t)(\\s)(h)(a)(s)", L"My dog cat has fleas.", L"$01$02$03$04$05$06$07$08$09$10$11", (RegexOptions)((int)RegexOptions::CultureInvariant | (int)RegexOptions::ECMAScript | (int)RegexOptions::IgnoreCase | (int)RegexOptions::Multiline), 21, 0, L"My dog cat has fleas."),
				ReplaseStruct(L"(d)(o)(g)(\\s)(c)(a)(t)(\\s)(h)(a)(s)", L"My dog cat has fleas.", L"$05$06$07$04$01$02$03$08$09$10$11", (RegexOptions)((int)RegexOptions::CultureInvariant | (int)RegexOptions::ECMAScript | (int)RegexOptions::IgnoreCase | (int)RegexOptions::Multiline), 21, 0, L"My cat dog has fleas."),

				// RightToLeft
				ReplaseStruct(L"a", L"bbbb", L"c", RegexOptions::RightToLeft, 4, 3, L"bbbb"),
				ReplaseStruct(L"", L"   ", L"123", RegexOptions::RightToLeft, 4, 3, L"123 123 123 123"),
				ReplaseStruct(L"foo\\s+", L"0123456789foo4567890foo         ", L"bar", RegexOptions::RightToLeft, 32, 32, L"0123456789foo4567890bar"),
				ReplaseStruct(L"\\d", L"0123456789foo4567890foo         ", L"#", RegexOptions::RightToLeft, 17, 32, L"##########foo#######foo         "),
				ReplaseStruct(L"\\d", L"0123456789foo4567890foo         ", L"#", RegexOptions::RightToLeft, 7, 32, L"0123456789foo#######foo         "),
				ReplaseStruct(L"\\d", L"0123456789foo4567890foo         ", L"#", RegexOptions::RightToLeft, 0, 32, L"0123456789foo4567890foo         "),
				ReplaseStruct(L"\\d", L"0123456789foo4567890foo         ", L"#", RegexOptions::RightToLeft, -1, 32, L"##########foo#######foo         "),

				ReplaseStruct(L"([1-9])([1-9])([1-9])def", L"abc123def!", L"$0", RegexOptions::RightToLeft, -1, 10, L"abc123def!"),
				ReplaseStruct(L"([1-9])([1-9])([1-9])def", L"abc123def!", L"$1", RegexOptions::RightToLeft, -1, 10, L"abc1!"),
				ReplaseStruct(L"([1-9])([1-9])([1-9])def", L"abc123def!", L"$2", RegexOptions::RightToLeft, -1, 10, L"abc2!"),
				ReplaseStruct(L"([1-9])([1-9])([1-9])def", L"abc123def!", L"$3", RegexOptions::RightToLeft, -1, 10, L"abc3!"),
				ReplaseStruct(L"([1-9])([1-9])([1-9])def", L"abc123def!", L"$4", RegexOptions::RightToLeft, -1, 10, L"abc$4!"),

				ReplaseStruct(L"([1-9])([1-9])([1-9])def", L"abc123def!", L"$$", RegexOptions::RightToLeft, -1, 10, L"abc$!"),
				ReplaseStruct(L"([1-9])([1-9])([1-9])def", L"abc123def!", L"$&", RegexOptions::RightToLeft, -1, 10, L"abc123def!"),
				ReplaseStruct(L"([1-9])([1-9])([1-9])def", L"abc123def!", L"$`", RegexOptions::RightToLeft, -1, 10, L"abcabc!"),
				ReplaseStruct(L"([1-9])([1-9])([1-9])def", L"abc123def!", L"$'", RegexOptions::RightToLeft, -1, 10, L"abc!!"),

				ReplaseStruct(L"([1-9])([1-9])([1-9])def", L"abc123def!", L"$+", RegexOptions::RightToLeft, -1, 10, L"abc3!"),
				ReplaseStruct(L"([1-9])([1-9])([1-9])def", L"abc123def!", L"$_", RegexOptions::RightToLeft, -1, 10, L"abcabc123def!!"),

				// Anchors
				ReplaseStruct(L"\\Ga", L"aaaaa", L"b", RegexOptions::None, 5, 0, L"bbbbb")
			};
		};

		static void Replace(wstring pattern, wstring input, wstring replacement, RegexOptions options, int count, int start, wstring expected)
		{
			Regex r = Regex(pattern, options);

			bool isDefaultStart = IsDefaultStart(input, options, start);
			bool isDefaultCount = IsDefaultCount(input, options, count);

			if (isDefaultStart)
			{
				if (isDefaultCount)
				{
					auto value_get_1 = r.Replace(input, replacement);
					auto value_get_2 = Regex::Replace(input, pattern, replacement, options);
					Assert::True(expected == value_get_1);
					Assert::True(expected == value_get_2);
				}

				Assert::True(expected == r.Replace(input, replacement, count));
			}

			Assert::True(expected == r.Replace(input, replacement, count, start));

		}

		static std::wstring MatchEvaluator1(Match* match)
		{
			return StringHelper::toLower(std::wstring(match->Value)) == L"big" ? L"Huge" : L"Tiny";
		}

		static std::wstring MatchEvaluator2(Match* match)
		{
			return L"SUCCESS";
		}

		static std::wstring MatchEvaluator3(Match* match)
		{
			if (match->Value == L"a" || match->Value == L"b" || match->Value == L"c")
				//return std::wstring(match->Value).ToUpperInvariant();
				return StringHelper::toUpper(match->Value).c_str();
			return L"";
		}

		static std::wstring MatchEvaluatorBold(Match* match)
		{
			return L"<b>" + match->Value + L"</b>";
		}

		static std::wstring MatchEvaluatorBar(Match* match)
		{
			return L"bar";
		}

		static std::wstring MatchEvaluatorPoundSign(Match* match)
		{
			return L"#";
		}

		static std::wstring Matchxxxxxn1(Match* match)
		{
			return L"uhoh";
		}

		static vector<StructMatchEval> Replace_MatchEvaluator_TestData()
		{
			// Regression test:
			// Regex treating Devanagari matra characters as matching "\b"
			// Unicode characters in the "Mark, NonSpacing" Category, U+0902=Devanagari sign anusvara, U+0947=Devanagri vowel sign E
			wstring boldInput = L"\u092f\u0939 \u0915\u0930 \u0935\u0939 \u0915\u0930\u0947\u0902 \u0939\u0948\u0964";
			wstring boldExpected = L"\u092f\u0939 <b>\u0915\u0930</b> \u0935\u0939 <b>\u0915\u0930\u0947\u0902</b> \u0939\u0948\u0964";

			return {

				StructMatchEval(L"a", L"bbbb", Matchxxxxxn1, RegexOptions::None, 4, 0, L"bbbb"),
				StructMatchEval(L"(Big|Small)", L"Big mountain", MatchEvaluator1, RegexOptions::None, 12, 0, L"Huge mountain"),
				StructMatchEval(L"(Big|Small)", L"Small village", MatchEvaluator1, RegexOptions::None, 13, 0, L"Tiny village"),


				StructMatchEval(L"(Big|Small)", L"sMaLl dog", MatchEvaluator1, RegexOptions::IgnoreCase, 9, 0, L"Tiny dog"),

				StructMatchEval(L".+", L"XSP_TEST_FAILURE", MatchEvaluator2, RegexOptions::None, 16, 0, L"SUCCESS"),
				StructMatchEval(L"[abcabc]", L"abcabc", MatchEvaluator3, RegexOptions::None, 6, 0, L"ABCABC"),
				StructMatchEval(L"[abcabc]", L"abcabc", MatchEvaluator3, RegexOptions::None, 3, 0, L"ABCabc"),
				StructMatchEval(L"[abcabc]", L"abcabc", MatchEvaluator3, RegexOptions::None, 3, 2, L"abCABc"),


				StructMatchEval(L"\\u0915\\u0930.*?\\b", boldInput, MatchEvaluatorBold, (RegexOptions)((int)RegexOptions::CultureInvariant | (int)RegexOptions::Singleline), boldInput.length(), 0, boldExpected),

				// RighToLeft
				StructMatchEval(L"a", L"bbbb", Matchxxxxxn1, RegexOptions::RightToLeft, 4, 3, L"bbbb"),
				StructMatchEval(L"foo\\s+", L"0123456789foo4567890foo         ", MatchEvaluatorBar, RegexOptions::RightToLeft, 32, 32, L"0123456789foo4567890bar"),
				StructMatchEval(L"\\d", L"0123456789foo4567890foo         ", MatchEvaluatorPoundSign, RegexOptions::RightToLeft, 17, 32, L"##########foo#######foo         "),
				StructMatchEval(L"\\d", L"0123456789foo4567890foo         ", MatchEvaluatorPoundSign, RegexOptions::RightToLeft, 7, 32, L"0123456789foo#######foo         "),
				StructMatchEval(L"\\d", L"0123456789foo4567890foo         ", MatchEvaluatorPoundSign, RegexOptions::RightToLeft, 0, 32, L"0123456789foo4567890foo         "),
				StructMatchEval(L"\\d", L"0123456789foo4567890foo         ", MatchEvaluatorPoundSign, RegexOptions::RightToLeft, -1, 32, L"##########foo#######foo         ")
			};
		};

		static void Replace_MatchEvaluator_Test(wstring pattern, wstring input, MatchEvaluator evaluator, RegexOptions options, int count, int start, wstring expected)
		{
			bool isDefaultStart = IsDefaultStart(input, options, start);
			bool isDefaultCount = IsDefaultCount(input, options, count);

			Regex r = Regex(pattern, options);

			if (isDefaultStart && isDefaultCount)
			{
				Assert::True(expected == r.Replace(input, evaluator));
			}

			if (isDefaultStart)
			{
				Assert::True(expected == r.Replace(input, evaluator, count));
			}

			Assert::True(expected == r.Replace(input, evaluator, count, start));

		}

		static void Replace_MatchEvaluator_Test_NonBacktracking_Matra()
		{
			// Regression test carried over from above to NonBacktracking mode:
			// Regex treating Devanagari matra characters as matching "\b"
			// Unicode characters in the "Mark, NonSpacing" Category, U+0902=Devanagari sign anusvara, U+0947=Devanagri vowel sign E
			wstring boldInput = L"\u092f\u0939 \u0915\u0930 \u0935\u0939 \u0915\u0930\u0947\u0902 \u0939\u0948\u0964";
			wstring boldExpected = L"\u092f\u0939 <b>\u0915\u0930</b> \u0935\u0939 <b>\u0915\u0930\u0947\u0902</b> \u0939\u0948\u0964";
			wstring pattern = L"\\u0915\\u0930.*?\\b";
			auto re = Regex(pattern, (RegexOptions)((int)RegexOptions::CultureInvariant | (int)RegexOptions::Singleline));
			Assert::True(boldExpected == re.Replace(boldInput, MatchEvaluatorBold));
		}

		static void Replace_NoMatch(RegexOptions options)
		{
			wstring input = L"";
			Assert::True(input == Regex::Replace(input, L"no-match", L"replacement", options));
			Assert::True(input == Regex::Replace(input, L"no-match", MatchEvaluator1, options));
		}

		static void Replace_Invalid()
		{
			//// Input is NULL
			//AssertThrows(ArgumentNullException, Regex::Replace(NULL, L"pattern", L"replacement"))
			//AssertThrows(ArgumentNullException, Regex::Replace(NULL, L"pattern", L"replacement", RegexOptions::None))
			//AssertThrows(ArgumentNullException, Regex::Replace(NULL, L"pattern", L"replacement", RegexOptions::None, TimeSpan::FromMilliseconds(1)))
			//AssertThrows(ArgumentNullException, Regex(L"pattern").Replace(NULL, L"replacement"))
			//AssertThrows(ArgumentNullException, Regex(L"pattern").Replace(NULL, L"replacement", 0))
			//AssertThrows(ArgumentNullException, Regex(L"pattern").Replace(NULL, L"replacement", 0, 0))

			//AssertThrows(ArgumentNullException, Regex::Replace(NULL, L"pattern", MatchEvaluator1))
			//AssertThrows(ArgumentNullException, Regex::Replace(NULL, L"pattern", MatchEvaluator1, RegexOptions::None))
			//AssertThrows(ArgumentNullException, Regex::Replace(NULL, L"pattern", MatchEvaluator1, RegexOptions::None, TimeSpan::FromMilliseconds(1)))
			//AssertThrows(ArgumentNullException, Regex(L"pattern").Replace(NULL, MatchEvaluator1))
			//AssertThrows(ArgumentNullException, Regex(L"pattern").Replace(NULL, MatchEvaluator1, 0))
			//AssertThrows(ArgumentNullException, Regex(L"pattern").Replace(NULL, MatchEvaluator1, 0, 0))


			//// Pattern is NULL
			//AssertThrows(ArgumentNullException, Regex::Replace(L"input", NULL, L"replacement"))
			//AssertThrows(ArgumentNullException, Regex::Replace(L"input", NULL, L"replacement", RegexOptions::None))
			//AssertThrows(ArgumentNullException, Regex::Replace(L"input", NULL, L"replacement", RegexOptions::None, TimeSpan::FromMilliseconds(1)))
			//AssertThrows(ArgumentNullException, Regex::Replace(L"input", NULL, MatchEvaluator1))
			//AssertThrows(ArgumentNullException, Regex::Replace(L"input", NULL, MatchEvaluator1, RegexOptions::None))
			//AssertThrows(ArgumentNullException, Regex::Replace(L"input", NULL, MatchEvaluator1, RegexOptions::None, TimeSpan::FromMilliseconds(1)))


			//// Replacement is NULL

			//AssertThrows(ArgumentNullException, Regex::Replace(L"input", L"pattern", (wstring)NULL))
			//AssertThrows(ArgumentNullException, Regex::Replace(L"input", L"pattern", (wstring)NULL, RegexOptions::None))
			//AssertThrows(ArgumentNullException, Regex::Replace(L"input", L"pattern", (wstring)NULL, RegexOptions::None, TimeSpan::FromMilliseconds(1)))
			//AssertThrows(ArgumentNullException, Regex(L"pattern").Replace(L"input", (wstring)NULL))
			//AssertThrows(ArgumentNullException, Regex(L"pattern").Replace(L"input", (wstring)NULL, 0))
			//AssertThrows(ArgumentNullException, Regex(L"pattern").Replace(L"input", (wstring)NULL, 0, 0))


			//AssertThrows(ArgumentNullException, Regex::Replace(L"input", L"pattern", (MatchEvaluator)NULL))
			//AssertThrows(ArgumentNullException, Regex::Replace(L"input", L"pattern", (MatchEvaluator)NULL, RegexOptions::None))
			//AssertThrows(ArgumentNullException, Regex::Replace(L"input", L"pattern", (MatchEvaluator)NULL, RegexOptions::None, TimeSpan::FromMilliseconds(1)))
			//AssertThrows(ArgumentNullException, Regex(L"pattern").Replace(L"input", (MatchEvaluator)NULL))
			//AssertThrows(ArgumentNullException, Regex(L"pattern").Replace(L"input", (MatchEvaluator)NULL, 0))
			//AssertThrows(ArgumentNullException, Regex(L"pattern").Replace(L"input", (MatchEvaluator)NULL, 0, 0))


			// Count is invalid
			AssertThrows(ArgumentOutOfRangeException, Regex(L"pattern").Replace(L"input", L"replacement", -2))
			AssertThrows(ArgumentOutOfRangeException, Regex(L"pattern").Replace(L"input", L"replacement", -2, 0))
			AssertThrows(ArgumentOutOfRangeException, Regex(L"pattern").Replace(L"input", MatchEvaluator1, -2))
			AssertThrows(ArgumentOutOfRangeException, Regex(L"pattern").Replace(L"input", MatchEvaluator1, -2, 0))


			// Start is invalid
			AssertThrows(ArgumentOutOfRangeException, Regex(L"pattern").Replace(L"input", L"replacement", 0, -1))
			AssertThrows(ArgumentOutOfRangeException, Regex(L"pattern").Replace(L"input", MatchEvaluator1, 0, -1))


			AssertThrows(ArgumentOutOfRangeException, Regex(L"pattern").Replace(L"input", L"replacement", 0, 6))
			AssertThrows(ArgumentOutOfRangeException, Regex(L"pattern").Replace(L"input", MatchEvaluator1, 0, 6))
		}

		static vector<StructReplaceCornerCases> TestReplaceCornerCases_TestData()
		{
			return
			{
				StructReplaceCornerCases(L"[ab]+", L"012aaabb34bba56", L"###", 0, L"012aaabb34bba56", RegexOptions::None),
				StructReplaceCornerCases(L"[ab]+", L"012aaabb34bba56", L"###", -1, L"012###34###56", RegexOptions::None),
				StructReplaceCornerCases(L"\\b", L"Hello World!", L"#", 2, L"#Hello# World!", RegexOptions::None),
				StructReplaceCornerCases(L"[ab]+", L"012aaabb34bba56", L"###", 1, L"012###34bba56", RegexOptions::None),
				StructReplaceCornerCases(L"\\b", L"Hello World!", L"#$$#", -1, L"#$#Hello#$# #$#World#$#!", RegexOptions::None),
				StructReplaceCornerCases(L"", L"hej", L"  ", -1, L"  h  e  j  ", RegexOptions::None),
				StructReplaceCornerCases(L"\\bis\\b", L"this is it", L"${2}", -1, L"this ${2} it", RegexOptions::None)
			};
		}

		static void TestReplaceCornerCases(wstring pattern, wstring input, wstring replacement, int count, wstring expectedoutput, RegexOptions opt)
		{
			Regex regex = Regex(pattern, opt);
			auto output = regex.Replace(input, replacement, count);
			Assert::True(expectedoutput == output);

		}

		static void TestReplaceWithSubstitution(wstring pattern, wstring input, wstring replacement, wstring expectedoutput, RegexOptions opt)
		{
			Regex regex = Regex(pattern, opt);
			auto output = regex.Replace(input, replacement, -1);
			Assert::True(expectedoutput == output);

		}

		static std::wstring Math_TestReplaceWithToUpperMatchEvaluator(Match* match)
		{
			return StringHelper::toUpper(match->Value);
		}

		static void TestReplaceWithToUpperMatchEvaluator(wstring pattern, wstring input, wstring expectedoutput, RegexOptions opt)
		{
			wstring output = Regex(pattern, opt).Replace(input, Math_TestReplaceWithToUpperMatchEvaluator);
			Assert::True(expectedoutput == output );
			auto output2 = Regex::Replace(input, pattern, Math_TestReplaceWithToUpperMatchEvaluator, opt);
			Assert::True(expectedoutput == output2);

		}
	};
}
