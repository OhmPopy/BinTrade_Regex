#pragma once

#include "RegexRunnerFactory.h"
#include "RegexOptions.h"
#include "RegexCode.h"
#include "RegexMatchCollection.h"
#include "RegexRunner.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <list>
#include <mutex>

#include "objectForRegex.h"
#include "TypeDef.h"
#include "TimeSpan.h"


namespace Regexs
{
	class ExclusiveReference;
	class SharedReference;
	class CachedCodeEntry;

	/// <devdoc>
	///    <para>
	///       Represents an immutable, compiled regular expression. Also
	///       contains static methods that allow use of regular expressions without instantiating
	///       a Regex explicitly.
	///    </para>
	/// </devdoc>
	 class REGEX_X_API Regex : public objectForRegex
	{
		static int cacheSize;
		static std::mutex _listManagerLock;
		std::vector<PtrObjectForRegex> _match_ptr;

	private:
		// Fields used by precompiled regexes
		std::wstring pattern;
		std::wstring key_cache;

	public:
		RegexRunnerFactory* factory; // if compiled, this is the RegexRunner subclass
		std::wstring GetPattern() const;
		RegexOptions GetRoptions() const;
		static void ResetAllStaticValue();
	protected:
		RegexOptions roptions; // = static_cast<RegexOptions>(0); // the top-level options from the options string
		std::vector<std::wstring> capslist; // if captures are sparse or named captures are used, this is the sorted list of names

		int capsize = 0; // the size of the capture array

	private:
		// We need this because time is queried using Environment.TickCount for performance reasons
		// (Environment.TickCount returns millisecs as an int and cycles):
		static const Regexs::TimeSpan MaximumMatchTimeout;

		// During static initialisation of Regex we check 
		static const std::wstring DefaultMatchTimeout_ConfigKeyName;

		bool _auto_release = true;
		bool _ready_release = false;
		bool _useCache ; //static method not use static will leak memory

	public:
		// InfiniteMatchTimeout specifies that match timeout is switched OFF. It allows for faster code paths
		// compared to simply having a very large timeout.
		// We do not want to ask users to use System.Threading.Timeout.InfiniteTimeSpan as a parameter because:
		//   (1) We do not want to imply any relation between having using a RegEx timeout and using multi-threading.
		//   (2) We do not want to require users to take ref to a contract assembly for threading just to use RegEx.
		//       There may in theory be a SKU that has RegEx, but no multithreading.
		// We create a public Regex.InfiniteMatchTimeout constant, which for consistency uses the save underlying
		// value as Timeout.InfiniteTimeSpan creating an implementation detail dependency only.
		static const Regexs::TimeSpan InfiniteMatchTimeout;

		// All these protected internal fields in this class really should not be protected. The historic reason
		// for this is that classes extending Regex that are generated via CompileToAssembly rely on the fact that
		// these are accessible as protected in order to initialise them in the generated constructor of the
		// extending class. We should update this initialisation logic to using a protected constructor, but until
		// that is done we stick to the existing pattern however ugly it may be.
		Regexs::TimeSpan internalMatchTimeout; // timeout for the execution of this regex

		// FallbackDefaultMatchTimeout specifies the match timeout to use if no other timeout was specified
		// by one means or another. For now it is set to InfiniteMatchTimeout, meaning timeouts are OFF by
		// default (for Dev12 we plan to set a positive value).
		// Having this field is helpful to read the code as it makes it clear when we mean
		// "default that is currently no-timeouts" and when we mean "actually no-timeouts".
		// In Silverlight, DefaultMatchTimeout is always set to FallbackDefaultMatchTimeout,
		// on desktop, DefaultMatchTimeout can be configured via AppDomain and falls back to
		// FallbackDefaultMatchTimeout, if no AppDomain setting is present (see InitDefaultMatchTimeout()).
		static const Regexs::TimeSpan FallbackDefaultMatchTimeout;

