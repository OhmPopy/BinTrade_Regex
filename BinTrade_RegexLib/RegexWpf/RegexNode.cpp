#include "RegexNode.h"

#include "RegexCharClass.h"
#include "stringbuilder.h"
#include "UtilsString.h"

namespace Regexs
{
	int RegexNode::Nothing = 22; //          []
	int RegexNode::Empty = 23; //          ()

	int RegexNode::Alternate = 24; //          a|b
	int RegexNode::Concatenate = 25; //          ab

	int RegexNode::Loop = 26; // m,x      * + ? {,}
	int RegexNode::Lazyloop = 27; // m,x      *? +? ?? {,}?

	int RegexNode::Capture = 28; // n        ()
	int RegexNode::Group = 29; //          (?:)
	int RegexNode::Require = 30; //          (?=) (?<=)
	int RegexNode::Prevent = 31; //          (?!) (?<!)
	int RegexNode::Greedy = 32; //          (?>) (?<)
	int RegexNode::Testref = 33; //          (?(n) | )
	int RegexNode::Testgroup = 34; //          (?(...) | )

	std::vector<std::wstring> RegexNode::TypeStr =
	{
		L"Onerep", L"Notonerep", L"Setrep",
		L"Oneloop", L"Notoneloop", L"Setloop",
		L"Onelazy", L"Notonelazy", L"Setlazy",
		L"One", L"Notone", L"Set",
		L"Multi", L"Ref",
		L"Bol", L"Eol", L"Boundary", L"Nonboundary",
		L"ECMABoundary", L"NonECMABoundary",
		L"Beginning", L"Start", L"EndZ", L"End",
		L"Nothing", L"Empty",
		L"Alternate", L"Concatenate",
		L"Loop", L"Lazyloop",
		L"Capture", L"Group", L"Require", L"Prevent", L"Greedy",
		L"Testref", L"Testgroup"
	};

	RegexNode::~RegexNode()
	{
		//if (_next)
		//{
		//	if (_next != this)
		//		delete _next;
		//}
	}

	RegexNode::RegexNode(std::vector<PtrObjectForRegex>* _ptr_mgr, int type, RegexOptions options): _ch(L'\0'), _m(0), _n(0), _next(NULL)
	{
		_ptr_mgr->push_back(PtrObjectForRegex(this));
		_type = type;
		_options = options;
	}

	RegexNode::RegexNode(std::vector<PtrObjectForRegex>* _ptr_mgr, int type, RegexOptions options, wchar_t ch): _m(0), _n(0), _next(NULL)
	{
		_ptr_mgr->push_back(PtrObjectForRegex(this));
		_type = type;
		_options = options;
		_ch = ch;
	}

	RegexNode::RegexNode(std::vector<PtrObjectForRegex>* _ptr_mgr, int type, RegexOptions options, const std::wstring& str): _ch(L'\0'), _m(0), _n(0), _next(NULL)
	{
		_ptr_mgr->push_back(PtrObjectForRegex(this));
		_type = type;
		_options = options;
		_str = str;
	}

	RegexNode::RegexNode(std::vector<PtrObjectForRegex>* _ptr_mgr, int type, RegexOptions options, int m): _ch(L'\0'), _n(0), _next(NULL)
	{
		_ptr_mgr->push_back(PtrObjectForRegex(this));
		_type = type;
		_options = options;
		_m = m;
	}

	RegexNode::RegexNode(std::vector<PtrObjectForRegex>* _ptr_mgr, int type, RegexOptions options, int m, int n): _ch(L'\0'), _next(NULL)
	{
		_ptr_mgr->push_back(PtrObjectForRegex(this));
		_type = type;
		_options = options;
		_m = m;
		_n = n;
	}

	bool RegexNode::UseOptionR()
	{
		return ((int)_options & (int)RegexOptions::RightToLeft) != 0;
	}

