// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: contourSurface.C,v 1.6 2003/08/26 09:17:45 oliver Exp $
//

#include <BALL/DATATYPE/contourSurface.h>

namespace BALL
{

	template <>
	HashIndex Hash(const std::pair<Position, Position>& p) 
		
	{
		return (HashIndex)(p.first + p.second);
	}

		
	static Index contour_surface_init_facet_data[15][12] = 
	{
		/* 0000 0000b */   {-1,-1,-1,  -1,-1,-1,  -1,-1,-1,  -1,-1,-1,},
		/* 0100 0000b */   { 6, 5,10,  -1,-1,-1,  -1,-1,-1,  -1,-1,-1,},
		/* 0000 0110b */   { 2,10, 9,   2, 9, 0,  -1,-1,-1,  -1,-1,-1,},
		/* 0100 0010b */   { 6, 5,10,   1, 9, 0,  -1,-1,-1,  -1,-1,-1,},			                     
		/* 0000 1111b */   {10, 9, 8,  10, 8,11,  -1,-1,-1,  -1,-1,-1,},
		/* 0001 1011b */   { 9, 4, 7,   9, 7, 1,   1, 7,11,   1,11, 2,},
		/* 1010 1010b */   { 1, 5, 4,   1, 4, 0,   3, 6, 2,   3, 7, 6,},
		/* 1000 1011b */   { 1, 9, 8,   1, 8, 6,   8, 7, 6,   1, 6, 2,},	
		/* 0100 0001b */   { 6, 5,10,   0, 8, 3,  -1,-1,-1,  -1,-1,-1,},
		/* 0000 1101b */   { 1, 0,10,   0, 8,10,  10, 8,11,  -1,-1,-1,},
		/* 1000 0110b */   {11, 7, 6,  10, 9, 0,  10, 0, 2,  -1,-1,-1,},
		/* 1010 0100b */   { 5, 4, 9,  11, 7, 6,   2,10, 1,  -1,-1,-1,},
		/* 0010 1101b */   { 5, 4, 9,   1, 0,10,  10, 0, 8,  10, 8,11,},
		/* 0101 1010b */   { 4, 7, 8,   6, 5,10,   1, 9, 0,   2, 3,11,},
		/* 0001 1101b */   { 4, 7,11,   1, 4,11,   1, 0, 4,   1,11,10,}
	};

	static Index contour_surface_init_facet_index[15][8] = 
	{
	  /* 0000 0000b */   {0,0,0,0, 0,0,0,0,},
		/* 0100 0000b */   {0,0,0,0, 0,0,1,0,},
		/* 0000 0110b */   {0,1,1,0, 0,0,0,0,},
		/* 0100 0010b */   {0,1,0,0, 0,0,1,0,},
    /* 0000 1111b */   {1,1,1,1, 0,0,0,0,},
		/* 0001 1011b */   {1,1,0,1, 1,0,0,0,},
	  /* 1010 1010b */   {0,1,0,1, 0,1,0,1,},
		/* 1000 1011b */   {1,1,0,1, 0,0,0,1,},
    /* 0100 0001b */   {1,0,0,0, 0,0,1,0,},
		/* 0000 1101b */   {1,0,1,1, 0,0,0,0,},
		/* 1000 0110b */   {0,1,1,0, 0,0,0,1,},
		/* 1010 0100b */   {0,0,1,0, 0,1,0,1,},
		/* 0010 1101b */   {1,0,1,1, 0,1,0,0,},
		/* 0101 1010b */   {0,1,0,1, 1,0,1,0,},
		/* 0001 1101b */   {1,0,1,1, 1,0,0,0,},
	};

	// The facet data for all 256 possible cube topologies
	// for positive and negative thresholds.
	static FacetArray contour_surface_facet_data_positive;
	static FacetArray contour_surface_facet_data_negative;

