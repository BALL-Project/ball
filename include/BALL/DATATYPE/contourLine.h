// $Id: contourLine.h,v 1.1 2000/11/28 17:21:58 anhi Exp $

#ifndef BALL_DATATYPE_CONTOURLINE_H
#define BALL_DATATYPE_CONTOURLINE_H

#ifndef BALL_COMMON_H
#       include <BALL/common.h>
#endif

#ifndef BALL_DATATYPE_REGULARDATA2D_H
#       include <BALL/DATATYPE/regularData2D.h>
#endif

#include <vector>


namespace BALL
{
  // First I define some macros needed for the marching cube-algorithm. The names come from the number associated with
  // the different corners of the square.
  #define INTERPOL12 { \
            from.getConvertedPosition(act_cell_x, act_cell_y, dummy);\
            d1 = from[act_cell_x + act_cell_y*(number_of_cells_x+1)];\
            d2 = from[act_cell_x + 1 + act_cell_y*(number_of_cells_x+1)];\
            from.getConvertedPosition(act_cell_x + 1, act_cell_y + 1, dummy2);\
            slope = (d2 - d1) / (dummy2.first - dummy.first);\
            dummy.first += (threshold - d1)/slope;\
            data_.push_back(dummy);\
      } 

   #define INTERPOL18 { \
             from.getConvertedPosition(act_cell_x, act_cell_y, dummy);\
             d1 = from[act_cell_x + act_cell_y*(number_of_cells_x+1)];\
   	     d2 = from[act_cell_x + (act_cell_y+1)*(number_of_cells_x+1)];\
             from.getConvertedPosition(act_cell_x, act_cell_y+1, dummy2);\
             slope = (d2 - d1) / (dummy2.second - dummy.second);\
             dummy.second += (threshold - d1)/slope;\
             data_.push_back(dummy);\
       }

  #define INTERPOL24 {  \
            from.getConvertedPosition(act_cell_x+1, act_cell_y, dummy);\
            d1 = from[act_cell_x+1 + act_cell_y*(number_of_cells_x+1)];\
            d2 = from[act_cell_x+1 + (act_cell_y+1)*(number_of_cells_x+1)];\
            from.getConvertedPosition(act_cell_x+1, act_cell_y+1, dummy2);\
            slope = (d2 - d1) / (dummy2.second - dummy.second);\
            dummy.second += (threshold - d1)/slope;\
            data_.push_back(dummy);\
      }

  #define INTERPOL48 {  \
	    from.getConvertedPosition(act_cell_x+1, act_cell_y+1, dummy);\
            d1 = from[act_cell_x+1 + (act_cell_y+1)*(number_of_cells_x+1)];\
            d2 = from[act_cell_x   + (act_cell_y+1)*(number_of_cells_x+1)];\
            from.getConvertedPosition(act_cell_x, act_cell_y+1, dummy2);\
            slope = (d2 - d1) / (dummy2.first - dummy.first);\
            dummy.second += (threshold - d1)/slope;\
            data_.push_back(dummy);\
      }


  /** This class is intended to store a single contour line generated from a RegularData2D - class.
      {\bf Definition:} \URL{BALL/DATATYPE/contourLine.h}
  */
  template <typename T>  
  class TContourLine
    {
    public:

      /**    @name Type definitions
       */
      //@{
      /**    The point type.
	             This type is used to store points in the 2-d regularData.
      */
      typedef std::pair<T, T> PointType;
      
      /**    The vector type.
	             This type is used to store the endpoints of the contour-line.
      */
      typedef std::vector<PointType> VectorType;
      //@}

      /** @name Constructors and Destructors.
       */
      //@{
      /// Default constructor
      TContourLine(T height);

      /// Copy constructor
      TContourLine(const TContourLine& copyTContourLine);

      /// Destructor
      virtual ~TContourLine();
      //@}

      /// Creates a contour line from a given data set.
      void createContourLine(const TRegularData2D<T>& from);

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

      /**    @name Accessors
       */
      //@{

      /**    Return the next endpoint.
       */
      bool getNextPoint(PointType &p);

      private:
        T height_;
	VectorType data_;
	VectorType::iterator it;
    };

    /**    Default type
     */
    typedef TContourLine<float> ContourLine;

    template <typename T>
    TContourLine<T>::TContourLine(T height) : height_(height)
    {
    }

    template <typename T>
    TContourLine<T>::~TContourLine()
    {
    }

    template <typename T>
    TContourLine<T>::TContourLine(const TContourLine<T>& from)
      : height_(from.height_),
        data_(from.data_)
    {
    }

    template <typename T>
    void TContourLine<T>::clear()
    {
      data_.clear();
    }

    template <typename T>
    const TContourLine<T>& TContourLine<T>::operator = (const TContourLine<T>& data)
    {
      data_ = data.data_;
      height_ = data.height_;
    }

    template <typename T>
    bool TContourLine<T>:: operator == (const TContourLine<T>& data) const
    {
      return ((height_ == data.height_)
               && (data_ == data.data_));
    }


    template <typename T>
    void TContourLine<T>::createContourLine(const TRegularData2D<T>& from)
    {
      // This function uses a "marching cubes"-style algorithm to determine the contour-lines.
      Size number_of_cells;
      Size number_of_cells_x;
      Size number_of_cells_y;
      Position act_cell_x;
      Position act_cell_y;
      PointType dummy, dummy2;
      double d1, d2, slope;
      double threshold = height_;

      number_of_cells_x = (Size) from.getXSize()-1;
      number_of_cells_y = (Size) from.getYSize()-1;
      number_of_cells   = number_of_cells_x * number_of_cells_y;
      
      for (act_cell_y = 0; act_cell_y < number_of_cells_y; act_cell_y++)
	{
	  for (act_cell_x = 0; act_cell_x < number_of_cells_x; act_cell_x++)
	    {
	      // First we have to find out the topology of the actual square.
	      int topology = 0;
	      
	      if (from[act_cell_x + act_cell_y*(number_of_cells_x+1)] > threshold)
		topology |= 1;
	      if (from[act_cell_x+1 + act_cell_y*(number_of_cells_x+1)] > threshold)
		topology |= 2;
	      if (from[act_cell_x+1 + (act_cell_y+1)*(number_of_cells_x+1)] > threshold)
		topology |= 4;
	      if (from[act_cell_x + (act_cell_y+1)*(number_of_cells_x+1)] > threshold)
		topology |= 8;
	      
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
      it = data_.begin();
    }

    template <typename T>
    bool TContourLine<T>::getNextPoint(TContourLine<T>::PointType &p)
    {
      if (it != data_.end()) {
	p = *it;
	it++;
	return (true);
      } else {
	return (false);
      };
    }
    
}
#endif
