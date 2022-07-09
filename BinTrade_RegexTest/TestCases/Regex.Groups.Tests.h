#pragma once

#include "UnitTestAssert.h"
#include "Other/UtilsString.h"

namespace RegexsTest
{
	using namespace std;
	using namespace Regexs;
	class RegexGroupsTests
	{
		struct StrundData
		{
			std::wstring cultureName;
			std::wstring pattern;
			std::wstring input;
			RegexOptions options;
			std::vector<std::wstring> expectedGroups;

			StrundData(const std::wstring& culture_name, const std::wstring& pattern, const std::wstring& input, RegexOptions options, const std::vector<std::wstring>& expected_groups)
				: cultureName(culture_name),
				  pattern(pattern),
				  input(input),
				  options(options),
				  expectedGroups(expected_groups)
			{
			}
		};

	public:
		static void Run(bool test_throw)
		{
			Assert::WriteLine(L"RegexGroupsTests");

			auto cache = GetCultureInfo::s_userDefaultCulture;

			Run(Groups_Basic_TestData());
			Run(Groups_CustomCulture_TestData_enUS());
			Run(Groups_CustomCulture_TestData_Czech());
			Run(Groups_CustomCulture_TestData_Danish());
			Run(Groups_CustomCulture_TestData_Turkish());
			Run(Groups_CustomCulture_TestData_AzeriLatin());

			if (test_throw)Synchronized_NullGroup_Throws();

			Synchronized_ValidGroup_Success(L"(cat)([\\v]*)(dog)", L"cat\\v\\v\\vdog");
			Synchronized_ValidGroup_Success(L"abc", L"def");


			GetCultureInfo::s_userDefaultCulture = cache;
		}

		static void Run(std::vector<StrundData> sss)
		{
			for (size_t i = 0; i < sss.size(); ++i)
			{
				auto item = sss[i];
				Groups(item.cultureName, item.pattern, item.input, item.options, item.expectedGroups);
			}
		}


