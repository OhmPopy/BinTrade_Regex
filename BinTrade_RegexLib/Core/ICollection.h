#pragma once
#include <cstddef>
#include "objectForRegex.h"

namespace Regexs
{
	class REGEX_X_API IEnumerator
	{
	public:
		virtual ~IEnumerator()
		{
		}

		virtual objectForRegex* getCurrent() { return NULL; }
		virtual bool MoveNext() { return false; }

		__declspec(property(get = getCurrent)) objectForRegex* Current;

		virtual void Reset()
		{
		}
	};

	class REGEX_X_API IEnumerable 
	{
	public:
		virtual ~IEnumerable()
		{
		}

		virtual IEnumerator* GetEnumerator() { return NULL; }
	};

	class REGEX_X_API ICollection : public IEnumerable
	{
	public:
		virtual ~ICollection()
		{
		}

		virtual int getCount() { return 0; }
		virtual objectForRegex* getSyncRoot() { return NULL; }
		virtual bool getIsSynchronized() { return false; }


		__declspec(property(get = getCount)) int Count;
		__declspec(property(get = getSyncRoot)) objectForRegex* SyncRoot;
		__declspec(property(get = getIsSynchronized)) bool IsSynchronized;


		virtual void CopyTo(void* array, int index)
		{
		}
	};
}
