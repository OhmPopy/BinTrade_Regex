#include "RegexCharClass.h"

#include <cassert>
#include <unordered_set>

#include "CultureInfo.h"
#include "UtilsCollections.h"
#include "UtilsString.h"

namespace Regexs
{
	int RegexCharClass::LowercaseSet = 0; // Set to arg.
	int RegexCharClass::LowercaseAdd = 1; // Add arg.
	int RegexCharClass::LowercaseBor = 2; // Bitwise or with 1.
	int RegexCharClass::LowercaseBad = 3; // Bitwise and with 1 and add original.

	const std::wstring RegexCharClass::InternalRegexIgnoreCase = L"__InternalRegexIgnoreCase__";
	const std::wstring RegexCharClass::Space = L"\x64";
	const std::wstring RegexCharClass::NotSpace = NegateCategory(Space);
	std::wstring RegexCharClass::Word;
	std::wstring RegexCharClass::NotWord;
	std::wstring RegexCharClass::SpaceClass;
	std::wstring RegexCharClass::NotSpaceClass;
	std::wstring RegexCharClass::WordClass;
	std::wstring RegexCharClass::NotWordClass;
	std::wstring RegexCharClass::DigitClass;
	std::wstring RegexCharClass::NotDigitClass;
	const std::wstring RegexCharClass::ECMASpaceSet = UtilsString::StringInitSpe(0x0009, 0x000e, 0x0020, 0x0021);
	const std::wstring RegexCharClass::NotECMASpaceSet = UtilsString::StringInitSpe(0x00, 0x09, 0x000e, 0x0020, 0x0021);
	const std::wstring RegexCharClass::ECMAWordSet = UtilsString::StringInitSpe(0x0030, 0x003a, 0x0041, 0x005b, 0x005f, 0x0060, 0x0061, 0x007b, 0x0130, 0x0131);
	const std::wstring RegexCharClass::NotECMAWordSet = UtilsString::StringInitSpe(0x0000, 0x0030, 0x003a, 0x0041, 0x005b, 0x005f, 0x0060, 0x0061, 0x007b, 0x0130, 0x0131);
	const std::wstring RegexCharClass::ECMADigitSet = UtilsString::StringInitSpe(0x0030, 0x003a);
	const std::wstring RegexCharClass::NotECMADigitSet = UtilsString::StringInitSpe(0x00, 0x030, 0x003a);
	const std::wstring RegexCharClass::ECMASpaceClass = UtilsString::StringInitSpe(0x00, 0x04, 0x00, 0x0009, 0x000e, 0x0020, 0x0021);
	const std::wstring RegexCharClass::NotECMASpaceClass = UtilsString::StringInitSpe(0x01, 0x04, 0x00, 0x0009, 0x000e, 0x0020, 0x0021);
	const std::wstring RegexCharClass::ECMAWordClass = UtilsString::StringInitSpe(0x00, 0x0A, 0x00, 0x0030, 0x003a, 0x0041, 0x005b, 0x005f, 0x0060, 0x0061, 0x007b, 0x0130, 0x0131);
	const std::wstring RegexCharClass::NotECMAWordClass = UtilsString::StringInitSpe(0x0001, 0x000a, 0x0000, 0x0030, 0x003a, 0x0041, 0x005b, 0x005f, 0x0060, 0x0061, 0x007b, 0x0130, 0x0131);
	const std::wstring RegexCharClass::ECMADigitClass = UtilsString::StringInitSpe(0x00, 0x02, 0x00, 0x0030, 0x003a);
	const std::wstring RegexCharClass::NotECMADigitClass = UtilsString::StringInitSpe(0x01, 0x02, 0x00, 0x0030, 0x003a);


	const std::wstring RegexCharClass::AnyClass = UtilsString::StringInitSpe(0x00, 0x01, 0x00, 0x00);
	const std::wstring RegexCharClass::EmptyClass = UtilsString::StringInitSpe(0x00, 0x00, 0x00);
	std::unordered_map<std::wstring, std::wstring> RegexCharClass::_definedCategories;


	//https://sheilart.github.io/Azure-Alphant/surrogates.html

	wchar_t uDB80 = (wchar_t)56192;
	wchar_t uDC00 = (wchar_t)56320;
	wchar_t uD800 = (wchar_t)55296;

	std::wstring uDB80uDC00()
	{
		std::wstring text;
		text.push_back(uDB80);
		text.push_back(uDC00);
		return std::wstring(text.begin(), text.end());
	}

	//uD800\uDB80
	std::wstring uD800uDB80()
	{
		std::wstring text;
		text.push_back(uD800);
		text.push_back(uDB80);
		return std::wstring(text.begin(), text.end());
	}

	//uDC00\uE000
	std::wstring uDC00uE000()
	{
		std::wstring text;
		text.push_back(uDC00);
		text += L"\uE000";
		return std::wstring(text.begin(), text.end());
	}

	//u0000\u0080  ; L"\u0000\u0080"
	std::wstring u0000u0080()
	{
		std::wstring text = UtilsString::StringInitSpe(0x00, 0x0080);
		return text;
	}

