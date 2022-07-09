#pragma once

#include "CultureInfo.h"
#include "RegexMatchTimeoutException.h"
#include "UnitTestAssert.h"
#include "Other/stringbuilder.h"

namespace RegexsTest
{
	using namespace std;
	using namespace Regexs;


	class RegexKnownPatternTests
	{
		struct Data1
		{
			std::wstring data;
			bool status;

			Data1(const std::wstring& data, bool status)
				: data(data),
				  status(status)
			{
			}
		};


		struct StructMemberData
		{
			wstring url;
			wstring expected;

			StructMemberData(const wstring& url, const wstring& expected)
				: url(url),
				  expected(expected)
			{
			}
		};

		struct StructRealWorld
		{
			wstring value;
			bool isExpectedMatch;

			StructRealWorld(const wstring& value, bool is_expected_match)
				: value(value),
				  isExpectedMatch(is_expected_match)
			{
			}
		};

		struct StructExternalEntryPoin
		{
			wstring value;
			wstring a;
			wstring b;
			wstring c;

			StructExternalEntryPoin(const wstring& value, const wstring& a, const wstring& b, const wstring& c)
				: value(value),
				  a(a),
				  b(b),
				  c(c)
			{
			}
		};

		struct StructSAD
		{
			wstring input;
			bool expectedMatch;

			StructSAD(const wstring& input, bool expected_match)
				: input(input),
				  expectedMatch(expected_match)
			{
			}
		};

	public:
		static void Run(bool test_throw)
		{
			Assert::WriteLine(L"RegexKnownPatternTests");

			Docs_Examples_ScanningHrefs();
			Docs_Examples_MDYtoDMY();
			voidDocs_Examples_ExtractProtocolPort();

			auto value_1 = Docs_Examples_ValidateEmail_TestData();
			for (size_t i = 0; i < value_1.size(); ++i)
			{
				Docs_Examples_ValidateEmail(value_1.at(i).data, value_1.at(i).status);
			}
			Docs_GroupingConstructs_MatchedSubexpression();
			Docs_GroupingConstructs_NamedMatchedSubexpression1();
			Docs_GroupingConstructs_NamedMatchedSubexpression2();
			Docs_GroupingConstructs_BalancingGroups();
			Docs_GroupingConstructs_NoncapturingGroups();
			Docs_GroupingConstructs_GroupOptions();
			Docs_GroupingConstructs_ZeroWidthPositiveLookaheadAssertions();
			Docs_GroupingConstructs_ZeroWidthNegativeLookaheadAssertions();


			Docs_GroupingConstructs_ZeroWidthPositiveLookbehindAssertions();
			Docs_GroupingConstructs_ZeroWidthNegativeLookbehindAssertions();
			Docs_GroupingConstructs_NonbacktrackingSubexpressions();
			Docs_GroupingConstructs_GroupCaptureRelationship();
			Docs_Capture_Sentences();
			Docs_Capture_ProductNumber();
			Docs_Backtracking_LinearComparisonWithoutBacktracking();
			Docs_Backtracking_WithOptionalQuantifiersOrAlternationConstructs();
			Docs_Backtracking_WithNestedOptionalQuantifiers_BacktrackingEliminated();
			Docs_Backtracking_LookbehindAssertions();
			Docs_Backtracking_LookaheadAssertions_BacktrackingEliminated();
			Docs_EngineCapabilities_LazyQuantifiers();
			Docs_EngineCapabilities_PositiveLookahead();
			Docs_EngineCapabilities_NegativeLookahead();
			Docs_EngineCapabilities_ConditionalEvaluation();
			Docs_EngineCapabilities_RightToLeftMatching();
			Docs_EngineCapabilities_PositiveNegativeLookbehind();
			Docs_InlineOptions();
			Docs_InlineComment();
			Docs_EndOfLineComment();
			Docs_Anchors_ContiguousMatches();
			Blog_Levithan_BalancingGroups_Palindromes();
			Blog_Levithan_BalancingGroups_MatchingParentheses();
			Blog_Levithan_BalancingGroups_WordLengthIncreases();


			auto value_2 = RealWorld_ExtractResourceUri_MemberData();
			for (StructMemberData member_data : value_2)
			{
				RealWorld_ExtractResourceUri(member_data.url, member_data.expected);
			}

			auto value_3 = RealWorld_IsValidCSharpName_MemberData();
			for (StructRealWorld member_data : value_3)
			{
				RealWorld_IsValidCSharpName(member_data.value, member_data.isExpectedMatch);
			}

			auto value_4 = RealWorld_IsCommentLine_MemberData();
			for (StructRealWorld member_data : value_4)
			{
				RealWorld_IsCommentLine(member_data.value, member_data.isExpectedMatch);
			}

			auto value_5 = RealWorld_IsSectionLine_MemberData();
			for (StructRealWorld member_data : value_5)
			{
				RealWorld_IsSectionLine(member_data.value, member_data.isExpectedMatch);
			}
			auto value_6 = RealWorld_ValueParse_MemberData();
			for (StructMemberData member_data : value_6)
			{
				RealWorld_ValueParse(member_data.url, member_data.expected);
			}
			auto value_7 = RealWorld_FirebirdVersionString_MemberData();
			for (StructMemberData member_data : value_7)
			{
				RealWorld_FirebirdVersionString(member_data.url, member_data.expected);
			}

			auto value_8 = RealWorld_ExternalEntryPoint_MemberData();
			for (StructExternalEntryPoin member_data : value_8)
			{
				RealWorld_ExternalEntryPoint(member_data.value, member_data.a, member_data.b, member_data.c);
			}


			//DifficultForBacktracking(L"((?:0*)+?(?:.*)+?)?", L"0a", 2);//todo slow
			//DifficultForBacktracking(L"(?:(?:0?)+?(?:a?)+?)?", L"0a", 2);
			//DifficultForBacktracking(L"(?i:(\\()((?<a>\\w+(\\.\\w+)*)(,(?<a>\\w+(\\.\\w+)*)*)?)(\\)))", L"some.text(this.is,the.match)", 1);


			if (test_throw)TerminationInNonBacktrackingVsBackTracking(RegexOptions::None);

			auto value_9 = RecreationalRegex_Rectangle_MemberData();
			for (size_t i = 0; i < value_9.size(); ++i)
			{
				StructSAD struct_sad = value_9.at(i);
				//RecreationalRegex_Rectangle(struct_sad.input, struct_sad.expectedMatch);// todo slow
			}
		}

		static void Docs_Examples_ScanningHrefs()
		{
			const std::wstring HrefPattern = L"href\\s*=\\s*(?:[\"'](?<1>[^\"']*)[\"']|(?<1>\\S+))";

			const std::wstring InputString = L"My favorite web sites include:</P><A HREF=\"http://msdn2.microsoft.com\">MSDN Home Page</A></P><A HREF=\"http://www.microsoft.com\">Microsoft Corporation Home Page</A></P><A HREF=\"http://blogs.msdn.com/bclteam\">.NET Base Class Library blog</A></P>";

			Regex r = Regex(HrefPattern, RegexOptions::IgnoreCase);

			Match* m = r.Match(&InputString);
			Assert::True(m->Success);
			Assert::True(L"http://msdn2.microsoft.com" == m->Groups->operator[](1)->ToString());
			Assert::True(43 == m->Groups->operator[](1)->getIndex());

			m = m->NextMatch();
			Assert::True(m->Success);
			Assert::True(L"http://www.microsoft.com" == m->Groups->operator[](1)->ToString());
			Assert::True(102 == m->Groups->operator[](1)->getIndex());

			m = m->NextMatch();
			Assert::True(m->Success);
			Assert::True(L"http://blogs.msdn.com/bclteam" == m->Groups->operator[](1)->ToString());
			Assert::True(176 == m->Groups->operator[](1)->getIndex());

			m = m->NextMatch();
			Assert::False(m->Success);
		}

