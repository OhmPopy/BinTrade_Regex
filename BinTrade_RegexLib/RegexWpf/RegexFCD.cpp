#include "RegexFCD.h"

#include "RegexCharClass.h"
#include "RegexNode.h"
#include "RegexOptions.h"
#include "RegexTree.h"
#include <unordered_set>
#include <stdint.h>
#include <cstdint>

#include "stringbuilder.h"
#include "UtilsString.h"

namespace Regexs
{
	int RegexFCD::BeforeChild = 64;
	int RegexFCD::AfterChild = 128;
	int RegexFCD::Beginning = 0x0001;
	int RegexFCD::Bol = 0x0002;
	int RegexFCD::Start = 0x0004;
	int RegexFCD::Eol = 0x0008;
	int RegexFCD::EndZ = 0x0010;
	int RegexFCD::End = 0x0020;
	int RegexFCD::Boundary = 0x0040;
	int RegexFCD::ECMABoundary = 0x0080;


	void RegexFCD::Releesa()
	{
		std::unordered_set<std::uintptr_t> release;
		for (size_t i = 0; i < _ptr_mgr.size(); ++i)
		{
			auto address = _ptr_mgr[i]._address;
			bool need = true;
			const bool is_in = release.find(address) != release.end();
			if (is_in)
				need = false;

			if (need)
			{
				if (_ptr_mgr[i].object)
				{
					release.insert(address);
					delete _ptr_mgr[i].object;
				}
			}
		}
	}

	RegexPrefix* RegexFCD::FirstChars(RegexTree* t)
	{
		RegexFCD s = RegexFCD();
		RegexFC* fc = s.RegexFCFromRegexTree(t);

		if (fc == NULL || fc->_nullable)
		{
			s.Releesa();
			return NULL;
		}

		ComCultureInfo* culture = (((int)t->_options & (int)RegexOptions::CultureInvariant) != 0) ? ComCultureInfo::InvariantCulture() : ComCultureInfo::CurrentCulture();

		auto result = new RegexPrefix(fc->GetFirstChars(culture), fc->IsCaseInsensitive());
		s.Releesa();
		return result;
	}

	void RegexFCD::Prefix(RegexTree* tree, RegexPrefix& prefix, bool& is_prefix_empty)
	{
		is_prefix_empty = true;
		RegexNode* curNode;
		RegexNode* concatNode = NULL;
		int nextChild = 0;

		curNode = tree->_root;

		for (;;)
		{
			if (curNode->_type == RegexNode::Concatenate)
			{
				if (curNode->ChildCount() > 0)
				{
					concatNode = curNode;
					nextChild = 0;
				}
			}
			else if (curNode->_type == RegexNode::Greedy || curNode->_type == RegexNode::Capture)
			{
				curNode = curNode->Child(0);
				concatNode = NULL;
				continue;
			}
			else if (curNode->_type == RegexCode::Oneloop || curNode->_type == RegexCode::Onelazy)
			{
				if (curNode->_m > 0)
				{
					//std::wstring pref = (std::wstring(L"")).PadRight(curNode->_m, curNode->_ch);
					std::wstring pref = L"";
					pref.insert(0, curNode->_m, curNode->_ch);


					prefix = RegexPrefix(pref, 0 != ((int)curNode->_options & (int)RegexOptions::IgnoreCase));
					is_prefix_empty = false;
					return;
				}
				prefix = *RegexPrefix::getEmpty();
				return;
			}
			else if (curNode->_type == RegexCode::One)
			{
				//return new RegexPrefix(curNode->_ch.ToString(CultureInfo::InvariantCulture), 0 != ((int)curNode->_options & (int)RegexOptions::IgnoreCase));
				auto text = UtilsCultureInfo::CharToString(ComCultureInfo::InvariantCulture(), curNode->_ch);
				prefix = RegexPrefix(text, 0 != ((int)curNode->_options & (int)RegexOptions::IgnoreCase));
				is_prefix_empty = false;
				return;
			}
			else if (curNode->_type == RegexCode::Multi)
			{
				prefix = RegexPrefix(curNode->_str, 0 != ((int)curNode->_options & (int)RegexOptions::IgnoreCase));
				is_prefix_empty = false;
				return;
			}
			else if (
				curNode->_type == RegexCode::Bol ||
				curNode->_type == RegexCode::Eol ||
				curNode->_type == RegexCode::Boundary ||
				curNode->_type == RegexCode::ECMABoundary ||
				curNode->_type == RegexCode::Beginning ||
				curNode->_type == RegexCode::Start ||
				curNode->_type == RegexCode::EndZ ||
				curNode->_type == RegexCode::End ||
				curNode->_type == RegexNode::Empty ||
				curNode->_type == RegexNode::Require ||
				curNode->_type == RegexNode::Prevent
			)
			{
			}
			else
			{
				prefix = *RegexPrefix::getEmpty();
				return;
			}

			if (concatNode == NULL || nextChild >= concatNode->ChildCount())
			{
				prefix = *RegexPrefix::getEmpty();
				return;
			}

			curNode = concatNode->Child(nextChild++);
		}
	}

