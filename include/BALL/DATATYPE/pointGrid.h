// $Id: pointGrid.h,v 1.1 1999/08/26 07:53:13 oliver Exp $ 

#ifndef BALL_DATATYPE_POINTGRID_H
#define BALL_DATATYPE_POINTGRID_H

//#ifndef BALL_COMMON_H
//#	include <BALL/common.h>
//#endif

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

namespace BALL 
{

	// BUG: egcs doesn't allow structs in template classes
	// until this is fixed, they remain outside

	/**	@name	Type Definitions
	*/
	//@{
	/**	IndexStruct
			PointGrid index type.
			This struct is used to represent the indices of a
			grid point.
			@see		PointGrid
			@see		GridPoint::getIndex
	*/
	struct IndexStruct 
	{
		///
		Position x;
		///
		Position y;
		///
		Position z;
	};
	/**	@name	Index
			@memo  Grid index type
	*/
	typedef struct IndexStruct GridIndex;
	//@}

	/**	Simple 3D grid class.
			Unlike BoxGrid this class represents a three-dimensional
			array. An instance of GridDataType will be created
			for each point of the grid upon instantiation of PointGrid.\\
		
			{\bf Definition:}\\
			\URL{BALL/DATATYPE/pointGrid.h}
			\\
			@see	HashGrid3
	*/
	template <class GridDataType>
	class PointGrid 
	{

		public:

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				Creates creates a PointGrid object without allocating a grid.
				@memo
		*/
		PointGrid();	

		/**	Copy constructor.
				Creates a copy of an existing PointGrid object.
				@memo
		*/
		PointGrid(PointGrid<GridDataType>& grid, bool deep = true);	

		/**	Constructor for PointGrid.
				{\em lower\_[x|y|z]} should be set to the coordinates of
				the "lower" corner of the box represented by the grid
				{\em upper\_[x|y|z]} should likewise contain the "upper" corner
				In fact, it doesn't really matter which coordinates are
				which, as this method always takes the lowest coordinates (x,y,z)
				for the lower corner and the highest coordinates for
				the upper corner.\\
				{\em grid\_points\_[x|y|z]} gives the number of grid points in 
				either direction. 
				@param	lower_x	float, the x coordinate of the lower corner of the grid
				@param	lower_y	float, the y coordinate of the lower corner of the grid
				@param	lower_z	float, the z coordinate of the lower corner of the grid
				@param	upper_x	float, the x coordinate of the upper corner of the grid
				@param	upper_y	float, the y coordinate of the upper corner of the grid
				@param	upper_z	float, the z coordinate of the upper corner of the grid
				@memo
		*/
		PointGrid
			(const float lower_x, 
			 const float lower_y, 
			 const float lower_z,
			 const float upper_x, 
			 const float upper_y, 
			 const float upper_z,
			 const Size grid_points_x, 
			 const Size grid_points_y, 
			 const Size grid_points_z);


		/**	Constructor.
				The grid's origin is at lower, it has grid\_points\_[x|y|z]
				points in each direction.
				@memo
		*/
		PointGrid
			(const Vector3& lower, 
			 const Vector3& upper,
			 const Size grid_points_x, 
			 const Size grid_points_y, 
			 const Size grid_points_z);


		/**	Constructor. 
				Takes lower and upper corners and the grid spacing
				The grid will include the coordinates given by upper/lower.
				Its origin is in the lower corner, it may extend up to spacing over
				the upper corner.
				@memo
		*/
		PointGrid(const Vector3& lower, const Vector3& upper, const float spacing);

		/**	Destructor. 
				Frees all allocated memory.
				@memo
		*/
		virtual ~PointGrid(void);
		//@}


		/**	@name	Debugging and Diagnostics
		*/
		//@{

		/**
		*/
		virtual void dump(ostream& stream) const; 

		/**	Returns the current stat of the object.	
				isValid() returns false, if the grid couldn't be initialized 
				correctly. Usually this is due to an allocation error in one
				of the constructors.
				@memo
		*/
		bool	isValid() const;
		//@}

		/**	@name	Accessors
		*/
		//@{

		/**	Returns the largest possible x coordinate for the box.
				@memo
		*/
		float getMaxX(void) const;

		/**	Returns the largest possible y coordinate for the box.
				@memo
		*/
		float getMaxY(void) const;

		/**	Returns the largest possible z coordinate for the box.
				@memo
		*/
		float getMaxZ(void) const;

		
		/**	Returns the x coordinate of the grid origin.
				@memo
		*/
		float getMinX(void) const;

