// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: energyMinimizer.C,v 1.20 2003/02/05 13:00:18 oliver Exp $

#include <BALL/MOLMEC/MINIMIZATION/energyMinimizer.h>

#include <BALL/COMMON/limits.h>
#include <BALL/MOLMEC/COMMON/forceFieldComponent.h>

using namespace std;

// #define BALL_DEBUG

namespace BALL 
{

	const char* EnergyMinimizer::Option::MAXIMAL_NUMBER_OF_ITERATIONS
		= "maximal_number_of_iterations";
	const char* EnergyMinimizer::Option::ENERGY_OUTPUT_FREQUENCY
		= "energy_output_frequency";
	const char* EnergyMinimizer::Option::SNAPSHOT_FREQUENCY
		= "snapshot_frequency";
	const char* EnergyMinimizer::Option::NUMBER_OF_ITERATION
		= "number_of_iteration";
  const char* EnergyMinimizer::Option::MAX_GRADIENT = "max_gradient";
	const char* EnergyMinimizer::Option::MAXIMUM_DISPLACEMENT = "MAXIMUM_DISPLACEMENT";
  const char* EnergyMinimizer::Option::MAX_SAME_ENERGY = "max_same_energy";
	const char* EnergyMinimizer::Option::ENERGY_DIFFERENCE_BOUND = "energy_difference_bound"; 

	Size EnergyMinimizer::Default::MAXIMAL_NUMBER_OF_ITERATIONS = 1000;
	Size EnergyMinimizer::Default::ENERGY_OUTPUT_FREQUENCY = 50;
	Size EnergyMinimizer::Default::SNAPSHOT_FREQUENCY = Limits<Size>::max();
  Size EnergyMinimizer::Default::MAX_SAME_ENERGY = 20; 
	Size EnergyMinimizer::Default::NUMBER_OF_ITERATION = 0;              // start number 
	float EnergyMinimizer::Default::ENERGY_DIFFERENCE_BOUND = 1e-2;      // in kJ/mol
  float EnergyMinimizer::Default::MAX_GRADIENT = 0.01;                 // in kJ/(mol A) 
	float EnergyMinimizer::Default::MAXIMUM_DISPLACEMENT = 0.8;          // Angstrom

	// default constructor
	EnergyMinimizer::EnergyMinimizer()
		:	initial_grad_(),
			current_grad_(),
			initial_energy_(0.0),
			current_energy_(0.0),
			old_grad_(),
			old_energy_(0.0),
			direction_(),
			valid_(false),
			snapshot_(0),
			force_field_(0),
			number_of_iterations_(0),
			maximal_number_of_iterations_(0),
			energy_output_frequency_(0),
			snapshot_frequency_(0),
			energy_difference_bound_(0.0),
			max_gradient_(0.0F),
			max_same_energy_(0),
			same_energy_counter_(0),
			maximum_displacement_(0.0F),
			force_update_counter_(0),
			energy_update_counter_(0)
	{
	}


	// copy constructor 
	EnergyMinimizer::EnergyMinimizer(const EnergyMinimizer& energy_minimizer)
		:	options(energy_minimizer.options),
			initial_grad_(energy_minimizer.initial_grad_),
			current_grad_(energy_minimizer.current_grad_),
			initial_energy_(energy_minimizer.initial_energy_),
			current_energy_(energy_minimizer.current_energy_),
			old_grad_(energy_minimizer.old_grad_),
			old_energy_(energy_minimizer.old_energy_),
			direction_(energy_minimizer.direction_),
			valid_(energy_minimizer.valid_),
			snapshot_(0),
			force_field_(energy_minimizer.force_field_),
			number_of_iterations_(energy_minimizer.number_of_iterations_),
			maximal_number_of_iterations_(energy_minimizer.maximal_number_of_iterations_),
			energy_output_frequency_(energy_minimizer.energy_output_frequency_),
			snapshot_frequency_(energy_minimizer.snapshot_frequency_),
			energy_difference_bound_(energy_minimizer.energy_difference_bound_),
			max_gradient_(energy_minimizer.max_gradient_),
			max_same_energy_(energy_minimizer.max_same_energy_),
			same_energy_counter_(energy_minimizer.same_energy_counter_),
			maximum_displacement_(energy_minimizer.maximum_displacement_),
			force_update_counter_(energy_minimizer.force_update_counter_),
			energy_update_counter_(energy_minimizer.energy_update_counter_)
	{
	}

