#pragma once

#include "RegexHelpers.h"
#include "UnitTestAssert.h"

namespace RegexsTest
{
	class RegexMultipleMatchesTests
	{
		struct StructMatches_TestData
		{
			std::wstring pattern;
			std::wstring input;
			RegexOptions options;
			vector<CaptureData> expected;

			StructMatches_TestData(const std::wstring& pattern, const std::wstring& input, RegexOptions options, const vector<CaptureData>& expected)
				: pattern(pattern),
				  input(input),
				  options(options),
				  expected(expected)
			{
			}
		};

	public:
		static void Run(bool test_throw)
		{
			Assert::WriteLine(L"RegexMultipleMatchesTests");

			Matches_MultipleCapturingGroups();
			auto value_1 = Matches_TestData();
			for (size_t i = 0; i < value_1.size(); ++i)
			{
				StructMatches_TestData struct_matches = value_1.at(i);
				Matches(struct_matches.pattern.c_str(), struct_matches.input.c_str(), struct_matches.options, struct_matches.expected);
			}

			if (test_throw) Matches_Invalid();
			NextMatch_EmptyMatch_ReturnsEmptyMatch();
		}

		static void Matches_MultipleCapturingGroups()
		{
			std::vector<wstring> expectedGroupValues = {L"abracadabra", L"abra", L"cad"};
			std::vector<wstring> expectedGroupCaptureValues = {L"abracad", L"abra"};

			// Another example - given by Brad Merril in an article on RegularExpressions
			Regex regex = Regex(L"(abra(cad)?)+");
			wstring input = L"abracadabra1abracadabra2abracadabra3";
			Match* match = regex.Match(&input);
			while (match->Success)
			{
				wstring expected = L"abracadabra";
				Assert::True(expected == match->ToString());


				Assert::True(3 == match->Groups->Count);
				for (int i = 0; i < match->Groups->Count; i++)
				{
					Assert::True(expectedGroupValues[i] == match->Groups->operator[](i)->ToString());
					if (i == 1)
					{
						Assert::True(2 == match->Groups->operator[](i)->Captures->Count);
						for (int j = 0; j < match->Groups->operator[](i)->Captures->Count; j++)
						{
							Assert::True(expectedGroupCaptureValues[j] == match->Groups->operator[](i)->Captures->operator[](j)->ToString());
						}
					}
					else if (i == 2)
					{
						Assert::True(1 == match->Groups->operator[](i)->Captures->Count);
						Assert::True(L"cad" == match->Groups->operator[](i)->Captures->operator[](0)->ToString());
					}
				}
				Assert::True(1 == match->Captures->Count);
				Assert::True(L"abracadabra" == match->Captures->operator[](0)->ToString());

				match = match->NextMatch();
			}

		}

