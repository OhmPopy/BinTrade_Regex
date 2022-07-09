#pragma once

#include "../RegexWpf/RegexGroupCollection.h"

namespace RegexsTest
{
	using namespace std;
	using namespace Regexs;
	class RegexEscapeUnescapeTests
	{
		static	std::wstring Repeat(std::wstring input, int count)
		{
			std::wstring result;
			for (int i = 0; i < count; ++i)
			{
				result += input;
			}
			return result;
		}
	public:
		static void Run(bool test_throw)
		{
			Assert::WriteLine(L"RegexEscapeUnescapeTests");

			Escape(L"Hello", L"Hello");
			Escape(L"#$^*+(){}<>\\|. ", L"\\#\\$\\^\\*\\+\\(\\)\\{}<>\\\\\\|\\.\\ ");
			Escape(L"\\n\\r\\t\\f", L"\\\\n\\\\r\\\\t\\\\f");
			Escape(L"\\", L"\\\\");
			Escape(L"", L"");

			if (test_throw) Escape_NullString_ThrowsArgumentNullException();


			Unescape(L"Hello", L"Hello");
			Unescape(L"\\#\\$\\^\\*\\+\\(\\)\\{}<>\\\\\\|\\.\\ ", L"#$^*+(){}<>\\|. ");
			Unescape(L"\\\\n\\\\r\\\\t\\\\f", L"\\n\\r\\t\\f");
			Unescape(L"\\\\", L"\\");
			Unescape(L"\\", L"");
			Unescape(L"", L"");


			if (test_throw) Unscape_NullString_ThrowsArgumentNullException();
		}

		
		static void Escape(std::wstring str, std::wstring expected)
		{
			auto vs = Regex::Escape(str);
			Assert::True(expected == vs);

			if (expected.length() > 0)
			{
				const int Count = 100;
				Assert::True(Repeat(expected, Count)== Regex::Escape(Repeat(str, Count)));
			}
		}

		static void Escape_NullString_ThrowsArgumentNullException()
		{
			//AssertThrows(ArgumentNullException, Regex::Escape(NULL))//todo alway PASS .string not null
		}

		static void Unescape(std::wstring str, std::wstring expected)
		{
			Assert::True(expected == Regex::Unescape(str));

			if (expected.length() > 0)
			{
				const int Count = 100;
				Assert::True(Repeat(expected, Count) == Regex::Unescape(Repeat(str, Count)));
			}
		}

		static void Unscape_NullString_ThrowsArgumentNullException()
		{
			//AssertThrows(ArgumentNullException, Regex::Unescape(NULL))//todo alway PASS .string not null
		}
	};
}
