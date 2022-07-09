#pragma once

#include "Assert.h"
#include "UnitTestAssert.h"
#include "RegexWpf/Regex.h"

namespace RegexsTest
{
	using namespace std;
	using namespace Regexs;
	class GroupCollectionReadOnlyDictionaryTests
	{
	public:
		static void Run(bool test_throw)
		{
			Assert::WriteLine(L"GroupCollectionReadOnlyDictionaryTests");

			IReadOnlyDictionary_TryGetValueSuccess();
			IReadOnlyDictionary_TryGetValue_DoesntExist();
			IReadOnlyDictionary_TryGetValue_NoMatch();
			IReadOnlyDictionary_TryGetValue_Number();
			IReadOnlyDictionary_Keys();
			IReadOnlyDictionary_Values();
			IReadOnlyDictionary_GetEnumerator();
			if (test_throw)
				GetEnumerator_Invalid();
			NotAllGroupsHaveNames();
		}

		static void IReadOnlyDictionary_TryGetValueSuccess()
		{
			Regex regex = Regex(L"(?<A1>a*)(?<A2>b*)(?<A3>c*)");
			std::wstring text = L"aaabbccccccccccaaaabc";
			Match* match = regex.Match(&text);

			GroupCollection* groups = match->Groups;

			Group* value = NULL;
			Assert::True(groups->TryGetValue(L"A1", value));
			Assert::IsNotNull(value);
			Assert::True(L"aaa" == value->getValue());
		}

		static void IReadOnlyDictionary_TryGetValue_DoesntExist()
		{
			Regex regex = Regex(L"(?<A1>a*)(?<A2>b*)(?<A3>c*)");
			std::wstring text = L"aaabbccccccccccaaaabc";
			Match* match = regex.Match(&text);

			GroupCollection* groups = match->Groups;

			Group* value = NULL;
			Assert::False(groups->TryGetValue(L"INVALID", value));
			Assert::IsNull(value);
		}

		static void IReadOnlyDictionary_TryGetValue_NoMatch()
		{
			Regex regex = Regex(L"(?<A1>a+)(?<A2>b+)(?<A3>c+)");
			std::wstring text = L"def";
			Match* match = regex.Match(&text);

			GroupCollection* groups = match->Groups;

			Group* value = NULL;
			Assert::False(groups->TryGetValue(L"A1", value));
			Assert::IsNull(value);
		}

		static void IReadOnlyDictionary_TryGetValue_Number()
		{
			Regex regex = Regex(L"(?<A1>a*)(?<A2>b*)(?<A3>c*)");
			std::wstring text = L"def";
			Match* match = regex.Match(&text);

			GroupCollection* groups = match->Groups;

			Group* value = NULL;
			Assert::True(groups->TryGetValue(L"0", value));
			Assert::IsNotNull(value);
			Assert::True(value->getSuccess());
		}

		static void IReadOnlyDictionary_Keys()
		{
			Regex regex = Regex(L"(?<A1>a*)(?<A2>b*)(?<A3>c*)");
			std::wstring text = L"aaabbbcccabc";
			Match* match = regex.Match(&text);

			GroupCollection* groups = match->Groups;

			auto group_name = regex.GetGroupNames();
			Assert::True(4 == groups->Count);

			Assert::True(L"0" == group_name[0]);
			Assert::True(L"A1" == group_name[1]);
			Assert::True(L"A2" == group_name[2]);
			Assert::True(L"A3" == group_name[3]);
		}

		static void IReadOnlyDictionary_Values()
		{
			Regex regex = Regex(L"(?<A1>a*)(?<A2>b*)(?<A3>c*)");
			std::wstring text = L"aaabbbcccabc";
			Match* match = regex.Match(&text);

			GroupCollection* groups = match->Groups;

			Assert::True(4 == groups->Count);

			Assert::True(L"aaabbbccc" == groups->operator[](0)->Value);
			Assert::True(L"aaa" == groups->operator[](1)->Value);
			Assert::True(L"bbb" == groups->operator[](2)->Value);
			Assert::True(L"ccc" == groups->operator[](3)->Value);
		}

		static void IReadOnlyDictionary_GetEnumerator()
		{
			Regex regex = Regex(L"(?<A1>a*)(?<A2>b*)(?<A3>c*)");
			std::wstring text = L"aaabbccccccccccaaaabc";
			Match* match = regex.Match(&text);

			//IReadOnlyDictionary<string, Group> groups = match.Groups;
			//auto enumerator = match->Groups->GetEnumerator();
			//for (int i = 0; i < 2; i++)
			//{
			//	int counter = 0;
			//	while (enumerator->MoveNext())
			//	{
			//		Regexs::Capture* result = enumerator->Current;
			//		Group group = match->Groups->operator[](counter);
			//		Assert::True(group == result.Value);
			//		Assert::True(group.Name == result.Key);
			//		counter++;
			//	}
			//	Assert::False(enumerator->MoveNext());
			//	Assert::True(counter == groups.Count);
			//	enumerator->Reset();
			//}

		}

		static void GetEnumerator_Invalid()
		{
			Regex regex = Regex(L"(?<A1>a*)(?<A2>b*)(?<A3>c*)");
			std::wstring text = L"aaabbccccccccccaaaabc";
			Match* match = regex.Match(&text);

			IEnumerator* enumerator = match->Groups->GetEnumerator();

			AssertThrows(InvalidOperationException, enumerator->Current)

			while (enumerator->MoveNext());
			AssertThrows(InvalidOperationException, enumerator->Current)

			enumerator->Reset();
			AssertThrows(InvalidOperationException, enumerator->Current)

		}

		static void NotAllGroupsHaveNames()
		{
			Regex regex = Regex(L"(a*)(?<B>b*)(?<C>c*)");
			std::wstring text = L"aaabbbccc";

			Match* match = regex.Match(&text);

			GroupCollection* groups = match->Groups;

			Assert::True(match->getSuccess());

			Assert::True(4 == groups->Count);

			Group* bByName = groups->operator[](L"B");
			Assert::True(L"bbb" == bByName->getValue());
			auto xxx1 = bByName->ToString();
			auto xxx2 = bByName->getValue();
			auto xxx3 = bByName->GetOriginalString();
			auto xxx4 = bByName->GetLeftSubstring();
			auto xxx5 = bByName->GetRightSubstring();
			//Assert::Equal("B", bByName->Name);

			Group* bByIndex = groups->operator[](2);
			Assert::True(L"bbb" == bByIndex->getValue());
			//Assert::Equal("B", bByIndex->Name);

			Group* groupZero = groups->operator[](0);
			Assert::True(L"aaabbbccc" == groupZero->getValue());

			Group* groupC = groups->operator[](3);
			Assert::True(groupC->getSuccess());

		}
	};
}
