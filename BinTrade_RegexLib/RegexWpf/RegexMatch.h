#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include "RegexGroup.h"

// Match is the result class for a regex search.
// It returns the location, length, and substring for
// the entire match as well as every captured group.

// Match is also used during the search to keep track of each capture for each group.  This is
// done using the "_matches" array.  _matches[x] represents an array of the captures for group x.  
// This array consists of start and length pairs, and may have empty entries at the end.  _matchcount[x] 
// stores how many captures a group has.  Note that _matchcount[x]*2 is the length of all the valid
// values in _matches.  _matchcount[x]*2-2 is the Start of the last capture, and _matchcount[x]*2-1 is the
// Length of the last capture
//
// For example, if group 2 has one capture starting at position 4 with length 6, 
// _matchcount[2] == 1
// _matches[2][0] == 4
// _matches[2][1] == 6
//
// Values in the _matches array can also be negative.  This happens when using the balanced match 
// construct, "(?<start-end>...)".  When the "end" group matches, a capture is added for both the "start" 
// and "end" groups.  The capture added for "start" receives the negative values, and these values point to 
// the next capture to be balanced.  They do NOT point to the capture that "end" just balanced out.  The negative 
// values are indices into the _matches array transformed by the formula -3-x.  This formula also untransforms. 
// 


namespace Regexs
{
	class GroupCollection;
	class Regex;

	/// <devdoc>
	///    <para>
	///       Represents 
	///          the results from a single regular expression match.
	///       </para>
	///    </devdoc>
	class REGEX_X_API Match : public Group
	{
		//static init 
		Match(Regex* regex, int capcount, const std::wstring* text, int begpos, int len, int startpos, bool is_static_init);
	public:
		static Match* _empty;
		GroupCollection* _groupcoll;

		// input to the match
		Regex* _regex;
		int _textbeg; 
		int _textpos;
		int _textend;
		int _textstart;

		// output from the match
		std::vector<std::vector<int>> _matches;
		std::vector<int> _matchcount;

		bool _balancing; // whether we've done any balancing with this match.  If we
		// have done balancing, we'll need to do extra work in Tidy().

		/// <devdoc>
		///    <para>
		///       Returns an empty Match object.
		///    </para>
		/// </devdoc>
		virtual ~Match();

		static Match* getEmpty();

		/*
		 * Nonpublic constructor
		 */
		Match(Regex* regex, int capcount, const std::wstring* text, int begpos, int len, int startpos);

		/*
		 * Nonpublic set-text method
		 */
		virtual void Reset(Regex* regex, const std::wstring* text, int textbeg, int textend, int textstart);

		virtual GroupCollection* getGroups();

		__declspec(property(get = getGroups)) GroupCollection* Groups;
		__declspec(property(get = getSuccess)) bool Success;

		/*
		 * Returns the next match
		 */
		/// <devdoc>
		///    <para>Returns a new Match with the results for the next match, starting
		///       at the position at which the last match ended (at the character beyond the last
		///       matched character).</para>
		/// </devdoc>
		Match* NextMatch();


		/*
		 * Return the result string (using the replacement pattern)
		 */
		/// <devdoc>
		///    <para>
		///       Returns the expansion of the passed replacement pattern. For
		///       example, if the replacement pattern is ?$1$2?, Result returns the concatenation
		///       of Group(1).ToString() and Group(2).ToString().
		///    </para>
		/// </devdoc>
		virtual std::wstring Result(const std::wstring& replacement);

		/*
		 * Used by the replacement code
		 */
		virtual std::wstring GroupToStringImpl(int groupnum);

		/*
		 * Used by the replacement code
		 */
		std::wstring LastGroupToStringImpl();


		/*
		 * Convert to a thread-safe object by precomputing cache contents
		 */
		/// <devdoc>
		///    <para>
		///       Returns a Match instance equivalent to the one supplied that is safe to share
		///       between multiple threads.
		///    </para>
		/// </devdoc>
		static Match* Synchronized(Match* inner);

		/*
		 * Nonpublic builder: add a capture to the group specified by "cap"
		 */
		virtual void AddMatch(int cap, int start, int len);

		/*
		 * Nonpublic builder: Add a capture to balance the specified group.  This is used by the 
		                      balanced match construct. (?<foo-foo2>...)

		   If there were no such thing as backtracking, this would be as simple as calling RemoveMatch(cap).
		   However, since we have backtracking, we need to keep track of everything. 
		 */
		virtual void BalanceMatch(int cap);

		/*
		 * Nonpublic builder: removes a group match by capnum
		 */
		virtual void RemoveMatch(int cap);

		/*
		 * Nonpublic: tells if a group was matched by capnum
		 */
		virtual bool IsMatched(int cap);

		/*
		 * Nonpublic: returns the index of the last specified matched group by capnum
		 */
		virtual int MatchIndex(int cap);

		/*
		 * Nonpublic: returns the length of the last specified matched group by capnum
		 */
		virtual int MatchLength(int cap);

		/*
		 * Nonpublic: tidy the match so that it can be used as an immutable result
		 */
		virtual void Tidy(int textpos);

		virtual std::wstring Dump();

		//Giải phóng thủ công khi được tạo bởi phương thức tĩnh không xóa tự động cùng regex
		void Release();

		friend class MatchCollection;
		friend class GroupCollection;
		friend class Regex;
	};


	/*
	 * MatchSparse is for handling the case where slots are
	 * sparsely arranged (e.g., if somebody says use slot 100000)
	 */
	class REGEX_X_API MatchSparse : public Match
	{
	public:
		// the lookup hashtable
		std::unordered_map<int, int> _caps;

		/*
		 * Nonpublic constructor
		 */
		MatchSparse(Regex* regex, std::unordered_map<int, int>& caps, int capcount, const std::wstring* text, int begpos, int len, int startpos);

		GroupCollection* getGroups();

		friend class Regex;
	};
}
