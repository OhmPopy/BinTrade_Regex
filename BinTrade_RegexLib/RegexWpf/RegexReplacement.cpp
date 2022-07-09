#include "RegexReplacement.h"

#include "Exception.h"
#include "Regex.h"
#include "RegexCode.h"
#include "RegexGroupCollection.h"
#include "RegexMatch.h"
#include "RegexNode.h" 

namespace Regexs
{
	RegexReplacement::RegexReplacement(const std::wstring& rep, RegexNode* concat, std::unordered_map<int, int>& _caps)
	{
		std::vector<std::wstring> strings;
		std::vector<int> rules;
		int slot;

		_rep = rep;

		if (concat->Type() != RegexNode::Concatenate)
		{
			throw std::invalid_argument("ReplacementError");
		}

		StringBuilder sb = StringBuilder();
		strings = std::vector<std::wstring>();
		rules = std::vector<int>();

		for (int i = 0; i < concat->ChildCount(); i++)
		{
			RegexNode* child = concat->Child(i);

			int type = child->Type();

			if (type == RegexCode::Multi)
			{
				sb.Append(child->_str);
			}
			else if (type == RegexCode::One)
			{
				sb.AppendChar(child->_ch);
			}
			else if (type == RegexCode::Ref)
			{
				if (sb.GetLength() > 0)
				{
					rules.push_back(strings.size());
					strings.push_back(sb.ToString());
					sb.setLength(0);
				}

				slot = child->_m;

				if (!_caps.empty() && slot >= 0)
				{
					slot = static_cast<int>(_caps[slot]);
				}

				rules.push_back(-Specials - 1 - slot);
			}
			else
			{
				throw std::invalid_argument("ReplacementError");
			}
		}

		if (sb.GetLength() > 0)
		{
			rules.push_back(strings.size());
			strings.push_back(sb.ToString());
		}

		_strings = strings;
		_rules = rules;
	}

	void RegexReplacement::ReplacementImpl(StringBuilder* sb, Match* match)
	{
		for (size_t i = 0; i < _rules.size(); i++)
		{
			int r = _rules[i];
			if (r >= 0) // string lookup
			{
				sb->Append(_strings[r]);
			}
			else if (r < -Specials) // group lookup
			{
				sb->Append(match->GroupToStringImpl(-Specials - 1 - r));
			}
			else
			{
				switch (-Specials - 1 - r)
				{
					// special insertion patterns
				case LeftPortion:
					sb->Append(match->GetLeftSubstring());
					break;
				case RightPortion:
					sb->Append(match->GetRightSubstring());
					break;
				case LastGroup:
					sb->Append(match->LastGroupToStringImpl());
					break;
				case WholeString:
					sb->Append(match->GetOriginalString());
					break;
				default: ;
				}
			}
		}
	}

	void RegexReplacement::ReplacementImplRTL(std::vector<std::wstring>& al, Match* match)
	{
		for (int i = _rules.size() - 1; i >= 0; i--)
		{
			int r = _rules[i];
			if (r >= 0) // string lookup
			{
				al.push_back(_strings[r]);
			}
			else if (r < -Specials) // group lookup
			{
				al.push_back(match->GroupToStringImpl(-Specials - 1 - r));
			}
			else
			{
				switch (-Specials - 1 - r)
				{
					// special insertion patterns
				case LeftPortion:
					al.push_back(match->GetLeftSubstring());
					break;
				case RightPortion:
					al.push_back(match->GetRightSubstring());
					break;
				case LastGroup:
					al.push_back(match->LastGroupToStringImpl());
					break;
				case WholeString:
					al.push_back(match->GetOriginalString());
					break;
				default: ;
				}
			}
		}
	}

	std::wstring RegexReplacement::getPattern() const
	{
		return _rep;
	}

	std::wstring RegexReplacement::Replacement(Match* match)
	{
		StringBuilder sb = StringBuilder();

		ReplacementImpl(&sb, match);

		return sb.ToString();
	}

	std::wstring RegexReplacement::Replace(Regex* regex, const std::wstring& input, int count, int startat)
	{
		if (count < -1)
		{
			throw ArgumentOutOfRangeException("count CountTooSmall");
		}
		if (startat < 0 || (size_t)startat > input.length())
		{
			throw ArgumentOutOfRangeException("startat BeginIndexNotNegative");
		}

		if (count == 0)
		{
			return input;
		}

		Match* match = regex->Match(&input, startat);
		if (!match->getSuccess())
		{
			return input;
		}
		else
		{
			StringBuilder sb;

			if (!regex->getRightToLeft())
			{
				sb = StringBuilder();
				int prevat = 0;

				do
				{
					if (match->getIndex() != prevat)
					{
						sb.Append(input, prevat, match->getIndex() - prevat);
					}

					prevat = match->getIndex() + match->getLength();
					ReplacementImpl(&sb, match);
					if (--count == 0)
					{
						break;
					}

					match = match->NextMatch();
				}
				while (match->getSuccess());

				if ((size_t)prevat < input.length())
				{
					sb.Append(input, prevat, input.length() - prevat);
				}
			}
			else
			{
				std::vector<std::wstring> al;
				int prevat = input.length();

				do
				{
					if (match->getIndex() + match->getLength() != prevat)
					{
						al.push_back(input.substr(match->getIndex() + match->getLength(), prevat - match->getIndex() - match->getLength()));
					}

					prevat = match->getIndex();
					ReplacementImplRTL(al, match);
					if (--count == 0)
					{
						break;
					}

					match = match->NextMatch();
				}
				while (match->getSuccess());

				sb = StringBuilder();

				if (prevat > 0)
				{
					sb.Append(input, 0, prevat);
				}

				for (int i = al.size() - 1; i >= 0; i--)
				{
					sb.Append(al[i]);
				}
			}

			return sb.ToString();
		}
	}

