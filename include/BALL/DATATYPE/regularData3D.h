// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: regularData3D.h,v 1.18 2003/05/03 17:29:19 oliver Exp $ 

#ifndef BALL_DATATYPE_REGULARDATA3D_H
#define BALL_DATATYPE_REGULARDATA3D_H

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

#include <iostream>

namespace BALL 
{
	/**	Simple 3D grid class.
			Unlike BoxGrid this class represents a three-dimensional
			array. An instance of ValueType will be created
			for each point of the grid upon instantiation of TRegularData3D. \par
			@see	HashGrid3
			
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

		/// Index type
		typedef TVector3<Position>	IndexType;
		/// Coordinate type
		typedef TVector3<float>			CoordinateType;
		//@}

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				Creates a TRegularData3D object without allocating a grid.
				The instance is not valid.
		*/
		TRegularData3D() throw();	

		/**	Copy constructor.
				Creates a copy of an existing TRegularData3D object.
				@param grid the grid to be copied
				@param bool ignored
		*/
		TRegularData3D(const TRegularData3D<ValueType>& grid)
			throw(Exception::OutOfMemory);	

		/**	Constructor for TRegularData3D.
				{\em lower_[x,y,z]} should be set to the coordinates of
				the "lower" corner of the box represented by the grid
				{\em upper_[x,y,z]} should likewise contain the "upper" corner
				In fact, it doesn't really matter which coordinates are
				which, as this method always takes the lowest coordinates (x,y,z)
				for the lower corner and the highest coordinates for
				the upper corner. \par
				{\em grid_points_[x,y,z]} gives the number of grid points in 
				either direction. 
				@param	lower_x	float, the x coordinate of the lower corner of the grid
				@param	lower_y	float, the y coordinate of the lower corner of the grid
				@param	lower_z	float, the z coordinate of the lower corner of the grid
				@param	upper_x	float, the x coordinate of the upper corner of the grid
				@param	upper_y	float, the y coordinate of the upper corner of the grid
				@param	upper_z	float, the z coordinate of the upper corner of the grid
		*/
		TRegularData3D
			(float lower_x, 
			 float lower_y, 
			 float lower_z,
			 float upper_x, 
			 float upper_y, 
			 float upper_z,
			 Size grid_points_x, 
			 Size grid_points_y, 
			 Size grid_points_z)
			 throw(Exception::OutOfMemory);

		/**	Constructor.
				The grid's origin is at lower, it has grid_points_[x,y,z]
				points in each direction.
		*/
		TRegularData3D
			(const Vector3& lower, 
			 const Vector3& upper,
			 Size grid_points_x, 
			 Size grid_points_y, 
			 Size grid_points_z)
			 throw(Exception::OutOfMemory);

		/**	Constructor. 
				Takes lower and upper corners and the grid spacing
				The grid will include the coordinates given by upper/lower.
				Its origin is in the lower corner, it may extend up to spacing over
				the upper corner.
		*/
		TRegularData3D(const Vector3& lower, const Vector3& upper, float spacing)
			throw(Exception::OutOfMemory);

		/**	Destructor. 
				Frees all allocated memory.
		*/
		virtual ~TRegularData3D()
			throw();

		/** Clear method.
				Frees all allocated memory.
				The instance is set to not valid.
		*/
		virtual void clear() 
			throw();

		//@}

		/**	@name Assignment
		*/
		//@{

		/**	Assignment operator.
				Implemented using  \link set set \endlink .
				@see set
		*/
		const TRegularData3D& operator = (const TRegularData3D& grid) throw(Exception::OutOfMemory);

		//@}

		/**	@name	Accessors
		*/
		//@{

		/**	Returns the largest possible x coordinate for the box.
		*/
		float getMaxX() const 
			throw();

		/**	Returns the largest possible y coordinate for the box.
		*/
		float getMaxY() const 
			throw();

		/**	Returns the largest possible z coordinate for the box.
		*/
		float getMaxZ() const 
			throw();

		/**	Returns the x coordinate of the grid origin.
		*/
		float getMinX() const 
			throw();

		/**	Returns the y coordinate of the grid origin.
		*/
		float getMinY() const 
			throw();

		/**	Returns the z coordinate of the grid origin.
		*/
		float getMinZ() const 
			throw();

