#include "RegexTree.h"
#include "RegexNode.h"

namespace Regexs
{
	RegexTree::~RegexTree()
	{
	}

	RegexTree::RegexTree(RegexNode* root, std::unordered_map<int, int>& caps, std::vector<int>& capnumlist, int captop, std::unordered_map<std::wstring, int>& capnames, std::vector<std::wstring>& capslist, RegexOptions opts, std::vector<PtrObjectForRegex> _ptr_mgr): _captop(0), _ptr_mgr(_ptr_mgr)
	{
		_root = root;
		_caps = caps;
		_capnumlist = capnumlist;
		_capnames = capnames;
		_capslist = capslist;
		_captop = captop;
		_options = opts;
	}
}
