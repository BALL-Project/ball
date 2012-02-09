// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_MATHS_TFFT3D_H
#define BALL_MATHS_TFFT3D_H

#ifndef BALL_COMMON_EXCEPTION_H
# include <BALL/COMMON/exception.h>
#endif


#ifndef BALL_DATATYPE_REGULARDATA3D_H
# include <BALL/DATATYPE/regularData3D.h>
#endif

//#ifndef BALL_MATHS_VECTOR2_H
//# include <BALL/MATHS/vector3.h>
//#endif

#include <BALL/MATHS/fftwCommon.h>
#include <cmath>
#include <complex>
#include <fftw3.h>

namespace BALL
{
	/** A class to perform Fast Fourier Transforms and inverse Fast Fourier Transforms
			on regularly spaced three dimensional data.  \par
			This class makes use of the freely available library <b>FFTW</b>, which can be
			found at www.fftw.org
			coordinate system can be handled automatically. The normalization is chosen
			symmetrically.
			 \par
			S/TFFT3D.h
	 \ingroup FFT
	 */
	template <typename ComplexTraits>
	class TFFT3D 
		: public TRegularData3D<std::complex<typename ComplexTraits::ComplexPrecision> >
	{
		public:
		
			typedef std::complex<typename ComplexTraits::ComplexPrecision> Complex;
			typedef TRegularData3D<std::complex<typename ComplexTraits::ComplexPrecision> >	ComplexVector;

			BALL_CREATE(TFFT3D)

			/**  @name Constructors and Destructors
			*/
			//@{
		
			/// Default constructor
			TFFT3D();
		
			/// Copy constructor
			TFFT3D(const TFFT3D &data);

			/** Detailed constructor.  \par
			 		@param ldnX The binary logarithm of the number of grid points in X direction (we use the logarithm to
										 ensure that the number of points is a power of two, which is important for
										 the FFT)
					@param ldnY The binary logarithm of the number of grid points in Y direction
					@param ldnZ The binary logarithm of the number of grid points in Z direction
					@param stepPhysX The step width in X direction in physical space
					@param stepPhysY The step width in Y direction in physical space
					@param stepPhysZ The step width in Z direction in physical space
					@param origin The origin of the coordinate system
					@param inFourierSpace Flag to decide whether the data is assumed to be in physical or Fourier
																space
			 */
			 // AR: ldn is not any longer the binary logarithm but the absolute number of grid points
			TFFT3D(Size ldnX, Size ldnY, Size ldnZ, double stepPhysX=1., double stepPhysY=1., double stepPhysZ=1., Vector3 origin=Vector3(0.,0.,0), bool inFourierSpace=false);

			/// Destructor
			virtual ~TFFT3D();
					
					//@}
		
					/** @name Assignment
					*/
					//@{
		
					/// Assignment operator
			const TFFT3D& operator = (const TFFT3D& fft_3d);
		
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
 			bool operator == (const TFFT3D& fft3d) const;
			//@}
			
			// @name Accessors
		
			//@{
			/** Perform a single fast Fourier transform on the data.
			 */
			void doFFT();

			/** Perform a single inverse Fourier transform on the data.
			 */
			void doiFFT();

			/** Translate the origin in physical space about {\em trans_origin},
					i.e. the new origin will be located at the former position {\em trans_origin}.
					If the result is out of bounds, the function does nothing and
					returns <b>  false </b>.
			 */
			bool translate(const Vector3& trans_origin);

			/** Set the step width in physical space to {\em new_width_x, new_width_y, new_width_z}.
				The step width in Fourier space is automatically adjusted
				accordingly. {\em new_width_x, new_width_y and new_width_z} must be positive, otherwise
				the function does nothing and retuns <b>  false </b>.
			 */
			bool setPhysStepWidth(double new_width_x, double new_width_y, double new_width_z);

			/** Returns the step width in physical space in X direction.
			 */
			double getPhysStepWidthX() const;

			/** Returns the step width in physical space in Y direction.
			 */
			double getPhysStepWidthY() const;

			/** Returns the step width in physical space in Z direction.
			 */
			double getPhysStepWidthZ() const;

			/** Returns the step width in Fourier space in X direction.
			 */
			double getFourierStepWidthX() const;

			/** Returns the step width in Fourier space in Y direction.
			 */
			double getFourierStepWidthY() const;

			/** Returns the step width in Fourier space in Z direction.
			 */
			double getFourierStepWidthZ() const;

			/** Returns the minimal position of the grid in physical space in X direction.
			 */
			double getPhysSpaceMinX() const;

			/** Returns the minimal position of the grid in physical space in Y direction.
			 */
			double getPhysSpaceMinY() const
;

			/** Returns the minimal position of the grid in physical space in Z direction.
			 */
			double getPhysSpaceMinZ() const;

			/** Returns the maximal position of the grid in physical space in X direction.
			 */
			double getPhysSpaceMaxX() const;

			/** Returns the maximal position of the grid in physical space in Y direction.
			 */
			double getPhysSpaceMaxY() const;

			/** Returns the maximal position of the grid in physical space in Z direction.
			 */
			double getPhysSpaceMaxZ() const;

			/** Returns the minimal position of the grid in Fourier space in X direction.
			 */
			double getFourierSpaceMinX() const;

			/** Returns the minimal position of the grid in Fourier space in Y direction.
			 */
			double getFourierSpaceMinY() const;

			/** Returns the minimal position of the grid in Fourier space in Z direction.
			 */
			double getFourierSpaceMinZ() const;

			/** Returns the maximal position of the grid in Fourier space in X direction.
			 */
			double getFourierSpaceMaxX() const;

			/** Returns the maximal position of the grid in Fourier space in Y direction.
			 */
			double getFourierSpaceMaxY() const;

			/** Returns the maximal position of the grid in Fourier space in Z direction.
			 */
			double getFourierSpaceMaxZ() const;

			/** Return the largest grid position for the x direction. 
			 		This method returns the maximum position allowed in the grid. As the point 
					in the origin has the indices (0, 0, 0), this method returns the number of 
					points in X direction minus one.
			  */
			Size getMaxXIndex() const;

			/** Return the largest grid position for the y direction. 
			 		This method returns the maximum position allowed in the grid. As the point 
					in the origin has the indices (0, 0, 0), this method returns the number of 
					points in Y direction minus one.
			  */
			Size getMaxYIndex() const;

			/** Return the largest grid position for the z direction. 
			 		This method returns the maximum position allowed in the grid. As the point 
					in the origin has the indices (0, 0, 0), this method returns the number of 
					points in Z direction minus one.
			  */
			Size getMaxZIndex() const;

			/** Return the number of inverse transforms that have been carried out using this class.
			 		This is an important factor for the normalization of the data.
			 */
			Size getNumberOfInverseTransforms() const;
			
			/** Returns the grid coordinate corresponding to the position.
			 */
			Vector3 getGridCoordinates(Position position) const;
			
			/** Returns the data at the grid position closest to <b>  pos </b>,
			 *	and automatically includes
			 *	the correct phase factor and (symmetric) normalization.
			 *
			 *  @throw Exception::OutOfGrid if pos is outside the grid boundaries
			 */
			Complex getData(const Vector3& pos) const;

			/** Returns the data at point <b>pos</b>. If <b>pos</b> is not a 
			 *	point on the grid, the data is linearly interpolated.
			 *	This method automatically includes the correct phase factor
			 *	and (symmetric) normalization.
			 *
			 *  @throw Exception::OutOfGrid if pos is outside the grid boundaries
			 */
			Complex getInterpolatedValue(const Vector3& pos) const;

			/** Sets the data point at the grid position closest to <b>  pos </b>
			 *  to the value <b>  val </b>, and -- if called in Fourier space --
			 *  automatically includes the correct phase factor and 
			 *  (symmetric) normalization.
			 *
			 *  @throw Exception::OutOfGrid if pos is outside the grid boundaries
			 */
			void setData(const Vector3& pos, Complex val);

			/** Access the data at the grid position closest to <b>  pos </b>.
			 *  This function returns the "raw" data at that position.
			 *
			 *  @throw Exception::OutOfGrid if pos is outside the grid boundaries
			 */
			Complex& operator[](const Vector3& pos);

			/** Access the data at the grid position closest to <b>  pos </b>.
			 *	This function returns the "raw" data at that position.
			 *
			 *  @throw Exception::OutOfGrid if pos is outside the grid boundaries
			 */
			const Complex& operator[](const Vector3& pos) const;

			/** Access the (raw) data at Position pos.
			 *
			 *  @throw Exception::OutOfGrid if pos is outside the grid boundaries
			 */
			Complex& operator[](const Position& pos)
			{
				return TRegularData3D<Complex>::operator [] (pos);
			}

			/** Access the (raw) data at Position pos. Const method.
			 *
			 *  @throw Exception::OutOfGrid if pos is outside the grid boundaries
			 */
			const Complex& operator[](const Position& pos) const
			{
				return TRegularData3D<Complex>::operator [] (pos);
			}
			
			// AR:
			void setNumberOfFFTTransforms(Size num)
			{
				numPhysToFourier_ = num;
			}
			
			// AR:
			void setNumberOfiFFTTransforms(Size num)
			{
				numFourierToPhys_ = num;
			}
			
				
			/** This computes the phase factor in Fourier space that results
				if the origin of the coordinate system in physical space
				is not in the "lower left corner".
			 */
			Complex phase(const Vector3& pos) const;
			//@}
			
			/** @name Predicates
			 */
			//@{

			/** Returns <b>true</b> if the data is considered to be in Fourier space,
			 		<b>false</b> otherwise.
			 */
			bool isInFourierSpace() const;
			//@}
			
		protected:
			Size lengthX_, lengthY_, lengthZ_;
			bool inFourierSpace_;
			Size numPhysToFourier_;
			Size numFourierToPhys_;
			Vector3 origin_;
			double stepPhysX_, stepPhysY_, stepPhysZ_;
			double stepFourierX_, stepFourierY_, stepFourierZ_;
      Vector3 minPhys_, maxPhys_;
      Vector3 minFourier_, maxFourier_;
			
			
			// AR: new version for FFTW3
			typename ComplexTraits::FftwPlan planForward_;
			typename ComplexTraits::FftwPlan planBackward_;
			
			// AR: to control plan calculation with new fftw3
			Size dataLength_;
			Complex *dataAdress_;
			bool planCalculated_;
			
	};
	
