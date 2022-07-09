#include "RegexWriter.h"
#include "RegexFCD.h"
#include "RegexBoyerMoore.h"
#include "RegexOptions.h"

#include "RegexNode.h"

namespace Regexs
{
	int RegexWriter::BeforeChild = 64;
	int RegexWriter::AfterChild = 128;

	RegexCode* RegexWriter::Write(RegexTree* t)
	{
		RegexWriter w = RegexWriter();
		RegexCode* retval = w.RegexCodeFromRegexTree(t);
		return retval;
	}

	RegexWriter::RegexWriter(): _depth(0), _curpos(0), _counting(false), _count(0), _trackcount(0)
	{
		_intStack = std::vector<int>(32);
		_emitted = std::vector<int>(32);
		_stringhash = std::unordered_map<std::wstring, int>();
		_stringtable = std::vector<std::wstring>();
	}

	void RegexWriter::PushInt(int I)
	{
		if ((size_t)_depth >= _intStack.size())
		{
			std::vector<int> expanded(_depth * 2);

			std::copy_n(_intStack.begin(), _depth, expanded.begin());

			_intStack = expanded;
		}

		_intStack[_depth++] = I;
	}

	bool RegexWriter::EmptyStack()
	{
		return _depth == 0;
	}

	int RegexWriter::PopInt()
	{
		return _intStack[--_depth];
	}

	int RegexWriter::CurPos()
	{
		return _curpos;
	}

	void RegexWriter::PatchJump(int Offset, int jumpDest)
	{
		_emitted[Offset + 1] = jumpDest;
	}

	void RegexWriter::Emit(int op)
	{
		if (_counting)
		{
			_count += 1;
			if (RegexCode::OpcodeBacktracks(op))
			{
				_trackcount += 1;
			}
			return;
		}

		_emitted[_curpos++] = op;
	}

	void RegexWriter::Emit(int op, int opd1)
	{
		if (_counting)
		{
			_count += 2;
			if (RegexCode::OpcodeBacktracks(op))
			{
				_trackcount += 1;
			}
			return;
		}

		_emitted[_curpos++] = op;
		_emitted[_curpos++] = opd1;
	}

	void RegexWriter::Emit(int op, int opd1, int opd2)
	{
		if (_counting)
		{
			_count += 3;
			if (RegexCode::OpcodeBacktracks(op))
			{
				_trackcount += 1;
			}
			return;
		}

		_emitted[_curpos++] = op;
		_emitted[_curpos++] = opd1;
		_emitted[_curpos++] = opd2;
	}

	int RegexWriter::StringCode(const std::wstring& str)
	{
		int i;

		if (_counting)
		{
			return 0;
		}

		if (_stringhash.find(str) != _stringhash.end())
		{
			i = static_cast<int>(_stringhash[str]);
		}
		else
		{
			i = _stringtable.size();
			_stringhash[str] = i;
			_stringtable.push_back(str);
		}

		return i;
	}

	std::invalid_argument RegexWriter::MakeException(const std::wstring& message)
	{
		std::string temp = std::string(message.begin(), message.end());
		return std::invalid_argument(temp);
	}

	int RegexWriter::MapCapnum(int capnum)
	{
		if (capnum == -1)
		{
			return -1;
		}

		if (!_caps.empty())
		{
			return _caps[capnum];
		}
		else
		{
			return capnum;
		}
	}

