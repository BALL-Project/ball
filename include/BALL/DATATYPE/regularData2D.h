// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: regularData2D.h,v 1.28 2003/05/04 20:15:20 oliver Exp $
//

#ifndef BALL_DATATYPE_TREGULARDATA2D_H
#define BALL_DATATYPE_TREGULARDATA2D_H

#ifndef BALL_MATHS_VECTOR2_H
# include <BALL/MATHS/vector2.h>
#endif

#include <iostream>
#include <fstream>
#include <iterator>
#include <algorithm>

namespace BALL 
{
	/**	Two-dimensional grid class.
			This class represents a two-dimensional array. 
			An instance of ValueType will be created for each point of the grid upon instantiation of TRegularData2D. 
			\par
      This class fulfills the STL <tt>Container</tt> and <tt>Unary Function</tt> requirements.
			Iteration is along the x-axis first, then along the y-axis.

			\ingroup  RegularData
	*/
	template <typename ValueType>
	class TRegularData2D 
	{
		public:

		BALL_CREATE(TRegularData2D<ValueType>)

		/**	@name	Type Definitions
		*/
		//@{

		/// The type containing an STL vector of the appropriate type
		typedef std::vector<ValueType> VectorType;
		/// The index type used to refer to a specific element in the grid (x-index and y-index)
		typedef TVector2<Position> IndexType;
		/// The coordinate type
		typedef TVector2<float> CoordinateType;
		/// A mutable iterator
		typedef typename std::vector<ValueType>::iterator Iterator;
		/// A nonmutable iterator
		typedef typename std::vector<ValueType>::const_iterator ConstIterator;
		//@}

    //  STL compatibility types
    //
    typedef ValueType value_type;
    typedef typename std::vector<ValueType>::iterator iterator;
    typedef typename std::vector<ValueType>::const_iterator const_iterator;
    typedef typename std::vector<ValueType>::reference reference;
    typedef typename std::vector<ValueType>::const_reference const_reference;
    typedef typename std::vector<ValueType>::pointer pointer;
    typedef typename std::vector<ValueType>::difference_type difference_type;
    typedef typename std::vector<ValueType>::size_type size_type;

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				Creates an empty TRegularData2D object.
		*/
		TRegularData2D() throw();	

		///	Copy constructor
		TRegularData2D(const TRegularData2D<ValueType>& data)
			throw(Exception::OutOfMemory);	

		/**	Constructor.
				@param	origin the origin of the grid
				@param	dimension the dimension of the grid (extension in x- and y-direction)
				@param	spacing the grid spacing along the x- and y-axis
			 @exception OutOfMemory if insufficient memory is available to allocate the grid
		*/
		TRegularData2D(const CoordinateType& origin, const CoordinateType& dimension, const CoordinateType& spacing)
			 throw(Exception::OutOfMemory);

		/* Constructor.
			 This constructor takes the size of the grid (as the number of grid points per dimension)
			 as input and (optionally) the origin and the dimension (in grid <em>coordinates</em>).
			 @exception OutOfMemory if insufficient memory is available to allocate the grid
		*/
		TRegularData2D(const IndexType& size, 
									 const CoordinateType& origin = CoordinateType(0.0), 
									 const CoordinateType& dimension = CoordinateType(1.0))
			throw(Exception::OutOfMemory);

		/**	Destructor. 
		*/
		virtual ~TRegularData2D() 
			throw();

		/** Clear method.
				Delete the grid contents and resize it to zero.
		*/
		virtual void clear() 
			throw();
		//@}

		/** @name Assignment
		*/
		//@{

		/**	Assignment operator.
				Copy the data, the origin, and the dimension (spacing is copied implicitly as well).
		*/
		const TRegularData2D& operator = (const TRegularData2D<ValueType>& data) 
			throw(Exception::OutOfMemory);
		//@}
			

		/** @name Predicates
		*/
		//@{
		
		/** Equality operator.
				Two point grids are equal if they have the same number of points in all two
				dimensions, same origin, spacing and the data fields are equal.
				Both grids have to be valid or false is returned.
		*/
		bool operator == (const TRegularData2D<ValueType>& grid) const throw();

    /// Inequality operator
    bool operator != (const TRegularData2D<ValueType>& data) const throw() { return !this->operator == (data); }