	/**	Default type
	*/
	typedef TFFT3D<BALL_FFTW_DEFAULT_TRAITS> FFT3D;

	/** Global assignment operator from TFFT3D to TRegularData3D<Complex>
	 */
	template <typename ComplexTraits>
	const TRegularData3D<typename TFFT3D<ComplexTraits>::Complex>& operator << 
		(TRegularData3D<typename TFFT3D<ComplexTraits>::Complex>& to, const TFFT3D<ComplexTraits>& from);
	
	/** Global assignment operator from TFFT3D to TRegularData3D<float>.
	 		This operator assigns the <b>real</b> part of the complex TFFT3D-data to the
			TRegularData3D<float> to.
	 */
	template <typename ComplexTraits>
	const RegularData3D& operator << (RegularData3D& to, const TFFT3D<ComplexTraits>& from);
		
		
	template <typename ComplexTraits>
	TFFT3D<ComplexTraits>::TFFT3D()
		: TRegularData3D<Complex>(),
			dataLength_(0),
			dataAdress_(0),
			planCalculated_(false)
	{
	}
	
	template <typename ComplexTraits>
	bool TFFT3D<ComplexTraits>::operator == (const TFFT3D& fft3D) const
	{
		
		// AR: test whether data_.size() == fft3D.data_.size()
		//     instead of testing 3 lengths. Better for vector handling.
		
		if (lengthX_ == fft3D.lengthX_ &&
				lengthY_ == fft3D.lengthY_ &&
				lengthZ_ == fft3D.lengthZ_ &&
				origin_ == fft3D.origin_ &&
				stepPhysX_ == fft3D.stepPhysX_ &&
				stepPhysY_ == fft3D.stepPhysY_ &&
				stepPhysZ_ == fft3D.stepPhysZ_ &&
				stepFourierX_ == fft3D.stepFourierX_ &&
				stepFourierY_ == fft3D.stepFourierY_ &&
				stepFourierZ_ == fft3D.stepFourierZ_ &&
				minPhys_ == fft3D.minPhys_ &&
				maxPhys_ == fft3D.maxPhys_ &&
				minFourier_ == fft3D.minFourier_ &&
				maxFourier_ == fft3D.maxFourier_ &&
				numPhysToFourier_ == fft3D.numPhysToFourier_ &&
				numFourierToPhys_ == fft3D.numFourierToPhys_ &&
				planCalculated_ == fft3D.planCalculated_)
		{
			Vector3 min  = inFourierSpace_ ?  minFourier_  :   minPhys_;
			Vector3 max  = inFourierSpace_ ?  maxFourier_  :   maxPhys_;
			double stepX = inFourierSpace_ ? stepFourierX_ : stepPhysX_;
			double stepY = inFourierSpace_ ? stepFourierY_ : stepPhysY_;	
			double stepZ = inFourierSpace_ ? stepFourierZ_ : stepPhysZ_;	
			
			for (double posX=min.x; posX<=max.x; posX+=stepX)
			{
				for (double posY=min.y; posY<=max.y; posY+=stepY)
				{
					for (double posZ=min.z; posZ<=max.z; posZ+=stepZ)
					{
						if (getData(Vector3(posX,posY,posZ)) != fft3D.getData(Vector3(posX,posY,posZ)))
						{
							return false;
						}
					}
				}
			}
			
			return true;
		}
	
		return false;
	}
	
