// $Id: resourceFile.h,v 1.1 1999/08/26 07:53:20 oliver Exp $

#ifndef BALL_FORMAT_RESFILE_H
#define BALL_FORMAT_RESFILE_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_FORWARDITERATOR_H
#	include <BALL/CONCEPT/forwardIterator.h>
#endif

#ifndef BALL_CONCEPT_VISITOR_H
#	include <BALL/CONCEPT/visitor.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
#	include <BALL/DATATYPE/string.h>
#endif

#ifndef BALL_CONCEPT_PREDICATE_H
#	include <BALL/CONCEPT/predicate.h>
#endif

#ifndef BALL_CONCEPT_PROCESSOR_H
#	include <BALL/CONCEPT/processor.h>
#endif

#ifndef BALL_SYSTEM_FILE_H
#	include <BALL/SYSTEM/file.h>
#endif

namespace BALL 
{

	/**	@name	Support for hierarchical text data.
	*/
	//@{ 

	/**	Resource entry class
	*/	
	class ResourceEntry
	{
		public:
		
		BALL_CREATE(ResourceEntry)

		/**	@name	Constructors and Destructors 
		*/
		//@{

		/**	Default constructor
		*/
		ResourceEntry();
			
		/**	Copy constructor.
				Creates a new resource entry from another.
				@param entry the ResourceEntry object to be copied
				@param deep create a deep ({\bf true}) or shallow ({\bf false}) copy
		*/
		ResourceEntry(const ResourceEntry& entry, bool deep = true);

		/**	
		*/
		ResourceEntry(const String& key, const String& value, ResourceEntry* parent = 0);

		/**	
		*/
		virtual ~ResourceEntry(void);
		//@}

		/**	@name	Assignment
		*/
		//@{

		/**	Assign a resource entry from another.
				@param entry the ResourceEntry object to be copied
				@param deep create a deep ({\bf true}) or shallow ({\bf false}) copy
		*/
		void set(const ResourceEntry& entry, bool deep = true);

		/**	Assignment operator.
				@param entry the ResourceEntry object to be copied
		*/
		ResourceEntry& operator = (const ResourceEntry &entry);
	
		/**	Assign the contents of a resource enytry to another one.
				@param entry the ResourceEntry object to be copied
				@param deep create a deep ({\bf true}) or shallow ({\bf false}) copy
		*/
		void get(ResourceEntry& entry, bool deep = true) const;
		//@}
	

		/**	@name	Miscellaneous
		*/
		//@{

		/**	
		*/
		ResourceEntry& getRoot(void);

		/**	
		*/
		const ResourceEntry& getRoot(void) const;

		/**	
		*/
		ResourceEntry* getParent(void);

		/**	
		*/
		const ResourceEntry* getParent(void) const;

		/**	
		*/
		ResourceEntry* getChild(Index index);

		/**	
		*/
		const ResourceEntry* getChild(Index index) const;

		/**	
		*/
		ResourceEntry* getEntry(const String& key_path);

		/**	
		*/
		const ResourceEntry* getEntry(const String& key_path) const;

		/**	
		*/
		const String& getKey(void) const;

		/**	
		*/
		void setValue(const String& value);

		/**	
		*/
		String& getValue(void);

		/**	
		*/
		const String& getValue(void) const;

		/**	
		*/
		String getPath(void) const;

		/**	
		*/
		Size countChildren(void) const;

		/**	
		*/
		Size countDescendants(void) const;

		/**	
		*/
		Size getSize(void) const;
	
		/**	
		*/
		Size getDepth(void) const;
	
		/**	
		*/
		ResourceEntry* insertChild(const String& key, const String& value, bool replace_value = true);

		/**	
		*/
		ResourceEntry* insertSibling(const String& key, const String& value, bool replace_value = true);

		/**	
		*/
		ResourceEntry* insertChild(ResourceEntry& entry, bool replace_value = true);
	
		/**	
		*/
		ResourceEntry* insert(const String& key_path, const String& name);

		/**	
		*/
		bool mergeChildrenOf(ResourceEntry& entry, bool replace_value = true);

		/**	
		*/
		bool removeChild(const String& key, ResourceEntry **removed = 0);

		/**	
		*/
		bool removeKey(const String& key_path);

