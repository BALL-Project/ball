#include <BALL/MATHS/cubicSpline1D.h>
#include <map>
#include <set>

using namespace std;

namespace BALL 
{
	const int CubicSpline1D::VERBOSITY_LEVEL_CRITICAL = 5;
	const int CubicSpline1D::VERBOSITY_LEVEL_DEBUG = 10;

	CubicSpline1D::CubicSpline1D()
		: sample_positions_(),
			sample_values_(),
			curvature_(),
			return_average_(),
			default_value_(),
			lower_bound_(),
			upper_bound_(), 
			is_natural_(),
			lower_derivative_(),
			upper_derivative_(),
			verbosity_(VERBOSITY_LEVEL_DEBUG)
	{
	}	
	
	CubicSpline1D::CubicSpline1D(const std::vector<float>& sample_positions, 
															 const std::vector<float>& sample_values, bool return_average, 
															 bool is_natural, float lower_derivative, float upper_derivative,
															 int verbosity)
		: sample_positions_(sample_positions),
			sample_values_(sample_values),
			curvature_(),
			return_average_(return_average),
			default_value_(std::numeric_limits<float>::min()),
			lower_bound_(sample_positions[0]),
			upper_bound_(sample_positions[sample_positions.size()-1]),
			is_natural_(is_natural),
			lower_derivative_(lower_derivative),
			upper_derivative_(upper_derivative),
			verbosity_(verbosity)
	{
		// Compute the spline.
		createSpline();	
	}

	CubicSpline1D::CubicSpline1D(const std::vector<float>& sample_positions, 
															 const std::vector<float>& sample_values, float default_value, 
															 bool is_natural, float lower_derivative, float upper_derivative,
															 int verbosity)
		: sample_positions_(sample_positions),
			sample_values_(sample_values),
			curvature_(),
			return_average_(false),
			default_value_(default_value),	
			lower_bound_(sample_positions[0]),
			upper_bound_(sample_positions[sample_positions.size()-1]),
			is_natural_(is_natural),
			lower_derivative_(lower_derivative),
			upper_derivative_(upper_derivative),
			verbosity_(verbosity)
	{
		// Compute the spline.
		createSpline();
	}
	

	CubicSpline1D::CubicSpline1D(const std::vector<float>& sample_positions, 
															 const std::vector<float>& sample_values, float default_value, 
															 float lower_bound, float upper_bound, 
															 bool is_natural, float lower_derivative, float upper_derivative,
															 int verbosity)
		: sample_positions_(sample_positions),
			sample_values_(sample_values),
			curvature_(),
			return_average_(false),
			default_value_(default_value),	
			lower_bound_(lower_bound),
			upper_bound_(upper_bound),
			is_natural_(is_natural),
			lower_derivative_(lower_derivative),
			upper_derivative_(upper_derivative),
			verbosity_(verbosity)
	{
		// Compute the spline.
		createSpline();
	}
	
	
	CubicSpline1D::CubicSpline1D(const std::vector<float>& sample_positions, 
										           const std::vector<float>& sample_values, 
															 float lower_bound, float upper_bound,  
															 bool return_average, float default_value,
															 bool is_natural, float lower_derivative, float upper_derivative,
															 int verbosity)
		: sample_positions_(sample_positions),
			sample_values_(sample_values),
			curvature_(),
			return_average_(return_average),
			default_value_(default_value),	
			lower_bound_(lower_bound),
			upper_bound_(upper_bound),
			is_natural_(is_natural),
			lower_derivative_(lower_derivative),
			upper_derivative_(upper_derivative),
			verbosity_(verbosity)

	{
		// Compute the spline.
		createSpline();			
	}
	
	CubicSpline1D::CubicSpline1D(const CubicSpline1D& cs1D)
		: sample_positions_(cs1D.sample_positions_),
			sample_values_(cs1D.sample_values_),
			curvature_(cs1D.curvature_),
			return_average_(cs1D.return_average_),
			default_value_(cs1D.default_value_),
			lower_bound_(cs1D.lower_bound_),
			upper_bound_(cs1D.upper_bound_), 
			is_natural_(cs1D.is_natural_),
			lower_derivative_(cs1D.lower_derivative_),
			upper_derivative_(cs1D.upper_derivative_),
			verbosity_(cs1D.verbosity_)
	{
	}
			
	CubicSpline1D::~CubicSpline1D()
	{}
			
