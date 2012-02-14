#ifndef BALL_MATHS_CUBICSPLINE1D_H
#define BALL_MATHS_CUBICSPLINE1D_H

#include <set>
#include <map>

#ifndef BALL_COMMON_H
# include <BALL/common.h>
#endif

#ifndef BALL_DATATYPE_OPTIONS_H
# include <BALL/DATATYPE/options.h>
#endif

namespace BALL 
{

	class BALL_EXPORT CubicSpline1D
	{
		public:	
			
			static const int VERBOSITY_LEVEL_DEBUG;
			static const int VERBOSITY_LEVEL_CRITICAL;

			/** @name	Constructors and Destructors.
			*/
			//@{
	
			BALL_CREATE(CubicSpline1D)
			

			/**	Default constructor.
			*/
			CubicSpline1D();
			
			/** Detailed constructor.
			 *  Given increasingly sorted positions {\tt sample_positions} and  
			 *  their corresponding values {\tt sample_values}, 
			 *  cubic splines are computed by determing and storing the curvatures of the cubic splines.
			 *  By default, we assume the first and the last values to be the 
			 *  lower and upper bounds.
			 *  If the {\tt return_average} flag is set to true, the {\tt default_value_} is set to the average, 
			 *  otherwise it is set to  <tt> std::numeric_limits<float>::min() </tt>.
			 *  If the {\tt is_natural} flag is set to true, the spline will be natural. 
			 *  Otherwise {\tt lower_derivative} and {\tt upper_derivative} are taken as
			 *  first derivative of the first/last sample position.
			 */
			CubicSpline1D(const std::vector<float>& sample_positions, 
										const std::vector<float>& sample_values, 
										bool return_average = false, 
										bool is_natural = true, 
									  float lower_derivative = 0.0,
										float upper_derivative = 0.0,
										int verbosity = VERBOSITY_LEVEL_DEBUG);
		
			/** Detailed constructor.
			 *  Given increasingly sorted positions {\tt sample_positions} and  
			 *  their corresponding values {\tt sample_values}, 
			 *  cubic splines are computed by determing and storing the curvatures of the cubic splines.
			 *  The {\tt default_value_} is set to the given default_value.
			 *  By default, we assume the first and the last values to be the 
			 *  lower and upper bounds. 
			 *  If the {\tt is_natural} flag is set to true, the spline will be natural. 
			 *  Otherwise {\tt lower_derivative} and {\tt upper_derivative} are taken as
			 *  first derivative of the first/last sample position.

			 */
			CubicSpline1D(const std::vector<float>& sample_positions, 
										const std::vector<float>& sample_values, 
										float default_value,
										bool is_natural = true, 
									  float lower_derivative = 0.0,
										float upper_derivative = 0.0,
										int verbosity = VERBOSITY_LEVEL_DEBUG);
		
			/** Detailed constructor.
			 *  Given increasingly sorted positions {\tt sample_positions} and  
			 *  their corresponding values {\tt sample_values}, 
			 *  a cubic spline is computed by determing and storing the curvatures of the cubic splines.
			 *  The {\tt default_value_} is set to the given default_value.
			 *  The {\tt lower_bound_} variable is set to the given lower_bound value, the 
			 *  {\tt upper_bound_} variable is set to the given upper_bound value. 
			 *  This allows extrapolation. 
			 *  If the {\tt is_natural} flag is set to true, the spline will be natural. 
			 *  Otherwise {\tt lower_derivative} and {\tt upper_derivative} are taken as
			 *  first derivative of the first/last sample position.
			 */
			CubicSpline1D(const std::vector<float>& sample_positions, 
										const std::vector<float>& sample_values, 
										float default_value, 
										float lower_bound, 
										float upper_bound,	
										bool is_natural = true, 
									  float lower_derivative = 0.0,
										float upper_derivative = 0.0,
										int verbosity = VERBOSITY_LEVEL_DEBUG);
			
			/** Detailed constructor.
			 *  Given increasingly sorted positions {\tt sample_positions} and  
			 *  their corresponding values {\tt sample_values}, 
			 *  cubic splines are computed by determing and storing the curvatures of the cubic splines.
			 *  If the {\tt return_average} flag is set to true, the {\tt default_value_} is set to the average, 
			 *  otherwise it is set to  <tt> std::numeric_limits<float>::min() </tt>.
			 *  The {\tt lower_bound_} variable is set to the given lower_bound value, the 
			 *  {\tt upper_bound_} variavle is set to the given upper_bound value. 
			 *  If the {\tt is_natural} flag is set to true, the spline will be natural. 
			 *  Otherwise {\tt lower_derivative} and {\tt upper_derivative} are taken as
			 *  first derivative of the first/last sample position.
			 */
			CubicSpline1D(const std::vector<float>& sample_positions, 
										const std::vector<float>& sample_values, 
										float lower_bound, 
										float upper_bound,  
										bool return_average = false,
										float default_value = std::numeric_limits<float>::min(), 
										bool is_natural = true, 
									  float lower_derivative = 0.0,
										float upper_derivative = 0.0,
										int verbosity = VERBOSITY_LEVEL_DEBUG);