	int RegexFCD::Anchors(RegexTree* tree)
	{
		RegexNode* curNode;
		RegexNode* concatNode = NULL;
		int nextChild = 0;
		int result = 0;

		curNode = tree->_root;

		for (;;)
		{
			switch (curNode->_type)
			{
				//case RegexNode::Concatenate:
			case 25:
				if (curNode->ChildCount() > 0)
				{
					concatNode = curNode;
					nextChild = 0;
				}

				break;

				//case RegexNode::Greedy:
				//case RegexNode::Capture:
			case 32:
			case 28:
				curNode = curNode->Child(0);
				concatNode = NULL;
				continue;

				//case RegexCode::Bol:
				//case RegexCode::Eol:
				//case RegexCode::Boundary:
				//case RegexCode::ECMABoundary:
				//case RegexCode::Beginning:
				//case RegexCode::Start:
				//case RegexCode::EndZ:
				//case RegexCode::End:
			case 14:
			case 15:
			case 16:
			case 41:
			case 18:
			case 19:
			case 20:
			case 21:
				return result | AnchorFromType(curNode->_type);

				//case RegexNode::Empty:
				//case RegexNode::Require:
				//case RegexNode::Prevent:
			case 23:
			case 30:
			case 31:
				break;

			default:
				return result;
			}

			if (concatNode == NULL || nextChild >= concatNode->ChildCount())
			{
				return result;
			}

			curNode = concatNode->Child(nextChild++);
		}
	}

	std::wstring RegexFCD::AnchorDescription(int anchors)
	{
		StringBuilder sb = StringBuilder();

		if (0 != (anchors & Beginning)) sb.Append(L", Beginning");
		if (0 != (anchors & Start)) sb.Append(L", Start");
		if (0 != (anchors & Bol)) sb.Append(L", Bol");
		if (0 != (anchors & Boundary)) sb.Append(L", Boundary");
		if (0 != (anchors & ECMABoundary)) sb.Append(L", ECMABoundary");
		if (0 != (anchors & Eol)) sb.Append(L", Eol");
		if (0 != (anchors & End)) sb.Append(L", End");
		if (0 != (anchors & EndZ)) sb.Append(L", EndZ");

		if (sb.GetLength() >= 2)
			return (sb.ToString(2, sb.GetLength() - 2));

		return L"None";
	}

	int RegexFCD::AnchorFromType(int type)
	{
		if (type == RegexCode::Bol)
		{
			return Bol;
		}
		if (type == RegexCode::Eol)
		{
			return Eol;
		}
		if (type == RegexCode::Boundary)
		{
			return Boundary;
		}
		if (type == RegexCode::ECMABoundary)
		{
			return ECMABoundary;
		}
		if (type == RegexCode::Beginning)
		{
			return Beginning;
		}
		if (type == RegexCode::Start)
		{
			return Start;
		}
		if (type == RegexCode::EndZ)
		{
			return EndZ;
		}
		if (type == RegexCode::End)
		{
			return End;
		}
		return 0;
	}