	void CubicSpline1D::createSpline()
	{
		// Do we have reasonable data?
		if (	 (sample_values_.size() != sample_positions_.size()) 
				&& (verbosity_ >= VERBOSITY_LEVEL_CRITICAL))
		{	
			Log.error() << "CubicSpline1D_::createSpline: number of sample positions != number of sample values" << std::endl;
			return;
		}		
		
		if (return_average_)
		{
			default_value_ = 0.;
			// In case we have too less values or the access-value is out of bound
			// we want to return the average -> compute the average.
			for (Position i=0; i < sample_values_.size(); i++)
			{
				default_value_  += sample_values_[i];
			}
			default_value_ /= sample_values_.size();
		}
			
		//
		// Now we compute the spline.
		// 
		float p, qn, sig, un;
		std::vector<float> u;

		// Set the positions and values.
		int n							= sample_positions_.size();

		// Initialize the vectors.
		curvature_.resize(n,0.);
		u.resize(n,0.); 
		
		if (is_natural_)
		{
			// Natural spline -> second derivative is set to zero 
			// 								-> first derivative does not change 
			curvature_[0] = 0.;        
			u[0] = 0.0;                
		}
		else
		{
			// Use the stored first derivatives of the boundaries.
			curvature_[0] = -0.5;
			u[0] =  (3.0 / (sample_positions_[1] - sample_positions_[0])) * 
							((sample_values_[1]-sample_values_[0]) / (sample_positions_[1]-sample_positions_[0]) - lower_derivative_ );
		}
		
		for (int i=1; i < n-1; i++) 
		{
			// This is the decomposition loop of the tridiagonal algorithm.
			// Curvature_ and u are used for temporary
			// storage of the decomposed factors.
			sig = (sample_positions_[i]-sample_positions_[i-1]) / (sample_positions_[i+1]-sample_positions_[i-1]);
			p 	=  sig * curvature_[i-1] + 2.0;
			curvature_[i] = (sig-1.0)/p;
			u[i] =  (sample_values_[i+1]-sample_values_[i]) / (sample_positions_[i+1]-sample_positions_[i]) 
				    - (sample_values_[i]-sample_values_[i-1]) / (sample_positions_[i]-sample_positions_[i-1]);
			u[i] =  (6.0*u[i] / (sample_positions_[i+1] - sample_positions_[i-1]) - sig*u[i-1])/p;
		}

		if (is_natural_)
		{ // For natural splines, the second derivative is zero
			qn = 0.0;
			un = 0.0; 
		}
		else
		{
		  // The first derivative of the upper bound is given to compute the second derivative! 
		 	qn = 0.5;
		 	un = (3.0/(sample_positions_[n]-sample_positions_[n-1]))*
			    (upper_derivative_ -(sample_values_[n]-sample_values_[n-1])/(sample_positions_[n]-sample_positions_[n-1]));
		}

		curvature_[n-1] = (un - qn*u[n-2])/(qn*curvature_[n-2] + 1.0);

		// Backsubstitution loop of the tridiagonal algorithm.
		for (int k = n-2; k >= 0; k--) 
		{	
			curvature_[k] = curvature_[k]*curvature_[k+1] + u[k]; 
		}	
		return;	
	}

	void CubicSpline1D::setCurvature(std::vector<float> curvature)
	{
		// Do we have enough curvature values?
		if (   (curvature.size() != sample_positions_.size()) 
				&& (verbosity_ >= VERBOSITY_LEVEL_CRITICAL))
		{
			Log.warn()<< "CubicSpline1D_::setCurvature: number of curvature values != number of sample values" << std::endl;
		}
		else
		{
			curvature_ = curvature;
		}

		// NOTE: a subsequent access(es)  will consider the new curvature, since 
		// the operator () evaluates a spline with equation
		// y = A_y_j + B_y_j+1 + C_curv_j + D_curv_j+1
		// A recomputation will overwrite the new curvature.
	}
	
	void CubicSpline1D::setValues(std::vector<float> values, bool recompute)
	{
		sample_values_ = values;
		if (recompute)
		{
			createSpline();
		}
	}
	
	void CubicSpline1D::setPositions(std::vector<float> positions, bool recompute)
	{
		sample_positions_= positions; 
		if (recompute)
		{
			createSpline();
		}
	}

	void CubicSpline1D::setLowerDerivative(float derivative, bool recompute)
	{
		lower_derivative_ = derivative;
		is_natural_ = false;
		if (recompute)
		{
			createSpline();
		}
	}
	
	void CubicSpline1D::setUpperDerivative(float derivative, bool recompute)
	{
		upper_derivative_ = derivative;
		is_natural_ = false;
		if (recompute)
		{
			createSpline();
		}
	}

	void CubicSpline1D::makeNatural(bool recompute)
	{
		is_natural_ = true;
		if (recompute)
		{
			createSpline();
		}
	}

	void CubicSpline1D::setBoudaryDerivatives(float lower_derivative, float upper_derivative, bool recompute)
	{
		is_natural_ = false;
		lower_derivative_ = lower_derivative;
		upper_derivative_ = upper_derivative;
		if (recompute)
		{
			createSpline();
		}
	}

	float CubicSpline1D::operator() (float x)
	{
		unsigned int n = sample_positions_.size();
		// Is this access-value inside the boundaries?
		if (!sample_positions_.empty() && ((x < lower_bound_) || (x > upper_bound_)))
		{
			// Something _really_ bad happened.
			if (		(!return_average_) 
					 && (verbosity_ >= VERBOSITY_LEVEL_CRITICAL))
			{
				Log.warn() << "invalid : access value " << x << " not between "<< sample_positions_[0] << " and " 
									<< sample_positions_[n-1]<< std::endl;
			}
			return default_value_; 
		}

		// Do we have enough points ?
		if (sample_positions_.size() < 3)
		{
			return default_value_; 
		}
			
		// First, we find the indices bracketing the access value x. 
		// We use bisection here.
		int lower_index = 0, upper_index = n-1;
		int index;
		while (upper_index - lower_index > 1) 
		{
			index = (upper_index + lower_index)/2;
			if (sample_positions_[index] > x)
			{
				upper_index = index;
			}
			else
			{
				lower_index = index;
			}
		} 
		
		float spacing = sample_positions_[upper_index] - sample_positions_[lower_index];
		if (	 (spacing == 0.0) 
				&& (verbosity_ >= VERBOSITY_LEVEL_CRITICAL))
		{
			Log.warn() << "Zero length interval" << std::endl;
			return std::numeric_limits<float>::min();
		}

		float a = (sample_positions_[upper_index]-x)/spacing; 
		float b = (x-sample_positions_[lower_index])/spacing;

		float result =  a*sample_values_[lower_index]      + b*sample_values_[upper_index]
						      + ((a*a*a-a)*curvature_[lower_index] + (b*b*b-b)*curvature_[upper_index])*(spacing*spacing)/6.0;

		return result;
	}
}	
