#include <BALL/MATHS/cubicSpline2D.h>
#include <map>
#include <set>

using namespace std;

namespace BALL 
{
	
	const int CubicSpline2D::VERBOSITY_LEVEL_DEBUG = 10;
	const int CubicSpline2D::VERBOSITY_LEVEL_CRITICAL = 5;

	//	Default constructor.
	CubicSpline2D::CubicSpline2D()
		: verbosity_(VERBOSITY_LEVEL_DEBUG)
	{}	
	
	CubicSpline2D::CubicSpline2D(const std::vector<std::vector<float> >& sample_positions_x,
										const std::vector<float>& sample_positions_y,
										const std::vector<std::vector<float> >& sample_values, 
										bool 	return_average,
										bool 	is_natural,  	
										const std::vector<float>& x_lower_derivatives,  
										const std::vector<float>& x_upper_derivatives,
										float y_lower_derivative, 
										float y_upper_derivative,
										int verbosity)
		:	sample_positions_x_(sample_positions_x),
			sample_positions_y_(sample_positions_y),
			splines_(),
			sample_values_(sample_values),
			return_average_(return_average),	
			x_default_values_(),
			y_default_value_(),
			x_lower_bounds_(),
			x_upper_bounds_(),
			y_lower_bound_(),
			y_upper_bound_(),
			x_is_natural_(),
			y_is_natural_(is_natural),
			x_lower_derivatives_(x_lower_derivatives),
			x_upper_derivatives_(x_upper_derivatives),
			y_lower_derivative_(y_lower_derivative),
			y_upper_derivative_(y_upper_derivative),
			verbosity_(verbosity)
	{	
		x_default_values_.resize(sample_positions_y_.size(), std::numeric_limits<float>::min());
		y_default_value_ = std::numeric_limits<float>::min();
		
		// Store the lower and upper bounds.
		// By default, we assume the first and the last values to be the lower and upper bounds.
		x_lower_bounds_.resize(sample_positions_y_.size());
		x_upper_bounds_.resize(sample_positions_y_.size());
		for (Size i = 0; i < sample_positions_x_.size(); i++)
		{
			x_lower_bounds_[i] = sample_positions_x_[i][0];
			x_upper_bounds_[i] = sample_positions_x_[i][ sample_positions_x_[i].size()-1];
		}
		
		y_lower_bound_ = sample_positions_y_[0];
		y_upper_bound_ = sample_positions_y_[sample_positions_y_.size()-1];
	
		if (x_lower_derivatives.size() == (Size) 0.)
		{
			x_lower_derivatives_.resize(sample_positions_y_.size(), 0.);
			x_upper_derivatives_.resize(sample_positions_y_.size(), 0.);
		}

		x_is_natural_.resize(sample_positions_y_.size(), is_natural);

		//  Finally, compute the splines.	
		createBiCubicSpline();
	}
	
