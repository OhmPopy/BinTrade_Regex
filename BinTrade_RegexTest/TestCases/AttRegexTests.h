#pragma once

#include "Regex.h"
#include "RegexGroup.h"
#include "RegexGroupCollection.h"
#include "RegexMatch.h"
#include "UnitTestAssert.h"
#include "RegexWpf/RegexOptions.h"

namespace RegexsTest
{
	using namespace std;
	using namespace Regexs;

	//https://github.com/dotnet/runtime/tree/b4c746b7712e887af066d66d1ec777be6283f6f9/src/libraries/System.Text.RegularExpressions/tests/FunctionalTests
	class AttRegexTests
	{
		struct Struct2Int
		{
			int Key;
			int Value;

			Struct2Int(int key, int value)
				: Key(key),
				  Value(value)
			{
			}
		};

		static std::vector<std::string>& splitStrA(const std::string& InputString, char delimiterChar, std::vector<std::string>& ResultVec)
		{
			std::stringstream sStream(InputString);
			std::string item;
			while (std::getline(sStream, item, delimiterChar))
			{
				ResultVec.push_back(item);
			}
			return ResultVec;
		}

		static std::vector<std::wstring>& splitStrW(const std::wstring& InputString, wchar_t delimiterChar, std::vector<std::wstring>& ResultVec)
		{
			std::wstringstream sStream(InputString);
			std::wstring item;

			while (std::getline(sStream, item, delimiterChar))
			{
				ResultVec.push_back(item);
			}
			return ResultVec;
		}

		static bool Contains(std::vector<Struct2Int> list, Struct2Int item)
		{
			for (size_t i = 0; i < list.size(); ++i)
			{
				if (list.at(i).Key == item.Key && list.at(i).Value == item.Value)
					return true;
			}
			return false;
		}

	public:
		static void Run(bool test_throw)
		{
			Assert::WriteLine(L"AttRegexTests");

			RunOpi(RegexOptions::None);
			RunOpi(RegexOptions::Multiline);
		}

