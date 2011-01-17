// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_FORMAT_RESOURCEFILE_H
#define BALL_FORMAT_RESOURCEFILE_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_FORWARDITERATOR_H
#	include <BALL/CONCEPT/forwardIterator.h>
#endif

#ifndef BALL_CONCEPT_VISITOR_H
#	include <BALL/CONCEPT/visitor.h>
#endif

#ifndef BALL_CONCEPT_AUTODELETABLE_H
#	include <BALL/CONCEPT/autoDeletable.h>
#endif

#ifndef BALL_CONCEPT_PROCESSOR_H
#	include <BALL/CONCEPT/processor.h>
#endif

#ifndef BALL_SYSTEM_FILE_H
#	include <BALL/SYSTEM/file.h>
#endif

namespace BALL 
{
	/**	\defgroup GeneralHierarchical Support for hierarchical text data.
			\ingroup General
	*/

	/**	Resource entry class
    	\ingroup  GeneralHierarchical
	*/	
	class BALL_EXPORT ResourceEntry
		:	public AutoDeletable
	{
		public:
		
		BALL_CREATE_DEEP(ResourceEntry)

		/**	@name	Constructors and Destructors 
		*/
		//@{

		/**	Default constructor
		*/
		ResourceEntry();
			
		/**	Copy constructor.
				Creates a new resource entry from another.
				@param entry the ResourceEntry object to be copied
				@param deep create a deep (<b>true</b>) or shallow (<b>false</b>) copy
		*/
		ResourceEntry(const ResourceEntry& entry, bool deep = true);

		/**	Create a new resource entry.
				An entry with <tt>key</tt> and <tt>value</tt> is created. If <tt>parent</tt> is given,
				the new entries parent pointer is set to this value. Take care: the parents
				child array is <b>  not </b> updated - so use this option only if you really know 
				what you are doing. To keep this data consistent, use  \link insertChild insertChild \endlink .
				@param	key the key of the new entry
				@param	value the value of the new entry
				@param	parent the content of the new entry's parent pointer
		*/
		ResourceEntry(const String& key, const String& value, ResourceEntry* parent = 0);

		/**	
		*/
		virtual ~ResourceEntry();

		/**	
		*/
		void clear();

		/**	
		*/
		void destroy();

		//@}

		/**	@name	Assignment
		*/
		//@{

		/**	Assign a resource entry from another.
				@param entry the ResourceEntry object to be copied
				@param deep create a deep (<b>true</b>) or shallow (<b>false</b>) copy
		*/
		void set(const ResourceEntry& entry, bool deep = true);

		/**	Assignment operator.
				@param entry the ResourceEntry object to be copied
		*/
		const ResourceEntry& operator = (const ResourceEntry &entry);
	
		/**	Assign the contents of a resource enytry to another one.
				@param entry the ResourceEntry object to be copied
				@param deep create a deep (<b>true</b>) or shallow (<b>false</b>) copy
		*/
		void get(ResourceEntry& entry, bool deep = true) const;
		//@}
	

		/**	@name	Miscellaneous
		*/
		//@{

		/** Equality operator
		*/
		bool operator == (const ResourceEntry& entry) const;

		/** Inequality operator
		*/
		bool operator != (const ResourceEntry& entry) const;

		/**	
		*/
		ResourceEntry& getRoot();

		/**	
		*/
		const ResourceEntry& getRoot() const;

		/**	
		*/
		ResourceEntry* getParent();

		/**	
		*/
		const ResourceEntry* getParent() const;

		/**	Return a pointer to a child node.	
				@return 0 if the child <tt>index</tt> does not exist
		*/
		ResourceEntry* getChild(Position index);

		/**	Return a const pointer to a child node.	
				@return 0 if the child <tt>index</tt> does not exist
		*/
		const ResourceEntry* getChild(Position index) const;

		/**	Return a pointer to an entry.
				Searches for an entry via its path.
				@return 0 if the specified path does not exist
		*/
		ResourceEntry* getEntry(const String& key_path);

		/**	Return a const pointer to an entry
				Searches for an entry via its path.
				@return 0 if the specified path does not exist
		*/
		const ResourceEntry* getEntry(const String& key_path) const;

		/**	Return the key of this ResourceEntry.
		*/
		const String& getKey() const;

		/** Set the value of this ResourceEntry.
		*/
		void setValue(const String& value);

		/**	Get a mutable reference to the value of this ResourceEntry.
		*/
		String& getValue();

		/**	Get a constant reference to the value of this ResourceEntry.
		*/
		const String& getValue() const;

		/**	Return the path of an entry.
		*/
		String getPath() const;

		/**	Return the number of child entries.
		*/
		Size countChildren() const;

		/**	Return the number of children which are not leaves.
		*/
		Size countDescendants() const;

		/**	????? What is the size of an entry? Why not use size()?
		*/
		Size getSize() const;
	
		/**	Compute the depth of this entry within the tree.
		*/
		Size getDepth() const;
	
		/**	Insert a child into the tree below this entry, if the key has not
				been used yet.
		*/
		ResourceEntry* insertChild(const String& key, const String& value, bool replace_value = true);

		/**	Insert a child into the tree below the parent entry.
		*/
		ResourceEntry* insertSibling(const String& key, const String& value, bool replace_value = true);

		/**	Insert a child into the tree below this entry, if the key has not
				been used yet.
		*/
		ResourceEntry* insertChild(ResourceEntry& entry, bool replace_value = true);
	
		/**	Insert a resource entry at the appropriate place in the tree.
		*/
		ResourceEntry* insert(const String& key_path, const String& name);

		/**	Merge the children of a resource entry.
		*/
		bool mergeChildrenOf(ResourceEntry& entry, bool replace_value = true);

		/** Remove a child of a resource entry.
		*/
		bool removeChild(const String& key, ResourceEntry** removed = 0);

		/**	
		*/
		bool removeKey(const String& key_path);

		/**	
		*/
		ResourceEntry* findChild(const String& key);

		/**	
		*/
		const ResourceEntry* findChild(const String& key) const;

		/**	
		*/
		ResourceEntry* findDescendant(const String& key);

		/**	
		*/
		const ResourceEntry* findDescendant(const String& key) const;

		/**	
		*/
		ResourceEntry* findEntry(const String& key);

		/**	
		*/
		const ResourceEntry* findEntry(const String& key) const;
		//@}

		/**	@name	Attributes
		*/
		//@{

		/**	
		*/
		bool hasChild(const String& key) const;

		/**	
		*/
		bool isEmpty() const;

		/**	
		*/
		bool isParentOf(const ResourceEntry& entry) const;

		/**	
		*/
		bool isChildOf(const ResourceEntry& entry) const;
	
		/**	
		*/
		bool isAncestorOf(const ResourceEntry& entry) const;

		/**	
		*/
		bool isDescendantOf(const ResourceEntry& entry) const;

		/**	
		*/
		bool isRelatedWith(const ResourceEntry& entry) const;
	
		/**	
		*/
		bool isRoot() const;

		//@}

		/**	@name	Debugging and Diagnostics
		*/
		//@{
		/**	
		*/
		bool isValid() const;
		/**	
		*/
		void dump(std::ostream& s = std::cout, Size depth = 0) const;
		//@}


		/**	@name	Processor and Visitor related methods
		*/
		//@{

		/**	
		*/
		void host(Visitor<ResourceEntry>& visitor);

		/**	
		*/
		bool apply(UnaryProcessor<ResourceEntry>& processor);

		/**	
		*/
		bool applyChildren(UnaryProcessor<ResourceEntry>& processor);
		//@}

		typedef ResourceEntry* IteratorPosition;
	
		class BALL_EXPORT IteratorTraits_
		{
			public:

			BALL_CREATE_DEEP(IteratorTraits_)

			IteratorTraits_()
				:	bound_(0),
					position_(0),
					stack_index_(new Index[1]),
					stack_capacity_(1),
					stack_size_(0)
			{
			}
			
			IteratorTraits_(const ResourceEntry& entry)
				:	bound_((ResourceEntry *)&entry),
					position_(0),
					stack_index_(new Index[1]),
					stack_capacity_(1),
					stack_size_(0)
			{
			}
			
			IteratorTraits_(const IteratorTraits_& traits, bool /* deep */ = true)
				:	bound_(traits.bound_),
					position_(traits.position_),
					stack_index_(new Index[traits.stack_capacity_]),
					stack_capacity_(traits.stack_capacity_),
					stack_size_(traits.stack_size_)
			{
				for (Index index = 0; index < (Index)stack_capacity_; ++index)
					stack_index_[index] = traits.stack_index_[index];
			}

			virtual ~IteratorTraits_()
			{
				delete[] stack_index_;
			}
			
			const IteratorTraits_& operator = (const IteratorTraits_ &traits)
			{
				bound_ = traits.bound_;
				position_ = traits.position_;
				delete[] stack_index_;
				stack_index_ = new Index[traits.stack_capacity_];
				stack_capacity_ = traits.stack_capacity_;
				stack_size_ = traits.stack_size_;
		
				for (Index index = 0; index < (Index)stack_capacity_; ++index)
				{
					stack_index_[index] = traits.stack_index_[index];
				}

				return *this;
			}

			ResourceEntry* getContainer()
			{
				return bound_;
			}
			
			const ResourceEntry* getContainer() const
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
				return (bound_ != 0 && position_ != 0
								&& stack_index_ != 0 
								&& stack_size_ <= stack_capacity_
								&& stack_capacity_ > 0);
			}

			void invalidate()
			{
				bound_ = 0;
				position_ = 0;
				stack_size_ = 0;
			}
			
			void toBegin()
			{
				stack_size_ = 0;
				position_ = bound_;
			}

			bool isBegin() const
			{
				return (position_ == bound_);
			}

			void toEnd()
			{
				position_ = 0;
			}
			
			bool isEnd() const
			{
				return (position_ == 0);
			}

			ResourceEntry& getData()
			{
				return *position_;
			}
			
			const ResourceEntry& getData() const
			{
				return *position_;
			}
			
			void forward()
			{
				if (position_->number_children_ > 0)
				{
					position_ = position_->child_[0];
					push(0);

				} 
				else 
				{
					Index next_child = INVALID_INDEX;

					do 
					{
						next_child = pop();
						if (next_child == INVALID_INDEX)
						{
							position_ = 0;
							return;
						} 
						else 
						{
							position_ = position_->parent_;
						}

					} while(++next_child >= (Index)position_->number_children_);

					push(next_child);
					position_ = position_->child_[next_child];
				}
			}

			// traits-specific:
			Size getDepth() const
			{
				return stack_size_;
			}

			private:

			void push(Index next_child)
			{
				if (stack_size_ >= stack_capacity_)
				{
					Index *new_stack_index = new Index[stack_capacity_ << 1];

					for (Index index = 0; index < (Index)stack_capacity_; ++index)
					{
						new_stack_index[index] = stack_index_[index];
					}

					delete[] stack_index_;
					stack_index_ = new_stack_index;
					stack_capacity_ <<= 1;
				}

				stack_index_[stack_size_] = next_child;
				++stack_size_;

			}

			Index pop()
			{
				if (stack_size_ == 0)
				{
					return INVALID_INDEX;
				}
				else 
				{
					return stack_index_[--stack_size_];
				}
			}

			ResourceEntry*			bound_;
			IteratorPosition		position_;
			Index*							stack_index_;
			Size								stack_capacity_;
			Size								stack_size_;
		};

