// $Id: regularData2D.h,v 1.13 2001/07/09 21:14:44 amoll Exp $

#ifndef BALL_DATATYPE_TRegularData2D_H
#define BALL_DATATYPE_TRegularData2D_H

#include <iostream>

#ifndef BALL_MATHS_Vector2_H
# include <BALL/MATHS/vector2.h>
#endif

using namespace std;

namespace BALL 
{

	/**	Simple 2D grid class.
			This class represents a two-dimensional array. 
			An instance of GridDataType will be created
			for each point of the grid upon instantiation of TRegularData2D.\\
			{\bf Definition:} \URL{BALL/DATATYPE/TRegularData2D.h}	\\
	*/
	template <typename GridDataType>
	class TRegularData2D 
	{
		public:

		BALL_CREATE_DEEP(TRegularData2D<GridDataType>)

		/**	@name	Type Definitions
		*/
		//@{

		/**	Grid position type
		*/
		typedef TVector2<Position> GridIndex;
		
		//@}
		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				Creates a TRegularData2D object without allocating a grid.
				The instance is not valid.
		*/
		TRegularData2D() throw();	

		/**	Copy constructor.
				Creates a copy of an existing TRegularData2D object.
				@param grid the grid to be copied
				@param bool ignored
		*/
		TRegularData2D(const TRegularData2D<GridDataType>& grid, bool deep = true)
			throw(Exception::OutOfMemory);	

		/**	Constructor for TRegularData2D.
				{\em lower_[x,y]} should be set to the coordinates of
				the "lower" corner of the rectangle represented by the grid
				{\em upper_[x,y]} should likewise contain the "upper" corner
				In fact, it doesn't really matter which coordinates are
				which, as this method always takes the lowest coordinates (x,y)
				for the lower corner and the highest coordinates for
				the upper corner.\\
				{\em grid_points_[x,y]} gives the number of grid points in 
				either direction. 
				@param	lower_x	float, the x coordinate of the lower corner of the grid
				@param	lower_y	float, the y coordinate of the lower corner of the grid
				@param	upper_x	float, the x coordinate of the upper corner of the grid
				@param	upper_y	float, the y coordinate of the upper corner of the grid
		*/
		TRegularData2D(float lower_x, float lower_y, 
									 float upper_x, float upper_y, 
									 Size grid_points_x, Size grid_points_y)
			 throw(Exception::OutOfMemory);

		/**	Constructor.
				The grid's origin is at lower, it has grid_points_[x,y]
				points in each direction.
		*/
		TRegularData2D
			(const Vector2& lower, const Vector2& upper,
			 Size grid_points_x, Size grid_points_y)
			 throw(Exception::OutOfMemory);

		/**	Constructor. 
				Takes lower and upper corners and the grid spacing
				The grid will include the coordinates given by upper/lower.
				Its origin is in the lower corner, it may extend up to spacing over
				the upper corner.
		*/
		TRegularData2D(const Vector2& lower, const Vector2& upper, float spacing)
			throw(Exception::OutOfMemory);

		/**	Destructor. 
				Frees all allocated memory.
		*/
		virtual ~TRegularData2D() throw()
		{
			delete [] data;
		}

		/** Clear method.
				Frees all allocated memory. The instance is set to not valid.
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
		void set(const TRegularData2D& grid) throw(Exception::OutOfMemory);

		/**	Assignment operator.
				Implemented using \Ref{set}.
				@see set
		*/
		const TRegularData2D& operator = (const TRegularData2D& grid) throw(Exception::OutOfMemory);

		//@}
		/**	@name	Debugging and Diagnostics
		*/
		//@{

		/** Internal state dump.
				Dump the current internal state of {\em *this} TRegularData2D to 
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

		/**	Returns the largest possible x coordinate for the rectangle.
		*/
		float getMaxX() const throw();

		/**	Returns the largest possible y coordinate for the rectangle.
		*/
		float getMaxY() const throw();


		/**	Returns the x coordinate of the grid origin.
		*/
		float getMinX() const throw();

		/**	Returns the y coordinate of the grid origin.
		*/
		float getMinY() const throw();

		/**	Return the largest grid position for the x direction.
				This method returns the maximum position allowed in the grid.
				As the point in the origin has the indices (0, 0), this
				method returns the number of points in X direction minus one.
		*/
		Size getMaxXIndex() const throw();

