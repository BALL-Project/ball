// $Id: hashSet.h,v 1.16 2000/09/05 07:16:59 oliver Exp $ 

#ifndef BALL_DATATYPE_HASHSET_H
#define BALL_DATATYPE_HASHSET_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_COMMON_HASH_H
#	include <BALL/COMMON/hash.h>
#endif

#ifndef BALL_CONCEPT_FORWARDITERATOR_H
#	include <BALL/CONCEPT/forwardIterator.h>
#endif

#ifndef BALL_CONCEPT_VISITOR_H
#	include <BALL/CONCEPT/visitor.h>
#endif

#ifndef BALL_DATATYPE_FOREACH_H
#	include <BALL/DATATYPE/forEach.h>
#endif

#ifndef BALL_CONCEPT_PREDICATE_H
#	include <BALL/CONCEPT/predicate.h>
#endif

#ifndef BALL_CONCEPT_PROCESSOR_H
#	include <BALL/CONCEPT/processor.h>
#endif

#include <algorithm>


namespace BALL
{

  /** Generic Hash Set Class.
      {\bf Definition:} \URL{BALL/DATATYPE/hashSet.h}
  */
 	template <class Key>
	class HashSet
	{
		public:

		/**	@name	Enums
		*/
		//@{

		enum
		{
			///
			INITIAL_CAPACITY          = 4,
			///
			INITIAL_NUMBER_OF_BUCKETS = 3
		};
		//@}

		/**	@name	Exceptions
		*/
		//@{

		/**	Illegal key exception.
				Thrown if access to a non-existent key is required by the constant
				version of \Ref{operator []}.
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
		//@}
		
		class IteratorTraits_;

		/**	@name	Type definitions
		*/
		//@{
		/**
		*/
		typedef Key ValueType;
			
		/**
		*/
		typedef Key KeyType;

		/**
		*/
		typedef Key* PointerType;
			
		/**
		*/
		typedef 
				ForwardIterator<HashSet<Key>, ValueType, PointerType, IteratorTraits_>
			Iterator;

		/**
		*/
		typedef 
				ConstForwardIterator <HashSet<Key>, ValueType, PointerType, IteratorTraits_>
			ConstIterator;
		//@}

		/**	@name Constructors and Destructors 
		*/
		//@{

		/**	Default Constructor.
		*/
		HashSet(Size initial_capacity = INITIAL_CAPACITY, Size number_of_buckets = INITIAL_NUMBER_OF_BUCKETS);
			
		/**	Copy Constructor.
		*/
		HashSet(const HashSet& hash_set);

		/**	Destructor.
		*/
		virtual ~HashSet()
		{
			destroy();
			
			deleteBuckets_();
		}

		/**	Clear the hash set.
				Remove all nodes from all buckets.
				The capacity and the number of buckets remain unchanged.
		*/
		virtual void clear();
	
		/**	Clear the hash set.
				Remove all nodes from all buckets.
				The capacity and the number of buckets remain unchanged.
				Simply calls clear;
		*/
		void destroy();
		//@}
		
		/**	@name Assignment 
		*/
		//@{

		/** Assign this HashSet with the contents of another HashSet
				@param hash_set the HashSet to assign from
		*/
		void set(const HashSet& hash_set);

		/** Assign this HashSet with the contents of another HashSet
				@param hash_set the HashSet to assign from
		*/
		HashSet& operator = (const HashSet& hash_set);

		/** Assing another HashSet with the contents of this HashSet
				@param hash_set the HashSet to assign to
		*/
		void get(HashSet& hash_set) const;

		/**	Swap the contents of two hash sets.
		*/
		void swap(HashSet& hash_set);
		//@}

		/**	@name	Accessors
		*/
		//@{
		/**	Return the number of buckets.
		*/
		Size getBucketSize() const;

		/** Return the capcacity of the hash set.
		*/
		Size getCapacity() const;

		/**	Return the number of elements in the hash set.
		*/
		Size getSize() const;
			
		/**	Return the number of elements in the hash set.
		*/
		Size size() const;

    /** Find the element whose key is {\tt key}.
    */
 		Iterator find(const Key& key);
	
    /** Find the element whose key is {\tt key}.
    */
		ConstIterator find(const Key& key) const;

		/**	Insert a new entry into the hash set.
		*/
		std::pair<Iterator, bool> insert(const ValueType& item);

		/**	Erase element with key {\tt key}.
				@return Size the number of elements erased (0 or 1)
		*/
		bool erase(const KeyType& key);

