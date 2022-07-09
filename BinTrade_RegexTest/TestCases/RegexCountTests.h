#pragma once

#include "RegexOptions.h"
#include "UnitTestAssert.h"
#include "../RegexWpf/RegexGroupCollection.h"


using namespace Regexs;

namespace RegexsTest
{
	class RegexCountTests
	{
		struct StructRegexEngine
		{
			std::wstring pattern;
			std::wstring input;
			RegexOptions options;
			int expectedCount;

			StructRegexEngine(const std::wstring& pattern, const std::wstring& input, RegexOptions options, int expected_count)
				: pattern(pattern),
				  input(input),
				  options(options),
				  expectedCount(expected_count)
			{
			}
		};

	public:
		static void Run(bool test_throw)
		{
			Assert::WriteLine(L"RegexCountTests");


			auto value = Count_ReturnsExpectedCount_TestData();
			for (StructRegexEngine regex_engine : value)
			{
				Count_ReturnsExpectedCount(regex_engine.pattern, regex_engine.input, regex_engine.options, regex_engine.expectedCount);
			}

			if (test_throw)Count_InvalidArguments_Throws();
			if (test_throw)Count_Timeout_ThrowsAfterTooLongExecution();
		}

		static void Count_ReturnsExpectedCount(std::wstring pattern, std::wstring input, RegexOptions options, int expectedCount)
		{
			//Regex r = Regex(pattern, options);
			//Assert::True(expectedCount== r.Count(input));
			//Assert::Equal(r.Count(input), r.Matches(input)->Count);

			//if (options == RegexOptions::None)
			//{
			//	Assert::Equal(expectedCount, Regex::Count(input, pattern));
			//}

			//Assert::Equal(expectedCount, Regex::Count(input, pattern, options | engineOptions));
			//Assert::Equal(expectedCount, Regex::Count(input, pattern, options | engineOptions, Regex::InfiniteMatchTimeout));
		}

		static std::vector<StructRegexEngine> Count_ReturnsExpectedCount_TestData()
		{
			std::vector<StructRegexEngine> result =
			{
				StructRegexEngine(L"", L"", RegexOptions::None, 1),
				StructRegexEngine(L"", L"a", RegexOptions::None, 2),
				StructRegexEngine(L"", L"ab", RegexOptions::None, 3),

				StructRegexEngine(L"\\w", L"", RegexOptions::None, 0),
				StructRegexEngine(L"\\w", L"a", RegexOptions::None, 1),
				StructRegexEngine(L"\\w", L"ab", RegexOptions::None, 2),

				StructRegexEngine(L"\\b\\w+\\b", L"abc def ghi jkl", RegexOptions::None, 4),

				StructRegexEngine(L"A", L"", RegexOptions::IgnoreCase, 0),
				StructRegexEngine(L"A", L"a", RegexOptions::IgnoreCase, 1),
				StructRegexEngine(L"A", L"aAaA", RegexOptions::IgnoreCase, 4),

				StructRegexEngine(L".", L"\\n\\n\\n", RegexOptions::None, 0),
				StructRegexEngine(L".", L"\\n\\n\\n", RegexOptions::Singleline, 3)
			};
			return result;
		}

		static void Count_InvalidArguments_Throws()
		{
			//// input is null
			//AssertThrows(ArgumentNullException, Regex(L"pattern").Count(NULL))
			//AssertThrows(ArgumentNullException, Regex::Count(NULL, L"pattern"))
			//AssertThrows(ArgumentNullException, Regex::Count(NULL, L"pattern", RegexOptions::None))
			//AssertThrows(ArgumentNullException, Regex::Count(NULL, L"pattern", RegexOptions::None, TimeSpan::FromMilliseconds(1)))
			//			// pattern is null
			//			AssertExtensions.Throws<ArgumentNullException>("pattern", () = > Regex.Count("input", null));
			//			AssertExtensions.Throws<ArgumentNullException>("pattern", () = > Regex.Count("input", null, RegexOptions.None));
			//			AssertExtensions.Throws<ArgumentNullException>("pattern", () = > Regex.Count("input", null, RegexOptions.None, TimeSpan.FromMilliseconds(1)));
			//
			//			// pattern is invalid
			//#pragma warning disable RE0001 // invalid regex pattern
			//			AssertExtensions.Throws<RegexParseException>(() = > Regex.Count("input", @"[abc"));
			//			AssertExtensions.Throws<RegexParseException>(() = > Regex.Count("input", @"[abc", RegexOptions.None));
			//			AssertExtensions.Throws<RegexParseException>(() = > Regex.Count("input", @"[abc", RegexOptions.None, TimeSpan.FromMilliseconds(1)));
			//#pragma warning restore RE0001
			//
			//				// options is invalid
			//				AssertExtensions.Throws<ArgumentOutOfRangeException>("options", () = > Regex.Count("input", @"[abc]", (RegexOptions)(-1)));
			//				AssertExtensions.Throws<ArgumentOutOfRangeException>("options", () = > Regex.Count("input", @"[abc]", (RegexOptions)(-1), TimeSpan.FromMilliseconds(1)));
			//
			//				// matchTimeout is invalid
			//				AssertExtensions.Throws<ArgumentOutOfRangeException>("matchTimeout", () = > Regex.Count("input", @"[abc]", RegexOptions.None, TimeSpan.FromMilliseconds(-2)));
		}

		static void Count_Timeout_ThrowsAfterTooLongExecution()
		{
			//const string Pattern = @"^(\w + \s ? )*$";
			//const string Input = "An input string that takes a very very very very very very very very very very very long time!";

			//Regex r = await
			//RegexHelpers.GetRegexAsync(engine, Pattern, RegexOptions::None, TimeSpan::FromMilliseconds(1));

			//Stopwatch sw = Stopwatch.StartNew();
			//Assert::Throws<RegexMatchTimeoutException>(()= > r.Count(Input));
			//Assert::InRange(sw.Elapsed.TotalSeconds, 0, 10); // arbitrary upper bound that should be well above what's needed with a 1ms timeout
		}
	};
}
