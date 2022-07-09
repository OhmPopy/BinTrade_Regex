#pragma once

#include "RegexRunner.h"

// This RegexRunnerFactory class is a base class for compiled regex code.
// we need to compile a factory because Type.CreateInstance is much slower
// than calling the constructor directly.


namespace Regexs
{
	class REGEX_X_API RegexRunnerFactory : public objectForRegex
	{
	public:
		virtual ~RegexRunnerFactory(){}

		virtual RegexRunner* CreateInstance() = 0;
	protected:
		RegexRunnerFactory();
	};
}