		static std::vector<StrundData> Groups_Basic_TestData()
		{
			std::wstring value_1 = UtilsString::StringInitSpe(0x00) + L"bbbaaaABCD09zzzyyy";
			std::wstring value_2 = L"[\\w-[b-y" + UtilsString::StringInitSpe(0x00) + L"-" + UtilsString::StringInitSpe(0x0F) + L"]]+";
			std::wstring value_3 = L"!!!" + UtilsString::StringInitSpe(0x00) + UtilsString::StringInitSpe(0x00) + L"\t\t  [[[[bbbcccaaa";
			std::wstring value_4 = UtilsString::StringInitSpe(0x00) + UtilsString::StringInitSpe(0x00) + L"\t\t  [[[[bbbccc";

			return std::vector<StrundData>{

				// (A - B) B is a subset of A(ie B only contains chars that are in A)
				StrundData(L"", L"[abcd-[d]]+", L"dddaabbccddd", RegexOptions::None, {L"aabbcc"}),

				StrundData(L"", L"[\\d-[357]]+", L"33312468955", RegexOptions::None, {L"124689"}),
				StrundData(L"", L"[\\d-[357]]+", L"51246897", RegexOptions::None, {L"124689"}),
				StrundData(L"", L"[\\d-[357]]+", L"3312468977", RegexOptions::None, {L"124689"}),

				StrundData(L"", L"[\\w-[b-y]]+", L"bbbaaaABCD09zzzyyy", RegexOptions::None, {L"aaaABCD09zzz"}),

				StrundData(L"", L"[\\w-[\\d]]+", L"0AZaz9", RegexOptions::None, {L"AZaz"}),
				StrundData(L"", L"[\\w-[\\p{Ll}]]+", L"a09AZz", RegexOptions::None, {L"09AZ"}),

				StrundData(L"", L"[\\d-[13579]]+", L"1024689", RegexOptions::ECMAScript, {L"02468"}),
				StrundData(L"", L"[\\d-[13579]]+", L"١02468٠", RegexOptions::ECMAScript, {L"02468"}),
				StrundData(L"", L"[\\d-[13579]]+", L"١02468٠", RegexOptions::None, {L"١02468٠"}),

				StrundData(L"", L"[\\p{Ll}-[ae-z]]+", L"aaabbbcccdddeee", RegexOptions::None, {L"bbbcccddd"}),
				StrundData(L"", L"[\\p{Nd}-[2468]]+", L"20135798", RegexOptions::None, {L"013579"}),

				StrundData(L"", L"[\\P{Lu}-[ae-z]]+", L"aaabbbcccdddeee", RegexOptions::None, {L"bbbcccddd"}),
				StrundData(L"", L"[\\P{Nd}-[\\p{Ll}]]+", L"az09AZ'[]", RegexOptions::None, {L"AZ'[]"}),

				// (A - B) B is a superset of A (ie B contains chars that are in A plus other chars that are not in A)
				StrundData(L"", L"[abcd-[def]]+", L"fedddaabbccddd", RegexOptions::None, {L"aabbcc"}),

				StrundData(L"", L"[\\d-[357a-z]]+", L"az33312468955", RegexOptions::None, {L"124689"}),
				StrundData(L"", L"[\\d-[de357fgA-Z]]+", L"AZ51246897", RegexOptions::None, {L"124689"}),
				StrundData(L"", L"[\\d-[357\\p{Ll}]]+", L"az3312468977", RegexOptions::None, {L"124689"}),

				StrundData(L"", L"[\\w-[b-y\\s]]+", L" \tbbbaaaABCD09zzzyyy", RegexOptions::None, {L"aaaABCD09zzz"}),

				StrundData(L"", L"[\\w-[\\d\\p{Po}]]+", L"!#0AZaz9", RegexOptions::None, {L"AZaz"}),
				StrundData(L"", L"[\\w-[\\p{Ll}\\s]]+", L"a09AZz", RegexOptions::None, {L"09AZ"}),

				StrundData(L"", L"[\\d-[13579a-zA-Z]]+", L"AZ1024689", RegexOptions::ECMAScript, {L"02468"}),
				StrundData(L"", L"[\\d-[13579abcd]]+", L"abcd١02468٠", RegexOptions::ECMAScript, {L"02468"}),
				StrundData(L"", L"[\\d-[13579\\s]]+", L"\t١02468٠", RegexOptions::None, {L"١02468٠"}),

				StrundData(L"", L"[\\w-[b-y\\p{Po}]]+", L"!#bbbaaaABCD09zzzyyy", RegexOptions::None, {L"aaaABCD09zzz"}),

				StrundData(L"", L"[\\w-[b-y!.,]]+", L"!.,bbbaaaABCD09zzzyyy", RegexOptions::None, {L"aaaABCD09zzz"}),
				StrundData(L"", value_2, value_1, RegexOptions::None, {L"aaaABCD09zzz"}),

				StrundData(L"", L"[\\p{Ll}-[ae-z0-9]]+", L"09aaabbbcccdddeee", RegexOptions::None, {L"bbbcccddd"}),
				StrundData(L"", L"[\\p{Nd}-[2468az]]+", L"az20135798", RegexOptions::None, {L"013579"}),

				StrundData(L"", L"[\\P{Lu}-[ae-zA-Z]]+", L"AZaaabbbcccdddeee", RegexOptions::None, {L"bbbcccddd"}),
				StrundData(L"", L"[\\P{Nd}-[\\p{Ll}0123456789]]+", L"09az09AZ'[]", RegexOptions::None, {L"AZ'[]"}),

				// (A - B) B only contains chars that are not in A
				StrundData(L"", L"[abc-[defg]]+", L"dddaabbccddd", RegexOptions::None, {L"aabbcc"}),

				StrundData(L"", L"[\\d-[abc]]+", L"abc09abc", RegexOptions::None, {L"09"}),
				StrundData(L"", L"[\\d-[a-zA-Z]]+", L"az09AZ", RegexOptions::None, {L"09"}),
				StrundData(L"", L"[\\d-[\\p{Ll}]]+", L"az09az", RegexOptions::None, {L"09"}),

				StrundData(L"", L"[\\w-[\\x00-\\x0F]]+", L"bbbaaaABYZ09zzzyyy", RegexOptions::None, {L"bbbaaaABYZ09zzzyyy"}),

				StrundData(L"", L"[\\w-[\\s]]+", L"0AZaz9", RegexOptions::None, {L"0AZaz9"}),
				StrundData(L"", L"[\\w-[\\W]]+", L"0AZaz9", RegexOptions::None, {L"0AZaz9"}),
				StrundData(L"", L"[\\w-[\\p{Po}]]+", L"#a09AZz!", RegexOptions::None, {L"a09AZz"}),

				StrundData(L"", L"[\\d-[\\D]]+", L"azAZ1024689", RegexOptions::ECMAScript, {L"1024689"}),
				StrundData(L"", L"[\\d-[a-zA-Z]]+", L"azAZ١02468٠", RegexOptions::ECMAScript, {L"02468"}),
				StrundData(L"", L"[\\d-[\\p{Ll}]]+", L"١02468٠", RegexOptions::None, {L"١02468٠"}),

				StrundData(L"", L"[a-zA-Z0-9-[\\s]]+", L" \tazAZ09", RegexOptions::None, {L"azAZ09"}),

				StrundData(L"", L"[a-zA-Z0-9-[\\W]]+", L"bbbaaaABCD09zzzyyy", RegexOptions::None, {L"bbbaaaABCD09zzzyyy"}),
				StrundData(L"", L"[a-zA-Z0-9-[^a-zA-Z0-9]]+", L"bbbaaaABCD09zzzyyy", RegexOptions::None, {L"bbbaaaABCD09zzzyyy"}),

				StrundData(L"", L"[\\p{Ll}-[A-Z]]+", L"AZaz09", RegexOptions::None, {L"az"}),
				StrundData(L"", L"[\\p{Nd}-[a-z]]+", L"az09", RegexOptions::None, {L"09"}),

				StrundData(L"", L"[\\P{Lu}-[\\p{Lu}]]+", L"AZazAZ", RegexOptions::None, {L"az"}),
				StrundData(L"", L"[\\P{Lu}-[A-Z]]+", L"AZazAZ", RegexOptions::None, {L"az"}),
				StrundData(L"", L"[\\P{Nd}-[\\p{Nd}]]+", L"azAZ09", RegexOptions::None, {L"azAZ"}),
				StrundData(L"", L"[\\P{Nd}-[2-8]]+", L"1234567890azAZ1234567890", RegexOptions::None, {L"azAZ"}),

				// Alternating construct
				StrundData(L"", L"([ ]|[\\w-[0-9]])+", L"09az AZ90", RegexOptions::None, {L"az AZ", L"Z"}),
				StrundData(L"", L"([0-9-[02468]]|[0-9-[13579]])+", L"az1234567890za", RegexOptions::None, {L"1234567890", L"0"}),
				StrundData(L"", L"([^0-9-[a-zAE-Z]]|[\\w-[a-zAF-Z]])+", L"azBCDE1234567890BCDEFza", RegexOptions::None, {L"BCDE1234567890BCDE", L"E"}),
				StrundData(L"", L"([\\p{Ll}-[aeiou]]|[^\\w-[\\s]])+", L"aeiobcdxyz!@#aeio", RegexOptions::None, {L"bcdxyz!@#", L"#"}),
				StrundData(L"", L"(?:hello|hi){1,3}", L"hello", RegexOptions::None, {L"hello"}),
				StrundData(L"", L"(hello|hi){1,3}", L"hellohihey", RegexOptions::None, {L"hellohi", L"hi"}),
				StrundData(L"", L"(?:hello|hi){1,3}", L"hellohihey", RegexOptions::None, {L"hellohi"}),
				StrundData(L"", L"(?:hello|hi){2,2}", L"hellohihey", RegexOptions::None, {L"hellohi"}),
				StrundData(L"", L"(?:hello|hi){2,2}?", L"hellohihihello", RegexOptions::None, {L"hellohi"}),
				StrundData(L"", L"(?:abc|def|ghi|hij|klm|no){1,4}", L"this is a test nonoabcxyz this is only a test", RegexOptions::None, {L"nonoabc"}),
				StrundData(L"", L"xyz(abc|def)xyz", L"abcxyzdefxyzabc", RegexOptions::None, {L"xyzdefxyz", L"def"}),
				StrundData(L"", L"abc|(?:def|ghi)", L"ghi", RegexOptions::None, {L"ghi"}),
				StrundData(L"", L"abc|(def|ghi)", L"def", RegexOptions::None, {L"def", L"def"}),

				// Multiple character classes using character class subtraction
				StrundData(L"", L"98[\\d-[9]][\\d-[8]][\\d-[0]]", L"98911 98881 98870 98871", RegexOptions::None, {L"98871"}),
				StrundData(L"", L"m[\\w-[^aeiou]][\\w-[^aeiou]]t", L"mbbt mect meet", RegexOptions::None, {L"meet"}),

				// Negation with character class subtraction
				StrundData(L"", L"[abcdef-[^bce]]+", L"adfbcefda", RegexOptions::None, {L"bce"}),
				StrundData(L"", L"[^cde-[ag]]+", L"agbfxyzga", RegexOptions::None, {L"bfxyz"}),

				// Misc The idea here is come up with real world examples of char class subtraction. Things that
				// would be difficult to define without it
				StrundData(L"", L"[\\p{L}-[^\\p{Lu}]]+", L"09',.abcxyzABCXYZ", RegexOptions::None, {L"ABCXYZ"}),

				StrundData(L"", L"[\\p{IsGreek}-[\\P{Lu}]]+", L"\u0390\u03FE\u0386\u0388\u03EC\u03EE\u0400", RegexOptions::None, {L"\u03FE\u0386\u0388\u03EC\u03EE"}),
				StrundData(L"", L"[\\p{IsBasicLatin}-[G-L]]+", L"GAFMZL", RegexOptions::None, {L"AFMZ"}),

				StrundData(L"", L"[a-zA-Z-[aeiouAEIOU]]+", L"aeiouAEIOUbcdfghjklmnpqrstvwxyz", RegexOptions::None, {L"bcdfghjklmnpqrstvwxyz"}),

				// The following is an overly complex way of matching an ip address using char class subtraction

				// Character Class Substraction
				StrundData(L"", L"[abcd\\-d-[bc]]+", L"bbbaaa---dddccc", RegexOptions::None, {L"aaa---ddd"}),
				StrundData(L"", L"[^a-f-[\\x00-\\x60\\u007B-\\uFFFF]]+", L"aaafffgggzzz{{{", RegexOptions::None, {L"gggzzz"}),
				StrundData(L"", L"[\\[\\]a-f-[[]]+", L"gggaaafff]]][[[", RegexOptions::None, {L"aaafff]]]"}),
				StrundData(L"", L"[\\[\\]a-f-[]]]+", L"gggaaafff[[[]]]", RegexOptions::None, {L"aaafff[[["}),

				StrundData(L"", L"[ab\\-\\[cd-[-[]]]]", L"a]]", RegexOptions::None, {L"a]]"}),
				StrundData(L"", L"[ab\\-\\[cd-[-[]]]]", L"b]]", RegexOptions::None, {L"b]]"}),
				StrundData(L"", L"[ab\\-\\[cd-[-[]]]]", L"c]]", RegexOptions::None, {L"c]]"}),
				StrundData(L"", L"[ab\\-\\[cd-[-[]]]]", L"d]]", RegexOptions::None, {L"d]]"}),

				StrundData(L"", L"[ab\\-\\[cd-[[]]]]", L"a]]", RegexOptions::None, {L"a]]"}),
				StrundData(L"", L"[ab\\-\\[cd-[[]]]]", L"b]]", RegexOptions::None, {L"b]]"}),
				StrundData(L"", L"[ab\\-\\[cd-[[]]]]", L"c]]", RegexOptions::None, {L"c]]"}),
				StrundData(L"", L"[ab\\-\\[cd-[[]]]]", L"d]]", RegexOptions::None, {L"d]]"}),
				StrundData(L"", L"[ab\\-\\[cd-[[]]]]", L"-]]", RegexOptions::None, {L"-]]"}),

				StrundData(L"", L"[a-[c-e]]+", L"bbbaaaccc", RegexOptions::None, {L"aaa"}),
				StrundData(L"", L"[a-[c-e]]+", L"```aaaccc", RegexOptions::None, {L"aaa"}),

				StrundData(L"", L"[a-d\\--[bc]]+", L"cccaaa--dddbbb", RegexOptions::None, {L"aaa--ddd"}),

				// Not Character class substraction
				StrundData(L"", L"[\\0- [bc]+", value_3, RegexOptions::None, {value_4}),
				StrundData(L"", L"[[abcd]-[bc]]+", L"a-b]", RegexOptions::None, {L"a-b]"}),
				StrundData(L"", L"[-[e-g]+", L"ddd[[[---eeefffggghhh", RegexOptions::None, {L"[[[---eeefffggg"}),
				StrundData(L"", L"[-e-g]+", L"ddd---eeefffggghhh", RegexOptions::None, {L"---eeefffggg"}),
				StrundData(L"", L"[a-e - m-p]+", L"---a b c d e m n o p---", RegexOptions::None, {L"a b c d e m n o p"}),
				StrundData(L"", L"[^-[bc]]", L"b] c] -] aaaddd]", RegexOptions::None, {L"d]"}),
				StrundData(L"", L"[^-[bc]]", L"b] c] -] aaa]ddd]", RegexOptions::None, {L"a]"}),

				// Make sure we correctly handle \-
				StrundData(L"", L"[a\\-[bc]+", L"```bbbaaa---[[[cccddd", RegexOptions::None, {L"bbbaaa---[[[ccc"}),
				StrundData(L"", L"[a\\-[\\-\\-bc]+", L"```bbbaaa---[[[cccddd", RegexOptions::None, {L"bbbaaa---[[[ccc"}),
				StrundData(L"", L"[a\\-\\[\\-\\[\\-bc]+", L"```bbbaaa---[[[cccddd", RegexOptions::None, {L"bbbaaa---[[[ccc"}),
				StrundData(L"", L"[abc\\--[b]]+", L"[[[```bbbaaa---cccddd", RegexOptions::None, {L"aaa---ccc"}),
				StrundData(L"", L"[abc\\-z-[b]]+", L"```aaaccc---zzzbbb", RegexOptions::None, {L"aaaccc---zzz"}),
				StrundData(L"", L"[a-d\\-[b]+", L"```aaabbbcccddd----[[[[]]]", RegexOptions::None, {L"aaabbbcccddd----[[[["}),
				StrundData(L"", L"[abcd\\-d\\-[bc]+", L"bbbaaa---[[[dddccc", RegexOptions::None, {L"bbbaaa---[[[dddccc"}),

				// Everything works correctly with option RegexOptions::IgnorePatternWhitespace
				StrundData(L"", L"[a - c - [ b ] ]+", L"dddaaa   ccc [[[[ bbb ]]]", RegexOptions::IgnorePatternWhitespace, {L" ]]]"}),
				StrundData(L"", L"[a - c - [ b ] +", L"dddaaa   ccc [[[[ bbb ]]]", RegexOptions::IgnorePatternWhitespace, {L"aaa   ccc [[[[ bbb "}),

				// Unicode Char Classes
				StrundData(L"", L"(\\p{Lu}\\w*)\\s(\\p{Lu}\\w*)", L"Hello World", RegexOptions::None, {L"Hello World", L"Hello", L"World"}),
				StrundData(L"", L"(\\p{Lu}\\p{Ll}*)\\s(\\p{Lu}\\p{Ll}*)", L"Hello World", RegexOptions::None, {L"Hello World", L"Hello", L"World"}),
				StrundData(L"", L"(\\P{Ll}\\p{Ll}*)\\s(\\P{Ll}\\p{Ll}*)", L"Hello World", RegexOptions::None, {L"Hello World", L"Hello", L"World"}),
				StrundData(L"", L"(\\P{Lu}+\\p{Lu})\\s(\\P{Lu}+\\p{Lu})", L"hellO worlD", RegexOptions::None, {L"hellO worlD", L"hellO", L"worlD"}),
				StrundData(L"", L"(\\p{Lt}\\w*)\\s(\\p{Lt}*\\w*)", L"\u01C5ello \u01C5orld", RegexOptions::None, {L"\u01C5ello \u01C5orld", L"\u01C5ello", L"\u01C5orld"}),
				StrundData(L"", L"(\\P{Lt}\\w*)\\s(\\P{Lt}*\\w*)", L"Hello World", RegexOptions::None, {L"Hello World", L"Hello", L"World"}),

				// Character ranges IgnoreCase
				StrundData(L"", L"[@-D]+", L"eE?@ABCDabcdeE", RegexOptions::IgnoreCase, {L"@ABCDabcd"}),
				StrundData(L"", L"[>-D]+", L"eE=>?@ABCDabcdeE", RegexOptions::IgnoreCase, {L">?@ABCDabcd"}),
				StrundData(L"", L"[\u0554-\u0557]+", L"\u0583\u0553\u0554\u0555\u0556\u0584\u0585\u0586\u0557\u0558", RegexOptions::IgnoreCase, {L"\u0554\u0555\u0556\u0584\u0585\u0586\u0557"}),
				StrundData(L"", L"[X-\\]]+", L"wWXYZxyz[\\]^", RegexOptions::IgnoreCase, {L"XYZxyz[\\]"}),
				StrundData(L"", L"[X-\u0533]+", L"\u0551\u0554\u0560AXYZaxyz\u0531\u0532\u0533\u0561\u0562\u0563\u0564", RegexOptions::IgnoreCase, {L"AXYZaxyz\u0531\u0532\u0533\u0561\u0562\u0563"}),
				StrundData(L"", L"[X-a]+", L"wWAXYZaxyz", RegexOptions::IgnoreCase, {L"AXYZaxyz"}),
				StrundData(L"", L"[X-c]+", L"wWABCXYZabcxyz", RegexOptions::IgnoreCase, {L"ABCXYZabcxyz"}),
				StrundData(L"", L"[X-\u00C0]+", L"\u00C1\u00E1\u00C0\u00E0wWABCXYZabcxyz", RegexOptions::IgnoreCase, {L"\u00C0\u00E0wWABCXYZabcxyz"}),
				StrundData(L"", L"[\u0100\u0102\u0104]+", L"\u00FF \u0100\u0102\u0104\u0101\u0103\u0105\u0106", RegexOptions::IgnoreCase, {L"\u0100\u0102\u0104\u0101\u0103\u0105"}),
				StrundData(L"", L"[B-D\u0130]+", L"aAeE\u0129\u0131\u0068 BCDbcD\u0130\u0069\u0070", RegexOptions::IgnoreCase, {L"BCDbcD\u0130\u0069"}),
				StrundData(L"", L"[\u013B\u013D\u013F]+", L"\u013A\u013B\u013D\u013F\u013C\u013E\u0140\u0141", RegexOptions::IgnoreCase, {L"\u013B\u013D\u013F\u013C\u013E\u0140"}),

				// Escape Chars
				StrundData(L"", L"(Cat)\r(Dog)", L"Cat\rDog", RegexOptions::None, {L"Cat\rDog", L"Cat", L"Dog"}),
				StrundData(L"", L"(Cat)\t(Dog)", L"Cat\tDog", RegexOptions::None, {L"Cat\tDog", L"Cat", L"Dog"}),
				StrundData(L"", L"(Cat)\f(Dog)", L"Cat\fDog", RegexOptions::None, {L"Cat\fDog", L"Cat", L"Dog"}),

				// Miscellaneous { witout matching }
				StrundData(L"", L"{5", L"hello {5 world", RegexOptions::None, {L"{5"}),
				StrundData(L"", L"{5,", L"hello {5, world", RegexOptions::None, {L"{5,"}),
				StrundData(L"", L"{5,6", L"hello {5,6 world", RegexOptions::None, {L"{5,6"}),

				// Miscellaneous inline options
				StrundData(L"", L"(?n:(?<cat>cat)(\\s+)(?<dog>dog))", L"cat   dog", RegexOptions::None, {L"cat   dog", L"cat", L"dog"}),
				StrundData(L"", L"(?n:(cat)(\\s+)(dog))", L"cat   dog", RegexOptions::None, {L"cat   dog"}),
				StrundData(L"", L"(?n:(cat)(?<SpaceChars>\\s+)(dog))", L"cat   dog", RegexOptions::None, {L"cat   dog", L"   "}),
				StrundData(L"", L"(?+i:cat)", L"CAT", RegexOptions::None, {L"CAT"}),

				// \d, \D, \s, \S, \w, \W, \P, \p inside character range
				StrundData(L"", L"cat([\\d]*)dog", L"hello123cat230927dog1412d", RegexOptions::None, {L"cat230927dog", L"230927"}),
				StrundData(L"", L"([\\D]*)dog", L"65498catdog58719", RegexOptions::None, {L"catdog", L"cat"}),
				StrundData(L"", L"cat([\\s]*)dog", L"wiocat   dog3270", RegexOptions::None, {L"cat   dog", L"   "}),
				StrundData(L"", L"cat([\\S]*)", L"sfdcatdog    3270", RegexOptions::None, {L"catdog", L"dog"}),
				StrundData(L"", L"cat([\\w]*)", L"sfdcatdog    3270", RegexOptions::None, {L"catdog", L"dog"}),
				StrundData(L"", L"cat([\\W]*)dog", L"wiocat   dog3270", RegexOptions::None, {L"cat   dog", L"   "}),
				StrundData(L"", L"([\\p{Lu}]\\w*)\\s([\\p{Lu}]\\w*)", L"Hello World", RegexOptions::None, {L"Hello World", L"Hello", L"World"}),
				StrundData(L"", L"([\\P{Ll}][\\p{Ll}]*)\\s([\\P{Ll}][\\p{Ll}]*)", L"Hello World", RegexOptions::None, {L"Hello World", L"Hello", L"World"}),

				// \x, \u, \a, \b, \e, \f, \n, \r, \t, \v, \c, inside character range
				StrundData(L"", L"(cat)([\\x41]*)(dog)", L"catAAAdog", RegexOptions::None, {L"catAAAdog", L"cat", L"AAA", L"dog"}),
				StrundData(L"", L"(cat)([\\u0041]*)(dog)", L"catAAAdog", RegexOptions::None, {L"catAAAdog", L"cat", L"AAA", L"dog"}),
				StrundData(L"", L"(cat)([\\a]*)(dog)", L"cat\a\a\adog", RegexOptions::None, {L"cat\a\a\adog", L"cat", L"\a\a\a", L"dog"}),
				StrundData(L"", L"(cat)([\\b]*)(dog)", L"cat\b\b\bdog", RegexOptions::None, {L"cat\b\b\bdog", L"cat", L"\b\b\b", L"dog"}),
				StrundData(L"", L"(cat)([\\e]*)(dog)", L"cat\u001B\u001B\u001Bdog", RegexOptions::None, {L"cat\u001B\u001B\u001Bdog", L"cat", L"\u001B\u001B\u001B", L"dog"}),
				StrundData(L"", L"(cat)([\\f]*)(dog)", L"cat\f\f\fdog", RegexOptions::None, {L"cat\f\f\fdog", L"cat", L"\f\f\f", L"dog"}),
				StrundData(L"", L"(cat)([\\r]*)(dog)", L"cat\r\r\rdog", RegexOptions::None, {L"cat\r\r\rdog", L"cat", L"\r\r\r", L"dog"}),
				StrundData(L"", L"(cat)([\\v]*)(dog)", L"cat\v\v\vdog", RegexOptions::None, {L"cat\v\v\vdog", L"cat", L"\v\v\v", L"dog"}),

				// \d, \D, \s, \S, \w, \W, \P, \p inside character range ([0-5]) with ECMA Option
				StrundData(L"", L"cat([\\d]*)dog", L"hello123cat230927dog1412d", RegexOptions::ECMAScript, {L"cat230927dog", L"230927"}),
				StrundData(L"", L"([\\D]*)dog", L"65498catdog58719", RegexOptions::ECMAScript, {L"catdog", L"cat"}),
				StrundData(L"", L"cat([\\s]*)dog", L"wiocat   dog3270", RegexOptions::ECMAScript, {L"cat   dog", L"   "}),
				StrundData(L"", L"cat([\\S]*)", L"sfdcatdog    3270", RegexOptions::ECMAScript, {L"catdog", L"dog"}),
				StrundData(L"", L"cat([\\w]*)", L"sfdcatdog    3270", RegexOptions::ECMAScript, {L"catdog", L"dog"}),
				StrundData(L"", L"cat([\\W]*)dog", L"wiocat   dog3270", RegexOptions::ECMAScript, {L"cat   dog", L"   "}),
				StrundData(L"", L"([\\p{Lu}]\\w*)\\s([\\p{Lu}]\\w*)", L"Hello World", RegexOptions::ECMAScript, {L"Hello World", L"Hello", L"World"}),
				StrundData(L"", L"([\\P{Ll}][\\p{Ll}]*)\\s([\\P{Ll}][\\p{Ll}]*)", L"Hello World", RegexOptions::ECMAScript, {L"Hello World", L"Hello", L"World"}),

				// \d, \D, \s, \S, \w, \W, \P, \p outside character range ([0-5]) with ECMA Option
				StrundData(L"", L"(cat)\\d*dog", L"hello123cat230927dog1412d", RegexOptions::ECMAScript, {L"cat230927dog", L"cat"}),
				StrundData(L"", L"\\D*(dog)", L"65498catdog58719", RegexOptions::ECMAScript, {L"catdog", L"dog"}),
				StrundData(L"", L"(cat)\\s*(dog)", L"wiocat   dog3270", RegexOptions::ECMAScript, {L"cat   dog", L"cat", L"dog"}),
				StrundData(L"", L"(cat)\\S*", L"sfdcatdog    3270", RegexOptions::ECMAScript, {L"catdog", L"cat"}),
				StrundData(L"", L"(cat)\\w*", L"sfdcatdog    3270", RegexOptions::ECMAScript, {L"catdog", L"cat"}),
				StrundData(L"", L"(cat)\\W*(dog)", L"wiocat   dog3270", RegexOptions::ECMAScript, {L"cat   dog", L"cat", L"dog"}),
				StrundData(L"", L"\\p{Lu}(\\w*)\\s\\p{Lu}(\\w*)", L"Hello World", RegexOptions::ECMAScript, {L"Hello World", L"ello", L"orld"}),
				StrundData(L"", L"\\P{Ll}\\p{Ll}*\\s\\P{Ll}\\p{Ll}*", L"Hello World", RegexOptions::ECMAScript, {L"Hello World"}),

				// Use < in a group
				StrundData(L"", L"cat(?<dog121>dog)", L"catcatdogdogcat", RegexOptions::None, {L"catdog", L"dog"}),
				StrundData(L"", L"(?<cat>cat)\\s*(?<cat>dog)", L"catcat    dogdogcat", RegexOptions::None, {L"cat    dog", L"dog"}),
				StrundData(L"", L"(?<1>cat)\\s*(?<1>dog)", L"catcat    dogdogcat", RegexOptions::None, {L"cat    dog", L"dog"}),
				StrundData(L"", L"(?<2048>cat)\\s*(?<2048>dog)", L"catcat    dogdogcat", RegexOptions::None, {L"cat    dog", L"dog"}),
				StrundData(L"", L"(?<cat>cat)\\w+(?<dog-cat>dog)", L"cat_Hello_World_dog", RegexOptions::None, {L"cat_Hello_World_dog", L"", L"_Hello_World_"}),
				StrundData(L"", L"(?<cat>cat)\\w+(?<-cat>dog)", L"cat_Hello_World_dog", RegexOptions::None, {L"cat_Hello_World_dog", L""}),
				StrundData(L"", L"(?<cat>cat)\\w+(?<cat-cat>dog)", L"cat_Hello_World_dog", RegexOptions::None, {L"cat_Hello_World_dog", L"_Hello_World_"}),
				StrundData(L"", L"(?<1>cat)\\w+(?<dog-1>dog)", L"cat_Hello_World_dog", RegexOptions::None, {L"cat_Hello_World_dog", L"", L"_Hello_World_"}),
				StrundData(L"", L"(?<cat>cat)\\w+(?<2-cat>dog)", L"cat_Hello_World_dog", RegexOptions::None, {L"cat_Hello_World_dog", L"", L"_Hello_World_"}),
				StrundData(L"", L"(?<1>cat)\\w+(?<2-1>dog)", L"cat_Hello_World_dog", RegexOptions::None, {L"cat_Hello_World_dog", L"", L"_Hello_World_"}),

				// Quantifiers
				StrundData(L"", L"(?<cat>cat){", L"STARTcat{", RegexOptions::None, {L"cat{", L"cat"}),
				StrundData(L"", L"(?<cat>cat){fdsa", L"STARTcat{fdsa", RegexOptions::None, {L"cat{fdsa", L"cat"}),
				StrundData(L"", L"(?<cat>cat){1", L"STARTcat{1", RegexOptions::None, {L"cat{1", L"cat"}),
				StrundData(L"", L"(?<cat>cat){1END", L"STARTcat{1END", RegexOptions::None, {L"cat{1END", L"cat"}),
				StrundData(L"", L"(?<cat>cat){1,", L"STARTcat{1,", RegexOptions::None, {L"cat{1,", L"cat"}),
				StrundData(L"", L"(?<cat>cat){1,END", L"STARTcat{1,END", RegexOptions::None, {L"cat{1,END", L"cat"}),
				StrundData(L"", L"(?<cat>cat){1,2", L"STARTcat{1,2", RegexOptions::None, {L"cat{1,2", L"cat"}),
				StrundData(L"", L"(?<cat>cat){1,2END", L"STARTcat{1,2END", RegexOptions::None, {L"cat{1,2END", L"cat"}),

				//// Use IgnorePatternWhitespace
				StrundData(L"", L"(cat) #cat\n\t\t\t\\s + #followed by 1 or more whitespace \n\t\t\t(dog)  #followed by dog\n", L"cat    dog", RegexOptions::IgnorePatternWhitespace, {L"cat    dog", L"cat", L"dog"}),
				StrundData(L"", L"(cat) #cat\n\t\t\t\\s + #followed by 1 or more whitespace \n\t\t\t(dog)  #followed by dog", L"cat    dog", RegexOptions::IgnorePatternWhitespace, {L"cat    dog", L"cat", L"dog"}),
				StrundData(L"", L"(cat) (?#cat)    \\s+ (?#followed by 1 or more whitespace) (dog)  (?#followed by dog)", L"cat    dog", RegexOptions::IgnorePatternWhitespace, {L"cat    dog", L"cat", L"dog"}),

				// Back Reference
				StrundData(L"", L"(?<cat>cat)(?<dog>dog)\\k<cat>", L"asdfcatdogcatdog", RegexOptions::None, {L"catdogcat", L"cat", L"dog"}),
				StrundData(L"", L"(?<cat>cat)\\s+(?<dog>dog)\\k<cat>", L"asdfcat   dogcat   dog", RegexOptions::None, {L"cat   dogcat", L"cat", L"dog"}),
				StrundData(L"", L"(?<cat>cat)\\s+(?<dog>dog)\\k'cat'", L"asdfcat   dogcat   dog", RegexOptions::None, {L"cat   dogcat", L"cat", L"dog"}),
				StrundData(L"", L"(?<cat>cat)\\s+(?<dog>dog)\\<cat>", L"asdfcat   dogcat   dog", RegexOptions::None, {L"cat   dogcat", L"cat", L"dog"}),
				StrundData(L"", L"(?<cat>cat)\\s+(?<dog>dog)\\'cat'", L"asdfcat   dogcat   dog", RegexOptions::None, {L"cat   dogcat", L"cat", L"dog"}),

				StrundData(L"", L"(?<cat>cat)\\s+(?<dog>dog)\\k<1>", L"asdfcat   dogcat   dog", RegexOptions::None, {L"cat   dogcat", L"cat", L"dog"}),
				StrundData(L"", L"(?<cat>cat)\\s+(?<dog>dog)\\k'1'", L"asdfcat   dogcat   dog", RegexOptions::None, {L"cat   dogcat", L"cat", L"dog"}),
				StrundData(L"", L"(?<cat>cat)\\s+(?<dog>dog)\\<1>", L"asdfcat   dogcat   dog", RegexOptions::None, {L"cat   dogcat", L"cat", L"dog"}),
				StrundData(L"", L"(?<cat>cat)\\s+(?<dog>dog)\\'1'", L"asdfcat   dogcat   dog", RegexOptions::None, {L"cat   dogcat", L"cat", L"dog"}),
				StrundData(L"", L"(?<cat>cat)\\s+(?<dog>dog)\\1", L"asdfcat   dogcat   dog", RegexOptions::None, {L"cat   dogcat", L"cat", L"dog"}),
				StrundData(L"", L"(?<cat>cat)\\s+(?<dog>dog)\\1", L"asdfcat   dogcat   dog", RegexOptions::ECMAScript, {L"cat   dogcat", L"cat", L"dog"}),

				StrundData(L"", L"(?<cat>cat)\\s+(?<dog>dog)\\k<dog>", L"asdfcat   dogdog   dog", RegexOptions::None, {L"cat   dogdog", L"cat", L"dog"}),
				StrundData(L"", L"(?<cat>cat)\\s+(?<dog>dog)\\2", L"asdfcat   dogdog   dog", RegexOptions::None, {L"cat   dogdog", L"cat", L"dog"}),
				StrundData(L"", L"(?<cat>cat)\\s+(?<dog>dog)\\2", L"asdfcat   dogdog   dog", RegexOptions::ECMAScript, {L"cat   dogdog", L"cat", L"dog"}),

				// Octal
				StrundData(L"", L"(cat)(\\077)", L"hellocat?dogworld", RegexOptions::None, {L"cat?", L"cat", L"?"}),
				StrundData(L"", L"(cat)(\\77)", L"hellocat?dogworld", RegexOptions::None, {L"cat?", L"cat", L"?"}),
				StrundData(L"", L"(cat)(\\176)", L"hellocat~dogworld", RegexOptions::None, {L"cat~", L"cat", L"~"}),
				StrundData(L"", L"(cat)(\\400)", L"hellocat" + UtilsString::StringInitSpe(0x00) + L"dogworld", RegexOptions::None, {L"cat" + UtilsString::StringInitSpe(0x00), L"cat", UtilsString::StringInitSpe(0x00)}),
				StrundData(L"", L"(cat)(\\300)", L"hellocat\u00C0dogworld", RegexOptions::None, {L"cat\u00C0", L"cat", L"\u00C0"}),
				StrundData(L"", L"(cat)(\\477)", L"hellocat\u003Fdogworld", RegexOptions::None, {L"cat\u003F", L"cat", L"\u003F"}),
				StrundData(L"", L"(cat)(\\777)", L"hellocat\u00FFdogworld", RegexOptions::None, {L"cat\u00FF", L"cat", L"\u00FF"}),
				StrundData(L"", L"(cat)(\\7770)", L"hellocat\u00FF0dogworld", RegexOptions::None, {L"cat\u00FF0", L"cat", L"\u00FF0"}),

				StrundData(L"", L"(cat)(\\077)", L"hellocat?dogworld", RegexOptions::ECMAScript, {L"cat?", L"cat", L"?"}),
				StrundData(L"", L"(cat)(\\77)", L"hellocat?dogworld", RegexOptions::ECMAScript, {L"cat?", L"cat", L"?"}),
				StrundData(L"", L"(cat)(\\7)", L"hellocat\adogworld", RegexOptions::ECMAScript, {L"cat\a", L"cat", L"\a"}),
				StrundData(L"", L"(cat)(\\40)", L"hellocat dogworld", RegexOptions::ECMAScript, {L"cat ", L"cat", L" "}),
				StrundData(L"", L"(cat)(\\040)", L"hellocat dogworld", RegexOptions::ECMAScript, {L"cat ", L"cat", L" "}),
				StrundData(L"", L"(cat)(\\176)", L"hellocatcat76dogworld", RegexOptions::ECMAScript, {L"catcat76", L"cat", L"cat76"}),
				StrundData(L"", L"(cat)(\\377)", L"hellocat\u00FFdogworld", RegexOptions::ECMAScript, {L"cat\u00FF", L"cat", L"\u00FF"}),
				StrundData(L"", L"(cat)(\\400)", L"hellocat 0Fdogworld", RegexOptions::ECMAScript, {L"cat 0", L"cat", L" 0"}),

				// Decimal
				StrundData(L"", L"(cat)\\s+(?<2147483646>dog)", L"asdlkcat  dogiwod", RegexOptions::None, {L"cat  dog", L"cat", L"dog"}),
				StrundData(L"", L"(cat)\\s+(?<2147483647>dog)", L"asdlkcat  dogiwod", RegexOptions::None, {L"cat  dog", L"cat", L"dog"}),

				// Hex
				StrundData(L"", L"(cat)(\\x2a*)(dog)", L"asdlkcat***dogiwod", RegexOptions::None, {L"cat***dog", L"cat", L"***", L"dog"}),
				StrundData(L"", L"(cat)(\\x2b*)(dog)", L"asdlkcat+++dogiwod", RegexOptions::None, {L"cat+++dog", L"cat", L"+++", L"dog"}),
				StrundData(L"", L"(cat)(\\x2c*)(dog)", L"asdlkcat,,,dogiwod", RegexOptions::None, {L"cat,,,dog", L"cat", L",,,", L"dog"}),
				StrundData(L"", L"(cat)(\\x2d*)(dog)", L"asdlkcat---dogiwod", RegexOptions::None, {L"cat---dog", L"cat", L"---", L"dog"}),
				StrundData(L"", L"(cat)(\\x2e*)(dog)", L"asdlkcat...dogiwod", RegexOptions::None, {L"cat...dog", L"cat", L"...", L"dog"}),
				StrundData(L"", L"(cat)(\\x2f*)(dog)", L"asdlkcat///dogiwod", RegexOptions::None, {L"cat///dog", L"cat", L"///", L"dog"}),

				StrundData(L"", L"(cat)(\\x2A*)(dog)", L"asdlkcat***dogiwod", RegexOptions::None, {L"cat***dog", L"cat", L"***", L"dog"}),
				StrundData(L"", L"(cat)(\\x2B*)(dog)", L"asdlkcat+++dogiwod", RegexOptions::None, {L"cat+++dog", L"cat", L"+++", L"dog"}),
				StrundData(L"", L"(cat)(\\x2C*)(dog)", L"asdlkcat,,,dogiwod", RegexOptions::None, {L"cat,,,dog", L"cat", L",,,", L"dog"}),
				StrundData(L"", L"(cat)(\\x2D*)(dog)", L"asdlkcat---dogiwod", RegexOptions::None, {L"cat---dog", L"cat", L"---", L"dog"}),
				StrundData(L"", L"(cat)(\\x2E*)(dog)", L"asdlkcat...dogiwod", RegexOptions::None, {L"cat...dog", L"cat", L"...", L"dog"}),
				StrundData(L"", L"(cat)(\\x2F*)(dog)", L"asdlkcat///dogiwod", RegexOptions::None, {L"cat///dog", L"cat", L"///", L"dog"}),

				// ScanControl
				StrundData(L"", L"(cat)(\\c@*)(dog)", L"asdlkcat" + UtilsString::StringInitSpe(0x00) + UtilsString::StringInitSpe(0x00) + L"dogiwod", RegexOptions::None, {L"cat" + UtilsString::StringInitSpe(0x00) + UtilsString::StringInitSpe(0x00) + L"dog", L"cat", UtilsString::StringInitSpe(0x00) + UtilsString::StringInitSpe(0x00), L"dog"}),
				StrundData(L"", L"(cat)(\\cA*)(dog)", L"asdlkcatdogiwod", RegexOptions::None, {L"cat\u0001dog", L"cat", L"\u0001", L"dog"}),
				StrundData(L"", L"(cat)(\\ca*)(dog)", L"asdlkcatdogiwod", RegexOptions::None, {L"cat\u0001dog", L"cat", L"\u0001", L"dog"}),

				StrundData(L"", L"(cat)(\\cC*)(dog)", L"asdlkcatdogiwod", RegexOptions::None, {L"cat\u0003dog", L"cat", L"\u0003", L"dog"}),
				StrundData(L"", L"(cat)(\\cc*)(dog)", L"asdlkcatdogiwod", RegexOptions::None, {L"cat\u0003dog", L"cat", L"\u0003", L"dog"}),

				StrundData(L"", L"(cat)(\\cD*)(dog)", L"asdlkcatdogiwod", RegexOptions::None, {L"cat\u0004dog", L"cat", L"\u0004", L"dog"}),
				StrundData(L"", L"(cat)(\\cd*)(dog)", L"asdlkcatdogiwod", RegexOptions::None, {L"cat\u0004dog", L"cat", L"\u0004", L"dog"}),

				StrundData(L"", L"(cat)(\\cX*)(dog)", L"asdlkcatdogiwod", RegexOptions::None, {L"cat\u0018dog", L"cat", L"\u0018", L"dog"}),
				StrundData(L"", L"(cat)(\\cx*)(dog)", L"asdlkcatdogiwod", RegexOptions::None, {L"cat\u0018dog", L"cat", L"\u0018", L"dog"}),

				StrundData(L"", L"(cat)(\\cZ*)(dog)", L"asdlkcat" + UtilsString::StringInitSpe(0x001a) + L"dogiwod", RegexOptions::None, {L"cat" + UtilsString::StringInitSpe(0x001a) + L"dog", L"cat", UtilsString::StringInitSpe(0x001a), L"dog"}),
				StrundData(L"", L"(cat)(\\cz*)(dog)", L"asdlkcat" + UtilsString::StringInitSpe(0x001a) + L"dogiwod", RegexOptions::None, {L"cat" + UtilsString::StringInitSpe(0x001a) + L"dog", L"cat", UtilsString::StringInitSpe(0x001a), L"dog"}),

				// Atomic Zero-Width Assertions \A \G ^ \Z \z \b \B
				//\A
				StrundData(L"", L"\\Acat\\s+dog", L"cat   \n\n\n   dog", RegexOptions::None, {L"cat   \n\n\n   dog"}),
				StrundData(L"", L"\\Acat\\s+dog", L"cat   \n\n\n   dog", RegexOptions::Multiline, {L"cat   \n\n\n   dog"}),
				StrundData(L"", L"\\A(cat)\\s+(dog)", L"cat   \n\n\n   dog", RegexOptions::None, {L"cat   \n\n\n   dog", L"cat", L"dog"}),
				StrundData(L"", L"\\A(cat)\\s+(dog)", L"cat   \n\n\n   dog", RegexOptions::Multiline, {L"cat   \n\n\n   dog", L"cat", L"dog"}),

				//\G
				StrundData(L"", L"\\Gcat\\s+dog", L"cat   \n\n\n   dog", RegexOptions::None, {L"cat   \n\n\n   dog"}),
				StrundData(L"", L"\\Gcat\\s+dog", L"cat   \n\n\n   dog", RegexOptions::Multiline, {L"cat   \n\n\n   dog"}),
				StrundData(L"", L"\\Gcat\\s+dog", L"cat   \n\n\n   dog", RegexOptions::ECMAScript, {L"cat   \n\n\n   dog"}),
				StrundData(L"", L"\\G(cat)\\s+(dog)", L"cat   \n\n\n   dog", RegexOptions::None, {L"cat   \n\n\n   dog", L"cat", L"dog"}),
				StrundData(L"", L"\\G(cat)\\s+(dog)", L"cat   \n\n\n   dog", RegexOptions::Multiline, {L"cat   \n\n\n   dog", L"cat", L"dog"}),
				StrundData(L"", L"\\G(cat)\\s+(dog)", L"cat   \n\n\n   dog", RegexOptions::ECMAScript, {L"cat   \n\n\n   dog", L"cat", L"dog"}),

				//^
				StrundData(L"", L"^cat\\s+dog", L"cat   \n\n\n   dog", RegexOptions::None, {L"cat   \n\n\n   dog"}),
				StrundData(L"", L"^cat\\s+dog", L"cat   \n\n\n   dog", RegexOptions::Multiline, {L"cat   \n\n\n   dog"}),
				StrundData(L"", L"mouse\\s\\n^cat\\s+dog", L"mouse\n\ncat   \n\n\n   dog", RegexOptions::Multiline, {L"mouse\n\ncat   \n\n\n   dog"}),
				StrundData(L"", L"^cat\\s+dog", L"cat   \n\n\n   dog", RegexOptions::ECMAScript, {L"cat   \n\n\n   dog"}),
				StrundData(L"", L"^(cat)\\s+(dog)", L"cat   \n\n\n   dog", RegexOptions::None, {L"cat   \n\n\n   dog", L"cat", L"dog"}),
				StrundData(L"", L"^(cat)\\s+(dog)", L"cat   \n\n\n   dog", RegexOptions::Multiline, {L"cat   \n\n\n   dog", L"cat", L"dog"}),
				StrundData(L"", L"(mouse)\\s\\n^(cat)\\s+(dog)", L"mouse\n\ncat   \n\n\n   dog", RegexOptions::Multiline, {L"mouse\n\ncat   \n\n\n   dog", L"mouse", L"cat", L"dog"}),
				StrundData(L"", L"^(cat)\\s+(dog)", L"cat   \n\n\n   dog", RegexOptions::ECMAScript, {L"cat   \n\n\n   dog", L"cat", L"dog"}),

				//\Z
				StrundData(L"", L"cat\\s+dog\\Z", L"cat   \n\n\n   dog", RegexOptions::None, {L"cat   \n\n\n   dog"}),
				StrundData(L"", L"cat\\s+dog\\Z", L"cat   \n\n\n   dog", RegexOptions::Multiline, {L"cat   \n\n\n   dog"}),
				StrundData(L"", L"cat\\s+dog\\Z", L"cat   \n\n\n   dog", RegexOptions::ECMAScript, {L"cat   \n\n\n   dog"}),
				StrundData(L"", L"cat\\s+dog\\Z", L"cat   \n\n\n   dog\n", RegexOptions::None, {L"cat   \n\n\n   dog"}),
				StrundData(L"", L"cat\\s+dog\\Z", L"cat   \n\n\n   dog\n", RegexOptions::Multiline, {L"cat   \n\n\n   dog"}),
				StrundData(L"", L"cat\\s+dog\\Z", L"cat   \n\n\n   dog\n", RegexOptions::ECMAScript, {L"cat   \n\n\n   dog"}),
				StrundData(L"", L"(cat)\\s+(dog)\\Z", L"cat   \n\n\n   dog", RegexOptions::None, {L"cat   \n\n\n   dog", L"cat", L"dog"}),
				StrundData(L"", L"(cat)\\s+(dog)\\Z", L"cat   \n\n\n   dog", RegexOptions::Multiline, {L"cat   \n\n\n   dog", L"cat", L"dog"}),
				StrundData(L"", L"(cat)\\s+(dog)\\Z", L"cat   \n\n\n   dog", RegexOptions::ECMAScript, {L"cat   \n\n\n   dog", L"cat", L"dog"}),
				StrundData(L"", L"(cat)\\s+(dog)\\Z", L"cat   \n\n\n   dog\n", RegexOptions::None, {L"cat   \n\n\n   dog", L"cat", L"dog"}),
				StrundData(L"", L"(cat)\\s+(dog)\\Z", L"cat   \n\n\n   dog\n", RegexOptions::Multiline, {L"cat   \n\n\n   dog", L"cat", L"dog"}),
				StrundData(L"", L"(cat)\\s+(dog)\\Z", L"cat   \n\n\n   dog\n", RegexOptions::ECMAScript, {L"cat   \n\n\n   dog", L"cat", L"dog"}),

				//\z
				StrundData(L"", L"cat\\s+dog\\z", L"cat   \n\n\n   dog", RegexOptions::None, {L"cat   \n\n\n   dog"}),
				StrundData(L"", L"cat\\s+dog\\z", L"cat   \n\n\n   dog", RegexOptions::Multiline, {L"cat   \n\n\n   dog"}),
				StrundData(L"", L"cat\\s+dog\\z", L"cat   \n\n\n   dog", RegexOptions::ECMAScript, {L"cat   \n\n\n   dog"}),
				StrundData(L"", L"(cat)\\s+(dog)\\z", L"cat   \n\n\n   dog", RegexOptions::None, {L"cat   \n\n\n   dog", L"cat", L"dog"}),
				StrundData(L"", L"(cat)\\s+(dog)\\z", L"cat   \n\n\n   dog", RegexOptions::Multiline, {L"cat   \n\n\n   dog", L"cat", L"dog"}),
				StrundData(L"", L"(cat)\\s+(dog)\\z", L"cat   \n\n\n   dog", RegexOptions::ECMAScript, {L"cat   \n\n\n   dog", L"cat", L"dog"}),

				//\b
				StrundData(L"", L"\\bcat\\b", L"cat", RegexOptions::None, {L"cat"}),
				StrundData(L"", L"\\bcat\\b", L"dog cat mouse", RegexOptions::None, {L"cat"}),
				StrundData(L"", L"\\bcat\\b", L"cat", RegexOptions::ECMAScript, {L"cat"}),
				StrundData(L"", L"\\bcat\\b", L"dog cat mouse", RegexOptions::ECMAScript, {L"cat"}),
				StrundData(L"", L".*\\bcat\\b", L"cat", RegexOptions::None, {L"cat"}),
				StrundData(L"", L".*\\bcat\\b", L"dog cat mouse", RegexOptions::None, {L"dog cat"}),
				StrundData(L"", L".*\\bcat\\b", L"cat", RegexOptions::ECMAScript, {L"cat"}),
				StrundData(L"", L".*\\bcat\\b", L"dog cat mouse", RegexOptions::ECMAScript, {L"dog cat"}),
				StrundData(L"", L"\\b@cat", L"123START123@catEND", RegexOptions::None, {L"@cat"}),
				StrundData(L"", L"\\b\\<cat", L"123START123<catEND", RegexOptions::None, {L"<cat"}),
				StrundData(L"", L"\\b,cat", L"satwe,,,START,catEND", RegexOptions::None, {L",cat"}),
				StrundData(L"", L"\\b\\[cat", L"`12START123[catEND", RegexOptions::None, {L"[cat"}),

				//\B
				StrundData(L"", L"\\Bcat\\B", L"dogcatmouse", RegexOptions::None, {L"cat"}),
				StrundData(L"", L"dog\\Bcat\\B", L"dogcatmouse", RegexOptions::None, {L"dogcat"}),
				StrundData(L"", L".*\\Bcat\\B", L"dogcatmouse", RegexOptions::None, {L"dogcat"}),
				StrundData(L"", L"\\Bcat\\B", L"dogcatmouse", RegexOptions::ECMAScript, {L"cat"}),
				StrundData(L"", L"dog\\Bcat\\B", L"dogcatmouse", RegexOptions::ECMAScript, {L"dogcat"}),
				StrundData(L"", L".*\\Bcat\\B", L"dogcatmouse", RegexOptions::ECMAScript, {L"dogcat"}),
				StrundData(L"", L"\\B@cat", L"123START123;@catEND", RegexOptions::None, {L"@cat"}),
				StrundData(L"", L"\\B\\<cat", L"123START123'<catEND", RegexOptions::None, {L"<cat"}),
				StrundData(L"", L"\\B,cat", L"satwe,,,START',catEND", RegexOptions::None, {L",cat"}),
				StrundData(L"", L"\\B\\[cat", L"`12START123'[catEND", RegexOptions::None, {L"[cat"}),

				// \w matching \p{Lm} (Letter, Modifier)
				StrundData(L"", L"\\w+\\s+\\w+", L"cat\u02b0 dog\u02b1", RegexOptions::None, {L"cat\u02b0 dog\u02b1"}),
				StrundData(L"", L"cat\\w+\\s+dog\\w+", L"STARTcat\u30FC dog\u3005END", RegexOptions::None, {L"cat\u30FC dog\u3005END"}),
				StrundData(L"", L"cat\\w+\\s+dog\\w+", L"STARTcat\uff9e dog\uff9fEND", RegexOptions::None, {L"cat\uff9e dog\uff9fEND"}),
				StrundData(L"", L"(\\w+)\\s+(\\w+)", L"cat\u02b0 dog\u02b1", RegexOptions::None, {L"cat\u02b0 dog\u02b1", L"cat\u02b0", L"dog\u02b1"}),
				StrundData(L"", L"(cat\\w+)\\s+(dog\\w+)", L"STARTcat\u30FC dog\u3005END", RegexOptions::None, {L"cat\u30FC dog\u3005END", L"cat\u30FC", L"dog\u3005END"}),
				StrundData(L"", L"(cat\\w+)\\s+(dog\\w+)", L"STARTcat\uff9e dog\uff9fEND", RegexOptions::None, {L"cat\uff9e dog\uff9fEND", L"cat\uff9e", L"dog\uff9fEND"}),

				// Positive and negative character classes [a-c]|[^b-c]
				StrundData(L"", L"[^a]|d", L"d", RegexOptions::None, {L"d"}),
				StrundData(L"", L"([^a]|[d])*", L"Hello Worlddf", RegexOptions::None, {L"Hello Worlddf", L"f"}),
				StrundData(L"", L"([^{}]|\n)+", L"{{{{Hello\n World \n}END", RegexOptions::None, {L"Hello\n World \n", L"\n"}),
				StrundData(L"", L"([a-d]|[^abcd])+", L"\tonce\n upon" + UtilsString::StringInitSpe(0x00) + L" a- ()*&^%#time?", RegexOptions::None, {L"\tonce\n upon" + UtilsString::StringInitSpe(0x00) + L" a- ()*&^%#time?", L"?"}),
				StrundData(L"", L"([^a]|[a])*", L"once upon a time", RegexOptions::None, {L"once upon a time", L"e"}),
				StrundData(L"", L"([a-d]|[^abcd]|[x-z]|^wxyz])+", L"\tonce\n upon" + UtilsString::StringInitSpe(0x00) + L" a- ()*&^%#time?", RegexOptions::None, {L"\tonce\n upon" + UtilsString::StringInitSpe(0x00) + L" a- ()*&^%#time?", L"?"}),
				StrundData(L"", L"([a-d]|[e-i]|[^e]|wxyz])+", L"\tonce\n upon" + UtilsString::StringInitSpe(0x00) + L" a- ()*&^%#time?", RegexOptions::None, {L"\tonce\n upon" + UtilsString::StringInitSpe(0x00) + L" a- ()*&^%#time?", L"?"}),

				// Canonical and noncanonical char class, where one group is in it's
				// simplest form [a-e] and another is more complex.
				StrundData(L"", L"^(([^b]+ )|(.* ))$", L"aaa ", RegexOptions::None, {L"aaa ", L"aaa ", L"aaa ", L""}),
				StrundData(L"", L"^(([^b]+ )|(.*))$", L"aaa", RegexOptions::None, {L"aaa", L"aaa", L"", L"aaa"}),
				StrundData(L"", L"^(([^b]+ )|(.* ))$", L"bbb ", RegexOptions::None, {L"bbb ", L"bbb ", L"", L"bbb "}),
				StrundData(L"", L"^(([^b]+ )|(.*))$", L"bbb", RegexOptions::None, {L"bbb", L"bbb", L"", L"bbb"}),
				StrundData(L"", L"^((a*)|(.*))$", L"aaa", RegexOptions::None, {L"aaa", L"aaa", L"aaa", L""}),
				StrundData(L"", L"^((a*)|(.*))$", L"aaabbb", RegexOptions::None, {L"aaabbb", L"aaabbb", L"", L"aaabbb"}),

				StrundData(L"", L"(([0-9])|([a-z])|([A-Z]))*", L"{hello 1234567890 world}", RegexOptions::None, {L"", L"", L"", L"", L""}),
				StrundData(L"", L"(([0-9])|([a-z])|([A-Z]))+", L"{hello 1234567890 world}", RegexOptions::None, {L"hello", L"o", L"", L"o", L""}),
				StrundData(L"", L"(([0-9])|([a-z])|([A-Z]))*", L"{HELLO 1234567890 world}", RegexOptions::None, {L"", L"", L"", L"", L""}),
				StrundData(L"", L"(([0-9])|([a-z])|([A-Z]))+", L"{HELLO 1234567890 world}", RegexOptions::None, {L"HELLO", L"O", L"", L"", L"O"}),
				StrundData(L"", L"(([0-9])|([a-z])|([A-Z]))*", L"{1234567890 hello  world}", RegexOptions::None, {L"", L"", L"", L"", L""}),
				StrundData(L"", L"(([0-9])|([a-z])|([A-Z]))+", L"{1234567890 hello world}", RegexOptions::None, {L"1234567890", L"0", L"0", L"", L""}),

				StrundData(L"", L"^(([a-d]*)|([a-z]*))$", L"aaabbbcccdddeeefff", RegexOptions::None, {L"aaabbbcccdddeeefff", L"aaabbbcccdddeeefff", L"", L"aaabbbcccdddeeefff"}),
				StrundData(L"", L"^(([d-f]*)|([c-e]*))$", L"dddeeeccceee", RegexOptions::None, {L"dddeeeccceee", L"dddeeeccceee", L"", L"dddeeeccceee"}),
				StrundData(L"", L"^(([c-e]*)|([d-f]*))$", L"dddeeeccceee", RegexOptions::None, {L"dddeeeccceee", L"dddeeeccceee", L"dddeeeccceee", L""}),

				StrundData(L"", L"(([a-d]*)|([a-z]*))", L"aaabbbcccdddeeefff", RegexOptions::None, {L"aaabbbcccddd", L"aaabbbcccddd", L"aaabbbcccddd", L""}),
				StrundData(L"", L"(([d-f]*)|([c-e]*))", L"dddeeeccceee", RegexOptions::None, {L"dddeee", L"dddeee", L"dddeee", L""}),
				StrundData(L"", L"(([c-e]*)|([d-f]*))", L"dddeeeccceee", RegexOptions::None, {L"dddeeeccceee", L"dddeeeccceee", L"dddeeeccceee", L""}),

				StrundData(L"", L"(([a-d]*)|(.*))", L"aaabbbcccdddeeefff", RegexOptions::None, {L"aaabbbcccddd", L"aaabbbcccddd", L"aaabbbcccddd", L""}),
				StrundData(L"", L"(([d-f]*)|(.*))", L"dddeeeccceee", RegexOptions::None, {L"dddeee", L"dddeee", L"dddeee", L""}),
				StrundData(L"", L"(([c-e]*)|(.*))", L"dddeeeccceee", RegexOptions::None, {L"dddeeeccceee", L"dddeeeccceee", L"dddeeeccceee", L""}),

				// \p{Pi} (Punctuation Initial quote) \p{Pf} (Punctuation Final quote)
				StrundData(L"", L"\\p{Pi}(\\w*)\\p{Pf}", L"\u00ABCat\u00BB   \u00BBDog\u00AB'", RegexOptions::None, {L"\u00ABCat\u00BB", L"Cat"}),
				StrundData(L"", L"\\p{Pi}(\\w*)\\p{Pf}", L"\u2018Cat\u2019   \u2019Dog\u2018'", RegexOptions::None, {L"\u2018Cat\u2019", L"Cat"}),

				// ECMAScript
				StrundData(L"", L"(?<cat>cat)\\s+(?<dog>dog)\\s+\\123\\s+\\234", L"asdfcat   dog     cat23    dog34eia", RegexOptions::ECMAScript, {L"cat   dog     cat23    dog34", L"cat", L"dog"}),

				// Balanced Matching

				// Character Classes and Lazy quantifier
				StrundData(L"", L"([0-9]+?)([\\w]+?)", L"55488aheiaheiad", RegexOptions::ECMAScript, {L"55", L"5", L"5"}),
				StrundData(L"", L"([0-9]+?)([a-z]+?)", L"55488aheiaheiad", RegexOptions::ECMAScript, {L"55488a", L"55488", L"a"}),

				// Miscellaneous/Regression scenarios


				// Nested Quantifiers
				StrundData(L"", L"^[abcd]{0,0x10}*$", L"a{0,0x10}}}", RegexOptions::None, {L"a{0,0x10}}}"}),

				// Lazy operator Backtracking
				StrundData(L"", L"http://([a-zA-z0-9\\-]*\\.?)*?(:[0-9]*)??/", L"http://www.msn.com/", RegexOptions::IgnoreCase, {L"http://www.msn.com/", L"com", L""}),
				StrundData(L"", L"http://([a-zA-Z0-9\\-]*\\.?)*?/", L"http://www.google.com/", RegexOptions::IgnoreCase, {L"http://www.google.com/", L"com"}),

				StrundData(L"", L"([a-z]*?)([\\w])", L"cat", RegexOptions::IgnoreCase, {L"c", L"", L"c"}),
				StrundData(L"", L"^([a-z]*?)([\\w])$", L"cat", RegexOptions::IgnoreCase, {L"cat", L"ca", L"t"}),

				// Backtracking
				StrundData(L"", L"([a-z]*)([\\w])", L"cat", RegexOptions::IgnoreCase, {L"cat", L"ca", L"t"}),
				StrundData(L"", L"^([a-z]*)([\\w])$", L"cat", RegexOptions::IgnoreCase, {L"cat", L"ca", L"t"}),

				// Backtracking with multiple (.*) groups -- important ASP.NET scenario
				StrundData(L"", L"(.*)/(.*).aspx", L"/.aspx", RegexOptions::None, {L"/.aspx", L"", L""}),
				StrundData(L"", L"(.*)/(.*).aspx", L"/homepage.aspx", RegexOptions::None, {L"/homepage.aspx", L"", L"homepage"}),
				StrundData(L"", L"(.*)/(.*).aspx", L"pages/.aspx", RegexOptions::None, {L"pages/.aspx", L"pages", L""}),
				StrundData(L"", L"(.*)/(.*).aspx", L"pages/homepage.aspx", RegexOptions::None, {L"pages/homepage.aspx", L"pages", L"homepage"}),
				StrundData(L"", L"(.*)/(.*).aspx", L"/pages/homepage.aspx", RegexOptions::None, {L"/pages/homepage.aspx", L"/pages", L"homepage"}),
				StrundData(L"", L"(.*)/(.*).aspx", L"/pages/homepage/index.aspx", RegexOptions::None, {L"/pages/homepage/index.aspx", L"/pages/homepage", L"index"}),
				StrundData(L"", L"(.*)/(.*).aspx", L"/pages/homepage.aspx/index.aspx", RegexOptions::None, {L"/pages/homepage.aspx/index.aspx", L"/pages/homepage.aspx", L"index"}),
				StrundData(L"", L"(.*)/(.*)/(.*).aspx", L"/pages/homepage.aspx/index.aspx", RegexOptions::None, {L"/pages/homepage.aspx/index.aspx", L"/pages", L"homepage.aspx", L"index"}),

				// Backtracking with multiple (.+) groups
				StrundData(L"", L"(.+)/(.+).aspx", L"pages/homepage.aspx", RegexOptions::None, {L"pages/homepage.aspx", L"pages", L"homepage"}),
				StrundData(L"", L"(.+)/(.+).aspx", L"/pages/homepage.aspx", RegexOptions::None, {L"/pages/homepage.aspx", L"/pages", L"homepage"}),
				StrundData(L"", L"(.+)/(.+).aspx", L"/pages/homepage/index.aspx", RegexOptions::None, {L"/pages/homepage/index.aspx", L"/pages/homepage", L"index"}),
				StrundData(L"", L"(.+)/(.+).aspx", L"/pages/homepage.aspx/index.aspx", RegexOptions::None, {L"/pages/homepage.aspx/index.aspx", L"/pages/homepage.aspx", L"index"}),
				StrundData(L"", L"(.+)/(.+)/(.+).aspx", L"/pages/homepage.aspx/index.aspx", RegexOptions::None, {L"/pages/homepage.aspx/index.aspx", L"/pages", L"homepage.aspx", L"index"}),

				// Backtracking with (.+) group followed by (.*)
				StrundData(L"", L"(.+)/(.*).aspx", L"pages/.aspx", RegexOptions::None, {L"pages/.aspx", L"pages", L""}),
				StrundData(L"", L"(.+)/(.*).aspx", L"pages/homepage.aspx", RegexOptions::None, {L"pages/homepage.aspx", L"pages", L"homepage"}),
				StrundData(L"", L"(.+)/(.*).aspx", L"/pages/homepage.aspx", RegexOptions::None, {L"/pages/homepage.aspx", L"/pages", L"homepage"}),
				StrundData(L"", L"(.+)/(.*).aspx", L"/pages/homepage/index.aspx", RegexOptions::None, {L"/pages/homepage/index.aspx", L"/pages/homepage", L"index"}),
				StrundData(L"", L"(.+)/(.*).aspx", L"/pages/homepage.aspx/index.aspx", RegexOptions::None, {L"/pages/homepage.aspx/index.aspx", L"/pages/homepage.aspx", L"index"}),
				StrundData(L"", L"(.+)/(.*)/(.*).aspx", L"/pages/homepage.aspx/index.aspx", RegexOptions::None, {L"/pages/homepage.aspx/index.aspx", L"/pages", L"homepage.aspx", L"index"}),

				// Backtracking with (.*) group followed by (.+)
				StrundData(L"", L"(.*)/(.+).aspx", L"/homepage.aspx", RegexOptions::None, {L"/homepage.aspx", L"", L"homepage"}),
				StrundData(L"", L"(.*)/(.+).aspx", L"pages/homepage.aspx", RegexOptions::None, {L"pages/homepage.aspx", L"pages", L"homepage"}),
				StrundData(L"", L"(.*)/(.+).aspx", L"/pages/homepage.aspx", RegexOptions::None, {L"/pages/homepage.aspx", L"/pages", L"homepage"}),
				StrundData(L"", L"(.*)/(.+).aspx", L"/pages/homepage/index.aspx", RegexOptions::None, {L"/pages/homepage/index.aspx", L"/pages/homepage", L"index"}),
				StrundData(L"", L"(.*)/(.+).aspx", L"/pages/homepage.aspx/index.aspx", RegexOptions::None, {L"/pages/homepage.aspx/index.aspx", L"/pages/homepage.aspx", L"index"}),
				StrundData(L"", L"(.*)/(.+)/(.+).aspx", L"/pages/homepage.aspx/index.aspx", RegexOptions::None, {L"/pages/homepage.aspx/index.aspx", L"/pages", L"homepage.aspx", L"index"}),

				// Captures inside varying constructs with backtracking needing to uncapture
				StrundData(L"", L"a(bc)d|abc(e)", L"abce", RegexOptions::None, {L"abce", L"", L"e"}), // alternation
				StrundData(L"", L"((ab){2}cd)*", L"ababcdababcdababc", RegexOptions::None, {L"ababcdababcd", L"ababcd", L"ab"}), // loop
				StrundData(L"", L"(ab(?=(\\w)\\w))*a", L"aba", RegexOptions::None, {L"a", L"", L""}), // positive lookahead in a loop
				StrundData(L"", L"(ab(?=(\\w)\\w))*a", L"ababa", RegexOptions::None, {L"aba", L"ab", L"a"}), // positive lookahead in a loop
				StrundData(L"", L"(ab(?=(\\w)\\w))*a", L"abababa", RegexOptions::None, {L"ababa", L"ab", L"a"}), // positive lookahead in a loop
				StrundData(L"", L"\\w\\w(?!(\\d)\\d)", L"aa..", RegexOptions::None, {L"aa", L""}), // negative lookahead
				StrundData(L"", L"\\w\\w(?!(\\d)\\d)", L"aa.3", RegexOptions::None, {L"aa", L""}), // negative lookahead

				// Quantifiers
				StrundData(L"", L"a*", L"", RegexOptions::None, {L""}),
				StrundData(L"", L"a*", L"a", RegexOptions::None, {L"a"}),
				StrundData(L"", L"a*", L"aa", RegexOptions::None, {L"aa"}),
				StrundData(L"", L"a*", L"aaa", RegexOptions::None, {L"aaa"}),
				StrundData(L"", L"a*?", L"", RegexOptions::None, {L""}),
				StrundData(L"", L"a*?", L"a", RegexOptions::None, {L""}),
				StrundData(L"", L"a*?", L"aa", RegexOptions::None, {L""}),
				StrundData(L"", L"a+?", L"aa", RegexOptions::None, {L"a"}),
				StrundData(L"", L"a{1,", L"a{1,", RegexOptions::None, {L"a{1,"}),
				StrundData(L"", L"a{1,3}", L"aaaaa", RegexOptions::None, {L"aaa"}),
				StrundData(L"", L"a{1,3}?", L"aaaaa", RegexOptions::None, {L"a"}),
				StrundData(L"", L"a{2,2}", L"aaaaa", RegexOptions::None, {L"aa"}),
				StrundData(L"", L"a{2,2}?", L"aaaaa", RegexOptions::None, {L"aa"}),
				StrundData(L"", L".{1,3}", L"bb\nba", RegexOptions::None, {L"bb"}),
				StrundData(L"", L".{1,3}?", L"bb\nba", RegexOptions::None, {L"b"}),
				StrundData(L"", L".{2,2}", L"bbb\nba", RegexOptions::None, {L"bb"}),
				StrundData(L"", L".{2,2}?", L"bbb\nba", RegexOptions::None, {L"bb"}),
				StrundData(L"", L"[abc]{1,3}", L"ccaba", RegexOptions::None, {L"cca"}),
				StrundData(L"", L"[abc]{1,3}?", L"ccaba", RegexOptions::None, {L"c"}),
				StrundData(L"", L"[abc]{2,2}", L"ccaba", RegexOptions::None, {L"cc"}),
				StrundData(L"", L"[abc]{2,2}?", L"ccaba", RegexOptions::None, {L"cc"}),
				StrundData(L"", L"(?:[abc]def){1,3}xyz", L"cdefxyz", RegexOptions::None, {L"cdefxyz"}),
				StrundData(L"", L"(?:[abc]def){1,3}xyz", L"adefbdefcdefxyz", RegexOptions::None, {L"adefbdefcdefxyz"}),
				StrundData(L"", L"(?:[abc]def){1,3}?xyz", L"cdefxyz", RegexOptions::None, {L"cdefxyz"}),
				StrundData(L"", L"(?:[abc]def){1,3}?xyz", L"adefbdefcdefxyz", RegexOptions::None, {L"adefbdefcdefxyz"}),
				StrundData(L"", L"(?:[abc]def){2,2}xyz", L"adefbdefcdefxyz", RegexOptions::None, {L"bdefcdefxyz"}),
				StrundData(L"", L"(?:[abc]def){2,2}?xyz", L"adefbdefcdefxyz", RegexOptions::None, {L"bdefcdefxyz"}),
				StrundData(L"", L"(?:[abc]def){1,3}", L"cdef", RegexOptions::None, {L"cdef"}),
				StrundData(L"", L"(?:[abc]def){1,3}", L"cdefadefbdef", RegexOptions::None, {L"cdefadefbdef"}),
				StrundData(L"", L"(?:[abc]def){1,3}", L"cdefadefbdefadef", RegexOptions::None, {L"cdefadefbdef"}),
				StrundData(L"", L"(?:[abc]def){1,3}?", L"cdef", RegexOptions::None, {L"cdef"}),
				StrundData(L"", L"(?:[abc]def){1,3}?", L"cdefadefbdef", RegexOptions::None, {L"cdef"}),
				StrundData(L"", L"(?:[abc]def){2,2}", L"cdefadefbdefadef", RegexOptions::None, {L"cdefadef"}),
				StrundData(L"", L"(?:[abc]def){2,2}?", L"cdefadefbdefadef", RegexOptions::None, {L"cdefadef"}),
				StrundData(L"", L"xyz(?:[abc]def){1,3}", L"xyzcdef", RegexOptions::None, {L"xyzcdef"}),
				StrundData(L"", L"xyz(?:[abc]def){1,3}", L"xyzcdefadefbdef", RegexOptions::None, {L"xyzcdefadefbdef"}),
				StrundData(L"", L"xyz(?:[abc]def){1,3}", L"xyzcdefadefbdefadef", RegexOptions::None, {L"xyzcdefadefbdef"}),
				StrundData(L"", L"xyz(?:[abc]def){1,3}?", L"xyzcdef", RegexOptions::None, {L"xyzcdef"}),
				StrundData(L"", L"xyz(?:[abc]def){1,3}?", L"xyzcdefadefbdef", RegexOptions::None, {L"xyzcdef"}),
				StrundData(L"", L"xyz(?:[abc]def){2,2}", L"xyzcdefadefbdefadef", RegexOptions::None, {L"xyzcdefadef"}),
				StrundData(L"", L"xyz(?:[abc]def){2,2}?", L"xyzcdefadefbdefadef", RegexOptions::None, {L"xyzcdefadef"}),
				StrundData(L"", L"(cat){", L"cat{", RegexOptions::None, {L"cat{", L"cat"}),
				StrundData(L"", L"(cat){}", L"cat{}", RegexOptions::None, {L"cat{}", L"cat"}),
				StrundData(L"", L"(cat){,", L"cat{,", RegexOptions::None, {L"cat{,", L"cat"}),
				StrundData(L"", L"(cat){,}", L"cat{,}", RegexOptions::None, {L"cat{,}", L"cat"}),
				StrundData(L"", L"(cat){cat}", L"cat{cat}", RegexOptions::None, {L"cat{cat}", L"cat"}),
				StrundData(L"", L"(cat){cat,5}", L"cat{cat,5}", RegexOptions::None, {L"cat{cat,5}", L"cat"}),
				StrundData(L"", L"(cat){5,dog}", L"cat{5,dog}", RegexOptions::None, {L"cat{5,dog}", L"cat"}),
				StrundData(L"", L"(cat){cat,dog}", L"cat{cat,dog}", RegexOptions::None, {L"cat{cat,dog}", L"cat"}),
				StrundData(L"", L"(cat){,}?", L"cat{,}?", RegexOptions::None, {L"cat{,}", L"cat"}),
				StrundData(L"", L"(cat){cat}?", L"cat{cat}?", RegexOptions::None, {L"cat{cat}", L"cat"}),
				StrundData(L"", L"(cat){cat,5}?", L"cat{cat,5}?", RegexOptions::None, {L"cat{cat,5}", L"cat"}),
				StrundData(L"", L"(cat){5,dog}?", L"cat{5,dog}?", RegexOptions::None, {L"cat{5,dog}", L"cat"}),
				StrundData(L"", L"(cat){cat,dog}?", L"cat{cat,dog}?", RegexOptions::None, {L"cat{cat,dog}", L"cat"}),

				// Atomic subexpressions
				// Implicitly upgrading (or not) oneloop to be atomic
				StrundData(L"", L"a*b", L"aaab", RegexOptions::None, {L"aaab"}),
				StrundData(L"", L"a*b+", L"aaab", RegexOptions::None, {L"aaab"}),
				StrundData(L"", L"a*b+?", L"aaab", RegexOptions::None, {L"aaab"}),
				StrundData(L"", L"a*(?>b+)", L"aaab", RegexOptions::None, {L"aaab"}),
				StrundData(L"", L"a*[^a]", L"aaab", RegexOptions::None, {L"aaab"}),
				StrundData(L"", L"a*[^a]+", L"aaab", RegexOptions::None, {L"aaab"}),
				StrundData(L"", L"a*[^a]+?", L"aaab", RegexOptions::None, {L"aaab"}),
				StrundData(L"", L"a*(?>[^a]+)", L"aaab", RegexOptions::None, {L"aaab"}),
				StrundData(L"", L"a*bcd", L"aaabcd", RegexOptions::None, {L"aaabcd"}),
				StrundData(L"", L"a*[bcd]", L"aaab", RegexOptions::None, {L"aaab"}),
				StrundData(L"", L"a*[bcd]+", L"aaab", RegexOptions::None, {L"aaab"}),
				StrundData(L"", L"a*[bcd]+?", L"aaab", RegexOptions::None, {L"aaab"}),
				StrundData(L"", L"a*(?>[bcd]+)", L"aaab", RegexOptions::None, {L"aaab"}),
				StrundData(L"", L"a*[bcd]{1,3}", L"aaab", RegexOptions::None, {L"aaab"}),
				StrundData(L"", L"a*([bcd]ab|[bef]cd){1,3}", L"aaababecdcac", RegexOptions::ExplicitCapture, {L"aaababecd"}),
				StrundData(L"", L"a*([bcd]|[aef]){1,3}", L"befb", RegexOptions::ExplicitCapture, {L"bef"}), // can't upgrade
				StrundData(L"", L"a*$", L"aaa", RegexOptions::None, {L"aaa"}),
				StrundData(L"", L"a*$", L"aaa", RegexOptions::Multiline, {L"aaa"}),
				StrundData(L"", L"a*\\b", L"aaa bbb", RegexOptions::None, {L"aaa"}),
				StrundData(L"", L"a*\\b", L"aaa bbb", RegexOptions::ECMAScript, {L"aaa"}),
				StrundData(L"", L"@*\\B", L"@@@", RegexOptions::None, {L"@@@"}),
				StrundData(L"", L"@*\\B", L"@@@", RegexOptions::ECMAScript, {L"@@@"}),
				StrundData(L"", L"(?:abcd*|efgh)i", L"efghi", RegexOptions::None, {L"efghi"}),
				StrundData(L"", L"(?:abcd|efgh*)i", L"efgi", RegexOptions::None, {L"efgi"}),
				StrundData(L"", L"(?:abcd|efghj{2,}|j[klm]o+)i", L"efghjjjjji", RegexOptions::None, {L"efghjjjjji"}),
				StrundData(L"", L"(?:abcd|efghi{2,}|j[klm]o+)i", L"efghiii", RegexOptions::None, {L"efghiii"}),
				StrundData(L"", L"(?:abcd|efghi{2,}|j[klm]o+)i", L"efghiiiiiiii", RegexOptions::None, {L"efghiiiiiiii"}),
				StrundData(L"", L"a?ba?ba?ba?b", L"abbabab", RegexOptions::None, {L"abbabab"}),
				StrundData(L"", L"a?ba?ba?ba?b", L"abBAbab", RegexOptions::IgnoreCase, {L"abBAbab"}),
				// Implicitly upgrading (or not) notoneloop to be atomic
				StrundData(L"", L"[^b]*b", L"aaab", RegexOptions::None, {L"aaab"}),
				StrundData(L"", L"[^b]*b+", L"aaab", RegexOptions::None, {L"aaab"}),
				StrundData(L"", L"[^b]*b+?", L"aaab", RegexOptions::None, {L"aaab"}),
				StrundData(L"", L"[^b]*(?>b+)", L"aaab", RegexOptions::None, {L"aaab"}),
				StrundData(L"", L"[^b]*bac", L"aaabac", RegexOptions::None, {L"aaabac"}),
				StrundData(L"", L"[^b]*", L"aaa", RegexOptions::None, {L"aaa"}),
				StrundData(L"", L"(?:abc[^b]*|efgh)i", L"efghi", RegexOptions::None, {L"efghi"}), // can't upgrade
				StrundData(L"", L"(?:abcd|efg[^b]*)b", L"efgb", RegexOptions::None, {L"efgb"}),
				StrundData(L"", L"(?:abcd|efg[^b]*)i", L"efgi", RegexOptions::None, {L"efgi"}), // can't upgrade
				StrundData(L"", L"[^a]?a[^a]?a[^a]?a[^a]?a", L"baababa", RegexOptions::None, {L"baababa"}),
				StrundData(L"", L"[^a]?a[^a]?a[^a]?a[^a]?a", L"BAababa", RegexOptions::IgnoreCase, {L"BAababa"}),
				// Implicitly upgrading (or not) setloop to be atomic
				StrundData(L"", L"[ac]*", L"aaa", RegexOptions::None, {L"aaa"}),
				StrundData(L"", L"[ac]*b", L"aaab", RegexOptions::None, {L"aaab"}),
				StrundData(L"", L"[ac]*b+", L"aaab", RegexOptions::None, {L"aaab"}),
				StrundData(L"", L"[ac]*b+?", L"aaab", RegexOptions::None, {L"aaab"}),
				StrundData(L"", L"[ac]*(?>b+)", L"aaab", RegexOptions::None, {L"aaab"}),
				StrundData(L"", L"[ac]*[^a]", L"aaab", RegexOptions::None, {L"aaab"}),
				StrundData(L"", L"[ac]*[^a]+", L"aaab", RegexOptions::None, {L"aaab"}),
				StrundData(L"", L"[ac]*[^a]+?", L"aaab", RegexOptions::None, {L"aaab"}),
				StrundData(L"", L"[ac]*(?>[^a]+)", L"aaab", RegexOptions::None, {L"aaab"}),
				StrundData(L"", L"[ac]*bcd", L"aaabcd", RegexOptions::None, {L"aaabcd"}),
				StrundData(L"", L"[ac]*[bd]", L"aaab", RegexOptions::None, {L"aaab"}),
				StrundData(L"", L"[ac]*[bd]+", L"aaab", RegexOptions::None, {L"aaab"}),
				StrundData(L"", L"[ac]*[bd]+?", L"aaab", RegexOptions::None, {L"aaab"}),
				StrundData(L"", L"[ac]*(?>[bd]+)", L"aaab", RegexOptions::None, {L"aaab"}),
				StrundData(L"", L"[ac]*[bd]{1,3}", L"aaab", RegexOptions::None, {L"aaab"}),
				StrundData(L"", L"[ac]*$", L"aaa", RegexOptions::None, {L"aaa"}),
				StrundData(L"", L"[ac]*$", L"aaa", RegexOptions::Multiline, {L"aaa"}),
				StrundData(L"", L"[ac]*\\b", L"aaa bbb", RegexOptions::None, {L"aaa"}),
				StrundData(L"", L"[ac]*\\b", L"aaa bbb", RegexOptions::ECMAScript, {L"aaa"}),
				StrundData(L"", L"[@']*\\B", L"@@@", RegexOptions::None, {L"@@@"}),
				StrundData(L"", L"[@']*\\B", L"@@@", RegexOptions::ECMAScript, {L"@@@"}),
				StrundData(L"", L".*.", L"@@@", RegexOptions::Singleline, {L"@@@"}),
				StrundData(L"", L"(?:abcd|efg[hij]*)h", L"efgh", RegexOptions::None, {L"efgh"}), // can't upgrade
				StrundData(L"", L"(?:abcd|efg[hij]*)ih", L"efgjih", RegexOptions::None, {L"efgjih"}), // can't upgrade
				StrundData(L"", L"(?:abcd|efg[hij]*)k", L"efgjk", RegexOptions::None, {L"efgjk"}),
				StrundData(L"", L"[ace]?b[ace]?b[ace]?b[ace]?b", L"cbbabeb", RegexOptions::None, {L"cbbabeb"}),
				StrundData(L"", L"[ace]?b[ace]?b[ace]?b[ace]?b", L"cBbAbEb", RegexOptions::IgnoreCase, {L"cBbAbEb"}),
				StrundData(L"", L"a[^wz]*w", L"abcdcdcdwz", RegexOptions::None, {L"abcdcdcdw"}),
				StrundData(L"", L"a[^wyz]*w", L"abcdcdcdwz", RegexOptions::None, {L"abcdcdcdw"}),
				StrundData(L"", L"a[^wyz]*W", L"abcdcdcdWz", RegexOptions::IgnoreCase, {L"abcdcdcdW"}),
				// Implicitly upgrading (or not) concat loops to be atomic
				StrundData(L"", L"(?:[ab]c[de]f)*", L"", RegexOptions::None, {L""}),
				StrundData(L"", L"(?:[ab]c[de]f)*", L"acdf", RegexOptions::None, {L"acdf"}),
				StrundData(L"", L"(?:[ab]c[de]f)*", L"acdfbcef", RegexOptions::None, {L"acdfbcef"}),
				StrundData(L"", L"(?:[ab]c[de]f)*", L"cdfbcef", RegexOptions::None, {L""}),
				StrundData(L"", L"(?:[ab]c[de]f)+", L"cdfbcef", RegexOptions::None, {L"bcef"}),
				StrundData(L"", L"(?:[ab]c[de]f)*", L"bcefbcdfacfe", RegexOptions::None, {L"bcefbcdf"}),
				// Implicitly upgrading (or not) nested loops to be atomic
				StrundData(L"", L"(?:a){3}", L"aaaaaaaaa", RegexOptions::None, {L"aaa"}),
				StrundData(L"", L"(?:a){3}?", L"aaaaaaaaa", RegexOptions::None, {L"aaa"}),
				StrundData(L"", L"(?:a{2}){3}", L"aaaaaaaaa", RegexOptions::None, {L"aaaaaa"}),
				StrundData(L"", L"(?:a{2}?){3}?", L"aaaaaaaaa", RegexOptions::None, {L"aaaaaa"}),
				StrundData(L"", L"(?:(?:[ab]c[de]f){3}){2}", L"acdfbcdfacefbcefbcefbcdfacdef", RegexOptions::None, {L"acdfbcdfacefbcefbcefbcdf"}),
				StrundData(L"", L"(?:(?:[ab]c[de]f){3}hello){2}", L"aaaaaacdfbcdfacefhellobcefbcefbcdfhellooooo", RegexOptions::None, {L"acdfbcdfacefhellobcefbcefbcdfhello"}),
				StrundData(L"", L"CN=(.*[^,]+).*", L"CN=localhost", RegexOptions::Singleline, {L"CN=localhost", L"localhost"}),
				// Nested atomic
				StrundData(L"", L"(?>abc[def]gh(i*))", L"123abceghiii456", RegexOptions::None, {L"abceghiii", L"iii"}),
				StrundData(L"", L"(?>(?:abc)*)", L"abcabcabc", RegexOptions::None, {L"abcabcabc"}),

				// Anchoring loops beginning with .* / .+
				StrundData(L"", L".*", L"", RegexOptions::None, {L""}),
				StrundData(L"", L".*", L"\n\n\n\n", RegexOptions::None, {L""}),
				StrundData(L"", L".*", L"\n\n\n\n", RegexOptions::Singleline, {L"\n\n\n\n"}),
				StrundData(L"", L".*[1a]", L"\n\n\n\n1", RegexOptions::None, {L"1"}),
				StrundData(L"", L"(?s).*(?-s)[1a]", L"1\n\n\n\n", RegexOptions::None, {L"1"}),
				StrundData(L"", L"(?s).*(?-s)[1a]", L"\n\n\n\n1", RegexOptions::None, {L"\n\n\n\n1"}),
				StrundData(L"", L".*|.*|.*", L"", RegexOptions::None, {L""}),
				StrundData(L"", L".*123|abc", L"abc\n123", RegexOptions::None, {L"abc"}),
				StrundData(L"", L".*123|abc", L"abc\n123", RegexOptions::Singleline, {L"abc\n123"}),
				StrundData(L"", L"abc|.*123", L"abc\n123", RegexOptions::Singleline, {L"abc"}),
				StrundData(L"", L".*", L"\n", RegexOptions::None, {L""}),
				StrundData(L"", L".*\\n", L"\n", RegexOptions::None, {L"\n"}),
				StrundData(L"", L".*", L"\n", RegexOptions::Singleline, {L"\n"}),
				StrundData(L"", L".*\\n", L"\n", RegexOptions::Singleline, {L"\n"}),
				StrundData(L"", L".*", L"abc", RegexOptions::None, {L"abc"}),
				StrundData(L"", L".*abc", L"abc", RegexOptions::None, {L"abc"}),
				StrundData(L"", L".*abc|ghi", L"ghi", RegexOptions::None, {L"ghi"}),
				StrundData(L"", L".*abc|.*ghi", L"abcghi", RegexOptions::None, {L"abc"}),
				StrundData(L"", L".*ghi|.*abc", L"abcghi", RegexOptions::None, {L"abcghi"}),
				StrundData(L"", L".*abc|.*ghi", L"bcghi", RegexOptions::None, {L"bcghi"}),
				StrundData(L"", L".*abc|.+c", L" \n   \n   bc", RegexOptions::None, {L"   bc"}),
				StrundData(L"", L".*abc", L"12345 abc", RegexOptions::None, {L"12345 abc"}),
				StrundData(L"", L".*abc", L"12345\n abc", RegexOptions::None, {L" abc"}),
				StrundData(L"", L".*abc", L"12345\n abc", RegexOptions::Singleline, {L"12345\n abc"}),
				StrundData(L"", L"(.*)abc\\1", L"\n12345abc12345", RegexOptions::Singleline, {L"12345abc12345", L"12345"}),
				StrundData(L"", L".*\\nabc", L"\n123\nabc", RegexOptions::None, {L"123\nabc"}),
				StrundData(L"", L".*\\nabc", L"\n123\nabc", RegexOptions::Singleline, {L"\n123\nabc"}),
				StrundData(L"", L".*abc", L"abc abc abc \nabc", RegexOptions::None, {L"abc abc abc"}),
				StrundData(L"", L".*abc", L"abc abc abc \nabc", RegexOptions::Singleline, {L"abc abc abc \nabc"}),
				StrundData(L"", L".*?abc", L"abc abc abc \nabc", RegexOptions::None, {L"abc"}),
				StrundData(L"", L"[^\\n]*abc", L"123abc\n456abc\n789abc", RegexOptions::None, {L"123abc"}),
				StrundData(L"", L"[^\\n]*abc", L"123abc\n456abc\n789abc", RegexOptions::Singleline, {L"123abc"}),
				StrundData(L"", L"[^\\n]*abc", L"123ab\n456abc\n789abc", RegexOptions::None, {L"456abc"}),
				StrundData(L"", L"[^\\n]*abc", L"123ab\n456abc\n789abc", RegexOptions::Singleline, {L"456abc"}),
				StrundData(L"", L".+", L"a", RegexOptions::None, {L"a"}),
				StrundData(L"", L".+", L"\nabc", RegexOptions::None, {L"abc"}),
				StrundData(L"", L".+", L"\n", RegexOptions::Singleline, {L"\n"}),
				StrundData(L"", L".+", L"\nabc", RegexOptions::Singleline, {L"\nabc"}),
				StrundData(L"", L".+abc", L"aaaabc", RegexOptions::None, {L"aaaabc"}),
				StrundData(L"", L".+abc", L"12345 abc", RegexOptions::None, {L"12345 abc"}),
				StrundData(L"", L".+abc", L"12345\n abc", RegexOptions::None, {L" abc"}),
				StrundData(L"", L".+abc", L"12345\n abc", RegexOptions::Singleline, {L"12345\n abc"}),
				StrundData(L"", L"(.+)abc\\1", L"\n12345abc12345", RegexOptions::Singleline, {L"12345abc12345", L"12345"}),

				// Unanchored .*
				StrundData(L"", L"\\A\\s*(?<name>\\w+)(\\s*\\((?<arguments>.*)\\))?\\s*\\Z", L"Match(Name)", RegexOptions::None, {L"Match(Name)", L"(Name)", L"Match", L"Name"}),
				StrundData(L"", L"\\A\\s*(?<name>\\w+)(\\s*\\((?<arguments>.*)\\))?\\s*\\Z", L"Match(Na\nme)", RegexOptions::Singleline, {L"Match(Na\nme)", L"(Na\nme)", L"Match", L"Na\nme"}),
				StrundData(L"", L"abcd.*", L"abcabcd", RegexOptions::None, {L"abcd"}),
				StrundData(L"", L"abcd.*", L"abcabcde", RegexOptions::None, {L"abcde"}),
				StrundData(L"", L"abcd.*", L"abcabcdefg", RegexOptions::None, {L"abcdefg"}),
				StrundData(L"", L"abcd(.*)", L"ababcd", RegexOptions::None, {L"abcd", L""}),
				StrundData(L"", L"abcd(.*)", L"aabcde", RegexOptions::None, {L"abcde", L"e"}),
				StrundData(L"", L"abcd(.*)", L"abcabcdefg", RegexOptions::None, {L"abcdefg", L"efg"}),
				StrundData(L"", L"abcd(.*)e", L"abcabcdefg", RegexOptions::None, {L"abcde", L""}),
				StrundData(L"", L"abcd(.*)f", L"abcabcdefg", RegexOptions::None, {L"abcdef", L"e"}),
				StrundData(L"", L"abcd.*", L"abcabcd", RegexOptions::Singleline, {L"abcd"}),
				StrundData(L"", L"abcd.*", L"abcabcde", RegexOptions::Singleline, {L"abcde"}),
				StrundData(L"", L"abcd.*", L"abcabcdefg", RegexOptions::Singleline, {L"abcdefg"}),
				StrundData(L"", L"abcd(.*)", L"ababcd", RegexOptions::Singleline, {L"abcd", L""}),
				StrundData(L"", L"abcd(.*)", L"aabcde", RegexOptions::Singleline, {L"abcde", L"e"}),
				StrundData(L"", L"abcd(.*)", L"abcabcdefg", RegexOptions::Singleline, {L"abcdefg", L"efg"}),
				StrundData(L"", L"abcd(.*)e", L"abcabcdefg", RegexOptions::Singleline, {L"abcde", L""}),
				StrundData(L"", L"abcd(.*)f", L"abcabcdefg", RegexOptions::Singleline, {L"abcdef", L"e"}),


				// Grouping Constructs
				StrundData(L"", L"()", L"cat", RegexOptions::None, {L"", L""}),
				StrundData(L"", L"(?<cat>)", L"cat", RegexOptions::None, {L"", L""}),
				StrundData(L"", L"(?'cat')", L"cat", RegexOptions::None, {L"", L""}),
				StrundData(L"", L"(?:)", L"cat", RegexOptions::None, {L""}),
				StrundData(L"", L"(?imn)", L"cat", RegexOptions::None, {L""}),
				StrundData(L"", L"(?imn)cat", L"(?imn)cat", RegexOptions::None, {L"cat"}),
				StrundData(L"", L"(?=)", L"cat", RegexOptions::None, {L""}),
				StrundData(L"", L"(?<=)", L"cat", RegexOptions::None, {L""}),
				StrundData(L"", L"(?>)", L"cat", RegexOptions::None, {L""}),

				// Alternation construct
				StrundData(L"", L"(?()|)", L"(?()|)", RegexOptions::None, {L""}),

				StrundData(L"", L"(?(cat)|)", L"cat", RegexOptions::None, {L""}),
				StrundData(L"", L"(?(cat)|)", L"dog", RegexOptions::None, {L""}),

				StrundData(L"", L"(?(cat)catdog|)", L"catdog", RegexOptions::None, {L"catdog"}),
				StrundData(L"", L"(?(cat)cat\\w\\w\\w)*", L"catdogcathog", RegexOptions::None, {L"catdogcathog"}),
				StrundData(L"", L"(?(?=cat)cat\\w\\w\\w)*", L"catdogcathog", RegexOptions::None, {L"catdogcathog"}),
				StrundData(L"", L"(?(cat)catdog|)", L"dog", RegexOptions::None, {L""}),
				StrundData(L"", L"(?(cat)dog|)", L"dog", RegexOptions::None, {L""}),
				StrundData(L"", L"(?(cat)dog|)", L"cat", RegexOptions::None, {L""}),

				StrundData(L"", L"(?(cat)|catdog)", L"cat", RegexOptions::None, {L""}),
				StrundData(L"", L"(?(cat)|catdog)", L"catdog", RegexOptions::None, {L""}),
				StrundData(L"", L"(?(cat)|dog)", L"dog", RegexOptions::None, {L"dog"}),

				StrundData(L"", L"(?((\\w{3}))\\1\\1|no)", L"dogdogdog", RegexOptions::None, {L"dogdog", L"dog"}),
				StrundData(L"", L"(?((\\w{3}))\\1\\1|no)", L"no", RegexOptions::None, {L"no", L""}),

				// Invalid unicode
				StrundData(L"", L"([" + UtilsString::StringInitSpe(0x00) + L"-" + UtilsString::StringInitSpe(0xffff) + L"-[azAZ09]]|[" + UtilsString::StringInitSpe(0x00) + L"-" + UtilsString::StringInitSpe(0xffff) + L"-[^azAZ09]])+", L"azAZBCDE1234567890BCDEFAZza", RegexOptions::None, {L"azAZBCDE1234567890BCDEFAZza", L"a"}),
				StrundData(L"", L"[" + UtilsString::StringInitSpe(0x00) + L"-" + UtilsString::StringInitSpe(0xffff) + L"-[" + UtilsString::StringInitSpe(0x00) + L"-" + UtilsString::StringInitSpe(0xffff) + L"-[" + UtilsString::StringInitSpe(0x00) + L"-" + UtilsString::StringInitSpe(0xffff) + L"-[" + UtilsString::StringInitSpe(0x00) + L"-" + UtilsString::StringInitSpe(0xffff) + L"-[" + UtilsString::StringInitSpe(0x00) + L"-" + UtilsString::StringInitSpe(0xffff) + L"-[a]]]]]]+", L"abcxyzABCXYZ123890", RegexOptions::None, {L"bcxyzABCXYZ123890"}),
				StrundData(L"", L"[" + UtilsString::StringInitSpe(0x00) + L"-" + UtilsString::StringInitSpe(0xffff) + L"-[" + UtilsString::StringInitSpe(0x00) + L"-" + UtilsString::StringInitSpe(0xffff) + L"-[" + UtilsString::StringInitSpe(0x00) + L"-" + UtilsString::StringInitSpe(0xffff) + L"-[" + UtilsString::StringInitSpe(0x00) + L"-" + UtilsString::StringInitSpe(0xffff) + L"-[" + UtilsString::StringInitSpe(0x00) + L"-" + UtilsString::StringInitSpe(0xffff) + L"-[" + UtilsString::StringInitSpe(0x00) + L"-" + UtilsString::StringInitSpe(0xffff) + L"-[a]]]]]]]+", L"bcxyzABCXYZ123890a", RegexOptions::None, {L"a"}),
				StrundData(L"", L"[" + UtilsString::StringInitSpe(0x00) + L"-" + UtilsString::StringInitSpe(0xffff) + L"-[\\p{P}\\p{S}\\p{C}]]+", L"!@`';.,$+<>=azAZ09", RegexOptions::None, {L"azAZ09"}),

				StrundData(L"", L"[\\uFFFD-\\uFFFF]+", L"\uFFFC\uFFFD\uFFFE\uFFFF", RegexOptions::IgnoreCase, {L"\uFFFD\uFFFE\uFFFF"}),
				StrundData(L"", L"[￼-￾]+", L"￻￼�￾￿", RegexOptions::IgnoreCase, {L"\uFFFC\uFFFD\uFFFE"}),

				// Empty Match
				StrundData(L"", L"([a*]*)+?$", L"ab", RegexOptions::None, {L"", L""}),
				StrundData(L"", L"(a*)+?$", L"b", RegexOptions::None, {L"", L""}),
			};
		}

