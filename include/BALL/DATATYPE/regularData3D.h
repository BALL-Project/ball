// $Id: regularData3D.h,v 1.4 2001/05/10 17:22:44 oliver Exp $ 

#ifndef BALL_DATATYPE_REGULARDATA3D_H
#define BALL_DATATYPE_REGULARDATA3D_H

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

namespace BALL 
{

	/**	Simple 3D grid class.
			Unlike BoxGrid this class represents a three-dimensional
			array. An instance of GridDataType will be created
			for each point of the grid upon instantiation of RegularData3D.\\
			{\bf Definition:}\\
			\URL{BALL/DATATYPE/RegularData3D.h}	\\
			@see	HashGrid3
	*/
	template <typename GridDataType>
	class RegularData3D 
	{
		public:

		BALL_CREATE_DEEP(RegularData3D<GridDataType>)

		/**	@name	Type Definitions
		*/
		//@{

		/**	PositionStruct
				RegularData3D position type.
				This struct is used to represent the indices of a
				grid point.
				@see		RegularData3D
				@see		RegularData3D::getPosition
		*/
		struct PositionStruct 
		{
			///
			Position x;
			///
			Position y;
			///
			Position z;
		};

		/**	@name	Position
				@memo Grid position type
		*/
		typedef struct PositionStruct GridIndex;

		//@}
		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				Creates a RegularData3D object without allocating a grid.
				The instance is not valid.
		*/
		RegularData3D() throw();	

		/**	Copy constructor.
				Creates a copy of an existing RegularData3D object.
				@param grid the grid to be copied
				@param bool ignored
		*/
		RegularData3D(const RegularData3D<GridDataType>& grid, bool deep = true)
			throw(Exception::OutOfMemory);	

		/**	Constructor for RegularData3D.
				{\em lower_[x,y,z]} should be set to the coordinates of
				the "lower" corner of the box represented by the grid
				{\em upper_[x,y,z]} should likewise contain the "upper" corner
				In fact, it doesn't really matter which coordinates are
				which, as this method always takes the lowest coordinates (x,y,z)
				for the lower corner and the highest coordinates for
				the upper corner.\\
				{\em grid_points_[x,y,z]} gives the number of grid points in 
				either direction. 
				@param	lower_x	float, the x coordinate of the lower corner of the grid
				@param	lower_y	float, the y coordinate of the lower corner of the grid
				@param	lower_z	float, the z coordinate of the lower corner of the grid
				@param	upper_x	float, the x coordinate of the upper corner of the grid
				@param	upper_y	float, the y coordinate of the upper corner of the grid
				@param	upper_z	float, the z coordinate of the upper corner of the grid
		*/
		RegularData3D
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
		RegularData3D
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
		RegularData3D(const Vector3& lower, const Vector3& upper, float spacing)
			throw(Exception::OutOfMemory);

		/**	Destructor. 
				Frees all allocated memory.
		*/
		virtual ~RegularData3D() throw()
		{
			delete [] data;
		}

		/** Clear method.
				Frees all allocated memory.
				The instance is set to not valid.
		*/
		virtual void clear() throw();

		//@}
		/**	@name Assignment
		*/
		//@{

		/**	Copy the contents of another grid.
				Replaces the contents and dimensions of the current
				grid with those of {\tt grid}. The previous content
				is deleted and memory is freed.
				If copying the grid fails (e.g., due to insufficient memory),
				\Ref{isValid} returns {\bf false} after this operation.
		*/
		void set(const RegularData3D& grid) throw(Exception::OutOfMemory);

		/**	Assignment operator.
				Implemented using \Ref{set}.
				@see set
		*/
		const RegularData3D& operator = (const RegularData3D& grid) throw(Exception::OutOfMemory);

		//@}
		/**	@name	Debugging and Diagnostics
		*/
		//@{

		/** Internal state dump.
				Dump the current internal state of {\em *this} RegularData3D to 
				the output ostream {\em s}.
				@param   s - output stream where to output the internal state of {\em *this}
		*/
		virtual void dump(std::ostream& stream) const throw(); 

		/**	Returns the current stat of the object.	
				isValid() returns false, if the grid couldn't be initialized 
				correctly. Usually this is due to an allocation error in one
				of the constructors.
		*/
		bool isValid() const throw();

		//@}
		/**	@name	Accessors
		*/
		//@{

