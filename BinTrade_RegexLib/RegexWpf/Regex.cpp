#include "Regex.h"

#include <unordered_set>

#include "Exception.h"
#include "RegexTree.h"
#include "RegexParser.h"
#include "RegexWriter.h"
#include "RegexReplacement.h"
#include "RegexInterpreter.h"
#include "RegexMatch.h"
#include "UtilsString.h"

namespace Regexs
{
	std::mutex Regex::_listManagerLock;
	const TimeSpan Regex::MaximumMatchTimeout = TimeSpan::FromMilliseconds(INT_MAX - 1);
	const TimeSpan Regex::InfiniteMatchTimeout = TimeSpan(0, 0, 0, 0, -1);
	const std::wstring Regex::DefaultMatchTimeout_ConfigKeyName = L"REGEX_DEFAULT_MATCH_TIMEOUT";
	const TimeSpan Regex::FallbackDefaultMatchTimeout = InfiniteMatchTimeout;
	const TimeSpan Regex::DefaultMatchTimeout = FallbackDefaultMatchTimeout;
	std::list<CachedCodeEntry*> Regex::livecode;
	int Regex::cacheSize = 15;

	std::wstring Regex::GetPattern() const
	{
		return pattern;
	}

	RegexOptions Regex::GetRoptions() const
	{
		return roptions;
	}

	void Regex::ResetAllStaticValue()
	{
		cacheSize = 15;
	}

	int Regex::GetCapsize() const
	{
		return capsize;
	}

	Regex::~Regex()
	{
		if (_auto_release)
		{
			Release();
		}
		//delete factory;//dont release

		//clear ptr
		CachedCodeEntry* cached = LookupCachedAndUpdate(key_cache, _useCache);
		if (cached == NULL)
		{
			SAFE_DELETE(code)
			SAFE_DELETE(runnerref)
			SAFE_DELETE(replref)
		}
	}

	void Regex::Release()
	{
		if (!_ready_release)
		{
			_ready_release = true;

			std::unordered_set<std::uintptr_t> release;
			for (size_t i = 0; i < _match_ptr.size(); ++i)
			{
				auto address = _match_ptr[i]._address;
				bool need = true;
				const bool is_in = release.find(address) != release.end();
				if (is_in)
					need = false;

				if (need)
				{
					if (_match_ptr[i].object)
					{
						release.insert(address);
						delete _match_ptr[i].object;
					}
				}
			}
		}
	}

	Regex::Regex(): factory(NULL), roptions(static_cast<RegexOptions>(0)), _useCache(false), runnerref(NULL), replref(NULL), code(NULL), refsInitialized(false)
	{
		// If a compiled-to-assembly RegEx was generated using an earlier version, then internalMatchTimeout will be uninitialised.
		// Let's do it here.
		// In distant future, when RegEx generated using pre Dev11 are not supported any more, we can remove this to aid performance:
		this->internalMatchTimeout = DefaultMatchTimeout;
	}

	Regex::Regex(const std::wstring& pattern) : Regex(pattern, RegexOptions::None, DefaultMatchTimeout, false)
	{
	}

	Regex::Regex(const std::wstring& pattern, RegexOptions options) : Regex(pattern, options, DefaultMatchTimeout, false)
	{
	}

	Regex::Regex(const std::wstring& pattern, RegexOptions options, TimeSpan matchTimeout) : Regex(pattern, options, matchTimeout, false)
	{
	}