	const std::vector<std::vector<std::wstring>> RegexCharClass::_propTable =
	{
		{L"IsAlphabeticPresentationForms", L"\uFB00\uFB50"},
		{L"IsArabic", L"\u0600\u0700"},
		{L"IsArabicPresentationForms-A", L"\uFB50\uFE00"},
		{L"IsArabicPresentationForms-B", L"\uFE70\uFF00"},
		{L"IsArmenian", L"\u0530\u0590"},
		{L"IsArrows", L"\u2190\u2200"},
		{L"IsBasicLatin", u0000u0080()},
		{L"IsBengali", L"\u0980\u0A00"},
		{L"IsBlockElements", L"\u2580\u25A0"},
		{L"IsBopomofo", L"\u3100\u3130"},
		{L"IsBopomofoExtended", L"\u31A0\u31C0"},
		{L"IsBoxDrawing", L"\u2500\u2580"},
		{L"IsBraillePatterns", L"\u2800\u2900"},
		{L"IsBuhid", L"\u1740\u1760"},
		{L"IsCJKCompatibility", L"\u3300\u3400"},
		{L"IsCJKCompatibilityForms", L"\uFE30\uFE50"},
		{L"IsCJKCompatibilityIdeographs", L"\uF900\uFB00"},
		{L"IsCJKRadicalsSupplement", L"\u2E80\u2F00"},
		{L"IsCJKSymbolsandPunctuation", L"\u3000\u3040"},
		{L"IsCJKUnifiedIdeographs", L"\u4E00\uA000"},
		{L"IsCJKUnifiedIdeographsExtensionA", L"\u3400\u4DC0"},
		{L"IsCherokee", L"\u13A0\u1400"},
		{L"IsCombiningDiacriticalMarks", L"\u0300\u0370"},
		{L"IsCombiningDiacriticalMarksforSymbols", L"\u20D0\u2100"},
		{L"IsCombiningHalfMarks", L"\uFE20\uFE30"},
		{L"IsCombiningMarksforSymbols", L"\u20D0\u2100"},
		{L"IsControlPictures", L"\u2400\u2440"},
		{L"IsCurrencySymbols", L"\u20A0\u20D0"},
		{L"IsCyrillic", L"\u0400\u0500"},
		{L"IsCyrillicSupplement", L"\u0500\u0530"},
		{L"IsDevanagari", L"\u0900\u0980"},
		{L"IsDingbats", L"\u2700\u27C0"},
		{L"IsEnclosedAlphanumerics", L"\u2460\u2500"},
		{L"IsEnclosedCJKLettersandMonths", L"\u3200\u3300"},
		{L"IsEthiopic", L"\u1200\u1380"},
		{L"IsGeneralPunctuation", L"\u2000\u2070"},
		{L"IsGeometricShapes", L"\u25A0\u2600"},
		{L"IsGeorgian", L"\u10A0\u1100"},
		{L"IsGreek", L"\u0370\u0400"},
		{L"IsGreekExtended", L"\u1F00\u2000"},
		{L"IsGreekandCoptic", L"\u0370\u0400"},
		{L"IsGujarati", L"\u0A80\u0B00"},
		{L"IsGurmukhi", L"\u0A00\u0A80"},
		{L"IsHalfwidthandFullwidthForms", L"\uFF00\uFFF0"},
		{L"IsHangulCompatibilityJamo", L"\u3130\u3190"},
		{L"IsHangulJamo", L"\u1100\u1200"},
		{L"IsHangulSyllables", L"\uAC00\uD7B0"},
		{L"IsHanunoo", L"\u1720\u1740"},
		{L"IsHebrew", L"\u0590\u0600"},
		//{L"IsHighPrivateUseSurrogates", L"\uDB80\uDC00"},//bdki
		{L"IsHighPrivateUseSurrogates", uDB80uDC00()},
		//{L"IsHighSurrogates", L"\uD800\uDB80"},//bdki
		{L"IsHighSurrogates", uD800uDB80()},
		{L"IsHiragana", L"\u3040\u30A0"},
		{L"IsIPAExtensions", L"\u0250\u02B0"},
		{L"IsIdeographicDescriptionCharacters", L"\u2FF0\u3000"},
		{L"IsKanbun", L"\u3190\u31A0"},
		{L"IsKangxiRadicals", L"\u2F00\u2FE0"},
		{L"IsKannada", L"\u0C80\u0D00"},
		{L"IsKatakana", L"\u30A0\u3100"},
		{L"IsKatakanaPhoneticExtensions", L"\u31F0\u3200"},
		{L"IsKhmer", L"\u1780\u1800"},
		{L"IsKhmerSymbols", L"\u19E0\u1A00"},
		{L"IsLao", L"\u0E80\u0F00"},
		{L"IsLatin-1Supplement", L"\u0080\u0100"},
		{L"IsLatinExtended-A", L"\u0100\u0180"},
		{L"IsLatinExtended-B", L"\u0180\u0250"},
		{L"IsLatinExtendedAdditional", L"\u1E00\u1F00"},
		{L"IsLetterlikeSymbols", L"\u2100\u2150"},
		{L"IsLimbu", L"\u1900\u1950"},
		//{L"IsLowSurrogates", L"\uDC00\uE000"},//bdki
		{L"IsLowSurrogates", uDC00uE000()},
		{L"IsMalayalam", L"\u0D00\u0D80"},
		{L"IsMathematicalOperators", L"\u2200\u2300"},
		{L"IsMiscellaneousMathematicalSymbols-A", L"\u27C0\u27F0"},
		{L"IsMiscellaneousMathematicalSymbols-B", L"\u2980\u2A00"},
		{L"IsMiscellaneousSymbols", L"\u2600\u2700"},
		{L"IsMiscellaneousSymbolsandArrows", L"\u2B00\u2C00"},
		{L"IsMiscellaneousTechnical", L"\u2300\u2400"},
		{L"IsMongolian", L"\u1800\u18B0"},
		{L"IsMyanmar", L"\u1000\u10A0"},
		{L"IsNumberForms", L"\u2150\u2190"},
		{L"IsOgham", L"\u1680\u16A0"},
		{L"IsOpticalCharacterRecognition", L"\u2440\u2460"},
		{L"IsOriya", L"\u0B00\u0B80"},
		{L"IsPhoneticExtensions", L"\u1D00\u1D80"},
		{L"IsPrivateUse", L"\uE000\uF900"},
		{L"IsPrivateUseArea", L"\uE000\uF900"},
		{L"IsRunic", L"\u16A0\u1700"},
		{L"IsSinhala", L"\u0D80\u0E00"},
		{L"IsSmallFormVariants", L"\uFE50\uFE70"},
		{L"IsSpacingModifierLetters", L"\u02B0\u0300"},
		{L"IsSpecials", L"\uFFF0"},
		{L"IsSuperscriptsandSubscripts", L"\u2070\u20A0"},
		{L"IsSupplementalArrows-A", L"\u27F0\u2800"},
		{L"IsSupplementalArrows-B", L"\u2900\u2980"},
		{L"IsSupplementalMathematicalOperators", L"\u2A00\u2B00"},
		{L"IsSyriac", L"\u0700\u0750"},
		{L"IsTagalog", L"\u1700\u1720"},
		{L"IsTagbanwa", L"\u1760\u1780"},
		{L"IsTaiLe", L"\u1950\u1980"},
		{L"IsTamil", L"\u0B80\u0C00"},
		{L"IsTelugu", L"\u0C00\u0C80"},
		{L"IsThaana", L"\u0780\u07C0"},
		{L"IsThai", L"\u0E00\u0E80"},
		{L"IsTibetan", L"\u0F00\u1000"},
		{L"IsUnifiedCanadianAboriginalSyllabics", L"\u1400\u1680"},
		{L"IsVariationSelectors", L"\uFE00\uFE10"},
		{L"IsYiRadicals", L"\uA490\uA4D0"},
		{L"IsYiSyllables", L"\uA000\uA490"},
		{L"IsYijingHexagramSymbols", L"\u4DC0\u4E00"},
		{L"_xmlC", L"\u002D\u002F\u0030\u003B\u0041\u005B\u005F\u0060\u0061\u007B\u00B7\u00B8\u00C0\u00D7\u00D8\u00F7\u00F8\u0132\u0134\u013F\u0141\u0149\u014A\u017F\u0180\u01C4\u01CD\u01F1\u01F4\u01F6\u01FA\u0218\u0250\u02A9\u02BB\u02C2\u02D0\u02D2\u0300\u0346\u0360\u0362\u0386\u038B\u038C\u038D\u038E\u03A2\u03A3\u03CF\u03D0\u03D7\u03DA\u03DB\u03DC\u03DD\u03DE\u03DF\u03E0\u03E1\u03E2\u03F4\u0401\u040D\u040E\u0450\u0451\u045D\u045E\u0482\u0483\u0487\u0490\u04C5\u04C7\u04C9\u04CB\u04CD\u04D0\u04EC\u04EE\u04F6\u04F8\u04FA\u0531\u0557\u0559\u055A\u0561\u0587\u0591\u05A2\u05A3\u05BA\u05BB\u05BE\u05BF\u05C0\u05C1\u05C3\u05C4\u05C5\u05D0\u05EB\u05F0\u05F3\u0621\u063B\u0640\u0653\u0660\u066A\u0670\u06B8\u06BA\u06BF\u06C0\u06CF\u06D0\u06D4\u06D5\u06E9\u06EA\u06EE\u06F0\u06FA\u0901\u0904\u0905\u093A\u093C\u094E\u0951\u0955\u0958\u0964\u0966\u0970\u0981\u0984\u0985\u098D\u098F\u0991\u0993\u09A9\u09AA\u09B1\u09B2\u09B3\u09B6\u09BA\u09BC\u09BD\u09BE\u09C5\u09C7\u09C9\u09CB\u09CE\u09D7\u09D8\u09DC\u09DE\u09DF\u09E4\u09E6\u09F2\u0A02\u0A03\u0A05\u0A0B\u0A0F\u0A11\u0A13\u0A29\u0A2A\u0A31\u0A32\u0A34\u0A35\u0A37\u0A38\u0A3A\u0A3C\u0A3D\u0A3E\u0A43\u0A47\u0A49\u0A4B\u0A4E\u0A59\u0A5D\u0A5E\u0A5F\u0A66\u0A75\u0A81\u0A84\u0A85\u0A8C\u0A8D\u0A8E\u0A8F\u0A92\u0A93\u0AA9\u0AAA\u0AB1\u0AB2\u0AB4\u0AB5\u0ABA\u0ABC\u0AC6\u0AC7\u0ACA\u0ACB\u0ACE\u0AE0\u0AE1\u0AE6\u0AF0\u0B01\u0B04\u0B05\u0B0D\u0B0F\u0B11\u0B13\u0B29\u0B2A\u0B31\u0B32\u0B34\u0B36\u0B3A\u0B3C\u0B44\u0B47\u0B49\u0B4B\u0B4E\u0B56\u0B58\u0B5C\u0B5E\u0B5F\u0B62\u0B66\u0B70\u0B82\u0B84\u0B85\u0B8B\u0B8E\u0B91\u0B92\u0B96\u0B99\u0B9B\u0B9C\u0B9D\u0B9E\u0BA0\u0BA3\u0BA5\u0BA8\u0BAB\u0BAE\u0BB6\u0BB7\u0BBA\u0BBE\u0BC3\u0BC6\u0BC9\u0BCA\u0BCE\u0BD7\u0BD8\u0BE7\u0BF0\u0C01\u0C04\u0C05\u0C0D\u0C0E\u0C11\u0C12\u0C29\u0C2A\u0C34\u0C35\u0C3A\u0C3E\u0C45\u0C46\u0C49\u0C4A\u0C4E\u0C55\u0C57\u0C60\u0C62\u0C66\u0C70\u0C82\u0C84\u0C85\u0C8D\u0C8E\u0C91\u0C92\u0CA9\u0CAA\u0CB4\u0CB5\u0CBA\u0CBE\u0CC5\u0CC6\u0CC9\u0CCA\u0CCE\u0CD5\u0CD7\u0CDE\u0CDF\u0CE0\u0CE2\u0CE6\u0CF0\u0D02\u0D04\u0D05\u0D0D\u0D0E\u0D11\u0D12\u0D29\u0D2A\u0D3A\u0D3E\u0D44\u0D46\u0D49\u0D4A\u0D4E\u0D57\u0D58\u0D60\u0D62\u0D66\u0D70\u0E01\u0E2F\u0E30\u0E3B\u0E40\u0E4F\u0E50\u0E5A\u0E81\u0E83\u0E84\u0E85\u0E87\u0E89\u0E8A\u0E8B\u0E8D\u0E8E\u0E94\u0E98\u0E99\u0EA0\u0EA1\u0EA4\u0EA5\u0EA6\u0EA7\u0EA8\u0EAA\u0EAC\u0EAD\u0EAF\u0EB0\u0EBA\u0EBB\u0EBE\u0EC0\u0EC5\u0EC6\u0EC7\u0EC8\u0ECE\u0ED0\u0EDA\u0F18\u0F1A\u0F20\u0F2A\u0F35\u0F36\u0F37\u0F38\u0F39\u0F3A\u0F3E\u0F48\u0F49\u0F6A\u0F71\u0F85\u0F86\u0F8C\u0F90\u0F96\u0F97\u0F98\u0F99\u0FAE\u0FB1\u0FB8\u0FB9\u0FBA\u10A0\u10C6\u10D0\u10F7\u1100\u1101\u1102\u1104\u1105\u1108\u1109\u110A\u110B\u110D\u110E\u1113\u113C\u113D\u113E\u113F\u1140\u1141\u114C\u114D\u114E\u114F\u1150\u1151\u1154\u1156\u1159\u115A\u115F\u1162\u1163\u1164\u1165\u1166\u1167\u1168\u1169\u116A\u116D\u116F\u1172\u1174\u1175\u1176\u119E\u119F\u11A8\u11A9\u11AB\u11AC\u11AE\u11B0\u11B7\u11B9\u11BA\u11BB\u11BC\u11C3\u11EB\u11EC\u11F0\u11F1\u11F9\u11FA\u1E00\u1E9C\u1EA0\u1EFA\u1F00\u1F16\u1F18\u1F1E\u1F20\u1F46\u1F48\u1F4E\u1F50\u1F58\u1F59\u1F5A\u1F5B\u1F5C\u1F5D\u1F5E\u1F5F\u1F7E\u1F80\u1FB5\u1FB6\u1FBD\u1FBE\u1FBF\u1FC2\u1FC5\u1FC6\u1FCD\u1FD0\u1FD4\u1FD6\u1FDC\u1FE0\u1FED\u1FF2\u1FF5\u1FF6\u1FFD\u20D0\u20DD\u20E1\u20E2\u2126\u2127\u212A\u212C\u212E\u212F\u2180\u2183\u3005\u3006\u3007\u3008\u3021\u3030\u3031\u3036\u3041\u3095\u3099\u309B\u309D\u309F\u30A1\u30FB\u30FC\u30FF\u3105\u312D\u4E00\u9FA6\uAC00\uD7A4"},
		{L"_xmlD", L"\u0030\u003A\u0660\u066A\u06F0\u06FA\u0966\u0970\u09E6\u09F0\u0A66\u0A70\u0AE6\u0AF0\u0B66\u0B70\u0BE7\u0BF0\u0C66\u0C70\u0CE6\u0CF0\u0D66\u0D70\u0E50\u0E5A\u0ED0\u0EDA\u0F20\u0F2A\u1040\u104A\u1369\u1372\u17E0\u17EA\u1810\u181A\uFF10\uFF1A"},
		{L"_xmlI", L"\u003A\u003B\u0041\u005B\u005F\u0060\u0061\u007B\u00C0\u00D7\u00D8\u00F7\u00F8\u0132\u0134\u013F\u0141\u0149\u014A\u017F\u0180\u01C4\u01CD\u01F1\u01F4\u01F6\u01FA\u0218\u0250\u02A9\u02BB\u02C2\u0386\u0387\u0388\u038B\u038C\u038D\u038E\u03A2\u03A3\u03CF\u03D0\u03D7\u03DA\u03DB\u03DC\u03DD\u03DE\u03DF\u03E0\u03E1\u03E2\u03F4\u0401\u040D\u040E\u0450\u0451\u045D\u045E\u0482\u0490\u04C5\u04C7\u04C9\u04CB\u04CD\u04D0\u04EC\u04EE\u04F6\u04F8\u04FA\u0531\u0557\u0559\u055A\u0561\u0587\u05D0\u05EB\u05F0\u05F3\u0621\u063B\u0641\u064B\u0671\u06B8\u06BA\u06BF\u06C0\u06CF\u06D0\u06D4\u06D5\u06D6\u06E5\u06E7\u0905\u093A\u093D\u093E\u0958\u0962\u0985\u098D\u098F\u0991\u0993\u09A9\u09AA\u09B1\u09B2\u09B3\u09B6\u09BA\u09DC\u09DE\u09DF\u09E2\u09F0\u09F2\u0A05\u0A0B\u0A0F\u0A11\u0A13\u0A29\u0A2A\u0A31\u0A32\u0A34\u0A35\u0A37\u0A38\u0A3A\u0A59\u0A5D\u0A5E\u0A5F\u0A72\u0A75\u0A85\u0A8C\u0A8D\u0A8E\u0A8F\u0A92\u0A93\u0AA9\u0AAA\u0AB1\u0AB2\u0AB4\u0AB5\u0ABA\u0ABD\u0ABE\u0AE0\u0AE1\u0B05\u0B0D\u0B0F\u0B11\u0B13\u0B29\u0B2A\u0B31\u0B32\u0B34\u0B36\u0B3A\u0B3D\u0B3E\u0B5C\u0B5E\u0B5F\u0B62\u0B85\u0B8B\u0B8E\u0B91\u0B92\u0B96\u0B99\u0B9B\u0B9C\u0B9D\u0B9E\u0BA0\u0BA3\u0BA5\u0BA8\u0BAB\u0BAE\u0BB6\u0BB7\u0BBA\u0C05\u0C0D\u0C0E\u0C11\u0C12\u0C29\u0C2A\u0C34\u0C35\u0C3A\u0C60\u0C62\u0C85\u0C8D\u0C8E\u0C91\u0C92\u0CA9\u0CAA\u0CB4\u0CB5\u0CBA\u0CDE\u0CDF\u0CE0\u0CE2\u0D05\u0D0D\u0D0E\u0D11\u0D12\u0D29\u0D2A\u0D3A\u0D60\u0D62\u0E01\u0E2F\u0E30\u0E31\u0E32\u0E34\u0E40\u0E46\u0E81\u0E83\u0E84\u0E85\u0E87\u0E89\u0E8A\u0E8B\u0E8D\u0E8E\u0E94\u0E98\u0E99\u0EA0\u0EA1\u0EA4\u0EA5\u0EA6\u0EA7\u0EA8\u0EAA\u0EAC\u0EAD\u0EAF\u0EB0\u0EB1\u0EB2\u0EB4\u0EBD\u0EBE\u0EC0\u0EC5\u0F40\u0F48\u0F49\u0F6A\u10A0\u10C6\u10D0\u10F7\u1100\u1101\u1102\u1104\u1105\u1108\u1109\u110A\u110B\u110D\u110E\u1113\u113C\u113D\u113E\u113F\u1140\u1141\u114C\u114D\u114E\u114F\u1150\u1151\u1154\u1156\u1159\u115A\u115F\u1162\u1163\u1164\u1165\u1166\u1167\u1168\u1169\u116A\u116D\u116F\u1172\u1174\u1175\u1176\u119E\u119F\u11A8\u11A9\u11AB\u11AC\u11AE\u11B0\u11B7\u11B9\u11BA\u11BB\u11BC\u11C3\u11EB\u11EC\u11F0\u11F1\u11F9\u11FA\u1E00\u1E9C\u1EA0\u1EFA\u1F00\u1F16\u1F18\u1F1E\u1F20\u1F46\u1F48\u1F4E\u1F50\u1F58\u1F59\u1F5A\u1F5B\u1F5C\u1F5D\u1F5E\u1F5F\u1F7E\u1F80\u1FB5\u1FB6\u1FBD\u1FBE\u1FBF\u1FC2\u1FC5\u1FC6\u1FCD\u1FD0\u1FD4\u1FD6\u1FDC\u1FE0\u1FED\u1FF2\u1FF5\u1FF6\u1FFD\u2126\u2127\u212A\u212C\u212E\u212F\u2180\u2183\u3007\u3008\u3021\u302A\u3041\u3095\u30A1\u30FB\u3105\u312D\u4E00\u9FA6\uAC00\uD7A4"},
		{L"_xmlW", L"\u0024\u0025\u002B\u002C\u0030\u003A\u003C\u003F\u0041\u005B\u005E\u005F\u0060\u007B\u007C\u007D\u007E\u007F\u00A2\u00AB\u00AC\u00AD\u00AE\u00B7\u00B8\u00BB\u00BC\u00BF\u00C0\u0221\u0222\u0234\u0250\u02AE\u02B0\u02EF\u0300\u0350\u0360\u0370\u0374\u0376\u037A\u037B\u0384\u0387\u0388\u038B\u038C\u038D\u038E\u03A2\u03A3\u03CF\u03D0\u03F7\u0400\u0487\u0488\u04CF\u04D0\u04F6\u04F8\u04FA\u0500\u0510\u0531\u0557\u0559\u055A\u0561\u0588\u0591\u05A2\u05A3\u05BA\u05BB\u05BE\u05BF\u05C0\u05C1\u05C3\u05C4\u05C5\u05D0\u05EB\u05F0\u05F3\u0621\u063B\u0640\u0656\u0660\u066A\u066E\u06D4\u06D5\u06DD\u06DE\u06EE\u06F0\u06FF\u0710\u072D\u0730\u074B\u0780\u07B2\u0901\u0904\u0905\u093A\u093C\u094E\u0950\u0955\u0958\u0964\u0966\u0970\u0981\u0984\u0985\u098D\u098F\u0991\u0993\u09A9\u09AA\u09B1\u09B2\u09B3\u09B6\u09BA\u09BC\u09BD\u09BE\u09C5\u09C7\u09C9\u09CB\u09CE\u09D7\u09D8\u09DC\u09DE\u09DF\u09E4\u09E6\u09FB\u0A02\u0A03\u0A05\u0A0B\u0A0F\u0A11\u0A13\u0A29\u0A2A\u0A31\u0A32\u0A34\u0A35\u0A37\u0A38\u0A3A\u0A3C\u0A3D\u0A3E\u0A43\u0A47\u0A49\u0A4B\u0A4E\u0A59\u0A5D\u0A5E\u0A5F\u0A66\u0A75\u0A81\u0A84\u0A85\u0A8C\u0A8D\u0A8E\u0A8F\u0A92\u0A93\u0AA9\u0AAA\u0AB1\u0AB2\u0AB4\u0AB5\u0ABA\u0ABC\u0AC6\u0AC7\u0ACA\u0ACB\u0ACE\u0AD0\u0AD1\u0AE0\u0AE1\u0AE6\u0AF0\u0B01\u0B04\u0B05\u0B0D\u0B0F\u0B11\u0B13\u0B29\u0B2A\u0B31\u0B32\u0B34\u0B36\u0B3A\u0B3C\u0B44\u0B47\u0B49\u0B4B\u0B4E\u0B56\u0B58\u0B5C\u0B5E\u0B5F\u0B62\u0B66\u0B71\u0B82\u0B84\u0B85\u0B8B\u0B8E\u0B91\u0B92\u0B96\u0B99\u0B9B\u0B9C\u0B9D\u0B9E\u0BA0\u0BA3\u0BA5\u0BA8\u0BAB\u0BAE\u0BB6\u0BB7\u0BBA\u0BBE\u0BC3\u0BC6\u0BC9\u0BCA\u0BCE\u0BD7\u0BD8\u0BE7\u0BF3\u0C01\u0C04\u0C05\u0C0D\u0C0E\u0C11\u0C12\u0C29\u0C2A\u0C34\u0C35\u0C3A\u0C3E\u0C45\u0C46\u0C49\u0C4A\u0C4E\u0C55\u0C57\u0C60\u0C62\u0C66\u0C70\u0C82\u0C84\u0C85\u0C8D\u0C8E\u0C91\u0C92\u0CA9\u0CAA\u0CB4\u0CB5\u0CBA\u0CBE\u0CC5\u0CC6\u0CC9\u0CCA\u0CCE\u0CD5\u0CD7\u0CDE\u0CDF\u0CE0\u0CE2\u0CE6\u0CF0\u0D02\u0D04\u0D05\u0D0D\u0D0E\u0D11\u0D12\u0D29\u0D2A\u0D3A\u0D3E\u0D44\u0D46\u0D49\u0D4A\u0D4E\u0D57\u0D58\u0D60\u0D62\u0D66\u0D70\u0D82\u0D84\u0D85\u0D97\u0D9A\u0DB2\u0DB3\u0DBC\u0DBD\u0DBE\u0DC0\u0DC7\u0DCA\u0DCB\u0DCF\u0DD5\u0DD6\u0DD7\u0DD8\u0DE0\u0DF2\u0DF4\u0E01\u0E3B\u0E3F\u0E4F\u0E50\u0E5A\u0E81\u0E83\u0E84\u0E85\u0E87\u0E89\u0E8A\u0E8B\u0E8D\u0E8E\u0E94\u0E98\u0E99\u0EA0\u0EA1\u0EA4\u0EA5\u0EA6\u0EA7\u0EA8\u0EAA\u0EAC\u0EAD\u0EBA\u0EBB\u0EBE\u0EC0\u0EC5\u0EC6\u0EC7\u0EC8\u0ECE\u0ED0\u0EDA\u0EDC\u0EDE\u0F00\u0F04\u0F13\u0F3A\u0F3E\u0F48\u0F49\u0F6B\u0F71\u0F85\u0F86\u0F8C\u0F90\u0F98\u0F99\u0FBD\u0FBE\u0FCD\u0FCF\u0FD0\u1000\u1022\u1023\u1028\u1029\u102B\u102C\u1033\u1036\u103A\u1040\u104A\u1050\u105A\u10A0\u10C6\u10D0\u10F9\u1100\u115A\u115F\u11A3\u11A8\u11FA\u1200\u1207\u1208\u1247\u1248\u1249\u124A\u124E\u1250\u1257\u1258\u1259\u125A\u125E\u1260\u1287\u1288\u1289\u128A\u128E\u1290\u12AF\u12B0\u12B1\u12B2\u12B6\u12B8\u12BF\u12C0\u12C1\u12C2\u12C6\u12C8\u12CF\u12D0\u12D7\u12D8\u12EF\u12F0\u130F\u1310\u1311\u1312\u1316\u1318\u131F\u1320\u1347\u1348\u135B\u1369\u137D\u13A0\u13F5\u1401\u166D\u166F\u1677\u1681\u169B\u16A0\u16EB\u16EE\u16F1\u1700\u170D\u170E\u1715\u1720\u1735\u1740\u1754\u1760\u176D\u176E\u1771\u1772\u1774\u1780\u17D4\u17D7\u17D8\u17DB\u17DD\u17E0\u17EA\u180B\u180E\u1810\u181A\u1820\u1878\u1880\u18AA\u1E00\u1E9C\u1EA0\u1EFA\u1F00\u1F16\u1F18\u1F1E\u1F20\u1F46\u1F48\u1F4E\u1F50\u1F58\u1F59\u1F5A\u1F5B\u1F5C\u1F5D\u1F5E\u1F5F\u1F7E\u1F80\u1FB5\u1FB6\u1FC5\u1FC6\u1FD4\u1FD6\u1FDC\u1FDD\u1FF0\u1FF2\u1FF5\u1FF6\u1FFF\u2044\u2045\u2052\u2053\u2070\u2072\u2074\u207D\u207F\u208D\u20A0\u20B2\u20D0\u20EB\u2100\u213B\u213D\u214C\u2153\u2184\u2190\u2329\u232B\u23B4\u23B7\u23CF\u2400\u2427\u2440\u244B\u2460\u24FF\u2500\u2614\u2616\u2618\u2619\u267E\u2680\u268A\u2701\u2705\u2706\u270A\u270C\u2728\u2729\u274C\u274D\u274E\u274F\u2753\u2756\u2757\u2758\u275F\u2761\u2768\u2776\u2795\u2798\u27B0\u27B1\u27BF\u27D0\u27E6\u27F0\u2983\u2999\u29D8\u29DC\u29FC\u29FE\u2B00\u2E80\u2E9A\u2E9B\u2EF4\u2F00\u2FD6\u2FF0\u2FFC\u3004\u3008\u3012\u3014\u3020\u3030\u3031\u303D\u303E\u3040\u3041\u3097\u3099\u30A0\u30A1\u30FB\u30FC\u3100\u3105\u312D\u3131\u318F\u3190\u31B8\u31F0\u321D\u3220\u3244\u3251\u327C\u327F\u32CC\u32D0\u32FF\u3300\u3377\u337B\u33DE\u33E0\u33FF\u3400\u4DB6\u4E00\u9FA6\uA000\uA48D\uA490\uA4C7\uAC00\uD7A4\uF900\uFA2E\uFA30\uFA6B\uFB00\uFB07\uFB13\uFB18\uFB1D\uFB37\uFB38\uFB3D\uFB3E\uFB3F\uFB40\uFB42\uFB43\uFB45\uFB46\uFBB2\uFBD3\uFD3E\uFD50\uFD90\uFD92\uFDC8\uFDF0\uFDFD\uFE00\uFE10\uFE20\uFE24\uFE62\uFE63\uFE64\uFE67\uFE69\uFE6A\uFE70\uFE75\uFE76\uFEFD\uFF04\uFF05\uFF0B\uFF0C\uFF10\uFF1A\uFF1C\uFF1F\uFF21\uFF3B\uFF3E\uFF3F\uFF40\uFF5B\uFF5C\uFF5D\uFF5E\uFF5F\uFF66\uFFBF\uFFC2\uFFC8\uFFCA\uFFD0\uFFD2\uFFD8\uFFDA\uFFDD\uFFE0\uFFE7\uFFE8\uFFEF\uFFFC\uFFFE"}
	};