		/**	Return the largest grid position for the y direction.
				This method returns the maximum position allowed in the grid.
				As the point in the origin has the indices (0, 0), this
				method returns the number of points in Y direction minus one.
		*/
		Size getMaxYIndex() const throw();

		/**	Returns the total number of grid points.
		*/
		Size getSize() const throw();

		/**	Returns the grid spacing in x direction.
		*/	
		float getXSpacing() const throw();

		/**	Returns the grid spacing in y direction.
		*/	
		float getYSpacing() const throw();

		/**	Returns the position of the grid point closest to the given vector.
				If there are multiple grid points with equal distance, the
				grid point with the lowest indices in x, y direction is returned.
				@exception OutOfGrid if the point is outside the grid
		*/
		GridIndex getIndex(const Vector2& v) const throw(Exception::OutOfGrid);

		/**	Returns the position of the grid point closest to two given coordinates.
				If there are multiple grid points with equal distance, the
				grid point with the lowest indices in x, y direction is returned.
				@exception OutOfGrid if the point is outside the grid
		*/
		GridIndex getIndex(float x, float y) const
			throw(Exception::OutOfGrid);
		
		/**	Returns a pointer to the grid contents determined by the two indices.
				@exception OutOfGrid if the point is outside the grid
		*/
		GridDataType* getData(const Position i, const Position j)
		  throw(Exception::OutOfGrid);

		/**	Returns a pointer to the grid contents closest to a given vector.
				Determination of the selected grid point is made via getPosition.
				@exception OutOfGrid if the point is outside the grid
		*/
		GridDataType* getData(const Vector2& r) throw(Exception::OutOfGrid);

		/**	Returns a pointer to the grid contents determined by the position.
				@exception OutOfGrid if the point is outside the grid
		*/
		GridDataType* getData(const Position position) throw(Exception::OutOfGrid);

		/**	Subscript operator.
				Returns the data of the grid point specified by its {\tt position}.
				@return			GridDataType
				@exception 	OutOfGrid if the point is outside the grid
				@param			position Position, the grid position
				@see				getData
		*/
		GridDataType& operator[](const Position position) throw(Exception::OutOfGrid);

		/**	Subscript operator.
				Returns the data of the grid point nearest to the given {\tt vector}
				@return			GridDataType
				@exception 	OutOfGrid if the point is outside the grid
				@param			vector Vector2, a position in the grid
				@see				getData
		*/
		GridDataType& operator[](const Vector2& vector) throw(Exception::OutOfGrid);

		/**	Returns the exact coordinates of a grid point.	
				@return			Vector2	
				@exception 	OutOfGrid if the point is outside the grid
				@param			i Position, grid x position
				@param			j Position, grid y position
		*/
		Vector2 getGridCoordinates(const Position i, const Position j) const
		  throw(Exception::OutOfGrid);

		/**	Returns the exact coordinates of a grid point.	
				@return		Vector2	
				@param		Position
		*/
		Vector2 getGridCoordinates(const Position position) const throw(Exception::OutOfGrid);

		/**	Return the indices of the grid points of the enclosing rectangle.
				This method calculates the grid rectangle that contains the given vector
				and returns the indices of the grid points forming this rectangle.
				The given point lies either in the rectangle or is the lower left front edge of the rectangle.
				@return bool {\bf true} if the vector is inside the grid
				@exception OutOfGrid if the point is outside the grid
				@param vector a point inside the grid
				@param ll  left lower corner of the rectangle
				@param lr  right lower corner of the rectangle
				@param ul  left upper corner of the rectangle
				@param ur  right upper corner of the rectangle
		*/
		void getRectangleIndices
			(const Vector2& vector,	Position& ll, Position& lr, Position& ul, Position& ur) const
			throw(Exception::OutOfGrid);
													
		/**	Return the data at the grid points of the enclosing rectangle. (Const method)
				This method calculates the grid rectangle that contains the given vector
				and returns the values at the grid points forming this rectangle.
				The given point lies either in the rectangle or is the lower left front edge of the rectangle.
				@see getRectangleIndices
				@return bool {\bf true} if the vector is inside the grid
				@exception OutOfGrid if the point is outside the grid
				@param vector a point inside the grid
				@param ll  value at the left lower corner of the rectangle
				@param lr  value at the right lower corner of the rectangle
				@param ul  value at the left upper corner of the rectangle
				@param ur  value at the right upper corner of the rectangle
		*/
		void getRectangleData (const Vector2& vector,
			GridDataType& ll, GridDataType& lr, GridDataType& ul, GridDataType& ur) const
			throw(Exception::OutOfGrid);
													