	Regex::Regex(const std::wstring& pattern, RegexOptions options, TimeSpan matchTimeout, bool useCache): factory(NULL), roptions(static_cast<RegexOptions>(0)), _useCache(useCache), runnerref(NULL), replref(NULL), code(NULL), refsInitialized(false)
	{
		std::wstring cultureKey;

		if (options < RegexOptions::None || ((static_cast<int>(options)) >> MaxOptionShift) != 0)
		{
			throw  ArgumentOutOfRangeException("options");
		}
		if (((int)options & (int)RegexOptions::ECMAScript) != 0 && ((int)options & ~((int)RegexOptions::ECMAScript | (int)RegexOptions::IgnoreCase | (int)RegexOptions::Multiline | (int)RegexOptions::CultureInvariant)) != 0)
		{
			throw ArgumentOutOfRangeException("options");
		}

		ValidateMatchTimeout(matchTimeout);

		// Try to look up this regex in the cache.  We do this regardless of whether useCache is true since there's
		// really no reason not to. 
		if (((int)options & (int)RegexOptions::CultureInvariant) != 0)
		{
			cultureKey = ComCultureInfo::InvariantCulture()->CultureName; // "English (United States)"
		}
		else
		{
			cultureKey = ComCultureInfo::CurrentCulture()->CultureName;
		}

		const std::wstring key = std::to_wstring((static_cast<int>(options))) + L":" + cultureKey + L":" + pattern;
		CachedCodeEntry* cached = LookupCachedAndUpdate(key, useCache);

		this->key_cache = key;
		this->pattern = pattern;
		this->roptions = options;

		this->internalMatchTimeout = matchTimeout;

		if (cached == NULL)
		{
			// Parse the input
			RegexTree* tree = RegexParser::Parse(pattern, roptions);

			// Extract the relevant information
			capnames = tree->_capnames;
			capslist = tree->_capslist;
			code = RegexWriter::Write(tree);
			caps = code->_caps;
			capsize = code->_capsize;

			InitializeReferences();

			if (useCache)
			{
				cached = CacheCode(key);
			}


			delete tree;
		}
		else
		{
			caps = cached->_caps;
			capnames = cached->_capnames;
			capslist = cached->_capslist;
			capsize = cached->_capsize;
			code = cached->_code;
			factory = cached->_factory;
			runnerref = cached->_runnerref;
			replref = cached->_replref;
			refsInitialized = true;
		}
	}

	void Regex::ValidateMatchTimeout(TimeSpan matchTimeout)
	{
		if (InfiniteMatchTimeout == matchTimeout)
		{
			return;
		}

		// Change this to make sure timeout is not longer then Environment.Ticks cycle length:
		if (TimeSpan::Zero < matchTimeout && matchTimeout <= MaximumMatchTimeout)
		{
			return;
		}

		throw ArgumentOutOfRangeException("matchTimeout");
	}

	std::wstring Regex::Escape(const std::wstring& str)
	{
		return RegexParser::Escape(str);
	}

	std::wstring Regex::Unescape(const std::wstring& str)
	{
		return RegexParser::Unescape(str);
	}

	int Regex::getCacheSize()
	{
		return cacheSize;
	}

	void Regex::setCacheSize(int value)
	{
		if (value < 0)
		{
			throw  ArgumentOutOfRangeException("value");
		}

		_listManagerLock.lock();
		cacheSize = value;
		if (livecode.size() > (size_t)cacheSize)
		{
			while (livecode.size() > (size_t)cacheSize)
			{
				livecode.pop_back();
			}
		}
		_listManagerLock.unlock();
	}

	RegexOptions Regex::getOptions() const
	{
		return roptions;
	}

	TimeSpan Regex::getMatchTimeout() const
	{
		return internalMatchTimeout;
	}

	bool Regex::getRightToLeft() const
	{
		return UseOptionR();
	}

	std::wstring Regex::ToString() const
	{
		return pattern;
	}

	std::vector<std::wstring> Regex::GetGroupNames()
	{
		std::vector<std::wstring> result;

		if (capslist.empty())
		{
			const int max = capsize;
			result = std::vector<std::wstring>(max);

			for (int i = 0; i < max; i++)
			{
				result[i] = UtilsCultureInfo::IntToString(ComCultureInfo::InvariantCulture(), i);
			}
		}
		else
		{
			result = std::vector<std::wstring>(capslist.size());

			std::copy_n(capslist.begin(), capslist.size(), result.begin());
		}

		return result;
	}

