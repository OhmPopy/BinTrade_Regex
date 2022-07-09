#include "RegexCaptureCollection.h"

#include "Exception.h"
#include "Regex.h"
#include "RegexGroup.h"
#include "RegexMatch.h" 

namespace Regexs
{
	CaptureCollection::~CaptureCollection()
	{
		for (size_t i = 0; i < _captures.size(); ++i)
		{
			if (_captures[i])
				delete _captures[i];
		}
	}

	CaptureCollection::CaptureCollection(Group* group)
	{
		_group = group;
		_capcount = _group->_capcount;
	}

	objectForRegex* CaptureCollection::getSyncRoot()
	{
		return _group;
	}

	bool CaptureCollection::getIsSynchronized()
	{
		return false;
	}

	bool CaptureCollection::getIsReadOnly() const
	{
		return true;
	}

	int CaptureCollection::getCount()
	{
		return _capcount;
	}

	Capture* CaptureCollection::operator [](int i)
	{
		return GetCapture(i);
	}

	void CaptureCollection::CopyTo(void* array, int arrayIndex)
	{
		if (array == NULL)
			throw ArgumentNullException("array");

		try
		{
			objectForRegex* obj = reinterpret_cast<objectForRegex*>(array);
			ArrayCapture* array_capture = dynamic_cast<ArrayCapture*>(obj);
			if (array_capture != NULL)
			{
				for (int i = arrayIndex, j = 0; j < getCount(); i++, j++)
				{
					array_capture->Data.at(i) = this->operator[](j);
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

	IEnumerator* CaptureCollection::GetEnumerator()
	{
		return new CaptureEnumerator(this);
	}

	Capture* CaptureCollection::GetCapture(int i)
	{
		if (i == _capcount - 1 && i >= 0)
		{
			return _group;
		}

		if (i >= _capcount || i < 0)
		{
			throw ArgumentOutOfRangeException("i");
		}

		// first time a capture is accessed, compute them all
		if (_captures.empty())
		{
			_captures = std::vector<Capture*>(_capcount);

			for (int j = 0; j < _capcount - 1; j++)
			{
				_captures[j] = new Capture(_group->_text, _group->_caps[j * 2], _group->_caps[j * 2 + 1]);
			}
		}

		return _captures[i];
	}

	CaptureEnumerator::~CaptureEnumerator()
	{
	}

	CaptureEnumerator::CaptureEnumerator(CaptureCollection* rcc)
	{
		_curindex = -1;
		_rcc = rcc;

		if (_rcc != NULL && _rcc->_group != NULL)
		{
			Match* match = dynamic_cast<Match*>(_rcc->_group);
			if (match)
				match->_regex->_match_ptr.push_back(PtrObjectForRegex(this));
		}
	}

	bool CaptureEnumerator::MoveNext()
	{
		int size = _rcc->getCount();

		if (_curindex >= size)
		{
			return false;
		}

		_curindex++;

		return (_curindex < size);
	}

	objectForRegex* CaptureEnumerator::getCurrent()
	{
		return getCapture();
	}

	Regexs::Capture* CaptureEnumerator::getCapture() const
	{
		if (_curindex < 0 || _curindex >= _rcc->getCount())
		{
			throw InvalidOperationException("EnumNotStarted");
		}

		return _rcc->operator[](_curindex);
	}

	void CaptureEnumerator::Reset()
	{
		_curindex = -1;
	}
}