		/**	Erase element at a given position.
				@param pos an iterator pointing to the element to delete
		*/
		void erase(Iterator pos);

		/**	Erase a range of elements.
				Erase all elements in the range {\tt \[f, l)}.
		*/
		void erase(Iterator f, Iterator l);
		//@}

		/**	@name Miscellaneous
		*/
		//@{

		/**	Host a visitor for all set entries.
		*/
		void host(Visitor<ValueType>& visitor);
		//@}
	
		/**	@name	Predicates
		*/
		//@{

		/**	Test whether the set contains the key {\tt key}.
		*/
		bool has(const Key& key) const;

		/**	Test whether the set is empty.
		*/
		bool isEmpty() const;

		/**	Compare two hash sets.
		*/
		bool operator == (const HashSet& hash_set) const;

		/**	Compare two hash sets.
		*/
		bool operator != (const HashSet& hash_set) const;
		//@}

		/**	@name	Debugging and Diagnostics
		*/
		//@{

		/**	Return true if the hash set is consistent.
				Condition: the number of entries in all buckets has to be equal the 
				stored number of entries (getSize()).
		*/
		bool isValid() const;

		/** Dump the constent of this instance to an ostream.
		*/
		virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;
		//@}

		// --- INTERNAL ITERATORS

		/** Apply a processor to all keys in this instance.
				@return true if the processor could be applied.
		*/
		bool apply(UnaryProcessor<ValueType>& processor);

		// --- STORERS

		/*
		friend istream& operator >> BALL_TEMPLATE_NULL_ARGS (std::istream& s, HashSet& hash_set);
			
		friend ostream& operator << BALL_TEMPLATE_NULL_ARGS (std::ostream& s, const HashSet& hash_set);

		void read(std::istream& s);

		void write(std::ostream& s) const;
		*/      

		// --- EXTERNAL ITERATORS
		struct Node
		{
			Node*				next;
			ValueType		value;

			Node(const KeyType& my_key, const Node* my_next)
				: next(const_cast<Node*>(my_next)),
					value(const_cast<ValueType&>(my_key))
			{
			}
		};

		typedef Node* IteratorPosition;
	
		class IteratorTraits_
		{
			friend class HashSet<Key>;
			public:

			IteratorTraits_()
				:	bound_(0),
					position_(0),
					bucket_(0)
			{
			}
			
			IteratorTraits_(const HashSet& hash_set)
				:	bound_(const_cast<HashSet*>(&hash_set)),
					position_(0),
					bucket_(0)
			{
			}
			
			IteratorTraits_(const IteratorTraits_& traits)
				:	bound_(traits.bound_),
					position_(traits.position_),
					bucket_(traits.bucket_)
			{
			}
			
			IteratorTraits_& operator = (const IteratorTraits_& traits)
			{
				bound_ = traits.bound_;
				position_ = traits.position_;
				bucket_ = traits.bucket_;
		
				return *this;
			}

			HashSet* getContainer()
			{
				return bound_;
			}
			
			const HashSet* getContainer() const
			{
				return bound_;
			}
			
			bool isSingular() const
			{
				return (bound_ == 0);
			}
			
			IteratorPosition& getPosition()
			{
				return position_;
			}

			const IteratorPosition& getPosition() const
			{
				return position_;
			}

			bool operator == (const IteratorTraits_& traits) const
			{
				return (position_ == traits.position_);
			}

			bool operator != (const IteratorTraits_& traits) const
			{
				return (position_ != traits.position_);
			}
			
			bool isValid() const
			{
				return ((bound_ != 0) && (position_ != 0)
											&& (bucket_ < (Position)bound_->bucket_.size()));
			}

			void invalidate()
			{
				bound_ = 0;
				position_ = 0;
				bucket_ = INVALID_INDEX;
			}
			
			void toBegin()
			{
				for (bucket_ = 0;  bucket_ < (Position)bound_->bucket_.size();  ++bucket_)
				{
					position_ = bound_->bucket_[bucket_];

					if (position_ != 0)
					{
						return;
					}
				}
			}

			bool isBegin() const
			{
				for (Position bucket = 0; bucket < (Position)bound_->bucket_.size();  ++bucket)
				{
					if (bound_->bucket_[bucket_] != 0)
					{
						if (position_ == bound_->bucket_[bucket_])
						{
							return true;
						} 
						else 
						{
							return false;
						}
					}
				}

				return false;
			}

			void toEnd()
			{
				position_ = 0;
			}
			
			bool isEnd() const
			{
				return (position_ == 0);
			}
			