		// DefaultMatchTimeout specifies the match timeout to use if no other timeout was specified
		// by one means or another. Typically, it is set to InfiniteMatchTimeout in Dev 11
		// (we plan to set a positive timeout in Dev12).
		// Hosts (e.g.) ASP may set an AppDomain property via SetData to change the default value.        
		static const Regexs::TimeSpan DefaultMatchTimeout;

		std::unordered_map<int, int> caps; // if captures are sparse, this is the hashtable capnum->index
		std::unordered_map<std::wstring, int> capnames; // if named captures are used, this maps names->index

		int GetCapsize() const;
		ExclusiveReference* runnerref; // cached runner
		SharedReference* replref; // cached parsed replacement pattern
		RegexCode* code; // if interpreted, this is the code for RegexIntepreter
		bool refsInitialized;

		static std::list<CachedCodeEntry*> livecode; // the cached of code and factories that are currently loaded

		static constexpr int MaxOptionShift = 10;

		virtual ~Regex();

		void Release();


	protected:
		Regex();
	public:
		/*
		 * Compiles and returns a Regex object corresponding to the given pattern
		 */
		/// <devdoc>
		///    <para>
		///       Creates and compiles a regular expression object for the specified regular
		///       expression.
		///    </para>
		/// </devdoc>
		Regex(const std::wstring& pattern);

		/*
		 * Returns a Regex object corresponding to the given pattern, compiled with
		 * the specified options.
		 */
		/// <devdoc>
		///    <para>
		///       Creates and compiles a regular expression object for the
		///       specified regular expression
		///       with options that modify the pattern.
		///    </para>
		/// </devdoc>
		Regex(const std::wstring& pattern, RegexOptions options);

		Regex(const std::wstring& pattern, RegexOptions options, Regexs::TimeSpan matchTimeout);
	private:
		Regex(const std::wstring& pattern, RegexOptions options, Regexs::TimeSpan matchTimeout, bool useCache);


	public:
		//* Note: "&lt;" is the XML entity for smaller ("<").
		/// <summary>
		/// Validates that the specified match timeout value is valid.
		/// The valid range is <code>TimeSpan.Zero &lt; matchTimeout &lt;= Regex.MaximumMatchTimeout</code>.
		/// </summary>
		/// <param name="matchTimeout">The timeout value to validate.</param>
		/// <exception cref="System.ArgumentOutOfRangeException">If the specified timeout is not within a valid range.        
		/// </exception>
		static void ValidateMatchTimeout(Regexs::TimeSpan matchTimeout);


		/*
		 * Escape metacharacters within the string
		 */
		/// <devdoc>
		///    <para>
		///       Escapes 
		///          a minimal set of metacharacters (\, *, +, ?, |, {, [, (, ), ^, $, ., #, and
		///          whitespace) by replacing them with their \ codes. This converts a string so that
		///          it can be used as a constant within a regular expression safely. (Note that the
		///          reason # and whitespace must be escaped is so the string can be used safely
		///          within an expression parsed with x mode. If future Regex features add
		///          additional metacharacters, developers should depend on Escape to escape those
		///          characters as well.)
		///       </para>
		///    </devdoc>
		static std::wstring Escape(const std::wstring& str);

		/*
		 * Unescape character codes within the string
		 */
		/// <devdoc>
		///    <para>
		///       Unescapes any escaped characters in the input string.
		///    </para>
		/// </devdoc>
		static std::wstring Unescape(const std::wstring& str);

		static int getCacheSize();
		static void setCacheSize(int value);

		/// <devdoc>
		///    <para>
		///       Returns the options passed into the constructor
		///    </para>
		/// </devdoc>
		RegexOptions getOptions() const;


		/// <summary>
		/// The match timeout used by this Regex instance.
		/// </summary>
		Regexs::TimeSpan getMatchTimeout() const;


		/*
		 * True if the regex is leftward
		 */
		/// <devdoc>
		///    <para>
		///       Indicates whether the regular expression matches from right to
		///       left.
		///    </para>
		/// </devdoc>
		bool getRightToLeft() const;

