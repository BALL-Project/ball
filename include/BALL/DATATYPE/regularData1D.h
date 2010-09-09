// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_DATATYPE_REGULARDATA1D_H
#define BALL_DATATYPE_REGULARDATA1D_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_SYSTEM_FILE_H
# include <BALL/SYSTEM/file.h>
#endif

#ifndef BALL_SYSTEM_BINARYFILEADAPTOR_H
# include <BALL/SYSTEM/binaryFileAdaptor.h>
#endif

#include <vector>
#include <iostream>
#include <fstream>
#include <iterator>
#include <algorithm>

namespace BALL
{
	/**	A class to store regularaly spaced data.
			This class can is intended to hold regularly spaced, one-dimensional data sets.
			It might be useful to hold data sets like spectra, or precomputed function values. \par
			The two bounds (set with  \link setBoundaries setBoundaries \endlink ) designate an X-range with  \link getSize getSize \endlink 
			equally spaced values. The data can be accessed in the same way as data of an STL vector
			(i.e., using operator [] and iterators).
			 \par
			This class fulfills the STL <tt>Container</tt> and <tt>Unary Function</tt> requirements.
			
			\ingroup  RegularData
	*/
	template <typename ValueType>
	class TRegularData1D
	{
		public:
			
		BALL_CREATE(TRegularData1D<ValueType>)

		/**	@name Type definitions
		*/
		//@{

		/// The IndexType
		typedef Position IndexType;
		/// The type containing an STL vector of the corresponding ValueType
		typedef std::vector<ValueType>	VectorType;
		/// The coordinate type
		typedef double CoordinateType;
		/// A mutable iterator
		typedef typename std::vector<ValueType>::iterator Iterator;
		/// A constant iterator
		typedef typename std::vector<ValueType>::const_iterator ConstIterator;
		//@}

		//	STL compatibility types
		//
		typedef ValueType value_type;
		typedef typename std::vector<ValueType>::iterator iterator;
		typedef typename std::vector<ValueType>::const_iterator const_iterator;
		typedef typename std::vector<ValueType>::reference reference;
		typedef typename std::vector<ValueType>::const_reference const_reference;
		typedef typename std::vector<ValueType>::pointer pointer;
		typedef typename std::vector<ValueType>::difference_type difference_type;
		typedef typename std::vector<ValueType>::size_type size_type;

		/** @name Constructors and Destructors.
		*/
		//@{
			
		///	Default constructor.
		TRegularData1D();

		/**	Copy constructor
		 *  @throw Exception::OutOfMemory if the memory for the copy could not be allocated.
		 */
		TRegularData1D(const TRegularData1D& data);
	
		/** Detailed constructor.
		 *  @throw Exception::OutOfMemory if the memory for the grid could not be allocated.
		 */
		TRegularData1D(const CoordinateType& origin, const CoordinateType& dimension, const CoordinateType& spacing);

		/** This constructor sets origin to 0.0 and dimension to 1.0
		 *  @throw Exception::OutOfMemory if the memory for the grid could not be allocated.
		 */
		TRegularData1D(const IndexType& size);

		/** This constructor sets origin to 0.0 and dimension to 1.0
		 *  @throw Exception::OutOfMemory if the memory for the grid could not be allocated.
		 */
		TRegularData1D(const VectorType& data, const CoordinateType& origin = 0.0, const CoordinateType& dimension = 1.0);

		///	Destructor
		virtual ~TRegularData1D();

		///	Clear the contents
		virtual void clear();
		//@}


		/**	@name Assignment
		*/
		//@{

		/**	Assignment operator.
		 *	Copy the data and the boundaries.
		 *  @throw Exception::OutOfMemory if the memory for the copy could not be allocated. 
		 */
		TRegularData1D& operator = (const TRegularData1D<ValueType>& data);

		/**	Assignment from a <tt>vector</tt> of <tt>ValueType</tt>.
		 *	Copy the contents of the data without changing the boundaries.
		 *  @throw Exception::OutOfMemory if the memory for the copy could not be allocated. 
		 */
		TRegularData1D& operator = (const VectorType& data);

