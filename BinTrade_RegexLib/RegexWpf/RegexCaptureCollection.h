#pragma once

#include <vector>

#include "ICollection.h"
#include "objectForRegex.h"

// The CaptureCollection lists the captured Capture numbers
// contained in a compiled Regex.


namespace Regexs
{
	class Group;
	class Capture;
	class CaptureEnumerator;


	//Copy type
	class REGEX_X_API ArrayCapture : public objectForRegex
	{
	public:
		std::vector<Capture*> Data;
		ArrayCapture(int size)
		{
			Data = std::vector<Capture*>(size);
		}
		ArrayCapture(){}
	};

	/*
	 * This collection returns the Captures for a group
	 * in the order in which they were matched (left to right
	 * or right to left). It is created by Group.Captures
	 */
	/// <devdoc>
	///    <para>
	///       Represents a sequence of capture substrings. The object is used
	///       to return the set of captures done by a single capturing group.
	///    </para>
	/// </devdoc>
	class REGEX_X_API CaptureCollection : public ICollection, public objectForRegex
	{
	public:
		Group* _group;
		int _capcount;
		std::vector<Capture*> _captures;

		/*
		 * Nonpublic constructor
		 */
		virtual ~CaptureCollection();

		CaptureCollection(Group* group);

		__declspec(property(get = getSyncRoot)) objectForRegex* SyncRoot;
		__declspec(property(get = getIsSynchronized)) bool IsSynchronized;
		__declspec(property(get = getIsReadOnly)) bool IsReadOnly;
		__declspec(property(get = getCount)) int Count;


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

		/*
		 * The number of captures for the group
		 */
		/// <devdoc>
		///    <para>
		///       Returns the number of captures.
		///    </para>
		/// </devdoc>
		int getCount() override;

		/*
		 * The ith capture in the group
		 */
		/// <devdoc>
		///    <para>
		///       Provides a means of accessing a specific capture in the collection.
		///    </para>
		/// </devdoc>
		Capture* operator [](int i);

		/*
		 * As required by ICollection
		 */
		/// <devdoc>
		///    <para>
		///       Copies all the elements of the collection to the given array
		///       beginning at the given index.
		///    </para>
		/// </devdoc>
		void CopyTo(void* array, int arrayIndex) override;

		/*
		 * As required by ICollection
		 */
		/// <devdoc>
		///    <para>
		///       Provides an enumerator in the same order as Item[].
		///    </para>
		/// </devdoc>
		IEnumerator* GetEnumerator() override;

		/*
		 * Nonpublic code to return set of captures for the group
		 */
		Capture* GetCapture(int i);

		friend class Group;
	};


	/*
	 * This non-public enumerator lists all the captures
	 * Should it be public?
	 */
	class CaptureEnumerator : public IEnumerator, public objectForRegex
	{
	public:
		CaptureCollection* _rcc;
		int _curindex;

		/*
		 * Nonpublic constructor
		 */
		virtual ~CaptureEnumerator();

		CaptureEnumerator(CaptureCollection* rcc);

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
		Regexs::Capture* getCapture() const;

		/*
		 * Reset to before the first item
		 */
		void Reset() override;
	};
}
