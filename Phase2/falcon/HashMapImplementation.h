#ifndef HASH_MAP_IMPLEMENTATION
#define HASH_MAP_IMPLEMENTATION

#include "CommonSettingsAndUtilities.h"

#include <string>
#include <cstring>
using namespace std;

#undef MY_WINDOWS

#ifdef MY_WINDOWS

	#include <hash_map>
	
	// The following class defines a hash function for strings 
	class stringhasher : public stdext::hash_compare <std::string>
	{
	public:
	  /**
	   * Required by 
	   * Inspired by the java.lang.String.hashCode() algorithm 
	   * (it's easy to understand, and somewhat processor cache-friendly)
	   * @param The string to be hashed
	   * @return The hash value of s
	   */
	  size_t operator() (const std::string& s) const
	  {
		size_t h = 0;
		std::string::const_iterator p, p_end;
		for(p = s.begin(), p_end = s.end(); p != p_end; ++p)
		{
		  h = 31 * h + (*p);
		}
		return h;
	  }

	  /**
	   * 
	   * @param s1 The first string
	   * @param s2 The second string
	   * @return true if the first string comes before the second in lexicographical order
	   */
	  bool operator() (const std::string& s1, const std::string& s2) const
	  {
		return s1 < s2;
	  }
	};

	//typedef stdext::hash_map<std::string, std::string, stringhasher> HASH_S_S;

	template <typename T>
	class HashMapImplementation
	{
		// member functions
	public:
		HashMapImplementation();
		~HashMapImplementation();

		bool InsertKeyValuePair(const char* key, T value);
		bool IsKeyPresent(const char* key);
		T& GetValue(const char* key);

		// data members
	public:
		stdext::hash_map<std::string, T, stringhasher> internalMap;
	};

	template <typename T>
	HashMapImplementation<T>::HashMapImplementation()
	{
	}

	template <typename T>
	HashMapImplementation<T>::~HashMapImplementation()
	{
		internalMap.clear();
	}

	template <typename T>
	bool HashMapImplementation<T>::InsertKeyValuePair(const char* key, T value)
	{
		if(internalMap.find(key) == internalMap.end())
		{
			internalMap[key] = value;
			return true;
		}

		return false;
	}

	template <typename T>
	T& HashMapImplementation<T>::GetValue(const char* key)
	{
		if(internalMap.find(key) != internalMap.end())
			return internalMap[key];
	}

	template <typename T>
	bool HashMapImplementation<T>::IsKeyPresent(const char* key)
	{
		if(internalMap.find(key) != internalMap.end())
			return true;
		else
			return false;
	}

#else

	//#include <ext/hash_map>
	#include <map>
	using namespace __gnu_cxx;

	struct eqstr
	{
		bool operator()(string &s1, string &s2) const
		{
			return (s1 == s2);
		}
	};

	template <typename T>
	class HashMapImplementation
	{
		// member functions
	public:
		HashMapImplementation() {};
		~HashMapImplementation() {};

		bool InsertKeyValuePair(string key, T value)
		{
			internalMap[key] = value;

			if(internalMap.find(key) == internalMap.end())
			{
				internalMap[key] = value;
				return true;
			}
			return false;
		}


		bool IsKeyPresent(string key)
		{
			if(internalMap.find(key) != internalMap.end())
				return true;
			else
				return false;
		}

		T& GetValue(string key)
		{
			if(internalMap.find(key) != internalMap.end())
					return internalMap[key];
		}

		//hash_map<const char*, T, hash<const char*>, eqstr > internalMap;
		map<string, T> internalMap;
	};

#endif

#endif
