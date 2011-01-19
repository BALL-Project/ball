#ifndef BALL_MATHS_CUBICSPLINE2D_H
#define BALL_MATHS_CUBICSPLINE2D_H

#include <set>
#include <map>

#ifndef BALL_MATHS_CUBICSPLINE1D_H
#	include <BALL/MATHS/cubicSpline1D.h>
#endif

#ifndef BALL_COMMON_EXCEPTION_H
#include <BALL/COMMON/exception.h>
#endif

namespace BALL 
{
	class BALL_EXPORT CubicSpline2D
	{
		public:

			static const int VERBOSITY_LEVEL_DEBUG;
			static const int VERBOSITY_LEVEL_CRITICAL;

			BALL_CREATE(CubicSpline2D)

			//@}
			/** @name	Constructors and Destructors.
			*/
			//@{

			/**	Default constructor.
			*/
			CubicSpline2D();
			

			/** Detailed constructor
				Given the increasingly sorted sample positions {\tt sample_positions_y_} in y direction, 
				increasingly sorted sample positions {\tt sample_positions_x_} in x direction and the
				corresponding sample values {\tt sample_values}
				a 2D spline is created by storing for each y sample position the corresponding 
				1d spline in x direction. 
				By default, we assume the first and the last values to be the lower and upper bounds.
				If the {\tt return_average_} flag is set to true, the default return values are set to the
				spline average.
			 	If the {\tt is_natural} flag is set to true, all 1D splines and the 1D spline in y direction used in the
				{\tt operator() } will be natural. 
			  Otherwise {\tt x_lower_derivatives} and {\tt x_upper_derivatives} are taken as
			 	first derivatives of the first/last sample positions in x direction 
				and {\tt y_lower_derivative} and {\tt y_upper_derivative} are taken as the first derivatives
				of the first/last derivative in y direction. By default, the derivatives are set to zero.
			*/ 
			CubicSpline2D(const std::vector<std::vector<float> >& sample_positions_x,
										const std::vector<float>& sample_positions_y,
										const std::vector<std::vector<float> >& sample_values, 
										bool 	return_average = false,
										bool 	is_natural = true,  	
										const std::vector<float>& x_lower_derivatives = std::vector<float>(),  
										const std::vector<float>& x_upper_derivatives = std::vector<float>(),
										float y_lower_derivative = 0., 
										float y_upper_derivative = 0.,
										int verbosity = VERBOSITY_LEVEL_DEBUG);
		
		

