// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: regularData3D.h,v 1.27 2003/06/23 14:49:04 amoll Exp $ 
//

#ifndef BALL_DATATYPE_REGULARDATA3D_H
#define BALL_DATATYPE_REGULARDATA3D_H

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

#ifndef BALL_SYSTEM_FILE_H
#	include <BALL/SYSTEM/file.h>
#endif

#include <iostream>
#include <fstream>
#include <iterator>
#include <algorithm>

namespace BALL 
{
	/**	Three-dimensional grid class.
			This class represents a three-dimensional
			array. An instance of ValueType will be created
			for each point of the grid upon instantiation of TRegularData3D and initialized
			with its default value
			\par
      This class fulfills the STL <tt>Container</tt> and <tt>Unary Function</tt> requirements.
      Iteration is along the x-axis first, then along the y-axis.			
			\ingroup  RegularData
	*/
	template <typename ValueType>
	class TRegularData3D 
	{
		public:

		BALL_CREATE(TRegularData3D<ValueType>)

		/**	@name	Type Definitions
		*/
		//@{

    /// The index type used to refer to a specific element in the grid (x-, y-, and z-index)
		class IndexType
		{
			public:
			inline IndexType() : x(0), y(0), z(0) {}
			inline IndexType(Position p) : x(p), y(p), z(p) {}
			inline IndexType(Position p, Position q, Position r) : x(p), y(q), z(r) {}

			///
			Position x;
			///
			Position y;
			///
			Position z;
		};

    /// The type containing an STL vector of the appropriate type
    typedef std::vector<ValueType> VectorType;
    /// The coordinate type
    typedef TVector3<float> CoordinateType;
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
				Creates a TRegularData3D object without allocating a grid.
			*/
		TRegularData3D() throw();	

	
		/**	Copy constructor.
		*/
		TRegularData3D(const TRegularData3D<ValueType>& grid)
			throw(Exception::OutOfMemory);

		/**	Constructor for TRegularData3D.
		*/
		TRegularData3D(const CoordinateType& origin, const CoordinateType& dimension, const CoordinateType& spacing)
			 throw(Exception::OutOfMemory);

		/**	Constructor.
		*/
		TRegularData3D
			(const IndexType& size, 
			 const CoordinateType& origin = CoordinateType(0.0),
			 const CoordinateType& dimension = CoordinateType(1.0))
			 throw(Exception::OutOfMemory);

		/**	Destructor. 
		*/
		virtual ~TRegularData3D()
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

    /** Assignment operator.
        Copy the data, the origin, and the dimension (spacing is copied implicitly as well).
    */
		TRegularData3D& operator = (const TRegularData3D<ValueType>& data)
      throw(Exception::OutOfMemory);
    //@}


		/**	@name Predicates
		*/
		//@{

		/** Equality operator.
				Two point grids are equal if they have the same number of points in
				all three dimensions, same origin, spacing and all array members
				are identical.
		*/
		bool operator == (const TRegularData3D<ValueType>& grid) const throw();

		/** Inequality operator.
		*/
		BALL_INLINE bool operator != (const TRegularData3D<ValueType>& grid) const throw() {	return !this->operator == (grid); }

		/// Empty predicate
		BALL_INLINE bool empty() const throw() { return data_.empty(); }

		/// Test if a given point is inside the grid.
		bool isInside(const CoordinateType& r) const throw();
		//@}

    /** @name Iterators
    */
    //@{
    ///
    BALL_INLINE ConstIterator begin() const throw() { return data_.begin(); }
    ///
    BALL_INLINE ConstIterator end() const throw() { return data_.end(); }
    ///
    BALL_INLINE Iterator begin() throw() { return data_.begin(); }
    ///
    BALL_INLINE Iterator end() throw() { return data_.end(); }
    //@}

		/**	@name	Accessors
		*/
		//@{

    // STL compatibility
    BALL_INLINE size_type size() const throw() { return data_.size(); }
    BALL_INLINE size_type max_size() const throw() { return data_.max_size(); }
    BALL_INLINE void swap(TRegularData3D<ValueType>& grid) { std::swap(*this, grid); }


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
    const ValueType& operator [] (const IndexType& index) const throw() 
		{		
			return data_[index.x + size_.x * index.y + index.z * size_.x * size_.y]; 
		}

