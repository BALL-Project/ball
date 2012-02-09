// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_MATHS_TFFT2D_H
#define BALL_MATHS_TFFT2D_H

#ifndef BALL_COMMON_EXCEPTION_H
# include <BALL/COMMON/exception.h>
#endif

#ifndef BALL_DATATYPE_REGULARDATA2D_H
# include <BALL/DATATYPE/regularData2D.h>
#endif

#ifndef BALL_MATHS_VECTOR2_H
# include <BALL/MATHS/vector2.h>
#endif

#include <cmath>
#include <complex>
#include <fftw3.h>

#include <BALL/MATHS/fftwCommon.h>


namespace BALL
{
	/** A class to perform Fast Fourier Transforms and inverse Fast Fourier Transforms
			on regularly spaced two dimensional data.  \par
			This class makes use of the freely available library <b>FFTW</b>, which can be
			found at http://www.fftw.org
			coordinate system can be handled automatically. The normaliztion is chosen
			symmetrically.
			 \par
			S/TFFT2D.h
	 \ingroup FFT
	 */

	template <typename ComplexTraits>
	class TFFT2D 
		: public TRegularData2D<std::complex<typename ComplexTraits::ComplexPrecision> >
	{
		public:
		
			typedef std::complex<typename ComplexTraits::ComplexPrecision> Complex;
			typedef TRegularData2D<std::complex<typename ComplexTraits::ComplexPrecision> >	ComplexVector;
			typedef typename TRegularData2D<std::complex<typename ComplexTraits::ComplexPrecision> >::IndexType IndexType;

      BALL_CREATE(TFFT2D)

      /**  @name Constructors and Destructors
       */
      //@{
 
      /// Default constructor
      TFFT2D();

			/// Copy constructor
			TFFT2D(const TFFT2D &data);

			/** Detailed constructor.  \par
			 		@param nX The number of grid points in X direction 
					@param nY The number of grid points in Y direction
					@param stepPhysX The step width in X direction in physical space
					@param stepPhysY The step width in Y direction in physical space
					@param origin The origin of the coordinate system
					@param inFourierSpace Flag to decide whether the data is assumed to be in physical or fourier
																space
			 */
			 // ldn is not any longer the binary logarithm but the absolute number of grid points
			TFFT2D(Size nX, Size nY, double stepPhysX=1., double stepPhysY=1., Vector2 origin=Vector2(0.,0.), bool inFourierSpace=false);

			/// Destructor
			virtual ~TFFT2D();
			
			//@}

			/** @name Assignment
			 */
			//@{

			/// Assignment operator
			const TFFT2D& operator = (const TFFT2D& fft_2d);
			
			/** Clear the contents.
			 */
			virtual void clear();
			
			/** Clear the contents and reset all attributes.
			 */
			virtual void destroy();

			//@}

			/** @name Predicates
			 */
			//@{

			/** Equality operator.
			 */
			bool operator == (const TFFT2D& fft_2d) const;
			//@}
			
			// @name Accessors
			
			/** Perform a single fast fourier transform on the data.
			 */
			void doFFT();

			/** Perform a single inverse fourier transform on the data.
			 */
			void doiFFT();

			/** Translate the origin in physical space about {\em trans_origin},
					i.e. the new origin will be located at the former position {\em trans_origin}.
					If the result is out of bounds, the function does nothing and
					returns <b>  false </b>.
			 */
			bool translate(const Vector2& trans_origin);

			/** Set the step width in physical space to {\em new_width_x, new_width_y}.
				The step width in fourier space is automatically adjusted
				accordingly. {\em new_width_x and new_width_y} must be positive, otherwise
				the function does nothing and retuns <b>  false </b>.
			 */
			bool setPhysStepWidth(double new_width_x, double new_width_y);

			/** Returns the step width in physical space in X direction.
			 */
			double getPhysStepWidthX() const;

			/** Returns the step width in physical space in Y direction.
			 */
			double getPhysStepWidthY() const;

			/** Returns the step width in fourier space in X direction.
			 */
			double getFourierStepWidthX() const;

			/** Returns the step width in fourier space in Y direction.
			 */
			double getFourierStepWidthY() const;

			/** Returns the minimal position of the grid in physical space in X direction.
			 */
			double getPhysSpaceMinX() const;

			/** Returns the minimal position of the grid in physical space in Y direction.
			 */
			double getPhysSpaceMinY() const;

			/** Returns the maximal position of the grid in physical space in X direction.
			 */
			double getPhysSpaceMaxX() const;

			/** Returns the maximal position of the grid in physical space in Y direction.
			 */
			double getPhysSpaceMaxY() const;

			/** Returns the minimal position of the grid in fourier space in X direction.
			 */
			double getFourierSpaceMinX() const;

			/** Returns the minimal position of the grid in fourier space in Y direction.
			 */
			double getFourierSpaceMinY() const;

			/** Returns the maximal position of the grid in fourier space in X direction.
			 */
			double getFourierSpaceMaxX() const;

			/** Returns the maximal position of the grid in fourier space in Y direction.
			 */
			double getFourierSpaceMaxY() const;
				
			/** Return the largest grid position for the x direction. 
			 		This method returns the maximum position allowed in the grid. As the point 
					in the origin has the indices (0, 0), this method returns the number of 
					points in X direction minus one.
			  */
			Size getMaxXIndex() const;

			/** Return the largest grid position for the y direction. 
			 		This method returns the maximum position allowed in the grid. As the point 
					in the origin has the indices (0, 0), this method returns the number of 
					points in Y direction minus one.
			  */
			Size getMaxYIndex() const;
				
			/** Return the number of inverse transforms that have been carried out using this class.
			 		This is an important factor for the normalization of the data.
			 */
			Size getNumberOfInverseTransforms() const;

			/** Returns the grid coordinate corresponding to the position.
			 */
			Vector2 getGridCoordinates(Position position) const;

			/** Returns the data at the grid position closest to <b>  pos </b>,
			 *	and automatically includes
			 * 	the correct phase factor and (symmetric) normalization.
			 *
			 *  @throw Exception::OutOfGrid if pos is outside the grid boundaries
			 */
			Complex getData(const Vector2& pos) const;

			/** Returns the data at point <b>pos</b>. If <b>pos</b> is not a 
			 *	point on the grid, the data is linearly interpolated.
			 *	This method automatically includes the correct phase factor
			 *	and (symmetric) normalization.
			 *
			 *  @throw Exception::OutOfGrid if pos is outside the grid boundaries
			 */
			Complex getInterpolatedValue(const Vector2& pos) const;

			/** Sets the data point at the grid position closest to <b>  pos </b>
			 *  to the value <b>  val </b>, and -- if called in fourier space --
			 *  automatically includes the correct phase factor and 
			 *  (symmetric) normalization.
			 *
			 *  @throw Exception::OutOfGrid if pos is outside the grid boundaries
			 */
			void setData(const Vector2& pos, Complex val);

			/** Access the data at the grid position closest to <b>  pos </b>.
			 *  This function returns the "raw" data at that position.
			 *
			 *  @throw Exception::OutOfGrid if pos is outside the grid boundaries
			 */
			Complex& operator[](const Vector2& pos);

			/** Access the data at the grid position closest to <b>  pos </b>.
			 *	This function returns the "raw" data at that position.
			 *
			 *  @throw Exception::OutOfGrid if pos is outside the grid boundaries
			 */
			const Complex& operator[](const Vector2& pos) const;

			/** Nonmutable random access operator.
			 *	@note No range checking is done. For a more robust version, please
			 *	use getData.
			 */
			const Complex& operator [] (const IndexType& index) const { return TRegularData2D<Complex>::operator [](index); }

			/** Mutable random access operator.
					@note No range checking is done. For a more robust version, please
					use getData.
			*/
			Complex& operator [] (const IndexType& index) {  return TRegularData2D<Complex>::operator [](index); }
			
			/** Access the (raw) data at Position pos.
			 *
			 *  @throw Exception::OutOfGrid if pos is outside the grid boundaries
			 */
			Complex& operator[](const Position& pos)
			{
				return TRegularData2D<Complex>::operator [] (pos);
			}

			/** Access the (raw) data at Position pos. Const method.
			 *
			 *  @throw Exception::OutOfGrid if pos is outside the grid boundaries
			 */
			const Complex& operator[](const Position& pos) const
			{
				return TRegularData2D<Complex>::operator [] (pos);
			}
			
			// 
			void setNumberOfFFTTransforms(Size num)
			{
				numPhysToFourier_ = num;
			}
			
			// 
			void setNumberOfiFFTTransforms(Size num)
			{
				numFourierToPhys_ = num;
			}
			
			/** This computes the phase factor in fourier space that results
				if the origin of the coordinate system in physical space
				is not in the "lower left corner".
			 */
			Complex phase(const Vector2& pos) const;
				
			/** Returns <b>true</b> if the data is considered to be in Fourier space,
			 		<b>false</b> otherwise.
			 */
			bool isInFourierSpace() const;

		protected:
			Size lengthX_, lengthY_;
			bool inFourierSpace_;
			Size numPhysToFourier_;
			Size numFourierToPhys_;
			Vector2 origin_;
			double stepPhysX_, stepPhysY_;
			double stepFourierX_, stepFourierY_;
      Vector2 minPhys_, maxPhys_;
      Vector2 minFourier_, maxFourier_;
      
      // new version for FFTW3
			typename ComplexTraits::FftwPlan planForward_;
			typename ComplexTraits::FftwPlan planBackward_;

			
			// to control plan calculation with new fftw3
			Size dataLength_;
			Complex *dataAdress_;
			bool planCalculated_;
			
	};
	