	CubicSpline2D::CubicSpline2D(const std::vector<std::vector<float> >& sample_positions_x,
										const std::vector<float>& sample_positions_y,
										const std::vector<std::vector<float> >& sample_values, 
										const std::vector<float>& x_default_values, 
										float y_default_value, 
										const std::vector<float>& x_lower_bounds,
										const std::vector<float>& x_upper_bounds,
										float y_lower_bound,
										float y_upper_bound,
										bool 	is_natural, 	
										const std::vector<float>& x_lower_derivatives,
										const std::vector<float>& x_upper_derivatives,
										float y_lower_derivative,
										float y_upper_derivative,
										int   verbosity)
		: sample_positions_x_(sample_positions_x),
			sample_positions_y_(sample_positions_y),
			splines_(),
			sample_values_(sample_values),
			return_average_(false),
			x_default_values_(x_default_values),
			y_default_value_(y_default_value),
			x_lower_bounds_(x_lower_bounds),
			x_upper_bounds_(x_upper_bounds),
			y_lower_bound_(y_lower_bound),
			y_upper_bound_(y_upper_bound),
			x_is_natural_(vector<bool>()), 
			y_is_natural_(is_natural),
			x_lower_derivatives_(x_lower_derivatives),
			x_upper_derivatives_(x_upper_derivatives),
			y_lower_derivative_(y_lower_derivative),
			y_upper_derivative_(y_upper_derivative),
			verbosity_(verbosity)
	{
		x_is_natural_.resize(sample_positions_y.size(), is_natural);
		
		if (x_lower_derivatives.size() == (Size) 0.)
		{
			x_lower_derivatives_.resize(sample_positions_y.size(), 0.);
			x_upper_derivatives_.resize(sample_positions_y.size(), 0.);
		}

		//  Finally, compute the splines.	
		createBiCubicSpline();
	}
			
	
//------------------------------simple constructors-----------------------------------------------------

	
	CubicSpline2D::CubicSpline2D(const std::vector<float>& sample_positions_x,
										const std::vector<float>& sample_positions_y,
										const std::vector<std::vector<float> >& sample_values, 
										bool 	return_average,
										bool 	is_natural,  	
										const std::vector<float>& x_lower_derivatives,  
										const std::vector<float>& x_upper_derivatives,
										float y_lower_derivative, 
										float y_upper_derivative,
										int 	verbosity)
		:	sample_positions_x_(),
			sample_positions_y_(sample_positions_y),
			splines_(),
			sample_values_(sample_values),
			return_average_(return_average),	
			x_default_values_(),
			y_default_value_(),
			x_lower_bounds_(),
			x_upper_bounds_(),
			y_lower_bound_(),
			y_upper_bound_(),
			x_is_natural_(),
			y_is_natural_(is_natural),
			x_lower_derivatives_(x_lower_derivatives),
			x_upper_derivatives_(x_upper_derivatives),
			y_lower_derivative_(y_lower_derivative),
			y_upper_derivative_(y_upper_derivative),
			verbosity_(verbosity)
	{	
		// Assuming that all rows have the same x-positions, 
		// we have to create a full set of x-positions
		std::vector<std::vector<float> > complete_x_positions(sample_positions_y.size());
		for (Position i=0; i<complete_x_positions.size(); i++)
		{
			complete_x_positions[i] = sample_positions_x;
		}
		
		sample_positions_x_ = complete_x_positions;
	
		x_default_values_.resize(sample_positions_y_.size(), std::numeric_limits<float>::min());
		y_default_value_ = std::numeric_limits<float>::min();
		
		// Store the lower and upper bounds.
		// By default, we assume the first and the last values to be the lower and upper bounds.
		x_lower_bounds_.resize(sample_positions_y.size());
		x_upper_bounds_.resize(sample_positions_y.size());
		for (Size i = 0; i < sample_positions_x.size(); i++)
		{
			x_lower_bounds_[i] = sample_positions_x_[i][0];
			x_upper_bounds_[i] = sample_positions_x_[i][sample_positions_x_[i].size()-1];
		}
		
		y_lower_bound_ = sample_positions_y_[0];
		y_upper_bound_ = sample_positions_y_[sample_positions_y.size()-1];
	
		if (x_lower_derivatives.size() == (Size) 0.)
		{
			x_lower_derivatives_.resize(sample_positions_y_.size(), 0.);
			x_upper_derivatives_.resize(sample_positions_y_.size(), 0.);
		}

		x_is_natural_.resize(sample_positions_y_.size(), is_natural);

		//  Finally, compute the splines.	
		createBiCubicSpline();
	}
	
	CubicSpline2D::CubicSpline2D(const std::vector<float>& sample_positions_x,
										const std::vector<float>& sample_positions_y,
										const std::vector<std::vector<float> >& sample_values, 
										const std::vector<float>& x_default_values, 
										float y_default_value, 
										const std::vector<float>& x_lower_bounds,
										const std::vector<float>& x_upper_bounds,
										float y_lower_bound,
										float y_upper_bound,
										bool 	is_natural, 	
										const std::vector<float>& x_lower_derivatives,
										const std::vector<float>& x_upper_derivatives,
										float y_lower_derivative,
										float y_upper_derivative,
										int		verbosity)
		: sample_positions_x_(),
			sample_positions_y_(sample_positions_y),
			splines_(),
			sample_values_(sample_values),
			return_average_(false),
			x_default_values_(x_default_values),
			y_default_value_(y_default_value),
			x_lower_bounds_(x_lower_bounds),
			x_upper_bounds_(x_upper_bounds),
			y_lower_bound_(y_lower_bound),
			y_upper_bound_(y_upper_bound),
			x_is_natural_(vector<bool>()), 
			y_is_natural_(is_natural),
			x_lower_derivatives_(x_lower_derivatives),
			x_upper_derivatives_(x_upper_derivatives),
			y_lower_derivative_(y_lower_derivative),
			y_upper_derivative_(y_upper_derivative),
			verbosity_(verbosity)
	{
		// Assuming that all rows have the same x-positions, 
		// we have to create a full set of x-positions
		std::vector<std::vector<float> > complete_x_positions(sample_positions_y.size());
		for (Position i=0; i<complete_x_positions.size(); i++)
		{
			complete_x_positions[i] = sample_positions_x;
		}
		
		sample_positions_x_ = complete_x_positions;
		
		x_is_natural_.resize(sample_positions_y_.size(), is_natural);
		
		if (x_lower_derivatives.size() == (Size) 0.)
		{
			x_lower_derivatives_.resize(sample_positions_y_.size(), 0.);
			x_upper_derivatives_.resize(sample_positions_y_.size(), 0.);
		}

		//  Finally, compute the splines.	
		createBiCubicSpline();
	}
	