	// assignment operator
	const EnergyMinimizer& EnergyMinimizer::operator = (const EnergyMinimizer& energy_minimizer)
	{
		// guard against self assignment
		if (&energy_minimizer != this) 
		{ 	
			// Copy the attributes
			options												= energy_minimizer.options;
			valid_												= energy_minimizer.valid_;
			snapshot_											= energy_minimizer.snapshot_;
			force_field_									= energy_minimizer.force_field_;
			number_of_iterations_					= energy_minimizer.number_of_iterations_;
			maximal_number_of_iterations_	= energy_minimizer.maximal_number_of_iterations_ ;
			energy_output_frequency_			= energy_minimizer.energy_output_frequency_;
			snapshot_frequency_						= energy_minimizer.snapshot_frequency_;
			energy_difference_bound_			= energy_minimizer.energy_difference_bound_ ;
			max_gradient_				          = energy_minimizer.max_gradient_ ;
      max_same_energy_              = energy_minimizer.max_same_energy_; 
			same_energy_counter_          = energy_minimizer.same_energy_counter_;
			maximum_displacement_         = energy_minimizer.maximum_displacement_;
      force_update_counter_         = energy_minimizer.force_update_counter_;
      energy_update_counter_        = energy_minimizer.energy_update_counter_;
	
		}

		return (*this);
	}

	// Constructor initialized with a force field
	EnergyMinimizer::EnergyMinimizer(ForceField& force_field)
	{
		valid_ = setup(force_field);

		if (!valid_) 
		{
			Log.error() << " Energy minimizer setup failed! " << endl;
		}
	}
	
	// Constructor initialized with a force field and a set of options
	EnergyMinimizer::EnergyMinimizer(ForceField& force_field, const Options& new_options)
	{
		valid_ = setup(force_field, new_options);

		if (!valid_) 
		{
			Log.error() << " Energy minimizer setup failed! " << endl;
		}
	}

	// Destructor
	EnergyMinimizer::~EnergyMinimizer()
	{
	}
		
	// Set the number of the current iteration
	void	EnergyMinimizer::setNumberOfIterations(Size number_of_iterations)
	{
		number_of_iterations_ = number_of_iterations;
	}

	Gradient& EnergyMinimizer::getGradient()
	{
		return current_grad_;
	}

	Gradient& EnergyMinimizer::getInitialGradient()
	{
		return initial_grad_;
	}

	double EnergyMinimizer::getEnergy() const
	{
		return current_energy_;
	}

	double& EnergyMinimizer::getEnergy()
	{
		return current_energy_;
	}

	double EnergyMinimizer::getInitialEnergy() const
	{
		return initial_energy_;
	}

	double& EnergyMinimizer::getInitialEnergy()
	{
		return initial_energy_;
	}

	Gradient& EnergyMinimizer::getDirection()
	{
		return direction_;
	}

	// Get the number of the current iteration
	Size	EnergyMinimizer::getNumberOfIterations() const
	{
		return number_of_iterations_;
	}

	// 
	void EnergyMinimizer::storeGradientEnergy() 
	{
		initial_energy_ = current_energy_;
		initial_grad_ = current_grad_;
	}

	// Set the maximal number of iterations
	void	EnergyMinimizer::setMaxNumberOfIterations(Size maximal_number_of_iterations)
	{
		maximal_number_of_iterations_ = maximal_number_of_iterations;
	}

	// Get the maximal number of iterations
	Size	EnergyMinimizer::getMaxNumberOfIterations() const
	{
		return maximal_number_of_iterations_;
	}

	// Is the energy minimizer valid: did the setup work?
	bool	EnergyMinimizer::isValid() const
	{
		return valid_;
	}

	//	Set the energy output frequency
	void EnergyMinimizer::setEnergyOutputFrequency(Size energy_output_frequency)
	{
		energy_output_frequency_ = energy_output_frequency;
	}

	//	Get the energy ouput frequency
	Size EnergyMinimizer::getEnergyOutputFrequency() const
	{
		return energy_output_frequency_;
	}

	// Set the energy difference bound
	void EnergyMinimizer::setEnergyDifferenceBound(float energy_difference_bound)
	{
		energy_difference_bound_ = energy_difference_bound;
	}


  // Set explicitly the option max_gradient_
  void  EnergyMinimizer::setMaxGradient(float max_gradient)
  {
    max_gradient_ = max_gradient;
  }


  // Get the current value of the maximum gradient bound
  float EnergyMinimizer::getMaxGradient() const
	{
    return max_gradient_;
  }


  // Set explicitly the number of iterations for detecting convergence due
  // to invariant energy 
  void  EnergyMinimizer::setMaxSameEnergy(Size number)
	{
    max_same_energy_ = number;
	}

  // Get the value of max_same_energy, i.e. the number
  // of iterations after which the algorithm is stopped when the
  // energy remains constant
  Size  EnergyMinimizer::getMaxSameEnergy() const
	{
    return max_same_energy_;
	}



	// Get the energy difference bound
	float	EnergyMinimizer::getEnergyDifferenceBound() const
	{
		return energy_difference_bound_;
	}