    /** Mutable random access operator.
        @note No range checking is done. For a more robust version, please
        use getData.
    */
    ValueType& operator [] (const IndexType& index) throw()
		{		
			return data_[index.x + size_.x * index.y + index.z * size_.x * size_.y]; 
		}

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

    /** Return the position of the grid point closest to the given vector.
        If there are multiple grid points with equal distance, the
        grid point with the lowest indices in x, y direction is returned.
        @exception OutOfGrid if the point is outside the grid
    */
    IndexType getClosestIndex(const CoordinateType& v) const
      throw(Exception::OutOfGrid);

    /** Return the position of the next grid point with coordinates smaller than the given vector.
				This yields the front lower left corner of the cube enclosing the specified position.
        @exception OutOfGrid if the point is outside the grid
    */
    IndexType getLowerIndex(const CoordinateType& v) const
      throw(Exception::OutOfGrid);

    /** Return the size of the grid.
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
    void setOrigin(const CoordinateType& origin) throw() { origin_ = origin; }

    /** Return the dimension of the data.
        The dimension represents the length of the data vector.
        Hence, the coordinate of the rightmost element, <tt>data_[getSize() - 1]</tt>
        is the origin plus the dimension (<tt>getOrigin() + getDimension()</tt>).
    */
    const CoordinateType& getDimension() const throw() { return dimension_; }

    /** Set the dimension of the data.
        This will affect neither the origin of the data, nor the number of
        elements stored (in contrast to \link resize() resize() \endlink).
        It will just store the appropriate scaling factor and affect the spacing.
    */
    void setDimension(const CoordinateType& dimension) throw()	
		{
			dimension_ = dimension;
			spacing_.x = dimension_.x / (double)(size_.x - 1);
			spacing_.y = dimension_.y / (double)(size_.y - 1);
			spacing_.z = dimension_.z / (double)(size_.z - 1);
		}

    /** Resize the data.
        If <tt>new_size</tt> is larger than the current size, the data
        <tt>vector</tt> is extended to the new size and filled with default
        constructed items of type <tt>ValueType</tt>. Resizing to a value lesser than
        the current size truncates the vector.
        \par
        The boundaries are adapted and the positions of the retained items
        fixed, i.e.  the dimension is increased or decreased proportionally
        while the origin remains unchanged.
        @param size the new size
    */
    void resize(const IndexType& size)
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

    /** Returns the exact coordinates of a grid point.
        @return     CoordinateType
        @exception  OutOfGrid if the point is outside the grid
    */
    CoordinateType getCoordinates(const IndexType& index) const
      throw(Exception::OutOfGrid);

    /** Returns the exact coordinates of a grid point.
        @return     CoordinateType
        @exception  OutOfGrid if the point is outside the grid
    */
    CoordinateType getCoordinates(Position index) const
      throw(Exception::OutOfGrid);

    /** Return the indices of the grid points of the enclosing box.
        This method calculates the grid box that contains the given vector
        and returns the indices of the grid points forming this box.
        The given point lies either in the box or is the lower left front edge of the box.
        @exception OutOfGrid if the point is outside the grid
        @param vector a point inside the grid
        @param llf  left lower front corner of the box
        @param rlf  right lower front corner of the box
        @param luf  left upper front corner of the box
        @param ruf  right upper front corner of the box
        @param llb  left lower back corner of the box
        @param rlb  right lower back corner of the box
        @param lub  left upper back corner of the box
        @param rub  right upper back corner of the box
    */
    void getEnclosingIndices
      (const CoordinateType& r,
      Position& llf, Position& rlf, Position& luf, Position& ruf,
      Position& llb, Position& rlb, Position& lub, Position& rub) const
      throw(Exception::OutOfGrid);

    /** Return the data at the grid points of the enclosing box.
				@sse getEnclosingIndices
    */
    void getEnclosingValues
      (const CoordinateType& r,
			 ValueType& llf, ValueType& rlf, ValueType& luf, ValueType& ruf,
			 ValueType& llb, ValueType& rlb, ValueType& lub, ValueType& rub) const
      throw(Exception::OutOfGrid);
		
		/** Write the grid contents in a (non-portable) binary format.
		 		@exception FileNotFound thrown if file could not be written
		*/
		void binaryWrite(const String& filename) const
			throw(Exception::FileNotFound);

		/** Read the grid contents from a file written with binaryWrite
		 		@exception FileNotFound thrown if file doesnt exists or could not be read
		*/
		void binaryRead(const String& filename)
			throw(Exception::FileNotFound);
		//@}
	