	template <typename ComplexTraits>
	bool TFFT3D<ComplexTraits>::translate(const Vector3& trans_origin)
	{
		Position internalOriginX = (Position) Maths::rint(trans_origin.x*stepPhysX_);
		Position internalOriginY = (Position) Maths::rint(trans_origin.y*stepPhysY_);
		Position internalOriginZ = (Position) Maths::rint(trans_origin.z*stepPhysZ_);
		
		if ((internalOriginX <= lengthX_) && (internalOriginY <= lengthY_) && (internalOriginZ <= lengthZ_))
		{
			origin_.x = trans_origin.x;
			origin_.y = trans_origin.y;
			origin_.z = trans_origin.z;
			
	    minPhys_ = Vector3(-origin_.x,-origin_.y,-origin_.z);
			maxPhys_ = Vector3(((lengthX_-1)*stepPhysX_)-origin_.x,((lengthY_-1)*stepPhysY_)-origin_.y,((lengthZ_-1)*stepPhysZ_)-origin_.z);
			minFourier_ = Vector3(-(lengthX_/2.-1)*stepFourierX_,-(lengthY_/2.-1)*stepFourierY_,-(lengthZ_/2.-1)*stepFourierZ_);
			maxFourier_ = Vector3((lengthX_/2.)*stepFourierX_,(lengthY_/2.)*stepFourierY_,(lengthZ_/2.)*stepFourierZ_);
	 
			return true;
		}
		else
		{
			return false;
		}
	}