	RegexNode* RegexNode::ReverseLeft()
	{
		if (UseOptionR() && _type == Concatenate && !_children.empty())
		{
			//_children.Reverse(0, _children.size());
			std::reverse(_children.begin(), _children.end());
		}

		return this;
	}

	void RegexNode::MakeRep(int type, int min, int max)
	{
		_type += (type - RegexCode::One);
		_m = min;
		_n = max;
	}

	RegexNode* RegexNode::Reduce(std::vector<PtrObjectForRegex>* _ptr_mgr)
	{
		RegexNode* n;
		auto type = Type();

		if (type == Alternate)
		{
			n = ReduceAlternation(_ptr_mgr);
		}
		else if (type == Concatenate)
		{
			n = ReduceConcatenation(_ptr_mgr);
		}
		else if (type == Loop || type == Lazyloop)
		{
			n = ReduceRep(_ptr_mgr);
		}
		else if (type == Group)
		{
			n = ReduceGroup();
		}
		else if (type == RegexCode::Set || type == RegexCode::Setloop)
		{
			n = ReduceSet();
		}
		else
		{
			n = this;
		}


		return n;
	}

	RegexNode* RegexNode::StripEnation(std::vector<PtrObjectForRegex>* _ptr_mgr, int emptyType)
	{
		switch (ChildCount())
		{
		case 0:
			return new RegexNode(_ptr_mgr, emptyType, _options);
		case 1:
			return Child(0);
		default:
			return this;
		}
	}

	RegexNode* RegexNode::ReduceGroup()
	{
		RegexNode* u;

		for (u = this; u->Type() == Group;)
		{
			u = u->Child(0);
		}

		return u;
	}

	RegexNode* RegexNode::ReduceRep(std::vector<PtrObjectForRegex>* _ptr_mgr)
	{
		RegexNode* u;
		RegexNode* child;
		int type;
		int min;
		int max;

		u = this;
		type = Type();
		min = _m;
		max = _n;

		for (;;)
		{
			if (u->ChildCount() == 0)
			{
				break;
			}

			child = u->Child(0);

			// multiply reps of the same type only
			if (child->Type() != type)
			{
				int childType = child->Type();

				if (!(childType >= RegexCode::Oneloop && childType <= RegexCode::Setloop && type == Loop || childType >= RegexCode::Onelazy && childType <= RegexCode::Setlazy && type == Lazyloop))
				{
					break;
				}
			}

			// child can be too lumpy to blur, e.g., (a {100,105}) {3} or (a {2,})?
			// [but things like (a {2,})+ are not too lumpy...]
			if (u->_m == 0 && child->_m > 1 || child->_n < child->_m * 2)
			{
				break;
			}

			u = child;
			if (u->_m > 0)
			{
				u->_m = min = ((INT_MAX - 1) / u->_m < min) ? INT_MAX : u->_m * min;
			}
			if (u->_n > 0)
			{
				u->_n = max = ((INT_MAX - 1) / u->_n < max) ? INT_MAX : u->_n * max;
			}
		}

		return min == INT_MAX ? new RegexNode(_ptr_mgr, Nothing, _options) : u;
	}

	RegexNode* RegexNode::ReduceSet()
	{
		// Extract empty-set, one and not-one case as special

		if (RegexCharClass::IsEmpty(_str))
		{
			_type = Nothing;
			_str = L"";
		}
		else if (RegexCharClass::IsSingleton(_str))
		{
			_ch = RegexCharClass::SingletonChar(_str);
			_str = L"";
			_type += (RegexCode::One - RegexCode::Set);
		}
		else if (RegexCharClass::IsSingletonInverse(_str))
		{
			_ch = RegexCharClass::SingletonChar(_str);
			_str = L"";
			_type += (RegexCode::Notone - RegexCode::Set);
		}

		return this;
	}

