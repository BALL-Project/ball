// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: hashGrid.h,v 1.34 2003/06/22 10:22:34 oliver Exp $
//

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

#ifndef BALL_CONCEPT_PROCESSOR_H
#	include <BALL/CONCEPT/processor.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

namespace BALL 
{
	/**	\defgroup GenericHash3D Three-dimensional Hash Grid
    	\ingroup  GenericHash
	*/
	//@{

	/**	Grid Box Class.
			These boxes represent the buckets of a threedimensional hash grid.
			Every such box contains a linear list of the objects that are
			contained in this box. This list is accessible through a
			DataIterator. \par

			\ingroup GenericHash3D
	*/
	template <typename Item>
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

		/// Destructor
		~HashGridBox3()
			throw();

		/// Clears the grid box
		void clear()
			throw();

		/** Clears the grid box.
				Same as clear.
		*/
		void destroy()
			throw();
	
		//@}
		/**	@name	Assignment 
		*/
		//@{

		///
		void set(const HashGridBox3& box,bool /* deep */ = true)
			throw(Exception::NotImplemented);

		///
		const HashGridBox3& operator = (const HashGridBox3& box)
			throw(Exception::NotImplemented);

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
				@return bool - <tt>true</tt>, if the item could be removed, <tt>false</tt> 
								otherwise.
		*/
		bool remove(const Item& item)
			throw();

		/** Remove all occurences of a certain item from the data item list.
				@param item the item to be removed
				@return bool - <tt>true</tt>, if the item could be removed, <tt>false</tt> 
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
				@return bool - <tt>true</tt> if instance has <tt>item</tt>, <tt>false</tt> otherwise.
		*/
		bool has(const Item& item) const
			throw();