	/**	Default type
	*/
	typedef TFFT2D<BALL_FFTW_DEFAULT_TRAITS> FFT2D;
	
	/** Global assignment operator from TFFT2D to TRegularData2D<Complex>
	 */
	template <typename ComplexTraits>
	const TRegularData2D<typename TFFT2D<ComplexTraits>::Complex>& operator<< 
			(TRegularData2D<typename TFFT2D<ComplexTraits>::Complex>& to, const TFFT2D<ComplexTraits>& from);
	
	/** Global assignment operator from FFT3D to TRegularData3D<float>.
	 		This operator assigns the <b>real</b> part of the complex TFFT2D-data to the
			TRegularData2D<float> to.
	 */
	template <typename ComplexTraits>
	const RegularData2D& operator << (RegularData2D& to, const TFFT2D<ComplexTraits>& from);
	
	template <typename ComplexTraits>
	TFFT2D<ComplexTraits>::TFFT2D()
		: TRegularData2D<Complex>(),
			dataLength_(0),
			dataAdress_(0),
			planCalculated_(false)
	{
	}
	
	template <typename ComplexTraits>
	bool TFFT2D<ComplexTraits>::operator == (const TFFT2D<ComplexTraits>& fft2D) const
	{
		// test whether data_.size() == fft2D.data_.size()
		// instead of testing 2 lengths. Better for vector handling.
		
		if (lengthX_ == fft2D.lengthX_ &&
				lengthY_ == fft2D.lengthY_ &&
				origin_ == fft2D.origin_ &&
				stepPhysX_ == fft2D.stepPhysX_ &&
				stepPhysY_ == fft2D.stepPhysY_ &&
				stepFourierX_ == fft2D.stepFourierX_ &&
				stepFourierY_ == fft2D.stepFourierY_ &&
				minPhys_ == fft2D.minPhys_ &&
				maxPhys_ == fft2D.maxPhys_ &&
				minFourier_ == fft2D.minFourier_ &&
				maxFourier_ == fft2D.maxFourier_ &&
				numPhysToFourier_ == fft2D.numPhysToFourier_ &&
				numFourierToPhys_ == fft2D.numFourierToPhys_)
		{
			Vector2 min  = inFourierSpace_ ?  minFourier_  :   minPhys_;
			Vector2 max  = inFourierSpace_ ?  maxFourier_  :   maxPhys_;
			double stepX = inFourierSpace_ ? stepFourierX_ : stepPhysX_;
			double stepY = inFourierSpace_ ? stepFourierY_ : stepPhysY_;	
			
			for (double posX=min.x; posX<=max.x; posX+=stepX)
			{
				for (double posY=min.y; posY<=max.y; posY+=stepY)
				{
					if (getData(Vector2(posX,posY)) != fft2D.getData(Vector2(posX,posY)))
					{
						return false;
					}
				}
			}
			
			return true;
		}
	
		return false;
	}
	