		static std::vector<StrundData> Groups_CustomCulture_TestData_enUS()
		{
			return std::vector<StrundData>{
				StrundData(L"en-US", L"CH", L"Ch", RegexOptions::IgnoreCase, {L"Ch"}),
				StrundData(L"en-US", L"cH", L"Ch", RegexOptions::IgnoreCase, {L"Ch"}),
				StrundData(L"en-US", L"AA", L"Aa", RegexOptions::IgnoreCase, {L"Aa"}),
				StrundData(L"en-US", L"aA", L"Aa", RegexOptions::IgnoreCase, {L"Aa"}),
				StrundData(L"en-US", L"\u0130", L"\u0049", RegexOptions::IgnoreCase, {L"\u0049"}),
				StrundData(L"en-US", L"\u0130", L"\u0069", RegexOptions::IgnoreCase, {L"\u0069"}),
			};
		}

		static std::vector<StrundData> Groups_CustomCulture_TestData_Czech()
		{
			return std::vector<StrundData>{
				StrundData(L"cs-CZ", L"CH", L"Ch", RegexOptions::IgnoreCase, {L"Ch"}),
				StrundData(L"cs-CZ", L"cH", L"Ch", RegexOptions::IgnoreCase, {L"Ch"}),
			};
		}

		static std::vector<StrundData> Groups_CustomCulture_TestData_Danish()
		{
			return std::vector<StrundData>{
				StrundData(L"da-DK", L"AA", L"Aa", RegexOptions::IgnoreCase, {L"Aa"}),
				StrundData(L"da-DK", L"aA", L"Aa", RegexOptions::IgnoreCase, {L"Aa"}),
			};
		}