		static void Docs_Examples_MDYtoDMY()
		{
			Regex r = Regex(L"\\b(?<month>\\d{1,2})/(?<day>\\d{1,2})/(?<year>\\d{2,4})\\b");

			std::wstring dt = L"01/08/2020";
			Assert::True(L"08-01-2020" == r.Replace(dt, L"${day}-${month}-${year}"));
		}

		static void voidDocs_Examples_ExtractProtocolPort()
		{
			Regex r = Regex(L"^(?<proto>\\w+)://[^/]+?(?<port>:\\d+)?/");

			wstring text_1 = L"http://www.contoso.com:8080/letters/readme.html";
			Match* m = r.Match(&text_1);
			Assert::True(m->Success);
			Assert::True(L"http:8080" == m->Result(L"${proto}${port}"));
		}

		static std::vector<Data1> Docs_Examples_ValidateEmail_TestData()
		{
			return std::vector<Data1>{
				Data1(L"david.jones@proseware.com", true),
				Data1(L"d.j@server1.proseware.com", true),
				Data1(L"jones@ms1.proseware.com", true),
				Data1(L"j.@server1.proseware.com", false),
				Data1(L"j@proseware.com9", true),
				Data1(L"js#internal@proseware.com", true),
				Data1(L"j_9@[129.126.118.1]", true),
				Data1(L"j..s@proseware.com", false),
				Data1(L"js*@proseware.com", false),
				Data1(L"js@proseware..com", false),
				Data1(L"js@proseware.com9", true),
				Data1(L"j.s@server1.proseware.com", true),
				Data1(L"\"j\\\"s\\\"\"@proseware.com", true),
				Data1(L"js@contoso." + UtilsString::StringInitSpe(0x4E2D) + UtilsString::StringInitSpe(0x56FD), false)
			};
		}

		static void Docs_Examples_ValidateEmail(std::wstring email, bool expectedIsValid)
		{
			try
			{
				Regex r = Regex(L"(@)(.+)$", RegexOptions::None, TimeSpan::FromMilliseconds(200));
				// not support class
			}
			catch (RegexMatchTimeoutException)
			{
				Assert::True(expectedIsValid == false);
			}
			catch (ArgumentException)
			{
				Assert::True(expectedIsValid == false);
			}

			try
			{
				Regex r = Regex(
					L"^(?(\")(\".+?(?<!\\\\)\"@)|(([0-9a-z]((\\.(?!\\.))|[-!#\\$%&'\\*\\+/=\\?\\^`\\{\\}\\|~\\w])*)(?<=[0-9a-z])@))(?(\\[)(\\[(\\d{1,3}\\.){3}\\d{1,3}\\])|(([0-9a-z][-0-9a-z]*[0-9a-z]*\\.)+[a-z0-9][\\-a-z0-9]{0,22}[a-z0-9]))$",
					RegexOptions::IgnoreCase, TimeSpan::FromMilliseconds(250));

				bool value_result = r.IsMatch(email);
				Assert::True(expectedIsValid == value_result);
			}
			catch (RegexMatchTimeoutException)
			{
				Assert::True(expectedIsValid == false);
			}
		}

		static void Docs_GroupingConstructs_MatchedSubexpression()
		{
			const std::wstring Pattern = L"(\\w+)\\s(\\1)";
			const std::wstring Input = L"He said that that was the the correct answer.";

			Regex r = Regex(Pattern, RegexOptions::IgnoreCase);

			Match* match = r.Match(&Input);

			Assert::True(match->Success);
			Assert::True(L"that" == match->Groups->operator[](1)->getValue());
			Assert::True(8 == match->Groups->operator[](1)->getIndex());
			Assert::True(13 == match->Groups->operator[](2)->getIndex());

			match = match->NextMatch();
			Assert::True(match->Success);
			Assert::True(L"the" == match->Groups->operator[](1)->getValue());
			Assert::True(22 == match->Groups->operator[](1)->getIndex());
			Assert::True(26 == match->Groups->operator[](2)->getIndex());

			Assert::False(match->NextMatch()->Success);
		}

		static void Docs_GroupingConstructs_NamedMatchedSubexpression1()
		{
			const std::wstring Pattern = L"(?<duplicateWord>\\w+)\\s\\k<duplicateWord>\\W(?<nextWord>\\w+)";
			const std::wstring Input = L"He said that that was the the correct answer.";

			Regex r = Regex(Pattern, RegexOptions::IgnoreCase);

			Match* match = r.Match(&Input);

			Assert::True(match->Success);
			Assert::True(L"that" == match->Groups->operator[](L"duplicateWord")->getValue());
			Assert::True(8 == match->Groups->operator[](L"duplicateWord")->Index);
			Assert::True(L"was" == match->Groups->operator[](L"nextWord")->Value);

			match = match->NextMatch();
			Assert::True(match->Success);
			Assert::True(L"the" == match->Groups->operator[](L"duplicateWord")->Value);
			Assert::True(22 == match->Groups->operator[](L"duplicateWord")->Index);
			Assert::True(L"correct" == match->Groups->operator[](L"nextWord")->Value);

			Assert::False(match->NextMatch()->Success);
		}

		static void Docs_GroupingConstructs_NamedMatchedSubexpression2()
		{
			const wstring Pattern = L"\\D+(?<digit>\\d+)\\D+(?<digit>\\d+)?";
			std::vector<wstring> inputs = {L"abc123def456", L"abc123def"};

			Regex r = Regex(Pattern);


			StringBuilder actual = StringBuilder();
			for (wstring input : inputs)
			{
				Match* m = r.Match(&input);
				if (m->Success)
				{
					actual.AppendLine(L"Match: " + m->Value);
					for (int grpCtr = 1; grpCtr < m->Groups->Count; grpCtr++)
					{
						Group* grp = m->Groups->operator[](grpCtr);
						wstring temp = L"Group " + std::to_wstring(grpCtr) + L": " + grp->Value;
						actual.AppendLine(temp);
						for (int capCtr = 0; capCtr < grp->Captures->Count; capCtr++)
						{
							temp = L"   Capture " + std::to_wstring(capCtr) + L": " + grp->Captures->operator[](capCtr)->Value;
							actual.AppendLine(temp);
						}
					}
				}
			}


			wstring expected = L"Match: abc123def456\r\nGroup 1: 456\r\n   Capture 0: 123\r\n   Capture 1: 456\r\nMatch: abc123def\r\nGroup 1: 123\r\n   Capture 0: 123\r\n";
			auto compare = actual.ToString();
			Assert::True(expected == compare);
		}

		static void Docs_GroupingConstructs_BalancingGroups()
		{
			const wstring Pattern = L"^[^<>]*(((?'Open'<)[^<>]*)+((?'Close-Open'>)[^<>]*)+)*(?(Open)(?!))$";
			const wstring Input = L"<abc><mno<xyz>>";

			Regex r = Regex(Pattern);

			StringBuilder actual;
			Match* m = r.Match(&Input);
			if (m->Success)
			{
				actual.AppendLine(L"Input: \"" + Input + L"\"");
				actual.AppendLine(L"Match: \"" + m->ToString() + L"\"");
				int grpCtr = 0;
				for (int i = 0; i < m->getGroups()->getCount(); ++i)
				{
					Group* grp = m->getGroups()->operator[](i);
					wstring temp1 = L"   Group ";
					temp1 += std::to_wstring(grpCtr) + L": " + grp->Value;
					actual.AppendLine(temp1);
					grpCtr++;
					int capCtr = 0;

					for (int j = 0; j < grp->Captures->Count; ++j)
					{
						Capture* cap = grp->Captures->operator[](j);
						wstring temp2 = L"      Capture ";
						temp2 += std::to_wstring(capCtr) + L": " + cap->Value;
						actual.AppendLine(temp2);
						capCtr++;
					}
				}
			}

			wstring expected =
				L"Input: \"<abc><mno<xyz>>\"" + Environment::NewLine +
				L"Match: \"<abc><mno<xyz>>\"" + Environment::NewLine +
				L"   Group 0: <abc><mno<xyz>>" + Environment::NewLine +
				L"      Capture 0: <abc><mno<xyz>>" + Environment::NewLine +
				L"   Group 1: <mno<xyz>>" + Environment::NewLine +
				L"      Capture 0: <abc>" + Environment::NewLine +
				L"      Capture 1: <mno<xyz>>" + Environment::NewLine +
				L"   Group 2: <xyz" + Environment::NewLine +
				L"      Capture 0: <abc" + Environment::NewLine +
				L"      Capture 1: <mno" + Environment::NewLine +
				L"      Capture 2: <xyz" + Environment::NewLine +
				L"   Group 3: >" + Environment::NewLine +
				L"      Capture 0: >" + Environment::NewLine +
				L"      Capture 1: >" + Environment::NewLine +
				L"      Capture 2: >" + Environment::NewLine +
				L"   Group 4: " + Environment::NewLine +
				L"   Group 5: mno<xyz>" + Environment::NewLine +
				L"      Capture 0: abc" + Environment::NewLine +
				L"      Capture 1: xyz" + Environment::NewLine +
				L"      Capture 2: mno<xyz>" + Environment::NewLine;

			Assert::True(expected == actual.ToString());
		}