		static vector<StructMatches_TestData> Matches_TestData()
		{
			return
			{
				StructMatches_TestData(L"[0-9]", L"12345asdfasdfasdfljkhsda67890", RegexOptions::None,
				                       {
					                       CaptureData(L"1", 0, 1),
					                       CaptureData(L"2", 1, 1),
					                       CaptureData(L"3", 2, 1),
					                       CaptureData(L"4", 3, 1),
					                       CaptureData(L"5", 4, 1),
					                       CaptureData(L"6", 24, 1),
					                       CaptureData(L"7", 25, 1),
					                       CaptureData(L"8", 26, 1),
					                       CaptureData(L"9", 27, 1),
					                       CaptureData(L"0", 28, 1),
				                       }),

				StructMatches_TestData(L"[a-z0-9]+", L"[token1]? GARBAGEtoken2GARBAGE ;token3!", RegexOptions::None,
				                       {
					                       CaptureData(L"token1", 1, 6),
					                       CaptureData(L"token2", 17, 6),
					                       CaptureData(L"token3", 32, 6)
				                       }),


				StructMatches_TestData(
					L"(abc){2}", L" !abcabcasl  dkfjasiduf 12343214-//asdfjzpiouxoifzuoxpicvql23r\\` #$3245,2345278 :asdfas & 100% @daeeffga (ryyy27343) poiweurwabcabcasdfalksdhfaiuyoiruqwer{234}/[(132387 + x)]'aaa''?", RegexOptions::None,
					{
						CaptureData(L"abcabc", 2, 6),
						CaptureData(L"abcabc", 125, 6)
					}),


				StructMatches_TestData(
					L"\\b\\w*\\b", L"handling words of various lengths", RegexOptions::None,
					{
						CaptureData(L"handling", 0, 8),
						CaptureData(L"", 8, 0),
						CaptureData(L"words", 9, 5),
						CaptureData(L"", 14, 0),
						CaptureData(L"of", 15, 2),
						CaptureData(L"", 17, 0),
						CaptureData(L"various", 18, 7),
						CaptureData(L"", 25, 0),
						CaptureData(L"lengths", 26, 7),
						CaptureData(L"", 33, 0),
					}),

				StructMatches_TestData(
					L"\\b\\w{2}\\b", L"handling words of various lengths", RegexOptions::None,
					{
						CaptureData(L"of", 15, 2),
					}),

				StructMatches_TestData(
					L"\\w{6,}", L"handling words of various lengths", RegexOptions::None,
					{
						CaptureData(L"handling", 0, 8),
						CaptureData(L"various", 18, 7),
						CaptureData(L"lengths", 26, 7)
					}),

				StructMatches_TestData(
					L"[a-z]", L"a", RegexOptions::None,
					{
						CaptureData(L"a", 0, 1)
					}
				),

				StructMatches_TestData(
					L"[a-z]", L"a1bc", RegexOptions::None,
					{
						CaptureData(L"a", 0, 1),
						CaptureData(L"b", 2, 1),
						CaptureData(L"c", 3, 1)
					}
				),

				StructMatches_TestData(
					L"(?:ab|cd|ef|gh|i)j", L"abj    cdj  efj           ghjij", RegexOptions::None,
					{
						CaptureData(L"abj", 0, 3),
						CaptureData(L"cdj", 7, 3),
						CaptureData(L"efj", 12, 3),
						CaptureData(L"ghj", 26, 3),
						CaptureData(L"ij", 29, 2),
					}),


				// Using ^ with multiline
				StructMatches_TestData(
					L"^", L"", RegexOptions::Multiline,
					{CaptureData(L"", 0, 0)}
				),

				StructMatches_TestData(
					L"^", L"\n\n\n", RegexOptions::Multiline,
					{
						CaptureData(L"", 0, 0),
						CaptureData(L"", 1, 0),
						CaptureData(L"", 2, 0),
						CaptureData(L"", 3, 0)
					}
				),

				StructMatches_TestData
				(
					L"^abc", L"abc\nabc \ndef abc \nab\nabc", RegexOptions::Multiline,
					{
						CaptureData(L"abc", 0, 3),
						CaptureData(L"abc", 4, 3),
						CaptureData(L"abc", 21, 3),
					}
				),

				StructMatches_TestData
				(
					L"^\\w{5}", L"abc\ndefg\n\nhijkl\n", RegexOptions::Multiline,
					{
						CaptureData(L"hijkl", 10, 5),
					}
				),

				StructMatches_TestData
				(
					L"^.*$", L"abc\ndefg\n\nhijkl\n", RegexOptions::Multiline,
					{
						CaptureData(L"abc", 0, 3),
						CaptureData(L"defg", 4, 4),
						CaptureData(L"", 9, 0),
						CaptureData(L"hijkl", 10, 5),
						CaptureData(L"", 16, 0),
					}
				),

				StructMatches_TestData
				(
					L".*", L"abc", RegexOptions::None,
					{
						CaptureData(L"abc", 0, 3),
						CaptureData(L"", 3, 0)
					}
				),

				//StructMatches_TestData
				//(
				//	// .NET Framework missing fix in https://github.com/dotnet/runtime/pull/1075
				//	L"[a -\\-\\b]", L"a #.", RegexOptions::None,
				//	{
				//		CaptureData(L"a", 0, 1),
				//		CaptureData(L" ", 1, 1),
				//		CaptureData(L"#", 2, 1),
				//	}
				//),


				StructMatches_TestData
				(
					L"foo\\d+", L"0123456789foo4567890foo1foo  0987", RegexOptions::RightToLeft,
					{
						CaptureData(L"foo1", 20, 4),
						CaptureData(L"foo4567890", 10, 10),
					}
				),

				StructMatches_TestData
				(
					L"(?(A)A123|C789)", L"A123 B456 C789", RegexOptions::None,
					{
						CaptureData(L"A123", 0, 4),
						CaptureData(L"C789", 10, 4),
					}
				),

				StructMatches_TestData
				(
					L"(?(A)A123|C789)", L"A123 B456 C789", RegexOptions::None,
					{
						CaptureData(L"A123", 0, 4),
						CaptureData(L"C789", 10, 4),
					}
				),

				StructMatches_TestData
				(
					L"(?(\\w+)\\w+|)", L"abcd", RegexOptions::None,
					{
						CaptureData(L"abcd", 0, 4),
						CaptureData(L"", 4, 0),
					}
				),


				//// .NET Framework has some behavioral inconsistencies when there's no else branch.
				//StructMatches_TestData
				//(
				//	L"(?(\\w+)\\w+)", L"abcd", RegexOptions::None,
				//	{
				//		CaptureData(L"abcd", 0, 4),
				//		CaptureData(L"", 4, 0),
				//	}
				//),

				StructMatches_TestData
				(
					L"^.*$", L"abc\ndefg\n\nhijkl\n", (RegexOptions)
					((int)RegexOptions::Multiline | (int)RegexOptions::RightToLeft),
					{
						CaptureData(L"", 16, 0),
						CaptureData(L"hijkl", 10, 5),
						CaptureData(L"", 9, 0),
						CaptureData(L"defg", 4, 4),
						CaptureData(L"abc", 0, 3),
					}
				),


				//// .NET Framework missing fix in https://github.com/dotnet/runtime/pull/993
				//StructMatches_TestData
				//(
				//	L"[^]", L"every", RegexOptions::ECMAScript,
				//	{
				//		CaptureData(L"e", 0, 1),
				//		CaptureData(L"v", 1, 1),
				//		CaptureData(L"e", 2, 1),
				//		CaptureData(L"r", 3, 1),
				//		CaptureData(L"y", 4, 1),
				//	}
				//)
			};
		}