		/// <devdoc>
		///    <para>
		///       Returns the regular expression pattern passed into the constructor
		///    </para>
		/// </devdoc>
		std::wstring ToString() const;

		/*
		 * Returns an array of the group names that are used to capture groups
		 * in the regular expression. Only needed if the regex is not known until
		 * runtime, and one wants to extract captured groups. (Probably unusual,
		 * but supplied for completeness.)
		 */
		/// <devdoc>
		///    Returns 
		///       the GroupNameCollection for the regular expression. This collection contains the
		///       set of strings used to name capturing groups in the expression. 
		///    </devdoc>
		std::vector<std::wstring> GetGroupNames();

		/*
		 * Returns an array of the group numbers that are used to capture groups
		 * in the regular expression. Only needed if the regex is not known until
		 * runtime, and one wants to extract captured groups. (Probably unusual,
		 * but supplied for completeness.)
		 */
		/// <devdoc>
		///    returns 
		///       the integer group number corresponding to a group name. 
		///    </devdoc>
		std::vector<int> GetGroupNumbers();

		/*
		 * Given a group number, maps it to a group name. Note that nubmered
		 * groups automatically get a group name that is the decimal string
		 * equivalent of its number.
		 *
		 * Returns null if the number is not a recognized group number.
		 */
		/// <devdoc>
		///    <para>
		///       Retrieves a group name that corresponds to a group number.
		///    </para>
		/// </devdoc>
		std::wstring GroupNameFromNumber(int i);

		/*
		 * Given a group name, maps it to a group number. Note that nubmered
		 * groups automatically get a group name that is the decimal string
		 * equivalent of its number.
		 *
		 * Returns -1 if the name is not a recognized group name.
		 */
		/// <devdoc>
		///    <para>
		///       Returns a group number that corresponds to a group name.
		///    </para>
		/// </devdoc>
		int GroupNumberFromName(const std::wstring& name);

		/*
		 * Static version of simple IsMatch call
		 */
		///    <devdoc>
		///       <para>
		///          Searches the input 
		///             string for one or more occurrences of the text supplied in the pattern
		///             parameter.
		///       </para>
		///    </devdoc>
		static bool IsMatch(const std::wstring& input, const std::wstring& pattern);

		/*
		 * Static version of simple IsMatch call
		 */
		/// <devdoc>
		///    <para>
		///       Searches the input string for one or more occurrences of the text 
		///          supplied in the pattern parameter with matching options supplied in the options
		///          parameter.
		///       </para>
		///    </devdoc>
		static bool IsMatch(const std::wstring& input, const std::wstring& pattern, RegexOptions options);

		static bool IsMatch(const std::wstring& input, const std::wstring& pattern, RegexOptions options, Regexs::TimeSpan matchTimeout);

		/*
		 * Returns true if the regex finds a match within the specified string
		 */
		/// <devdoc>
		///    <para>
		///       Searches the input string for one or 
		///          more matches using the previous pattern, options, and starting
		///          position.
		///       </para>
		///    </devdoc>
		bool IsMatch(const std::wstring& input);

		/*
		 * Returns true if the regex finds a match after the specified position
		 * (proceeding leftward if the regex is leftward and rightward otherwise)
		 */
		/// <devdoc>
		///    <para>
		///       Searches the input 
		///          string for one or more matches using the previous pattern and options, with
		///          a new starting position.
		///    </para>
		/// </devdoc>
		bool IsMatch(const std::wstring& input, int startat);

		/*
		 * Static version of simple Match call
		 */
		///    <devdoc>
		///       <para>
		///          Searches the input string for one or more occurrences of the text 
		///             supplied in the pattern parameter.
		///       </para>
		///    </devdoc>
		static Regexs::Match* Match(const std::wstring* input, const std::wstring& pattern);

		/*
		 * Static version of simple Match call
		 */
		/// <devdoc>
		///    <para>
		///       Searches the input string for one or more occurrences of the text 
		///          supplied in the pattern parameter. Matching is modified with an option
		///          string.
		///       </para>
		///    </devdoc>
		static Regexs::Match* Match(const std::wstring* input, const std::wstring& pattern, RegexOptions options);

