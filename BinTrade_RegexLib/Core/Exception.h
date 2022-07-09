#pragma once

#include <string>

namespace Regexs
{

	class ArgumentOutOfRangeException : public std::exception
	{
	private:
		std::string msg;

	public:
		ArgumentOutOfRangeException(const std::string& message = "") : msg(message)
		{
		}

		const char* what() const noexcept
		{
			return msg.c_str();
		}
	};

	class NotSupportedException : public std::exception
	{
	private:
		std::string msg;

	public:
		NotSupportedException(const std::string& message = "") : msg(message)
		{
		}

		const char* what() const noexcept
		{
			return msg.c_str();
		}
	};

	class InvalidOperationException : public std::exception
	{
	private:
		std::string msg;

	public:
		InvalidOperationException(const std::string& message = "") : msg(message)
		{
		}

		const char* what() const noexcept
		{
			return msg.c_str();
		}
	};

	class NotImplementedException : public std::exception
	{
	private:
		std::string msg;

	public:
		NotImplementedException(const std::string& message = "") : msg(message)
		{
		}

		const char* what() const noexcept
		{
			return msg.c_str();
		}
	};

	class ArrayTypeMismatchException : public std::exception
	{
	private:
		std::string msg;

	public:
		ArrayTypeMismatchException(const std::string& message = "") : msg(message)
		{
		}

		const char* what() const noexcept
		{
			return msg.c_str();
		}
	};

	class ArgumentException : public std::exception
	{
	private:
		std::string msg;

	public:
		ArgumentException(const std::string& message = "") : msg(message)
		{
		}

		const char* what() const noexcept
		{
			return msg.c_str();
		}
	};

	class ArgumentNullException : public std::exception
	{
	private:
		std::string msg;

	public:
		ArgumentNullException(const std::string& message = "") : msg(message)
		{
		}

		const char* what() const noexcept
		{
			return msg.c_str();
		}
	};

	class IndexOutOfRangeException : public std::exception
	{
	private:
		std::string msg;

	public:
		IndexOutOfRangeException(const std::string& message = "") : msg(message)
		{
		}

		const char* what() const noexcept
		{
			return msg.c_str();
		}
	};

	
	class TimeoutException : public std::exception
	{
	private:
		std::string msg;

	public:
		TimeoutException(const std::string& message = "") : msg(message)
		{
		}

		TimeoutException(const std::string& message , const char* inner) : msg(message)
		{
		}

		const char* what() const noexcept
		{
			return msg.c_str();
		}
	};

	


}
