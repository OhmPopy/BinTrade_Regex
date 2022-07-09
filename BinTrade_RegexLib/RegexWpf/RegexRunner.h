#pragma once

#include <string>
#include <vector>

#include "objectForRegex.h"
#include "TimeSpan.h"

// This RegexRunner class is a base class for compiled regex code.

// Implementation notes:
// 
// RegexRunner provides a common calling convention and a common
// runtime environment for the interpreter and the compiled code.
//
// It provides the driver code that call's the subclass's Go()
// method for either scanning or direct execution.
//
// It also maintains memory allocation for the backtracking stack,
// the grouping stack and the longjump crawlstack, and provides
// methods to push new subpattern match results into (or remove
// backtracked results from) the Match instance.


namespace Regexs
{
	class Match;
	class Regex;

	class REGEX_X_API RegexRunner : public objectForRegex
	{
	protected:
		int runtextbeg; // beginning of text to search
		int runtextend; // end of text to search
		int runtextstart; // starting point for search

		const std::wstring* runtext; // text to search
		int runtextpos; // current position in text

		std::vector<int> runtrack; // The backtracking stack.  Opcodes use this to store data regarding

		int runtrackpos; // what they have matched and where to backtrack to.  Each "frame" on
		// the stack takes the form of [CodePosition Data1 Data2...], where 
		// CodePosition is the position of the current opcode and 
		// the data values are all optional.  The CodePosition can be negative, and
		// these values (also called "back2") are used by the BranchMark family of opcodes
		// to indicate whether they are backtracking after a successful or failed
		// match.  
		// When we backtrack, we pop the CodePosition off the stack, set the current
		// instruction pointer to that code position, and mark the opcode 
		// with a backtracking flag ("Back").  Each opcode then knows how to 
		// handle its own data. 

		std::vector<int> runstack; // This stack is used to track text positions across different opcodes.

		int runstackpos; // For example, in /(a*b)+/, the parentheses result in a SetMark/CaptureMark
		// pair. SetMark records the text position before we match a*b.  Then
		// CaptureMark uses that position to figure out where the capture starts.
		// Opcodes which push onto this stack are always paired with other opcodes
		// which will pop the value from it later.  A successful match should mean
		// that this stack is empty. 

		std::vector<int> runcrawl; // The crawl stack is used to keep track of captures.  Every time a group

		int runcrawlpos; // has a capture, we push its group number onto the runcrawl stack.  In
		// the case of a balanced match, we push BOTH groups onto the stack. 

		int runtrackcount; // count of states that may do backtracking

		Match* runmatch; // result object
		Regex* runregex; // regex object

	private:
		int timeout; // timeout in millisecs (needed for actual)
		bool ignoreTimeout;
		int timeoutOccursAt;


		// GPaperin: We have determined this value in a series of experiments where x86 retail
		// builds (ono-lab-optimised) were run on different pattern/input pairs. Larger values
		// of TimeoutCheckFrequency did not tend to increase performance; smaller values
		// of TimeoutCheckFrequency tended to slow down the execution.
		static constexpr int TimeoutCheckFrequency = 1000;
		int timeoutChecksToSkip;

	public:
		virtual ~RegexRunner();

	protected:
		RegexRunner();

		/*
		 * Scans the string to find the first match. Uses the Match object
		 * both to feed text in and as a place to store matches that come out.
		 *
		 * All the action is in the abstract Go() method defined by subclasses. Our
		 * responsibility is to load up the class members (as done here) before
		 * calling Go.
		 *
		 * <
		*/
		Match* Scan(Regex* regex, const std::wstring* text, int textbeg, int textend, int textstart, int prevlen, bool quick);

	public:
		Match* Scan(Regex* regex, const std::wstring* text, int textbeg, int textend, int textstart, int prevlen, bool quick, TimeSpan timeout);

		void CheckTimeout();

	private:
		void StartTimeoutWatch();

		void DoCheckTimeout();

	protected:
		/*
		 * The responsibility of Go() is to run the regular expression at
		 * runtextpos and call Capture() on all the captured subexpressions,
		 * then to leave runtextpos at the ending position. It should leave
		 * runtextpos where it started if there was no match.
		 */
		virtual void Go() = 0;

		/*
		 * The responsibility of FindFirstChar() is to advance runtextpos
		 * until it is at the next position which is a candidate for the
		 * beginning of a successful match.
		 */
		virtual bool FindFirstChar() = 0;

		/*
		 * InitTrackCount must initialize the runtrackcount field; this is
		 * used to know how large the initial runtrack and runstack arrays
		 * must be.
		 */
		virtual void InitTrackCount() = 0;

	private:
		/*
		 * Initializes all the data members that are used by Go()
		 */
		void InitMatch();

		/*
		 * Put match in its canonical form before returning it.
		 */
		Match* TidyMatch(bool quick);

	protected:
		/*
		 * Called by the implemenation of Go() to increase the size of storage
		 */
		void EnsureStorage();

		/*
		 * Called by the implemenation of Go() to decide whether the pos
		 * at the specified index is a boundary or not. It's just not worth
		 * emitting inline code for this logic.
		 */
		virtual bool IsBoundary(int index, int startpos, int endpos);

		bool IsECMABoundary(int index, int startpos, int endpos);

		static bool CharInSet(wchar_t ch, const std::wstring& set, const std::wstring& category);

		static bool CharInClass(wchar_t ch, const std::wstring& charClass);

		/*
		 * Called by the implemenation of Go() to increase the size of the
		 * backtracking stack.
		 */
		void DoubleTrack();

		/*
		 * Called by the implemenation of Go() to increase the size of the
		 * grouping stack.
		 */
		void DoubleStack();

		/*
		 * Increases the size of the longjump unrolling stack.
		 */
		void DoubleCrawl();

		/*
		 * Save a number on the longjump unrolling stack
		 */
		void Crawl(int i);

		/*
		 * Remove a number from the longjump unrolling stack
		 */
		int Popcrawl();

		/*
		 * Get the height of the stack
		 */
		int Crawlpos();

		/*
		 * Called by Go() to capture a subexpression. Note that the
		 * capnum used here has already been mapped to a non-sparse
		 * index (by the code generator RegexWriter).
		 */
		void Capture(int capnum, int start, int end);

		/*
		 * Called by Go() to capture a subexpression. Note that the
		 * capnum used here has already been mapped to a non-sparse
		 * index (by the code generator RegexWriter).
		 */
		void TransferCapture(int capnum, int uncapnum, int start, int end);

		/*
		 * Called by Go() to revert the last capture
		 */
		void Uncapture();

		/*
		 * Call out to runmatch to get around visibility issues
		 */
		bool IsMatched(int cap);

		/*
		 * Call out to runmatch to get around visibility issues
		 */
		int MatchIndex(int cap);

		/*
		 * Call out to runmatch to get around visibility issues
		 */
		int MatchLength(int cap);

		/*
		* Dump the current state
		*/
		virtual void DumpState();

		static std::wstring StackDescription(std::vector<int> A, int Index);

		virtual std::wstring TextposDescription();
	};
}
