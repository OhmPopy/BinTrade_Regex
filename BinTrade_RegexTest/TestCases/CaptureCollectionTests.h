#pragma once

#include "Regex.h"
#include "RegexMatch.h"
#include "../RegexWpf/RegexCaptureCollection.h"
#include "../RegexWpf/RegexGroupCollection.h"
#include "UnitTestAssert.h"

namespace RegexsTest
{
	using namespace std;
	using namespace Regexs;
	class CaptureCollectionTests
	{
	public:
		static void Run(bool test_throw)
		{
			Assert::WriteLine(L"CaptureCollectionTests");

			GetEnumerator();
			if (test_throw)
				GetEnumerator_Invalid();
			Item_Get();
			if (test_throw)
				Item_Get_InvalidIndex_ThrowsArgumentOutOfRangeException();
			ICollection_Properties();
			ICollection_CopyTo(0);
			ICollection_CopyTo(5);
			if (test_throw)
				ICollection_CopyTo_Invalid();
		}


		static void GetEnumerator()
		{
			Regex regex = Regex(L"(?<A1>a*)(?<A2>b*)(?<A3>c*)");
			std::wstring text = L"aaabbccccccccccaaaabc";
			Match* match = regex.Match(&text);

			CaptureCollection* captures = match->getCaptures();
			auto enumerator = captures->GetEnumerator();
			for (int i = 0; i < 2; i++)
			{
				int counter = 0;
				while (enumerator->MoveNext())
				{
					Assert::True(captures->operator[](counter) == enumerator->getCurrent());
					counter++;
				}

				Assert::False(enumerator->MoveNext());
				Assert::True(captures->getCount() == counter);
				enumerator->Reset();
			}
		}

		static void GetEnumerator_Invalid()
		{
			Regex regex = Regex(L"(?<A1>a*)(?<A2>b*)(?<A3>c*)");
			std::wstring text = L"aaabbccccccccccaaaabc";
			Match* match = regex.Match(&text);
			auto enumerator = match->getCaptures()->GetEnumerator();

			AssertThrows(InvalidOperationException, enumerator->getCurrent())

			while (enumerator->MoveNext());
			AssertThrows(InvalidOperationException, enumerator->getCurrent())

			enumerator->Reset();
			AssertThrows(InvalidOperationException, enumerator->getCurrent())
		}

		static void Item_Get()
		{
			Regex regex = Regex(L"\\b(\\w+\\s*)+\\.");
			std::wstring text = L"This is a sentence.";
			Match* match = regex.Match(&text);
			CaptureCollection* collection = match->getGroups()->operator[](1)->getCaptures();

			Assert::IsTrue(L"This " == collection->operator[](0)->ToString());
			Assert::IsTrue(L"is " == collection->operator[](1)->ToString());
			Assert::IsTrue(L"a " == collection->operator[](2)->ToString());
			Assert::IsTrue(L"sentence" == collection->operator[](3)->ToString());
		}

		static void Item_Get_InvalidIndex_ThrowsArgumentOutOfRangeException()
		{
			Regex regex = Regex(L"(?<A1>a*)(?<A2>b*)(?<A3>c*)");
			std::wstring text = L"aaabbccccccccccaaaabc";
			CaptureCollection* captures = regex.Match(&text)->getCaptures();

			AssertThrows(ArgumentOutOfRangeException, captures->operator[](-1))
			AssertThrows(ArgumentOutOfRangeException, captures->operator[](captures->getCount()))
		}

		static void ICollection_Properties()
		{
			Regex regex = Regex(L"(?<A1>a*)(?<A2>b*)(?<A3>c*)");
			std::wstring text = L"aaabbccccccccccaaaabc";
			CaptureCollection* captures = regex.Match(&text)->getCaptures();
			ICollection* collection = captures;

			Assert::False(collection->IsSynchronized);
			Assert::IsNotNull(collection->SyncRoot);
			Assert::True(collection->SyncRoot == collection->SyncRoot);
		}

		static void ICollection_CopyTo(int index)
		{
			Regex regex = Regex(L"(?<A1>a*)(?<A2>b*)(?<A3>c*)");
			std::wstring text = L"aaabbccccccccccaaaabc";
			CaptureCollection* captures = regex.Match(&text)->getCaptures();
			ICollection* collection = captures;

			ArrayCapture copy(collection->Count + index);
			collection->CopyTo(&copy, index);

			for (int i = 0; i < index; i++)
			{
				Assert::IsNull(copy.Data[i]);
			}
			for (size_t i = index; i < copy.Data.size(); i++)
			{
				Assert::True(captures->operator[](i - index) == copy.Data[i]);
			}
		}

		static void ICollection_CopyTo_Invalid()
		{
			Regex regex = Regex(L"(?<A1>a*)(?<A2>b*)(?<A3>c*)");
			std::wstring text = L"aaabbccccccccccaaaabc";
			ICollection* collection = regex.Match(&text)->getCaptures();

			// Array is null
			AssertThrows(ArgumentNullException, collection->CopyTo(NULL, 0))


			// Array is multidimensional
			int multidimensional[10][10];
			AssertThrows(ArgumentException, collection->CopyTo(&multidimensional, 0))

			// Array has a non-zero lower bound
			std::vector<std::vector<int>> o = {{10}, {10}}; //error casttype
			AssertThrows(ArgumentException, collection->CopyTo(&o, 0));

			// Index < 0
			ArrayCapture Index(collection->Count);
			AssertThrows(IndexOutOfRangeException, collection->CopyTo(&Index, -1))
			// Invalid index + length
			ArrayCapture Invalid_1(collection->Count);
			ArrayCapture Invalid_2(collection->Count + 1);
			AssertThrows(IndexOutOfRangeException, collection->CopyTo(&Invalid_1, 1))
			AssertThrows(IndexOutOfRangeException, collection->CopyTo(&Invalid_2, 2))
		}
	};
}
