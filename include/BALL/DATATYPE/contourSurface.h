// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: contourSurface.h,v 1.4 2002/10/24 14:50:08 aubertin Exp $

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

/* cube parameters */
#define NUM_CUBE_PERMS         256
#define NUM_BASIS_CUBES        15
#define NUM_CUBE_VERTICES      8
#define NUM_CUBE_EDGES         12
#define NUM_CUBE_ROTATIONS     24

namespace BALL
{
	typedef pair<Position,Position> KeyType;


	template<>
	HashIndex BALL::Hash(const KeyType& p);

static int init_facet_data[NUM_BASIS_CUBES][NUM_CUBE_EDGES] = {
	  /* 7654 3210 (corners markers) */
	  
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
		/* 0001 1101b */   { 4, 7,11,   1, 4,11,   1, 0, 4,   1,11,10,},
};

static int init_facet_index[NUM_BASIS_CUBES][NUM_CUBE_VERTICES] = {
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

/***************************************************************************
 *   FACET_DATA: Holds the facet information for all 256 permutations of the 
 *                 cube. 
 *****************************************************************************/
static int facet_data[NUM_CUBE_PERMS][NUM_CUBE_EDGES];

/***************************************************************************
 *   EDGE_DATA: Contains the same information as FACET_DATA, but multiple 
 *              occurrences of vertices are removed and the edges are ordered 
 *              in the following order: 
 *               11,2,1,  0,4,6,  3,7,5,  8,9,10
 *****************************************************************************/
	/*static int edge_data[NUM_CUBE_PERMS][NUM_CUBE_EDGES];
static int edge_list[NUM_CUBE_EDGES]={11,2,1, 0,4,6, 3,7,5, 8,9,10};
	*/

/***************************************************************************
 *   EDGE_ROTATION and CORNER_ROTATION: 
 *   These tables contain the edge and corner positions for every possible 
 *   cube rotation, including the non-rotated position.
 *   The total number of cube rotations possible
 *   = 6 faces * 4 positions per face = 24 entries.
 *****************************************************************************/

static int edge_rotation[NUM_CUBE_ROTATIONS][NUM_CUBE_EDGES] = {
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

static int corner_rotation[NUM_CUBE_ROTATIONS][NUM_CUBE_VERTICES] = {
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

	

  /** This class is intended to store a single contour surface generated from a RegularData3D - class.
      {\bf Definition:} \URL{BALL/DATATYPE/contourLine.h}
  */
  template <typename T>  
  class TContourSurface : public Surface
  {
    public:

      /** @name Type definitions
       */
      //@{
      
      /** The point type.
	        This type is used to store points in the 3-d regularData.
      */
      typedef Vector3 PointType;
      
      /** The vector type.
	        This type is used to store the edgepoints of the contour-Surface.
      */
      typedef std::vector<pair<PointType,pair<Position,Position> > > VectorType;
      //@}

      /** @name Constructors and Destructors.
       */
      //@{

      
      /// Default constructor
      TContourSurface(T height = 0);

      /// Copy constructor
      TContourSurface(const TContourSurface& copyTContourSurface);

      /// Destructor
      virtual ~TContourSurface();
      //@}

      /// Creates a contour surface from a given data set.
      void createContourSurface(TRegularData3D<T>& from);

      /** @name Assignment
       */
      //@{
      
      /// Assignment operator
      const TContourSurface& operator = (const TContourSurface& assigTContourSurface);

      /// Clear method
      virtual void clear();
      //@}

      /** @name Predicates
       */
      //@{

      /// Equality operator
      bool operator == (const TContourSurface& compTContourSurface) const;

      //@}

      /** @name Accessors
       */
      //@{

      /** Return the next endpoint.
       */
      bool getNextPoint(PointType &p);

      /** Reset the counter.
       */
      void resetCounter();

      //@}

		  void generateFacetData();


		  void computeTriangles(int topology, RegularData3D& from);

      

      //  private:
      T height_;
			unsigned int data_[3];
      Size number_of_cells_x;
      Size number_of_cells_y;
			Size number_of_cells_z;
      Position act_cell_x;
      Position act_cell_y;
	    Position act_cell_z;
		  PointType currentCoords;
      Position currentPosition;
      double d1, d2, slope;
      HashMap<pair<Position,Position>, unsigned int> h;
		  unsigned int size, pos;
    };

    /** Default type
     */
    typedef TContourSurface<float> ContourSurface;

    template <typename T>
      TContourSurface<T>::TContourSurface(T height)
			: height_(height)
    {
    }

    template <typename T>
    TContourSurface<T>::~TContourSurface()
    {
    }

    template <typename T>
    TContourSurface<T>::TContourSurface(const TContourSurface<T>& from)
      : height_(from.height_),
        data_(from.data_)
    {
    }

    template <typename T>
    void TContourSurface<T>::clear()
    {
    }

    template <typename T>
    const TContourSurface<T>& TContourSurface<T>::operator = (const TContourSurface<T>& data)
    {
      data_ = data.data_;
      height_ = data.height_;
			return *this;
    }

    template <typename T>
    bool TContourSurface<T>:: operator == (const TContourSurface<T>& data) const
    {
      return ((height_    == data.height_)
               && (data_  == data.data_));
    }

    template <typename T>
    void TContourSurface<T>::createContourSurface(TRegularData3D<T>& from)
    {
      // This function the "marching cubes" algorithm to determine the contour-surfaces.
      double threshold = height_;
      PointType origin = from.getOrigin();
      float x_spacing = from.getXSpacing();
      float y_spacing = from.getYSpacing();
      float z_spacing = from.getZSpacing();
      number_of_cells_x = (Size) from.getMaxXIndex()+1;
      number_of_cells_y = (Size) from.getMaxYIndex()+1;
	    number_of_cells_z = (Size) from.getMaxZIndex()+1;
      size=0;
      currentCoords = origin;
      currentPosition = 0;
      
      generateFacetData();
    

      for (act_cell_z = 0; act_cell_z < number_of_cells_z-1; act_cell_z++)
			{ 
				for (act_cell_y = 0; act_cell_y < number_of_cells_y-1; act_cell_y++)
				{
					for (act_cell_x = 0; act_cell_x < number_of_cells_x-1; act_cell_x++)
					{
					// First we have to find out the topology of the actual cube.
					int topology = 0;
           if (from[currentPosition + number_of_cells_y*number_of_cells_x] > threshold)
					{
						topology |= 1;
					}
					if (from[currentPosition] > threshold)
					{
						topology |= 2;
					}
					if (from[currentPosition + 1] > threshold)
					{
						topology |= 4;
					}
          if (from[currentPosition + 1 + number_of_cells_y*number_of_cells_x] > threshold)
					{
						topology |= 8;
					}
          if (from[currentPosition + (number_of_cells_y+1)*number_of_cells_x] > threshold)
					{
						topology |= 16;
					}
          if (from[currentPosition + number_of_cells_x ] > threshold)
					{
						topology |= 32;
					}
          if (from[currentPosition + 1 + number_of_cells_x] > threshold)
					{
						topology |= 64;
					}
          if (from[currentPosition + 1 + (number_of_cells_y+1)*number_of_cells_x] > threshold)
					{
						topology |= 128;
					}
          
					// now we can use this information to compute the intersection points of the current cube.
          
					computeTriangles(topology, from);
					currentCoords.x+=x_spacing;
          currentPosition++;
					}
          currentCoords.x=origin.x;
          currentCoords.y+=y_spacing;
          currentPosition++;
				}
        currentCoords.x=origin.x;
        currentCoords.y=origin.y;
        currentCoords.z+=z_spacing;
        currentPosition+=number_of_cells_y;
			}
      unsigned int i=0;
      double l;
			
      // normalize each vertex normal		
      for (i=0;i<normal.size();i++)
				{
          l=normal[i].getLength();
          //cout<<"Laenge Punktnormale "<<i<<": "<<l<<" Vektor: "<<normal[i]<<endl;
					if (l!=0) 
          {
          normal[i]/=l;
          }
				}
      Log.info()<<"Number of triangles generated: "<<triangle.size()<<endl;
      Log.info()<<"consisting of "<<vertex.size()<<" vertices"<<endl;
		}

template<typename T>
void TContourSurface<T>::generateFacetData() {
  int i,j,k,l,cube_index;
  for (i=0; i<NUM_CUBE_PERMS; i++) 
    {                           /* i is the targeted cube_index */
      for (j=0; j<NUM_BASIS_CUBES; j++) 
        {                       /* scan each of the NUM_BASIS_CUBES possible basis cubes */
          for (k=0; k<NUM_CUBE_ROTATIONS; k++) 
            {                   /* and every possible rotation for those cubes */
              /* first determine if we have a matching cube_index */
              cube_index=0;
              for (l=0; l<NUM_CUBE_VERTICES; l++) 
                {
                  if (init_facet_index[j][l]) 
                    cube_index |= (1 << corner_rotation[k][l]);
                }
          
              if (cube_index==i) 
                {               /* we have located a matching cube_index */
                  for (l=0; l<NUM_CUBE_EDGES; l++) 
                    {
                      if (init_facet_data[j][l]>=0) 
                        { 
                          if(height_>0)
														{
                          facet_data[i][11-l] = edge_rotation[k][init_facet_data[j][l]];
                          facet_data[0xFF-i][l] = edge_rotation[k][init_facet_data[j][l]];
														}
                          else
                            {
                          facet_data[i][l] = edge_rotation[k][init_facet_data[j][l]];
                          facet_data[0xFF-i][11-l] = edge_rotation[k][init_facet_data[j][l]];
													}
                        }
                      else 
                        { 
                          if(height_>0)
														{
                          facet_data[i][11-l] = (-1);
                          facet_data[0xFF-i][l] = (-1);
														}
                         else
                            {
                          facet_data[i][l] = (-1);
                          facet_data[0xFF-i][11-l] = (-1);
													}
                        }
                    }
                }
            }
        }
    }
}


template <typename T>
void TContourSurface<T>::computeTriangles(int topology, RegularData3D& from) { 
	
  PointType vec,h1,h2;
  float x_spacing = from.getXSpacing();
  float y_spacing = from.getYSpacing();
  float z_spacing = from.getZSpacing();
  Triangle t;
  Normal normale;
  pos=0;
  double threshold = height_;
  KeyType key;

    for(int i=0;i<12;i++) { 
      vec=currentCoords;
			switch(facet_data[topology][i])
				{
        case -1: break;
        case 0:
					  key=pair<Position,Position>(currentPosition,currentPosition+number_of_cells_x*number_of_cells_y);
            d1  = from[currentPosition];
            d2  = from[currentPosition + number_of_cells_y*number_of_cells_x];
            slope = (d2 - d1) / z_spacing;
            vec.z += (threshold - d1)/slope;
						break;

        case 1: 
					  key=pair<Position,Position>(currentPosition,currentPosition+1);
						d1  = from[currentPosition];
						d2  = from[currentPosition + 1];
						slope = (d2 - d1) / x_spacing;
						vec.x += (threshold - d1)/slope;	
            break;

        case 2:
					  key=pair<Position,Position>(currentPosition+1,currentPosition+1+number_of_cells_x*number_of_cells_y);
            vec.x+=x_spacing;
						d1  = from[currentPosition + 1];
            d2  = from[currentPosition + 1 + number_of_cells_x*number_of_cells_y];
						slope = (d2 - d1) / z_spacing;
						vec.z += (threshold - d1)/slope; 	
						break;

        case 3:
					  key=pair<Position,Position>(currentPosition+number_of_cells_x*number_of_cells_y,currentPosition+1+number_of_cells_x*number_of_cells_y);
            vec.z+=z_spacing;
						d1  = from[currentPosition + number_of_cells_x*number_of_cells_y];
						d2  = from[currentPosition + 1 + number_of_cells_x*number_of_cells_y];
						slope = (d2 - d1) / x_spacing;
						vec.x += (threshold - d1)/slope;
            break;

        case 4:
					  key=pair<Position,Position>(currentPosition+number_of_cells_x,currentPosition+number_of_cells_x*(number_of_cells_y+1));
            vec.y+=y_spacing;
            d1  = from[currentPosition + number_of_cells_x];
            d2  = from[currentPosition + number_of_cells_x*(number_of_cells_y+1)];
            slope = (d2 - d1) / z_spacing;
            vec.z += (threshold - d1)/slope;
            break;

				case 5:
			  		key=pair<Position,Position>(currentPosition+number_of_cells_x,currentPosition+number_of_cells_x+1);
            vec.y+=y_spacing;
            d1  = from[currentPosition + number_of_cells_x];
            d2  = from[currentPosition + 1 + number_of_cells_x];
            slope = (d2 - d1) / x_spacing;
            vec.x += (threshold - d1)/slope;
            break;

				case 6:
					  key=pair<Position,Position>(currentPosition+1+number_of_cells_x,currentPosition+1+number_of_cells_x*(number_of_cells_y+1));
            vec.y+=y_spacing;
            vec.x+=x_spacing;
            d1  = from[currentPosition + 1 + number_of_cells_x];
            d2  = from[currentPosition + 1 + number_of_cells_x*(number_of_cells_y+1)];
            slope = (d2 - d1) / z_spacing;
            vec.z += (threshold - d1)/slope;
						break;

			  case 7:
					  key=pair<Position,Position>(currentPosition+number_of_cells_x*(number_of_cells_y+1),currentPosition+1+number_of_cells_x*(number_of_cells_y+1));
            vec.z+=z_spacing;
						vec.y+=y_spacing;
            d1  = from[currentPosition + number_of_cells_x*(number_of_cells_y+1)];
            d2  = from[currentPosition + 1 + number_of_cells_x*(number_of_cells_y+1)];
            slope = (d2 - d1) / x_spacing;
            vec.x += (threshold - d1)/slope;
						break;


				case 8:
					  key=pair<Position,Position>(currentPosition+number_of_cells_x*number_of_cells_y,currentPosition+number_of_cells_x*(number_of_cells_y+1));
            vec.z+=z_spacing;
            d1  = from[currentPosition + number_of_cells_x*number_of_cells_y];
            d2  = from[currentPosition + number_of_cells_x*(number_of_cells_y+1)];
            slope = (d2 - d1) / y_spacing;
            vec.y += (threshold - d1)/slope; 
            break;

				case 9:
				    key=pair<Position,Position>(currentPosition,currentPosition+number_of_cells_x);
            d1  = from[currentPosition];
            d2  = from[currentPosition + number_of_cells_x];
            slope = (d2 - d1) / y_spacing;
            vec.y += (threshold - d1)/slope;
            break;

			 case 10:
			      key=pair<Position,Position>(currentPosition+1,currentPosition+1+number_of_cells_x);
            vec.x+=x_spacing;
						d1  = from[currentPosition + 1];
            d2  = from[currentPosition + 1 + number_of_cells_x];
						slope = (d2 - d1) / y_spacing;
						vec.y += (threshold - d1)/slope;
            break;

			 case 11:
			      key=pair<Position,Position>(currentPosition+1+number_of_cells_x*number_of_cells_y,currentPosition+1+number_of_cells_x*(number_of_cells_y+1));
            vec.z+=z_spacing;
            vec.x+=x_spacing;
						d1  = from[currentPosition + 1 + number_of_cells_x*number_of_cells_y];
            d2  = from[currentPosition + 1 + number_of_cells_x*(number_of_cells_y+1)];
						slope = (d2 - d1) / y_spacing;
						vec.y += (threshold - d1)/slope;
            break;
						}

			if ((facet_data[topology][i])!=-1)
       {
				 if (h.has(key))
					 {
						 data_[pos]=h[key];
					 }
				 else 
					 {
						 h.insert(pair<KeyType,unsigned int>(key, size));
						 vertex.push_back(vec);
						 normal.push_back(Vector3(0,0,0));
						 data_[pos]=size;
						 size++;
					 }
				 pos++;
				 if (pos==3)
					 {
						 t.v1=data_[0];
						 t.v2=data_[1];
						 t.v3=data_[2];
						 triangle.push_back(t);
						 pos=0;
						 h1=vertex[t.v1]-vertex[t.v2];
						 h2=vertex[t.v3]-vertex[t.v2];
						 normale.x=h1.y*h2.z-h1.z*h2.y;
						 normale.y=h1.z*h2.x-h2.z*h1.x;
						 normale.z=h1.x*h2.y-h1.y*h2.x;
						 normal[t.v1]+=normale;
						 normal[t.v2]+=normale;
						 normal[t.v3]+=normale;
					 }
			 }
    }
}

}
#endif

