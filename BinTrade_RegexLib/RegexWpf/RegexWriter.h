#pragma once

#include "RegexCode.h"
#include "RegexTree.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <stdexcept>

// This RegexWriter class is internal to the Regex package.
// It builds a block of regular expression codes (RegexCode)
// from a RegexTree parse tree.

// Implementation notes:
// 
// This step is as simple as walking the tree and emitting
// sequences of codes.
//


namespace Regexs
{
	class REGEX_X_API RegexWriter final : public objectForRegex
	{
	public:
		std::vector<int> _intStack;
		int _depth;
		std::vector<int> _emitted;
		int _curpos;
		std::unordered_map<std::wstring, int> _stringhash;

		std::vector<std::wstring> _stringtable;

		// not used! internal int         _stringcount;
		bool _counting;
		int _count;
		int _trackcount;
		std::unordered_map<int, int> _caps;

		static int BeforeChild;
		static int AfterChild;

		/*
		 * This is the only function that should be called from outside.
		 * It takes a RegexTree and creates a corresponding RegexCode.
		 */
		static RegexCode* Write(RegexTree* t);

	private:
		/*
		 * private constructor; can't be created outside
		 */
		RegexWriter();

	public:
		/*
		 * To avoid recursion, we use a simple integer stack.
		 * This is the push.
		 */
		void PushInt(int I);

		/*
		 * True if the stack is empty.
		 */
		bool EmptyStack();

		/*
		 * This is the pop.
		 */
		int PopInt();

		/*
		 * Returns the current position in the emitted code.
		 */
		int CurPos();

		/*
		 * Fixes up a jump instruction at the specified offset
		 * so that it jumps to the specified jumpDest.
		 */
		void PatchJump(int Offset, int jumpDest);

		/*
		 * Emits a zero-argument operation. Note that the emit
		 * functions all run in two modes: they can emit code, or
		 * they can just count the size of the code.
		 */
		void Emit(int op);

		/*
		 * Emits a one-argument operation.
		 */
		void Emit(int op, int opd1);

		/*
		 * Emits a two-argument operation.
		 */
		void Emit(int op, int opd1, int opd2);

		/*
		 * Returns an index in the string table for a string;
		 * uses a hashtable to eliminate duplicates.
		 */
		int StringCode(const std::wstring& str);

		/*
		 * Just returns an exception; should be dead code
		 */
		std::invalid_argument MakeException(const std::wstring& message);

		/*
		 * When generating code on a regex that uses a sparse set
		 * of capture slots, we hash them to a dense set of indices
		 * for an array of capture slots. Instead of doing the hash
		 * at match time, it's done at compile time, here.
		 */
		int MapCapnum(int capnum);

		/*
		 * The top level RegexCode generator. It does a depth-first walk
		 * through the tree and calls EmitFragment to emits code before
		 * and after each child of an interior node, and at each leaf.
		 *
		 * It runs two passes, first to count the size of the generated
		 * code, and second to generate the code.
		 *
		 * <

*/
		RegexCode* RegexCodeFromRegexTree(RegexTree* tree);

		/*
		 * The main RegexCode generator. It does a depth-first walk
		 * through the tree and calls EmitFragment to emits code before
		 * and after each child of an interior node, and at each leaf.
		 */
		void EmitFragment(const int& nodetype, RegexNode* node, int CurIndex);
	};
}
