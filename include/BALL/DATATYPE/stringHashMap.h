// $Id: stringHashMap.h,v 1.8 2000/02/17 00:30:39 oliver Exp $

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

	template <class Value>
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
				@param	deep ignored
		*/
		StringHashMap(const StringHashMap& map, bool /* deep = true */)
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
    void set(const StringHashMap& hash_map, bool /* deep */ = true)
		{
			clear();

			ConstIterator it = hash_map.begin();
			for ( ; it != hash_map.end(); ++it)
			{
				insert(*it);
			}
		}

		/** Assign a hash map from another.
		*/
		StringHashMap& operator = (const StringHashMap& hash_map)
		{
			set(hash_map);
			return *this;
		}
			
		/// Assigns the content of a hash map to another
    void get(StringHashMap& hash_map, bool deep = true) const
		{
			hash_map.set(*this, deep);
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
		std::pair<Iterator, bool> insert(const String& key, const Value& value)
		{
			return HashMap<String, Value>::insert(HashMap<String, Value>::ValueType(key, value));
		}

			
		/**	Remove the entry {\tt key} from the map.
				@param	key the key of the entry to be removed
				@return	bool {\bf true} if the key was removed
		*/
		bool remove(const String& key)	
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
		bool has(const String& key) const
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
				StringHashMaps may be visited.
				@param	visitor	the visitor
		*/
		void host(Visitor<StringHashMap<Value> >& visitor)
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
		bool apply(UnaryProcessor<StringHashMap<Value> >& processor)
		{
			// BAUSTELLE
		}
		//@}

	};


}// namespace BALL

#endif // BALL_DATATYPE_HASHMAP_H