	//	Set the maximal shift
	void EnergyMinimizer::setMaximumDisplacement(float displacement)
	{
		maximum_displacement_ = displacement;
	}

	//	Get the maximal shift
	float	EnergyMinimizer::getMaximumDisplacement() const
	{
		return maximum_displacement_;
	}

	//	Set the trajectory ouput frequency
	void EnergyMinimizer::setSnapShotFrequency(Size snapshot_frequency)
	{
		snapshot_frequency_ = snapshot_frequency;
	}

	//	Get the trajectory ouput frequency
	Size EnergyMinimizer::getSnapShotFrequency() const
	{
		return snapshot_frequency_;
	}

	// Get the force field of the energy minimizer
	ForceField*	EnergyMinimizer::getForceField()
	{
		return force_field_;
	}

	Size EnergyMinimizer::getForceUpdateCounter() const
		throw()
	{
		return force_update_counter_;
	}

	Size EnergyMinimizer::getEnergyUpdateCounter() const
		throw()
	{
		return energy_update_counter_;
	}

	// setup methods
	bool EnergyMinimizer::setup(ForceField& force_field)
	{
    // Default: no snapshot manager available 
    snapshot_ = 0;

		// store the specified force field
		force_field_ = &force_field; 

		valid_ = force_field_->isValid();

		if (!valid_)
		{
			Log.error() << "EnergyMinimizer: The force field of the energy minimizer is not valid! " 
									<< "Check the definition and initialization of the force field! " << endl;
			return valid_;
		}

		// Check options
		maximal_number_of_iterations_ = (Size)options.setDefaultInteger
			(EnergyMinimizer::Option::MAXIMAL_NUMBER_OF_ITERATIONS, 
			 (long)EnergyMinimizer::Default::MAXIMAL_NUMBER_OF_ITERATIONS);

		energy_output_frequency_ = (Size)options.setDefaultInteger
			(EnergyMinimizer::Option::ENERGY_OUTPUT_FREQUENCY, 
			 (long)EnergyMinimizer::Default::ENERGY_OUTPUT_FREQUENCY);

		snapshot_frequency_ = (Size)options.setDefaultInteger
			(EnergyMinimizer::Option::SNAPSHOT_FREQUENCY, 
			 (long)EnergyMinimizer::Default::SNAPSHOT_FREQUENCY);

		number_of_iterations_ = (Size)options.setDefaultInteger
			(EnergyMinimizer::Option::NUMBER_OF_ITERATION, 
			 (long)EnergyMinimizer::Default::NUMBER_OF_ITERATION);

    max_same_energy_ = (Size)options.setDefaultInteger
			(EnergyMinimizer::Option::MAX_SAME_ENERGY, 
			 (long) EnergyMinimizer::Default::MAX_SAME_ENERGY);

		energy_difference_bound_ = options.setDefaultReal
			(EnergyMinimizer::Option::ENERGY_DIFFERENCE_BOUND, 
			 EnergyMinimizer::Default::ENERGY_DIFFERENCE_BOUND);

    max_gradient_ = options.setDefaultReal
			(EnergyMinimizer::Option::MAX_GRADIENT,
			 EnergyMinimizer::Default::MAX_GRADIENT);

		maximum_displacement_ = options.setDefaultReal
			(EnergyMinimizer::Option::MAXIMUM_DISPLACEMENT, 
			 EnergyMinimizer::Default::MAXIMUM_DISPLACEMENT);

    energy_update_counter_ = 0; 
    force_update_counter_ = 0; 

		// minimizer specific parts
		valid_ = specificSetup();
		if (!valid_) 
		{
			Log.error() << "EnergyMinimizer::specificSetup failed!" << endl;
		}

		return valid_;
	}

  // Setup with a force field and a snapshot manager 
  bool EnergyMinimizer::setup
		(ForceField& force_field, SnapShotManager* ssm)
	{
    bool result = setup(force_field);

    // set a pointer to the indicated snapshot manager
    if (ssm->isValid())
		{
      snapshot_ = ssm; 
		}

    return result; 
	}

  // Setup with a force field and a snapshot manager and options 
  bool EnergyMinimizer::setup
		(ForceField& force_field, SnapShotManager* ssm, const Options& new_options)
	{
    bool result = setup(force_field,new_options);

    // set a pointer to the indicated snapshot manager
    if(ssm->isValid())
		{
			snapshot_ = ssm; 
		}

    return result; 
	}

  // Setup with a force field and a set of options
  bool EnergyMinimizer::setup(ForceField& force_field, const Options& new_options)
	{
		options = new_options;
		valid_ = setup(force_field);
		return valid_;
	}

	// virtual function for the specific setup of derived classes
	bool EnergyMinimizer::specificSetup()
	{
		return true;
	}

