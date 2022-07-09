#pragma once

#include <string>
#include <vector>
#include "objectForRegex.h"
#include  <Com/ComCultureInfo.h>

// The RegexBoyerMoore object precomputes the Boyer-Moore
// tables for fast string scanning. These tables allow
// you to scan for the first occurance of a string within
// a large body of text without examining every character.
// The performance of the heuristic depends on the actual
// string and the text being searched, but usually, the longer
// the string that is being searched for, the fewer characters
// need to be examined.


namespace Regexs
{
	class REGEX_X_API RegexBoyerMoore final : public objectForRegex
	{
	public:
		std::vector<int> _positive;
		std::vector<int> _negativeASCII;
		std::vector<std::vector<int>> _negativeUnicode;
		std::wstring _pattern;
		int _lowASCII;
		int _highASCII;
		bool _rightToLeft;
		bool _caseInsensitive;
		ComCultureInfo* _culture;

		static constexpr int infinite = 0x7FFFFFFF;

		/*
		 * Constructs a Boyer-Moore state machine for searching for the string
		 * pattern. The string must not be zero-length.
		 */
		virtual ~RegexBoyerMoore();

		RegexBoyerMoore(const std::wstring& pattern, bool caseInsensitive, bool rightToLeft, ComCultureInfo* culture);

	private:
		bool MatchPattern(const std::wstring* text, int index);

	public:
		/*
		 * When a regex is anchored, we can do a quick IsMatch test instead of a Scan
		 */
		bool IsMatch(const std::wstring* text, int index, int beglimit, int endlimit);

		/*
		 * Scan uses the Boyer-Moore algorithm to find the first occurrance
		 * of the specified string within text, beginning at index, and
		 * constrained within beglimit and endlimit.
		 *
		 * The direction and case-sensitivity of the match is determined
		 * by the arguments to the RegexBoyerMoore constructor.
		 */
		int Scan(const std::wstring* text, int index, int beglimit, int endlimit);

		/*
		 * Used when dumping for debugging.
		 */
		std::wstring ToString();

		std::wstring Dump(std::wstring indent);
	};
}