		/**	Returns a vector to the grid's origin.
				@return		Vector2& the grid origin
		*/
		Vector2& getOrigin() throw();

		/**	Returns a vector to the grid's origin (const method).
				@return		Vector2&
		*/
		const Vector2& getOrigin() const throw();

		/**	Modifies the grid's origin
				@param		origin, new origin
		*/
		void setOrigin(const Vector2& origin) throw();

		/**	Modifies the grid's origin
				@param		x new origin x
				@param		y new origin y
		*/
		void setOrigin(float x, float y) throw();

		/**	Returns a Vector2 containing the grid's dimensions.
				@return		Vector2&
		*/
		Vector2& getDimension() throw();
		
		/**	Returns a Vector2 containing the grid's dimensions (const method).
				@return		Vector2&
		*/
		const Vector2& getDimension() const throw();

		/** Test if a given point is inside the grid.
				Also returns false if instance is not valid.
				@param vector the point
				@return bool
		*/
		bool has(const Vector2& vector) const throw();

		/** Test if a given point is inside the grid.
				Also returns false if instance is not valid.
				@param x, y the coordinates
				@return bool
		*/
		bool has(float x, float y) const throw();

		//BAUSTELLE
		/**	Returns the linear interpolation of the eight surrounding grid points.
				This method calculates the corresponding rectangle to a Vector2 and linearly.
				Then, it calculates the position of this Vector2 in rectangle coordinates dx, dy
				with $0 \le dx, dy \le 1$. It then calculates the weighted average of 
				the values at the eight surrounding grid points $v_i$:
				\TEX{
					\begin{eqnarray*}
						{\mathrm value} & = & v_1 dx dy dz\\
														& + & v_2 (1 - dx) dy dz\\
														& + & v_3 dx (1 - dy) dz\\
														& + & v_4 (1 - dx) (1 - dy) dz\\
					\end{eqnarray*}
				}
				@exception OutOfGrid if the point is outside the grid
				@param	vector the position to evaluate
		*/
		GridDataType getInterpolatedValue(const Vector2& vector) const throw(Exception::OutOfGrid);
		
		/** Get the maximum value in the the data set.
		 * 	If the data grid is empty, 0 is returned.
		 * 	@param position returns the position of the first maximum value
		 * 	@return GridDataTyperData2D the maximum value
		 */ 	
		const GridDataType* getMaxValue(Vector2& position) const throw();
		
		/** Get the minimum value in the the data set.
		 * 	If the data grid is empty, 0 is returned.
		 * 	@param position returns the position of the first minimum value
		 * 	@return GridDataTyperData2D the minimum value
		 */ 	
		const GridDataType* getMinValue(Vector2& position) const throw();
			
		/** Rescale the data.
		 * 	All data values are rescaled to fit between a minimum and a maximum value.
		 * 	@param minValue the new lower boundary for the values
		 * 	@param maxValue the new upper boundary for the values
		 */
		void rescale(const GridDataType& minValue, const GridDataType& maxValue) throw();
		
		/** Equality operator.
				Two point grids are equal if they have the same number of points in all two
				dimensions, same origin, spacing and the data fields are equal.
				Both grids have to be valid or false is returned.
		*/
		bool operator == (const TRegularData2D<GridDataType>& grid) const throw();

		/** Inequality operator.
				@see operator ==
		*/
		bool operator != (const TRegularData2D<GridDataType>& grid) const throw();

		//@}

		/**	The grid data
		*/
		GridDataType* data;

		protected:
			
		/*_ origin of the rectangle (offset) 
		*/	
		Vector2	origin_;

		/*_ size of the rectangle 
		*/
		Vector2	size_;

		/*_ distance grid spacing (spacing = size / (number_of_points - 1))
		*/
		Vector2 spacing_;

		/*_ number of grid points of the rectangle
		*/
		Size number_of_points_x_, number_of_points_y_;

		/*_ contains the total number of grid points
		*/
		Size number_of_grid_points_;

		/*_ contains the topmost edge
		*/
		Vector2 upper_;

		/*_ Is set to true, if the grid has been set up correctly.
				If the requested memory couldn't be allocated, valid_ is set to
				false. The state of this flag can be queried by isValid()
				@see	isValid
		*/
		bool valid_;
	};