	template <typename ComplexTraits>
	bool TFFT2D<ComplexTraits>::translate(const Vector2& trans_origin)
	{
		Position internalOriginX = (Position) Maths::rint(trans_origin.x*stepPhysX_);
		Position internalOriginY = (Position) Maths::rint(trans_origin.y*stepPhysY_);
		
		if ((internalOriginX <= lengthX_) && (internalOriginY <= lengthY_))
		{
			origin_.x = trans_origin.x;
			origin_.y = trans_origin.y;
			
      minPhys_ = Vector2(-origin_.x,-origin_.y);
  	  maxPhys_ = Vector2(((lengthX_-1)*stepPhysX_)-origin_.x,((lengthY_-1)*stepPhysY_)-origin_.y);
	 	  minFourier_ = Vector2(-(lengthX_/2.-1)*stepFourierX_,-(lengthY_/2.-1)*stepFourierY_);
			maxFourier_ = Vector2((lengthX_/2.)*stepFourierX_,(lengthY_/2.)*stepFourierY_);
		 
			return true;
		}
		else
		{
			return false;
		}
	}

	template <typename ComplexTraits>
	bool TFFT2D<ComplexTraits>::setPhysStepWidth(double new_width_x, double new_width_y)
	{
		if ((new_width_x <= 0) || (new_width_y <= 0))
		{
			return false;
		}
		else
		{
			stepPhysX_ = new_width_x;
			stepPhysY_ = new_width_y;
			stepFourierX_ = 2.*M_PI/(stepPhysX_*lengthX_);
			stepFourierY_ = 2.*M_PI/(stepPhysY_*lengthY_);

			minPhys_ = Vector2(-origin_.x,-origin_.y);
  	  maxPhys_ = Vector2(((lengthX_-1)*stepPhysX_)-origin_.x,((lengthY_-1)*stepPhysY_)-origin_.y);
	 	  minFourier_ = Vector2(-(lengthX_/2.-1)*stepFourierX_,-(lengthY_/2.-1)*stepFourierY_);
			maxFourier_ = Vector2((lengthX_/2.)*stepFourierX_,(lengthY_/2.)*stepFourierY_);
	
      return true;
		}
	}
	