		/**	Returns the largest possible x coordinate for the box.
		*/
		float getMaxX() const throw();

		/**	Returns the largest possible y coordinate for the box.
		*/
		float getMaxY() const throw();

		/**	Returns the largest possible z coordinate for the box.
		*/
		float getMaxZ() const throw();

		/**	Returns the x coordinate of the grid origin.
		*/
		float getMinX() const throw();

		/**	Returns the y coordinate of the grid origin.
		*/
		float getMinY() const throw();

		/**	Returns the z coordinate of the grid origin.
		*/
		float getMinZ() const throw();

		/**	Return the largest grid position for the x direction.
				This method returns the maximum position allowed in the grid.
				As the point in the origin has the indices (0, 0, 0), this
				method returns the number of points in X direction minus one.
		*/
		Size getMaxXIndex() const throw();

		/**	Return the largest grid position for the y direction.
				This method returns the maximum position allowed in the grid.
				As the point in the origin has the indices (0, 0, 0), this
				method returns the number of points in Y direction minus one.
		*/
		Size getMaxYIndex() const throw();

		/**	Return the largest grid position for the z direction.
				This method returns the maximum position allowed in the grid.
				As the point in the origin has the indices (0, 0, 0), this
				method returns the number of points in Z direction minus one.
		*/
		Size getMaxZIndex() const throw();

		/**	Returns the total number of grid points.
		*/
		Size getSize() const throw();

		/**	Returns the grid spacing in x direction.
		*/	
		float getXSpacing() const throw();

		/**	Returns the grid spacing in y direction.
		*/	
		float getYSpacing() const throw();

		/**	Returns the grid spacing in z direction.
		*/	
		float getZSpacing() const throw();

		/**	Returns the position of the grid point closest to the given vector.
				If there are multiple grid points with equal distance, the
				grid point with the lowest indices in x, y, and z direction is
				returned.
				@exception OutOfGrid if the point is outside the grid
		*/
		GridIndex getIndex(const Vector3&) const throw(Exception::OutOfGrid);

		/**	Returns the position of the grid point closest to three given coordinates.
				If there are multiple grid points with equal distance, the
				grid point with the lowest indices in x, y, and z direction is
				returned.
				@exception OutOfGrid if the point is outside the grid
		*/
		GridIndex getIndex(float x, float y, float z) const
			throw(Exception::OutOfGrid);
		
		/**	Returns a pointer to the grid contents determined by the three indices.
				@exception OutOfGrid if the point is outside the grid
		*/
		GridDataType* getData(const Position i, const Position j, const Position k)
		  throw(Exception::OutOfGrid);

		/**	Returns a pointer to the grid contents closest to a given vector.
				Determination of the selected grid point is made via getPosition.
				@exception OutOfGrid if the point is outside the grid
		*/
		GridDataType* getData(const Vector3& r) throw(Exception::OutOfGrid);

		/**	Returns a pointer to the grid contents determined by the position.
				@exception OutOfGrid if the point is outside the grid
		*/
		GridDataType* getData(const Position position) throw(Exception::OutOfGrid);

		/**	Subscript operator.
				Returns the data of the grid point specified by its {\tt position}.
				@return		GridDataType
				@exception OutOfGrid if the point is outside the grid
				@param		position Position, the grid position
				@see			getData
		*/
		GridDataType& operator[](const Position position) throw(Exception::OutOfGrid);

		/**	Subscript operator.
				Returns the data of the grid point nearest to the given
				{\tt vector}
				@return		GridDataType
				@exception OutOfGrid if the point is outside the grid
				@param		vector Vector3, a position in the grid
				@see			getData
		*/
		GridDataType& operator[](const Vector3& vector) throw(Exception::OutOfGrid);

		/**	Returns the exact coordinates of a grid point.	
				@return		Vector3
				@exception OutOfGrid if the point is outside the grid
				@param		i Position, grid x position
				@param		j Position, grid y position
				@param		k Position, grid z position
		*/
		Vector3 getGridCoordinates(const Position i, const Position j, const Position k) const
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
		Vector3 getGridCoordinates(const Vector3& r) const throw(Exception::OutOfGrid);

		/**	Returns the exact coordinates of a grid point.	
				@return		Vector3
				@param		Position
		*/
		Vector3 getGridCoordinates(const Position position) const throw(Exception::OutOfGrid);