	std::vector<int> Regex::GetGroupNumbers()
	{
		std::vector<int> result;

		if (caps.empty())
		{
			const int max = capsize;
			result = std::vector<int>(max);

			for (int i = 0; i < max; i++)
			{
				result[i] = i;
			}
		}
		else
		{
			result = std::vector<int>(caps.size());

			//IDictionaryEnumerator* de = caps.begin();
			//while (de->MoveNext())
			//{
			//	result[std::any_cast<int>(de->Value)] = std::any_cast<int>(de->Key);
			//	de++;
			//}

			for (const auto& it : caps)
			{
				result[it.second] = it.first;
			}
		}

		return result;
	}

	std::wstring Regex::GroupNameFromNumber(int i)
	{
		if (capslist.empty())
		{
			if (i >= 0 && i < capsize)
			{
				return UtilsCultureInfo::IntToString(ComCultureInfo::InvariantCulture(), i);
			}

			return L"";
		}
		else
		{
			if (!caps.empty())
			{
				if (caps.find(i) == caps.end())
				{
					return L"";
				}

				i = caps[i];
			}

			if (i >= 0 && (size_t)i < capslist.size())
			{
				return capslist[i];
			}

			return L"";
		}
	}

	int Regex::GroupNumberFromName(const std::wstring& name)
	{
		// look up name if we have a hashtable of names
		if (!capnames.empty())
		{
			if (capnames.find(name) == capnames.end())
			{
				return -1;
			}

			return capnames[name];
		}

		// convert to an int if it looks like a number
		int result = 0;
		for (size_t i = 0; i < name.length(); i++)
		{
			const wchar_t ch = name[i];

			if (ch > L'9' || ch < L'0')
			{
				return -1;
			}

			result *= 10;
			result += (ch - L'0');
		}

		// return int if it's in range
		if (result >= 0 && result < capsize)
		{
			return result;
		}

		return -1;
	}

	bool Regex::IsMatch(const std::wstring& input, const std::wstring& pattern)
	{
		return IsMatch(input, pattern, RegexOptions::None, DefaultMatchTimeout);
	}

	bool Regex::IsMatch(const std::wstring& input, const std::wstring& pattern, RegexOptions options)
	{
		return IsMatch(input, pattern, options, DefaultMatchTimeout);
	}

	bool Regex::IsMatch(const std::wstring& input, const std::wstring& pattern, RegexOptions options, TimeSpan matchTimeout)
	{
		Regex tempVar(pattern, options, matchTimeout, false);
		tempVar._auto_release = false;

		bool result = tempVar.IsMatch(input);

		tempVar._auto_release = true;
		tempVar.Release();


		return result;
	}

	bool Regex::IsMatch(const std::wstring& input)
	{
		return IsMatch(input, UseOptionR() ? input.length() : 0);
	}

	bool Regex::IsMatch(const std::wstring& input, int startat)
	{
		const std::wstring* text = new std::wstring(input);
		const auto result = (NULL == Run(true, -1, text, 0, input.length(), startat));
		delete text;
		return result;
	}

	Regexs::Match* Regex::Match(const std::wstring* input, const std::wstring& pattern)
	{
		return Match(input, pattern, RegexOptions::None, DefaultMatchTimeout);
	}

	Regexs::Match* Regex::Match(const std::wstring* input, const std::wstring& pattern, RegexOptions options)
	{
		return Match(input, pattern, options, DefaultMatchTimeout);
	}

	Regexs::Match* Regex::Match(const std::wstring* input, const std::wstring& pattern, RegexOptions options, TimeSpan matchTimeout)
	{
		Regex* tempVar = new Regex(pattern, options, matchTimeout, false);
		tempVar->_auto_release = false;
		Regexs::Match* match = tempVar->Match(input);
		if (match == Regexs::Match::_empty)
		{
			tempVar->_auto_release = true;
			delete tempVar;
		}
		return match;
	}

	Regexs::Match* Regex::Match(const std::wstring* input)
	{
		return Match(input, UseOptionR() ? input->length() : 0);
	}

