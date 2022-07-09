#pragma once

#include "Assert.h"

namespace RegexsTest
{
	using namespace std;
	using namespace Regexs;
	class RegexGetGroupNamesTests
	{
		struct StructGroupNamesAndNumbers
		{
			std::wstring pattern;
			std::wstring input;
			std::vector<std::wstring> expectedNames;
			std::vector<int> expectedNumbers;
			std::vector<std::wstring> expectedGroups;
			RegexOptions options;

			StructGroupNamesAndNumbers(const std::wstring& pattern, const std::wstring& input, const std::vector<std::wstring>& expected_names, const std::vector<int>& expected_numbers, const std::vector<std::wstring>& expected_groups, RegexOptions options)
				: pattern(pattern),
				  input(input),
				  expectedNames(expected_names),
				  expectedNumbers(expected_numbers),
				  expectedGroups(expected_groups),
				  options(options)
			{
			}
		};

	public:
		static void Run(bool test_throw)
		{
			Assert::WriteLine(L"RegexGetGroupNamesTests");

			std::vector<std::wstring> sss = {L"0", L"first_name", L"last_name"};
			GetGroupNames(L"(?<first_name>\\S+)\\s(?<last_name>\\S+)", RegexOptions::None, sss);

			auto value_2 = GroupNamesAndNumbers_TestData();
			for (size_t i = 0; i < value_2.size(); ++i)
			{
				StructGroupNamesAndNumbers numbers = value_2.at(i);
				GroupNamesAndNumbers(numbers.pattern, numbers.input, numbers.expectedNames, numbers.expectedNumbers, numbers.expectedGroups, numbers.options);
			}

			GroupNameFromNumber_InvalidIndex_ReturnsEmptyString(L"foo", 1, RegexOptions::None);
			GroupNameFromNumber_InvalidIndex_ReturnsEmptyString(L"foo", -1, RegexOptions::None);
			GroupNameFromNumber_InvalidIndex_ReturnsEmptyString(L"(?<first_name>\\S+)\\s(?<last_name>\\S+)", -1, RegexOptions::None);
			GroupNameFromNumber_InvalidIndex_ReturnsEmptyString(L"(?<first_name>\\S+)\\s(?<last_name>\\S+)", 3, RegexOptions::None);
			GroupNameFromNumber_InvalidIndex_ReturnsEmptyString(L"((?<256>abc)\\d+)?(?<16>xyz)(.*)", -1, RegexOptions::None);


			GroupNumberFromName_InvalidName_ReturnsMinusOne(L"foo", L"no-such-name", RegexOptions::None);
			GroupNumberFromName_InvalidName_ReturnsMinusOne(L"foo", L"1", RegexOptions::None);
			GroupNumberFromName_InvalidName_ReturnsMinusOne(L"(?<first_name>\\S+)\\s(?<last_name>\\S+)", L"no-such-name", RegexOptions::None);
			GroupNumberFromName_InvalidName_ReturnsMinusOne(L"(?<first_name>\\S+)\\s(?<last_name>\\S+)", L"FIRST_NAME", RegexOptions::None);

			if (test_throw)GroupNumberFromName_NullName_ThrowsArgumentNullException();
		}

		static void GetGroupNames(std::wstring pattern, RegexOptions options, std::vector<std::wstring> expectedGroupNames)
		{
			Regex regex = Regex(pattern, options);
			auto xxx = regex.GetGroupNames();

			Assert::IsTrue(expectedGroupNames.size() == xxx.size());
			if (expectedGroupNames.size() == xxx.size())
			{
				for (size_t i = 0; i < expectedGroupNames.size(); ++i)
				{
					Assert::True(expectedGroupNames[i] == xxx[i]);
				}
			}
		}