		/**	Return the indices of the grid points of the enclosing box.
				This method calculates the grid box that contains the given vector
				and returns the indices of the grid points forming this box.
				The given point lies either in the box or is the lower left front edge of the box.
				@return bool {\bf true} if the vector is inside the grid
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
				@return bool {\bf true} if the vector is inside the grid
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
			GridDataType& llf, GridDataType& rlf, GridDataType& luf, GridDataType& ruf,
			GridDataType& llb, GridDataType& rlb, GridDataType& lub, GridDataType& rub) const
			throw(Exception::OutOfGrid);
													
		/**	Returns a vector to the grid's origin.
				@return		Vector3& the grid origin
		*/
		Vector3& getOrigin() throw();

		/**	Returns a vector to the grid's origin (const method).
				@return		Vector3&
		*/
		const Vector3& getOrigin() const throw();

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
		Vector3& getDimension() throw();
		
		/**	Returns a vector containing the grid's dimensions (const method).
				@return		Vector3&
		*/
		const Vector3& getDimension() const throw();

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
						{\mathrm value} & = & v_1 dx dy dz\\
														& + & v_2 (1 - dx) dy dz\\
														& + & v_3 dx (1 - dy) dz\\
														& + & v_4 (1 - dx) (1 - dy) dz\\
														& + & v_5 dx dy (1 - dz)\\
														& + & v_6 (1 - dx) dy (1 - dz)\\
														& + & v_7 dx (1 - dy) (1 - dz)\\
														& + & v_8 (1 - dx) (1 - dy) (1 - dz)\\
					\end{eqnarray*}
				}
				@exception OutOfGrid if the point is outside the grid
				@param	vector the position to evaluate
		*/
		GridDataType getInterpolatedValue(const Vector3& vector) const throw(Exception::OutOfGrid);
		
		/** Equality operator.
				Two point grids are equal if they have the same number of points in all three
				dimensions, same origin, spacing  and the data fields are equal.
				Both grids have to be valid or false is returned.
		*/
		bool operator == (const RegularData3D<GridDataType>& grid) const throw();

		/** Inequality operator.
				@see operator ==
		*/
		bool operator != (const RegularData3D<GridDataType>& grid) const throw();

		//@}

		/**	The grid data
		*/
		GridDataType* data;

		protected:
			
		/*_ origin of the box (offset) 
		*/	
		Vector3	origin_;

		/*_ size of the box 
		*/
		Vector3	size_;

		/*_ distance grid spacing (spacing = size / (number_of_points - 1))
		*/
		Vector3 spacing_;

		/*_ number of grid points of the box
		*/
		Size number_of_points_x_, number_of_points_y_, number_of_points_z_;

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

	// default constructor.
	template <class GridDataType>
	RegularData3D<GridDataType>::RegularData3D()
		throw()
		: data(0),
			origin_(0,0,0),
			size_(0,0,0),
			spacing_(0,0,0),
			number_of_points_x_(0),
			number_of_points_y_(0),
			number_of_points_z_(0),
			number_of_grid_points_(0),
			upper_(0,0,0),
			valid_(false)
	{
	}

	// copy constructor
	template <class GridDataType>
	RegularData3D<GridDataType>::RegularData3D
		(const RegularData3D<GridDataType>& grid, bool /* deep */)
		throw(Exception::OutOfMemory)
		: data(0),
			origin_(0,0,0),
			size_(0,0,0),
			spacing_(0,0,0),
			number_of_points_x_(0),
			number_of_points_y_(0),
			number_of_points_z_(0),
			number_of_grid_points_(0),
			upper_(0,0,0),
			valid_(false)
	{
		set(grid);
	}

	// assignment operator
	template <typename GridDataType>
	BALL_INLINE
	const RegularData3D<GridDataType>& RegularData3D<GridDataType>::operator = 
		(const RegularData3D<GridDataType>& grid)
		throw(Exception::OutOfMemory)
	{
		set(grid);
		return *this;
	}