		static void Docs_GroupingConstructs_NoncapturingGroups()
		{
			const wstring Pattern = L"(?:\\b(?:\\w+)\\W*)+\\.";
			const wstring Input = L"This is a short sentence.";

			Regex r = Regex(Pattern);

			Match* match = r.Match(&Input);
			Assert::True(match->Success);
			Assert::True(L"This is a short sentence." == match->Value);
			Assert::True(1 == match->Groups->Count);
		}

		static void Docs_GroupingConstructs_GroupOptions()
		{
			const wstring Pattern = L"\\b(?ix: d \\w+)\\s";
			const wstring Input = L"Dogs are decidedly good pets.";

			Regex r = Regex(Pattern);

			Match* match = r.Match(&Input);
			Assert::True(match->Success);
			Assert::True(L"Dogs " == match->Value);
			Assert::True(0 == match->Index);

			match = match->NextMatch();
			Assert::True(match->Success);
			Assert::True(L"decidedly " == match->Value);
			Assert::True(9 == match->Index);

			Assert::False(match->NextMatch()->Success);
		}

		static void Docs_GroupingConstructs_ZeroWidthPositiveLookaheadAssertions()
		{
			const wstring Pattern = L"\\b\\w+(?=\\sis\\b)";
			Match* match;

			Regex r = Regex(Pattern);

			wstring text_1 = L"The dog is a Malamute.";
			wstring text_2 = L"The island has beautiful birds.";
			wstring text_3 = L"The pitch missed home plate.";
			wstring text_4 = L"Sunday is a weekend day.";

			match = r.Match(&text_1);
			Assert::True(match->Success);
			Assert::True(L"dog" == match->Value);

			match = r.Match(&text_2);
			Assert::False(match->Success);

			match = r.Match(&text_3);
			Assert::False(match->Success);

			match = r.Match(&text_4);
			Assert::True(match->Success);
			Assert::True(L"Sunday" == match->Value);
		}

		static void Docs_GroupingConstructs_ZeroWidthNegativeLookaheadAssertions()
		{
			const wstring Pattern = L"\\b(?!un)\\w+\\b";
			const wstring Input = L"unite one unethical ethics use untie ultimate";

			Regex r = Regex(Pattern, RegexOptions::IgnoreCase);

			MatchCollection* matches = r.Matches(&Input);
			Assert::True(L"one" == matches->operator[](0)->Value);
			Assert::True(L"ethics" == matches->operator[](1)->Value);
			Assert::True(L"use" == matches->operator[](2)->Value);
			Assert::True(L"ultimate" == matches->operator[](3)->Value);
		}

		static void Docs_GroupingConstructs_ZeroWidthPositiveLookbehindAssertions()
		{
			const wstring Pattern = L"(?<=\\b20)\\d{2}\\b";
			const wstring Input = L"2010 1999 1861 2140 2009";

			Regex r = Regex(Pattern, RegexOptions::IgnoreCase);

			MatchCollection* matches = r.Matches(&Input);
			Assert::True(L"10" == matches->operator[](0)->Value);
			Assert::True(L"09" == matches->operator[](1)->Value);
		}

		static void Docs_GroupingConstructs_ZeroWidthNegativeLookbehindAssertions()
		{
			const wstring Pattern = L"(?<!(Saturday|Sunday) )\\b\\w+ \\d{1,2}, \\d{4}\\b";

			Regex r = Regex(Pattern);

			wstring text_1 = L"Monday February 1, 2010";
			wstring text_2 = L"Wednesday February 3, 2010";
			wstring text_3 = L"Monday, February 8, 2010";

			Assert::True(L"February 1, 2010" == r.Match(&text_1)->Value);
			Assert::True(L"February 3, 2010" == r.Match(&text_2)->Value);
			Assert::False(r.IsMatch(L"Saturday February 6, 2010"));
			Assert::False(r.IsMatch(L"Sunday February 7, 2010"));
			Assert::True(L"February 8, 2010" == r.Match(&text_3)->Value);
		}

		static void Docs_GroupingConstructs_NonbacktrackingSubexpressions()
		{
			Regex rBack = Regex(L"(\\w)\\1+.\\b");
			Regex rNoBack = Regex(L"(?>(\\w)\\1+).\\b");
			Match* back;
			Match* noback;
			wstring text_1 = L"cccd.";
			wstring text_2 = L"aaad";
			wstring text_3 = L"aaaa";

			back = rBack.Match(&text_1);
			noback = rNoBack.Match(&text_1);
			Assert::True(back->Success);
			Assert::True(noback->Success);
			Assert::True(L"cccd" == back->Value);
			Assert::True(L"cccd" == noback->Value);

			back = rBack.Match(&text_2);
			noback = rNoBack.Match(&text_2);
			Assert::True(back->Success);
			Assert::True(noback->Success);
			Assert::True(L"aaad" == back->Value);
			Assert::True(L"aaad" == noback->Value);

			back = rBack.Match(&text_3);
			noback = rNoBack.Match(&text_3);
			Assert::True(back->Success);
			Assert::False(noback->Success);
			Assert::True(L"aaaa" == back->Value);
		}

		static void Docs_GroupingConstructs_GroupCaptureRelationship()
		{
			const wstring Pattern = L"(\\b(\\w+)\\W+)+";
			const wstring Input = L"This is a short sentence.";

			Regex r = Regex(Pattern);

			Match* match = r.Match(&Input);

			StringBuilder actual = StringBuilder();
			wstring temp1 = L"Match: '";
			temp1 += match->Value;
			temp1 += L"'";

			actual.AppendLine(temp1);
			for (int ctr = 1; ctr < match->Groups->Count; ctr++)
			{
				temp1 = L"   Group ";
				temp1 += std::to_wstring(ctr);
				temp1 += L": '";
				temp1 += match->Groups->operator[](ctr)->Value;
				temp1 += L"'";
				actual.AppendLine(temp1);
				int capCtr = 0;

				for (int j = 0; j < match->Groups->operator[](ctr)->Captures->getCount(); ++j)
				{
					auto capture = match->Groups->operator[](ctr)->Captures->operator[](j);

					wstring temp2 = L"      Capture ";
					temp2 += std::to_wstring(capCtr);
					temp2 += L": '";
					temp2 += capture->Value;
					temp2 += L"'";
					actual.AppendLine(temp2);
					capCtr++;
				}
			}

			wstring expected =
				L"Match: 'This is a short sentence.'" + Environment::NewLine +
				L"   Group 1: 'sentence.'" + Environment::NewLine +
				L"      Capture 0: 'This '" + Environment::NewLine +
				L"      Capture 1: 'is '" + Environment::NewLine +
				L"      Capture 2: 'a '" + Environment::NewLine +
				L"      Capture 3: 'short '" + Environment::NewLine +
				L"      Capture 4: 'sentence.'" + Environment::NewLine +
				L"   Group 2: 'sentence'" + Environment::NewLine +
				L"      Capture 0: 'This'" + Environment::NewLine +
				L"      Capture 1: 'is'" + Environment::NewLine +
				L"      Capture 2: 'a'" + Environment::NewLine +
				L"      Capture 3: 'short'" + Environment::NewLine +
				L"      Capture 4: 'sentence'" + Environment::NewLine;

			Assert::True(expected == actual.ToString());
		}