		/**	Return the largest grid position for the x direction.
				This method returns the maximum position allowed in the grid.
				As the point in the origin has the indices (0, 0, 0), this
				method returns the number of points in X direction minus one.
		*/
		Size getMaxXIndex() const 
			throw();

		/**	Return the largest grid position for the y direction.
				This method returns the maximum position allowed in the grid.
				As the point in the origin has the indices (0, 0, 0), this
				method returns the number of points in Y direction minus one.
		*/
		Size getMaxYIndex() const 
			throw();

		/**	Return the number of grid points for each direction.
		*/
		const IndexType& getSize() const throw() { return size_; }

		/**	Returns the total number of grid points.
		*/
		Size size() const 
			throw();

		/**	Returns the grid spacing in x direction.
		*/	
		const Vector3& getSpacing() const throw() { return spacing_; }

		/**	Returns the position of the grid point closest to the given vector.
				If there are multiple grid points with equal distance, the
				grid point with the lowest indices in x, y, and z direction is
				returned.
				@exception OutOfGrid if the point is outside the grid
		*/
		IndexType getIndex(const Vector3& r) const 
			throw(Exception::OutOfGrid);

		/**	Returns the position of the grid point closest to three given
				coordinates.
				If there are multiple grid points with equal distance, the
				grid point with the lowest indices in x, y, and z direction is
				returned.
				@exception OutOfGrid if the point is outside the grid
		*/
		IndexType getIndex(float x, float y, float z) const
			throw(Exception::OutOfGrid);
		
		/**	Returns a pointer to the grid contents determined by the three 
				indices.
				@exception OutOfGrid if the point is outside the grid
		*/
		ValueType* getData(Position i, Position j, Position k)
		  throw(Exception::OutOfGrid);

		/**	Returns a pointer to the grid contents closest to a given vector.
				Determination of the selected grid point is made via getPosition.
				@exception OutOfGrid if the point is outside the grid
		*/
		ValueType* getData(const Vector3& r) 
			throw(Exception::OutOfGrid);

		/**	Returns a pointer to the grid contents determined by the position.
				@exception OutOfGrid if the point is outside the grid
		*/
		ValueType* getData(Position position) 
			throw(Exception::OutOfGrid);

		/**	Subscript operator.
				Returns the data of the grid point specified by its <tt>position</tt>.
				@return		ValueType
				@exception OutOfGrid if the point is outside the grid
				@param		position Position, the grid position
				@see			getData
		*/
		ValueType& operator [] (Position position) 
			throw(Exception::OutOfGrid);

		/**	Subscript operator.
				Returns the data of the grid point nearest to the given
				<tt>vector</tt>
				@return		ValueType
				@exception OutOfGrid if the point is outside the grid
				@param		vector Vector3, a position in the grid
				@see			getData
		*/
		ValueType& operator [] (const Vector3& vector) 
			throw(Exception::OutOfGrid);

		/**	Returns the exact coordinates of a grid point.	
				@return		Vector3
				@exception OutOfGrid if the point is outside the grid
				@param		i Position, grid x position
				@param		j Position, grid y position
				@param		k Position, grid z position
		*/
		Vector3 getCoordinates(Position i, Position j, Position k) const
		  throw(Exception::OutOfGrid);

		/**	Returns the exact coordinates of the grid point near to a vector r.	
				This function calculates the exact coordinates of the 
				closest grid point whose x, y, and z-coordinates are smaller
				than the vector`s coordinates. With this function it is easily
				possible to determine the "box" of points that encloses the given 
				vector.
				@return		Vector3
				@exception OutOfGrid if the point is outside the grid
				@param		r Vector3
		*/
		Vector3 getCoordinates(const Vector3& r) const 
			throw(Exception::OutOfGrid);

		/**	Returns the exact coordinates of a grid point.	
				@return		Vector3
				@param		Position
		*/
		Vector3 getCoordinates(Position position) const 
			throw(Exception::OutOfGrid);

