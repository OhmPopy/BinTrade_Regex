#include "Program.h"

#include "UnitTestAssert.h"
#include "BinTrade_RegexLib/Com/GetCultureInfo.h"
#include "TestCases/AttRegexTests.h"
#include "TestCases/CaptureCollectionTests.h"
#include "TestCases/CaptureCollectionTests2.h"
#include "TestCases/GroupCollectionReadOnlyDictionaryTests.h"
#include "TestCases/GroupCollectionTests.h"
#include "TestCases/MatchCollectionTests.h"
#include "TestCases/Regex.Cache.Tests.h"
#include "TestCases/Regex.EscapeUnescape.Tests.h"
#include "TestCases/Regex.GetGroupNames.Tests.h"
#include "TestCases/Regex.Groups.Tests.h"
#include "TestCases/Regex.KnownPattern.Tests.h"
#include "TestCases/Regex.Match.Tests.h"
#include "TestCases/Regex.MultipleMatches.Tests.h"
#include "TestCases/Regex.Replace.Tests.h"
#include "TestCases/Regex.Split.Tests.h"
#include "TestCases/Regex.UnicodeChar.Tests.h"
#include "TestCases/RegexCharacterSetTests.h"
#include "TestCases/RegexConstructorTests.h"
#include "TestCases/RegexCountTests.h"
#include "TestCases/RegexCultureTests.h"
#include "TestCases/RegexGroupNameTests.h"
#include "TestCases/RegexMatchTimeoutExceptionTests.h"
#include "TestCases/RegexParserTests.h"


namespace RegexsTest
{
	void Program::Main()
	{
	}

	void Program::TestAll()
	{
		GetCultureInfo::Init();
		ComCultureInfo::CurrentCulture();


		bool test_throw = true;

		Assert::WriteLine(L"Start Test");
		// test 
		AttRegexTests::Run(test_throw);//PASS
		CaptureCollectionTests::Run(test_throw);//PASS
		CaptureCollectionTests2::Run(test_throw);//PASS
		GroupCollectionReadOnlyDictionaryTests::Run(test_throw);//PASS
		GroupCollectionTests::Run(test_throw);//PASS
		MatchCollectionTests::Run(test_throw);//PASS
		RegexCacheTests::Run(test_throw);//PASS
		RegexCountTests::Run(test_throw);//PASS
		RegexConstructorTests::Run(test_throw);//PASS
		RegexEscapeUnescapeTests::Run(test_throw);//PASS
		RegexGetGroupNamesTests::Run(test_throw);//PASS
		RegexGroupsTests::Run(test_throw);//PASS
		RegexKnownPatternTests::Run(test_throw);//PASS
		RegexMatchTests::Run(test_throw);//PASS
		RegexMultipleMatchesTests::Run(test_throw);//PASS
		RegexReplaceTests::Run(test_throw);//PASS
		RegexSplitTests::Run(test_throw);//PASS
		RegexUnicodeCharTests::Run(test_throw);//PASS
		
		RegexCharacterSetTests::Run(test_throw);//PASS slow
		
		RegexCultureTests::Run(test_throw);//PASS 
		RegexMatchTimeoutExceptionTests::Run(test_throw);//PASS
		RegexGroupNameTests::Run(test_throw);//PASS
		RegexParserTests::Run(test_throw);//PASS




		Assert::WriteLineSpace();
		Assert::WriteLine(L"-------------------------");
		Assert::WriteLine(L"Complete test");
		const std::wstring t1 = L"Total Pass: " + std::to_wstring(Assert::GetTotalPass());
		const std::wstring t2 = L"Total Faild: " + std::to_wstring(Assert::GetTotalFaild());
		Assert::WriteLine(t1.c_str());
		Assert::WriteLine(t2.c_str());
		Assert::WriteLine(L"");
	}
}
 