		static void Docs_Capture_Sentences()
		{
			const wstring Pattern = L"((\\w+)[\\s.])+";
			const wstring Input = L"Yes. This dog is very friendly.";

			Regex r = Regex(Pattern);

			StringBuilder actual = StringBuilder();

			for (int i = 0; i < r.Matches(&Input)->Count; ++i)
			{
				Match* match = r.Matches(&Input)->operator[](i);

				actual.AppendLine(L"Match: " + match->Value);
				for (int groupCtr = 0; groupCtr < match->Groups->Count; groupCtr++)
				{
					Group* group = match->Groups->operator[](groupCtr);
					wstring temp = L"   Group " + std::to_wstring(groupCtr) + L": " + group->Value;
					actual.AppendLine(temp);
					for (int captureCtr = 0; captureCtr < group->Captures->Count; captureCtr++)
					{
						wstring temp2 = L"      Capture ";
						temp2 += std::to_wstring(captureCtr);
						temp2 += L": ";
						temp2 += group->Captures->operator[](captureCtr)->Value;

						actual.AppendLine(temp2);
					}
				}
			}


			wstring expected =
				L"Match: Yes." + Environment::NewLine +
				L"   Group 0: Yes." + Environment::NewLine +
				L"      Capture 0: Yes." + Environment::NewLine +
				L"   Group 1: Yes." + Environment::NewLine +
				L"      Capture 0: Yes." + Environment::NewLine +
				L"   Group 2: Yes" + Environment::NewLine +
				L"      Capture 0: Yes" + Environment::NewLine +
				L"Match: This dog is very friendly." + Environment::NewLine +
				L"   Group 0: This dog is very friendly." + Environment::NewLine +
				L"      Capture 0: This dog is very friendly." + Environment::NewLine +
				L"   Group 1: friendly." + Environment::NewLine +
				L"      Capture 0: This " + Environment::NewLine +
				L"      Capture 1: dog " + Environment::NewLine +
				L"      Capture 2: is " + Environment::NewLine +
				L"      Capture 3: very " + Environment::NewLine +
				L"      Capture 4: friendly." + Environment::NewLine +
				L"   Group 2: friendly" + Environment::NewLine +
				L"      Capture 0: This" + Environment::NewLine +
				L"      Capture 1: dog" + Environment::NewLine +
				L"      Capture 2: is" + Environment::NewLine +
				L"      Capture 3: very" + Environment::NewLine +
				L"      Capture 4: friendly" + Environment::NewLine;

			Assert::True(expected == actual.ToString());
		}

		static void Docs_Capture_ProductNumber()
		{
			const wstring Pattern = L"^([a-z]+)(\\d+)?\\.([a-z]+(\\d)*)$";
			std::vector<wstring> values = {L"AC10", L"Za203.CYM", L"XYZ.CoA", L"ABC.x170"};

			Regex r = Regex(Pattern, RegexOptions::IgnoreCase);

			StringBuilder actual = StringBuilder();

			//for (int i = 0; i < values.size(); ++i)
			//{
			//	auto value = values.at(i);
			//	Match* m = r.Match(value);
			//	if (m->Success)
			//	{
			//		actual.AppendLine(L"Match: '"+ m->Value +L"'");
			//		actual.AppendLine(L"   Number of Capturing Groups: "+ m->Groups->Count);
			//		for (int gCtr = 0; gCtr < m->Groups->Count; gCtr++)
			//		{
			//			Group* group = m->Groups->operator[](gCtr);
			//			actual.AppendLine(L"      Group "+ gCtr +": {(group.Value == "" ? "<empty>" : "'" + group.Value + "'")}");
			//			actual.AppendLine($"         Number of Captures: {group.Captures.Count}");
			//			for (int cCtr = 0; cCtr < group->Captures->Count; cCtr++)
			//			{
			//				actual.AppendLine($"            Capture {cCtr}: {group.Captures[cCtr].Value}");
			//			}
			//		}
			//	}
			//	else
			//	{
			//		actual.AppendLine($"No match for {value}: Match.Value is {(m.Value == String.Empty ? "<empty>" : m.Value)}");
			//	}
			//}


			//wstring expected;
			//expected  =
			//	L"No match for AC10: Match.Value is <empty>" + Environment::NewLine +
			//	L"Match: 'Za203.CYM'" + Environment::NewLine +
			//	L"   Number of Capturing Groups: 5" + Environment::NewLine +
			//	L"      Group 0: 'Za203.CYM'" + Environment::NewLine +
			//	L"         Number of Captures: 1" + Environment::NewLine +
			//	L"            Capture 0: Za203.CYM" + Environment::NewLine +
			//	L"      Group 1: 'Za'" + Environment::NewLine +
			//	L"         Number of Captures: 1" + Environment::NewLine +
			//	L"            Capture 0: Za" + Environment::NewLine +
			//	L"      Group 2: '203'" + Environment::NewLine +
			//	L"         Number of Captures: 1" + Environment::NewLine +
			//	L"            Capture 0: 203" + Environment::NewLine +
			//	L"      Group 3: 'CYM'" + Environment::NewLine +
			//	L"         Number of Captures: 1" + Environment::NewLine +
			//	L"            Capture 0: CYM" + Environment::NewLine +
			//	L"      Group 4: <empty>" + Environment::NewLine +
			//	L"         Number of Captures: 0" + Environment::NewLine +
			//	L"Match: 'XYZ.CoA'" + Environment::NewLine +
			//	L"   Number of Capturing Groups: 5" + Environment::NewLine +
			//	L"      Group 0: 'XYZ.CoA'" + Environment::NewLine +
			//	L"         Number of Captures: 1" + Environment::NewLine +
			//	L"            Capture 0: XYZ.CoA" + Environment::NewLine +
			//	L"      Group 1: 'XYZ'" + Environment::NewLine +
			//	L"         Number of Captures: 1" + Environment::NewLine +
			//	L"            Capture 0: XYZ" + Environment::NewLine +
			//	L"      Group 2: <empty>" + Environment::NewLine +
			//	L"         Number of Captures: 0" + Environment::NewLine +
			//	L"      Group 3: 'CoA'" + Environment::NewLine +
			//	L"         Number of Captures: 1" + Environment::NewLine +
			//	L"            Capture 0: CoA" + Environment::NewLine +
			//	L"      Group 4: <empty>" + Environment::NewLine +
			//	L"         Number of Captures: 0" + Environment::NewLine +
			//	L"Match: 'ABC.x170'" + Environment::NewLine +
			//	L"   Number of Capturing Groups: 5" + Environment::NewLine +
			//	L"      Group 0: 'ABC.x170'" + Environment::NewLine +
			//	L"         Number of Captures: 1" + Environment::NewLine +
			//	L"            Capture 0: ABC.x170" + Environment::NewLine +
			//	L"      Group 1: 'ABC'" + Environment::NewLine +
			//	L"         Number of Captures: 1" + Environment::NewLine +
			//	L"            Capture 0: ABC" + Environment::NewLine +
			//	L"      Group 2: <empty>" + Environment::NewLine +
			//	L"         Number of Captures: 0" + Environment::NewLine +
			//	L"      Group 3: 'x170'" + Environment::NewLine +
			//	L"         Number of Captures: 1" + Environment::NewLine +
			//	L"            Capture 0: x170" + Environment::NewLine +
			//	L"      Group 4: '0'" + Environment::NewLine +
			//	L"         Number of Captures: 3" + Environment::NewLine +
			//	L"            Capture 0: 1" + Environment::NewLine +
			//	L"            Capture 1: 7" + Environment::NewLine +
			//	L"            Capture 2: 0" + Environment::NewLine;

			//Assert::True(expected== actual.ToString());
		}

