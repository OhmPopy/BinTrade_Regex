#pragma once

#include <unordered_set>

#include "random.h"
#include "Core/CultureInfo.h"
#include "Other/stringbuilder.h"
#include "Other/UtilsChar.h"


using namespace Regexs;
namespace RegexsTest
{
	class RegexUnicodeCharTests
	{
		static bool Contains(unordered_set<UnicodeCategory> hash, UnicodeCategory c)
		{
			for (auto itr = hash.begin(); itr != hash.end(); ++itr)
			{
				UnicodeCategory elem = itr._Ptr->_Myval;
				if (c == elem)return true;
			}
			return false;
		}

	public:
		static void Run(bool test_throw)
		{
			Assert::WriteLine(L"RegexUnicodeCharTests");

			RegexUnicodeChar();
			//ValidateCategoriesParticipatingInCaseConversion();//todo check pass but slow
			WideLatin();
		}

		static int MaxUnicodeRange() { return 2 << 15; }

		static void RegexUnicodeChar()
		{
			// Regex engine is Unicode aware now for the \w and \d character classes
			// \s is not - i.e. it still only recognizes the ASCII space separators, not Unicode ones
			// The new character classes for this:
			// [\p{L1}\p{Lu}\p{Lt}\p{Lo}\p{Nd}\p{Pc}]
			vector<char> validChars;
			vector<char> invalidChars;
			for (int i = 0; i < MaxUnicodeRange(); i++)
			{
				char c = (char)i;
				switch (CharUnicodeInfo::GetUnicodeCategory(c))
				{
				case UnicodeCategory::UppercaseLetter: //Lu
				case UnicodeCategory::LowercaseLetter: //Li
				case UnicodeCategory::TitlecaseLetter: // Lt
				case UnicodeCategory::ModifierLetter: // Lm
				case UnicodeCategory::OtherLetter: // Lo
				case UnicodeCategory::DecimalDigitNumber: // Nd
					//                    case UnicodeCategory::LetterNumber:           // ??
					//                    case UnicodeCategory::OtherNumber:            // ??
				case UnicodeCategory::NonSpacingMark:
					//                    case UnicodeCategory::SpacingCombiningMark:   // Mc
				case UnicodeCategory::ConnectorPunctuation: // Pc
					validChars.push_back(c);
					break;
				default:
					invalidChars.push_back(c);
					break;
				}
			}

			// \w - we will create strings from valid characters that form \w and make sure that the regex engine catches this.
			// Build a random string with valid characters followed by invalid characters
			Random random;
			random.Init(-55);

			Regex regex_1 = Regex(L"\\w*");

			int validCharLength = 10;
			int invalidCharLength = 15;

			for (int i = 0; i < 100; i++)
			{
				StringBuilder builder1 = StringBuilder();
				StringBuilder builder2 = StringBuilder();

				for (int j = 0; j < validCharLength; j++)
				{
					wchar_t c = validChars[random.Next(validChars.size())];
					builder1.AppendChar(c);
					builder2.AppendChar(c);
				}

				for (int j = 0; j < invalidCharLength; j++)
				{
					builder1.AppendChar(invalidChars[random.Next(invalidChars.size())]);
				}

				wstring input = builder1.ToString();
				Match* match = regex_1.Match(&input);
				Assert::True(match->Success);

				Assert::True(builder2.ToString() == match->Value);
				Assert::True(0 == match->Index);
				Assert::True(validCharLength == match->Length);

				match = match->NextMatch();
				do
				{
					// We get empty matches for each of the non-matching characters of input to match
					// the * wildcard in regex pattern.
					Assert::True(L"" == match->Value);
					Assert::True(0 == match->Length);
					match = match->NextMatch();
				}
				while (match->Success);
			}

			// Build a random string with invalid characters followed by valid characters and then again invalid
			random.Init(-55);
			Regex	regex_2 = Regex(L"\\w+");

			validCharLength = 10;
			invalidCharLength = 15;

			for (int i = 0; i < 500; i++)
			{
				StringBuilder builder1 = StringBuilder();
				StringBuilder builder2 = StringBuilder();

				for (int j = 0; j < invalidCharLength; j++)
				{
					builder1.AppendChar(invalidChars[random.Next(invalidChars.size())]);
				}

				for (int j = 0; j < validCharLength; j++)
				{
					char c = validChars[random.Next(validChars.size())];
					builder1.AppendChar(c);
					builder2.AppendChar(c);
				}

				for (int j = 0; j < invalidCharLength; j++)
				{
					builder1.AppendChar(invalidChars[random.Next(invalidChars.size())]);
				}

				wstring input = builder1.ToString();

				Match* match = regex_2.Match(&input);
				Assert::True(match->Success);

				Assert::True(builder2.ToString() == match->Value);
				Assert::True(invalidCharLength == match->Index);
				Assert::True(validCharLength == match->Length);

				match = match->NextMatch();
				Assert::False(match->Success);
			}

			validChars = vector<char>();
			invalidChars = vector<char>();
			for (int i = 0; i < MaxUnicodeRange(); i++)
			{
				char c = (char)i;
				if (CharUnicodeInfo::GetUnicodeCategory(c) == UnicodeCategory::DecimalDigitNumber)
				{
					validChars.push_back(c);
				}
				else
				{
					invalidChars.push_back(c);
				}
			}

			// \d - we will create strings from valid characters that form \d and make sure that the regex engine catches this.
			// Build a random string with valid characters and then again invalid
			Regex regex_3 = Regex(L"\\d+");

			validCharLength = 10;
			invalidCharLength = 15;

			for (int i = 0; i < 100; i++)
			{
				StringBuilder builder1 = StringBuilder();
				StringBuilder builder2 = StringBuilder();

				for (int j = 0; j < validCharLength; j++)
				{
					char c = validChars[random.Next(validChars.size())];
					builder1.AppendChar(c);
					builder2.AppendChar(c);
				}

				for (int j = 0; j < invalidCharLength; j++)
				{
					builder1.AppendChar(invalidChars[random.Next(invalidChars.size())]);
				}

				wstring input = builder1.ToString();
				Match* match = regex_3.Match(&input);


				Assert::True(builder2.ToString() == match->Value);
				Assert::True(0 == match->Index);
				Assert::True(validCharLength == match->Length);

				match = match->NextMatch();
				Assert::False(match->Success);
			}

			// Build a random string with invalid characters, valid and then again invalid
			Regex regex_4 = Regex(L"\\d+");

			validCharLength = 10;
			invalidCharLength = 15;

			for (int i = 0; i < 100; i++)
			{
				StringBuilder builder1 = StringBuilder();
				StringBuilder builder2 = StringBuilder();

				for (int j = 0; j < invalidCharLength; j++)
				{
					builder1.AppendChar(invalidChars[random.Next(invalidChars.size())]);
				}

				for (int j = 0; j < validCharLength; j++)
				{
					char c = validChars[random.Next(validChars.size())];
					builder1.AppendChar(c);
					builder2.AppendChar(c);
				}

				for (int j = 0; j < invalidCharLength; j++)
				{
					builder1.AppendChar(invalidChars[random.Next(invalidChars.size())]);
				}

				wstring input = builder1.ToString();

				Match* match = regex_4.Match(&input);
				Assert::True(match->Success);

				Assert::True(builder2.ToString() == match->Value);
				Assert::True(invalidCharLength == match->Index);
				Assert::True(validCharLength == match->Length);

				match = match->NextMatch();
				Assert::False(match->Success);
			}

		}

