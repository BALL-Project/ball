// $Id: hashGrid.h,v 1.16 2001/03/06 14:02:10 anker Exp $

#ifndef BALL_DATATYPE_HASHGRID_H
#define BALL_DATATYPE_HASHGRID_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
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

#ifndef BALL_CONCEPT_PROCESSOR_H
#	include <BALL/CONCEPT/processor.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

namespace BALL 
{

	/**	@name	Three-dimensional Hash Grid
	*/
	//@{

	/**	Grid Box Class.
			These boxes represent the buckets of a threedimensional hash grid.
			Every such box contains a linear list of the objects that are
			contained in this box. This list is accessible through a
			DataIterator.\\
			{\bf Definition:} \URL{BALL/DATATYPE/hashGrid.h}
	*/
	template <class Item>
	class HashGridBox3
	{
		public:

		/**	@name	Constructors and Destructors 
		*/	
		//@{

		///	Default constructor
		HashGridBox3()
			throw();
			
		/// Copy constructor
		HashGridBox3(const HashGridBox3& grid_box, bool deep = true)
			throw();

		// BUG in egcs: destructor may not be virtual, if a template class contains
		// structs/classes and is contained in a namespace
		/// Destructor
		virtual ~HashGridBox3()
			throw();

		/// Clears the grid box
		virtual void clear()
			throw();

		/** Clears the grid box.
				Same as clear.
		*/
		virtual void destroy()
			throw();
	
		//@}
		/**	@name	Assignment 
		*/
		//@{

		///
		void set(const HashGridBox3& box,bool /* deep */ = true)
			throw();

		///
		const HashGridBox3& operator = (const HashGridBox3& box)
			throw();

		///
		void get(HashGridBox3& box, bool deep = true) const
			throw();

		//@}
		/**	@name	Accessors 
		*/
		//@{

		/** Find an item in the item list of this grid box.
				@param item the item to be searched for
				@return a pointer to the desired item or a NULL pointer, if the 
								item could not be found.
		*/
		Item* find(const Item &item)
			throw();

		/// The const version of find()
		const Item* find(const Item& item) const
			throw();

		/** Counts all items in the data item list.
				@return the size of the data item list.
		*/
		Size getSize() const
			throw();

		/** Insert an item into the data item list of a grid box.
				@param item the item to  be inserted
		*/
		void insert(const Item& item)
			throw();

		/** Remove the first occurrence of a certain item from the data item list.
				@param item the item to be removed
				@return bool - {\tt true}, if the item could be removed, {\tt false} 
								otherwise.
		*/
		bool remove(const Item& item)
			throw();

		/** Remove all occurences of a certain item from the data item list.
				@param item the item to be removed
				@return bool - {\tt true}, if the item could be removed, {\tt false} 
								otherwise.
		*/
		bool removeAll(const Item& item)
			throw();
			
		//@}			
		/**	@name	Miscellaneous 
		*/
		//@{

		/// Host method
		void host(Visitor<HashGridBox3> &visitor)
			throw();

		//@}
		/**	@name	Predicates 
		*/
		//@{

		/// Equality operator
		bool operator == (const HashGridBox3& box) const
			throw();

		/// Inequality operator
		bool operator != (const HashGridBox3& box) const
			throw();

		/** Test whether an item is in the data item list
				@param item
				@return bool - {\tt true} if instance has {\tt item}, {\tt false} otherwise.
		*/
		bool has(const Item& item) const
			throw();

		/** Test, whether this box is empty, i. e. the data item list contains
				nothing
				@return bool - {\tt true}, if {\tt this} is empty. {\tt false} otherwise. 
		*/
		bool isEmpty() const
			throw();

		//@}
		/**	@name	Debugging and Diagnostics 
		*/
		//@{

		///
		bool isValid() const
			throw();

		///
		void dump(std::ostream& s = std::cout, Size depth = 0) const
			throw();

		//@}
		/**	@name	Internal Iterators 
		*/
		//@{

		///
		bool apply(UnaryProcessor<Item>& processor)
			throw();

		///
		bool apply(UnaryProcessor< HashGridBox3<Item> >& processor)
			throw();

		//@}
		/** @name	External Iterators 
		*/
		//@{

		struct DataItem_
		{
			public:
		
			DataItem_(const Item& item, DataItem_* next)
				throw()
			:	item_(item),
				previous_(0),
				next_(next)
			{
				if (next_ != 0)
				{
					next_->previous_ = this;
				}
			}

			~DataItem_()
				throw()
			{
			}

			Item 			item_;
			DataItem_* previous_;
			DataItem_* next_;
		};
			
		struct NeighbourBoxItem_
		{
			public:
		
			NeighbourBoxItem_(HashGridBox3* box, NeighbourBoxItem_* next)
				throw()
				: box_(box),
					previous_(0),
					next_(next)
			{
				if (next_ != 0)
				{
					next_->previous_ = this;
				}
			}

			~NeighbourBoxItem_()
				throw()
			{
			}

			HashGridBox3<Item>* 	box_;
			NeighbourBoxItem_* 		previous_;
			NeighbourBoxItem_* 		next_;
		};


		typedef NeighbourBoxItem_* BoxIteratorPosition;
		
		class BoxIteratorTraits_
		{
			public:

			BALL_CREATE_DEEP(BoxIteratorTraits_)

			BoxIteratorTraits_()
				throw()
				:	bound_(0),
					position_(0)
			{
			}
				
			BoxIteratorTraits_(const HashGridBox3& box)
				throw()
				:	bound_((HashGridBox3 *)&box),
					position_(0)
			{
			}
				
			BoxIteratorTraits_(const BoxIteratorTraits_& traits, bool /* deep */ = true)
				throw()
				:	bound_(traits.bound_),
					position_(traits.position_)
			{
			}
				
			const BoxIteratorTraits_& operator = (const BoxIteratorTraits_& traits)
				throw()
			{
				bound_ = traits.bound_;
				position_ = traits.position_;
				return *this;
			}

			HashGridBox3* getContainer()
				throw()
			{
				return bound_;
			}

			const HashGridBox3* getContainer() const
				throw()
			{
				return bound_;
			}

			bool isSingular() const
				throw()
			{
				return (bound_ == 0);
			}
				
			BoxIteratorPosition& getPosition()
				throw()
			{
				return position_;
			}

			const BoxIteratorPosition& getPosition() const
				throw()
			{
				return position_;
			}

			bool operator == (const BoxIteratorTraits_& traits) const
				throw()
			{
				return (position_ == traits.position_);
			}

