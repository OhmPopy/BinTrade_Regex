#pragma once

#include "RegexNode.h"
#include "RegexOptions.h"
#include "RegexTree.h"
#include "RegexReplacement.h"
#include "RegexCharClass.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <stdexcept>

// This RegexParser class is internal to the Regex package.
// It builds a tree of RegexNodes from a regular expression

// Implementation notes:
//
// It would be nice to get rid of the comment modes, since the
// ScanBlank() calls are just kind of duct-taped in.


namespace Regexs
{
	class REGEX_X_API RegexParser final : public objectForRegex
	{

	public:
		RegexNode* _stack;
		RegexNode* _group;
		RegexNode* _alternation;
		RegexNode* _concatenation;
		RegexNode* _unit;

		std::vector<PtrObjectForRegex> _ptr_mgr;

		std::wstring _pattern;
		int _currentPos = 0;
		ComCultureInfo* _culture;

		int _autocap = 0;
		int _capcount = 0;
		int _captop = 0;
		int _capsize = 0;
		std::unordered_map<int, int> _caps;
		std::unordered_map<std::wstring, int> _capnames;
		std::vector<int> _capnumlist;
		std::vector<std::wstring> _capnamelist;

		RegexOptions _options = static_cast<RegexOptions>(0);
		std::vector<RegexOptions> _optionsStack;

		bool _ignoreNextParen = false;

		static int MaxValueDiv10;
		static int MaxValueMod10;

		/*
		 * This static call constructs a RegexTree from a regular expression
		 * pattern string and an option string.
		 *
		 * The method creates, drives, and drops a parser instance.
		 */
		virtual ~RegexParser();

		static RegexTree* Parse(const std::wstring& re, RegexOptions op);

		/*
		 * This static call constructs a flat concatenation node given
		 * a replacement pattern.
		 */
		static RegexReplacement* ParseReplacement(const std::wstring& rep, std::unordered_map<int, int>& caps, int capsize, std::unordered_map<std::wstring, int>& capnames, RegexOptions op);

		/*
		 * Escapes all metacharacters (including |,(,),[,{,|,^,$,*,+,?,\, spaces and #)
		 */
		static std::wstring Escape(const std::wstring& input);

		/*
		 * Escapes all metacharacters (including (,),[,],{,},|,^,$,*,+,?,\, spaces and #)
		 */
		static std::wstring Unescape(const std::wstring& input);

	private:
		/*
		 * Private constructor.
		 */
		RegexParser(ComCultureInfo* culture);

	public:
		/*
		 * Drops a string into the pattern buffer.
		 */
		void SetPattern(const std::wstring& Re);

		/*
		 * Resets parsing to the beginning of the pattern.
		 */
		void Reset(RegexOptions topopts);

		/*
		 * The main parsing function.
		 */
		RegexNode* ScanRegex();

		/*
		 * Simple parsing for replacement patterns
		 */
		RegexNode* ScanReplacement();

		/*
		 * Scans contents of [] (not including []'s), and converts to a
		 * RegexCharClass.
		 */
		RegexCharClass* ScanCharClass(bool caseInsensitive);

		/*
		 * Scans contents of [] (not including []'s), and converts to a
		 * RegexCharClass.
		 */
		RegexCharClass* ScanCharClass(bool caseInsensitive, bool scanOnly);

		/*
		 * Scans chars following a '(' (not counting the '('), and returns
		 * a RegexNode for the type of group scanned, or null if the group
		 * simply changed options (?cimsx-cimsx) or was a comment (#...).
		 */
		RegexNode* ScanGroupOpen();

		/*
		 * Scans whitespace or x-mode comments.
		 */
		void ScanBlank();

		/*
		 * Scans chars following a '\' (not counting the '\'), and returns
		 * a RegexNode for the type of atom scanned.
		 */
		RegexNode* ScanBackslash();

		/*
		 * Scans \-style backreferences and character escapes
		 */
		RegexNode* ScanBasicBackslash();

		/*
		 * Scans $ patterns recognized within replacment patterns
		 */
		RegexNode* ScanDollar();

		/*
		 * Scans a capture name: consumes word chars
		 */
		std::wstring ScanCapname();


		/*
		 * Scans up to three octal digits (stops before exceeding 0377).
		 */
		wchar_t ScanOctal();

		/*
		 * Scans any number of decimal digits (pegs value at 2^31-1 if too large)
		 */
		int ScanDecimal();

		/*
		 * Scans exactly c hex digits (c=2 for \xFF, c=4 for \uFFFF)
		 */
		wchar_t ScanHex(int c);

		/*
		 * Returns n <= 0xF for a hex digit.
		 */
		static int HexDigit(wchar_t ch);

		/*
		 * Grabs and converts an ascii control character
		 */
		wchar_t ScanControl();

		/*
		 * Returns true for options allowed only at the top level
		 */
		bool IsOnlyTopOption(RegexOptions option);

		/*
		 * Scans cimsx-cimsx option string, stops at the first unrecognized char.
		 */
		void ScanOptions();

		/*
		 * Scans \ code for escape codes that map to single unicode chars.
		 */
		wchar_t ScanCharEscape();

		/*
		 * Scans X for \p{X} or \P{X}
		 */
		std::wstring ParseProperty();

		/*
		 * Returns ReNode type for zero-length assertions with a \ code.
		 */
		int TypeFromCode(wchar_t ch);

		/*
		 * Returns option bit from single-char (?cimsx) code.
		 */
		static RegexOptions OptionFromCode(wchar_t ch);

