// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: FFT3D.h,v 1.1.2.6 2002/11/07 19:21:55 anhi Exp $

#ifndef BALL_MATHS_FFT3D_H
#define BALL_MATHS_FFT3D_H

#ifndef BALL_COMMON_H
# include <BALL/common.h>
#endif

#ifndef BALL_MATHS_COMPLEX_H
# include <BALL/MATHS/complex.h>
#endif

#ifndef BALL_DATATYPE_REGULARDATA3D_H
# include <BALL/DATATYPE/regularData3D.h>
#endif

#ifndef BALL_MATHS_VECTOR2_H
# include <BALL/MATHS/vector2.h>
#endif

#include <math.h>
#include <fftw.h>

namespace BALL
{

	/** A class to perform Fast Fourier Transforms and inverse Fast Fourier Transforms
			on regularly spaced three dimensional data. \\
			This class makes use of the freely available library {\bf FFTW}, which can be
			found at \URL{www.fftw.org}. Normalization, translation and scaling of the
			coordinate system can be handled automatically. The normalization is chosen
			symmetrically.
			\\
			{\bf Definition:} \URL{BALL/MATHS/FFT3D.h}
	 */

	class FFT3D : public TRegularData3D<FFTW_COMPLEX>
	{
		public:

      BALL_CREATE(FFT3D)

      /**  @name Constructors and Destructors
       */
      //@{
 
      /// Default constructor
      FFT3D()
        throw();

			/// Copy constructor
			FFT3D(const FFT3D &data)
				throw();

			/** Detailed constructor. \\
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
			FFT3D(Size ldnX, Size ldnY, Size ldnZ, double stepPhysX=1., double stepPhysY=1., 
						double stepPhysZ=1., Vector3 origin=Vector3(0.,0.,0), bool inFourierSpace=false)
				throw();
      
			/// Destructor
			virtual ~FFT3D()
				throw();
			
			//@}

			/** @name Assignment
			 */
			//@{

			/// Assignment operator
			const FFT3D& operator = (const FFT3D& FFT3D)
				throw();
			
			/** Clear the contents.
			 */
			virtual void clear()
				throw();
			
			/** Clear the contents and reset all attributes.
			 */
			virtual void destroy()
				throw();

			//@}

			/** @name Predicates
			 */
			//@{

			/** Equality operator.
			 */
			bool operator == (const FFT3D& FFT3D) const
				throw();
			//@}
			
			// @name Accessors
		
			//@{
			/** Perform a single fast Fourier transform on the data.
			 */
			void doFFT()
				throw();

			/** Perform a single inverse Fourier transform on the data.
			 */
			void doiFFT()
				throw();

			/** Translate the origin in physical space about {\em trans_origin},
					i.e. the new origin will be located at the former position {\em trans_origin}.
					If the result is out of bounds, the function does nothing and
					returns {\em false}.
			 */
			bool translate(const Vector3& trans_origin)
				throw();

			/** Set the step width in physical space to {\em new_width_x, new_width_y, new_width_z}.
				The step width in Fourier space is automatically adjusted
				accordingly. {\em new_width_x, new_width_y and new_width_z} must be positive, otherwise
				the function does nothing and retuns {\em false}.
			 */
			bool setPhysStepWidth(double new_width_x, double new_width_y, double new_width_z)
				throw();

			/** Returns the step width in physical space in X direction.
			 */
			double getPhysStepWidthX() const
				throw();

			/** Returns the step width in physical space in Y direction.
			 */
			double getPhysStepWidthY() const
				throw();

			/** Returns the step width in physical space in Z direction.
			 */
			double getPhysStepWidthZ() const
				throw();

			/** Returns the step width in Fourier space in X direction.
			 */
			double getFourierStepWidthX() const
				throw();

			/** Returns the step width in Fourier space in Y direction.
			 */
			double getFourierStepWidthY() const
				throw();

			/** Returns the step width in Fourier space in Z direction.
			 */
			double getFourierStepWidthZ() const
				throw();

			/** Returns the minimal position of the grid in physical space in X direction.
			 */
			double getPhysSpaceMinX() const
				throw();

			/** Returns the minimal position of the grid in physical space in Y direction.
			 */
			double getPhysSpaceMinY() const
				throw();

			/** Returns the minimal position of the grid in physical space in Z direction.
			 */
			double getPhysSpaceMinZ() const
				throw();

			/** Returns the maximal position of the grid in physical space in X direction.
			 */
			double getPhysSpaceMaxX() const
				throw();

			/** Returns the maximal position of the grid in physical space in Y direction.
			 */
			double getPhysSpaceMaxY() const
				throw();

			/** Returns the maximal position of the grid in physical space in Z direction.
			 */
			double getPhysSpaceMaxZ() const
				throw();

