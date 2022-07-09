#pragma once

#include "RegexRunner.h"
#include "RegexCode.h"
#include "RegexBoyerMoore.h"
#include <string>
#include <vector>

// This RegexInterpreter class is internal to the RegularExpression package.
// It executes a block of regular expression codes while consuming
// input.


namespace Regexs
{
	class REGEX_X_API RegexInterpreter final : public RegexRunner
	{
	public:
		int runoperator;
		std::vector<int> runcodes;
		int runcodepos;
		std::vector<std::wstring> runstrings;
		RegexCode* runcode;
		RegexPrefix* runfcPrefix;
		RegexBoyerMoore* runbmPrefix;
		int runanchors;
		bool runrtl;
		bool runci;
		ComCultureInfo* runculture;

		virtual ~RegexInterpreter();

		RegexInterpreter(RegexCode* code, ComCultureInfo* culture);

	protected:
		void InitTrackCount() override;

	private:
		void Advance();

		void Advance(int i);

		void Goto(int newpos);

		void Textto(int newpos);

		void Trackto(int newpos);

		int Textstart();

		int Textpos();

		// push onto the backtracking stack
		int Trackpos();

		void TrackPush();

		void TrackPush(int I1);

		void TrackPush(int I1, int I2);

		void TrackPush(int I1, int I2, int I3);

		void TrackPush2(int I1);

		void TrackPush2(int I1, int I2);

		void Backtrack();

		void SetOperator(int op);

		void TrackPop();

		// pop framesize items from the backtracking stack
		void TrackPop(int framesize);

		// Technically we are actually peeking at items already popped.  So if you want to 
		// get and pop the top item from the stack, you do 
		// TrackPop();
		// TrackPeek();
		int TrackPeek();

		// get the ith element down on the backtracking stack
		int TrackPeek(int i);

		// Push onto the grouping stack
		void StackPush(int I1);

		void StackPush(int I1, int I2);

		void StackPop();

		// pop framesize items from the grouping stack
		void StackPop(int framesize);

		// Technically we are actually peeking at items already popped.  So if you want to 
		// get and pop the top item from the stack, you do 
		// StackPop();
		// StackPeek();
		int StackPeek();

		// get the ith element down on the grouping stack
		int StackPeek(int i);

		int Operator();

		int Operand(int i);

		int Leftchars();

		int Rightchars();

		int Bump();

		int Forwardchars();

		wchar_t Forwardcharnext();

		bool Stringmatch(const std::wstring& str);

		bool Refmatch(int index, int len);

		void Backwardnext();

		wchar_t CharAt(int j);

	protected:
		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		// !!!! This function must be kept synchronized with GenerateFindFirstChar !!!!
		// !!!! in RegexCompiler.cs                                                !!!!
		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		bool FindFirstChar() override;

		void Go() override;
	};
}