	RegexCode* RegexWriter::RegexCodeFromRegexTree(RegexTree* tree)
	{
		RegexNode* curNode;
		int curChild;
		int capsize;
		RegexPrefix* fcPrefix;
		RegexPrefix prefix;
		bool is_prefix_empty = true;
		int anchors;
		RegexBoyerMoore* bmPrefix;
		bool rtl;
		std::vector<PtrObjectForRegex> _ptr_mgr = tree->_ptr_mgr;

		// construct sparse capnum mapping if some numbers are unused

		if (tree->_capnumlist.empty() || tree->_captop == tree->_capnumlist.size())
		{
			capsize = tree->_captop;
			_caps = std::unordered_map<int, int>();
		}
		else
		{
			capsize = tree->_capnumlist.size();
			_caps = tree->_caps;
			for (size_t i = 0; i < tree->_capnumlist.size(); i++)
			{
				_caps[tree->_capnumlist[i]] = i;
			}
		}

		_counting = true;

		for (;;)
		{
			if (!_counting)
			{
				_emitted = std::vector<int>(_count);
			}

			curNode = tree->_root;
			_ptr_mgr.push_back(PtrObjectForRegex(curNode));
			curChild = 0;

			Emit(RegexCode::Lazybranch, 0);

			for (;;)
			{
				if (curNode->_children.empty())
				{
					EmitFragment(curNode->_type, curNode, 0);
				}
				else if ((size_t)curChild < curNode->_children.size())
				{
					EmitFragment(curNode->_type | BeforeChild, curNode, curChild);

					curNode = static_cast<RegexNode*>(curNode->_children[curChild]);
					_ptr_mgr.push_back(PtrObjectForRegex(curNode));

					PushInt(curChild);
					curChild = 0;
					continue;
				}

				if (EmptyStack())
				{
					break;
				}

				curChild = PopInt();
				curNode = curNode->_next;
				_ptr_mgr.push_back(PtrObjectForRegex(curNode));

				EmitFragment(curNode->_type | AfterChild, curNode, curChild);
				curChild++;
			}

			PatchJump(0, CurPos());
			Emit(RegexCode::Stop);

			if (!_counting)
			{
				break;
			}

			_counting = false;
		}

		fcPrefix = RegexFCD::FirstChars(tree);

		 RegexFCD::Prefix(tree, prefix, is_prefix_empty);
		rtl = (((int)tree->_options & (int)RegexOptions::RightToLeft) != 0);

		ComCultureInfo* culture = ((int)tree->_options & (int)RegexOptions::CultureInvariant) != 0 ? ComCultureInfo::InvariantCulture() : ComCultureInfo::CurrentCulture();
		if (is_prefix_empty && prefix.getPrefix().length() > 0)
		{
			bmPrefix = new RegexBoyerMoore(prefix.getPrefix(), prefix.getCaseInsensitive(), rtl, culture);
		}
		else
		{
			bmPrefix = NULL;
		}

		anchors = RegexFCD::Anchors(tree);


		std::vector<std::uintptr_t> release;
		for (size_t i = 0; i < _ptr_mgr.size(); ++i)
		{
			bool need = true;
			for (int j = 0; j < release.size(); ++j)
			{
				if (_ptr_mgr[i]._address == release[j])
				{
					need = false;
					break;
				}
			}

			if (need)
			{
				if (_ptr_mgr[i].object)
				{
					release.push_back(_ptr_mgr[i]._address);
					delete _ptr_mgr[i].object;

				}
			}
		}

		delete bmPrefix;
		return new RegexCode(_emitted, _stringtable, _trackcount, _caps, capsize, bmPrefix, fcPrefix, anchors, rtl);
	}

