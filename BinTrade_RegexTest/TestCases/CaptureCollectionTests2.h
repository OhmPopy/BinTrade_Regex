#pragma once

#include "../RegexWpf/RegexGroupCollection.h"

namespace RegexsTest
{
	using namespace std;
	using namespace Regexs;
	class CaptureCollectionTests2
	{
	public:
		static void Run(bool test_throw)
		{
		}

		static CaptureCollection* CreateCollection()
		{
			Regex regex = Regex(L"\\b(\\w+\\s*)+\\.");
			std::wstring text = L"This is a sentence.";
			Match* match = regex.Match(&text);
			return match->getGroups()->operator[](1)->getCaptures();
		}
	};
}
