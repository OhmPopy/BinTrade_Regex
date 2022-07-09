#include "RegexRunner.h"

#include "Regex.h"
#include "RegexMatchTimeoutException.h"
#include "RegexCharClass.h"
#include "RegexMatch.h"

namespace Regexs
{
	RegexRunner::~RegexRunner()
	{
	}

	RegexRunner::RegexRunner(): runtextbeg(0), runtextend(0), runtextstart(0), runtext(NULL),
	                            runtextpos(0), runtrackpos(0), runstackpos(0), runcrawlpos(0),
	                            runtrackcount(0), runmatch(NULL), runregex(NULL), timeout(0),
	                            ignoreTimeout(false), timeoutOccursAt(0), timeoutChecksToSkip(0)
	{
	}

	Match* RegexRunner::Scan(Regex* regex, const std::wstring* text, int textbeg, int textend, int textstart, int prevlen, bool quick)
	{
		return Scan(regex, text, textbeg, textend, textstart, prevlen, quick, regex->getMatchTimeout());
	}

	Match* RegexRunner::Scan(Regex* regex, const std::wstring* text, int textbeg, int textend, int textstart, int prevlen, bool quick, TimeSpan timeout)
	{
		int bump;
		int stoppos;
		bool initted = false;

		// We need to re-validate timeout here because Scan is historically protected and
		// thus there is a possibility it is called from user code:
		Regex::ValidateMatchTimeout(timeout);

		this->ignoreTimeout = (Regex::InfiniteMatchTimeout == timeout);
		this->timeout = this->ignoreTimeout ? static_cast<int>(Regex::InfiniteMatchTimeout.TotalMilliseconds) : static_cast<int>(timeout.TotalMilliseconds + 0.5); // Round

		runregex = regex;
		runtext = text;
		runtextbeg = textbeg;
		runtextend = textend;
		runtextstart = textstart;

		bump = runregex->getRightToLeft() ? -1 : 1;
		stoppos = runregex->getRightToLeft() ? runtextbeg : runtextend;

		runtextpos = textstart;

		// If previous match was empty or failed, advance by one before matching

		if (prevlen == 0)
		{
			if (runtextpos == stoppos)
			{
				return Match::getEmpty();
			}

			runtextpos += bump;
		}

		StartTimeoutWatch();

		for (;;)
		{
			if (FindFirstChar())
			{
				CheckTimeout();

				if (!initted)
				{
					InitMatch();
					initted = true;
				}
				Go();

				if (runmatch->_matchcount[0] > 0)
				{
					// <
					return TidyMatch(quick);
				}

				// reset state for another go
				runtrackpos = runtrack.size();
				runstackpos = runstack.size();
				runcrawlpos = runcrawl.size();
			}

			// failure!

			if (runtextpos == stoppos)
			{
				TidyMatch(true);
				return Match::getEmpty();
			}

			// <

			// Bump by one and start again

			runtextpos += bump;
		}

		// We never get here
	}

	void RegexRunner::StartTimeoutWatch()
	{
		if (ignoreTimeout)
		{
			return;
		}

		timeoutChecksToSkip = TimeoutCheckFrequency;

		// We are using Environment.TickCount and not Timewatch for performance reasons.
		// Environment.TickCount is an int that cycles. We intentionally let timeoutOccursAt
		// overflow it will still stay ahead of Environment.TickCount for comparisons made
		// in DoCheckTimeout():
		//		unchecked
		timeoutOccursAt = GetTickCount() + timeout;
	}

	void RegexRunner::CheckTimeout()
	{
		if (ignoreTimeout)
		{
			return;
		}

		if (--timeoutChecksToSkip != 0)
		{
			return;
		}

		timeoutChecksToSkip = TimeoutCheckFrequency;
		DoCheckTimeout();
	}

	void RegexRunner::DoCheckTimeout()
	{
		// Note that both, Environment.TickCount and timeoutOccursAt are ints and can overflow and become negative.
		// See the comment in StartTimeoutWatch().

		int currentMillis = GetTickCount();

		if (currentMillis < timeoutOccursAt)
		{
			return;
		}

		if (0 > timeoutOccursAt && 0 < currentMillis)
		{
			return;
		}


		throw RegexMatchTimeoutException(*runtext, runregex->GetPattern(), TimeSpan::FromMilliseconds(timeout));
	}