	template <typename ComplexTraits>
	bool TFFT3D<ComplexTraits>::setPhysStepWidth(double new_width_x, double new_width_y, double new_width_z)
	{
		if ((new_width_x <= 0) || (new_width_y <= 0) || (new_width_z <= 0))
		{
			return false;
		}
		else
		{
			stepPhysX_ = new_width_x;
			stepPhysY_ = new_width_y;
			stepPhysZ_ = new_width_z;
			stepFourierX_ = 2.*M_PI/(stepPhysX_*lengthX_);
			stepFourierY_ = 2.*M_PI/(stepPhysY_*lengthY_);
			stepFourierZ_ = 2.*M_PI/(stepPhysZ_*lengthZ_);

			minPhys_ = Vector3(-origin_.x,-origin_.y,-origin_.z);
			maxPhys_ = Vector3(((lengthX_-1)*stepPhysX_)-origin_.x,((lengthY_-1)*stepPhysY_)-origin_.y,((lengthZ_-1)*stepPhysZ_)-origin_.z);
			minFourier_ = Vector3(-(lengthX_/2.-1)*stepFourierX_,-(lengthY_/2.-1)*stepFourierY_,-(lengthZ_/2.-1)*stepFourierZ_);
			maxFourier_ = Vector3((lengthX_/2.)*stepFourierX_,(lengthY_/2.)*stepFourierY_,(lengthZ_/2.)*stepFourierZ_);
	
      return true;
		}
	}
	
	template <typename ComplexTraits>
	double TFFT3D<ComplexTraits>::getPhysStepWidthX() const
	{
		return stepPhysX_;
	}

	template <typename ComplexTraits>
	double TFFT3D<ComplexTraits>::getPhysStepWidthY() const
	{
		return stepPhysY_;
	}

	template <typename ComplexTraits>
	double TFFT3D<ComplexTraits>::getPhysStepWidthZ() const
	{
		return stepPhysZ_;
	}
	
	template <typename ComplexTraits>
	double TFFT3D<ComplexTraits>::getFourierStepWidthX() const
	{
		return stepFourierX_;
	}

	template <typename ComplexTraits>
	double TFFT3D<ComplexTraits>::getFourierStepWidthY() const
	{
		return stepFourierY_;
	}

	template <typename ComplexTraits>
	double TFFT3D<ComplexTraits>::getFourierStepWidthZ() const
	{
		return stepFourierZ_;
	}

	template <typename ComplexTraits>
	double TFFT3D<ComplexTraits>::getPhysSpaceMinX() const
	{
    return minPhys_.x;
  }

	template <typename ComplexTraits>
	double TFFT3D<ComplexTraits>::getPhysSpaceMinY() const
	{
    return minPhys_.y;
  }

	template <typename ComplexTraits>
	double TFFT3D<ComplexTraits>::getPhysSpaceMinZ() const
	{
    return minPhys_.z;
  }

	template <typename ComplexTraits>
	double TFFT3D<ComplexTraits>::getPhysSpaceMaxX() const
	{
    return maxPhys_.x;
 	}

	template <typename ComplexTraits>
	double TFFT3D<ComplexTraits>::getPhysSpaceMaxY() const
	{
    return maxPhys_.y;
 	}

	template <typename ComplexTraits>
	double TFFT3D<ComplexTraits>::getPhysSpaceMaxZ() const
	{
    return maxPhys_.z;
 	}
	
	template <typename ComplexTraits>
	double TFFT3D<ComplexTraits>::getFourierSpaceMinX() const

	{
		return minFourier_.x;
	}

	template <typename ComplexTraits>
	double TFFT3D<ComplexTraits>::getFourierSpaceMinY() const
	{
		return minFourier_.y;
	}

	template <typename ComplexTraits>
	double TFFT3D<ComplexTraits>::getFourierSpaceMinZ() const
	{
		return minFourier_.z;
	}

	template <typename ComplexTraits>
	double TFFT3D<ComplexTraits>::getFourierSpaceMaxX() const
	{
		return maxFourier_.x;
	}

	template <typename ComplexTraits>
	double TFFT3D<ComplexTraits>::getFourierSpaceMaxY() const
	{
		return maxFourier_.y;
	}

	template <typename ComplexTraits>
	double TFFT3D<ComplexTraits>::getFourierSpaceMaxZ() const
	{
		return maxFourier_.z;
	}

	template <typename ComplexTraits>
	Size TFFT3D<ComplexTraits>::getMaxXIndex() const
	{
		return (lengthX_ - 1);
	}
	
	template <typename ComplexTraits>
	Size TFFT3D<ComplexTraits>::getMaxYIndex() const
	{
		return (lengthY_ - 1);
	}
	
	template <typename ComplexTraits>
	Size TFFT3D<ComplexTraits>::getMaxZIndex() const
	{
		return (lengthZ_ - 1);
	}
	
	template <typename ComplexTraits>
	Size TFFT3D<ComplexTraits>::getNumberOfInverseTransforms() const
	{
		return numFourierToPhys_;
	}