			/** Returns the minimal position of the grid in Fourier space in X direction.
			 */
			double getFourierSpaceMinX() const
				throw();

			/** Returns the minimal position of the grid in Fourier space in Y direction.
			 */
			double getFourierSpaceMinY() const
				throw();

			/** Returns the minimal position of the grid in Fourier space in Z direction.
			 */
			double getFourierSpaceMinZ() const
				throw();

			/** Returns the maximal position of the grid in Fourier space in X direction.
			 */
			double getFourierSpaceMaxX() const
				throw();

			/** Returns the maximal position of the grid in Fourier space in Y direction.
			 */
			double getFourierSpaceMaxY() const
				throw();

			/** Returns the maximal position of the grid in Fourier space in Z direction.
			 */
			double getFourierSpaceMaxZ() const
				throw();

			/** Return the largest grid position for the x direction. 
			 		This method returns the maximum position allowed in the grid. As the point 
					in the origin has the indices (0, 0, 0), this method returns the number of 
					points in X direction minus one.
			  */
			Size getMaxXIndex() const
				throw();

			/** Return the largest grid position for the y direction. 
			 		This method returns the maximum position allowed in the grid. As the point 
					in the origin has the indices (0, 0, 0), this method returns the number of 
					points in Y direction minus one.
			  */
			Size getMaxYIndex() const
				throw();

			/** Return the largest grid position for the z direction. 
			 		This method returns the maximum position allowed in the grid. As the point 
					in the origin has the indices (0, 0, 0), this method returns the number of 
					points in Z direction minus one.
			  */
			Size getMaxZIndex() const
				throw();

			/** Return the number of inverse transforms that have been carried out using this class.
			 		This is an important factor for the normalization of the data.
			 */
			Size getNumberOfInverseTransforms() const
				throw();
			
			/** Returns the grid coordinate corresponding to the position.
			 */
			Vector3 getGridCoordinates(Position position) const
				throw();
			
			/** Returns the data at the grid position closest to {\em pos},
				and automatically includes
				the correct phase factor and (symmetric) normalization.
			 */
			Complex getData(const Vector3& pos) const
				throw(Exception::OutOfGrid);

			/** Returns the data at point {\bf pos}. If {\bf pos} is not a 
			 		point on the grid, the data is linearly interpolated.
					This method automatically includes the correct phase factor
					and (symmetric) normalization.
				*/
			Complex getInterpolatedValue(const Vector3& pos) const
				throw(Exception::OutOfGrid);

			/** Sets the data point at the grid position closest to {\em pos}
				to the value {\em val}, and -- if called in Fourier space --
				automatically includes the correct phase factor and 
				(symmetric) normalization.
			 */
			void setData(const Vector3& pos, Complex val)
				throw(Exception::OutOfGrid);

			/** Access the data at the grid position closest to {\em pos}.
				This function returns the "raw" data at that position.
			 */
			FFTW_COMPLEX& operator[](const Vector3& pos)
				throw(Exception::OutOfGrid);

			/** Access the data at the grid position closest to {\em pos}.
			 		This function returns the "raw" data at that position.
				*/
			const FFTW_COMPLEX& operator[](const Vector3& pos) const
				throw(Exception::OutOfGrid);
			
			/** Access the (raw) data at Position pos.
			 */
			FFTW_COMPLEX& operator[](const Position& pos)
				throw(Exception::OutOfGrid);

			/** Access the (raw) data at Position pos. Const method.
				*/
			const FFTW_COMPLEX& operator[](const Position& pos) const
				throw(Exception::OutOfGrid);
				
			/** This computes the phase factor in Fourier space that results
				if the origin of the coordinate system in physical space
				is not in the "lower left corner".
			 */
			Complex phase(const Vector3& pos) const
				throw();
			//@}
			
			/** @name Predicates
			 */
			//@{

			/** Returns {\bf true} if the data is considered to be in Fourier space,
			 		{\bf false} otherwise.
			 */
			bool isInFourierSpace() const
				throw();
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
			fftwnd_plan planForward_;
			fftwnd_plan planBackward_;
	};

	/** Global assignment operator from FFT3D to TRegularData3D<Complex>
	 */
	const TRegularData3D<Complex>& operator << (TRegularData3D<Complex>& to, const FFT3D& from)
		throw();
	
	/** Global assignment operator from FFT3D to TRegularData3D<float>.
	 		This operator assigns the {\bf real} part of the complex FFT3D-data to the
			TRegularData3D<float> to.
	 */
	const RegularData3D& operator << (RegularData3D& to, const FFT3D& from)
		throw();
	
}

#endif // BALL_MATHS_FFT3D_H
