// $Id: hashSet.h,v 1.7 2000/03/29 17:25:17 oliver Exp $ 

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

namespace BALL
{

  /** Generic Hash Set Class.
      {\bf Definition:} \URL{BALL/DATATYPE/hashMap.h}
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
				version of operator \[\].
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


		/**
		*/
		virtual void clear();
	
		/**
		*/
		void destroy();
		//@}
		
		/**	@name Assignment 
		*/
		//@{

		/**
		*/
		void set(const HashSet& hash_set);

		/**
		*/
		HashSet& operator = (const HashSet& hash_set);

		/**
		*/
		void get(HashSet& hash_set) const;

		/**
		*/
		void swap(HashSet& hash_set);
		//@}

		/**	@name	Accessors
		*/
		//@{
		/**	Return the number of buckets.
		*/
		Size getBucketSize() const;

		/**
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

		std::pair<Iterator, bool> insert(const ValueType& item);

		void erase(Iterator pos);

		Size erase(const KeyType& key);

		void erase(Iterator f, Iterator l);
		//@}

		/**	@name Miscellaneous
		*/
		//@{

		void host(Visitor<ValueType>& visitor);
		//@}
	
		/**	@name	Predicates
		*/
		//@{

		bool has(const Key& key) const;

		bool isEmpty() const;

		bool operator == (const HashSet& hash_set) const;

		bool operator != (const HashSet& hash_set) const;
		//@}

		/**	@name	Debugging and Diagnostics
		*/
		//@{

		bool isValid() const;

		virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;
		//@}

		// --- STORERS

		/*
		friend istream& operator >> BALL_TEMPLATE_NULL_ARGS (std::istream& s, HashSet& hash_set);
			
		friend ostream& operator << BALL_TEMPLATE_NULL_ARGS (std::ostream& s, const HashSet& hash_set);

		void read(std::istream& s);

		void write(std::ostream& s) const;
		*/      

		// --- INTERNAL ITERATORS

		bool apply(UnaryProcessor<ValueType>& processor);

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
				return (bool)(bound_ == 0);
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
				return (bool)(position_ == traits.position_);
			}

			bool operator != (const IteratorTraits_& traits) const
			{
				return (bool)(position_ != traits.position_);
			}
			
			bool isValid() const
			{
				return (bool)((bound_ != 0) && (position_ != 0)
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
						} else {
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
				return (bool)(position_ == 0);
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

		Iterator begin()
		{
			return Iterator::begin(*this);
		}

		Iterator end()
		{
			return Iterator::end(*this);
		}

		ConstIterator begin() const
		{
			return ConstIterator::begin(*this);
		}

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
		Size					size_;

		/*_	The capacity - usually the number of buckets
		*/
		Size					capacity_;

		/*_	Buckets are stored as a vector of linked lists of Nodes 
		*/
		vector<Node*>							bucket_;
	};

	template <class Key>
	HashSet<Key>::HashSet
		(Size initial_capacity, Size number_of_buckets)
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
	HashSet<Key>::HashSet
		(const HashSet& hash_set)
		:	size_(hash_set.size_),
			capacity_(hash_set.capacity_),
			bucket_(hash_set.bucket_.size())
	{
		Node* node = 0;
		
		for (Position bucket = 0; bucket < (Position)bucket_.size(); ++bucket)
		{
			bucket_[bucket] = 0;

			for (node = hash_set.bucket_[bucket]; node != 0; node = node->next)
			{
				bucket_[bucket] = newNode_(node->value, bucket_[bucket]);
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
	void HashSet<Key>::set
		(const HashSet& hash_set)
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

		PointerType item = 0;
		
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
	void HashSet<Key>::swap(HashSet& hash_set)
	{
		swap(size_, hash_set.size_);
		swap(capacity_, hash_set.capacity_);
		swap(bucket_, hash_set.bucket_);
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
	BALL_INLINE 
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
	Size HashSet<Key>::erase(const KeyType& key)
	{
		Position	bucket = hashBucket_(key);
		Node*			previous = 0;
		Node*			node_ptr = bucket_[bucket];
		
		for (; node_ptr != 0; node_ptr = node_ptr->next)
		{
			if (node_ptr->value == key)
			{
				break;
			}

			previous = node_ptr;
		}

		if (node_ptr != 0)
		{
			if (node_ptr == bucket_[bucket])
			{
				bucket_[bucket] = node_ptr->next;
			} else {
				previous->next = node_ptr->next;
			}

			deleteNode_(node_ptr);

			--size_;

			return true;
		} else
		{
			return false;
		}
	}

	template <class Key>
	BALL_INLINE 
	void HashSet<Key>::host(Visitor<ValueType>& visitor)
	{
		visitor.visit(*this);
	}
		
	template <class Key>
	BALL_INLINE 
	bool HashSet<Key>::has(const Key& key) const
	{
		return (bool)(find(key) != end());
	}

	template <class Key>
	BALL_INLINE 
	bool HashSet<Key>::isEmpty() const
	{
		return (bool)(size_ == 0);
	}

	template <class Key>
	BALL_INLINE 
	bool HashSet<Key>::operator == (const HashSet& hash_set) const
	{
		return (bool)(size_ == hash_set.size_ && isSubsetOf(hash_set));
	}

	template <class Key>
	BALL_INLINE
	bool HashSet<Key>::operator != (const HashSet& hash_set) const
	{
		return (bool)!(*this == hash_set);
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

		return (bool)(size_ == size);
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
  BALL_INLINE
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
	bool HashSet<Key>::apply(UnaryProcessor<ValueType>& processor)
	{
		//BAUSTELLE
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
	HashIndex HashSet<Key>::hash(const Key& key) const
	{
		return Hash(key);
	}

	template <class Key>
	BALL_INLINE 
	bool HashSet<Key>::needRehashing_() const
	{
		return (bool)(size_ >= capacity_);
	}

	template <class Key>
	BALL_INLINE 
	void HashSet<Key>::rehash()
	{
		capacity_ = (Size)getNextPrime(bucket_.size() << 1);
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