		static Regexs::Match* Match(const std::wstring* input, const std::wstring& pattern, RegexOptions options, Regexs::TimeSpan matchTimeout);

		/*
		 * Finds the first match for the regular expression starting at the beginning
		 * of the string (or at the end of the string if the regex is leftward)
		 */
		/// <devdoc>
		///    <para>
		///       Matches a regular expression with a string and returns
		///       the precise result as a RegexMatch object.
		///    </para>
		/// </devdoc>
		Regexs::Match* Match(const std::wstring* input);

		/*
		 * Finds the first match, starting at the specified position
		 */
		/// <devdoc>
		///    Matches a regular expression with a string and returns
		///    the precise result as a RegexMatch object.
		/// </devdoc>
		Regexs::Match* Match(const std::wstring* input, int startat);

		/*
		 * Finds the first match, restricting the search to the specified interval of
		 * the char array.
		 */
		/// <devdoc>
		///    <para>
		///       Matches a
		///       regular expression with a string and returns the precise result as a
		///       RegexMatch object.
		///    </para>
		/// </devdoc>
		Regexs::Match* Match(const std::wstring* input, int beginning, int length);

		/*
		 * Static version of simple Matches call
		 */
		///    <devdoc>
		///       <para>
		///          Returns all the successful matches as if Match were
		///          called iteratively numerous times.
		///       </para>
		///    </devdoc>
		static MatchCollection* Matches(const std::wstring* input, const std::wstring& pattern);

		/*
		 * Static version of simple Matches call
		 */
		/// <devdoc>
		///    <para>
		///       Returns all the successful matches as if Match were called iteratively
		///       numerous times.
		///    </para>
		/// </devdoc>
		static MatchCollection* Matches(const std::wstring* input, const std::wstring& pattern, RegexOptions options);

		static MatchCollection* Matches(const std::wstring* input, const std::wstring& pattern, RegexOptions options, Regexs::TimeSpan matchTimeout);

		/*
		 * Finds the first match for the regular expression starting at the beginning
		 * of the string Enumerator(or at the end of the string if the regex is leftward)
		 */
		/// <devdoc>
		///    <para>
		///       Returns
		///       all the successful matches as if Match was called iteratively numerous
		///       times.
		///    </para>
		/// </devdoc>
		MatchCollection* Matches(const std::wstring* input);

		/*
		 * Finds the first match, starting at the specified position
		 */
		/// <devdoc>
		///    <para>
		///       Returns
		///       all the successful matches as if Match was called iteratively numerous
		///       times.
		///    </para>
		/// </devdoc>
		MatchCollection* Matches(const std::wstring* input, int startat);

		/*
		 * Static version of simple Replace call
		 */
		/// <devdoc>
		///    <para>
		///       Replaces 
		///          all occurrences of the pattern with the <paramref name="replacement"/> pattern, starting at
		///          the first character in the input string. 
		///       </para>
		///    </devdoc>
		static std::wstring Replace(const std::wstring& input, const std::wstring& pattern, const std::wstring& replacement);

		/*
		 * Static version of simple Replace call
		 */
		/// <devdoc>
		///    <para>
		///       Replaces all occurrences of 
		///          the <paramref name="pattern "/>with the <paramref name="replacement "/>
		///          pattern, starting at the first character in the input string. 
		///       </para>
		///    </devdoc>
		static std::wstring Replace(const std::wstring& input, const std::wstring& pattern, const std::wstring& replacement, RegexOptions options);

		static std::wstring Replace(const std::wstring& input, const std::wstring& pattern, const std::wstring& replacement, RegexOptions options, Regexs::TimeSpan matchTimeout);

		/*
		 * Does the replacement
		 */
		/// <devdoc>
		///    <para>
		///       Replaces all occurrences of 
		///          the <paramref name="pattern "/> with the <paramref name="replacement"/> pattern, starting at the
		///          first character in the input string, using the previous patten. 
		///       </para>
		///    </devdoc>
		std::wstring Replace(const std::wstring& input, const std::wstring& replacement);

