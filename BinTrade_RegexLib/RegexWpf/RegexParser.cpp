#include "RegexParser.h"

#include <unordered_set>

#include "stringhelper.h"
#include "UtilsCollections.h"
#include "UtilsString.h"


namespace Regexs
{
	int RegexParser::MaxValueDiv10 = INT_MAX / 10;
	int RegexParser::MaxValueMod10 = INT_MAX % 10;

	RegexParser::~RegexParser()
	{
		//delete _stack;
		//delete _group;
		//delete _alternation;
		//delete _concatenation;
		//delete _unit;
		//delete _culture;
	}

	RegexTree* RegexParser::Parse(const std::wstring& re, RegexOptions op)
	{
		RegexParser* p;
		RegexNode* root;
		std::vector<std::wstring> capnamelist;

		p = new RegexParser(((int)op & (int)RegexOptions::CultureInvariant) != 0 ? ComCultureInfo::InvariantCulture() : ComCultureInfo::CurrentCulture());

		p->_options = op;

		p->SetPattern(re);
		p->CountCaptures();
		p->Reset(op);
		root = p->ScanRegex();

		if (p->_capnamelist.empty())
		{
			capnamelist = std::vector<std::wstring>();
		}
		else
		{
			capnamelist = p->_capnamelist;
		}


		RegexTree* value = new RegexTree(root, p->_caps, p->_capnumlist, p->_captop, p->_capnames, capnamelist, op, p->_ptr_mgr);
		delete p;
		return value;
	}

	RegexReplacement* RegexParser::ParseReplacement(const std::wstring& rep, std::unordered_map<int, int>& caps, int capsize, std::unordered_map<std::wstring, int>& capnames, RegexOptions op)
	{
		RegexParser* p;
		RegexNode* root;

		p = new RegexParser(((int)op & (int)RegexOptions::CultureInvariant) != 0 ? ComCultureInfo::InvariantCulture() : ComCultureInfo::CurrentCulture());

		p->_options = op;

		p->NoteCaptures(caps, capsize, capnames);
		p->SetPattern(rep);
		root = p->ScanReplacement();


		RegexReplacement* value = new RegexReplacement(rep, root, caps);


		std::unordered_set<std::uintptr_t> release;
		for (size_t i = 0; i < p->_ptr_mgr.size(); ++i)
		{
			auto address = p->_ptr_mgr[i]._address;
			bool need = true;
			const bool is_in = release.find(address) != release.end();
			if (is_in)
				need = false;

			if (need)
			{
				if (p->_ptr_mgr[i].object)
				{
					release.insert(address);
					delete p->_ptr_mgr[i].object;
				}
			}
		}
		delete p;
		return value;
	}

	std::wstring RegexParser::Escape(const std::wstring& input)
	{
		for (size_t i = 0; i < input.length(); i++)
		{
			if (IsMetachar(input[i]))
			{
				StringBuilder sb = StringBuilder();
				wchar_t ch = input[i];
				int lastpos;

				sb.Append(input, 0, i);
				do
				{
					sb.AppendChar(L'\\');
					switch (ch)
					{
					case L'\n':
						ch = L'n';
						break;
					case L'\r':
						ch = L'r';
						break;
					case L'\t':
						ch = L't';
						break;
					case L'\f':
						ch = L'f';
						break;
					default: ;
					}

					sb.AppendChar(ch);
					i++;
					lastpos = i;

					while (i < input.length())
					{
						ch = input[i];
						if (IsMetachar(ch))
						{
							break;
						}

						i++;
					}

					sb.Append(input, lastpos, i - lastpos);
				}
				while (i < input.length());

				return sb.ToString();
			}
		}

		return input;
	}

	std::wstring RegexParser::Unescape(const std::wstring& input)
	{
		for (size_t i = 0; i < input.length(); i++)
		{
			if (input[i] == L'\\')
			{
				StringBuilder sb = StringBuilder();
				RegexParser* p = new RegexParser(ComCultureInfo::InvariantCulture());
				int lastpos;
				p->SetPattern(input);

				sb.Append(input, 0, i);
				do
				{
					i++;
					p->Textto(i);
					if (i < input.length())
					{
						sb.AppendChar(p->ScanCharEscape());
					}
					i = p->Textpos();
					lastpos = i;
					while (i < input.length() && input[i] != L'\\')
					{
						i++;
					}
					sb.Append(input, lastpos, i - lastpos);
				}
				while (i < input.length());

				delete p;
				return sb.ToString();
			}
		}

		return input;
	}

	RegexParser::RegexParser(ComCultureInfo* culture): _stack(NULL), _group(NULL), _alternation(NULL), _concatenation(NULL), _unit(NULL)
	{
		_culture = culture;
		_optionsStack = std::vector<RegexOptions>();
		_caps = std::unordered_map<int, int>();
	}

	void RegexParser::SetPattern(const std::wstring& Re)
	{
		if (Re == L"")
		{
		}
		_pattern = Re;
		_currentPos = 0;
	}

	void RegexParser::Reset(RegexOptions topopts)
	{
		_currentPos = 0;
		_autocap = 1;
		_ignoreNextParen = false;

		if (_optionsStack.size() > 0)
		{
			//_optionsStack.RemoveRange(0, _optionsStack.Count - 1);
			const int count = _optionsStack.size() - 1;
			_optionsStack.erase(_optionsStack.begin(), _optionsStack.begin() + count);
		}

		_options = topopts;
		_stack = NULL;
	}

