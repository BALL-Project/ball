// $Id: regularData2D.h,v 1.4 2000/11/28 17:21:01 anhi Exp $

#ifndef BALL_DATATYPE_REGULARDATA2D_H
#define BALL_DATATYPE_REGULARDATA2D_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#include <vector>
namespace BALL
{

	/**	A class to store regularaly spaced two-dimensional data.
			This class is intended to hold regularly spaced, two-dimensional data sets.
			It might be useful to hold data sets like spectra, or precomputed function values.
			\\
			{\bf Definition:} \URL{BALL/DATATYPE/regularData2D.h}
	*/
	template <typename T>
	class TRegularData2D
	{
		public:
			
		BALL_CREATE(TRegularData2D<T>)

		/**	@name Type definitions
		*/
		//@{
		/**	The vector type.
				This type is used to store the data. In this two-dimensional case,
				the data is stored one line after another in a linear way.
		*/
		typedef std::vector<T>	VectorType;
		//@}
	
		/** @name Constructors and Destructors.
		*/
		//@{
			
		/**	Default constructor
		*/
		TRegularData2D();

		/**	Copy constructor
		*/
		TRegularData2D(const TRegularData2D& data);

		/**	Destructor
		*/
		virtual ~TRegularData2D();

		/**	Clear the contents
		*/
		virtual void clear();

		/**	Clear the contents and reset the object dimensions
		*/
		virtual void destroy();

		//@}

		/**	@name Assignment
		*/
		//@{

		/**	Assignment operator.
				Copy the data and the boundaries.
		*/
		const TRegularData2D& operator = (const TRegularData2D& data);

		/**	Assignment from a {\tt vector} of {\tt T}.
				Copy the contents of the data without changing the boundaries.
		*/
		const TRegularData2D& operator = (const VectorType& data);
		//@}
		
		/**	@name Predicates
		*/
		//@{
		/**	Equality operator
		*/
		bool operator == (const TRegularData2D& data) const;
		//@}
	
		/**	@name	Accessors
		*/
		//@{	
			
		/**	Constant random access operator.
				@exception IndexOverflow if {\tt index} is out of range
		*/	
		const T& operator [] (Position index) const;
			
		/**	Mutable random access operator.
				@exception IndexOverflow if {\tt index} is out of range
		*/	
		T& operator [] (Position index);
		
		/**     Return the position of the data point nearest to x, y.
		 */
		void getNearestPosition(double x, double y, pair<Position, Position>& dum);

		/**     Return the position of x, y in the internal coordinate system.
		 */
		void getConvertedPosition(Position x, Position y, pair<T, T>& dum) const;

		/**     Return the value at coordinates x, y. If there's no data point at that location, it will be
                        interpolated.
		*/
		const T& getValue(T valx, T valy);

		/**     Sample 10% of the data, calculate their mean (should be close to the "ground state") and their standard deviation.
		 */
		void createGroundState();

		/**     Return the approximated value of the ground state. This must have been created earlier by using
		 *      createGroundState();
		 */
		const T& getGroundState();

		/**     Return the standard deviation of the approximated value of the ground state. This must have been created
		 *      earlier by using createGroundState();
		 */		
		const T& getSigmaGroundState();

		/**	Return the number of items 
		*/
		Size getSize() const;

		/**     Return the number of items per line
		 */
		Size getXSize() const;

		/**     Return the number of items per column
		 */
		Size getYSize() const;

		/**	Return the lower bound in x direction
		*/
		double getXLower() const;
		
		/**	Return the upper bound in x direction
		*/
		double getXUpper() const;

		/**     Return the lower bound in y direction
		 */
		double getYLower() const;

		/**     Return the upper bound in y direction
		 */
		double getYUpper() const;

		/**     Set the size in x direction
		 */
		void setXSize(Size xsize);

		/**     Set the size in y direction
		 */
		void setYSize(Size ysize);

		/**	Set the upper bound in x direction
 		*/
		void setXUpper(double xupper);

		/**	Set the lower bound in x direction
		*/
		void setXLower(double xlower);

		/**     Set the upper bound in y direction
		 */
		void setYUpper(double yupper);

		/**     Set the lower bound in y direction
		 */
		void setYLower(double ylower);

		/**	Resize the data.
				If {\tt new_size_(x/y)} is larger than the current size, the data {\tt vector}
				is extended to the new size and filled with default constructed items of type
				{\tt T}. Resizing to a value lesser than the current size truncates the vector.
				\\
				The boundaries are adapted keep the positions of the retained items fixed, i.e.
				the upper bound is increased or decreased proportionally while the lower bound
				remains unchanged.
				
				@param new_size_x the new size in x direction
				@param new_size_y the new size in y direction
		*/
		void resize(Size new_size_x, Size new_size_y);
		//@}
	
		protected:
		/**	The lower bound in x direction
		*/
		double			xlower_;

		/**	The upper bound in x direction
		*/
		double			xupper_;

