#pragma once
#ifndef _UtilsCollections_H_
#define _UtilsCollections_H_


#include <vector>

#include "Exception.h"
#include "IComparer.h"


class Int_Sort : public Regexs::IComparer<int>
{
	int Compare(int r1, int r2) override
	{
		return CompareTo(r1, r2);
	}

	static int CompareTo(int val1, int val2)
	{
		if (val1 < val2)
		{
			return -1;
		}
		if (val1 > val2)
		{
			return 1;
		}
		return 0;
	}
};

class UtilsCollections
{
public:
	//https://referencesource.microsoft.com/#mscorlib/system/collections/generic/list.cs,e455396b6f564c3f
	//https://github.com/0xFireball/NativeSharp/blob/7e530a810983e941e0765f3e10875f52a246d3d4/mscorlib/src/System/Collections/Generic/MangoArraySortHelper.cs
	template <class T>
	static int BinarySearch(std::vector<T> arrays, int index, int length, T value, Regexs::IComparer<T>* comparer)
	{
		if (index < 0)
			throw Regexs::ArgumentOutOfRangeException("index");
		if (length < 0)
			throw Regexs::ArgumentOutOfRangeException("length");
		if (!(index >= 0 && length >= 0 && ((int)arrays.size() - index >= length)))
			throw Regexs::InvalidOperationException("Check the arguments in the caller!");
		int lo = index;
		int hi = index + length - 1;
		while (lo <= hi)
		{
			int i = lo + ((hi - lo) >> 1);
			int order;
			try
			{
				order = comparer->Compare(arrays[i], value);
			}
			catch (std::exception)
			{
				throw  Regexs::InvalidOperationException();
			}

			if (order == 0) return i;
			if (order < 0)
			{
				lo = i + 1;
			}
			else
			{
				hi = i - 1;
			}
		}

		return ~lo;
	}


	//https://github.com/0xFireball/NativeSharp/blob/7e530a810983e941e0765f3e10875f52a246d3d4/mscorlib/src/System/Collections/Generic/MangoArraySortHelper.cs
	//https://referencesource.microsoft.com/#mscorlib/system/collections/generic/list.cs,e455396b6f564c3f
	template <class T>
	static void Sort(std::vector<T>* arrays, Regexs::IComparer<T>* comparer)
	{
		if (comparer == NULL)
		{
			throw Regexs::ArgumentNullException("match");
		}

		//https://referencesource.microsoft.com/#mscorlib/system/array.cs,54496ee33e3b155a
		//Sort(array, index, length, comparer);


		if (arrays->size() > 0)
		{
			Sort(arrays, 0, arrays->size(), comparer);
		}
	}

	template <class T>
	static void Sort(std::vector<T>* items, int index, int length, Regexs::IComparer<T>* comparer)
	{
		SortPri(items, (std::vector<int>*)NULL, index, length, comparer);
	}

private:
	template <class T, class TValue>
	static void SortPri(std::vector<T>* keys, std::vector<TValue>* values, int index, int length, Regexs::IComparer<T>* comparer)
	{
		//BCLDebug.Assert(keys != null, "Check the arguments in the caller!");
		//BCLDebug.Assert(index >= 0 && length >= 0 && (keys.Length - index >= length), "Check the arguments in the caller!");

		//??? Comparer<T>.Default no implement
		//if (comparer == NULL || comparer == Comparer<T>.Default) {
		//	comparer = Comparer<T>.Default;
		//}

		QuickSort(keys, values, index, index + (length - 1), comparer);
	}

	template <class T, class TValue>
	static void QuickSort(std::vector<T>* keys, std::vector<TValue>* values, int left, int right, Regexs::IComparer<T>* comparer)
	{
		if (keys->size() > 0)
		{
			do
			{
				int i = left;
				int j = right;
				T x = keys->at(i + ((j - i) >> 1));
				do
				{
					// Add a try block here to detect IComparers (or their
					// underlying IComparables, etc) that are bogus.
					try
					{
						while (comparer->Compare(keys->at(i), x) < 0) i++;
						while (comparer->Compare(x, keys->at(j)) < 0) j--;
					}
					catch (Regexs::IndexOutOfRangeException)
					{
						throw  Regexs::ArgumentException("keys");
					}
					catch (std::exception)
					{
						throw Regexs::InvalidOperationException();
					}
					//BCLDebug.Assert(i >= left && j <= right, "(i>=left && j<=right)  Sort failed - Is your IComparer bogus?");
					if (i > j) break;
					if (i < j)
					{
						T key = keys->at(i);
						keys->at(i) = keys->at(j);
						keys->at(j) = key;
						if (values != NULL)
						{
							TValue value = values->at(i);
							values->at(i) = values->at(j);
							values->at(j) = value;
						}
					}
					i++;
					j--;
				}
				while (i <= j);
				if (j - left <= right - i)
				{
					if (left < j) QuickSort(keys, values, left, j, comparer);
					left = i;
				}
				else
				{
					if (i < right) QuickSort(keys, values, i, right, comparer);
					right = j;
				}
			}
			while (left < right);
		}
	}
};

#endif