	RegexFCD::RegexFCD(): _intDepth(0), _fcDepth(0), _skipAllChildren(false), _skipchild(false), _failed(false)
	{
		_fcStack = std::vector<RegexFC*>(32);
		_intStack = std::vector<int>(32);
	}

	RegexFCD::~RegexFCD()
	{
	}

	void RegexFCD::PushInt(int I)
	{
		if ((size_t)_intDepth >= _intStack.size())
		{
			std::vector<int> expanded(_intDepth * 2);

			std::copy_n(_intStack.begin(), _intDepth, expanded.begin());

			_intStack = expanded;
		}

		_intStack[_intDepth++] = I;
	}

	bool RegexFCD::IntIsEmpty()
	{
		return _intDepth == 0;
	}

	int RegexFCD::PopInt()
	{
		return _intStack[--_intDepth];
	}

	void RegexFCD::PushFC(RegexFC* fc)
	{
		if ((size_t)_fcDepth >= _fcStack.size())
		{
			std::vector<RegexFC*> expanded(_fcDepth * 2);

			std::copy_n(_fcStack.begin(), _fcDepth, expanded.begin());
			_fcStack = expanded;
		}

		_ptr_mgr.push_back(PtrObjectForRegex(fc));
		_fcStack[_fcDepth++] = fc;
	}

	bool RegexFCD::FCIsEmpty()
	{
		return _fcDepth == 0;
	}

	RegexFC* RegexFCD::PopFC()
	{
		return _fcStack[--_fcDepth];
	}

	RegexFC* RegexFCD::TopFC()
	{
		return _fcStack[_fcDepth - 1];
	}

	RegexFC* RegexFCD::RegexFCFromRegexTree(RegexTree* tree)
	{
		RegexNode* curNode;
		int curChild;

		curNode = tree->_root;
		curChild = 0;

		for (;;)
		{
			if (curNode->_children.empty())
			{
				// This is a leaf node
				CalculateFC(curNode->_type, curNode, 0);
			}
			else if ((size_t)curChild < curNode->_children.size() && !_skipAllChildren)
			{
				// This is an interior node, and we have more children to analyze
				CalculateFC(curNode->_type | BeforeChild, curNode, curChild);

				if (!_skipchild)
				{
					curNode = static_cast<RegexNode*>(curNode->_children[curChild]);
					// this stack is how we get a depth first walk of the tree. 
					PushInt(curChild);
					curChild = 0;
				}
				else
				{
					curChild++;
					_skipchild = false;
				}

				continue;
			}

			// This is an interior node where we've finished analyzing all the children, or
			// the end of a leaf node. 
			_skipAllChildren = false;

			if (IntIsEmpty())
			{
				break;
			}

			curChild = PopInt();
			curNode = curNode->_next;

			CalculateFC(curNode->_type | AfterChild, curNode, curChild);
			if (_failed)
			{
				return NULL;
			}

			curChild++;
		}

		if (FCIsEmpty())
		{
			return NULL;
		}

		return PopFC();
	}

	void RegexFCD::SkipChild()
	{
		_skipchild = true;
	}