			/**	Copy constructor.
			*/
			CubicSpline1D(const CubicSpline1D& cs1D);
			
			/**	Destructor.
			*/
			virtual ~CubicSpline1D();
	
			///
			void setVerbosity(int verbosity) { verbosity_ = verbosity; }

			///
			int  getVerbosity() const { return verbosity_; }
			
			/** A method to evaluate the spline at the access value x.
			 *  If the access value is out of bound
			 *  or if we have too less points given for the spline 
			 *  the {\tt default_value} is returned. 
			 *  If the spacing is zero, <tt> std::numeric_limits<float>::min() </tt>
			 *  is returned.
			 */
			float operator () (float x);

			/** Get the curvature.
			 */
			std::vector<float> getCurvature() const {return curvature_;}
			
			/** Set the curvature of the spline.
			 *  Note: - the spline is not recomputed, but any access 
			 *  				will consider the new curvature!
			 *  			- a subsequent recomputation will overwrite the 
			 *  			  new curvature!
			 */
			void setCurvature(std::vector<float> curvature);
			
			/** Set the sample values of the spline.
			 *  Note: if the flag <tt> recompute </tt> is set to false, the 
			 *  spline will not be recomputed.
			 */
			void setValues(std::vector<float> values, bool recompute = true);
			
			/** Get the sample values of the spline.
			 */
			std::vector<float> getValues() const {return sample_values_;}
			
			/** Get the sample positions of the spline.
			 */
			std::vector<float> getPositions() const {return sample_positions_;}
			
			/** Set the positions of the spline.
			 *  Note: if the flag <tt> recompute </tt> is set to false, the 
			 *  spline is not recomputed.
			 */
			void setPositions(std::vector<float> positions, bool recompute = true);
			
			/** A method to set the default value of the splines. 
			*/
			void  setDefaultValue(float value) {default_value_ = value;}
			float getDefaultValue() const {return default_value_;}              

			/** Set the upper and lower bounds of the spline. 
			    We allow to set the spline upper and lower bounds to something different from the x-axis extrema.
			    This allows extrapolation.
			*/
			void setLowerBound(float lb) {lower_bound_ = lb;}
			void setUpperBound(float ub) {upper_bound_ = ub;}

			/** Get the lower/upper bounds of the spline
			 */
			float getLowerBound() const {return lower_bound_;}
			float getUpperBound() const {return upper_bound_;}
		
			/** Returns true, if the spline is a natural spline.*/
			bool isNatural() const {return  is_natural_;}
			
			/** Sets the variable {\tt is_natural_} to true. 
			 *  By default the method recomputes the spline. 
			 *  If the argument is false, no recomputation is done.*/
			void makeNatural(bool recompute = true);
			
			/** Sets the first derivatives of the boundary derivatives.
			 *  By default the method recomputes the spline. 
			 *  If the argument is false, no recomputation is done.
			 * */
			void setBoudaryDerivatives(float lower_derivative, float upper_derivative, bool recompute = true);
			
			/** Set the first derivative of the lowest sample point of the spline.
			 *  The spline is  no longer natural! 
			 * 	If the flag <tt> recompute </tt> is set to false, the 
			 * 	spline is not recomputed.
			 * */
			void setLowerDerivative(float derivative, bool recompute = true);
			
			/** Returns the first derivative of the lowest sample point of the spline. */
			float getLowerDerivative() const {return lower_derivative_;}
			
			/** Set the first derivative of the upper sample point of the spline. 
			 *  The spline is  no longer natural.
			 *  If the flag <tt> recompute </tt> is set to false, the 
			 * 	spline is not recomputed.
			 * */
			void setUpperDerivative(float derivative, bool recompute = true);
			
			/** Returns the first derivative of the upper sample point of the spline.  */
			float getUpperDerivative() const  {return upper_derivative_;}

		private :	
			
			/**	A method to create a spline. 
			 *  Note: - we assume the sample positions to be sorted increasingly!
			 *        - this method relies on the correct settings of the {\tt is_natural_ } flag, 
			 *  				the {\tt	lower_derivative_} and 	{\tt	upper_derivative_} variables.
			 *  			 */
			void createSpline(); 
		
			// Sample positions of the spline.
			std::vector<float> sample_positions_;
			// Sample values of the spline.
			std::vector<float> sample_values_;
			// Curvature of the spline.
			std::vector<float> curvature_;
			
			// Flag to denote, if the default values should be set to the average 
			bool return_average_;
			
			/** The default value of the spline.
			 		In case the access-value is out of bounds or if we have to less points, 
					the {\tt default_value_} is returned.
		  */
			float default_value_; 
			
			// Lower bound of the spline
			float lower_bound_;

			// Upper bound of the spline
			float upper_bound_;
			
			// Flag to denote, if the spline is natural 
			bool is_natural_;
			
			// Value of the first derivative of the lower sample position
			float lower_derivative_;
			
			// Value of the first derivative of the upper sample position
			float upper_derivative_;

			///
			int	verbosity_;
	};

}
#endif