	std::vector<RegexCharClass::LowerCaseMapping> RegexCharClass::_lcTable =
	{
		LowerCaseMapping(L'\u0041', L'\u005A', LowercaseAdd, 32),
		LowerCaseMapping(L'\u00C0', L'\u00DE', LowercaseAdd, 32),
		LowerCaseMapping(L'\u0100', L'\u012E', LowercaseBor, 0),
		LowerCaseMapping(L'\u0130', L'\u0130', LowercaseSet, 0x0069),
		LowerCaseMapping(L'\u0132', L'\u0136', LowercaseBor, 0),
		LowerCaseMapping(L'\u0139', L'\u0147', LowercaseBad, 0),
		LowerCaseMapping(L'\u014A', L'\u0176', LowercaseBor, 0),
		LowerCaseMapping(L'\u0178', L'\u0178', LowercaseSet, 0x00FF),
		LowerCaseMapping(L'\u0179', L'\u017D', LowercaseBad, 0),
		LowerCaseMapping(L'\u0181', L'\u0181', LowercaseSet, 0x0253),
		LowerCaseMapping(L'\u0182', L'\u0184', LowercaseBor, 0),
		LowerCaseMapping(L'\u0186', L'\u0186', LowercaseSet, 0x0254),
		LowerCaseMapping(L'\u0187', L'\u0187', LowercaseSet, 0x0188),
		LowerCaseMapping(L'\u0189', L'\u018A', LowercaseAdd, 205),
		LowerCaseMapping(L'\u018B', L'\u018B', LowercaseSet, 0x018C),
		LowerCaseMapping(L'\u018E', L'\u018E', LowercaseSet, 0x01DD),
		LowerCaseMapping(L'\u018F', L'\u018F', LowercaseSet, 0x0259),
		LowerCaseMapping(L'\u0190', L'\u0190', LowercaseSet, 0x025B),
		LowerCaseMapping(L'\u0191', L'\u0191', LowercaseSet, 0x0192),
		LowerCaseMapping(L'\u0193', L'\u0193', LowercaseSet, 0x0260),
		LowerCaseMapping(L'\u0194', L'\u0194', LowercaseSet, 0x0263),
		LowerCaseMapping(L'\u0196', L'\u0196', LowercaseSet, 0x0269),
		LowerCaseMapping(L'\u0197', L'\u0197', LowercaseSet, 0x0268),
		LowerCaseMapping(L'\u0198', L'\u0198', LowercaseSet, 0x0199),
		LowerCaseMapping(L'\u019C', L'\u019C', LowercaseSet, 0x026F),
		LowerCaseMapping(L'\u019D', L'\u019D', LowercaseSet, 0x0272),
		LowerCaseMapping(L'\u019F', L'\u019F', LowercaseSet, 0x0275),
		LowerCaseMapping(L'\u01A0', L'\u01A4', LowercaseBor, 0),
		LowerCaseMapping(L'\u01A7', L'\u01A7', LowercaseSet, 0x01A8),
		LowerCaseMapping(L'\u01A9', L'\u01A9', LowercaseSet, 0x0283),
		LowerCaseMapping(L'\u01AC', L'\u01AC', LowercaseSet, 0x01AD),
		LowerCaseMapping(L'\u01AE', L'\u01AE', LowercaseSet, 0x0288),
		LowerCaseMapping(L'\u01AF', L'\u01AF', LowercaseSet, 0x01B0),
		LowerCaseMapping(L'\u01B1', L'\u01B2', LowercaseAdd, 217),
		LowerCaseMapping(L'\u01B3', L'\u01B5', LowercaseBad, 0),
		LowerCaseMapping(L'\u01B7', L'\u01B7', LowercaseSet, 0x0292),
		LowerCaseMapping(L'\u01B8', L'\u01B8', LowercaseSet, 0x01B9),
		LowerCaseMapping(L'\u01BC', L'\u01BC', LowercaseSet, 0x01BD),
		LowerCaseMapping(L'\u01C4', L'\u01C5', LowercaseSet, 0x01C6),
		LowerCaseMapping(L'\u01C7', L'\u01C8', LowercaseSet, 0x01C9),
		LowerCaseMapping(L'\u01CA', L'\u01CB', LowercaseSet, 0x01CC),
		LowerCaseMapping(L'\u01CD', L'\u01DB', LowercaseBad, 0),
		LowerCaseMapping(L'\u01DE', L'\u01EE', LowercaseBor, 0),
		LowerCaseMapping(L'\u01F1', L'\u01F2', LowercaseSet, 0x01F3),
		LowerCaseMapping(L'\u01F4', L'\u01F4', LowercaseSet, 0x01F5),
		LowerCaseMapping(L'\u01FA', L'\u0216', LowercaseBor, 0),
		LowerCaseMapping(L'\u0386', L'\u0386', LowercaseSet, 0x03AC),
		LowerCaseMapping(L'\u0388', L'\u038A', LowercaseAdd, 37),
		LowerCaseMapping(L'\u038C', L'\u038C', LowercaseSet, 0x03CC),
		LowerCaseMapping(L'\u038E', L'\u038F', LowercaseAdd, 63),
		LowerCaseMapping(L'\u0391', L'\u03AB', LowercaseAdd, 32),
		LowerCaseMapping(L'\u03E2', L'\u03EE', LowercaseBor, 0),
		LowerCaseMapping(L'\u0401', L'\u040F', LowercaseAdd, 80),
		LowerCaseMapping(L'\u0410', L'\u042F', LowercaseAdd, 32),
		LowerCaseMapping(L'\u0460', L'\u0480', LowercaseBor, 0),
		LowerCaseMapping(L'\u0490', L'\u04BE', LowercaseBor, 0),
		LowerCaseMapping(L'\u04C1', L'\u04C3', LowercaseBad, 0),
		LowerCaseMapping(L'\u04C7', L'\u04C7', LowercaseSet, 0x04C8),
		LowerCaseMapping(L'\u04CB', L'\u04CB', LowercaseSet, 0x04CC),
		LowerCaseMapping(L'\u04D0', L'\u04EA', LowercaseBor, 0),
		LowerCaseMapping(L'\u04EE', L'\u04F4', LowercaseBor, 0),
		LowerCaseMapping(L'\u04F8', L'\u04F8', LowercaseSet, 0x04F9),
		LowerCaseMapping(L'\u0531', L'\u0556', LowercaseAdd, 48),
		LowerCaseMapping(L'\u10A0', L'\u10C5', LowercaseAdd, 48),
		LowerCaseMapping(L'\u1E00', L'\u1EF8', LowercaseBor, 0),
		LowerCaseMapping(L'\u1F08', L'\u1F0F', LowercaseAdd, -8),
		LowerCaseMapping(L'\u1F18', L'\u1F1F', LowercaseAdd, -8),
		LowerCaseMapping(L'\u1F28', L'\u1F2F', LowercaseAdd, -8),
		LowerCaseMapping(L'\u1F38', L'\u1F3F', LowercaseAdd, -8),
		LowerCaseMapping(L'\u1F48', L'\u1F4D', LowercaseAdd, -8),
		LowerCaseMapping(L'\u1F59', L'\u1F59', LowercaseSet, 0x1F51),
		LowerCaseMapping(L'\u1F5B', L'\u1F5B', LowercaseSet, 0x1F53),
		LowerCaseMapping(L'\u1F5D', L'\u1F5D', LowercaseSet, 0x1F55),
		LowerCaseMapping(L'\u1F5F', L'\u1F5F', LowercaseSet, 0x1F57),
		LowerCaseMapping(L'\u1F68', L'\u1F6F', LowercaseAdd, -8),
		LowerCaseMapping(L'\u1F88', L'\u1F8F', LowercaseAdd, -8),
		LowerCaseMapping(L'\u1F98', L'\u1F9F', LowercaseAdd, -8),
		LowerCaseMapping(L'\u1FA8', L'\u1FAF', LowercaseAdd, -8),
		LowerCaseMapping(L'\u1FB8', L'\u1FB9', LowercaseAdd, -8),
		LowerCaseMapping(L'\u1FBA', L'\u1FBB', LowercaseAdd, -74),
		LowerCaseMapping(L'\u1FBC', L'\u1FBC', LowercaseSet, 0x1FB3),
		LowerCaseMapping(L'\u1FC8', L'\u1FCB', LowercaseAdd, -86),
		LowerCaseMapping(L'\u1FCC', L'\u1FCC', LowercaseSet, 0x1FC3),
		LowerCaseMapping(L'\u1FD8', L'\u1FD9', LowercaseAdd, -8),
		LowerCaseMapping(L'\u1FDA', L'\u1FDB', LowercaseAdd, -100),
		LowerCaseMapping(L'\u1FE8', L'\u1FE9', LowercaseAdd, -8),
		LowerCaseMapping(L'\u1FEA', L'\u1FEB', LowercaseAdd, -112),
		LowerCaseMapping(L'\u1FEC', L'\u1FEC', LowercaseSet, 0x1FE5),
		LowerCaseMapping(L'\u1FF8', L'\u1FF9', LowercaseAdd, -128),
		LowerCaseMapping(L'\u1FFA', L'\u1FFB', LowercaseAdd, -126),
		LowerCaseMapping(L'\u1FFC', L'\u1FFC', LowercaseSet, 0x1FF3),
		LowerCaseMapping(L'\u2160', L'\u216F', LowercaseAdd, 16),
		LowerCaseMapping(L'\u24B6', L'\u24D0', LowercaseAdd, 26),
		LowerCaseMapping(L'\uFF21', L'\uFF3A', LowercaseAdd, 32)
	};