			bool operator != (const BoxIteratorTraits_& traits) const
				throw()
			{
				return (position_ != traits.position_);
			}
				
			bool isValid() const
				throw()
			{
				return (bound_ != 0 && position_ != 0);
			}

			void invalidate()
				throw()
			{
				bound_ = 0;
				position_ = 0;
			}

			void toBegin()
				throw()
			{
				position_ = bound_->first_neighbour_;
			}

			bool isBegin() const
				throw()
			{
				return (position_ == bound_->first_neighbour_);
			}

			void toEnd()
				throw()
			{
				position_ = 0;
			}

			bool isEnd() const
				throw()
			{
				return (position_ == 0);
			}

			HashGridBox3<Item>& getData()
				throw()
			{
				return *(position_->box_);
			}

			const HashGridBox3<Item>& getData() const
				throw()
			{
				return *(position_->box_);
			}

			void forward()
				throw()
			{
				position_ = position_->next_;
			}
		
			private:

			HashGridBox3<Item> *bound_;
			BoxIteratorPosition position_;
		};

		friend class BoxIteratorTraits_;

		/** BoxIterators iterate over all non-empty boxes that lie in the
				direct neighbourhood to a box. Such an iterator traverses at most
				26 boxes.
		*/
		typedef ForwardIterator
			<HashGridBox3<Item>, HashGridBox3<Item>,
			BoxIteratorPosition, BoxIteratorTraits_>
				BoxIterator;

		/// get the first non-empty box 
		BoxIterator beginBox()
			throw()
		{
			return BoxIterator::begin(*this);
		}

		/// get the last non-empty box
		BoxIterator endBox()
			throw()
		{
			return BoxIterator::end(*this);
		}


		/// This is the const version of \Ref{BoxIterator}
		typedef ConstForwardIterator
			<HashGridBox3<Item>, HashGridBox3<Item>,
			BoxIteratorPosition, BoxIteratorTraits_>
				ConstBoxIterator;

		/// get the first non-empty box 
		ConstBoxIterator beginBox() const
			throw()
		{
			return ConstBoxIterator::begin(*this);
		}

		/// get the last non-empty box
		ConstBoxIterator endBox() const
			throw()
		{
			return ConstBoxIterator::end(*this);
		}


		typedef DataItem_* DataIteratorPosition;
		
		class DataIteratorTraits_
		{
			public:

			BALL_CREATE_DEEP(DataIteratorTraits_)

			DataIteratorTraits_()
				throw()
			:	bound_(0),
				position_(0)
			{
			}
				
			DataIteratorTraits_(const HashGridBox3& box)
				throw()
			:	bound_((HashGridBox3 *)&box),
				position_(0)
			{
			}
				
			DataIteratorTraits_(const DataIteratorTraits_& traits, bool /* deep */ = true)
				throw()
			:	bound_(traits.bound_),
				position_(traits.position_)
			{
			}
				
			const DataIteratorTraits_& operator = (const DataIteratorTraits_ &traits)
				throw()
			{
				bound_ = traits.bound_;
				position_ = traits.position_;
				return *this;
			}

			HashGridBox3* getContainer()
				throw()
			{
				return bound_;
			}

			const HashGridBox3* getContainer() const
				throw()
			{
				return bound_;
			}

			bool isSingular() const
				throw()
			{
				return (bound_ == 0);
			}
				
			DataIteratorPosition& getPosition()
				throw()
			{
				return position_;
			}

			const DataIteratorPosition& getPosition() const
				throw()
			{
				return position_;
			}

			bool operator == (const DataIteratorTraits_ &traits) const
				throw()
			{
				return (position_ == traits.position_);
			}

			bool operator != (const DataIteratorTraits_ &traits) const
				throw()
			{
				return (position_ != traits.position_);
			}

			bool isValid() const
				throw()
			{
				return (bound_ != 0 && position_ != 0);
			}

			void invalidate()
				throw()
			{
				bound_ = 0;
				position_ = 0;
			}

			void toBegin()
				throw()
			{
				position_ = bound_->first_item_;
			}

			bool isBegin() const
				throw()
			{
				return (position_ == bound_->first_item_);
			}

			void toEnd()
				throw()
			{
				position_ = 0;
			}

			bool isEnd() const
				throw()
			{
				return (position_ == 0);
			}

			Item& getData()
				throw()
			{
				return position_->item_;
			}

			const Item& getData() const
				throw()
			{
				return position_->item_;
			}

			void forward()
				throw()
			{
				position_ = position_->next_;
			}
	
			private:

			HashGridBox3<Item>* 	bound_;
			DataIteratorPosition 	position_;
		};

		friend class DataIteratorTraits_;

		/** Data iterator for grid boxes.
				This iterator traverses the list of data items store in a \Ref{HashGridBox3}.
		*/
		typedef ForwardIterator
			<HashGridBox3<Item>, Item,
			 DataIteratorPosition, DataIteratorTraits_>
			DataIterator;

		///
		DataIterator beginData()
			throw()
		{
			return DataIterator::begin(*this);
		}

		///
		DataIterator endData()
			throw()
		{
			return DataIterator::end(*this);
		}


		/** Const data iterator for grid boxes.
				This is the const version of \Ref{DataIterator}
		*/
		typedef ConstForwardIterator
			<HashGridBox3<Item>, Item,
			 DataIteratorPosition, DataIteratorTraits_>
			ConstDataIterator;

		///
		ConstDataIterator beginData() const
			throw()
		{
			return ConstDataIterator::begin(*this);
		}

		///
		ConstDataIterator endData() const
			throw()
		{
			return ConstDataIterator::end(*this);
		}

		//@}	

		//_
		HashGridBox3 *previous_;
		//_
		HashGridBox3 *next_;

		//  private:
	
		void insert_(HashGridBox3 *box)
			throw();

		bool remove_(HashGridBox3 *box)
			throw();

		NeighbourBoxItem_* 	first_neighbour_;
		DataItem_* 					first_item_;
	};

	template<class Item>  
	HashGridBox3<Item>::HashGridBox3()
		throw()
		:	previous_(0),
			next_(0),
			first_neighbour_(0),
			first_item_(0)
	{
	}

	template<class Item>  
	HashGridBox3<Item>::HashGridBox3(const HashGridBox3<Item>& box, bool deep)
		throw()
		:	previous_(0),
			next_(0),
			first_neighbour_(0),
			first_item_(0)
	{
		set(box, deep);
	}