	void RegexFCD::CalculateFC(int NodeType, RegexNode* node, int CurIndex)
	{
		bool ci = false;
		bool rtl = false;

		if (NodeType <= RegexCode::Ref)
		{
			if (((int)node->_options & (int)RegexOptions::IgnoreCase) != 0)
			{
				ci = true;
			}
			if (((int)node->_options & (int)RegexOptions::RightToLeft) != 0)
			{
				rtl = true;
			}
		}

		if (
			NodeType == (RegexNode::Concatenate | BeforeChild) ||
			NodeType == (RegexNode::Alternate | BeforeChild) ||
			NodeType == (RegexNode::Testref | BeforeChild) ||
			NodeType == (RegexNode::Loop | BeforeChild) ||
			NodeType == (RegexNode::Lazyloop | BeforeChild)
		)
		{
		}
		else if (NodeType == (RegexNode::Testgroup | BeforeChild))
		{
			if (CurIndex == 0)
			{
				SkipChild();
			}
		}
		else if (NodeType == RegexNode::Empty)
		{
			RegexFC* tempVar = new RegexFC(true);
			PushFC(tempVar);
		}
		else if (NodeType == (RegexNode::Concatenate | AfterChild))
		{
			if (CurIndex != 0)
			{
				RegexFC* child = PopFC();
				RegexFC* cumul = TopFC();

				_failed = !cumul->AddFC(child, true);
			}

			if (!TopFC()->_nullable)
			{
				_skipAllChildren = true;
			}
		}
		else if (NodeType == (RegexNode::Testgroup | AfterChild))
		{
			if (CurIndex > 1)
			{
				RegexFC* child = PopFC();
				RegexFC* cumul = TopFC();

				_failed = !cumul->AddFC(child, false);
			}
		}
		else if (NodeType == (RegexNode::Alternate | AfterChild) || NodeType == (RegexNode::Testref | AfterChild))
		{
			if (CurIndex != 0)
			{
				RegexFC* child = PopFC();
				RegexFC* cumul = TopFC();

				_failed = !cumul->AddFC(child, false);
			}
		}
		else if (NodeType == (RegexNode::Loop | AfterChild) || NodeType == (RegexNode::Lazyloop | AfterChild))
		{
			if (node->_m == 0)
			{
				TopFC()->_nullable = true;
			}
		}
		else if (
			NodeType == (RegexNode::Group | BeforeChild) ||
			NodeType == (RegexNode::Group | AfterChild) ||
			NodeType == (RegexNode::Capture | BeforeChild) ||
			NodeType == (RegexNode::Capture | AfterChild) ||
			NodeType == (RegexNode::Greedy | BeforeChild) ||
			NodeType == (RegexNode::Greedy | AfterChild)
		)
		{
		}
		else if (NodeType == (RegexNode::Require | BeforeChild) || NodeType == (RegexNode::Prevent | BeforeChild))
		{
			SkipChild();
			RegexFC* tempVar2 = new RegexFC(true);
			PushFC(tempVar2);
		}
		else if (NodeType == (RegexNode::Require | AfterChild) || NodeType == (RegexNode::Prevent | AfterChild))
		{
		}
		else if (NodeType == RegexCode::One || NodeType == RegexCode::Notone)
		{
			RegexFC* tempVar3 = new RegexFC(node->_ch, NodeType == RegexCode::Notone, false, ci);
			PushFC(tempVar3);
		}
		else if (NodeType == RegexCode::Oneloop || NodeType == RegexCode::Onelazy)
		{
			RegexFC* tempVar4 = new RegexFC(node->_ch, false, node->_m == 0, ci);
			PushFC(tempVar4);
		}
		else if (NodeType == RegexCode::Notoneloop || NodeType == RegexCode::Notonelazy)
		{
			RegexFC* tempVar5 = new RegexFC(node->_ch, true, node->_m == 0, ci);
			PushFC(tempVar5);
		}
		else if (NodeType == RegexCode::Multi)
		{
			if (node->_str.length() == 0)
			{
				RegexFC* tempVar6 = new RegexFC(true);
				PushFC(tempVar6);
			}
			else if (!rtl)
			{
				RegexFC* tempVar7 = new RegexFC(node->_str[0], false, false, ci);
				PushFC(tempVar7);
			}
			else
			{
				RegexFC* tempVar8 = new RegexFC(node->_str[node->_str.length() - 1], false, false, ci);
				PushFC(tempVar8);
			}
		}
		else if (NodeType == RegexCode::Set)
		{
			RegexFC* tempVar9 = new RegexFC(node->_str, false, ci);
			PushFC(tempVar9);
		}
		else if (NodeType == RegexCode::Setloop || NodeType == RegexCode::Setlazy)
		{
			RegexFC* tempVar10 = new RegexFC(node->_str, node->_m == 0, ci);
			PushFC(tempVar10);
		}
		else if (NodeType == RegexCode::Ref)
		{
			RegexFC* tempVar11 = new RegexFC(RegexCharClass::AnyClass, true, false);
			PushFC(tempVar11);
		}
		else if (
			NodeType == RegexNode::Nothing ||
			NodeType == RegexCode::Bol ||
			NodeType == RegexCode::Eol ||
			NodeType == RegexCode::Boundary ||
			NodeType == RegexCode::Nonboundary ||
			NodeType == RegexCode::ECMABoundary ||
			NodeType == RegexCode::NonECMABoundary ||
			NodeType == RegexCode::Beginning ||
			NodeType == RegexCode::Start ||
			NodeType == RegexCode::EndZ ||
			NodeType == RegexCode::End
		)
		{
			RegexFC* tempVar12 = new RegexFC(true);
			PushFC(tempVar12);
		}
		else
		{
			throw std::invalid_argument("UnexpectedOpcode" + std::to_string(NodeType));
		}
	}

