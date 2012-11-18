// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_DATATYPE_CONTOURSURFACE_H
#define BALL_DATATYPE_CONTOURSURFACE_H

#ifndef BALL_COMMON_H
# include <BALL/common.h>
#endif

#ifndef BALL_DATATYPE_REGULARDATA3D_H
# include <BALL/DATATYPE/regularData3D.h>
#endif

#ifndef BALL_MATHS_SURFACE_H
# include <BALL/MATHS/surface.h>
#endif

#ifndef BALL_DATATYPE_HASHMAP_H
# include <BALL/DATATYPE/hashMap.h>
#endif

#include <vector>
#include <math.h>

namespace BALL
{
	template<>
	BALL_EXPORT HashIndex Hash(const std::pair<Position, Position>& p);

	// 
	typedef Index FacetArray[256][12];

	// function defined in contourSurface.C to precompute some tables.
	BALL_EXPORT extern const FacetArray& getContourSurfaceFacetData(double threshold);

  /** This class contains a contour surface.
			Contour surfaces are created from 3D (volume) data sets, in general from 
			data sets store in \link RegularData3D RegularData3D \endlink using a 
			marching cube algorithm.
    	\ingroup  DatatypeMiscellaneous
  */
  template <typename T>  
  class TContourSurface 
		: public Surface
  {
    public:

		/** @name Type definitions
		 */
		//@{
		
		/**
		*/
		typedef std::pair<Position, Position> KeyType;

		/** The point type.
				This type is used to store points in the 3-d regularData.
		*/
		typedef Vector3 PointType;
		
		/** The vector type.
				This type is used to store the edge points of the contour-Surface.
		*/
		typedef std::vector<std::pair<PointType, std::pair<Position, Position> > > VectorType;
		//@}

		/** @name Constructors and Destructors.
		 */
		//@{

		/// Default constructor
		TContourSurface();

		/// Constructor with threshold
		TContourSurface(T threshold);

		/// Copy constructor
		TContourSurface(const TContourSurface& surface);

		/// Constructor for TRegularData3D
		TContourSurface(const TRegularData3D<T>& data, T threshold = 0.0);

		/// Destructor
		virtual ~TContourSurface();
		//@}

		/** @name Assignment
		 */
		//@{
		
		/// Assignment operator
		const TContourSurface& operator = (const TContourSurface<T>& surface);

		/// Create a contour surface from a given data set.
		const TContourSurface<T>& operator << (const TRegularData3D<T>& data);

		/// Clear method
		virtual void clear();
		//@}

		/** @name Predicates
		 */
		//@{

		/// Equality operator
		bool operator == (const TContourSurface<T>& surface) const;

		//@}

		
		protected:

		/** A cube in the grid.
				This class is used to extract information from the grid,
				store the values of the eight corners of a cube, and 
				determine the topology and the triangles in the cube.
		*/
		class Cube
		{	
			public:

			Cube(const TRegularData3D<T>& grid)
				:	grid_(&grid),
					current_position_(0),
					ptr_(0),
					spacing_(grid.getSpacing().x, grid.getSpacing().y, grid.getSpacing().z)
			{
				// Retrieve the number of points in the grid along the x- and y-axes.
				Size nx = grid.getSize().x;
				Size ny = grid.getSize().y;

				// Compute the offsets in the grid for the eight 
				// corners of the cube (in absolute grid indices).
				grid_offset_[0] = nx * ny;
				grid_offset_[1] = 0;
				grid_offset_[2] = 1;
				grid_offset_[3] = 1 + nx * ny;
				grid_offset_[4] = nx * ny + nx;
				grid_offset_[5] = nx;
				grid_offset_[6] = nx + 1;
				grid_offset_[7] = nx * ny + nx + 1;
			}

			void setTo(Position p) 
			{
				current_position_ = p;

				ptr_ = &(const_cast<TRegularData3D<T>*>(grid_)->getData(current_position_));
				for (Position i = 0; i < 8; i++)
				{
					values[i] = *(ptr_ + grid_offset_[i]);
				}
			}

			inline Vector3 getOrigin() const
			{
				return grid_->getCoordinates(current_position_);
			}

			inline const Vector3& getSpacing() const
			{
				return spacing_;
			}

			inline Vector3 getCoordinates(Position index) const
			{
				return grid_->getCoordinates(index);
			}

			/// Return the absolute grid position for a given corner
			inline Position getIndex(Position corner) const
			{
				return current_position_ + grid_offset_[corner];
			}

			void shift() 
			{
				// Shift the cube by one along the x-axis.
				current_position_++;
				ptr_++;
				
				// Keep the four old values for x = 1.
				values[0] = values[3];
				values[1] = values[2];
				values[4] = values[7];
				values[5] = values[6];
				
				// Retrieve the four new values.
				values[3] = *(ptr_ + grid_offset_[3]);
				values[2] = *(ptr_ + grid_offset_[2]);
				values[7] = *(ptr_ + grid_offset_[7]);
				values[6] = *(ptr_ + grid_offset_[6]);
			}

			/// Compute the topology code for the current cube.
			Position computeTopology(double threshold)	
			{
				static const Position topology_modifier[8] = {1, 2, 4, 8, 16, 32, 64, 128};

				// The topology is a bitvector constructed by ORing the
				// bits corresponding to each of the inside/outside status of
				// of each individual corner.
				Position topology = 0;
				for (Position i = 0; i < 8; i++)
				{
					topology |= ((values[i] > threshold) ? topology_modifier[i] : 0);
				}
				
				return topology;
			}

			// The values at the eight corners of the cube.
			double										values[8];

			protected:

			// A pointer to the grid.
			const TRegularData3D<T>*	grid_;

			// The current position of the cube as an absolute index into the grid.
			Position									current_position_;

			// The gridd offsets: what to add to current_index_ to get to the correct
			// grid coordinate.
			Position									grid_offset_[8];

			// A pointer into (nasty hack!) the grid itself. For speed's sake.
			const T*									ptr_;
		
			// The spacing of the grid.
			Vector3	spacing_;
		};


		/// 
		void addTriangles_(Cube& cube, const FacetArray& facet_data);

		///
		void computeTriangles(Size topology, const TRegularData3D<T>& data);

		/// The threshold separating inside and outside
		T threshold_;

		//
		HashMap<std::pair<Position, Position>, Position> cut_hash_map_;
	};