		/// Empty predicate
		bool empty() const throw() { return data_.empty(); }

		/// Test if a given point is inside the grid.
		bool isInside(const CoordinateType& r) const throw();
		//@}

    /** @name Iterators
    */
    //@{
    ///
    ConstIterator begin() const throw() { return data_.begin(); }
    ///
    ConstIterator end() const throw() { return data_.end(); }
    ///
    Iterator begin() throw() { return data_.begin(); }
    ///
    Iterator end() throw() { return data_.end(); }
    //@}

    /** @name Accessors
    */
    //@{

    // STL compatibility
    size_type max_size() const throw() { return data_.max_size(); }
		void swap(TRegularData2D<ValueType>& grid) { std::swap(*this, grid); }
		
		/// Return the total number of grid points 
    size_type size() const throw() { return data_.size(); }

    /** Return a nonmutable reference to a specific data element.
        This is the range chacking version of <tt>operator []</tt>.
     */
    const ValueType& getData(const IndexType& index) const
      throw(Exception::OutOfGrid);

    /** Return a mutable reference to a specific data element.
        This is the range chacking version of <tt>operator []</tt>.
     */
    ValueType& getData(const IndexType& index)
      throw(Exception::OutOfGrid);

    /** Return a nonmutable reference to a specific data element.
        This is the range chacking version of <tt>operator []</tt>.
     */
    const ValueType& getData(Position index) const
      throw(Exception::OutOfGrid);

    /** Return a mutable reference to a specific data element.
        This is the range chacking version of <tt>operator []</tt>.
     */
    ValueType& getData(Position index)
      throw(Exception::OutOfGrid);

    /** Nonmutable random access operator.
        @note No range checking is done. For a more robust version, please
        use getData.
    */
    const ValueType& operator [] (const IndexType& index) const throw() { return data_[index.x + size_.x * index.y]; }

    /** Mutable random access operator.
        @note No range checking is done. For a more robust version, please
        use getData.
    */
    ValueType& operator [] (const IndexType& index) throw() { return data_[index.x + size_.x * index.y]; }

    /** Nonmutable random access operator.
        @note No range checking is done. For a more robust version, please
        use getData.
    */
    const ValueType& operator [] (Position index) const throw() { return data_[index]; }

    /** Mutable random access operator.
        @note No range checking is done. For a more robust version, please
        use getData.
    */
    ValueType& operator [] (Position index) throw() { return data_[index]; }

    /** Function operator.
        This operator allows the use of a TRegularData1D instance
        as a unary function. As required by the STL <tt>Unary Function</tt>
        concept, the argument <tt>x</tt> is required to be within the
        correct range. A more robust (range-checking) version of
        this operator is implemented as \link getInterpolatedValue
        getInterpolatedValue \endlink.
        \link getInterpolatedValue() getInterpolatedValue() \endlink.
        @precondition getOrigin() <= x <= getOrigin() + getDimension()
    */
    ValueType operator () (const CoordinateType& x) const throw();

    /** Return the linearly interpolated value of the surrounding two grid points.
        This method first performs a range check for the argument <tt>x</tt>
        and then calls <tt>operator () (x)</tt> to determine an interpolated
        value at that position.
    */
    ValueType getInterpolatedValue(const CoordinateType& x) const
      throw(Exception::OutOfGrid);

    /** Return a nonmutable reference to the closest non-interpolated value.
        This method first performs a range check for the argument <tt>x</tt>
        and then returns the value of the closest data point to the left or
        right of <tt>x</tt>.
    */
    const ValueType& getClosestValue(const CoordinateType& x) const
      throw(Exception::OutOfGrid);

    /** Return a mutable reference to the closest non-interpolated value.
        This method first performs a range check for the argument <tt>x</tt>
        and then returns the value of the closest data point to the left or
        right of <tt>x</tt>.
    */
    ValueType& getClosestValue(const CoordinateType& x)
      throw(Exception::OutOfGrid);

		/**	Return the position of the grid point closest to the given vector.
				If there are multiple grid points with equal distance, the
				grid point with the lowest indices in x, y direction is returned.
				@exception OutOfGrid if the point is outside the grid
		*/
		IndexType getClosestIndex(const CoordinateType& v) const 
			throw(Exception::OutOfGrid);

