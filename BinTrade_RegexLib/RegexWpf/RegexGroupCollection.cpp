#include "RegexGroupCollection.h"

#include "Exception.h"
#include "Regex.h"
#include "RegexMatch.h"

namespace Regexs
{
	GroupCollection::~GroupCollection()
	{
		for (size_t i = 0; i < _groups.size(); ++i)
		{
			if (_groups[i])
				delete _groups[i];
		}
	}

	GroupCollection::GroupCollection(Match* match, std::unordered_map<int, int>& caps)
	{
		_match = match;
		_captureMap = caps;
	}

	bool GroupCollection::TryGetValue(const std::wstring& key, Group*& value)
	{
		value = operator[](key);
		if (value == Group::_emptygroup)
		{
			value = NULL;
			return false;
		}
		return true;
	}

	objectForRegex* GroupCollection::getSyncRoot()
	{
		return _match;
	}

	bool GroupCollection::getIsSynchronized()
	{
		return false;
	}

	bool GroupCollection::getIsReadOnly() const
	{
		return true;
	}

	int GroupCollection::getCount()
	{
		return _match->_matchcount.size();
	}

	Group* GroupCollection::operator [](int groupnum)
	{
		return GetGroup(groupnum);
	}

	Group* GroupCollection::operator [](const std::wstring& groupname)
	{
		if (_match->_regex == NULL)
		{
			return Group::_emptygroup;
		}

		return GetGroup(_match->_regex->GroupNumberFromName(groupname));
	}

	Group* GroupCollection::GetGroup(int groupnum)
	{
		if (!_captureMap.empty())
		{
			std::unordered_map<int, int>::const_iterator o = _captureMap.find(groupnum);

			if (o == _captureMap.end())
			{
				return Group::_emptygroup;
			}
			//throw new ArgumentOutOfRangeException("groupnum");
			return GetGroupImpl(o->second);
		}
		else
		{
			//if (groupnum >= _match._regex.CapSize || groupnum < 0)
			//   throw new ArgumentOutOfRangeException("groupnum");
			if (groupnum >= (int)_match->_matchcount.size() || groupnum < 0)
			{
				return Group::_emptygroup;
			}

			return GetGroupImpl(groupnum);
		}
	}

	Group* GroupCollection::GetGroupImpl(int groupnum)
	{
		if (groupnum == 0)
		{
			return _match;
		}

		// Construct all the Group objects the first time GetGroup is called

		if (_groups.empty())
		{
			_groups = std::vector<Group*>(_match->_matchcount.size() - 1);
			for (size_t i = 0; i < _groups.size(); i++)
			{
				_groups[i] = new Group(_match->_text, _match->_matches[i + 1], _match->_matchcount[i + 1]);
			}
		}

		return _groups[groupnum - 1];
	}

	void GroupCollection::CopyTo(void* array, int arrayIndex)
	{
		if (array == NULL)
			throw ArgumentNullException("array");

		try
		{
			objectForRegex* obj = reinterpret_cast<objectForRegex*>(array);
			ArrayGroup* array_group = dynamic_cast<ArrayGroup*>(obj);
			if (array_group != NULL)
			{
				for (int i = arrayIndex, j = 0; j < getCount(); i++, j++)
				{
					array_group->Data.at(i) = this->operator[](j);
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
			throw IndexOutOfRangeException("array");
		}
	}

	IEnumerator* GroupCollection::GetEnumerator()
	{
		return new GroupEnumerator(this);
	}

	GroupEnumerator::GroupEnumerator(GroupCollection* rgc)
	{
		_curindex = -1;
		_rgc = rgc;
		if (_rgc != NULL && _rgc->_match != NULL && _rgc->_match->_regex != NULL)
		{
			_rgc->_match->_regex->_match_ptr.push_back(PtrObjectForRegex(this));
		}
	}

	bool GroupEnumerator::MoveNext()
	{
		const int size = _rgc->getCount();

		if (_curindex >= size)
		{
			return false;
		}

		_curindex++;

		return (_curindex < size);
	}

	objectForRegex* GroupEnumerator::getCurrent()
	{
		return getCapture();
	}

	Regexs::Capture* GroupEnumerator::getCapture() const
	{
		if (_curindex < 0 || _curindex >= _rgc->getCount())
		{
			throw InvalidOperationException("EnumNotStarted");
		}

		return _rgc->operator[](_curindex);
	}

	void GroupEnumerator::Reset()
	{
		_curindex = -1;
	}
}