	/// Default type
  typedef TContourSurface<float> ContourSurface;

	template <typename T>
	TContourSurface<T>::TContourSurface()
		: threshold_(0.0)
	{
	}

	template <typename T>
	TContourSurface<T>::TContourSurface(T threshold)
		: threshold_(threshold)
	{
	}
   
	template <typename T>
	TContourSurface<T>::TContourSurface(const TRegularData3D<T>& data, T threshold)
		: threshold_(threshold)
	{
		this->operator << (data);
	}
   
	template <typename T>
	TContourSurface<T>::~TContourSurface()	
  {
  }

	template <typename T>
	TContourSurface<T>::TContourSurface(const TContourSurface<T>& from)
		: threshold_(from.threshold_)
	{
  }

	template <typename T>
	void TContourSurface<T>::clear()
	{
		Surface::clear();
		cut_hash_map_.clear();
	}

	template <typename T>
	const TContourSurface<T>& TContourSurface<T>::operator = (const TContourSurface<T>& data)
	{
		// Avoid self-assignment
		if (&data != this)
		{
			threshold_ = data.threshold_;
		}

		return *this;
	}

	template <typename T>
	bool TContourSurface<T>:: operator == (const TContourSurface<T>& data) const
	{
		return ((threshold_    == data.threshold_)
						 && Surface::operator == (data.data_));
	}

  template <typename T>
	const TContourSurface<T>& TContourSurface<T>::operator << (const TRegularData3D<T>& data)
	{
		// Clear the old stuff:
		clear();
		
		
		// Marching cube algorithm: construct a contour surface from
		// a volume data set.

		// Get the dimensions of the volume data set.
		Vector3 origin = data.getOrigin();
		Size number_of_cells_x = (Size)data.getSize().x;
		Size number_of_cells_y = (Size)data.getSize().y;
		Size number_of_cells_z = (Size)data.getSize().z;

		// Precompute the facet data. This depends on the threshold!
		const FacetArray& facet_data = getContourSurfaceFacetData(threshold_);

		// We start in the left-front-bottom-most corner of the grid.
		Position current_index = 0;
		Cube cube(data);
		for (Position curr_cell_z = 0; curr_cell_z < (number_of_cells_z - 1); curr_cell_z++)
		{ 
			// Determine the start position in the current XY plane.
			current_index = curr_cell_z * number_of_cells_y * number_of_cells_x;

			// Walk along the y-axis....
			for (Position curr_cell_y = 0; curr_cell_y < (number_of_cells_y - 1); curr_cell_y++)
			{
				// Retrieve the cube from the current grid position (the first position along
				// along the x-axis).
				cube.setTo(current_index);

				// Walk along the x-axis....
				for (Position curr_cell_x = 0; (curr_cell_x < (number_of_cells_x - 2)); )
				{
					// Compute topology, triangles, and add those triangles to the surface.
					addTriangles_(cube, facet_data);
						
					// Done. cube.shift() will now shift the cube
					// along the x-axis and efficently retrieve the four new values.
					curr_cell_x++;
					cube.shift();
				}

				// Add the triangles from the last cube position.
				addTriangles_(cube, facet_data);
	
				// Shift the cube by one along the y-axis.
				current_index += number_of_cells_x;
			}
		}

		// Normalize the vertex normals.
		for (Position i = 0; i < normal.size(); i++)
		{
			try
			{
				normal[i].normalize();
			}
			catch (...)
			{
			}
		}

		// Return this (stream operator, for command chaining...)
		return *this;
	}

