#include "RegexCode.h"
#include "RegexBoyerMoore.h"
#include "RegexFCD.h"

namespace Regexs
{
	int RegexCode::Onerep = 0;
	int RegexCode::Notonerep = 1;
	int RegexCode::Setrep = 2;

	int RegexCode::Oneloop = 3;
	int RegexCode::Notoneloop = 4;
	int RegexCode::Setloop = 5;

	int RegexCode::Onelazy = 6;
	int RegexCode::Notonelazy = 7;
	int RegexCode::Setlazy = 8;

	int RegexCode::One = 9;
	int RegexCode::Notone = 10;
	int RegexCode::Set = 11;

	int RegexCode::Multi = 12;
	int RegexCode::Ref = 13;

	int RegexCode::Bol = 14;
	int RegexCode::Eol = 15;
	int RegexCode::Boundary = 16;
	int RegexCode::Nonboundary = 17;
	int RegexCode::Beginning = 18;
	int RegexCode::Start = 19;
	int RegexCode::EndZ = 20;
	int RegexCode::End = 21;

	int RegexCode::Nothing = 22;

	int RegexCode::Lazybranch = 23;
	int RegexCode::Branchmark = 24;
	int RegexCode::Lazybranchmark = 25;
	int RegexCode::Nullcount = 26;
	int RegexCode::Setcount = 27;
	int RegexCode::Branchcount = 28;
	int RegexCode::Lazybranchcount = 29;
	int RegexCode::Nullmark = 30;
	int RegexCode::Setmark = 31;
	int RegexCode::Capturemark = 32;
	int RegexCode::Getmark = 33;
	int RegexCode::Setjump = 34;
	int RegexCode::Backjump = 35;
	int RegexCode::Forejump = 36;
	int RegexCode::Testref = 37;
	int RegexCode::Goto = 38;

	int RegexCode::Prune = 39;
	int RegexCode::Stop = 40;

	int RegexCode::ECMABoundary = 41;
	int RegexCode::NonECMABoundary = 42;


	int RegexCode::Mask = 63;
	int RegexCode::Rtl = 64;
	int RegexCode::Back = 128;
	int RegexCode::Back2 = 256;
	int RegexCode::Ci = 512;


	RegexCode::~RegexCode()
	{
		delete _fcPrefix;
		delete _bmPrefix;
	}

	RegexCode::RegexCode(std::vector<int>& codes, std::vector<std::wstring>& stringlist, int trackcount, std::unordered_map<int, int>& caps, int capsize, RegexBoyerMoore* bmPrefix, RegexPrefix* fcPrefix, int anchors, bool rightToLeft)
	{
		_codes = codes;
		_strings = stringlist;
		_trackcount = trackcount;
		_caps = caps;
		_capsize = capsize;
		_bmPrefix = bmPrefix;
		_fcPrefix = fcPrefix;
		_anchors = anchors;
		_rightToLeft = rightToLeft;
	}

	bool RegexCode::OpcodeBacktracks(int Op)
	{
		Op &= Mask;

		if (
			Op == Oneloop ||
			Op == Notoneloop ||
			Op == Setloop ||
			Op == Onelazy ||
			Op == Notonelazy ||
			Op == Setlazy ||
			Op == Lazybranch ||
			Op == Branchmark ||
			Op == Lazybranchmark ||
			Op == Nullcount ||
			Op == Setcount ||
			Op == Branchcount ||
			Op == Lazybranchcount ||
			Op == Setmark ||
			Op == Capturemark ||
			Op == Getmark ||
			Op == Setjump ||
			Op == Backjump ||
			Op == Forejump ||
			Op == Goto
		)
		{
			return true;
		}
		return false;
	}

	int RegexCode::OpcodeSize(int Opcode)
	{
		Opcode &= Mask;

		if (
			Opcode == Nothing ||
			Opcode == Bol ||
			Opcode == Eol ||
			Opcode == Boundary ||
			Opcode == Nonboundary ||
			Opcode == ECMABoundary ||
			Opcode == NonECMABoundary ||
			Opcode == Beginning ||
			Opcode == Start ||
			Opcode == EndZ ||
			Opcode == End ||

			Opcode == Nullmark ||
			Opcode == Setmark ||
			Opcode == Getmark ||
			Opcode == Setjump ||
			Opcode == Backjump ||
			Opcode == Forejump ||
			Opcode == Stop
		)
		{
			return 1;
		}
		else if (
			Opcode == One ||
			Opcode == Notone ||
			Opcode == Multi ||
			Opcode == Ref ||
			Opcode == Testref ||

			Opcode == Goto ||
			Opcode == Nullcount ||
			Opcode == Setcount ||
			Opcode == Lazybranch ||
			Opcode == Branchmark ||
			Opcode == Lazybranchmark ||
			Opcode == Prune ||
			Opcode == Set
		)
		{
			return 2;
		}
		else if (
			Opcode == Capturemark ||
			Opcode == Branchcount ||
			Opcode == Lazybranchcount ||

			Opcode == Onerep ||
			Opcode == Notonerep ||
			Opcode == Oneloop ||
			Opcode == Notoneloop ||
			Opcode == Onelazy ||
			Opcode == Notonelazy ||
			Opcode == Setlazy ||
			Opcode == Setrep ||
			Opcode == Setloop
		)
		{
			return 3;
		}
		throw MakeException(L"UnexpectedOpcode  Opcode");
	}

	std::invalid_argument RegexCode::MakeException(const std::wstring& message)
	{
		std::string temp = std::string(message.begin(), message.end());
		return std::invalid_argument(temp);
	}
}