		/** Test, whether this box is empty, i. e. the data item list contains nothing
				@return bool - <tt>true</tt>, if <tt>this</tt> is empty. <tt>false</tt> otherwise. 
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

		/// ?????
		class DataItem
		{
			public:
		
			DataItem(const Item& item, DataItem* next)
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

			Item 			item_;
			DataItem* previous_;
			DataItem* next_;
		};
			
		/// ?????
		class NeighborBoxItem
		{
			public:
		
			NeighborBoxItem(HashGridBox3* box, NeighborBoxItem* next)
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

			HashGridBox3<Item>* 	box_;
			NeighborBoxItem* 		previous_;
			NeighborBoxItem* 		next_;
		};


		typedef NeighborBoxItem* BoxIteratorPosition;
		
		class BoxIteratorTraits
		{
			public:

			BALL_CREATE_DEEP(BoxIteratorTraits)

			BoxIteratorTraits()
				throw()
				:	bound_(0),
					position_(0)
			{
			}
				
			BoxIteratorTraits(const HashGridBox3& box)
				throw()
				:	bound_((HashGridBox3 *)&box),
					position_(0)
			{
			}
				
			BoxIteratorTraits(const BoxIteratorTraits& traits, bool /* deep */ = true)
				throw()
				:	bound_(traits.bound_),
					position_(traits.position_)
			{
			}
				
			const BoxIteratorTraits& operator = (const BoxIteratorTraits& traits)
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

			bool operator == (const BoxIteratorTraits& traits) const
				throw()
			{
				return (position_ == traits.position_);
			}

			bool operator != (const BoxIteratorTraits& traits) const
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
				position_ = bound_->first_neighbor_;
			}

			bool isBegin() const
				throw()
			{
				return (position_ == bound_->first_neighbor_);
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

		friend class BoxIteratorTraits;

		/** BoxIterators iterate over all non-empty boxes that lie in the
				direct neighborhood to a box. Such an iterator traverses at most
				26 boxes.
		*/
		typedef ForwardIterator
			<HashGridBox3<Item>, HashGridBox3<Item>,
			BoxIteratorPosition, BoxIteratorTraits>
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


		/// This is the const version of  \link BoxIterator BoxIterator \endlink 
		typedef ConstForwardIterator
			<HashGridBox3<Item>, HashGridBox3<Item>,
			BoxIteratorPosition, BoxIteratorTraits>
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


		typedef DataItem* DataIteratorPosition;
		
		class DataIteratorTraits
		{
			public:

			BALL_CREATE_DEEP(DataIteratorTraits)

			DataIteratorTraits()
				throw()
			:	bound_(0),
				position_(0)
			{
			}
				
			DataIteratorTraits(const HashGridBox3& box)
				throw()
			:	bound_((HashGridBox3 *)&box),
				position_(0)
			{
			}
				
			DataIteratorTraits(const DataIteratorTraits& traits, bool /* deep */ = true)
				throw()
			:	bound_(traits.bound_),
				position_(traits.position_)
			{
			}
				
			const DataIteratorTraits& operator = (const DataIteratorTraits &traits)
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

			bool operator == (const DataIteratorTraits &traits) const
				throw()
			{
				return (position_ == traits.position_);
			}

			bool operator != (const DataIteratorTraits &traits) const
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

		friend class DataIteratorTraits;

		/** Data iterator for grid boxes.
				This iterator traverses the list of data items store in a  \link HashGridBox3 HashGridBox3 \endlink .
		*/
		typedef ForwardIterator
			<HashGridBox3<Item>, Item,
			 DataIteratorPosition, DataIteratorTraits>
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
				This is the const version of  \link DataIterator DataIterator \endlink 
		*/
		typedef ConstForwardIterator
			<HashGridBox3<Item>, Item,
			 DataIteratorPosition, DataIteratorTraits>
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

		NeighborBoxItem* 	first_neighbor_;
		DataItem* 					first_item_;
	};

	template<typename Item>  
	HashGridBox3<Item>::HashGridBox3()
		throw()
		:	previous_(0),
			next_(0),
			first_neighbor_(0),
			first_item_(0)
	{
	}

	template<typename Item>  
	HashGridBox3<Item>::HashGridBox3(const HashGridBox3<Item>& box, bool deep)
		throw()
		:	previous_(0),
			next_(0),
			first_neighbor_(0),
			first_item_(0)
	{
		set(box, deep);
	}

	template<typename Item>  
	HashGridBox3<Item>::~HashGridBox3()
		throw()
	{
		clear();
	}

	template<typename Item>  
	void HashGridBox3<Item>::clear()
		throw()
	{
		for (NeighborBoxItem* next = 0; first_neighbor_ != 0; first_neighbor_ = next)
		{
			next = first_neighbor_->next_;
			delete first_neighbor_;
		}

		for (DataItem* next_item = 0; first_item_ != 0; first_item_ = next_item)
		{
			next_item = first_item_->next_;
			delete first_item_;
		}
	}

	template<typename Item>  
	BALL_INLINE 
	void HashGridBox3<Item>::destroy()
		throw()
	{
		clear();
	}

	template<typename Item>  
	void HashGridBox3<Item>::set(const HashGridBox3<Item>& box,  bool deep)
		throw(Exception::NotImplemented)
  { 
		// ????? - not implemented
    throw Exception::NotImplemented(__FILE__, __LINE__);
	}
 
	template<typename Item>  
	BALL_INLINE 
	const HashGridBox3<Item>& HashGridBox3<Item>::operator = (const HashGridBox3<Item>& box)
		throw(Exception::NotImplemented)
	{
		set(box);
		
		return *this;
	}

	template<typename Item>  
	Item* HashGridBox3<Item>::find(const Item& item)
		throw()
	{
		for (DataItem* item_ptr= first_item_; item_ptr != 0; item_ptr = item_ptr->next_)
		{
			if (item_ptr->item_ == item)
			{
				return &(item_ptr->item_);
			}
		}

		return 0;
	}

	template<typename Item>  
	BALL_INLINE 
	const Item* HashGridBox3<Item>::find(const Item& item) const
		throw()
	{
		return const_cast<HashGridBox3*>(this)->find(item);
	}

	template<typename Item>  
	Size HashGridBox3<Item>::getSize() const
		throw()
	{
		Size size = 0;

		// count all items in the box
		for (const DataItem* item = first_item_; item != 0; item = item->next_, size++);
		
		return size;
	}

	template<typename Item>  
	BALL_INLINE 
	void HashGridBox3<Item>::insert(const Item& item)
		throw()
	{
		first_item_ = new DataItem(item, first_item_);
	}

	template<typename Item>  
	bool HashGridBox3<Item>::remove(const Item& item)
		throw()
	{
		for (DataItem* item_ptr = first_item_; item_ptr != 0; item_ptr = item_ptr->next_)
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

	template<typename Item>  
	bool HashGridBox3<Item>::removeAll(const Item& item)
		throw()
	{
		bool found = false;
		DataItem* next_item = 0;
		DataItem* item_ptr = first_item_;
		
		while(item_ptr != 0)
		{
			next_item = item_ptr->next_;

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

	template <typename Item>
	BALL_INLINE 
	void HashGridBox3<Item>::host(Visitor< HashGridBox3<Item> >& visitor)
		throw()
	{
		visitor.visit(*this);
	}

	template<typename Item>  
	bool HashGridBox3<Item>::operator == (const HashGridBox3<Item>& box) const
		throw()
	{
		const DataItem* a = first_item_;
		const DataItem* b = box.first_item_;
		
		for (; a != 0 && b != 0; a = a->next_, b = b->next_)
		{
			if (a->item_ != b->item_)
			{
				return false;
			}
		}
		
		return (a == b);
	}

	template<typename Item>  
	BALL_INLINE 
	bool HashGridBox3<Item>::operator != (const HashGridBox3<Item>& box) const
		throw()
	{
		return !(*this == box);
	}

	template<typename Item>  
	BALL_INLINE 
	bool HashGridBox3<Item>::has(const Item& item) const
		throw()
	{
		return (find(item) != 0);
	}

	template<typename Item>  
	BALL_INLINE 
	bool HashGridBox3<Item>::isEmpty() const
		throw()
	{
		return (first_item_ == 0);
	}

	template<typename Item>  
	bool HashGridBox3<Item>::isValid() const
		throw()
	{
		Size size = 0;
		NeighborBoxItem* item_ptr_ = 0;
		
		for (item_ptr_ = first_neighbor_; item_ptr_ != 0; item_ptr_ = item_ptr_->next_)
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
		DataItem* item = 0;
		
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

	template<typename Item>  
	void HashGridBox3<Item>::dump(std::ostream& s, Size depth) const
		throw()
	{
		BALL_DUMP_STREAM_PREFIX(s);
		
		BALL_DUMP_DEPTH(s, depth);
		
		BALL_DUMP_DEPTH(s, depth);
		s << "  size: " << getSize() << std::endl;
		
		BALL_DUMP_DEPTH(s, depth);
		s << "  data:" << std::endl;
		for (DataItem *item = first_item_; item != 0; item = item->next_)
		{
			BALL_DUMP_DEPTH(s, depth);
			s << "    " << item->item_ << std::endl;
		}
		
		BALL_DUMP_DEPTH(s, depth);
		s << "  neighbor boxes:" << std::endl;
		for (NeighborBoxItem *item_ptr_ = first_neighbor_; item_ptr_ != 0; item_ptr_ = item_ptr_->next_)
		{
			BALL_DUMP_DEPTH(s, depth);
			s << "    " << item_ptr_->box_ << std::endl;
		}
		
		BALL_DUMP_STREAM_SUFFIX(s);
	}

	template <typename Item>
	bool HashGridBox3<Item>::apply(UnaryProcessor<Item>& processor)
		throw()
	{
		if (processor.start() == false)
		{
			return false;
		}

		Processor::Result result;
			
		for (DataItem *item = first_item_; item != 0; item = item->next_)
		{
			result = processor(item->item_);

			if (result <= Processor::BREAK)
			{
				return (result == Processor::BREAK) ? true : false;
			}
		}

		return processor.finish(); 
	}

	template <typename Item>
	bool HashGridBox3<Item>::apply(UnaryProcessor< HashGridBox3<Item> >& processor)
		throw()
	{
    if (processor.start() == false)
		{
      return false;
		}
 
		Processor::Result result;

		for (NeighborBoxItem* neighbor_item = first_neighbor_;
				 neighbor_item != 0; neighbor_item = neighbor_item->next_)
		{
			result = processor(*(neighbor_item->box_));

			if (result <= Processor::BREAK)
			{
				return (result == Processor::BREAK) ? true : false;
			}
		}

		return processor.finish(); 
	}

	template<typename Item>  
	BALL_INLINE 
	void HashGridBox3<Item>::insert_(HashGridBox3* box)
		throw()
	{
		first_neighbor_ = new NeighborBoxItem(box, first_neighbor_);
	}

	template<typename Item>  
	bool HashGridBox3<Item>::remove_(HashGridBox3* box)
		throw()
	{
		for (NeighborBoxItem* neighbor_item = first_neighbor_; 
				 neighbor_item != 0; neighbor_item = neighbor_item->next_)
		{
			if (neighbor_item->box_ == box)
			{
				if (neighbor_item == first_neighbor_)
				{
					first_neighbor_ = first_neighbor_->next_;
				}
				
				if (neighbor_item->next_ != 0)
				{
					neighbor_item->next_->previous_ = neighbor_item->previous_;
				}
			
				if (neighbor_item->previous_ != 0)
				{
					neighbor_item->previous_->next_ = neighbor_item->next_;
				}
			
				delete neighbor_item;
				
				return true;
			}
		}
		
		return false;
	}


	/**	Three-dimensional Hash Grid Class.
			????? \par
			
	*/
	template <typename Item>
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
				It creates a hashgrid at <tt>origin</tt> with axis-dependant spacings. 
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
				This constructor creates a hash grid at <tt>origin</tt> with spacing
				<tt>spacing</tt>. The vector <tt>size</tt> has to be relative to <tt>origin</tt>
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

		/// Clears the HashGridBox3 at position <tt>vector</tt>
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
		
		/// Returns the size of a grid, i. e. ?????
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
		
		/// Insert an item at position <tt>vector</tt>
		void insert(const Vector3& vector, const Item& item)
			throw();

		/// Remove an item from position (x, y ,z)
		bool remove(Position x, Position y, Position z, const Item& item)
			throw();

		/// Remove an item from position <tt>vector</tt>
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

		/// Tests, whether <tt>this</tt> is empty
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
		
		class BoxIteratorTraits
		{
			public:

			BALL_CREATE_DEEP(BoxIteratorTraits)

			BoxIteratorTraits()
				throw()
				:	bound_(0),
					position_(0)
			{
			}
				
			BoxIteratorTraits(const HashGrid3 &grid)
				throw()
				:	bound_((HashGrid3 *)&grid),
					position_(0)
			{
			}
				
			BoxIteratorTraits(const BoxIteratorTraits& traits, bool /* deep */ = true)
				throw()
				:	bound_(traits.bound_),
					position_(traits.position_)
			{
			}
				
			const BoxIteratorTraits& operator = (const BoxIteratorTraits& traits)
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

			bool operator == (const BoxIteratorTraits& traits) const
				throw()
			{
				return (position_ == traits.position_);
			}

			bool operator != (const BoxIteratorTraits& traits) const
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

		friend class BoxIteratorTraits;

		///
		typedef ForwardIterator
			<HashGrid3<Item>, HashGridBox3<Item>,
			 BoxIteratorPosition, BoxIteratorTraits>
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
			 BoxIteratorPosition, BoxIteratorTraits>
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


	template <typename Item>
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

	template <typename Item>
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

	template <typename Item>
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
	template <typename Item>
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

	template <typename Item>
	HashGrid3<Item>::HashGrid3(const HashGrid3<Item>& grid, bool deep)
		throw()
		:	box_(0),
			first_nonempty_(0),
			origin_(),
			unit_()
	{
		set(grid, deep);
	}

	template <typename Item>
	HashGrid3<Item>::~HashGrid3()
		throw()
	{
		clear();
		delete [] box_;
	}

	template <typename Item>
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

	template <typename Item>
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

	template <typename Item>
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

	template <typename Item>
	BALL_INLINE 
	void HashGrid3<Item>::destroy()
		throw()
	{
		clear();
	}

	template <typename Item>
	BALL_INLINE 
	void HashGrid3<Item>::destroy(Position x, Position y, Position z)
		throw()
	{
		clear(x, y, z);
	}

	template <typename Item>
	BALL_INLINE 
	void HashGrid3<Item>::destroy(const Vector3 &vector)
		throw()
	{
		clear(vector);
	}

	template <typename Item>
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
		Size n = getSize();
		box_ = new HashGridBox3<Item> [n];
	}

	template <typename Item>
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

	template <typename Item>
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
				for (typename HashGridBox3<Item>::DataItem* item  = sourcebox->first_item_; item != 0; item = item->next_)
				{
					insert_(targetbox, item->item_);
				}
			}
		}
	}

	template <typename Item>
	BALL_INLINE 
	const HashGrid3<Item>& HashGrid3<Item>::operator = (const HashGrid3<Item> &grid)
		throw()
	{
		set(grid);
		
		return *this;
	}

	template <typename Item>
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

	template <typename Item>
	BALL_INLINE void 
	HashGrid3<Item>::get(HashGrid3<Item> &grid, bool deep) const
		throw()
	{
		grid.set(*this, deep);
	}

	template <typename Item>
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

	template <typename Item>
	BALL_INLINE 
	Size HashGrid3<Item>::getSize() const
		throw()
	{
		return (dimension_x_ * dimension_y_ * dimension_z_);
	}

	template <typename Item>
	BALL_INLINE 
	Vector3& HashGrid3<Item>::getOrigin()
		throw()
	{
		return origin_;
	}

	template <typename Item>
	BALL_INLINE 
	const Vector3& HashGrid3<Item>::getOrigin() const
		throw()
	{
		return origin_;
	}

	template <typename Item>
	BALL_INLINE 
	Vector3& HashGrid3<Item>::getUnit()
		throw()
	{
		return unit_;
	}

	template <typename Item>
	BALL_INLINE 
	const Vector3& HashGrid3<Item>::getUnit() const
		throw()
	{
		return unit_;
	}

	template <typename Item>
	BALL_INLINE 
	Size HashGrid3<Item>::getSizeX() const
		throw()
	{
		return dimension_x_;
	}

	template <typename Item>
	BALL_INLINE 
	Size HashGrid3<Item>::getSizeY() const
		throw()
	{
		return dimension_y_;
	}

	template <typename Item>
	BALL_INLINE 
	Size HashGrid3<Item>::getSizeZ() const
		throw()
	{
		return dimension_z_;
	}

	template <typename Item>
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

	template <typename Item>
	BALL_INLINE 
	const HashGridBox3<Item>* HashGrid3<Item>::getBox(Position x, Position y, Position z) const
		throw()
	{
		return ((HashGrid3*)this)->getBox(x, y, z);
	}

	template <typename Item>
	BALL_INLINE 
	HashGridBox3<Item>* HashGrid3<Item>::getBox(const Vector3& vector)
		throw()
	{
		float x = (vector.x - origin_.x) / unit_.x;
		float y = (vector.y - origin_.y) / unit_.y;
		float z = (vector.z - origin_.z) / unit_.z;

		return getBox((Index)Maths::floor(x), (Index)Maths::floor(y), (Index)Maths::floor(z));
	}

	template <typename Item>
	BALL_INLINE 
	const HashGridBox3<Item>* HashGrid3<Item>::getBox(const Vector3& vector) const
		throw()
	{
		return ((HashGrid3 *)this)->getBox(vector);
	}

	template <typename Item>
	BALL_INLINE 
	bool HashGrid3<Item>::getIndices
		(const HashGridBox3<Item>& box,
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

	template <typename Item>
	BALL_INLINE 
	void HashGrid3<Item>::insert
		(Position x, Position y, Position z, const Item& item)
		throw()
	{
		HashGridBox3<Item>* box = getBox(x, y, z);
		
		if (box != 0)
		{
			insert_(box, item);
		}
	}

	template <typename Item>
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

	template <typename Item>
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

	template <typename Item>
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

	template <typename Item>
	BALL_INLINE 
	void HashGrid3<Item>::host(Visitor< HashGrid3<Item> >& visitor)
		throw()
	{
		visitor.visit(*this);
	}

	template <typename Item>
	BALL_INLINE 
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

	template <typename Item>
	BALL_INLINE 
	bool HashGrid3<Item>::operator !=	(const HashGrid3<Item>& grid) const
		throw()
	{
		return !(*this == grid);
	}

	template <typename Item>
	BALL_INLINE 
	bool HashGrid3<Item>::isEmpty() const
		throw()
	{
		return (getSize() == 0);
	}

	template <typename Item>
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

	template <typename Item>
	void HashGrid3<Item>::dump(std::ostream& s, Size depth) const
		throw()
	{
		BALL_DUMP_STREAM_PREFIX(s);
		
		BALL_DUMP_DEPTH(s, depth);
		
		BALL_DUMP_DEPTH(s, depth);
		s << "  origin: " << origin_ << std::endl;
		
		BALL_DUMP_DEPTH(s, depth);
		s << "  unit: " << unit_.z << std::endl;
		
		BALL_DUMP_DEPTH(s, depth);
		s << "  dimension: " << dimension_x_ << " " 
			<< dimension_y_ << " " 
			<< dimension_z_ << std::endl;
		
		Size size = getSize();
		BALL_DUMP_DEPTH(s, depth);
		s << "  size: " << size << std::endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "  non empty boxes: " << countNonEmptyBoxes() << std::endl;
		
		BALL_DUMP_DEPTH(s, depth);
		s << "  boxes:" << std::endl;
		Position x, y, z;
		for (Position index = 0; index < (Position)size; ++index)
		{
			BALL_DUMP_DEPTH(s, depth);
			getIndices(box_[index], x, y, z);
			s << "    " << index << ". box: (" 
			  << x << ',' << y << ',' << z << ')' << std::endl;
			box_[index].dump(s, 1);
		}
		
		BALL_DUMP_DEPTH(s, depth);
		s << "  non-empty boxes:" << std::endl;

		for (const HashGridBox3<Item>* box = first_nonempty_; box != 0; box = box->next_)
		{
			s << "    " << getIndex_(*box) << std::endl;
		}
		BALL_DUMP_STREAM_SUFFIX(s);
	}

	template <typename Item>
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
			for (typename HashGridBox3<Item>::DataItem *item = box->first_item_; item != 0; item = item->next_)
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

	template <typename Item>
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

	template <typename Item>
	BALL_INLINE 
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

	template <typename Item>
	BALL_INLINE 
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
			
			HashGridBox3<Item>* neighborbox = 0;
			for (Position x = end_x - 2; x <= end_x; ++x)
			{
				for (Position y = end_y - 2; y <= end_y; ++y)
				{
					for (Position z = end_z - 2; z <= end_z; ++z)
					{
						neighborbox = getBox(x, y, z);
			
						if (neighborbox != 0)
							neighborbox->insert_(box);
					}
				}
			}
		}
		
		box->insert(item);
	}

	template <typename Item>
	BALL_INLINE 
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
			
			HashGridBox3<Item> *neighborbox = 0;
			for (Index x = end_x - 2; x <= end_x; ++x)
			{
				for (Index y = end_y - 2; y <= end_y; ++y)
				{
					for (Index z = end_z - 2; z <= end_z; ++z)
					{
						neighborbox = getBox(x, y, z);
			
						if (neighborbox != 0)
						{
							neighborbox->remove_(box);
						}
					}
				}
			}
		}
		
		return result;
	}
  /** @} */
} // namespace BALL

#endif // BALL_DATATYPE_HASHGRID_H
