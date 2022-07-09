#include "RegexCapture.h"

#include "stringbuilder.h"


namespace Regexs
{
	Capture::Capture(const std::wstring* text, int i, int l): _text(text)
	{
		_index = i;
		_length = l;
	}

	int Capture::getIndex() const
	{
		return _index;
	}

	int Capture::getLength() const
	{
		return _length;
	}

	std::wstring Capture::getValue() const
	{
		return _text->substr(_index, _length);
	}

	std::wstring Capture::ToString() const
	{
		return getValue();
	}

	std::wstring Capture::GetOriginalString() const
	{
		if (_text != NULL)
			return *_text;
		return L"";
	}

	std::wstring Capture::GetLeftSubstring() const
	{
		return _text->substr(0, _index);
	}

	std::wstring Capture::GetRightSubstring() const
	{
		return _text->substr(_index + _length, _text->length() - _index - _length);
	}

	std::wstring Capture::Description() const
	{
		StringBuilder Sb = StringBuilder();

		Sb.Append(L"(I = ");
		Sb.Append(std::to_wstring(_index));
		Sb.Append(L", L = ");
		Sb.Append(std::to_wstring(_length));
		Sb.Append(L"): ");
		Sb.Append(*_text, _index, _length);

		return Sb.ToString();
	}
}