			ValueType& getData()
			{
				return position_->value;
			}

			const ValueType& getData() const
			{
				return position_->value;
			}

			void forward()
			{
				position_ = position_->next;

				if (position_ != 0)
				{
					return;
				}

				for (++bucket_;  bucket_ < (Position)bound_->bucket_.size();  ++bucket_)
				{
					position_ = bound_->bucket_[bucket_];

					if (position_ != 0)
					{
						return;
					}
				}
			} 

			protected:

			HashSet*						bound_;
			IteratorPosition		position_;
			Position						bucket_;
		};

		friend class IteratorTraits_;

		/**
		*/
		Iterator begin()
		{
			return Iterator::begin(*this);
		}

		/**
		*/
		Iterator end()
		{
			return Iterator::end(*this);
		}

		/**
		*/
		ConstIterator begin() const
		{
			return ConstIterator::begin(*this);
		}

		/**
		*/
		ConstIterator end() const
		{
			return ConstIterator::end(*this);
		}


		protected:

		virtual Node* newNode_(const ValueType& value, Node* next) const;

		virtual void deleteNode_(Node* node) const;
	
		virtual HashIndex hash(const Key& key) const;

		virtual bool needRehashing_() const;

		virtual void rehash();

		private:

		void deleteBuckets_();

		Position hashBucket_(const Key& key) const;

		void rehash_();

		// --- ATTRIBUTES

		/*_	The number of elements in the hash set
		*/
		Size	size_;

		/*_	The capacity - usually the number of buckets
		*/
		Size	capacity_;

		/*_	Buckets are stored as a vector of linked lists of Nodes 
		*/
		vector<Node*>	bucket_;
	};


	template <class Key>
	HashSet<Key>::HashSet(Size initial_capacity, Size number_of_buckets)
		:	size_(0),
			capacity_(initial_capacity),
			bucket_(number_of_buckets)
	{
		for (Position bucket = 0; bucket < (Position)bucket_.size(); ++bucket)
		{
			bucket_[bucket] = 0;
		}
	}

	template <class Key>
	HashSet<Key>::HashSet(const HashSet& hash_set)
		:	size_(hash_set.size_),
			capacity_(hash_set.capacity_),
			bucket_(hash_set.bucket_.size())
	{
		Node* item = 0;
		
		for (Position bucket = 0; bucket < (Position)bucket_.size(); ++bucket)
		{
			bucket_[bucket] = 0;

			for (item = hash_set.bucket_[bucket]; item != 0; item = item->next)
			{
				bucket_[bucket]	= newNode_(item->value, bucket_[bucket]);
			}
		}

	}

	template <class Key>
	void HashSet<Key>::set(const HashSet& hash_set)
	{
		if (&hash_set == this)
		{
			return;
		}

		destroy();
		
		deleteBuckets_();

		size_ = hash_set.size_;
		capacity_ = hash_set.capacity_;
		bucket_.resize(hash_set.bucket_.size());

		Node* item = 0;
		
		for (Position bucket = 0; bucket < (Position)bucket_.size(); ++bucket)
		{
			bucket_[bucket] = 0;

			for (item = hash_set.bucket_[bucket]; item != 0; item = item->next)
			{
				bucket_[bucket]	= newNode_(item->value, bucket_[bucket]);
			}
		}
	}

	template <class Key>
	void HashSet<Key>::clear()
	{
		Node* node = 0;
		Node* next_node = 0;
		
		for (Position bucket = 0; bucket < (Position)bucket_.size(); ++bucket)
		{
			for (node = bucket_[bucket]; node != 0; node = next_node)
			{
				next_node = node->next;
				deleteNode_(node);
			}
			
			bucket_[bucket] = 0;
		}

		size_ = 0;
	}

	template <class Key>
	BALL_INLINE 
	void HashSet<Key>::destroy()
	{
		clear();
	}

	template <class Key>
	BALL_INLINE 
	HashSet<Key>& HashSet<Key>::operator = (const HashSet& hash_set)
	{
		set(hash_set);
		return *this;
	}

	template <class Key>
	BALL_INLINE 
	void HashSet<Key>::get(HashSet& hash_set) const
	{
		hash_set.set(*this);
	}

	template <class Key>
	BALL_INLINE 
	void HashSet<Key>::swap(HashSet& hash_set)
	{
		std::swap(size_, hash_set.size_);
		std::swap(capacity_, hash_set.capacity_);
		std::swap(bucket_, hash_set.bucket_);
	}