	// set method
	template <typename GridDataType>
	BALL_INLINE
	void RegularData3D<GridDataType>::set(const RegularData3D<GridDataType>& grid)
		throw(Exception::OutOfMemory)
	{
		// throw away the old data 
		if (data != 0)
		{
			delete [] data;
		}

		// create a new array to hold the contents of the grid
		data = new GridDataType[grid.number_of_grid_points_];

		// if the alloc failed, mark this instance as invalid
		valid_ = (data != 0);
		
		if (!valid_)
		{
			throw Exception::OutOfMemory(__FILE__, __LINE__, 
																	 grid.number_of_grid_points_ * sizeof(GridDataType));
		}

		// copy the remaining attributes
		origin_ = grid.getOrigin();
		size_ = grid.getDimension();
		spacing_.set(grid.getXSpacing(), grid.getYSpacing(), grid.getZSpacing());
		number_of_points_x_ = grid.number_of_points_x_;
		number_of_points_y_ = grid.number_of_points_y_;
		number_of_points_z_ = grid.number_of_points_z_;
		number_of_grid_points_ = grid.number_of_grid_points_;
		upper_ = grid.upper_;

		// copy the contents of grid (if enough memory could be allocated)
		if (valid_)
		{
			for (Position i = 0; i < number_of_grid_points_; i++)
			{
				data[i] = grid.data[i];
			}
		}
  }

	//  First constructor for RegularData3D
	//  lower_[x,y,z] should be set to the coordinates of
	//  the "lower" corner of the box represented by the grid
	//  upper_[x,y,z] should likewise contain the "upper" corner
	//  In fact, it doesn't really matter which coordinates are
	//  which, as it always takes the lowest coordinates (x,y,z)
	//  for the lower corner and the highest coordinates for
	//  the upper corner.
	//  grid_points_[x,y,z] gives the number of grid points in
	//  either direction.
	template <class GridDataType>
	BALL_INLINE
	RegularData3D<GridDataType>::RegularData3D
		(float lower_x, float lower_y, float lower_z,
		 float upper_x, float upper_y, float upper_z,
		 Size grid_points_x, Size grid_points_y, Size grid_points_z)
		throw(Exception::OutOfMemory)
	{

		// set data and number_of_grid_points_ to 0/0, just to be sure 
		data = (GridDataType*) 0;
		number_of_grid_points_ = 0;

		// set the number of grid points in all directions
		number_of_points_x_ = grid_points_x;
		number_of_points_y_ = grid_points_y;
		number_of_points_z_ = grid_points_z;

		// if the number of grid points in any direction is below 2
		// (which means that the grid is not three-dimensional!)
		// then increase to at least one point in each dimension
		if (number_of_points_x_ < 2)
		{
			number_of_points_x_ = 2;
		}
		if (number_of_points_y_ < 2)
		{
			number_of_points_y_ = 2;
		}
		if (number_of_points_y_ < 2)
		{
			number_of_points_y_ = 2;
		}

		// calculate the origin as the lowest given coordinates
		// of each direction
		origin_.x = ( lower_x < upper_x ) ? lower_x : upper_x;
		origin_.y = ( lower_y < upper_y ) ? lower_y : upper_y;
		origin_.z = ( lower_z < upper_z ) ? lower_z : upper_z;

		// calculate the box sizes
		size_.x = fabs(upper_x - lower_x);
		size_.y = fabs(upper_y - lower_y);
		size_.z = fabs(upper_z - lower_z);

		// calculate the topmost edge
		upper_.set(origin_.x + size_.x, origin_.y + size_.y, origin_.z + size_.z); 

		// calculate the grid spacing in all directions
		spacing_.x = size_.x / (number_of_points_x_ - 1);
		spacing_.y = size_.y / (number_of_points_y_ - 1);
		spacing_.z = size_.z / (number_of_points_z_ - 1);
		
		// calculate the total number of grid points
		number_of_grid_points_ = number_of_points_x_ * number_of_points_y_ * number_of_points_z_;
		
		// allocate space for the array containing pointers to the objects
		data = new GridDataType[number_of_grid_points_];

		// mark this instance as invalid if the alloc failed
		valid_ = (data != 0);			

		if (!valid_)
		{
			throw Exception::OutOfMemory(__FILE__, __LINE__, 
																	 number_of_grid_points_ * sizeof(GridDataType));
		}
	}

	template <class GridDataType>
	RegularData3D<GridDataType>::RegularData3D
		(const Vector3& lower, const Vector3& upper,
		 Size grid_points_x, Size grid_points_y, Size grid_points_z) 
		throw(Exception::OutOfMemory)
		: data(0)
	{
		*this = RegularData3D(lower.x, lower.y, lower.z, upper.x, upper.y, upper.z,
											grid_points_x, grid_points_y, grid_points_z);
	}

