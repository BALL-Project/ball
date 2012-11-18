// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_DATATYPE_HASHMAP_H
#define BALL_DATATYPE_HASHMAP_H

#ifndef BALL_COMMON_EXCEPTION_H
# include <BALL/COMMON/exception.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
# include <BALL/DATATYPE/string.h>
#endif

#ifndef BALL_DATATYPE_TRIPLE_H
# include <BALL/DATATYPE/triple.h>
#endif

#ifndef BALL_DATATYPE_QUADRUPLE_H
# include <BALL/DATATYPE/quadruple.h>
#endif

#include <boost/unordered_map.hpp>
#include <boost/functional/hash.hpp>

namespace boost
{
	template<>
	struct hash<BALL::String>
	{
		size_t operator () (const BALL::String& s) const { return boost::hash<std::string>()(s); }
	};

	template <typename T1, typename T2, typename T3>
	struct hash<BALL::Triple<T1, T2, T3> >
	{
		size_t operator () (const BALL::Triple<T1, T2, T3>& s) const
		{
			size_t hash = 0;
			boost::hash_combine(hash, s.first);
			boost::hash_combine(hash, s.second);
			boost::hash_combine(hash, s.third);

			return hash;
		}
	};

	template <typename T1, typename T2, typename T3, typename T4>
	struct hash<BALL::Quadruple<T1, T2, T3, T4> >
	{
		size_t operator () (const BALL::Quadruple<T1, T2, T3, T4>& s) const
		{
			size_t hash = 0;
			boost::hash_combine(hash, s.first);
			boost::hash_combine(hash, s.second);
			boost::hash_combine(hash, s.third);
			boost::hash_combine(hash, s.fourth);

			return hash;
		}
	};
}

namespace BALL
{
	/**
		@brief HashMap class based on the STL map (containing serveral convenience functions)

		@ingroup Datastructures
	*/
	template <class Key, class T>
	class HashMap : public boost::unordered_map<Key, T>
	{
		public:

			/**
			 * @brief HashMap illegal key exception
			 * 
			 * @ingroup Exceptions
			 */
			class IllegalKey : public Exception::GeneralException
			{
				public:
					IllegalKey(const char* file, int line)
						: Exception::GeneralException(file, line)
					{
					}
			};
			
			///@name OpenMS style typedefs
			//@{
			typedef boost::unordered_map<Key, T> Base;
			typedef typename Base::value_type ValueType;
			typedef Key KeyType;
			typedef typename Base::value_type* PointerType;
			typedef typename Base::iterator Iterator;
			typedef typename Base::const_iterator ConstIterator;
			//@}

			///Test whether the map contains the given key.
			inline bool has(const Key& key) const
			{
				return Base::find(key) != Base::end();
			}

			/**
			 * @brief Return a constant reference to the element whose key is @p key.
			 *
			 * @exception IllegalKey if the given key does not exist
			*/
			const T& operator [] (const Key& key) const;

			/// Return a mutable reference to the element whose key is @p key. If an element with the key @p key does not exist, it is inserted.
			T& operator [] (const Key& key);

			/// Equality operator. Check whether two two hashmaps contain the same elements. O(n) runtime.
			bool operator == (const HashMap<Key, T>& rhs) const;
			
			/// Return the size of the hash map.
			/// TODO: Remove this. This narrows the precision of size_t! (But of course breaks persistence...)
			Size size() const { return Base::size(); }
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
		if (Base::size() != rhs.size()) 
		{
			return false;	
		}

		// Equality if bothe have the same size and every element of lhs is 
		// is contained in lhs. Testing the other way round is obviously
		// unnecessary.
		ConstIterator it(Base::begin());
		for (; it != Base::end(); ++it)
		{
			if (!rhs.has(it->first)) return false;
		}

		return true;
	}
	
	template <class Key, class T>
	T& HashMap<Key, T>::operator [] (const Key& key)
	{
		return Base::operator[] (key);
	}

} // namespace BALL

#endif // BALL_DATATYPE_HASHMAP_H
