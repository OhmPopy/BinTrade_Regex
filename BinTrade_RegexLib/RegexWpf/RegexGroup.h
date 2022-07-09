#pragma once

#include "RegexCapture.h"
#include <string>
#include <vector>
#include <stdexcept>


// Group represents the substring or substrings that
// are captured by a single capturing group after one
// regular expression match.


namespace Regexs
{
	class CaptureCollection;

	/// <devdoc>
	///    Group 
	///       represents the results from a single capturing group. A capturing group can
	///       capture zero, one, or more strings in a single match because of quantifiers, so
	///       Group supplies a collection of Capture objects. 
	///    </devdoc>
	class REGEX_X_API Group : public Capture
	{
		Group(const std::wstring* text, const std::vector<int>& caps, int capcount, bool is_static_init);
	public:
		// the empty group object
		static Group* _emptygroup;

		std::vector<int> _caps;
		int _capcount;
		CaptureCollection* _capcoll;

		virtual ~Group();

		Group(const std::wstring* text, const std::vector<int>& caps, int capcount);
		


		__declspec(property(get = getSuccess)) bool Success;
		__declspec(property(get = getCaptures)) CaptureCollection* Captures;

		/*
		 * True if the match was successful
		 */
		/// <devdoc>
		///    <para>Indicates whether the match is successful.</para>
		/// </devdoc>
		bool getSuccess() const;

		/*
		 * The collection of all captures for this group
		 */
		/// <devdoc>
		///    <para>
		///       Returns a collection of all the captures matched by the capturing
		///       group, in innermost-leftmost-first order (or innermost-rightmost-first order if
		///       compiled with the "r" option). The collection may have zero or more items.
		///    </para>
		/// </devdoc>
		CaptureCollection* getCaptures();

		/*
		 * Convert to a thread-safe object by precomputing cache contents
		 */
		/// <devdoc>
		///    <para>Returns 
		///       a Group object equivalent to the one supplied that is safe to share between
		///       multiple threads.</para>
		/// </devdoc>
		static Group* Synchronized(Group* inner);


		friend class CaptureCollection;
	};
}