		static void Docs_Backtracking_LinearComparisonWithoutBacktracking()
		{
			const wstring Pattern = L"e{2}\\w\\b";
			const wstring Input = L"needing a reed";

			Regex r = Regex(Pattern);

			MatchCollection* matches = r.Matches(&Input);
			Assert::True(1 == matches->Count);
			Assert::True(L"eed" == matches->operator[](0)->Value);
			Assert::True(11 == matches->operator[](0)->Index);
		}

		static void Docs_Backtracking_WithOptionalQuantifiersOrAlternationConstructs()
		{
			const wstring Pattern = L".*(es)";
			const wstring Input = L"Essential services are provided by regular expressions.";

			Regex r = Regex(Pattern, RegexOptions::IgnoreCase);

			Match* m = r.Match(&Input);
			Assert::True(m->Success);
			Assert::True(L"Essential services are provided by regular expres" == m->Value);
			Assert::True(0 == m->Index);
			Assert::True(47 == m->Groups->operator[](1)->Index);

			Assert::False(m->NextMatch()->Success);
		}

		static void Docs_Backtracking_WithNestedOptionalQuantifiers_BacktrackingEliminated()
		{
			const wstring Input = L"b51:4:1DB:9EE1:5:27d60:f44:D4:cd:E:5:0A5:4a:D24:41Ad:";
			Regex r = Regex(L"^((?>[0-9a-fA-F]{1,4}:)*(?>[0-9a-fA-F]{1,4}))*(::)$"); // Using atomic to avoid backtracking
			Assert::False(r.IsMatch(Input));
		}

		static void Docs_Backtracking_LookbehindAssertions()
		{
			const wstring Input = L"test@contoso.com";
			Regex rPattern = Regex(L"^[0-9A-Z]([-.\\w]*[0-9A-Z])?@", RegexOptions::IgnoreCase);
			Assert::True(rPattern.IsMatch(Input));

			Regex rBehindPattern = Regex(L"^[0-9A-Z][-.\\w]*(?<=[0-9A-Z])@", RegexOptions::IgnoreCase);
			Assert::True(rBehindPattern.IsMatch(Input));
		}

		static void Docs_Backtracking_LookaheadAssertions_BacktrackingEliminated()
		{
			Regex r = Regex(L"^((?=[A-Z])\\w+\\.)*[A-Z]\\w*$", RegexOptions::IgnoreCase);
			Assert::False(r.IsMatch(L"aaaaaaaaaaaaaaaaaaaaaa."));
		}

		static void Docs_EngineCapabilities_LazyQuantifiers()
		{
			const wstring Input = L"This sentence ends with the number 107325.";

			Regex rGreedy = Regex(L".+(\\d+)\\.");
			Match* match = rGreedy.Match(&Input);
			Assert::True(match->Success);
			Assert::True(L"5" == match->Groups->operator[](1)->Value);

			Regex rLazy = Regex(L".+?(\\d+)\\.");
			match = rLazy.Match(&Input);
			Assert::True(match->Success);
			Assert::True(L"107325" == match->Groups->operator[](1)->Value);
		}

		static void Docs_EngineCapabilities_PositiveLookahead()
		{
			const wstring Pattern = L"\\b[A-Z]+\\b(?=\\P{P})";
			const wstring Input = L"If so, what comes next?";

			Regex r = Regex(Pattern, RegexOptions::IgnoreCase);

			MatchCollection* matches = r.Matches(&Input);
			Assert::True(3 == matches->Count);
			Assert::True(L"If" == matches->operator[](0)->Value);
			Assert::True(L"what" == matches->operator[](1)->Value);
			Assert::True(L"comes" == matches->operator[](2)->Value);
		}

		static void Docs_EngineCapabilities_NegativeLookahead()
		{
			const wstring Pattern = L"\\b(?!non)\\w+\\b";
			const wstring Input = L"Nonsense is not always non-functional.";
			Regex r = Regex(Pattern, RegexOptions::IgnoreCase);

			MatchCollection* matches = r.Matches(&Input);
			Assert::True(4 == matches->Count);
			Assert::True(L"is" == matches->operator[](0)->Value);
			Assert::True(L"not" == matches->operator[](1)->Value);
			Assert::True(L"always" == matches->operator[](2)->Value);
			Assert::True(L"functional" == matches->operator[](3)->Value);
		}

		static void Docs_EngineCapabilities_ConditionalEvaluation()
		{
			const wstring Pattern = L"\\b(?(\\d{2}-)\\d{2}-\\d{7}|\\d{3}-\\d{2}-\\d{4})\\b";
			const wstring Input = L"01-9999999 020-333333 777-88-9999";

			Regex r = Regex(Pattern);

			MatchCollection* matches = r.Matches(&Input);
			Assert::True(2 == matches->Count);

			Assert::True(L"01-9999999" == matches->operator[](0)->Value);
			Assert::True(0 == matches->operator[](0)->Index);

			Assert::True(L"777-88-9999" == matches->operator[](1)->Value);
			Assert::True(22 == matches->operator[](1)->Index);
		}

		static void Docs_EngineCapabilities_RightToLeftMatching()
		{
			const wstring GreedyPattern = L".+(\\d+)\\.";
			const wstring Input = L"This sentence ends with the number 107325.";

			Regex rLTR = Regex(GreedyPattern);
			Regex rRTL = Regex(GreedyPattern, RegexOptions::RightToLeft);

			// Match from left-to-right using lazy quantifier .+?.
			Match* match = rLTR.Match(&Input);
			Assert::True(match->Success);
			Assert::True(L"5" == match->Groups->operator[](1)->Value);

			// Match from right-to-left using greedy quantifier .+.
			match = rRTL.Match(&Input);
			Assert::True(match->Success);
			Assert::True(L"107325" == match->Groups->operator[](1)->Value);
		}

		static void Docs_EngineCapabilities_PositiveNegativeLookbehind()
		{
			const wstring Pattern = L"^[A-Z0-9]([-!#$%&'.*+/=?^`{}|~\\w])*(?<=[A-Z0-9])$";

			Regex r = Regex(Pattern, RegexOptions::IgnoreCase);

			Assert::True(r.IsMatch(L"jack.sprat"));
			Assert::False(r.IsMatch(L"dog#"));
			Assert::True(r.IsMatch(L"dog#1"));
			Assert::True(r.IsMatch(L"me.myself"));
			Assert::False(r.IsMatch(L"me.myself!"));
		}

		static void Docs_InlineOptions()
		{
			const wstring Input = L"double dare double Double a Drooling dog The Dreaded Deep";

			StringBuilder actual = StringBuilder();

			Regex r1 = Regex(L"\\b(D\\w+)\\s(d\\w+)\\b");
			auto maths1 = r1.Matches(&Input);
			for (int i = 0; i < maths1->Count; ++i)
			{
				Match* match = maths1->operator[](i);
				actual.AppendLine(match->Value);
				if (match->Groups->Count > 1)
				{
					for (int ctr = 1; ctr < match->Groups->Count; ctr++)
					{
						wstring text = L"   Group ";
						text += std::to_wstring(ctr);
						text += L": ";
						text += match->Groups->operator[](ctr)->Value;

						actual.AppendLine(text);
					}
				}
			}

			actual.AppendLine();

			Regex r2 = Regex(L"\\b(D\\w+)(?ixn) \\s (d\\w+) \\b");
			auto maths2 = r2.Matches(&Input);

			for (int i = 0; i < maths2->Count; ++i)
			{
				Match* match = maths2->operator[](i);
				actual.AppendLine(match->Value);
				if (match->Groups->Count > 1)
				{
					for (int ctr = 1; ctr < match->Groups->Count; ctr++)
					{
						wstring text = L"   Group ";
						text += std::to_wstring(ctr);
						text += L": '";
						text += match->Groups->operator[](ctr)->Value;
						text += L"'";

						actual.AppendLine(text);
					}
				}
			}

			wstring expected =
				L"Drooling dog" + Environment::NewLine +
				L"   Group 1: Drooling" + Environment::NewLine +
				L"   Group 2: dog" + Environment::NewLine +
				Environment::NewLine +
				L"Drooling dog" + Environment::NewLine +
				L"   Group 1: 'Drooling'" + Environment::NewLine +
				L"Dreaded Deep" + Environment::NewLine +
				L"   Group 1: 'Dreaded'" + Environment::NewLine;

			Assert::True(expected == actual.ToString());
		}