	template <class Key>
	BALL_INLINE 
	Size HashSet<Key>::getBucketSize() const
	{
		return bucket_.size();
	}

	template <class Key>
	BALL_INLINE 
	Size HashSet<Key>::getCapacity() const
	{
		return capacity_;
	}

	template <class Key>
	BALL_INLINE 
	Size HashSet<Key>::getSize() const
	{
		return size_;
	}

	template <class Key>
	BALL_INLINE 
	Size HashSet<Key>::size() const
	{
		return size_;
	}

	template <class Key>
	HashSet<Key>::Iterator HashSet<Key>::find(const Key& key)
	{
		Iterator it = end();
		Position bucket = hashBucket_(key);
		Node* node_ptr = bucket_[bucket];
		while (node_ptr != 0)
		{
			if (node_ptr->value == key)
			{
				it.getTraits().position_ = node_ptr;
				it.getTraits().bucket_ = bucket;
				break;
			}
			node_ptr = node_ptr->next;
		}

		return it;
	}
		
	template <class Key>
	BALL_INLINE 
	HashSet<Key>::ConstIterator HashSet<Key>::find(const Key& key) const
	{
		return (const_cast<HashSet*>(this))->find(key);
	}

	template <class Key>
	std::pair<typename HashSet<Key>::Iterator, bool> HashSet<Key>::insert
		(const ValueType& item)
	{
		Iterator it = find(item);
		if (it == end())
		{
			if (needRehashing_() == true)
			{
				rehash_();
			}
			
			Position bucket = hashBucket_(item);
			
			bucket_[bucket] = newNode_(item, bucket_[bucket]);
			
			++size_;
			// BAUSTELLE: Effizienz?
			it = find(item);
		}

		return std::pair<Iterator, bool>(it, true);
	}

	template <class Key>
	bool HashSet<Key>::erase(const KeyType& key)
	{
		Position	bucket = hashBucket_(key);
		Node*			previous = 0;
		Node*			node_ptr = bucket_[bucket];
		
		while (node_ptr != 0 && node_ptr->value != key)
		{
			previous = node_ptr;
			node_ptr = node_ptr->next;
		}

		if (node_ptr == 0)
		{
			return false;
		}

		if (node_ptr == bucket_[bucket])
		{
			bucket_[bucket] = node_ptr->next;
		} 
		else 
		{
			previous->next = node_ptr->next;
		}

		deleteNode_(node_ptr);
		--size_;
		return true;
	}

	template <class Key>
	void HashSet<Key>::erase(Iterator pos)
	{
		if (pos.getTraits().bound_ != this)
		{
			throw Exception::IncompatibleIterators(__FILE__, __LINE__);
		}

		if (pos == end())
		{
			return;
		}
				
		if (pos.getTraits().getPosition() == bucket_[pos.getTraits().bucket_])
		{
			bucket_[pos.getTraits().bucket_] = pos.getTraits().getPosition()->next;
		} 
		else 
		{
			(pos.getTraits().getPosition() - 1)->next = pos.getTraits().getPosition()->next;
		}


		deleteNode_(pos.getTraits().getPosition());
		--size_;
	}

	template <class Key>
	void HashSet<Key>::erase(Iterator f, Iterator l)
	{
		if (f.getTraits().bound_ != this || l.getTraits().bound_ != this)
		{
			throw Exception::IncompatibleIterators(__FILE__, __LINE__);
		}
		
		if (f.position_ > l.position_)
		{
			std::swap(f, l);
		}

		if (f == end())
		{
			return;
		}

		if (f == bucket_[f.getTraits().bucket_])
		{
			bucket_[f.getTraits().bucket_] = l;
		} 
		else 
		{
			(f-1)->next = l;
		}

		Index diff = f.position_ - l.position_;
		while (f.position_ < l.position_)
		{
			deleteNode_(f);
			f++;
		}
		size_ -= diff;
	}

	template <class Key>
	BALL_INLINE 
	void HashSet<Key>::host(Visitor<ValueType>& visitor)
	{
		Iterator it = begin();
		for (; it != end(); ++it)
		{
			visitor.visit(*it);
		}
	}
		
	template <class Key>
	BALL_INLINE 
	bool HashSet<Key>::has(const Key& key) const
	{
		return (find(key) != end());
	}

	template <class Key>
	BALL_INLINE 
	bool HashSet<Key>::isEmpty() const
	{
		return (size_ == 0);
	}