	template <typename ComplexTraits>
	double TFFT2D<ComplexTraits>::getPhysStepWidthX() const
	{
		return stepPhysX_;
	}

	template <typename ComplexTraits>
	double TFFT2D<ComplexTraits>::getPhysStepWidthY() const
	{
		return stepPhysY_;
	}

	template <typename ComplexTraits>
	double TFFT2D<ComplexTraits>::getFourierStepWidthX() const
	{
		return stepFourierX_;
	}

	template <typename ComplexTraits>
	double TFFT2D<ComplexTraits>::getFourierStepWidthY() const
	{
		return stepFourierY_;
	}

	template <typename ComplexTraits>
	double TFFT2D<ComplexTraits>::getPhysSpaceMinX() const
	{
    return minPhys_.x;
  }

	template <typename ComplexTraits>
	double TFFT2D<ComplexTraits>::getPhysSpaceMinY() const
	{
    return minPhys_.y;
  }

	template <typename ComplexTraits>
	double TFFT2D<ComplexTraits>::getPhysSpaceMaxX() const
	{
    return maxPhys_.x;
 	}

	template <typename ComplexTraits>
	double TFFT2D<ComplexTraits>::getPhysSpaceMaxY() const
	{
    return maxPhys_.y;
 	}

	template <typename ComplexTraits>
	double TFFT2D<ComplexTraits>::getFourierSpaceMinX() const
	{
		return minFourier_.x;
	}