		static void Docs_InlineComment()
		{
			const wstring Pattern = L"\\b((?# case-sensitive comparison)D\\w+)\\s(?ixn)((?#case-insensitive comparison)d\\w+)\\b";
			const wstring Input = L"double dare double Double a Drooling dog The Dreaded Deep";

			Regex r = Regex(Pattern);

			Match* match = r.Match(&Input);
			Assert::True(match->Success);
			Assert::True(L"Drooling dog" == match->Value);
			Assert::True(2 == match->Groups->Count);
			Assert::True(L"Drooling" == match->Groups->operator[](1)->Value);

			match = match->NextMatch();
			Assert::True(match->Success);
			Assert::True(L"Dreaded Deep" == match->Value);
			Assert::True(2 == match->Groups->Count);
			Assert::True(L"Dreaded" == match->Groups->operator[](1)->Value);

			Assert::False(match->NextMatch()->Success);
		}

		static void Docs_EndOfLineComment()
		{
			const wstring Pattern = L"\\{\\d+(,-*\\d+)*(\\:\\w{1,4}?)*\\}(?x) # Looks for a composite format item.";
			const wstring Input = L"{0,-3:F}";

			Regex r = Regex(Pattern);

			Assert::True(r.IsMatch(Input));
		}

		static void Docs_Anchors_ContiguousMatches()
		{
			const wstring Input = L"capybara,squirrel,chipmunk,porcupine";
			const wstring Pattern = L"\\G(\\w+\\s?\\w*),?";
			std::vector<wstring> expected = {L"capybara", L"squirrel", L"chipmunk", L"porcupine"};

			Regex r = Regex(Pattern);

			Match* m = r.Match(&Input);

			std::vector<wstring> actual(4);
			for (int i = 0; i < 4; i++)
			{
				Assert::True(m->Success);
				actual[i] = m->Groups->operator[](1)->Value;
				m = m->NextMatch();
			}
			Assert::False(m->Success);
			Assert::True(expected == actual);
		}

		static void Blog_Levithan_BalancingGroups_Palindromes()
		{
			Regex r = Regex(L"(?<N>.)+.?(?<-N>\\k<N>)+(?(N)(?!))");

			// Palindromes
			std::vector<wstring> expected = {L"kayak", L"racecar", L"never odd or even", L"madam im adam"};
			for (size_t i = 0; i < expected.size(); i++)
			{
				Assert::True(r.IsMatch(expected[i]));
			}

			// Non-Palindromes
			Assert::False(r.IsMatch(L"canoe"));
			Assert::False(r.IsMatch(L"raceboat"));
		}

		static void Blog_Levithan_BalancingGroups_MatchingParentheses()
		{
			Regex r = Regex(L"^\\((?>[^()]+|\\( (?<Depth>)|\\)     (?<-Depth>))*(?(Depth)(?!))\\)$", RegexOptions::IgnorePatternWhitespace);

			Assert::True(r.IsMatch(L"()"));
			Assert::True(r.IsMatch(L"(a(b c(de(f(g)hijkl))mn))"));

			Assert::False(r.IsMatch(L"("));
			Assert::False(r.IsMatch(L")"));
			Assert::False(r.IsMatch(L"())"));
			Assert::False(r.IsMatch(L"(()"));
			Assert::False(r.IsMatch(L"(ab(cd)ef"));
		}

		static void Blog_Levithan_BalancingGroups_WordLengthIncreases()
		{
			Regex r = Regex(L"^(?:(?(A)\\s|)(?<B>)(?<C-B>\\w)+ (?(B)(?!))(?:\\s(?<C>)(?<B-C>\\w)+ (?(C)(?!))(?<A>))?)+ \\b$", RegexOptions::IgnorePatternWhitespace);

			Assert::True(r.IsMatch(L"a bc def ghij klmni"));
			Assert::False(r.IsMatch(L"a bc def ghi klmn"));
		}

		static vector<StructMemberData> RealWorld_ExtractResourceUri_MemberData()
		{
			return {
				StructMemberData(L"https://foo.com:443/bar/17/groups/0ad1/providers/Network/public/4e-ip?version=16", L"Network/public/4e-ip"),
				StructMemberData(L"ftp://443/notproviders/17/groups/0ad1/providers/Network/public/4e-ip?version=16", L"Network/public/4e-ip"),
				StructMemberData(L"ftp://443/providersnot/17/groups/0ad1/providers/Network/public/4e-ip?version=16", L"Network/public/4e-ip"),
			};
		}

		static void RealWorld_ExtractResourceUri(wstring url, wstring expected)
		{
			Regex r = Regex(L"/providers/(.+?)\\?");
			Match* m = r.Match(&url);
			Assert::True(m->Success);
			Assert::True(2 == m->Groups->Count);
			Assert::True(expected == m->Groups->operator[](1)->Value);
		}

		static vector<StructRealWorld> RealWorld_IsValidCSharpName_MemberData()
		{
			return {
				StructRealWorld(L"IsValidCSharpName", true),
				StructRealWorld(L"_IsValidCSharpName", true),
				StructRealWorld(L"__", true),
				StructRealWorld(L"a\u2169", true), // \u2169 is in {Nl}
				StructRealWorld(L"\u2169b", true), // \u2169 is in {Nl}
				StructRealWorld(L"a\u0600", true), // \u0600 is in {Cf}
				StructRealWorld(L"\u0600b", false), // \u0600 is in {Cf}
				StructRealWorld(L"a\u0300", true), // \u0300 is in {Mn}
				StructRealWorld(L"\u0300b", false), // \u0300 is in {Mn}
				StructRealWorld(L"https://foo.com:443/bar/17/groups/0ad1/providers/Network/public/4e-ip?version=16", false),
				StructRealWorld(L"david.jones@proseware.com", false),
				StructRealWorld(L"~david", false),
				StructRealWorld(L"david~", false),
			};
		}

		static void RealWorld_IsValidCSharpName(wstring value, bool isExpectedMatch)
		{
			const wstring StartCharacterRegex = L"_|[\\p{Lu}\\p{Ll}\\p{Lt}\\p{Lm}\\p{Lo}\\p{Nl}]";
			const wstring PartCharactersRegex = L"[\\p{Lu}\\p{Ll}\\p{Lt}\\p{Lm}\\p{Lo}\\p{Nl}\\p{Mn}\\p{Mc}\\p{Nd}\\p{Pc}\\p{Cf}]";
			const wstring IdentifierRegex = L"^(" + StartCharacterRegex + L")(" + PartCharactersRegex + L")*$";

			Regex r = Regex(IdentifierRegex);
			Assert::True(isExpectedMatch == r.IsMatch(value));
		}

		static vector<StructRealWorld> RealWorld_IsCommentLine_MemberData()
		{
			return {
				StructRealWorld(L"; this is a comment", true),
				StructRealWorld(L"\t; so is this", true),
				StructRealWorld(L"  ; and this", true),
				StructRealWorld(L";", true),
				StructRealWorld(L";comment\nNotThisBecauseOfNewLine", false),
				StructRealWorld(L"-;not a comment", false),
			};
		}

