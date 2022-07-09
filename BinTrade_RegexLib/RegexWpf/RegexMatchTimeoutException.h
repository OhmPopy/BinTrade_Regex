#pragma once

#include <string>
#include <stdexcept>

#include "Exception.h"
#include "TimeSpan.h"


namespace Regexs
{
	/// <summary>
	/// This is the exception that is thrown when a RegEx matching timeout occurs.
	/// </summary>

	class REGEX_X_API RegexMatchTimeoutException : public  TimeoutException
	{
	private:
		std::wstring regexInput = L"";

		std::wstring regexPattern = L"";

		TimeSpan matchTimeout = TimeSpan::FromTicks(-1);

	public:
		/// <summary>
		/// This is the preferred constructor to use.
		/// The other constructors are provided for compliance to Fx design guidelines.
		/// </summary>
		/// <param name="regexInput">Matching timeout occured during mathing within the specified input.</param>
		/// <param name="regexPattern">Matching timeout occured during mathing to the specified pattern.</param>
		/// <param name="matchTimeout">Matching timeout occured becasue matching took longer than the specified timeout.</param>
		RegexMatchTimeoutException(const std::wstring& regexInput, const std::wstring& regexPattern, TimeSpan matchTimeout);

		/// <summary>
		/// This constructor is provided in compliance with common NetFx design patterns;
		/// developers should prefer using the constructor
		/// <code>public RegexMatchTimeoutException(string input, string pattern, TimeSpan matchTimeout)</code>.
		/// </summary>    
		RegexMatchTimeoutException();

		/// <summary>
		/// This constructor is provided in compliance with common NetFx design patterns;
		/// developers should prefer using the constructor
		/// <code>public RegexMatchTimeoutException(string input, string pattern, TimeSpan matchTimeout)</code>.
		/// </summary>
		/// <param name="message">The error message that explains the reason for the exception.</param>
		RegexMatchTimeoutException(const std::wstring& message);

		/// <summary>
		/// This constructor is provided in compliance with common NetFx design patterns;
		/// developers should prefer using the constructor
		/// <code>public RegexMatchTimeoutException(string input, string pattern, TimeSpan matchTimeout)</code>.
		/// </summary>
		/// <param name="message">The error message that explains the reason for the exception.</param>
		/// <param name="inner">The exception that is the cause of the current exception, or a <code>null</code>.</param>
		RegexMatchTimeoutException(const std::wstring& message, std::runtime_error inner);


	private:
		void Init();

		void Init(const std::wstring& input, const std::wstring& pattern, TimeSpan timeout);

	public:
		std::wstring getPattern() const;

		std::wstring getInput() const;


		TimeSpan getMatchTimeout() const;
	};  
}  
