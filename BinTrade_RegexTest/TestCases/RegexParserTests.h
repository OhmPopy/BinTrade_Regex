#pragma once

#include "RegexParseError.h"
#include "UnitTestAssert.h"

using namespace Regexs;

namespace RegexsTest
{
	class RegexParserTests
	{
	public:
		static void Run(bool test_throw)
		{
			Assert::WriteLine(L"RegexParserTests");
			if (!test_throw) return;


			ParseCheckOffset(L"", Regexs::RegexOptions::None);
			ParseCheckOffset(L" ", RegexOptions::IgnorePatternWhitespace);
			ParseCheckOffset(L"  ", RegexOptions::IgnorePatternWhitespace);
			ParseCheckOffset(L"(?#)", RegexOptions::None);
			ParseCheckOffset(L"(?# )", RegexOptions::None);
			ParseCheckOffset(L"(?#", RegexOptions::None, RegexParseError::UnterminatedComment, 3);
			ParseCheckOffset(L"(?# ", RegexOptions::None, RegexParseError::UnterminatedComment, 4);
			ParseCheckOffset(L"(?#)(?#)", RegexOptions::None);
			ParseCheckOffset(L"(?#)(?#)", RegexOptions::IgnorePatternWhitespace);
			ParseCheckOffset(L"(?#) (?#)", RegexOptions::None);
			ParseCheckOffset(L"(?#) (?#)", RegexOptions::IgnorePatternWhitespace);
			ParseCheckOffset(L"[a\\p{Lu}(?#)b]", RegexOptions::None);
			ParseCheckOffset(L"[a\\0(?#)b]", RegexOptions::None);
			ParseCheckOffset(L"[a\\a(?#)b]", RegexOptions::None);
			ParseCheckOffset(L"[a\\x00(?#)b]", RegexOptions::None);
			ParseCheckOffset(L"[a\\u0000(?#)b]", RegexOptions::None);
			ParseCheckOffset(L"[a\\](?#)b]", RegexOptions::None);
			ParseCheckOffset(L"(?", RegexOptions::None, RegexParseError::InvalidGroupingConstruct, 2);
			ParseCheckOffset(L"(?", RegexOptions::IgnorePatternWhitespace, RegexParseError::InvalidGroupingConstruct, 2);
			ParseCheckOffset(L"(? ", RegexOptions::None, RegexParseError::InvalidGroupingConstruct, 3);
			ParseCheckOffset(L"(? ", RegexOptions::IgnorePatternWhitespace, RegexParseError::InvalidGroupingConstruct, 3);
			ParseCheckOffset(L"(?i)", RegexOptions::None);
			ParseCheckOffset(L"(?im)", RegexOptions::None);
			ParseCheckOffset(L"(?im-x)", RegexOptions::None);
			ParseCheckOffset(L"(?im-x+n)", RegexOptions::None);
			ParseCheckOffset(L"(?i) ", RegexOptions::None);
			ParseCheckOffset(L"(?x) ", RegexOptions::None);
			ParseCheckOffset(L" (?x) ", RegexOptions::None);
			ParseCheckOffset(L" (?-x) ", RegexOptions::IgnorePatternWhitespace);
			ParseCheckOffset(L" ( (?-x) ) ", RegexOptions::IgnorePatternWhitespace);
			ParseCheckOffset(L" (?-x:) ", RegexOptions::IgnorePatternWhitespace);
			ParseCheckOffset(L" (?-x: ) ", RegexOptions::IgnorePatternWhitespace);
			ParseCheckOffset(L" (?-x: (?+x: ) ) ", RegexOptions::IgnorePatternWhitespace);
			ParseCheckOffset(L"(?-x", RegexOptions::IgnorePatternWhitespace, RegexParseError::InvalidGroupingConstruct, 4);
			ParseCheckOffset(L"(?-x ", RegexOptions::IgnorePatternWhitespace, RegexParseError::InvalidGroupingConstruct, 5);
			ParseCheckOffset(L"(?-x :", RegexOptions::IgnorePatternWhitespace, RegexParseError::InvalidGroupingConstruct, 5);
			ParseCheckOffset(L"(?-x )", RegexOptions::IgnorePatternWhitespace, RegexParseError::InvalidGroupingConstruct, 5);
			ParseCheckOffset(L"(?-x :)", RegexOptions::IgnorePatternWhitespace, RegexParseError::InvalidGroupingConstruct, 5);
			ParseCheckOffset(L")", RegexOptions::None, RegexParseError::InsufficientOpeningParentheses, 1);
			ParseCheckOffset(L"a", RegexOptions::None);
			ParseCheckOffset(L"ab", RegexOptions::None);
			ParseCheckOffset(L"a*", RegexOptions::None);
			ParseCheckOffset(L"a*?", RegexOptions::None);
			ParseCheckOffset(L"a+", RegexOptions::None);
			ParseCheckOffset(L"a+?", RegexOptions::None);
			ParseCheckOffset(L"a?", RegexOptions::None);
			ParseCheckOffset(L"a??", RegexOptions::None);
			ParseCheckOffset(L"()", RegexOptions::None);
			ParseCheckOffset(L"(a)", RegexOptions::None);
			ParseCheckOffset(L"(", RegexOptions::None, RegexParseError::InsufficientClosingParentheses, 1);
			ParseCheckOffset(L"(a", RegexOptions::None, RegexParseError::InsufficientClosingParentheses, 2);
			ParseCheckOffset(L"|", RegexOptions::None);
			ParseCheckOffset(L" |", RegexOptions::None);
			ParseCheckOffset(L"| ", RegexOptions::None);
			ParseCheckOffset(L" | ", RegexOptions::None);
			ParseCheckOffset(L"|", RegexOptions::IgnorePatternWhitespace);
			ParseCheckOffset(L" |", RegexOptions::IgnorePatternWhitespace);
			ParseCheckOffset(L"| ", RegexOptions::IgnorePatternWhitespace);
			ParseCheckOffset(L" | ", RegexOptions::IgnorePatternWhitespace);
			ParseCheckOffset(L"||", RegexOptions::None);
			ParseCheckOffset(L"(|)", RegexOptions::None);
			ParseCheckOffset(L"a{0}", RegexOptions::None);
			ParseCheckOffset(L"a{0,}", RegexOptions::None);
			ParseCheckOffset(L"a{0,1}", RegexOptions::None);
			ParseCheckOffset(L"a{1,0}", RegexOptions::None, RegexParseError::ReversedQuantifierRange, 6);
			ParseCheckOffset(L"a{0}?", RegexOptions::None);
			ParseCheckOffset(L"a{0,}?", RegexOptions::None);
			ParseCheckOffset(L"a{0,1}?", RegexOptions::None);
			ParseCheckOffset(L"a{", RegexOptions::None);
			ParseCheckOffset(L"a{0", RegexOptions::None);
			ParseCheckOffset(L"a{0,", RegexOptions::None);
			ParseCheckOffset(L"a{0,1", RegexOptions::None);
			ParseCheckOffset(L"a{0 }", RegexOptions::IgnorePatternWhitespace);
			ParseCheckOffset(L"a{0, }", RegexOptions::IgnorePatternWhitespace);
			ParseCheckOffset(L"a{0 ,}", RegexOptions::IgnorePatternWhitespace);
			ParseCheckOffset(L"a{0 ,1}", RegexOptions::IgnorePatternWhitespace);
			ParseCheckOffset(L"a{0, 1}", RegexOptions::IgnorePatternWhitespace);
			ParseCheckOffset(L"a{0,1 }", RegexOptions::IgnorePatternWhitespace);
			ParseCheckOffset(L"a* ?", RegexOptions::IgnorePatternWhitespace);
			ParseCheckOffset(L"a* ?", RegexOptions::None);
			ParseCheckOffset(L"*", RegexOptions::None, RegexParseError::QuantifierAfterNothing, 1);
			ParseCheckOffset(L"(*)", RegexOptions::None, RegexParseError::QuantifierAfterNothing, 2);
			ParseCheckOffset(L"a**", RegexOptions::None, RegexParseError::NestedQuantifiersNotParenthesized, 3);
			ParseCheckOffset(L"+", RegexOptions::None, RegexParseError::QuantifierAfterNothing, 1);
			ParseCheckOffset(L"(+)", RegexOptions::None, RegexParseError::QuantifierAfterNothing, 2);
			ParseCheckOffset(L"a*+", RegexOptions::None, RegexParseError::NestedQuantifiersNotParenthesized, 3);
			ParseCheckOffset(L"?", RegexOptions::None, RegexParseError::QuantifierAfterNothing, 1);
			ParseCheckOffset(L"(?)", RegexOptions::None, RegexParseError::QuantifierAfterNothing, 2);
			ParseCheckOffset(L"a*??", RegexOptions::None, RegexParseError::NestedQuantifiersNotParenthesized, 4);
			ParseCheckOffset(L"{0}", RegexOptions::None, RegexParseError::QuantifierAfterNothing, 1);
			ParseCheckOffset(L"({0})", RegexOptions::None, RegexParseError::QuantifierAfterNothing, 2);
			ParseCheckOffset(L"a*{0}", RegexOptions::None, RegexParseError::NestedQuantifiersNotParenthesized, 3);
			ParseCheckOffset(L"{0", RegexOptions::None);
			ParseCheckOffset(L"({0)", RegexOptions::None);
			ParseCheckOffset(L"a*{0", RegexOptions::None);
			ParseCheckOffset(L"\\w", RegexOptions::None);
			ParseCheckOffset(L"\\b\\B\\A\\G\\Z\\z\\w\\W\\s\\W\\s\\S\\d\\D", RegexOptions::None);
			ParseCheckOffset(L"\\c", RegexOptions::None, RegexParseError::MissingControlCharacter, 2);
			ParseCheckOffset(L"\\c<", RegexOptions::None, RegexParseError::UnrecognizedControlCharacter, 3);
			ParseCheckOffset(L"\\ca", RegexOptions::None);
			ParseCheckOffset(L"\\cA", RegexOptions::None);
			ParseCheckOffset(L"\\c A", RegexOptions::None, RegexParseError::UnrecognizedControlCharacter, 3);
			ParseCheckOffset(L"\\c(a)", RegexOptions::None, RegexParseError::UnrecognizedControlCharacter, 3);
			ParseCheckOffset(L"\\c>", RegexOptions::None, RegexParseError::UnrecognizedControlCharacter, 3);
			ParseCheckOffset(L"\\c?", RegexOptions::None, RegexParseError::UnrecognizedControlCharacter, 3);
			ParseCheckOffset(L"\\c@", RegexOptions::None);
			ParseCheckOffset(L"\\c^", RegexOptions::None);
			ParseCheckOffset(L"\\c_", RegexOptions::None);
			ParseCheckOffset(L"\\c`", RegexOptions::None, RegexParseError::UnrecognizedControlCharacter, 3);
			ParseCheckOffset(L"\\c{", RegexOptions::None, RegexParseError::UnrecognizedControlCharacter, 3);
			ParseCheckOffset(L"\\cz", RegexOptions::None);
			ParseCheckOffset(L"\\cZ", RegexOptions::None);
			ParseCheckOffset(L"\\m", RegexOptions::None, RegexParseError::UnrecognizedEscape, 2);
			ParseCheckOffset(L"\\x", RegexOptions::None, RegexParseError::InsufficientOrInvalidHexDigits, 2);
			ParseCheckOffset(L"\\x ", RegexOptions::None, RegexParseError::InsufficientOrInvalidHexDigits, 2);
			ParseCheckOffset(L"\\x0", RegexOptions::None, RegexParseError::InsufficientOrInvalidHexDigits, 2);
			ParseCheckOffset(L"\\x0 ", RegexOptions::None, RegexParseError::InsufficientOrInvalidHexDigits, 4);
			ParseCheckOffset(L"\\x00", RegexOptions::None);
			ParseCheckOffset(L"\\x00 ", RegexOptions::None);
			ParseCheckOffset(L"\\x000", RegexOptions::None);
			ParseCheckOffset(L"\\xff", RegexOptions::None);
			ParseCheckOffset(L"\\xFF", RegexOptions::None);
			ParseCheckOffset(L"\\xfF", RegexOptions::None);
			ParseCheckOffset(L"\\xfff", RegexOptions::None);
			ParseCheckOffset(L"\\xgg", RegexOptions::None, RegexParseError::InsufficientOrInvalidHexDigits, 3);
			ParseCheckOffset(L"\\m ", RegexOptions::None, RegexParseError::UnrecognizedEscape, 2);
			ParseCheckOffset(L"\\u", RegexOptions::None, RegexParseError::InsufficientOrInvalidHexDigits, 2);
			ParseCheckOffset(L"\\u0", RegexOptions::None, RegexParseError::InsufficientOrInvalidHexDigits, 2);
			ParseCheckOffset(L"\\u00", RegexOptions::None, RegexParseError::InsufficientOrInvalidHexDigits, 2);
			ParseCheckOffset(L"\\u000", RegexOptions::None, RegexParseError::InsufficientOrInvalidHexDigits, 2);
			ParseCheckOffset(L"\\u0000", RegexOptions::None);
			ParseCheckOffset(L"\\u0000 ", RegexOptions::None);
			ParseCheckOffset(L"\\u ", RegexOptions::None, RegexParseError::InsufficientOrInvalidHexDigits, 2);
			ParseCheckOffset(L"\\u0 ", RegexOptions::None, RegexParseError::InsufficientOrInvalidHexDigits, 2);
			ParseCheckOffset(L"\\ugggg", RegexOptions::None, RegexParseError::InsufficientOrInvalidHexDigits, 3);
			ParseCheckOffset(L"\\0", RegexOptions::None);
			ParseCheckOffset(L"\\0 ", RegexOptions::None);
			ParseCheckOffset(L"\\00", RegexOptions::None);
			ParseCheckOffset(L"\\00 ", RegexOptions::None);
			ParseCheckOffset(L"\\000", RegexOptions::None);
			ParseCheckOffset(L"\\000 ", RegexOptions::None);
			ParseCheckOffset(L"\\0000", RegexOptions::None);
			ParseCheckOffset(L"\\0000 ", RegexOptions::None);
			ParseCheckOffset(L"\\7", RegexOptions::None, RegexParseError::UndefinedNumberedReference, 2);
			ParseCheckOffset(L"\\78", RegexOptions::None);
			ParseCheckOffset(L"\\8", RegexOptions::None, RegexParseError::UndefinedNumberedReference, 2);
			ParseCheckOffset(L"\\40", RegexOptions::ECMAScript);
			ParseCheckOffset(L"\\401", RegexOptions::ECMAScript);
			ParseCheckOffset(L"\\37", RegexOptions::ECMAScript);
			ParseCheckOffset(L"\\371", RegexOptions::ECMAScript);
			ParseCheckOffset(L"\\0000", RegexOptions::ECMAScript);
			ParseCheckOffset(L"\\k", RegexOptions::None, RegexParseError::MalformedNamedReference, 1);
			ParseCheckOffset(L"\\k ", RegexOptions::None, RegexParseError::MalformedNamedReference, 3);
			ParseCheckOffset(L"\\k<", RegexOptions::None, RegexParseError::MalformedNamedReference, 3);
			ParseCheckOffset(L"\\k< ", RegexOptions::None, RegexParseError::UnrecognizedEscape, 2);
			ParseCheckOffset(L"\\k<0", RegexOptions::None, RegexParseError::UnrecognizedEscape, 2);
			ParseCheckOffset(L"\\k<0 ", RegexOptions::None, RegexParseError::UnrecognizedEscape, 2);
			ParseCheckOffset(L"\\k<0>", RegexOptions::None);
			ParseCheckOffset(L"\\k<0> ", RegexOptions::None);
			ParseCheckOffset(L"\\k<00> ", RegexOptions::None);
			ParseCheckOffset(L"\\k<a> ", RegexOptions::None, RegexParseError::UndefinedNamedReference, 5);
			ParseCheckOffset(L"(?<a>)\\k<a> ", RegexOptions::None);
			ParseCheckOffset(L"\\k", RegexOptions::ECMAScript, RegexParseError::MalformedNamedReference, 1);
			ParseCheckOffset(L"\\k ", RegexOptions::ECMAScript, RegexParseError::MalformedNamedReference, 3);
			ParseCheckOffset(L"\\k<", RegexOptions::ECMAScript, RegexParseError::MalformedNamedReference, 3);
			ParseCheckOffset(L"\\k< ", RegexOptions::ECMAScript);
			ParseCheckOffset(L"\\k<0", RegexOptions::ECMAScript);
			ParseCheckOffset(L"\\k<0 ", RegexOptions::ECMAScript);
			ParseCheckOffset(L"\\k<0>", RegexOptions::ECMAScript);
			ParseCheckOffset(L"\\k<0> ", RegexOptions::ECMAScript);
			ParseCheckOffset(L"\\k'", RegexOptions::None, RegexParseError::MalformedNamedReference, 3);
			ParseCheckOffset(L"\\k' ", RegexOptions::None, RegexParseError::UnrecognizedEscape, 2);
			ParseCheckOffset(L"\\k'0", RegexOptions::None, RegexParseError::UnrecognizedEscape, 2);
			ParseCheckOffset(L"\\k'0 ", RegexOptions::None, RegexParseError::UnrecognizedEscape, 2);
			ParseCheckOffset(L"\\k'0'", RegexOptions::None);
			ParseCheckOffset(L"\\k'0' ", RegexOptions::None);
			ParseCheckOffset(L"\\k'00' ", RegexOptions::None);
			ParseCheckOffset(L"\\k'a' ", RegexOptions::None, RegexParseError::UndefinedNamedReference, 5);
			ParseCheckOffset(L"(?<a>)\\k'a' ", RegexOptions::None);
			ParseCheckOffset(L"\\k<0' ", RegexOptions::None, RegexParseError::UnrecognizedEscape, 2);
			ParseCheckOffset(L"\\k'0> ", RegexOptions::None, RegexParseError::UnrecognizedEscape, 2);
			ParseCheckOffset(L"\\\\", RegexOptions::None, RegexParseError::UnescapedEndingBackslash, 1);
			ParseCheckOffset(L"\\\\ ", RegexOptions::None);
			ParseCheckOffset(L"\\<", RegexOptions::None);
			ParseCheckOffset(L"\\< ", RegexOptions::None);
			ParseCheckOffset(L"\\<0", RegexOptions::None);
			ParseCheckOffset(L"\\<0 ", RegexOptions::None);
			ParseCheckOffset(L"\\<0>", RegexOptions::None);
			ParseCheckOffset(L"\\<0> ", RegexOptions::None);
			ParseCheckOffset(L"\\<00> ", RegexOptions::None);
			ParseCheckOffset(L"\\<a> ", RegexOptions::None, RegexParseError::UndefinedNamedReference, 4);
			ParseCheckOffset(L"(?<a>)\\<a> ", RegexOptions::None);
			ParseCheckOffset(L"\\", RegexOptions::ECMAScript, RegexParseError::UnescapedEndingBackslash, 1);
			ParseCheckOffset(L"\\ ", RegexOptions::ECMAScript);
			ParseCheckOffset(L"\\<", RegexOptions::ECMAScript);
			ParseCheckOffset(L"\\< ", RegexOptions::ECMAScript);
			ParseCheckOffset(L"\\<0", RegexOptions::ECMAScript);
			ParseCheckOffset(L"\\<0 ", RegexOptions::ECMAScript);
			ParseCheckOffset(L"\\<0>", RegexOptions::ECMAScript);
			ParseCheckOffset(L"\\<0> ", RegexOptions::ECMAScript);
			ParseCheckOffset(L"\\'", RegexOptions::None);
			ParseCheckOffset(L"\\' ", RegexOptions::None);
			ParseCheckOffset(L"\\'0", RegexOptions::None);
			ParseCheckOffset(L"\\'0 ", RegexOptions::None);
			ParseCheckOffset(L"\\'0'", RegexOptions::None);
			ParseCheckOffset(L"\\'0' ", RegexOptions::None);
			ParseCheckOffset(L"\\'00' ", RegexOptions::None);
			ParseCheckOffset(L"\\'a' ", RegexOptions::None, RegexParseError::UndefinedNamedReference, 4);
			ParseCheckOffset(L"(?<a>)\\'a' ", RegexOptions::None);
			ParseCheckOffset(L"\\<0' ", RegexOptions::None);
			ParseCheckOffset(L"\'0> ", RegexOptions::None);
			ParseCheckOffset(L"\\p{Cc}", RegexOptions::None);
			ParseCheckOffset(L"\\p{ Cc }", RegexOptions::None, RegexParseError::InvalidUnicodePropertyEscape, 4);
			ParseCheckOffset(L"\\p{ Cc }", RegexOptions::IgnorePatternWhitespace, RegexParseError::InvalidUnicodePropertyEscape, 4);
			ParseCheckOffset(L"\\p {Cc}", RegexOptions::IgnorePatternWhitespace, RegexParseError::MalformedUnicodePropertyEscape, 3);
			ParseCheckOffset(L"\\p{xxx}", RegexOptions::None, RegexParseError::UnrecognizedUnicodeProperty, 7);
			ParseCheckOffset(L"\\p", RegexOptions::None, RegexParseError::InvalidUnicodePropertyEscape, 2);
			ParseCheckOffset(L"\\p{", RegexOptions::None, RegexParseError::InvalidUnicodePropertyEscape, 2);
			ParseCheckOffset(L"\\p{}", RegexOptions::None, RegexParseError::InvalidUnicodePropertyEscape, 2);
			ParseCheckOffset(L"\\p{} ", RegexOptions::None, RegexParseError::UnrecognizedUnicodeProperty, 4);
			ParseCheckOffset(L"\\p {} ", RegexOptions::None, RegexParseError::MalformedUnicodePropertyEscape, 3);
			ParseCheckOffset(L"\\p{Cc ", RegexOptions::None, RegexParseError::InvalidUnicodePropertyEscape, 6);
			ParseCheckOffset(L"\\p{IsArabicPresentationForms-A}", RegexOptions::None);
			ParseCheckOffset(L"(?:)", RegexOptions::None);
			ParseCheckOffset(L"(?:a)", RegexOptions::None);
			ParseCheckOffset(L"(?:", RegexOptions::None, RegexParseError::InsufficientClosingParentheses, 3);
			ParseCheckOffset(L"(?: ", RegexOptions::IgnorePatternWhitespace, RegexParseError::InsufficientClosingParentheses, 4);
			ParseCheckOffset(L"(?=)", RegexOptions::None);
			ParseCheckOffset(L"(?=a)", RegexOptions::None);
			ParseCheckOffset(L"(?=", RegexOptions::None, RegexParseError::InsufficientClosingParentheses, 3);
			ParseCheckOffset(L"(?= ", RegexOptions::IgnorePatternWhitespace, RegexParseError::InsufficientClosingParentheses, 4);
			ParseCheckOffset(L"(?!)", RegexOptions::None);
			ParseCheckOffset(L"(?!a)", RegexOptions::None);
			ParseCheckOffset(L"(?!", RegexOptions::None, RegexParseError::InsufficientClosingParentheses, 3);
			ParseCheckOffset(L"(?! ", RegexOptions::IgnorePatternWhitespace, RegexParseError::InsufficientClosingParentheses, 4);
			ParseCheckOffset(L"(?>)", RegexOptions::None);
			ParseCheckOffset(L"(?>a)", RegexOptions::None);
			ParseCheckOffset(L"(?>", RegexOptions::None, RegexParseError::InsufficientClosingParentheses, 3);
			ParseCheckOffset(L"(?> ", RegexOptions::IgnorePatternWhitespace, RegexParseError::InsufficientClosingParentheses, 4);
			ParseCheckOffset(L"(?<=)", RegexOptions::None);
			ParseCheckOffset(L"(?<=a)", RegexOptions::None);
			ParseCheckOffset(L"(?<=", RegexOptions::None, RegexParseError::InsufficientClosingParentheses, 4);
			ParseCheckOffset(L"(?<= ", RegexOptions::IgnorePatternWhitespace, RegexParseError::InsufficientClosingParentheses, 5);
			ParseCheckOffset(L"(?<!)", RegexOptions::None);
			ParseCheckOffset(L"(?<!a)", RegexOptions::None);
			ParseCheckOffset(L"(?<!", RegexOptions::None, RegexParseError::InsufficientClosingParentheses, 4);
			ParseCheckOffset(L"(?<! ", RegexOptions::IgnorePatternWhitespace, RegexParseError::InsufficientClosingParentheses, 5);
			ParseCheckOffset(L"(?<", RegexOptions::IgnorePatternWhitespace, RegexParseError::InvalidGroupingConstruct, 3);
			ParseCheckOffset(L"(?<>", RegexOptions::IgnorePatternWhitespace, RegexParseError::CaptureGroupNameInvalid, 3);
			ParseCheckOffset(L"(?<a", RegexOptions::IgnorePatternWhitespace, RegexParseError::InvalidGroupingConstruct, 4);
			ParseCheckOffset(L"(?<a>", RegexOptions::IgnorePatternWhitespace, RegexParseError::InsufficientClosingParentheses, 5);
			ParseCheckOffset(L"(?<a>a", RegexOptions::IgnorePatternWhitespace, RegexParseError::InsufficientClosingParentheses, 6);
			ParseCheckOffset(L"(?<a>a)", RegexOptions::IgnorePatternWhitespace);
			ParseCheckOffset(L"(?<a >a)", RegexOptions::None, RegexParseError::CaptureGroupNameInvalid, 4);
			ParseCheckOffset(L"(?<a >a)", RegexOptions::IgnorePatternWhitespace, RegexParseError::CaptureGroupNameInvalid, 4);
			ParseCheckOffset(L"(?< a>a)", RegexOptions::None, RegexParseError::CaptureGroupNameInvalid, 3);
			ParseCheckOffset(L"(?< a>a)", RegexOptions::IgnorePatternWhitespace, RegexParseError::CaptureGroupNameInvalid, 3);
			ParseCheckOffset(L"(?< a >a)", RegexOptions::None, RegexParseError::CaptureGroupNameInvalid, 3);
			ParseCheckOffset(L"(?< a >a)", RegexOptions::IgnorePatternWhitespace, RegexParseError::CaptureGroupNameInvalid, 3);
			ParseCheckOffset(L"(?<ab>a)", RegexOptions::IgnorePatternWhitespace);
			ParseCheckOffset(L"(?<0>a)", RegexOptions::IgnorePatternWhitespace, RegexParseError::CaptureGroupOfZero, 4);
			ParseCheckOffset(L"(?<1>a)", RegexOptions::IgnorePatternWhitespace);
			ParseCheckOffset(L"(?<10>a)", RegexOptions::IgnorePatternWhitespace);
			ParseCheckOffset(L"(?<1>)", RegexOptions::None);
			ParseCheckOffset(L"(?<1> )", RegexOptions::None);
			ParseCheckOffset(L"(?<1> )", RegexOptions::IgnorePatternWhitespace);
			ParseCheckOffset(L"(?'a')", RegexOptions::IgnorePatternWhitespace);
			ParseCheckOffset(L"(?(", RegexOptions::IgnorePatternWhitespace, RegexParseError::InsufficientClosingParentheses, 3);
			ParseCheckOffset(L"(?(0", RegexOptions::IgnorePatternWhitespace, RegexParseError::AlternationHasMalformedReference, 4);
			ParseCheckOffset(L"(?(0)", RegexOptions::IgnorePatternWhitespace, RegexParseError::InsufficientClosingParentheses, 5);
			ParseCheckOffset(L"(?(0))", RegexOptions::IgnorePatternWhitespace);
			ParseCheckOffset(L"(?(0)a)", RegexOptions::IgnorePatternWhitespace);
			ParseCheckOffset(L"(?(0)a|)", RegexOptions::IgnorePatternWhitespace);
			ParseCheckOffset(L"(?(0)a|b)", RegexOptions::IgnorePatternWhitespace);
			ParseCheckOffset(L"(?(0)a|b|)", RegexOptions::IgnorePatternWhitespace, RegexParseError::AlternationHasTooManyConditions, 10);
			ParseCheckOffset(L"(?(0)a|b|c)", RegexOptions::IgnorePatternWhitespace, RegexParseError::AlternationHasTooManyConditions, 11);
			ParseCheckOffset(L"(?(0 )", RegexOptions::IgnorePatternWhitespace, RegexParseError::AlternationHasMalformedReference, 5);
			ParseCheckOffset(L"(?(1))", RegexOptions::IgnorePatternWhitespace, RegexParseError::AlternationHasUndefinedReference, 5);
			ParseCheckOffset(L"(?(00))", RegexOptions::IgnorePatternWhitespace);
			ParseCheckOffset(L"(?(a))", RegexOptions::IgnorePatternWhitespace);
			ParseCheckOffset(L"(?<a>)(?(a))", RegexOptions::IgnorePatternWhitespace);
			ParseCheckOffset(L"(?<a>)(?(a ))", RegexOptions::IgnorePatternWhitespace);
			ParseCheckOffset(L"(?<a>)(?( a))", RegexOptions::IgnorePatternWhitespace);
			ParseCheckOffset(L"(?(()a()))", RegexOptions::None);
			ParseCheckOffset(L"(?((?<x>)a(?<y>)))", RegexOptions::None);
			ParseCheckOffset(L"(?(?'", RegexOptions::IgnorePatternWhitespace, RegexParseError::AlternationHasNamedCapture, 2);
			ParseCheckOffset(L"(?(?'x'))", RegexOptions::IgnorePatternWhitespace, RegexParseError::AlternationHasNamedCapture, 2);
			ParseCheckOffset(L"(?(?#", RegexOptions::IgnorePatternWhitespace, RegexParseError::UnterminatedComment, 5);
			ParseCheckOffset(L"(?(?#)", RegexOptions::IgnorePatternWhitespace, RegexParseError::AlternationHasComment, 2);
			ParseCheckOffset(L"(?(?#))", RegexOptions::IgnorePatternWhitespace, RegexParseError::AlternationHasComment, 2);
			ParseCheckOffset(L"(?(?<", RegexOptions::IgnorePatternWhitespace, RegexParseError::InvalidGroupingConstruct, 5);
			ParseCheckOffset(L"(?(?<a", RegexOptions::IgnorePatternWhitespace, RegexParseError::AlternationHasNamedCapture, 2);
			ParseCheckOffset(L"(?(?<a>", RegexOptions::IgnorePatternWhitespace, RegexParseError::AlternationHasNamedCapture, 2);
			ParseCheckOffset(L"(?(?<a>)", RegexOptions::IgnorePatternWhitespace, RegexParseError::AlternationHasNamedCapture, 2);
			ParseCheckOffset(L"(?(?<a>))", RegexOptions::IgnorePatternWhitespace, RegexParseError::AlternationHasNamedCapture, 2);
			ParseCheckOffset(L"(?(?<=))", RegexOptions::IgnorePatternWhitespace);
			ParseCheckOffset(L"(?(?<!))", RegexOptions::IgnorePatternWhitespace);
			ParseCheckOffset(L"\\1", RegexOptions::None, RegexParseError::UndefinedNumberedReference, 2);
			ParseCheckOffset(L"\\1 ", RegexOptions::None, RegexParseError::UndefinedNumberedReference, 2);
			ParseCheckOffset(L"()\\1", RegexOptions::None);
			ParseCheckOffset(L"()\\1 ", RegexOptions::None);
			ParseCheckOffset(L"()\\10 ", RegexOptions::None);
			ParseCheckOffset(L"\\1", RegexOptions::ECMAScript);
			ParseCheckOffset(L"\\1 ", RegexOptions::ECMAScript);
			ParseCheckOffset(L"()\\1", RegexOptions::ECMAScript);
			ParseCheckOffset(L"()\\1 ", RegexOptions::ECMAScript);
			ParseCheckOffset(L"()\\10 ", RegexOptions::ECMAScript);
			ParseCheckOffset(L"()()()()()()()()()()\10 ", RegexOptions::ECMAScript);
			ParseCheckOffset(L"[", RegexOptions::None, RegexParseError::UnterminatedBracket, 1);
			ParseCheckOffset(L"[ ", RegexOptions::None, RegexParseError::UnterminatedBracket, 2);
			ParseCheckOffset(L"[]", RegexOptions::None, RegexParseError::UnterminatedBracket, 2);
			ParseCheckOffset(L"[] ", RegexOptions::None, RegexParseError::UnterminatedBracket, 3);
			ParseCheckOffset(L"[a]", RegexOptions::None);
			ParseCheckOffset(L"[a] ", RegexOptions::None);
			ParseCheckOffset(L"[a-", RegexOptions::None, RegexParseError::UnterminatedBracket, 3);
			ParseCheckOffset(L"[a- ", RegexOptions::None, RegexParseError::UnterminatedBracket, 4);
			ParseCheckOffset(L"[a-]", RegexOptions::None);
			ParseCheckOffset(L"[a-] ", RegexOptions::None);
			ParseCheckOffset(L"[a-b]", RegexOptions::None);
			ParseCheckOffset(L"[a-b] ", RegexOptions::None);
			ParseCheckOffset(L"[a-[b]] ", RegexOptions::None);
			ParseCheckOffset(L"[a-b-[c]] ", RegexOptions::None);
			ParseCheckOffset(L"[a-[b]-c] ", RegexOptions::None, RegexParseError::ExclusionGroupNotLast, 6);
			ParseCheckOffset(L"[a-z-[b]12]", RegexOptions::None, RegexParseError::ExclusionGroupNotLast, 8);
			ParseCheckOffset(L"[[a]-b] ", RegexOptions::None);
			ParseCheckOffset(L"[[a]-[b]] ", RegexOptions::None);
			ParseCheckOffset(L"[\\w-a] ", RegexOptions::None);
			ParseCheckOffset(L"[a-\\w] ", RegexOptions::None, RegexParseError::ShorthandClassInCharacterRange, 5);
			ParseCheckOffset(L"[\\p{llll}-a] ", RegexOptions::None, RegexParseError::UnrecognizedUnicodeProperty, 9);
			ParseCheckOffset(L"[\\p{Lu}-a] ", RegexOptions::None);
			ParseCheckOffset(L"[a-\\p{Lu}] ", RegexOptions::None, RegexParseError::ShorthandClassInCharacterRange, 5);
			ParseCheckOffset(L"[a-[:Ll:]] ", RegexOptions::None);
			ParseCheckOffset(L"[a-[:Ll]] ", RegexOptions::None);
			ParseCheckOffset(L"[a-[:", RegexOptions::None, RegexParseError::UnterminatedBracket, 5);
			ParseCheckOffset(L"[a-[:L", RegexOptions::None, RegexParseError::UnterminatedBracket, 6);
			ParseCheckOffset(L"[a-[:L:", RegexOptions::None, RegexParseError::UnterminatedBracket, 7);
			ParseCheckOffset(L"[a-[:L:]", RegexOptions::None, RegexParseError::UnterminatedBracket, 8);
			ParseCheckOffset(L"[\\-]", RegexOptions::None);
			ParseCheckOffset(L"[a-b-c] ", RegexOptions::None);
			ParseCheckOffset(L"[-b-c] ", RegexOptions::None);
			ParseCheckOffset(L"[-[b] ", RegexOptions::None);
			ParseCheckOffset(L"[-[b]] ", RegexOptions::None);
			ParseCheckOffset(L"[--b ", RegexOptions::None, RegexParseError::UnterminatedBracket, 5);
			ParseCheckOffset(L"[--b] ", RegexOptions::None);
			ParseCheckOffset(L"[--[b ", RegexOptions::None, RegexParseError::UnterminatedBracket, 6);
			ParseCheckOffset(L"[--[b] ", RegexOptions::None, RegexParseError::ExclusionGroupNotLast, 6);
			ParseCheckOffset(L"[--[b]] ", RegexOptions::None);
			ParseCheckOffset(L"[a--[b ", RegexOptions::None, RegexParseError::UnterminatedBracket, 7);
			ParseCheckOffset(L"[,--[a] ", RegexOptions::None);
			ParseCheckOffset(L"[,--[a]] ", RegexOptions::None);
			ParseCheckOffset(L"[\\s-a]", RegexOptions::None);
			ParseCheckOffset(L"[\\p{Lu}-a]", RegexOptions::None);
			ParseCheckOffset(L"[\\c<-\\c>]", RegexOptions::None, RegexParseError::UnrecognizedControlCharacter, 4);
			ParseCheckOffset(L"[\\c>-\\c<]", RegexOptions::None, RegexParseError::UnrecognizedControlCharacter, 4);
			ParseCheckOffset(L"[\\c>-a]", RegexOptions::None, RegexParseError::UnrecognizedControlCharacter, 4);
			ParseCheckOffset(L"[a-\\c>]", RegexOptions::None, RegexParseError::UnrecognizedControlCharacter, 6);
			ParseCheckOffset(L"[a--]", RegexOptions::None, RegexParseError::ReversedCharacterRange, 4);
			ParseCheckOffset(L"[--a]", RegexOptions::None);
			ParseCheckOffset(L"[\\--a]", RegexOptions::None);
			ParseCheckOffset(L"[\\0-\\1]", RegexOptions::None);
			ParseCheckOffset(L"[\\1-\\0]", RegexOptions::None, RegexParseError::ReversedCharacterRange, 6);
			ParseCheckOffset(L"[\\0-\\01]", RegexOptions::None);
			ParseCheckOffset(L"[\\01-\\0]", RegexOptions::None, RegexParseError::ReversedCharacterRange, 7);
			ParseCheckOffset(L"[[:x:]-a]", RegexOptions::None);
			ParseCheckOffset(L"[a-[:x:]]", RegexOptions::None);
			ParseCheckOffset(L"[\\0-\\ca]", RegexOptions::None);
			ParseCheckOffset(L"[\\ca-\\0]", RegexOptions::None, RegexParseError::ReversedCharacterRange, 7);
			ParseCheckOffset(L"[\\ca-\\cA]", RegexOptions::None);
			ParseCheckOffset(L"[\\cA-\\ca]", RegexOptions::None);
			ParseCheckOffset(L"[\\u0-\\u1]", RegexOptions::None, RegexParseError::InsufficientOrInvalidHexDigits, 5);
			ParseCheckOffset(L"[\\u1-\\u0]", RegexOptions::None, RegexParseError::InsufficientOrInvalidHexDigits, 5);
			ParseCheckOffset(L"[\\u0000-\\u0000]", RegexOptions::None);
			ParseCheckOffset(L"[\\u0000-\\u0001]", RegexOptions::None);
			ParseCheckOffset(L"[\\u0001-\\u0000]", RegexOptions::None, RegexParseError::ReversedCharacterRange, 14);
			ParseCheckOffset(L"[\\u0001-a]", RegexOptions::None);
			ParseCheckOffset(L"[a-\\u0001]", RegexOptions::None, RegexParseError::ReversedCharacterRange, 9);
			ParseCheckOffset(L"[a-a]", RegexOptions::None);
			ParseCheckOffset(L"[a-A]", RegexOptions::None, RegexParseError::ReversedCharacterRange, 4);
			ParseCheckOffset(L"[A-a]", RegexOptions::None);
			ParseCheckOffset(L"[a-a]", RegexOptions::IgnoreCase);
			ParseCheckOffset(L"[a-A]", RegexOptions::IgnoreCase, RegexParseError::ReversedCharacterRange, 4);
			ParseCheckOffset(L"[A-a]", RegexOptions::IgnoreCase);
			ParseCheckOffset(L"[a-\\x61]", RegexOptions::None);
			ParseCheckOffset(L"[\\x61-a]", RegexOptions::None);
			ParseCheckOffset(L"[a-\\x60]", RegexOptions::None, RegexParseError::ReversedCharacterRange, 7);
			ParseCheckOffset(L"[\\x62-a]", RegexOptions::None, RegexParseError::ReversedCharacterRange, 7);
			ParseCheckOffset(L"[a-\\x62]", RegexOptions::None);
			ParseCheckOffset(L"[\\3-\\cc]", RegexOptions::None);
			ParseCheckOffset(L"[\\cc-\\3]", RegexOptions::None);
			ParseCheckOffset(L"[\\2-\\cc]", RegexOptions::None);
			ParseCheckOffset(L"[\\cc-\\2]", RegexOptions::None, RegexParseError::ReversedCharacterRange, 7);
			ParseCheckOffset(L"[\\4-\\cc]", RegexOptions::None, RegexParseError::ReversedCharacterRange, 7);
			ParseCheckOffset(L"[\\cc-\\4]", RegexOptions::None);
			ParseCheckOffset(L"[\\ca-\\cb]", RegexOptions::None);
			ParseCheckOffset(L"[\\ca-\\cB]", RegexOptions::None);
			ParseCheckOffset(L"[\\cA-\\cb]", RegexOptions::None);
			ParseCheckOffset(L"[\\cA-\\cB]", RegexOptions::None);
			ParseCheckOffset(L"[\\cb-\\ca]", RegexOptions::None, RegexParseError::ReversedCharacterRange, 8);
			ParseCheckOffset(L"[\\cb-\\cA]", RegexOptions::None, RegexParseError::ReversedCharacterRange, 8);
			ParseCheckOffset(L"[\\cB-\\ca]", RegexOptions::None, RegexParseError::ReversedCharacterRange, 8);
			ParseCheckOffset(L"[\\cB-\\cA]", RegexOptions::None, RegexParseError::ReversedCharacterRange, 8);
			ParseCheckOffset(L"[\\--#]", RegexOptions::None);
			ParseCheckOffset(L"[b-\\-a]", RegexOptions::None, RegexParseError::ReversedCharacterRange, 5);
			ParseCheckOffset(L"[b-\\-\\-a]", RegexOptions::None, RegexParseError::ReversedCharacterRange, 5);
			ParseCheckOffset(L"()\\2", RegexOptions::None, RegexParseError::UndefinedNumberedReference, 4);
			ParseCheckOffset(L"()()\\2", RegexOptions::None);
			ParseCheckOffset(L"()\\1", RegexOptions::ExplicitCapture, RegexParseError::UndefinedNumberedReference, 4);
			ParseCheckOffset(L"()\\2", RegexOptions::ExplicitCapture, RegexParseError::UndefinedNumberedReference, 4);
			ParseCheckOffset(L"()()\\2", RegexOptions::ExplicitCapture, RegexParseError::UndefinedNumberedReference, 6);
			ParseCheckOffset(L"()()(?n)\\1\\2", RegexOptions::None);
			ParseCheckOffset(L"()(?n)()\\1\\2", RegexOptions::None, RegexParseError::UndefinedNumberedReference, 12);
			ParseCheckOffset(L"(?n)()()\\1\\2", RegexOptions::None, RegexParseError::UndefinedNumberedReference, 10);
			ParseCheckOffset(L"()()(?n)\\1\\2", RegexOptions::ExplicitCapture, RegexParseError::UndefinedNumberedReference, 10);
			ParseCheckOffset(L"()(?n)()\\1\\2", RegexOptions::ExplicitCapture, RegexParseError::UndefinedNumberedReference, 10);
			ParseCheckOffset(L"(?n)()()\\1\\2", RegexOptions::ExplicitCapture, RegexParseError::UndefinedNumberedReference, 10);
			ParseCheckOffset(L"()()(?-n)\\1\\2", RegexOptions::None);
			ParseCheckOffset(L"()(?-n)()\\1\\2", RegexOptions::None);
			ParseCheckOffset(L"(?-n)()()\\1\\2", RegexOptions::None);
			ParseCheckOffset(L"()()(?-n)\\1\\2", RegexOptions::ExplicitCapture, RegexParseError::UndefinedNumberedReference, 11);
			ParseCheckOffset(L"()(?-n)()\\1\\2", RegexOptions::ExplicitCapture, RegexParseError::UndefinedNumberedReference, 13);
			ParseCheckOffset(L"(?-n)()()\\1\\2", RegexOptions::ExplicitCapture);
			ParseCheckOffset(L"()()(?n:\\1\\2)", RegexOptions::None);
			ParseCheckOffset(L"()()(?n:\\1\\2)", RegexOptions::ExplicitCapture, RegexParseError::UndefinedNumberedReference, 10);
			ParseCheckOffset(L"()()(?-n:\\1\\2)", RegexOptions::None);
			ParseCheckOffset(L"()()(?-n:\\1\\2)", RegexOptions::ExplicitCapture, RegexParseError::UndefinedNumberedReference, 11);
			ParseCheckOffset(L"(?n:)()()\\1\\2", RegexOptions::None);
			ParseCheckOffset(L"(?n:)()()\\1\\2", RegexOptions::ExplicitCapture, RegexParseError::UndefinedNumberedReference, 11);
			ParseCheckOffset(L"(?-n:)()()\\1\\2", RegexOptions::None);
			ParseCheckOffset(L"(?-n:)()()\\1\\2", RegexOptions::ExplicitCapture, RegexParseError::UndefinedNumberedReference, 12);
			ParseCheckOffset(L"(?n)(?-n)()()\\1\\2", RegexOptions::None);
			ParseCheckOffset(L"(?n)(?-n)()()\\1\\2", RegexOptions::ExplicitCapture);
			ParseCheckOffset(L"(?-n)(?n)()()\\1\\2", RegexOptions::None, RegexParseError::UndefinedNumberedReference, 15);
			ParseCheckOffset(L"(?-n)(?n)()()\\1\\2", RegexOptions::ExplicitCapture, RegexParseError::UndefinedNumberedReference, 15);
			// References
			ParseCheckOffset(L"[aeiou]", RegexOptions::None);
			ParseCheckOffset(L"(?<duplicateWord>\\w+)\\s\\k<duplicateWord>\\W(?<nextWord>\\w+)", RegexOptions::None);
			ParseCheckOffset(L"((?<One>abc)\\d+)?(?<Two>xyz)(.*)", RegexOptions::None);
			ParseCheckOffset(L"(\\w+)\\s(\\1)", RegexOptions::None);
			ParseCheckOffset(L"\\Bqu\\w+", RegexOptions::None);
			ParseCheckOffset(L"\\bare\\w*\\b", RegexOptions::None);
			ParseCheckOffset(L"\\G(\\w+\\s?\\w*),?", RegexOptions::None);
			ParseCheckOffset(L"\\D+(?<digit>\\d+)\\D+(?<digit>\\d+)?", RegexOptions::None);
			ParseCheckOffset(L"(\\s\\d{4}(-(\\d{4}&#124;present))?,?)+", RegexOptions::None);
			ParseCheckOffset(L"^((\\w+(\\s?)){2,}),\\s(\\w+\\s\\w+),(\\s\\d{4}(-(\\d{4}|present))?,?)+", RegexOptions::None);
			ParseCheckOffset(L"^[0-9-[2468]]+$", RegexOptions::None);
			ParseCheckOffset(L"[a-z-[0-9]]", RegexOptions::None);
			ParseCheckOffset(L"[\\p{IsBasicLatin}-[\\x00-\\x7F]]", RegexOptions::None);
			ParseCheckOffset(L"[\\u0000-\\uFFFF-[\\s\\p{P}\\p{IsGreek}\\x85]]", RegexOptions::None);
			ParseCheckOffset(L"[a-z-[d-w-[m-o]]]", RegexOptions::None);
			ParseCheckOffset(L"((\\w+(\\s?)){2,}", RegexOptions::None, RegexParseError::InsufficientClosingParentheses, 15);
			ParseCheckOffset(L"[a-z-[djp]]", RegexOptions::None);
			ParseCheckOffset(L"(\\w)\\1+.\\b", RegexOptions::None);
			ParseCheckOffset(L"\\d{4}\\b", RegexOptions::None);
			ParseCheckOffset(L"\\d{1,2},", RegexOptions::None);
			ParseCheckOffset(L"(?<!(Saturday|Sunday) )\\b\\w+ \\d{1,2}, \\d{4}\\b", RegexOptions::None);
			ParseCheckOffset(L"(?<=\\b20)\\d{2}\\b", RegexOptions::None);
			ParseCheckOffset(L"\\b\\w+\\b(?!\\p{P})", RegexOptions::None);
			ParseCheckOffset(L"\\b(?!un)\\w+\\b", RegexOptions::None);
			ParseCheckOffset(L"\\b(?ix: d \\w+)\\s", RegexOptions::None);
			ParseCheckOffset(L"(?:\\w+)", RegexOptions::None);
			ParseCheckOffset(L"(?:\\b(?:\\w+)\\W*)+", RegexOptions::None);
			ParseCheckOffset(L"(?:\\b(?:\\w+)\\W*)+\\.", RegexOptions::None);
			ParseCheckOffset(L"[^<>]*", RegexOptions::None);
			ParseCheckOffset(L"\\b\\w+(?=\\sis\\b)", RegexOptions::None);
			ParseCheckOffset(L"[a-z-[m]]", RegexOptions::None);
			ParseCheckOffset(L"^\\D\\d{1,5}\\D*$", RegexOptions::None);
			ParseCheckOffset(L"[^0-9]", RegexOptions::None);
			ParseCheckOffset(L"(\\p{IsGreek}+(\\s)?)+", RegexOptions::None);
			ParseCheckOffset(L"\\b(\\p{IsGreek}+(\\s)?)+\\p{Pd}\\s(\\p{IsBasicLatin}+(\\s)?)+", RegexOptions::None);
			ParseCheckOffset(L"\\b.*[.?!;:](\\s|\\z)", RegexOptions::None);
			ParseCheckOffset(L"^.+", RegexOptions::None);
			ParseCheckOffset(L"[^o]", RegexOptions::None);
			ParseCheckOffset(L"\\bth[^o]\\w+\\b", RegexOptions::None);
			ParseCheckOffset(L"(\\P{Sc})+", RegexOptions::None);
			ParseCheckOffset(L"[^\\p{P}\\d]", RegexOptions::None);
			ParseCheckOffset(L"\\b[A-Z]\\w*\\b", RegexOptions::None);
			ParseCheckOffset(L"\\S+?", RegexOptions::None);
			ParseCheckOffset(L"y\\s", RegexOptions::None);
			ParseCheckOffset(L"gr[ae]y\\s\\S+?[\\s\\p{P}]", RegexOptions::None);
			ParseCheckOffset(L"[\\s\\p{P}]", RegexOptions::None);
			ParseCheckOffset(L"[\\p{P}\\d]", RegexOptions::None);
			ParseCheckOffset(L"[^aeiou]", RegexOptions::None);
			ParseCheckOffset(L"(\\w)\\1", RegexOptions::None);
			ParseCheckOffset(L"[^\\p{Ll}\\p{Lu}\\p{Lt}\\p{Lo}\\p{Nd}\\p{Pc}\\p{Lm}] ", RegexOptions::None);
			ParseCheckOffset(L"[^a-zA-Z_0-9]", RegexOptions::None);
			ParseCheckOffset(L"\\P{Nd}", RegexOptions::None);
			ParseCheckOffset(L"(\\(?\\d{3}\\)?[\\s-])?", RegexOptions::None);
			ParseCheckOffset(L"^(\\(?\\d{3}\\)?[\\s-])?\\d{3}-\\d{4}$", RegexOptions::None);
			ParseCheckOffset(L"[0-9]", RegexOptions::None);
			ParseCheckOffset(L"\\p{Nd}", RegexOptions::None);
			ParseCheckOffset(L"\\b(\\S+)\\s?", RegexOptions::None);
			ParseCheckOffset(L"[^ \\f\\n\\r\\t\\v]", RegexOptions::None);
			ParseCheckOffset(L"[^\\f\\n\\r\\t\\v\\x85\\p{Z}]", RegexOptions::None);
			ParseCheckOffset(L"(\\s|$)", RegexOptions::None);
			ParseCheckOffset(L"\\b\\w+(e)?s(\\s|$)", RegexOptions::None);
			ParseCheckOffset(L"[ \\f\\n\\r\\t\\v]", RegexOptions::None);
			ParseCheckOffset(L"(\\W){1,2}", RegexOptions::None);
			ParseCheckOffset(L"(\\w+)", RegexOptions::None);
			ParseCheckOffset(L"\\b", RegexOptions::None);
			ParseCheckOffset(L"\\b(\\w+)(\\W){1,2}", RegexOptions::None);
			ParseCheckOffset(L"(?>(\\w)\\1+).\\b", RegexOptions::None);
			ParseCheckOffset(L"(\\b(\\w+)\\W+)+", RegexOptions::None);
			ParseCheckOffset(L"\\p{Sc}*(\\s?\\d+[.,]?\\d*)\\p{Sc}*", RegexOptions::None);
			ParseCheckOffset(L"p{Sc}*(?<amount>\\s?\\d+[.,]?\\d*)\\p{Sc}*", RegexOptions::None);
			ParseCheckOffset(L"^(\\w+\\s?)+$", RegexOptions::None);
			ParseCheckOffset(L"(?ix) d \\w+ \\s", RegexOptions::None);
			ParseCheckOffset(L"\\bthe\\w*\\b", RegexOptions::None);
			ParseCheckOffset(L"\\b(?i:t)he\\w*\\b", RegexOptions::None);
			ParseCheckOffset(L"^(\\w+)\\s(\\d+)$", RegexOptions::None);
			ParseCheckOffset(L"^(\\w+)\\s(\\d+)\\r*$", RegexOptions::Multiline);
			ParseCheckOffset(L"(?m)^(\\w+)\\s(\\d+)\\r*$", RegexOptions::Multiline);
			ParseCheckOffset(L"(?s)^.+", RegexOptions::None);
			ParseCheckOffset(L"\\b(\\d{2}-)*(?(1)\\d{7}|\\d{3}-\\d{2}-\\d{4})\\b", RegexOptions::None);
			ParseCheckOffset(L"\\b\\(?((\\w+),?\\s?)+[\\.!?]\\)?", RegexOptions::None);
			ParseCheckOffset(L"(?n)\\b\\(?((?>\\w+),?\\s?)+[\\.!?]\\)?", RegexOptions::None);
			ParseCheckOffset(L"\\b\\(?(?n:(?>\\w+),?\\s?)+[\\.!?]\\)?", RegexOptions::None);
			ParseCheckOffset(L"\\b\\(?((?>\\w+),?\\s?)+[\\.!?]\\)?", RegexOptions::IgnorePatternWhitespace);
			ParseCheckOffset(L"(?x)\\b \\(? ( (?>\\w+) ,?\\s? )+  [\\.!?] \\)? # Matches an entire sentence.", RegexOptions::None);
			ParseCheckOffset(L"\\bb\\w+\\s", RegexOptions::RightToLeft);
			ParseCheckOffset(L"(?<=\\d{1,2}\\s)\\w+,?\\s\\d{4}", RegexOptions::RightToLeft);
			ParseCheckOffset(L"\\b(\\w+\\s*)+", RegexOptions::ECMAScript);
			ParseCheckOffset(L"((a+)(\\1) ?)+", RegexOptions::ECMAScript);
			ParseCheckOffset(L"\\b(D\\w+)\\s(d\\w+)\\b", RegexOptions::None);
			ParseCheckOffset(L"\\b(D\\w+)(?ixn) \\s (d\\w+) \\b", RegexOptions::None);
			ParseCheckOffset(L"\\b((?# case-sensitive comparison)D\\w+)\\s((?#case-insensitive comparison)d\\w+)\\b", RegexOptions::None);
			ParseCheckOffset(L"\\b\\(?((?>\\w+),?\\s?)+[\\.!?]\\)?", RegexOptions::None);
			ParseCheckOffset(L"\\b(?<n2>\\d{2}-)*(?(n2)\\d{7}|\\d{3}-\\d{2}-\\d{4})\\b", RegexOptions::None);
			ParseCheckOffset(L"\\b(\\d{2}-\\d{7}|\\d{3}-\\d{2}-\\d{4})\\b", RegexOptions::None);
			ParseCheckOffset(L"\\bgr(a|e)y\\b", RegexOptions::None);
			ParseCheckOffset(L"\\b91*9*\\b", RegexOptions::None);
			ParseCheckOffset(L"\\ban+\\w*?\\b", RegexOptions::None);
			ParseCheckOffset(L"\\ban?\\b", RegexOptions::None);
			ParseCheckOffset(L"\\b\\d+\\,\\d{3}\\b", RegexOptions::None);
			ParseCheckOffset(L"\\b\\d{2,}\\b\\D+", RegexOptions::None);
			ParseCheckOffset(L"(00\\s){2,4}", RegexOptions::None);
			ParseCheckOffset(L"\\b\\w*?oo\\w*?\\b", RegexOptions::None);
			ParseCheckOffset(L"\\b\\w+?\\b", RegexOptions::None);
			ParseCheckOffset(L"^\\s*(System.)??Console.Write(Line)??\\(??", RegexOptions::None);
			ParseCheckOffset(L"(System.)??", RegexOptions::None);
			ParseCheckOffset(L"\\b(\\w{3,}?\\.){2}?\\w{3,}?\\b", RegexOptions::None);
			ParseCheckOffset(L"\\b[A-Z](\\w*?\\s*?){1,10}[.!?]", RegexOptions::None);
			ParseCheckOffset(L"b.*([0-9]{4})\\b", RegexOptions::None);
			ParseCheckOffset(L"\\b.*?([0-9]{4})\\b", RegexOptions::None);
			ParseCheckOffset(L"(a?)*", RegexOptions::None);
			ParseCheckOffset(L"(a\\1|(?(1)\\1)){0,2}", RegexOptions::None);
			ParseCheckOffset(L"(a\\1|(?(1)\\1)){2}", RegexOptions::None);
			ParseCheckOffset(L"(?<char>\\w)\\k<char>", RegexOptions::None);
			ParseCheckOffset(L"(?<2>\\w)\\k<2>", RegexOptions::None);
			ParseCheckOffset(L"(?<1>a)(?<1>\\1b)*", RegexOptions::None);
			ParseCheckOffset(L"\\b(\\p{Lu}{2})(\\d{2})?(\\p{Lu}{2})\\b", RegexOptions::None);
			ParseCheckOffset(L"\\bgr[ae]y\\b", RegexOptions::None);
			ParseCheckOffset(L"\\b((?# case sensitive comparison)D\\w+)\\s(?ixn)((?#case insensitive comparison)d\\w+)\\b", RegexOptions::None);
			ParseCheckOffset(L"\\{\\d+(,-*\\d+)*(\\:\\w{1,4}?)*\\}(?x) # Looks for a composite format item.", RegexOptions::None);
			ParseCheckOffset(L"cat([a-\\d]*)dog", RegexOptions::None, RegexParseError::ShorthandClassInCharacterRange, 9);
			ParseCheckOffset(L"\\k<1", RegexOptions::None, RegexParseError::UnrecognizedEscape, 2);
			ParseCheckOffset(L"(?')", RegexOptions::None, RegexParseError::CaptureGroupNameInvalid, 3);
			ParseCheckOffset(L"(?<)", RegexOptions::None, RegexParseError::CaptureGroupNameInvalid, 3);
			ParseCheckOffset(L"(?imn )", RegexOptions::None, RegexParseError::InvalidGroupingConstruct, 6);
			ParseCheckOffset(L"(?imn", RegexOptions::None, RegexParseError::InvalidGroupingConstruct, 5);
			ParseCheckOffset(L"(?'cat'", RegexOptions::None, RegexParseError::InsufficientClosingParentheses, 7);
			ParseCheckOffset(L"(?'", RegexOptions::None, RegexParseError::InvalidGroupingConstruct, 3);
			ParseCheckOffset(L"(?'=)", RegexOptions::None, RegexParseError::InvalidGroupingConstruct, 4);
			ParseCheckOffset(L"(?'!)", RegexOptions::None, RegexParseError::InvalidGroupingConstruct, 4);
			ParseCheckOffset(L"[^", RegexOptions::None, RegexParseError::UnterminatedBracket, 2);
			ParseCheckOffset(L"[cat", RegexOptions::None, RegexParseError::UnterminatedBracket, 4);
			ParseCheckOffset(L"[^cat", RegexOptions::None, RegexParseError::UnterminatedBracket, 5);
			ParseCheckOffset(L"\\p{cat", RegexOptions::None, RegexParseError::InvalidUnicodePropertyEscape, 6);
			ParseCheckOffset(L"\\k<cat", RegexOptions::None, RegexParseError::UnrecognizedEscape, 2);
			ParseCheckOffset(L"\\p{cat}", RegexOptions::None, RegexParseError::UnrecognizedUnicodeProperty, 7);
			ParseCheckOffset(L"\\P{cat", RegexOptions::None, RegexParseError::InvalidUnicodePropertyEscape, 6);
			ParseCheckOffset(L"\\P{cat}", RegexOptions::None, RegexParseError::UnrecognizedUnicodeProperty, 7);
			ParseCheckOffset(L"(?<cat>", RegexOptions::None, RegexParseError::InsufficientClosingParentheses, 7);
			ParseCheckOffset(L"\\P{", RegexOptions::None, RegexParseError::InvalidUnicodePropertyEscape, 2);
			ParseCheckOffset(L"\\k<>", RegexOptions::None, RegexParseError::UnrecognizedEscape, 2);
			ParseCheckOffset(L"(?(", RegexOptions::None, RegexParseError::InsufficientClosingParentheses, 3);
			ParseCheckOffset(L"(?()|", RegexOptions::None, RegexParseError::InsufficientClosingParentheses, 5);
			ParseCheckOffset(L"?(a|b)", RegexOptions::None, RegexParseError::QuantifierAfterNothing, 1);
			ParseCheckOffset(L"?((a)", RegexOptions::None, RegexParseError::QuantifierAfterNothing, 1);
			ParseCheckOffset(L"?((a)a", RegexOptions::None, RegexParseError::QuantifierAfterNothing, 1);
			ParseCheckOffset(L"?((a)a|", RegexOptions::None, RegexParseError::QuantifierAfterNothing, 1);
			ParseCheckOffset(L"?((a)a|b", RegexOptions::None, RegexParseError::QuantifierAfterNothing, 1);
			ParseCheckOffset(L"?(a)", RegexOptions::None, RegexParseError::QuantifierAfterNothing, 1);
			ParseCheckOffset(L"[a", RegexOptions::None, RegexParseError::UnterminatedBracket, 2);
			ParseCheckOffset(L"?(a:b)", RegexOptions::None, RegexParseError::QuantifierAfterNothing, 1);
			ParseCheckOffset(L"(?(?", RegexOptions::None, RegexParseError::InvalidGroupingConstruct, 4);
			ParseCheckOffset(L"(?(cat", RegexOptions::None, RegexParseError::InsufficientClosingParentheses, 6);
			ParseCheckOffset(L"(?(cat)|", RegexOptions::None, RegexParseError::InsufficientClosingParentheses, 8);
			ParseCheckOffset(L"foo(?<0>bar)", RegexOptions::None, RegexParseError::CaptureGroupOfZero, 7);
			ParseCheckOffset(L"foo(?'0'bar)", RegexOptions::None, RegexParseError::CaptureGroupOfZero, 7);
			ParseCheckOffset(L"foo(?<1bar)", RegexOptions::None, RegexParseError::CaptureGroupNameInvalid, 7);
			ParseCheckOffset(L"foo(?'1bar)", RegexOptions::None, RegexParseError::CaptureGroupNameInvalid, 7);
			ParseCheckOffset(L"\\p{klsak", RegexOptions::None, RegexParseError::InvalidUnicodePropertyEscape, 8);
			ParseCheckOffset(L"(?c:cat)", RegexOptions::None, RegexParseError::InvalidGroupingConstruct, 3);
			ParseCheckOffset(L"(??e:cat)", RegexOptions::None, RegexParseError::InvalidGroupingConstruct, 3);
			ParseCheckOffset(L"[a-f-[]]+", RegexOptions::None, RegexParseError::UnterminatedBracket, 9);
			ParseCheckOffset(L"[A-[]+", RegexOptions::None, RegexParseError::UnterminatedBracket, 6);
			ParseCheckOffset(L"(?(?e))", RegexOptions::None, RegexParseError::InvalidGroupingConstruct, 5);
			ParseCheckOffset(L"(?(?a)", RegexOptions::None, RegexParseError::InvalidGroupingConstruct, 5);
			ParseCheckOffset(L"(?r:cat)", RegexOptions::None, RegexParseError::InvalidGroupingConstruct, 3);
			ParseCheckOffset(L"\\x2", RegexOptions::None, RegexParseError::InsufficientOrInvalidHexDigits, 2);
			ParseCheckOffset(L"(cat) (?#cat)    \\s+ (?#followed by 1 or more whitespace", RegexOptions::IgnorePatternWhitespace, RegexParseError::UnterminatedComment, 56);
			ParseCheckOffset(L"cat(?(?afdcat)dog)", RegexOptions::None, RegexParseError::InvalidGroupingConstruct, 8);
			ParseCheckOffset(L"cat(?(?<cat>cat)dog)", RegexOptions::None, RegexParseError::AlternationHasNamedCapture, 5);
			ParseCheckOffset(L"cat(?(?'cat'cat)dog)", RegexOptions::None, RegexParseError::AlternationHasNamedCapture, 5);
			ParseCheckOffset(L"cat(?(?#COMMENT)cat)", RegexOptions::None, RegexParseError::AlternationHasComment, 5);
			ParseCheckOffset(L"cat(?<>dog)", RegexOptions::None, RegexParseError::CaptureGroupNameInvalid, 6);
			ParseCheckOffset(L"cat(?<dog<>)_*>dog)", RegexOptions::None, RegexParseError::CaptureGroupNameInvalid, 9);
			ParseCheckOffset(L"cat(?<dog >)_*>dog)", RegexOptions::None, RegexParseError::CaptureGroupNameInvalid, 9);
			ParseCheckOffset(L"cat(?<dog!>)_*>dog)", RegexOptions::None, RegexParseError::CaptureGroupNameInvalid, 9);
			ParseCheckOffset(L"cat(?<dog)_*>dog)", RegexOptions::None, RegexParseError::CaptureGroupNameInvalid, 9);
			ParseCheckOffset(L"cat(?<1dog>dog)", RegexOptions::None, RegexParseError::CaptureGroupNameInvalid, 7);
			ParseCheckOffset(L"cat(?<0>dog)", RegexOptions::None, RegexParseError::CaptureGroupOfZero, 7);
			ParseCheckOffset(L"([5-\\D]*)dog", RegexOptions::None, RegexParseError::ShorthandClassInCharacterRange, 6);
			ParseCheckOffset(L"cat([6-\\s]*)dog", RegexOptions::None, RegexParseError::ShorthandClassInCharacterRange, 9);
			ParseCheckOffset(L"cat([c-\\S]*)", RegexOptions::None, RegexParseError::ShorthandClassInCharacterRange, 9);
			ParseCheckOffset(L"cat([7-\\w]*)", RegexOptions::None, RegexParseError::ShorthandClassInCharacterRange, 9);
			ParseCheckOffset(L"cat([a-\\W]*)dog", RegexOptions::None, RegexParseError::ShorthandClassInCharacterRange, 9);
			ParseCheckOffset(L"([f-\\p{Lu}]\\w*)\\s([\\p{Lu}]\\w*)", RegexOptions::None, RegexParseError::ShorthandClassInCharacterRange, 6);
			ParseCheckOffset(L"(cat) (?#cat)    \\s+ (?#followed by 1 or more whitespace", RegexOptions::None, RegexParseError::UnterminatedComment, 56);
			ParseCheckOffset(L"([1-\\P{Ll}][\\p{Ll}]*)\\s([\\P{Ll}][\\p{Ll}]*)", RegexOptions::None, RegexParseError::ShorthandClassInCharacterRange, 6);
			ParseCheckOffset(L"[\\P]", RegexOptions::None, RegexParseError::InvalidUnicodePropertyEscape, 3);
			ParseCheckOffset(L"([\\pcat])", RegexOptions::None, RegexParseError::MalformedUnicodePropertyEscape, 5);
			ParseCheckOffset(L"([\\Pcat])", RegexOptions::None, RegexParseError::MalformedUnicodePropertyEscape, 5);
			ParseCheckOffset(L"(\\p{", RegexOptions::None, RegexParseError::InvalidUnicodePropertyEscape, 3);
			ParseCheckOffset(L"(\\p{Ll", RegexOptions::None, RegexParseError::InvalidUnicodePropertyEscape, 6);
			ParseCheckOffset(L"(cat)([\\o]*)(dog)", RegexOptions::None, RegexParseError::UnrecognizedEscape, 9);
			ParseCheckOffset(L"[\\p]", RegexOptions::None, RegexParseError::InvalidUnicodePropertyEscape, 3);
			ParseCheckOffset(L"(?<cat>cat)\\s+(?<dog>dog)\\kcat", RegexOptions::None, RegexParseError::MalformedNamedReference, 28);
			ParseCheckOffset(L"(?<cat>cat)\\s+(?<dog>dog)\\k<cat2>", RegexOptions::None, RegexParseError::UndefinedNamedReference, 33);
			ParseCheckOffset(L"(?<cat>cat)\\s+(?<dog>dog)\\k<8>cat", RegexOptions::None, RegexParseError::UndefinedNumberedReference, 30);
			ParseCheckOffset(L"^[abcd]{1}?*$", RegexOptions::None, RegexParseError::NestedQuantifiersNotParenthesized, 12);
			ParseCheckOffset(L"^[abcd]*+$", RegexOptions::None, RegexParseError::NestedQuantifiersNotParenthesized, 9);
			ParseCheckOffset(L"^[abcd]+*$", RegexOptions::None, RegexParseError::NestedQuantifiersNotParenthesized, 9);
			ParseCheckOffset(L"^[abcd]?*$", RegexOptions::None, RegexParseError::NestedQuantifiersNotParenthesized, 9);
			ParseCheckOffset(L"^[abcd]*?+$", RegexOptions::None, RegexParseError::NestedQuantifiersNotParenthesized, 10);
			ParseCheckOffset(L"^[abcd]+?*$", RegexOptions::None, RegexParseError::NestedQuantifiersNotParenthesized, 10);
			ParseCheckOffset(L"^[abcd]{1,}?*$", RegexOptions::None, RegexParseError::NestedQuantifiersNotParenthesized, 13);
			ParseCheckOffset(L"^[abcd]??*$", RegexOptions::None, RegexParseError::NestedQuantifiersNotParenthesized, 10);
			ParseCheckOffset(L"^[abcd]+{0,5}$", RegexOptions::None, RegexParseError::NestedQuantifiersNotParenthesized, 9);
			ParseCheckOffset(L"^[abcd]?{0,5}$", RegexOptions::None, RegexParseError::NestedQuantifiersNotParenthesized, 9);
			ParseCheckOffset(L"\\ua", RegexOptions::None, RegexParseError::InsufficientOrInvalidHexDigits, 2);
			ParseCheckOffset(L"^[abcd]*{0,5}$", RegexOptions::None, RegexParseError::NestedQuantifiersNotParenthesized, 9);
			ParseCheckOffset(L"^[abcd]{0,16}?*$", RegexOptions::None, RegexParseError::NestedQuantifiersNotParenthesized, 15);
			ParseCheckOffset(L"^[abcd]{1,}*$", RegexOptions::None, RegexParseError::NestedQuantifiersNotParenthesized, 12);
			ParseCheckOffset(L"(?<cat>cat)\\s+(?<dog>dog)\\k<8>cat", RegexOptions::ECMAScript, RegexParseError::UndefinedNumberedReference, 30);
			ParseCheckOffset(L"(?<cat>cat)\\s+(?<dog>dog)\\k8", RegexOptions::None, RegexParseError::MalformedNamedReference, 28);
			ParseCheckOffset(L"(?<cat>cat)\\s+(?<dog>dog)\\k8", RegexOptions::ECMAScript, RegexParseError::MalformedNamedReference, 28);
			ParseCheckOffset(L"(cat)(\\7)", RegexOptions::None, RegexParseError::UndefinedNumberedReference, 8);
			ParseCheckOffset(L"(cat)\\s+(?<2147483648>dog)", RegexOptions::None, RegexParseError::QuantifierOrCaptureGroupOutOfRange, 21);
			ParseCheckOffset(L"(cat)\\s+(?<21474836481097>dog)", RegexOptions::None, RegexParseError::QuantifierOrCaptureGroupOutOfRange, 21);
			ParseCheckOffset(L"^[abcd]{1}*$", RegexOptions::None, RegexParseError::NestedQuantifiersNotParenthesized, 11);
			ParseCheckOffset(L"(cat)(\\c*)(dog)", RegexOptions::None, RegexParseError::UnrecognizedControlCharacter, 9);
			ParseCheckOffset(L"(cat)(\\c *)(dog)", RegexOptions::None, RegexParseError::UnrecognizedControlCharacter, 9);
			ParseCheckOffset(L"(cat)(\\c?*)(dog)", RegexOptions::None, RegexParseError::UnrecognizedControlCharacter, 9);
			ParseCheckOffset(L"(cat)(\\c`*)(dog)", RegexOptions::None, RegexParseError::UnrecognizedControlCharacter, 9);
			ParseCheckOffset(L"(cat)(\\c\\|*)(dog)", RegexOptions::None, RegexParseError::QuantifierAfterNothing, 11);
			ParseCheckOffset(L"^[abcd]{0,16}*$", RegexOptions::None, RegexParseError::NestedQuantifiersNotParenthesized, 14);
			ParseCheckOffset(L"(cat)\\c", RegexOptions::None, RegexParseError::MissingControlCharacter, 7);
			// Deep recursion
			ParseCheckOffset(L"(((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((", RegexOptions::None, RegexParseError::InsufficientClosingParentheses, 1183);
			// Scan control
			ParseCheckOffset(L"(cat)(\\\\c\\0*)(dog)", RegexOptions::None, RegexParseError::UnrecognizedControlCharacter, 9);
			ParseCheckOffset(L"(cat)(\\c\\[*)(dog)", RegexOptions::None, RegexParseError::UnterminatedBracket, 17);
			// Invalid grouping constructs
			ParseCheckOffset(L"(?<", RegexOptions::None, RegexParseError::InvalidGroupingConstruct, 3);
			ParseCheckOffset(L"(?>-", RegexOptions::None, RegexParseError::InsufficientClosingParentheses, 4);
			// Testgroup with options
			ParseCheckOffset(L"())", RegexOptions::None, RegexParseError::InsufficientOpeningParentheses, 3);
			ParseCheckOffset(L"[a-z-[aeiuo]", RegexOptions::None, RegexParseError::UnterminatedBracket, 12);
			ParseCheckOffset(L"[a-z-[aeiuo", RegexOptions::None, RegexParseError::UnterminatedBracket, 11);
			ParseCheckOffset(L"[a-z-[b]", RegexOptions::None, RegexParseError::UnterminatedBracket, 8);
			ParseCheckOffset(L"[a-z-[b", RegexOptions::None, RegexParseError::UnterminatedBracket, 7);
			ParseCheckOffset(L"[b-a]", RegexOptions::None, RegexParseError::ReversedCharacterRange, 4);
			ParseCheckOffset(L"[a-c]{2,1}", RegexOptions::None, RegexParseError::ReversedQuantifierRange, 10);
			ParseCheckOffset(L"\\d{2147483648}", RegexOptions::None, RegexParseError::QuantifierOrCaptureGroupOutOfRange, 13);
			ParseCheckOffset(L"[a-z-[b][", RegexOptions::None, RegexParseError::UnterminatedBracket, 9);
			ParseCheckOffset(L"(?()|||||)", RegexOptions::None, RegexParseError::AlternationHasTooManyConditions, 10);
			ParseCheckOffset(L"[^]", RegexOptions::None, RegexParseError::UnterminatedBracket, 3);
			ParseCheckOffset(L"??", RegexOptions::None, RegexParseError::QuantifierAfterNothing, 1);
			ParseCheckOffset(L"(?=*)", RegexOptions::None, RegexParseError::QuantifierAfterNothing, 4);
			ParseCheckOffset(L"((((((*))))))", RegexOptions::None, RegexParseError::QuantifierAfterNothing, 7);
		}