		static std::vector<StructGroupNamesAndNumbers> GroupNamesAndNumbers_TestData()
		{
			RegexOptions options = RegexOptions::None;

			return
			{
				StructGroupNamesAndNumbers(
					L"(?<first_name>\\S+)\\s(?<last_name>\\S+)", L"Ryan Byington",
					{L"0", L"first_name", L"last_name"},
					{0, 1, 2},
					{L"Ryan Byington", L"Ryan", L"Byington"},
					options
				),

				StructGroupNamesAndNumbers(
					L"((?<One>abc)\\d+)?(?<Two>xyz)(.*)", L"abc208923xyzanqnakl",
					{L"0", L"1", L"2", L"One", L"Two"},
					{0, 1, 2, 3, 4},
					{L"abc208923xyzanqnakl", L"abc208923", L"anqnakl", L"abc", L"xyz"},
					options
				),

				StructGroupNamesAndNumbers(
					L"((?<256>abc)\\d+)?(?<16>xyz)(.*)", L"0272saasdabc8978xyz][]12_+-",
					{L"0", L"1", L"2", L"16", L"256"},
					{0, 1, 2, 16, 256},
					{L"abc8978xyz][]12_+-", L"abc8978", L"][]12_+-", L"xyz", L"abc"},
					options
				),

				StructGroupNamesAndNumbers(
					L"((?<4>abc)(?<digits>\\d+))?(?<2>xyz)(?<everything_else>.*)", L"0272saasdabc8978xyz][]12_+-",
					{L"0", L"1", L"2", L"digits", L"4", L"everything_else"},
					{0, 1, 2, 3, 4, 5},
					{L"abc8978xyz][]12_+-", L"abc8978", L"xyz", L"8978", L"abc", L"][]12_+-"},
					options
				),

				StructGroupNamesAndNumbers(
					L"(?<first_name>\\S+)\\s(?<first_name>\\S+)", L"Ryan Byington",
					{L"0", L"first_name"},
					{0, 1},
					{L"Ryan Byington", L"Byington"},
					options
				),

				StructGroupNamesAndNumbers(
					L"(?<15>\\S+)\\s(?<15>\\S+)", L"Ryan Byington",
					{L"0", L"15"},
					{0, 15},
					{L"Ryan Byington", L"Byington"},
					options
				),

				StructGroupNamesAndNumbers(
					L"(?'first_name'\\S+)\\s(?'last_name'\\S+)", L"Ryan Byington",
					{L"0", L"first_name", L"last_name"},
					{0, 1, 2},
					{L"Ryan Byington", L"Ryan", L"Byington"},
					options
				),

				StructGroupNamesAndNumbers(
					L"((?'One'abc)\\d+)?(?'Two'xyz)(.*)", L"abc208923xyzanqnakl",
					{L"0", L"1", L"2", L"One", L"Two"},
					{0, 1, 2, 3, 4},
					{L"abc208923xyzanqnakl", L"abc208923", L"anqnakl", L"abc", L"xyz"},
					options
				),

				StructGroupNamesAndNumbers(
					L"((?'256'abc)\\d+)?(?'16'xyz)(.*)", L"0272saasdabc8978xyz][]12_+-",
					{L"0", L"1", L"2", L"16", L"256"},
					{0, 1, 2, 16, 256},
					{L"abc8978xyz][]12_+-", L"abc8978", L"][]12_+-", L"xyz", L"abc"},
					options
				),

				StructGroupNamesAndNumbers(
					L"((?'4'abc)(?'digits'\\d+))?(?'2'xyz)(?'everything_else'.*)", L"0272saasdabc8978xyz][]12_+-",
					{L"0", L"1", L"2", L"digits", L"4", L"everything_else"},
					{0, 1, 2, 3, 4, 5},
					{L"abc8978xyz][]12_+-", L"abc8978", L"xyz", L"8978", L"abc", L"][]12_+-"},
					options
				),

				StructGroupNamesAndNumbers(
					L"(?'first_name'\\S+)\\s(?'first_name'\\S+)", L"Ryan Byington",
					{L"0", L"first_name"},
					{0, 1},
					{L"Ryan Byington", L"Byington"},
					options
				),

				StructGroupNamesAndNumbers(
					L"(?'15'\\S+)\\s(?'15'\\S+)", L"Ryan Byington",
					{L"0", L"15"},
					{0, 15},
					{L"Ryan Byington", L"Byington"},
					options
				),
			};
		}

		static void GroupNamesAndNumbers(std::wstring pattern, std::wstring input, std::vector<std::wstring> expectedNames, std::vector<int> expectedNumbers, std::vector<std::wstring> expectedGroups, RegexOptions options = RegexOptions::None)
		{
			Regex regex = Regex(pattern, options);
			Match* match = regex.Match(&input);
			Assert::IsTrue(match->Success);

			auto numbers = regex.GetGroupNumbers();
			Assert::IsTrue(expectedNumbers.size()== numbers.size());

			auto names = regex.GetGroupNames();
			Assert::IsTrue(expectedNames.size()== names.size());

			Assert::IsTrue(expectedGroups.size()== match->Groups->Count);
			for (size_t i = 0; i < expectedNumbers.size(); i++)
			{
				Assert::IsTrue(expectedGroups[i]== match->Groups->operator[](expectedNames[i])->getValue());
				Assert::IsTrue(expectedGroups[i]== match->Groups->operator[](expectedNumbers[i])->getValue());

				Assert::IsTrue(expectedNumbers[i]== numbers[i]);
				Assert::IsTrue(expectedNumbers[i]== regex.GroupNumberFromName(expectedNames[i]));

				Assert::IsTrue(expectedNames[i]== names[i]);
				Assert::IsTrue(expectedNames[i]== regex.GroupNameFromNumber(expectedNumbers[i]));
			}
		}


		static void GroupNameFromNumber_InvalidIndex_ReturnsEmptyString(std::wstring pattern, int index, RegexOptions options = RegexOptions::None)
		{
			Assert::IsTrue(L"" == Regex(pattern, options).GroupNameFromNumber(index));
		}

		static void GroupNumberFromName_InvalidName_ReturnsMinusOne(std::wstring pattern, std::wstring name, RegexOptions options = RegexOptions::None)
		{
			auto temp = Regex(pattern, options).GroupNumberFromName(name);
			Assert::True(-1 == temp);
		}

		static void GroupNumberFromName_NullName_ThrowsArgumentNullException()
		{
			//todo alway PASS
			//AssertThrows(ArgumentNullException, Regex(L"foo").GroupNumberFromName(NULL)) 
		}
	};
}
