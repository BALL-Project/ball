// $Id: lineSearch.C,v 1.1 1999/08/26 08:02:45 oliver Exp $

#include <BALL/MOLMEC/MINIMIZATION/lineSearch.h>
#include <BALL/COMMON/limits.h>

namespace BALL {


	float LineSearchMinimizer::Default::ALPHA = 1e-4;
	float LineSearchMinimizer::Default::BETA = 0.9;
	Size	LineSearchMinimizer::Default::MAX_STEPS = 6;
	float LineSearchMinimizer::Default::MAX_GRADIENT = 0.001;

	const char* LineSearchMinimizer::Option::ALPHA = "alpha";
	const char* LineSearchMinimizer::Option::BETA = "beta";
	const char* LineSearchMinimizer::Option::MAX_STEPS = "max_steps";
	const char*	LineSearchMinimizer::Option::MAX_GRADIENT = "max_gradient";

	// default constructor
	LineSearchMinimizer::LineSearchMinimizer()
		:	EnergyMinimizer(),
			direction_(0)
	{
	}


	// copy constructor 
	LineSearchMinimizer::LineSearchMinimizer(const LineSearchMinimizer& line_search_minimizer, bool /* deep */)
		:	EnergyMinimizer(line_search_minimizer)
	{
		direction_ = line_search_minimizer.direction_;
	}

	// assignment operator
	LineSearchMinimizer& LineSearchMinimizer::operator = (const LineSearchMinimizer& line_search_minimizer)
	{
		EnergyMinimizer::operator = (line_search_minimizer);
		direction_ = line_search_minimizer.direction_;
		return *this;
	}

	// Constructor initialized with a force field
	LineSearchMinimizer::LineSearchMinimizer(ForceField& force_field)
		:	EnergyMinimizer()
	{
		direction_ = 0;
    valid_ = setup(force_field);

    if (!valid_)
    {
      Log.level(LogStream::ERROR) << " line search minimizer setup failed! " << endl;
		}
	}
	
	// Constructor initialized with a force field and a set of options
	LineSearchMinimizer::LineSearchMinimizer(ForceField& force_field, const Options& new_options)
		:	EnergyMinimizer()
	{
		direction_ = 0;
    valid_ = setup(force_field, new_options);

    if (!valid_)
    {
      Log.level(LogStream::ERROR) << " Line search minimizer setup failed! " << endl;
		}
	}

	// destructor
	LineSearchMinimizer::~LineSearchMinimizer()
	{
	}
		
	// virtual function for the specific setup of derived classes
	bool LineSearchMinimizer::specificSetup()
	{
		alpha_ = options.setDefaultReal(LineSearchMinimizer::Option::ALPHA, LineSearchMinimizer::Default::ALPHA);
		beta_ = options.setDefaultReal(LineSearchMinimizer::Option::BETA, LineSearchMinimizer::Default::BETA);
		max_steps_ = (Size)options.setDefaultInteger(LineSearchMinimizer::Option::MAX_STEPS, (long)LineSearchMinimizer::Default::MAX_STEPS);
		max_gradient_ = options.setDefaultReal(LineSearchMinimizer::Option::MAX_GRADIENT, LineSearchMinimizer::Default::MAX_GRADIENT);

		return true;
	}

	// Set the parameter alpha_
	void	LineSearchMinimizer::setAlpha(float alpha)
	{
		alpha_ = alpha;
		options.setReal(LineSearchMinimizer::Option::ALPHA, alpha);
	}

	// Get the parameter alpha_
	float	LineSearchMinimizer::getAlpha() const
	{
		return alpha_;
	}

	// Set the parameter beta_
	void	LineSearchMinimizer::setBeta(float beta)
	{
		beta_ = beta;
		options.setReal(LineSearchMinimizer::Option::BETA, beta);
	}

	// Get the parameter beta_
	float	LineSearchMinimizer::getBeta() const
	{
		return beta_;
	}

	// Set the parameter max_steps_
	void LineSearchMinimizer::setMaxSteps(Size max_steps)
	{
		max_steps_	= max_steps;
		options.setInteger(LineSearchMinimizer::Option::MAX_STEPS, (long)max_steps);
	}

	//	Get the parameter max_steps_
	Size	LineSearchMinimizer::getMaxSteps() const
	{
		return max_steps_;
	}