		/**	Returns the y coordinate of the grid origin.
				@memo
		*/
		float getMinY(void) const;

		/**	Returns the z coordinate of the grid origin.
				@memo
		*/
		float getMinZ(void) const;

		

		/**	Return the largest grid index for the x direction.
				This method returns the maximum index allowed in the grid.
				As the point in the origin has the indices (0, 0, 0), this
				method returns the number of points in each direction.
				@memo
		*/
		Size getMaxXIndex(void) const;

		/**	Return the largest grid index for the y direction.
				This method returns the maximum index allowed in the grid.
				As the point in the origin has the indices (0, 0, 0), this
				method returns the number of points in each direction.
				@memo
		*/
		Size getMaxYIndex(void) const;

		/**	Return the largest grid index for the z direction.
				This method returns the maximum index allowed in the grid.
				As the point in the origin has the indices (0, 0, 0), this
				method returns the number of points in each direction.
				@memo
		*/
		Size getMaxZIndex(void) const;


		/**	Returns the total number of grid points.
				@memo
		*/
		Size getSize(void) const;


		/**	Returns the grid spacing in x direction.
				@memo
		*/	
		float getXSpacing(void) const;

		/**	Returns the grid spacing in y direction.
				@memo
		*/	
		float getYSpacing(void) const;

		/**	Returns the grid spacing in z direction.
				@memo
		*/	
		float getZSpacing(void) const;

		/**	Returns the index of the grid point closest to the given vector.
				If there are multiple grid points with equal distance, the
				grid point with the lowest indices in x, y, and z direction is
				returned. The given vector may also lie outside the grid!
				@memo
		*/
		GridIndex getIndex(const Vector3&) const;

		/**	Returns the index of the grid point closest to three given coordinates.
				If there are multiple grid points with equal distance, the
				grid point with the lowest indices in x, y, and z direction is
				returned. The three coordinates may also be outside the grid.
				@memo
		*/
		GridIndex getIndex(const float, const float, const float) const;
		

		/**	Returns a pointer to the grid contents determined by the three indices.
				If any of the indices is invalid (i.e. negative or larger than the
				maximum allowed index) 0 is returned.
				@memo
		*/
		GridDataType* getData(const Position i, const Position j, const Position k);

		/**	Returns a pointer to the grid contents closest to a given vector.
				Determination of the selected grid point is made via getIndex.
				@memo
		*/
		GridDataType* getData(const Vector3& r);

		/**	Returns a pointer to the grid contents determined by the index.
				If the index is negative or greater or equal than the number of 
				grid points, 0 is returned.
				@memo
		*/
		GridDataType* getData(const Position index);

		/**	Subscript operator.
				Returns the data of the grid point specified by its {\tt index}.
				@return					GridDataType
				@param					index Index, the grid index
				@see						getData
				@memo
		*/
		GridDataType& operator[](const Position index);

		/**	Subscript operator.
				Returns the data of the grid point nearest to the given
				{\tt vector}
				@return					GridDataType
				@param					vector Vector3, a position in the grid
				@see						getData
				@memo
		*/
		GridDataType& operator[](const Vector3& vector);

		/**	returns the exact coordinates of a grid point.	
				@return					Vector3\&
				@param					i Index, grid x index
				@param					j Index, grid y index
				@param					k Index, grid z index
				@memo
		*/
		Vector3 getGridCoordinates(const Index i, const Index j, const Index k) const;

		/**		returns the exact coordinates of the grid point near to a vector r.	
					This function calculates the exact coordinates of the 
					closest grid point whose x-, y, and z-coordinates are smaller
					than the vector`s coordinates. With this function it is easily
					possible to determine the "box" of points that enclose the given 
					vector.
					@return					Vector3\&
					@param					r Vector3
				@memo
		*/
		Vector3 getGridCoordinates(const Vector3& r) const;

		/**		returns the exact coordinates of a grid point.	
					@return					Vector3\&
					@param					index Index, grid index
				@memo
		*/
		Vector3 getGridCoordinates(const Index index) const;
		
		/**		Returns a vector to the grid's origin.
					@return					Vector3\&
				@memo
		*/
		Vector3& getOrigin(void);

		/**		Returns a vector to the grid's origin (const method).
					@return					Vector3
				@memo

		*/
		const Vector3& getOrigin(void) const;

