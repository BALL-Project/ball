// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: hashMap.h,v 1.41 2005/12/23 17:01:42 amoll Exp $ 
//

#ifndef BALL_DATATYPE_HASHMAP_H
#define BALL_DATATYPE_HASHMAP_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_COMMON_HASH_H
#	include <BALL/COMMON/hash.h>
#endif

#include <utility>
#include <algorithm>

#ifdef BALL_EXT_INCLUDE_PREFIX
# include <ext/hash_map>
# include <ext/hash_fun.h>
#else
# include <hash_map>
# include <hash_fun.h>
#endif

namespace BALL_EXT_NAMESPACE
{

	template<class T>
  struct hash<T*>
  {
		size_t operator()(const T* x) const { return (size_t)x; }
	};

	template<>
  struct hash<BALL::String>
  {
    size_t operator () (const BALL::String& s) const {return __stl_hash_string(s.c_str());}
	};

#ifdef BALL_NEEDS_LONGSIZE_HASH
  template<>
	struct hash<BALL::LongSize>
	{
		size_t operator()(BALL::LongSize x) const { return (size_t)x; }
	};
#endif

}

namespace BALL
{
	/**
		@brief HashMap class based on the STL map (containing serveral convenience functions)

		@ingroup Datastructures
	*/
	template <class Key, class T>
	class HashMap
	  : public BALL_EXT_NAMESPACE::hash_map<Key,T>
	{
		public:

			/**
				@brief HashMap illegal key exception
	
				@ingroup Exceptions
			*/
			class IllegalKey
				:	public Exception::GeneralException
			{
				public:
				IllegalKey(const char* file, int line, const String& function)
					:	Exception::GeneralException(file, line, function, "")
				{
				}
			};
			
			///@name OpenMS style typedefs
			//@{
			typedef BALL_EXT_NAMESPACE::hash_map<Key,T> Base;
			typedef typename Base::value_type ValueType;
			typedef Key KeyType;
			typedef typename Base::value_type* PointerType;
			typedef typename Base::iterator Iterator;
			typedef typename Base::const_iterator ConstIterator;
			//@}

			///Test whether the map contains the given key.
			inline bool has(const Key& key) const
			{
				return Base::find(key)!=Base::end();
			}

			/**	
				@brief Return a constant reference to the element whose key is @p key.
				
				@exception IllegalKey if the given key does not exist
			*/
			const T& operator [] (const Key& key) const;

			/// Return a mutable reference to the element whose key is @p key. If an element with the key @p key does not exist, it is inserted.
			T& operator [] (const Key& key);
			
			/// Equality operator. Check whether two two hashmaps contain the same elements. O(n) runtime.
			bool operator == (const HashMap<Key, T>& rhs) const;
			
			Size size() const { return BALL_EXT_NAMESPACE::hash_map<Key, T>::size(); }
	};
	
	//******************************************************************************************
	// Implementations of template methods
	//******************************************************************************************
	
	template <class Key, class T>
	const T& HashMap<Key, T>::operator [] (const Key& key) const
	{
		ConstIterator it = find(key);
		if (it == Base::end())
		{
			throw IllegalKey(__FILE__, __LINE__, __PRETTY_FUNCTION__);
		}
		else
		{
			return it->second;
		}
	}


	template <class Key, class T>
	bool HashMap<Key, T>::operator == (const HashMap<Key, T>& rhs) const
	{
		// No equality if sizes differ.
		if (size() != rhs.size()) 
		{
			return false;	
		}

		// Equality if bothe have the same size and every element of lhs is 
		// is contained in lhs. Testing the other way round is obviously
		// unnecessary.
		ConstIterator it(BALL_EXT_NAMESPACE::hash_map<Key, T>::begin());
		for (; it != BALL_EXT_NAMESPACE::hash_map<Key, T>::end(); ++it)
		{
			if (!rhs.has(it->first)) return false;
		}
		return true;
	}
	
	template <class Key, class T>
	T& HashMap<Key, T>::operator [] (const Key& key)
		
	{
		Iterator it = find(key);
		if (it == Base::end())
		{
			it = insert(ValueType(key, T())).first;
		}
		return it->second;
	}

} // namespace BALL

#endif // BALL_DATATYPE_HASHMAP_H