		/**     The lower bound in y direction
		 */
		double                  ylower_;

		/**     The upper bound in y direction
		 */
		double                  yupper_;

		/**     The size in x direction
		 */
		Size xsize_;

		/**     The size in y direction
		 */
		Size ysize_;

		/**	The data
		*/
		VectorType	data_;

		/**     The value of the "ground state"
		 */
		T groundState_;

		/**     The standard deviation of the sample used to calculate the ground state.
		 */
		T sigmaGroundState_;
	};

	/**	Default type
	*/
	typedef TRegularData2D<float> RegularData2D;
	
	template <typename T>
	TRegularData2D<T>::TRegularData2D()
	{
	}

	template <typename T>
	TRegularData2D<T>::~TRegularData2D()
	{
	}

	template <typename T>
	TRegularData2D<T>::TRegularData2D(const TRegularData2D<T>& data)
		:	xlower_(data.xlower_),
			xupper_(data.xupper_),
	                ylower_(data.ylower_),
                	yupper_(data.yupper_),
	                xsize_(data.xsize_),
	                ysize_(data.ysize_),
	                data_(data.data_),
	                groundState_(data.groundState_),
	                sigmaGroundState_(data.sigmaGroundState_)
	{
	}

	template <typename T>
	void TRegularData2D<T>::clear()
	{
		// iterate over the data and reset all values to their default
		// boundaries and vector size remain unchanged
		T default_value;
		VectorType::iterator it = data_.begin();
		for (; it != data_.end(); ++it)
		{
			*it = default_value;
		}
	}

	template <typename T>
	void TRegularData2D<T>::destroy()
	{
		// clear the vector and the boundaries
		data_.clear();
		xlower_ = 0.0;
		xupper_ = 0.0;
		ylower_ = 0.0;
		yupper_ = 0.0;
		xsize_ = 0;
		ysize_ = 0;
		groundState_ = 0.0;
		sigmaGroundState_ = 0.0;
	}

	template <typename T>
	const TRegularData2D<T>& TRegularData2D<T>::operator = (const TRegularData2D<T>& data)
	{
		// copy all members...
		data_ = data.data_;
		xlower_ = data.xlower_;
		xupper_ = data.xupper_;
		ylower_ = data.ylower_;
		yupper_ = data.yupper_;
		xsize_ = data.xsize_;
		ysize_ = data.ysize_;
		groundState_ = data.groundState_;
		sigmaGroundState_ = data.sigmaGroundState_;
	}

	template <typename T>
	const TRegularData2D<T>& TRegularData2D<T>::operator = (const TRegularData2D<T>::VectorType& data)
	{
		// Copy the data. The boundaries remain unchanged.
		data_ = data;
	}

	template <typename T>
	bool TRegularData2D<T>::operator == (const TRegularData2D<T>& data) const
	{
		return ((xlower_ == data.xlower_) 
						&& (xupper_ == data.xupper_)
                  			        && (ylower_ == data.ylower_)
			                        && (yupper_ == data.yupper_)
			                        && (xsize_ == data.xsize_)
			                        && (ysize_ == data.ysize_)
						&& (data_ == data.data_)
			                        && (groundState_ == data.groundState_)
			                        && (sigmaGroundState_ == data.sigmaGroundState_));
	}
	
	template <typename T>
	BALL_INLINE
	const T& TRegularData2D<T>::operator [] (Position index) const
	{
		if (index >= data_.size())
		{
			throw Exception::IndexOverflow(__FILE__, __LINE__, index);
		}
		
		return data_[index];
	}	
	
	template <typename T>
	BALL_INLINE
	T& TRegularData2D<T>::operator [] (Position index)
	{
		if (index >= data_.size())
		{
			throw Exception::IndexOverflow(__FILE__, __LINE__, index);
		}
		
		return data_[index];
	}	
	
	template <typename T>
	BALL_INLINE
	void TRegularData2D<T>::getNearestPosition(double x, double y, pair<Position, Position>& res)
	{
	  if (x > xlower_)
	  {
	    if (x < xupper_)
	    {
	      res.first = (Position) ((x - xlower_) / (xupper_ - xlower_) * xsize_);
	    }
	    else
	    {
	      res.first = 0;
	    }
	  } 
	  else 
	  {
	    res.first = 0;
	  }

	  if (y > ylower_)
	  {
	    if (y < yupper_)
	    {
	      res.second = (Position) ((y - ylower_) / (yupper_ - ylower_) * ysize_);
	    }
	    else
	    {
	      res.second = 0;
	    }
	  } 
	  else 
	  {
	    res.second = 0;
	  }
	}

	template <typename T>
	BALL_INLINE
	void TRegularData2D<T>::getConvertedPosition(Position x, Position y, pair<T, T>& dum) const
	{
	  dum.first = xlower_ - (double) x / xsize_ * (double)(xupper_ - xlower_) / xsize_;
	  dum.second = ylower_ - (double) y / ysize_ * (double)(yupper_ - ylower_) / ysize_;
	}

