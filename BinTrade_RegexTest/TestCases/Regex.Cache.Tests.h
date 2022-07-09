#pragma once


namespace RegexsTest
{
	using namespace std;
	using namespace Regexs;
	class RegexCacheTests
	{
	public:
		static void Run(bool test_throw)
		{
			Assert::WriteLine(L"RegexCacheTests");

			int originalCacheSize = Regex::getCacheSize();

			CacheSize_Set(0);
			CacheSize_Set(12);
			if (test_throw)CacheSize_Set_NegativeValue_ThrowsArgumentOutOfRangeException();
			Ctor_Cache_Second_drops_first();
			Ctor_Cache_Shrink_cache();
			Ctor_Cache_Promote_entries();
			Ctor_Cache_Uses_culture_and_options();
			Ctor_Cache_Uses_dictionary_linked_list_switch_does_not_throw();

			Regex::setCacheSize(originalCacheSize);
		}

		static void CacheSize_Set(int newCacheSize)
		{
			int originalCacheSize = Regex::getCacheSize();

			try
			{
				Regex::setCacheSize(newCacheSize);
				Assert::True(newCacheSize == Regex::getCacheSize());
			}
			catch (...)
			{
				Regex::setCacheSize(originalCacheSize);
			}
		}

		static void CacheSize_Set_NegativeValue_ThrowsArgumentOutOfRangeException()
		{
			AssertThrows(ArgumentOutOfRangeException, Regex::setCacheSize(-1))
		}

		static void Ctor_Cache_Second_drops_first()
		{
			Regex::setCacheSize(1);
			Assert::True(Regex::IsMatch(L"1", L"1"));
			Assert::True(Regex::IsMatch(L"2", L"2")); // previous removed from cache
		}

		static void Ctor_Cache_Shrink_cache()
		{
			Regex::setCacheSize(2);
			Assert::True(Regex::IsMatch(L"1", L"1"));
			Assert::True(Regex::IsMatch(L"2", L"2"));
			Regex::setCacheSize(1);
			Regex::setCacheSize(0); // clear
		}

		static void Ctor_Cache_Promote_entries()
		{
			Regex::setCacheSize(3);
			Assert::True(Regex::IsMatch(L"1", L"1"));
			Assert::True(Regex::IsMatch(L"2", L"2"));
			Assert::True(Regex::IsMatch(L"3", L"3"));
			Assert::True(Regex::IsMatch(L"1", L"1")); // should be put first
			Regex::setCacheSize(1); // only 1 stays
		}

		static void Ctor_Cache_Uses_culture_and_options()
		{
			Regex::setCacheSize(0);
			Regex::setCacheSize(3);
			Assert::True(Regex::IsMatch(L"1", L"1", RegexOptions::IgnoreCase));
			Assert::True(Regex::IsMatch(L"1", L"1", RegexOptions::Multiline));
			// Force to set a different culture than the current culture!
			auto cache = ComCultureInfo::CurrentCulture();
			auto current = ComCultureInfo::CreateSpecificCulture(L"de-DE");
			GetCultureInfo::s_userDefaultCulture = current;

			Assert::True(Regex::IsMatch(L"1", L"1", RegexOptions::Multiline));

			GetCultureInfo::s_userDefaultCulture = cache;
		}

		static void Ctor_Cache_Uses_dictionary_linked_list_switch_does_not_throw()
		{
			int original = Regex::getCacheSize();
			Regex::setCacheSize(0);
			Fill(original);
			const int limit = 10;
			Regex::setCacheSize(limit - 1);
			Regex::setCacheSize(0);
			Fill(original);
			Remove(original);
		}

		static void Fill(int n)
		{
			for (int i = 0; i < n; i++)
			{
				Regex::setCacheSize(Regex::getCacheSize() + 1);
				Assert::True(Regex::IsMatch(std::to_wstring(i), std::to_wstring(i)));
				//Assert::True(GetCachedItemsNum() == i + 1);
			}
		}

		static void Remove(int n)
		{
			for (int i = 0; i < n; i++)
			{
				Regex::setCacheSize(Regex::getCacheSize() - 1);
				//Assert::True(GetCachedItemsNum() == Regex.CacheSize);
			}
		}
	};
}
