// $Id: molecularSurfaceGrid.C,v 1.3 1999/12/29 01:39:58 oliver Exp $

#include <BALL/SOLVATION/molecularSurfaceGrid.h>

#include <BALL/KERNEL/forEach.h>

namespace BALL {

	// needed for quicksort (below)
	int compareLong_(const void* a, const void* b){
		return (int)(*(long*)a - *(long*)b);
	}

	PointGrid<char> *calculateSESGrid
		(const Vector3& lower, const Vector3& upper,
		 const float spacing, const System& system, 
		 const float probe_radius)
	{

		// points in the grid marked with
		//   CCONN__OUTSIDE are outside of the molecule (default)
		//   CCONN__INSIDE are sure inside the molecule

		#define CCONN__OUTSIDE 1
		#define CCONN__INSIDE  0
		#define	CCONN__INSIDE_PROBE 128

		long* fast_sphere;
		long* fast_sphere_relative;
		
		// contains the squared length of the diagonal distance in the grid
		float	d2, d;

		// contains the squared grid spacing
		float spacing2 = spacing * spacing;

		// the atom_radius
		float atom_radius;
		
		// The radius of the border points around an atom
		float R_b, R_b2;

		// the grid itself...
		PointGrid<char>	*grid;

		// squared distance of any two vectors
		float squared_distance;
		float x, y, z;
		float origin_x, origin_y, origin_z;


		// indices used in between to calculated the bounding boxes of spheres in the grid
		
		PointGrid<char>::GridIndex upper_index, lower_index;


		// pointer to grid data
		char *grid_value;
		
					
		// vector, describing the atom's coordinates
		Vector3 r0;

		// Here we go...
		//
		//

		// First, create the grid...

		grid = new PointGrid<char>(lower, upper, spacing);
		
		if ((grid == 0) || (!grid->isValid()))
		{
			return 0;
		}
				

		// ..then, calculate the grid's diagonal length

		d2 = spacing * spacing * spacing;

		d = sqrt(d2);

		// calculate some constants used for FAST access to the grid.
		//

		origin_x = grid->getMinX();
		origin_y = grid->getMinY();
		origin_z = grid->getMinZ();

		// Nx is the number of points in the grid along the x-axis
		unsigned long Nx;

		// Nxy is the number of points in an xy-plane
		unsigned long Nxy;

		unsigned long i, j, k;

		// assign values for fast grid access
		Nx = grid->getMaxXIndex() + 1;
		Nxy = (grid->getMaxYIndex() + 1) * Nx;

		// constructing the FAST probe sphere, a collection of points 
		// of a sphere on the grid relative to the sphere's origin
		
		unsigned long count, relative_count;


		// the probe_radius (squared) in squared grid units
		unsigned short grid_radius;
		grid_radius = (unsigned short)((probe_radius) * (probe_radius) / spacing2 + 0.5);
		count = 0;
		for (i = 0; i < (unsigned long)(probe_radius / spacing + 2); i++)
			for (j = 0; j < (unsigned long)(probe_radius / spacing + 2); j++)
					for (k = 0; k < (unsigned long)(probe_radius / spacing + 2); k++)
						if ((i * i + j * j + k * k) < grid_radius)
							count++;

		fast_sphere = new long[count * 8];
		fast_sphere_relative = new long[count * 8];

		count = 0;
		for (i = 0; i < (unsigned long)(probe_radius / spacing + 2); i++)
			for (j = 0; j < (unsigned long)(probe_radius / spacing + 2); j++)
					for (k = 0; k < (unsigned long)(probe_radius / spacing + 2); k++)
						if ((i * i + j * j + k * k) < grid_radius){
							fast_sphere[count++] =  (long)k + (long)j * (long)Nx + (long)i * (long)Nxy;
							fast_sphere[count++] = -(long)k + (long)j * (long)Nx + (long)i * (long)Nxy;
							fast_sphere[count++] =  (long)k - (long)j * (long)Nx + (long)i * (long)Nxy;
							fast_sphere[count++] = -(long)k - (long)j * (long)Nx + (long)i * (long)Nxy;
							fast_sphere[count++] =  (long)k + (long)j * (long)Nx - (long)i * (long)Nxy;
							fast_sphere[count++] = -(long)k + (long)j * (long)Nx - (long)i * (long)Nxy;
							fast_sphere[count++] =  (long)k - (long)j * (long)Nx - (long)i * (long)Nxy;
							fast_sphere[count++] = -(long)k - (long)j * (long)Nx - (long)i * (long)Nxy; 
						}

		// now, for speed's sake, we sort the entries in fast_sphere
		qsort(fast_sphere, count, sizeof(long), compareLong_);

		long last_index;
		relative_count = 1;
		last_index = fast_sphere[0];
		// loop variable
		unsigned long u;

		fast_sphere_relative[0] = fast_sphere[0];
		for (u = 1; u < count; u++){
			if (fast_sphere[u] != fast_sphere[u - 1]){
				fast_sphere_relative[relative_count] = fast_sphere[u] - last_index;
				last_index = fast_sphere[u];
				relative_count++;
			}
		}
					
		// throw away the original (unsorted) sphere data
		delete [] fast_sphere;

		// mark the whole grid with CCONN__OUTSIDE, meaning OUTSIDE
		// There will be three different marks: INSIDE(I), OUTSIDE(O), and BORDER(B)
		// BORDER is just used temporarily. In the end, The grid will just contain INSIDEs and OUTSIDEs

		memset((void*) grid->getData(0), CCONN__OUTSIDE, grid->getSize() * sizeof(char));

		// for each atom do...
		AtomIterator	atom_iterator;
		BALL_FOREACH_ATOM(system, atom_iterator){
					
				atom_radius = (*atom_iterator).getRadius();	

				// consider the atom only if it`s radius is large
				// against grid spacing
				if (atom_radius > spacing / 100){

					R_b = atom_radius + probe_radius;
					R_b2 = R_b * R_b;





					r0 = (*atom_iterator).getPosition();
					lower_index = grid->getIndex(r0.x - R_b - d, 
																			 r0.y - R_b - d, 
																			 r0.z - R_b - d);

					upper_index = grid->getIndex(r0.x + R_b + d, 
																			 r0.y + R_b + d, 
																			 r0.z + R_b + d);



					for(k = lower_index.z; k <= upper_index.z; k++)
						for(j = lower_index.y; j <= upper_index.y; j++)
							for(i = lower_index.x; i <= upper_index.x; i++){
								x = (float)i * spacing + origin_x;
								y = (float)j * spacing + origin_y;
								z = (float)k * spacing + origin_z;

								grid_value = &(grid->data[i + Nx * j + Nxy * k]);
																
									
								if (*grid_value != CCONN__INSIDE){
									squared_distance =  (r0.x - x) * (r0.x - x)
																	 +  (r0.y - y) * (r0.y - y)
																	 +  (r0.z - z) * (r0.z - z);

									if (squared_distance <= R_b2){
										*grid_value = CCONN__INSIDE;
									}
								}
							}
				}
			}


		// now check for all points in the grid area that has been changed
		// for points marked as BORDER 
		// If one of these points is found, remove all points marked INSIDE
		// which are less then probe_radius apart from the border point
		// (actually this means to "roll" the probe sphere along the border
		// and just retain points which haven't been touched by the probe sphere)


	 
		register unsigned long idx;
		register long grid_pointer, grid_begin, grid_end;
		register long* fast_sphere_end;
		register long* sphere_pointer;

		unsigned short border;

		unsigned long border_count;
		border_count = 0;
					
		grid_begin = (long)grid->getData(0);
		grid_end = (long)grid->getData(grid->getSize());

		unsigned long s;
		unsigned long t;
		unsigned long q;

		for (s = 1; s < grid->getMaxZIndex(); s++)
			for (t = 1; t < grid->getMaxYIndex(); t++)
				for (q = 1; q < grid->getMaxXIndex(); q++){

					// calculate the absolute grid index the hard way (faster!)
					idx = q + Nx * t + s * Nxy;

					if ((grid->data[idx] & 127) == CCONN__OUTSIDE){
									
						border	= grid->data[idx - 1]
										+ grid->data[idx + 1]
										+ grid->data[idx - Nx]
										+ grid->data[idx + Nx]
										+ grid->data[idx - Nxy]
										+ grid->data[idx + Nxy];

						border &= 127;
					
						if ((border > 0) && (border < 6)){  ///TESTTEST
									
							border_count++;
							// Okay, we found a point on the boundary
												
							grid_pointer = (long)&(grid->data[idx]);
							fast_sphere_end = &(fast_sphere_relative[relative_count - 1]);

							for (sphere_pointer = fast_sphere_relative; sphere_pointer <= fast_sphere_end; sphere_pointer++){
								grid_pointer += *sphere_pointer;
								if ((grid_pointer <= grid_end) && (grid_pointer >= grid_begin))
									*((char*)grid_pointer) |= CCONN__INSIDE_PROBE;
							}
						}
					}
				}

		delete [] fast_sphere_relative;


		// now, check for each point if it is inside
		// A point is outside, if it has not been marked inside
		// (inside the vdW-radius) and has not been marked inside
		// the probe_radius of any border point

		grid_value = grid->getData(0);
		unsigned long l;

		for (l = 0; l < grid->getSize(); l++){
			*grid_value = (*grid_value == 0);
			grid_value++;
		}

		return &(*grid);
	}

