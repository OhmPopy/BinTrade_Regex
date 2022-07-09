#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "objectForRegex.h"
#include "RegexOptions.h"

// RegexTree is just a wrapper for a node tree with some
// global information attached.


namespace Regexs
{
	class RegexNode;

	class REGEX_X_API RegexTree final : public objectForRegex
	{
	public:
		virtual ~RegexTree();

		RegexTree(RegexNode* root, std::unordered_map<int, int>& caps, std::vector<int>& capnumlist, int captop, std::unordered_map<std::wstring, int>& capnames, std::vector<std::wstring>& capslist, RegexOptions opts, std::vector<PtrObjectForRegex> _ptr_mgr);

		RegexNode* _root;
		std::unordered_map<int, int> _caps;
		std::vector<int> _capnumlist;
		std::unordered_map<std::wstring, int> _capnames;
		std::vector<std::wstring> _capslist;
		RegexOptions _options ;
		int _captop;
		std::vector<PtrObjectForRegex> _ptr_mgr;
	};
}
