#include "RegexMatch.h"

#include <cassert>

#include "Exception.h"
#include "Regex.h"
#include "RegexCaptureCollection.h"
#include "RegexGroupCollection.h"
#include "RegexReplacement.h"
#include "RegexParser.h"

namespace Regexs
{
	Match* Match::_empty = new Match(NULL, 1, new std::wstring(L""), 0, 0, 0, true);

	Match::~Match()
	{
		if (_empty == this)
		{
			int a = 0;
		}
		if (_groupcoll)
			delete _groupcoll;
	}

	Match* Match::getEmpty()
	{
		return _empty;
	}

	Match::Match(Regex* regex, int capcount, const std::wstring* text, int begpos, int len, int startpos, bool is_static_init) :
		Match(regex, capcount, text, begpos, len, startpos)
	{
		if (_groupcoll == NULL)
		{
			std::unordered_map<int, int> temp = std::unordered_map<int, int>();
			_groupcoll = new GroupCollection(this, temp);
		}
		if (_capcoll == NULL)
		{
			_capcoll = new CaptureCollection(this);
		}
	}

	Match::Match(Regex* regex, int capcount, const std::wstring* text, int begpos, int len, int startpos) : Group(text, std::vector<int>(2), 0), _groupcoll(NULL), _textpos(0)
	{
		_regex = regex;
		if (_regex)
			_regex->_match_ptr.push_back(PtrObjectForRegex(this));
		_matchcount = std::vector<int>(capcount);
		_matches = std::vector<std::vector<int>>(capcount);

		_matches[0] = _caps;
		_textbeg = begpos;
		_textend = begpos + len;
		_textstart = startpos;
		_balancing = false;

		// No need for an exception here.  This is only called internally, so we'll use an Assert instead
		assert((!(_textbeg < 0 || _textstart < _textbeg || _textend < _textstart || _text->length() < (size_t)_textend), L"The parameters are out of range."));
	}

	void Match::Reset(Regex* regex, const std::wstring* text, int textbeg, int textend, int textstart)
	{
		_regex = regex;
		_text = text;
		_textbeg = textbeg;
		_textend = textend;
		_textstart = textstart;

		for (size_t i = 0; i < _matchcount.size(); i++)
		{
			_matchcount[i] = 0;
		}

		_balancing = false;
	}

	GroupCollection* Match::getGroups()
	{
		if (_groupcoll == NULL)
		{
			std::unordered_map<int, int> temp = std::unordered_map<int, int>();
			_groupcoll = new GroupCollection(this, temp);
		}

		return _groupcoll;
	}

	Match* Match::NextMatch()
	{
		if (_regex == NULL)
		{
			return this;
		}

		return _regex->Run(false, _length, _text, _textbeg, _textend - _textbeg, _textpos);
	}

	std::wstring Match::Result(const std::wstring& replacement)
	{
		if (_regex == NULL)
		{
			throw NotSupportedException("NoResultOnFailed");
		}

		RegexReplacement* repl = NULL;

		if (repl == NULL || repl->getPattern() != replacement)
			repl = RegexParser::ParseReplacement(replacement, _regex->caps, _regex->GetCapsize(), _regex->capnames, _regex->GetRoptions());

		auto result = repl->Replacement(this);
		delete repl;
		return result;
	}

	std::wstring Match::GroupToStringImpl(int groupnum)
	{
		const int c = _matchcount[groupnum];
		if (c == 0)
		{
			return L"";
		}

		const std::vector<int> matches = _matches[groupnum];

		return _text->substr(matches[(c - 1) * 2], matches[(c * 2) - 1]);
	}

	std::wstring Match::LastGroupToStringImpl()
	{
		return GroupToStringImpl(_matchcount.size() - 1);
	}

	Match* Match::Synchronized(Match* inner)
	{
		if (inner == NULL)
		{
			throw ArgumentNullException("inner");
		}

		const int numgroups = inner->_matchcount.size();

		// Populate all groups by looking at each one
		for (int i = 0; i < numgroups; i++)
		{
			Group* group = inner->getGroups()->operator[](i);

			// Depends on the fact that Group.Synchronized just
			// operates on and returns the same instance
			Group::Synchronized(group);
		}

		return inner;
	}

	void Match::AddMatch(int cap, int start, int len)
	{
		if (_matches[cap].empty())
		{
			_matches[cap] = std::vector<int>(2);
		}

		const int capcount = _matchcount[cap];

		if ((size_t)capcount * 2 + 2 > _matches[cap].size())
		{
			const std::vector<int> oldmatches = _matches[cap];
			std::vector<int> newmatches(capcount * 8);
			for (int j = 0; j < capcount * 2; j++)
			{
				newmatches[j] = oldmatches[j];
			}
			_matches[cap] = newmatches;
		}

		_matches[cap][capcount * 2] = start;
		_matches[cap][capcount * 2 + 1] = len;
		_matchcount[cap] = capcount + 1;
	}