	static Index contour_surface_edge_rotation[24][12] = 
	{
	  /* face 5,6,2,1 forward */
	  /* 00 */  { 0, 1, 2, 3,   4, 5, 6, 7,   8, 9,10,11,},
		/* 01 */  { 2,10, 6,11,   0, 9, 4, 8,   3, 1, 5, 7,},
		/* 02 */  { 6, 5, 4, 7,   2, 1, 0, 3,  11,10, 9, 8,},
		/* 03 */  { 4, 9, 0, 8,   6,10, 2,11,   7, 5, 1, 3,},

		/* face 6,7,3,2 forward */
		/* 04 */  { 1, 2, 3, 0,   5, 6, 7, 4,   9,10,11, 8,},
		/* 05 */  { 3,11, 7, 8,   1,10, 5, 9,   0, 2, 6, 4,},
		/* 06 */  { 7, 6, 5, 4,   3, 2, 1, 0,   8,11,10, 9,},
		/* 07 */  { 5,10, 1, 9,   7,11, 3, 8,   4, 6, 2, 0,},

		/* face 7,4,0,3 forward */
		/* 08 */  { 2, 3, 0, 1,   6, 7, 4, 5,  10,11, 8, 9,},
		/* 09 */  { 0, 8, 4, 9,   2,11, 6,10,   1, 3, 7, 5,},
		/* 10 */  { 4, 7, 6, 5,   0, 3, 2, 1,   9, 8,11,10,},
		/* 11 */  { 6,11, 2,10,   4, 8, 0, 9,   5, 7, 3, 1,},

	  /* face 4,5,1,0 forward */
		/* 12 */  { 3, 0, 1, 2,   7, 4, 5, 6,  11, 8, 9,10,},
		/* 13 */  { 1, 9, 5,10,   3, 8, 7,11,   2, 0, 4, 6,},
		/* 14 */  { 5, 4, 7, 6,   1, 0, 3, 2,  10, 9, 8,11,},
		/* 15 */  { 7, 8, 3,11,   5, 9, 1,10,   6, 4, 0, 2,},

		/* face 4,7,6,5 forward */
		/* 16 */  { 9, 5,10, 1,   8, 7,11, 3,   0, 4, 6, 2,},
		/* 17 */  {10, 6,11, 2,   9, 4, 8, 0,   1, 5, 7, 3,},
		/* 18 */  {11, 7, 8, 3,  10, 5, 9, 1,   2, 6, 4, 0,},
		/* 19 */  { 8, 4, 9, 0,  11, 6,10, 2,   3, 7, 5, 1,},

		/* face 1,2,3,0 forward */
		/* 20 */  { 8, 3,11, 7,   9, 1,10, 5,   4, 0, 2, 6,},
		/* 21 */  {11, 2,10, 6,   8, 0, 9, 4,   7, 3, 1, 5,},
		/* 22 */  {10, 1, 9, 5,  11, 3, 8, 7,   6, 2, 0, 4,},
		/* 23 */  { 9, 0, 8, 4,  10, 2,11, 6,   5, 1, 3, 7,},
	};