		/*
		 * Does the replacement
		 */
		/// <devdoc>
		///    <para>
		///    Replaces all occurrences of the (previously defined) <paramref name="pattern "/>with the 
		///    <paramref name="replacement"/> pattern, starting at the first character in the input string. 
		/// </para>
		/// </devdoc>
		std::wstring Replace(const std::wstring& input, const std::wstring& replacement, int count);

		/*
		 * Does the replacement
		 */
		/// <devdoc>
		///    <para>
		///    Replaces all occurrences of the <paramref name="pattern "/>with the recent 
		///    <paramref name="replacement"/> pattern, starting at the character position 
		///    <paramref name="startat."/>
		/// </para>
		/// </devdoc>
		std::wstring Replace(const std::wstring& input, const std::wstring& replacement, int count, int startat);

		/*
		 * Static version of simple Replace call
		 */
		/// <devdoc>
		///    <para>
		///    Replaces all occurrences of the <paramref name="pattern "/>with the 
		///    <paramref name="replacement"/> pattern 
		///    <paramref name="."/>
		/// </para>
		/// </devdoc>
		static std::wstring Replace(const std::wstring& input, const std::wstring& pattern, MatchEvaluator evaluator);

		/*
		 * Static version of simple Replace call
		 */
		/// <devdoc>
		///    <para>
		///    Replaces all occurrences of the <paramref name="pattern "/>with the recent 
		///    <paramref name="replacement"/> pattern, starting at the first character<paramref name="."/>
		/// </para>
		/// </devdoc>
		static std::wstring Replace(const std::wstring& input, const std::wstring& pattern, MatchEvaluator evaluator, RegexOptions options);


		static std::wstring Replace(const std::wstring& input, const std::wstring& pattern, MatchEvaluator evaluator, RegexOptions options, Regexs::TimeSpan matchTimeout);

		/*
		 * Does the replacement
		 */
		/// <devdoc>
		///    <para>
		///    Replaces all occurrences of the <paramref name="pattern "/>with the recent 
		///    <paramref name="replacement"/> pattern, starting at the first character 
		///    position<paramref name="."/>
		/// </para>
		/// </devdoc>
		std::wstring Replace(const std::wstring& input, MatchEvaluator evaluator);

		/*
		 * Does the replacement
		 */
		/// <devdoc>
		///    <para>
		///    Replaces all occurrences of the <paramref name="pattern "/>with the recent 
		///    <paramref name="replacement"/> pattern, starting at the first character 
		///    position<paramref name="."/>
		/// </para>
		/// </devdoc>
		std::wstring Replace(const std::wstring& input, MatchEvaluator evaluator, int count);

		/*
		 * Does the replacement
		 */
		/// <devdoc>
		///    <para>
		///    Replaces all occurrences of the (previouly defined) <paramref name="pattern "/>with 
		///       the recent <paramref name="replacement"/> pattern, starting at the character
		///    position<paramref name=" startat."/> 
		/// </para>
		/// </devdoc>
		std::wstring Replace(const std::wstring& input, MatchEvaluator evaluator, int count, int startat);

		/*
		 * Static version of simple Split call
		 */
		///    <devdoc>
		///       <para>
		///          Splits the <paramref name="input "/>string at the position defined
		///          by <paramref name="pattern"/>.
		///       </para>
		///    </devdoc>
		static std::vector<std::wstring> Split(const std::wstring& input, const std::wstring& pattern);

		/*
		 * Static version of simple Split call
		 */
		/// <devdoc>
		///    <para>
		///       Splits the <paramref name="input "/>string at the position defined by <paramref name="pattern"/>.
		///    </para>
		/// </devdoc>
		static std::vector<std::wstring> Split(const std::wstring& input, const std::wstring& pattern, RegexOptions options);


		static std::vector<std::wstring> Split(const std::wstring& input, const std::wstring& pattern, RegexOptions options, Regexs::TimeSpan matchTimeout);