	std::wstring RegexReplacement::Replace(MatchEvaluator evaluator, Regex* regex, const std::wstring& input, int count, int startat)
	{
		if (evaluator == NULL)
		{
			throw std::invalid_argument("evaluator");
		}
		if (count < -1)
		{
			throw ArgumentOutOfRangeException("count CountTooSmall");
		}
		if (startat < 0 || (size_t)startat > input.length())
		{
			throw ArgumentOutOfRangeException("startat BeginIndexNotNegative");
		}

		if (count == 0)
		{
			return input;
		}

		Match* match = regex->Match(&input, startat);

		if (!match->getSuccess())
		{
			return input;
		}
		else
		{
			StringBuilder sb;

			if (!regex->getRightToLeft())
			{
				sb = StringBuilder();
				int prevat = 0;

				do
				{
					if (match->getIndex() != prevat)
					{
						sb.Append(input, prevat, match->getIndex() - prevat);
					}

					prevat = match->getIndex() + match->getLength();

					sb.Append(evaluator(match));

					if (--count == 0)
					{
						break;
					}

					match = match->NextMatch();
				}
				while (match->getSuccess());

				if ((size_t)prevat < input.length())
				{
					sb.Append(input, prevat, input.length() - prevat);
				}
			}
			else
			{
				std::vector<std::wstring> al;
				int prevat = input.length();

				do
				{
					if (match->getIndex() + match->getLength() != prevat)
					{
						al.push_back(input.substr(match->getIndex() + match->getLength(), prevat - match->getIndex() - match->getLength()));
					}

					prevat = match->getIndex();

					al.push_back(evaluator(match));

					if (--count == 0)
					{
						break;
					}

					match = match->NextMatch();
				}
				while (match->getSuccess());

				sb = StringBuilder();

				if (prevat > 0)
				{
					sb.Append(input, 0, prevat);
				}

				for (int i = al.size() - 1; i >= 0; i--)
				{
					sb.Append(al[i]);
				}
			}

			return sb.ToString();
		}
	}

	std::vector<std::wstring> RegexReplacement::Split(Regex* regex, const std::wstring& input, int count, int startat)
	{
		std::vector<std::wstring> result;

		if (count < 0)
		{
			throw ArgumentOutOfRangeException("count CountTooSmall");
		}

		if (startat < 0 || (size_t)startat > input.length())
		{
			throw ArgumentOutOfRangeException("startat BeginIndexNotNegative");
		}

		if (count == 1)
		{
			result = std::vector<std::wstring>(1);
			result[0] = input;
			return result;
		}

		count -= 1;

		Match* match = regex->Match(&input, startat);

		if (!match->getSuccess())
		{
			result = std::vector<std::wstring>(1);
			result[0] = input;
			return result;
		}
		else
		{
			std::vector<std::wstring> al;

			if (!regex->getRightToLeft())
			{
				int prevat = 0;

				for (;;)
				{
					al.push_back(input.substr(prevat, match->getIndex() - prevat));

					prevat = match->getIndex() + match->getLength();

					// add all matched capture groups to the list.
					for (int i = 1; i < match->getGroups()->getCount(); i++)
					{
						if (match->IsMatched(i))
						{
							al.push_back(match->getGroups()->operator[](i)->ToString());
						}
					}

					if (--count == 0)
					{
						break;
					}

					match = match->NextMatch();

					if (!match->getSuccess())
					{
						break;
					}
				}

				al.push_back(input.substr(prevat, input.length() - prevat));
			}
			else
			{
				int prevat = input.length();

				for (;;)
				{
					al.push_back(input.substr(match->getIndex() + match->getLength(), prevat - match->getIndex() - match->getLength()));

					prevat = match->getIndex();

					// add all matched capture groups to the list.
					for (int i = 1; i < match->getGroups()->getCount(); i++)
					{
						if (match->IsMatched(i))
						{
							al.push_back(match->getGroups()->operator[](i)->ToString());
						}
					}

					if (--count == 0)
					{
						break;
					}

					match = match->NextMatch();

					if (!match->getSuccess())
					{
						break;
					}
				}

				al.push_back(input.substr(0, prevat));

				//al.Reverse(0, al.size());
				std::reverse(al.begin(), al.end());
			}

			return al;
		}
	}
}