	Regexs::Match* Regex::Match(const std::wstring* input, int startat)
	{
		return Run(false, -1, input, 0, input->length(), startat);
	}

	Regexs::Match* Regex::Match(const std::wstring* input, int beginning, int length)
	{
		return Run(false, -1, input, beginning, length, UseOptionR() ? beginning + length : beginning);
	}

	MatchCollection* Regex::Matches(const std::wstring* input, const std::wstring& pattern)
	{
		return Matches(input, pattern, RegexOptions::None, DefaultMatchTimeout);
	}

	MatchCollection* Regex::Matches(const std::wstring* input, const std::wstring& pattern, RegexOptions options)
	{
		return Matches(input, pattern, options, DefaultMatchTimeout);
	}

	MatchCollection* Regex::Matches(const std::wstring* input, const std::wstring& pattern, RegexOptions options, TimeSpan matchTimeout)
	{
		Regex* tempVar = new Regex(pattern, options, matchTimeout, false);
		tempVar->_auto_release = false;
		return tempVar->Matches(input);
	}

	MatchCollection* Regex::Matches(const std::wstring* input)
	{
		return Matches(input, UseOptionR() ? input->length() : 0);
	}

	MatchCollection* Regex::Matches(const std::wstring* input, int startat)
	{
		return new MatchCollection(this, input, 0, input->length(), startat);
	}

	std::wstring Regex::Replace(const std::wstring& input, const std::wstring& pattern, const std::wstring& replacement)
	{
		return Replace(input, pattern, replacement, RegexOptions::None, DefaultMatchTimeout);
	}

	std::wstring Regex::Replace(const std::wstring& input, const std::wstring& pattern, const std::wstring& replacement, RegexOptions options)
	{
		return Replace(input, pattern, replacement, options, DefaultMatchTimeout);
	}

	std::wstring Regex::Replace(const std::wstring& input, const std::wstring& pattern, const std::wstring& replacement, RegexOptions options, TimeSpan matchTimeout)
	{
		Regex tempVar(pattern, options, matchTimeout, false);
		return tempVar.Replace(input, replacement);
	}

	std::wstring Regex::Replace(const std::wstring& input, const std::wstring& replacement)
	{
		return Replace(input, replacement, -1, UseOptionR() ? input.length() : 0);
	}

	std::wstring Regex::Replace(const std::wstring& input, const std::wstring& replacement, int count)
	{
		return Replace(input, replacement, count, UseOptionR() ? input.length() : 0);
	}

	std::wstring Regex::Replace(const std::wstring& input, const std::wstring& replacement, int count, int startat)
	{
		// a little code to grab a cached parsed replacement object
		RegexReplacement* repl = NULL;

		if (repl == NULL || repl->getPattern() != replacement)
			repl = RegexParser::ParseReplacement(replacement, caps, capsize, capnames, this->roptions);

		auto result = repl->Replace(this, input, count, startat);
		delete repl;
		return result;
	}

	std::wstring Regex::Replace(const std::wstring& input, const std::wstring& pattern, MatchEvaluator evaluator)
	{
		return Replace(input, pattern, evaluator, RegexOptions::None, DefaultMatchTimeout);
	}

	std::wstring Regex::Replace(const std::wstring& input, const std::wstring& pattern, MatchEvaluator evaluator, RegexOptions options)
	{
		return Replace(input, pattern, evaluator, options, DefaultMatchTimeout);
	}

	std::wstring Regex::Replace(const std::wstring& input, const std::wstring& pattern, MatchEvaluator evaluator, RegexOptions options, TimeSpan matchTimeout)
	{
		Regex tempVar(pattern, options, matchTimeout, false);
		return tempVar.Replace(input, evaluator);
	}

	std::wstring Regex::Replace(const std::wstring& input, MatchEvaluator evaluator)
	{
		return Replace(input, evaluator, -1, UseOptionR() ? input.length() : 0);
	}