	template <typename ComplexTraits>
	double TFFT2D<ComplexTraits>::getFourierSpaceMinY() const
	{
		return minFourier_.y;
	}

	template <typename ComplexTraits>
	double TFFT2D<ComplexTraits>::getFourierSpaceMaxX() const
	{
		return maxFourier_.x;
	}

	template <typename ComplexTraits>
	double TFFT2D<ComplexTraits>::getFourierSpaceMaxY() const
	{
		return maxFourier_.y;
	}
	
	template <typename ComplexTraits>
	Size TFFT2D<ComplexTraits>::getMaxXIndex() const
	{
		return (lengthX_ - 1);
	}
	
	template <typename ComplexTraits>
	Size TFFT2D<ComplexTraits>::getMaxYIndex() const
	{
		return (lengthY_ - 1);
	}
	
	template <typename ComplexTraits>
	Size TFFT2D<ComplexTraits>::getNumberOfInverseTransforms() const
	{
		return numFourierToPhys_;
	}
	
	// new for compatibility with FFT3D
	template <typename ComplexTraits>
	Vector2 TFFT2D<ComplexTraits>::getGridCoordinates(Position position) const
	{
		if (!inFourierSpace_)
		{
			if (position >= ComplexVector::size())
			{
				throw Exception::OutOfGrid(__FILE__, __LINE__);
			}
		
			Vector2 r;
			Position  x, y;


			// AR: ??????
			y = position % lengthY_;
			x = position / lengthY_;

			r.set(-origin_.x + (float)x * stepPhysX_,
						-origin_.y + (float)y * stepPhysY_);

			return r;
		}
		else
		{
			if (position >= ComplexVector::size())
			{
				throw Exception::OutOfGrid(__FILE__, __LINE__);
			}
		
			Vector2 r;
			Index x, y;
	
			// AR: ??????
			y = position % lengthY_;
			x = position / lengthY_;

			if (x>=lengthX_/2.)
			{
				x-=lengthX_;
			}
			
			if (y>=lengthY_/2.)
			{
				y-=lengthY_;
			}

			r.set((float)x * stepFourierX_,
						(float)y * stepFourierY_);

			return r;
		}
	}
	
	
	
	template <typename ComplexTraits>
	typename TFFT2D<ComplexTraits>::Complex TFFT2D<ComplexTraits>::getData(const Vector2& pos) const
	{
		Complex result;
		double normalization=1.;

		if (!inFourierSpace_)
		{
			result = (*this)[pos];
			normalization=1./((float)pow((float)(lengthX_*lengthY_),(int)numFourierToPhys_));
		}
		else
		{
			result = (*this)[pos] * phase(pos);
			normalization=1./(2.*M_PI)*(stepPhysX_*stepPhysY_)/((float)pow((float)(lengthX_*lengthY_),(int)numFourierToPhys_));
		}

		result *= normalization;
		
		return result;
	}

	template <typename ComplexTraits>
	typename TFFT2D<ComplexTraits>::Complex TFFT2D<ComplexTraits>::getInterpolatedValue(const Vector2& pos) const
	{
		Complex result;
		
		Vector2 min  = inFourierSpace_ ? minFourier_   :   minPhys_;
		Vector2 max  = inFourierSpace_ ? maxFourier_   :   maxPhys_;
		double stepX = inFourierSpace_ ? stepFourierX_ : stepPhysX_;
		double stepY = inFourierSpace_ ? stepFourierY_ : stepPhysY_;
		
		if (    (pos.x < min.x) || (pos.y < min.y)
				 || (pos.x > max.x) || (pos.y > max.y)  )
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		}

		Vector2 h(pos.x - min.x, pos.y - min.y);
		double modX = fmod((double)h.x,stepX);
		double modY = fmod((double)h.y,stepY);

		if (modX==0 && modY ==0) // we are on the grid
		{
			return getData(pos);
		}

