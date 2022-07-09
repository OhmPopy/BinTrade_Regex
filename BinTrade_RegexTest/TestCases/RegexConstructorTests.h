#pragma once

#include "RegexMatch.h"
#include "../RegexWpf/RegexGroupCollection.h"
 

namespace RegexsTest
{
	class RegexConstructorTests
	{
		struct structCtor
		{
			std::wstring pattern;
			RegexOptions options;
			TimeSpan matchTimeout;

			structCtor(const std::wstring& pattern, RegexOptions options, const TimeSpan& match_timeout)
				: pattern(pattern),
				  options(options),
				  matchTimeout(match_timeout)
			{
			}
		};

	public:
		static void Run(bool test_throw)
		{
			Assert::WriteLine(L"RegexConstructorTests");

			const auto datas = Ctor_TestData();
			for (const structCtor item : datas)
			{
				Ctor(item.pattern, item.options, item.matchTimeout);
			}
			CtorDebugInvoke(RegexOptions::None);

			if (test_throw)Ctor_Invalid();
		}

		static std::vector<structCtor> Ctor_TestData()
		{
			std::vector<structCtor> result = {
				structCtor(L"foo", RegexOptions::None, Regex::InfiniteMatchTimeout),
				structCtor(L"foo", RegexOptions::RightToLeft, Regex::InfiniteMatchTimeout),
				//structCtor(L"foo", RegexOptions::Compiled, Regex::InfiniteMatchTimeout),
				structCtor(L"foo", (RegexOptions)((int)RegexOptions::ECMAScript | (int)RegexOptions::IgnoreCase | (int)RegexOptions::Multiline | (int)RegexOptions::CultureInvariant), Regex::InfiniteMatchTimeout),
				//structCtor(L"foo", RegexOptions::ECMAScript | RegexOptions::IgnoreCase | RegexOptions::Multiline | RegexOptions::CultureInvariant | RegexOptions::Compiled, Regex::InfiniteMatchTimeout),
				structCtor(L"foo", RegexOptions::None, TimeSpan(1)),
				structCtor(L"foo", RegexOptions::None, TimeSpan::FromMilliseconds(INT_MAX - 1)),
			};
			return result;
		}

		static void Ctor(std::wstring pattern, RegexOptions options, TimeSpan matchTimeout)
		{
			if (matchTimeout == Regex::InfiniteMatchTimeout)
			{
				if (options == RegexOptions::None)
				{
					Regex regex1 = Regex(pattern);
					Assert::True(pattern == regex1.ToString());
					Assert::True(options == regex1.getOptions());
					Assert::False(regex1.getRightToLeft());
					Assert::True(matchTimeout == regex1.getMatchTimeout());
				}
				Regex regex2 = Regex(pattern, options);
				Assert::True(pattern == regex2.ToString());
				Assert::True(options == regex2.getOptions());
				Assert::True(((int)options & (int)RegexOptions::RightToLeft) != 0 == regex2.getRightToLeft());
				Assert::True(matchTimeout == regex2.getMatchTimeout());
			}
			Regex regex3 = Regex(pattern, options, matchTimeout);
			Assert::True(pattern == regex3.ToString());
			Assert::True(options == regex3.getOptions());
			Assert::True(((int)options & (int)RegexOptions::RightToLeft) != 0 == regex3.getRightToLeft());
			Assert::True(matchTimeout == regex3.getMatchTimeout());
		}

		static void CtorDebugInvoke(RegexOptions options)
		{
			std::wstring text_1 = L"a";
			std::wstring text_2 = L"adefghi";
			std::wstring text_3 = L"jkl";
			std::wstring text_4 = L"ghi";
			std::wstring text_5 = L"jkl";
			std::wstring text_6 = L"ghi";
			Regex r1 = Regex(L"[abc]def(ghi|jkl)", options);
			Assert::False(r1.Match(&text_1)->Success);
			Assert::True(r1.Match(&text_2)->Success);
			std::wstring repl = r1.Replace(L"123adefghi78bdefjkl9", L"###");
			Assert::True(L"123###78###9" == repl);

			Regex r2 = Regex(L"(ghi|jkl)*ghi", options);
			Assert::False(r2.Match(&text_3)->Success);
			Assert::True(r2.Match(&text_4)->Success);
			Assert::True(L"123456789" == r2.Replace(L"123ghi789", L"456"));

			Regex r3 = Regex(L"(ghi|jkl)*ghi", options, TimeSpan::FromDays(1));
			Assert::False(r3.Match(&text_5)->Success);
			Assert::True(r3.Match(&text_6)->Success);
			Assert::True(L"123456789" == r3.Replace(L"123ghi789", L"456"));
		}

		static void Ctor_Invalid()
		{
			//// Pattern is null todo alway pass
			// AssertThrows(ArgumentNullException, Regex(NULL))//string alway not null
			// AssertThrows(ArgumentNullException, Regex(NULL, RegexOptions::None))//string alway not null
			// AssertThrows(ArgumentNullException, Regex(NULL, RegexOptions::None,  TimeSpan()))//string alway not null


			// Options are invalid
			AssertThrows(ArgumentOutOfRangeException, Regex(L"foo", (RegexOptions)(-1)))
			AssertThrows(ArgumentOutOfRangeException, Regex(L"foo", (RegexOptions)(-1), TimeSpan()))


			AssertThrows(ArgumentOutOfRangeException, Regex(L"foo", (RegexOptions)0x800))
			AssertThrows(ArgumentOutOfRangeException, Regex(L"foo", (RegexOptions)0x800, TimeSpan()))


			AssertThrows(ArgumentOutOfRangeException, Regex(L"foo", (RegexOptions)((int)RegexOptions::ECMAScript |(int) RegexOptions::IgnoreCase | (int)RegexOptions::Multiline | (int)RegexOptions::CultureInvariant | (int)RegexOptions::RightToLeft)))
			AssertThrows(ArgumentOutOfRangeException, Regex(L"foo", (RegexOptions)((int)RegexOptions::ECMAScript | (int)RegexOptions::IgnoreCase | (int)RegexOptions::Multiline | (int)RegexOptions::CultureInvariant | (int)RegexOptions::ExplicitCapture)))
			AssertThrows(ArgumentOutOfRangeException, Regex(L"foo", (RegexOptions)((int)RegexOptions::ECMAScript | (int)RegexOptions::IgnoreCase | (int)RegexOptions::Multiline | (int)RegexOptions::CultureInvariant | (int)RegexOptions::Singleline)))
			AssertThrows(ArgumentOutOfRangeException, Regex(L"foo", (RegexOptions)((int)RegexOptions::ECMAScript | (int)RegexOptions::IgnoreCase | (int)RegexOptions::Multiline | (int)RegexOptions::CultureInvariant | (int)RegexOptions::IgnorePatternWhitespace)))


			// MatchTimeout is invalid
			AssertThrows(ArgumentOutOfRangeException, Regex(L"foo", RegexOptions::None, TimeSpan(-1)))
			AssertThrows(ArgumentOutOfRangeException, Regex(L"foo", RegexOptions::None, TimeSpan::Zero))
			AssertThrows(ArgumentOutOfRangeException, Regex(L"foo", RegexOptions::None, TimeSpan::FromMilliseconds(INT_MAX)))
		}
	};
}
