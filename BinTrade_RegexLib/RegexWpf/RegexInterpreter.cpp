#include "RegexInterpreter.h"

#include "Exception.h"
#include "Regex.h"
#include "RegexCharClass.h"
#include "RegexFCD.h"
#include "RegexOptions.h" 

namespace Regexs
{
	RegexInterpreter::~RegexInterpreter()
	{
		//delete runcode;
		//delete runfcPrefix;
		//delete runbmPrefix;
		//delete runculture;
	}

	RegexInterpreter::RegexInterpreter(RegexCode* code, ComCultureInfo* culture): runoperator(0), runcodepos(0), runrtl(false), runci(false)
	{
		runcode = code;
		runcodes = code->_codes;
		runstrings = code->_strings;
		runfcPrefix = code->_fcPrefix;
		runbmPrefix = code->_bmPrefix;
		runanchors = code->_anchors;
		runculture = culture;
	}

	void RegexInterpreter::InitTrackCount()
	{
		runtrackcount = runcode->_trackcount;
	}

	void RegexInterpreter::Advance()
	{
		Advance(0);
	}

	void RegexInterpreter::Advance(int i)
	{
		runcodepos += (i + 1);
		SetOperator(runcodes[runcodepos]);
	}

	void RegexInterpreter::Goto(int newpos)
	{
		// when branching backward, ensure storage
		if (newpos < runcodepos)
		{
			EnsureStorage();
		}

		SetOperator(runcodes[newpos]);
		runcodepos = newpos;
	}

	void RegexInterpreter::Textto(int newpos)
	{
		runtextpos = newpos;
	}

	void RegexInterpreter::Trackto(int newpos)
	{
		runtrackpos = runtrack.size() - newpos;
	}

	int RegexInterpreter::Textstart()
	{
		return runtextstart;
	}

	int RegexInterpreter::Textpos()
	{
		return runtextpos;
	}

	int RegexInterpreter::Trackpos()
	{
		return runtrack.size() - runtrackpos;
	}

	void RegexInterpreter::TrackPush()
	{
		runtrack[--runtrackpos] = runcodepos;
	}

	void RegexInterpreter::TrackPush(int I1)
	{
		runtrack[--runtrackpos] = I1;
		runtrack[--runtrackpos] = runcodepos;
	}

	void RegexInterpreter::TrackPush(int I1, int I2)
	{
		runtrack[--runtrackpos] = I1;
		runtrack[--runtrackpos] = I2;
		runtrack[--runtrackpos] = runcodepos;
	}

	void RegexInterpreter::TrackPush(int I1, int I2, int I3)
	{
		runtrack[--runtrackpos] = I1;
		runtrack[--runtrackpos] = I2;
		runtrack[--runtrackpos] = I3;
		runtrack[--runtrackpos] = runcodepos;
	}

	void RegexInterpreter::TrackPush2(int I1)
	{
		runtrack[--runtrackpos] = I1;
		runtrack[--runtrackpos] = -runcodepos;
	}

	void RegexInterpreter::TrackPush2(int I1, int I2)
	{
		runtrack[--runtrackpos] = I1;
		runtrack[--runtrackpos] = I2;
		runtrack[--runtrackpos] = -runcodepos;
	}

	void RegexInterpreter::Backtrack()
	{
		int newpos = runtrack[runtrackpos++];

		if (newpos < 0)
		{
			newpos = -newpos;
			SetOperator(runcodes[newpos] | RegexCode::Back2);
		}
		else
		{
			SetOperator(runcodes[newpos] | RegexCode::Back);
		}

		// When branching backward, ensure storage
		if (newpos < runcodepos)
		{
			EnsureStorage();
		}

		runcodepos = newpos;
	}

	void RegexInterpreter::SetOperator(int op)
	{
		runci = (0 != (op & RegexCode::Ci));
		runrtl = (0 != (op & RegexCode::Rtl));
		runoperator = op & ~(RegexCode::Rtl | RegexCode::Ci);
	}

	void RegexInterpreter::TrackPop()
	{
		runtrackpos++;
	}

	void RegexInterpreter::TrackPop(int framesize)
	{
		runtrackpos += framesize;
	}

	int RegexInterpreter::TrackPeek()
	{
		return runtrack[runtrackpos - 1];
	}