		/**	Return the size of the grid.
				This method yields the number of grid points in x- and y-direction.
				Use \link size size \endlink to obtain the <em>total</em> number of 
				points in the grid.
		*/
		inline const IndexType& getSize() const throw() { return size_; }
				
		/** Return the origin of the data.
        The origin represents the coordinate of the very first
				(lower left) element, i.e. <tt>data_[0]</tt>.
    */
    inline const CoordinateType& getOrigin() const throw() { return origin_; }

    /** Return the spacing of the data.
        The spacing corresponds to the distance between two adjacent
        data elements.
    */
    const CoordinateType& getSpacing() const throw() {  return spacing_; }

    /** Set the origin of the data.
    */
    void setOrigin(const CoordinateType& origin) throw();

    /** Return the dimension of the data.
        The dimension represents the length of the data vector.
        Hence, the coordinate of the rightmost element, <tt>data_[getSize() - 1]</tt>
        is the origin plus the dimension (<tt>getOrigin() + getDimension()</tt>).
    */
    const CoordinateType& getDimension() const throw() { return dimension_; }

    /** Set the dimension of the data.
        This will affect neither the origin of the data, nor the number of
        elements stored (in contrast to \link resize() resize() \endlink).
        It will just store the appropriate scling factor and affect the spacing.
    */
    void setDimension(const CoordinateType& dimension) throw() { dimension_ = dimension; }

    /** Resize the data.
        If <tt>new_size</tt> is larger than the current size, the data
        <tt>vector</tt> is extended to the new size and filled with default
        constructed items of type <tt>ValueType</tt>. Resizing to a value lesser than
        the current size truncates the vector.
        \par
        The boundaries are adapted and the positions of the retained items
        fixed, i.e.  the dimension is increased or decreased proportionally
        while the origin remains unchanged.
        @param new_size the new size
    */
    void resize(const IndexType& new_size)
      throw(Exception::OutOfMemory);

    /** Rescale the data.
        Keep the current boundaries of the data and reinterpolate
        the data to reflect the new size. To create a data set of <tt>new_size</tt>
        data points, the data is interpolated linearly at the new data points from
        the closest points in the old data set.

        @param new_size the new data set size
    */
    void rescale(const IndexType& new_size)
      throw(Exception::OutOfMemory);

		/**	Return the exact coordinates of a grid point.	
				@return			CoordinateType	
				@exception 	OutOfGrid if the point is outside the grid
		*/
		CoordinateType getCoordinates(const IndexType& index) const
		  throw(Exception::OutOfGrid);

		/**	Return the exact coordinates of a grid point.	
				@return			CoordinateType	
				@exception 	OutOfGrid if the point is outside the grid
		*/
		CoordinateType getCoordinates(Position index) const
		  throw(Exception::OutOfGrid);

		/**	Return the indices of the grid points of the enclosing rectangle.
				This method calculates the grid rectangle that contains the given vector
				and returns the indices of the grid points forming this rectangle.
				The given point lies either in the rectangle or is the lower left front edge of the rectangle.
				@return bool <b>true</b> if the vector is inside the grid
				@exception OutOfGrid if the point is outside the grid
				@param r	 a point inside the grid
				@param ll  left lower corner of the rectangle
				@param lr  right lower corner of the rectangle
				@param ul  left upper corner of the rectangle
				@param ur  right upper corner of the rectangle
		*/
		void getEnclosingIndices
			(const CoordinateType& r,	Position& ll, Position& lr, Position& ul, Position& ur) const
			throw(Exception::OutOfGrid);
									
		/**	Return the values at the grid points of the enclosing the spcified position.
				@see getEnclosingIndices
		*/
		void getEnclosingValues
			(const CoordinateType& r,	ValueType& ll, ValueType& lr, ValueType& ul, ValueType& ur) const
			throw(Exception::OutOfGrid);
									
		//@}

		protected:
			
		/// The grid data
		VectorType data_;

		/// Origin of the grid (offset) 
		CoordinateType	origin_;

		/// Dimension of the grid
		CoordinateType	dimension_;

		/// Grid spacing
		CoordinateType	spacing_;

		/// The dimensions in grid points
		IndexType size_;
	};

