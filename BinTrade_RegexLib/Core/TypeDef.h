#pragma once
#include <functional>
#include <string>

namespace Regexs
{
	class Match;

	/*
	 * Callback class
	 */
	/// <devdoc>
	/// </devdoc>
	typedef std::function<std::wstring(Match* match)> MatchEvaluator;

	typedef unsigned char Byte;


	class WeakReference
	{
		
	};

#define SAFE_DELETE(x) {if((x)!=NULL){ delete (x); (x) = NULL;}}

#ifdef REGEXNETLIB
#ifdef REGEXNETLIB_EXPORTS
#define REGEX_X_API __declspec(dllexport)
#else
#define REGEX_X_API __declspec(dllimport)
#endif
#else
#define REGEX_X_API
#endif
}