	std::vector<wchar_t> RegexCharClass::Hex = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

	std::vector<std::wstring> RegexCharClass::Categories =
	{
		L"Lu", L"Ll", L"Lt", L"Lm", L"Lo", InternalRegexIgnoreCase,
		L"Mn", L"Mc", L"Me",
		L"Nd", L"Nl", L"No",
		L"Zs", L"Zl", L"Zp",
		L"Cc", L"Cf", L"Cs", L"Co",
		L"Pc", L"Pd", L"Ps", L"Pe", L"Pi", L"Pf", L"Po",
		L"Sm", L"Sc", L"Sk", L"So",
		L"Cn"
	};

	RegexCharClass::StaticConstructor::StaticConstructor()
	{
		// addressing Dictionary versus Hashtable thread safety difference by using
		// a temp Dictionary. Note that this is just a theoretical concern since this
		// is a static ctor and getter methods aren't called until after this is 
		// done; this is just to avoid the long-term possibility of thread safety 
		// problems.
		std::unordered_map<std::wstring, std::wstring> tempCategories(32);

		std::vector<wchar_t> groups(9);

		StringBuilder word = StringBuilder(11);

		word.AppendChar(GroupChar);
		groups[0] = GroupChar;

		// We need the UnicodeCategory enum values as a char so we can put them in a string
		// in the hashtable.  In order to get there, we first must cast to an int, 
		// then cast to a char
		// Also need to distinguish between positive and negative values.  UnicodeCategory is zero 
		// based, so we add one to each value and subtract it off later

		// Others
		groups[1] = static_cast<wchar_t>(static_cast<int>(UnicodeCategory::Control) + 1);
		tempCategories[L"Cc"] = UtilsString::StringInitSpe(groups[1]); // Control
		groups[2] = static_cast<wchar_t>(static_cast<int>(UnicodeCategory::Format) + 1);
		tempCategories[L"Cf"] = UtilsString::StringInitSpe(groups[2]); // Format
		groups[3] = static_cast<wchar_t>(static_cast<int>(UnicodeCategory::OtherNotAssigned) + 1);
		tempCategories[L"Cn"] =  UtilsString::StringInitSpe(groups[3]); // Not assigned
		groups[4] = static_cast<wchar_t>(static_cast<int>(UnicodeCategory::PrivateUse) + 1);
		tempCategories[L"Co"] =  UtilsString::StringInitSpe(groups[4]); // Private use
		groups[5] = static_cast<wchar_t>(static_cast<int>(UnicodeCategory::Surrogate) + 1);
		tempCategories[L"Cs"] =  UtilsString::StringInitSpe(groups[5]); // Surrogate

		groups[6] = GroupChar;
		tempCategories[L"C"] = UtilsString::wstring_convert_from_bytes(groups, 0, 7);

		// Letters
		groups[1] = static_cast<wchar_t>(static_cast<int>(UnicodeCategory::LowercaseLetter) + 1);
		tempCategories[L"Ll"] =  UtilsString::StringInitSpe(groups[1]); // Lowercase
		groups[2] = static_cast<wchar_t>(static_cast<int>(UnicodeCategory::ModifierLetter) + 1);
		tempCategories[L"Lm"] =  UtilsString::StringInitSpe(groups[2]); // Modifier
		groups[3] = static_cast<wchar_t>(static_cast<int>(UnicodeCategory::OtherLetter) + 1);
		tempCategories[L"Lo"] =  UtilsString::StringInitSpe(groups[3]); // Other
		groups[4] = static_cast<wchar_t>(static_cast<int>(UnicodeCategory::TitlecaseLetter) + 1);
		tempCategories[L"Lt"] =  UtilsString::StringInitSpe(groups[4]); // Titlecase
		groups[5] = static_cast<wchar_t>(static_cast<int>(UnicodeCategory::UppercaseLetter) + 1);
		tempCategories[L"Lu"] =  UtilsString::StringInitSpe(groups[5]); // Uppercase

		//groups[6] = GroupChar;
		tempCategories[L"L"] = UtilsString::wstring_convert_from_bytes(groups, 0, 7);
		word.Append(UtilsString::wstring_convert_from_bytes(groups, 1, 5));

		// InternalRegexIgnoreCase = {LowercaseLetter} OR {TitlecaseLetter} OR {UppercaseLetter}
		// !!!This category should only ever be used in conjunction with RegexOptions.IgnoreCase code paths!!!
		tempCategories[InternalRegexIgnoreCase] = UtilsString::Formats(ComCultureInfo::InvariantCulture(), L"{0}{1}{2}{3}{4}", GroupChar, groups[1], groups[4], groups[5], groups[6]);

		// Marks        
		groups[1] = static_cast<wchar_t>(static_cast<int>(UnicodeCategory::SpacingCombiningMark) + 1);
		tempCategories[L"Mc"] =  UtilsString::StringInitSpe(groups[1]); // Spacing combining
		groups[2] = static_cast<wchar_t>(static_cast<int>(UnicodeCategory::EnclosingMark) + 1);
		tempCategories[L"Me"] =  UtilsString::StringInitSpe(groups[2]); // Enclosing
		groups[3] = static_cast<wchar_t>(static_cast<int>(UnicodeCategory::NonSpacingMark) + 1);
		tempCategories[L"Mn"] =  UtilsString::StringInitSpe(groups[3]); // Non-spacing

		groups[4] = GroupChar;
		tempCategories[L"M"] = UtilsString::wstring_convert_from_bytes(groups, 0, 5);
		//word.Append(groups[1]);
		word.AppendChar(groups[3]);

		// Numbers
		groups[1] = static_cast<wchar_t>(static_cast<int>(UnicodeCategory::DecimalDigitNumber) + 1);
		tempCategories[L"Nd"] =  UtilsString::StringInitSpe(groups[1]); // Decimal digit
		groups[2] = static_cast<wchar_t>(static_cast<int>(UnicodeCategory::LetterNumber) + 1);
		tempCategories[L"Nl"] =  UtilsString::StringInitSpe(groups[2]); // Letter
		groups[3] = static_cast<wchar_t>(static_cast<int>(UnicodeCategory::OtherNumber) + 1);
		tempCategories[L"No"] =  UtilsString::StringInitSpe(groups[3]); // Other

		//groups[4] = GroupChar;
		tempCategories[L"N"] = UtilsString::wstring_convert_from_bytes(groups, 0, 5);
		word.AppendChar(groups[1]);
		//word.Append(new String(groups, 1, 3));

		// Punctuation
		groups[1] = static_cast<wchar_t>(static_cast<int>(UnicodeCategory::ConnectorPunctuation) + 1);
		tempCategories[L"Pc"] =  UtilsString::StringInitSpe(groups[1]); // Connector
		groups[2] = static_cast<wchar_t>(static_cast<int>(UnicodeCategory::DashPunctuation) + 1);
		tempCategories[L"Pd"] =  UtilsString::StringInitSpe(groups[2]); // Dash
		groups[3] = static_cast<wchar_t>(static_cast<int>(UnicodeCategory::ClosePunctuation) + 1);
		tempCategories[L"Pe"] =  UtilsString::StringInitSpe(groups[3]); // Close
		groups[4] = static_cast<wchar_t>(static_cast<int>(UnicodeCategory::OtherPunctuation) + 1);
		tempCategories[L"Po"] =  UtilsString::StringInitSpe(groups[4]); // Other
		groups[5] = static_cast<wchar_t>(static_cast<int>(UnicodeCategory::OpenPunctuation) + 1);
		tempCategories[L"Ps"] =  UtilsString::StringInitSpe(groups[5]); // Open
		groups[6] = static_cast<wchar_t>(static_cast<int>(UnicodeCategory::FinalQuotePunctuation) + 1);
		tempCategories[L"Pf"] =  UtilsString::StringInitSpe(groups[6]); // Inital quote
		groups[7] = static_cast<wchar_t>(static_cast<int>(UnicodeCategory::InitialQuotePunctuation) + 1);
		tempCategories[L"Pi"] =  UtilsString::StringInitSpe(groups[7]); // Final quote

		groups[8] = GroupChar;
		tempCategories[L"P"] = UtilsString::wstring_convert_from_bytes(groups, 0, 9);
		word.AppendChar(groups[1]);

		// Symbols
		groups[1] = static_cast<wchar_t>(static_cast<int>(UnicodeCategory::CurrencySymbol) + 1);
		tempCategories[L"Sc"] =  UtilsString::StringInitSpe(groups[1]); // Currency
		groups[2] = static_cast<wchar_t>(static_cast<int>(UnicodeCategory::ModifierSymbol) + 1);
		tempCategories[L"Sk"] =  UtilsString::StringInitSpe(groups[2]); // Modifier
		groups[3] = static_cast<wchar_t>(static_cast<int>(UnicodeCategory::MathSymbol) + 1);
		tempCategories[L"Sm"] =  UtilsString::StringInitSpe(groups[3]); // Math
		groups[4] = static_cast<wchar_t>(static_cast<int>(UnicodeCategory::OtherSymbol) + 1);
		tempCategories[L"So"] =  UtilsString::StringInitSpe(groups[4]); // Other

		groups[5] = GroupChar;
		tempCategories[L"S"] = UtilsString::wstring_convert_from_bytes(groups, 0, 6);

		// Separators
		groups[1] = static_cast<wchar_t>(static_cast<int>(UnicodeCategory::LineSeparator) + 1);
		tempCategories[L"Zl"] =  UtilsString::StringInitSpe(groups[1]); // Line
		groups[2] = static_cast<wchar_t>(static_cast<int>(UnicodeCategory::ParagraphSeparator) + 1);
		tempCategories[L"Zp"] =  UtilsString::StringInitSpe(groups[2]); // Paragraph
		groups[3] = static_cast<wchar_t>(static_cast<int>(UnicodeCategory::SpaceSeparator) + 1);
		tempCategories[L"Zs"] =  UtilsString::StringInitSpe(groups[3]); // Space

		groups[4] = GroupChar;
		tempCategories[L"Z"] = UtilsString::wstring_convert_from_bytes(groups, 0, 5);


		word.AppendChar(GroupChar);
		Word = word.ToString();
		NotWord = NegateCategory(Word);


		SpaceClass = UtilsString::StringInitSpe(0x00, 0x00, 0x01, 0x0064);
		NotSpaceClass = UtilsString::StringInitSpe(0x01, 0x00, 0x01, 0x0064);
		WordClass = UtilsString::StringInitSpe(0x00, 0x00, Word.length()) + Word;
		NotWordClass = UtilsString::StringInitSpe(0x01, 0x00, Word.length()) + Word;
		DigitClass = UtilsString::StringInitSpe(0x00, 0x00, 0x01, static_cast<int>(UnicodeCategory::DecimalDigitNumber) + 1);
		NotDigitClass = UtilsString::StringInitSpe(0x00, 0x00, 0x01, -(static_cast<int>(UnicodeCategory::DecimalDigitNumber) + 1));


		_definedCategories = tempCategories;
	}

