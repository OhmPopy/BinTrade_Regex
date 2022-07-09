#include "RegexBoyerMoore.h"

#include <cassert>

#include "CultureInfo.h"
#include "Regex.h"
#include "stringbuilder.h"
#include "UtilsString.h"

namespace Regexs
{
	RegexBoyerMoore::~RegexBoyerMoore()
	{
	}

	RegexBoyerMoore::RegexBoyerMoore(const std::wstring& input_pattern, bool caseInsensitive, bool rightToLeft, ComCultureInfo* culture):
		_culture(NULL), _lowASCII(0), _highASCII(0), _rightToLeft(false), _caseInsensitive(false)
	{
		std::wstring pattern = input_pattern;
		/*
		 * Sorry,  you just can't use Boyer-Moore to find an empty pattern.
		 * We're doing this for your own protection. (Really, for speed.)
		 */
		assert((pattern.length() != 0, L"RegexBoyerMoore called with an empty string.  This is bad for perf"));

		int beforefirst;
		int last;
		int bump;
		int examine;
		int scan;
		int match;
		wchar_t ch;

		// We do the ToLower character by character for consistency.  With surrogate chars, doing
		// a ToLower on the entire string could actually change the surrogate pair.  This is more correct
		// linguistically, but since Regex doesn't support surrogates, it's more important to be 
		// consistent. 
		if (caseInsensitive)
		{
			std::wstring sb = pattern;
			for (size_t i = 0; i < pattern.length(); i++)
				sb[i] = UtilsCharInternal::ToLower(pattern[i], culture);
			pattern = sb;
		}

		_pattern = pattern;
		_rightToLeft = rightToLeft;
		_caseInsensitive = caseInsensitive;
		_culture = culture;

		if (!rightToLeft)
		{
			beforefirst = -1;
			last = pattern.length() - 1;
			bump = 1;
		}
		else
		{
			beforefirst = pattern.length();
			last = 0;
			bump = -1;
		}

		/*
		 * PART I - the good-suffix shift table
		 * 
		 * compute the positive requirement:
		 * if char "i" is the first one from the right that doesn't match,
		 * then we know the matcher can advance by _positive[i].
		 *
		 * <STRIP>  This algorithm appears to be a simplified variant of the 
		 *          standard Boyer-Moore good suffix calculation.  It could
		 *          be one of D.M. Sunday's variations, but I have not found which one.
		 * </STRIP>
		 * <
	
	
	*/
		_positive = std::vector<int>(pattern.length());

		examine = last;
		ch = pattern[examine];
		_positive[examine] = bump;
		examine -= bump;

		for (;;)
		{
			// find an internal char (examine) that matches the tail

			for (;;)
			{
				if (examine == beforefirst)
				{
					goto OuterloopBreak;
				}
				if (pattern[examine] == ch)
				{
					break;
				}
				examine -= bump;
			}

			match = last;
			scan = examine;

			// find the length of the match

			for (;;)
			{
				if (scan == beforefirst || pattern[match] != pattern[scan])
				{
					// at the end of the match, note the difference in _positive
					// this is not the length of the match, but the distance from the internal match
					// to the tail suffix. 
					if (_positive[match] == 0)
					{
						_positive[match] = match - scan;
					}

					// System.Diagnostics.Debug.WriteLine("Set positive[" + match + "] to " + (match - scan));

					break;
				}

				scan -= bump;
				match -= bump;
			}

			examine -= bump;
		}

	OuterloopBreak:

		match = last - bump;

		// scan for the chars for which there are no shifts that yield a different candidate

		/* <STRIP>
		 *  The inside of the if statement used to say 
		 *  "_positive[match] = last - beforefirst;"
		 *  I've changed it to the below code.  This
		 *  is slightly less agressive in how much we skip, but at worst it 
		 *  should mean a little more work rather than skipping a potential
		 *  match.
		 * </STRIP>
		 */
		while (match != beforefirst)
		{
			if (_positive[match] == 0)
			{
				_positive[match] = bump;
			}

			match -= bump;
		}

		//System.Diagnostics.Debug.WriteLine("good suffix shift table:");
		//for (int i=0; i<_positive.Length; i++)
		//    System.Diagnostics.Debug.WriteLine("\t_positive[" + i + "] = " + _positive[i]);


		/*
		 * PART II - the bad-character shift table
		 * 
		 * compute the negative requirement:
		 * if char "ch" is the reject character when testing position "i",
		 * we can slide up by _negative[ch];
		 * (_negative[ch] = str.Length - 1 - str.LastIndexOf(ch))
		 *
		 * the lookup table is divided into ASCII and Unicode portions;
		 * only those parts of the Unicode 16-bit code set that actually
		 * appear in the string are in the table. (Maximum size with
		 * Unicode is 65K; ASCII only case is 512 bytes.)
		 */

		_negativeASCII = std::vector<int>(128);

		for (int i = 0; i < 128; i++)
		{
			_negativeASCII[i] = last - beforefirst;
		}

		_lowASCII = 127;
		_highASCII = 0;

		for (examine = last; examine != beforefirst; examine -= bump)
		{
			ch = pattern[examine];

			if (ch < 128)
			{
				if (_lowASCII > ch)
				{
					_lowASCII = ch;
				}

				if (_highASCII < ch)
				{
					_highASCII = ch;
				}

				if (_negativeASCII[ch] == last - beforefirst)
				{
					_negativeASCII[ch] = last - examine;
				}
			}
			else
			{
				int i = ch >> 8;
				int j = ch & 0xFF;

				if (_negativeUnicode.empty())
				{
					_negativeUnicode = std::vector<std::vector<int>>(256);
				}

				if (_negativeUnicode[i].empty())
				{
					std::vector<int> newarray(256);

					for (int k = 0; k < 256; k++)
					{
						newarray[k] = last - beforefirst;
					}

					if (i == 0)
					{
						std::copy_n(_negativeASCII.begin(), 128, newarray.begin());
						_negativeASCII = newarray;
					}

					_negativeUnicode[i] = newarray;
				}

				if (_negativeUnicode[i][j] == last - beforefirst)
				{
					_negativeUnicode[i][j] = last - examine;
				}
			}
		}
	}