	template <class GridDataType>
	RegularData3D<GridDataType>::RegularData3D
		(const Vector3& lower,
		 const Vector3& upper,
		 float spacing) 
		throw(Exception::OutOfMemory)
		: data(0)
	{
		*this = RegularData3D(lower.x, lower.y, lower.z, upper.x, upper.y, upper.z,
											(Size)((upper.x - lower.x) / spacing + 1), 
											(Size)((upper.y - lower.y) / spacing + 1), 
											(Size)((upper.z - lower.z) / spacing + 1));
	}
			
	template <class GridDataType>
	void RegularData3D<GridDataType>::dump(std::ostream& stream) const throw()
	{
		stream << "Dump of " << typeid(this).name()<< " (" << getMaxXIndex() 
					 << "x" << getMaxYIndex() << "x" << getMaxZIndex() 
					 << ")" << std::endl;
		stream << "--------------------------------------" << std::endl;
		for (Position i = 0; i < number_of_grid_points_; i++)
		{
			stream << "(" << data[i] << ")";
		}
		stream << std::endl << "--------------------------------------" << std::endl;
	}

	// returns the state of this instance
	template <class GridDataType>
	BALL_INLINE 
	bool RegularData3D<GridDataType>::isValid() const throw()
	{
		return valid_;
	}

	// getMax[x,y,z] returns the maximum possible coordinates for
	// the box, i.e. origin + size
	template <class GridDataType>
	BALL_INLINE 
	float RegularData3D<GridDataType>::getMaxX() const throw()
	{
		return upper_.x;
	}

	template <class GridDataType>
	BALL_INLINE 
	float RegularData3D<GridDataType>::getMaxY() const throw()
	{
		return upper_.y;
	}

	template <class GridDataType>
	BALL_INLINE 
	float RegularData3D<GridDataType>::getMaxZ() const throw()
	{
		return upper_.x;
	}

	// getMax[x,y,z] returns the maximum possible coordinates for
	// the box, i.e. origin + size
	template <class GridDataType>
	BALL_INLINE 
	float RegularData3D<GridDataType>::getMinX() const throw()
	{
		return origin_.x;
	}

	template <class GridDataType>
	BALL_INLINE 
	float RegularData3D<GridDataType>::getMinY() const throw()
	{
		return origin_.y;
	}

	template <class GridDataType>
	BALL_INLINE 
	float RegularData3D<GridDataType>::getMinZ() const throw()
	{
		return origin_.z;
	}

	// getMax[x,y,z]Position returns the maximum grid position for the box.
	// first point has position 0, getMax[x,y,z]Position therefore returns number_of_points
	template <class GridDataType>
	BALL_INLINE 
	Size RegularData3D<GridDataType>::getMaxXIndex() const throw()
	{
		return number_of_points_x_ - 1;
	}

	template <class GridDataType>
	BALL_INLINE 
	Size RegularData3D<GridDataType>::getMaxYIndex() const throw()
	{
		return number_of_points_y_ - 1;
	}

	template <class GridDataType>
	BALL_INLINE 
	Size RegularData3D<GridDataType>::getMaxZIndex() const throw()
	{
		return number_of_points_z_ - 1;
	}

	// getSize() returns the total number of grid points
	template <class GridDataType>
	BALL_INLINE 
	Size RegularData3D<GridDataType>::getSize() const throw()
	{
		return number_of_grid_points_;
	}

	template <class GridDataType>
	RegularData3D<GridDataType>::GridIndex RegularData3D<GridDataType>::getIndex(const Vector3& r) const 
		throw(Exception::OutOfGrid)
	{
		return getIndex(r.x, r.y, r.z);
	}

	// get[x,y,z]Spacing returns the grid spacing, i.e. the distance
	// between two grid points in the given direction
	template <class GridDataType>
	BALL_INLINE 
	float RegularData3D<GridDataType>::getXSpacing() const  	throw()
	{
		return spacing_.x;
	}

	template <class GridDataType>
	BALL_INLINE 
	float RegularData3D<GridDataType>::getYSpacing() const  	throw()
	{
		return spacing_.y;
	}

