#pragma once

namespace Regexs
{
	//c# IComparer
	template <typename T>
	class IComparer
	{
	public:
		virtual ~IComparer()
		{
		}

		//
		// Summary:
		//     Compares two objects and returns a value indicating whether one is less than,
		//     equal to, or greater than the other.
		//
		// Parameters:
		//   x:
		//     The first object to compare.
		//
		//   y:
		//     The second object to compare.
		//
		// Returns:
		//     Value Condition Less than zerox is less than y.Zerox equals y.Greater than zerox
		//     is greater than y.
		virtual int Compare(T x, T y) = 0;
	};
}
