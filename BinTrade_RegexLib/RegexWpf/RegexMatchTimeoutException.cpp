#include "RegexMatchTimeoutException.h"

namespace Regexs
{
	RegexMatchTimeoutException::RegexMatchTimeoutException(const std::wstring& regexInput, const std::wstring& regexPattern, TimeSpan matchTimeout) : TimeoutException("RegexMatchTimeoutException_Occurred")
	{
		Init(regexInput, regexPattern, matchTimeout);
	}

	RegexMatchTimeoutException::RegexMatchTimeoutException() : TimeoutException()
	{
		Init();
	}

	RegexMatchTimeoutException::RegexMatchTimeoutException(const std::wstring& message) : TimeoutException(std::string(message.begin(), message.end()))
	{
		Init();
	}

	RegexMatchTimeoutException::RegexMatchTimeoutException(const std::wstring& message, std::runtime_error inner) : TimeoutException(std::string(message.begin(), message.end()), inner.what())
	{
		Init();
	}

	void RegexMatchTimeoutException::Init()
	{
		Init(L"", L"", TimeSpan::FromTicks(-1));
	}

	void RegexMatchTimeoutException::Init(const std::wstring& input, const std::wstring& pattern, TimeSpan timeout)
	{
		this->regexInput = input;
		this->regexPattern = pattern;
		this->matchTimeout = timeout;
	}

	std::wstring RegexMatchTimeoutException::getPattern() const
	{
		return regexPattern;
	}

	std::wstring RegexMatchTimeoutException::getInput() const
	{
		return regexInput;
	}

	TimeSpan RegexMatchTimeoutException::getMatchTimeout() const
	{
		return matchTimeout;
	}
}