	// Set the parameter max_gradient_
	void	LineSearchMinimizer::setMaxGradient(float max_gradient)
	{
		max_gradient_	= max_gradient;
		options.setReal(LineSearchMinimizer::Option::MAX_GRADIENT, max_gradient);
	}

	//	Get the parameter max_steps_
	float	LineSearchMinimizer::getMaxGradient() const
	{
		return max_gradient_;
	}

	void LineSearchMinimizer::setDirection(const vector<Vector3>& direction)
	{
		direction_ = const_cast<vector<Vector3>*>(&direction);
	}

	/*	The minimizer optimizes the energy of the system 
			using a modified line search algorithm.
	*/
	bool LineSearchMinimizer::minimize(Size /* max_steps */, bool /* restart */)
	{
		// check whether a direction and a force field are defined
		if (!valid_)
		{
			return false;
		}

		// calculate the initial energy and forces
		float energy1 = force_field_->updateEnergy();
		force_field_->updateForces();

		// calculate the initial gradient
		Size number_of_atoms = direction_->size();
		Size i;
		float gradient1 = 0;
		for (i = 0; i < number_of_atoms; ++i)	
		{
			gradient1 -= force_field_->getAtoms()[i]->getPosition() * (*direction_)[i];
		}
		gradient1 *= Constants::AVOGADRO / 1e13;

		// prerequisite: g(k)^T p(k) < 0 
		// (gradient is negative in the given direction)
		if (gradient1 >= 0.0)
		{
			return false;
		}

		// initial guess: lambda = 1.0
		float lambda2 = 1.0;

		// displace all atoms along p(k) (direction_) for initial lambda
		for (i = 0; i < number_of_atoms; i++)
		{
			force_field_->getAtoms()[i]->getPosition() += (*direction_)[i] * lambda2;
		}

		// calculate energy and forces for lambda = 1.0
		float energy2 = force_field_->updateEnergy();
		force_field_->updateForces();

		// calculate the norm of the current gradient
		float gradient2 = 0;
		for (i = 0; i < number_of_atoms; ++i)	
		{
			gradient2 += - force_field_->getAtoms()[i]->getPosition() * (*direction_)[i];
		}
		gradient2 *= Constants::AVOGADRO / 1e13;

		// remember the current ''position`` along the line
		float lambda = 1.0;
		

		// now iterate until the new lambda fulfils the line search criterions
		// (sufficient decrease + sufficient directional derivative reduction)
		bool done = false;
		while (!done) 
		{		
			// calculate cubic interpolation 
			float d_lambda = lambda2 - 0.0;
			float a = (- 2.0 * (energy2 - energy1) + (gradient1 + gradient2) * d_lambda) 
								/ (d_lambda * d_lambda * d_lambda);
			float b = (3.0 * (energy2 - energy1) - (2.0 * gradient1 + gradient2) * d_lambda)
								/ (d_lambda * d_lambda);
			float c = gradient1;

			// calculate new lambda
			if ((a != 0.0) && ((b * b - 3.0 * a * c) >= 0.0))
			{         // cubic interpolation
				lambda2 = (-b + sqrt(b * b - 3.0 * a * c)) / (3.0 * a);
			} else {  // quadratic interpolation 
				lambda2 = - c / (2.0 * b);
			}

			// translate to the new position and remember the position in lambda
			d_lambda = lambda2 - lambda;
			lambda = lambda2;
			for (i = 0; i < number_of_atoms; i++)
			{
				force_field_->getAtoms()[i]->getPosition() += (*direction_)[i] * d_lambda;
			}

			// update forces and energy
			float energy2 = force_field_->updateEnergy();
			force_field_->updateForces();

			// calculate the inner product of the current gradient and the direction
			float gradient2 = 0;
			for (i = 0; i < number_of_atoms; ++i)	
			{
				gradient2 += - force_field_->getAtoms()[i]->getPosition() * (*direction_)[i];
			}
			gradient2 *= Constants::AVOGADRO / 1e13;


			// check whether the criterions are fulfilled

			// first: sufficient decrease: 
			//   f(x_{k+1}) <= f(x_k) + alpha * lambda * <g(x_k), p_k>
			done = (energy2 <= (energy1 + alpha_ * lambda * gradient1));
			done &= (fabs(gradient2) <= beta_ * fabs(gradient1));

			// if the criterions are not fulfilled, the current lambda
			// will be used to recalculate a new minimum
		}
		
		return true;
	}
	
} // namespace Ball