	RegexNode* RegexParser::ScanRegex()
	{
		wchar_t ch = L'@'; // nonspecial ch, means at beginning
		bool isQuantifier = false;

		RegexNode* tempVar = new RegexNode(&_ptr_mgr, RegexNode::Capture, _options, 0, -1);
		StartGroup(tempVar);

		while (CharsRight() > 0)
		{
			bool wasPrevQuantifier = isQuantifier;
			isQuantifier = false;

			ScanBlank();

			int startpos = Textpos();

			// move past all of the normal characters.  We'll stop when we hit some kind of control character, 
			// or if IgnorePatternWhiteSpace is on, we'll stop when we see some whitespace. 
			if (UseOptionX())
			{
				while (CharsRight() > 0 && (!IsStopperX(ch = RightChar()) || ch == L'{' && !IsTrueQuantifier()))
				{
					MoveRight();
				}
			}
			else
			{
				while (CharsRight() > 0 && (!IsSpecial(ch = RightChar()) || ch == L'{' && !IsTrueQuantifier()))
				{
					MoveRight();
				}
			}

			int endpos = Textpos();

			ScanBlank();

			if (CharsRight() == 0)
			{
				ch = L'!'; // nonspecial, means at end
			}
			else if (IsSpecial(ch = RightChar()))
			{
				isQuantifier = IsQuantifier(ch);
				MoveRight();
			}
			else
			{
				ch = L' '; // nonspecial, means at ordinary char
			}

			if (startpos < endpos)
			{
				int cchUnquantified = endpos - startpos - (isQuantifier ? 1 : 0);

				wasPrevQuantifier = false;

				if (cchUnquantified > 0)
				{
					AddConcatenate(startpos, cchUnquantified, false);
				}

				if (isQuantifier)
				{
					AddUnitOne(CharAt(endpos - 1));
				}
			}

			switch (ch)
			{
			case L'!':
				goto BreakOuterScan;

			case L' ':
				goto ContinueOuterScan;

			case L'[':
				{
					auto temp = ScanCharClass(UseOptionI());
					AddUnitSet(temp->ToStringClass());
					delete temp;
				}

				break;

			case L'(':
				{
					RegexNode* grouper;

					PushOptions();

					if (NULL == (grouper = ScanGroupOpen()))
					{
						PopKeepOptions();
					}
					else
					{
						PushGroup();
						StartGroup(grouper);
					}
				}
				continue;

			case L'|':
				AddAlternate();
				goto ContinueOuterScan;

			case L')':
				if (EmptyStack())
				{
					throw MakeException(L"TooManyParens");
				}

				AddGroup();
				PopGroup();
				PopOptions();

				if (Unit() == NULL)
				{
					goto ContinueOuterScan;
				}
				break;

			case L'\\':
				AddUnitNode(ScanBackslash());
				break;

			case L'^':
				AddUnitType(UseOptionM() ? RegexCode::Bol : RegexCode::Beginning);
				break;

			case L'$':
				AddUnitType(UseOptionM() ? RegexCode::Eol : RegexCode::EndZ);
				break;

			case L'.':
				if (UseOptionS())
				{
					AddUnitSet(RegexCharClass::AnyClass);
				}
				else
				{
					AddUnitNotone(L'\n');
				}
				break;

			case L'{':
			case L'*':
			case L'+':
			case L'?':
				if (Unit() == NULL)
				{
					throw MakeException(wasPrevQuantifier ? L"NestedQuantify" + StringHelper::toString(ch) : L"QuantifyAfterNothing");
				}
				MoveLeft();
				break;

			default:
				throw MakeException(L"InternalError");
			}

			ScanBlank();

			if (CharsRight() == 0 || !(isQuantifier = IsTrueQuantifier()))
			{
				AddConcatenate();
				goto ContinueOuterScan;
			}

			ch = MoveRightGetChar();

			// Handle quantifiers
			while (Unit() != NULL)
			{
				int min;
				int max;
				bool lazy;

				switch (ch)
				{
				case L'*':
					min = 0;
					max = INT_MAX;
					break;

				case L'?':
					min = 0;
					max = 1;
					break;

				case L'+':
					min = 1;
					max = INT_MAX;
					break;

				case L'{':
					{
						startpos = Textpos();
						max = min = ScanDecimal();
						if (startpos < Textpos())
						{
							if (CharsRight() > 0 && RightChar() == L',')
							{
								MoveRight();
								if (CharsRight() == 0 || RightChar() == L'}')
								{
									max = INT_MAX;
								}
								else
								{
									max = ScanDecimal();
								}
							}
						}

						if (startpos == Textpos() || CharsRight() == 0 || MoveRightGetChar() != L'}')
						{
							AddConcatenate();
							Textto(startpos - 1);
							goto ContinueOuterScan;
						}
					}

					break;

				default:
					throw MakeException(L"InternalError");
				}

				ScanBlank();

				if (CharsRight() == 0 || RightChar() != L'?')
				{
					lazy = false;
				}
				else
				{
					MoveRight();
					lazy = true;
				}

				if (min > max)
				{
					throw MakeException(L"IllegalRange");
				}

				AddConcatenate(lazy, min, max);
			}

		ContinueOuterScan:
			;
		}

	BreakOuterScan:
		;

		if (!EmptyStack())
		{
			throw MakeException(L"NotEnoughParens");
		}

		AddGroup();

		return Unit();
	}

	RegexNode* RegexParser::ScanReplacement()
	{
		int c;
		int startpos;

		_concatenation = new RegexNode(&_ptr_mgr, RegexNode::Concatenate, _options);
		for (;;)
		{
			c = CharsRight();
			if (c == 0)
			{
				break;
			}

			startpos = Textpos();

			while (c > 0 && RightChar() != L'$')
			{
				MoveRight();
				c--;
			}

			AddConcatenate(startpos, Textpos() - startpos, true);

			if (c > 0)
			{
				if (MoveRightGetChar() == L'$')
				{
					AddUnitNode(ScanDollar());
				}
				AddConcatenate();
			}
		}

		return _concatenation;
	}

	RegexCharClass* RegexParser::ScanCharClass(bool caseInsensitive)
	{
		return ScanCharClass(caseInsensitive, false);
	}

	RegexCharClass* RegexParser::ScanCharClass(bool caseInsensitive, bool scanOnly)
	{
		wchar_t ch = L'\0';
		wchar_t chPrev = L'\0';
		bool inRange = false;
		bool firstChar = true;
		bool closed = false;

		RegexCharClass* cc = scanOnly ? NULL : new RegexCharClass();

		if (CharsRight() > 0 && RightChar() == L'^')
		{
			MoveRight();
			if (!scanOnly)
			{
				cc->setNegate(true);
			}
		}

		for (; CharsRight() > 0; firstChar = false)
		{
			bool fTranslatedChar = false;
			ch = MoveRightGetChar();
			if (ch == L']')
			{
				if (!firstChar)
				{
					closed = true;
					break;
				}
			}
			else if (ch == L'\\' && CharsRight() > 0)
			{
				switch (ch = MoveRightGetChar())
				{
				case L'D':
				case L'd':
					if (!scanOnly)
					{
						if (inRange)
						{
							throw MakeException(L"BadClassInCharRange " + StringHelper::toString(ch));
						}
						cc->AddDigit(UseOptionE(), ch == L'D', _pattern);
					}

					continue;

				case L'S':
				case L's':
					if (!scanOnly)
					{
						if (inRange)
						{
							throw MakeException(L"BadClassInCharRange " + StringHelper::toString(ch));
						}
						cc->AddSpace(UseOptionE(), ch == L'S');
					}

					continue;

				case L'W':
				case L'w':
					if (!scanOnly)
					{
						if (inRange)
						{
							throw MakeException(L"BadClassInCharRange " + StringHelper::toString(ch));
						}

						cc->AddWord(UseOptionE(), ch == L'W');
					}

					continue;

				case L'p':
				case L'P':
					if (!scanOnly)
					{
						if (inRange)
						{
							throw MakeException(L"BadClassInCharRange " + StringHelper::toString(ch));
						}
						cc->AddCategoryFromName(ParseProperty(), (ch != L'p'), caseInsensitive, _pattern);
					}
					else
					{
						ParseProperty();
					}

					continue;

				case L'-':
					if (!scanOnly)
					{
						cc->AddRange(ch, ch);
					}
					continue;

				default:
					MoveLeft();
					ch = ScanCharEscape(); // non-literal character
					fTranslatedChar = true;
					break; // this break will only break out of the switch
				}
			}
			else if (ch == L'[')
			{
				// This is code for Posix style properties - [:Ll:] or [:IsTibetan:].
				// It currently doesn't do anything other than skip the whole thing!
				if (CharsRight() > 0 && RightChar() == L':' && !inRange)
				{
					int savePos = Textpos();

					MoveRight();
					std::wstring name = ScanCapname();
					if (CharsRight() < 2 || MoveRightGetChar() != L':' || MoveRightGetChar() != L']')
					{
						Textto(savePos);
					}
					// else lookup name (nyi)
				}
			}


			if (inRange)
			{
				inRange = false;
				if (!scanOnly)
				{
					if (ch == L'[' && !fTranslatedChar && !firstChar)
					{
						// We thought we were in a range, but we're actually starting a subtraction. 
						// In that case, we'll add chPrev to our char class, skip the opening [, and
						// scan the new character class recursively. 
						cc->AddChar(chPrev);
						cc->AddSubtraction(ScanCharClass(caseInsensitive, false));

						if (CharsRight() > 0 && RightChar() != L']')
						{
							throw MakeException(L"SubtractionMustBeLast");
						}
					}
					else
					{
						// a regular range, like a-z
						if (chPrev > ch)
						{
							throw MakeException(L"ReversedCharRange");
						}
						cc->AddRange(chPrev, ch);
					}
				}
			}
			else if (CharsRight() >= 2 && RightChar() == L'-' && RightChar(1) != L']')
			{
				// this could be the start of a range
				chPrev = ch;
				inRange = true;
				MoveRight();
			}
			else if (CharsRight() >= 1 && ch == L'-' && !fTranslatedChar && RightChar() == L'[' && !firstChar)
			{
				// we aren't in a range, and now there is a subtraction.  Usually this happens
				// only when a subtraction follows a range, like [a-z-[b]]
				if (!scanOnly)
				{
					MoveRight(1);
					cc->AddSubtraction(ScanCharClass(caseInsensitive, false));

					if (CharsRight() > 0 && RightChar() != L']')
					{
						throw MakeException(L"SubtractionMustBeLast");
					}
				}
				else
				{
					MoveRight(1);
					ScanCharClass(caseInsensitive, true);
				}
			}
			else
			{
				if (!scanOnly)
				{
					cc->AddRange(ch, ch);
				}
			}
		}

		if (!closed)
		{
			throw MakeException(L"UnterminatedBracket");
		}

		if (!scanOnly && caseInsensitive)
		{
			cc->AddLowercase(_culture);
		}

		return cc;
	}

