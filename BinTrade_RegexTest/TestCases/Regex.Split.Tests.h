#pragma once

#include "UnitTestAssert.h"

namespace RegexsTest
{
	class RegexSplitTests
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


		struct StructSplit
		{
			std::wstring pattern;
			std::wstring input;
			RegexOptions options;
			int count;
			int start;
			vector<std::wstring> expected;

			StructSplit(const std::wstring& pattern, const std::wstring& input, RegexOptions options, int count, int start, const vector<std::wstring>& expected)
				: pattern(pattern),
				  input(input),
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
			Assert::WriteLine(L"RegexSplitTests");
			GetCultureInfo::UserDefaultCulture();

			auto value1 = Split_TestData();
			for (size_t i = 0; i < value1.size(); ++i)
			{
				StructSplit struct_split = value1.at(i);
				Split(struct_split.pattern.c_str(), struct_split.input.c_str(), struct_split.options, struct_split.count, struct_split.start, struct_split.expected);
			}
			if (test_throw)Split_Invalid();
		}

		static vector<StructSplit> Split_TestData()
		{
			return
			{
				StructSplit(L"", L"", RegexOptions::None, 0, 0, {L"", L""}),
				StructSplit(L"123", L"abc", RegexOptions::None, 3, 0, {L"abc"}),

				StructSplit(L"    ", L"word0    word1    word2    word3", RegexOptions::None, 32, 0, {L"word0", L"word1", L"word2", L"word3"}),

				StructSplit(L":", L"kkk:lll:mmm:nnn:ooo", RegexOptions::None, 19, 0, {L"kkk", L"lll", L"mmm", L"nnn", L"ooo"}),
				StructSplit(L":", L"kkk:lll:mmm:nnn:ooo", RegexOptions::None, 0, 0, {L"kkk", L"lll", L"mmm", L"nnn", L"ooo"}),

				// IgnoreCase
				StructSplit(L"[abc]", L"1A2B3C4", RegexOptions::IgnoreCase, 7, 0, {L"1", L"2", L"3", L"4"}),

				// Custom index
				StructSplit(L":", L"kkk:lll:mmm:nnn:ooo", RegexOptions::None, 2, 0, {L"kkk", L"lll:mmm:nnn:ooo"}),
				StructSplit(L":", L"kkk:lll:mmm:nnn:ooo", RegexOptions::None, 3, 6, {L"kkk:lll", L"mmm", L"nnn:ooo"}),

				// Tricky corner cases involving empty matches of anchors
				StructSplit(L"\\b", L"Hello World!", RegexOptions::None, 3, 6, {L"Hello ", L"World", L"!"}),
				StructSplit(L"\\b", L"Hello World!", RegexOptions::None, 0, 0, {L"", L"Hello", L" ", L"World", L"!"}),
				StructSplit(L"^", L"Hello \nWorld!", (RegexOptions)((int)RegexOptions::None | (int)RegexOptions::Multiline), 0, 0, {L"", L"Hello \n", L"World!"}),
				StructSplit(L"$", L"Hello \nWorld!", (RegexOptions)((int)RegexOptions::None | (int)RegexOptions::Multiline), 0, 0, {L"Hello ", L"\nWorld!", L""}),
				StructSplit(L"(\\s)?(-)", L"once -upon-a time", RegexOptions::None, 17, 0, {L"once", L" ", L"-", L"upon", L"-", L"a time"}),
				StructSplit(L"(\\s)?(-)", L"once upon a time", RegexOptions::None, 16, 0, {L"once upon a time"}),
				StructSplit(L"(\\s)?(-)", L"once - -upon- a- time", RegexOptions::None, 21, 0, {L"once", L" ", L"-", L"", L" ", L"-", L"upon", L"-", L" a", L"-", L" time"}),

				StructSplit(L"a(.)c(.)e", L"123abcde456aBCDe789", RegexOptions::None, 19, 0, {L"123", L"b", L"d", L"456aBCDe789"}),
				StructSplit(L"a(.)c(.)e", L"123abcde456aBCDe789", RegexOptions::IgnoreCase, 19, 0, {L"123", L"b", L"d", L"456", L"B", L"D", L"789"}),

				StructSplit(L"a(?<dot1>.)c(.)e", L"123abcde456aBCDe789", RegexOptions::None, 19, 0, {L"123", L"d", L"b", L"456aBCDe789"}),
				StructSplit(L"a(?<dot1>.)c(.)e", L"123abcde456aBCDe789", RegexOptions::IgnoreCase, 19, 0, {L"123", L"d", L"b", L"456", L"D", L"B", L"789"}),


				
				// RightToLeft
				StructSplit(L"", L"", RegexOptions::RightToLeft, 0, 0, {L"", L""}),
				StructSplit(L"123", L"abc", RegexOptions::RightToLeft, 3, 0, {L"abc"}),

				StructSplit(L"a(.)c(.)e", L"123abcde456aBCDe789", RegexOptions::RightToLeft, 19, 19, {L"123", L"d", L"b", L"456aBCDe789"}),
				StructSplit(L"a(.)c(.)e", L"123abcde456aBCDe789", (RegexOptions)((int)RegexOptions::RightToLeft | (int)RegexOptions::IgnoreCase), 19, 19, {L"123", L"d", L"b", L"456", L"D", L"B", L"789"}),

				StructSplit(L"a(?<dot1>.)c(.)e", L"123abcde456aBCDe789", RegexOptions::RightToLeft, 19, 19, {L"123", L"b", L"d", L"456aBCDe789"}),
				StructSplit(L"a(?<dot1>.)c(.)e", L"123abcde456aBCDe789", (RegexOptions)((int)RegexOptions::RightToLeft | (int)RegexOptions::IgnoreCase), 19, 19, {L"123", L"b", L"d", L"456", L"B", L"D", L"789"}),

				StructSplit(L"foo", L"0123456789foo4567890foo         ", RegexOptions::RightToLeft, 32, 32, {L"0123456789", L"4567890", L"         "}),

				StructSplit(L"\\d", L"1a2b3c4d5e6f7g8h9i0k", RegexOptions::RightToLeft, 20, 20, {L"", L"a", L"b", L"c", L"d", L"e", L"f", L"g", L"h", L"i", L"k"}),
				StructSplit(L"\\d", L"1a2b3c4d5e6f7g8h9i0k", RegexOptions::RightToLeft, 10, 20, {L"1a", L"b", L"c", L"d", L"e", L"f", L"g", L"h", L"i", L"k"}),
				StructSplit(L"\\d", L"1a2b3c4d5e6f7g8h9i0k", RegexOptions::RightToLeft, 2, 20, {L"1a2b3c4d5e6f7g8h9i", L"k"}),
				StructSplit(L"\\d", L"1a2b3c4d5e6f7g8h9i0k", RegexOptions::RightToLeft, 1, 20, {L"1a2b3c4d5e6f7g8h9i0k"}),

				// Anchors
				StructSplit(L"(?<=\\G..)(?=..)", L"aabbccdd", RegexOptions::None, 8, 0, {L"aa", L"bb", L"cc", L"dd"})
			};
		}

