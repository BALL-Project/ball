// $Id: hashMap.h,v 1.3 1999/12/22 17:00:08 oliver Exp $

#ifndef BALL_DATATYPE_HASHMAP_H
#define BALL_DATATYPE_HASHMAP_H

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

#include <BALL/DATATYPE/internal_hash_map.h>
#include <algorithm>

namespace BALL 
{

	template 
		<class Key, class Value, 
		 class Hasher = HashFunction<Key>, 
	   class EqualKey = equal_to<Key> >
	class HashMap
		:	public hash_map<Key, Value, Hasher, EqualKey>
	{
		public:

		BALL_CREATE(HashMap)

		/**	@name Type Definitions
		*/
		//@{

		/**	Iterator type
		*/
		typedef typename hash_map<Key, Value, Hasher, EqualKey>::iterator Iterator;

		/**	Const iterator type
		*/
		typedef typename hash_map<Key, Value, Hasher, EqualKey>::const_iterator ConstIterator;	

		/**	Value type
		*/
		typedef typename hash_map<Key, Value, Hasher, EqualKey>::value_type ValueType;	
		//@}

		/**	@name	Constructors and Destructors */
		//@{

		/**	Default constructor.
				Create an empty hash map.
		*/
		HashMap()
			: hash_map<Key, Value, Hasher, EqualKey>()
		{
		}

		/** Copy constructor.
				Create a copy of an existing hash map.
				@param	map the hash map to be copied
				@param	deep ignored
		*/
		HashMap(const HashMap& map, bool /* deep = true */)
			: hash_map<Key, Value, Hasher, EqualKey>(map)
		{
		}
			
		/** Destructor.
				Destruct the hash map and free all used memory.
		*/
		virtual ~HashMap()
		{
		}

		/** Clear the hash map.
				Remove all contents from the hash map.
		*/
		void destroy()
		{
			clear();
		}
	
		//@}

		/**	@name	Assignment */
		//@{
			
		/** Assign a hash map from another.
				Create a copy of a hash map.
				@param	hash_map	the map to be copied
				@param	deep ignored
		*/
    void set(const HashMap& hash_map, bool /* deep */ = true)
		{
			clear();

			const_iterator it = hash_map.begin();
			for ( ; it != hash_map.end(); ++it)
			{
				insert(*it);
			}
		}
		
		/** Assign a hash map from another.
		*/
		HashMap& operator = (const HashMap& hash_map)
		{
			set(hash_map);
			return *this;
		}
			
		/// Assigns the content of a hash map to another
    void get(HashMap& hash_map, bool deep = true) const
		{
			hash_map.set(*this, deep);
		}

		/// Swaps the contents of two hash maps
    void swap(HashMap& hash_map)
		{
			swap(*this, hash_map);
		}

		//@}

		/**	@name Accessors */
		//@{

		/**	Insert a pair of key and value.
		*/
		pair<Iterator, bool> insert(const ValueType& obj)
		{
			return hash_map<Key, Value, Hasher, EqualKey>::insert(obj);
		}
		

		/**	Insert a given value and key.
				@param	value the value to be inserted
				@param	key the value`s key
		*/
		pair<Iterator, bool> insert(const Key key, Value value)
		{
			return insert(pair<const Key, Value>(key, value));
		}

			
		/**	Remove the entry {\tt key} from the map.
				@param	key the key of the entry to be removed
				@return	{\bf true} if the key was removed
		*/
		bool remove(const Key key)	
		{
			// search the key
			Iterator it = find(key);
			if (it == end())
			{
				// we didn't find it..
				return false;
			}
			
			// found it: delete it
			erase(it);

			return true;
		}

		/** Return the size of the hash map.
		*/
		Size getSize() const
		{
			return size();
		}
		
		/**	Return the load factor of the hash map.
				The load factor is defined as the quotient of
				the hash map size (the number of entries) and the number
				of buckets.
		*/
		float getLoadFactor() const
		{
			return (float)size() / (float)bucket_count();
		}

		/**	Return the number of buckets.
		*/
		Size getBucketSize() const
		{
			return bucket_count();
		}
		
		//@}

		/**	@name	Predicates */
		//@{

		/** Decide whether the hash map contains a given key.
		*/
		bool has(const Key& key) const
		{
			return !(find(key) == end());
		}

		/** Return true if the hash map is empty.
				This method return {\bf true} if the hash map does not contain any entries.
		*/
		bool isEmpty() const
		{
			return (size() == 0);
		}
		//@}

		/**	@name	Miscellaneous */
		//@{

		/**	Visitor host method.
				HashMaps may be visited.
				@param	visitor	the visitor
		*/
		void host(Visitor<HashMap<Key, Value> >& visitor)
		{
			//BAUSTELLE
		}
  			
		//@}

		/**	@name	Internal Iterators */
		//@{

		/** Processor application method.
				Applies the processor to each entry of the hash map.
				@param processor the processor to be applied
		*/
		bool apply(UnaryProcessor<HashMap<Key, Value> >& processor)
		{
			// BAUSTELLE
		}
		//@}

	};


}// namespace BALL

#endif // BALL_DATATYPE_HASHMAP_H