	RegexNode* RegexParser::ScanGroupOpen()
	{
		wchar_t ch = L'\0';
		int NodeType;
		wchar_t close = L'>';


		// just return a RegexNode if we have:
		// 1. "(" followed by nothing
		// 2. "(x" where x != ?
		// 3. "(?)"
		if (CharsRight() == 0 || RightChar() != L'?' || (RightChar() == L'?' && (CharsRight() > 1 && RightChar(1) == L')')))
		{
			if (UseOptionN() || _ignoreNextParen)
			{
				_ignoreNextParen = false;
				return new RegexNode(&_ptr_mgr, RegexNode::Group, _options);
			}
			else
			{
				return new RegexNode(&_ptr_mgr, RegexNode::Capture, _options, _autocap++, -1);
			}
		}

		MoveRight();

		for (;;)
		{
			if (CharsRight() == 0)
			{
				break;
			}

			switch (ch = MoveRightGetChar())
			{
			case L':':
				NodeType = RegexNode::Group;
				break;

			case L'=':
				//_options &= ~(RegexOptions::RightToLeft);
				{
					int temp = (int)_options;
					temp &= ~(int)(RegexOptions::RightToLeft);
					_options = (RegexOptions)temp;
				}
				NodeType = RegexNode::Require;
				break;

			case L'!':
				//_options &= ~(RegexOptions::RightToLeft);
				{
					int temp = (int)_options;
					temp &= ~(int)(RegexOptions::RightToLeft);
					_options = (RegexOptions)temp;
				}
				NodeType = RegexNode::Prevent;
				break;

			case L'>':
				NodeType = RegexNode::Greedy;
				break;

			case L'\'':
				close = L'\'';
				goto CaseLabel1;
				// fallthrough

			case L'<':
			CaseLabel1:
				if (CharsRight() == 0)
				{
					goto BreakRecognize;
				}

				switch (ch = MoveRightGetChar())
				{
				case L'=':
					if (close == L'\'')
					{
						goto BreakRecognize;
					}

					//_options |= RegexOptions::RightToLeft;
					{
						int temp = (int)_options;
						temp |= (int)RegexOptions::RightToLeft;
						_options = (RegexOptions)temp;
					}
					NodeType = RegexNode::Require;
					break;

				case L'!':
					if (close == L'\'')
					{
						goto BreakRecognize;
					}

					//_options |= RegexOptions::RightToLeft;
					{
						int temp = (int)_options;
						temp |= (int)RegexOptions::RightToLeft;
						_options = (RegexOptions)temp;
					}
					NodeType = RegexNode::Prevent;
					break;

				default:
					MoveLeft();
					int capnum = -1;
					int uncapnum = -1;
					bool proceed = false;

					// grab part before -

					if (ch >= L'0' && ch <= L'9')
					{
						capnum = ScanDecimal();

						if (!IsCaptureSlot(capnum))
						{
							capnum = -1;
						}

						// check if we have bogus characters after the number
						if (CharsRight() > 0 && !(RightChar() == close || RightChar() == L'-'))
						{
							throw MakeException(L"InvalidGroupName");
						}
						if (capnum == 0)
						{
							throw MakeException(L"CapnumNotZero");
						}
					}
					else if (RegexCharClass::IsWordChar(ch))
					{
						std::wstring capname = ScanCapname();

						if (IsCaptureName(capname))
						{
							capnum = CaptureSlotFromName(capname);
						}

						// check if we have bogus character after the name
						if (CharsRight() > 0 && !(RightChar() == close || RightChar() == L'-'))
						{
							throw MakeException(L"InvalidGroupName");
						}
					}
					else if (ch == L'-')
					{
						proceed = true;
					}
					else
					{
						// bad group name - starts with something other than a word character and isn't a number
						throw MakeException(L"InvalidGroupName");
					}

					// grab part after - if any

					if ((capnum != -1 || proceed == true) && CharsRight() > 0 && RightChar() == L'-')
					{
						MoveRight();
						ch = RightChar();

						if (ch >= L'0' && ch <= L'9')
						{
							uncapnum = ScanDecimal();

							if (!IsCaptureSlot(uncapnum))
							{
								throw MakeException(L"UndefinedBackref" + std::to_wstring(uncapnum));
							}

							// check if we have bogus characters after the number
							if (CharsRight() > 0 && RightChar() != close)
							{
								throw MakeException(L"InvalidGroupName");
							}
						}
						else if (RegexCharClass::IsWordChar(ch))
						{
							std::wstring uncapname = ScanCapname();

							if (IsCaptureName(uncapname))
							{
								uncapnum = CaptureSlotFromName(uncapname);
							}
							else
							{
								throw MakeException(L"UndefinedNameRef " + uncapname);
							}

							// check if we have bogus character after the name
							if (CharsRight() > 0 && RightChar() != close)
							{
								throw MakeException(L"InvalidGroupName");
							}
						}
						else
						{
							// bad group name - starts with something other than a word character and isn't a number
							throw MakeException(L"InvalidGroupName");
						}
					}

					// actually make the node

					if ((capnum != -1 || uncapnum != -1) && CharsRight() > 0 && MoveRightGetChar() == close)
					{
						return new RegexNode(&_ptr_mgr, RegexNode::Capture, _options, capnum, uncapnum);
					}

					goto BreakRecognize;
				}

				break;

			case L'(':
				{
					// alternation construct (?(...) | )

					int parenPos = Textpos();
					if (CharsRight() > 0)
					{
						ch = RightChar();

						// check if the alternation condition is a backref
						if (ch >= L'0' && ch <= L'9')
						{
							int capnum = ScanDecimal();
							if (CharsRight() > 0 && MoveRightGetChar() == L')')
							{
								if (IsCaptureSlot(capnum))
								{
									return new RegexNode(&_ptr_mgr, RegexNode::Testref, _options, capnum);
								}
								else
								{
									throw MakeException(L"UndefinedReference " + std::to_wstring(capnum));
								}
							}
							else
							{
								throw MakeException(L"MalformedReference " + std::to_wstring(capnum));
							}
						}
						else if (RegexCharClass::IsWordChar(ch))
						{
							std::wstring capname = ScanCapname();

							if (IsCaptureName(capname) && CharsRight() > 0 && MoveRightGetChar() == L')')
							{
								return new RegexNode(&_ptr_mgr, RegexNode::Testref, _options, CaptureSlotFromName(capname));
							}
						}
					}

					// not a backref
					NodeType = RegexNode::Testgroup;
					Textto(parenPos - 1); // jump to the start of the parentheses
					_ignoreNextParen = true; // but make sure we don't try to capture the insides

					int charsRight = CharsRight();
					if (charsRight >= 3 && RightChar(1) == L'?')
					{
						wchar_t rightchar2 = RightChar(2);
						// disallow comments in the condition
						if (rightchar2 == L'#')
						{
							throw MakeException(L"AlternationCantHaveComment");
						}

						// disallow named capture group (?<..>..) in the condition
						if (rightchar2 == L'\'')
						{
							throw MakeException(L"AlternationCantCapture");
						}
						else
						{
							if (charsRight >= 4 && (rightchar2 == L'<' && RightChar(3) != L'!' && RightChar(3) != L'='))
							{
								throw MakeException(L"AlternationCantCapture");
							}
						}
					}

					break;
				}
			default:
				MoveLeft();

				NodeType = RegexNode::Group;
				ScanOptions();
				if (CharsRight() == 0)
				{
					goto BreakRecognize;
				}

				if ((ch = MoveRightGetChar()) == L')')
				{
					return NULL;
				}

				if (ch != L':')
				{
					goto BreakRecognize;
				}
				break;
			}

			return new RegexNode(&_ptr_mgr, NodeType, _options);
		}

	BreakRecognize:
		;
		// break Recognize comes here

		throw MakeException(L"UnrecognizedGrouping");
	}