		/**		Modifies the grid's origin
					@return					bool, always true
					@param				  origin, new origin
				@memo
		*/
		bool setOrigin(const Vector3& origin);

		/**		Modifies the grid's origin
					@return					bool, always true
					@param				  x new origin x
					@param				  y new origin y
					@param				  z new origin z
				@memo
		*/
		bool setOrigin(const float x, const float y, const float z);

		/**		Returns a vector containing the grid's dimensions.
					@return					Vector3\&
				@memo
		*/
		Vector3& getDimension(void);
		
		/**		Returns a vector containing the grid's dimensions (const method).
					@return					Vector3
				@memo
		*/
		const Vector3& getDimension(void) const;

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
				@param	vector the position to evaluate
		*/
		GridDataType getInterpolatedValue(const Vector3& vector) const;
		
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
		Index number_of_points_x_, number_of_points_y_, number_of_points_z_;

		/*_ contains the total number of grid points
		*/
		Index number_of_grid_points_;

		/*_ is set to true, if the grid has been set up correctly.
				If the requested memory couldn't be allocated, valid_ is set to
				false. The state of thei flag can be queried by isValid()
				@see	isValid
		*/
		bool valid_;
	};

	// default constructor.
	template <class GridDataType>
	PointGrid<GridDataType>::PointGrid()
		: data(0),
			origin_(0,0,0),
			size_(0,0,0),
			spacing_(0,0,0),
			number_of_points_x_(0),
			number_of_points_y_(0),
			number_of_points_z_(0),
			number_of_grid_points_(0)
	{
	}

	// copy constructor
	template <class GridDataType>
	PointGrid<GridDataType>::PointGrid(PointGrid<GridDataType>& grid, bool deep)
		:	data(0),
			origin_(0,0,0),
			size_(0,0,0),
			spacing_(0,0,0),
			number_of_points_x_(0),
			number_of_points_y_(0),
			number_of_points_z_(0),
			number_of_grid_points_(0)
	{
		data = new GridDataType[grid.getSize()];

		// if the alloc failed, mark this instance as invalid
		valid_ = (data != 0);

		origin_ = grid.getOrigin();
		size_ = grid.getDimension();
		spacing_.set(grid.getXSpacing(), grid.getYSpacing(), grid.getZSpacing());
		number_of_points_x_ = grid.number_of_points_x_;
		number_of_points_y_ = grid.number_of_points_y_;
		number_of_points_z_ = grid.number_of_points_z_;
		number_of_grid_points_ = grid.number_of_grid_points_;

		// copy the grid if enough memory could be allocated
		if (valid_){
			memcpy(data, grid.data, grid.getSize());
		}
	}

	//  First constructor for PointGrid
	//  lower_[x|y|z] should be set to the coordinates of
	//  the "lower" corner of the box represented by the grid
	//  upper_[x|y|z] should likewise contain the "upper" corner
	//  In fact, it doesn't really matter which coordinates are
	//  which, as it always takes the lowest coordinates (x,y,z)
	//  for the lower corner and the highest coordinates for
	//  the upper corner.
	//  grid_points_[x|y|z] gives the number of grid points in
	//  either direction.

	template <class GridDataType>
	PointGrid<GridDataType>::PointGrid
		(const float lower_x, const float lower_y, const float lower_z,
		 const float upper_x, const float upper_y, const float upper_z,
		 const Size grid_points_x,
		 const Size grid_points_y,
		 const Size grid_points_z)
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
			number_of_points_x_ = 2;
		if (number_of_points_y_ < 2)
			number_of_points_y_ = 2;
		if (number_of_points_y_ < 2)
			number_of_points_y_ = 2;


		// calculate the origin as the lowest given coordinates
		// of each direction
		origin_.x = ( lower_x < upper_x ) ? lower_x : upper_x;
		origin_.y = ( lower_y < upper_y ) ? lower_y : upper_y;
		origin_.z = ( lower_z < upper_z ) ? lower_z : upper_z;