		static void RealWorld_IsCommentLine(wstring value, bool isExpectedMatch)
		{
			const wstring CommentLineRegex = L"^\\s*;\\s*(.*?)\\s*$";

			Regex r = Regex(CommentLineRegex);
			Assert::True(isExpectedMatch == r.IsMatch(value));
		}

		static vector<StructRealWorld> RealWorld_IsSectionLine_MemberData()
		{
			return {
				StructRealWorld(L"[ThisIsASection]", true),
				StructRealWorld(L" [ThisIsASection] ", true),
				StructRealWorld(L"\t[ThisIs\\ASection]\t", true),
				StructRealWorld(L"\t[This.Is:(A+Section)]\t", true),
				StructRealWorld(L"[This Is Not]", false),
				StructRealWorld(L"This is not[]", false),
				StructRealWorld(L"[Nor This]/", false),
			};
		}

		static void RealWorld_IsSectionLine(wstring value, bool isExpectedMatch)
		{
			const wstring SectionLineRegex = L"^\\s*\\[([\\w\\.\\-\\+:\\/\\(\\)\\\\]+)\\]\\s*$";

			Regex r = Regex(SectionLineRegex);
			Assert::True(isExpectedMatch == r.IsMatch(value));
		}

		static vector<StructMemberData> RealWorld_ValueParse_MemberData()
		{
			return {
				StructMemberData(L"Jiri: 10", L"10"),
				StructMemberData(L"jiri: -10.01", L"-10.01"),
				StructMemberData(L"jiri: .-22", L"-22"),
				StructMemberData(L"jiri: .-22.3", L"-22.3"),
				StructMemberData(L"foo15.0", L"15.0"),
				StructMemberData(L"foo15", L"15"),
				StructMemberData(L"foo16bar", L"16"),
				StructMemberData(L"fds:-4", L"-4"),
				StructMemberData(L"dsa:-20.04", L"-20.04"),
				StructMemberData(L"dsa:15.a", L"15"),
			};
		}

		static void RealWorld_ValueParse(wstring value, wstring expected)
		{
			Regex r = Regex(L"(?<value>-?\\d+(\\.\\d+)?)");
			Match* m = r.Match(&value);
			Assert::True(m->Success);
			Assert::True(expected == m->Groups->operator[](L"value")->Value);
		}

		static vector<StructMemberData> RealWorld_FirebirdVersionString_MemberData()
		{
			return {
				StructMemberData(L"WI-T4.0.0.1963 Firebird 4.0 Beta 2", L"4.0.0.1963"),
				StructMemberData(L"WI-V3.0.5.33220 Firebird 3.0", L"3.0.5.33220"),
			};
		}

		static void RealWorld_FirebirdVersionString(wstring value, wstring expected)
		{
			Regex r = Regex(L"\\w{2}-\\w(\\d+\\.\\d+\\.\\d+\\.\\d+)");
			Match* m = r.Match(&value);
			Assert::True(m->Success);
			Assert::True(expected == m->Groups->operator[](1)->Value);
		}

		static vector<StructExternalEntryPoin> RealWorld_ExternalEntryPoint_MemberData()
		{
			return {
				StructExternalEntryPoin(L"Foo!Bar.M", L"Foo", L"Bar", L"M"),
				StructExternalEntryPoin(L"Foo!Bar.A.B.C", L"Foo", L"Bar.A.B", L"C"),
				StructExternalEntryPoin(L"Foo1.Foo2.Foo!Bar.A.B.C", L"Foo1.Foo2.Foo", L"Bar.A.B", L"C"),
				StructExternalEntryPoin(L"Foo1\\Foo2.Foo!Bar.A.B.C", L"Foo1\\Foo2.Foo", L"Bar.A.B", L"C"),
			};
		}

		static void RealWorld_ExternalEntryPoint(wstring value, wstring a, wstring b, wstring c)
		{
			Regex r = Regex(L"^(.+)!(.+)\\.([^.]+)$");
			Match* m = r.Match(&value);
			Assert::True(m->Success);
			Assert::True(a == m->Groups->operator[](1)->Value);
			Assert::True(b == m->Groups->operator[](2)->Value);
			Assert::True(c == m->Groups->operator[](3)->Value);
		}

		static void DifficultForBacktracking(wstring pattern, wstring input, int matchcount)
		{
			Regex regex = Regex(pattern);
			vector<Match*> matches = vector<Match*>();
			auto match = regex.Match(&input);
			while (match->Success)
			{
				matches.push_back(match);
				match = match->NextMatch();
			}
			Assert::True(matchcount == matches.size());
		}

		static void TerminationInNonBacktrackingVsBackTracking(RegexOptions options)
		{
			wstring input = L" 123456789 123456789 123456789 123456789 123456789";
			for (int i = 0; i < 12; i++)
			{
				input += input;
			}

			// The input has 2^12 * 50 = 204800 characters
			wstring rawregex = L"[\\/]?[^\\/]*?(heythere|hej)[^\\/]*?$";

			// It takes over 4min with backtracking, so it should certainly timeout given a 1 second timeout
			Regex reC = Regex(rawregex, options, TimeSpan::FromSeconds(1));


			AssertThrows(RegexMatchTimeoutException, reC.Match(&input))


			//// NonBacktracking needs way less than 1s, but use 10s to account for the slowest possible CI machine
			//Regex re =   Regex(rawregex,  TimeSpan::FromSeconds(10));
			//Assert::False(re.Match(input).Success);
		}

		static vector<StructSAD> RecreationalRegex_Rectangle_MemberData()
		{
			return
			{
				StructSAD(L"\r\n1\r\n1\r\n1\r\n\r\n~~~\r\n", true),
				StructSAD(L"\r\n11\r\n11\r\n\r\n2\r\n2\r\n\r\n~~~\r\n~~~\r\n", true),
				StructSAD(L"\r\n11\r\n11\r\n\r\n22\r\n\r\n~~~\r\n~~~\r\n", true),
				StructSAD(L"\r\n11\r\n11\r\n\r\n2222\r\n\r\n33\r\n\r\n4\r\n\r\n5\r\n\r\n~~~~\r\n~~~~\r\n~~~~\r\n", true),
				StructSAD(L"\r\n11\r\n11\r\n11\r\n\r\n~~~\r\n~~~\r\n", true),
				StructSAD(L"\r\n111\r\n\r\n~\r\n~\r\n~\r\n", true),
				StructSAD(L"\r\n111\r\n\r\n~~~\r\n", true),
				StructSAD(L"\r\n111\r\n111\r\n\r\n~~~\r\n~~~\r\n", true),
				StructSAD(L"\r\n1111\r\n\r\n222\r\n222\r\n\r\n333333\r\n\r\n444\r\n444\r\n\r\n5555\r\n\r\n666666\r\n\r\n7777\r\n7777\r\n7777\r\n\r\n6\r\n\r\n9\r\n\r\n88\r\n88\r\n\r\naaaa\r\naaaa\r\n\r\n~~~~~~~~~~\r\n~~~~~~~~~~\r\n~~~~~~~~~~\r\n~~~~~~~~~~\r\n~~~~~~~~~~\r\n", true),
				StructSAD(L"\r\n1111\r\n\r\n2222\r\n\r\n3\r\n3\r\n3\r\n3\r\n\r\n4\r\n4\r\n4\r\n4\r\n\r\nxxx\r\nxxx\r\nxxx\r\n\r\n~~~~~\r\n~~~~~\r\n~~~~~\r\n~~~~~\r\n~~~~~\r\n", true),
				StructSAD(L"\r\n1111\r\n\r\n2222\r\n\r\n3333\r\n\r\n4444\r\n\r\nxxx\r\nxxx\r\nxxx\r\n\r\n~~~~~\r\n~~~~~\r\n~~~~~\r\n~~~~~\r\n~~~~~\r\n", true),
				StructSAD(L"\r\n2\r\n2\r\n\r\n11\r\n11\r\n\r\n~~\r\n~~\r\n", true),

				StructSAD(L"\r\n1\r\n\r\n~~\r\n", false),
				StructSAD(L"\r\n11\r\n11\r\n\r\n2222\r\n\r\n33\r\n\r\n4\r\n\r\n5\r\n\r\n~~~~\r\n~~~~\r\n~~~~\r\n~~~~\r\n", false),
				StructSAD(L"\r\n111\r\n\r\n~~\r\n", false),
				StructSAD(L"\r\n111\r\n\r\n~~~\r\n~~~\r\n", false),
				StructSAD(L"\r\n111\r\n\r\n222\r\n\r\n33\r\n\r\n~~~~\r\n", false),
			};
		}