	void RegexParser::ScanBlank()
	{
		if (UseOptionX())
		{
			for (;;)
			{
				while (CharsRight() > 0 && IsSpace(RightChar()))
				{
					MoveRight();
				}

				if (CharsRight() == 0)
				{
					break;
				}

				if (RightChar() == L'#')
				{
					while (CharsRight() > 0 && RightChar() != L'\n')
					{
						MoveRight();
					}
				}
				else if (CharsRight() >= 3 && RightChar(2) == L'#' && RightChar(1) == L'?' && RightChar() == L'(')
				{
					while (CharsRight() > 0 && RightChar() != L')')
					{
						MoveRight();
					}
					if (CharsRight() == 0)
					{
						throw MakeException(L"UnterminatedComment");
					}
					MoveRight();
				}
				else
				{
					break;
				}
			}
		}
		else
		{
			for (;;)
			{
				if (CharsRight() < 3 || RightChar(2) != L'#' || RightChar(1) != L'?' || RightChar() != L'(')
				{
					return;
				}

				while (CharsRight() > 0 && RightChar() != L')')
				{
					MoveRight();
				}
				if (CharsRight() == 0)
				{
					throw MakeException(L"UnterminatedComment");
				}
				MoveRight();
			}
		}
	}

	RegexNode* RegexParser::ScanBackslash()
	{
		wchar_t ch;

		if (CharsRight() == 0)
		{
			throw MakeException(L"IllegalEndEscape");
		}

		switch (ch = RightChar())
		{
		case L'b':
		case L'B':
		case L'A':
		case L'G':
		case L'Z':
		case L'z':
			MoveRight();
			return new RegexNode(&_ptr_mgr, TypeFromCode(ch), _options);

		case L'w':
			MoveRight();
			if (UseOptionE())
			{
				return new RegexNode(&_ptr_mgr, RegexCode::Set, _options, RegexCharClass::ECMAWordClass);
			}
			return new RegexNode(&_ptr_mgr, RegexCode::Set, _options, RegexCharClass::WordClass);

		case L'W':
			MoveRight();
			if (UseOptionE())
			{
				return new RegexNode(&_ptr_mgr, RegexCode::Set, _options, RegexCharClass::NotECMAWordClass);
			}
			return new RegexNode(&_ptr_mgr, RegexCode::Set, _options, RegexCharClass::NotWordClass);

		case L's':
			MoveRight();
			if (UseOptionE())
			{
				return new RegexNode(&_ptr_mgr, RegexCode::Set, _options, RegexCharClass::ECMASpaceClass);
			}
			return new RegexNode(&_ptr_mgr, RegexCode::Set, _options, RegexCharClass::SpaceClass);

		case L'S':
			MoveRight();
			if (UseOptionE())
			{
				return new RegexNode(&_ptr_mgr, RegexCode::Set, _options, RegexCharClass::NotECMASpaceClass);
			}
			return new RegexNode(&_ptr_mgr, RegexCode::Set, _options, RegexCharClass::NotSpaceClass);

		case L'd':
			MoveRight();
			if (UseOptionE())
			{
				return new RegexNode(&_ptr_mgr, RegexCode::Set, _options, RegexCharClass::ECMADigitClass);
			}
			return new RegexNode(&_ptr_mgr, RegexCode::Set, _options, RegexCharClass::DigitClass);

		case L'D':
			MoveRight();
			if (UseOptionE())
			{
				return new RegexNode(&_ptr_mgr, RegexCode::Set, _options, RegexCharClass::NotECMADigitClass);
			}
			return new RegexNode(&_ptr_mgr, RegexCode::Set, _options, RegexCharClass::NotDigitClass);

		case L'p':
		case L'P':
			{
				MoveRight();
				RegexCharClass cc = RegexCharClass();
				cc.AddCategoryFromName(ParseProperty(), (ch != L'p'), UseOptionI(), _pattern);
				if (UseOptionI())
				{
					cc.AddLowercase(_culture);
				}

				return new RegexNode(&_ptr_mgr, RegexCode::Set, _options, cc.ToStringClass());
			}

		default:
			return ScanBasicBackslash();
		}
	}