		protected:
			
		/// The grid data
		VectorType data_;

    /// Origin of the grid (offset)
    CoordinateType  origin_;

    /// Dimension of the grid
    CoordinateType  dimension_;

    /// Grid spacing
    CoordinateType  spacing_;

    /// The dimensions in grid points
    IndexType size_;

		/// The block data type for reading and writing binary data
		typedef struct { ValueType bt[1024]; } BlockValueType;
	};

	/**	Default type
	*/
	typedef TRegularData3D<float> RegularData3D;

	// default constructor.
	template <class ValueType>
	TRegularData3D<ValueType>::TRegularData3D()
		throw()
		: data_(0),
			origin_(0.0),
			dimension_(0.0),
			spacing_(1.0),
			size_(0, 0, 0)
	{
	}

	// copy constructor
	template <class ValueType>
	TRegularData3D<ValueType>::TRegularData3D
		(const TRegularData3D<ValueType>& data)
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
  TRegularData3D<ValueType>::TRegularData3D
    (const TRegularData3D<ValueType>::IndexType& size,
     const TRegularData3D<ValueType>::CoordinateType& origin,
     const TRegularData3D<ValueType>::CoordinateType& dimension)
    throw(Exception::OutOfMemory)
    : data_(),
      origin_(origin),
      dimension_(dimension),
      spacing_(0.0),
      size_(size)
  {
    // Compute the grid spacing
    spacing_.x = dimension_.x / (double)(size_.x - 1);
    spacing_.y = dimension_.y / (double)(size_.y - 1);
    spacing_.z = dimension_.z / (double)(size_.z - 1);

    // Compute the number of grid points
    size_type number_of_points = size_.x * size_.y * size_.z;
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
  TRegularData3D<ValueType>::TRegularData3D
    (const TRegularData3D<ValueType>::CoordinateType& origin,
     const TRegularData3D<ValueType>::CoordinateType& dimension,
     const TRegularData3D<ValueType>::CoordinateType& spacing)
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
    size_.z = (Size)(dimension_.z / spacing_.z + 0.5) + 1;

    // Compute the number of grid points
    size_type size = size_.x * size_.y * size_.z;
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
    spacing_.z = dimension_.z / (double)(size_.z - 1);
	}


	template <class ValueType>
	TRegularData3D<ValueType>::~TRegularData3D()
		throw()
	{
	}

