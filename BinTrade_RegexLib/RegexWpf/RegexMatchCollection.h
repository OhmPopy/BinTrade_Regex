#pragma once

#include <string>
#include <vector>
#include <stdexcept>

#include "ICollection.h"
#include "objectForRegex.h"

// The MatchCollection lists the successful matches that
// result when searching a string for a regular expression.


namespace Regexs
{
	class Regex;
	class Match;
	class MatchEnumerator;

	//Copy type
	class REGEX_X_API ArrayMatch : public objectForRegex
	{
	public:
		std::vector<Match*> Data;
		ArrayMatch(int size)
		{
			Data = std::vector<Match*>(size);
		}
		ArrayMatch() {}
	};

	/*
	 * This collection returns a sequence of successful match results, either
	 * from GetMatchCollection() or GetExecuteCollection(). It stops when the
	 * first failure is encountered (it does not return the failed match).
	 */
	/// <devdoc>
	///    <para>
	///       Represents the set of names appearing as capturing group
	///       names in a regular expression.
	///    </para>
	/// </devdoc>
	class REGEX_X_API MatchCollection : public ICollection, public objectForRegex
	{
	public:
		Regex* _regex;
		std::vector<Match*> _matches;
		bool _done;
		const std::wstring *_input;
		int _beginning;
		int _length;
		int _startat;
		int _prevlen;

	private:
		static int infinite;

	public:
		/*
		 * Nonpublic constructor
		 */
		virtual ~MatchCollection();

		MatchCollection(Regex* regex, const std::wstring* input, int beginning, int length, int startat);

		Match* GetMatch(int i);

		__declspec(property(get = getIsSynchronized)) bool IsSynchronized;
		__declspec(property(get = getIsReadOnly)) bool IsReadOnly;
		__declspec(property(get = getCount)) int Count;
		__declspec(property(get = getSyncRoot)) objectForRegex* SyncRoot;


		/// <devdoc>
		///    <para>
		///       Returns the number of captures.
		///    </para>
		/// </devdoc>
		int getCount() override;

		objectForRegex* getSyncRoot() override;

		bool getIsSynchronized() override;

		bool getIsReadOnly() const;

		/// <devdoc>
		///    <para>
		///       Returns the ith Match in the collection.
		///    </para>
		/// </devdoc>
		virtual Match* operator [](int i);

		/// <devdoc>
		///    <para>
		///       Copies all the elements of the collection to the given array
		///       starting at the given index.
		///    </para>
		/// </devdoc>
		void CopyTo(void* array, int arrayIndex) override;

		/// <devdoc>
		///    <para>
		///       Provides an enumerator in the same order as Item[i].
		///    </para>
		/// </devdoc>
		IEnumerator* GetEnumerator();

		friend class Match;
	};

	/*
	 * This non-public enumerator lists all the group matches.
	 * Should it be public?
	 */
	class REGEX_X_API MatchEnumerator : public IEnumerator, public objectForRegex
	{
	public:
		MatchCollection* _matchcoll;
		Match* _match;
		int _curindex;
		bool _done;

		/*
		 * Nonpublic constructor
		 */
		virtual ~MatchEnumerator();

		MatchEnumerator(MatchCollection* matchcoll);

		__declspec(property(get = getCurrent)) objectForRegex* Current;

		/*
		 * Advance to the next match
		 */
		bool MoveNext() override;

		/*
		 * The current match
		 */
		objectForRegex* getCurrent() override;

		/*
		 * Position before the first item
		 */
		void Reset() override;

		friend class Match;
	};
}