	RegexNode* RegexParser::ScanBasicBackslash()
	{
		if (CharsRight() == 0)
		{
			throw MakeException(L"IllegalEndEscape");
		}

		wchar_t ch;
		bool angled = false;
		wchar_t close = L'\0';
		int backpos;

		backpos = Textpos();
		ch = RightChar();

		// allow \k<foo> instead of \<foo>, which is now deprecated

		if (ch == L'k')
		{
			if (CharsRight() >= 2)
			{
				MoveRight();
				ch = MoveRightGetChar();

				if (ch == L'<' || ch == L'\'')
				{
					angled = true;
					close = (ch == L'\'') ? L'\'' : L'>';
				}
			}

			if (!angled || CharsRight() <= 0)
			{
				throw MakeException(L"MalformedNameRef");
			}

			ch = RightChar();
		}

			// Note angle without \g <

		else if ((ch == L'<' || ch == L'\'') && CharsRight() > 1)
		{
			angled = true;
			close = (ch == L'\'') ? L'\'' : L'>';

			MoveRight();
			ch = RightChar();
		}

		// Try to parse backreference: \<1> or \<cap>

		if (angled && ch >= L'0' && ch <= L'9')
		{
			int capnum = ScanDecimal();

			if (CharsRight() > 0 && MoveRightGetChar() == close)
			{
				if (IsCaptureSlot(capnum))
				{
					return new RegexNode(&_ptr_mgr, RegexCode::Ref, _options, capnum);
				}
				else
				{
					throw MakeException(L"UndefinedBackref " + std::to_wstring(capnum));
				}
			}
		}

			// Try to parse backreference or octal: \1

		else if (!angled && ch >= L'1' && ch <= L'9')
		{
			if (UseOptionE())
			{
				int capnum = -1;
				int newcapnum = static_cast<int>(ch - L'0');
				int pos = Textpos() - 1;
				while (newcapnum <= _captop)
				{
					if (IsCaptureSlot(newcapnum) && (_caps.empty() || static_cast<int>(_caps[newcapnum]) < pos))
					{
						capnum = newcapnum;
					}
					MoveRight();
					if (CharsRight() == 0 || (ch = RightChar()) < L'0' || ch > L'9')
					{
						break;
					}
					newcapnum = newcapnum * 10 + static_cast<int>(ch - L'0');
				}

				if (capnum >= 0)
				{
					return new RegexNode(&_ptr_mgr, RegexCode::Ref, _options, capnum);
				}
			}
			else
			{
				int capnum = ScanDecimal();
				if (IsCaptureSlot(capnum))
				{
					return new RegexNode(&_ptr_mgr, RegexCode::Ref, _options, capnum);
				}
				else if (capnum <= 9)
				{
					throw MakeException(L"UndefinedBackref " + std::to_wstring(capnum));
				}
			}
		}

		else if (angled && RegexCharClass::IsWordChar(ch))
		{
			std::wstring capname = ScanCapname();

			if (CharsRight() > 0 && MoveRightGetChar() == close)
			{
				if (IsCaptureName(capname))
				{
					return new RegexNode(&_ptr_mgr, RegexCode::Ref, _options, CaptureSlotFromName(capname));
				}
				else
				{
					throw MakeException(L"UndefinedNameRef " + capname);
				}
			}
		}

		// Not backreference: must be char code

		Textto(backpos);
		ch = ScanCharEscape();

		if (UseOptionI())
		{
			ch = UtilsCharInternal::ToLower(ch, _culture);
		}

		return new RegexNode(&_ptr_mgr, RegexCode::One, _options, ch);
	}

	RegexNode* RegexParser::ScanDollar()
	{
		if (CharsRight() == 0)
		{
			return new RegexNode(&_ptr_mgr, RegexCode::One, _options, L'$');
		}

		wchar_t ch = RightChar();
		bool angled;
		int backpos = Textpos();
		int lastEndPos = backpos;

		// Note angle

		if (ch == L'{' && CharsRight() > 1)
		{
			angled = true;
			MoveRight();
			ch = RightChar();
		}
		else
		{
			angled = false;
		}

		// Try to parse backreference: \1 or \{1} or \{cap}

		if (ch >= L'0' && ch <= L'9')
		{
			if (!angled && UseOptionE())
			{
				int capnum = -1;
				int newcapnum = static_cast<int>(ch - L'0');
				MoveRight();
				if (IsCaptureSlot(newcapnum))
				{
					capnum = newcapnum;
					lastEndPos = Textpos();
				}

				while (CharsRight() > 0 && (ch = RightChar()) >= L'0' && ch <= L'9')
				{
					int digit = static_cast<int>(ch - L'0');
					if (newcapnum > (MaxValueDiv10) || (newcapnum == (MaxValueDiv10) && digit > (MaxValueMod10)))
					{
						throw MakeException(L"CaptureGroupOutOfRange");
					}

					newcapnum = newcapnum * 10 + digit;

					MoveRight();
					if (IsCaptureSlot(newcapnum))
					{
						capnum = newcapnum;
						lastEndPos = Textpos();
					}
				}

				Textto(lastEndPos);
				if (capnum >= 0)
				{
					return new RegexNode(&_ptr_mgr, RegexCode::Ref, _options, capnum);
				}
			}
			else
			{
				int capnum = ScanDecimal();
				if (!angled || CharsRight() > 0 && MoveRightGetChar() == L'}')
				{
					if (IsCaptureSlot(capnum))
					{
						return new RegexNode(&_ptr_mgr, RegexCode::Ref, _options, capnum);
					}
				}
			}
		}
		else if (angled && RegexCharClass::IsWordChar(ch))
		{
			std::wstring capname = ScanCapname();

			if (CharsRight() > 0 && MoveRightGetChar() == L'}')
			{
				if (IsCaptureName(capname))
				{
					return new RegexNode(&_ptr_mgr, RegexCode::Ref, _options, CaptureSlotFromName(capname));
				}
			}
		}
		else if (!angled)
		{
			int capnum = 1;

			switch (ch)
			{
			case L'$':
				MoveRight();
				return new RegexNode(&_ptr_mgr, RegexCode::One, _options, L'$');

			case L'&':
				capnum = 0;
				break;

			case L'`':
				capnum = RegexReplacement::LeftPortion;
				break;

			case L'\'':
				capnum = RegexReplacement::RightPortion;
				break;

			case L'+':
				capnum = RegexReplacement::LastGroup;
				break;

			case L'_':
				capnum = RegexReplacement::WholeString;
				break;
			default: ;
			}

			if (capnum != 1)
			{
				MoveRight();
				return new RegexNode(&_ptr_mgr, RegexCode::Ref, _options, capnum);
			}
		}

		// unrecognized $: literalize

		Textto(backpos);
		return new RegexNode(&_ptr_mgr, RegexCode::One, _options, L'$');
	}

	std::wstring RegexParser::ScanCapname()
	{
		int startpos = Textpos();

		while (CharsRight() > 0)
		{
			if (!RegexCharClass::IsWordChar(MoveRightGetChar()))
			{
				MoveLeft();
				break;
			}
		}

		return _pattern.substr(startpos, Textpos() - startpos);
	}

	wchar_t RegexParser::ScanOctal()
	{
		int d;
		int i;
		int c;

		// Consume octal chars only up to 3 digits and value 0377

		c = 3;

		if (c > CharsRight())
		{
			c = CharsRight();
		}

		for (i = 0; c > 0 && static_cast<unsigned int>(d = RightChar() - L'0') <= 7; c -= 1)
		{
			MoveRight();
			i *= 8;
			i += d;
			if (UseOptionE() && i >= 0x20)
			{
				break;
			}
		}

		// Octal codes only go up to 255.  Any larger and the behavior that Perl follows
		// is simply to truncate the high bits. 
		i &= 0xFF;

		return static_cast<wchar_t>(i);
	}