	template<class Item>  
	HashGridBox3<Item>::~HashGridBox3()
		throw()
	{
		clear();
	}

	template<class Item>  
	void HashGridBox3<Item>::clear()
		throw()
	{
		for (NeighbourBoxItem_* next = 0; first_neighbour_ != 0; first_neighbour_ = next)
		{
			next = first_neighbour_->next_;
			delete first_neighbour_;
		}

		for (DataItem_* next_item = 0; first_item_ != 0; first_item_ = next_item)
		{
			next_item = first_item_->next_;
			delete first_item_;
		}
	}

	template<class Item>  
	BALL_INLINE 
	void HashGridBox3<Item>::destroy()
		throw()
	{
		clear();
	}

	template<class Item>  
	void HashGridBox3<Item>::set(const HashGridBox3<Item>& box,  bool deep)
		throw()
  { // BAUSTELLE - not implemented
    throw Exception::NotImplemented(__FILE__, __LINE__);
	}
 
	template<class Item>  
	BALL_INLINE 
	const HashGridBox3<Item>& HashGridBox3<Item>::operator = (const HashGridBox3<Item>& box)
		throw()
	{
		set(box);
		
		return *this;
	}

	template<class Item>  
	BALL_INLINE 
	void HashGridBox3<Item>::get(HashGridBox3<Item>& box, bool deep) const
		throw()
	{
		box.set(*this, deep);
	}

	template<class Item>  
	Item* HashGridBox3<Item>::find(const Item& item)
		throw()
	{
		for (DataItem_* item_ = first_item_; item != 0; item = item->next_)
		{
			if (item->item_ == item)
			{
				return &(item->item_);
			}
		}

		return 0;
	}

	template<class Item>  
	BALL_INLINE 
	const Item* HashGridBox3<Item>::find(const Item& item) const
		throw()
	{
		return const_cast<HashGridBox3*>(this)->find(item);
	}

	template<class Item>  
	Size HashGridBox3<Item>::getSize() const
		throw()
	{
		Size size = 0;

		// count all items in the box
		for (const DataItem_* item = first_item_; item != 0; item = item->next_, size++);
		
		return size;
	}

	template<class Item>  
	BALL_INLINE 
	void HashGridBox3<Item>::insert(const Item& item)
		throw()
	{
		first_item_ = new DataItem_(item, first_item_);
	}

	template<class Item>  
	bool HashGridBox3<Item>::remove(const Item& item)
		throw()
	{
		for (DataItem_* item_ptr = first_item_; item_ptr != 0; item_ptr = item->next_)
		{
			if (item_ptr->item_ == item)
			{
				if (item_ptr == first_item_)
				{
					first_item_ = first_item_->next_;
				}
			
				if (item_ptr->next_ != 0)
				{
					item_ptr->next_->previous_ = item_ptr->previous_;
				}
			
				if (item_ptr->previous_ != 0)
				{
					item_ptr->previous_->next_ = item_ptr->next_;
				}
			
				delete item_ptr;
				
				return true;
			}
		}
		
		return false;
	}

	template<class Item>  
	bool HashGridBox3<Item>::removeAll(const Item& item)
		throw()
	{
		bool found = false;
		DataItem_* next_item = 0;
		DataItem_* item_ptr = first_item_;
		
		while(item_ptr != 0)
		{
			next_item = item->next_;

			if (item_ptr->item_ == item)
			{
				if (item_ptr == first_item_)
				{
					first_item_ = first_item_->next_;
				}
				
				if (item_ptr->next_ != 0)
				{
					item_ptr->next_->previous_ = item_ptr->previous_;
				}
			
				if (item_ptr->previous_ != 0)
				{
					item_ptr->previous_->next_ = item_ptr->next_;
				}
			
				delete item_ptr;
				
				found = true;
			}
			
			item_ptr = next_item;
		}

		return found;
	}

	template <class Item>
	BALL_INLINE 
	void HashGridBox3<Item>::host(Visitor< HashGridBox3<Item> >& visitor)
		throw()
	{
		visitor.visit(*this);
	}

	template<class Item>  
	bool HashGridBox3<Item>::operator == (const HashGridBox3<Item>& box) const
		throw()
	{
		const DataItem_* a = first_item_;
		const DataItem_* b = box.first_item_;
		
		for (; a != 0 && b != 0; a = a->next_, b = b->next_)
		{
			if (a->item_ != b->item_)
			{
				return false;
			}
		}
		
		return (a == b);
	}

	template<class Item>  
	BALL_INLINE 
	bool HashGridBox3<Item>::operator != (const HashGridBox3<Item>& box) const
		throw()
	{
		return !(*this == box);
	}

	template<class Item>  
	BALL_INLINE 
	bool HashGridBox3<Item>::has(const Item& item) const
		throw()
	{
		return (find(item) != 0);
	}

	template<class Item>  
	BALL_INLINE 
	bool HashGridBox3<Item>::isEmpty() const
		throw()
	{
		return (first_item_ == 0);
	}

	template<class Item>  
	bool HashGridBox3<Item>::isValid() const
		throw()
	{
		Size size = 0;
		NeighbourBoxItem_* item_ptr_ = 0;
		
		for (item_ptr_ = first_neighbour_; item_ptr_ != 0; item_ptr_ = item_ptr_->next_)
		{
			++size;

			if (item_ptr_->next_ == 0)
			{
				break;
			}
		}
		
		for (; item_ptr_ != 0; item_ptr_ = item_ptr_->previous_, --size);
		
		if (size != 0)
		{
			return false;
		}
		
		size = 0;
		DataItem_* item = 0;
		
		for (item = first_item_; item != 0; item = item->next_)
		{
			++size;
			
			if (item->next_ == 0)
			{
				break;
			}
		}
		
		for (; item != 0; item = item->previous_, --size);
		
		return (size == 0);
	}

	template<class Item>  
	void HashGridBox3<Item>::dump(std::ostream& s, Size depth) const
		throw()
	{
		BALL_DUMP_STREAM_PREFIX(s);
		
		BALL_DUMP_DEPTH(s, depth);
		
		BALL_DUMP_DEPTH(s, depth);
		s << "  size: " << getSize() << endl;
		
		BALL_DUMP_DEPTH(s, depth);
		s << "  data:" << endl;
		for (DataItem_ *item = first_item_; item != 0; item = item->next_)
		{
			BALL_DUMP_DEPTH(s, depth);
			s << "    " << item->item_ << endl;
		}
		
		BALL_DUMP_DEPTH(s, depth);
		s << "  neighbour boxes:" << endl;
		for (NeighbourBoxItem_ *item_ptr_ = first_neighbour_; item_ptr_ != 0; item_ptr_ = item_ptr_->next_)
		{
			BALL_DUMP_DEPTH(s, depth);
			s << "    " << item_ptr_->box_ << endl;
		}
		
		BALL_DUMP_STREAM_SUFFIX(s);
	}