		static void RunOpi(RegexOptions opi)
		{
			RunItem(opi, L"abracadabra$", L"abracadabracadabra", L"(7,18)");
			RunItem(opi, L"a...b", L"abababbb", L"(2,7)");
			RunItem(opi, L"XXXXXX", L"..XXXXXX", L"(2,8)");
			RunItem(opi, L"\\)", L"()", L"(1,2)");
			RunItem(opi, L"a]", L"a]a", L"(0,2)");
			RunItem(opi, L"}", L"}", L"(0,1)");
			RunItem(opi, L"\\}", L"}", L"(0,1)");
			RunItem(opi, L"\\]", L"]", L"(0,1)");
			RunItem(opi, L"]", L"]", L"(0,1)");
			RunItem(opi, L"{", L"{", L"(0,1)");
			RunItem(opi, L"^a", L"ax", L"(0,1)");
			RunItem(opi, L"\\^a", L"a^a", L"(1,3)");
			RunItem(opi, L"a\\^", L"a^", L"(0,2)");
			RunItem(opi, L"a$", L"aa", L"(1,2)");
			RunItem(opi, L"a\\$", L"a$", L"(0,2)");
			RunItem(opi, L"^$", L"NULL", L"(0,0)");
			RunItem(opi, L"$^", L"NULL", L"(0,0)");
			RunItem(opi, L"a($)", L"aa", L"(1,2)(2,2)");
			RunItem(opi, L"a*(^a)", L"aa", L"(0,1)(0,1)");
			RunItem(opi, L"(..)*(...)*", L"a", L"(0,0)");
			RunItem(opi, L"(..)*(...)*", L"abcd", L"(0,4)(2,4)");
			RunItem(opi, L"(ab|a)(bc|c)", L"abc", L"(0,3)(0,2)(2,3)");
			RunItem(opi, L"(ab)c|abc", L"abc", L"(0,3)(0,2)");
			RunItem(opi, L"a{0}b", L"ab", L"(1,2)");
			RunItem(opi, L"(a*)(b?)(b+)b{3}", L"aaabbbbbbb", L"(0,10)(0,3)(3,4)(4,7)");
			RunItem(opi, L"(a*)(b{0,1})(b{1,})b{3}", L"aaabbbbbbb", L"(0,10)(0,3)(3,4)(4,7)");
			RunItem(opi, L"a{9876543210}", L"NULL", L"BADBR");
			RunItem(opi, L"((a|a)|a)", L"a", L"(0,1)(0,1)(0,1)");
			RunItem(opi, L"(a*)(a|aa)", L"aaaa", L"(0,4)(0,3)(3,4)");
			RunItem(opi, L"a*(a.|aa)", L"aaaa", L"(0,4)(2,4)");
			RunItem(opi, L"(a|b)?.*", L"b", L"(0,1)(0,1)");
			RunItem(opi, L"(a|b)c|a(b|c)", L"ac", L"(0,2)(0,1)");
			RunItem(opi, L"(a|b)*c|(a|ab)*c", L"abc", L"(0,3)(1,2)");
			RunItem(opi, L"(a|b)*c|(a|ab)*c", L"xc", L"(1,2)");
			RunItem(opi, L"(.a|.b).*|.*(.a|.b)", L"xa", L"(0,2)(0,2)");
			RunItem(opi, L"a?(ab|ba)ab", L"abab", L"(0,4)(0,2)");
			RunItem(opi, L"a?(ac{0}b|ba)ab", L"abab", L"(0,4)(0,2)");
			RunItem(opi, L"ab|abab", L"abbabab", L"(0,2)");
			RunItem(opi, L"aba|bab|bba", L"baaabbbaba", L"(5,8)");
			RunItem(opi, L"aba|bab", L"baaabbbaba", L"(6,9)");
			RunItem(opi, L"(aa|aaa)*|(a|aaaaa)", L"aa", L"(0,2)(0,2)");
			RunItem(opi, L"(a.|.a.)*|(a|.a...)", L"aa", L"(0,2)(0,2)");
			RunItem(opi, L"ab|a", L"xabc", L"(1,3)");
			RunItem(opi, L"ab|a", L"xxabc", L"(2,4)");
			RunItem(opi, L"(?i)(Ab|cD)*", L"aBcD", L"(0,4)(2,4)");
			RunItem(opi, L"[^-]", L"--a", L"(2,3)");
			RunItem(opi, L"[a-]*", L"--a", L"(0,3)");
			RunItem(opi, L"[a-m-]*", L"--amoma--", L"(0,4)");
			RunItem(opi, L":::1:::0:|:::1:1:0:", L":::0:::1:::1:::0:", L"(8,17)");
			RunItem(opi, L":::1:::0:|:::1:1:1:", L":::0:::1:::1:::0:", L"(8,17)");
			RunItem(opi, L"\n", L"\n", L"(0,1)");
			RunItem(opi, L"[^a]", L"\n", L"(0,1)");
			RunItem(opi, L"\na", L"\na", L"(0,2)");
			RunItem(opi, L"(a)(b)(c)", L"abc", L"(0,3)(0,1)(1,2)(2,3)");
			RunItem(opi, L"xxx", L"xxx", L"(0,3)");
			RunItem(opi, L"(^|[ (,;])((([Ff]eb[^ ]* *|0*2/|\\* */?)0*[6-7]))([^0-9]|$)", L"feb 6,", L"(0,6)");
			RunItem(opi, L"(^|[ (,;])((([Ff]eb[^ ]* *|0*2/|\\* */?)0*[6-7]))([^0-9]|$)", L"2/7", L"(0,3)");
			RunItem(opi, L"(^|[ (,;])((([Ff]eb[^ ]* *|0*2/|\\* */?)0*[6-7]))([^0-9]|$)", L"feb 1,Feb 6", L"(5,11)");
			RunItem(opi, L"((((((((((((((((((((((((((((((x))))))))))))))))))))))))))))))", L"x", L"(0,1)(0,1)(0,1)");
			RunItem(opi, L"((((((((((((((((((((((((((((((x))))))))))))))))))))))))))))))*", L"xx", L"(0,2)(1,2)(1,2)");
			RunItem(opi, L"a?(ab|ba)*", L"ababababababababababababababababababababababababababababababababababababababababa", L"(0,81)(79,81)");
			RunItem(opi, L"abaa|abbaa|abbbaa|abbbbaa", L"ababbabbbabbbabbbbabbbbaa", L"(18,25)");
			RunItem(opi, L"abaa|abbaa|abbbaa|abbbbaa", L"ababbabbbabbbabbbbabaa", L"(18,22)");
			RunItem(opi, L"aaac|aabc|abac|abbc|baac|babc|bbac|bbbc", L"baaabbbabac", L"(7,11)");
			RunItem(opi, L".*", L"\x0001\x00ff", L"(0,2)");
			RunItem(opi, L"aaaa|bbbb|cccc|ddddd|eeeeee|fffffff|gggg|hhhh|iiiii|jjjjj|kkkkk|llll", L"XaaaXbbbXcccXdddXeeeXfffXgggXhhhXiiiXjjjXkkkXlllXcbaXaaaa", L"(53,57)");
			RunItem(opi, L"aaaa\nbbbb\ncccc\nddddd\neeeeee\nfffffff\ngggg\nhhhh\niiiii\njjjjj\nkkkkk\nllll", L"XaaaXbbbXcccXdddXeeeXfffXgggXhhhXiiiXjjjXkkkXlllXcbaXaaaa", L"NOMATCH");
			RunItem(opi, L"a*a*a*a*a*b", L"aaaaaaaaab", L"(0,10)");
			RunItem(opi, L"^", L"NULL", L"(0,0)");
			RunItem(opi, L"$", L"NULL", L"(0,0)");
			RunItem(opi, L"^a$", L"a", L"(0,1)");
			RunItem(opi, L"abc", L"abc", L"(0,3)");
			RunItem(opi, L"abc", L"xabcy", L"(1,4)");
			RunItem(opi, L"abc", L"ababc", L"(2,5)");
			RunItem(opi, L"ab*c", L"abc", L"(0,3)");
			RunItem(opi, L"ab*bc", L"abc", L"(0,3)");
			RunItem(opi, L"ab*bc", L"abbc", L"(0,4)");
			RunItem(opi, L"ab*bc", L"abbbbc", L"(0,6)");
			RunItem(opi, L"ab+bc", L"abbc", L"(0,4)");
			RunItem(opi, L"ab+bc", L"abbbbc", L"(0,6)");
			RunItem(opi, L"ab?bc", L"abbc", L"(0,4)");
			RunItem(opi, L"ab?bc", L"abc", L"(0,3)");
			RunItem(opi, L"ab?c", L"abc", L"(0,3)");
			RunItem(opi, L"^abc$", L"abc", L"(0,3)");
			RunItem(opi, L"^abc", L"abcc", L"(0,3)");
			RunItem(opi, L"abc$", L"aabc", L"(1,4)");
			RunItem(opi, L"^", L"abc", L"(0,0)");
			RunItem(opi, L"$", L"abc", L"(3,3)");
			RunItem(opi, L"a.c", L"abc", L"(0,3)");
			RunItem(opi, L"a.c", L"axc", L"(0,3)");
			RunItem(opi, L"a.*c", L"axyzc", L"(0,5)");
			RunItem(opi, L"a[bc]d", L"abd", L"(0,3)");
			RunItem(opi, L"a[b-d]e", L"ace", L"(0,3)");
			RunItem(opi, L"a[b-d]", L"aac", L"(1,3)");
			RunItem(opi, L"a[-b]", L"a-", L"(0,2)");
			RunItem(opi, L"a[b-]", L"a-", L"(0,2)");
			RunItem(opi, L"a]", L"a]", L"(0,2)");
			RunItem(opi, L"a[]]b", L"a]b", L"(0,3)");
			RunItem(opi, L"a[^bc]d", L"aed", L"(0,3)");
			RunItem(opi, L"a[^-b]c", L"adc", L"(0,3)");
			RunItem(opi, L"a[^]b]c", L"adc", L"(0,3)");
			RunItem(opi, L"ab|cd", L"abc", L"(0,2)");
			RunItem(opi, L"ab|cd", L"abcd", L"(0,2)");
			RunItem(opi, L"a\\(b", L"a(b", L"(0,3)");
			RunItem(opi, L"a\\(*b", L"ab", L"(0,2)");
			RunItem(opi, L"a\\(*b", L"a((b", L"(0,4)");
			RunItem(opi, L"((a))", L"abc", L"(0,1)(0,1)(0,1)");
			RunItem(opi, L"(a)b(c)", L"abc", L"(0,3)(0,1)(2,3)");
			RunItem(opi, L"a+b+c", L"aabbabc", L"(4,7)");
			RunItem(opi, L"a*", L"aaa", L"(0,3)");
			RunItem(opi, L"(a*)*", L"-", L"(0,0)(0,0)");
			RunItem(opi, L"(a*)+", L"-", L"(0,0)(0,0)");
			RunItem(opi, L"(a*|b)*", L"-", L"(0,0)(0,0)");
			RunItem(opi, L"(a+|b)*", L"ab", L"(0,2)(1,2)");
			RunItem(opi, L"(a+|b)+", L"ab", L"(0,2)(1,2)");
			RunItem(opi, L"(a+|b)?", L"ab", L"(0,1)(0,1)");
			RunItem(opi, L"[^ab]*", L"cde", L"(0,3)");
			RunItem(opi, L"(^)*", L"-", L"(0,0)(0,0)");
			RunItem(opi, L"a*", L"NULL", L"(0,0)");
			RunItem(opi, L"([abc])*d", L"abbbcd", L"(0,6)(4,5)");
			RunItem(opi, L"([abc])*bcd", L"abcd", L"(0,4)(0,1)");
			RunItem(opi, L"a|b|c|d|e", L"e", L"(0,1)");
			RunItem(opi, L"(a|b|c|d|e)f", L"ef", L"(0,2)(0,1)");
			RunItem(opi, L"((a*|b))*", L"-", L"(0,0)(0,0)(0,0)");
			RunItem(opi, L"abcd*efg", L"abcdefg", L"(0,7)");
			RunItem(opi, L"ab*", L"xabyabbbz", L"(1,3)");
			RunItem(opi, L"ab*", L"xayabbbz", L"(1,2)");
			RunItem(opi, L"(ab|cd)e", L"abcde", L"(2,5)(2,4)");
			RunItem(opi, L"[abhgefdc]ij", L"hij", L"(0,3)");
			RunItem(opi, L"(a|b)c*d", L"abcd", L"(1,4)(1,2)");
			RunItem(opi, L"(ab|ab*)bc", L"abc", L"(0,3)(0,1)");
			RunItem(opi, L"a([bc]*)c*", L"abc", L"(0,3)(1,3)");
			RunItem(opi, L"a([bc]*)(c*d)", L"abcd", L"(0,4)(1,3)(3,4)");
			RunItem(opi, L"a([bc]+)(c*d)", L"abcd", L"(0,4)(1,3)(3,4)");
			RunItem(opi, L"a([bc]*)(c+d)", L"abcd", L"(0,4)(1,2)(2,4)");
			RunItem(opi, L"a[bcd]*dcdcde", L"adcdcde", L"(0,7)");
			RunItem(opi, L"(ab|a)b*c", L"abc", L"(0,3)(0,2)");
			RunItem(opi, L"((a)(b)c)(d)", L"abcd", L"(0,4)(0,3)(0,1)(1,2)(3,4)");
			RunItem(opi, L"[A-Za-z_][A-Za-z0-9_]*", L"alpha", L"(0,5)");
			RunItem(opi, L"^a(bc+|b[eh])g|.h$", L"abh", L"(1,3)");
			RunItem(opi, L"(bc+d$|ef*g.|h?i(j|k))", L"effgz", L"(0,5)(0,5)");
			RunItem(opi, L"(bc+d$|ef*g.|h?i(j|k))", L"ij", L"(0,2)(0,2)(1,2)");
			RunItem(opi, L"(bc+d$|ef*g.|h?i(j|k))", L"reffgz", L"(1,6)(1,6)");
			RunItem(opi, L"(((((((((a)))))))))", L"a", L"(0,1)(0,1)(0,1)(0,1)(0,1)(0,1)(0,1)(0,1)(0,1)(0,1)");
			RunItem(opi, L"multiple words", L"multiple words yeah", L"(0,14)");
			RunItem(opi, L"(.*)c(.*)", L"abcde", L"(0,5)(0,2)(3,5)");
			RunItem(opi, L"abcd", L"abcd", L"(0,4)");
			RunItem(opi, L"a(bc)d", L"abcd", L"(0,4)(1,3)");
			RunItem(opi, L"a[-]?c", L"ac", L"(0,3)");
			RunItem(opi, L"a+(b|c)*d+", L"aabcdd", L"(0,6)(3,4)");
			RunItem(opi, L"^.+$", L"vivi", L"(0,4)");
			RunItem(opi, L"^(.+)$", L"vivi", L"(0,4)(0,4)");
			RunItem(opi, L"^([^!.]+).att.com!(.+)$", L"gryphon.att.com!eby", L"(0,19)(0,7)(16,19)");
			RunItem(opi, L"^([^!]+!)?([^!]+)$", L"bar!bas", L"(0,7)(0,4)(4,7)");
			RunItem(opi, L"^([^!]+!)?([^!]+)$", L"foo!bas", L"(0,7)(0,4)(4,7)");
			RunItem(opi, L"^.+!([^!]+!)([^!]+)$", L"foo!bar!bas", L"(0,11)(4,8)(8,11)");
			RunItem(opi, L"((foo)|(bar))!bas", L"foo!bas", L"(0,7)(0,3)(0,3)");
			RunItem(opi, L"((foo)|bar)!bas", L"bar!bas", L"(0,7)(0,3)");
			RunItem(opi, L"((foo)|bar)!bas", L"foo!bar!bas", L"(4,11)(4,7)");
			RunItem(opi, L"((foo)|bar)!bas", L"foo!bas", L"(0,7)(0,3)(0,3)");
			RunItem(opi, L"(foo|(bar))!bas", L"bar!bas", L"(0,7)(0,3)(0,3)");
			RunItem(opi, L"(foo|(bar))!bas", L"foo!bar!bas", L"(4,11)(4,7)(4,7)");
			RunItem(opi, L"(foo|(bar))!bas", L"foo!bas", L"(0,7)(0,3)");
			RunItem(opi, L"(foo|bar)!bas", L"bar!bas", L"(0,7)(0,3)");
			RunItem(opi, L"(foo|bar)!bas", L"foo!bar!bas", L"(4,11)(4,7)");
			RunItem(opi, L"(foo|bar)!bas", L"foo!bas", L"(0,7)(0,3)");
			RunItem(opi, L"^([^!]+!)?([^!]+)$|^.+!([^!]+!)([^!]+)$", L"bar!bas", L"(0,7)(0,4)(4,7)");
			RunItem(opi, L"^([^!]+!)?([^!]+)$|^.+!([^!]+!)([^!]+)$", L"foo!bas", L"(0,7)(0,4)(4,7)");
			RunItem(opi, L"^(([^!]+!)?([^!]+)|.+!([^!]+!)([^!]+))$", L"bar!bas", L"(0,7)(0,7)(0,4)(4,7)");
			RunItem(opi, L"^(([^!]+!)?([^!]+)|.+!([^!]+!)([^!]+))$", L"foo!bas", L"(0,7)(0,7)(0,4)(4,7)");
			RunItem(opi, L".*(/XXX).*", L"/XXX", L"(0,4)(0,4)");
			RunItem(opi, L".*(\\\\XXX).*", L"\\XXX", L"(0,4)(0,4)");
			RunItem(opi, L"\\\\XXX", L"\\XXX", L"(0,4)");
			RunItem(opi, L".*(/000).*", L"/000", L"(0,4)(0,4)");
			RunItem(opi, L".*(\\\\000).*", L"\\000", L"(0,4)(0,4)");
			RunItem(opi, L"\\\\000", L"\\000", L"(0,4)");

			// repetition.dat
			RunItem(opi, L"((..)|(.))", L"NULL", L"NOMATCH");
			RunItem(opi, L"((..)|(.))((..)|(.))", L"NULL", L"NOMATCH");
			RunItem(opi, L"((..)|(.))((..)|(.))((..)|(.))", L"NULL", L"NOMATCH");
			RunItem(opi, L"((..)|(.)){1}", L"NULL", L"NOMATCH");
			RunItem(opi, L"((..)|(.)){2}", L"NULL", L"NOMATCH");
			RunItem(opi, L"((..)|(.)){3}", L"NULL", L"NOMATCH");
			RunItem(opi, L"((..)|(.))*", L"NULL", L"(0,0)");
			RunItem(opi, L"((..)|(.))((..)|(.))", L"a", L"NOMATCH");
			RunItem(opi, L"((..)|(.))((..)|(.))((..)|(.))", L"a", L"NOMATCH");
			RunItem(opi, L"((..)|(.)){2}", L"a", L"NOMATCH");
			RunItem(opi, L"((..)|(.)){3}", L"a", L"NOMATCH");
			RunItem(opi, L"((..)|(.))((..)|(.))((..)|(.))", L"aa", L"NOMATCH");
			RunItem(opi, L"((..)|(.)){3}", L"aa", L"NOMATCH");
			RunItem(opi, L"X(.?){0,}Y", L"X1234567Y", L"(0,9)(8,8)"); // was "(0,9)(7,8)"
			RunItem(opi, L"X(.?){1,}Y", L"X1234567Y", L"(0,9)(8,8)"); // was "(0,9)(7,8)"
			RunItem(opi, L"X(.?){2,}Y", L"X1234567Y", L"(0,9)(8,8)"); // was "(0,9)(7,8)"
			RunItem(opi, L"X(.?){3,}Y", L"X1234567Y", L"(0,9)(8,8)"); // was "(0,9)(7,8)"
			RunItem(opi, L"X(.?){4,}Y", L"X1234567Y", L"(0,9)(8,8)"); // was "(0,9)(7,8)"
			RunItem(opi, L"X(.?){5,}Y", L"X1234567Y", L"(0,9)(8,8)"); // was "(0,9)(7,8)"
			RunItem(opi, L"X(.?){6,}Y", L"X1234567Y", L"(0,9)(8,8)"); // was "(0,9)(7,8)"
			RunItem(opi, L"X(.?){7,}Y", L"X1234567Y", L"(0,9)(8,8)"); // was "(0,9)(7,8)"
			RunItem(opi, L"X(.?){8,}Y", L"X1234567Y", L"(0,9)(8,8)");
			RunItem(opi, L"X(.?){0,8}Y", L"X1234567Y", L"(0,9)(8,8)"); // was "(0,9)(7,8)"
			RunItem(opi, L"X(.?){1,8}Y", L"X1234567Y", L"(0,9)(8,8)"); // was "(0,9)(7,8)"
			RunItem(opi, L"X(.?){2,8}Y", L"X1234567Y", L"(0,9)(8,8)"); // was "(0,9)(7,8)"
			RunItem(opi, L"X(.?){3,8}Y", L"X1234567Y", L"(0,9)(8,8)"); // was "(0,9)(7,8)"
			RunItem(opi, L"X(.?){4,8}Y", L"X1234567Y", L"(0,9)(8,8)"); // was "(0,9)(7,8)"
			RunItem(opi, L"X(.?){5,8}Y", L"X1234567Y", L"(0,9)(8,8)"); // was "(0,9)(7,8)"
			RunItem(opi, L"X(.?){6,8}Y", L"X1234567Y", L"(0,9)(8,8)"); // was "(0,9)(7,8)"
			RunItem(opi, L"X(.?){7,8}Y", L"X1234567Y", L"(0,9)(8,8)"); // was "(0,9)(7,8)"
			RunItem(opi, L"X(.?){8,8}Y", L"X1234567Y", L"(0,9)(8,8)");
			RunItem(opi, L"(a|ab|c|bcd){0,}(d*)", L"ababcd", L"(0,1)(1,1)"); // was "(0,6)(3,6)(6,6)"
			RunItem(opi, L"(a|ab|c|bcd){1,}(d*)", L"ababcd", L"(0,1)(1,1)"); // was "(0,6)(3,6)(6,6)"
			RunItem(opi, L"(a|ab|c|bcd){2,}(d*)", L"ababcd", L"(0,6)(3,6)(6,6)");
			RunItem(opi, L"(a|ab|c|bcd){3,}(d*)", L"ababcd", L"(0,6)(3,6)(6,6)");
			RunItem(opi, L"(a|ab|c|bcd){4,}(d*)", L"ababcd", L"NOMATCH");
			RunItem(opi, L"(a|ab|c|bcd){0,10}(d*)", L"ababcd", L"(0,1)(1,1)"); // was "(0,6)(3,6)(6,6)"
			RunItem(opi, L"(a|ab|c|bcd){1,10}(d*)", L"ababcd", L"(0,1)(1,1)"); // was "(0,6)(3,6)(6,6)"
			RunItem(opi, L"(a|ab|c|bcd){2,10}(d*)", L"ababcd", L"(0,6)(3,6)(6,6)");
			RunItem(opi, L"(a|ab|c|bcd){3,10}(d*)", L"ababcd", L"(0,6)(3,6)(6,6)");
			RunItem(opi, L"(a|ab|c|bcd){4,10}(d*)", L"ababcd", L"NOMATCH");
			RunItem(opi, L"(a|ab|c|bcd)*(d*)", L"ababcd", L"(0,1)(1,1)"); // was "(0,6)(3,6)(6,6)"
			RunItem(opi, L"(a|ab|c|bcd)+(d*)", L"ababcd", L"(0,1)(1,1)"); // was "(0,6)(3,6)(6,6)"
			RunItem(opi, L"(ab|a|c|bcd){0,}(d*)", L"ababcd", L"(0,6)(4,5)(5,6)"); // was "(0,6)(3,6)(6,6)"
			RunItem(opi, L"(ab|a|c|bcd){1,}(d*)", L"ababcd", L"(0,6)(4,5)(5,6)"); // was "(0,6)(3,6)(6,6)"
			RunItem(opi, L"(ab|a|c|bcd){2,}(d*)", L"ababcd", L"(0,6)(4,5)(5,6)"); // was "(0,6)(3,6)(6,6)"
			RunItem(opi, L"(ab|a|c|bcd){3,}(d*)", L"ababcd", L"(0,6)(4,5)(5,6)"); // was "(0,6)(3,6)(6,6)"
			RunItem(opi, L"(ab|a|c|bcd){4,}(d*)", L"ababcd", L"NOMATCH");
			RunItem(opi, L"(ab|a|c|bcd){0,10}(d*)", L"ababcd", L"(0,6)(4,5)(5,6)"); // was "(0,6)(3,6)(6,6)"
			RunItem(opi, L"(ab|a|c|bcd){1,10}(d*)", L"ababcd", L"(0,6)(4,5)(5,6)"); // was "(0,6)(3,6)(6,6)"
			RunItem(opi, L"(ab|a|c|bcd){2,10}(d*)", L"ababcd", L"(0,6)(4,5)(5,6)"); // was "(0,6)(3,6)(6,6)"
			RunItem(opi, L"(ab|a|c|bcd){3,10}(d*)", L"ababcd", L"(0,6)(4,5)(5,6)"); // was "(0,6)(3,6)(6,6)"
			RunItem(opi, L"(ab|a|c|bcd){4,10}(d*)", L"ababcd", L"NOMATCH");
			RunItem(opi, L"(ab|a|c|bcd)*(d*)", L"ababcd", L"(0,6)(4,5)(5,6)"); // was "(0,6)(3,6)(6,6)"
			RunItem(opi, L"(ab|a|c|bcd)+(d*)", L"ababcd", L"(0,6)(4,5)(5,6)"); // was "(0,6)(3,6)(6,6)"

			// unknownassoc.dat
			RunItem(opi, L"(a|ab)(c|bcd)(d*)", L"abcd", L"(0,4)(0,1)(1,4)(4,4)");
			RunItem(opi, L"(a|ab)(bcd|c)(d*)", L"abcd", L"(0,4)(0,1)(1,4)(4,4)");
			RunItem(opi, L"(ab|a)(c|bcd)(d*)", L"abcd", L"(0,4)(0,2)(2,3)(3,4)");
			RunItem(opi, L"(ab|a)(bcd|c)(d*)", L"abcd", L"(0,4)(0,2)(2,3)(3,4)");
			RunItem(opi, L"(a*)(b|abc)(c*)", L"abc", L"(0,3)(0,1)(1,2)(2,3)");
			RunItem(opi, L"(a*)(abc|b)(c*)", L"abc", L"(0,3)(0,1)(1,2)(2,3)");
			RunItem(opi, L"(a|ab)(c|bcd)(d|.*)", L"abcd", L"(0,4)(0,1)(1,4)(4,4)");
			RunItem(opi, L"(a|ab)(bcd|c)(d|.*)", L"abcd", L"(0,4)(0,1)(1,4)(4,4)");
			RunItem(opi, L"(ab|a)(c|bcd)(d|.*)", L"abcd", L"(0,4)(0,2)(2,3)(3,4)");
			RunItem(opi, L"(ab|a)(bcd|c)(d|.*)", L"abcd", L"(0,4)(0,2)(2,3)(3,4)");

			// nullsubexpr.dat
			RunItem(opi, L"(a*)*", L"a", L"(0,1)(0,1)");
			RunItem(opi, L"(a*)*", L"x", L"(0,0)(0,0)");
			RunItem(opi, L"(a*)*", L"aaaaaa", L"(0,6)(0,6)");
			RunItem(opi, L"(a*)*", L"aaaaaax", L"(0,6)(0,6)");
			RunItem(opi, L"(a*)+", L"a", L"(0,1)(0,1)");
			RunItem(opi, L"(a+)*", L"a", L"(0,1)(0,1)");
			RunItem(opi, L"(a+)*", L"x", L"(0,0)");
			RunItem(opi, L"(a+)+", L"a", L"(0,1)(0,1)");
			RunItem(opi, L"(a+)+", L"x", L"NOMATCH");
			RunItem(opi, L"([a]*)*", L"a", L"(0,1)(0,1)");
			RunItem(opi, L"([a]*)+", L"a", L"(0,1)(0,1)");
			RunItem(opi, L"([^b]*)*", L"a", L"(0,1)(0,1)");
			RunItem(opi, L"([^b]*)*", L"b", L"(0,0)(0,0)");
			RunItem(opi, L"([^b]*)*", L"aaaaaab", L"(0,6)(0,6)");
			RunItem(opi, L"([ab]*)*", L"a", L"(0,1)(0,1)");
			RunItem(opi, L"([ab]*)*", L"ababab", L"(0,6)(0,6)");
			RunItem(opi, L"([ab]*)*", L"bababa", L"(0,6)(0,6)");
			RunItem(opi, L"([ab]*)*", L"b", L"(0,1)(0,1)");
			RunItem(opi, L"([ab]*)*", L"bbbbbb", L"(0,6)(0,6)");
			RunItem(opi, L"([ab]*)*", L"aaaabcde", L"(0,5)(0,5)");
			RunItem(opi, L"([^a]*)*", L"b", L"(0,1)(0,1)");
			RunItem(opi, L"([^a]*)*", L"aaaaaa", L"(0,0)(0,0)");
			RunItem(opi, L"([^ab]*)*", L"ccccxx", L"(0,6)(0,6)");
			RunItem(opi, L"([^ab]*)*", L"ababab", L"(0,0)(0,0)");
			RunItem(opi, L"((z)+|a)*", L"zabcde", L"(0,2)(1,2)");
			RunItem(opi, L"a+?", L"aaaaaa", L"(0,1)");
			RunItem(opi, L"(a)", L"aaa", L"(0,1)(0,1)");
			RunItem(opi, L"(a*?)", L"aaa", L"(0,0)(0,0)");
			RunItem(opi, L"(a)*?", L"aaa", L"(0,0)");
			RunItem(opi, L"(a*?)*?", L"aaa", L"(0,0)");
			RunItem(opi, L"(a*)*(x)", L"x", L"(0,1)(0,0)(0,1)");
			RunItem(opi, L"(a*)*(x)(\\1)", L"x", L"(0,1)(0,0)(0,1)(1,1)", true);
			RunItem(opi, L"(a*)*(x)(\\1)", L"ax", L"(0,2)(1,1)(1,2)(2,2)", true);
			RunItem(opi, L"(a*)*(x)(\\1)", L"axa", L"(0,2)(1,1)(1,2)(2,2)", true); // was "(0,3)(0,1)(1,2)(2,3)"
			RunItem(opi, L"(a*)*(x)(\\1)(x)", L"axax", L"(0,4)(0,1)(1,2)(2,3)(3,4)", true);
			RunItem(opi, L"(a*)*(x)(\\1)(x)", L"axxa", L"(0,3)(1,1)(1,2)(2,2)(2,3)", true);
			RunItem(opi, L"(a*)*(x)", L"ax", L"(0,2)(1,1)(1,2)");
			RunItem(opi, L"(a*)*(x)", L"axa", L"(0,2)(1,1)(1,2)"); // was "(0,2)(0,1)(1,2)"
			RunItem(opi, L"(a*)+(x)", L"x", L"(0,1)(0,0)(0,1)");
			RunItem(opi, L"(a*)+(x)", L"ax", L"(0,2)(1,1)(1,2)"); // was "(0,2)(0,1)(1,2)"
			RunItem(opi, L"(a*)+(x)", L"axa", L"(0,2)(1,1)(1,2)"); // was "(0,2)(0,1)(1,2)"
			RunItem(opi, L"(a*){2}(x)", L"x", L"(0,1)(0,0)(0,1)");
			RunItem(opi, L"(a*){2}(x)", L"ax", L"(0,2)(1,1)(1,2)");
			RunItem(opi, L"(a*){2}(x)", L"axa", L"(0,2)(1,1)(1,2)");
		}