	int RegexParser::ScanDecimal()
	{
		int i = 0;
		int d;

		while (CharsRight() > 0 && static_cast<unsigned int>(d = static_cast<wchar_t>(RightChar() - L'0')) <= 9)
		{
			MoveRight();

			if (i > (MaxValueDiv10) || (i == (MaxValueDiv10) && d > (MaxValueMod10)))
			{
				throw MakeException(L"CaptureGroupOutOfRange");
			}

			i *= 10;
			i += d;
		}

		return i;
	}

	wchar_t RegexParser::ScanHex(int c)
	{
		int i;
		int d;

		i = 0;

		if (CharsRight() >= c)
		{
			for (; c > 0 && ((d = HexDigit(MoveRightGetChar())) >= 0); c -= 1)
			{
				i *= 0x10;
				i += d;
			}
		}

		if (c > 0)
		{
			throw MakeException(L"TooFewHex");
		}

		return static_cast<wchar_t>(i);
	}

	int RegexParser::HexDigit(wchar_t ch)
	{
		int d;

		if (static_cast<unsigned int>(d = ch - L'0') <= 9)
		{
			return d;
		}

		if (static_cast<unsigned int>(d = ch - L'a') <= 5)
		{
			return d + 0xa;
		}

		if (static_cast<unsigned int>(d = ch - L'A') <= 5)
		{
			return d + 0xa;
		}

		return -1;
	}

	wchar_t RegexParser::ScanControl()
	{
		wchar_t ch;

		if (CharsRight() <= 0)
		{
			throw MakeException(L"MissingControl");
		}

		ch = MoveRightGetChar();

		// \ca interpreted as \cA

		if (ch >= L'a' && ch <= L'z')
		{
			ch = static_cast<wchar_t>(ch - (L'a' - L'A'));
		}

		if ((ch = static_cast<wchar_t>(ch - L'@')) < L' ')
		{
			return ch;
		}

		throw MakeException(L"UnrecognizedControl");
	}

	bool RegexParser::IsOnlyTopOption(RegexOptions option)
	{
		return (option == RegexOptions::RightToLeft || option == RegexOptions::CultureInvariant || option == RegexOptions::ECMAScript);
	}

	void RegexParser::ScanOptions()
	{
		wchar_t ch;
		bool off;
		RegexOptions option;

		for (off = false; CharsRight() > 0; MoveRight())
		{
			ch = RightChar();

			if (ch == L'-')
			{
				off = true;
			}
			else if (ch == L'+')
			{
				off = false;
			}
			else
			{
				option = OptionFromCode(ch);
				if (option == (Regexs::RegexOptions)0 || IsOnlyTopOption(option))
				{
					return;
				}

				if (off)
				{
					//_options &= ~option;
					{
						int temp = (int)_options;
						temp &= ~(int)option;
						_options = (RegexOptions)temp;
					}
				}
				else
				{
					//_options |= option;
					{
						int temp = (int)_options;
						temp |= (int)option;
						_options = (RegexOptions)temp;
					}
				}
			}
		}
	}

	wchar_t RegexParser::ScanCharEscape()
	{
		wchar_t ch;

		ch = MoveRightGetChar();

		if (ch >= L'0' && ch <= L'7')
		{
			MoveLeft();
			return ScanOctal();
		}

		switch (ch)
		{
		case L'x':
			return ScanHex(2);
		case L'u':
			return ScanHex(4);
		case L'a':
			return L'\u0007';
		case L'b':
			return L'\b';
		case L'e':
			return L'\u001B';
		case L'f':
			return L'\f';
		case L'n':
			return L'\n';
		case L'r':
			return L'\r';
		case L't':
			return L'\t';
		case L'v':
			return L'\u000B';
		case L'c':
			return ScanControl();
		default:
			if (!UseOptionE() && RegexCharClass::IsWordChar(ch))
			{
				throw MakeException(L"UnrecognizedEscape " + StringHelper::toString(ch));
			}
			return ch;
		}
	}

	std::wstring RegexParser::ParseProperty()
	{
		if (CharsRight() < 3)
		{
			throw MakeException(L"IncompleteSlashP");
		}

		wchar_t ch = MoveRightGetChar();
		if (ch != L'{')
		{
			throw MakeException(L"MalformedSlashP");
		}

		int startpos = Textpos();
		while (CharsRight() > 0)
		{
			ch = MoveRightGetChar();
			if (!(RegexCharClass::IsWordChar(ch) || ch == L'-'))
			{
				MoveLeft();
				break;
			}
		}

		std::wstring capname = _pattern.substr(startpos, Textpos() - startpos);

		if (CharsRight() == 0 || MoveRightGetChar() != L'}')
		{
			throw MakeException(L"IncompleteSlashP");
		}

		return capname;
	}

	int RegexParser::TypeFromCode(wchar_t ch)
	{
		switch (ch)
		{
		case L'b':
			return UseOptionE() ? RegexCode::ECMABoundary : RegexCode::Boundary;
		case L'B':
			return UseOptionE() ? RegexCode::NonECMABoundary : RegexCode::Nonboundary;
		case L'A':
			return RegexCode::Beginning;
		case L'G':
			return RegexCode::Start;
		case L'Z':
			return RegexCode::EndZ;
		case L'z':
			return RegexCode::End;
		default:
			return RegexNode::Nothing;
		}
	}

	RegexOptions RegexParser::OptionFromCode(wchar_t ch)
	{
		// case-insensitive
		if (ch >= L'A' && ch <= L'Z')
		{
			ch += static_cast<wchar_t>(L'a' - L'A');
		}

		switch (ch)
		{
		case L'i':
			return RegexOptions::IgnoreCase;
		case L'r':
			return RegexOptions::RightToLeft;
		case L'm':
			return RegexOptions::Multiline;
		case L'n':
			return RegexOptions::ExplicitCapture;
		case L's':
			return RegexOptions::Singleline;
		case L'x':
			return RegexOptions::IgnorePatternWhitespace;
		case L'e':
			return RegexOptions::ECMAScript;
		default:
			return (RegexOptions)0;
		}
	}

