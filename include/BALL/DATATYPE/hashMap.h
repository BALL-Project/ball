// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_DATATYPE_HASHMAP_H
#define BALL_DATATYPE_HASHMAP_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_COMMON_HASH_H
#	include <BALL/COMMON/hash.h>
#endif

#ifndef BALL_DATATYPE_TRIPLE_H
#	include <BALL/DATATYPE/triple.h>
#endif

#ifndef BALL_DATATYPE_QUADRUPLE_H
#	include <BALL/DATATYPE/quadruple.h>
#endif

#include <utility>
#include <algorithm>

#ifdef BALL_HAS_UNORDERED_MAP

#if defined(BALL_HAS_STD_UNORDERED_MAP)
# include <unordered_map>
#elif defined(BALL_HAS_TR1_UNORDERED_MAP)
# include <tr1/unordered_map>
#elif defined(BALL_HAS_BOOST_UNORDERED_MAP)
# include <boost/unordered_map.hpp>
#endif

#elif defined(BALL_HAS_HASH_MAP)
#if defined(BALL_EXT_INCLUDE_PREFIX)
# include <ext/hash_map>
# include <ext/hash_fun.h>
#else
# include <hash_map>
# include <hash_fun.h>
#endif
#else
# include <map>
#endif

#if defined(BALL_HAS_UNORDERED_MAP) && !defined(BALL_HAS_BOOST_UNORDERED_MAP)

#ifdef BALL_EXTEND_HASH_IN_STD_NS
namespace std
{
#endif // BALL_EXTEND_HASH_IN_STD_NS

#ifdef BALL_HAS_TR1_UNORDERED_MAP
	namespace tr1
	{
#endif // BALL_HAS_TR1_UNORDERED_MAP
		
		// borrowed from boost
		template<typename T> 
		void hash_combine_ala_boost(size_t & seed, T const & v)
		{
			hash<T> h;
			seed ^= h(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
		}

		template<class A, class B>
		struct hash<pair<A, B> > : public std::unary_function<pair<A,B>, size_t>
		{
			inline size_t
			operator()(pair<A, B> p) const
			{
				size_t seed = 0;
				hash_combine_ala_boost(seed, p.first);
				hash_combine_ala_boost(seed, p.second);

				return seed;
			}
		};

		template <class A, class B, class C>
		struct hash< ::BALL::Triple<A, B, C> > : public std::unary_function< ::BALL::Triple<A, B, C>, size_t>
		{
			inline size_t
			operator()(::BALL::Triple<A, B, C> t) const
			{
				size_t seed = 0;
				hash_combine_ala_boost(seed, t.first);
				hash_combine_ala_boost(seed, t.second);
				hash_combine_ala_boost(seed, t.third);

				return seed;
			}
		};

		template <class A, class B, class C, class D>
		struct hash< ::BALL::Quadruple<A, B, C, D> > : public std::unary_function< const ::BALL::Quadruple<A, B, C, D>, size_t> 
		{
			inline size_t
			operator()( ::BALL::Quadruple<A, B, C, D> q) const
			{
				size_t seed = 0;
				hash_combine_ala_boost(seed, q.first);
				hash_combine_ala_boost(seed, q.second);
				hash_combine_ala_boost(seed, q.third);
				hash_combine_ala_boost(seed, q.fourth);

				return seed;
			}
		};

#if !defined(BALL_COMPILER_MSVC) && !defined(BALL_COMPILER_LLVM)
		template<>
		struct hash<const ::BALL::String&> : public std::unary_function<const ::BALL::String &, size_t>
		{
			inline size_t
			operator()(const ::BALL::String& s) const
			{
				hash<const string&> h;
				return h(s);
			}
		};
#endif

		template<>
		struct hash< ::BALL::String > : public std::unary_function< ::BALL::String, size_t >
		{
			inline size_t
			operator()( ::BALL::String s) const
			{
				hash<string> h;
				return h(s);
			}
		};
#ifdef BALL_HAS_TR1_UNORDERED_MAP
	}
#endif // BALL_HAS_TR1_UNORDERED_MAP

#ifdef BALL_EXTEND_HASH_IN_STD_NS
}
#endif // BALL_EXTEND_HASH_IN_STD_NS

#endif // if defined(BALL_HAS_UNORDERED_MAP) && !defined(BALL_HAS_BOOST_UNORDERED_MAP)

#ifdef BALL_HAS_HASH_MAP
namespace BALL_MAP_NAMESPACE
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
#endif // BALL_HAS_HASH_MAP

namespace BALL
{
	/**
		@brief HashMap class based on the STL map (containing serveral convenience functions)

		@ingroup Datastructures
	*/
	template <class Key, class T>
	class HashMap
	  : public BALL_MAP_NAME
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
				IllegalKey(const char* file, int line)
					:	Exception::GeneralException(file, line)
				{
				}
			};
			
			///@name OpenMS style typedefs
			//@{
			typedef BALL_MAP_NAME Base;
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
			
			Size size() const { return BALL_MAP_NAME::size(); }
	};
	
	//******************************************************************************************
	// Implementations of template methods
	//******************************************************************************************
	
	template <class Key, class T>
	const T& HashMap<Key, T>::operator [] (const Key& key) const
	{
		ConstIterator it = this->find(key);
		if (it == Base::end())
		{
			throw IllegalKey(__FILE__, __LINE__);
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
		ConstIterator it(BALL_MAP_NAME::begin());
		for (; it != BALL_MAP_NAME::end(); ++it)
		{
			if (!rhs.has(it->first)) return false;
		}
		return true;
	}
	
	template <class Key, class T>
	T& HashMap<Key, T>::operator [] (const Key& key)
		
	{
		return BALL_MAP_NAME::operator[] (key);
	}

} // namespace BALL

#endif // BALL_DATATYPE_HASHMAP_H
