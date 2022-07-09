#pragma once

#include <string>

#include "objectForRegex.h"

// Capture is just a location/length pair that indicates the
// location of a regular expression match. A single regexp
// search may return multiple Capture within each capturing
// RegexGroup.


namespace Regexs
{
	/// <devdoc>
	///    <para> 
	///       Represents the results from a single subexpression capture. The object represents
	///       one substring for a single successful capture.</para>
	/// </devdoc>
	class REGEX_X_API Capture : public objectForRegex
	{
	protected:
		const std::wstring* _text;
		int _index;
		int _length;

	public:
		Capture(const std::wstring* text, int i, int l);

		virtual ~Capture()
		{
		}

		__declspec(property(get = getValue)) std::wstring Value;

		__declspec(property(get = getIndex)) int Index;

		__declspec(property(get = getLength)) int Length;

		/*
		 * The index of the beginning of the matched capture
		 */
		/// <devdoc>
		///    <para>Returns the position in the original string where the first character of
		///       captured substring was found.</para>
		/// </devdoc>
		int getIndex() const;

		/*
		 * The length of the matched capture
		 */
		/// <devdoc>
		///    <para>
		///       Returns the length of the captured substring.
		///    </para>
		/// </devdoc>
		int getLength() const;

		std::wstring getValue() const;

		/*
		 * The capture as a string
		 */
		/// <devdoc>
		///    <para>
		///       Returns 
		///          the substring that was matched.
		///       </para>
		///    </devdoc>
		std::wstring ToString() const;

		/*
		 * The original string
		 */
		std::wstring GetOriginalString() const;

		/*
		 * The substring to the left of the capture
		 */
		std::wstring GetLeftSubstring() const;

		/*
		 * The substring to the right of the capture
		 */
		std::wstring GetRightSubstring() const;

		std::wstring Description() const;
	};
}