	RegexNode* RegexNode::ReduceAlternation(std::vector<PtrObjectForRegex>* _ptr_mgr)
	{
		// Combine adjacent sets/chars

		bool wasLastSet;
		bool lastNodeCannotMerge;
		RegexOptions optionsLast;
		RegexOptions optionsAt;
		size_t i;
		int j;
		RegexNode* at;
		RegexNode* prev;

		if (_children.empty())
		{
			return new RegexNode(_ptr_mgr, RegexNode::Nothing, _options);
		}

		wasLastSet = false;
		lastNodeCannotMerge = false;
		optionsLast = (Regexs::RegexOptions)0;

		for (i = 0, j = 0; i < _children.size(); i++, j++)
		{
			at = _children[i];

			if ((size_t)j < i)
			{
				_children[j] = at;
			}

			for (;;)
			{
				if (at->_type == Alternate)
				{
					for (size_t k = 0; k < at->_children.size(); k++)
					{
						at->_children[k]->_next = this;
					}

					//_children.InsertRange(i + 1, at->_children);
					_children.insert(_children.begin() + i + 1, at->_children.begin(), at->_children.end());

					j--;
				}
				else if (at->_type == RegexCode::Set || at->_type == RegexCode::One)
				{
					// Cannot merge sets if L or I options differ, or if either are negated.
					//optionsAt = at->_options & (RegexOptions::RightToLeft | RegexOptions::IgnoreCase);
					optionsAt = (RegexOptions)((int)at->_options & ((int)RegexOptions::RightToLeft | (int)RegexOptions::IgnoreCase));


					if (at->_type == RegexCode::Set)
					{
						if (!wasLastSet || optionsLast != optionsAt || lastNodeCannotMerge || !RegexCharClass::IsMergeable(at->_str))
						{
							wasLastSet = true;
							lastNodeCannotMerge = !RegexCharClass::IsMergeable(at->_str);
							optionsLast = optionsAt;
							break;
						}
					}
					else if (!wasLastSet || optionsLast != optionsAt || lastNodeCannotMerge)
					{
						wasLastSet = true;
						lastNodeCannotMerge = false;
						optionsLast = optionsAt;
						break;
					}


					// The last node was a Set or a One, we're a Set or One and our options are the same.
					// Merge the two nodes.
					j--;
					prev = _children[j];

					RegexCharClass* prevCharClass;
					if (prev->_type == RegexCode::One)
					{
						prevCharClass = new RegexCharClass();
						prevCharClass->AddChar(prev->_ch);
					}
					else
					{
						prevCharClass = RegexCharClass::Parse(prev->_str);
					}

					if (at->_type == RegexCode::One)
					{
						prevCharClass->AddChar(at->_ch);
					}
					else
					{
						RegexCharClass* atCharClass = RegexCharClass::Parse(at->_str);
						prevCharClass->AddCharClass(atCharClass);
						delete atCharClass;
					}

					prev->_type = RegexCode::Set;
					prev->_str = prevCharClass->ToStringClass();

					delete prevCharClass;
				}
				else if (at->_type == RegexNode::Nothing)
				{
					j--;
				}
				else
				{
					wasLastSet = false;
					lastNodeCannotMerge = false;
				}

				break;
			}
		}

		if ((size_t)j < i)
		{
			//_children.RemoveRange(j, i - j);
			int count = i - j;
			_children.erase(_children.begin() + j, _children.begin() + j + count);
		}

		return StripEnation(_ptr_mgr, RegexNode::Nothing);
	}

