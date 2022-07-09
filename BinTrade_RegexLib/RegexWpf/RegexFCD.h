#pragma once

#include <string>
#include <vector>
#include <stdexcept>

#include "CultureInfo.h"
#include "objectForRegex.h"

// This RegexFCD class is internal to the Regex package.
// It builds a bunch of FC information (RegexFC) about
// the regex for optimization purposes.

// Implementation notes:
// 
// This step is as simple as walking the tree and emitting
// sequences of codes.


namespace Regexs
{
	class RegexFC;
	class RegexPrefix;
	class RegexTree;
	class RegexNode;
	class RegexCharClass;

	class REGEX_X_API RegexFCD final : public objectForRegex
	{
	private:
		std::vector<int> _intStack;
		int _intDepth;
		std::vector<RegexFC*> _fcStack;
		int _fcDepth;
		bool _skipAllChildren; // don't process any more children at the current level
		bool _skipchild; // don't process the current child.
		bool _failed;

		static int BeforeChild;
		static int AfterChild;

		// where the regex can be pegged

	public:

		std::vector<PtrObjectForRegex> _ptr_mgr;

		static int Beginning;
		static int Bol;
		static int Start;
		static int Eol;
		static int EndZ;
		static int End;
		static int Boundary;
		static int ECMABoundary;

		void Releesa();
		/*
		 * This is the one of the only two functions that should be called from outside.
		 * It takes a RegexTree and computes the set of chars that can start it.
		 */
		static RegexPrefix* FirstChars(RegexTree* t);

		/*
		 * This is a related computation: it takes a RegexTree and computes the
		 * leading substring if it see one. It's quite trivial and gives up easily.
		 */
		static void Prefix(RegexTree* tree, RegexPrefix& prefix, bool& is_prefix_empty);

		/*
		 * Yet another related computation: it takes a RegexTree and computes the
		 * leading anchors that it encounters.
		 */
		static int Anchors(RegexTree* tree);

		static std::wstring AnchorDescription(int anchors);

	private:

		/*
		 * Convert anchor type to anchor bit.
		 */
		static int AnchorFromType(int type);

		/*
		 * private constructor; can't be created outside
		 */
		RegexFCD();

		~RegexFCD();

		/*
		 * To avoid recursion, we use a simple integer stack.
		 * This is the push.
		 */
		void PushInt(int I);

		/*
		 * True if the stack is empty.
		 */
		bool IntIsEmpty();

		/*
		 * This is the pop.
		 */
		int PopInt();

		/*
		  * We also use a stack of RegexFC objects.
		  * This is the push.
		  */
		void PushFC(RegexFC* fc);

		/*
		 * True if the stack is empty.
		 */
		bool FCIsEmpty();

		/*
		 * This is the pop.
		 */
		RegexFC* PopFC();

		/*
		 * This is the top.
		 */
		RegexFC* TopFC();

		/*
		 * The main FC computation. It does a shortcutted depth-first walk
		 * through the tree and calls CalculateFC to emits code before
		 * and after each child of an interior node, and at each leaf.
		 */
		RegexFC* RegexFCFromRegexTree(RegexTree* tree);

		/*
		 * Called in Beforechild to prevent further processing of the current child
		 */
		void SkipChild();

		/*
		 * FC computation and shortcut cases for each node type
		 */
		void CalculateFC(int NodeType, RegexNode* node, int CurIndex);
	};

	class REGEX_X_API  RegexFC final : public objectForRegex
	{
	public:
		RegexCharClass* _cc;
		bool _nullable = false;
		bool _caseInsensitive = false;

		virtual ~RegexFC();

		RegexFC(bool nullable);

		RegexFC(wchar_t ch, bool not_Keyword, bool nullable, bool caseInsensitive);

		RegexFC(const std::wstring& charClass, bool nullable, bool caseInsensitive);

		bool AddFC(RegexFC* fc, bool concatenate);

		std::wstring GetFirstChars(ComCultureInfo* culture);

		bool IsCaseInsensitive();
	};

	class REGEX_X_API RegexPrefix final : public objectForRegex
	{
	public:
		std::wstring _prefix;
		bool _caseInsensitive = false;

		static RegexPrefix* _empty;

		RegexPrefix();
		RegexPrefix(const std::wstring& prefix, bool ci);

		~RegexPrefix();

		std::wstring getPrefix() const;

		bool getCaseInsensitive() const;

		static RegexPrefix* getEmpty();
	};
}