		static void Split(wstring pattern, wstring input, RegexOptions options, int count, int start, vector<std::wstring> expected)
		{
			bool isDefaultStart = IsDefaultStart(input, options, start);
			bool isDefaultCount = IsDefaultCount(input, options, count);

			Regex r = Regex(pattern, options);

			if (isDefaultStart && isDefaultCount)
			{
				auto asdasdasd = r.Split(input);
				if (asdasdasd.size() == expected.size())
				{
					for (size_t i = 0; i < expected.size(); ++i)
					{
						Assert::True(expected[i] == asdasdasd.at(i));
					}
				}
				else
					Assert::False(true, L"Size");
			}

			if (isDefaultStart)
			{
				auto asdasdasd = r.Split(input, count);
				if (asdasdasd.size() == expected.size())
				{
					for (size_t i = 0; i < expected.size(); ++i)
					{
						Assert::True(expected[i] == r.Split(input, count).at(i));
					}
				}
				else
					Assert::False(true, L"Size");
			}
			auto asdasdasd = r.Split(input, count, start);
			if (asdasdasd.size() == expected.size())
			{
				for (size_t i = 0; i < expected.size(); ++i)
				{
					Assert::True(expected[i] == r.Split(input, count, start).at(i));
				}
			}
			else
				Assert::False(true, L"Size");

		}

		static void Split_Invalid()
		{
			// Input is null
			//AssertThrows(ArgumentNullException, Regex::Split(NULL, L"pattern"))
			//AssertThrows(ArgumentNullException, Regex::Split(NULL, L"pattern", RegexOptions::None))
			//AssertThrows(ArgumentNullException, Regex::Split(NULL, L"pattern", RegexOptions::None, TimeSpan::FromMilliseconds(1)))
			//AssertThrows(ArgumentNullException, Regex(L"pattern").Split(NULL))
			//AssertThrows(ArgumentNullException, Regex(L"pattern").Split(NULL, 0))
			//AssertThrows(ArgumentNullException, Regex(L"pattern").Split(NULL, 0, 0))


			// Pattern is null
			//AssertThrows(ArgumentNullException, Regex::Split(L"input", NULL))
			//AssertThrows(ArgumentNullException, Regex::Split(L"input", NULL, RegexOptions::None))
			//AssertThrows(ArgumentNullException, Regex::Split(L"input", NULL, RegexOptions::None, TimeSpan::FromMilliseconds(1)))

			// Count is invalid
			AssertThrows(ArgumentOutOfRangeException, Regex(L"pattern").Split(L"input", -1))
			AssertThrows(ArgumentOutOfRangeException, Regex(L"pattern").Split(L"input", -1, 0))

			// Start is invalid
			AssertThrows(ArgumentOutOfRangeException, Regex(L"pattern").Split(L"input", 0, -1))
			AssertThrows(ArgumentOutOfRangeException, Regex(L"pattern").Split(L"input", 0, 6))
		}
	};
}