	template <class GridDataType>
	BALL_INLINE 
	float RegularData3D<GridDataType>::getZSpacing() const  	throw()
	{
		return spacing_.z;
	}

	template <class GridDataType> 
	BALL_INLINE
	const Vector3& RegularData3D<GridDataType>::getOrigin() const  	throw()
	{
		return origin_;
	}

	template <class GridDataType> 
	BALL_INLINE
	Vector3& RegularData3D<GridDataType>::getOrigin()  	throw()
	{
		return origin_;
	}

	template <class GridDataType>
	void RegularData3D<GridDataType>::setOrigin(const Vector3& origin) 	throw()
	{
		origin_ = origin;
	}

	template <class GridDataType>
	void RegularData3D<GridDataType>::setOrigin(float x, float y, float z) 
	 	throw()
	{
		origin_.set(x, y, z);
	}

	template <class GridDataType> 
	const Vector3& RegularData3D<GridDataType>::getDimension() const  throw()
	{
		return size_;
	}

	template <class GridDataType> 
	Vector3& RegularData3D<GridDataType>::getDimension() 	throw()
	{
		return size_;
	}

	template <class GridDataType> 
	BALL_INLINE
	bool RegularData3D<GridDataType>::has(float x, float y, float z) const		
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

	template <class GridDataType> 
	BALL_INLINE
	bool RegularData3D<GridDataType>::has(const Vector3& vector) const
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

	template <class GridDataType>
	BALL_INLINE 
	RegularData3D<GridDataType>::GridIndex RegularData3D<GridDataType>::getIndex
		(float x, float y, float z) const 
		throw(Exception::OutOfGrid)
	{
		if (!has(x, y, z))
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		}				
		
		GridIndex	position;

		position.x = (Position) ((x - origin_.x) / spacing_.x + 0.5);
		position.y = (Position) ((y - origin_.y) / spacing_.y + 0.5);
		position.z = (Position) ((z - origin_.z) / spacing_.z + 0.5);