	RegexCharClass::LowerCaseMapping::LowerCaseMapping()
	{
	}

	RegexCharClass::StaticConstructor RegexCharClass::staticConstructor;

	RegexCharClass::~RegexCharClass()
	{
		delete _categories;
		delete _subtractor;

		std::unordered_set<std::uintptr_t> release;
		for (size_t i = 0; i < _rangelistPtr.size(); ++i)
		{
			auto address = _rangelistPtr[i]._address;
			bool need = true;
			const bool is_in = release.find(address) != release.end();
			if (is_in)
				need = false;

			if (need)
			{
				if (_rangelistPtr[i].object)
				{
					release.insert(address);
					delete _rangelistPtr[i].object;
				}
			}
		}
	}

	RegexCharClass::RegexCharClass(): _subtractor(NULL)
	{
		_rangelist = std::vector<SingleRange*>();
		_rangelist.reserve(6);
		_canonical = true;
		_categories = new StringBuilder();
	}

	RegexCharClass::RegexCharClass(bool negate, std::vector<SingleRange>& ranges, StringBuilder* categories, RegexCharClass* subtraction)
	{
		for (size_t i = 0; i < ranges.size(); ++i)
		{
			auto t = new SingleRange(ranges[i]);
			_rangelist.push_back(t) ;
			_rangelistPtr.push_back(PtrObjectForRegex(t));
		}
		
		_categories = categories;
		_canonical = true;
		_negate = negate;
		_subtractor = subtraction;
	}