		/**	Return the indices of the grid points of the enclosing box.
				This method calculates the grid box that contains the given vector
				and returns the indices of the grid points forming this box.
				The given point lies either in the box or is the lower left front edge of the box.
				@return bool <b>true</b> if the vector is inside the grid
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
		void getBoxIndices
			(const Vector3& vector,
			Position& llf, Position& rlf, Position& luf, Position& ruf,
			Position& llb, Position& rlb, Position& lub, Position& rub) const
			throw(Exception::OutOfGrid);
													
		/**	Return the data at the grid points of the enclosing box. (Const method)
				This method calculates the grid box that contains the given vector
				and returns the values at the grid points forming this box.
				The given point lies either in the box or is the lower left front edge of the box.
				@see getBoxIndices
				@return bool <b>true</b> if the vector is inside the grid
				@exception OutOfGrid if the point is outside the grid
				@param vector a point inside the grid
				@param llf  value at the left lower front corner of the box
				@param rlf  value at the right lower front corner of the box
				@param luf  value at the left upper front corner of the box
				@param ruf  value at the right upper front corner of the box
				@param llb  value at the left lower back corner of the box
				@param rlb  value at the right lower back corner of the box
				@param lub  value at the left upper back corner of the box
				@param rub  value at the right upper back corner of the box
		*/
		void getBoxData
			(const Vector3& vector,
			ValueType& llf, ValueType& rlf, ValueType& luf, ValueType& ruf,
			ValueType& llb, ValueType& rlb, ValueType& lub, ValueType& rub) const
			throw(Exception::OutOfGrid);
													
		/**	Returns a vector to the grid's origin.
				@return		Vector3& the grid origin
		*/
		const Vector3& getOrigin() const throw() { return origin_; }

		/**	Modifies the grid's origin
				@param		origin, new origin
		*/
		void setOrigin(const Vector3& origin) throw();

		/**	Modifies the grid's origin
				@param		x new origin x
				@param		y new origin y
				@param		z new origin z
		*/
		void setOrigin(float x, float y, float z) throw();

		/**	Returns a vector containing the grid's dimensions.
				@return		Vector3&
		*/
		Vector3 getDimension() const throw() { return upper_ - origin_; }
		
		/** Test if a given point is inside the grid.
				Also returns false if instance is not valid.
				@param vector the point
				@return bool
		*/
		bool has(const Vector3& vector) const throw();

		/** Test if a given point is inside the grid.
				Also returns false if instance is not valid.
				@param x, y, z the coordinates
				@return bool
		*/
		bool has(float x, float y, float z) const throw();

		/**	Returns the linear interpolation of the eight surrounding grid points.
				This method calculates the corresponding box to a vector and linearly.
				Then, it calculates the position of this vector in box coordinates dx, dy, and dz
				with $0 \le dx, dy, dz \le 1$. It then calculates the weighted average of 
				the values at the eight surrounding grid points $v_i$:
				\TEX{
					\begin{eqnarray*}
						{\mathrm value} & = & v_1 dx dy dz \par
														& + & v_2 (1 - dx) dy dz \par
														& + & v_3 dx (1 - dy) dz \par
														& + & v_4 (1 - dx) (1 - dy) dz \par
														& + & v_5 dx dy (1 - dz) \par
														& + & v_6 (1 - dx) dy (1 - dz) \par
														& + & v_7 dx (1 - dy) (1 - dz) \par
														& + & v_8 (1 - dx) (1 - dy) (1 - dz) \par
					\end{eqnarray*}
				}
				@exception OutOfGrid if the point is outside the grid
				@param	vector the position to evaluate
		*/
		ValueType getInterpolatedValue(const Vector3& vector) 
			const throw(Exception::OutOfGrid);
		
		/** Equality operator.
				Two point grids are equal if they have the same number of points in
				all three dimensions, same origin, spacing and the data fields are
				equal. Both grids have to be valid or false is returned.
		*/
		bool operator == (const TRegularData3D<ValueType>& grid) const
			throw();

		/** Inequality operator.
				@see operator ==
		*/
		bool operator != (const TRegularData3D<ValueType>& grid) const throw() {	return !this->operator == (grid); }

		//@}
		
		///
		void resize(Size i, Size j, Size k)
			throw(Exception::OutOfMemory);

		///
		void resize(float lower_x, float lower_y, float lower_z,
				float upper_x, float upper_y, float upper_z,
				Size number_of_grid_points_x, Size number_of_grid_points_y, Size number_of_grid_points_z)
			throw(Exception::OutOfMemory);

		///
		void resize(Vector3 lower, Vector3 upper,
				Size number_of_grid_points_x, Size number_of_grid_points_y, Size number_of_grid_points_z)
			throw(Exception::OutOfMemory);