	template <typename T>
	BALL_INLINE
	const T& TRegularData2D<T>::getValue(T valx, T valy)
	{
	  double stepx, stepy;
	  double res=0.0;
	  double t, u;

	  if (xsize_ && ysize_)
	  {
	    stepx = (xupper_ - xlower_) / ((double) xsize_);
	    stepy = (yupper_ - ylower_) / ((double) ysize_);
	  };
	  if (stepx && stepy)
	  {
	    Position ax, ay;

	    // Find out where we are in the data
	    ax = (Position) floor((valx - xlower_) / stepx); // this is the index of the lower left corner.
	    ay = (Position) floor((valy - ylower_) / stepy);

	    t = (valx - (xlower_ + ax*stepx))  / (stepx);
	    u = (valy - (ylower_ + ay*stepy)) /  (stepy);

	    res = (1-t)*(1-u)*data_[ax + xsize_*ay] + t*(1-u)*data_[(ax+1) + xsize_*ay] \
	      + t*u*data_[(ax+1) + xsize_*(ay+1)] + (1-t)*u*data_[ax + xsize_*(ay+1)];

	  }
	  return res;
	}

	template <typename T>
	BALL_INLINE
	const T& TRegularData2D<T>::getGroundState()
	{
	  return groundState_;
	}

	template <typename T>
	BALL_INLINE
	const T& TRegularData2D<T>::getSigmaGroundState()
	{
	    return sigmaGroundState_;
	}

	template <typename T>
	BALL_INLINE
	Size TRegularData2D<T>::getSize() const
	{
		return data_.size();
	}

	template <typename T>
	BALL_INLINE
	Size TRegularData2D<T>::getXSize() const
	{
		return xsize_;
	}

	template <typename T>
	BALL_INLINE
	Size TRegularData2D<T>::getYSize() const
	{
		return ysize_;
	}
	
	template <typename T>
	BALL_INLINE
	double TRegularData2D<T>::getXLower() const
	{
		return xlower_;
	}
	
	template <typename T>
	BALL_INLINE
	double TRegularData2D<T>::getYLower() const
	{
		return ylower_;
	}

	template <typename T>
	BALL_INLINE
	double TRegularData2D<T>::getXUpper() const
	{
		return xupper_;
	}

	template <typename T>
	BALL_INLINE
	double TRegularData2D<T>::getYUpper() const
	{
		return yupper_;
	}

	template <typename T>
	BALL_INLINE
	void TRegularData2D<T>::setXLower(double xlower)
	{
		xlower_ = xlower;
	}
	
	template <typename T>
	BALL_INLINE
	void TRegularData2D<T>::setYLower(double ylower)
	{
		ylower_ = ylower;
	}

	template <typename T>
	BALL_INLINE
	void TRegularData2D<T>::setXUpper(double xupper)
	{
		xupper_ = xupper;
	}
	
	template <typename T>
	BALL_INLINE
	void TRegularData2D<T>::setYUpper(double yupper)
	{
		yupper_ = yupper;
	}

	template <typename T>
	BALL_INLINE
	void TRegularData2D<T>::setXSize(Size xsize)
	{
	        xsize_ = xsize;
	}

	template <typename T>
	BALL_INLINE
	void TRegularData2D<T>::setYSize(Size ysize)
	{
	        ysize_ = ysize;
	}

	template <typename T>
	void TRegularData2D<T>::resize(Size new_size_x, Size new_size_y)
	{
		if (data_.size() > 0)
		{
			xupper_ *= (double)new_size_x / xsize_;
			yupper_ *= (double)new_size_y / ysize_;
		};
		data_.resize(new_size_x*new_size_y);
	}
		
	template <typename T>
	void TRegularData2D<T>::createGroundState()
	{
	  Size numsamples, actnum, actval;
	  
	  Size num;
	  double ran;

	  srand48(42);
	  
	  num = data_.size();
	  numsamples = (Size) (num / 100);
	  vector<T> samplevals(numsamples);
	  
	  groundState_ = 0;
	  sigmaGroundState_ = 0;

	  // calculation of mean value
	  for (actnum = 0; actnum < numsamples; actnum++)
	  {
	    ran = drand48();
	    actval = (Position) (ran * num);
	    
	    samplevals[actnum] = data_[actval];
	    groundState_ += data_[actval];
	  };

	  groundState_ /= numsamples;

	  // calculation of standard deviation
	  for (actnum = 0; actnum < numsamples; actnum++)
	  {
	    sigmaGroundState_ += pow(samplevals[actnum] - groundState_, 2);
	  };
	  
	  sigmaGroundState_ /= (numsamples - 1);
	  sigmaGroundState_ = sqrt(sigmaGroundState_);
	};

} // namespace BALL

#endif // BALL_DATATYPE_REGULARDATA2D_H