		/*
		 * a prescanner for deducing the slots used for
		 * captures by doing a partial tokenization of the pattern.
		 */
		void CountCaptures();

		/*
		 * Notes a used capture slot
		 */
		void NoteCaptureSlot(int i, int pos);

		/*
		 * Notes a used capture slot
		 */
		void NoteCaptureName(const std::wstring& name, int pos);

		/*
		 * For when all the used captures are known: note them all at once
		 */
		void NoteCaptures(std::unordered_map<int, int>& caps, int capsize, std::unordered_map<std::wstring, int>& capnames);

		/*
		 * Assigns unused slot numbers to the capture names
		 */
		void AssignNameSlots();

		/*
		 * Looks up the slot number for a given name
		 */
		int CaptureSlotFromName(const std::wstring& capname);

		/*
		 * True if the capture slot was noted
		 */
		bool IsCaptureSlot(int i);

		/*
		 * Looks up the slot number for a given name
		 */
		bool IsCaptureName(const std::wstring& capname);

		/*
		 * True if N option disabling '(' autocapture is on.
		 */
		bool UseOptionN();

		/*
		 * True if I option enabling case-insensitivity is on.
		 */
		bool UseOptionI();

		/*
		 * True if M option altering meaning of $ and ^ is on.
		 */
		bool UseOptionM();

		/*
		 * True if S option altering meaning of . is on.
		 */
		bool UseOptionS();

		/*
		 * True if X option enabling whitespace/comment mode is on.
		 */
		bool UseOptionX();

		/*
		 * True if E option enabling ECMAScript behavior is on.
		 */
		bool UseOptionE();

		static constexpr unsigned char Q = 5; // quantifier
		static constexpr unsigned char S = 4; // ordinary stoppper
		static constexpr unsigned char Z = 3; // ScanBlank stopper
		static constexpr unsigned char X = 2; // whitespace
		static constexpr unsigned char E = 1; // should be escaped

		/*
		 * For categorizing ascii characters.
		*/
		static const std::vector<unsigned char> _category;

		/*
		 * Returns true for those characters that terminate a string of ordinary chars.
		 */
		static bool IsSpecial(wchar_t ch);

		/*
		 * Returns true for those characters that terminate a string of ordinary chars.
		 */
		static bool IsStopperX(wchar_t ch);

		/*
		 * Returns true for those characters that begin a quantifier.
		 */
		static bool IsQuantifier(wchar_t ch);

		bool IsTrueQuantifier();

		/*
		 * Returns true for whitespace.
		 */
		static bool IsSpace(wchar_t ch);

		/*
		 * Returns true for chars that should be escaped.
		 */
		static bool IsMetachar(wchar_t ch);


		/*
		 * Add a string to the last concatenate.
		 */
		void AddConcatenate(int pos, int cch, bool isReplacement);

		/*
		 * Push the parser state (in response to an open paren)
		 */
		void PushGroup();

		/*
		 * Remember the pushed state (in response to a ')')
		 */
		void PopGroup();

		/*
		 * True if the group stack is empty.
		 */
		bool EmptyStack();

		/*
		 * Start a new round for the parser state (in response to an open paren or string start)
		 */
		void StartGroup(RegexNode* openGroup);

		/*
		 * Finish the current concatenation (in response to a |)
		 */
		void AddAlternate();

		/*
		 * Finish the current quantifiable (when a quantifier is not found or is not possible)
		 */
		void AddConcatenate();

		/*
		 * Finish the current quantifiable (when a quantifier is found)
		 */
		void AddConcatenate(bool lazy, int min, int max);

		/*
		 * Returns the current unit
		 */
		RegexNode* Unit();

		/*
		 * Sets the current unit to a single char node
		 */
		void AddUnitOne(wchar_t ch);

		/*
		 * Sets the current unit to a single inverse-char node
		 */
		void AddUnitNotone(wchar_t ch);

		/*
		 * Sets the current unit to a single set node
		 */
		void AddUnitSet(const std::wstring& cc);

		/*
		 * Sets the current unit to a subtree
		 */
		void AddUnitNode(RegexNode* node);

		/*
		 * Sets the current unit to an assertion of the specified type
		 */
		void AddUnitType(int type);

		/*
		 * Finish the current group (in response to a ')' or end)
		 */
		void AddGroup();

		/*
		 * Saves options on a stack.
		 */
		void PushOptions();

		/*
		 * Recalls options from the stack.
		 */
		void PopOptions();

		/*
		 * True if options stack is empty.
		 */
		bool EmptyOptionsStack();

		/*
		 * Pops the option stack, but keeps the current options unchanged.
		 */
		void PopKeepOptions();

		/*
		 * Fills in an ArgumentException
		 */
		std::invalid_argument MakeException(const std::wstring& message);

		/*
		 * Returns the current parsing position.
		 */
		int Textpos();

		/*
		 * Zaps to a specific parsing position.
		 */
		void Textto(int pos);

		/*
		 * Returns the char at the right of the current parsing position and advances to the right.
		 */
		wchar_t MoveRightGetChar();

		/*
		 * Moves the current position to the right. 
		 */
		void MoveRight();

		void MoveRight(int i);

		/*
		 * Moves the current parsing position one to the left.
		 */
		void MoveLeft();

		/*
		 * Returns the char left of the current parsing position.
		 */
		wchar_t CharAt(int i);

		/*
		 * Returns the char right of the current parsing position.
		 */
		wchar_t RightChar();

		/*
		 * Returns the char i chars right of the current parsing position.
		 */
		wchar_t RightChar(int i);

		/*
		 * Number of characters to the right of the current parsing position.
		 */
		int CharsRight();
	};
}