	RegexNode* RegexNode::ReduceConcatenation(std::vector<PtrObjectForRegex>* _ptr_mgr)
	{
		// Eliminate empties and concat adjacent strings/chars

		bool wasLastString;
		RegexOptions optionsLast;
		RegexOptions optionsAt;
		size_t i;
		int j;

		if (_children.empty())
		{
			return new RegexNode(_ptr_mgr, RegexNode::Empty, _options);
		}

		wasLastString = false;
		optionsLast = (Regexs::RegexOptions)0;

		for (i = 0, j = 0; i < _children.size(); i++, j++)
		{
			RegexNode* at;
			RegexNode* prev;

			at = _children[i];

			if ((size_t)j < i)
			{
				_children[j] = at;
			}

			if (at->_type == RegexNode::Concatenate && (((int)at->_options & (int)RegexOptions::RightToLeft) == ((int)_options & (int)RegexOptions::RightToLeft)))
			{
				for (size_t k = 0; k < at->_children.size(); k++)
				{
					at->_children[k]->_next = this;
				}

				//_children.InsertRange(i + 1, at->_children);
				_children.insert(_children.begin() + i + 1, at->_children.begin(), at->_children.end());


				j--;
			}
			else if (at->_type == RegexCode::Multi || at->_type == RegexCode::One)
			{
				// Cannot merge strings if L or I options differ
				//optionsAt = at->_options & (RegexOptions::RightToLeft | RegexOptions::IgnoreCase);
				optionsAt = (RegexOptions)((int)at->_options & ((int)RegexOptions::RightToLeft | (int)RegexOptions::IgnoreCase));

				if (!wasLastString || optionsLast != optionsAt)
				{
					wasLastString = true;
					optionsLast = optionsAt;
					continue;
				}

				prev = _children[--j];

				if (prev->_type == RegexCode::One)
				{
					prev->_type = RegexCode::Multi;
					prev->_str = UtilsCultureInfo::CharToString(ComCultureInfo::InvariantCulture(), prev->_ch);
				}

				if (((int)optionsAt & (int)RegexOptions::RightToLeft) == 0)
				{
					if (at->_type == RegexCode::One)
					{
						prev->_str += std::wstring(1, at->_ch);
					}
					else
					{
						prev->_str += at->_str;
					}
				}
				else
				{
					if (at->_type == RegexCode::One)
					{
						prev->_str = std::wstring(1, at->_ch) + prev->_str;
					}
					else
					{
						prev->_str = at->_str + prev->_str;
					}
				}
			}
			else if (at->_type == RegexNode::Empty)
			{
				j--;
			}
			else
			{
				wasLastString = false;
			}
		}

		if ((size_t)j < i)
		{
			//_children.RemoveRange(j, i - j);
			int count = i - j;

			_children.erase(_children.begin() + j, _children.begin() + j + count);
		}

		return StripEnation(_ptr_mgr, RegexNode::Empty);
	}

	RegexNode* RegexNode::MakeQuantifier(std::vector<PtrObjectForRegex>* _ptr_mgr, bool lazy, int min, int max)
	{
		RegexNode* result;

		if (min == 0 && max == 0)
		{
			return new RegexNode(_ptr_mgr, RegexNode::Empty, _options);
		}

		if (min == 1 && max == 1)
		{
			return this;
		}

		if (_type == RegexCode::One || _type == RegexCode::Notone || _type == RegexCode::Set)
		{
			MakeRep(lazy ? RegexCode::Onelazy : RegexCode::Oneloop, min, max);
			return this;
		}
		else
		{
			result = new RegexNode(_ptr_mgr, lazy ? RegexNode::Lazyloop : RegexNode::Loop, _options, min, max);
			result->AddChild(_ptr_mgr, this);

			return result;
		}
	}

	void RegexNode::AddChild(std::vector<PtrObjectForRegex>* _ptr_mgr, RegexNode* newChild)
	{
		RegexNode* reducedChild;

		if (_children.empty())
		{
			_children = std::vector<RegexNode*>();
			_children.reserve(4);
		}

		reducedChild = newChild->Reduce(_ptr_mgr);

		_children.push_back(reducedChild);
		reducedChild->_next = this;
	}

	RegexNode* RegexNode::Child(int i)
	{
		return _children[i];
	}

	int RegexNode::ChildCount()
	{
		return _children.empty() ? 0 : _children.size();
	}

	int RegexNode::Type()
	{
		return _type;
	}