	bool RegexCharClass::getCanMerge() const
	{
		return !_negate && _subtractor == NULL;
	}

	void RegexCharClass::setNegate(bool value)
	{
		_negate = value;
	}

	void RegexCharClass::AddChar(wchar_t c)
	{
		AddRange(c, c);
	}

	void RegexCharClass::AddCharClass( RegexCharClass* cc)
	{
		assert((cc->getCanMerge() && this->getCanMerge(), L"Both character classes added together must be able to merge"));

		if (!cc->_canonical)
		{
			// if the new char class to add isn't canonical, we're not either.
			_canonical = false;
		}
		else if (_canonical && RangeCount() > 0 && cc->RangeCount() > 0 && cc->GetRangeAt(0)->_first <= GetRangeAt(RangeCount() - 1)->_last)
		{
			_canonical = false;
		}

		for (int i = 0; i < cc->RangeCount(); i += 1)
		{
			SingleRange* single = cc->GetRangeAt(i);
			SingleRange* tempVar = new SingleRange(single->_first, single->_last);

			_rangelistPtr.push_back(PtrObjectForRegex(tempVar));
			_rangelist.push_back(tempVar);
		}

		_categories->Append(cc->_categories->ToString());
	}

	void RegexCharClass::AddSet(const std::wstring& set)
	{
		size_t i;

		if (_canonical && RangeCount() > 0 && set.length() > 0 && set[0] <= GetRangeAt(RangeCount() - 1)->_last)
		{
			_canonical = false;
		}

		for (i = 0; i < set.length() - 1; i += 2)
		{
			SingleRange* tempVar = new SingleRange(set[i], static_cast<wchar_t>(set[i + 1] - 1));
			_rangelistPtr.push_back(PtrObjectForRegex(tempVar));
			_rangelist.push_back(tempVar);
		}

		if (i < set.length())
		{
			SingleRange* tempVar2 = new SingleRange(set[i], Lastchar);
			_rangelistPtr.push_back(PtrObjectForRegex(tempVar2));
			_rangelist.push_back(tempVar2);
		}
	}

	void RegexCharClass::AddSubtraction(RegexCharClass* sub)
	{
		assert((_subtractor == NULL, L"Can't add two subtractions to a char class. "));
		_subtractor = sub;
	}

	void RegexCharClass::AddRange(wchar_t first, wchar_t last)
	{
		SingleRange* tempVar = new SingleRange(first, last);
		_rangelistPtr.push_back(PtrObjectForRegex(tempVar));
		_rangelist.push_back(tempVar);
		if (_canonical && _rangelist.size() > 0 && first <= _rangelist[_rangelist.size() - 1]->_last)
		{
			_canonical = false;
		}
	}

	void RegexCharClass::AddCategoryFromName(const std::wstring& categoryName, bool invert, bool caseInsensitive, const std::wstring& pattern)
	{
		std::wstring cat;
		std::unordered_map<std::wstring, std::wstring>::const_iterator _definedCategories_iterator = _definedCategories.find(categoryName);
		if (_definedCategories_iterator != _definedCategories.end())
		{
			cat = _definedCategories_iterator->second;
		}

		if (cat != L"" && categoryName != InternalRegexIgnoreCase)
		{
			std::wstring catstr = cat;

			if (caseInsensitive)
			{
				if (categoryName == L"Ll" || categoryName == L"Lu" || categoryName == L"Lt")
				{
					// when RegexOptions.IgnoreCase is specified then {Ll}, {Lu}, and {Lt} cases should all match
					catstr = static_cast<std::wstring>(_definedCategories[InternalRegexIgnoreCase]);
				}
			}

			if (invert)
			{
				catstr = NegateCategory(catstr); // negate the category
			}

			_categories->Append(static_cast<std::wstring>(catstr));
		}
		else
		{
			AddSet(SetFromProperty(categoryName, invert, pattern));
		}
	}

	void RegexCharClass::AddCategory(const std::wstring& category)
	{
		_categories->Append(category);
	}

	void RegexCharClass::AddLowercase(ComCultureInfo* culture)
	{
		int i;
		int origSize;
		SingleRange* range;

		_canonical = false;

		for (i = 0, origSize = _rangelist.size(); i < origSize; i++)
		{
			range = _rangelist[i];
			if (range->_first == range->_last)
			{
				range->_first = range->_last = UtilsCharInternal::ToLower(range->_first, culture);
			}
			else
			{
				AddLowercaseRange(range->_first, range->_last, culture);
			}
		}
	}