	PointGrid<char> *calculateSASGrid(
				const Vector3 &lower, 
				const Vector3 &upper,
				const float spacing,
				const System &system, 
				const float probe_radius)
	{

		// points in the grid marked with
		//   CCONN__OUTSIDE are outside of the molecule (default)
		//   CCONN__INSIDE are sure inside the molecule

		#define CCONN__OUTSIDE 1
		#define CCONN__INSIDE  0

		// contains the squared length of the diagonal distance in the grid
		float	d2, d;

		// the atom_radius
		float atom_radius;
		
		// The radius of the border points around an atom
		float R_b, R_b2;

		// the grid itself...
		PointGrid<char>	*grid;

		// squared distance of any two vectors
		float squared_distance;
		float x, y, z;
		float origin_x, origin_y, origin_z;


		// indices used in between to calculated the bounding boxes of spheres in the grid
		PointGrid<char>::GridIndex upper_index, lower_index;


		// pointer to grid data
		char *grid_value;
		
					
		// vector, describing the atom's coordinates
		Vector3 r0;

					
		// Here we go...
		//
		//

		// First, create the grid...

		grid = new PointGrid<char>(lower, upper, spacing);

		if ((grid == 0) || !grid->isValid())
		{
			throw Exception::OutOfMemory(__FILE__, __LINE__);
		}
				
		

		// ..then, calculate the grid's diagonal length

		d2 = spacing * spacing * spacing;

		d = sqrt(d2);

		// calculate some constants used for FAST access to the grid.
		//

		origin_x = grid->getMinX();
		origin_y = grid->getMinY();
		origin_z = grid->getMinZ();

		// Nx is the number of points in the grid along the x-axis
		unsigned long Nx;

		// Nxy is the number of points in an xy-plane
		unsigned long Nxy;

		// assign values for fast grid access
		Nx = grid->getMaxXIndex() + 1;
		Nxy = (grid->getMaxYIndex() + 1) * Nx;

		// mark the whole grid with CCONN__OUTSIDE, meaning OUTSIDE
		// There will be three different marks: INSIDE(I), OUTSIDE(O), and BORDER(B)
		// BORDER is just used temporarily. In the end, The grid will just contain INSIDEs and OUTSIDEs

		memset((void*) grid->getData(0), CCONN__OUTSIDE, grid->getSize() * sizeof(char));

		// for each atom do...
		AtomIterator	atom_iterator;
		BALL_FOREACH_ATOM(system, atom_iterator){
					
				atom_radius = (*atom_iterator).getRadius();	

				// consider the atom only if it`s radius is large
				// against grid spacing
				if (atom_radius > spacing / 100){

					R_b = atom_radius + probe_radius;
					R_b2 = R_b * R_b;





					r0 = (*atom_iterator).getPosition();
					lower_index = grid->getIndex(r0.x - R_b - d, 
																			 r0.y - R_b - d, 
																			 r0.z - R_b - d);

					upper_index = grid->getIndex(r0.x + R_b + d, 
																			 r0.y + R_b + d, 
																			 r0.z + R_b + d);



					for(unsigned long k = lower_index.z; k <= upper_index.z; k++)
						for(unsigned long j = lower_index.y; j <= upper_index.y; j++)
							for(unsigned long i = lower_index.x; i <= upper_index.x; i++){
								x = (float)i * spacing + origin_x;
								y = (float)j * spacing + origin_y;
								z = (float)k * spacing + origin_z;

								grid_value = &(grid->data[i + Nx * j + Nxy * k]);
																
									
								if (*grid_value != CCONN__INSIDE){
									squared_distance =  (r0.x - x) * (r0.x - x)
																	 +  (r0.y - y) * (r0.y - y)
																	 +  (r0.z - z) * (r0.z - z);

									if (squared_distance <= R_b2){
										*grid_value = CCONN__INSIDE;
									}
								}
							}
				}
			}

		return &(*grid);
	}

} // namespace BALL
