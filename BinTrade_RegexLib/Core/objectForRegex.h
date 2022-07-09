#pragma once

#include <stdint.h>
#include <cstdint>
#include <TypeDef.h>

namespace Regexs
{
	class REGEX_X_API objectForRegex
	{
	public:
		objectForRegex()
		{
		}

		virtual ~objectForRegex()
		{
		}
	};

	struct PtrObjectForRegex
	{
		std::uintptr_t	_address;
		objectForRegex* object;

		explicit PtrObjectForRegex(objectForRegex* object) : object(object)
		{
			_address = reinterpret_cast<std::uintptr_t>(object);
		}
	};

}