	void Match::BalanceMatch(int cap)
	{
		_balancing = true;

		// we'll look at the last capture first
		const int capcount = _matchcount[cap];
		int target = capcount * 2 - 2;

		// first see if it is negative, and therefore is a reference to the next available
		// capture group for balancing.  If it is, we'll reset target to point to that capture.
		if (_matches[cap][target] < 0)
		{
			target = -3 - _matches[cap][target];
		}

		// move back to the previous capture
		target -= 2;

		// if the previous capture is a reference, just copy that reference to the end.  Otherwise, point to it. 
		if (target >= 0 && _matches[cap][target] < 0)
		{
			AddMatch(cap, _matches[cap][target], _matches[cap][target + 1]);
		}
		else
		{
			AddMatch(cap, -3 - target, -4 - target);
		}
	}

	void Match::RemoveMatch(int cap)
	{
		_matchcount[cap]--;
	}

	bool Match::IsMatched(int cap)
	{
		return (size_t)cap < _matchcount.size() && _matchcount[cap] > 0 && _matches[cap][_matchcount[cap] * 2 - 1] != (-3 + 1);
	}

	int Match::MatchIndex(int cap)
	{
		const int i = _matches[cap][_matchcount[cap] * 2 - 2];
		if (i >= 0)
		{
			return i;
		}

		return _matches[cap][-3 - i];
	}

	int Match::MatchLength(int cap)
	{
		const int i = _matches[cap][_matchcount[cap] * 2 - 1];
		if (i >= 0)
		{
			return i;
		}

		return _matches[cap][-3 - i];
	}

	void Match::Tidy(int textpos)
	{
		const std::vector<int> interval = _matches[0];
		_index = interval[0];
		_length = interval[1];
		_textpos = textpos;
		_capcount = _matchcount[0];

		if (_balancing)
		{
			// The idea here is that we want to compact all of our unbalanced captures.  To do that we
			// use j basically as a count of how many unbalanced captures we have at any given time 
			// (really j is an index, but j/2 is the count).  First we skip past all of the real captures
			// until we find a balance captures.  Then we check each subsequent entry.  If it's a balance
			// capture (it's negative), we decrement j.  If it's a real capture, we increment j and copy 
			// it down to the last free position. 
			for (size_t cap = 0; cap < _matchcount.size(); cap++)
			{
				std::vector<int> matcharray;

				const int limit = _matchcount[cap] * 2;
				matcharray = _matches[cap];

				int i = 0;
				int j;

				for (i = 0; i < limit; i++)
				{
					if (matcharray[i] < 0)
					{
						break;
					}
				}

				for (j = i; i < limit; i++)
				{
					if (matcharray[i] < 0)
					{
						// skip negative values
						j--;
					}
					else
					{
						// but if we find something positive (an actual capture), copy it back to the last 
						// unbalanced position. 
						if (i != j)
						{
							matcharray[j] = matcharray[i];
						}
						j++;
					}
				}

				_matches[cap] = matcharray;
				_matchcount[cap] = j / 2;
			}

			_balancing = false;
		}
	}

	std::wstring Match::Dump()
	{
		StringBuilder builder;
		for (size_t i = 0; i < _matchcount.size(); i++)
		{
			builder.AppendLine(L"Capnum " + std::to_wstring(i) + L":");

			for (int j = 0; j < _matchcount[i]; j++)
			{
				std::wstring text = L"";

				if (_matches[i][j * 2] >= 0)
					text = _text->substr(_matches[i][j * 2], _matches[i][j * 2 + 1]);

				builder.AppendLine(L"  (" + std::to_wstring(_matches[i][j * 2]) + L"," + std::to_wstring(_matches[i][j * 2 + 1]) + L") " + text);
			}
		}
		return builder.ToString();
	}

	void Match::Release()
	{
		if (_regex)
		{
			if (!_regex->_auto_release)
			{
				_regex->_auto_release = true;
				delete _regex;
			}
		}
		else
		{
			if (_empty != this)
				delete this;
		}
	}


	MatchSparse::MatchSparse(Regex* regex, std::unordered_map<int, int>& caps, int capcount, const std::wstring* text, int begpos, int len, int startpos) : Match(regex, capcount, text, begpos, len, startpos)
	{
		if (_regex)
			_regex->_match_ptr.push_back(PtrObjectForRegex(this));
		_caps = caps;
	}

	GroupCollection* MatchSparse::getGroups()
	{
		if (_groupcoll == NULL)
		{
			_groupcoll = new GroupCollection(this, _caps);
		}

		return _groupcoll;
	}
}
