// $Id: energyMinimizer.C,v 1.1 1999/08/26 08:02:45 oliver Exp $

#include <BALL/MOLMEC/MINIMIZATION/energyMinimizer.h>
#include <BALL/COMMON/limits.h>

namespace BALL {

	const char* EnergyMinimizer::Option::MAXIMAL_NUMBER_OF_ITERATIONS = "maximal_number_of_iterations";
	const char* EnergyMinimizer::Option::ENERGY_OUTPUT_FREQUENCY = "energy_output_frequency";
	const char* EnergyMinimizer::Option::SNAPSHOT_OUTPUT_FREQUENCY = "snapshot_output_frequency";
	const char* EnergyMinimizer::Option::NUMBER_OF_ITERATION = "number_of_iteration";
	const char* EnergyMinimizer::Option::ENERGY_DIFFERENCE_BOUND = "energy_difference_bound";
	const char* EnergyMinimizer::Option::MAXIMAL_SHIFT = "MAXIMAL_SHIFT";

	Size EnergyMinimizer::Default::MAXIMAL_NUMBER_OF_ITERATIONS = 1000;
	Size EnergyMinimizer::Default::ENERGY_OUTPUT_FREQUENCY = 50;
	Size EnergyMinimizer::Default::SNAPSHOT_OUTPUT_FREQUENCY = Limits<Size>::max();
	Size EnergyMinimizer::Default::NUMBER_OF_ITERATION = 0;
	float EnergyMinimizer::Default::ENERGY_DIFFERENCE_BOUND = 0.01;  // kJ/mol
	float EnergyMinimizer::Default::MAXIMAL_SHIFT = 0.4; // Angstrom

	// default constructor
	EnergyMinimizer::EnergyMinimizer()
		:	valid_(false),
			force_field_(0)
	{
	}


	// copy constructor 
	EnergyMinimizer::EnergyMinimizer(const EnergyMinimizer& energy_minimizer, bool /* deep */)
	{
		// Copy the attributes
		force_field_										= energy_minimizer.force_field_;
		options													= energy_minimizer.options;
		valid_													= energy_minimizer.valid_;
// BAUSTELLE
//		snapshot_												= energy_minimizer.snapshot_;
		number_of_iteration_						= energy_minimizer.number_of_iteration_;
		maximal_number_of_iterations_		= energy_minimizer.maximal_number_of_iterations_ ;
		energy_output_frequency_				= energy_minimizer.energy_output_frequency_;
		snapshot_output_frequency_			= energy_minimizer.snapshot_output_frequency_;
		energy_difference_bound_				= energy_minimizer.energy_difference_bound_ ;
		maximal_shift_                  = energy_minimizer.maximal_shift_;
	}

	// assignment operator
	EnergyMinimizer& EnergyMinimizer::operator = (const EnergyMinimizer& energy_minimizer)
	{

		// guard against self assignment
		if (&energy_minimizer != this) 
		{ 	
			// Copy the attributes
			force_field_									= energy_minimizer.force_field_;
			options												= energy_minimizer.options;
			valid_												= energy_minimizer.valid_;
// BAUSTELLE
//			snapshot_											= energy_minimizer.snapshot_;
			number_of_iteration_						= energy_minimizer.number_of_iteration_;
			maximal_number_of_iterations_		= energy_minimizer.maximal_number_of_iterations_ ;
			energy_output_frequency_				= energy_minimizer.energy_output_frequency_;
			snapshot_output_frequency_			= energy_minimizer.snapshot_output_frequency_;
			energy_difference_bound_				= energy_minimizer.energy_difference_bound_ ;
			maximal_shift_                  = energy_minimizer.maximal_shift_;
	
		}
		return (*this);
	}

	// Constructor initialized with a force field
	EnergyMinimizer::EnergyMinimizer(ForceField& force_field)
	{
		valid_ = setup(force_field);

		if (!valid_) 
		{
			Log.level(LogStream::ERROR) << " Energy minimizer setup failed! " << endl;
		}
	}
	
	// Constructor initialized with a force field and a set of options
	EnergyMinimizer::EnergyMinimizer(ForceField& force_field, const Options& new_options)
	{
		valid_ = setup(force_field, new_options);

		if (!valid_) 
		{
			Log.level(LogStream::ERROR) << " Energy minimizer setup failed! " << endl;
		}
	}

	// Destructor
	EnergyMinimizer::~EnergyMinimizer()
	{
	}
		
	// Set the number of the current iteration
	void	EnergyMinimizer::setNumberOfIteration(Size number_of_iteration)
	{
		number_of_iteration_ = number_of_iteration;
	}

	// Get the number of the current iteration
	Size	EnergyMinimizer::getNumberOfIteration() const
	{
		return number_of_iteration_;
	}

	// Set the maximal number of iterations
	void	EnergyMinimizer::setMaximalNumberOfIterations(Size maximal_number_of_iterations)
	{
		maximal_number_of_iterations_ = maximal_number_of_iterations;
	}