	void RegexRunner::InitMatch()
	{
		// Use a hashtable'ed Match object if the capture numbers are sparse

		if (runmatch == NULL)
		{
			if (!runregex->caps.empty())
			{
				runmatch = new MatchSparse(runregex, runregex->caps, runregex->GetCapsize(), runtext, runtextbeg, runtextend - runtextbeg, runtextstart);
			}
			else
			{
				runmatch = new Match(runregex, runregex->GetCapsize(), runtext, runtextbeg, runtextend - runtextbeg, runtextstart);
			}
		}
		else
		{
			runmatch->Reset(runregex, runtext, runtextbeg, runtextend, runtextstart);
		}

		// note we test runcrawl, because it is the last one to be allocated
		// If there is an alloc failure in the middle of the three allocations,
		// we may still return to reuse this instance, and we want to behave
		// as if the allocations didn't occur. (we used to test _trackcount != 0)

		if (!runcrawl.empty())
		{
			runtrackpos = runtrack.size();
			runstackpos = runstack.size();
			runcrawlpos = runcrawl.size();
			return;
		}

		InitTrackCount();

		int tracksize = runtrackcount * 8;
		int stacksize = runtrackcount * 8;

		if (tracksize < 32)
		{
			tracksize = 32;
		}
		if (stacksize < 16)
		{
			stacksize = 16;
		}

		runtrack = std::vector<int>(tracksize);
		runtrackpos = tracksize;

		runstack = std::vector<int>(stacksize);
		runstackpos = stacksize;

		runcrawl = std::vector<int>(32);
		runcrawlpos = 32;
	}

	Match* RegexRunner::TidyMatch(bool quick)
	{
		if (!quick)
		{
			Match* match = runmatch;

			runmatch = NULL;

			match->Tidy(runtextpos);
			return match;
		}
		else
		{
			// in quick mode, a successful match returns null, and
			// the allocated match object is left in the cache

			return NULL;
		}
	}

	void RegexRunner::EnsureStorage()
	{
		if (runstackpos < runtrackcount * 4)
		{
			DoubleStack();
		}
		if (runtrackpos < runtrackcount * 4)
		{
			DoubleTrack();
		}
	}

	bool RegexRunner::IsBoundary(int index, int startpos, int endpos)
	{
		return (index > startpos && RegexCharClass::IsWordChar(runtext->at(index - 1))) != (index < endpos && RegexCharClass::IsWordChar(runtext->at(index)));
	}

	bool RegexRunner::IsECMABoundary(int index, int startpos, int endpos)
	{
		return (index > startpos && RegexCharClass::IsECMAWordChar(runtext->at(index - 1))) != (index < endpos && RegexCharClass::IsECMAWordChar(runtext->at(index)));
	}

	bool RegexRunner::CharInSet(wchar_t ch, const std::wstring& set, const std::wstring& category)
	{
		std::wstring charClass = RegexCharClass::ConvertOldStringsToClass(set, category);
		return RegexCharClass::CharInClass(ch, charClass);
	}

	bool RegexRunner::CharInClass(wchar_t ch, const std::wstring& charClass)
	{
		return RegexCharClass::CharInClass(ch, charClass);
	}

	void RegexRunner::DoubleTrack()
	{
		std::vector<int> newtrack = std::vector<int>(runtrack.size() * 2);

		std::copy_n(runtrack.begin(), runtrack.size(), newtrack.begin() + runtrack.size());
		runtrackpos += runtrack.size();
		runtrack = newtrack;
	}

	void RegexRunner::DoubleStack()
	{
		std::vector<int> newstack = std::vector<int>(runstack.size() * 2);

		std::copy_n(runstack.begin(), runstack.size(), newstack.begin() + runstack.size());
		runstackpos += runstack.size();
		runstack = newstack;
	}

	void RegexRunner::DoubleCrawl()
	{
		std::vector<int> newcrawl = std::vector<int>(runcrawl.size() * 2);

		std::copy_n(runcrawl.begin(), runcrawl.size(), newcrawl.begin() + runcrawl.size());
		runcrawlpos += runcrawl.size();
		runcrawl = newcrawl;
	}