		//@}

		/**	@name Predicates
		*/
		//@{
		///	Equality operator
		bool operator == (const TRegularData1D& data) const;

		/// Inequality operator
		BALL_INLINE bool operator != (const TRegularData1D& data) const { return !this->operator == (data); }

		///	Empty predicate
		BALL_INLINE bool empty() const { return data_.empty(); }

		/// Test whether a point is inside the grid
		bool isInside(const CoordinateType& x) const;
		//@}

		/**	@name	Iterators
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
		BALL_INLINE void swap(TRegularData1D<ValueType>& data) { std::swap(*this, data); }

		/** Return a nonmutable reference to a specific data element.
		 *	This is the range checking version of <tt>operator []</tt>.
		 *  @throw Exception::OutOfGrid if index is outside the grid boundaries
		 */
		const ValueType& getData(const IndexType& index) const;

		/** Return a mutable reference to a specific data element.
		 *  This is the range checking version of <tt>operator []</tt>.
		 *  @throw Exception::OutOfGrid if index is outside the grid boundaries
		 */
		ValueType& getData(const IndexType& index);

		/**	Constant random access operator.
				@note No range checking is done. For a more robust version, please
				use getData.
		*/	
		const ValueType& operator [] (const IndexType& index) const { return data_[index]; }
			
		/**	Mutable random access operator.
				@note No range checking is done. For a more robust version, please
				use getData.
		*/	
		ValueType& operator [] (const IndexType& index) { return data_[index]; }
		
		/**	Function operator.
				This operator allows the use of a TRegularData1D instance
				as a unary function. As required by the STL <tt>Unary Function</tt>
				concept, the argument <tt>x</tt> is required to be within the
				correct range. A more robust (range-checking) version of 
				this operator is implemented as \link getInterpolatedValue 
				getInterpolatedValue \endlink.
		*/
		ValueType operator () (const CoordinateType& x) const;

		/** Return the linearly interpolated value of the surrounding two grid points.
		 *	This method first performs a range check for the argument <tt>x</tt>
		 *  and then calls <tt>operator () (x)</tt> to determine an interpolated
		 *	value at that position.
		 *  @throw Exception::OutOfGrid if x is outside the grid boundaries
		*/
		ValueType getInterpolatedValue(const CoordinateType& x) const;
			
    /** Return the indices of the grid points to the left and to the right of a point.
     *  @param x a point inside the grid
     *  @param lower  index of the grid point to the left
     *  @param upper  index of the grid point to the right
     *  @throw Exception::OutOfGrid if the point is outside the grid boundaries
    */
		void getEnclosingIndices(const CoordinateType& x, Position& lower, Position& upper) const;

    /** Return the data at the grid points to the left and to the right of a point.
     *  @throw Exception::OutOfGrid if the point is outside the grid boundaries
     *  @see getEnclosingIndices
		 */
		void getEnclosingValues(const CoordinateType& x, ValueType& lower, ValueType& upper) const;

    /** Return the exact coordinates of a grid point.
     *  @return     CoordinateType
     *  @exception  Exception::OutOfGrid if the point is outside the grid boundaries
     */
    CoordinateType getCoordinates(const IndexType& index) const;

		/** Return the index of the closest grid point.
		 *	This method first performs a range check for the argument <tt>x</tt>
		 *	and then returns the index of the closest grid point to the left or
		 *	right of <tt>x</tt>.
     *  @exception  Exception::OutOfGrid if the point is outside the grid boundaries
		*/
		IndexType getClosestIndex(const CoordinateType& x) const;
			
		/** Return the index of the grid point with the next lowest coordinate.
		 *	This method first performs a range check for the argument <tt>x</tt>
		 *	and then returns the index of the closest grid point to the left (i.e. with a lesser coordinate)
		 *	of <tt>x</tt>.
     *  @exception  Exception::OutOfGrid if the point is outside the grid boundaries
		 */
		IndexType getLowerIndex(const CoordinateType& x) const;
			