	// Copy  constructor
	CubicSpline2D::CubicSpline2D(const CubicSpline2D& cs2D)
		: sample_positions_x_(cs2D.sample_positions_x_),
			sample_positions_y_(cs2D.sample_positions_y_),
			splines_(cs2D.splines_),
			sample_values_(cs2D.sample_values_),
			return_average_(cs2D.return_average_),
			x_default_values_(cs2D.x_default_values_), 
			y_default_value_(cs2D.y_default_value_),
			default_value_(cs2D.default_value_),
			x_lower_bounds_(cs2D.x_lower_bounds_),
			x_upper_bounds_(cs2D.x_upper_bounds_),
			y_lower_bound_(cs2D.y_lower_bound_),
			y_upper_bound_(cs2D.y_upper_bound_),
			x_is_natural_(cs2D.x_is_natural_),
			y_is_natural_(cs2D.y_is_natural_),
			x_lower_derivatives_(cs2D.x_lower_derivatives_),
			x_upper_derivatives_(cs2D.x_upper_derivatives_),
			y_lower_derivative_(cs2D.y_lower_derivative_),
			y_upper_derivative_(cs2D.y_upper_derivative_),
			verbosity_(cs2D.verbosity_)
	{
	}
			
 	// Destructor
	CubicSpline2D::~CubicSpline2D()
	{}

	// A complex version to create a 2D Cubic spline. 
	void CubicSpline2D::createBiCubicSpline()
	{
		// For each y sample position
		// a 1D spline upon the corresponding x sample positions 
		// and the x sample values is created and stored.
		int m = sample_positions_x_.size();

		// If neccessary determine the default value.
		if (return_average_)
		{
			default_value_ = 0.;
		}
		for (int j = 0; j < m; j++)
		{
			// compute a 1D spline
			CubicSpline1D cs(sample_positions_x_[j], sample_values_[j], x_lower_bounds_[j], x_upper_bounds_[j], 
											 return_average_, x_default_values_[j], x_is_natural_[j], x_lower_derivatives_[j], x_upper_derivatives_[j]);
			cs.setVerbosity(verbosity_);

			if (return_average_)
			{
				default_value_ += cs.getDefaultValue()/m;
			}
			// store the 1D splines
			splines_.push_back(cs);	
		}
	}

	void CubicSpline2D::setVerbosity(int verbosity)
	{
		verbosity_ = verbosity;
		for (Position i=0; i<splines_.size(); i++)
			splines_[i].setVerbosity(verbosity_);
	}

	float CubicSpline2D::operator () (float x, float y)
	{ 
		// We are looking for the interpolation of the 2D spline
		// at (x,y). 
		// So, we need to evaluate all 1D splines in x direction at value x, 
		// create a temporary 1D spline upon these values and the y positions, and
		// evaluate the temporary spline at value y.
		
		// For the evaluation spline we need
	  // sample values...
		std::vector<float> values;
		
		// The number of 1D splines.
		int n = sample_positions_y_.size();  

		// First, the sample values of our temporary
		// 1D spline are determined by evaluating 
		// for each y position the 1D splines at x.
		for (int i=0; i < n; i++)
		{
			values.push_back(splines_[i](x));
		}

		// Set the default value to a correct value.
		float default_value = y_default_value_;
		if (return_average_)
			default_value = default_value_;	
		
		// Then, a new 1D spline is constructed. 
		CubicSpline1D cs(sample_positions_y_, values, 
										 y_lower_bound_, y_upper_bound_, 
										 return_average_, default_value, 
										 y_is_natural_, y_lower_derivative_, y_upper_derivative_);
		cs.setVerbosity(verbosity_);
		// Evaluate the new spline at y.
		return cs(y);
	}
	
	
	float CubicSpline2D::getXDefaultValue(Index x) const
	{
		if (x >= (Index)x_default_values_.size())
		{	
			throw Exception::OutOfRange(__FILE__, __LINE__);
		}
		else
		{
			return x_default_values_[x];
		}
	}
	
