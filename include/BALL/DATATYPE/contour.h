// $Id: contour.h,v 1.1 2000/12/01 17:19:35 anhi Exp $

#ifndef BALL_DATATYPE_CONTOUR_H
#define BALL_DATATYPE_CONTOUR_H

using namespace BALL;
using namespace BALL::VIEW;

#ifndef BALL_DATATYPE_REGULARDATA2D_H
#       include <BALL/DATATYPE/regularData2D.h>
#endif

#ifndef BALL_DATATYPE_CONTOURLINE_H
#       include <BALL/DATATYPE/contourLine.h>
#endif

/** This class is intended to create and store ContourLines belonging to the same data-set.
*/
  template <typename T>
  class TContour
{
    public:

// macro for create method
  BALL_CREATE(TContour<T>)

/** @name    Constructors and Destructors
*/
//@{
    /// Default constructor
    TContour(Size num_lines=0, double start=0, double end=0);

    /// Copy constructor
    TContour(const TContour& copyTContour);

    /// Destructor
    virtual ~TContour();
//@}

    /** @name    Accessors
     */
    //@{

    /// Gives access to the next ContourLine. Returns false if we had already returned the last line.
    bool getNextContourLine(TContourLine<T>& cont);

    //@}

    /** @name    Assignment
     */
    //@{
    /// Assignment operator
    const TContour& operator = (const TContour& assigTContour);

    void apply(const TRegularData2D<T>& data);

    /// Clear method
    virtual void clear();

    /// Reset the ContourLine-counter.
    void resetCounter();

    //@}

    /** @name    Predicates
     */
    //@{
    
    /// Equality operator
    bool operator == (const TContour& compTContour) const;

    //@}

   private:
    
    vector< TContourLine<T> > lines_;
    Size num_lines_;
    double start_;
    double end_;
    vector< TContourLine<T> >::const_iterator it_;
};

/**     Default type
 */
typedef TContour<float> Contour;

template <typename T>
TContour<T>::TContour(Size num_lines, double start, double end) : num_lines_(num_lines), start_(start), end_(end), lines_(num_lines)
{
}

template <typename T>
TContour<T>::TContour(const TContour& copyTContour)
{
}

template <typename T>
TContour<T>::~TContour()
{
}

template <typename T>
const TContour<T>& TContour<T>::operator = (const TContour& assigTContour)
{
  start_     = assigTContour.start_;
  end_       = assigTContour.end_;
  data_      = assigTContour.data_;
  num_lines_ = assigTContour.num_lines_;
  it_        = assigTContour.it_;
}

template <typename T>
void TContour<T>::clear()
{
  start_     = 0;
  end_       = 0;
  num_lines_ = 0;
  lines_     = vector< TContourLine<T> >(0);
}

template <typename T>
bool TContour<T>::operator == (const TContour& compTContour) const
{
  return ((start_ == compTContour.start_) && (end_ == compTContour.end_) && (lines_ == compTContour.lines_)
	  && (num_lines_ == compTContour.num_lines_) && (it_ == compTContour.it_));
}

template <typename T>
void TContour<T>::apply(const TRegularData2D<T>& data)
{
  Position i;
  double step = (end_ - start_) / num_lines_;

  for (i=0; i<num_lines_; i++)
  {
    TContourLine<T> con(start_ + i*step);
    con.createContourLine(data);
    lines_.push_back(con);
  };

  if (num_lines_ > 0)
  {
    it_ = lines_.begin();
  };
}

template <typename T>
bool getNextContourLine(TContourLine<T>& cont)
{
  if (it_ != 0)
  {
    ++it_;
    if (it_ == lines_.last())
    {
      return false;
    } else {
      cont = it_;
      return true;
    };
  };
  return false;
}

template <typename T>
void TContour<T>::resetCounter()
{
  if (lines_)
  {
    it_ = lines_.begin();
  };
}

#endif