	template <typename ComplexTraits>
	Vector3 TFFT3D<ComplexTraits>::getGridCoordinates(Position position) const
	{
		if (!inFourierSpace_)
		{
			if (position >= ComplexVector::size())
			{
				throw Exception::OutOfGrid(__FILE__, __LINE__);
			}
		
			Vector3 r;
			Position  x, y, z;

			z = position % lengthZ_;
			y = (position % (lengthY_ * lengthZ_)) / lengthZ_;
			x =  position / (lengthY_ * lengthZ_);

			r.set(-origin_.x + (float)x * stepPhysX_,
						-origin_.y + (float)y * stepPhysY_,
						-origin_.z + (float)z * stepPhysZ_);

			return r;
		}
		else
		{
			if (position >= ComplexVector::size())
			{
				throw Exception::OutOfGrid(__FILE__, __LINE__);
			}
		
			Vector3 r;
			Index x, y, z;
	
			z = position % lengthZ_;
			y = (position % (lengthY_ * lengthZ_)) / lengthZ_;
			x =  position / (lengthY_ * lengthZ_);

			if (x>=lengthX_/2.)
			{
				x-=lengthX_;
			}
			
			if (y>=lengthY_/2.)
			{
				y-=lengthY_;
			}

			if (z>=lengthZ_/2.)
			{
				z-=lengthZ_;
			}

			r.set((float)x * stepFourierX_,
						(float)y * stepFourierY_,
						(float)z * stepFourierZ_);

			return r;
		}
	}
	
	template <typename ComplexTraits>
	typename TFFT3D<ComplexTraits>::Complex TFFT3D<ComplexTraits>::getData(const Vector3& pos) const
	{
		Complex result;
		double normalization=1.;

		if (!inFourierSpace_)
		{
			result = (*this)[pos];
			normalization=1./((float)pow((float)(lengthX_*lengthY_*lengthZ_),(int)numFourierToPhys_));
		}
		else
		{
			// AR:
			//old: result = (*this)[pos];
			result = (*this)[pos]*phase(pos);
			
			//normalization=1./pow(sqrt(2.*M_PI),3)*(stepPhysX_*stepPhysY_*stepPhysZ_)/((float)pow((float)(lengthX_*lengthY_*lengthZ_),(int)numFourierToPhys_));
			
			normalization=1./pow(sqrt(2.*M_PI),3)/((float)pow((float)(lengthX_*lengthY_*lengthZ_),(int)numFourierToPhys_));
			//normalization=1./(sqrt(2.*M_PI))*(stepPhysX_*stepPhysY_*stepPhysZ_)/((float)pow((float)(lengthX_*lengthY_*lengthZ_),(int)numFourierToPhys_));
		}

		result *= normalization;
		
		return result;
	}

	template <typename ComplexTraits>
	typename TFFT3D<ComplexTraits>::Complex TFFT3D<ComplexTraits>::getInterpolatedValue(const Vector3& pos) const
	{
		Complex result;
		
		Vector3 min  = inFourierSpace_ ? minFourier_   :   minPhys_;
		Vector3 max  = inFourierSpace_ ? maxFourier_   :   maxPhys_;
		double stepX = inFourierSpace_ ? stepFourierX_ : stepPhysX_;
		double stepY = inFourierSpace_ ? stepFourierY_ : stepPhysY_;
		double stepZ = inFourierSpace_ ? stepFourierZ_ : stepPhysZ_;
		
		if (    (pos.x < min.x) || (pos.y < min.y) || (pos.z < min.z)
				 || (pos.x > max.x) || (pos.y > max.y) || (pos.z > max.z) )
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		}

		Vector3 h(pos.x - min.x, pos.y - min.y, pos.z - min.z);
		double modX = fmod((double)h.x,stepX);
		double modY = fmod((double)h.y,stepY);
		double modZ = fmod((double)h.z,stepZ);

		if (modX==0 && modY==0 && modZ==0) // we are on the grid
		{
			return getData(pos);
		}

		double beforeX = floor(h.x/stepX)*stepX+min.x;
		double beforeY = floor(h.y/stepY)*stepY+min.y;
		double beforeZ = floor(h.z/stepZ)*stepZ+min.z;
		double afterX  =  ceil(h.x/stepX)*stepX+min.x;
		double afterY  =  ceil(h.y/stepY)*stepY+min.y;
		double afterZ  =  ceil(h.z/stepZ)*stepZ+min.z;
			
		double tx = (pos.x - beforeX)/stepX;
		double ty = (pos.y - beforeY)/stepY;
		double tz = (pos.z - beforeZ)/stepZ;

		result  = getData(Vector3(beforeX,beforeY,beforeZ))*(typename ComplexTraits::ComplexPrecision)((1.-tx)*(1.-ty)*(1.-tz));
		result += getData(Vector3(afterX, beforeY,beforeZ))*(typename ComplexTraits::ComplexPrecision)(    tx *(1.-ty)*(1.-tz));
		result += getData(Vector3(beforeX,afterY, beforeZ))*(typename ComplexTraits::ComplexPrecision)((1.-tx)*    ty *(1.-tz));
		result += getData(Vector3(beforeX,beforeY,afterZ ))*(typename ComplexTraits::ComplexPrecision)((1.-tx)*(1.-ty)*    tz );
		result += getData(Vector3(afterX, afterY, beforeZ))*(typename ComplexTraits::ComplexPrecision)(    tx *    ty *(1.-tz));
		result += getData(Vector3(afterX, beforeY,afterZ ))*(typename ComplexTraits::ComplexPrecision)(    tx *(1.-ty)*    tz );
		result += getData(Vector3(beforeX,afterY, afterZ ))*(typename ComplexTraits::ComplexPrecision)((1.-tx)*    ty *    tz );
		result += getData(Vector3(afterX, afterY, afterZ ))*(typename ComplexTraits::ComplexPrecision)(    tx *    ty *    tz );