		friend class IteratorTraits_;

		typedef ForwardIterator<ResourceEntry, ResourceEntry, ResourceEntry*, IteratorTraits_> Iterator;
		typedef ConstForwardIterator<ResourceEntry, ResourceEntry, ResourceEntry*, IteratorTraits_> ConstIterator;

		Iterator begin() { return Iterator::begin(*this); }
		Iterator end() { return Iterator::end(*this); }


		ConstIterator begin() const	{	return ConstIterator::begin(*this); }
		ConstIterator end() const	{	return ConstIterator::end(*this); }

		protected:

		virtual ResourceEntry* newEntry
			(const String& key, const String& value, ResourceEntry* parent = 0) const;

		virtual ResourceEntry** newEntryArray(Size size) const;

		virtual void deleteEntry(ResourceEntry* entry) const;
	
		virtual void deleteEntryArray(ResourceEntry** entry_array) const;
	
		bool applyNostart_(UnaryProcessor<ResourceEntry>& processor);


		private:

		ResourceEntry* clone_(ResourceEntry* parent) const;
		bool findGreaterOrEqual_(const String& key, Index& found) const;

		String 						key_;
		String 						value_;
		ResourceEntry* 		parent_;
		ResourceEntry** 	child_;
		Size 							number_children_;
	};


	/**	Resource file class
    	\ingroup  GeneralHierarchical
	*/
	class BALL_EXPORT ResourceFile
		: public File
	{
		public:
		
		/**	@name	Constants
		*/	
		//@{
			
		/**	
		*/
		static char ENTRY_BEGIN;

		/**	
		*/
		static char ENTRY_END;

		/**	
		*/
		static char SEPARATOR;
		//@}

		/**	@name	Type Definition
		*/
		//@{
			
		/**
		*/
		typedef ResourceEntry Entry;
		//@}

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor
		*/
		ResourceFile();

		/**	Detailed constructor.
		 *  @throw Exception::FileNotFound if the file could not be opened
		 */
		ResourceFile(const String& name);

		/**	Destructor
		*/
		virtual ~ResourceFile();

		/**	Destroy method
		*/
		void destroy();

		/**	Clear method
		*/
		virtual void clear();

		//@}

		
		/**	@name	File-related methods.
		*/
		//@{
			
		/*
		*/
		bool open(const String& name);

		/*
		*/
		void close();

		/*
		*/
		static void saveAs(const Entry& entry, const String& name);

		/*
		*/
		void saveAs(const String& name);

		/*
		*/
		void save(const Entry& entry);

		/*
		*/
		void save();
		//@}


		/**	@name	Accessors
		*/
		//@{
		/*
		*/
		Size getSize() const;
	
		/*
		*/
		Entry& getRoot();

		/*
		*/
		const Entry& getRoot() const;

		/*
		*/
		Entry* getEntry(const String& key_path);

		/*
		*/
		const Entry* getEntry(const String& key_path) const;

		/*
		*/
		String* getValue(const String& key_path);

		/*
		*/
		const String* getValue(const String& key_path) const;

		/*
		*/
		Entry* insert(const String& key_path, const String& name);

		/*
		*/
		bool removeKey(const String& key_path);

		//@}
	
		/**	@name	Attributes
		*/
		//@{

		/** Equality operator
		*/
		bool operator == (const ResourceFile& entry) const;

		/** Inequality operator
		*/
		bool operator != (const ResourceFile& entry) const;

		/*	Return true if the key exists somewhere in the tree.
		*/
		bool hasKey(const String& key_path) const;

		/*	Return true if the entry has no children.
		*/
		bool isEmpty() const;
		//@}
	

		/**	@name	Debugging and Diagnostics
		*/
		//@{
		
		/**	
		*/
		bool isValid() const;

		/**	
		*/
		void dump(std::ostream& s = std::cout, Size depth = 0) const;
		//@}


		/**	@name	Storers
		*/	
		//@{
		
		/**
		*/
		friend std::istream& operator >> (std::istream& s, ResourceFile& resource_file);
		//@}

		/**	@name Processor	and Visitor related methods
		*/
		//@{
	
		/**
		*/
		void host(Visitor<ResourceFile>& visitor);

		/**
		*/
		bool apply(UnaryProcessor<Entry>& processor);

		/**
		*/
		static bool applyChildren(Entry& entry, UnaryProcessor<Entry>& processor);
		//@}

		typedef Entry::Iterator Iterator;

		Iterator begin()
		{
			return Iterator::begin(root_);
		}

		Iterator end()
		{
			return Iterator::end(root_);
		}


		typedef Entry::ConstIterator ConstIterator;
	
		ConstIterator begin() const
		{
			return ConstIterator::begin(root_);
		}

		ConstIterator end() const
		{
			return ConstIterator::end(root_);
		}


		private:

		const ResourceFile& operator = (const ResourceFile& file);

		static void save_(File& file, const Entry* entry, Size& depth);

		bool validateSyntax_();

		void skipWhitespaces_();

		Entry root_;
	};

#	ifndef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/FORMAT/resourceFile.iC>
#	endif
} // namespace BALL

#endif // BALL_FORMAT_RESOURCEFILE_H