		/** Detailed constructor
				Given the increasingly sorted sample positions {\tt sample_positions_y_} in y direction, 
				increasingly sorted sample positions {\tt sample_positions_x_} in x direction and the
				corresponding sample values {\tt sample_values}
				a 2D spline is created by storing for each y sample position the corresponding 
				1d spline in x direction. 				
				The {\tt return_average} flag is set to false and the default values {\tt x_default_values_} 
				and {\tt y_default_value_} is set to the given values.
				The lower and upper bounds in x and y direction are set to the given values.
			 	If the {\tt is_natural} flag is set to true, all  1D splines and the 1D spline in y direction used in the
				{\tt operator() } will be natural. 
			  Otherwise {\tt x_lower_derivatives} and {\tt x_upper_derivatives} are taken as
			 	first derivatives of the first/last sample positions in x direction 
				and {\tt y_lower_derivative} and {\tt y_upper_derivative} are taken as the first derivatives
				of the first/last derivative in y direction. By default, the derivatives are set to zero.
			*/  
			CubicSpline2D(const std::vector<std::vector<float> >& sample_positions_x,
										const std::vector<float>& sample_positions_y,
										const std::vector<std::vector<float> >& sample_values, 
										const std::vector<float>& x_default_values, 
										float y_default_value, 
										const std::vector<float>& x_lower_bounds,
										const std::vector<float>& x_upper_bounds,
										float y_lower_bound,
										float y_upper_bound,
										bool 	is_natural = true, 	
										const std::vector<float>& x_lower_derivatives = std::vector<float>(), 
										const std::vector<float>& x_upper_derivatives = std::vector<float>(),
										float y_lower_derivative = 0.0,
										float y_upper_derivative = 0.0,
										int verbosity = VERBOSITY_LEVEL_DEBUG);
			
//-------------------------- Constructors with equal x sample positions for all y ------------------------
     //Assumes that all rows have the same x-positions.


/** Detailed constructor
				Given the increasingly sorted sample positions {\tt sample_positions_y_} in y direction, 
				increasingly sorted sample positions {\tt sample_positions_x_} in x direction (same for all splines in x direction! )
				and the corresponding sample values {\tt sample_values}
				a 2D spline is created by storing for each y sample position the corresponding 
				1d spline in x direction. 
				By default, we assume the first and the last values to be the lower and upper bounds.
				If the {\tt return_average_} flag is set to true, the default return values are set to the
				spline average.
			 	If the {\tt is_natural} flag is set to true, all 1D splines and the 1D spline in y direction used in the
				{\tt operator() } will be natural. 
			  Otherwise {\tt x_lower_derivatives} and {\tt x_upper_derivatives} are taken as
			 	first derivatives of the first/last sample positions in x direction 
				and {\tt y_lower_derivative} and {\tt y_upper_derivative} are taken as the first derivatives
				of the first/last derivative in y direction. By default, the derivatives are set to zero.
			*/ 
			CubicSpline2D(const std::vector<float>& sample_positions_x,
										const std::vector<float>& sample_positions_y,
										const std::vector<std::vector<float> >& sample_values, 
										bool 	return_average = false,
										bool 	is_natural = true,  	
										const std::vector<float>& x_lower_derivatives = std::vector<float>(),  
										const std::vector<float>& x_upper_derivatives = std::vector<float>(),
										float y_lower_derivative = 0., 
										float y_upper_derivative = 0.,
										int verbosity = VERBOSITY_LEVEL_DEBUG);
		
		/** Detailed constructor
				Given the increasingly sorted sample positions {\tt sample_positions_y_} in y direction, 
				increasingly sorted sample positions {\tt sample_positions_x_} in x direction and the
				corresponding sample values {\tt sample_values}
				a 2D spline is created by storing for each y sample position the corresponding 
				1d spline in x direction. 				
				The {\tt return_average} flag is set to false and the default values {\tt x_default_values_} 
				and {\tt y_default_value_} is set to the given values.
				The lower and upper bounds in x and y direction are set to the given values.
			 	If the {\tt is_natural} flag is set to true, all  1D splines and the 1D spline in y direction used in the
				{\tt operator() } will be natural. 
			  Otherwise {\tt x_lower_derivatives} and {\tt x_upper_derivatives} are taken as
			 	first derivatives of the first/last sample positions in x direction 
				and {\tt y_lower_derivative} and {\tt y_upper_derivative} are taken as the first derivatives
				of the first/last derivative in y direction. By default, the derivatives are set to zero.
			*/  
			CubicSpline2D(const std::vector<float>& sample_positions_x,
										const std::vector<float>& sample_positions_y,
										const std::vector<std::vector<float> >& sample_values, 
										const std::vector<float>& x_default_values, 
										float y_default_value, 
										const std::vector<float>& x_lower_bounds,
										const std::vector<float>& x_upper_bounds,
										float y_lower_bound,
										float y_upper_bound,
										bool 	is_natural = true, 	
										const std::vector<float>& x_lower_derivatives = std::vector<float>(), 
										const std::vector<float>& x_upper_derivatives = std::vector<float>(),
										float y_lower_derivative = 0.0,
										float y_upper_derivative = 0.0,
										int verbosity = VERBOSITY_LEVEL_DEBUG);

			
			/**	Copy constructor.
			*/
			CubicSpline2D(const CubicSpline2D& cs2D);
			
			/**	Destructor.
			*/
			virtual ~CubicSpline2D();
	
			/// Set the verbosity of the spline computation and evaluation.
			void setVerbosity(int verbosity);