		/*
		 * Does a split
		 */
		/// <devdoc>
		///    <para>
		///       Splits the <paramref name="input "/>string at the position defined by
		///       a previous <paramref name="pattern"/>
		///       .
		///    </para>
		/// </devdoc>
		std::vector<std::wstring> Split(const std::wstring& input);

		/*
		 * Does a split
		 */
		/// <devdoc>
		///    <para>
		///       Splits the <paramref name="input "/>string at the position defined by a previous
		///    <paramref name="pattern"/> . 
		///    </para>
		/// </devdoc>
		std::vector<std::wstring> Split(const std::wstring& input, int count);

		/*
		 * Does a split
		 */
		/// <devdoc>
		///    <para>
		///       Splits the <paramref name="input "/>string at the position defined by a previous
		///    <paramref name="pattern"/> . 
		///    </para>
		/// </devdoc>
		std::vector<std::wstring> Split(const std::wstring& input, int count, int startat);

	protected:
		/// <devdoc>
		/// </devdoc>
		void InitializeReferences();

	public:
		/*
		 * Internal worker called by all the public APIs
		 */
		Regexs::Match* Run(bool quick, int prevlen, const std::wstring* input, int beginning, int length, int startat);

	private:
		/*
		 * Find code cache based on options+pattern
		 */
		static CachedCodeEntry* LookupCachedAndUpdate(const std::wstring& key, bool status_lock_cache);

		/*
		 * Add current code to the cache
		 */
		CachedCodeEntry* CacheCode(const std::wstring& key);

	protected:
		/*
		 * True if the L option was set
		 */
		/// <internalonly/>
		/// <devdoc>
		/// </devdoc>
		bool UseOptionR() const;

	public:
		bool UseOptionInvariant() const;

		friend class Match;
		friend class MatchSparse;
		friend class MatchCollection;
		friend class MatchEnumerator;
		friend class GroupCollection;
		friend class GroupEnumerator;
		friend class CaptureEnumerator;
	};


	/*
	 * Used to cache byte codes or compiled factories
	 */
	class CachedCodeEntry final
	{
	public:
		std::wstring _key;
		RegexCode* _code;
		std::unordered_map<int, int> _caps;
		std::unordered_map<std::wstring, int> _capnames;
		std::vector<std::wstring> _capslist;
		int _capsize = 0;
		RegexRunnerFactory* _factory;
		ExclusiveReference* _runnerref;
		SharedReference* _replref;

		~CachedCodeEntry();

		CachedCodeEntry(const std::wstring& key, std::unordered_map<std::wstring, int>& capnames, std::vector<std::wstring>& capslist, RegexCode* code, std::unordered_map<int, int>& caps, int capsize, ExclusiveReference* runner, SharedReference* repl);
	};

	/*
	 * Used to cache one exclusive runner reference
	 */
	class ExclusiveReference final
	{
		RegexRunner* _ref;
		objectForRegex* _obj;
		int _locked;

	public:
		/*
		 * Return an object and grab an exclusive lock.
		 *
		 * If the exclusive lock can't be obtained, null is returned;
		 * if the object can't be returned, the lock is released.
		 *
		 */
		ExclusiveReference();

		~ExclusiveReference();

		objectForRegex* Get();

		/*
		 * Release an object back to the cache
		 *
		 * If the object is the one that's under lock, the lock
		 * is released.
		 *
		 * If there is no cached object, then the lock is obtained
		 * and the object is placed in the cache.
		 *
		 */
		void Release(objectForRegex* obj);
	};

	/*
	 * Used to cache a weak reference in a threadsafe way
	 */
	class SharedReference final
	{
	private:
		WeakReference* _ref = new WeakReference();
		int _locked = 0;

	public:
		/*
		 * Return an object from a weakref, protected by a lock.
		 *
		 * If the exclusive lock can't be obtained, null is returned;
		 *
		 * Note that _ref.Target is referenced only under the protection
		 * of the lock. (Is this necessary?)
		 */
		~SharedReference();

		objectForRegex* Get();

	};
}