	/**	Default type
	*/
	typedef TRegularData2D<float> RegularData2D;

	// default constructor.
	template <class ValueType>
	TRegularData2D<ValueType>::TRegularData2D()
		throw()
		: data_(),
			origin_(0.0),
			dimension_(0.0),
			spacing_(1.0),
			size_(0)
	{
	}

	// copy constructor
	template <class ValueType>
	TRegularData2D<ValueType>::TRegularData2D(const TRegularData2D<ValueType>& data)
		throw(Exception::OutOfMemory)
		: data_(),
			origin_(data.origin_),
			dimension_(data.dimension_),
			spacing_(data.spacing_),
			size_(data.size_)
	{
		try
		{
			data_ = data.data_;
		}
		catch (std::bad_alloc&)
		{
			data_.resize(0);
			throw Exception::OutOfMemory(__FILE__, __LINE__, data.data_.size() * sizeof(ValueType));
		}
	}

	template <class ValueType>
	TRegularData2D<ValueType>::TRegularData2D
		(const TRegularData2D<ValueType>::IndexType& size,
		 const TRegularData2D<ValueType>::CoordinateType& origin,
		 const TRegularData2D<ValueType>::CoordinateType& dimension)
		throw(Exception::OutOfMemory)
		: data_(),
			origin_(origin),
			dimension_(dimension),
			spacing_(0.0, 0.0),
			size_(size)
	{
		// Compute the grid spacing
		spacing_.x = dimension_.x / (double)(size_.x - 1);
		spacing_.y = dimension_.y / (double)(size_.y - 1);

		// Compute the number of grid points
		size_type number_of_points = size_.x * size_.y;
		try
		{
			data_.resize(number_of_points);
		}
		catch (std::bad_alloc&)
		{
			data_.resize(0);
			throw Exception::OutOfMemory(__FILE__, __LINE__, number_of_points * sizeof(ValueType));
		}
	}

	template <class ValueType>
	TRegularData2D<ValueType>::TRegularData2D
		(const TRegularData2D<ValueType>::CoordinateType& origin,
		 const TRegularData2D<ValueType>::CoordinateType& dimension,
		 const TRegularData2D<ValueType>::CoordinateType& spacing)
		throw(Exception::OutOfMemory)
		: data_(),
			origin_(origin),
			dimension_(dimension),
			spacing_(spacing),
			size_(0)
	{
		// Compute the grid size
		size_.x = (Size)(dimension_.x / spacing_.x + 0.5) + 1;
		size_.y = (Size)(dimension_.y / spacing_.y + 0.5) + 1;
		
		// Compute the number of grid points
		size_type size = size_.x * size_.y;
		try
		{
			data_ .resize(size);
		}
		catch (std::bad_alloc&)
		{
			data_.resize(0);
			throw Exception::OutOfMemory(__FILE__, __LINE__, size * sizeof(ValueType));
		}
		
		// Adjust the spacing -- dimension has precedence.
		spacing_.x = dimension_.x / (double)(size_.x - 1);
		spacing_.y = dimension_.y / (double)(size_.y - 1);
	}

	template <class ValueType>
	TRegularData2D<ValueType>::~TRegularData2D()
		throw()
	{
	}

	// assignment operator
	template <typename ValueType>
	BALL_INLINE
	const TRegularData2D<ValueType>& TRegularData2D<ValueType>::operator = 
		(const TRegularData2D<ValueType>& rhs)
		throw(Exception::OutOfMemory)
	{
		// Avoid self assignment
		if (&rhs != this)
		{
			// Copy the coordinate-related attributes and
			// the size.
			origin_ = rhs.origin_;
			dimension_ = rhs.dimension_;
			spacing_ = rhs.spacing_;
			size_ = rhs.size_;

			// Copy the data itself and rethrow allocation exceptions.
			try
			{
				data_ = rhs.data_;
			}
			catch (std::bad_alloc&)
			{
				data_.resize(0);
				throw Exception::OutOfMemory(__FILE__, __LINE__, rhs.data_.size() * sizeof(ValueType));
			}
		}

		return *this;
	}

