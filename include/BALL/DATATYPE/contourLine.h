// $Id: contourLine.h,v 1.7 2001/06/06 14:16:09 anhi Exp $

#ifndef BALL_DATATYPE_CONTOURLINE_H
#define BALL_DATATYPE_CONTOURLINE_H

#ifndef BALL_COMMON_H
# include <BALL/common.h>
#endif

#ifndef BALL_DATATYPE_REGULARDATA2D_H
# include <BALL/DATATYPE/regularData2D.h>
#endif

#include <vector>

namespace BALL
{
  // First I define some macros needed for the marching cube-algorithm. 
	// The names come from the number associated with the different corners of the square.
  #define INTERPOL12 { \
						vec = from.getGridCoordinates(act_cell_x, act_cell_y);\
						d1  = from[act_cell_x + act_cell_y*(number_of_cells_x+1)];\
						d2  = from[act_cell_x + 1 + act_cell_y*(number_of_cells_x+1)];\
						vec2 = from.getGridCoordinates(act_cell_x + 1, act_cell_y + 1);\
						slope = (d2 - d1) / (vec2.x - vec.x);\
						vec.x += (threshold - d1)/slope;\
						data_.push_back(vec);\
  } 

  #define INTERPOL18 { \
						vec = from.getGridCoordinates(act_cell_x, act_cell_y);\
						d1  = from[act_cell_x + act_cell_y*(number_of_cells_x+1)];\
						d2  = from[act_cell_x + (act_cell_y+1)*(number_of_cells_x+1)];\
						vec2 = from.getGridCoordinates(act_cell_x, act_cell_y+1);\
						slope = (d2 - d1) / (vec2.y - vec.y);\
						vec.y += (threshold - d1)/slope;\
						data_.push_back(vec);\
  }

  #define INTERPOL24 {  \
            vec = from.getGridCoordinates(act_cell_x+1, act_cell_y);\
            d1  = from[act_cell_x+1 + act_cell_y*(number_of_cells_x+1)];\
            d2  = from[act_cell_x+1 + (act_cell_y+1)*(number_of_cells_x+1)];\
            vec2 = from.getGridCoordinates(act_cell_x+1, act_cell_y+1);\
            slope = (d2 - d1) / (vec2.y - vec.y);\
            vec.y += (threshold - d1)/slope;\
            data_.push_back(vec);\
  }

	// is it vec.x += or vec.y += ...?
  #define INTERPOL48 {  \
				    vec = from.getGridCoordinates(act_cell_x+1, act_cell_y+1);\
            d1  = from[act_cell_x+1 + (act_cell_y+2)*(number_of_cells_x+1)];\
            d2  = from[act_cell_x   + (act_cell_y+1)*(number_of_cells_x+1)];\
            vec2 = from.getGridCoordinates(act_cell_x, act_cell_y+1);\
            slope = (d2 - d1) / (vec2.x - vec.x);\
            vec.x += (threshold - d1)/slope;\
						data_.push_back(vec);\
  }


  /** This class is intended to store a single contour line generated from a RegularData2D - class.
      {\bf Definition:} \URL{BALL/DATATYPE/contourLine.h}
  */
  template <typename T>  
  class TContourLine
  {
    public:

      /** @name Type definitions
       */
      //@{
      
      /** The point type.
	        This type is used to store points in the 2-d regularData.
      */
      typedef Vector2 PointType;
      
      /** The vector type.
	        This type is used to store the endpoints of the contour-line.
      */
      typedef std::vector<PointType> VectorType;
      //@}

      /** @name Constructors and Destructors.
       */
      //@{
      
      /// Default constructor
      TContourLine(T height = 0);

      /// Copy constructor
      TContourLine(const TContourLine& copyTContourLine);

      /// Destructor
      virtual ~TContourLine();
      //@}

      /// Creates a contour line from a given data set.
      void createContourLine(TRegularData2D<T>& from);

      /// Internal functions used for the marching cube-algorithm.
      void interpol12();
      void interpol18();
      void interpol24();
      void interpol48();

      /** @name Assignment
       */
      //@{
      
      /// Assignment operator
      const TContourLine& operator = (const TContourLine& assigTContourLine);

      /// Clear method
      virtual void clear();
      //@}

      /** @name Predicates
       */
      //@{

      /// Equality operator
      bool operator == (const TContourLine& compTContourLine) const;

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

      //  private:
      T height_;
			VectorType data_;
			typename VectorType::iterator it_;
			Position index_;
    };

    /** Default type
     */
    typedef TContourLine<float> ContourLine;

    template <typename T>
      TContourLine<T>::TContourLine(T height)
			: height_(height), 
				index_(0)
    {
    }

    template <typename T>
    TContourLine<T>::~TContourLine()
    {
    }

    template <typename T>
    TContourLine<T>::TContourLine(const TContourLine<T>& from)
      : height_(from.height_),
        data_(from.data_),
				it_(from.it_),
				index_(from.index_)
    {
    }

    template <typename T>
    void TContourLine<T>::clear()
    {
      data_.clear();
      it_=data_.begin();
      index_ = 0;
    }

    template <typename T>
    const TContourLine<T>& TContourLine<T>::operator = (const TContourLine<T>& data)
    {
      data_ = data.data_;
      height_ = data.height_;
      it_ = data.it_;
      index_ = data.index_;

			return *this;
    }

    template <typename T>
    bool TContourLine<T>:: operator == (const TContourLine<T>& data) const
    {
      return ((height_    == data.height_)
               && (data_  == data.data_)
							 && (it_    == data.it_)
							 && (index_ == data.index_));
    }

    template <typename T>
    void TContourLine<T>::createContourLine(TRegularData2D<T>& from)
    {
      // This function uses a "marching cubes"-style algorithm to determine the contour-lines.
      //Size number_of_cells;
      Size number_of_cells_x;
      Size number_of_cells_y;
      Position act_cell_x;
      Position act_cell_y;
      PointType vec, vec2;
      double d1, d2, slope;
      double threshold = height_;

      number_of_cells_x = (Size) from.getMaxXIndex();
      number_of_cells_y = (Size) from.getMaxYIndex();
      //number_of_cells   = number_of_cells_x * number_of_cells_y;
      
      for (act_cell_y = 0; act_cell_y < number_of_cells_y; act_cell_y++)
			{
				for (act_cell_x = 0; act_cell_x < number_of_cells_x; act_cell_x++)
				{
					// First we have to find out the topology of the actual square.
					int topology = 0;
					
					if (from[act_cell_x + act_cell_y*(number_of_cells_x+1)] > threshold)
					{
						topology |= 1;
					}
					if (from[act_cell_x+1 + act_cell_y*(number_of_cells_x+1)] > threshold)
					{
						topology |= 2;
					}
					if (from[act_cell_x+1 + (act_cell_y+1)*(number_of_cells_x+1)] > threshold)
					{
						topology |= 4;
					}
					if (from[act_cell_x + (act_cell_y+1)*(number_of_cells_x+1)] > threshold)
					{
						topology |= 8;
					}
					// now we can use this information to compute the contour-line.
					switch (topology)
					{
						// no cut of contour-line here
						case 0  :
						case 15 : break;
							
						// Line from upper left to lower right
						case 1  : 
						case 14 : INTERPOL18
											INTERPOL12
											break;
						
						case 4  :
						case 11 : INTERPOL48
											INTERPOL24
											break;
						
						// Line from upper right to lower left
						case 2  :
						case 13 : INTERPOL12
											INTERPOL24
											break;

						case 8  :
						case 7  : INTERPOL18
											INTERPOL48
											break;
						
						// Line through the middle (upwards)
						case 9  :
						case 6  : INTERPOL12
											INTERPOL48
											break;

						// Line through the middle (left to right)
						case 3  :
						case 12 : INTERPOL18
											INTERPOL24
											break;

						// Two lines from upper right to lower left
						case 10 : INTERPOL18
											INTERPOL12
											INTERPOL48
											INTERPOL24
											break;
					
						// Two lines from upper left to lower right
						case 5  : INTERPOL12
											INTERPOL24
											INTERPOL18
											INTERPOL48
											break;
					};
				}
			}
      index_ = 0;
      it_ = data_.begin();
    }

    template <typename T>
    bool TContourLine<T>::getNextPoint(TContourLine<T>::PointType &p)
    {
      if (index_ < data_.size()) 
			{
				p = *it_;
				index_++;
				it_++;
				return true;
			} 
			else 
			{
				return false;
			}
    }

    template <typename T>
    void TContourLine<T>::resetCounter()
    {
      it_ = data_.begin();
      index_ = 0;
    }
}
#endif