		return position;
	}

	template <class GridDataType>
	BALL_INLINE 
	GridDataType* RegularData3D<GridDataType>::getData
		(const Position i, const Position j, const Position k) 
		throw(Exception::OutOfGrid)
	{
		if (i > number_of_points_x_ ||	j > number_of_points_y_ ||	k > number_of_points_z_
				|| !valid_)
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		}		

		return &(data[i + j * number_of_points_x_ + k 
												* number_of_points_x_ 
												* number_of_points_y_]);
	}

	template <class GridDataType>
	BALL_INLINE 
	GridDataType* RegularData3D<GridDataType>::getData(const Vector3& r) 
		throw(Exception::OutOfGrid)
	{	
		GridIndex	position = getIndex(r);
		return getData(position.x, position.y, position.z);
	}

	template <class GridDataType>
	BALL_INLINE 
	GridDataType* RegularData3D<GridDataType>::getData(const Position position) 
		throw(Exception::OutOfGrid)
	{
		if (position > number_of_grid_points_)
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		} 
		return &(data[position]);
	}

	template <class GridDataType>
	GridDataType& RegularData3D<GridDataType>::operator[] (const Position position)
		throw(Exception::OutOfGrid)
	{
		if (position > number_of_grid_points_)
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		} 
		return *(getData(position));
	}

	template <class GridDataType>
	GridDataType& RegularData3D<GridDataType>::operator[](const Vector3& v)
		throw(Exception::OutOfGrid)
	{
		return *(getData(v));
	}

	template <class GridDataType>
	BALL_INLINE 
	Vector3 RegularData3D<GridDataType>::getGridCoordinates
		(const Position i, const Position j, const Position k) const 
		throw(Exception::OutOfGrid)
	{
		if (i > number_of_points_x_ ||	j > number_of_points_y_ ||	k > number_of_points_z_)
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		}		
		Vector3	r;
		
		r.set(origin_.x + i * spacing_.x,
					origin_.y + j * spacing_.y,
					origin_.z + k * spacing_.z);

		return r;
	}

	template <class GridDataType>
	BALL_INLINE 
	Vector3 RegularData3D<GridDataType>::getGridCoordinates(const Vector3& v) const 
		throw(Exception::OutOfGrid)
	{
		if (v.x < 0 || v.y < 0 || v.z < 0 )
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		}
		return getGridCoordinates(v.x, v.y, v.z);
	}

	template <class GridDataType> 
	BALL_INLINE 
	Vector3 RegularData3D<GridDataType>::getGridCoordinates(const Position position) const 
		throw(Exception::OutOfGrid)
	{
		if (position > number_of_grid_points_)
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		} 
		Vector3	r;
		Position	x, y, z;
		
		x = position % number_of_points_x_;
		y = (position % (number_of_points_x_ * number_of_points_y_)) / number_of_points_x_;
		z =  position / (number_of_points_x_ * number_of_points_y_);

		r.set(origin_.x + (float)x * spacing_.x,
					origin_.y + (float)y * spacing_.y,
					origin_.z + (float)z * spacing_.z);

		return r;
	}

	template <typename GridDataType>
	BALL_INLINE
	void RegularData3D<GridDataType>::getBoxIndices
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
		GridIndex position;
		position.x = (Position)((vector.x - origin_.x) / spacing_.x);
		position.y = (Position)((vector.y - origin_.y) / spacing_.y);
		position.z = (Position)((vector.z - origin_.z) / spacing_.z);
		
		// calculate the (linear) indices of the eight
		// box corners
		llf = position.x + number_of_points_x_ * position.y 
				+ number_of_points_x_ * number_of_points_y_ * position.z;
		rlf = llf + 1;
		luf = llf + number_of_points_x_;
		ruf = luf + 1;
		llb = llf + number_of_points_x_ * number_of_points_y_;
		rlb = llb + 1;
		lub = llb + number_of_points_x_;
		rub = lub + 1;
	}

	template <typename GridDataType>
	BALL_INLINE
	void RegularData3D<GridDataType>::getBoxData
		(const Vector3& vector,
		GridDataType& llf, GridDataType& rlf, GridDataType& luf, GridDataType& ruf,
		GridDataType& llb, GridDataType& rlb, GridDataType& lub, GridDataType& rub) const
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

	template <typename GridDataType>
	BALL_INLINE
	GridDataType RegularData3D<GridDataType>::getInterpolatedValue(const Vector3& vector) const
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

		unsigned long Nx = number_of_points_x_;
		unsigned long Nxy = number_of_points_z_ * Nx;
		unsigned long l = x + Nx * y + Nxy * z;
		Vector3 r_0 = getGridCoordinates((Position)l);

		float dx = 1 - ((vector.x - r_0.x) / getXSpacing());
		float dy = 1 - ((vector.y - r_0.y) / getYSpacing());
		float dz = 1 - ((vector.z - r_0.z) / getZSpacing());

		return  data[l] * dx * dy * dz
					+ data[l + 1] * (1 - dx) * dy * dz
					+ data[l + Nx] * dx * (1 - dy) * dz
					+ data[l + Nx + 1] * (1 - dx) * (1 - dy) * dz
					+ data[l + Nxy] * dx * dy * (1 - dz)
					+ data[l + Nxy + 1] * (1 - dx) * dy * (1 - dz)
					+ data[l + Nxy + Nx] * dx * (1 - dy) * (1 - dz)
					+ data[l + Nxy + Nx + 1] * (1 - dx) * (1 - dy) * (1 - dz);
	}

	template <typename GridDataType>
	void RegularData3D<GridDataType>::clear() throw()
	{
		delete [] data;
		data = 0;

		origin_		=
		size_			=
		spacing_  =
		upper_		=	Vector3(0, 0, 0);

		number_of_points_x_ =
		number_of_points_y_ =
		number_of_points_z_ =
		number_of_grid_points_ = 0;

		valid_ = false;
	}

	template <typename GridDataType>	
	bool RegularData3D<GridDataType>::operator == (const RegularData3D<GridDataType>& grid) const 
		throw()
	{
		if (!valid_ || !grid.valid_ ||
				number_of_points_x_ != grid.number_of_points_x_ ||
				number_of_points_y_ != grid.number_of_points_y_ ||
				number_of_points_z_ != grid.number_of_points_z_ ||
				origin_							!= grid.origin_							||
				spacing_						!= grid.spacing_							)
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

	template <typename GridDataType>	
	bool RegularData3D<GridDataType>::operator != (const RegularData3D<GridDataType>& grid) const 
		throw()
	{
		return !(*this == grid);
	}

 } // namespace BALL

#endif // BALL_DATATYPE_REGULARDATA3D_H