		/**	
		*/
		void clear(void);

		/**	
		*/
		void destroy(void);

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

		/**	
		*/
		void host(Visitor<ResourceEntry>& visitor);

		/**	
		*/
		bool hasChild(const String& key) const;

		/**	
		*/
		bool isEmpty(void) const;

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
		bool isRoot(void) const;

		/**	
		*/
		bool isValid(void) const;

		/**	
		*/
		void dump(ostream& s = cout, unsigned long depth = 0) const;

		/**	
		*/
		bool apply(UnaryProcessor<ResourceEntry>& processor);

		/**	
		*/
		bool applyChildren(UnaryProcessor<ResourceEntry>& processor);
		//@}

		typedef ResourceEntry* IteratorPosition;
	
		class IteratorTraits_
		{
			public:

			BALL_CREATE(IteratorTraits_)

			IteratorTraits_(void)
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
				for (register Index index = 0; index < (Index)stack_capacity_; ++index)
					stack_index_[index] = traits.stack_index_[index];
			}

			virtual ~IteratorTraits_(void)
			{
				delete [] stack_index_;
			}
			
			IteratorTraits_ &operator = (const IteratorTraits_ &traits)
			{
				bound_ = traits.bound_;
				position_ = traits.position_;
				delete [] stack_index_;
				stack_index_ = new Index[traits.stack_capacity_];
				stack_capacity_ = traits.stack_capacity_;
				stack_size_ = traits.stack_size_;
		
				for (register Index index = 0; index < (Index)stack_capacity_; ++index)
				{
					stack_index_[index] = traits.stack_index_[index];
				}

				return *this;
			}

			ResourceEntry* getContainer(void)
			{
				return bound_;
			}
			
			const ResourceEntry* getContainer(void) const
			{
				return bound_;
			}
			
			bool isSingular(void) const
			{
				return (bool)(bound_ == 0);
			}
			
			IteratorPosition &getPosition(void)
			{
				return position_;
			}

			const IteratorPosition &getPosition(void) const
			{
				return position_;
			}

			bool operator == (const IteratorTraits_ &traits) const
			{
				return (bool)(position_ == traits.position_);
			}

			bool operator != (const IteratorTraits_ &traits) const
			{
				return (bool)(position_ != traits.position_);
			}
			
			bool isValid(void) const
			{
				return (bool)(bound_ != 0
					&& position_ != 0
					&& stack_index_ != 0
					&& stack_size_ <= stack_capacity_
					&& stack_capacity_ > 0);
			}

			void invalidate(void)
			{
				bound_ = 0;
				position_ = 0;
				stack_size_ = 0;
			}
			
			void toBegin(void)
			{
				stack_size_ = 0;
				position_ = bound_;
			}

			bool isBegin(void) const
			{
				return (bool)(position_ == bound_);
			}

			void toEnd(void)
			{
				position_ = 0;
			}
			
			bool isEnd(void) const
			{
				return (bool)(position_ == 0);
			}

			ResourceEntry &getData(void)
			{
				return *position_;
			}
			
			const ResourceEntry& getData(void) const
			{
				return *position_;
			}
			
			void forward(void)
			{
				if (position_->number_children_ > 0)
				{
					position_ = position_->child_[0];

					push(0);

				} else {

					register Index next_child = INVALID_INDEX;

					do 
					{
						next_child = pop();
						if (next_child == INVALID_INDEX)
						{
							position_ = 0;

							return;

						} else {

							position_ = position_->parent_;

						}
					} while(++next_child >= (Index)position_->number_children_);

					push(next_child);

					position_ = position_->child_[next_child];
				}
			}

			// traits-specific:
			Size getDepth(void) const
			{
				return stack_size_;
			}

			private:

			void push(Index next_child)
			{
				if (stack_size_ >= stack_capacity_)
				{
					Index *new_stack_index = new Index[stack_capacity_ << 1];

					for (register Index index = 0; index < (Index)stack_capacity_; ++index)
						new_stack_index[index] = stack_index_[index];

					delete [] stack_index_;
					stack_index_ = new_stack_index;
					stack_capacity_ <<= 1;
				}

				stack_index_[stack_size_] = next_child;
				++stack_size_;

			}

