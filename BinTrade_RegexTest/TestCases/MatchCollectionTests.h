#pragma once

#include "../RegexWpf/RegexGroupCollection.h"

namespace RegexsTest
{
	using namespace std;
	using namespace Regexs;
	class MatchCollectionTests
	{
	public:
		static void Run(bool test_throw)
		{
			Assert::WriteLine(L"MatchCollectionTests");

			GetEnumerator(RegexOptions::None);
			//GetEnumerator(RegexOptions::Compiled);//not support

			if (test_throw) GetEnumerator_Invalid(RegexOptions::None);
			Item_Get();
			if (test_throw) Item_Get_InvalidIndex_ThrowsArgumentOutOfRangeException(RegexOptions::None);
			ICollection_Properties(RegexOptions::None);
			ICollection_CopyTo(RegexOptions::None);
			if (test_throw) ICollection_CopyTo_Invalid(RegexOptions::None);
		}

		static void GetEnumerator(RegexOptions options)
		{
			Regex regex = Regex(L"e", options);
			std::wstring input = L"dotnet";
			MatchCollection* matches = regex.Matches(&input);
			IEnumerator* enumerator = matches->GetEnumerator();
			for (int i = 0; i < 2; i++)
			{
				int counter = 0;
				while (enumerator->MoveNext())
				{
					Assert::True(matches->operator[](counter) == enumerator->Current);
					counter++;
				}
				Assert::False(enumerator->MoveNext());
				Assert::True(matches->Count == counter);
				enumerator->Reset();
			}
		}

		static void GetEnumerator_Invalid(RegexOptions options)
		{
			Regex regex = Regex(L"e", options);
			std::wstring input = L"dotnet";
			MatchCollection* matches = regex.Matches(&input);
			IEnumerator* enumerator = matches->GetEnumerator();

			AssertThrows(InvalidOperationException, enumerator->Current)

			while (enumerator->MoveNext());
			AssertThrows(InvalidOperationException, enumerator->Current)

			enumerator->Reset();
			Assert::True(enumerator->MoveNext());
			enumerator->Reset();
			AssertThrows(InvalidOperationException, enumerator->Current)
		}

		static void Item_Get()
		{
			auto r1 = Regex(L"t");
			std::wstring input = L"dotnet";
			MatchCollection* collection = r1.Matches(&input);
			Assert::True(L"t" == collection->operator[](0)->ToString());
			Assert::True(L"t" == collection->operator[](1)->ToString());
		}

		static void Item_Get_InvalidIndex_ThrowsArgumentOutOfRangeException(RegexOptions options)
		{
			Regex regex = Regex(L"e", options);
			std::wstring input = L"dotnet";
			MatchCollection* matches = regex.Matches(&input);

			AssertThrows(ArgumentOutOfRangeException, matches->operator[](-1))
			AssertThrows(ArgumentOutOfRangeException, matches->operator[](matches->Count))
		}

		static void ICollection_Properties(RegexOptions options)
		{
			Regex regex = Regex(L"e", options);
			std::wstring input = L"dotnet";
			MatchCollection* matches = regex.Matches(&input);
			ICollection* collection = matches;

			Assert::False(collection->IsSynchronized);
			Assert::IsNotNull(collection->SyncRoot);
			Assert::True(matches == collection->SyncRoot);
		}

		static void ICollection_CopyTo(RegexOptions options)
		{
			std::vector<int> sss = {0, 5};
			for (int index : sss)
			{
				Regex regex = Regex(L"e", options);
				std::wstring input = L"dotnet";
				MatchCollection* matches = regex.Matches(&input);
				ICollection* collection = matches;

				ArrayMatch copy(collection->Count + index);
				collection->CopyTo(&copy, index);

				for (int i = 0; i < index; i++)
				{
					Assert::IsNull(copy.Data[i]);
				}
				for (size_t i = index; i < copy.Data.size(); i++)
				{
					Assert::True(matches->operator[](i - index) == copy.Data[i]);
				}
			}
		}

		static void ICollection_CopyTo_Invalid(RegexOptions options)
		{
			Regex regex = Regex(L"e", options);
			std::wstring input = L"dotnet";
			ICollection* collection = regex.Matches(&input);

			// Array is null
			AssertThrows(ArgumentNullException, collection->CopyTo(NULL, 0))


			// Array is multidimensional
			std::vector<int> multidimensional = {10, 10};
			AssertThrows(ArgumentException, collection->CopyTo(&multidimensional, 0))


			// Index < 0
			ArrayMatch Index(collection->Count);
			AssertThrows(ArgumentOutOfRangeException, collection->CopyTo(&Index, -1))


			// Invalid index + length
			ArrayMatch Invalid_1(collection->Count);
			ArrayMatch Invalid_2(collection->Count + 1);
			AssertThrows(ArgumentException, collection->CopyTo(&Invalid_1, 1))
			AssertThrows(ArgumentException, collection->CopyTo(&Invalid_2, 2))
		}
		 
	};
}
