#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <stdexcept>

#include "ICollection.h"
#include "objectForRegex.h"


namespace Regexs
{
	class Match;
	class Group;
	class Capture;
	class GroupEnumerator;


	//Copy type
	class REGEX_X_API ArrayGroup : public objectForRegex
	{
	public:
		std::vector<Group*> Data;
		ArrayGroup(int size)
		{
			Data = std::vector<Group*>(size);
		}
		ArrayGroup() {}
	};

	/// <devdoc>
	///    <para>
	///       Represents a sequence of capture substrings. The object is used
	///       to return the set of captures done by a single capturing group.
	///    </para>
	/// </devdoc>
	class REGEX_X_API GroupCollection : public ICollection, public objectForRegex
	{
	public:
		Match* _match;
		std::unordered_map<int, int> _captureMap;

		// cache of Group objects fed to the user
		std::vector<Group*> _groups;

		/*
		 * Nonpublic constructor
		 */
		virtual ~GroupCollection();


		GroupCollection(Match* match, std::unordered_map<int, int>& caps);


		__declspec(property(get = getIsSynchronized)) bool IsSynchronized;
		__declspec(property(get = getIsReadOnly)) bool IsReadOnly;
		__declspec(property(get = getCount)) int Count;
		__declspec(property(get = getSyncRoot)) objectForRegex* SyncRoot;


		bool TryGetValue(const std::wstring& key, Group*& value);

		/*
		 * The object on which to synchronize
		 */
		objectForRegex* getSyncRoot() override;

		/*
		 * ICollection
		 */
		bool getIsSynchronized() override;

		/*
		 * ICollection
		 */
		bool getIsReadOnly() const;

		/// <devdoc>
		///    <para>
		///       Returns the number of groups.
		///    </para>
		/// </devdoc>
		int getCount() override;

		Group* operator [](int groupnum);

		Group* operator [](const std::wstring& groupname);

		Group* GetGroup(int groupnum);


		/*
		 * Caches the group objects
		 */
		Group* GetGroupImpl(int groupnum);

		/// <devdoc>
		///    <para>
		///       Copies all the elements of the collection to the given array
		///       beginning at the given index.
		///    </para>
		/// </devdoc>
		void CopyTo(void* array, int arrayIndex) override;

		/// <devdoc>
		///    <para>
		///       Provides an enumerator in the same order as Item[].
		///    </para>
		/// </devdoc>
		IEnumerator* GetEnumerator() override;

		friend class Group;
	};


	/*
	 * This non-public enumerator lists all the captures
	 * Should it be public?
	 */
	class REGEX_X_API GroupEnumerator : public IEnumerator, public objectForRegex
	{
	public:
		GroupCollection* _rgc;
		int _curindex;

		/*
		 * Nonpublic constructor
		 */
		virtual ~GroupEnumerator()
		{
		}

		GroupEnumerator(GroupCollection* rgc);

		__declspec(property(get = getCurrent)) objectForRegex* Current;

		/*
		 * As required by IEnumerator
		 */
		bool MoveNext() override;

		/*
		 * As required by IEnumerator
		 */
		objectForRegex* getCurrent() override;

		/*
		 * Returns the current capture
		 */
		Capture* getCapture() const;

		/*
		 * Reset to before the first item
		 */
		void Reset();
	};
}