	template <typename T>
	void TContourSurface<T>::addTriangles_
		(typename TContourSurface<T>::Cube& cube, const FacetArray& facet_data)
	{ 
		// Some static variables we need below -- since we will
		// call this rather often, we would rather want to avoid
		// to many ctor/dtor calls.
		static Triangle t;
		static std::pair<Position, Position> key;
		static std::pair<Position, Position> indices;

		// The indices of the corners of a cube's twelve edges.
		static const Position edge_indices[12][2] 
			= {{1, 0}, {1, 2}, {2, 3}, {0, 3}, {5, 4}, {5, 6},
				 {6, 7}, {4, 7}, {0, 4}, {1, 5}, {2, 6}, {3, 7}
				};

		// The index (into Vector3) of the axis along which the
		// current edged runs (0: x, 1: y, 2: z).
		static const Position edge_axis[12] 
			= {2, 0, 2, 0, 2, 0, 2, 0, 1, 1, 1, 1};

		// Retrieve some basic grid properties.
		const Vector3& spacing = cube.getSpacing();

		// The indices (into Surface::vertex) of the triangle
		// under construction.
		TVector3<Position> triangle_vertices;

		// A counter for the number of vertices already in triangle_vertices
		Size vertex_counter = 0;
		
		// Compute the cube's topology
		Position topology = cube.computeTopology(threshold_);
		if (topology == 0)
		{
			return;
		}

		// Iterate over all 12 edges and determine whether
		// there's a cut. 
		for (Position i = 0; i < 12; i++) 
		{ 
			// facet_data_ defines whether there's a cut for
			// a given topology and a given edge.
			Index facet_index = facet_data[topology][i];

			// There's a cut only for values larger than -1
			if (facet_index != -1)
			{	
				// There is a cut -- determine its position along the edge.

				// The axis: x = 0, y = 1, z = 2 -- used as in index into Vector3.
				Position edge = edge_axis[facet_index];

				indices.first = edge_indices[facet_index][0];
				indices.second = edge_indices[facet_index][1];
				key.first = cube.getIndex(indices.first);
				key.second = cube.getIndex(indices.second);

				// Check whether we computed this cut already.
				if (!cut_hash_map_.has(key))
				{
					// Compute the position of the cut.
					
					// Get the position of the d1 point
					Vector3 pos = cube.getCoordinates(key.first);

					// Compute the position of the cut along the edge.
					const double& d1 = cube.values[indices.first];
					const double& d2 = cube.values[indices.second];
					pos[edge] += ((double)threshold_ - d1) / (d2 - d1) * spacing[edge];
					
					// Store it as a triangle vertex.
					triangle_vertices[vertex_counter++] = vertex.size();

					// Store the index of the vertex in the hash map under the
					// indices of its grid points.
					cut_hash_map_.insert(std::pair<std::pair<Position, Position>, Position>(key, (Size)vertex.size()));

					// Create a vertex and a normal (the normal reamins empty for now).
					vertex.push_back(pos);
					static Vector3 null_normal(0.0, 0.0, 0.0);
					normal.push_back(null_normal);
				}
				else
				{
					// This one we know already! Retrieve it from the hash map.
					triangle_vertices[vertex_counter++] = cut_hash_map_[key];
				}
				
				// For every three vertices, create a new triangle.
				if (vertex_counter == 3)
				{
					// Create a new triangle.
					t.v1 = triangle_vertices.x;
					t.v2 = triangle_vertices.y;
					t.v3 = triangle_vertices.z;
					triangle.push_back(t);

					// We can start with the next one.
					vertex_counter = 0;

					// Compute the normals: add the triangle
					// normals to each of the triangle vertices.
					// We will average them out to the correct normals later.
					Vector3 h1(vertex[t.v1] - vertex[t.v2]);
					Vector3 h2(vertex[t.v3] - vertex[t.v2]);
					Vector3 current_normal(h1.y * h2.z - h1.z * h2.y,
																 h1.z * h2.x - h2.z * h1.x,
																 h1.x * h2.y - h1.y * h2.x);
					normal[t.v1] += current_normal;
					normal[t.v2] += current_normal;
					normal[t.v3] += current_normal;
				}
			}
		}
	}
}
#endif

