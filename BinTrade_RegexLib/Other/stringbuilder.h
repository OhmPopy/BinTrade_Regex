#pragma once

#include <string>

class StringBuilder
{
private:
	std::wstring privateString;

public:
	StringBuilder()
	{
	}

	StringBuilder(const std::wstring& initialString)
	{
		privateString = initialString;
	}

	StringBuilder(std::size_t capacity)
	{
		EnsureCapacity(capacity);
	}

	StringBuilder(const std::wstring& initialString, std::size_t capacity)
	{
		privateString = initialString;
		EnsureCapacity(capacity);
	}

	wchar_t operator [](std::size_t index)
	{
		return privateString[index];
	}

	StringBuilder* Append(const std::wstring& toAppend)
	{
		privateString += toAppend;
		return this;
	}

	StringBuilder* Append(const std::wstring& toAppend, int startIndex, int count)
	{
		privateString += toAppend.substr(startIndex, count);
		return this;
	}

	StringBuilder* AppendLine()
	{
		privateString += L"\r\n";
		return this;
	}

	StringBuilder* AppendLine(const std::wstring& toAppend)
	{
		privateString += toAppend + L"\r\n";
		return this;
	}

	StringBuilder* Insert(std::size_t position, const std::wstring& toInsert)
	{
		privateString.insert(position, toInsert);
		return this;
	}

	StringBuilder* Modify(std::size_t position, wchar_t value)
	{
		privateString[position] = value;
		return this;
	}


	std::wstring ToString() const
	{
		return privateString;
	}

	std::wstring ToString(std::size_t start, std::size_t length) const
	{
		return privateString.substr(start, length);
	}

	std::size_t GetLength() const
	{
		return privateString.length();
	}

	void setLength(std::size_t newLength)
	{
		privateString.resize(newLength);
	}

	std::size_t Capacity() const
	{
		return privateString.capacity();
	}

	void EnsureCapacity(std::size_t minimumCapacity)
	{
		privateString.reserve(minimumCapacity);
	}

	std::size_t MaxCapacity() const
	{
		return privateString.max_size();
	}

	void Clear()
	{
		privateString.clear();
	}

	StringBuilder* Remove(std::size_t start, std::size_t length)
	{
		privateString.erase(start, length);
		return this;
	}

	StringBuilder* Replace(const std::wstring& oldString, const std::wstring& newString)
	{
		std::size_t pos = 0;
		while ((pos = privateString.find(oldString, pos)) != std::wstring::npos)
		{
			privateString.replace(pos, oldString.length(), newString);
			pos += newString.length();
		}
		return this;
	}

	StringBuilder* AppendChar(wchar_t flags)
	{
		privateString.resize(privateString.length() + 1);
		privateString[privateString.length() - 1] = flags;
		return this;
	}
};