	int RegexInterpreter::TrackPeek(int i)
	{
		return runtrack[runtrackpos - i - 1];
	}

	void RegexInterpreter::StackPush(int I1)
	{
		runstack[--runstackpos] = I1;
	}

	void RegexInterpreter::StackPush(int I1, int I2)
	{
		runstack[--runstackpos] = I1;
		runstack[--runstackpos] = I2;
	}

	void RegexInterpreter::StackPop()
	{
		runstackpos++;
	}

	void RegexInterpreter::StackPop(int framesize)
	{
		runstackpos += framesize;
	}

	int RegexInterpreter::StackPeek()
	{
		return runstack[runstackpos - 1];
	}

	int RegexInterpreter::StackPeek(int i)
	{
		return runstack[runstackpos - i - 1];
	}

	int RegexInterpreter::Operator()
	{
		return runoperator;
	}

	int RegexInterpreter::Operand(int i)
	{
		return runcodes[runcodepos + i + 1];
	}

	int RegexInterpreter::Leftchars()
	{
		return runtextpos - runtextbeg;
	}

	int RegexInterpreter::Rightchars()
	{
		return runtextend - runtextpos;
	}

	int RegexInterpreter::Bump()
	{
		return runrtl ? -1 : 1;
	}

	int RegexInterpreter::Forwardchars()
	{
		return runrtl ? runtextpos - runtextbeg : runtextend - runtextpos;
	}

	wchar_t RegexInterpreter::Forwardcharnext()
	{
		wchar_t ch = (runrtl ? runtext->at(--runtextpos) : runtext->at(runtextpos++));

		return (runci ? UtilsCharInternal::ToLower(ch, runculture) : ch);
	}

	bool RegexInterpreter::Stringmatch(const std::wstring& str)
	{
		int c;
		int pos;

		if (!runrtl)
		{
			if (runtextend - runtextpos < (c = str.length()))
			{
				return false;
			}

			pos = runtextpos + c;
		}
		else
		{
			if (runtextpos - runtextbeg < (c = str.length()))
			{
				return false;
			}

			pos = runtextpos;
		}

		if (!runci)
		{
			while (c != 0)
			{
				if (str[--c] != runtext->at(--pos))
				{
					return false;
				}
			}
		}
		else
		{
			while (c != 0)
			{
				if (str[--c] != UtilsCharInternal::ToLower(runtext->at(--pos), runculture))
				{
					return false;
				}
			}
		}

		if (!runrtl)
		{
			pos += str.length();
		}

		runtextpos = pos;

		return true;
	}

	bool RegexInterpreter::Refmatch(int index, int len)
	{
		int c;
		int pos;
		int cmpos;

		if (!runrtl)
		{
			if (runtextend - runtextpos < len)
			{
				return false;
			}

			pos = runtextpos + len;
		}
		else
		{
			if (runtextpos - runtextbeg < len)
			{
				return false;
			}

			pos = runtextpos;
		}

		cmpos = index + len;

		c = len;

		if (!runci)
		{
			while (c-- != 0)
			{
				if (runtext->at(--cmpos) != runtext->at(--pos))
				{
					return false;
				}
			}
		}
		else
		{
			while (c-- != 0)
			{
				if (UtilsCharInternal::ToLower(runtext->at(--cmpos), runculture) != UtilsCharInternal::ToLower(runtext->at(--pos), runculture))
				{
					return false;
				}
			}
		}

		if (!runrtl)
		{
			pos += len;
		}

		runtextpos = pos;

		return true;
	}

	void RegexInterpreter::Backwardnext()
	{
		runtextpos += runrtl ? 1 : -1;
	}

	wchar_t RegexInterpreter::CharAt(int j)
	{
		return runtext->at(j);
	}

