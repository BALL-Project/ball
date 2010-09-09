// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_DATATYPE_REGULARDATA3D_H
#define BALL_DATATYPE_REGULARDATA3D_H

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

#ifndef BALL_SYSTEM_FILE_H
#	include <BALL/SYSTEM/file.h>
#endif

#ifndef BALL_SYSTEM_BINARYFILEADAPTOR_H
# include <BALL/SYSTEM/binaryFileAdaptor.h>
#endif

#ifndef BALL_MATHS_COMMON_H
# include <BALL/MATHS/common.h>
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
			\par
			A TRegularData3D instance can represent a non-orthogonal grid. In this case, a Matrix33
			for the conversion to Cartesian coordinates and its inverse must be provided.
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
		 *	Creates a TRegularData3D object without allocating a grid.
		 */
		TRegularData3D();	
	
		/**	Copy constructor.
		 *  @throw Exception::OutOfMemory if the memory for the copy could not be allocated
		 */
		TRegularData3D(const TRegularData3D<ValueType>& grid);

		/**	Constructor for an orthogonal TRegularData3D.
		 *  @throw Exception::OutOfMemory if the memory for the grid could not be allocated
		 */
		TRegularData3D(const CoordinateType& origin, const CoordinateType& dimension, const CoordinateType& spacing);

		/**	Constructor for a non-orthogonal TRegularData3D.
		 *  @throw Exception::OutOfMemory if the memory for the grid could not be allocated
		 */
		TRegularData3D(const CoordinateType& origin, const CoordinateType& x_axis, 
									 const CoordinateType& y_axis, const CoordinateType& z_axis, const IndexType& size);

		/**	Constructor for a standard orthogonal TRegularData3D.
		 *  @throw Exception::OutOfMemory if the memory for the grid could not be allocated
		 */
		TRegularData3D
			(const IndexType& size, 
			 const CoordinateType& origin = CoordinateType(0.0),
			 const CoordinateType& dimension = CoordinateType(1.0));

		/**	Destructor. 
		*/
		virtual ~TRegularData3D();

		/** Clear method.
			 Delete the grid contents and resize it to zero.
		*/
		virtual void clear();
		//@}

    /** @name Assignment
    */
    //@{

    /** Assignment operator.
     *  Copy the data, the origin, and the dimension (spacing is copied implicitly as well).
		 *  @throw Exception::OutOfMemory if the memory for the copy could not be allocated
     */
		TRegularData3D& operator = (const TRegularData3D<ValueType>& data);
    //@}


		/**	@name Predicates
		*/
		//@{

		/** Equality operator.
				Two point grids are equal if they have the same number of points in
				all three dimensions, same origin, spacing and all array members
				are identical.
		*/
		bool operator == (const TRegularData3D<ValueType>& grid) const;

		/** Inequality operator.
		*/
		BALL_INLINE bool operator != (const TRegularData3D<ValueType>& grid) const {	return !this->operator == (grid); }

		/// Empty predicate
		BALL_INLINE bool empty() const { return data_.empty(); }

		/// Test if a given point is inside the grid.
		bool isInside(const CoordinateType& r) const;
		//@}

		/// State whether grid is orthogonal or not.
		BALL_INLINE bool isOrthogonal() const { return is_orthogonal_;}

    /** @name Iterators
    */
    //@{
    ///
    BALL_INLINE ConstIterator begin() const { return data_.begin(); }
    ///
    BALL_INLINE ConstIterator end() const { return data_.end(); }
    ///
    BALL_INLINE Iterator begin() { return data_.begin(); }
    ///
    BALL_INLINE Iterator end() { return data_.end(); }
    //@}

		/**	@name	Accessors
		*/
		//@{

    // STL compatibility
    BALL_INLINE size_type size() const { return data_.size(); }
    BALL_INLINE size_type max_size() const { return data_.max_size(); }
    BALL_INLINE void swap(TRegularData3D<ValueType>& grid) { std::swap(*this, grid); }


		/// Get the full data
		const vector<ValueType>& getData() const;

    /** Return a nonmutable reference to a specific data element.
     *  This is the range checking version of <tt>operator []</tt>.
		 *  @throw Exception::OutOfGrid if index is outside the grid boundaries
     */
    const ValueType& getData(const IndexType& index) const;

    /** Return a mutable reference to a specific data element.
     *  This is the range checking version of <tt>operator []</tt>.
		 *  @throw Exception::OutOfGrid if index is outside the grid boundaries
     */
    ValueType& getData(const IndexType& index);

    /** Return a nonmutable reference to a specific data element.
     *  This is the range checking version of <tt>operator []</tt>.
		 *  @throw Exception::OutOfGrid if index is outside the grid boundaries
     */
    const ValueType& getData(Position index) const;

    /** Return a mutable reference to a specific data element.
     *  This is the range checking version of <tt>operator []</tt>.
		 *  @throw Exception::OutOfGrid if index is outside the grid boundaries
     */
    ValueType& getData(Position index);

    /** Nonmutable random access operator.
        @note No range checking is done. For a more robust version, please
        use getData.
    */
    const ValueType& operator [] (const IndexType& index) const 
		{		
			return data_[index.x + size_.x * index.y + index.z * size_.x * size_.y]; 
		}

    /** Mutable random access operator.
        @note No range checking is done. For a more robust version, please
        use getData.
    */
    ValueType& operator [] (const IndexType& index)
		{		
			return data_[index.x + size_.x * index.y + index.z * size_.x * size_.y]; 
		}

    /** Nonmutable random access operator.
        @note No range checking is done. For a more robust version, please
        use getData.
    */
    const ValueType& operator [] (Position index) const { return data_[index]; }

    /** Mutable random access operator.
        @note No range checking is done. For a more robust version, please
        use getData.
    */
    ValueType& operator [] (Position index) { return data_[index]; }

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
    ValueType operator () (const CoordinateType& x) const;
		
    /** Return the linearly interpolated value of the surrounding two grid points.
     *  This method first performs a range check for the argument <tt>x</tt>
     *  and then calls <tt>operator () (x)</tt> to determine an interpolated
     *  value at that position.
		 *  @throw Exception::OutOfGrid if x is outside the grid boundaries
     */
    ValueType getInterpolatedValue(const CoordinateType& x) const;

    /** Return a nonmutable reference to the closest non-interpolated value.
     *  This method first performs a range check for the argument <tt>x</tt>
     *  and then returns the value of the closest data point to the left or
     *  right of <tt>x</tt>.
		 *  @throw Exception::OutOfGrid if x is outside the grid boundaries
     */
    const ValueType& getClosestValue(const CoordinateType& x) const;

    /** Return a mutable reference to the closest non-interpolated value.
     *  This method first performs a range check for the argument <tt>x</tt>
     *  and then returns the value of the closest data point to the left or
     *  right of <tt>x</tt>.
		 *  @throw Exception::OutOfGrid if x is outside the grid boundaries
     */
    ValueType& getClosestValue(const CoordinateType& x);

    /** Return the position of the grid point closest to the given vector.
     *  If there are multiple grid points with equal distance, the
     *  grid point with the lowest indices in x, y direction is returned.
     *  @throw Exception::OutOfGrid if the point is outside the grid
     */
    IndexType getClosestIndex(const CoordinateType& v) const;

    /** Return the position of the next grid point with coordinates smaller than the given vector.
		 *	This yields the front lower left corner of the cube enclosing the specified position.
     *  @throw Exception::OutOfGrid if the point is outside the grid
     */
    IndexType getLowerIndex(const CoordinateType& v) const;

    /** Return the size of the grid.
        This method yields the number of grid points in x- and y-direction.
        Use \link size size \endlink to obtain the <em>total</em> number of
        points in the grid.
    */
    inline const IndexType& getSize() const { return size_; }
		
    /** Return the origin of the data.
        The origin represents the coordinate of the very first
        (lower left) element, i.e. <tt>data_[0]</tt>.
    */
    inline const CoordinateType& getOrigin() const { return origin_; }

    /** Return the spacing of the data.
        The spacing corresponds to the distance between two adjacent
        data elements.
    */
    const CoordinateType& getSpacing() const {  return spacing_; }
    /** Set the origin of the data.
    */
    void setOrigin(const CoordinateType& origin) { origin_ = origin; }

    /** Return the dimension of the data.
        The dimension represents the length of the data vector.
        Hence, the coordinate of the rightmost element, <tt>data_[getSize() - 1]</tt>
        is the origin plus the dimension (<tt>getOrigin() + getDimension()</tt>).
    */
    const CoordinateType& getDimension() const { return dimension_; }

    /** Set the dimension of the data.
        This will affect neither the origin of the data, nor the number of
        elements stored (in contrast to \link resize() resize() \endlink).
        It will just store the appropriate scaling factor and affect the spacing.
				\par
				Note: For non-orthogonal grids, the spacing cannot be correctly deduced and
				will consequently not be changed.
    */
    void setDimension(const CoordinateType& dimension)	
		{
			dimension_ = dimension;
			if (is_orthogonal_)
			{
				spacing_.x = dimension_.x / (double)(size_.x - 1);
				spacing_.y = dimension_.y / (double)(size_.y - 1);
				spacing_.z = dimension_.z / (double)(size_.z - 1);
			}
		}

    /** Resize the data.
     *  If <tt>new_size</tt> is larger than the current size, the data
     *  <tt>vector</tt> is extended to the new size and filled with default
     *  constructed items of type <tt>ValueType</tt>. Resizing to a value lesser than
     *  the current size truncates the vector.
     *  \par
     *  The boundaries are adapted and the positions of the retained items
     *  fixed, i.e.  the dimension is increased or decreased proportionally
     *  while the origin remains unchanged.
		 *	\par
		 *	Note: this function currently only works on orthogonal grids! For non-orthogonal
		 *	      data calls to resize are ignored.
     *  @param size the new size
		 *	@throw Exception::OutOfMemory if the memory for the resized grid could not be allocated
     */
    void resize(const IndexType& size);

    /** Rescale the data.
     *  Keep the current boundaries of the data and reinterpolate
     *  the data to reflect the new size. To create a data set of <tt>new_size</tt>
     *  data points, the data is interpolated linearly at the new data points from
     *  the closest points in the old data set.
		 *	\par
		 *	Note: this function currently only works on orthogonal grids! For non-orthogonal
		 *	      data calls to rescale are ignored.
		 * 
     *  @param new_size the new data set size
		 *	@throw Exception::OutOfMemory if the memory for the resized grid could not be allocated
		 */
    void rescale(const IndexType& new_size);

    /** Returns the exact coordinates of a grid point.
     *  @return     CoordinateType
     *  @exception  Exception::OutOfGrid if the point is outside the grid boundaries
     */
    CoordinateType getCoordinates(const IndexType& index) const;

    /** Returns the exact coordinates of a grid point.
     *  @return     CoordinateType
     *  @exception  Exception::OutOfGrid if the point is outside the grid boundaries
     */
    CoordinateType getCoordinates(Position index) const;

    /** Return the indices of the grid points of the enclosing box.
     *  This method calculates the grid box that contains the given vector
     *  and returns the indices of the grid points forming this box.
     *  The given point lies either in the box or is the lower left front edge of the box.
     *  @param vector a point inside the grid
     *  @param llf  left lower front corner of the box
     *  @param rlf  right lower front corner of the box
     *  @param luf  left upper front corner of the box
     *  @param ruf  right upper front corner of the box
     *  @param llb  left lower back corner of the box
     *  @param rlb  right lower back corner of the box
     *  @param lub  left upper back corner of the box
     *  @param rub  right upper back corner of the box
     *  @exception  Exception::OutOfGrid if the point is outside the grid boundaries
     */
    void getEnclosingIndices
      (const CoordinateType& r,
      Position& llf, Position& rlf, Position& luf, Position& ruf,
      Position& llb, Position& rlb, Position& lub, Position& rub) const;

    /** Return the data at the grid points of the enclosing box.
     *  @exception  Exception::OutOfGrid if the point is outside the grid boundaries
		 *	@see getEnclosingIndices
     */
    void getEnclosingValues
      (const CoordinateType& r,
			 ValueType& llf, ValueType& rlf, ValueType& luf, ValueType& ruf,
			 ValueType& llb, ValueType& rlb, ValueType& lub, ValueType& rub) const;
		
		/** Calculate the mean of the dataset
		 		@return ValueType
		*/
		ValueType calculateMean() const;
		
		/** Calculate the standard deviation of the dataset
		 		@return ValueType
		*/
		ValueType calculateSD() const;
		
		/** Write the grid contents in a (non-portable) binary format.
		 * 	\par Note: this currently only works correctly for orthogonal grids.
		 *	@throw Exception::FileNotFound thrown if the file could not be written
		 */
		void binaryWrite(const String& filename) const;		

		/**
		 * Writes out raw binary plus text header file.
		 * !!!WARNING!!! This method is defined only for float template parameters.
		 * If you try it to call it from any other specialization, it will give out
		 * compile error that the function has no body.
		 * @throw Exception::FileNotFound thrown if the file could not be written
		 */ 
		void binaryWriteRaw(const String& filename) const;

		/** Read the grid contents from a file written with binaryWrite
		 * 	\par Note: this currently only works correctly for orthogonal grids.
		 *	@throw Exception::FileNotFound thrown if file does not exist or could not be read
		 */
		void binaryRead(const String& filename);
		//@}
		
		protected:
			
		/// map indices to Cartesian coordinates
		const CoordinateType mapToCartesian_(CoordinateType r) const
		{
			Vector3 result;
			r.x /= (size_.x - 1.);
			r.y /= (size_.y - 1.);
			r.z /= (size_.z - 1.);

			result.x = mapping_[0] * r.x + mapping_[1] * r.y + mapping_[2] * r.z + origin_.x;
			result.y = mapping_[3] * r.x + mapping_[4] * r.y + mapping_[5] * r.z + origin_.y;
			result.z = mapping_[6] * r.x + mapping_[7] * r.y + mapping_[8] * r.z + origin_.z;

			return result;
		}
		
		/// map Cartesian coordinates to indices (note: this does not yet convert the double values to Index)
		const CoordinateType mapInverse_(CoordinateType r) const
		{
			r -= origin_;

			Vector3 result;
			result.x = inverse_mapping_[0] * r.x + inverse_mapping_[1] * r.y + inverse_mapping_[2] * r.z;
			result.y = inverse_mapping_[3] * r.x + inverse_mapping_[4] * r.y + inverse_mapping_[5] * r.z;
			result.z = inverse_mapping_[6] * r.x + inverse_mapping_[7] * r.y + inverse_mapping_[8] * r.z;

			result.x *= (size_.x - 1);
			result.y *= (size_.y - 1);
			result.z *= (size_.z - 1);

			return result;
		}
		
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

		/// A flag deciding whether the grid is orthogonal or not
		bool is_orthogonal_;

		/// mapping matrix and its inverse
		std::vector<double> mapping_;
		std::vector<double> inverse_mapping_;
	};

	/**	Default type
	*/
	typedef TRegularData3D<float> RegularData3D;

	// default constructor.
	template <class ValueType>
	TRegularData3D<ValueType>::TRegularData3D()
		: data_(0),
			origin_(0.0),
			dimension_(0.0),
			spacing_(1.0),
			size_(0, 0, 0),
			is_orthogonal_(true)
	{
	}

	// copy constructor
	template <class ValueType>
	TRegularData3D<ValueType>::TRegularData3D
		(const TRegularData3D<ValueType>& data)
		: data_(),
			origin_(data.origin_),
			dimension_(data.dimension_),
			spacing_(data.spacing_),
			size_(data.size_),
			is_orthogonal_(data.is_orthogonal_),
			mapping_(data.mapping_),
			inverse_mapping_(data.inverse_mapping_)
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
    (const typename TRegularData3D<ValueType>::IndexType& size,
     const typename TRegularData3D<ValueType>::CoordinateType& origin,
     const typename TRegularData3D<ValueType>::CoordinateType& dimension)
    : data_(),
      origin_(origin),
      dimension_(dimension),
      spacing_(0.0),
      size_(size),
			is_orthogonal_(true)
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
    (const typename TRegularData3D<ValueType>::CoordinateType& origin,
     const typename TRegularData3D<ValueType>::CoordinateType& dimension,
     const typename TRegularData3D<ValueType>::CoordinateType& spacing)
    : data_(),
      origin_(origin),
      dimension_(dimension),
      spacing_(spacing),
      size_(0),
			is_orthogonal_(true)
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
	TRegularData3D<ValueType>::TRegularData3D
		(const typename TRegularData3D<ValueType>::CoordinateType& origin, 
		 const typename TRegularData3D<ValueType>::CoordinateType& x_axis,
	 	 const typename TRegularData3D<ValueType>::CoordinateType& y_axis, 
		 const typename TRegularData3D<ValueType>::CoordinateType& z_axis, 
		 const typename TRegularData3D<ValueType>::IndexType& new_size)
		: data_(),
		  origin_(origin),
			dimension_(x_axis+y_axis+z_axis),
			spacing_(0.0),
			size_(new_size),
			is_orthogonal_(false)
	{
		// compute the spacing
		spacing_.x = x_axis.getLength() / (new_size.x - 1.);
		spacing_.y = y_axis.getLength() / (new_size.y - 1.);
		spacing_.z = z_axis.getLength() / (new_size.z - 1.);	

		size_type size = size_.x * size_.y * size_.z;
		try
		{
			data_.resize(size);
		}
		catch (std::bad_alloc&)
    {
      data_.resize(0);
      throw Exception::OutOfMemory(__FILE__, __LINE__, size * sizeof(ValueType));
		}

		// prepare the mapping matrix and its inverse
		mapping_.resize(9);
		inverse_mapping_.resize(9);

		mapping_[0] = x_axis.x; mapping_[1] = y_axis.x; mapping_[2] = z_axis.x;
		mapping_[3] = x_axis.y; mapping_[4] = y_axis.y; mapping_[5] = z_axis.y;
		mapping_[6] = x_axis.z; mapping_[7] = y_axis.z; mapping_[8] = z_axis.z;

		// this is numerically instable and only works well for the "simple"
		// cases. should be replaced by QR or an SVD

		// just for readability
		double a = mapping_[0]; double b = mapping_[1]; double c = mapping_[2];
		double d = mapping_[3]; double e = mapping_[4]; double f = mapping_[5];
		double g = mapping_[6]; double h = mapping_[7]; double i = mapping_[8];

		double determinant = 1. / (a*(e*i - f*h) - b*(d*i - f*g) + c*(d*h - e*g));

		inverse_mapping_[0] = determinant * (e*i - f*h);
		inverse_mapping_[1] = determinant * (b*i - c*h);
		inverse_mapping_[2] = determinant * (b*f - c*e);

		inverse_mapping_[3] = determinant * (f*g - d*i);
		inverse_mapping_[4] = determinant * (a*i - c*g);
		inverse_mapping_[5] = determinant * (c*d - a*f);

		inverse_mapping_[6] = determinant * (d*h - e*g);
		inverse_mapping_[7] = determinant * (b*g - a*h);
		inverse_mapping_[8] = determinant * (a*e - b*d);
	}		

	template <class ValueType>
	TRegularData3D<ValueType>::~TRegularData3D()
	{
	}

  template <typename ValueType>
  BALL_INLINE
  TRegularData3D<ValueType>& TRegularData3D<ValueType>::operator =
    (const TRegularData3D<ValueType>& rhs)
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
			
			is_orthogonal_ = rhs.is_orthogonal_;
			mapping_ = rhs.mapping_;
			inverse_mapping_ = rhs.inverse_mapping_;
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
  {
		if (is_orthogonal_)
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
			catch (std::bad_alloc&)
			{
				throw Exception::OutOfMemory(__FILE__, __LINE__, new_size * (Size)sizeof(ValueType));
			}
		}
	}

  template <typename ValueType>
  void TRegularData3D<ValueType>::rescale(const typename TRegularData3D<ValueType>::IndexType& size)
  {
		if (is_orthogonal_)
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

				// Resize the data array to its new size.
				data_.resize(new_size);
				spacing_.x = dimension_.x / (double)(size.x - 1);
				spacing_.y = dimension_.y / (double)(size.y - 1);
				spacing_.z = dimension_.z / (double)(size.z - 1);

				// Correct the grid size. Origin and dimension remain constant.
				size_ = size;

				// Walk over the new grid and copy the (interpolated) old stuff back.
				for (size_type i = 0; i < data_.size(); i++)
				{
					try
					{
						data_[i] = old_data.getInterpolatedValue(getCoordinates(i));
					}
					catch (Exception::OutOfGrid&)
					{
						data_[i] = old_data.getClosestValue(getCoordinates(i));
					}
				}
			}
			catch (std::bad_alloc&)
			{
				throw Exception::OutOfMemory(__FILE__, __LINE__, new_size * (Size)sizeof(ValueType));
			}
		}
	}

	template <class ValueType> 
	BALL_INLINE
	bool TRegularData3D<ValueType>::isInside(const typename TRegularData3D<ValueType>::CoordinateType& r) const		
	{
		if (is_orthogonal_)
		{
			return !((r.x > (origin_.x + dimension_.x )) 
					|| (r.y > (origin_.y + dimension_.y))  
					|| (r.z > (origin_.z + dimension_.z))
					|| (r.x < origin_.x) || (r.y < origin_.y) || (r.z < origin_.z));
		}	
		else 
		{
			// compute A^-1 * pos and see whether the indices are part of the grid
			CoordinateType ri = mapInverse_(r);
			ri.x = Maths::round(ri.x);
			ri.y = Maths::round(ri.y);
			ri.z = Maths::round(ri.z);
			
			return !(	  (ri.x < 0) || (ri.y < 0) || (ri.z < 0)
							 || (ri.x >= size_.x) || (ri.y >= size_.y) || (ri.z >= size_.z) );
		}
	}

  template <class ValueType>
  BALL_INLINE
  const ValueType& TRegularData3D<ValueType>::getData
    (const typename TRegularData3D<ValueType>::IndexType& index) const
  {
    size_type pos = index.x + index.y * size_.x + index.z * size_.x * size_.y;
    if (pos >= data_.size())
    {
      throw Exception::OutOfGrid(__FILE__, __LINE__);
		}
    return data_[pos];
	}

  template <class ValueType>
  BALL_INLINE
  const vector<ValueType>& TRegularData3D<ValueType>::getData() const
  {
    return data_;
	}

	template <typename ValueType>
  BALL_INLINE
  ValueType& TRegularData3D<ValueType>::getData
    (const typename TRegularData3D<ValueType>::IndexType& index)
  {
    size_type pos = index.x + index.y * size_.x + index.z * size_.x * size_.y;
    if (pos >= data_.size())
    {
      throw Exception::OutOfGrid(__FILE__, __LINE__);
		}
    return data_[pos];
	}

  template <class ValueType>
  BALL_INLINE
  const ValueType& TRegularData3D<ValueType>::getData(Position index) const
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
    (const typename TRegularData3D<ValueType>::IndexType& index) const
  {
    if ((index.x >= size_.x) || (index.y >= size_.y) || (index.z >= size_.z))
    {
      throw Exception::OutOfGrid(__FILE__, __LINE__);
		}

		if (is_orthogonal_)
		{
			CoordinateType r(origin_.x + index.x * spacing_.x,
					origin_.y + index.y * spacing_.y,
					origin_.z + index.z * spacing_.z);
			return r;
		}
		else
		{
			CoordinateType r(index.x, index.y, index.z);
			r = mapToCartesian_(r);

			return r;
		}
	}

  template <class ValueType>
  BALL_INLINE
  typename TRegularData3D<ValueType>::CoordinateType
  TRegularData3D<ValueType>::getCoordinates(Position position) const
  {
    if (position >= data_.size())
    {
      throw Exception::OutOfGrid(__FILE__, __LINE__);
		}

    Position x = (Position)(position %  size_.x);
    Position y = (Position)((position % (size_.x * size_.y))/ size_.x);
    Position z = (Position)(position / (size_.x * size_.y));

		if (is_orthogonal_)
		{
			return CoordinateType(origin_.x + (double)x * spacing_.x,
										 			  origin_.y + (double)y * spacing_.y,
														origin_.z + (double)z * spacing_.z);
		}
		else
		{
			CoordinateType r(x,y,z);
			r = mapToCartesian_(r);

			return r;
		}
	}

	template <typename ValueType>
	BALL_INLINE
	void TRegularData3D<ValueType>::getEnclosingIndices
		(const typename TRegularData3D<ValueType>::CoordinateType& r,
		Position& llf, Position& rlf, Position& luf, Position& ruf,
		Position& llb, Position& rlb, Position& lub, Position& rub) const
	{
		if (!isInside(r))
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		}		

		// calculate the grid indices of the lower left front corner
		// of the enclosing box
		IndexType position;
		if (is_orthogonal_)
		{
			position.x = (Position)((r.x - origin_.x) / spacing_.x);
			position.y = (Position)((r.y - origin_.y) / spacing_.y);
			position.z = (Position)((r.z - origin_.z) / spacing_.z);
		}
		else
		{
			CoordinateType pos = mapInverse_(r);
			position.x = (Position) pos.x;	
			position.y = (Position) pos.y;	
			position.z = (Position) pos.z;	
		}	
		
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
	{
		Position x;
		Position y;		
		Position z;		
		TVector3<double> r_0;
		
	  if (is_orthogonal_)
		{	
			Vector3 h(r - origin_);

			x = (Position)(h.x / spacing_.x);
			y = (Position)(h.y / spacing_.y);
			z = (Position)(h.z / spacing_.z);

			while (x >= (size_.x - 1)) x--;
			while (y >= (size_.y - 1)) y--;
			while (z >= (size_.z - 1)) z--;

			r_0.x = origin_.x + x*spacing_.x;
			r_0.y = origin_.y + y*spacing_.y;
			r_0.z = origin_.z + z*spacing_.z;
		}
		else
		{
			Vector3 pos = mapInverse_(r);
			x = (Position) pos.x;
			y = (Position) pos.y;
			z = (Position) pos.z;

			while (x >= (size_.x - 1)) x--;
			while (y >= (size_.y - 1)) y--;
			while (z >= (size_.z - 1)) z--;

			// This can probably be done much faster...
			Vector3 lower_pos(x,y,z);
			lower_pos = mapToCartesian_(lower_pos);
			r_0.x = lower_pos.x;
			r_0.y = lower_pos.y;
			r_0.z = lower_pos.z;
		}
		
		Position Nx = size_.x;
		Position Nxy = size_.y * Nx;
		Position l = x + Nx * y + Nxy * z;

		double dx = 1. - ((double)(r.x - r_0.x) / spacing_.x);
		double dy = 1. - ((double)(r.y - r_0.y) / spacing_.y);
		double dz = 1. - ((double)(r.z - r_0.z) / spacing_.z);

		return  data_[l] * dx * dy * dz
					+ data_[l + 1] * (1. - dx) * dy * dz
					+ data_[l + Nx] * dx * (1. - dy) * dz
					+ data_[l + Nx + 1] * (1. - dx) * (1. - dy) * dz
					+ data_[l + Nxy] * dx * dy * (1. - dz)
					+ data_[l + Nxy + 1] * (1. - dx) * dy * (1. - dz)
					+ data_[l + Nxy + Nx] * dx * (1. - dy) * (1. - dz)
					+ data_[l + Nxy + Nx + 1] * (1. - dx) * (1. - dy) * (1. - dz);
	}

  template <typename ValueType>
  BALL_INLINE
  typename TRegularData3D<ValueType>::IndexType TRegularData3D<ValueType>::getClosestIndex
    (const typename TRegularData3D<ValueType>::CoordinateType& r) const
  {
    if (!isInside(r))
    {
      throw Exception::OutOfGrid(__FILE__, __LINE__);
		}

    static IndexType position;

		if (is_orthogonal_)
		{
			position.x = (Position)((r.x - origin_.x) / spacing_.x + 0.5);
			position.y = (Position)((r.y - origin_.y) / spacing_.y + 0.5);
			position.z = (Position)((r.z - origin_.z) / spacing_.z + 0.5);
		}
		else
		{
			Vector3 pos = mapInverse_(r);
			position.x = (Position) Maths::round(pos.x);
			position.y = (Position) Maths::round(pos.y);
			position.z = (Position) Maths::round(pos.z);
		}
			
    return position;
	}

  template <typename ValueType>
  BALL_INLINE
  typename TRegularData3D<ValueType>::IndexType  TRegularData3D<ValueType>::getLowerIndex
    (const typename TRegularData3D<ValueType>::CoordinateType& r) const
  {
    if (!isInside(r))
    {
      throw Exception::OutOfGrid(__FILE__, __LINE__);
		}

    static IndexType position;
		if (is_orthogonal_)
		{
			position.x = (Position)((r.x - origin_.x) / spacing_.x);
			position.y = (Position)((r.y - origin_.y) / spacing_.y);
			position.z = (Position)((r.z - origin_.z) / spacing_.z);
		}
		else
		{
			Vector3 pos = mapInverse_(r);
			position.x = (Position)pos.x;
			position.y = (Position)pos.y;
			position.z = (Position)pos.z;
		}

    return position;
	}

  template <typename ValueType>
  BALL_INLINE
  const ValueType& TRegularData3D<ValueType>::getClosestValue
    (const typename TRegularData3D<ValueType>::CoordinateType& r) const
  {
    if (!isInside(r))
    {
      throw Exception::OutOfGrid(__FILE__, __LINE__);
		}

    static IndexType position;

		if (is_orthogonal_)
		{
			position.x = (Position)((r.x - origin_.x) / spacing_.x + 0.5);
			position.y = (Position)((r.y - origin_.y) / spacing_.y + 0.5);
			position.z = (Position)((r.z - origin_.z) / spacing_.z + 0.5);
		}
		else
		{
			static Vector3 pos = mapInverse_(r);
			position.x = (Position) Maths::round(pos.x);
			position.y = (Position) Maths::round(pos.y);
			position.z = (Position) Maths::round(pos.z);
		}

		return operator [] (position);
	}

  template <typename ValueType>
  BALL_INLINE
  ValueType& TRegularData3D<ValueType>::getClosestValue
    (const typename TRegularData3D<ValueType>::CoordinateType& r)
  {
    if (!isInside(r))
    {
      throw Exception::OutOfGrid(__FILE__, __LINE__);
		}

    static IndexType position;

		if (is_orthogonal_)
		{
			position.x = (Position)((r.x - origin_.x) / spacing_.x + 0.5);
			position.y = (Position)((r.y - origin_.y) / spacing_.y + 0.5);
			position.z = (Position)((r.z - origin_.z) / spacing_.z + 0.5);
		}
		else
		{
			static Vector3 pos = mapInverse_(r);
			position.x = (Position) Maths::round(pos.x);
			position.y = (Position) Maths::round(pos.y);
			position.z = (Position) Maths::round(pos.z);
		}


    return operator [] (position);
	}

	template <typename ValueType>
  BALL_INLINE
	ValueType TRegularData3D<ValueType>::calculateMean() const
	{
		Position data_points	= (size_.x * size_.y * size_.z);
		ValueType mean = 0;
		for (Position i = 0; i < data_points; i++)
		{
		  mean += data_[i];
		}
		mean /= data_points;
		return mean;
	}

	template <typename ValueType>
  BALL_INLINE
	ValueType TRegularData3D<ValueType>::calculateSD() const
	{
		Position data_points	= (size_.x * size_.y * size_.z);
		ValueType stddev = 0;
		ValueType mean = this->calculateMean();
		for (Position i = 0; i < data_points; i++)
		{
			stddev += (pow(data_[i]-mean,2));
		}
		stddev /= (data_points-1);
		stddev = sqrt(stddev);
		return stddev;
	}
	
	template <typename ValueType>
	void TRegularData3D<ValueType>::clear()
	{
    data_.resize(0);

    origin_.set(0.0);
    dimension_.set(0.0);
    size_.x = 0;
    size_.y = 0;
    size_.z = 0;
    spacing_.set(1.0);		
		is_orthogonal_ = true;
	}

	
	template <typename ValueType>	
	bool TRegularData3D<ValueType>::operator == (const TRegularData3D<ValueType>& grid) const 
	{
    return ((origin_ == grid.origin_)
            && (dimension_ == grid.dimension_)
            && (size_.x == grid.size_.x)
            && (size_.y == grid.size_.y)
            && (size_.z == grid.size_.z)
            && (data_ == grid.data_)
						&& (is_orthogonal_ == grid.is_orthogonal_));
	}	

	template <typename ValueType>
	void TRegularData3D<ValueType>::binaryWrite(const String& filename) const
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