		return result;
	}

	template <typename ComplexTraits>
	void TFFT3D<ComplexTraits>::setData(const Vector3& pos, Complex val)
	{
		Complex dummy;
	
		if (!inFourierSpace_)
		{
			dummy = Complex(val.real()*((typename ComplexTraits::ComplexPrecision)pow((typename ComplexTraits::ComplexPrecision)(lengthX_*lengthY_*lengthZ_),(int)numFourierToPhys_)),
												val.imag()*((typename ComplexTraits::ComplexPrecision)pow((typename ComplexTraits::ComplexPrecision)(lengthX_*lengthY_*lengthZ_),(int)numFourierToPhys_)));
	
			(*this)[pos]=dummy;
		}
		else
		{
			val*=phase(pos)*(typename ComplexTraits::ComplexPrecision)((pow(sqrt(2*M_PI),3)/(stepPhysX_*stepPhysY_*stepPhysZ_)))
				             *((typename ComplexTraits::ComplexPrecision)pow((typename ComplexTraits::ComplexPrecision)(lengthX_*lengthY_*lengthZ_),(int)numFourierToPhys_));
		  /*val*=phase(pos)*(typename ComplexTraits::ComplexPrecision)((sqrt(2*M_PI)/(stepPhysX_*stepPhysY_*stepPhysZ_)))
				             *((typename ComplexTraits::ComplexPrecision)pow((typename ComplexTraits::ComplexPrecision)(lengthX_*lengthY_*lengthZ_),(int)numFourierToPhys_));*/
			
			dummy = val;
			
			(*this)[pos]=dummy;
		}
	}

	template <typename ComplexTraits>
	typename TFFT3D<ComplexTraits>::Complex& TFFT3D<ComplexTraits>::operator[](const Vector3& pos)
	{
		Index internalPos;

		if (!inFourierSpace_)
		{
			Index i, j, k;

			i = (Index) Maths::rint((pos.x+origin_.x)/stepPhysX_);
			j = (Index) Maths::rint((pos.y+origin_.y)/stepPhysY_);
			k = (Index) Maths::rint((pos.z+origin_.z)/stepPhysZ_);
			
			internalPos = (k + (j + i*lengthY_)*lengthZ_);
			
			/*(Index) rint(       (pos.z+origin_.z)/stepPhysZ_
																  +	(	  (pos.y+origin_.y)/stepPhysY_
																	    + (pos.x+origin_.x)/stepPhysX_*lengthY_ 
																		) * lengthZ_
																); */
		}
		else
		{
			Index i, j, k;

			i = (Index) Maths::rint(pos.x/stepFourierX_);
			j = (Index) Maths::rint(pos.y/stepFourierY_);
			k = (Index) Maths::rint(pos.z/stepFourierZ_);

			if (i<0)
			{
				i+=lengthX_;
			}

			if (j<0)
			{
				j+=lengthY_;
			}

			if (k<0)
			{
				k+=lengthZ_;
			}

			internalPos = (k + (j + i*lengthY_)*lengthZ_);
		}

		if ((internalPos < 0) || (internalPos>=(Index) (lengthX_*lengthY_*lengthZ_)))
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		}
		
		return TRegularData3D<Complex>::operator[]((Position)internalPos);
	}

	template <typename ComplexTraits>
	const typename TFFT3D<ComplexTraits>::Complex& TFFT3D<ComplexTraits>::operator[](const Vector3& pos) const
	{
		Index internalPos;

		if (!inFourierSpace_)
		{
			Index i, j, k;

			i = (Index) Maths::rint((pos.x+origin_.x)/stepPhysX_);
			j = (Index) Maths::rint((pos.y+origin_.y)/stepPhysY_);
			k = (Index) Maths::rint((pos.z+origin_.z)/stepPhysZ_);
			
			internalPos = (k + (j + i*lengthY_)*lengthZ_);
			
			/*(Index) rint(       (pos.z+origin_.z)/stepPhysZ_
																  +	(	  (pos.y+origin_.y)/stepPhysY_
																	    + (pos.x+origin_.x)/stepPhysX_*lengthY_ 
																		) * lengthZ_
																); */
		}
		else
		{
			Index i, j, k;

			i = (Index) Maths::rint(pos.x/stepFourierX_);
			j = (Index) Maths::rint(pos.y/stepFourierY_);
			k = (Index) Maths::rint(pos.z/stepFourierZ_);

			if (i<0)
			{
				i+=lengthX_;
			}

			if (j<0)
			{
				j+=lengthY_;
			}

			if (k<0)
			{
				k+=lengthZ_;
			}

			internalPos = (k + (j + i*lengthY_)*lengthZ_);
		}

		if ((internalPos < 0) || (internalPos>=(Index) (lengthX_*lengthY_*lengthZ_)))
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		}
		
		return TRegularData3D<Complex>::operator[]((Position)internalPos);
	}

	/*Complex& TFFT3D<ComplexTraits>::operator[](const Position& pos)
	{
		return operator [] (pos);
	}

	const Complex& TFFT3D<ComplexTraits>::operator[](const Position& pos) const
	{
		return operator [] (pos);
	}
*/
	template <typename ComplexTraits>
	typename TFFT3D<ComplexTraits>::Complex TFFT3D<ComplexTraits>::phase(const Vector3& pos) const
	{
		
		// AR: old version: -2.*M_PI...
	  double phase = 2.*M_PI*(  (Maths::rint(pos.x/stepFourierX_))*(Maths::rint(origin_.x/stepPhysX_))
															/lengthX_
														+ (Maths::rint(pos.y/stepFourierY_))*(Maths::rint(origin_.y/stepPhysY_))
															/lengthY_
														+ (Maths::rint(pos.z/stepFourierZ_))*(Maths::rint(origin_.z/stepPhysZ_))
															/lengthZ_ );
	

		Complex result = Complex(cos(phase), sin(phase));
						
		return result;
		
		/*double phase = -2.*M_PI*(  (rint(pos.x/stepFourierX_))*(rint(origin_.x/stepPhysX_))
															/lengthX_
														+ (Maths::rint(pos.y/stepFourierY_))*(Maths::rint(origin_.y/stepPhysY_))
															/lengthY_
														+ (Maths::rint(pos.z/stepFourierZ_))*(Maths::rint(origin_.z/stepPhysZ_))
															/lengthZ_ );
	

		Complex result = Complex(cos(phase), sin(phase));
						
		return result;*/
	}

	template <typename ComplexTraits>
	bool TFFT3D<ComplexTraits>::isInFourierSpace() const
	{
		return inFourierSpace_;
	}
	
	template <typename ComplexTraits>
	const TRegularData3D<typename TFFT3D<ComplexTraits>::Complex>& operator<< 
		(TRegularData3D<typename TFFT3D<ComplexTraits>::Complex>& to, const TFFT3D<ComplexTraits>& from)
	{
		// first decide if the TFFT3D data is in Fourier space.
		if (!from.isInFourierSpace())
		{
			// create a new grid
			Size lengthX = from.getMaxXIndex()+1;
			Size lengthY = from.getMaxYIndex()+1;
			Size lengthZ = from.getMaxZIndex()+1;
			
			TRegularData3D<typename TFFT3D<ComplexTraits>::Complex> newGrid(TRegularData3D<typename TFFT3D<ComplexTraits>::Complex>::IndexType(lengthX, lengthY, lengthZ),
																			Vector3(from.getPhysSpaceMinX(), from.getPhysSpaceMinY(), from.getPhysSpaceMinZ()),
																			Vector3(from.getPhysSpaceMaxX(), from.getPhysSpaceMaxY(), from.getPhysSpaceMaxZ()));

			// and fill it
			double normalization=1./(pow((float)(lengthX*lengthY*lengthZ),(int)from.getNumberOfInverseTransforms()));
			typename TFFT3D<ComplexTraits>::Complex dataIn;
			typename TFFT3D<ComplexTraits>::Complex dataOut;
			
			for (Position i = 0; i < from.size(); i++)
			{
				Position x, y, z;

				z =  i % lengthZ;
				y = (i % (lengthY * lengthZ)) / lengthZ;
				x =  i / (lengthY * lengthZ);

				dataIn  = from[i];
				dataOut = dataIn;
				
				newGrid[x + (y + z*lengthY)*lengthZ] = dataOut*(typename ComplexTraits::ComplexPrecision)normalization;
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
			Size lengthZ = from.getMaxZIndex()+1;
			//float stepPhysX = from.getPhysStepWidthX();
			//float stepPhysY = from.getPhysStepWidthY();
			//float stepPhysZ = from.getPhysStepWidthZ();
			float stepFourierX = from.getFourierStepWidthX();
			float stepFourierY = from.getFourierStepWidthY();
			float stepFourierZ = from.getFourierStepWidthZ();


			
			TRegularData3D<typename TFFT3D<ComplexTraits>::Complex> newGrid(TRegularData3D<typename TFFT3D<ComplexTraits>::Complex>::IndexType(lengthX, lengthY, lengthZ),
																			Vector3(from.getFourierSpaceMinX(), 
																							from.getFourierSpaceMinY(),
																							from.getFourierSpaceMinZ()),
																			Vector3(from.getFourierSpaceMaxX(),
																							from.getFourierSpaceMaxY(),
																							from.getFourierSpaceMaxZ()));

			// and fill it
			// AR: old double normalization=1./(sqrt(2.*M_PI))*(stepPhysX*stepPhysY*stepPhysZ)/(pow((float)(lengthX*lengthY*lengthZ),from.getNumberOfInverseTransforms()));
			double normalization=1./pow(sqrt(2.*M_PI),3)/(pow((float)(lengthX*lengthY*lengthZ),(int)from.getNumberOfInverseTransforms()));
			
			
			Index x, y, z;
			Vector3 r;
			typename TFFT3D<ComplexTraits>::Complex dataIn;
			typename TFFT3D<ComplexTraits>::Complex dataOut;
	
			for (Position i = 0; i < from.size(); i++)
			{
				z =  i % lengthZ;
				y = (i % (lengthY * lengthZ)) / lengthZ;
				x =  i / (lengthY * lengthZ);

				if (x>lengthX/2.)
				{
					x-=lengthX;
				}

				if (y>lengthY/2.)
				{
					y-=lengthY;
				}

				if (z>lengthZ/2.)
				{
					z-=lengthZ;
				}

				r.set((float)x * stepFourierX,
							(float)y * stepFourierY,
							(float)z * stepFourierZ);

				dataIn = from[i];
				dataOut = dataIn;
				
				newGrid[x + (y + z*lengthY)*lengthZ] = dataOut*(typename ComplexTraits::ComplexPrecision)normalization*from.phase(r);
			}

			to = newGrid;

			return to;
		}
	}
	
	template <typename ComplexTraits>
	const RegularData3D& operator << (RegularData3D& to, const TFFT3D<ComplexTraits>& from)
	{
		// first decide if the TFFT3D data is in Fourier space.
		if (!from.isInFourierSpace())
		{
			// create a new grid
			Size lengthX = from.getMaxXIndex()+1;
			Size lengthY = from.getMaxYIndex()+1;
			Size lengthZ = from.getMaxZIndex()+1;
			
			RegularData3D newGrid(RegularData3D::IndexType(lengthX, lengthY, lengthZ), Vector3(from.getPhysSpaceMinX(), from.getPhysSpaceMinY(), from.getPhysSpaceMinZ()),
Vector3(from.getPhysSpaceMaxX(), from.getPhysSpaceMaxY(), from.getPhysSpaceMaxZ()));

			// and fill it
			double normalization = 1./(pow((float)(lengthX*lengthY*lengthZ),(int)from.getNumberOfInverseTransforms()));
			typename TFFT3D<ComplexTraits>::Complex dataIn;
			typename TFFT3D<ComplexTraits>::Complex dataOut;
			
			for (Position i = 0; i < from.size(); i++)
			{
				Position x, y, z;

				z =  i % lengthZ;
				y = (i % (lengthY * lengthZ)) / lengthZ;
				x =  i / (lengthY * lengthZ);

				dataIn  = from[i];
				dataOut = dataIn;
				
				newGrid[x + (y + z*lengthY)*lengthZ] = dataOut.real()*normalization;
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
			Size lengthZ = from.getMaxZIndex()+1;
			//float stepPhysX = from.getPhysStepWidthX();
			//float stepPhysY = from.getPhysStepWidthY();
			//float stepPhysZ = from.getPhysStepWidthZ();
			float stepFourierX = from.getFourierStepWidthX();
			float stepFourierY = from.getFourierStepWidthY();
			float stepFourierZ = from.getFourierStepWidthZ();


			
			RegularData3D newGrid(RegularData3D::IndexType(lengthX, lengthY, lengthZ), Vector3(from.getFourierSpaceMinX(), from.getFourierSpaceMinY(), from.getFourierSpaceMinZ()), Vector3(from.getFourierSpaceMaxX(), from.getFourierSpaceMaxY(), from.getFourierSpaceMaxZ()));

			// and fill it
			// AR: old version double normalization=1./(sqrt(2.*M_PI))*(stepPhysX*stepPhysY*stepPhysZ)/(pow((float)(lengthX*lengthY*lengthZ),from.getNumberOfInverseTransforms()));
			double normalization=1./pow(sqrt(2.*M_PI),3)/(pow((float)(lengthX*lengthY*lengthZ),(int)from.getNumberOfInverseTransforms()));
			
			Index x, y, z;
			signed int xp, yp, zp;
			Vector3 r;
			typename TFFT3D<ComplexTraits>::Complex dataIn;
			typename TFFT3D<ComplexTraits>::Complex dataOut;
	
			for (Position i = 0; i < from.size(); i++)
			{
				z =  i % lengthZ;
				y = (i % (lengthY * lengthZ)) / lengthZ;
				x =  i / (lengthY * lengthZ);
				
				xp = x;
				yp = y;
				zp = z;

				if (xp>=lengthX/2.)
				{
					xp-=(int)lengthX;
				}
				if (yp>=lengthY/2.)
				{
					yp-=(int)lengthY;
				}
				if (zp>=lengthZ/2.)
				{
					zp-=(int)lengthZ;
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

				if (z>=lengthZ/2.)
				{
					z-=(int)(lengthZ/2.);
				}
				else
				{
					z+=(int)(lengthZ/2.);
				}

				r.set((float)xp * stepFourierX,
							(float)yp * stepFourierY,
							(float)zp * stepFourierZ);

				dataIn = from[i];
				dataOut = dataIn;

				newGrid[x + (y + z*lengthY)*lengthZ] = (dataOut*(typename ComplexTraits::ComplexPrecision)normalization*from.phase(r)).real();
			}

			to = newGrid;

			return to;
		}
	}	
}

#endif // BALL_MATHS_TFFT3D_H