	template <class Item>
	bool HashGridBox3<Item>::apply(UnaryProcessor<Item>& processor)
		throw()
	{
		if (processor.start() == false)
		{
			return false;
		}

		Processor::Result result;
			
		for (DataItem_ *item = first_item_; item != 0; item = item->next_)
		{
			result = processor(item->item_);

			if (result <= Processor::BREAK)
			{
				return (result == Processor::BREAK) ? true : false;
			}
		}

		return preocessor->finish(); 
	}

	template <class Item>
	bool HashGridBox3<Item>::apply(UnaryProcessor< HashGridBox3<Item> >& processor)
		throw()
	{
    if (processor.start() == false)
		{
      return false;
		}
 
		Processor::Result result;

		for (NeighbourBoxItem_* neighbour_item = first_neighbour_;
				 neighbour_item != 0; neighbour_item = neighbour_item->next_)
		{
			result = processor(*(neighbour_item->box_));

			if (result <= Processor::BREAK)
			{
				return (result == Processor::BREAK) ? true : false;
			}
		}

		return processor->finish(); 
	}

	template<class Item>  
	BALL_INLINE 
	void HashGridBox3<Item>::insert_(HashGridBox3* box)
		throw()
	{
		first_neighbour_ = new NeighbourBoxItem_(box, first_neighbour_);
	}

	template<class Item>  
	bool HashGridBox3<Item>::remove_(HashGridBox3* box)
		throw()
	{
		for (NeighbourBoxItem_* neighbour_item = first_neighbour_; 
				 neighbour_item != 0; neighbour_item = neighbour_item->next_)
		{
			if (neighbour_item->box_ == box)
			{
				if (neighbour_item == first_neighbour_)
				{
					first_neighbour_ = first_neighbour_->next_;
				}
				
				if (neighbour_item->next_ != 0)
				{
					neighbour_item->next_->previous_ = neighbour_item->previous_;
				}
			
				if (neighbour_item->previous_ != 0)
				{
					neighbour_item->previous_->next_ = neighbour_item->next_;
				}
			
				delete neighbour_item;
				
				return true;
			}
		}
		
		return false;
	}


	/**	Three-dimensional Hash Grid Class.\\
			BAUSTELLE\\
			{\bf Definition:} \URL{BALL/DATATYPE/hashGrid.h}
	*/
	template <class Item>
	class HashGrid3
	{
		public:

		BALL_CREATE(HashGrid3)

		/**	@name	Constructors and destructor 
		*/
		//@{

		/// Default constructor
		HashGrid3()
			throw();
			
		/** Constructor using origin, dimensions, and spacings of the grid.
				It creates a hashgrid at {\tt origin} with axis-dependant spacings. 
				@param origin
				@param dimension_x
				@param dimension_y
				@param dimension_z
				@param spacing_x
				@param spacing_y
				@param spacing_z
		*/
		HashGrid3(const Vector3& origin, Size dimension_x, Size dimension_y,
				Size dimension_z, float spacing_x, float spacing_y, float spacing_z)
			throw();

		/** Constructor using origin, dimensions, and a single spacing (only
				cubic grids)
		*/ 
		HashGrid3(const Vector3& origin, Size dimension_x, Size dimension_y, 
				Size dimension_z, float spacing)
			throw();

		/** Constructor using two vectors and a single spacing.
				This constructor creates a hash grid at {\tt origin} with spacing
				{\tt spacing}. The vector {\tt size} has to be relative to {\tt origin}
				and defines the opposite corner of the grid, thereby setting the size 
				of the grid.
				@param origin a vector defining the origin of our cubic hash grid
				@param size a vector defining the opposite corner of the cubic grid
				@param spacing this float will be used as spacing in all three dimensions
		*/
		HashGrid3(const Vector3& origin, const Vector3& size, float spacing)
			throw();

		/// Copy constructor
		HashGrid3(const HashGrid3& grid, bool deep = true)
			throw();

		/// Destructor
		virtual ~HashGrid3()
			throw();

		/// Clears the whole grid
		virtual void clear()
			throw();

		/// Clears the HashGridBox3 at position (x, y, z)
		void clear(Position x, Position y, Position z)
			throw();

		/// Clears the HashGridBox3 at position {\tt vector}
		void clear(const Vector3 &vector)
			throw();

		/// Destroys the grid (obsolete, only calls clear())
		void destroy()
			throw();

		/// Destroys a box of the grid (obsolete, only calls clear())
		void destroy(Position x, Position y, Position z)
			throw();

		/// Destroys a box of the grid (obsolete, only calls clear())
		void destroy(const Vector3& vector)
			throw();

		//@}
		/**	@name	Assignment 
		*/
		//@{

		/// assigns the content of a hash grid (obsolete)
		void set(const Vector3& origin, const Vector3& unit, 
				Size	dimension_x, Size	dimension_y, Size	dimension_z)
			throw();

		/// assigns the content of a hash grid (obsolete)
		void set(const Vector3& origin, float unit, Size size)
			throw();

		/// assigns the content of a hash grid (obsolete)
		void set(const HashGrid3& grid, bool deep = true)
			throw();

		/// Assignment operator
		const HashGrid3& operator = (const HashGrid3& grid)
			throw();

		///
		void get(Vector3& origin, Vector3& unit, Size&	dimension_x, Size&	dimension_y, Size&	dimension_z) const
			throw(); 

		///
		void get(HashGrid3& grid, bool deep = true) const
			throw();

		//@}
		/**	@name	Accessors 
		*/
		//@{

		/// Counts the non-empty boxes of a grid.
		Size countNonEmptyBoxes() const
			throw();
		
		/// Returns the size of a grid, i. e. BAUSTELLE
		Size getSize() const
			throw();

		/// Returns the origin of the grid
		Vector3& getOrigin()
			throw();

		/// Returns a const reference of the grid origin
		const Vector3& getOrigin() const
			throw();

		/// Returns the unit of the grid
		Vector3& getUnit()
			throw();

		/// Returns the unit of the grid (const version)
		const Vector3& getUnit() const
			throw();