	RegexFC::~RegexFC()
	{
		delete _cc;
	}

	RegexFC::RegexFC(bool nullable)
	{
		_cc = new RegexCharClass();
		_nullable = nullable;
	}

	RegexFC::RegexFC(wchar_t ch, bool not_Keyword, bool nullable, bool caseInsensitive)
	{
		_cc = new RegexCharClass();

		if (not_Keyword)
		{
			if (ch > 0)
			{
				_cc->AddRange(L'\0', static_cast<wchar_t>(ch - 1));
			}
			if (ch < 0xFFFF)
			{
				_cc->AddRange(static_cast<wchar_t>(ch + 1), L'\uFFFF');
			}
		}
		else
		{
			_cc->AddRange(ch, ch);
		}

		_caseInsensitive = caseInsensitive;
		_nullable = nullable;
	}

	RegexFC::RegexFC(const std::wstring& charClass, bool nullable, bool caseInsensitive)
	{
		_cc = RegexCharClass::Parse(charClass);

		_nullable = nullable;
		_caseInsensitive = caseInsensitive;
	}

	bool RegexFC::AddFC(RegexFC* fc, bool concatenate)
	{
		if (!_cc->getCanMerge() || !fc->_cc->getCanMerge())
		{
			return false;
		}

		if (concatenate)
		{
			if (!_nullable)
			{
				return true;
			}

			if (!fc->_nullable)
			{
				_nullable = false;
			}
		}
		else
		{
			if (fc->_nullable)
			{
				_nullable = true;
			}
		}

		_caseInsensitive |= fc->_caseInsensitive;
		_cc->AddCharClass(fc->_cc);
		return true;
	}

	std::wstring RegexFC::GetFirstChars(ComCultureInfo* culture)
	{
		if (_caseInsensitive)
		{
			_cc->AddLowercase(culture);
		}

		return _cc->ToStringClass();
	}

	bool RegexFC::IsCaseInsensitive()
	{
		return _caseInsensitive;
	}

	RegexPrefix* RegexPrefix::_empty = new RegexPrefix(L"", false);

	RegexPrefix::RegexPrefix()
	{
		_caseInsensitive = false;
	}

	RegexPrefix::RegexPrefix(const std::wstring& prefix, bool ci)
	{
		_prefix = prefix;
		_caseInsensitive = ci;
	}

	RegexPrefix::~RegexPrefix()
	{
	}

	std::wstring RegexPrefix::getPrefix() const
	{
		return _prefix;
	}

	bool RegexPrefix::getCaseInsensitive() const
	{
		return _caseInsensitive;
	}

	RegexPrefix* RegexPrefix::getEmpty()
	{
		return _empty;
	}
}