	void RegexParser::CountCaptures()
	{
		wchar_t ch;

		NoteCaptureSlot(0, 0);

		_autocap = 1;

		while (CharsRight() > 0)
		{
			int pos = Textpos();
			ch = MoveRightGetChar();
			switch (ch)
			{
			case L'\\':
				if (CharsRight() > 0)
				{
					MoveRight();
				}
				break;

			case L'#':
				if (UseOptionX())
				{
					MoveLeft();
					ScanBlank();
				}

				break;

			case L'[':
				ScanCharClass(false, true);
				break;

			case L')':
				if (!EmptyOptionsStack())
				{
					PopOptions();
				}
				break;

			case L'(':
				if (CharsRight() >= 2 && RightChar(1) == L'#' && RightChar() == L'?')
				{
					MoveLeft();
					ScanBlank();
				}
				else
				{
					PushOptions();
					if (CharsRight() > 0 && RightChar() == L'?')
					{
						// we have (?...
						MoveRight();

						if (CharsRight() > 1 && (RightChar() == L'<' || RightChar() == L'\''))
						{
							// named group: (?<... or (?'...

							MoveRight();
							ch = RightChar();

							if (ch != L'0' && RegexCharClass::IsWordChar(ch))
							{
								//if (_ignoreNextParen) 
								//    throw MakeException(SR.GetString(SR.AlternationCantCapture));
								if (ch >= L'1' && ch <= L'9')
								{
									NoteCaptureSlot(ScanDecimal(), pos);
								}
								else
								{
									NoteCaptureName(ScanCapname(), pos);
								}
							}
						}
						else
						{
							// (?...

							// get the options if it's an option construct (?cimsx-cimsx...)
							ScanOptions();

							if (CharsRight() > 0)
							{
								if (RightChar() == L')')
								{
									// (?cimsx-cimsx)
									MoveRight();
									PopKeepOptions();
								}
								else if (RightChar() == L'(')
								{
									// alternation construct: (?(foo)yes|no)
									// ignore the next paren so we don't capture the condition
									_ignoreNextParen = true;

									// break from here so we don't reset _ignoreNextParen
									break;
								}
							}
						}
					}
					else
					{
						if (!UseOptionN() && !_ignoreNextParen)
						{
							NoteCaptureSlot(_autocap++, pos);
						}
					}
				}

				_ignoreNextParen = false;
				break;
			default: ;
			}
		}

		AssignNameSlots();
	}

	void RegexParser::NoteCaptureSlot(int i, int pos)
	{
		if (_caps.find(i) == _caps.end())
		{
			// the rhs of the hashtable isn't used in the parser

			_caps.emplace(i, pos);
			_capcount++;

			if (_captop <= i)
			{
				if (i == INT_MAX)
				{
					_captop = i;
				}
				else
				{
					_captop = i + 1;
				}
			}
		}
	}

	void RegexParser::NoteCaptureName(const std::wstring& name, int pos)
	{
		if (_capnames.empty())
		{
			_capnames = std::unordered_map<std::wstring, int>();
			_capnamelist = std::vector<std::wstring>();
		}

		if (_capnames.find(name) == _capnames.end())
		{
			_capnames.emplace(name, pos);
			_capnamelist.push_back(name);
		}
	}

	void RegexParser::NoteCaptures(std::unordered_map<int, int>& caps, int capsize, std::unordered_map<std::wstring, int>& capnames)
	{
		_caps = caps;
		_capsize = capsize;
		_capnames = capnames;
	}

	void RegexParser::AssignNameSlots()
	{
		if (!_capnames.empty())
		{
			for (size_t i = 0; i < _capnamelist.size(); i++)
			{
				while (IsCaptureSlot(_autocap))
				{
					_autocap++;
				}
				std::wstring name = _capnamelist[i];
				int pos = static_cast<int>(_capnames[name]);
				_capnames[name] = _autocap;
				NoteCaptureSlot(_autocap, pos);

				_autocap++;
			}
		}

		// if the caps array has at least one gap, construct the list of used slots

		if (_capcount < _captop)
		{
			_capnumlist = std::vector<int>(_capcount);
			int i = 0;

			//for (IDictionaryEnumerator* de = _caps.begin(); de->MoveNext();)
			//{
			//	_capnumlist[i++] = std::any_cast<int>(de->Key);
			//}
			//System::Array::Sort(_capnumlist, Comparer<int>::Default);

			for (auto& it : _caps)
			{
				_capnumlist[i++] = it.first;
			}

			Int_Sort range_sort;
			UtilsCollections::Sort<int>(&_capnumlist, &range_sort);
		}

		// merge capsnumlist into capnamelist

		if (!_capnames.empty() || !_capnumlist.empty())
		{
			std::vector<std::wstring> oldcapnamelist;
			int next;
			int k = 0;

			if (_capnames.empty())
			{
				oldcapnamelist = std::vector<std::wstring>();
				_capnames = std::unordered_map<std::wstring, int>();
				_capnamelist = std::vector<std::wstring>();
				next = -1;
			}
			else
			{
				oldcapnamelist = _capnamelist;
				_capnamelist = std::vector<std::wstring>();
				next = static_cast<int>(_capnames[oldcapnamelist[0]]);
			}

			for (int i = 0; i < _capcount; i++)
			{
				int j = (_capnumlist.empty()) ? i : static_cast<int>(_capnumlist[i]);

				if (next == j)
				{
					_capnamelist.push_back(oldcapnamelist[k++]);
					next = (k == oldcapnamelist.size()) ? -1 : static_cast<int>(_capnames[oldcapnamelist[k]]);
				}
				else
				{
					std::wstring str = UtilsCultureInfo::IntToString(_culture, j);
					_capnamelist.push_back(str);
					_capnames[str] = j;
				}
			}
		}
	}

	int RegexParser::CaptureSlotFromName(const std::wstring& capname)
	{
		return static_cast<int>(_capnames[capname]);
	}

	bool RegexParser::IsCaptureSlot(int i)
	{
		if (!_caps.empty())
		{
			return _caps.find(i) != _caps.end();
		}

		return (i >= 0 && i < _capsize);
	}

	bool RegexParser::IsCaptureName(const std::wstring& capname)
	{
		if (_capnames.empty())
		{
			return false;
		}

		return _capnames.find(capname) != _capnames.end();
	}

	bool RegexParser::UseOptionN()
	{
		return ((int)_options & (int)RegexOptions::ExplicitCapture) != 0;
	}

	bool RegexParser::UseOptionI()
	{
		return ((int)_options & (int)RegexOptions::IgnoreCase) != 0;
	}

	bool RegexParser::UseOptionM()
	{
		return ((int)_options & (int)RegexOptions::Multiline) != 0;
	}

	bool RegexParser::UseOptionS()
	{
		return ((int)_options & (int)RegexOptions::Singleline) != 0;
	}

	bool RegexParser::UseOptionX()
	{
		return ((int)_options & (int)RegexOptions::IgnorePatternWhitespace) != 0;
	}

	bool RegexParser::UseOptionE()
	{
		return ((int)_options & (int)RegexOptions::ECMAScript) != 0;
	}

	const std::vector<unsigned char> RegexParser::_category = {0, 0, 0, 0, 0, 0, 0, 0, 0, X, X, 0, X, X, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, X, 0, 0, Z, S, 0, 0, 0, S, S, Q, Q, 0, 0, S, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, Q, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, S, S, 0, S, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, Q, S, 0, 0, 0};

	bool RegexParser::IsSpecial(wchar_t ch)
	{
		return (ch <= L'|' && _category[ch] >= S);
	}

	bool RegexParser::IsStopperX(wchar_t ch)
	{
		return (ch <= L'|' && _category[ch] >= X);
	}

	bool RegexParser::IsQuantifier(wchar_t ch)
	{
		return (ch <= L'{' && _category[ch] >= Q);
	}