		static std::vector<StrundData> Groups_CustomCulture_TestData_Turkish()
		{
			return std::vector<StrundData>{
				StrundData(L"tr-TR", L"\u0131", L"\u0049", RegexOptions::IgnoreCase, {L"\u0049"}),
				StrundData(L"tr-TR", L"\u0130", L"\u0069", RegexOptions::IgnoreCase, {L"\u0069"}),
			};
		}

		static std::vector<StrundData> Groups_CustomCulture_TestData_AzeriLatin()
		{
			return std::vector<StrundData>{
				StrundData(L"az-Latn-AZ", L"\u0131", L"\u0049", RegexOptions::IgnoreCase, {L"\u0049"}),
				StrundData(L"az-Latn-AZ", L"\u0130", L"\u0069", RegexOptions::IgnoreCase, {L"\u0069"}),
			};
		}

		static void Groups(std::wstring cultureName, std::wstring pattern, std::wstring input, RegexOptions options, std::vector<std::wstring> expectedGroups)
		{
			ComCultureInfo* culture = ComCultureInfo::CreateSpecificCulture(cultureName);
			if (culture->CultureName != cultureName || cultureName.empty())
				culture = ComCultureInfo::CreateSpecificCulture(L"en-US");

			ComCultureInfo::CurrentCulture();
			GetCultureInfo::s_userDefaultCulture = culture;

			Regex regex(pattern, options);

			std::vector<std::wstring> temp_loop{L"", L"IGNORETHIS"};
			for (size_t index = 0; index < temp_loop.size(); ++index)
			{
				std::wstring prefix = temp_loop.at(index);
				std::wstring temp = prefix + input;

				Match* match = prefix.length() == 0 ? regex.Match(&input) : regex.Match(&temp, prefix.length(), input.length());


				bool succes = match->Success;
				Assert::True(succes);
				auto get_value = match->getValue();
				Assert::True(expectedGroups[0] == get_value);

				Assert::True(expectedGroups.size() == match->Groups->Count);

				std::vector<int> groupNumbers = regex.GetGroupNumbers();
				std::vector<std::wstring> groupNames = regex.GetGroupNames();
				for (size_t i = 0; i < expectedGroups.size(); i++)
				{
					auto text_1 = expectedGroups[i];
					auto text_2 = match->Groups->operator[](groupNumbers[i])->getValue();

					Assert::IsTrue(text_1 == text_2);
					Assert::IsTrue(match->Groups->operator[](groupNumbers[i]) == match->Groups->operator[](groupNames[i]));

					Assert::IsTrue(groupNumbers[i] == regex.GroupNumberFromName(groupNames[i]));
					Assert::IsTrue(groupNames[i] == regex.GroupNameFromNumber(groupNumbers[i]));
				}
			}
		}

		static void Synchronized_NullGroup_Throws()
		{
			AssertThrows(ArgumentNullException, Group::Synchronized(NULL))
		}

		static void Synchronized_ValidGroup_Success(std::wstring pattern, std::wstring input)
		{
			Match* match = Regex::Match(&input, pattern);

			Group* synchronizedGroup = Group::Synchronized(match->Groups->operator[](0));
			Assert::IsNotNull(synchronizedGroup);
		}
	};
}