	/**	Default type
	*/
	typedef TRegularData2D<float> RegularData2D;

	// default constructor.
	template <class GridDataType>
	TRegularData2D<GridDataType>::TRegularData2D()
		throw()
		: data(0),
			origin_(0,0),
			size_(0,0),
			spacing_(0,0),
			number_of_points_x_(0),
			number_of_points_y_(0),
			number_of_grid_points_(0),
			upper_(0,0),
			valid_(false)
	{
	}

	// copy constructor
	template <class GridDataType>
	TRegularData2D<GridDataType>::TRegularData2D
		(const TRegularData2D<GridDataType>& grid, bool /* deep */)
		throw(Exception::OutOfMemory)
		: data(0),
			origin_(0,0),
			size_(0,0),
			spacing_(0,0),
			number_of_points_x_(0),
			number_of_points_y_(0),
			number_of_grid_points_(0),
			upper_(0,0),
			valid_(false)
	{
		set(grid);
	}

	// assignment operator
	template <typename GridDataType>
	BALL_INLINE
	const TRegularData2D<GridDataType>& TRegularData2D<GridDataType>::operator = 
		(const TRegularData2D<GridDataType>& grid)
		throw(Exception::OutOfMemory)
	{
		set(grid);
		return *this;
	}

	// set method
	template <typename GridDataType>
	BALL_INLINE
	void TRegularData2D<GridDataType>::set(const TRegularData2D<GridDataType>& grid)
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
		origin_ = grid.origin_;
		size_   = grid.size_;

		spacing_ = Vector2(grid.getXSpacing(), grid.getYSpacing());

		number_of_points_x_ = grid.number_of_points_x_;
		number_of_points_y_ = grid.number_of_points_y_;
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

	template <class GridDataType>
	BALL_INLINE
	TRegularData2D<GridDataType>::TRegularData2D
		(float lower_x, float lower_y, float upper_x, float upper_y, Size grid_points_x, Size grid_points_y)
		throw(Exception::OutOfMemory)
	{
		// set data and number_of_grid_points_ to 0/0, just to be sure 
		data = (GridDataType*) 0;
		number_of_grid_points_ = 0;

		// set the number of grid points in all directions
		number_of_points_x_ = grid_points_x;
		number_of_points_y_ = grid_points_y;

		// if the number of grid points in any direction is below 2
		// (which means that the grid is not two-dimensional!)
		// then increase to at least one point in each dimension
		if (number_of_points_x_ < 2)
		{
			number_of_points_x_ = 2;
		}
		if (number_of_points_y_ < 2)
		{
			number_of_points_y_ = 2;
		}
		// calculate the origin as the lowest given coordinates
		// of each direction
		origin_ = Vector2((lower_x < upper_x ) ? lower_x : upper_x,
							        (lower_y < upper_y ) ? lower_y : upper_y);

		// calculate the rectangle sizes
		size_ = Vector2(fabs(upper_x - lower_x), fabs(upper_y - lower_y));

		// calculate the topmost edge
		upper_ = Vector2(origin_.x + size_.x, origin_.y + size_.y);

		// calculate the grid spacing in all directions
		spacing_ = Vector2(size_.x / (number_of_points_x_ - 1),
											 size_.y / (number_of_points_y_ - 1));

		// calculate the total number of grid points
		number_of_grid_points_ = number_of_points_x_ * number_of_points_y_;
		
		// allocate space for the array containing pointers to the objects
		data = new GridDataType[number_of_grid_points_];

		// mark this instance as invalid if the alloc failed
		valid_ = (data != 0);			

		if (!valid_)
		{
			throw Exception::OutOfMemory(__FILE__, __LINE__, 
									number_of_grid_points_ * sizeof(GridDataType));
		}

		for (Position pos = 0; pos < number_of_grid_points_; pos++)
		{
			data[pos] = GridDataType();
		}
	}

	template <class GridDataType>
	TRegularData2D<GridDataType>::TRegularData2D
		(const Vector2& lower, const Vector2& upper, Size grid_points_x, Size grid_points_y) 
		throw(Exception::OutOfMemory)
		: data(0)
	{
		*this = TRegularData2D(lower.x, lower.y, upper.x, upper.y,
													grid_points_x, grid_points_y);
	}

