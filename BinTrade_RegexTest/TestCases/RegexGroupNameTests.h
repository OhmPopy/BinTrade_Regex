#pragma once

#include "UnitTestAssert.h"


using namespace Regexs;
namespace RegexsTest
{
	class RegexGroupNameTests
	{
	public:
		static void Run(bool test_throw)
		{
			Assert::WriteLine(L"RegexGroupNameTests");

			NameTests();
		}

		static void NameTests()
		{
			//Debugger.Launch();

			wstring pattern = L"\\b(?<FirstWord>\\w+)\\s?((\\w+)\\s)*(?<LastWord>\\w+)?(?<Punctuation>\\p{Po})";
			wstring input = L"The cow jumped over the moon.";
			Regex regex = Regex(pattern);
			Match* match = regex.Match(&input);
			Assert::True(match->Success);

			vector<wstring> names = regex.GetGroupNames();
			for (size_t i = 0; i < names.size(); i++)
			{
				//not support
				//Assert::True(names[i] == match->Groups->operator[](i)->Name);
				//Assert::True(names[i] == match->Groups->operator[](i)->GetOriginalString());
			}
		}
	};
}