		static void Matches(wstring pattern, wstring input, RegexOptions options, vector<CaptureData> expected)
		{
			Regex regexAdvanced = Regex(pattern, options);
			VerifyMatches(regexAdvanced.Matches(&input), expected);
			VerifyMatches(regexAdvanced.Match(&input), expected);

		}

		static void VerifyMatches(Match* match, vector<CaptureData> expected)
		{
			for (int i = 0; match->Success; i++, match = match->NextMatch())
			{
				if ((size_t)i < expected.size())
					VerifyMatch(match, expected[i]);
			}
		}

		static void VerifyMatches(MatchCollection* matches, vector<CaptureData> expected)
		{
			auto count = matches->Count;
			Assert::True(expected.size() == count);
			if (expected.size() == count)
			{
				for (size_t i = 0; i < expected.size(); i++)
				{
					VerifyMatch(matches->operator[](i), expected[i]);
				}
			}
		}

		static void VerifyMatch(Match* match, CaptureData expected)
		{
			Assert::True(match->Success);
			Assert::True(expected.Index == match->Index);
			Assert::True(expected.Length == match->Length);
			Assert::True(expected.Value == match->ToString());

			Assert::True(expected.Index == match->Groups->operator[](0)->Index);
			Assert::True(expected.Length == match->Groups->operator[](0)->Length);
			Assert::True(expected.Value == match->Groups->operator[](0)->ToString());

			Assert::True(1 == match->Captures->Count);
			Assert::True(expected.Index == match->Captures->operator[](0)->Index);
			Assert::True(expected.Length == match->Captures->operator[](0)->Length);
			Assert::True(expected.Value == match->Captures->operator[](0)->ToString());
		}

		static void Matches_Invalid()
		{
			std::wstring input = L"input";
			//// Input is null
			//AssertThrows(ArgumentNullException, Regex::Matches(NULL, L"pattern"))
			//AssertThrows(ArgumentNullException, Regex::Matches(NULL, L"pattern", RegexOptions::None))
			//AssertThrows(ArgumentNullException, Regex::Matches(NULL, L"pattern", RegexOptions::None, TimeSpan::FromSeconds(1)))
			//AssertThrows(ArgumentNullException, Regex(L"pattern").Matches(NULL))
			//AssertThrows(ArgumentNullException, Regex(L"pattern").Matches(NULL, 0))


			//// Pattern is null
			////AssertThrows(ArgumentNullException, Regex::Matches(L"input", NULL))
			//AssertThrows(ArgumentNullException, Regex::Matches(L"input", NULL, RegexOptions::None))
			//AssertThrows(ArgumentNullException, Regex::Matches(L"input", NULL, RegexOptions::None, TimeSpan::FromSeconds(1)))

			// Options are invalid
			AssertThrows(ArgumentOutOfRangeException, Regex::Matches(&input, L"pattern", (RegexOptions)(-1)))
			AssertThrows(ArgumentOutOfRangeException, Regex::Matches(&input, L"pattern", (RegexOptions)(-1), TimeSpan::FromSeconds(1)))

			// 0x400 is new NonBacktracking mode that is now valid, 0x800 is still invalid
			AssertThrows(ArgumentOutOfRangeException, Regex::Matches(&input, L"pattern", (RegexOptions)0x800))
			AssertThrows(ArgumentOutOfRangeException, Regex::Matches(&input, L"pattern", (RegexOptions)0x800, TimeSpan::FromSeconds(1)))

			// MatchTimeout is invalid
			AssertThrows(ArgumentOutOfRangeException, Regex::Matches(&input, L"pattern", RegexOptions::None, TimeSpan::Zero))
			AssertThrows(ArgumentOutOfRangeException, Regex::Matches(&input, L"pattern", RegexOptions::None, TimeSpan::Zero))

			// Start is invalid
			AssertThrows(ArgumentOutOfRangeException, Regex(L"pattern").Matches(&input, -1))
			AssertThrows(ArgumentOutOfRangeException, Regex(L"pattern").Matches(&input, 6))
		}

		static void NextMatch_EmptyMatch_ReturnsEmptyMatch()
		{
			Assert::True(Match::getEmpty() == Match::getEmpty()->NextMatch());
		}
	};
}