		double beforeX = floor(h.x/stepX)*stepX+ min.x;
		double beforeY = floor(h.y/stepY)*stepY+ min.y;
		double afterX  =  ceil(h.x/stepX)*stepX+ min.x;
		double afterY  =  ceil(h.y/stepY)*stepY+ min.y;
			
		double tx = (pos.x - beforeX)/stepX;
		double ty = (pos.y - beforeY)/stepY;

		result  = getData(Vector2(beforeX,beforeY))*(typename ComplexTraits::ComplexPrecision)((1.-tx)*(1.-ty));
		result += getData(Vector2(afterX, beforeY))*(typename ComplexTraits::ComplexPrecision)(    tx *(1.-ty));
		result += getData(Vector2(beforeX,afterY ))*(typename ComplexTraits::ComplexPrecision)((1.-tx)*    ty );
		result += getData(Vector2(afterX, afterY ))*(typename ComplexTraits::ComplexPrecision)(    tx *    ty );

		return result;
	}

	template <typename ComplexTraits>
	void TFFT2D<ComplexTraits>::setData(const Vector2& pos, Complex val)
	{
		Complex dummy;
	
		if (!inFourierSpace_)
		{
			dummy = Complex(val.real()*((float)pow((float)(lengthX_*lengthY_),(int)numFourierToPhys_)),
												val.imag()*((float)pow((float)(lengthX_*lengthY_),(int)numFourierToPhys_)));
	
			(*this)[pos]=dummy;
		}
		else
		{
			val*=phase(pos)*(typename ComplexTraits::ComplexPrecision)((2*M_PI/(stepPhysX_*stepPhysY_)))
										 *(typename ComplexTraits::ComplexPrecision)pow((typename ComplexTraits::ComplexPrecision)(lengthX_*lengthY_),(int)numFourierToPhys_);
			
			dummy = val;
		
			(*this)[pos]=dummy;
		}
	}

	template <typename ComplexTraits>
	typename TFFT2D<ComplexTraits>::Complex& TFFT2D<ComplexTraits>::operator[](const Vector2& pos)
	{
		Index internalPos;

		if (!inFourierSpace_)
		{
			Index i, j;
			
			i = (Index) Maths::rint((pos.x+origin_.x)/stepPhysX_);
			j = (Index) Maths::rint((pos.y+origin_.y)/stepPhysY_);

			internalPos = j + i*lengthY_;
		}
		else
		{
			Index i, j;

			i = (Index) Maths::rint(pos.x/stepFourierX_);
			j = (Index) Maths::rint(pos.y/stepFourierY_);

			if (i<0)
			{
				i+=lengthX_;
			}

			if (j<0)
			{
				j+=lengthY_;
			}

			internalPos = (j + i*lengthY_);
		}

		if ((internalPos < 0) || (internalPos>=(Index) (lengthX_*lengthY_)))
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		}
		
		return operator [] (internalPos);
	}

	template <typename ComplexTraits>
	const typename TFFT2D<ComplexTraits>::Complex& TFFT2D<ComplexTraits>::operator[](const Vector2& pos) const
	{
		Index internalPos;

		if (!inFourierSpace_)
		{
			Index i, j;
			
			i = (Index) Maths::rint((pos.x+origin_.x)/stepPhysX_);
			j = (Index) Maths::rint((pos.y+origin_.y)/stepPhysY_);

			internalPos = j + i*lengthY_;
		}
		else
		{
			Index i, j;

			i = (Index) Maths::rint(pos.x/stepFourierX_);
			j = (Index) Maths::rint(pos.y/stepFourierY_);

			if (i<0)
			{
				i+=lengthX_;
			}

			if (j<0)
			{
				j+=lengthY_;
			}

			internalPos = (j + i*lengthY_);
		}

		if ((internalPos < 0) || (internalPos>=(Index) (lengthX_*lengthY_)))
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		}
		
		return operator [] (internalPos);
	}
	
	template <typename ComplexTraits>
	typename TFFT2D<ComplexTraits>::Complex TFFT2D<ComplexTraits>::phase(const Vector2& pos) const
	{
	  double phase = 2.*M_PI*(  Maths::rint(pos.x/stepFourierX_)*Maths::rint(origin_.x/stepPhysX_)
															/lengthX_
														+ Maths::rint(pos.y/stepFourierY_)*Maths::rint(origin_.y/stepPhysY_)
															/lengthY_ );

		Complex result = Complex(cos(phase), sin(phase));
						
		return result;
	}
	
	template <typename ComplexTraits>
	bool TFFT2D<ComplexTraits>::isInFourierSpace() const
	{
		return inFourierSpace_;
	}
	
	template <typename ComplexTraits>
	const TRegularData2D<typename TFFT2D<ComplexTraits>::Complex>& operator << 
		(TRegularData2D<typename TFFT2D<ComplexTraits>::Complex>& to, const TFFT2D<ComplexTraits>& from)
	{
		// first decide if the FFT3D data is in Fourier space.
		if (!from.isInFourierSpace())
		{
			// create a new grid
			Size lengthX = from.getMaxXIndex()+1;
			Size lengthY = from.getMaxYIndex()+1;
			
			TRegularData2D<typename TFFT2D<ComplexTraits>::Complex> newGrid(TRegularData2D<typename TFFT2D<ComplexTraits>::Complex>::IndexType(lengthX, lengthY),
																			Vector2(from.getPhysSpaceMinX(), from.getPhysSpaceMinY()),
																			Vector2(from.getPhysSpaceMaxX(), from.getPhysSpaceMaxY()));

			// and fill it
			double normalization=1./(pow((float)(lengthX*lengthY),from.getNumberOfInverseTransforms()));
			typename TFFT2D<ComplexTraits>::Complex dataIn;
			typename TFFT2D<ComplexTraits>::Complex dataOut;
			
			for (Position i = 0; i < from.size(); i++)
			{
				Position x, y;

				y =  i % lengthY;
				x =  i / lengthY;

				dataIn  = from[i];
				dataOut = dataIn;
				
				newGrid[x + y*lengthY] = dataOut*(typename ComplexTraits::ComplexPrecision)normalization;
			}

			to = newGrid;

			return to;
		}
		else
		{
			// we are in Fourier space
			
			// create a new grid
			Size lengthX = from.getMaxXIndex()+1;
			Size lengthY = from.getMaxYIndex()+1;
			//float stepPhysX = from.getPhysStepWidthX();
			//float stepPhysY = from.getPhysStepWidthY();
			float stepFourierX = from.getFourierStepWidthX();
			float stepFourierY = from.getFourierStepWidthY();


			
			TRegularData2D<typename TFFT2D<ComplexTraits>::Complex> newGrid(TRegularData2D<typename TFFT2D<ComplexTraits>::Complex>::IndexType(lengthX, lengthY),
																			Vector2(from.getFourierSpaceMinX(), 
																							from.getFourierSpaceMinY()),
																			Vector2(from.getFourierSpaceMaxX(),
																							from.getFourierSpaceMaxY()));

			// and fill it
			// AR: old double normalization=1./(sqrt(2.*M_PI))*(stepPhysX*stepPhysY*stepPhysZ)/(pow((float)(lengthX*lengthY*lengthZ),from.getNumberOfInverseTransforms()));
			double normalization=1./(2.*M_PI)/(pow((float)(lengthX*lengthY),from.getNumberOfInverseTransforms()));
			
			
			Index x, y;
			Vector2 r;
			typename TFFT2D<ComplexTraits>::Complex dataIn;
			typename TFFT2D<ComplexTraits>::Complex dataOut;
	
			for (Position i = 0; i < from.size(); i++)
			{
				y =  i % lengthY;
				x =  i / lengthY;

				if (x>lengthX/2.)
				{
					x-=lengthX;
				}

				if (y>lengthY/2.)
				{
					y-=lengthY;
				}

				r.set((float)x * stepFourierX,
							(float)y * stepFourierY);

				dataIn = from[i];
				dataOut = dataIn;
				
				newGrid[x + y*lengthY] = dataOut*(typename ComplexTraits::ComplexPrecision)normalization*from.phase(r);
			}

			to = newGrid;

			return to;
		}
	}
	
	template <typename ComplexTraits>
	const RegularData2D& operator << (RegularData2D& to, const TFFT2D<ComplexTraits>& from)
	{
		// first decide if the FFT2D data is in Fourier space.
		if (!from.isInFourierSpace())
		{
			// create a new grid
			Size lengthX = from.getMaxXIndex()+1;
			Size lengthY = from.getMaxYIndex()+1;
			
			RegularData2D newGrid(RegularData2D::IndexType(lengthX, lengthY),
														Vector2(from.getPhysSpaceMinX(), 
																		from.getPhysSpaceMinY()),
														Vector2(from.getPhysSpaceMaxX(),
																		from.getPhysSpaceMaxY()));

			// and fill it
			double normalization = 1./(pow((float)(lengthX*lengthY),from.getNumberOfInverseTransforms()));
			typename TFFT2D<ComplexTraits>::Complex dataIn;
			typename TFFT2D<ComplexTraits>::Complex dataOut;

			typename TFFT2D<ComplexTraits>::IndexType current_index;
			typename RegularData2D::IndexType regdat_index;
			for (current_index.x = 0; current_index.x < lengthX; current_index.x++)
			{
				for (current_index.y = 0; current_index.y < lengthY; current_index.y++)
				{
					regdat_index.x = current_index.x;
					regdat_index.y = current_index.y;

					dataIn  = from[current_index];
					dataOut = dataIn;
				
					newGrid[regdat_index] = dataOut.real()*normalization;
				}
			}

			to = newGrid;

			return to;
		}
		else
		{
			// we are in Fourier space
			
			// create a new grid
			Size lengthX = from.getMaxXIndex()+1;
			Size lengthY = from.getMaxYIndex()+1;
			//float stepPhysX = from.getPhysStepWidthX();
			//float stepPhysY = from.getPhysStepWidthY();
			float stepFourierX = from.getFourierStepWidthX();
			float stepFourierY = from.getFourierStepWidthY();
	
			RegularData2D newGrid(RegularData2D::IndexType(lengthX, lengthY), Vector2(from.getFourierSpaceMinX(), from.getFourierSpaceMinY()), Vector2(from.getFourierSpaceMaxX(), from.getFourierSpaceMaxY()));

			// and fill it
			// AR: old version double normalization=1./(sqrt(2.*M_PI))*(stepPhysX*stepPhysY*stepPhysZ)/(pow((float)(lengthX*lengthY*lengthZ),from.getNumberOfInverseTransforms()));
			double normalization=1./(2.*M_PI)/(pow((float)(lengthX*lengthY),from.getNumberOfInverseTransforms()));
			
			Index x, y;
			signed int xp, yp;
			Vector2 r;
			typename TFFT2D<ComplexTraits>::Complex dataIn;
			typename TFFT2D<ComplexTraits>::Complex dataOut;
	
			RegularData2D::IndexType current_index;
			for (Position i = 0; i < from.size(); i++)
			{
				y =  i % lengthY;
				x =  i / lengthY;
				
				xp = x;
				yp = y;

				if (xp>=lengthX/2.)
				{
					xp-=(int)lengthX;
				}
				if (yp>=lengthY/2.)
				{
					yp-=(int)lengthY;
				}

				if (x>=lengthX/2.)
				{
					x-=(int)(lengthX/2.);
				}
				else
				{
					x+=(int)(lengthX/2.);
				}

				if (y>=lengthY/2.)
				{
					y-=(int)(lengthY/2.);
				}
				else
				{
					y+=(int)(lengthY/2.);
				}


				r.set((float)xp * stepFourierX,
							(float)yp * stepFourierY);

				dataIn = from[i];
				dataOut = dataIn;

				current_index.x = x;
				current_index.y = y;

				newGrid[current_index] = (dataOut*(typename ComplexTraits::ComplexPrecision)normalization*from.phase(r)).real();
			}

			to = newGrid;

			return to;
		}
	}
	
	
	
}

#endif // BALL_MATHS_TFFT2D_H