	template <typename ValueType>
	void TRegularData2D<ValueType>::rescale(const typename TRegularData2D<ValueType>::IndexType& size)
		throw(Exception::OutOfMemory)
	{
		// If the old size equals the new size, we're done.
		if ((size.x == size_.x) && (size_.y == size.y))
		{
			return;
		}
	
		// If the new grid is empty, this whole thing is quite easy.
		if ((size.x == 0) || (size.y == 0))
		{
			data_.resize(0);
			dimension_.set(0.0);
			return;
		}

		// Compute the new array size.
		size_type new_size = (size_type)(size.x * size.y);

		// Catch any bad_allocs thrown by vector::resize
		try
		{
			// Create a new temporary array.
			TRegularData2D<ValueType> old_data(*this);

			// Resize the data to its new size.
			data_.resize(new_size);
			spacing_.x = dimension_.x / (double)(size.x - 1);
			spacing_.y = dimension_.y / (double)(size.y - 1);
			
			// Walk over the new grid and copy the (interpolated) old stuff back.
			CoordinateType v;
			for (size_type i = 0; i < new_size; i++)
			{	
				Position x = i % size.x;
				Position y = i / size.x;
				v.x = origin_.x + x * spacing_.x;
				v.y = origin_.y + y * spacing_.y;
				data_[i] = old_data(v);
			}
			
			// Correct the grid dimension. Origin and spacing remain constant.
			size_ = size;
		}
		catch (std::bad_alloc& e)
		{
			throw Exception::OutOfMemory(__FILE__, __LINE__, new_size * (Size)sizeof(ValueType));
		}
	}

	template <typename ValueType>
	void TRegularData2D<ValueType>::resize(const typename TRegularData2D<ValueType>::IndexType& size)
		throw(Exception::OutOfMemory)
	{
		// If the old size equals the new size, we're done.
		if (size.x == size_.x && size_.y == size.y)
		{
			return;
		}
	
		// If the new grid is empty, this whole thing is quite easy.
		if ((size.x == 0) || (size.y == 0))
		{
			data_.resize(0);
			dimension_.set(0.0, 0.0);
			return;
		}

		// Compute the new array size.
		size_type new_size = (size_type)(size.x * size.y);

		// Catch any bad_allocs thrown by vector::resize
		try
		{
			// Create a new temporary array.
			std::vector<ValueType> old_data(data_);

			// Resize the data to its new size.
			data_.resize(new_size);
			
			// walk over the new grid and copy the old stuff back.
			static ValueType default_value = (ValueType)0;
			for (size_type i = 0; i < new_size; i++)
			{	
				size_type x = i % size.x;
				size_type y = i / size.x;
				if (x >= size_.x || y >= size_.y)
				{
					data_[i] = default_value;
				}
				else
				{
					data_[i] = old_data[x + y * size_.x];
				}
			}
			
			// Correct the grid dimension. Origin and spacing remain constant.
			dimension_.x *= (double)size.x / (double)size_.x;
			dimension_.y *= (double)size.y / (double)size_.y;
			size_ = size;
		}
		catch (std::bad_alloc& e)
		{
			throw Exception::OutOfMemory(__FILE__, __LINE__, new_size * (Size)sizeof(ValueType));
		}
	}

	template <class ValueType>
	void TRegularData2D<ValueType>::setOrigin(const typename TRegularData2D<ValueType>::CoordinateType& origin)
		throw()
	{
		origin_ = origin;
	}

	template <class ValueType> 
	BALL_INLINE
	bool TRegularData2D<ValueType>::isInside(const typename TRegularData2D<ValueType>::CoordinateType& r) const		
		throw()
	{
		return ((r.x >= origin_.x) && (r.x <= (origin_.x + dimension_.x))
						&& (r.y >= origin_.y) && (r.y <= (origin_.y + dimension_.y)));
	}

