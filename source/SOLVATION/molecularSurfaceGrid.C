// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: molecularSurfaceGrid.C,v 1.24 2005/12/23 17:03:03 amoll Exp $
//

#include <BALL/SOLVATION/molecularSurfaceGrid.h>
#include <BALL/KERNEL/forEach.h>

namespace BALL 
{
	const char CCONN__INSIDE = 0;
	const char CCONN__OUTSIDE = 1;
	const char CCONN__INSIDE_PROBE = (char) 64;

	// needed for quicksort (below)
	extern "C" int 
	#ifdef BALL_COMPILER_MSVC
	__cdecl 
	#endif
	comparePointerSizeIntPtr_(const void* a, const void* b)
	{
		return (int)(*(PointerSizeUInt*)a - *(PointerSizeUInt*)b);
	}



	TRegularData3D<char>* calculateSESGrid
		(const Vector3& lower, const Vector3& upper,
		 float spacing, const System& system, 
		 float probe_radius)
	{

		// points in the grid marked with

		PointerSizeUInt* fast_sphere;
		PointerSizeUInt* fast_sphere_relative;
		
		// contains the squared length of the diagonal distance in the grid
		float d;

		// contains the squared grid spacing
		float spacing2 = spacing * spacing;

		// the atom_radius
		float atom_radius;
		
		// The radius of the border points around an atom
		float R_b, R_b2;

		// the grid itself...
		TRegularData3D<char>	*grid;

		// squared distance of any two vectors
		float squared_distance;
		float x, y, z;
		Vector3 origin;

		// indices used in between to calculated the bounding boxes of spheres in the grid
		TRegularData3D<char>::IndexType upper_index;
		TRegularData3D<char>::IndexType lower_index;

		// pointer to grid data
		char* grid_value;
		
		// vector, describing the atom's coordinates
		Vector3 r0;

		// Here we go...
		// First, create the grid...
		grid = new TRegularData3D<char>(lower, upper - lower, Vector3(spacing));
		
		if (grid == 0)
		{
			return 0;
		}

		// ..then, calculate the grid's diagonal length
		d = sqrt(3.f) * spacing;

		// calculate some constants used for FAST access to the grid.
		origin = grid->getOrigin();

		// Nx is the number of points in the grid along the x-axis
		PointerSizeUInt Nx = grid->getSize().x;

		// Nxy is the number of points in an xy-plane
		PointerSizeUInt Nxy = Nx * grid->getSize().y;

		PointerSizeUInt i;
		PointerSizeUInt j;
		PointerSizeUInt k;

		// constructing the FAST probe sphere, a collection of points 
		// of a sphere on the grid relative to the sphere's origin
		
		PointerSizeUInt count;
		PointerSizeUInt relative_count;

		// the probe_radius (squared) in squared grid units
		unsigned short grid_radius;
		grid_radius = (unsigned short)((probe_radius) * (probe_radius) / spacing2 + 0.5);
		count = 0;
		for (i = 0; i < (PointerSizeUInt)(probe_radius / spacing + 2); i++)
		{
			for (j = 0; j < (PointerSizeUInt)(probe_radius / spacing + 2); j++)
			{
				for (k = 0; k < (PointerSizeUInt)(probe_radius / spacing + 2); k++)
				{
					if ((i * i + j * j + k * k) < grid_radius)
					{
						count++;
					}
				}
			}
		}

		fast_sphere = new PointerSizeUInt[count * 8];
		fast_sphere_relative = new PointerSizeUInt[count * 8];

		count = 0;
		for (i = 0; i < (PointerSizeUInt)(probe_radius / spacing + 2); i++)
			for (j = 0; j < (PointerSizeUInt)(probe_radius / spacing + 2); j++)
					for (k = 0; k < (PointerSizeUInt)(probe_radius / spacing + 2); k++)
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
		qsort(fast_sphere, count, sizeof(long), comparePointerSizeIntPtr_);

		long last_index;
		relative_count = 1;
		last_index = fast_sphere[0];
		// loop variable
		PointerSizeUInt u;

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

		memset((void*)&grid->getData(0), CCONN__OUTSIDE, grid->size() * sizeof(char));

		// for each atom do...
		AtomConstIterator	atom_iterator;
		BALL_FOREACH_ATOM(system, atom_iterator)
		{
			atom_radius = (*atom_iterator).getRadius();	

			// consider the atom only if it`s radius is large
			// against grid spacing
			if (atom_radius > spacing / 100)
			{
				R_b = atom_radius + probe_radius;
				R_b2 = R_b * R_b;
				
				r0 = atom_iterator->getPosition();
				lower_index = grid->getClosestIndex(Vector3(r0.x - R_b - d, r0.y - R_b - d, r0.z - R_b - d));
				upper_index = grid->getClosestIndex(Vector3(r0.x + R_b + d, r0.y + R_b + d, r0.z + R_b + d));

				char* data = &(grid->getData(0));
				for(k = lower_index.z; k <= upper_index.z; k++)
					for(j = lower_index.y; j <= upper_index.y; j++)
						for(i = lower_index.x; i <= upper_index.x; i++){
							x = (float)i * spacing + origin.x;
							y = (float)j * spacing + origin.y;
							z = (float)k * spacing + origin.z;

							grid_value = &(data[i + Nx * j + Nxy * k]);
															
								
							if (*grid_value != CCONN__INSIDE)
							{
								squared_distance =  (r0.x - x) * (r0.x - x)
																 +  (r0.y - y) * (r0.y - y)
																 +  (r0.z - z) * (r0.z - z);
								if (squared_distance <= R_b2)
								{
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
		PointerSizeUInt idx;
		PointerSizeUInt grid_pointer, grid_begin, grid_end;
		PointerSizeUInt* fast_sphere_end;
		PointerSizeUInt* sphere_pointer;

		unsigned short border;

		PointerSizeUInt border_count;
		border_count = 0;
					
		grid_begin = (PointerSizeUInt)grid->getData(0);
		grid_end = (PointerSizeUInt)grid->getData(grid->size()-1);

		PointerSizeUInt s;
		PointerSizeUInt t;
		PointerSizeUInt q;

		char* data = &(grid->getData(0));
		for (s = 1; s < grid->getSize().z - 1; s++)
		{
			for (t = 1; t < grid->getSize().y - 1; t++)
			{
				for (q = 1; q < grid->getSize().x - 1; q++)
				{
					// calculate the absolute grid index the hard way (faster!)
					idx = q + Nx * t + s * Nxy;

					if ((data[idx] & 127) == CCONN__OUTSIDE)
					{
						border	= data[idx - 1]
										+ data[idx + 1]
										+ data[idx - Nx]
										+ data[idx + Nx]
										+ data[idx - Nxy]
										+ data[idx + Nxy];

						border &= 127;
					
						if ((border > 0) && (border < 6))
						{			
							// Okay, we found a point on the boundary
							border_count++;												
							grid_pointer = reinterpret_cast<PointerSizeUInt>(&(data[idx]));
							fast_sphere_end = &(fast_sphere_relative[relative_count - 1]);

							for (sphere_pointer = fast_sphere_relative; sphere_pointer <= fast_sphere_end; sphere_pointer++)
							{
								grid_pointer += static_cast<PointerSizeUInt>(*sphere_pointer);
								if ((grid_pointer <= grid_end) && (grid_pointer >= grid_begin))
								{
									*((char*)grid_pointer) |= CCONN__INSIDE_PROBE;
								}
							}
						}
					}
				}
			}
		}

		delete [] fast_sphere_relative;


		// now, check for each point if it is inside
		// A point is outside, if it has not been marked inside
		// (inside the vdW-radius) and has not been marked inside
		// the probe_radius of any border point

		grid_value = &(grid->getData(0));
		PointerSizeUInt l;

		for (l = 0; l < grid->size(); l++)
		{
			*grid_value = (*grid_value == 0);
			grid_value++;
		}

		return &(*grid);
	}

	TRegularData3D<char>* calculateSASGrid
		(const Vector3& lower, const Vector3& upper,
		 float spacing, const System& system, float probe_radius)
	{

		// points in the grid marked with
		// CCONN__OUTSIDE are outside of the molecule (default)
		// CCONN__INSIDE are sure inside the molecule

		// contains the squared length of the diagonal distance in the grid
		float d;

		// the atom_radius
		float atom_radius;
		
		// The radius of the border points around an atom
		float R_b, R_b2;

		// the grid itself...
		TRegularData3D<char>	*grid;

		// squared distance of any two vectors
		float squared_distance;
		float x, y, z;
		Vector3 origin;


		// indices used in between to calculated the bounding boxes of spheres in the grid
		TRegularData3D<char>::IndexType upper_index;
		TRegularData3D<char>::IndexType lower_index;

		// pointer to grid data
		char *grid_value;
					
		// vector, describing the atom's coordinates
		Vector3 r0;
					
		// Here we go...
		// First, create the grid...

		grid = new TRegularData3D<char>(lower, upper - lower, Vector3(spacing));

		if (grid == 0)
		{
			throw Exception::OutOfMemory(__FILE__, __LINE__);
		}
				
		// ..then, calculate the grid's diagonal length
		d = sqrt(3.f) * spacing;

		// calculate some constants used for FAST access to the grid.
		origin = grid->getOrigin();

		// Nx is the number of points in the grid along the x-axis
		PointerSizeUInt Nx = grid->getSize().x;

		// Nxy is the number of points in an xy-plane
		PointerSizeUInt Nxy = Nx * grid->getSize().y;

		// mark the whole grid with CCONN__OUTSIDE, meaning OUTSIDE
		// There will be three different marks: INSIDE(I), OUTSIDE(O), and BORDER(B)
		// BORDER is just used temporarily. In the end, The grid will just contain INSIDEs and OUTSIDEs

		memset((void*)&grid->getData(0), CCONN__OUTSIDE, grid->size() * sizeof(char));

		// for each atom do...	
		char* data = &(grid->getData(0));
		AtomConstIterator	atom_iterator;
		BALL_FOREACH_ATOM(system, atom_iterator)
		{
			atom_radius = (*atom_iterator).getRadius();	

			// consider the atom only if it`s radius is large
			// against grid spacing
			if (atom_radius > spacing / 100)
			{
				R_b = atom_radius + probe_radius;
				R_b2 = R_b * R_b;

				r0 = (*atom_iterator).getPosition();
				lower_index = grid->getClosestIndex(Vector3(r0.x - R_b - d, r0.y - R_b - d, r0.z - R_b - d));

				upper_index = grid->getClosestIndex(Vector3(r0.x + R_b + d, r0.y + R_b + d, r0.z + R_b + d));

				for (PointerSizeUInt k = lower_index.z; k <= upper_index.z; k++)
				{
					for (PointerSizeUInt j = lower_index.y; j <= upper_index.y; j++)
					{
						for (PointerSizeUInt i = lower_index.x; i <= upper_index.x; i++)
						{
							x = (float)i * spacing + origin.x;
							y = (float)j * spacing + origin.y;
							z = (float)k * spacing + origin.z;

							grid_value = &(data[i + Nx * j + Nxy * k]);
															
							if (*grid_value != CCONN__INSIDE)
							{
								squared_distance =  (r0.x - x) * (r0.x - x)
																 +  (r0.y - y) * (r0.y - y)
																 +  (r0.z - z) * (r0.z - z);

								if (squared_distance <= R_b2)
								{
									*grid_value = CCONN__INSIDE;
								}
							}
						}
					}
				}
			}
		}

		return &(*grid);
	}

} // namespace BALL