			/** A method to evaluate the 2Dspline at the access values x | y.
			 *  First all spline in y-direction are evaluated at x.
			 *  Based on these values a temporary 1D spline is created, 
			 *  which will be evaluated at y.
			 *  If the access values are out of bound
			 *  or if we have too less points given for a spline 
			 *  the {\tt default_value} is returned. 
			 *  If the spacing is zero, <tt> std::numeric_limits<float>::min() </tt>
			 *  is returned.
			 */
			float operator () (float x, float y); 
					
			/** Returns the x-th default value in x direction.
			 *  @throw Exception::OutOfRange if x >= number_of_default_values
			 */
			float getXDefaultValue(Index x) const;

			/** Returns the y-th default value in x direction.
			 */
			float getYDefaultValue() const {return y_default_value_;}

			// Set the default values in x direction.
			void setXDefaultValues(vector<float> x_default_values) {x_default_values_ = x_default_values;}

			// Set the default values in y direction.
			void setYDefaultValue(float y_default_value) {y_default_value_ = y_default_value;} 
				
			
			// Set the lower/upper bound in y direction
			void  setYLowerBound(float lb) {y_lower_bound_ = lb;}
			void  setYUpperBound(float ub) {y_upper_bound_ = ub;}
			// Get the lower/upper bound in y direction
			float getYLowerBound() {return y_lower_bound_;}
			float getYUpperBound() {return y_upper_bound_;}
			// Set the lower/upper bounds in x direction
			void  setXLowerBounds(vector<float> lb) {x_lower_bounds_ = lb;}
			void  setXUpperBounds(vector<float> ub) {x_upper_bounds_ = ub;}

			/** Returns all lower bounds in x direction.
			 */
			const vector<float>&  getXLowerBounds() const {return x_lower_bounds_ ;}  

			/** Returns all upper bounds in x direction.
			 */
			const vector<float>&  getXUpperBounds() const {return x_upper_bounds_;}  

			/** Get the lower bounds in x direction  
			 *	@throw Exception::OutOfRange if x  >= x_lower_bounds_.size()
			 */
			float getXLowerBounds(Index x) const;  

			/** Get the upper bounds in x direction  
			 *	@throw Exception::OutOfRange if x  >= x_upper_bounds_.size()
			 */
			float getXUpperBounds(Index x) const;  

			/** Returns true if the x-th spline in x direction is natural.
			 *  If x is out of bound,  <tt> std::numeric_limits<float>::min() </tt> is returned.*/ 
			bool isXNatural(Index x);
			vector<bool> isXNatural() const {return x_is_natural_;}

			/** Sets the flag {\tt is_natural_} for the x-th spline to true. 
			 *  By default the method recomputes the spline. 
			 *  If the argument is false, no recomputation is done.*/
			void makeXNatural(Index x, bool recompute = true);

			/** Sets the flag {\tt is_natural_} for all x-th splines to true. 
			 *  By default the method recomputes all splines. 
			 *  If the argument is false, no recomputation is done.*/
			void makeAllXNatural(bool recompute = true);

			/** Sets the flag {\tt y_is_natural_} to true.
			 * 	Since the spline in y direction is created 
			 * 	for each call of {\tt operator ()} new, 
			 * 	recomputation is not necessary.*/
			void makeYNatural(bool y_is_natural, bool recompute = true);

			/** Returns true if the spline in y direction for each call in {\tt operator ()} is natural.
			 */
			bool isYNatural() {return y_is_natural_;}
			
			// Set the lower/upper derivatives in x direction
			void  setXLowerDerivatives(vector<float> ld, bool recompute = true); 
			void  setXUpperDerivatives(vector<float> ud, bool recompute = true); 

			/** Get the lower derivatives in x direction  
			 *  @throw Exception::OutOfRange if x >= x_lower_derivatives_.size()
			 */
			float getXLowerDerivatives(Index x);

			/** Get the upper derivatives in x direction  
			 *  @throw Exception::OutOfRange if x >= x_upper_derivatives_.size()
			 */
			float getXUpperDerivatives(Index x); 