	void RegexWriter::EmitFragment(const int& nodetype, RegexNode* node, int CurIndex)
	{
		int bits = 0;

		if (nodetype <= RegexCode::Ref)
		{
			if (node->UseOptionR())
			{
				bits |= RegexCode::Rtl;
			}
			if (((int)node->_options & (int)RegexOptions::IgnoreCase) != 0)
			{
				bits |= RegexCode::Ci;
			}
		}


		switch (nodetype)
		{
			//case RegexNode::Concatenate | BeforeChild:
			//case RegexNode::Concatenate | AfterChild:
			//case RegexNode::Empty:
		case 25 | 64:
		case 25 | 128:
		case 23:

			break;

			//case RegexNode::Alternate | BeforeChild:
		case 24 | 64:
			if ((size_t)CurIndex < node->_children.size() - 1)
			{
				PushInt(CurPos());
				Emit(RegexCode::Lazybranch, 0);
			}
			break;

			//case RegexNode::Alternate | AfterChild:
		case 24 | 128:
			{
				if ((size_t)CurIndex < node->_children.size() - 1)
				{
					int LBPos = PopInt();
					PushInt(CurPos());
					Emit(RegexCode::Goto, 0);
					PatchJump(LBPos, CurPos());
				}
				else
				{
					int I;
					for (I = 0; I < CurIndex; I++)
					{
						PatchJump(PopInt(), CurPos());
					}
				}
				break;
			}

			//case RegexNode::Testref | BeforeChild:
		case 33 | 64:
			switch (CurIndex)
			{
			case 0:
				Emit(RegexCode::Setjump);
				PushInt(CurPos());
				Emit(RegexCode::Lazybranch, 0);
				Emit(RegexCode::Testref, MapCapnum(node->_m));
				Emit(RegexCode::Forejump);
				break;
			default: ;
			}
			break;
			//case RegexNode::Testref | AfterChild:
		case 33 | 128:
			switch (CurIndex)
			{
			case 0:
				{
					int Branchpos = PopInt();
					PushInt(CurPos());
					Emit(RegexCode::Goto, 0);
					PatchJump(Branchpos, CurPos());
					Emit(RegexCode::Forejump);
					if (node->_children.size() > 1)
					{
						break;
					}
					// else fallthrough
					goto CaseLabel1;
				}
			case 1:
			CaseLabel1:
				PatchJump(PopInt(), CurPos());
				break;
			default: ;
			}
			break;

			//case RegexNode::Testgroup | BeforeChild:
		case 34 | 64:
			switch (CurIndex)
			{
			case 0:
				Emit(RegexCode::Setjump);
				Emit(RegexCode::Setmark);
				PushInt(CurPos());
				Emit(RegexCode::Lazybranch, 0);
				break;
			default: ;
			}
			break;

			//case RegexNode::Testgroup | AfterChild:
		case 34 | 128:
			switch (CurIndex)
			{
			case 0:
				Emit(RegexCode::Getmark);
				Emit(RegexCode::Forejump);
				break;
			case 1:
				{
					int Branchpos = PopInt();
					PushInt(CurPos());
					Emit(RegexCode::Goto, 0);
					PatchJump(Branchpos, CurPos());
					Emit(RegexCode::Getmark);
					Emit(RegexCode::Forejump);

					if (node->_children.size() > 2)
					{
						break;
					}
					// else fallthrough
					goto CaseLabel2;
				}
			case 2:
			CaseLabel2:
				PatchJump(PopInt(), CurPos());
				break;
			default: ;
			}
			break;

			//case RegexNode::Loop | BeforeChild:
			//case RegexNode::Lazyloop | BeforeChild:
		case 26 | 64:
		case 27 | 64:

			if (node->_n < INT_MAX || node->_m > 1)
			{
				Emit(node->_m == 0 ? RegexCode::Nullcount : RegexCode::Setcount, node->_m == 0 ? 0 : 1 - node->_m);
			}
			else
			{
				Emit(node->_m == 0 ? RegexCode::Nullmark : RegexCode::Setmark);
			}

			if (node->_m == 0)
			{
				PushInt(CurPos());
				Emit(RegexCode::Goto, 0);
			}

			PushInt(CurPos());
			break;

			//case RegexNode::Loop | AfterChild:
			//case RegexNode::Lazyloop | AfterChild:
		case 26 | 128:
		case 27 | 128:
			{
				int StartJumpPos = CurPos();
				int Lazy = (nodetype - (RegexNode::Loop | AfterChild));

				if (node->_n < INT_MAX || node->_m > 1)
				{
					Emit(RegexCode::Branchcount + Lazy, PopInt(), node->_n == INT_MAX ? INT_MAX : node->_n - node->_m);
				}
				else
				{
					Emit(RegexCode::Branchmark + Lazy, PopInt());
				}

				if (node->_m == 0)
				{
					PatchJump(PopInt(), StartJumpPos);
				}
			}
			break;

			//case RegexNode::Group | BeforeChild:
			//case RegexNode::Group | AfterChild:
		case 29 | 64:
		case 29 | 128:
			break;

			//case RegexNode::Capture | BeforeChild:
		case 28 | 64:
			Emit(RegexCode::Setmark);
			break;

			//case RegexNode::Capture | AfterChild:
		case 28 | 128:
			Emit(RegexCode::Capturemark, MapCapnum(node->_m), MapCapnum(node->_n));
			break;

			//case RegexNode::Require | BeforeChild:
		case 30 | 64:
			// NOTE: the following line causes lookahead/lookbehind to be
			// NON-BACKTRACKING. It can be commented out with (*)
			Emit(RegexCode::Setjump);


			Emit(RegexCode::Setmark);
			break;

			//case RegexNode::Require | AfterChild:
		case 30 | 128:
			Emit(RegexCode::Getmark);

			// NOTE: the following line causes lookahead/lookbehind to be
			// NON-BACKTRACKING. It can be commented out with (*)
			Emit(RegexCode::Forejump);

			break;

			//case RegexNode::Prevent | BeforeChild:
		case 31 | 64:
			Emit(RegexCode::Setjump);
			PushInt(CurPos());
			Emit(RegexCode::Lazybranch, 0);
			break;

			//case RegexNode::Prevent | AfterChild:
		case 31 | 128:
			Emit(RegexCode::Backjump);
			PatchJump(PopInt(), CurPos());
			Emit(RegexCode::Forejump);
			break;

			//case RegexNode::Greedy | BeforeChild:
		case 32 | 64:
			Emit(RegexCode::Setjump);
			break;

			//case RegexNode::Greedy | AfterChild:
		case 32 | 128:
			Emit(RegexCode::Forejump);
			break;

			//case RegexCode::One:
			//case RegexCode::Notone:
		case 9:
		case 10:
			Emit(node->_type | bits, static_cast<int>(node->_ch));
			break;

			//case RegexCode::Notoneloop:
			//case RegexCode::Notonelazy:
			//case RegexCode::Oneloop:
			//case RegexCode::Onelazy:
		case 4:
		case 7:
		case 3:
		case 6:
			if (node->_m > 0)
			{
				Emit(((node->_type == RegexCode::Oneloop || node->_type == RegexCode::Onelazy) ? RegexCode::Onerep : RegexCode::Notonerep) | bits, static_cast<int>(node->_ch), node->_m);
			}
			if (node->_n > node->_m)
			{
				Emit(node->_type | bits, static_cast<int>(node->_ch), node->_n == INT_MAX ? INT_MAX : node->_n - node->_m);
			}
			break;

			//case RegexCode::Setloop:
			//case RegexCode::Setlazy:
		case 5:
		case 8:
			if (node->_m > 0)
			{
				Emit(RegexCode::Setrep | bits, StringCode(node->_str), node->_m);
			}
			if (node->_n > node->_m)
			{
				Emit(node->_type | bits, StringCode(node->_str), (node->_n == INT_MAX) ? INT_MAX : node->_n - node->_m);
			}
			break;

			//case RegexCode::Multi:
		case 12:
			Emit(node->_type | bits, StringCode(node->_str));
			break;

			//case RegexCode::Set:
		case 11:
			Emit(node->_type | bits, StringCode(node->_str));
			break;

			//case RegexCode::Ref:
		case 13:
			Emit(node->_type | bits, MapCapnum(node->_m));
			break;

			//case RegexNode::Nothing:
			//case RegexCode::Bol:
			//case RegexCode::Eol:
			//case RegexCode::Boundary:
			//case RegexCode::Nonboundary:
			//case RegexCode::ECMABoundary:
			//case RegexCode::NonECMABoundary:
			//case RegexCode::Beginning:
			//case RegexCode::Start:
			//case RegexCode::EndZ:
			//case RegexCode::End:
		case 22:
		case 14:
		case 15:
		case 16:
		case 17:
		case 41:
		case 42:
		case 18:
		case 19:
		case 20:
		case 21:
			Emit(node->_type);
			break;

		default:
			throw MakeException(L"UnexpectedOpcode" + std::to_wstring(nodetype));
		}
	}
}