		static void RunItem(RegexOptions options, std::wstring pattern, std::wstring input, std::wstring expected, bool skipNonBacktracking = false)
		{
			if (input == L"NULL")
			{
				input = L"";
			}

			if (expected == L"BADBR")
			{
				return;
			}

			if (skipNonBacktracking)
			{
				return;
			}

			Regex r = Regex(pattern.c_str(), options);

			if (expected == L"NOMATCH")
			{
				Assert::False(r.IsMatch(input.c_str()));
				return;
			}

			Match* match = r.Match(&input);
			Assert::True(match->getSuccess());

			std::vector<Struct2Int> expectedSet;
			std::vector<Struct2Int> actualSet;
			const string expecteds = string(expected.begin(), expected.end());


			std::vector<string> split1;
			std::vector<string> split2;
			splitStrA(expecteds, '(', split1);
			for (size_t i = 0; i < split1.size(); ++i)
			{
				splitStrA(split1.at(i), ')', split2);
			}
			for (size_t i = 0; i < split2.size(); ++i)
			{
				std::vector<string> split3;
				splitStrA(split2.at(i), ',', split3);


				auto sl2_0 = split3.at(0);
				auto sl2_1 = split3.at(1);
				Struct2Int struct2_int(atoi(sl2_0.c_str()), atoi(sl2_1.c_str()));


				if (!Contains(expectedSet, struct2_int))
					expectedSet.push_back(struct2_int);
			}


			for (int i = 0; i < match->getGroups()->getCount(); ++i)
			{
				const auto item = match->getGroups()->GetGroup(i);
				Group* group = dynamic_cast<Group*>(item);
				if (group != NULL && group->getSuccess())
				{
					Struct2Int key_value_pair(item->getIndex(), item->getIndex() + item->getLength());
					if (!Contains(actualSet, key_value_pair))
						actualSet.push_back(key_value_pair);
				}
			}

			if (expectedSet.size() > actualSet.size())
				Assert::False(true); //

			for (size_t i = 0; i < expectedSet.size(); ++i)
			{
				if (!Contains(actualSet, expectedSet.at(i)))
				{
					Assert::False(true);
				}
			}
		}
	};
}