	bool RegexBoyerMoore::MatchPattern(const std::wstring* text, int index)
	{
		if (_caseInsensitive)
		{
			if (text->length() - index < _pattern.length())
			{
				return false;
			}

			TextInfo* textinfo = new TextInfo(_culture);
			for (size_t i = 0; i < _pattern.length(); i++)
			{
				assert((textinfo->ToLower(_pattern[i]) == _pattern[i], L"pattern should be converted to lower case in constructor!"));
				if (textinfo->ToLower(text->at(index + i)) != _pattern[i])
				{
					return false;
				}
			}

			return true;
		}
		else
		{
			return (0 == UtilsString::CompareOrdinal(_pattern, 0, *text, index, _pattern.length()));
		}
	}

	bool RegexBoyerMoore::IsMatch(const std::wstring* text, int index, int beglimit, int endlimit)
	{
		if (!_rightToLeft)
		{
			if (index < beglimit || endlimit - index < (int)_pattern.length())
			{
				return false;
			}

			return MatchPattern(text, index);
		}
		else
		{
			if (index > endlimit || index - beglimit < (int)_pattern.length())
			{
				return false;
			}

			return MatchPattern(text, index - _pattern.length());
		}
	}

	int RegexBoyerMoore::Scan(const std::wstring* text, int index, int beglimit, int endlimit)
	{
		int test;
		int test2;
		int match;
		int startmatch;
		int endmatch;
		int advance;
		int defadv;
		int bump;
		wchar_t chMatch;
		wchar_t chTest;
		std::vector<int> unicodeLookup;

		if (!_rightToLeft)
		{
			defadv = _pattern.length();
			startmatch = _pattern.length() - 1;
			endmatch = 0;
			test = index + defadv - 1;
			bump = 1;
		}
		else
		{
			defadv = _pattern.length();
			defadv = -defadv;
			startmatch = 0;
			endmatch = -defadv - 1;
			test = index + defadv;
			bump = -1;
		}

		chMatch = _pattern[startmatch];

		for (;;)
		{
			if (test >= endlimit || test < beglimit)
			{
				return -1;
			}

			chTest = text->at(test);

			if (_caseInsensitive)
			{
				chTest = UtilsCharInternal::ToLower(chTest, _culture);
			}

			if (chTest != chMatch)
			{
				if (chTest < 128)
				{
					advance = _negativeASCII[chTest];
				}
				else if (!_negativeUnicode.empty() && (! (unicodeLookup = _negativeUnicode[chTest >> 8]).empty()))
				{
					advance = unicodeLookup[chTest & 0xFF];
				}
				else
				{
					advance = defadv;
				}

				test += advance;
			}
			else
			{
				// if (chTest == chMatch)
				test2 = test;
				match = startmatch;

				for (;;)
				{
					if (match == endmatch)
					{
						return (_rightToLeft ? test2 + 1 : test2);
					}

					match -= bump;
					test2 -= bump;

					chTest = text->at(test2);

					if (_caseInsensitive)
					{
						chTest = UtilsCharInternal::ToLower(chTest, _culture);
					}

					if (chTest != _pattern[match])
					{
						advance = _positive[match];
						if ((chTest & 0xFF80) == 0)
						{
							test2 = (match - startmatch) + _negativeASCII[chTest];
						}
						else if (! _negativeUnicode.empty() && (!(unicodeLookup = _negativeUnicode[chTest >> 8]).empty()))
						{
							test2 = (match - startmatch) + unicodeLookup[chTest & 0xFF];
						}
						else
						{
							test += advance;
							break;
						}

						if (_rightToLeft ? test2 < advance : test2 > advance)
						{
							advance = test2;
						}

						test += advance;
						break;
					}
				}
			}
		}
	}

	std::wstring RegexBoyerMoore::ToString()
	{
		return _pattern;
	}

	std::wstring RegexBoyerMoore::Dump(std::wstring indent)
	{
		StringBuilder sb = StringBuilder();

		sb.Append(indent + L"BM Pattern: " + _pattern + L"\n");
		sb.Append(indent + L"Positive: ");
		for (size_t i = 0; i < _positive.size(); i++)
		{
			sb.Append(std::to_wstring(_positive[i]) + L" ");
		}
		sb.Append(L"\n");

		if (!_negativeASCII.empty())
		{
			sb.Append(indent + L"Negative table\n");
			for (size_t i = 0; i < _negativeASCII.size(); i++)
			{
				if (_negativeASCII[i] != _pattern.size())
				{
					sb.Append(indent + L"  " + Regex::Escape(std::to_wstring(i)) + L" " + std::to_wstring(_negativeASCII[i]) + L"\n");
				}
			}
		}

		return sb.ToString();
	}
}