	bool RegexInterpreter::FindFirstChar()
	{
		int i;
		std::wstring set;

		if (0 != (runanchors & (RegexFCD::Beginning | RegexFCD::Start | RegexFCD::EndZ | RegexFCD::End)))
		{
			if (!runcode->_rightToLeft)
			{
				if ((0 != (runanchors & RegexFCD::Beginning) && runtextpos > runtextbeg) || (0 != (runanchors & RegexFCD::Start) && runtextpos > runtextstart))
				{
					runtextpos = runtextend;
					return false;
				}

				if (0 != (runanchors & RegexFCD::EndZ) && runtextpos < runtextend - 1)
				{
					runtextpos = runtextend - 1;
				}
				else if (0 != (runanchors & RegexFCD::End) && runtextpos < runtextend)
				{
					runtextpos = runtextend;
				}
			}
			else
			{
				if ((0 != (runanchors & RegexFCD::End) && runtextpos < runtextend) || (0 != (runanchors & RegexFCD::EndZ) && (runtextpos < runtextend - 1 || (runtextpos == runtextend - 1 && CharAt(runtextpos) != L'\n'))) || (0 != (runanchors & RegexFCD::Start) && runtextpos < runtextstart))
				{
					runtextpos = runtextbeg;
					return false;
				}

				if (0 != (runanchors & RegexFCD::Beginning) && runtextpos > runtextbeg)
				{
					runtextpos = runtextbeg;
				}
			}

			if (runbmPrefix != NULL)
			{
				return runbmPrefix->IsMatch(runtext, runtextpos, runtextbeg, runtextend);
			}

			return true; // found a valid start or end anchor
		}
		if (runbmPrefix != NULL)
		{
			runtextpos = runbmPrefix->Scan(runtext, runtextpos, runtextbeg, runtextend);

			if (runtextpos == -1)
			{
				runtextpos = (runcode->_rightToLeft ? runtextbeg : runtextend);
				return false;
			}

			return true;
		}
		if (runfcPrefix == NULL)
		{
			return true;
		}

		runrtl = runcode->_rightToLeft;
		runci = runfcPrefix->getCaseInsensitive();
		set = runfcPrefix->getPrefix();

		if (RegexCharClass::IsSingleton(set))
		{
			wchar_t ch = RegexCharClass::SingletonChar(set);

			for (i = Forwardchars(); i > 0; i--)
			{
				if (ch == Forwardcharnext())
				{
					Backwardnext();
					return true;
				}
			}
		}
		else
		{
			for (i = Forwardchars(); i > 0; i--)
			{
				if (RegexCharClass::CharInClass(Forwardcharnext(), set))
				{
					Backwardnext();
					return true;
				}
			}
		}

		return false;
	}