	static Position contour_surface_corner_rotation[24][8] = 
	{
	  /* face 5,6,2,1 forward */
	  /* 00 */  { 0, 1, 2, 3,   4, 5, 6, 7,},
		/* 01 */  { 3, 2, 6, 7,   0, 1, 5, 4,},
		/* 02 */  { 7, 6, 5, 4,   3, 2, 1, 0,},
		/* 03 */  { 4, 5, 1, 0,   7, 6, 2, 3,},

	  /* face 6,7,3,2 forward */
		/* 04 */  { 1, 2, 3, 0,   5, 6, 7, 4,},
		/* 05 */  { 0, 3, 7, 4,   1, 2, 6, 5,},
		/* 06 */  { 4, 7, 6, 5,   0, 3, 2, 1,},
		/* 07 */  { 5, 6, 2, 1,   4, 7, 3, 0,},

		/* face 7,4,0,3 forward */
		/* 08 */  { 2, 3, 0, 1,   6, 7, 4, 5,},
		/* 09 */  { 1, 0, 4, 5,   2, 3, 7, 6,},
		/* 10 */  { 5, 4, 7, 6,   1, 0, 3, 2,},
		/* 11 */  { 6, 7, 3, 2,   5, 4, 0, 1,},

		/* face 4,5,1,0 forward */
		/* 12 */  { 3, 0, 1, 2,   7, 4, 5, 6,},
		/* 13 */  { 2, 1, 5, 6,   3, 0, 4, 7,},
		/* 14 */  { 6, 5, 4, 7,   2, 1, 0, 3,},
		/* 15 */  { 7, 4, 0, 3,   6, 5, 1, 2,},

	  /* face 4,7,6,5 forward */
	  /* 16 */  { 1, 5, 6, 2,   0, 4, 7, 3,},
		/* 17 */  { 2, 6, 7, 3,   1, 5, 4, 0,},
		/* 18 */  { 3, 7, 4, 0,   2, 6, 5, 1,},
		/* 19 */  { 0, 4, 5, 1,   3, 7, 6, 2,},

		/* face 1,2,3,0 forward */
		/* 20 */  { 4, 0, 3, 7,   5, 1, 2, 6,},
		/* 21 */  { 7, 3, 2, 6,   4, 0, 1, 5,},
		/* 22 */  { 6, 2, 1, 5,   7, 3, 0, 4,},
		/* 23 */  { 5, 1, 0, 4,   6, 2, 3, 7,},
	};
	
	bool contour_surface_facet_data_initialized = false;
	
	const FacetArray& getContourSurfaceFacetData(double threshold) 
	{
		if (!contour_surface_facet_data_initialized)
		{
			// We will do this just once. And then never again!
			contour_surface_facet_data_initialized = true;

			for (Index i = 0; i < 256; i++) 
			{                           /* i is the targeted cube_index */
				for (Index j = 0; j < 15; j++) 
				{                       /* scan each of the 15 possible basis cubes */
					for (Index k = 0; k < 24; k++) 
					{                   /* and every possible rotation for those cubes */
						/* first determine if we have a matching cube_index */
						Index cube_index = 0;
						for (Index l = 0; l < 8; l++) 
						{
							if (contour_surface_init_facet_index[j][l]) 
							{
								cube_index |= (1 << contour_surface_corner_rotation[k][l]);
							}
						}
						if (cube_index == i) 
						{               /* we have located a matching cube_index */
							for (Index l = 0; l < 12; l++) 
							{
								if (contour_surface_init_facet_data[j][l] >= 0) 
								{ 
									// (threshold_ > 0.0) -> index = 0
									contour_surface_facet_data_positive[i][11 - l] = contour_surface_edge_rotation[k][contour_surface_init_facet_data[j][l]];
									contour_surface_facet_data_positive[255 - i][l] = contour_surface_edge_rotation[k][contour_surface_init_facet_data[j][l]];

									contour_surface_facet_data_negative[i][l] = contour_surface_edge_rotation[k][contour_surface_init_facet_data[j][l]];
									contour_surface_facet_data_negative[255 - i][11 - l] = contour_surface_edge_rotation[k][contour_surface_init_facet_data[j][l]];
								}
								else 
								{ 
									contour_surface_facet_data_positive[i][11 - l] = -1;
									contour_surface_facet_data_positive[255 - i][l] = -1;
									contour_surface_facet_data_negative[i][l] = -1;
									contour_surface_facet_data_negative[255 - i][11 - l] = -1;
								}
							}
						}
					}
				}
			}
		}
		
		// Return the correct facet data set for the given threshold.
		if (threshold > 0.0)
		{
			return contour_surface_facet_data_positive;
		}
		else
		{
			return contour_surface_facet_data_negative;
		}
	}
}