		/** Return a nonmutable reference to the closest non-interpolated value.
		 *	This method first performs a range check for the argument <tt>x</tt>
		 *	and then returns the value of the closest data point to the left or
		 *	right of <tt>x</tt>.
     *  @exception  Exception::OutOfGrid if the point is outside the grid boundaries
		 */
		const ValueType& getClosestValue(const CoordinateType& x) const;
			
		/** Return a mutable reference to the closest non-interpolated value.
		 *	This method first performs a range check for the argument <tt>x</tt>
		 *	and then returns the value of the closest data point to the left or
		 *	right of <tt>x</tt>.
     *  @exception  Exception::OutOfGrid if the point is outside the grid boundaries
		 */
		ValueType& getClosestValue(const CoordinateType& x);
			
		///	Return the number of points in the data set.
		BALL_INLINE IndexType getSize() const { return (IndexType)data_.size(); }

		/**	Return the origin of the data.
				The origin represents the coordinate of the very first
			(leftmost) element, i.e. <tt>data_[0]</tt>.
		*/
		BALL_INLINE const CoordinateType& getOrigin() const { return origin_; }
		
		/**	Return the spacing of the data.
				The spacing corresponds to the distance between two adjacent
				data elements.
		*/
		BALL_INLINE const CoordinateType& getSpacing() const {	return spacing_; }
					
		/**	Set the origin of the data.
		*/
		BALL_INLINE void setOrigin(const CoordinateType& origin) { origin_ = origin; }

		/**	Return the dimension of the data.
				The dimension represents the length of the data vector.
				Hence, the coordinate of the rightmost element, <tt>data_[getSize() - 1]</tt>
				is the origin plus the dimension (<tt>getOrigin() + getDimension()</tt>).
		*/
		BALL_INLINE const CoordinateType& getDimension() const { return dimension_; }

		/**	Set the dimension of the data.
				This will affect neither the origin of the data, nor the number of
				elements stored (in contrast to \link resize() resize() \endlink).
				It will just store the appropriate scaling factor and affect the spacing.
		*/
		BALL_INLINE void setDimension(const CoordinateType& dimension) { dimension_ = dimension; }

		/**	Resize the data.
		 *	If <tt>new_size</tt> is larger than the current size, the data 
		 *	<tt>vector</tt> is extended to the new size and filled with default
		 *	constructed items of type <tt>ValueType</tt>. Resizing to a value lesser than
		 *	the current size truncates the vector.  
		 *	\par
		 *	The boundaries are adapted and the positions of the retained items
		 *	fixed, i.e.  the dimension is increased or decreased proportionally
		 *	while the origin remains unchanged.
		 *	@param new_size the new size
		 *	@throw Exception::OutOfMemory if the memory for the resized grid could not be allocated
		 */
		void resize(const IndexType& size);

		/**	Rescale the data.
		 *	Keep the current boundaries of the data and reinterpolate
		 *	the data to reflect the new size. To create a data set of <tt>new_size</tt>
		 *	data points, the data is interpolated linearly at the new data points from
		 *	the closest points in the old data set.
		 *	
		 *	@param new_size the new data set size
		 *	@throw Exception::OutOfMemory if the memory for the resized grid could not be allocated
		 */
		void rescale(const IndexType& new_size);

		/** Calculate the mean of the dataset
		 		@return ValueType
		*/
		ValueType calculateMean() const;
		
		/** Calculate the standard deviation of the dataset
		 		@return ValueType
		*/
		ValueType calculateSD() const;
		
		/** Write the grid contents in a (non-portable) binary format.
		 *	@throw FileNotFound thrown if the file could not be written
		*/
		void binaryWrite(const String& filename) const;

