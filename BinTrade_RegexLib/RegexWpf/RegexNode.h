#pragma once

#include "RegexCode.h"
#include "RegexOptions.h"
#include <string>
#include <vector>

// This RegexNode class is internal to the Regex package.
// It is built into a parsed tree for a regular expression.

// Implementation notes:
// 
// Since the node tree is a temporary data structure only used
// during compilation of the regexp to integer codes, it's
// designed for clarity and convenience rather than
// space efficiency.
//
// RegexNodes are built into a tree, linked by the _children list.
// Each node also has a _parent and _ichild member indicating
// its parent and which child # it is in its parent's list.
//
// RegexNodes come in as many types as there are constructs in
// a regular expression, for example, "concatenate", "alternate",
// "one", "rept", "group". There are also node types for basic
// peephole optimizations, e.g., "onerep", "notsetrep", etc.
//
// Because perl 5 allows "lookback" groups that scan backwards,
// each node also gets a "direction". Normally the value of
// boolean _backward = false.
//
// During parsing, top-level nodes are also stacked onto a parse
// stack (a stack of trees). For this purpose we have a _next
// pointer. [Note that to save a few bytes, we could overload the
// _parent pointer instead.]
//
// On the parse stack, each tree has a "role" - basically, the
// nonterminal in the grammar that the parser has currently
// assigned to the tree. That code is stored in _role.
//
// Finally, some of the different kinds of nodes have data.
// Two integers (for the looping constructs) are stored in
// _operands, an an object (either a string or a set)
// is stored in _data


namespace Regexs
{
	class REGEX_X_API RegexNode final : public objectForRegex
	{
		/*
		 * RegexNode types
		 */

		// the following are leaves, and correspond to primitive operations

		//    static final int Onerep     = RegexCode.Onerep;     // c,n      a {n}
		//    static final int Notonerep  = RegexCode.Notonerep;  // c,n      .{n}
		//    static final int Setrep     = RegexCode.Setrep;     // set,n    \d {n}

		const std::wstring Space = L"                                ";
		static std::vector<std::wstring> TypeStr;

	public:
		// (note: End               = 21;)

		// interior nodes do not correpond to primitive operations, but
		// control structures compositing other operations

		// concat and alternate take n children, and can run forward or backwards

		static int Nothing; //          []
		static int Empty; //          ()

		static int Alternate; //          a|b
		static int Concatenate; //          ab

		static int Loop; // m,x      * + ? {,}
		static int Lazyloop; // m,x      *? +? ?? {,}?

		static int Capture; // n        ()
		static int Group; //          (?:)
		static int Require; //          (?=) (?<=)
		static int Prevent; //          (?!) (?<!)
		static int Greedy; //          (?>) (?<)
		static int Testref; //          (?(n) | )
		static int Testgroup; //          (?(...) | )

		/*
		 * RegexNode data members
		 * 
		 */

		int _type;

		std::vector<RegexNode*> _children;

		std::wstring _str;
		wchar_t _ch;
		int _m;
		int _n;
		RegexOptions _options; //= static_cast<RegexOptions>(0)

		RegexNode* _next;

		virtual ~RegexNode();

		RegexNode(std::vector<PtrObjectForRegex>* _ptr_mgr, int type, RegexOptions options);

		RegexNode(std::vector<PtrObjectForRegex>* _ptr_mgr, int type, RegexOptions options, wchar_t ch);

		RegexNode(std::vector<PtrObjectForRegex>* _ptr_mgr, int type, RegexOptions options, const std::wstring& str);

		RegexNode(std::vector<PtrObjectForRegex>* _ptr_mgr, int type, RegexOptions options, int m);

		RegexNode(std::vector<PtrObjectForRegex>* _ptr_mgr, int type, RegexOptions options, int m, int n);

		bool UseOptionR();

		RegexNode* ReverseLeft();


		// Pass type as OneLazy or OneLoop
		void MakeRep(int type, int min, int max);

		/*
		 * Reduce
		 *
		 * Removes redundant nodes from the subtree, and returns a reduced subtree.
		 */
		RegexNode* Reduce(std::vector<PtrObjectForRegex>* _ptr_mgr);


		/*
		 * StripEnation:
		 *
		 * Simple optimization. If a concatenation or alternation has only
		 * one child strip out the intermediate node. If it has zero children,
		 * turn it into an empty.
		 * 
		 */

		RegexNode* StripEnation(std::vector<PtrObjectForRegex>* _ptr_mgr, int emptyType);

		/*
		 * ReduceGroup:
		 *
		 * Simple optimization. Once parsed into a tree, noncapturing groups
		 * serve no function, so strip them out.
		 */

		RegexNode* ReduceGroup();

		/*
		 * ReduceRep:
		 *
		 * Nested repeaters just get multiplied with each other if they're not
		 * too lumpy
		 */

		RegexNode* ReduceRep(std::vector<PtrObjectForRegex>* _ptr_mgr);

		/*
		 * ReduceSet:
		 *
		 * Simple optimization. If a set is a singleton, an inverse singleton,
		 * or empty, it's transformed accordingly.
		 */

		RegexNode* ReduceSet();

		/*
		 * ReduceAlternation:
		 *
		 * Basic optimization. Single-letter alternations can be replaced
		 * by faster set specifications, and nested alternations with no
		 * intervening operators can be flattened:
		 *
		 * a|b|c|def|g|h -> [a-c]|def|[gh]
		 * apple|(?:orange|pear)|grape -> apple|orange|pear|grape
		 *
		 * <
*/

		RegexNode* ReduceAlternation(std::vector<PtrObjectForRegex>* _ptr_mgr);

		/*
		 * ReduceConcatenation:
		 *
		 * Basic optimization. Adjacent strings can be concatenated.
		 *
		 * (?:abc)(?:def) -> abcdef
		 */

		RegexNode* ReduceConcatenation(std::vector<PtrObjectForRegex>* _ptr_mgr);

		RegexNode* MakeQuantifier(std::vector<PtrObjectForRegex>* _ptr_mgr, bool lazy, int min, int max);

		void AddChild(std::vector<PtrObjectForRegex>* _ptr_mgr, RegexNode* newChild);

		RegexNode* Child(int i);

		int ChildCount();

		int Type();


		std::wstring Description();

		std::wstring Dump();
	};
}