		/// Get the x dimension of the grid
		Size getSizeX() const
			throw();

		/// Get the y dimension of the grid
		Size getSizeY() const
			throw();

		/// Get the z dimension of the grid
		Size getSizeZ() const
			throw();

		/// Return the HashGridBox3 at position (x, y, z)
		HashGridBox3<Item>* getBox(Position x, Position y, Position z)
			throw();

		/// Return the HashGridBox3 at position (x, y, z) (const version)
		const HashGridBox3<Item>* getBox(Position x, Position y, Position z) const
			throw();

		/// Return the HashGridBox3 at position vector 
		HashGridBox3<Item>* getBox(const Vector3& vector)
			throw();

		/// Return the HashGridBox3 at position vector (const version)
		const HashGridBox3<Item>* getBox(const Vector3 &vector) const
			throw();

		/// Get the position indices of a HashGridBox3
		bool getIndices(const HashGridBox3<Item>& box, 
				Position& x, Position& y, Position& z) const
			throw();

		/// Insert an item at position (x, y, z)
		void insert(Position x, Position y, Position z, const Item& item)
			throw();
		
		/// Insert an item at position {\tt vector}
		void insert(const Vector3& vector, const Item& item)
			throw();

		/// Remove an item from position (x, y ,z)
		bool remove(Position x, Position y, Position z, const Item& item)
			throw();

		/// Remove an item from position {\tt vector}
		bool remove(const Vector3& vector, const Item& item)
			throw();

		//@}
		/**	@name Miscellaneous 
		*/
		//@{

		///
		void host(Visitor<HashGrid3>& visitor)
			throw();

		//@}
		/**	@name	Predicates 
		*/
		//@{

		/// Equality operator
		bool operator == (const HashGrid3& grid) const
			throw();

		/// Inequality operator
		bool operator != (const HashGrid3& grid) const
			throw();

		/// Tests, whether {\tt this} is empty
		bool isEmpty() const
			throw();

		//@}
		/**	@name	Debugging and Diagnostics 
		*/
		//@{

		/// Validity check
		virtual bool isValid() const
			throw();

		/// Dump the contents of a HashGrid3 to a stream
		virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
			throw();

		//@}
		/**	@name Internal Iterators 
		*/
		//@{

		///
		bool apply(UnaryProcessor<Item> &processor)
			throw();

		///
		bool apply(UnaryProcessor< HashGridBox3<Item> > &processor)
			throw();

		//@}
		/**	@name	External Iterators 
		*/
		//@{

		typedef HashGridBox3<Item>* BoxIteratorPosition;
		
		class BoxIteratorTraits_
		{
			public:

			BALL_CREATE_DEEP(BoxIteratorTraits_)

			BoxIteratorTraits_()
				throw()
				:	bound_(0),
					position_(0)
			{
			}
				
			BoxIteratorTraits_(const HashGrid3 &grid)
				throw()
				:	bound_((HashGrid3 *)&grid),
					position_(0)
			{
			}
				
			BoxIteratorTraits_(const BoxIteratorTraits_& traits, bool /* deep */ = true)
				throw()
				:	bound_(traits.bound_),
					position_(traits.position_)
			{
			}
				
			const BoxIteratorTraits_& operator = (const BoxIteratorTraits_& traits)
				throw()
			{
				bound_ = traits.bound_;
				position_ = traits.position_;
				return *this;
			}

			HashGrid3* getContainer()
				throw()
			{
				return bound_;
			}

			const HashGrid3* getContainer() const
				throw()
			{
				return bound_;
			}

			bool isSingular() const
				throw()
			{
				return (bound_ == 0);
			}
				
			BoxIteratorPosition& getPosition()
				throw()
			{
				return position_;
			}

			const BoxIteratorPosition& getPosition() const
				throw()
			{
				return position_;
			}

			bool operator == (const BoxIteratorTraits_& traits) const
				throw()
			{
				return (position_ == traits.position_);
			}

			bool operator != (const BoxIteratorTraits_& traits) const
				throw()
			{
				return (position_ != traits.position_);
			}
				
			bool isValid() const
				throw()
			{
				return (bound_ != 0 && position_ != 0);
			}

			void invalidate()
				throw()
			{
				bound_ = 0;
				position_ = 0;
			}

			void toBegin()
				throw()
			{
				position_ = bound_->first_nonempty_;
			}

			bool isBegin() const
				throw()
			{
				return (position_ == bound_->first_nonempty_);
			}

			void toEnd()
				throw()
			{
				position_ = 0;
			}

			bool isEnd() const
				throw()
			{
				return (position_ == 0);
			}

			HashGridBox3<Item>& getData()
				throw()
			{
				return *position_;
			}

			const HashGridBox3<Item>& getData() const
				throw()
			{
				return *position_;
			}

			void forward()
				throw()
			{
				position_ = position_->next_;
			}				
		
			private:

			HashGrid3<Item>* bound_;
			BoxIteratorPosition position_;
		};

		friend class BoxIteratorTraits_;

		///
		typedef ForwardIterator
			<HashGrid3<Item>, HashGridBox3<Item>,
			 BoxIteratorPosition, BoxIteratorTraits_>
			BoxIterator;

		/// 
		BoxIterator beginBox()
			throw()
		{
			return BoxIterator::begin(*this);
		}

		///
		BoxIterator endBox()
			throw()
		{
			return BoxIterator::end(*this);
		}


		///
		typedef ConstForwardIterator
			<HashGrid3<Item>, HashGridBox3<Item>,
			 BoxIteratorPosition, BoxIteratorTraits_>
			ConstBoxIterator;

		/// 
		ConstBoxIterator beginBox() const
			throw()
		{
			return ConstBoxIterator::begin(*this);
		}

		/// 
		ConstBoxIterator endBox() const
			throw()
		{
			return ConstBoxIterator::end(*this);
		}

		//@}

		private:

		//_
		Index getIndex_(const HashGridBox3<Item>& box) const
			throw();

		//_
		void insert_(HashGridBox3<Item>* box, const Item& item)
			throw();

		//_
		bool remove_(HashGridBox3<Item>* box, const Item& item)
			throw();

		//_
		HashGridBox3<Item>* box_;
		//_
		HashGridBox3<Item>* first_nonempty_;
		//_
		Vector3 origin_;
		//_
		Vector3 unit_;
		//_
		Size	dimension_x_;
		//_
		Size	dimension_y_;
		//_
		Size	dimension_z_;
	};

		
	//@}