	std::wstring Regex::Replace(const std::wstring& input, MatchEvaluator evaluator, int count)
	{
		return Replace(input, evaluator, count, UseOptionR() ? input.length() : 0);
	}

	std::wstring Regex::Replace(const std::wstring& input, MatchEvaluator evaluator, int count, int startat)
	{
		return RegexReplacement::Replace(evaluator, this, input, count, startat);
	}

	std::vector<std::wstring> Regex::Split(const std::wstring& input, const std::wstring& pattern)
	{
		return Split(input, pattern, RegexOptions::None, DefaultMatchTimeout);
	}

	std::vector<std::wstring> Regex::Split(const std::wstring& input, const std::wstring& pattern, RegexOptions options)
	{
		return Split(input, pattern, options, DefaultMatchTimeout);
	}

	std::vector<std::wstring> Regex::Split(const std::wstring& input, const std::wstring& pattern, RegexOptions options, TimeSpan matchTimeout)
	{
		Regex tempVar(pattern, options, matchTimeout, false);
		return tempVar.Split(input);
	}

	std::vector<std::wstring> Regex::Split(const std::wstring& input)
	{
		return Split(input, 0, UseOptionR() ? input.length() : 0);
	}

	std::vector<std::wstring> Regex::Split(const std::wstring& input, int count)
	{
		return RegexReplacement::Split(this, input, count, UseOptionR() ? input.length() : 0);
	}

	std::vector<std::wstring> Regex::Split(const std::wstring& input, int count, int startat)
	{
		return RegexReplacement::Split(this, input, count, startat);
	}

	void Regex::InitializeReferences()
	{
		if (refsInitialized)
		{
			throw NotSupportedException("OnlyAllowedOnce");
		}

		refsInitialized = true;
		runnerref = new ExclusiveReference();
		replref = new SharedReference();
	}

	Regexs::Match* Regex::Run(bool quick, int prevlen, const std::wstring* input, int beginning, int length, int startat)
	{
		if (startat < 0 || (size_t)startat > input->length())
		{
			throw ArgumentOutOfRangeException("start BeginIndexNotNegative");
		}

		if (length < 0 || (size_t)length > input->length())
		{
			throw ArgumentOutOfRangeException("length LengthNotNegative");
		}

		// There may be a cached runner; grab ownership of it if we can.
		Regexs::Match* match = NULL;
		RegexRunner* runner = dynamic_cast<RegexRunner*>(runnerref->Get());

		// Create a RegexRunner instance if we need to
		if (runner == NULL)
		{
			// Use the compiled RegexRunner factory if the code was compiled to MSIL

			if (factory != NULL)
			{
				runner = factory->CreateInstance();
			}
			else
			{
				runner = new RegexInterpreter(code, UseOptionInvariant() ? ComCultureInfo::InvariantCulture() : ComCultureInfo::CurrentCulture());
			}
		}

		try
		{
			// Do the scan starting at the requested position            
			match = runner->Scan(this, input, beginning, beginning + length, startat, prevlen, quick, internalMatchTimeout);
		}
		catch (...)
		{
		}
		// Release or fill the cache slot
		runnerref->Release(runner);

		return match;
	}

	CachedCodeEntry* Regex::LookupCachedAndUpdate(const std::wstring& key, bool status_lock_cache)
	{
		if (status_lock_cache) return NULL;

		_listManagerLock.lock();

		for (std::list<CachedCodeEntry*>::iterator it = livecode.begin(); it != livecode.end(); ++it)
		{
			const auto current = it.operator*();
			if (current->_key == key)
			{
				// If we find an entry in the cache, move it to the head at the same time. 
				livecode.remove(current);
				livecode.push_front(current);
				_listManagerLock.unlock();
				return current;
			}
		}

		//for (LinkedListNode<CachedCodeEntry*>* current = livecode.First; current != NULL; current = current->Next)
		//{
		//	if (current->Value->_key == key)
		//	{
		//		// If we find an entry in the cache, move it to the head at the same time. 
		//		livecode.remove(current);
		//		livecode.push_front(current);
		//		_listManagerLock.unlock();
		//		return current->Value;
		//	}
		//}

		_listManagerLock.unlock();
		return NULL;
	}