	void RegexCharClass::AddLowercaseRange(wchar_t chMin, wchar_t chMax, ComCultureInfo* culture)
	{
		size_t i, iMax, iMid;
		wchar_t chMinT, chMaxT;
		LowerCaseMapping lc;

		for (i = 0, iMax = _lcTable.size(); i < iMax;)
		{
			iMid = (i + iMax) / 2;
			if (_lcTable[iMid]._chMax < chMin)
			{
				i = iMid + 1;
			}
			else
			{
				iMax = iMid;
			}
		}

		if (i >= _lcTable.size())
		{
			return;
		}

		for (; i < _lcTable.size() && (lc = _lcTable[i])._chMin <= chMax; i++)
		{
			if ((chMinT = lc._chMin) < chMin)
			{
				chMinT = chMin;
			}

			if ((chMaxT = lc._chMax) > chMax)
			{
				chMaxT = chMax;
			}

			if (lc._lcOp == LowercaseSet)
			{
				chMinT = static_cast<wchar_t>(lc._data);
				chMaxT = static_cast<wchar_t>(lc._data);
			}
			else if (lc._lcOp == LowercaseAdd)
			{
				chMinT += static_cast<wchar_t>(lc._data);
				chMaxT += static_cast<wchar_t>(lc._data);
			}
			else if (lc._lcOp == LowercaseBor)
			{
				chMinT |= static_cast<wchar_t>(1);
				chMaxT |= static_cast<wchar_t>(1);
			}
			else if (lc._lcOp == LowercaseBad)
			{
				chMinT += static_cast<wchar_t>(chMinT & 1);
				chMaxT += static_cast<wchar_t>(chMaxT & 1);
			}

			if (chMinT < chMin || chMaxT > chMax)
			{
				AddRange(chMinT, chMaxT);
			}
		}
	}

	void RegexCharClass::AddWord(bool ecma, bool negate)
	{
		if (negate)
		{
			if (ecma)
			{
				AddSet(RegexCharClass::NotECMAWordSet);
			}
			else
			{
				AddCategory(RegexCharClass::NotWord);
			}
		}
		else
		{
			if (ecma)
			{
				AddSet(RegexCharClass::ECMAWordSet);
			}
			else
			{
				AddCategory(RegexCharClass::Word);
			}
		}
	}

	void RegexCharClass::AddSpace(bool ecma, bool negate)
	{
		if (negate)
		{
			if (ecma)
			{
				AddSet(RegexCharClass::NotECMASpaceSet);
			}
			else
			{
				AddCategory(RegexCharClass::NotSpace);
			}
		}
		else
		{
			if (ecma)
			{
				AddSet(RegexCharClass::ECMASpaceSet);
			}
			else
			{
				AddCategory(RegexCharClass::Space);
			}
		}
	}

	void RegexCharClass::AddDigit(bool ecma, bool negate, const std::wstring& pattern)
	{
		if (ecma)
		{
			if (negate)
			{
				AddSet(RegexCharClass::NotECMADigitSet);
			}
			else
			{
				AddSet(RegexCharClass::ECMADigitSet);
			}
		}
		else
		{
			AddCategoryFromName(L"Nd", negate, false, pattern);
		}
	}

	std::wstring RegexCharClass::ConvertOldStringsToClass(const std::wstring& set, const std::wstring& category)
	{
		StringBuilder sb = StringBuilder(set.length() + category.length() + 3);

		if (set.length() >= 2 && set[0] == L'\0' && set[1] == L'\0')
		{
			sb.AppendChar(static_cast<wchar_t>(0x1));
			sb.AppendChar(static_cast<wchar_t>(set.length() - 2));
			sb.AppendChar(static_cast<wchar_t>(category.length()));
			sb.Append(set.substr(2));
		}
		else
		{
			sb.AppendChar(static_cast<wchar_t>(0x0));
			sb.AppendChar(static_cast<wchar_t>(set.length()));
			sb.AppendChar(static_cast<wchar_t>(category.length()));
			sb.Append(set);
		}

		sb.Append(category);

		return sb.ToString();
	}

	wchar_t RegexCharClass::SingletonChar(const std::wstring& set)
	{
		assert((IsSingleton(set) || IsSingletonInverse(set), L"Tried to get the singleton char out of a non singleton character class"));
		return set[SETSTART];
	}

	bool RegexCharClass::IsMergeable(const std::wstring& charClass)
	{
		return (!IsNegated(charClass) && !IsSubtraction(charClass));
	}