	template <class ValueType>
	BALL_INLINE 
	const ValueType& TRegularData2D<ValueType>::getData
		(const typename TRegularData2D<ValueType>::IndexType& index) const
		throw(Exception::OutOfGrid)
	{	
		size_type pos = index.x + index.x * size_.x;
		if (pos >= data_.size())
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		}				
		return data_[pos];
	}

	template <class ValueType>
	BALL_INLINE 
	ValueType& TRegularData2D<ValueType>::getData(const typename TRegularData2D<ValueType>::IndexType& index)
		throw(Exception::OutOfGrid)
	{	
		size_type pos = index.x + index.x * size_.x;
		if (pos >= data_.size())
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		}				
		return data_[pos];
	}

	template <class ValueType>
	BALL_INLINE 
	const ValueType& TRegularData2D<ValueType>::getData(Position index) const
		throw(Exception::OutOfGrid)
	{	
		if (index >= data_.size())
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		}				
		return data_[index];
	}

	template <class ValueType>
	BALL_INLINE 
	ValueType& TRegularData2D<ValueType>::getData(Position index)
		throw(Exception::OutOfGrid)
	{	
		if (index >= data_.size())
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		}				
		return data_[index];
	}

	template <class ValueType>
	BALL_INLINE 
	typename TRegularData2D<ValueType>::CoordinateType TRegularData2D<ValueType>::getCoordinates
		(const TRegularData2D<ValueType>::IndexType& index) const 
		throw(Exception::OutOfGrid)
	{
		if ((index.x >= size_.x) ||	(index.y >= size_.y))
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		}		
		
		CoordinateType r(origin_.x + index.x * spacing_.x,
										 origin_.y + index.y * spacing_.y);

		return r;
	}

	template <class ValueType> 
	BALL_INLINE 
	typename TRegularData2D<ValueType>::CoordinateType
	TRegularData2D<ValueType>::getCoordinates(Position position) const 
		throw(Exception::OutOfGrid)
	{
		if (position >= data_.size())
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		} 
		
		Position x = (Position)(position %  size_.x);
		Position y = (Position)(position / size_.x);

		return CoordinateType(origin_.x + (double)x * spacing_.x,
													origin_.y + (double)y * spacing_.y);
	}

	template <typename ValueType>
	BALL_INLINE
	void TRegularData2D<ValueType>::getEnclosingIndices
		(const TRegularData2D<ValueType>::CoordinateType& r,
		 Position& ll, Position& lr, Position& ul, Position& ur) const
		throw(Exception::OutOfGrid)
	{
		if (!isInside(r))
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		}		

		// Calculate the grid indices of the lower left front corner
		// of the enclosing rectangle
		IndexType position;
		position.x = (Position)((r.x - origin_.x) / spacing_.x);
		position.y = (Position)((r.y - origin_.y) / spacing_.y);
		
		// Calculate the (linear) indices of the four rectangle corners
		ll = position.x + size_.x * position.y;
		lr = ll + 1;
		ul = ll + size_.x;
		ur = ul + 1;
	}

	template <typename ValueType>
	BALL_INLINE
	void TRegularData2D<ValueType>::getEnclosingValues
		(const typename TRegularData2D<ValueType>::CoordinateType& r,
		 ValueType& ll, ValueType& lr, ValueType& ul, ValueType& ur) const
		throw(Exception::OutOfGrid)
	{
		if (!isInside(r))
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		}		
		
		// compute the four grid indices forming the enclosing rectangle
		Position ll_id, lr_id, ul_id, ur_id;
		getEnclosingIndices(r, ll_id, lr_id, ul_id, ur_id);
				
		// Retrieve the grid values
		ll = data_[ll_id];
		lr = data_[lr_id];
		ul = data_[ul_id];
		ur = data_[ur_id];
	}

	template <typename ValueType>
	BALL_INLINE
	ValueType TRegularData2D<ValueType>::getInterpolatedValue
		(const typename TRegularData2D<ValueType>::CoordinateType& r) const
		throw(Exception::OutOfGrid)
	{
		if (!isInside(r))
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		}		
		
		return this->operator () (r);
	}

	template <typename ValueType>
	BALL_INLINE
	ValueType TRegularData2D<ValueType>::operator ()
		(const typename TRegularData2D<ValueType>::CoordinateType& r) const
		throw()
	{
		CoordinateType h(r - origin_);
		Position x = (Position)(h.x / spacing_.x);
		Position y = (Position)(h.y / spacing_.y);

		// correct for numerical inaccuracies
		if (x >= (size_.x - 1))
		{
			x = size_.x - 2;
		}
		if (y >= (size_.y - 1))
		{
			y = size_.y - 2;
		}

		Size l = x + size_.x * y;
		CoordinateType r_0(getCoordinates(l));
		double dx = 1.0 - ((r.x - r_0.x) / spacing_.x);
		double dy = 1.0 - ((r.y - r_0.y) / spacing_.y);

		return  data_[l] * dx * dy
					+ data_[l + 1] * (1.0 - dx) * dy
					+ data_[l + size_.x] * dx * (1.0 - dy)
					+ data_[l + size_.x + 1] * (1.0 - dx) * (1.0 - dy);
	}

	template <typename ValueType>
	BALL_INLINE
	typename TRegularData2D<ValueType>::IndexType TRegularData2D<ValueType>::getClosestIndex
		(const typename TRegularData2D<ValueType>::CoordinateType& r) const
		throw(Exception::OutOfGrid)
	{
		if (!isInside(r))
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		}		
		
		static IndexType position;
		position.x = (Position)((r.x - origin_.x) / spacing_.x + 0.5);
		position.y = (Position)((r.y - origin_.y) / spacing_.y + 0.5);
		
		return position;
	}

	template <typename ValueType>
	BALL_INLINE
	const ValueType& TRegularData2D<ValueType>::getClosestValue
		(const typename TRegularData2D<ValueType>::CoordinateType& r) const
		throw(Exception::OutOfGrid)
	{
		if (!isInside(r))
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		}		
		
		static IndexType position;
		position.x = (Position)((r.x - origin_.x) / spacing_.x + 0.5);
		position.y = (Position)((r.y - origin_.y) / spacing_.y + 0.5);

		return data_[position];
	}

	template <typename ValueType>
	BALL_INLINE
	ValueType& TRegularData2D<ValueType>::getClosestValue
		(const typename TRegularData2D<ValueType>::CoordinateType& r)
		throw(Exception::OutOfGrid)
	{
		if (!isInside(r))
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		}		
		
		static IndexType position;
		position.x = (Position)((r.x - origin_.x) / spacing_.x + 0.5);
		position.y = (Position)((r.y - origin_.y) / spacing_.y + 0.5);

		return data_[position];
	}

	template <typename ValueType>
	void TRegularData2D<ValueType>::clear() throw()
	{
		data_.resize(0);
		
		origin_.set(0.0);
		dimension_.set(0.0, 0.0);
		size_.set(0, 0);
		spacing_.set(1.0, 1.0);
	}

	template <typename ValueType>	
	bool TRegularData2D<ValueType>::operator == (const TRegularData2D<ValueType>& grid) const 
		throw()
	{
		return ((origin_ == grid.origin_)
						&& (dimension_ == grid.dimension_)
						&& (size_.x == grid.size_.x)
						&& (size_.y == grid.size_.y)
						&& (data_ == grid.data_));
	}	


	/** @name Stream I/O */
	//@{
	/// Output operator
	template <typename ValueType>
  std::ostream& operator << (std::ostream& os, const TRegularData2D<ValueType>& grid)
    throw()
  {
    // Write the grid origin, dimension, and number of grid points
    os << grid.getOrigin().x << " " << grid.getOrigin().y
      << std::endl
      << grid.getOrigin().x + grid.getDimension().x << " "
      << grid.getOrigin().y + grid.getDimension().y
      << std::endl
      << grid.getSize().x - 1 << " " << grid.getSize().y - 1
      << std::endl;

    // Write the array contents.
    std::copy(data_.begin(), data_.end(), std::ostream_iterator<ValueType>(os, "\n"));
    return os;
	}

	/// Input operator
	template <typename ValueType>
  std::istream& operator >> (std::istream& is, TRegularData2D<ValueType>& grid)
    throw()
  {
    typename TRegularData2D<ValueType>::CoordinateType origin;
    typename TRegularData2D<ValueType>::CoordinateType dimension;
    typename TRegularData2D<ValueType>::IndexType size;

    is >> origin.x >> origin.y;
    is >> dimension.x >> dimension.y;
    is >> size.x >> size.y;
		
		dimension -= origin;
		size += TRegularData2D<ValueType>::IndexType(1);

    grid.resize(origin, dimension, size);
		std::copy_n(std::istream_iterator<ValueType>(is), grid.size(), grid.begin());
		
		return is;
	}
	//@}
	
 } // namespace BALL

#endif // BALL_DATATYPE_TREGULARDATA2D_H