		/** Read the grid contents from a file written with binaryWrite
		 *	@throw FileNotFound thrown if file doesnt exists or could not be read
		 */
		void binaryRead(const String& filename);
		//@}
	
		
		protected:
		///	The origin of the data set
		CoordinateType	origin_;

		///	The dimension (length)
		CoordinateType	dimension_;

		///	The spacing
		CoordinateType	spacing_;

		///	The data
		VectorType			data_;

		/// The block data type for reading and writing binary data
		typedef struct { ValueType bt[1024]; } BlockValueType;
	};

	/**	Default type
	*/
	typedef TRegularData1D<float> RegularData1D;
	
	template <typename ValueType>
	TRegularData1D<ValueType>::TRegularData1D()
		:	origin_(0.0),
			dimension_(0.0),
			spacing_(1.0),
			data_()
	{
	}

	template <typename ValueType>
	TRegularData1D<ValueType>::~TRegularData1D()
	{
	}

	template <typename ValueType>
	TRegularData1D<ValueType>::TRegularData1D(const TRegularData1D<ValueType>& data)
		:	origin_(data.origin_),
			dimension_(data.dimension_),
			spacing_(data.spacing_),
			data_()
	{
		// Try to catch allocation errors and rethrow them as OutOfMemory
		try
		{
			data_ = data.data_;
		}
		catch (std::bad_alloc&)
		{
			throw Exception::OutOfMemory(__FILE__, __LINE__, data.size() * sizeof(ValueType));
		}
	}

	template <typename ValueType>
	TRegularData1D<ValueType>::TRegularData1D
		(const typename TRegularData1D<ValueType>::CoordinateType& origin, 
		 const typename TRegularData1D<ValueType>::CoordinateType& dimension, 
		 const typename TRegularData1D<ValueType>::CoordinateType& spacing)
		: origin_(origin),
			dimension_(dimension),
			spacing_(spacing),
			data_()
	{
		// Determine the size of the vector
		size_type size = (size_type)(dimension_ / spacing_ + 1.0);

		// Try to catch allocation errors and rethrow them as OutOfMemory
		try
		{
			data_.resize(size);
		}
		catch (std::bad_alloc&)
		{
			throw Exception::OutOfMemory(__FILE__, __LINE__, size * sizeof(ValueType));
		}
	}
			
	template <typename ValueType>
	TRegularData1D<ValueType>::TRegularData1D
		(const typename TRegularData1D<ValueType>::VectorType& data, 
		 const typename TRegularData1D<ValueType>::CoordinateType& origin, 
		 const typename TRegularData1D<ValueType>::CoordinateType& dimension)
		: origin_(origin),
			dimension_(dimension),
			spacing_(dimension / ((double)data.size()-1)),
			data_()
	{
		// Try to catch allocation errors and rethrow them as OutOfMemory
		try
		{
			data_ = data;
		}
		catch (std::bad_alloc&)
		{
			throw Exception::OutOfMemory(__FILE__, __LINE__, data.size() * sizeof(ValueType));
		}
	}


  template <class ValueType>
  TRegularData1D<ValueType>::TRegularData1D
    (const typename TRegularData1D<ValueType>::IndexType& size)
    : origin_(0.0),
      dimension_(1.0),
			data_()
  {
    // Compute the grid spacing
    spacing_ = dimension_ / (double)(size - 1);

    try
    {
      data_.resize(size);
		}
    catch (std::bad_alloc&)
    {
      data_.resize(0);
      throw Exception::OutOfMemory(__FILE__, __LINE__, size * sizeof(ValueType));
		}
	}

	template <typename ValueType>
	void TRegularData1D<ValueType>::clear()
	{
		// iterate over the data and reset all values to their default
		// boundaries and vector size remain unchanged
		static ValueType default_value = ValueType();
		std::fill(data_.begin(), data_.end(), default_value);
	}

