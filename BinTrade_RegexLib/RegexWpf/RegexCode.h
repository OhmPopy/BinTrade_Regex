#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <stdexcept>

#include "objectForRegex.h"

// This RegexCode class is internal to the regular expression package.
// It provides operator constants for use by the Builder and the Machine.

// Implementation notes:
//
// Regexps are built into RegexCodes, which contain an operation array,
// a string table, and some constants.
//
// Each operation is one of the codes below, followed by the integer
// operands specified for each op.
//
// Strings and sets are indices into a string table.


namespace Regexs
{
	class RegexBoyerMoore;
	class RegexPrefix;

	class REGEX_X_API RegexCode final : public objectForRegex
	{
		// the following primitive operations come directly from the parser

		// lef/back operands        description

	public:
		static int Onerep; // lef,back char,min,max    a {n}
		static int Notonerep; // lef,back char,min,max    .{n}
		static int Setrep; // lef,back set,min,max     [\d]{n}

		static int Oneloop; // lef,back char,min,max    a {,n}
		static int Notoneloop; // lef,back char,min,max    .{,n}
		static int Setloop; // lef,back set,min,max     [\d]{,n}

		static int Onelazy; // lef,back char,min,max    a {,n}?
		static int Notonelazy; // lef,back char,min,max    .{,n}?
		static int Setlazy; // lef,back set,min,max     [\d]{,n}?

		static int One; // lef      char            a
		static int Notone; // lef      char            [^a]
		static int Set; // lef      set             [a-z\s]  \w \s \d

		static int Multi; // lef      string          abcd
		static int Ref; // lef      group           \#

		static int Bol; //                          ^
		static int Eol; //                          $
		static int Boundary; //                          \b
		static int Nonboundary; //                          \B
		static int Beginning; //                          \A
		static int Start; //                          \G
		static int EndZ; //                          \Z
		static int End; //                          \Z

		static int Nothing; //                          Reject!


		static int Lazybranch; // back     jump            straight first
		static int Branchmark; // back     jump            branch first for loop
		static int Lazybranchmark; // back     jump            straight first for loop
		static int Nullcount; // back     val             set counter, null mark
		static int Setcount; // back     val             set counter, make mark
		static int Branchcount; // back     jump,limit      branch++ if zero<=c<limit
		static int Lazybranchcount; // back     jump,limit      same, but straight first
		static int Nullmark; // back                     save position
		static int Setmark; // back                     save position
		static int Capturemark; // back     group           define group
		static int Getmark; // back                     recall position
		static int Setjump; // back                     save backtrack state
		static int Backjump; //                          zap back to saved state
		static int Forejump; //                          zap backtracking state
		static int Testref; //                          backtrack if ref undefined
		static int Goto; //          jump            just go

		static int Prune; //                          prune it baby
		static int Stop; //                          done!

		static int ECMABoundary; //                          \b
		static int NonECMABoundary; //                          \B

		// modifiers for alternate modes

		static int Mask; // Mask to get unmodified ordinary operator
		static int Rtl; // bit to indicate that we're reverse scanning.
		static int Back; // bit to indicate that we're backtracking.
		static int Back2; // bit to indicate that we're backtracking on a second branch.
		static int Ci; // bit to indicate that we're case-insensitive.

		// the code

		std::vector<int> _codes; // the code

		std::vector<std::wstring> _strings; // the string/set table

		// not used! internal int[]           _sparseIndex;           // a list of the groups that are used
		int _trackcount; // how many instructions use backtracking
		std::unordered_map<int, int> _caps; // mapping of user group numbers -> impl group slots
		int _capsize; // number of impl group slots
		RegexPrefix* _fcPrefix; // the set of candidate first characters (may be null)
		RegexBoyerMoore* _bmPrefix; // the fixed prefix string as a Boyer-Moore machine (may be null)
		int _anchors; // the set of zero-length start anchors (RegexFCD.Bol, etc)
		bool _rightToLeft; // true if right to left

		// optimizations

		// constructor

		~RegexCode();

		RegexCode(std::vector<int>& codes, std::vector<std::wstring>& stringlist, int trackcount, std::unordered_map<int, int>& caps, int capsize, RegexBoyerMoore* bmPrefix, RegexPrefix* fcPrefix, int anchors, bool rightToLeft);

		static bool OpcodeBacktracks(int Op);

		static int OpcodeSize(int Opcode);

		static std::invalid_argument MakeException(const std::wstring& message);

		// Debug only code below
	};
}