	template <class GridDataType>
	TRegularData2D<GridDataType>::TRegularData2D(const Vector2& lower, const Vector2& upper, float spacing) 
		throw(Exception::OutOfMemory)
		: data(0)
	{
		*this = TRegularData2D(lower.x, lower.y, upper.x, upper.y,
											(Size)((upper.x - lower.x) / spacing + 1), 
											(Size)((upper.y - lower.y) / spacing + 1));
	}
			
	template <class GridDataType>
	void TRegularData2D<GridDataType>::dump(std::ostream& stream) const throw()
	{
		stream << "Dump of " << typeid(this).name()<< " (" << getMaxXIndex() 
					 << "x" << getMaxYIndex() << ")" << std::endl;
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
	bool TRegularData2D<GridDataType>::isValid() const throw()
	{
		return valid_;
	}

	// getMax[x,y] returns the maximum possible coordinates for
	// the rectangle, i.e. origin + size
	template <class GridDataType>
	BALL_INLINE 
	float TRegularData2D<GridDataType>::getMaxX() const throw()
	{
		return upper_.x;
	}

	template <class GridDataType>
	BALL_INLINE 
	float TRegularData2D<GridDataType>::getMaxY() const throw()
	{
		return upper_.y;
	}

	// getMax[x,y] returns the maximum possible coordinates for
	// the rectangle, i.e. origin + size
	template <class GridDataType>
	BALL_INLINE 
	float TRegularData2D<GridDataType>::getMinX() const throw()
	{
		return origin_.x;
	}

	template <class GridDataType>
	BALL_INLINE 
	float TRegularData2D<GridDataType>::getMinY() const throw()
	{
		return origin_.y;
	}

	// getMax[x,y]Position returns the maximum grid position for the rectangle.
	//.x point has position 0, getMax[x,y]Position therefore returns number_of_points
	template <class GridDataType>
	BALL_INLINE 
	Size TRegularData2D<GridDataType>::getMaxXIndex() const
		throw()
	{
		return number_of_points_x_ - 1;
	}

	template <class GridDataType>
	BALL_INLINE 
	Size TRegularData2D<GridDataType>::getMaxYIndex() const
		throw()
	{
		return number_of_points_y_ - 1;
	}

	// getSize() returns the total number of grid points
	template <class GridDataType>
	BALL_INLINE 
	Size TRegularData2D<GridDataType>::getSize() const
		throw()
	{
		return number_of_grid_points_;
	}

	template <class GridDataType>
	TRegularData2D<GridDataType>::GridIndex 
		TRegularData2D<GridDataType>::getIndex(const Vector2& r) const 
		throw(Exception::OutOfGrid)
	{
		return getIndex(r.x, r.y);
	}

	// get[x,y]Spacing returns the grid spacing, i.e. the distance
	// between two grid points in the given direction
	template <class GridDataType>
	BALL_INLINE 
	float TRegularData2D<GridDataType>::getXSpacing() const
		throw()
	{
		return spacing_.x;
	}

	template <class GridDataType>
	BALL_INLINE 
	float TRegularData2D<GridDataType>::getYSpacing() const
		throw()
	{
		return spacing_.y;
	}

	template <class GridDataType> 
	BALL_INLINE
	const Vector2& 
		TRegularData2D<GridDataType>::getOrigin() const
		throw()
	{
		return origin_;
	}

	template <class GridDataType> 
	BALL_INLINE
	Vector2& 
		TRegularData2D<GridDataType>::getOrigin()
		throw()
	{
		return origin_;
	}

	template <class GridDataType>
	void TRegularData2D<GridDataType>::setOrigin(const Vector2& origin)
		throw()
	{
		origin_ = origin;
	}

	template <class GridDataType>
	void TRegularData2D<GridDataType>::setOrigin(float x, float y) 
	 	throw()
	{
		origin_ = Vector2(x, y);
	}

	template <class GridDataType> 
	const Vector2& 
		TRegularData2D<GridDataType>::getDimension() const
		throw()
	{
		return size_;
	}

	template <class GridDataType> 
	Vector2& 
		TRegularData2D<GridDataType>::getDimension()
		throw()
	{
		return size_;
	}

	template <class GridDataType> 
	BALL_INLINE
	bool TRegularData2D<GridDataType>::has(float x, float y) const		
		throw()
	{
		if (x > upper_.x  ||	y > upper_.y  ||
				x < origin_.x ||	y < origin_.y || !valid_)
		{
			return false;
		}		

		return true;
	}

	template <class GridDataType> 
	BALL_INLINE
	bool TRegularData2D<GridDataType>::has(const Vector2& vector) const
		throw()
	{
		if (vector.x > upper_.x  ||	vector.y > upper_.y  ||
				vector.x < origin_.x ||	vector.y < origin_.y ||	!valid_)
		{
			return false;
		}		

		return true;
	}

	template <class GridDataType>
	BALL_INLINE 
	TRegularData2D<GridDataType>::GridIndex 
		TRegularData2D<GridDataType>::getIndex(float x, float y) const 
		throw(Exception::OutOfGrid)
	{
		if (!has(x, y))
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		}				
		
		GridIndex	position;

		position.x = (Position) ((x - origin_.x) / spacing_.x + 0.5);
		position.y = (Position) ((y - origin_.y) / spacing_.y + 0.5);

		return position;
	}

