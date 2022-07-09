# BinTrade_Regex
Regex .net in c++. Convert from System/regex/system/text/regularexpressions. Full test
not yet optimized for running speed but acceptable

use like c#


	void Program::Main()
	{
		std::wstring pattern = L"(a*){2}(x)";
		std::wstring input = L"axa";
		RegexOptions options = RegexOptions::None;

		Regex r = Regex(pattern, options);
		Match* match = r.Match(&input);

	}