	float CubicSpline2D::getXLowerBounds(Index x) const
	{
		if (x >= (Index)x_lower_bounds_.size())
		{	
			throw Exception::OutOfRange(__FILE__, __LINE__);
		}
		else
		{
			return x_lower_bounds_[x];
		}
	}
	
	float CubicSpline2D::getXUpperBounds(Index x) const
	{
		if (x >= (Index)x_upper_bounds_.size())
		{	
			throw Exception::OutOfRange(__FILE__, __LINE__);
		}
		else
		{
			return x_upper_bounds_[x];
		}
	}
	
	bool CubicSpline2D::isXNatural(Index x)
	{
		if (x > (Index)x_is_natural_.size())
		{	
			throw Exception::OutOfRange(__FILE__, __LINE__);
		}
		else
		{
			return x_is_natural_[x];
		}
	}
	
	/** Sets the flag {\tt is_natural_} for the x-th spline to true. 
	 *  By default the method recomputes the spline. 
	 *  If the argument is false, no recomputation is done.*/
	void CubicSpline2D::makeXNatural(Index x, bool recompute)
	{
		if (x > (Index)x_is_natural_.size())
		{	
			throw Exception::OutOfRange(__FILE__, __LINE__);
		}
		else
		{
			x_is_natural_[x] = true;
		}
		
		if (recompute)
		{
			createBiCubicSpline();
		}
	}
	
	
	void CubicSpline2D::makeAllXNatural(bool recompute)
	{
		for (Size i = 0; i < x_is_natural_.size(); i++)
		{
			x_is_natural_[i] = true;
		}
		if (recompute)
		{
			createBiCubicSpline();
		}
	}
		
	void CubicSpline2D::makeYNatural(bool y_is_natural, bool recompute)
	{ 
		y_is_natural_ = y_is_natural;	
		
		if (recompute)
		{
			createBiCubicSpline();
		}
	}
	
	void CubicSpline2D::setXLowerDerivatives(vector<float> ld, bool recompute)
	{
		x_lower_derivatives_ = ld;
		if (recompute)
		{
			createBiCubicSpline();
		}

	}
	
	void CubicSpline2D::setXUpperDerivatives(vector<float> ud, bool recompute) 
	{
		x_upper_derivatives_ = ud;	
		if (recompute)
		{
			createBiCubicSpline();
		}
	}	
	
	
	void CubicSpline2D::setYLowerDerivative (float ld, bool recompute)
	{
		y_lower_derivative_ = ld;
		if (recompute)
		{
			createBiCubicSpline();
		}
	}
	
	void CubicSpline2D::setYUpperDerivative (float ud, bool recompute)
	{
		y_upper_derivative_ = ud;
		if (recompute)
		{
			createBiCubicSpline();
		}
	}

	
	float CubicSpline2D::getXLowerDerivatives(Index x)
	{
		if (x >= (Index)x_lower_derivatives_.size())
		{	
			throw Exception::OutOfRange(__FILE__, __LINE__);
		}
		else
		{
			return x_lower_derivatives_[x];
		}
	}
	
	float CubicSpline2D::getXUpperDerivatives(Index x)
	{
		if (x >= (Index)x_upper_derivatives_.size())
		{	
			throw Exception::OutOfRange(__FILE__, __LINE__);
		}
		else
		{
			return x_upper_derivatives_[x];
		}
	}
	
	CubicSpline1D& CubicSpline2D::getSpline(Position i) 
	{
		if (i < (Position)splines_.size()) 
			return splines_[i]; 
		else 
			throw Exception::OutOfRange(__FILE__, __LINE__);
	} 
	
	const CubicSpline1D& CubicSpline2D::getSpline(Position i) const 
	{
		if (i < (Position)splines_.size()) 
			return splines_[i]; 
		else 
			throw Exception::OutOfRange(__FILE__, __LINE__);
	}

}