	// Get the maximal number of iterations
	Size	EnergyMinimizer::getMaximalNumberOfIterations() const
	{
		return maximal_number_of_iterations_;
	}

	// Is the energy minimizer valid: did the setup work?
	bool	EnergyMinimizer::isValid() const
	{
		return valid_;
	}
/*
	// Return the trajectory of the minimization procedure
	Snapshot& EnergyMinimizer::getSnapshot() const
	{
		return snapshot_;
	}
	// Set the trajectory
	void	EnergyMinimizer::setSnapshot(Snapshot& snapshot)
	{
		snapshot_ = snapshot;
	}
*/
	//	Set the energy output frequency
	void    EnergyMinimizer::setEnergyOutputFrequency(Size   energy_output_frequency)
	{
		energy_output_frequency_ = energy_output_frequency;
	}

	//	Get the energy ouput frequency
	Size    EnergyMinimizer::getEnergyOutputFrequency() const
	{
		return	energy_output_frequency_;
	}

	// Set the energy difference bound
	void	EnergyMinimizer::setEnergyDifferenceBound(float energy_difference_bound)
	{
		energy_difference_bound_ = energy_difference_bound;
	}

	// Get the energy difference bound
	float	EnergyMinimizer::getEnergyDifferenceBound() const
	{
		return energy_difference_bound_;
	}

	//	Set the maximal shift
	void	EnergyMinimizer::setMaximalShift( float maximal_shift )
	{
		maximal_shift_ = maximal_shift;
	}

	//	Get the maximal shift
	float	EnergyMinimizer::getMaximalShift() const
	{
		return maximal_shift_;
	}

	//	Set the trajectory ouput frequency
	void    EnergyMinimizer::setSnapshotOutputFrequency(Size snapshot_output_frequency)
	{
		snapshot_output_frequency_ = snapshot_output_frequency;
	}

	//	Get the trajectory ouput frequency
	Size    EnergyMinimizer::getSnapshotOutputFrequency() const
	{
		return	snapshot_output_frequency_;
	}

	// Get the force field of the energy minimizer
	ForceField*	EnergyMinimizer::getForceField()
	{
		return force_field_;
	}

	// setup methods
	bool  EnergyMinimizer::setup(ForceField& force_field)
	{
		// store the specified force field
		force_field_ = &force_field; 

		valid_ = force_field_->isValid();
		if (!valid_)
		{
			Log.error() << "The force field of the energy minimizer is not valid! " <<
							"Check the definition and initialization of the force field! " << endl;
			return valid_;
		}


		// Check options

		options.setDefaultInteger(EnergyMinimizer::Option::MAXIMAL_NUMBER_OF_ITERATIONS, (long)EnergyMinimizer::Default::MAXIMAL_NUMBER_OF_ITERATIONS);
		maximal_number_of_iterations_ = (Size)(options.getInteger(EnergyMinimizer::Option::MAXIMAL_NUMBER_OF_ITERATIONS));

		options.setDefaultInteger(EnergyMinimizer::Option::ENERGY_OUTPUT_FREQUENCY, (long)EnergyMinimizer::Default::ENERGY_OUTPUT_FREQUENCY);
		energy_output_frequency_ = (Size)(options.getInteger(EnergyMinimizer::Option::ENERGY_OUTPUT_FREQUENCY));

		options.setDefaultInteger(EnergyMinimizer::Option::SNAPSHOT_OUTPUT_FREQUENCY, (long)EnergyMinimizer::Default::SNAPSHOT_OUTPUT_FREQUENCY);
		snapshot_output_frequency_ = (Size)(options.getInteger(EnergyMinimizer::Option::SNAPSHOT_OUTPUT_FREQUENCY));

		options.setDefaultInteger(EnergyMinimizer::Option::NUMBER_OF_ITERATION, (long)EnergyMinimizer::Default::NUMBER_OF_ITERATION);
		number_of_iteration_ = (Size)(options.getInteger(EnergyMinimizer::Option::NUMBER_OF_ITERATION));

		options.setDefaultReal(EnergyMinimizer::Option::ENERGY_DIFFERENCE_BOUND, EnergyMinimizer::Default::ENERGY_DIFFERENCE_BOUND);
		energy_difference_bound_ = options.getReal(EnergyMinimizer::Option::ENERGY_DIFFERENCE_BOUND);

		options.setDefaultReal(EnergyMinimizer::Option::MAXIMAL_SHIFT, EnergyMinimizer::Default::MAXIMAL_SHIFT);
		maximal_shift_ = options.getReal(EnergyMinimizer::Option::MAXIMAL_SHIFT);

		// minimizer specific parts
		valid_ = specificSetup();
		if (!valid_) 
		{
			Log.level(LogStream::ERROR) << "Energy minimizer specificSetup failed!" << endl;
			return valid_;
		}

		valid_ = true;
		return valid_;
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

	/*	The minimizer optimizes the energy of the system bound to the force field.
			The function is virtual.
	*/

	bool EnergyMinimizer::minimize(Size /* steps */, bool /* restart */)
	{
		return true;
	}
	

} // namespace Ball