	template <class Item>
	HashGrid3<Item>::HashGrid3()
		throw()
		:	box_(0),
			first_nonempty_(0),
			origin_(0,0,0),
			unit_(0,0,0),
			dimension_x_(0),
			dimension_y_(0),
			dimension_z_(0)
	{
	}

	template <class Item>
	HashGrid3<Item>::HashGrid3
		(const Vector3 &originvector,
		 Size dimension_x, Size dimension_y, Size dimension_z,
		 float spacing_x, float spacing_y, float spacing_z)
		throw()
		:	box_(0),
			first_nonempty_(0),
			origin_(originvector),
			unit_(spacing_x, spacing_y, spacing_z),
			dimension_x_(dimension_x),
			dimension_y_(dimension_y),
			dimension_z_(dimension_z)
	{
		box_ = new HashGridBox3<Item>[dimension_x * dimension_y * dimension_z];
	}

	template <class Item>
	HashGrid3<Item>::HashGrid3
		(const Vector3& origin,
		 Size dimension_x, Size dimension_y, Size dimension_z, float spacing)
		throw()
	 :	box_(0),
			first_nonempty_(0),
			origin_(origin),
			unit_(spacing, spacing, spacing),
			dimension_x_(dimension_x),
			dimension_y_(dimension_y),
			dimension_z_(dimension_z)
	{
		box_ = new HashGridBox3<Item>[dimension_x * dimension_y * dimension_z];
	}

	// this constructor creates a linear array of HashGridBox3 objects.
	template <class Item>
	HashGrid3<Item>::HashGrid3(const Vector3& origin, const Vector3& size,
			float spacing)
		throw()
		:	box_(0),
			first_nonempty_(0),
			origin_(origin),
			unit_(spacing, spacing, spacing),
			dimension_x_((Size)(size.x / spacing + 1.0)),
			dimension_y_((Size)(size.y / spacing + 1.0)),
			dimension_z_((Size)(size.z / spacing + 1.0))
	{
		box_ = new HashGridBox3<Item>[dimension_x_ * dimension_y_ * dimension_z_];
	}

	template <class Item>
	HashGrid3<Item>::HashGrid3(const HashGrid3<Item>& grid, bool deep)
		throw()
		:	box_(0),
			first_nonempty_(0),
			origin_(),
			unit_()
	{
		set(grid, deep);
	}

	template <class Item>
	HashGrid3<Item>::~HashGrid3()
		throw()
	{
		clear();
		delete [] box_;
	}

	template <class Item>
	void HashGrid3<Item>::clear()
		throw()
	{
		if (box_ != 0)
		{
			for(HashGridBox3<Item>* nextbox = 0; first_nonempty_ != 0; first_nonempty_ = nextbox)
			{
				nextbox = first_nonempty_->next_;
				
				first_nonempty_->previous_ = first_nonempty_->next_ = 0;
			}

			Size size = dimension_x_ * dimension_y_ * dimension_z_;

			for (Position index = 0; index < (Position)size; ++index)
			{
				box_[index].clear();
			}
		}
	}

	template <class Item>
	BALL_INLINE 
	void HashGrid3<Item>::clear(Position x, Position y, Position z)
		throw()
	{
		HashGridBox3<Item>* box = getBox(x, y, z);
		
		if (box != 0)
		{
			box->clear();
		
			if (box->previous_ != 0)
			{
				box->previous_->next_ = box->next_;
			} 
			else 
			{
				first_nonempty_ = box->next_;
			}
			
			if (box->next_ != 0)
			{
				box->next_->previous_ = box->previous_;
			}

			box->previous_ = box->next_ = 0;
		}
	}

	template <class Item>
	BALL_INLINE 
	void HashGrid3<Item>::clear(const Vector3& vector)
		throw()
	{
		HashGridBox3<Item>* box = getBox(vector);
		
		if (box != 0)
		{
			box->clear();
		
			if (box->previous_ != 0)
			{
				box->previous_->next_ = box->next_;
			} 
			else 
			{
				first_nonempty_ = box->next_;
			}
			
			if (box->next_ != 0)
			{
				box->next_->previous_ = box->previous_;
			}

			box->previous_ = box->next_ = 0;
		}
	}

	template <class Item>
	BALL_INLINE 
	void HashGrid3<Item>::destroy()
		throw()
	{
		clear();
	}

	template <class Item>
	BALL_INLINE 
	void HashGrid3<Item>::destroy(Position x, Position y, Position z)
		throw()
	{
		clear(x, y, z);
	}

	template <class Item>
	BALL_INLINE 
	void HashGrid3<Item>::destroy(const Vector3 &vector)
		throw()
	{
		clear(vector);
	}

	template <class Item>
	void HashGrid3<Item>::set
		(const Vector3& origin, const Vector3& unit,
		 Size dimension_x, Size dimension_y, Size dimension_z)
		 	throw()
	{
		clear();
		if (box_ != 0)
		{
			delete [] box_;
		}
		origin_.set(origin);
		unit_.set(unit);
		dimension_x_ = dimension_x;
		dimension_y_ = dimension_y;
		dimension_z_ = dimension_z;
		box_ = new HashGridBox3<Item>[getSize()];
	}

	template <class Item>
	void HashGrid3<Item>::set(const Vector3& origin, float unit, Size size)
		throw()
	{
		clear();
		if (box_ != 0)
		{
			delete [] box_;
		}
		origin_.set(origin);
		unit_.set(unit, unit, unit);
		dimension_x_ = size;
		dimension_y_ = size;
		dimension_z_ = size;
		box_ = new HashGridBox3<Item>[getSize()];
	}

	template <class Item>
	void HashGrid3<Item>::set(const HashGrid3<Item>& grid, bool /* deep */)
		throw()
	{
		set(grid.origin_, grid.unit_, grid.dimension_x_, grid.dimension_y_, grid.dimension_z_);

		const HashGridBox3<Item>* sourcebox = grid.box_;
		
		HashGridBox3<Item>* targetbox = box_;
		
		const HashGridBox3<Item>* endbox = &grid.box_[grid.getSize()];
		
		for (; sourcebox < endbox; ++sourcebox, ++targetbox)
		{
			if (sourcebox->isEmpty() == false)
			{
				for (HashGridBox3<Item>::DataItem_* item  = sourcebox->first_item_; item != 0; item = item->next_)
				{
					insert_(targetbox, item->item_);
				}
			}
		}
	}

