#pragma once

#include "UnitTestAssert.h"
#include "vector"


using namespace Regexs;
namespace RegexsTest
{
	class CaptureData
	{
	public:
		CaptureData(std::wstring value, int index, int length, bool createCaptures)
		{
			Value = value;
			Index = index;
			Length = length;

			// Prevent a StackOverflow recursion in the constructor
			if (createCaptures)
			{
				Captures = {CaptureData(value, index, length, false)};
			}
		}

		CaptureData(std::wstring value, int index, int length) : CaptureData(value, index, length, true)
		{
		}

		CaptureData(std::wstring value, int index, int length, std::vector<CaptureData> captures) : CaptureData(value, index, length, false)
		{
			Captures = captures;
		}

		std::wstring Value;
		int Index;
		int Length;
		std::vector<CaptureData> Captures;
	};
}