		static void ValidateCategoriesParticipatingInCaseConversion()
		{
			// Some optimizations in RegexCompiler rely on only some Unicode categories participating
			// in case conversion.  If this test ever fails, that optimization needs to be revisited,
			// as our assumptions about the Unicode spec may have been invalidated.

			unordered_set<UnicodeCategory> nonParticipatingCategories = unordered_set<UnicodeCategory>
			{
				UnicodeCategory::ClosePunctuation,
				UnicodeCategory::ConnectorPunctuation,
				UnicodeCategory::Control,
				UnicodeCategory::DashPunctuation,
				UnicodeCategory::DecimalDigitNumber,
				UnicodeCategory::FinalQuotePunctuation,
				UnicodeCategory::InitialQuotePunctuation,
				UnicodeCategory::LineSeparator,
				UnicodeCategory::OpenPunctuation,
				UnicodeCategory::OtherNumber,
				UnicodeCategory::OtherPunctuation,
				UnicodeCategory::ParagraphSeparator,
				UnicodeCategory::SpaceSeparator,
			};

			auto cache = GetCultureInfo::s_userDefaultCulture;

			auto all_AllCultures = ComCultureInfo::GetAllCultureName();
			for (auto all_all_culture : all_AllCultures)
			{
				ComCultureInfo* culture_info = ComCultureInfo::CreateSpecificCulture(all_all_culture);

				GetCultureInfo::s_userDefaultCulture = culture_info;
				for (int i = 0; i <= 0xFFFF; i++)
				{
					wchar_t ch = (wchar_t)i;
					wchar_t upper = UtilsCharInternal::ToUpper(ch, culture_info);
					wchar_t lower = UtilsCharInternal::ToLower(ch, culture_info);


					if (Contains(nonParticipatingCategories, CharUnicodeInfo::GetUnicodeCategory(ch)))
					{
						// If this character is in one of these categories, make sure it doesn't change case.
						Assert::True(ch == upper, (culture_info->CultureName + L" -- " +  std::to_wstring(i)).c_str());
						Assert::True(ch == lower, (culture_info->CultureName + L" -- " + std::to_wstring(i)).c_str());
					}
					else
					{
						// If it's not in one of these categories, make sure it doesn't change case to
						// something in one of these categories.
						UnicodeCategory upperCategory = CharUnicodeInfo::GetUnicodeCategory(upper);
						UnicodeCategory lowerCategory = CharUnicodeInfo::GetUnicodeCategory(lower);
						Assert::False(Contains(nonParticipatingCategories, upperCategory), (culture_info->CultureName + L" -- " + std::to_wstring(i)).c_str());
						Assert::False(Contains(nonParticipatingCategories, lowerCategory), (culture_info->CultureName + L" -- " + std::to_wstring(i)).c_str());
					}
				}
			}

			GetCultureInfo::s_userDefaultCulture = cache;
		}

		static void WideLatin()
		{
			const wstring OrigPattern = L"abc";

			//shift each char in the pattern to the Wide-Latin alphabet of Unicode
			wstring pattern_WL = L"ａｂｃ";
			wstring pattern = L"(abc===ａｂｃ)+";

			Regex re = Regex(pattern, RegexOptions::IgnoreCase);
			wstring input = L"=====ABC===ａｂｃabc===ＡＢＣ===abc===abc";

			auto match1 = re.Match(&input);
			Assert::True(match1->Success);
			Assert::True(5 == match1->Index);
			Assert::True(2 * (OrigPattern.length() + 3 + pattern_WL.length()) == match1->Length);

			auto match2 = match1->NextMatch();
			Assert::False(match2->Success);

		}
	};
}