	bool RegexCharClass::IsEmpty(const std::wstring& charClass)
	{
		if (charClass[CATEGORYLENGTH] == 0 && charClass[FLAGS] == 0 && charClass[SETLENGTH] == 0 && !IsSubtraction(charClass))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool RegexCharClass::IsSingleton(const std::wstring& set)
	{
		if (set[FLAGS] == 0 && set[CATEGORYLENGTH] == 0 && set[SETLENGTH] == 2 && !IsSubtraction(set) && (set[SETSTART] == Lastchar || set[SETSTART] + 1 == set[SETSTART + 1]))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool RegexCharClass::IsSingletonInverse(const std::wstring& set)
	{
		if (set[FLAGS] == 1 && set[CATEGORYLENGTH] == 0 && set[SETLENGTH] == 2 && !IsSubtraction(set) && (set[SETSTART] == Lastchar || set[SETSTART] + 1 == set[SETSTART + 1]))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool RegexCharClass::IsSubtraction(const std::wstring& charClass)
	{
		auto t1 = charClass[SETLENGTH];
		auto t2 = charClass[CATEGORYLENGTH];
		return (charClass.length() > (size_t)SETSTART + t1 + t2);
	}

	bool RegexCharClass::IsNegated(const std::wstring& set)
	{
		return (set != L"" && set[FLAGS] == 1);
	}

	bool RegexCharClass::IsECMAWordChar(wchar_t ch)
	{
		// According to ECMA-262, \s, \S, ., ^, and $ use Unicode-based interpretations of
		// whitespace and newline, while \d, \D\, \w, \W, \b, and \B use ASCII-only 
		// interpretations of digit, word character, and word boundary.  In other words,
		// no special treatment of Unicode ZERO WIDTH NON-JOINER (ZWNJ U+200C) and 
		// ZERO WIDTH JOINER (ZWJ U+200D) is required for ECMA word boundaries.
		return CharInClass(ch, ECMAWordClass);
	}

	bool RegexCharClass::IsWordChar(wchar_t ch)
	{
		// According to UTS#18 Unicode Regular Expressions (http://www.unicode.org/reports/tr18/)
		// RL 1.4 Simple Word Boundaries  The class of <word_character> includes all Alphabetic
		// values from the Unicode character database, from UnicodeData.txt [UData], plus the U+200C
		// ZERO WIDTH NON-JOINER and U+200D ZERO WIDTH JOINER.
		return CharInClass(ch, WordClass) || ch == ZeroWidthJoiner || ch == ZeroWidthNonJoiner;
	}

	bool RegexCharClass::CharInClass(wchar_t ch, const std::wstring& set)
	{
		return CharInClassRecursive(ch, set, 0);
	}

	bool RegexCharClass::CharInClassRecursive(wchar_t ch, const std::wstring& set, int start)
	{
		int mySetLength = set[start + SETLENGTH];
		int myCategoryLength = set[start + CATEGORYLENGTH];
		int myEndPosition = start + SETSTART + mySetLength + myCategoryLength;

		bool subtracted = false;

		if (set.length() > (size_t)myEndPosition)
		{
			subtracted = CharInClassRecursive(ch, set, myEndPosition);
		}

		bool b = CharInClassInternal(ch, set, start, mySetLength, myCategoryLength);

		// Note that we apply the negation *before* performing the subtraction.  This is because
		// the negation only applies to the first char class, not the entire subtraction. 
		if (set[start + FLAGS] == 1)
		{
			b = !b;
		}

		return b && !subtracted;
	}

	bool RegexCharClass::CharInClassInternal(wchar_t ch, const std::wstring& set, int start, int mySetLength, int myCategoryLength)
	{
		int min;
		int max;
		int mid;
		min = start + SETSTART;
		max = min + mySetLength;

		while (min != max)
		{
			mid = (min + max) / 2;
			if (ch < set[mid])
			{
				max = mid;
			}
			else
			{
				min = mid + 1;
			}
		}

		// The starting position of the set within the character class determines
		// whether what an odd or even ending position means.  If the start is odd, 
		// an *even* ending position means the character was in the set.  With recursive 
		// subtractions in the mix, the starting position = start+SETSTART.  Since we know that 
		// SETSTART is odd, we can simplify it out of the equation.  But if it changes we need to 
		// reverse this check. 
		assert(((SETSTART & 0x1) == 1, L"If SETSTART is not odd, the calculation below this will be reversed"));
		if ((min & 0x1) == (start & 0x1))
		{
			return true;
		}
		else
		{
			if (myCategoryLength == 0)
			{
				return false;
			}

			return CharInCategory(ch, set, start, mySetLength, myCategoryLength);
		}
	}

	bool RegexCharClass::CharInCategory(wchar_t ch, const std::wstring& set, int start, int mySetLength, int myCategoryLength)
	{
		UnicodeCategory chcategory = CharUnicodeInfo::GetUnicodeCategory(ch);

		int i = start + SETSTART + mySetLength;
		int end = i + myCategoryLength;
		while (i < end)
		{
			int curcat = static_cast<short>(set[i]);

			if (curcat == 0)
			{
				// zero is our marker for a group of categories - treated as a unit
				if (CharInCategoryGroup(ch, chcategory, set, i))
				{
					return true;
				}
			}
			else if (curcat > 0)
			{
				// greater than zero is a positive case

				if (curcat == SpaceConst)
				{
					if (UtilsChar::IsWhiteSpace(ch))
					{
						return true;
					}
					else
					{
						i++;
						continue;
					}
				}

				--curcat;

				if (chcategory == static_cast<UnicodeCategory>(curcat))
				{
					return true;
				}
			}
			else
			{
				// less than zero is a negative case
				if (curcat == NotSpaceConst)
				{
					if (!UtilsChar::IsWhiteSpace(ch))
					{
						return true;
					}
					else
					{
						i++;
						continue;
					}
				}

				//curcat = -curcat;
				//--curcat;
				curcat = -1 - curcat;

				if (chcategory != static_cast<UnicodeCategory>(curcat))
				{
					return true;
				}
			}

			i++;
		}

		return false;
	}

	bool RegexCharClass::CharInCategoryGroup(wchar_t ch, UnicodeCategory chcategory, const std::wstring& category, int& i)
	{
		i++;

		int curcat = static_cast<short>(category[i]);
		if (curcat > 0)
		{
			// positive case - the character must be in ANY of the categories in the group
			bool answer = false;

			while (curcat != 0)
			{
				if (!answer)
				{
					--curcat;
					if (chcategory == static_cast<UnicodeCategory>(curcat))
					{
						answer = true;
					}
				}

				i++;
				curcat = static_cast<short>(category[i]);
			}

			return answer;
		}
		else
		{
			// negative case - the character must be in NONE of the categories in the group
			bool answer = true;

			while (curcat != 0)
			{
				if (answer)
				{
					//curcat = -curcat;
					//--curcat;
					curcat = -1 - curcat;
					if (chcategory == static_cast<UnicodeCategory>(curcat))
					{
						answer = false;
					}
				}

				i++;
				curcat = static_cast<short>(category[i]);
			}

			return answer;
		}
	}

	std::wstring RegexCharClass::NegateCategory(const std::wstring& category)
	{
		if (category == L"")
		{
			return L"";
		}

		std::wstring sb = category;
		for (size_t i = 0; i < category.length(); i++)
		{
			short ch = static_cast<short>(category[i]);
			sb[i] = static_cast<wchar_t>(-ch);
		}

		auto temp = sb;
		return temp;
	}

	RegexCharClass* RegexCharClass::Parse(const std::wstring& charClass)
	{
		return ParseRecursive(charClass, 0);
	}

	RegexCharClass* RegexCharClass::ParseRecursive(const std::wstring& charClass, int start)
	{
		int mySetLength = charClass[start + SETLENGTH];
		int myCategoryLength = charClass[start + CATEGORYLENGTH];
		int myEndPosition = start + SETSTART + mySetLength + myCategoryLength;

		std::vector<SingleRange> ranges;
		ranges.reserve(mySetLength);

		int i = start + SETSTART;
		int end = i + mySetLength;
		while (i < end)
		{
			wchar_t first = charClass[i];
			i++;

			wchar_t last;
			if (i < end)
			{
				last = static_cast<wchar_t>(charClass[i] - 1);
			}
			else
			{
				last = Lastchar;
			}
			i++;
			SingleRange tempVar =  SingleRange(first, last);
			ranges.push_back(tempVar);
		}

		RegexCharClass* sub = NULL;
		if (charClass.length() > (size_t)myEndPosition)
		{
			sub = ParseRecursive(charClass, myEndPosition);
		}

		StringBuilder* tempVar2 = new StringBuilder(charClass.substr(end, myCategoryLength));
		return new RegexCharClass(charClass[start + FLAGS] == 1, ranges, tempVar2, sub);
	}

	int RegexCharClass::RangeCount()
	{
		return _rangelist.size();
	}

	std::wstring RegexCharClass::ToStringClass()
	{
		if (!_canonical)
		{
			Canonicalize();
		}

		// make a guess about the length of the ranges.  We'll update this at the end. 
		// This is important because if the last range ends in LastChar, we won't append
		// LastChar to the list. 
		int rangeLen = _rangelist.size() * 2;
		StringBuilder sb = StringBuilder(rangeLen + _categories->GetLength() + 3);

		int flags;
		if (_negate)
		{
			flags = 1;
		}
		else
		{
			flags = 0;
		}

		sb.AppendChar(static_cast<wchar_t>(flags));
		sb.AppendChar(static_cast<wchar_t>(rangeLen));
		sb.AppendChar(static_cast<wchar_t>(_categories->GetLength()));

		for (size_t i = 0; i < _rangelist.size(); i++)
		{
			SingleRange* currentRange = _rangelist[i];
			sb.AppendChar(currentRange->_first);

			if (currentRange->_last != Lastchar)
			{
				sb.AppendChar(static_cast<wchar_t>(currentRange->_last + 1));
			}
		}

		sb.Modify(SETLENGTH, static_cast<wchar_t>(sb.GetLength() - SETSTART));

		sb.Append(_categories->ToString());

		if (_subtractor != NULL)
		{
			sb.Append(_subtractor->ToStringClass());
		}


		auto result = sb.ToString();
		return result;
	}

	RegexCharClass::SingleRange* RegexCharClass::GetRangeAt(int i)
	{
		return _rangelist[i];
	}

	void RegexCharClass::Canonicalize()
	{
		SingleRange* CurrentRange;
		int i;
		int j;
		wchar_t last;
		bool Done;

		_canonical = true;
		SingleRangeComparer tempVar;
		//_rangelist.Sort(0, _rangelist.size(), &tempVar);
		UtilsCollections::Sort<SingleRange*>(&_rangelist, 0, _rangelist.size(), &tempVar);


		//
		// Find and eliminate overlapping or abutting ranges
		//

		if (_rangelist.size() > 1)
		{
			Done = false;

			for (i = 1, j = 0;; i++)
			{
				for (last = _rangelist[j]->_last;; i++)
				{
					if (i == _rangelist.size() || last == Lastchar)
					{
						Done = true;
						break;
					}

					if ((CurrentRange = _rangelist[i])->_first > last + 1)
					{
						break;
					}

					if (last < CurrentRange->_last)
					{
						last = CurrentRange->_last;
					}
				}

				_rangelist[j]->_last = last;

				j++;

				if (Done)
				{
					break;
				}

				if (j < i)
				{
					_rangelist[j] = _rangelist[i];
				}
			}

			//_rangelist.RemoveRange(j, _rangelist.size() - j);

			size_t temp_count = _rangelist.size() - j;
			_rangelist.erase(_rangelist.begin() + j, _rangelist.begin() + j + temp_count);
		}
	}

	std::wstring RegexCharClass::SetFromProperty(const std::wstring& capname, bool invert, const std::wstring& pattern)
	{
		int min = 0;
		int max = _propTable.size();
		while (min != max)
		{
			int mid = (min + max) / 2;
			int res = UtilsString::Compare(capname, _propTable[mid][0], StringComparison::Ordinal);
			if (res < 0)
			{
				max = mid;
			}
			else if (res > 0)
			{
				min = mid + 1;
			}
			else
			{
				std::wstring set = _propTable[mid][1];
				assert((!set.empty(), L"Found a null/empty element in RegexCharClass prop table"));
				if (invert)
				{
					if (set[0] == Nullchar)
					{
						return set.substr(1);
					}

					return  UtilsString::StringInitSpe(Nullchar) + set;
				}
				else { return set; }
			}
		}


		std::string temp = "MakeException UnknownProperty";
		temp += std::string(capname.begin(), capname.end());
		throw std::invalid_argument(temp);
	}

	std::wstring RegexCharClass::SetDescription(std::wstring set)
	{
		int mySetLength = set[SETLENGTH];
		int myCategoryLength = set[CATEGORYLENGTH];
		int myEndPosition = SETSTART + mySetLength + myCategoryLength;

		StringBuilder desc = StringBuilder();
		desc.Append(L"[");


		//int index = SETSTART;
		//char ch1;
		//char ch2;

		//if (IsNegated(set))
		//	desc.AppendChar('^');

		//while (index < SETSTART + set[SETLENGTH])
		//{
		//	ch1 = set[index];
		//	if (index + 1 < set.length())
		//		ch2 = (char)(set[index + 1] - 1);
		//	else
		//		ch2 = Lastchar;

		//	desc.Append(CharDescription(ch1));

		//	if (ch2 != ch1)
		//	{
		//		if (ch1 + 1 != ch2)
		//			desc.AppendChar('-');
		//		desc.Append(CharDescription(ch2));
		//	}
		//	index += 2;
		//}

		//while (index < SETSTART + set[SETLENGTH] + set[CATEGORYLENGTH])
		//{
		//	ch1 = set[index];
		//	if (ch1 == 0)
		//	{
		//		bool found = false;

		//		int lastindex = set.IndexOf(GroupChar, index + 1);
		//		std::wstring group = set.substr(index, lastindex - index + 1);

		//		IDictionaryEnumerator en = _definedCategories.GetEnumerator();
		//		while (en.MoveNext())
		//		{
		//			if (group == (en.Value))
		//			{
		//				if ((short)set[index + 1] > 0)
		//					desc.Append("\\p{" + en.Key + "}");
		//				else
		//					desc.Append("\\P{" + en.Key + "}");

		//				found = true;
		//				break;
		//			}
		//		}

		//		if (!found)
		//		{
		//			if (group == (Word))
		//				desc.Append(L"\\w");
		//			else if (group == (NotWord))
		//				desc.Append(L"\\W");
		//			else
		//				Debug.Assert(false, "Couldn't find a goup to match '" + group + "'");
		//		}

		//		index = lastindex;
		//	}
		//	else
		//	{
		//		desc.Append(CategoryDescription(ch1));
		//	}

		//	index++;
		//}

		//if (set.Length > myEndPosition)
		//{
		//	desc.AppendChar('-');
		//	desc.Append(SetDescription(set.Substring(myEndPosition)));
		//}

		//desc.AppendChar(']');

		return desc.ToString();
	}

	std::wstring RegexCharClass::CharDescription(wchar_t ch)
	{
		StringBuilder sb = StringBuilder();
		int shift;

		if (ch == '\\')
			return L"\\\\";

		if (ch >= ' ' && ch <= '~')
		{
			sb.AppendChar(ch);
			return sb.ToString();
		}

		if (ch < 256)
		{
			sb.Append(L"\\x");
			shift = 8;
		}
		else
		{
			sb.Append(L"\\u");
			shift = 16;
		}

		while (shift > 0)
		{
			shift -= 4;
			sb.AppendChar(Hex[(ch >> shift) & 0xF]);
		}

		return sb.ToString();
	}

	std::wstring RegexCharClass::CategoryDescription(wchar_t ch)
	{
		if (ch == SpaceConst)
			return L"\\s";
		else if ((short)ch == NotSpaceConst)
			return L"\\S";
		else if ((short)ch < 0)
		{
			return L"\\P{" + Categories[(-((short)ch) - 1)] + L"}";
		}
		else
		{
			return L"\\p{" + Categories[(ch - 1)] + L"}";
		}
	}

	RegexCharClass::LowerCaseMapping::LowerCaseMapping(wchar_t chMin, wchar_t chMax, int lcOp, int data)
	{
		_chMin = chMin;
		_chMax = chMax;
		_lcOp = lcOp;
		_data = data;
	}

	int RegexCharClass::SingleRangeComparer::Compare(SingleRange* x, SingleRange* y)
	{
		return ((x)->_first < (y)->_first ? -1 : ((x)->_first > (y)->_first ? 1 : 0));
	}

	RegexCharClass::SingleRange::SingleRange(wchar_t first, wchar_t last)
	{
		_first = first;
		_last = last;
	}
}