		// calculate the box sizes
		size_.x = fabs(upper_x - lower_x);
		size_.y = fabs(upper_y - lower_y);
		size_.z = fabs(upper_z - lower_z);

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
			
	}

	template <class GridDataType>
	PointGrid<GridDataType>::PointGrid(
				const Vector3 &lower,
				const Vector3 &upper,
				const Size grid_points_x,
				const Size grid_points_y,
				const Size grid_points_z) {

		// set data and number_of_grid_points_ to 0/0, just to be sure 
		data = (GridDataType*) 0;
		number_of_grid_points_ = 0;

		// set the number of grid points in all directions
		number_of_points_x_ = grid_points_x;
		number_of_points_y_ = grid_points_y;
		number_of_points_z_ = grid_points_z;

		
		// if the number of grid points in any direction is below 2
		// (which means that the grid is not three-dimensional!)
		// an error occured, so destroy everything again
		if ((number_of_points_x_ < 2) || (number_of_points_y_ < 2) || (number_of_points_z_ < 2)){
			delete this;
			return;
		}

		// calculate the origin as the lowest given coordinates
		// of each direction
		origin_.x = ( lower.x < upper.x ) ? lower.x : upper.x;
		origin_.y = ( lower.y < upper.y ) ? lower.y : upper.y;
		origin_.z = ( lower.z < upper.z ) ? lower.z : upper.z;


		// calculate the box sizes
		size_.x = fabs(upper.x - lower.x);
		size_.y = fabs(upper.y - lower.y);
		size_.z = fabs(upper.z - lower.z);

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
	}

	template <class GridDataType>
	PointGrid<GridDataType>::PointGrid(
				const Vector3 &lower,
				const Vector3 &upper,
				const float spacing) {

		// set data and number_of_grid_points_ to 0/0, just to be sure 
		data = (GridDataType*) 0;
		number_of_grid_points_ = 0;

		// calculate the origin as the lowest given coordinates
		// of each direction
		origin_.x = ( lower.x < upper.x ) ? lower.x : upper.x;
		origin_.y = ( lower.y < upper.y ) ? lower.y : upper.y;
		origin_.z = ( lower.z < upper.z ) ? lower.z : upper.z;


		// calculate the box sizes
		size_.x = fabs(upper.x - lower.x);
		size_.y = fabs(upper.y - lower.y);
		size_.z = fabs(upper.z - lower.z);

		// set the number of grid points in all directions
		number_of_points_x_ = (Index)(size_.x / spacing) + 1;
		number_of_points_y_ = (Index)(size_.y / spacing) + 1;
		number_of_points_z_ = (Index)(size_.z / spacing) + 1;

		// recalculate the box sizes
		size_.x = (number_of_points_x_ - 1) * spacing;
		size_.y = (number_of_points_y_ - 1) * spacing;
		size_.z = (number_of_points_z_ - 1) * spacing;

		// if the number of grid points in any direction is below 2
		// (which means that the grid is not three-dimensional!)
		// an error occured, so destroy everything again
		if ((number_of_points_x_ < 2) || (number_of_points_y_ < 2) || (number_of_points_z_ < 2)){
			delete this;
			return;
		}

		// calculate the grid spacing in all directions
		spacing_.x = spacing;
		spacing_.y = spacing;
		spacing_.z = spacing;
		
		// calculate the total number of grid points
		number_of_grid_points_ = number_of_points_x_ * number_of_points_y_ * number_of_points_z_;
		

		// allocate space for the array containing pointers to the objects
		data = new GridDataType[number_of_grid_points_];

		// mark this instance as invalid
		valid_ = (data != 0);
	}
			
		
	/*
					Destructor, frees the array of allocated grid elements 
	*/
	template <class GridDataType>
	PointGrid<GridDataType>::~PointGrid(void) {
		delete [] data;
	}


	 

	template <class GridDataType>
	void PointGrid<GridDataType>::dump(ostream& stream) const
	{
		Index i;

		stream << "Dump of " << typeid(this).name()<< " (" << getMaxXIndex() 
					 << "x" << getMaxYIndex() << "x" << getMaxZIndex() 
					 << ")" << endl;
		stream << "--------------------------------------" << endl;
		for (i = 0; i < number_of_grid_points_; i++){
			stream << "(" << data[i] << ")";
		}
		stream << endl << "--------------------------------------" << endl;
	}


	// returns the state of this instance
	template <class GridDataType>
	BALL_INLINE 
	bool PointGrid<GridDataType>::isValid(void) const
	{
		return valid_;
	}

	// getMax[X|Y|Z] returns the maximum possible coordinates for
	// the box, i.e. origin + size
	template <class GridDataType>
	BALL_INLINE 
	float PointGrid<GridDataType>::getMaxX(void) const 
	{
		return origin_.x + size_.x;
	}

	template <class GridDataType>
	BALL_INLINE 
	float PointGrid<GridDataType>::getMaxY(void) const 
	{
		return origin_.y + size_.y;
	}


	template <class GridDataType>
	BALL_INLINE 
	float PointGrid<GridDataType>::getMaxZ(void) const 
	{
		return origin_.z + size_.x;
	}

	// getMax[X|Y|Z] returns the maximum possible coordinates for
	// the box, i.e. origin + size
	template <class GridDataType>
	BALL_INLINE 
	float PointGrid<GridDataType>::getMinX(void) const 
	{
		return origin_.x;
	}

	template <class GridDataType>
	BALL_INLINE 
	float PointGrid<GridDataType>::getMinY(void) const 
	{
		return origin_.y;
	}


	template <class GridDataType>
	BALL_INLINE 
	float PointGrid<GridDataType>::getMinZ(void) const 
	{
		return origin_.z;
	}

	// getMax[X|Y|Z]Index returns the maximum grid index for the box.
	// first point has index 0, getMax[X|Y|Z]Index therefore returns number_of_points
	template <class GridDataType>
	BALL_INLINE 
	Size PointGrid<GridDataType>::getMaxXIndex(void) const 
	{
		return number_of_points_x_ - 1;
	}

	template <class GridDataType>
	BALL_INLINE 
	Size PointGrid<GridDataType>::getMaxYIndex(void) const 
	{
		return number_of_points_y_ - 1;
	}

	template <class GridDataType>
	BALL_INLINE 
	Size PointGrid<GridDataType>::getMaxZIndex(void) const 
	{
		return number_of_points_z_ - 1;
	}

	// getSize() returns the total number of grid points
	template <class GridDataType>
	BALL_INLINE 
	Size PointGrid<GridDataType>::getSize(void) const 
	{
		return number_of_grid_points_;
	}

	template <class GridDataType>
	GridIndex PointGrid<GridDataType>::getIndex(const Vector3& r) const 
	{
		GridIndex	index;
		Index 		i;
		
		i = (long)((r.x - origin_.x) / spacing_.x + 0.5);
		if (i < 0){
			index.x = 0;
		} else {
			if  (i >= (long)number_of_points_x_){
				index.x = number_of_points_x_ - 1;
			} else {
				index.x = (Index) i;
			}
		}

		i = (long)((r.y - origin_.y) / spacing_.y + 0.5);
		if (i < 0){
			index.y = 0;
		} else {
			if  (i >= (long)number_of_points_y_){
				index.y = number_of_points_y_ - 1;
			} else {
				index.y = (Index) i;
			}
		}

		i = (long)((r.z - origin_.z) / spacing_.z + 0.5);
		if (i < 0){
			index.z = 0;
		} else {
			if  (i >= (long)number_of_points_z_){
				index.z = number_of_points_z_ - 1;
			} else {
				index.z = (Index) i;
			}
		}

		return index;
	}


	// get[X|Y|Z]Spacing returns the grid spacing, i.e. the distance
	// between two grid points in the given direction
	template <class GridDataType>
	BALL_INLINE 
	float PointGrid<GridDataType>::getXSpacing(void) const 
	{
		return spacing_.x;
	}

	template <class GridDataType>
	BALL_INLINE 
	float PointGrid<GridDataType>::getYSpacing(void) const 
	{
		return spacing_.y;
	}

	template <class GridDataType>
	BALL_INLINE 
	float PointGrid<GridDataType>::getZSpacing(void) const 
	{
		return spacing_.z;
	}


	template <class GridDataType>
	BALL_INLINE 
	GridIndex PointGrid<GridDataType>::getIndex
		(const float x, const float y, const float z) const 
	{
		GridIndex	index;
		long i;
		
		i = (long)((x - origin_.x) / spacing_.x + 0.5);

		if (i < 0){
			index.x = 0;
		} else {
			if  (i >= (long)number_of_points_x_){
				index.x = number_of_points_x_ - 1;
			} else {
				index.x = (Index) i;
			}
		}

		i = (long)((y - origin_.y) / spacing_.y + 0.5);

		if (i < 0){
			index.y = 0;
		} else {
			if  (i >= (long)number_of_points_y_){
				index.y = number_of_points_y_ - 1;
			} else {
				index.y = (Index) i;
			}
		}

		i = (long)((z - origin_.z) / spacing_.z + 0.5);

		if (i < 0){
					index.z = 0;
		} else {
			if  (i >= (long)number_of_points_z_){
				index.z = number_of_points_z_ - 1;
			} else {
				index.z = (Index) i;
			}
		}

		return index;
	}

	template <class GridDataType>
	BALL_INLINE 
	GridDataType* PointGrid<GridDataType>::getData
		(const Position i, const Position j, const Position k) 
	{

		Position index;

		index = i + j * number_of_points_x_ 
					+ k * number_of_points_x_ * number_of_points_y_;

		if ((index >= (Position)number_of_grid_points_) || (index < 0))
		{
			return 0;
		}

		return &(data[index]);
	}

	template <class GridDataType>
	BALL_INLINE 
	GridDataType* PointGrid<GridDataType>::getData(const Vector3& r) 
	{
		GridIndex	index;
		index = getIndex(r);
		return getData(index.x, index.y, index.z);
	}

	template <class GridDataType>
	BALL_INLINE 
	GridDataType* PointGrid<GridDataType>::getData(const Position index) 
	{
		if (index > (Position)number_of_grid_points_)
		{
			return 0;
		} else {
			return &(data[index]);
		}
	}


	template <class GridDataType>
	GridDataType& PointGrid<GridDataType>::operator[] (const Position index)
	{
		return *(getData(index));
	}

	template <class GridDataType>
	GridDataType& PointGrid<GridDataType>::operator[](const Vector3& v)
	{
		return *(getData(v));
	}

	template <class GridDataType>
	BALL_INLINE 
	Vector3 PointGrid<GridDataType>::getGridCoordinates
		(const Index i, const Index j, const Index k) const 
	{
		Vector3	r;
		
		r.set(origin_.x + i * spacing_.x,
					origin_.y + j * spacing_.y,
					origin_.z + k * spacing_.z);

		return r;
	}


	template <class GridDataType>
	BALL_INLINE 
	Vector3 PointGrid<GridDataType>::getGridCoordinates(const Vector3& v) const 
	{
		Vector3		r;
		GridIndex index;
		
		index.x = (int)((v.x - origin_.x) / spacing_.x);
		index.y = (int)((v.y - origin_.y) / spacing_.y);
		index.z = (int)((v.z - origin_.z) / spacing_.z);

		r.set(origin_.x + index.x * spacing_.x,
					origin_.y + index.y * spacing_.y,
					origin_.z + index.z * spacing_.z);

		return r;
	}

	template <class GridDataType> 
	BALL_INLINE 
	Vector3 PointGrid<GridDataType>::getGridCoordinates(const Index index) const 
	{
		Vector3	r;
		Index		x, y, z;
		
		x = index % number_of_points_x_;
		y = (index % (number_of_points_x_ * number_of_points_y_)) / number_of_points_x_;
		z = index / (number_of_points_x_ * number_of_points_y_);

		r.set(origin_.x + (float)x * spacing_.x,
						 origin_.y + (float)y * spacing_.y,
						 origin_.z + (float)z * spacing_.z);

		return r;
	}

	template <class GridDataType> 
	BALL_INLINE
	const Vector3& PointGrid<GridDataType>::getOrigin(void) const 
	{
		return origin_;
	}

	template <class GridDataType> 
	BALL_INLINE
	Vector3& PointGrid<GridDataType>::getOrigin(void) 
	{
		return origin_;
	}

	template <class GridDataType>
	bool PointGrid<GridDataType>::setOrigin(const Vector3& origin)
	{
		origin_ = origin;

		return true;
	}

	template <class GridDataType>
	bool PointGrid<GridDataType>::setOrigin(const float x, const float y, const float z) {
		origin_.set(x, y, z);

		return true;
	}

	template <class GridDataType> 
	const Vector3& PointGrid<GridDataType>::getDimension(void) const 
	{
		return size_;
	}

	template <class GridDataType> 
	Vector3& PointGrid<GridDataType>::getDimension(void) 
	{
		return size_;
	}


	template <typename GridDataType>
	GridDataType PointGrid<GridDataType>::getInterpolatedValue(const Vector3& vector) const
	{
		Vector3 h(vector - origin_);
		Index x = (int)(h.x / spacing_.x);
		Index y = (int)(h.y / spacing_.y);
		Index z = (int)(h.z / spacing_.z);

		unsigned long Nx = number_of_points_x_;
		unsigned long Nxy = number_of_points_z_ * Nx;
		unsigned long l = x + Nx * y + Nxy * z;
		Vector3 r_0 = getGridCoordinates((Index)l);

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
		
 

} // namespace BALL

#endif // BALL_DATATYPE_POINTGRID_H