	void RegexInterpreter::Go()
	{
		Goto(0);

		for (;;)
		{
			CheckTimeout();

			switch (Operator())
			{
				//case RegexCode::Stop:
			case 40:
				return;

				//case RegexCode::Nothing:
			case 22:
				break;

				//case RegexCode::Goto:
			case 38:
				Goto(Operand(0));
				continue;

				//case RegexCode::Testref:
			case 37:
				if (!IsMatched(Operand(0)))
				{
					break;
				}
				Advance(1);
				continue;

				//case RegexCode::Lazybranch:
			case 23:
				TrackPush(Textpos());
				Advance(1);
				continue;

				//case RegexCode::Lazybranch | RegexCode::Back:
			case 23 | 128:
				TrackPop();
				Textto(TrackPeek());
				Goto(Operand(0));
				continue;

				//case RegexCode::Setmark:
			case 31:
				StackPush(Textpos());
				TrackPush();
				Advance();
				continue;

				//case RegexCode::Nullmark:
			case 30:
				StackPush(-1);
				TrackPush();
				Advance();
				continue;

				//case RegexCode::Setmark | RegexCode::Back:
				//case RegexCode::Nullmark | RegexCode::Back:
			case 31 | 128:
			case 30 | 128:
				StackPop();
				break;

				//case RegexCode::Getmark:
			case 33:
				StackPop();
				TrackPush(StackPeek());
				Textto(StackPeek());
				Advance();
				continue;

				//case RegexCode::Getmark | RegexCode::Back:
			case 33 | 128:
				TrackPop();
				StackPush(TrackPeek());
				break;

				//case RegexCode::Capturemark:
			case 32:
				if (Operand(1) != -1 && !IsMatched(Operand(1)))
				{
					break;
				}
				StackPop();
				if (Operand(1) != -1)
				{
					TransferCapture(Operand(0), Operand(1), StackPeek(), Textpos());
				}
				else
				{
					Capture(Operand(0), StackPeek(), Textpos());
				}
				TrackPush(StackPeek());

				Advance(2);

				continue;

				//case RegexCode::Capturemark | RegexCode::Back:
			case 32 | 128:
				TrackPop();
				StackPush(TrackPeek());
				Uncapture();
				if (Operand(0) != -1 && Operand(1) != -1)
				{
					Uncapture();
				}

				break;

				//case RegexCode::Branchmark:
			case 24:
				{
					int matched;
					StackPop();

					matched = Textpos() - StackPeek();

					if (matched != 0)
					{
						// Nonempty match -> loop now
						TrackPush(StackPeek(), Textpos()); // Save old mark, textpos
						StackPush(Textpos()); // Make new mark
						Goto(Operand(0)); // Loop
					}
					else
					{
						// Empty match -> straight now
						TrackPush2(StackPeek()); // Save old mark
						Advance(1); // Straight
					}

					continue;
				}

				//case RegexCode::Branchmark | RegexCode::Back:
			case 24 | 128:
				TrackPop(2);
				StackPop();
				Textto(TrackPeek(1)); // Recall position
				TrackPush2(TrackPeek()); // Save old mark
				Advance(1); // Straight
				continue;

				//case RegexCode::Branchmark | RegexCode::Back2:
			case 24 | 256:
				TrackPop();
				StackPush(TrackPeek()); // Recall old mark
				break; // Backtrack

				//case RegexCode::Lazybranchmark:
			case 25:
				{
					// We hit this the first time through a lazy loop and after each 
					// successful match of the inner expression.  It simply continues
					// on and doesn't loop. 
					StackPop();

					int oldMarkPos = StackPeek();

					if (Textpos() != oldMarkPos)
					{
						// Nonempty match -> try to loop again by going to 'back' state
						if (oldMarkPos != -1)
						{
							TrackPush(oldMarkPos, Textpos()); // Save old mark, textpos
						}
						else
						{
							TrackPush(Textpos(), Textpos());
						}
					}
					else
					{
						// The inner expression found an empty match, so we'll go directly to 'back2' if we
						// backtrack.  In this case, we need to push something on the stack, since back2 pops.
						// However, in the case of ()+? or similar, this empty match may be legitimate, so push the text 
						// position associated with that empty match.
						StackPush(oldMarkPos);

						TrackPush2(StackPeek()); // Save old mark
					}

					Advance(1);
					continue;
				}

				//case RegexCode::Lazybranchmark | RegexCode::Back:
			case 25 | 128:
				{
					// After the first time, Lazybranchmark | RegexCode.Back occurs
					// with each iteration of the loop, and therefore with every attempted
					// match of the inner expression.  We'll try to match the inner expression, 
					// then go back to Lazybranchmark if successful.  If the inner expression 
					// failes, we go to Lazybranchmark | RegexCode.Back2
					int pos;

					TrackPop(2);
					pos = TrackPeek(1);
					TrackPush2(TrackPeek()); // Save old mark
					StackPush(pos); // Make new mark
					Textto(pos); // Recall position
					Goto(Operand(0)); // Loop
					continue;
				}

				//case RegexCode::Lazybranchmark | RegexCode::Back2:
			case 25 | 256:
				// The lazy loop has failed.  We'll do a true backtrack and 
				// start over before the lazy loop. 
				StackPop();
				TrackPop();
				StackPush(TrackPeek()); // Recall old mark
				break;

				//case RegexCode::Setcount:
			case 27:
				StackPush(Textpos(), Operand(0));
				TrackPush();
				Advance(1);
				continue;

				//case RegexCode::Nullcount:
			case 26:
				StackPush(-1, Operand(0));
				TrackPush();
				Advance(1);
				continue;

				//case RegexCode::Setcount | RegexCode::Back:
			case 27 | 128:
				StackPop(2);
				break;

				//case RegexCode::Nullcount | RegexCode::Back:
			case 26 | 128:
				StackPop(2);
				break;

				//case RegexCode::Branchcount:
			case 28:
				// StackPush:
				//  0: Mark
				//  1: Count
				{
					StackPop(2);
					int mark = StackPeek();
					int count = StackPeek(1);
					int matched = Textpos() - mark;

					if (count >= Operand(1) || (matched == 0 && count >= 0))
					{
						// Max loops or empty match -> straight now
						TrackPush2(mark, count); // Save old mark, count
						Advance(2); // Straight
					}
					else
					{
						// Nonempty match -> count+loop now
						TrackPush(mark); // remember mark
						StackPush(Textpos(), count + 1); // Make new mark, incr count
						Goto(Operand(0)); // Loop
					}

					continue;
				}

				//case RegexCode::Branchcount | RegexCode::Back:
			case 28 | 128:
				// TrackPush:
				//  0: Previous mark
				// StackPush:
				//  0: Mark (= current pos, discarded)
				//  1: Count
				TrackPop();
				StackPop(2);
				if (StackPeek(1) > 0)
				{
					// Positive -> can go straight
					Textto(StackPeek()); // Zap to mark
					TrackPush2(TrackPeek(), StackPeek(1) - 1); // Save old mark, old count
					Advance(2); // Straight
					continue;
				}

				StackPush(TrackPeek(), StackPeek(1) - 1); // recall old mark, old count
				break;

				//case RegexCode::Branchcount | RegexCode::Back2:
			case 28 | 256:
				// TrackPush:
				//  0: Previous mark
				//  1: Previous count
				TrackPop(2);
				StackPush(TrackPeek(), TrackPeek(1)); // Recall old mark, old count
				break; // Backtrack


				//case RegexCode::Lazybranchcount:
			case 29:
				// StackPush:
				//  0: Mark
				//  1: Count
				{
					StackPop(2);
					int mark = StackPeek();
					int count = StackPeek(1);

					if (count < 0)
					{
						// Negative count -> loop now
						TrackPush2(mark); // Save old mark
						StackPush(Textpos(), count + 1); // Make new mark, incr count
						Goto(Operand(0)); // Loop
					}
					else
					{
						// Nonneg count -> straight now
						TrackPush(mark, count, Textpos()); // Save mark, count, position
						Advance(2); // Straight
					}

					continue;
				}

				//case RegexCode::Lazybranchcount | RegexCode::Back:
			case 29 | 128:
				// TrackPush:
				//  0: Mark
				//  1: Count
				//  2: Textpos
				{
					TrackPop(3);
					int mark = TrackPeek();
					int textpos = TrackPeek(2);

					if (TrackPeek(1) < Operand(1) && textpos != mark)
					{
						// Under limit and not empty match -> loop
						Textto(textpos); // Recall position
						StackPush(textpos, TrackPeek(1) + 1); // Make new mark, incr count
						TrackPush2(mark); // Save old mark
						Goto(Operand(0)); // Loop
						continue;
					}
					// Max loops or empty match -> backtrack
					StackPush(TrackPeek(), TrackPeek(1)); // Recall old mark, count
					break; // backtrack
				}

				//case RegexCode::Lazybranchcount | RegexCode::Back2:
			case 29 | 256:
				// TrackPush:
				//  0: Previous mark
				// StackPush:
				//  0: Mark (== current pos, discarded)
				//  1: Count
				TrackPop();
				StackPop(2);
				StackPush(TrackPeek(), StackPeek(1) - 1); // Recall old mark, count
				break; // Backtrack

				//case RegexCode::Setjump:
			case 34:
				StackPush(Trackpos(), Crawlpos());
				TrackPush();
				Advance();
				continue;

				//case RegexCode::Setjump | RegexCode::Back:
			case 34 | 128:
				StackPop(2);
				break;

				//case RegexCode::Backjump:
			case 35:
				// StackPush:
				//  0: Saved trackpos
				//  1: Crawlpos
				StackPop(2);
				Trackto(StackPeek());

				while (Crawlpos() != StackPeek(1))
				{
					Uncapture();
				}

				break;

				//case RegexCode::Forejump:
			case 36:
				// StackPush:
				//  0: Saved trackpos
				//  1: Crawlpos
				StackPop(2);
				Trackto(StackPeek());
				TrackPush(StackPeek(1));
				Advance();
				continue;

				//case RegexCode::Forejump | RegexCode::Back:
			case 36 | 128:
				// TrackPush:
				//  0: Crawlpos
				TrackPop();

				while (Crawlpos() != TrackPeek())
				{
					Uncapture();
				}

				break;

				//case RegexCode::Bol:
			case 14:
				if (Leftchars() > 0 && CharAt(Textpos() - 1) != L'\n')
				{
					break;
				}
				Advance();
				continue;

				//case RegexCode::Eol:
			case 15:
				if (Rightchars() > 0 && CharAt(Textpos()) != L'\n')
				{
					break;
				}
				Advance();
				continue;

				//case RegexCode::Boundary:
			case 16:
				if (!IsBoundary(Textpos(), runtextbeg, runtextend))
				{
					break;
				}
				Advance();
				continue;

				//case RegexCode::Nonboundary:
			case 17:
				if (IsBoundary(Textpos(), runtextbeg, runtextend))
				{
					break;
				}
				Advance();
				continue;

				//case RegexCode::ECMABoundary:
			case 41:
				if (!IsECMABoundary(Textpos(), runtextbeg, runtextend))
				{
					break;
				}
				Advance();
				continue;

				//case RegexCode::NonECMABoundary:
			case 42:
				if (IsECMABoundary(Textpos(), runtextbeg, runtextend))
				{
					break;
				}
				Advance();
				continue;

				//case RegexCode::Beginning:
			case 18:
				if (Leftchars() > 0)
				{
					break;
				}
				Advance();
				continue;

				//case RegexCode::Start:
			case 19:
				if (Textpos() != Textstart())
				{
					break;
				}
				Advance();
				continue;

				//case RegexCode::EndZ:
			case 20:
				if (Rightchars() > 1 || Rightchars() == 1 && CharAt(Textpos()) != L'\n')
				{
					break;
				}
				Advance();
				continue;

				//case RegexCode::End:
			case 21:
				if (Rightchars() > 0)
				{
					break;
				}
				Advance();
				continue;

				//case RegexCode::One:
			case 9:
				if (Forwardchars() < 1 || Forwardcharnext() != static_cast<wchar_t>(Operand(0)))
				{
					break;
				}

				Advance(1);
				continue;

				//case RegexCode::Notone:
			case 10:
				if (Forwardchars() < 1 || Forwardcharnext() == static_cast<wchar_t>(Operand(0)))
				{
					break;
				}

				Advance(1);
				continue;

				//case RegexCode::Set:
			case 11:
				if (Forwardchars() < 1 || !RegexCharClass::CharInClass(Forwardcharnext(), runstrings[Operand(0)]))
				{
					break;
				}

				Advance(1);
				continue;

				//case RegexCode::Multi:
			case 12:
				{
					if (!Stringmatch(runstrings[Operand(0)]))
					{
						break;
					}

					Advance(1);
					continue;
				}

				//case RegexCode::Ref:
			case 13:
				{
					int capnum = Operand(0);

					if (IsMatched(capnum))
					{
						if (!Refmatch(MatchIndex(capnum), MatchLength(capnum)))
						{
							break;
						}
					}
					else
					{
						if ((static_cast<int>(runregex->getOptions()) & static_cast<int>(RegexOptions::ECMAScript)) == 0)
						{
							break;
						}
					}

					Advance(1);
					continue;
				}

				//case RegexCode::Onerep:
			case 0:
				{
					int c = Operand(1);

					if (Forwardchars() < c)
					{
						break;
					}

					wchar_t ch = static_cast<wchar_t>(Operand(0));

					while (c-- > 0)
					{
						if (Forwardcharnext() != ch)
						{
							goto BreakBackward;
						}
					}

					Advance(2);
					continue;
				}

				//case RegexCode::Notonerep:
			case 1:
				{
					int c = Operand(1);

					if (Forwardchars() < c)
					{
						break;
					}

					wchar_t ch = static_cast<wchar_t>(Operand(0));

					while (c-- > 0)
					{
						if (Forwardcharnext() == ch)
						{
							goto BreakBackward;
						}
					}

					Advance(2);
					continue;
				}

				//case RegexCode::Setrep:
			case 2:
				{
					int c = Operand(1);

					if (Forwardchars() < c)
					{
						break;
					}

					std::wstring set = runstrings[Operand(0)];

					while (c-- > 0)
					{
						if (!RegexCharClass::CharInClass(Forwardcharnext(), set))
						{
							goto BreakBackward;
						}
					}

					Advance(2);
					continue;
				}

				//case RegexCode::Oneloop:
			case 3:
				{
					int c = Operand(1);

					if (c > Forwardchars())
					{
						c = Forwardchars();
					}

					wchar_t ch = static_cast<wchar_t>(Operand(0));
					int i;

					for (i = c; i > 0; i--)
					{
						if (Forwardcharnext() != ch)
						{
							Backwardnext();
							break;
						}
					}

					if (c > i)
					{
						TrackPush(c - i - 1, Textpos() - Bump());
					}

					Advance(2);
					continue;
				}

				//case RegexCode::Notoneloop:
			case 4:
				{
					int c = Operand(1);

					if (c > Forwardchars())
					{
						c = Forwardchars();
					}

					wchar_t ch = static_cast<wchar_t>(Operand(0));
					int i;

					for (i = c; i > 0; i--)
					{
						if (Forwardcharnext() == ch)
						{
							Backwardnext();
							break;
						}
					}

					if (c > i)
					{
						TrackPush(c - i - 1, Textpos() - Bump());
					}

					Advance(2);
					continue;
				}

				//case RegexCode::Setloop:
			case 5:
				{
					int c = Operand(1);

					if (c > Forwardchars())
					{
						c = Forwardchars();
					}

					std::wstring set = runstrings[Operand(0)];
					int i;

					for (i = c; i > 0; i--)
					{
						if (!RegexCharClass::CharInClass(Forwardcharnext(), set))
						{
							Backwardnext();
							break;
						}
					}

					if (c > i)
					{
						TrackPush(c - i - 1, Textpos() - Bump());
					}

					Advance(2);
					continue;
				}

				//case RegexCode::Oneloop | RegexCode::Back:
				//case RegexCode::Notoneloop | RegexCode::Back:
			case 3 | 128:
			case 4 | 128:
				{
					TrackPop(2);
					int i = TrackPeek();
					int pos = TrackPeek(1);

					Textto(pos);

					if (i > 0)
					{
						TrackPush(i - 1, pos - Bump());
					}

					Advance(2);
					continue;
				}

				//case RegexCode::Setloop | RegexCode::Back:
			case 5 | 128:
				{
					TrackPop(2);
					int i = TrackPeek();
					int pos = TrackPeek(1);

					Textto(pos);

					if (i > 0)
					{
						TrackPush(i - 1, pos - Bump());
					}

					Advance(2);
					continue;
				}

				//case RegexCode::Onelazy:
				//case RegexCode::Notonelazy:
			case 6:
			case 7:
				{
					int c = Operand(1);

					if (c > Forwardchars())
					{
						c = Forwardchars();
					}

					if (c > 0)
					{
						TrackPush(c - 1, Textpos());
					}

					Advance(2);
					continue;
				}

				//case RegexCode::Setlazy:
			case 8:
				{
					int c = Operand(1);

					if (c > Forwardchars())
					{
						c = Forwardchars();
					}

					if (c > 0)
					{
						TrackPush(c - 1, Textpos());
					}

					Advance(2);
					continue;
				}

				//case RegexCode::Onelazy | RegexCode::Back:
			case 6 | 128:
				{
					TrackPop(2);
					int pos = TrackPeek(1);
					Textto(pos);

					if (Forwardcharnext() != static_cast<wchar_t>(Operand(0)))
					{
						break;
					}

					int i = TrackPeek();

					if (i > 0)
					{
						TrackPush(i - 1, pos + Bump());
					}

					Advance(2);
					continue;
				}

				//case RegexCode::Notonelazy | RegexCode::Back:
			case 7 | 128:
				{
					TrackPop(2);
					int pos = TrackPeek(1);
					Textto(pos);

					if (Forwardcharnext() == static_cast<wchar_t>(Operand(0)))
					{
						break;
					}

					int i = TrackPeek();

					if (i > 0)
					{
						TrackPush(i - 1, pos + Bump());
					}

					Advance(2);
					continue;
				}

				//case RegexCode::Setlazy | RegexCode::Back:
			case 8 | 128:
				{
					TrackPop(2);
					int pos = TrackPeek(1);
					Textto(pos);

					if (!RegexCharClass::CharInClass(Forwardcharnext(), runstrings[Operand(0)]))
					{
						break;
					}

					int i = TrackPeek();

					if (i > 0)
					{
						TrackPush(i - 1, pos + Bump());
					}

					Advance(2);
					continue;
				}

			default:
				throw  NotImplementedException("UnimplementedState");
			}

		BreakBackward:
			;

			// "break Backward" comes here:
			Backtrack();
		}
	}
}