	bool RegexParser::IsTrueQuantifier()
	{
		int nChars = CharsRight();
		if (nChars == 0)
		{
			return false;
		}
		int startpos = Textpos();
		wchar_t ch = CharAt(startpos);
		if (ch != L'{')
		{
			return ch <= L'{' && _category[ch] >= Q;
		}
		int pos = startpos;
		while (--nChars > 0 && (ch = CharAt(++pos)) >= L'0' && ch <= L'9')
		{
			;
		}
		if (nChars == 0 || pos - startpos == 1)
		{
			return false;
		}
		if (ch == L'}')
		{
			return true;
		}
		if (ch != L',')
		{
			return false;
		}
		while (--nChars > 0 && (ch = CharAt(++pos)) >= L'0' && ch <= L'9')
		{
			;
		}
		return nChars > 0 && ch == L'}';
	}

	bool RegexParser::IsSpace(wchar_t ch)
	{
		return (ch <= L' ' && _category[ch] == X);
	}

	bool RegexParser::IsMetachar(wchar_t ch)
	{
		return (ch <= L'|' && _category[ch] >= E);
	}

	void RegexParser::AddConcatenate(int pos, int cch, bool isReplacement)
	{
		RegexNode* node;

		if (cch == 0)
		{
			return;
		}

		if (cch > 1)
		{
			std::wstring str = _pattern.substr(pos, cch);

			if (UseOptionI() && !isReplacement)
			{
				// We do the ToLower character by character for consistency.  With surrogate chars, doing
				// a ToLower on the entire string could actually change the surrogate pair.  This is more correct
				// linguistically, but since Regex doesn't support surrogates, it's more important to be 
				// consistent.
				std::wstring sb = str;
				for (size_t i = 0; i < str.length(); i++)
					sb[i] = UtilsCharInternal::ToLower(str[i], _culture);
				str = sb;
			}

			node = new RegexNode(&_ptr_mgr, RegexCode::Multi, _options, str);
		}
		else
		{
			wchar_t ch = _pattern[pos];

			if (UseOptionI() && !isReplacement)
			{
				ch = UtilsCharInternal::ToLower(ch, _culture);
			}

			node = new RegexNode(&_ptr_mgr, RegexCode::One, _options, ch);
		}

		_concatenation->AddChild(&_ptr_mgr, node);
	}

	void RegexParser::PushGroup()
	{
		_group->_next = _stack;
		_alternation->_next = _group;
		_concatenation->_next = _alternation;
		_stack = _concatenation;
	}

	void RegexParser::PopGroup()
	{
		_concatenation = _stack;
		_alternation = _concatenation->_next;
		_group = _alternation->_next;
		_stack = _group->_next;

		// The first () inside a Testgroup group goes directly to the group
		if (_group->Type() == RegexNode::Testgroup && _group->ChildCount() == 0)
		{
			if (_unit == NULL)
			{
				throw MakeException(L"IllegalCondition");
			}

			_group->AddChild(&_ptr_mgr, _unit);
			_unit = NULL;
		}
	}

	bool RegexParser::EmptyStack()
	{
		return _stack == NULL;
	}

	void RegexParser::StartGroup(RegexNode* openGroup)
	{
		_group = openGroup;
		_alternation = new RegexNode(&_ptr_mgr, RegexNode::Alternate, _options);
		_concatenation = new RegexNode(&_ptr_mgr, RegexNode::Concatenate, _options);
	}

	void RegexParser::AddAlternate()
	{
		// The | parts inside a Testgroup group go directly to the group

		if (_group->Type() == RegexNode::Testgroup || _group->Type() == RegexNode::Testref)
		{
			_group->AddChild(&_ptr_mgr, _concatenation->ReverseLeft());
		}
		else
		{
			_alternation->AddChild(&_ptr_mgr, _concatenation->ReverseLeft());
		}

		_concatenation = new RegexNode(&_ptr_mgr, RegexNode::Concatenate, _options);
	}

	void RegexParser::AddConcatenate()
	{
		// The first (| inside a Testgroup group goes directly to the group

		_concatenation->AddChild(&_ptr_mgr, _unit);
		_unit = NULL;
	}

	void RegexParser::AddConcatenate(bool lazy, int min, int max)
	{
		_concatenation->AddChild(&_ptr_mgr, _unit->MakeQuantifier(&_ptr_mgr, lazy, min, max));
		_unit = NULL;
	}

	RegexNode* RegexParser::Unit()
	{
		return _unit;
	}

	void RegexParser::AddUnitOne(wchar_t ch)
	{
		if (UseOptionI())
		{
			ch = UtilsCharInternal::ToLower(ch, _culture);
		}

		_unit = new RegexNode(&_ptr_mgr, RegexCode::One, _options, ch);
	}

	void RegexParser::AddUnitNotone(wchar_t ch)
	{
		if (UseOptionI())
		{
			ch = UtilsCharInternal::ToLower(ch, _culture);
		}

		_unit = new RegexNode(&_ptr_mgr, RegexCode::Notone, _options, ch);
	}

	void RegexParser::AddUnitSet(const std::wstring& cc)
	{
		_unit = new RegexNode(&_ptr_mgr, RegexCode::Set, _options, cc);
	}

	void RegexParser::AddUnitNode(RegexNode* node)
	{
		_unit = node;
	}

	void RegexParser::AddUnitType(int type)
	{
		_unit = new RegexNode(&_ptr_mgr, type, _options);
	}

	void RegexParser::AddGroup()
	{
		if (_group->Type() == RegexNode::Testgroup || _group->Type() == RegexNode::Testref)
		{
			_group->AddChild(&_ptr_mgr, _concatenation->ReverseLeft());

			if (_group->Type() == RegexNode::Testref && _group->ChildCount() > 2 || _group->ChildCount() > 3)
			{
				throw MakeException(L"TooManyAlternates");
			}
		}
		else
		{
			_alternation->AddChild(&_ptr_mgr, _concatenation->ReverseLeft());
			_group->AddChild(&_ptr_mgr, _alternation);
		}

		_unit = _group;
	}

	void RegexParser::PushOptions()
	{
		_optionsStack.push_back(_options);
	}

	void RegexParser::PopOptions()
	{
		_options = _optionsStack[_optionsStack.size() - 1];
		_optionsStack.pop_back();
	}

	bool RegexParser::EmptyOptionsStack()
	{
		return (_optionsStack.empty());
	}

	void RegexParser::PopKeepOptions()
	{
		_optionsStack.pop_back();
	}

	std::invalid_argument RegexParser::MakeException(const std::wstring& message)
	{
		std::string temp = std::string(message.begin(), message.end());
		return std::invalid_argument(temp);
	}

	int RegexParser::Textpos()
	{
		return _currentPos;
	}

	void RegexParser::Textto(int pos)
	{
		_currentPos = pos;
	}

	wchar_t RegexParser::MoveRightGetChar()
	{
		return _pattern[_currentPos++];
	}

	void RegexParser::MoveRight()
	{
		MoveRight(1);
	}

	void RegexParser::MoveRight(int i)
	{
		_currentPos += i;
	}

	void RegexParser::MoveLeft()
	{
		--_currentPos;
	}

	wchar_t RegexParser::CharAt(int i)
	{
		return _pattern[i];
	}

	wchar_t RegexParser::RightChar()
	{
		return _pattern[_currentPos];
	}

	wchar_t RegexParser::RightChar(int i)
	{
		return _pattern[_currentPos + i];
	}

	int RegexParser::CharsRight()
	{
		return _pattern.length() - _currentPos;
	}
}