	template <class Item>
	BALL_INLINE 
	const HashGrid3<Item>& HashGrid3<Item>::operator = (const HashGrid3<Item> &grid)
		throw()
	{
		set(grid);
		
		return *this;
	}

	template <class Item>
	BALL_INLINE 
	void HashGrid3<Item>::get(Vector3 &origin, Vector3 &unit,
														Size& dimension_x, Size& dimension_y, Size& dimension_z) const
		throw()
	{
		origin.set(origin_);
		unit.set(unit_);
		dimension_x = dimension_x_;
		dimension_y = dimension_y_;
		dimension_z = dimension_z_;
	}

	template <class Item>
	BALL_INLINE void 
	HashGrid3<Item>::get(HashGrid3<Item> &grid, bool deep) const
		throw()
	{
		grid.set(*this, deep);
	}

	template <class Item>
	Size 
	HashGrid3<Item>::countNonEmptyBoxes() const
		throw()
	{
		Size size = 0;

		for(HashGridBox3<Item>* box	= first_nonempty_; box != 0; box = box->next_)
		{
			++size;
		}

		return size;
	}

	template <class Item>
	BALL_INLINE 
	Size HashGrid3<Item>::getSize() const
		throw()
	{
		return (dimension_x_ * dimension_y_ * dimension_z_);
	}

	template <class Item>
	BALL_INLINE 
	Vector3& HashGrid3<Item>::getOrigin()
		throw()
	{
		return origin_;
	}

	template <class Item>
	BALL_INLINE 
	const Vector3& HashGrid3<Item>::getOrigin() const
		throw()
	{
		return origin_;
	}

	template <class Item>
	BALL_INLINE 
	Vector3& HashGrid3<Item>::getUnit()
		throw()
	{
		return unit_;
	}

	template <class Item>
	BALL_INLINE 
	const Vector3& HashGrid3<Item>::getUnit() const
		throw()
	{
		return unit_;
	}

	template <class Item>
	BALL_INLINE 
	Size HashGrid3<Item>::getSizeX() const
		throw()
	{
		return dimension_x_;
	}

	template <class Item>
	BALL_INLINE 
	Size HashGrid3<Item>::getSizeY() const
		throw()
	{
		return dimension_y_;
	}

	template <class Item>
	BALL_INLINE 
	Size HashGrid3<Item>::getSizeZ() const
		throw()
	{
		return dimension_z_;
	}

	template <class Item>
	BALL_INLINE
	HashGridBox3<Item>* HashGrid3<Item>::getBox(Position x, Position y, Position z)
		throw()
	{
		if (x >= (Position)dimension_x_ ||
			  y >= (Position)dimension_y_ ||
			  z >= (Position)dimension_z_)
		{
			return 0;
		} 
		else 
		{
			return &(box_[x * dimension_y_ * dimension_z_ + y * dimension_z_ + z]);
		}
	}

	template <class Item>
	BALL_INLINE 
	const HashGridBox3<Item>* HashGrid3<Item>::getBox(Position x, Position y, Position z) const
		throw()
	{
		return ((HashGrid3*)this)->getBox(x, y, z);
	}

	template <class Item>
	BALL_INLINE 
	HashGridBox3<Item>* HashGrid3<Item>::getBox(const Vector3& vector)
		throw()
	{
		float x = (vector.x - origin_.x) / unit_.x;
		float y = (vector.y - origin_.y) / unit_.y;
		float z = (vector.z - origin_.z) / unit_.z;

		return getBox((Index)Maths::floor(x), (Index)Maths::floor(y), (Index)Maths::floor(z));
	}

	template <class Item>
	BALL_INLINE 
	const HashGridBox3<Item>* HashGrid3<Item>::getBox(const Vector3& vector) const
		throw()
	{
		return ((HashGrid3 *)this)->getBox(vector);
	}

	template <class Item>
	bool HashGrid3<Item>::getIndices(const HashGridBox3<Item>& box,
																	 Position& x, Position& y, Position& z) const
		throw()
	{
		Index index = getIndex_(box);
		
		if (index == INVALID_INDEX)
		{
			x = y = z = INVALID_POSITION;
			
			return false;
		}
		
		x = index / (dimension_y_ * dimension_z_);
		index  -= x * dimension_y_ * dimension_z_;
		y = index / dimension_z_;
		z = index - y * dimension_z_;
		
		return true;
	}

	template <class Item>
	BALL_INLINE 
	void HashGrid3<Item>::insert(Position x, Position y, Position z,
			const Item& item)
		throw()
	{
		HashGridBox3<Item>* box = getBox(x, y, z);
		
		if (box != 0)
		{
			insert_(box, item);
		}
	}

	template <class Item>
	BALL_INLINE 
	void HashGrid3<Item>::insert(const Vector3& vector, const Item& item)
		throw()
	{
		HashGridBox3<Item> *box = getBox(vector);
		
		if (box != 0)
		{
			insert_(box, item);
		}
	}

	template <class Item>
	BALL_INLINE 
	bool HashGrid3<Item>::remove(Position x, Position y, Position z, const Item& item)
		throw()
	{
		HashGridBox3<Item>* box = getBox(x, y, z);
		
		if (box != 0)
		{
			return remove_(box, item);
		}
		
		return false;
	}

	template <class Item>
	BALL_INLINE 
	bool HashGrid3<Item>::remove(const Vector3& vector, const Item& item)
		throw()
	{
		HashGridBox3<Item>* box = getBox(vector);
		
		if (box != 0)
		{
			return remove_(box, item);
		}

		return false;
	}

	template <class Item>
	BALL_INLINE 
	void HashGrid3<Item>::host(Visitor< HashGrid3<Item> >& visitor)
		throw()
	{
		visitor.visit(*this);
	}

	template <class Item>
	bool HashGrid3<Item>::operator ==	(const HashGrid3<Item>& grid) const
		throw()
	{
		if (getSize() != grid.getSize()
				|| origin_ != grid.origin_
				|| unit_ != grid.unit_
				|| dimension_x_ != grid.dimension_x_
				|| dimension_y_ != grid.dimension_y_
				|| dimension_z_ != grid.dimension_z_)
		{
			return false;
		}
		
		const HashGridBox3<Item>* abox = box_;
		
		const HashGridBox3<Item>* bbox = grid.box_;
		
		const HashGridBox3<Item>* endbox = &box_[getSize()];
		
		while (abox < endbox)
		{
			if (*abox++ != *bbox++)
			{
				return false;
			}
		}
		
		return true;
	}