	CachedCodeEntry* Regex::CacheCode(const std::wstring& key)
	{
		CachedCodeEntry* newcached = NULL;

		_listManagerLock.lock();

		// first look for it in the cache and move it to the head

		for (std::list<CachedCodeEntry*>::iterator it = livecode.begin(); it != livecode.end(); ++it)
		{
			const auto current = it.operator*();
			if (current->_key == key)
			{
				livecode.remove(current);
				livecode.push_front(current);
				return current;
			}
		}

		//for (LinkedListNode<CachedCodeEntry*>* current = livecode.First; current != NULL; current = current->Next)
		//{
		//	if (current->Value->_key == key)
		//	{
		//		livecode.remove(current);
		//		livecode.push_front(current);
		//		return current->Value;
		//	}
		//}

		// it wasn't in the cache, so we'll add a new one.  Shortcut out for the case where cacheSize is zero.
		if (cacheSize != 0)
		{
			newcached = new CachedCodeEntry(key, capnames, capslist, code, caps, capsize, runnerref, replref);
			livecode.push_front(newcached);
			if (livecode.size() > (size_t)cacheSize)
			{
				livecode.pop_back();
			}
		}

		_listManagerLock.unlock();

		return newcached;
	}

	bool Regex::UseOptionR() const
	{
		return ((int)roptions & (int)RegexOptions::RightToLeft) != 0;
	}

	bool Regex::UseOptionInvariant() const
	{
		return ((int)roptions & (int)RegexOptions::CultureInvariant) != 0;
	}

	CachedCodeEntry::~CachedCodeEntry()
	{
		//delete _code;
		//delete _factory;
		//delete _runnerref;
		//delete _replref;
	}

	CachedCodeEntry::CachedCodeEntry(const std::wstring& key, std::unordered_map<std::wstring, int>& capnames, std::vector<std::wstring>& capslist, RegexCode* code, std::unordered_map<int, int>& caps, int capsize, ExclusiveReference* runner, SharedReference* repl): _factory(NULL)
	{
		_key = key;
		_capnames = capnames;
		_capslist = capslist;

		_code = code;
		_caps = caps;
		_capsize = capsize;

		_runnerref = runner;
		_replref = repl;
	}

	ExclusiveReference::ExclusiveReference(): _ref(NULL), _obj(NULL), _locked(0)
	{
	}

	ExclusiveReference::~ExclusiveReference()
	{
		delete _ref;
	}

	objectForRegex* ExclusiveReference::Get()
	{
		// try to obtain the lock

		//if (0 == Interlocked::Exchange(_locked, 1))//bdki ???
		{
			// grab reference


			objectForRegex* obj = _ref;

			// release the lock and return null if no reference

			if (obj == NULL)
			{
				_locked = 0;
				return NULL;
			}

			// remember the reference and keep the lock

			_obj = obj;
			return obj;
		}

		return NULL;
	}

	void ExclusiveReference::Release(objectForRegex* obj)
	{
		if (obj == NULL)
		{
			throw std::invalid_argument("obj");
		}

		// if this reference owns the lock, release it

		if (_obj == obj)
		{
			_obj = NULL;
			_locked = 0;
			return;
		}

		// if no reference owns the lock, try to cache this reference

		if (_obj == NULL)
		{
			// try to obtain the lock

			//if (0 == Interlocked::Exchange(_locked, 1))//bdki ???
			{
				// if there's really no reference, cache this reference

				if (_ref == NULL)
				{
					_ref = dynamic_cast<RegexRunner*>(obj);
				}

				// release the lock

				_locked = 0;
				return;
			}
		}
	}

	SharedReference::~SharedReference()
	{
		delete _ref;
	}

	objectForRegex* SharedReference::Get()
	{
		return NULL;
	}

}