		static void ParseCheckOffset(wstring pattern, RegexOptions options, RegexParseError error, int offset = -1)
		{
			Parse(pattern, options, (int)error, offset);
		}

		static void ParseCheckOffset(wstring pattern, RegexOptions options)
		{
			Parse(pattern, options, -999, -1);
		}

		static void Parse(wstring pattern, RegexOptions options, int error, int offset = -1)
		{
			if (error != -999)
			{
				if (offset < 0)
					Assert::False(true);
				AssertThrows2(ArgumentException, invalid_argument, new Regex(pattern, options))
				return;
			}

			Assert::True(-1 == offset);
			LogActual(pattern, options, RegexParseError::Unknown, -1);

			// Nothing to assert here without having access to internals.
			new Regex(pattern, options); // Does not throw

			ParsePatternFragments(pattern, options);
		}

		static void LogActual(wstring pattern, RegexOptions options, RegexParseError error, int offset)
		{
			// To conveniently add new interesting patterns to these tests, add them to the code in the format:
			//
			// [InlineData("SOMEREGEX1", RegexOptions.None, null)]
			// [InlineData("SOMEREGEX2", RegexOptions.None, null)]
			// ...
			//
			// then uncomment the lines below, and the correct baseline will be written to the file, eg
			//
			// [InlineData(@"SOMEREGEX1", RegexOptions.None, RegexParseError::UnrecognizedEscape, 3)]
			// [InlineData(@"SOMEREGEX2", RegexOptions.None, InsufficientClosingParentheses, 2)]
			// ...
			// 
			//string s = (error == RegexParseError::Unknown) ?
			//    @$"        [InlineData(@""{pattern}"", RegexOptions.{options.ToString()}, null)]" :
			//    @$"        [InlineData(@""{pattern}"", RegexOptions.{options.ToString()}, RegexParseError::{error.ToString()}, {offset})]";

			// File.AppendAllText(@"/tmp/out.cs", s + "\n");
		}

		static void ParsePatternFragments(wstring pattern, RegexOptions options)
		{
			// Trim the input in various places and parse.
			// Verify that if it throws, it's the correct exception type
			for (size_t i = pattern.length() - 1; i > 0; i--)
			{
				if (i > pattern.length()) break;
				wstring str = pattern.substr(0, i);
				AssertThrows2(ArgumentException, invalid_argument,   Regex(str, options));
			}

			for (size_t i = 1; i < pattern.size(); i++)
			{
				if (i > pattern.length()) break;
				wstring str = pattern.substr(i);
				AssertThrows2(ArgumentException, invalid_argument,   Regex(str, options));
			}

			for (size_t i = 1; i < pattern.size(); i++)
			{
				if (i > pattern.length()) break;
				wstring str = pattern.substr(0, i) + pattern.substr(i + 1);
				AssertThrows2(ArgumentException, invalid_argument,   Regex(str, options));
			}
		}
	};
}