	template <typename ValueType>
	TRegularData1D<ValueType>& TRegularData1D<ValueType>::operator = (const TRegularData1D<ValueType>& rhs)
	{
		// copy all members...
		origin_ = rhs.origin_;
		dimension_ = rhs.dimension_;
		spacing_ = rhs.spacing_;
		try 
		{
			data_ = rhs.data_;
		}
		catch (std::bad_alloc&)
		{
			data_.resize(0);
			throw Exception::OutOfMemory(__FILE__, __LINE__, rhs.size() * sizeof(ValueType));
		}

		return *this;
	}

	template <typename ValueType>
	TRegularData1D<ValueType>& TRegularData1D<ValueType>::operator = (const VectorType& rhs)
	{
		// Copy the data. The boundaries remain unchanged.
		try 
		{
			data_ = rhs;
		}
		catch (std::bad_alloc&)
		{
			data_.resize(0);
			throw Exception::OutOfMemory(__FILE__, __LINE__, rhs.size() * sizeof(ValueType));
		}
		
		return *this;
	}

	template <typename ValueType>
	bool TRegularData1D<ValueType>::operator == (const TRegularData1D<ValueType>& data) const
	{
		return (origin_ == data.origin_ 
						&& dimension_ == data.dimension_ 
						&& data_ == data.data_);
	}

	template <class ValueType>
	BALL_INLINE
	bool TRegularData1D<ValueType>::isInside(const typename TRegularData1D<ValueType>::CoordinateType& r) const
  {
    return ((r >= origin_) && (r <= (origin_ + dimension_)));
	}

