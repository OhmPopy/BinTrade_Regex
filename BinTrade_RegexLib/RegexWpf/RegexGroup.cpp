#include "RegexGroup.h"

#include "Exception.h"
#include "RegexCaptureCollection.h"

namespace Regexs
{
	Group* Group::_emptygroup = new Group(new std::wstring(L""), std::vector<int>(0) , 0, true);

	Group::Group(const std::wstring* text, const std::vector<int>& caps, int capcount, bool is_static_init) : Group(text, caps, capcount)
	{
		if (_capcoll == NULL)
		{
			_capcoll = new CaptureCollection(this);
		}
	}

	Group::~Group()
	{
		if (_capcoll)
			delete _capcoll;
	}

	Group::Group(const std::wstring* text,const std::vector<int>& caps, int capcount) : Capture(text, capcount == 0 ? 0 : caps[(capcount - 1) * 2], capcount == 0 ? 0 : caps[(capcount * 2) - 1]), _capcoll(NULL)
	{
		_caps = caps;
		_capcount = capcount;
	}

	bool Group::getSuccess() const
	{
		return _capcount != 0;
	}

	CaptureCollection* Group::getCaptures()
	{
		if (_capcoll == NULL)
		{
			_capcoll = new CaptureCollection(this);
		}

		return _capcoll;
	}

	Group* Group::Synchronized(Group* inner)
	{
		if (inner == NULL)
			throw ArgumentNullException("inner");

		// force Captures to be computed.

		CaptureCollection* capcoll = inner->Captures;

		if (inner->_capcount > 0)
			Capture* dummy = capcoll->operator[](0);

		return inner;
	}
}