			vector<float>& getXLowerDerivatives() {return x_lower_derivatives_;}
			vector<float>& getXUpperDerivatives() {return x_upper_derivatives_;}

			// Set the lower/upper derivative in y direction
			void setYLowerDerivative (float ld, bool recompute = true); 
			void setYUpperDerivative (float ud, bool recompute = true);

			// Get the lower/upper derivative in y direction
			float getYLowerDerivative() {return y_lower_derivative_;}
			float getYUpperDerivative() {return y_upper_derivative_;}

			
			/** Return the i-th spline
			 *  @throw Exception::OutOfRange if i >= getNumberOfSplines()
			 */
			CubicSpline1D& getSpline(Position i); 

			/** Return the i-th spline
			 *  @throw Exception::OutOfRange if i >= getNumberOfSplines()
			 */
			const CubicSpline1D& getSpline(Position i) const;

			Size getNumberOfSplines() const {return splines_.size();}	
	
		private :
			
			/** Method to create a 2D cubic spline relying to be given _increasingly ordered_ sample positions in y direction, 
			 * for each y position  _increasingly ordered_ sample positions in x direction, and
			 * the corresponding sample values.
			 * If the {\tt return_average_} flag is set to true, the {\tt default_value_} is the average 
			 * of the 1D spline averages, otherwise it is set to  <tt> std::numeric_limits<float>::min() </tt>.
			 * By default the boundaries in y direction are set to the lower and the upper y sample position. 
			 * The {\tt operator ()} returns the 2D interpolation for given x and y values. 
			 */
			void createBiCubicSpline(); 
				
			// Sample x positions of the spline.
			// Note: we allow for each y value different x positions.
			std::vector< std::vector<float> > sample_positions_x_;
			
			// Sample y positions of the spline.
			std::vector<float> 								sample_positions_y_;

			// For each y position a 1D cubic spline is stored.
			std::vector<CubicSpline1D> 				splines_;
			
			// Sample values of the spline.
			std::vector<std::vector<float> > 	sample_values_;
			
			//
			//  Parameters 
			//
			
			// Flag to denote, if the default values should be set to the average of the spline averages. 
			bool 									 return_average_;
			
			/** The default values of the splines in x direction.
			 		In case the access-value of the x-th spline in x direction is out of bounds 
					or if too less points are given, 
					the corresponding {\tt default_value_(x)} is returned.
		  */
			std::vector<float> 		x_default_values_; 
			
			/** The default value of the splines in y direction.
			 		In case the access-value of the y-th spline in y direction is out of bounds or if 
					too less points for this spline are given, 
					the corresponding {\tt default_value_(y)} is returned.
		  */
			float 								y_default_value_; 

			/** The default value of splines in general. If the return_average_ flag is set
			 *  to true, the averall average of the splines is computed. Otherwise it is set 
			 *  to  <tt> std::numeric_limits<float>::min() </tt>.	 */
			float default_value_;
			
			// Lower bounds of the spline in x direction.
			vector<float>	 				x_lower_bounds_;

			// Upper bounds of the spline in x direction.	
			vector<float> 				x_upper_bounds_;
			
			// Lower bound of the splines in y direction.
			float 								y_lower_bound_;

			// Upper bound of the splines in y direction.	
			float 								y_upper_bound_;
			
			
			// Flag to denote, if the splines in x direction is natural.
			vector<bool> 					x_is_natural_;
			
			// Flag to denote, if the splines in y direction are natural.
			bool 									y_is_natural_;

			
			// Values of the first derivatives of the lower x sample position
			vector<float> 				x_lower_derivatives_;
			
			// Values of the first derivatives of the upper x sample position
			vector<float> 				x_upper_derivatives_;
			
			// Value of the first derivatives of the lower y sample position
			float 								y_lower_derivative_;
			
			// Value of the first derivatives of the upper y sample position
			float 								y_upper_derivative_;

			///
			int 									verbosity_;
	};



}
#endif