	template <typename ValueType>
	BALL_INLINE
	const ValueType& TRegularData1D<ValueType>::getData(const IndexType& index) const
	{
		if (index >= data_.size())
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		}
		return data_[index];
	}
		
	template <typename ValueType>
	BALL_INLINE
	ValueType& TRegularData1D<ValueType>::getData(const IndexType& index)
	{
		if (index >= data_.size())
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		}
		return data_[index];
	}
		
	template <typename ValueType>
	void TRegularData1D<ValueType>::getEnclosingIndices
		(const typename TRegularData1D<ValueType>::CoordinateType& x,
		 Position& lower, Position& upper) const
	{
		if (!isInside(x) || (data_.size() < 2))
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		}
		lower = (Position)floor((x - origin_) / spacing_);
		if (lower == data_.size() - 1)
		{
			// If we are on the right most data point, we cannot interpolate to the right!
			lower = data_.size() - 2;
		}
		upper = lower + 1;
	}

	template <typename ValueType>
	void TRegularData1D<ValueType>::getEnclosingValues
		(const typename TRegularData1D<ValueType>::CoordinateType& x,
		 ValueType& lower, ValueType& upper) const
	{
		Position lower_index;
		Position upper_index;
		getEnclosingIndices(x, lower_index, upper_index);
		lower = data_[lower_index];
		upper = data_[upper_index];
	}

	template <typename ValueType>
	BALL_INLINE
	ValueType TRegularData1D<ValueType>::getInterpolatedValue(const CoordinateType& x) const
	{
		if (!isInside(x))
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		}
		return operator () (x);
	}
			
	template <typename ValueType>
	BALL_INLINE
	typename TRegularData1D<ValueType>::CoordinateType TRegularData1D<ValueType>::getCoordinates
		(const typename TRegularData1D<ValueType>::IndexType& index) const
	{
		if ((index >= data_.size()) || (data_.size() == 0))
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		}

		return (CoordinateType)(origin_ + (double)index / ((double)data_.size()-1) * dimension_);
	}

	template <typename ValueType>
	BALL_INLINE
	typename TRegularData1D<ValueType>::IndexType TRegularData1D<ValueType>::getClosestIndex(const CoordinateType& x) const
	{
		if ((x < origin_) || (x > (origin_ + dimension_)))
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		}

		return (IndexType)(size_type)floor((x - origin_) / spacing_ + 0.5);
	}
			
	template <typename ValueType>
	BALL_INLINE
	typename TRegularData1D<ValueType>::IndexType TRegularData1D<ValueType>::getLowerIndex(const CoordinateType& x) const
	{
		if ((x < origin_) || (x > (origin_ + dimension_)))
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		}

		return (IndexType)(size_type)floor((x - origin_) / spacing_);
	}
			
	template <typename ValueType>
	BALL_INLINE
	const ValueType& TRegularData1D<ValueType>::getClosestValue(const CoordinateType& x) const
	{
		if ((x < origin_) || (x > (origin_ + dimension_)))
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		}
		
		// Round to the closest data point.
		size_type index = (size_type)floor((x - origin_) / spacing_ + 0.5);
		return data_[index];
	}
			
	template <typename ValueType>
	BALL_INLINE
	ValueType& TRegularData1D<ValueType>::getClosestValue(const CoordinateType& x)
	{
		if ((x < origin_) || (x > (origin_ + dimension_)))
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		}
		
		// Round to the closest data point.
		size_type index = (size_type)floor((x - origin_) / spacing_ + 0.5);
		return data_[index];
	}
			
	template <typename ValueType>
	BALL_INLINE
	ValueType TRegularData1D<ValueType>::calculateMean() const
	{
		IndexType data_points	= this->getSize();
		ValueType mean = 0;
		for (IndexType i = 0; i < data_points; i++)
		{
		  mean += data_[i];
		}
		mean /= data_points;
		return mean;
	}
	
	template <typename ValueType>
	BALL_INLINE
	ValueType TRegularData1D<ValueType>::calculateSD() const
	{
		IndexType data_points	= this->getSize();
		ValueType stddev = 0;
		ValueType mean = this->calculateMean();
		for (IndexType i = 0; i < data_points; i++)
		{
			stddev += (pow(data_[i]-mean,2));
		}
		stddev /= (data_points-1);
		stddev = sqrt(stddev);
		return stddev;
	}
		
	template <typename ValueType>
	BALL_INLINE
	ValueType TRegularData1D<ValueType>::operator () (const CoordinateType& x) const
	{
		size_type left_index = (size_type)floor((x - origin_) / spacing_);
		if (left_index == data_.size() - 1)
		{
			// If we are on the right most data point, we cannot interpolate to the right!
			return data_[data_.size() - 1];
		}
		
		// Interpolate between the point to the left and the point to the right.
		double d = 1.0 - (((x - origin_) - (double)left_index * spacing_) / spacing_);
		return data_[left_index] * d + (1.0 - d) * data_[left_index + 1];
	}
			
	template <typename ValueType>
	void TRegularData1D<ValueType>::resize
		(const typename TRegularData1D<ValueType>::IndexType& new_size)
	{
		// Rescale dimension to the new size.
		if (data_.size() > 0)
		{
			dimension_ *= (double)new_size / (double)data_.size();
		}

		// Try to resize the vactor and rethrow any bad_allocs.
		try
		{
			data_.resize(new_size);
		}
		catch (std::bad_alloc&)
		{
			// The resulting vector is empty and thus well-defined.
			data_.resize(0);
			throw Exception::OutOfMemory(__FILE__, __LINE__, new_size * sizeof(ValueType));
		}
	}
		
	template <typename ValueType>
	void TRegularData1D<ValueType>::rescale
		(const typename TRegularData1D<ValueType>::IndexType& new_size)
	{
		// if the new and the old size coincide: done.
		if (new_size == (IndexType)data_.size())
		{
			return;
		}

		// Catch any bad_allocs throw by vector::resize
		try
		{
			// if the data set is empty...
			if (data_.size() == 0)
			{
				// ...there's nothing to do: a resize was requested
				data_.resize(new_size);
				return;
			}
			
			// if the data set contains only a single value,
			// we fill everything with this value
			if ((data_.size() == 1) && (new_size > 1))
			{
				ValueType old_value = data_[0];
				data_.resize(new_size);
				for (IndexType i = 1; i < new_size; i++)
				{
					data_[i] = old_value;
				}

				return;
			}

			// that's the default case: use linear interpolation
			// to determine the values at the new positions
			VectorType new_data(new_size);
			CoordinateType factor1 = (CoordinateType)data_.size() / (CoordinateType)new_size;
			CoordinateType factor2 = (CoordinateType)(data_.size() - 1) / (new_size - 1);

			for (Size i = 0; i < new_size; i++)
			{
				// determine the interval of the old data set we are currently in
				// ([old_idx, old_idx + 1])
				IndexType old_idx = (IndexType)((CoordinateType)i * factor1);

				// consider numerical inaccuracies...
				if (old_idx >= (data_.size() - 1))
				{
					old_idx = data_.size() - 2;
				}
				CoordinateType factor3 = (CoordinateType)i * factor2 - (CoordinateType)old_idx;
				new_data[i] = data_[old_idx] * (1 - factor3) + factor3 * data_[old_idx + 1];
			}

			// assign the new data
			data_ = new_data;
		}
		catch (std::bad_alloc&)
		{
			// Make sure we are in a well-defined state.
			data_.resize(0);
			throw Exception::OutOfMemory(__FILE__, __LINE__, new_size * sizeof(ValueType));			
		}
	}

	/** @name Stream I/O */
	//@{
	/// Output operator
	template <typename ValueType>
  std::ostream& operator << (std::ostream& os, const TRegularData1D<ValueType>& data)
  {
    // Write the grid origin, dimension, and number of grid points
    os << data.getOrigin() << std::endl
       << data.getOrigin() + data.getDimension() << std::endl
       << data.getSize() - 1 << std::endl;

    // Write the array contents.
    std::copy(data.begin(), data.end(), std::ostream_iterator<ValueType>(os, "\n"));
    return os;
	}

	/// Input operator
	template <typename ValueType>
	std::istream& operator >> (std::istream& is, TRegularData1D<ValueType>& grid)
  {
    typename TRegularData1D<ValueType>::CoordinateType origin;
    typename TRegularData1D<ValueType>::CoordinateType dimension;
    typename TRegularData1D<ValueType>::IndexType size;

    is >> origin;
    is >> dimension;
    is >> size;
		
		dimension -= origin;
		size++;

    grid.resize(size);
		grid.setOrigin(origin);
		grid.setDimension(dimension);

		std::copy(std::istream_iterator<ValueType>(is), 
							std::istream_iterator<ValueType>(), 
							grid.begin());
		//		std::copy_n(std::istream_iterator<ValueType>(is), grid.size(), grid.begin());
		
		return is;
	}

	template <typename ValueType>
	void TRegularData1D<ValueType>::binaryWrite(const String& filename) const
	{
		File outfile(filename, std::ios::out|std::ios::binary);
		if (!outfile.isValid()) 
		{
			throw Exception::FileNotFound(__FILE__, __LINE__, filename);
		}

		BinaryFileAdaptor<BlockValueType> adapt_block;
		BinaryFileAdaptor<ValueType>			adapt_single;
		
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

		// we slide a window of size 1024 over our data
		Index window_pos = 0;
		while (((int)data_.size() - (1024 + window_pos)) >= 0 )
		{
			adapt_block.setData(*(BlockValueType*)&(data_[window_pos]));
			outfile << adapt_block;
			window_pos += 1024;
		}

		// now we have to write the remaining data one by one
		for (Size i = window_pos; i < data_.size(); i++)
		{
			adapt_single.setData(data_[i]);
			outfile << adapt_single;
		}

		// that's it. I hope...
		outfile.close();
	}

	template <typename ValueType>
	void TRegularData1D<ValueType>::binaryRead(const String& filename)
	{
		File infile(filename, std::ios::in|std::ios::binary);
		if (!infile.isValid()) throw Exception::FileNotFound(__FILE__, __LINE__, filename);
		
		BinaryFileAdaptor<BlockValueType> adapt_block;
		BinaryFileAdaptor<ValueType>		  adapt_single;
		
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
} // namespace BALL

#endif // BALL_DATATYPE_REGULARDATA1D_H
