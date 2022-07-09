#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "objectForRegex.h"
#include "stringbuilder.h"
#include "TypeDef.h"

// The RegexReplacement class represents a substitution string for
// use when using regexs to search/replace, etc. It's logically
// a sequence intermixed (1) constant strings and (2) group numbers.


namespace Regexs
{
	class RegexNode;
	class Match;
	class Regex;

	class REGEX_X_API RegexReplacement final : public objectForRegex
	{
	public:
		/*
		 * Since RegexReplacement shares the same parser as Regex,
		 * the constructor takes a RegexNode which is a concatenation
		 * of constant strings and backreferences.
		 */
		RegexReplacement(const std::wstring& rep, RegexNode* concat, std::unordered_map<int, int>& _caps);

		std::wstring _rep;
		std::vector<std::wstring> _strings; // table of string constants
		std::vector<int> _rules; // negative -> group #, positive -> string #

		// constants for special insertion patterns

		static constexpr int Specials = 4;
		static constexpr int LeftPortion = -1;
		static constexpr int RightPortion = -2;
		static constexpr int LastGroup = -3;
		static constexpr int WholeString = -4;

	private:
		/*       
		 * Given a Match, emits into the StringBuilder the evaluated
		 * substitution pattern.
		 */
		void ReplacementImpl(StringBuilder* sb, Match* match);

		/*       
		 * Given a Match, emits into the List<String> the evaluated
		 * Right-to-Left substitution pattern.
		 */
		void ReplacementImplRTL(std::vector<std::wstring>& al, Match* match);

	public:
		/*
		 * The original pattern string
		 */
		std::wstring getPattern() const;

		/*
		 * Returns the replacement result for a single match
		 */
		std::wstring Replacement(Match* match);

		/*
		 * Three very similar algorithms appear below: replace (pattern),
		 * replace (evaluator), and split.
		 */


		/*
		 * Replaces all ocurrances of the regex in the string with the
		 * replacement pattern.
		 *
		 * Note that the special case of no matches is handled on its own:
		 * with no matches, the input string is returned unchanged.
		 * The right-to-left case is split out because StringBuilder
		 * doesn't handle right-to-left string building directly very well.
		 */
		std::wstring Replace(Regex* regex, const std::wstring& input, int count, int startat);

		/*
		 * Replaces all ocurrances of the regex in the string with the
		 * replacement evaluator.
		 *
		 * Note that the special case of no matches is handled on its own:
		 * with no matches, the input string is returned unchanged.
		 * The right-to-left case is split out because StringBuilder
		 * doesn't handle right-to-left string building directly very well.
		 */
		static std::wstring Replace(MatchEvaluator evaluator, Regex* regex, const std::wstring& input, int count, int startat);

		/*
		 * Does a split. In the right-to-left case we reorder the
		 * array to be forwards.
		 */
		static std::vector<std::wstring> Split(Regex* regex, const std::wstring& input, int count, int startat);
	};
}