	//	The minimizer optimizes the energy of the system bound to the force field.
	//	The function is virtual.
	//
	bool EnergyMinimizer::minimize(Size /* steps */, bool /* restart */)
	{
		throw Exception::NotImplemented(__FILE__, __LINE__);
	}

	// udpate the search direction
	void EnergyMinimizer::updateDirection()
	{
		throw Exception::NotImplemented(__FILE__, __LINE__);
	}

	// determine the new step along direction_
	double EnergyMinimizer::findStep()
	{
		throw Exception::NotImplemented(__FILE__, __LINE__);
	}

	// calculate a new energy
	double EnergyMinimizer::updateEnergy()
	{
		if (force_field_ != 0)
		{
			// recalculate the energy and ...
			current_energy_ = force_field_->updateEnergy();

			// ...increase the update counter
			energy_update_counter_++;
		}
		#ifdef BALL_DEBUG
			Log.info() << "EnergyMinimizer: new energy E=" << current_energy_ << std::endl;	
		#endif

		// return the current energy
		return current_energy_;
	}

	// calculate new forces
	void EnergyMinimizer::updateForces()
	{
		if (force_field_ != 0)
		{
			// recalculate the forces and the energy and ...
			force_field_->updateForces();

			// assign the current gradient
			current_grad_.set(force_field_->getAtoms());

			// ...increase the update counter
			force_update_counter_++;

			#ifdef BALL_DEBUG
				Log.info() << "EnergyMinimizer: new forces RMS = " << current_grad_.rms << std::endl;	
			#endif
		}
	}

	bool EnergyMinimizer::isConverged() const
	{
		bool converged = ((current_grad_.rms <= max_gradient_)
											|| (same_energy_counter_ >= max_same_energy_)
											|| (fabs(old_energy_ - initial_energy_) < energy_difference_bound_));
		return converged;
	}

	void EnergyMinimizer::printEnergy() const
	{
		if (isValid())
		{
			Log.info() << "iteration " << number_of_iterations_
								 << "  RMS gradient " << current_grad_.rms
								 << " kJ/(mol A)      total energy " << force_field_->getEnergy() << " kJ/mol"
								 << std::endl;
		}
		//if (force_field_ != 0)
		//{
		//	Log.info() << "  components:" << endl;
		//	for (Position i = 0; i < force_field_->countComponents(); i++)
		//	{
		//		Log.info() << "     " << force_field_->getComponent(i)->getName() << ": " 
		//							 << force_field_->getComponent(i)->getEnergy() << " kJ/mol" << endl;
		//	}
		//}
	}

	void EnergyMinimizer::takeSnapShot() const
	{
		// if a snapshot manager is defined, use it!
		if (snapshot_ != 0)
		{
			snapshot_->takeSnapShot();
		}
	}

	void EnergyMinimizer::finishIteration()
	{
		// perform a force field update in regular intervals
		// (to update the pair list)
		if (number_of_iterations_ % force_field_->getUpdateFrequency() == 0)
		{
			force_field_->update();
		}

		// take a snapshot of the system every snapshot_frequency_ iterations
		if ((snapshot_ != 0) && (number_of_iterations_ % snapshot_frequency_ == 0))
		{
			takeSnapShot();
		}

		// print the energy every energy_output_frequency_ iterations
		if (number_of_iterations_ % energy_output_frequency_ == 0)
		{
			printEnergy();
		}

		// check whether there the new energy and the old energy differ
		// significantly
		if (fabs(initial_energy_ - old_energy_) < energy_difference_bound_)
		{
			// count if there is the same energy between last iteration and
			// this iteration
			same_energy_counter_++;
		}
		else
		{
			same_energy_counter_ = 0;
		}

		// increment the iteration counter
		number_of_iterations_++;
	}

	bool EnergyMinimizer::operator == (const EnergyMinimizer& energy_minimizer)
		throw()
	{
		return ((force_field_ == energy_minimizer.force_field_)
			&& (options == energy_minimizer.options)
			&& (valid_ == energy_minimizer.valid_)
			&& (number_of_iterations_ == energy_minimizer.number_of_iterations_)
			&& (maximal_number_of_iterations_ == energy_minimizer.maximal_number_of_iterations_ )
			&& (energy_output_frequency_ == energy_minimizer.energy_output_frequency_)
			&& (snapshot_frequency_ == energy_minimizer.snapshot_frequency_)
      && (max_same_energy_ == energy_minimizer.max_same_energy_) 
			&& (energy_difference_bound_ == energy_minimizer.energy_difference_bound_ )
			&& (max_gradient_ == energy_minimizer.max_gradient_ )
			&& (maximum_displacement_ == energy_minimizer.maximum_displacement_)
      && (force_update_counter_ == energy_minimizer.force_update_counter_)
			&& (energy_update_counter_ == energy_minimizer.energy_update_counter_));
	}

} // namespace Ball