		static void RecreationalRegex_Rectangle(wstring input, bool expectedMatch)
		{
			std::string generalString;
			generalString += "\n";
			generalString += "                \\A\n";
			generalString += "                (?=(?<NextPos>[^~]*))       # \\k<NextPos> always matches the position *before* the next free tilde.\n";
			generalString += "                (?:\n";
			generalString += "                    (?:\n";
			generalString += "                        (?<IndexLength>.)+?             # Match n characters. We will use rectangle number n in position <Index>.\n";
			generalString += "                                                        # 0 is not an option - we need some shape to be first, second, third, etc.\n";
			generalString += "                        (?<=\\A(?=(?<Index>(?<-IndexLength>.)+)).*)    # Capture into <Index> the first n characters.\n";
			generalString += "                        (?<=\\A(?<CurrentIndex>\\k<Index>).*)           # Copy Index into CurrentIndex\n";
			generalString += "                        (?=.*\\Z                                       # Ensure <Index> is unique. We cannot use the same rectangle twice\n";
			generalString += "                            (?<!(?=\\A\\k<Index>(?<=\\A\\k<CurrentIndex>))\\A.*(?<-Index>.)+) \n";
			generalString += "                        )\n";
			generalString += "                        (?(IndexLength)(?!)|)   # Not needed, just an assert.\n";
			generalString += "        \n";
			generalString += "                        #Copy the shape of rectangle <Index> to the target area.\n";
			generalString += "                        #Find rectangle number <Index>\n";
			generalString += "                        (?<=(?=.(?<IndexLength>.)*?(?<=\\A\\k<Index>))\\A.*)    # Populate <IndexLength> again.\n";
			generalString += "                             # ^-- we are skipping over one character. We want to reach rectangle number <IndexLength>,\n";
			generalString += "                             #     so we're skiping over <IndexLength>-1 rectangles\n";
			generalString += "                        (?<=(?=\\s*(?<-IndexLength>(?:\\w+\\r?\\n)+\\r?\\n)*(?<=(?<RectangleStart>.*))\\w)\\A.*) # Capture starting position of this rectangle.\n";
			generalString += "                        (?(IndexLength)(?!))\n";
			generalString += "                        (?:\n";
			generalString += "                            (?<Rotate>)     # Capture 0 characters into <Rotate>. Indicates that we are not rotating this rectangle.\n";
			generalString += "                            |\n";
			generalString += "                            (?<Rotate>.)    # Capture 1 character into <Rotate>. Indicates that we are rotating this rectangle.\n";
			generalString += "                            (?<TempRotate>) # Also mark <TempRotate>. This is a flag that is cleared for each rectangle. Allows conditional matching.\n";
			generalString += "                        )\n";
			generalString += "                        (?<=(?=\\k<NextPos>(?<=(?<X>~*)))\\A.*)   # Init <X> with the number of tildes to the left of the starting position.\n";
			generalString += "                        # Basically `(?:\\w+\\n)+\\n` to match the whole rectangle.\n";
			generalString += "                        (?<=(?=\\k<RectangleStart>   # Go to the position before the first character in the current rectangle.\n";
			generalString += "                            (?<Rectangle>           # Capture this rectangle, just so we have it while printing the solution.\n";
			generalString += "                                (?=(?(TempRotate)(?<TempHeight>\\w)|(?<TempWidth>\\w))+\\r?\\n) \n";
			generalString += "                                (?:  \n";
			generalString += "                                    (?<SolutionChar>\\w)+\n";
			generalString += "                                    (?(TempRotate)(?<TempWidth>\\r?\\n)|(?<TempHeight>\\r?\\n))\n";
			generalString += "                                )+\n";
			generalString += "                            )\n";
			generalString += "                            \\r?\\n        # Match until we reach the end of this rectangle.\n";
			generalString += "                        )\\A.*)\n";
			generalString += "                        (?<=(?=[^~]+(?<Width>(?<-TempWidth>~)+))\\A.*)(?(TempWidth)(?!)) # Capture as many tildes as the current width.\n";
			generalString += "                        (?=(?<-TempHeight>\\S*(?<Height>\\r?\\n))+)(?(TempHeight)(?!))     # Capture newlines into stack <Height>.\n";
			generalString += "                        (?(TempRotate)(?<-TempRotate>))                                 # Clear <TempRotate>.\n";
			generalString += "                        # Try to fit the rectangle into empty positions in the target rectangle.\n";
			generalString += "                        (?<=(?=\\k<NextPos>\n";
			generalString += "                            (?:\n";
			generalString += "                                (?:                         # Match tildes\n";
			generalString += "                                    ~\n";
			generalString += "                                    (?<=(?<Filled>\\A.*))               # Push the current position to <Filled>\n";
			generalString += "                                    (?<=(?<TempCurrentFilled>\\A.*))    # Also push the current position to <TempCurrentFilled>\n";
			generalString += "                                    (?=.*\\Z                            # Ensure <Filled> is unique. No overlap between rectangles.\n";
			generalString += "                                        (?<!(?=\\A\\k<Filled>(?<=\\A\\k<TempCurrentFilled>))\\A.*(?<-Filled>.)+) \n";
			generalString += "                                    )\n";
			generalString += "                                )+?\n";
			generalString += "                                (?<=^\\k<X>\\k<Width>)        # Match exactly <Width> tildes.\n";
			generalString += "                                ~*(?<-Height>\\k<Height>\\k<X>|\\r?\\n)     # Match until the same position on the net line (or just the last line).\n";
			generalString += "                            )+\n";
			generalString += "                            (?(Height)(?!))\n";
			generalString += "                        )\\A.*)\n";
			generalString += "                        # Find the next free position - <NextPos>.\n";
			generalString += "                        (?<=(?=.*?                      \n";
			generalString += "                            (?<=(?<NextPos>\\A.*))       # <NextPos> is the position before the next free tilde.\n";
			generalString += "                            ~\n";
			generalString += "                            (?<=(?<TempNextPos>\\A.*))   # We compare it to <Filled>, which is the position including the tilde.\n";
			generalString += "                            (?=.*\\Z\n";
			generalString += "                                (?<!(?=\\A\\k<Filled>(?<=\\A\\k<TempNextPos>))\\A.*(?<-Filled>.)*) \n";
			generalString += "                            )\n";
			generalString += "                            |\n";
			generalString += "                            .*\r?\\n\\Z (?<Done>)         # If we cannot find more an empty position it means we are done. Set the <Done> flag.\n";
			generalString += "                        )\\A.*)\n";
			generalString += "        \n";
			generalString += "                    )\n";
			generalString += "                )+\n";
			generalString += "                (?(Done)|(?!))\n";
			generalString += "                \n";

			wstring clone = wstring(generalString.begin(), generalString.end());
			Regex r = Regex(clone, (RegexOptions)((int)RegexOptions::Multiline | (int)RegexOptions::Singleline | (int)RegexOptions::IgnorePatternWhitespace));

			Assert::True(expectedMatch == r.IsMatch(input));
		}
	};
}