	std::wstring RegexNode::Description()
	{
		StringBuilder ArgSb = StringBuilder();

		ArgSb.Append(TypeStr[_type]);

		if (((int)_options & (int)RegexOptions::ExplicitCapture) != 0)
			ArgSb.Append(L"-C");
		if (((int)_options & (int)RegexOptions::IgnoreCase) != 0)
			ArgSb.Append(L"-I");
		if (((int)_options & (int)RegexOptions::RightToLeft) != 0)
			ArgSb.Append(L"-L");
		if (((int)_options & (int)RegexOptions::Multiline) != 0)
			ArgSb.Append(L"-M");
		if (((int)_options & (int)RegexOptions::Singleline) != 0)
			ArgSb.Append(L"-S");
		if (((int)_options & (int)RegexOptions::IgnorePatternWhitespace) != 0)
			ArgSb.Append(L"-X");
		if (((int)_options & (int)RegexOptions::ECMAScript) != 0)
			ArgSb.Append(L"-E");

		switch (_type)
		{
			//case RegexCode::Oneloop:
			//case RegexCode::Notoneloop:
			//case RegexCode::Onelazy:
			//case RegexCode::Notonelazy:
			//case RegexCode::One:
			//case RegexCode::Notone:

		case 3:
		case 4:
		case 6:
		case 7:
		case 9:
		case 10:
			ArgSb.Append(L"(Ch = " + RegexCharClass::CharDescription(_ch) + L")");
			break;

			//case Capture:
		case 28:
			ArgSb.Append(L"(index = " + std::to_wstring(_m) + L", unindex = " + std::to_wstring(_n) + L")");
			break;

			//case RegexCode::Ref:
			//case Testref:
		case 13:
		case 33:
			ArgSb.Append(L"(index = " + std::to_wstring(_m) + L")");
			break;
			//case RegexCode::Multi:
		case 12:
			ArgSb.Append(L"(String = " + _str + L")");
			break;
			//case RegexCode::Set:
			//case RegexCode::Setloop:
			//case RegexCode::Setlazy:

		case 11:
		case 5:
		case 8:
			ArgSb.Append(L"(Set = " + RegexCharClass::SetDescription(_str) + L")");
			break;
		default: ;
		}

		switch (_type)
		{
			//case RegexCode::Oneloop:
			//case RegexCode::Notoneloop:
			//case RegexCode::Onelazy:
			//case RegexCode::Notonelazy:
			//case RegexCode::Setloop:
			//case RegexCode::Setlazy:
			//case Loop:
			//case Lazyloop:

		case 3:
		case 4:
		case 6:
		case 7:
		case 5:
		case 8:
		case 26:
		case 27:
			ArgSb.Append(L"(Min = " + std::to_wstring(_m) + L", Max = " + (_n == INT_MAX ? L"inf" : UtilsCultureInfo::CharToString(ComCultureInfo::InvariantCulture(), _n)) + L")");
			break;
		default: ;
		}

		return ArgSb.ToString();
	}

	std::wstring RegexNode::Dump()
	{
		std::vector<int> Stack;
		int CurChild;

		RegexNode* CurNode = this;
		CurChild = 0;

		StringBuilder builder;
		builder.AppendLine(CurNode->Description());

		for (;;)
		{
			if ((size_t)CurChild < CurNode->_children.size())
			{
				Stack.push_back(CurChild + 1);
				CurNode = CurNode->_children[CurChild];
				CurChild = 0;

				int Depth = Stack.size();
				if (Depth > 32)
					Depth = 32;

				builder.AppendLine(Space.substr(0, Depth) + CurNode->Description());
			}
			else
			{
				if (Stack.size() == 0)
					break;

				CurChild = Stack[Stack.size() - 1];
				Stack.erase(Stack.begin() + Stack.size() - 1);
				CurNode = CurNode->_next;
			}
		}

		return builder.ToString();
	}
}