	void RegexRunner::Crawl(int i)
	{
		if (runcrawlpos == 0)
		{
			DoubleCrawl();
		}

		runcrawl[--runcrawlpos] = i;
	}

	int RegexRunner::Popcrawl()
	{
		return runcrawl[runcrawlpos++];
	}

	int RegexRunner::Crawlpos()
	{
		return runcrawl.size() - runcrawlpos;
	}

	void RegexRunner::Capture(int capnum, int start, int end)
	{
		if (end < start)
		{
			int T;

			T = end;
			end = start;
			start = T;
		}

		Crawl(capnum);
		runmatch->AddMatch(capnum, start, end - start);
	}

	void RegexRunner::TransferCapture(int capnum, int uncapnum, int start, int end)
	{
		int start2;
		int end2;

		// these are the two intervals that are cancelling each other

		if (end < start)
		{
			int T;

			T = end;
			end = start;
			start = T;
		}

		start2 = MatchIndex(uncapnum);
		end2 = start2 + MatchLength(uncapnum);

		// The new capture gets the innermost defined interval

		if (start >= end2)
		{
			end = start;
			start = end2;
		}
		else if (end <= start2)
		{
			start = start2;
		}
		else
		{
			if (end > end2)
			{
				end = end2;
			}
			if (start2 > start)
			{
				start = start2;
			}
		}

		Crawl(uncapnum);
		runmatch->BalanceMatch(uncapnum);

		if (capnum != -1)
		{
			Crawl(capnum);
			runmatch->AddMatch(capnum, start, end - start);
		}
	}

	void RegexRunner::Uncapture()
	{
		int capnum = Popcrawl();
		runmatch->RemoveMatch(capnum);
	}

	bool RegexRunner::IsMatched(int cap)
	{
		return runmatch->IsMatched(cap);
	}

	int RegexRunner::MatchIndex(int cap)
	{
		return runmatch->MatchIndex(cap);
	}

	int RegexRunner::MatchLength(int cap)
	{
		return runmatch->MatchLength(cap);
	}

	void RegexRunner::DumpState()
	{
		//bin::Debug::WriteLine(L"Text:  " + TextposDescription());
		//bin::Debug::WriteLine(L"Track: " + StackDescription(runtrack, runtrackpos));
		//bin::Debug::WriteLine(L"Stack: " + StackDescription(runstack, runstackpos));
	}

	std::wstring RegexRunner::StackDescription(std::vector<int> A, int Index)
	{
		StringBuilder Sb = StringBuilder();

		Sb.Append(std::to_wstring((A.size() - Index)));
		Sb.AppendChar('/');
		Sb.Append(std::to_wstring(A.size()));

		if (Sb.GetLength() < 8)
		{
			std::wstring temp(8 - Sb.GetLength(), ' ');
			Sb.Append(temp);
		}


		Sb.Append(L"(");

		for (size_t i = Index; i < A.size(); i++)
		{
			if (i > (size_t)Index)
				Sb.AppendChar(' ');
			Sb.AppendChar(A[i]);
		}

		Sb.AppendChar(')');

		return Sb.ToString();
	}

	std::wstring RegexRunner::TextposDescription()
	{
		StringBuilder Sb = StringBuilder();

		Sb.Append(std::to_wstring(runtextpos));

		if (Sb.GetLength() < 8)
		{
			std::wstring temp(8 - Sb.GetLength(), ' ');
			Sb.Append(temp);
		}

		if (runtextpos > runtextbeg)
			Sb.Append(RegexCharClass::CharDescription(runtext->at(runtextpos - 1)));
		else
			Sb.AppendChar('^');

		Sb.AppendChar('>');

		int remaining = runtextend - runtextpos;

		for (int i = runtextpos; i < runtextend; i++)
		{
			Sb.Append(RegexCharClass::CharDescription(runtext->at(i)));
		}
		if (Sb.GetLength() >= 64)
		{
			Sb.setLength(61);
			Sb.Append(L"...");
		}
		else
		{
			Sb.AppendChar('$');
		}

		return Sb.ToString();
	}
}