  template <typename ValueType>
  BALL_INLINE
  TRegularData3D<ValueType>& TRegularData3D<ValueType>::operator =
    (const TRegularData3D<ValueType>& rhs)
    throw(Exception::OutOfMemory)
  {
		// Avoid self-assignment.
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
  void TRegularData3D<ValueType>::resize(const typename TRegularData3D<ValueType>::IndexType& size)
    throw(Exception::OutOfMemory)
  {
    // If the old size equals the new size, we're done.
    if ((size.x == size_.x) && (size_.y == size.y) && (size_.z == size.z))
    {
      return;
		}

    // If the new grid is empty, this whole thing is quite easy.
    if ((size.x == 0) || (size.y == 0) || (size.z == 0))
    {
      data_.resize(0);
      dimension_.set(0.0, 0.0, 0.0);
      return;
		}
    // Compute the new array size.
    size_type new_size = (size_type)(size.x * size.y * size.z);

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
        size_type y = (i % (size.x * size.y)) / size.x;
        size_type z = i / (size.x * size.y);
        if ((x >= size_.x) || (y >= size_.y) || (z >= size_.z))
        {
          data_[i] = default_value;
				}
        else
        {
          data_[i] = old_data[x + y * size_.x + z * size_.x * size_.y];
				}
			}

      // Correct the grid dimension. Origin and spacing remain constant.
			if ((size_.x != 0) && (size_.y != 0) && (size_.z != 0))
			{
				dimension_.x *= (double)size.x / (double)size_.x;
				dimension_.y *= (double)size.y / (double)size_.y;
				dimension_.z *= (double)size.z / (double)size_.z;
			}
      size_ = size;
		}
    catch (std::bad_alloc& e)
    {
      throw Exception::OutOfMemory(__FILE__, __LINE__, new_size * (Size)sizeof(ValueType));
		}
	}

  template <typename ValueType>
  void TRegularData3D<ValueType>::rescale(const typename TRegularData3D<ValueType>::IndexType& size)
    throw(Exception::OutOfMemory)
  {
    // If the old size equals the new size, we're done.
    if ((size.x == size_.x) && (size_.y == size.y) && (size_.z == size.z))
    {
      return;
		}

    // If the new grid is empty, this whole thing is quite easy.
    if ((size.x == 0) || (size.y == 0) || (size.z == 0))
    {
      data_.resize(0);
      dimension_.set(0.0);
      return;
		}

    // Compute the new array size.
    size_type new_size = (size_type)(size.x * size.y * size.z);

    // Catch any bad_allocs thrown by vector::resize
    try
    {
      // Create a new temporary array.
      TRegularData3D<ValueType> old_data(*this);

      // Resize the data to its new size.
      data_.resize(new_size);
      spacing_.x = dimension_.x / (double)(size.x - 1);
      spacing_.y = dimension_.y / (double)(size.y - 1);
      spacing_.z = dimension_.z / (double)(size.z - 1);

      // Walk over the new grid and copy the (interpolated) old stuff back.
      CoordinateType v;
      for (size_type i = 0; i < new_size; i++)
      {
        Position x = i % size.x;
        Position y = (i % (size.x * size.y)) / size.x;
				Position z = i / (size.x * size.y);
        v.x = origin_.x + x * spacing_.x;
        v.y = origin_.y + y * spacing_.y;
        v.y = origin_.z + z * spacing_.z;
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

	template <class ValueType> 
	BALL_INLINE
	bool TRegularData3D<ValueType>::isInside(const TRegularData3D<ValueType>::CoordinateType& r) const		
		throw()
	{
		return !((r.x > (origin_.x + dimension_.x )) 
				|| (r.y > (origin_.y + dimension_.y))  
				|| (r.z > (origin_.z + dimension_.z))
				|| (r.x < origin_.x) || (r.y < origin_.y) || (r.z < origin_.z));
	}

  template <class ValueType>
  BALL_INLINE
  const ValueType& TRegularData3D<ValueType>::getData
    (const typename TRegularData3D<ValueType>::IndexType& index) const
    throw(Exception::OutOfGrid)
  {
    size_type pos = index.x + index.x * size_.x + index.y * size_.x * size_.y;
    if (pos >= data_.size())
    {
      throw Exception::OutOfGrid(__FILE__, __LINE__);
		}
    return data_[pos];
	}


	template <typename ValueType>
  BALL_INLINE
  ValueType& TRegularData3D<ValueType>::getData
    (const typename TRegularData3D<ValueType>::IndexType& index)
    throw(Exception::OutOfGrid)
  {
    size_type pos = index.x + index.x * size_.x + index.y * size_.x * size_.y;
    if (pos >= data_.size())
    {
      throw Exception::OutOfGrid(__FILE__, __LINE__);
		}
    return data_[pos];
	}

  template <class ValueType>
  BALL_INLINE
  const ValueType& TRegularData3D<ValueType>::getData(Position index) const
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
  ValueType& TRegularData3D<ValueType>::getData(Position index)
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
  typename TRegularData3D<ValueType>::CoordinateType TRegularData3D<ValueType>::getCoordinates
    (const TRegularData3D<ValueType>::IndexType& index) const
    throw(Exception::OutOfGrid)
  {
    if ((index.x >= size_.x) || (index.y >= size_.y) || (index.z >= size_.z))
    {
      throw Exception::OutOfGrid(__FILE__, __LINE__);
		}

    CoordinateType r(origin_.x + index.x * spacing_.x,
                     origin_.y + index.y * spacing_.y,
										 origin_.z + index.z * spacing_.z);
    return r;
	}

  template <class ValueType>
  BALL_INLINE
  typename TRegularData3D<ValueType>::CoordinateType
  TRegularData3D<ValueType>::getCoordinates(Position position) const
    throw(Exception::OutOfGrid)
  {
    if (position >= data_.size())
    {
      throw Exception::OutOfGrid(__FILE__, __LINE__);
		}

    Position x = (Position)(position %  size_.x);
    Position y = (Position)((position % (size_.x * size_.y))/ size_.x);
    Position z = (Position)(position / (size_.x * size_.y));

    return CoordinateType(origin_.x + (double)x * spacing_.x,
                          origin_.y + (double)y * spacing_.y,
                          origin_.z + (double)z * spacing_.z);
	}

	template <typename ValueType>
	BALL_INLINE
	void TRegularData3D<ValueType>::getEnclosingIndices
		(const typename TRegularData3D<ValueType>::CoordinateType& r,
		Position& llf, Position& rlf, Position& luf, Position& ruf,
		Position& llb, Position& rlb, Position& lub, Position& rub) const
		throw(Exception::OutOfGrid)
	{
		if (!isInside(r))
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		}		

		// calculate the grid indices of the lower left front corner
		// of the enclosing box
		IndexType position;
		position.x = (Position)((r.x - origin_.x) / spacing_.x);
		position.y = (Position)((r.y - origin_.y) / spacing_.y);
		position.z = (Position)((r.z - origin_.z) / spacing_.z);
		
		// calculate the (linear) indices of the eight
		// box corners
		llf = position.x + size_.x * position.y 
				+ size_.x * size_.y * position.z;
		rlf = llf + 1;
		luf = llf + size_.x;
		ruf = luf + 1;
		llb = llf + size_.x * size_.y;
		rlb = llb + 1;
		lub = llb + size_.x;
		rub = lub + 1;
	}

	template <typename ValueType>
	BALL_INLINE
	void TRegularData3D<ValueType>::getEnclosingValues
		(const typename TRegularData3D<ValueType>::CoordinateType& r,
		ValueType& llf, ValueType& rlf, ValueType& luf, ValueType& ruf,
		ValueType& llb, ValueType& rlb, ValueType& lub, ValueType& rub) const
		throw(Exception::OutOfGrid)
	{
		if (!isInside(r))
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		}		
		
		// compute the eight grid indices forming the enclosing box
		Position llf_idx, rlf_idx, luf_idx, ruf_idx, llb_idx, rlb_idx, lub_idx, rub_idx;
		getEnclosingIndices(r, llf_idx, rlf_idx, luf_idx, ruf_idx, llb_idx, rlb_idx, lub_idx, rub_idx);
				
		// retrieve the grid values
		llf = data_[llf_idx];
		rlf = data_[rlf_idx];
		luf = data_[luf_idx];
		ruf = data_[ruf_idx];
		llb = data_[llb_idx];
		rlb = data_[rlb_idx];
		lub = data_[lub_idx];
		rub = data_[rub_idx];
	}

	template <typename ValueType>
	BALL_INLINE
	ValueType TRegularData3D<ValueType>::getInterpolatedValue
		(const typename TRegularData3D<ValueType>::CoordinateType& r) const
		throw(Exception::OutOfGrid)
	{
		if (!isInside(r))
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		}		
		return operator () (r);
	}

	template <typename ValueType>
	BALL_INLINE
	ValueType TRegularData3D<ValueType>::operator ()
		(const typename TRegularData3D<ValueType>::CoordinateType& r) const
		throw()
	{
		Vector3 h(r - origin_);
		Position x = (Position)(h.x / spacing_.x);
		Position y = (Position)(h.y / spacing_.y);
		Position z = (Position)(h.z / spacing_.z);

		Position Nx = size_.x;
		Position Nxy = size_.z * Nx;
		Position l = x + Nx * y + Nxy * z;
		Vector3 r_0(origin_.x + x * spacing_.x,
								origin_.y + y * spacing_.y,
								origin_.z + z * spacing_.z);


		double dx = 1. - ((double)(r.x - r_0.x) / spacing_.x);
		double dy = 1. - ((double)(r.y - r_0.y) / spacing_.y);
		double dz = 1. - ((double)(r.z - r_0.z) / spacing_.z);

		return  data_[l] * dx * dy * dz
					+ data_[l + 1] * (1 - dx) * dy * dz
					+ data_[l + Nx] * dx * (1 - dy) * dz
					+ data_[l + Nx + 1] * (1 - dx) * (1 - dy) * dz
					+ data_[l + Nxy] * dx * dy * (1 - dz)
					+ data_[l + Nxy + 1] * (1 - dx) * dy * (1 - dz)
					+ data_[l + Nxy + Nx] * dx * (1 - dy) * (1 - dz)
					+ data_[l + Nxy + Nx + 1] * (1 - dx) * (1 - dy) * (1 - dz);
	}

  template <typename ValueType>
  BALL_INLINE
  typename TRegularData3D<ValueType>::IndexType TRegularData3D<ValueType>::getClosestIndex
    (const typename TRegularData3D<ValueType>::CoordinateType& r) const
    throw(Exception::OutOfGrid)
  {
    if (!isInside(r))
    {
      throw Exception::OutOfGrid(__FILE__, __LINE__);
		}

    static IndexType position;
    position.x = (Position)((r.x - origin_.x) / spacing_.x + 0.5);
    position.y = (Position)((r.y - origin_.y) / spacing_.y + 0.5);
    position.z = (Position)((r.z - origin_.z) / spacing_.z + 0.5);

    return position;
	}

  template <typename ValueType>
  BALL_INLINE
  typename TRegularData3D<ValueType>::IndexType  TRegularData3D<ValueType>::getLowerIndex
    (const typename TRegularData3D<ValueType>::CoordinateType& r) const
    throw(Exception::OutOfGrid)
  {
    if (!isInside(r))
    {
      throw Exception::OutOfGrid(__FILE__, __LINE__);
		}

    static IndexType position;
    position.x = (Position)((r.x - origin_.x) / spacing_.x);
    position.y = (Position)((r.y - origin_.y) / spacing_.y);
    position.z = (Position)((r.z - origin_.z) / spacing_.z);

    return position;
	}

  template <typename ValueType>
  BALL_INLINE
  const ValueType& TRegularData3D<ValueType>::getClosestValue
    (const typename TRegularData3D<ValueType>::CoordinateType& r) const
    throw(Exception::OutOfGrid)
  {
    if (!isInside(r))
    {
      throw Exception::OutOfGrid(__FILE__, __LINE__);
		}

    static IndexType position;
    position.x = (Position)((r.x - origin_.x) / spacing_.x + 0.5);
    position.y = (Position)((r.y - origin_.y) / spacing_.y + 0.5);
    position.z = (Position)((r.z - origin_.z) / spacing_.z + 0.5);

    return operator [] (position);
	}

  template <typename ValueType>
  BALL_INLINE
  ValueType& TRegularData3D<ValueType>::getClosestValue
    (const typename TRegularData3D<ValueType>::CoordinateType& r)
    throw(Exception::OutOfGrid)
  {
    if (!isInside(r))
    {
      throw Exception::OutOfGrid(__FILE__, __LINE__);
		}

    static IndexType position;
    position.x = (Position)((r.x - origin_.x) / spacing_.x + 0.5);
    position.y = (Position)((r.y - origin_.y) / spacing_.y + 0.5);
    position.z = (Position)((r.z - origin_.z) / spacing_.z + 0.5);

    return operator [] (position);
	}


	template <typename ValueType>
	void TRegularData3D<ValueType>::clear() throw()
	{
    data_.resize(0);

    origin_.set(0.0);
    dimension_.set(0.0);
    size_.x = 0;
    size_.y = 0;
    size_.z = 0;
    spacing_.set(1.0);		
	}

	
	template <typename ValueType>	
	bool TRegularData3D<ValueType>::operator == (const TRegularData3D<ValueType>& grid) const 
		throw()
	{
    return ((origin_ == grid.origin_)
            && (dimension_ == grid.dimension_)
            && (size_.x == grid.size_.x)
            && (size_.y == grid.size_.y)
            && (size_.z == grid.size_.z)
            && (data_ == grid.data_));
	}	

	template <typename ValueType>
	void TRegularData3D<ValueType>::binaryWrite(const String& filename) const
		throw(Exception::FileNotFound)
	{
		File outfile(filename, std::ios::out|std::ios::binary);
		if (!outfile.isValid()) throw Exception::FileNotFound(__FILE__, __LINE__, filename);
		
		BinaryFileAdaptor< BlockValueType > adapt_block;
		BinaryFileAdaptor< ValueType >			 adapt_single;
		
		// TODO: check for endiannes and swap bytes accordingly

		// write all information we need to recreate the grid
		BinaryFileAdaptor<CoordinateType> adapt_coordinate;
		BinaryFileAdaptor<Size> 					adapt_size;

		adapt_size.setData(data_.size());
		outfile << adapt_size;
		
		adapt_coordinate.setData(origin_);
		outfile << adapt_coordinate;

		adapt_coordinate.setData(dimension_);
		outfile << adapt_coordinate;

		adapt_coordinate.setData(spacing_);
		outfile << adapt_coordinate;

		BinaryFileAdaptor<IndexType> adapt_index;
		adapt_index.setData(size_);
		outfile << adapt_index;
	
		// we slide a window of size 1024 over our data
		Index window_pos = 0;
		
		while ( ((int)data_.size() - (1024 + window_pos)) >= 0 )
		{
			adapt_block.setData(* (BlockValueType*)&(data_[window_pos]));
			outfile << adapt_block;
			window_pos+=1024;
		}

		// now we have to write the remaining data one by one
		for (Size i=window_pos; i<data_.size(); i++)
		{
			adapt_single.setData(data_[i]);
			outfile << adapt_single;
		}

		// that's it. I hope...
		outfile.close();
	}

	template <typename ValueType>
	void TRegularData3D<ValueType>::binaryRead(const String& filename)
		throw(Exception::FileNotFound)
	{
		File infile(filename, std::ios::in|std::ios::binary);
		if (!infile.isValid()) throw Exception::FileNotFound(__FILE__, __LINE__, filename);
		
		BinaryFileAdaptor< BlockValueType > adapt_block;
		BinaryFileAdaptor< ValueType >		  adapt_single;
		
		// TODO: check for endiannes and swap bytes accordingly

		// read all information we need to recreate the grid
		BinaryFileAdaptor<CoordinateType> adapt_coordinate;
		BinaryFileAdaptor<Size> 					adapt_size;

		infile >> adapt_size;
		Size new_size = adapt_size.getData();
	
		infile >> adapt_coordinate;
		origin_ = adapt_coordinate.getData();

		infile >> adapt_coordinate;
		dimension_ = adapt_coordinate.getData();

		infile >> adapt_coordinate;
		spacing_ = adapt_coordinate.getData();

		BinaryFileAdaptor<IndexType> adapt_index;
		infile >> adapt_index;
		size_ =  adapt_index.getData();
	
		data_.resize(new_size);

		// we slide a window of size 1024 over our data
		Index window_pos = 0;
		
		while ( ((int)data_.size() - (1024 + window_pos)) >= 0 )
		{
			infile >> adapt_block;
			*(BlockValueType*)(&(data_[window_pos])) = adapt_block.getData();
			/*
			for (Size i=0; i<1024; i++)
			{
				data_[i+window_pos] = adapt_block.getData().bt[i];
			}
			*/
			window_pos+=1024;
		}

		// now we have to read the remaining data one by one
		for (Size i=window_pos; i<data_.size(); i++)
		{
			infile >> adapt_single;
			data_[i] = adapt_single.getData();
		}

		// that's it. I hope...
		infile.close();
	}

	/**	@name Stream I/O */
	//@{
	/// Output operator
	template <typename ValueType>
	std::ostream& operator << (std::ostream& os, const TRegularData3D<ValueType>& grid) 
		throw()
	{
		// Write the grid origin, dimension, and number of grid points
		os << grid.getOrigin().x << " " << grid.getOrigin().y << " " << grid.getOrigin().z
			 << std::endl
			 << grid.getOrigin().x + grid.getDimension().x << " " 
			 << grid.getOrigin().y + grid.getDimension().y << " " 
			 << grid.getOrigin().z + grid.getDimension().z
			 << std::endl
			 << grid.getSize().x - 1 << " " << grid.getSize().y - 1 << " " << grid.getSize().z - 1 
			 << std::endl;

		// Write the array contents.
		std::copy(grid.begin(), grid.end(), std::ostream_iterator<ValueType>(os, "\n"));

		return os;
	}

	/// Input operator 
	template <typename ValueType>
	std::istream& operator >> (std::istream& is, TRegularData3D<ValueType>& grid) 
		throw()
	{
    typename TRegularData3D<ValueType>::CoordinateType origin;
    typename TRegularData3D<ValueType>::CoordinateType dimension;
    typename TRegularData3D<ValueType>::IndexType size;

    is >> origin.x >> origin.y >> origin.z;
    is >> dimension.x >> dimension.y >> dimension.z;
    is >> size.x >> size.y >> size.z;

    dimension -= origin;
    size.x++;
    size.y++;
    size.z++;

    grid.resize(size);
		grid.setOrigin(origin);
		grid.setDimension(dimension);
    std::copy(std::istream_iterator<ValueType>(is), std::istream_iterator<ValueType>(), grid.begin());

    return is;
	}
	//@}
	
 } // namespace BALL

#endif // BALL_DATATYPE_REGULARDATA3D_H
