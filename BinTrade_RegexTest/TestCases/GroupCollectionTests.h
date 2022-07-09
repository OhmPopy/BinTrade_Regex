#pragma once


namespace RegexsTest
{
	using namespace std;
	using namespace Regexs;
	class GroupCollectionTests
	{
	public:
		static void Run(bool test_throw)
		{
			Assert::WriteLine(L"GroupCollectionTests");

			GetEnumerator();
			GetEnumerator_Generic();
			if (test_throw)GetEnumerator_Invalid();
			if (test_throw)GetEnumerator_Generic_Invalid();
			Item_Get();
			ContainsKey();
			Item_Invalid(-1);
			Item_Invalid(4);
			ICollection_Properties();
			ICollection_CopyTo(0);
			ICollection_CopyTo(5);
			if (test_throw) ICollection_CopyTo_Invalid();
		}

		static void GetEnumerator()
		{
			Regex regex = Regex(L"(?<A1>a*)(?<A2>b*)(?<A3>c*)");
			std::wstring text = L"aaabbccccccccccaaaabc";
			Match* match = regex.Match(&text);

			GroupCollection* groups = match->Groups;
			IEnumerator* enumerator = groups->GetEnumerator();
			for (int i = 0; i < 2; i++)
			{
				int counter = 0;
				while (enumerator->MoveNext())
				{
					Assert::True((objectForRegex*)groups->operator[](counter) == enumerator->Current);
					counter++;
				}
				Assert::False(enumerator->MoveNext());
				Assert::True(groups->Count == counter);
				enumerator->Reset();
			}
		}

		static void GetEnumerator_Generic()
		{
			Regex regex = Regex(L"(?<A1>a*)(?<A2>b*)(?<A3>c*)");
			std::wstring text = L"aaabbccccccccccaaaabc";
			Match* match = regex.Match(&text);

			GroupCollection* groups = match->Groups;
			//IEnumerator<KeyValuePair<string, Group>> enumerator = ((IEnumerable<KeyValuePair<string, Group>>)groups).GetEnumerator();
			//for (int i = 0; i < 2; i++)
			//{
			//	int counter = 0;
			//	while (enumerator.MoveNext())
			//	{
			//		Assert::Equal(groups[counter], enumerator.Current.Value);
			//		counter++;
			//	}
			//	Assert::False(enumerator.MoveNext());
			//	Assert::Equal(groups.Count, counter);
			//	enumerator.Reset();
			//}
		}

		static void GetEnumerator_Invalid()
		{
			Regex regex = Regex(L"(?<A1>a*)(?<A2>b*)(?<A3>c*)");
			std::wstring text = L"aaabbccccccccccaaaabc";
			Match* match = regex.Match(&text);

			IEnumerator* enumerator = match->Groups->GetEnumerator();


			AssertThrows(InvalidOperationException, enumerator->getCurrent())

			while (enumerator->MoveNext());
			AssertThrows(InvalidOperationException, enumerator->getCurrent())

			enumerator->Reset();
			AssertThrows(InvalidOperationException, enumerator->getCurrent())
		}

		static void GetEnumerator_Generic_Invalid()
		{
			//Regex regex = new Regex(@"( ? <A1>a*)(? <A2>b*)(? <A3>c*)");
			//	Match match = regex.Match("aaabbccccccccccaaaabc");

			//IEnumerator<KeyValuePair<string, Group>> enumerator = ((IEnumerable<KeyValuePair<string, Group>>)match.Groups).GetEnumerator();

			//Assert::Throws<InvalidOperationException>(() = > enumerator.Current);

			//while (enumerator.MoveNext());
			//Assert::Throws<InvalidOperationException>(() = > enumerator.Current);

			//enumerator.Reset();
			//Assert::Throws<InvalidOperationException>(() = > enumerator.Current);
		}

		static void Item_Get()
		{
			Regex regex = Regex(L"(\\d{3})-(\\d{3}-\\d{4})");
			std::wstring text = L"212-555-6666";
			Match* match = regex.Match(&text);
			GroupCollection* collection = match->Groups;
			Assert::True(L"212-555-6666" == collection->operator[](0)->ToString());
			Assert::True(L"212" == collection->operator[](1)->ToString());
			Assert::True(L"555-6666" == collection->operator[](2)->ToString());
		}

		static void ContainsKey()
		{
			Regex regex = Regex(L"(\\d{3})-(\\d{3}-\\d{4})");
			std::wstring text = L"212-555-6666";
			Match* match = regex.Match(&text);
			GroupCollection* collection = match->Groups;
			//Assert::True(collection->ContainsKey(L"0"));
			//Assert::True(collection->ContainsKey(L"1"));
			//Assert::True(collection->ContainsKey(L"2"));
			//Assert::False(collection->ContainsKey(L"3"));
		}


		static void Item_Invalid(int groupNumber)
		{
			Regex regex = Regex(L"(?<A1>a*)(?<A2>b*)(?<A3>c*)");
			std::wstring text = L"aaabbccccccccccaaaabc";
			GroupCollection* groups = regex.Match(&text)->Groups;

			Group* group = groups->operator[](groupNumber);
			Assert::True(L"" == group->getValue());
			Assert::True(0 == group->getIndex());
			Assert::True(0 == group->getLength());
			Assert::True(0 == group->getCaptures()->Count);
		}

		static void ICollection_Properties()
		{
			Regex regex = Regex(L"(?<A1>a*)(?<A2>b*)(?<A3>c*)");
			std::wstring text = L"aaabbccccccccccaaaabc";
			GroupCollection* groups = regex.Match(&text)->Groups;
			ICollection* collection = groups;

			Assert::False(collection->IsSynchronized);
			Assert::IsNotNull(collection->SyncRoot);
		}

		static void ICollection_CopyTo(int index)
		{
			Regex regex = Regex(L"(?<A1>a*)(?<A2>b*)(?<A3>c*)");
			std::wstring text = L"aaabbccccccccccaaaabc";
			GroupCollection* groups = regex.Match(&text)->Groups;
			ICollection* collection = groups;

			ArrayGroup copy(collection->Count + index);
			collection->CopyTo(&copy, index);
			for (int i = 0; i < index; i++)
			{
				Assert::IsNull(copy.Data[i]);
			}
			for (size_t i = index; i < copy.Data.size(); i++)
			{
				Assert::True(groups->operator[](i - index) == copy.Data[i]);
			}
		}

		static void ICollection_CopyTo_Invalid()
		{
			Regex regex = Regex(L"e");
			std::wstring text = L"aaabbccccccccccaaaabc";
			ICollection* collection = regex.Match(&text)->Groups;

			// Array is null
			AssertThrows(ArgumentNullException, collection->CopyTo(NULL, 0))

			// Array is multidimensional
			std::vector<int> multidimensional = {10, 10};
			AssertThrows(ArgumentException, collection->CopyTo(&multidimensional, 0))


			// Index < 0
			ArrayGroup Index(collection->Count);
			AssertThrows(IndexOutOfRangeException, collection->CopyTo(&Index, -1))


			// Invalid index + length
			ArrayGroup Invalid_1(collection->Count);
			ArrayGroup Invalid_2(collection->Count + 1);

			AssertThrows(IndexOutOfRangeException, collection->CopyTo(&Invalid_1, 1))
			AssertThrows(IndexOutOfRangeException, collection->CopyTo(&Invalid_2, 2))
		}

	 
	};
}