	template <class Item>
	BALL_INLINE 
	bool HashGrid3<Item>::operator !=	(const HashGrid3<Item>& grid) const
		throw()
	{
		return !(*this == grid);
	}

	template <class Item>
	BALL_INLINE 
	bool HashGrid3<Item>::isEmpty() const
		throw()
	{
		return (getSize() == 0);
	}

	template <class Item>
	bool HashGrid3<Item>::isValid() const
		throw()
	{
		Size size = getSize();
		
		for (Position index = 0; index < (Position)size; ++index)
		{
			if (box_[index].isValid() == false)
			{
				return false;
			}
		}
		
		const HashGridBox3<Item>* box = box_;
		Size nonempty_boxes = 0;
		
		for (; box < &box_[size]; ++box)
		{
			if (box->isEmpty() == false)
			{
				++nonempty_boxes;
			}
		}

		if (nonempty_boxes != countNonEmptyBoxes())
		{
			return false;
		}
		
		for (box = first_nonempty_; box != 0; box = box->next_)
		{
			if (box->next_ == 0)
			{
				break;
			}
		}
		
		for (; box != 0; box = box->previous_)
		{
			if (box->previous_ == 0)
			{
				break;
			}
		}
		
		return (box == first_nonempty_);
	}

	template <class Item>
	void HashGrid3<Item>::dump(std::ostream& s, Size depth) const
		throw()
	{
		BALL_DUMP_STREAM_PREFIX(s);
		
		BALL_DUMP_DEPTH(s, depth);
		
		BALL_DUMP_DEPTH(s, depth);
		s << "  origin: " << origin_ << endl;
		
		BALL_DUMP_DEPTH(s, depth);
		s << "  unit: " << unit_.z << endl;
		
		BALL_DUMP_DEPTH(s, depth);
		s << "  dimension: " << dimension_x_ << " " 
			<< dimension_y_ << " " 
			<< dimension_z_ << endl;
		
		Size size = getSize();
		BALL_DUMP_DEPTH(s, depth);
		s << "  size: " << size << endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "  non empty boxes: " << countNonEmptyBoxes() << endl;
		
		BALL_DUMP_DEPTH(s, depth);
		s << "  boxes:" << endl;
		Position x, y, z;
		for (Position index = 0; index < (Position)size; ++index)
		{
			BALL_DUMP_DEPTH(s, depth);
			getIndices(box_[index], x, y, z);
			s << "    " << index << ". box: (" 
			  << x << ',' << y << ',' << z << ')' << endl;
			box_[index].dump(s, 1);
		}
		
		BALL_DUMP_DEPTH(s, depth);
		s << "  non-empty boxes:" << endl;

		for (const HashGridBox3<Item>* box = first_nonempty_; box != 0; box = box->next_)
		{
			s << "    " << getIndex_(*box) << endl;
		}
		BALL_DUMP_STREAM_SUFFIX(s);
	}

	template <class Item>
	bool HashGrid3<Item>::apply(UnaryProcessor<Item>& processor)
		throw()
	{
		if (processor.start() == false)
		{
			return false;
		}
		Processor::Result result;

		for (HashGridBox3<Item>* box = first_nonempty_; box != 0; box = box->next_)
		{
			for (HashGridBox3<Item>::DataItem_ *item = box->first_item_; item != 0; item = item->next_)
			{
				result = processor(item->item_);

				if (result <= Processor::BREAK)
				{
					return (result == Processor::BREAK) ? true : false;
				}
			}
		}

		return processor->finish();
	}

	template <class Item>
	bool HashGrid3<Item>::apply(UnaryProcessor< HashGridBox3<Item> >& processor)
		throw()
	{
		if (processor.start() == false)
		{
			return false;
		}

		Processor::Result result;

		for (HashGridBox3<Item>* box = first_nonempty_; box != 0; box = box->next_)
		{
			result = processor(*box);

			if (result <= Processor::BREAK)
			{
				return (result == Processor::BREAK) ? true : false;
			}
		}

		return processor->finish();
	}

	template <class Item>
	Index HashGrid3<Item>::getIndex_(const HashGridBox3<Item>& box) const
		throw()
	{
		if (&box < box_ ||& box >= &box_[getSize()])
		{
			return INVALID_INDEX;
		} 
		else 
		{
			return (Index)(&box - box_);
		}
	}

	template <class Item>
	void  HashGrid3<Item>::insert_(HashGridBox3<Item>* box, const Item& item)
		throw()
	{
		if (box->isEmpty() == true)
		{
			box->previous_ = 0;
			box->next_ = first_nonempty_;
			if (first_nonempty_ != 0)
			{
				first_nonempty_->previous_ = box;
			}

			first_nonempty_ = box;
			
			Position end_x, end_y, end_z;
			getIndices(*box, end_x, end_y, end_z);
			++end_x;
			++end_y;
			++end_z;
			
			HashGridBox3<Item>* neighbourbox = 0;
			for (Position x = end_x - 2; x <= end_x; ++x)
			{
				for (Position y = end_y - 2; y <= end_y; ++y)
				{
					for (Position z = end_z - 2; z <= end_z; ++z)
					{
						neighbourbox = getBox(x, y, z);
			
						if (neighbourbox != 0)
							neighbourbox->insert_(box);
					}
				}
			}
		}
		
		box->insert(item);
	}

	template <class Item>
	bool HashGrid3<Item>::remove_(HashGridBox3<Item>* box, const Item& item)
		throw()
	{
		bool result = box->remove(item);
		
		if (result == true && box->isEmpty() == true)
		{
			if (box->previous_ != 0)
			{
				box->previous_->next_ = box->next_;
			}
			else
			{
				first_nonempty_ = box->next_;
			}
			
			if (box->next_ != 0)
			{
				box->next_->previous_ = box->previous_;
			}
			
			Position end_x, end_y, end_z;
			getIndices(*box, end_x, end_y, end_z);
			++end_x;
			++end_y;
			++end_z;
			
			HashGridBox3<Item> *neighbourbox = 0;
			for (Index x = end_x - 2; x <= end_x; ++x)
			{
				for (Index y = end_y - 2; y <= end_y; ++y)
				{
					for (Index z = end_z - 2; z <= end_z; ++z)
					{
						neighbourbox = getBox(x, y, z);
			
						if (neighbourbox != 0)
						{
							neighbourbox->remove_(box);
						}
					}
				}
			}
		}
		
		return result;
	}

} // namespace BALL

#endif // BALL_DATATYPE_HASHGRID_H
