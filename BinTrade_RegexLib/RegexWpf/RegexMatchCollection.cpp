#include "RegexMatchCollection.h"

#include "Exception.h"
#include "Regex.h"
#include "RegexMatch.h" 


namespace Regexs
{
	int MatchCollection::infinite = 0x7FFFFFFF;

	MatchCollection::~MatchCollection()
	{
	}

	MatchCollection::MatchCollection(Regex* regex, const std::wstring* input, int beginning, int length, int startat): _done(false), _beginning(0), _length(0), _startat(0), _prevlen(0), _input(input)
	{
		if (startat < 0 || (size_t)startat > input->length())
		{
			throw ArgumentOutOfRangeException("startat BeginIndexNotNegative");
		}

		_regex = regex;
		if (_regex)
			_regex->_match_ptr.push_back(PtrObjectForRegex(this));

		_beginning = beginning;
		_length = length;
		_startat = startat;
		_prevlen = -1;
		_matches = std::vector<Match*>();
		_done = false;
	}

	Match* MatchCollection::GetMatch(int i)
	{
		if (i < 0)
		{
			return NULL;
		}

		if (_matches.size() > (size_t)i)
		{
			return static_cast<Match*>(_matches[i]);
		}

		if (_done)
		{
			return NULL;
		}

		Match* match;

		do
		{
			match = _regex->Run(false, _prevlen, _input, _beginning, _length, _startat);

			if (!match->getSuccess())
			{
				_done = true;
				return NULL;
			}

			_matches.push_back(match);

			_prevlen = match->_length;
			_startat = match->_textpos;
		}
		while (_matches.size() <= (size_t)i);

		return match;
	}

	int MatchCollection::getCount()
	{
		if (_done)
		{
			return _matches.size();
		}

		GetMatch(infinite);

		return _matches.size();
	}

	objectForRegex* MatchCollection::getSyncRoot()
	{
		return this;
	}

	bool MatchCollection::getIsSynchronized()
	{
		return false;
	}

	bool MatchCollection::getIsReadOnly() const
	{
		return true;
	}

	Match* MatchCollection::operator [](int i)
	{
		Match* match = GetMatch(i);

		if (match == NULL)
		{
			throw ArgumentOutOfRangeException("i");
		}

		return match;
	}

	void MatchCollection::CopyTo(void* array, int arrayIndex)
	{
		if (array == NULL)
			throw ArgumentNullException("array");

		if (arrayIndex < 0)
			throw ArgumentOutOfRangeException("array");
		try
		{
			objectForRegex* obj = reinterpret_cast<objectForRegex*>(array);
			ArrayMatch* array_match = dynamic_cast<ArrayMatch*>(obj);
			if (array_match != NULL)
			{
				const int count = getCount();

				// property access to force computation of whole array
				for (int i = 0; i < count; ++i)
				{
					if (i > arrayIndex) break;
					array_match->Data.at(arrayIndex + i) = _matches.at(i);
				}
			}
			else
			{
				throw ArgumentException("array");
			}
		}
		catch (std::bad_typeid e)
		{
			throw ArgumentException("array");
		}
		catch (std::out_of_range e)
		{
			throw ArgumentException("array");
		}
		catch (ArrayTypeMismatchException ex)
		{
			throw ArgumentException("Arg_InvalidArrayType ");
		}
	}

	IEnumerator* MatchCollection::GetEnumerator()
	{
		return new MatchEnumerator(this);
	}

	MatchEnumerator::~MatchEnumerator()
	{
	}

	MatchEnumerator::MatchEnumerator(MatchCollection* matchcoll): _match(NULL), _curindex(0), _done(false)
	{
		_matchcoll = matchcoll;
		if (_matchcoll != NULL && _matchcoll->_regex != NULL)
		{
			_matchcoll->_regex->_match_ptr.push_back(PtrObjectForRegex(this));
		}
	}

	bool MatchEnumerator::MoveNext()
	{
		if (_done)
		{
			return false;
		}

		_match = _matchcoll->GetMatch(_curindex);
		_curindex++;

		if (_match == NULL)
		{
			_done = true;
			return false;
		}

		return true;
	}

	objectForRegex* MatchEnumerator::getCurrent()
	{
		if (_match == NULL)
		{
			throw InvalidOperationException("EnumNotStarted");
		}
		return _match;
	}

	void MatchEnumerator::Reset()
	{
		_curindex = 0;
		_done = false;
		_match = NULL;
	}
}