			Index pop(void)
			{
				if (stack_size_ == 0)
				{
					return INVALID_INDEX;
				} else {
					return stack_index_[--stack_size_];
				}
			}

			ResourceEntry *bound_;
			IteratorPosition position_;
			Index *stack_index_;
			Size stack_capacity_;
			Size stack_size_ ;
		};

		friend class IteratorTraits_;

		typedef ForwardIterator<ResourceEntry, ResourceEntry, ResourceEntry* , IteratorTraits_> Iterator;

		Iterator begin(void)
		{
			return Iterator::begin(*this);
		}

		Iterator end(void)
		{
			return Iterator::end(*this);
		}



		typedef ConstForwardIterator<ResourceEntry, ResourceEntry, ResourceEntry*, IteratorTraits_> ConstIterator;

		ConstIterator begin(void) const
		{
			return ConstIterator::begin(*this);
		}

		ConstIterator end(void) const
		{
			return ConstIterator::end(*this);
		}



		protected:

		virtual ResourceEntry* newEntry
			(const String& key, const String& value, ResourceEntry* parent = 0) const;

		virtual ResourceEntry** newEntryArray(Size size) const;

		virtual void deleteEntry(ResourceEntry* entry) const;
	
		virtual void deleteEntryArray(ResourceEntry** entry_array) const;
	
		bool applyNostart_(UnaryProcessor<ResourceEntry>& processor);



		private:

		ResourceEntry *clone_(ResourceEntry* parent) const;

		bool findGreaterOrEqual_(const String& key, Index& found) const;

		String 						key_;
		String 						value_;
		ResourceEntry* 		parent_;
		ResourceEntry** 	child_;
		Size 							number_children_;
	};


	/**	Resource file class
	*/
	class ResourceFile
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
			ResourceFile(void);

			/**	
			*/
			ResourceFile(const String& name);

			/**	Destructor
			*/
			virtual ~ResourceFile(void);
			//@}
			
			bool open(const String& name);

			void close(void);

			static void saveAs(const Entry& entry, const String& name);

			void saveAs(const String& name);

			void save(const Entry& entry);

			void save(void);

			Size getSize(void) const;
		
			Entry& getRoot(void);

			const Entry& getRoot(void) const;

			Entry* getEntry(const String& key_path);

			const Entry* getEntry(const String& key_path) const;

			String* getValue(const String& key_path);

			const String* getValue(const String& key_path) const;

			Entry* insert(const String& key_path, const String& name);

			bool removeKey(const String& key_path);

			void destroy(void);

			void host(Visitor<ResourceFile>& visitor);
		
			bool hasKey(const String& key_path) const;

			bool isEmpty(void) const;
		

			/**	@name	Debugging and Diagnostics
			*/
			//@{
			
			/**	
			*/
			bool isValid(void) const;

			/**	
			*/
			void dump(ostream& s = cout, unsigned long depth = 0) const;
			//@}


			/**	@name	Storers
			*/	
			//@{
			
			/**
			*/
			friend istream& operator >> (istream& s, ResourceFile& resource_file);
			//@}

			bool apply(UnaryProcessor<Entry>& processor);

			static bool applyChildren(Entry& entry, UnaryProcessor<Entry>& processor);

			typedef Entry::Iterator Iterator;

			Iterator begin(void)
			{
				return Iterator::begin(root_);
			}

			Iterator end(void)
			{
				return Iterator::end(root_);
			}


			typedef Entry::ConstIterator ConstIterator;
		
			ConstIterator begin(void) const
			{
				return ConstIterator::begin(root_);
			}

			ConstIterator end(void) const
			{
				return ConstIterator::end(root_);
			}

		private:

			ResourceFile(const ResourceFile& file);

			ResourceFile& operator = (const ResourceFile& file);

			static void save_(File& file, const Entry* entry, unsigned long& depth);

			bool validateSyntax_(void);

			void skipWhitespaces_(void)
			{
				char c = 0;

				while(get(c) && isspace(c));

				putback(c);
			}

			Entry root_;
	};

	//@}

#	ifndef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/FORMAT/resourceFile.iC>
#	endif

} // namespace BALL

#endif // BALL_FORMAT_RESFILE_H