		///
		void resize(Vector3 lower, Vector3 upper, TVector3<Size> number_of_grid_points)
			throw(Exception::OutOfMemory);

		/**	The grid data
		*/
		ValueType* data;

		protected:
			
		/*_ origin of the box (offset) 
		*/	
		Vector3	origin_;

		/*_ size of the box 
		*/
		Vector3	dimension_;

		/*_ distance grid spacing (spacing = size / (number_of_points - 1))
		*/
		Vector3 spacing_;

		/*_ number of grid points of the box
		*/
		IndexType size_;

		/*_ contains the total number of grid points
		*/
		Size number_of_grid_points_;

		/*_ contains the topmost edge
		*/
		Vector3 upper_;

		/*_ is set to true, if the grid has been set up correctly.
				If the requested memory couldn't be allocated, valid_ is set to
				false. The state of this flag can be queried by isValid()
				@see	isValid
		*/
		bool valid_;
	};

	template <typename T>
	std::ostream& operator << (std::ostream& os, const TRegularData3D<T>& grid) 
		throw()
	{
		os << grid.getMinX() << " " << grid.getMinY() << " " << grid.getMinZ() 
			<< std::endl
			<< grid.getMaxX() << " " << grid.getMaxY() << " " << grid.getMaxZ() 
			<< std::endl
			<< grid.getSize().x - 1 << " " << grid.getSize().y - 1 << " " << grid.getSize().z - 1 
			<< std::endl;

		for (Position i = 0; i < grid.size(); i++)	
		{
			os << grid.data[i] << std::endl;
		}
		return os;
	}

	template <typename T>
	std::istream& operator >> (std::istream& is, TRegularData3D<T>& grid) 
		throw()
	{
		Vector3 lower;
		Vector3 upper;
		TVector3<Size> number_of_grid_points;

		is >> lower.x >> lower.y >> lower.z;
		is >> upper.x >> upper.y >> upper.z;
		is >> number_of_grid_points.x >> number_of_grid_points.y 
			 >> number_of_grid_points.z;

		grid.resize(lower, upper, number_of_grid_points.x + 1, 
								number_of_grid_points.y + 1, number_of_grid_points.z + 1);

		for (Position i = 0; i < grid.size(); i++)
		{
			is >> grid[i];
		}
		
		return is;
	}
	
	/**	Default type
	*/
	typedef TRegularData3D<float> RegularData3D;

	// default constructor.
	template <class ValueType>
	TRegularData3D<ValueType>::TRegularData3D()
		throw()
		: data(0),
			origin_(0,0,0),
			dimension_(0,0,0),
			spacing_(0,0,0),
			size_(0, 0, 0),
			number_of_grid_points_(0),
			upper_(0,0,0),
			valid_(false)
	{
	}

	// copy constructor
	template <class ValueType>
	TRegularData3D<ValueType>::TRegularData3D
		(const TRegularData3D<ValueType>& grid)
		throw(Exception::OutOfMemory)
		: data(0),
			origin_(0,0,0),
			dimension_(0,0,0),
			spacing_(0,0,0),
			size_(0, 0, 0),
			number_of_grid_points_(0),
			upper_(0,0,0),
			valid_(false)
	{

		// resize the grid
		resize(grid.origin_, grid.upper_, grid.size_.x, grid.size_.y, grid.size_.z);

		// copy data
		if (valid_)
		{
			for (Position i = 0; i < number_of_grid_points_; i++)
			{
				data[i] = grid.data[i];
			}
		}

	}

	//  First constructor for TRegularData3D
	//  lower_[x,y,z] should be set to the coordinates of
	//  the "lower" corner of the box represented by the grid
	//  upper_[x,y,z] should likewise contain the "upper" corner
	//  In fact, it doesn't really matter which coordinates are
	//  which, as it always takes the lowest coordinates (x,y,z)
	//  for the lower corner and the highest coordinates for
	//  the upper corner.
	//  grid_points_[x,y,z] gives the number of grid points in
	//  either direction.
	template <class ValueType>
	BALL_INLINE
	TRegularData3D<ValueType>::TRegularData3D
		(float lower_x, float lower_y, float lower_z,
		 float upper_x, float upper_y, float upper_z,
		 Size grid_points_x, Size grid_points_y, Size grid_points_z)
		throw(Exception::OutOfMemory)
		: data(0),
			origin_(0,0,0),
			dimension_(0,0,0),
			spacing_(0,0,0),
			size_(0, 0, 0),
			number_of_grid_points_(0),
			upper_(0,0,0),
			valid_(false)
	{
		resize(lower_x, lower_y, lower_z, upper_x, upper_y, upper_z, 
				grid_points_x, grid_points_y, grid_points_z);
	}

	template <class ValueType>
	TRegularData3D<ValueType>::TRegularData3D
		(const Vector3& lower, const Vector3& upper,
		 Size grid_points_x, Size grid_points_y, Size grid_points_z) 
		throw(Exception::OutOfMemory)
		: data(0),
			origin_(0,0,0),
			dimension_(0,0,0),
			spacing_(0,0,0),
			size_(0, 0, 0),
			number_of_grid_points_(0),
			upper_(0,0,0),
			valid_(false)
	{
		resize(lower, upper, 
				grid_points_x, grid_points_y, grid_points_z);
	}

	template <class ValueType>
	TRegularData3D<ValueType>::TRegularData3D
		(const Vector3& lower,
		 const Vector3& upper,
		 float spacing) 
		throw(Exception::OutOfMemory)
		: data(0),
			origin_(0,0,0),
			dimension_(0,0,0),
			spacing_(0,0,0),
			size_(0, 0, 0),
			number_of_grid_points_(0),
			upper_(0,0,0),
			valid_(false)
	{
		resize(lower.x, lower.y, lower.z, upper.x, upper.y, upper.z,
				(Size)((upper.x - lower.x) / spacing + 1), 
				(Size)((upper.y - lower.y) / spacing + 1), 
				(Size)((upper.z - lower.z) / spacing + 1));
	}

	template <class ValueType>
	TRegularData3D<ValueType>::~TRegularData3D()
		throw()
	{
		clear();
	}

	// assignment operator
	template <typename ValueType>
	BALL_INLINE
	const TRegularData3D<ValueType>& TRegularData3D<ValueType>::operator = 
		(const TRegularData3D<ValueType>& grid)
		throw(Exception::OutOfMemory)
	{
		resize(grid.origin_, grid.upper_, grid.size_.x, grid.size_.y, grid.size_.z);
		if (valid_)
		{
			for (Position i = 0; i < number_of_grid_points_; i++)
			{
				data[i] = grid.data[i];
			}
		}

		return *this;
	}

	template <typename ValueType>
	BALL_INLINE
	void TRegularData3D<ValueType>::resize(Size i, Size j, Size k)
		throw(Exception::OutOfMemory)
	{
		// 
		if (data != 0)
		{
			delete [] data;
			data = 0;
		}

		// set the number of grid points in all directions
		size_.set(i, j, k);

		// if the number of grid points in any direction is below 2
		// (which means that the grid is not three-dimensional!)
		// then increase to at least one point in each dimension
		if (size_.x < 2)
		{
			size_.x = 2;
		}
		if (size_.y < 2)
		{
			size_.y = 2;
		}
		if (size_.z < 2)
		{
			size_.z = 2;
		}

		// calculate the total number of grid points
		number_of_grid_points_ 
			= size_.x * size_.y * size_.z;
		
		// allocate space for the array containing pointers to the objects
		data = new ValueType[number_of_grid_points_];

		// mark this instance as invalid if the alloc failed
		valid_ = (data != 0);			

		if (!valid_)
		{
			throw Exception::OutOfMemory(__FILE__, __LINE__, 
					number_of_grid_points_ * (Size)sizeof(ValueType));
		}
	}

	template <typename ValueType>
	BALL_INLINE
	void TRegularData3D<ValueType>::resize(float lower_x, float lower_y, float lower_z,
			float upper_x, float upper_y, float upper_z,
			Size number_of_grid_points_x, Size number_of_grid_points_y, Size number_of_grid_points_z)
		throw(Exception::OutOfMemory)
	{

		// throw away the old data
		clear();

		// resize the data section and set the internal variables holding the
		// number of grid points for every direction
		resize(number_of_grid_points_x, number_of_grid_points_y,
				number_of_grid_points_z);

		// calculate the origin as the lowest given coordinates
		// of each direction
		origin_.x = ( lower_x < upper_x ) ? lower_x : upper_x;
		origin_.y = ( lower_y < upper_y ) ? lower_y : upper_y;
		origin_.z = ( lower_z < upper_z ) ? lower_z : upper_z;

		// calculate the box sizes
		dimension_.x = fabs(upper_x - lower_x);
		dimension_.y = fabs(upper_y - lower_y);
		dimension_.z = fabs(upper_z - lower_z);

		// calculate the topmost edge
		upper_.set(origin_.x + dimension_.x, origin_.y + dimension_.y, origin_.z + dimension_.z); 

		// calculate the grid spacing in all directions
		spacing_.x = dimension_.x / (size_.x - 1);
		spacing_.y = dimension_.y / (size_.y - 1);
		spacing_.z = dimension_.z / (size_.z - 1);
  }

	template <typename ValueType>
	BALL_INLINE
	void TRegularData3D<ValueType>::resize(Vector3 lower, Vector3 upper,
			Size number_of_grid_points_x, Size number_of_grid_points_y, Size number_of_grid_points_z)
		throw(Exception::OutOfMemory)
	{
		resize(lower.x, lower.y, lower.z, upper.x, upper.y, upper.z,
				number_of_grid_points_x, number_of_grid_points_y, number_of_grid_points_z);
	}

	template <typename ValueType>
	BALL_INLINE
	void TRegularData3D<ValueType>::resize(Vector3 lower, Vector3 upper, TVector3<Size> number_of_grid_points)
		throw(Exception::OutOfMemory)
	{
		resize(lower.x, lower.y, lower.z, upper.x, upper.y, upper.z,
				number_of_grid_points.x, number_of_grid_points.y, number_of_grid_points.z);
	}

	// getMax[x,y,z] returns the maximum possible coordinates for
	// the box, i.e. origin + size
	template <class ValueType>
	BALL_INLINE 
	float TRegularData3D<ValueType>::getMaxX() const throw()
	{
		return upper_.x;
	}

	template <class ValueType>
	BALL_INLINE 
	float TRegularData3D<ValueType>::getMaxY() const throw()
	{
		return upper_.y;
	}

	template <class ValueType>
	BALL_INLINE 
	float TRegularData3D<ValueType>::getMaxZ() const throw()
	{
		return upper_.z;
	}

	// getMax[x,y,z] returns the maximum possible coordinates for
	// the box, i.e. origin + size
	template <class ValueType>
	BALL_INLINE 
	float TRegularData3D<ValueType>::getMinX() const throw()
	{
		return origin_.x;
	}

	template <class ValueType>
	BALL_INLINE 
	float TRegularData3D<ValueType>::getMinY() const throw()
	{
		return origin_.y;
	}

	template <class ValueType>
	BALL_INLINE 
	float TRegularData3D<ValueType>::getMinZ() const throw()
	{
		return origin_.z;
	}

	// getMax[x,y,z]Position returns the maximum grid position for the box.
	// first point has position 0, getMax[x,y,z]Position therefore returns number_of_points
	template <class ValueType>
	BALL_INLINE 
	Size TRegularData3D<ValueType>::getMaxXIndex() const throw()
	{
		return size_.x - 1;
	}

	template <class ValueType>
	BALL_INLINE 
	Size TRegularData3D<ValueType>::getMaxYIndex() const throw()
	{
		return size_.y - 1;
	}

	// getSize() returns the total number of grid points
	template <class ValueType>
	BALL_INLINE 
	Size TRegularData3D<ValueType>::size() const throw()
	{
		return number_of_grid_points_;
	}

	template <class ValueType>
	typename TRegularData3D<ValueType>::IndexType TRegularData3D<ValueType>::getIndex(const Vector3& r) const 
		throw(Exception::OutOfGrid)
	{
		return getIndex(r.x, r.y, r.z);
	}

	template <class ValueType>
	void TRegularData3D<ValueType>::setOrigin(const Vector3& origin) 	throw()
	{
		Vector3 diff(upper_ - origin_);
		origin_ = origin;
		upper_ = origin_ + diff;
	}

	template <class ValueType>
	void TRegularData3D<ValueType>::setOrigin(float x, float y, float z) 
	 	throw()
	{
		Vector3 diff(upper_ - origin_);
		origin_.set(x, y, z);
		upper_ = origin_ + diff;
	}

	template <class ValueType> 
	BALL_INLINE
	bool TRegularData3D<ValueType>::has(float x, float y, float z) const		
		throw()
	{
		if (x > upper_.x  ||	y > upper_.y  ||	z > upper_.z  ||
				x < origin_.x ||	y < origin_.y ||	z < origin_.z ||
				!valid_)
		{
			return false;
		}		
		return true;
	}

	template <class ValueType> 
	BALL_INLINE
	bool TRegularData3D<ValueType>::has(const Vector3& vector) const
		throw()
	{
		if (vector.x > upper_.x  ||	vector.y > upper_.y  ||	vector.z > upper_.z  ||
				vector.x < origin_.x ||	vector.y < origin_.y ||	vector.z < origin_.z ||
				!valid_)
		{
			return false;
		}		
		return true;
	}

	template <class ValueType>
	BALL_INLINE 
	typename TRegularData3D<ValueType>::IndexType TRegularData3D<ValueType>::getIndex
		(float x, float y, float z) const 
		throw(Exception::OutOfGrid)
	{
		if (!has(x, y, z))
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		}				
		
		IndexType	position;

		position.x = (Position) ((x - origin_.x) / spacing_.x + 0.5);
		position.y = (Position) ((y - origin_.y) / spacing_.y + 0.5);
		position.z = (Position) ((z - origin_.z) / spacing_.z + 0.5);

		return position;
	}

	template <class ValueType>
	BALL_INLINE 
	ValueType* TRegularData3D<ValueType>::getData
		(Position i, Position j, Position k) 
		throw(Exception::OutOfGrid)
	{
		if (i > size_.x 
			|| j > size_.y 
			|| k > size_.z
			|| !valid_)
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		}		

		return &(data[i + j * size_.x 
				            + k * size_.x 
												* size_.y]);
	}

	template <class ValueType>
	BALL_INLINE 
	ValueType* TRegularData3D<ValueType>::getData(const Vector3& r) 
		throw(Exception::OutOfGrid)
	{	
		IndexType	position = getIndex(r);
		return getData(position.x, position.y, position.z);
	}

	template <class ValueType>
	BALL_INLINE 
	ValueType* TRegularData3D<ValueType>::getData(Position position) 
		throw(Exception::OutOfGrid)
	{
		if (position >= number_of_grid_points_)
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		} 
		return &(data[position]);
	}

	template <class ValueType>
	ValueType& TRegularData3D<ValueType>::operator [] (Position position)
		throw(Exception::OutOfGrid)
	{
		return *(getData(position));
	}

	template <class ValueType>
	ValueType& TRegularData3D<ValueType>::operator [] (const Vector3& v)
		throw(Exception::OutOfGrid)
	{
		return *(getData(v));
	}

	template <class ValueType>
	BALL_INLINE 
	Vector3 TRegularData3D<ValueType>::getCoordinates
		(Position i, Position j, Position k) const 
		throw(Exception::OutOfGrid)
	{
		if (i > size_.x ||	j > size_.y ||	k > size_.z)
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		}		
		Vector3	r;
		
		r.set(origin_.x + i * spacing_.x,
					origin_.y + j * spacing_.y,
					origin_.z + k * spacing_.z);

		return r;
	}

	template <class ValueType>
	BALL_INLINE 
	Vector3 TRegularData3D<ValueType>::getCoordinates(const Vector3& v) const 
		throw(Exception::OutOfGrid)
	{
		if (v.x < 0 || v.y < 0 || v.z < 0 )
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		}
		
		return getCoordinates((Position)((v.x - origin_.x) / spacing_.x), 
															(Position)((v.y - origin_.y) / spacing_.y), 
															(Position)((v.z - origin_.z) / spacing_.z));
	}

	template <class ValueType> 
	BALL_INLINE 
	Vector3 TRegularData3D<ValueType>::getCoordinates(Position position) const 
		throw(Exception::OutOfGrid)
	{
		if (position > number_of_grid_points_)
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		} 
		Vector3	r;
		Position	x, y, z;
		
		x = position % size_.x;
		y = (position % (size_.x * size_.y)) / size_.x;
		z =  position / (size_.x * size_.y);

		r.set(origin_.x + (float)x * spacing_.x,
					origin_.y + (float)y * spacing_.y,
					origin_.z + (float)z * spacing_.z);

		return r;
	}

	template <typename ValueType>
	BALL_INLINE
	void TRegularData3D<ValueType>::getBoxIndices
		(const Vector3& vector,
		Position& llf, Position& rlf, Position& luf, Position& ruf,
		Position& llb, Position& rlb, Position& lub, Position& rub) const
		throw(Exception::OutOfGrid)
	{
		if (!has(vector))
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		}		
		// calculate the grid indices of the lower left front corner
		// of the enclosing box
		IndexType position;
		position.x = (Position)((vector.x - origin_.x) / spacing_.x);
		position.y = (Position)((vector.y - origin_.y) / spacing_.y);
		position.z = (Position)((vector.z - origin_.z) / spacing_.z);
		
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
	void TRegularData3D<ValueType>::getBoxData
		(const Vector3& vector,
		ValueType& llf, ValueType& rlf, ValueType& luf, ValueType& ruf,
		ValueType& llb, ValueType& rlb, ValueType& lub, ValueType& rub) const
		throw(Exception::OutOfGrid)
	{
		if (!has(vector))
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		}		
		
		// compute the eight grid indices forming the enclosing box
		Position llf_idx, rlf_idx, luf_idx, ruf_idx, llb_idx, rlb_idx, lub_idx, rub_idx;
		getBoxIndices(vector, llf_idx, rlf_idx, luf_idx, ruf_idx, llb_idx, rlb_idx, lub_idx, rub_idx);
				
		// retrieve the grid values
		llf = data[llf_idx];
		rlf = data[rlf_idx];
		luf = data[luf_idx];
		ruf = data[ruf_idx];
		llb = data[llb_idx];
		rlb = data[rlb_idx];
		lub = data[lub_idx];
		rub = data[rub_idx];
	}

	template <typename ValueType>
	BALL_INLINE
	ValueType TRegularData3D<ValueType>::getInterpolatedValue(const Vector3& vector) const
		throw(Exception::OutOfGrid)
	{
		if (!has(vector))
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		}		

		Vector3 h(vector - origin_);
		Position x = (int)(h.x / spacing_.x);
		Position y = (int)(h.y / spacing_.y);
		Position z = (int)(h.z / spacing_.z);

		unsigned long Nx = size_.x;
		unsigned long Nxy = size_.z * Nx;
		unsigned long l = x + Nx * y + Nxy * z;
		Vector3 r_0 = getCoordinates((Position)l);

		float dx = 1 - ((vector.x - r_0.x) / spacing_.x);
		float dy = 1 - ((vector.y - r_0.y) / spacing_.y);
		float dz = 1 - ((vector.z - r_0.z) / spacing_.z);

		return  data[l] * dx * dy * dz
					+ data[l + 1] * (1 - dx) * dy * dz
					+ data[l + Nx] * dx * (1 - dy) * dz
					+ data[l + Nx + 1] * (1 - dx) * (1 - dy) * dz
					+ data[l + Nxy] * dx * dy * (1 - dz)
					+ data[l + Nxy + 1] * (1 - dx) * dy * (1 - dz)
					+ data[l + Nxy + Nx] * dx * (1 - dy) * (1 - dz)
					+ data[l + Nxy + Nx + 1] * (1 - dx) * (1 - dy) * (1 - dz);
	}

	template <typename ValueType>
	void TRegularData3D<ValueType>::clear() throw()
	{
		if (data != 0)
		{
			delete [] data;
			data = 0;
		}

		origin_ = dimension_ = spacing_ =	upper_ = Vector3(0, 0, 0);

		size_.x =
		size_.y =
		size_.z =
		number_of_grid_points_ = 0;

		valid_ = false;
	}

	template <typename ValueType>	
	bool TRegularData3D<ValueType>::operator == (const TRegularData3D<ValueType>& grid) const 
		throw()
	{
		if (size_.x != grid.size_.x || size_.y != grid.size_.y || size_.z != grid.size_.z 
				|| origin_ != grid.origin_ || spacing_ != grid.spacing_)
		{
			return false;
		}

		for (Position pos = 0; pos < number_of_grid_points_; pos++)
		{
			if (data[pos] != grid.data[pos])
			{
				return false;
			}
		}

		return true;
	}	
 } // namespace BALL

#endif // BALL_DATATYPE_REGULARDATA3D_H
