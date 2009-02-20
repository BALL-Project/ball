// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: stringHashMap.h,v 1.25 2004/05/06 21:39:37 oliver Exp $
//

#ifndef BALL_DATATYPE_STRINGHASHMAP_H
#define BALL_DATATYPE_STRINGHASHMAP_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_COMMON_HASH_H
#	include <BALL/COMMON/hash.h>
#endif

#ifndef BALL_CONCEPT_VISITOR_H
#	include <BALL/CONCEPT/visitor.h>
#endif

#ifndef BALL_CONCEPT_PROCESSOR_H
#	include <BALL/CONCEPT/processor.h>
#endif

#ifndef BALL_DATATYPE_HASHMAP_H
#	include <BALL/DATATYPE/hashMap.h>
#endif

#include <algorithm>

namespace BALL 
{


	/** StringHashMap
	    \ingroup  GenericHash
	*/
	template <typename Value>
	class StringHashMap
		:	public HashMap<String, Value>
	{
		public:

		BALL_CREATE(StringHashMap)

		/**	@name Type Definitions
		*/
		//@{

		/**	Iterator type
		*/
		typedef typename HashMap<String, Value>::Iterator Iterator;

		/**	Const iterator type
		*/
		typedef typename HashMap<String, Value>::ConstIterator ConstIterator;	
		
		/**	Value type
		*/
		typedef typename HashMap<String, Value>::ValueType ValueType;	
		//@}

		/**	@name	Constructors and Destructors */
		//@{

		/**	Default constructor.
				Create an empty hash map.
		*/
		StringHashMap()
			: HashMap<String, Value>()
		{
		}

		/** Copy constructor.
				Create a copy of an existing hash map.
				@param	map the hash map to be copied
		*/
		StringHashMap(const StringHashMap& map)
			: HashMap<String, Value>(map)
		{
		}
			
		/** Destructor.
				Destruct the hash map and free all used memory.
		*/
		virtual ~StringHashMap()
		{
		}

		/** Clear the hash map.
				Remove all contents from the hash map.
		*/
		void destroy()
		{
			HashMap<String, Value>::clear();
		}
	
		//@}

		/**	@name	Assignment */
		//@{
			
		/** Assign a hash map from another.
				Create a copy of a hash map.
				@param	hash_map	the map to be copied
		*/
    void set(const StringHashMap& hash_map)
		{
			HashMap<String, Value>::clear();

			ConstIterator it = hash_map.begin();
			for ( ; it != hash_map.end(); ++it)
			{
				insert(*it);
			}
		}

		/// Assign a hash map from another
		const StringHashMap& operator = (const StringHashMap& hash_map)
		{
			set(hash_map);
			return *this;
		}
			
		/// Assigns the content of a hash map to another
    void get(StringHashMap& hash_map) const
		{
			hash_map.set(*this);
		}

		/// Swaps the contents of two hash maps
    void swap(StringHashMap& hash_map)
		{
			std::swap(*this, hash_map);
		}

		//@}

		/**	@name Accessors */
		//@{

		/**	Insert a pair of key and value.
		*/
		std::pair<Iterator, bool> insert(const ValueType& obj)
		{
			return HashMap<String, Value>::insert(obj);
		}
		
		/**	Insert a given value and key.
				@param	value the value to be inserted
				@param	key the value`s key
		*/
		::std::pair<Iterator, bool> insert(const String& key, const Value& value)
		{
			return HashMap<String, Value>::insert(::std::pair<String, Value>(key, value));
		}

			
		/**	Remove the entry <tt>key</tt> from the map.
				@param	key the key of the entry to be removed
				@return	bool <b>true</b> if the key was removed
		*/
		bool remove(const String& key)
		{
			// search the key
			Iterator it = HashMap<String, Value>::find(key);
			if (it == HashMap<String, Value>::end())
			{
				// we didn't find it..
				return false;
			}
			
			// found it: delete it
			HashMap<String, Value>::erase(it);

			return true;
		}

		/** Return the size of the hash map.
		*/
		Size getSize() const
		{
			return HashMap<String, Value>::size();
		}
		
		//@}

		/**	@name	Predicates */
		//@{

		/**	Compare two string hash maps.
		*/
		bool operator == (const StringHashMap<Value>& hash_map) const
		{
			return HashMap<String, Value>::operator == (hash_map);
		}

		/**	Compare two string hash maps.
		*/
		bool operator != (const StringHashMap<Value>& hash_map) const
		{
			return !(HashMap<String, Value>::operator == (hash_map));
		}

		/** Decide whether the hash map contains a given key.
		*/
		bool has(const String& key) const
		{
			return !(HashMap<String, Value>::find(key) == HashMap<String, Value>::end());
		}

		/** Return true if the hash map is empty.
				This method return <b>true</b> if the hash map does not contain any entries.
		*/
		bool isEmpty() const
		{
			return (HashMap<String, Value>::size() == 0);
		}
		//@}

		/**	@name	Miscellaneous */
		//@{

		/**	Visitor host method.
				StringHashMaps may be visited.
				@param	visitor	the visitor
		*/
		void host(Visitor<StringHashMap<Value> >& visitor)
		{
			visitor.visit(*this);  
		}
		//@}

	};

// required for visual studio
#ifdef BALL_COMPILER_MSVC
template class BALL_EXPORT StringHashMap<String>;
#endif

}// namespace BALL

#endif // BALL_DATATYPE_HASHMAP_H