	template <class GridDataType>
	BALL_INLINE 
	GridDataType* TRegularData2D<GridDataType>::getData(const Position i, const Position j) 
		throw(Exception::OutOfGrid)
	{
		if (i >= number_of_points_x_ ||	j >= number_of_points_y_ || !valid_)
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		}		

		return &(data[i + j * number_of_points_x_ ]);
	}

	template <class GridDataType>
	BALL_INLINE 
	GridDataType* TRegularData2D<GridDataType>::getData(const Vector2& r) 
		throw(Exception::OutOfGrid)
	{	
		GridIndex	position = getIndex(r);
		return getData(position.x, position.y);
	}

	template <class GridDataType>
	BALL_INLINE 
	GridDataType* TRegularData2D<GridDataType>::getData(const Position position) 
		throw(Exception::OutOfGrid)
	{
		if (position > number_of_grid_points_)
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		} 
		return &(data[position]);
	}

	template <class GridDataType>
	GridDataType& TRegularData2D<GridDataType>::operator[] (const Position position)
		throw(Exception::OutOfGrid)
	{
		if (position > number_of_grid_points_)
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		} 
		return *(getData(position));
	}

	template <class GridDataType>
	GridDataType& TRegularData2D<GridDataType>::operator[](const Vector2& v)
		throw(Exception::OutOfGrid)
	{
		return *(getData(v));
	}

	template <class GridDataType>
	BALL_INLINE 
	Vector2 TRegularData2D<GridDataType>::getGridCoordinates
		(const Position i, const Position j) const 
		throw(Exception::OutOfGrid)
	{
		if (i >= number_of_points_x_ ||	j >= number_of_points_y_)
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		}		
		
		Vector2 r(origin_.x + i * spacing_.x,
							origin_.y + j * spacing_.y);

		return r;
	}

	template <class GridDataType> 
	BALL_INLINE 
	Vector2 
		TRegularData2D<GridDataType>::getGridCoordinates(const Position position) const 
		throw(Exception::OutOfGrid)
	{
		if (position >= number_of_grid_points_)
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		} 
		
		Position x =  position %  number_of_points_x_;
		Position y = (position % (number_of_points_x_ * number_of_points_y_)) / number_of_points_x_;

		Vector2 r( origin_.x + (float)x * spacing_.x,
							origin_.y + (float)y * spacing_.y);

		return r;
	}

	template <typename GridDataType>
	BALL_INLINE
	void TRegularData2D<GridDataType>::getRectangleIndices(const Vector2& vector,
		Position& ll, Position& lr, Position& ul, Position& ur) const
		throw(Exception::OutOfGrid)
	{
		if (!has(vector))
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		}		
		// calculate the grid indices of the lower left front corner
		// of the enclosing rectangle
		GridIndex position;
		position.x = (Position)((vector.x - origin_.x) / spacing_.x);
		position.y = (Position)((vector.y - origin_.y) / spacing_.y);
		
		// calculate the (linear) indices of the four rectangle corners
		ll = position.x + number_of_points_x_ * position.y;
		lr = ll + 1;
		ul = ll + number_of_points_x_;
		ur = ul + 1;
	}

	template <typename GridDataType>
	BALL_INLINE
	void TRegularData2D<GridDataType>::getRectangleData(const Vector2& vector,
		GridDataType& ll, GridDataType& lr, GridDataType& ul, GridDataType& ur) const
		throw(Exception::OutOfGrid)
	{
		if (!has(vector))
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		}		
		
		// compute the four grid indices forming the enclosing rectangle
		Position ll_id, lr_id, ul_id, ur_id;
		getRectangleIndices(vector, ll_id, lr_id, ul_id, ur_id);
				
		// retrieve the grid values
		ll = data[ll_id];
		lr = data[lr_id];
		ul = data[ul_id];
		ur = data[ur_id];
	}

	template <typename GridDataType>
	BALL_INLINE
	GridDataType TRegularData2D<GridDataType>::getInterpolatedValue(const Vector2& vector) const
		throw(Exception::OutOfGrid)
	{
		if (!has(vector))
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		}		

		Vector2 h(vector.x - origin_.x, vector.y - origin_.y);
		Position x = (int)(h.x / spacing_.x);
		Position y = (int)(h.y / spacing_.y);

		unsigned long Nx = number_of_points_x_;
		unsigned long l = x + Nx * y;
		Vector2 r_0(getGridCoordinates((Position)l));
		float dx = 1 - ((vector.x  - r_0.x ) / getXSpacing());
		float dy = 1 - ((vector.y - r_0.y) / getYSpacing());

		return  data[l] * dx * dy
					+ data[l + 1] * (1 - dx) * dy
					+ data[l + Nx] * dx * (1 - dy)
					+ data[l + Nx + 1] * (1 - dx) * (1 - dy);
	}

	template <typename GridDataType>
	void TRegularData2D<GridDataType>::clear() throw()
	{
		delete [] data;
		data = 0;

		origin_		=
		size_			=
		spacing_  =
		upper_		=	Vector2(0, 0);

		number_of_points_x_ =
		number_of_points_y_ =
		number_of_grid_points_ = 0;

		valid_ = false;
	}

	template <typename GridDataType>
	const GridDataType* TRegularData2D<GridDataType>::getMinValue(Vector2& position) const throw()
	{
		if (number_of_grid_points_ == 0)
		{
			return 0;
		}
		
		GridDataType* min = new GridDataType(data[0]);
		Position pos(0);
		for (Position i = 0; i < number_of_grid_points_; i++)
		{
			if (data[i] < *min)
			{
				*min = data[i];
				pos = i;
			}
		}
		
		position = getGridCoordinates(pos);
		return min;
	}
	
	template <typename GridDataType>
	const GridDataType* TRegularData2D<GridDataType>::getMaxValue(Vector2& position) const throw()
	{
		if (number_of_grid_points_ == 0)
		{
			return 0;
		}
		
		GridDataType* max = new GridDataType(data[0]);
		Position pos(0);
		for (Position i = 0; i < number_of_grid_points_; i++)
		{
			if (data[i] > *max)
			{
				*max = data[i];
				pos = i;
			}
		}
		
		position = getGridCoordinates(pos);
		return max;
	}
	
	template <typename GridDataType>
	void TRegularData2D<GridDataType>::rescale(const GridDataType& minValue, const GridDataType& maxValue) 
		throw()
	{
		GridDataType new_min;
		GridDataType new_max;
		if (maxValue > minValue)
		{
			new_min = minValue;
			new_max = maxValue;
		}	
		else
		{
			new_min = maxValue;
			new_max = minValue;
		}
		
		Vector2 dummy;
		GridDataType old_min = *getMinValue(dummy);
		GridDataType old_max = *getMaxValue(dummy);

		GridDataType shift_factor(old_min - new_min);
		GridDataType resize_factor = new_max / (old_max - shift_factor);
		
		for (Position i = 0; i < number_of_grid_points_; i++)
		{
			data[i] -= shift_factor;
			data[i] *= resize_factor;
		}
	}
		
	template <typename GridDataType>	
	bool TRegularData2D<GridDataType>::operator == (const TRegularData2D<GridDataType>& grid) const 
		throw()
	{
		if ((!valid_)						|| 
				(!grid.valid_)			||
				(number_of_points_x_ != grid.number_of_points_x_) ||
				(number_of_points_y_ != grid.number_of_points_y_) ||
				(origin_						 != grid.origin_						)	||
				(spacing_						 != grid.spacing_						)	)
		{
			return false;
		}

		for (Position pos = 0; pos < number_of_grid_points_; pos++)
		{
			if (!(data[pos] == grid.data[pos]))
			{
				return false;
			}
		}

		return true;
	}	

	template <typename GridDataType>	
	bool TRegularData2D<GridDataType>::operator != (const TRegularData2D<GridDataType>& grid) const 
		throw()
	{
		return !(*this == grid);
	}

 } // namespace BALL

#endif // BALL_DATATYPE_TRegularData2D_H