	template <class Key>
	bool HashSet<Key>::operator == (const HashSet& hash_set) const
	{
		if (size_ != hash_set.size_)
		{
			return false;
		}

		Iterator it1 = begin();
		Iterator it2 = hash_set.begin();
		while (it1 != end())
		{
			if (*it1 != *it2)
			{
				return false;
			}
			it1++;
			it2++;
		}
		return true;
	}

	template <class Key>
	BALL_INLINE
	bool HashSet<Key>::operator != (const HashSet& hash_set) const
	{
		return !(*this == hash_set);
	}

	template <class Key>
	bool HashSet<Key>::isValid() const
	{
		Size size = 0;
		Node* node = 0;

		for (Position bucket = 0; bucket < (Position)bucket_.size(); ++bucket)
		{
			for (node = bucket_[bucket]; node != 0; node = node->next)
			{
				++size;

				if (node->next == 0)
				{
					break;
				}
			}
		}

		return (size_ == size);
	}      

	template <class Key>
	void HashSet<Key>::dump(std::ostream& s, Size depth) const
	{
		BALL_DUMP_STREAM_PREFIX(s);

		BALL_DUMP_DEPTH(s, depth);

		BALL_DUMP_DEPTH(s, depth);
		s << "  size: " << getSize() << std::endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "  bucket size: " << getBucketSize() << std::endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "  capacity: " << getCapacity() << std::endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "  load factor: " << (float)size_ / (float)bucket_.size() << std::endl;

		for (Position bucket = 0; bucket < (Position)bucket_.size(); ++bucket)
		{
			BALL_DUMP_DEPTH(s, depth);
			s << "    bucket " << bucket << " (" << (void*)bucket_[bucket] << "):" << std::endl;
		}

		BALL_DUMP_STREAM_SUFFIX(s);
	} 
 
	template <class Key>
	bool HashSet<Key>::apply(UnaryProcessor<ValueType>& processor)
	{
    if (processor.start() == false)
		{
			return false;
		}

    Processor::Result result;

		Iterator it = begin();
		while (it != end())
		{
			result = processor(*it);
			if (result <= Processor::BREAK)
			{
				return (result == Processor::BREAK);
			}
			it++;
		}

    return processor.finish();
	}

	template <class Key>
	BALL_INLINE 
	HashIndex HashSet<Key>::hash(const Key& key) const
	{
		return Hash(key);
	}

	template <class Key>
	BALL_INLINE 
	void HashSet<Key>::rehash()
	{
		capacity_ = (Size)getNextPrime(bucket_.size() << 1);
	}

  template <class Key>
  void HashSet<Key>::deleteBuckets_()
  {
    Size i = 0;
    Node* node = 0;
    Node* next_node = 0;
    for (i = 0; i < bucket_.size(); i++)
    {
      node = bucket_[i];
      while (node != 0)
      {
        next_node = node->next;
        delete node;
        node = next_node;
			}
      bucket_[i] = 0;
		}
	}

	template <class Key>
	BALL_INLINE 
	HashSet<Key>::Node* HashSet<Key>::newNode_
		(const ValueType& value, HashSet<Key>::Node* next) const
	{
		return new Node(value, next);
	}

	template <class Key>
	BALL_INLINE 
	void HashSet<Key>::deleteNode_(HashSet<Key>::Node* node) const
	{
		delete node;
	}

	template <class Key>
	BALL_INLINE 
	bool HashSet<Key>::needRehashing_() const
	{
		return (size_ >= capacity_);
	}

	template <class Key>
	BALL_INLINE 
	HashIndex HashSet<Key>::hashBucket_(const Key& key) const
	{
		return (Position)((HashIndex)hash(key) % (HashIndex)bucket_.size());
	}

	template <class Key>
	void HashSet<Key>::rehash_()
  {
    // calculate the new number of buckets (in capacity_)
    rehash();

    // save the old contents
    vector<Node*> old_buckets(bucket_);

    // resize the bucket vector and initialize it with zero
    bucket_.clear();
    bucket_.resize(capacity_);
    Position i;
    for (i = 0; i < capacity_; i++)
    {
      bucket_[i] = 0;
		}

    // rehash the old contents into the new buckets
    Node* node;
    Node* next_node;
    for (Position i = 0; i < (Position)old_buckets.size(); ++i)
    {
      for (node = old_buckets[i]; node != 0; node = next_node)
      {
        next_node = node->next;
        Position new_bucket = hashBucket_(node->value);
        node->next = bucket_[new_bucket];
        bucket_[new_bucket] = node;
			}
		}
	}
 
} // namespace BALL

#endif // BALL_DATATYPE_HASHSET_H
