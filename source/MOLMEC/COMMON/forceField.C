// $Id: forceField.C,v 1.13 2000/02/06 20:08:27 oliver Exp $

#include <BALL/MOLMEC/COMMON/forceField.h>

#include <BALL/MOLMEC/COMMON/forceFieldComponent.h>
#include <BALL/MOLMEC/COMMON/periodicBoundary.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/forEach.h>

using namespace std;

namespace BALL 
{

	// default constructor
	ForceField::ForceField()
		:	periodic_boundary(*this),
			system_( 0 ),
			valid_(true),
			name_("Force Field"),
			number_of_movable_atoms_(0),
			use_selection_(false)
	{
	}


	// copy constructor 
	ForceField::ForceField(const ForceField& force_field, bool /* deep */)
	{
		// Copy the attributes
		name_   = force_field.name_;
		energy_ = force_field.energy_;
		options = force_field.options;
		system_ = force_field.system_;
		atoms_  = force_field.atoms_;
		number_of_movable_atoms_ = force_field.number_of_movable_atoms_;
		parameters_ = force_field.parameters_;
		periodic_boundary = force_field.periodic_boundary;
		use_selection_ = force_field.use_selection_;
		valid_ = force_field.valid_;

		
		// Copy the component vector
		for (Size i = 0; i < force_field.components_.size(); i++) 
		{
			components_.push_back((ForceFieldComponent*)force_field.components_[i]->create());
		}
	}

	// assignment operator
	ForceField& ForceField::operator = (const ForceField& force_field)
	{

		// guard against self assignment
		if (&force_field != this) 
		{ 	
			atoms_.clear();
			atoms_  = force_field.atoms_;
			number_of_movable_atoms_ = force_field.number_of_movable_atoms_;

			name_   = force_field.name_;
			energy_ = force_field.energy_;
			options = force_field.options;
			system_ = force_field.system_;
			parameters_ = force_field.parameters_;
			periodic_boundary = force_field.periodic_boundary;
			use_selection_ = force_field.use_selection_;
			valid_ = force_field.valid_;

			Size i;
			for (i = 0; i < components_.size(); i++) 
			{
				delete components_[i];
			}
			components_.clear();

			for (i = 0; i < force_field.components_.size(); i++) 
			{
				components_.push_back((ForceFieldComponent*)force_field.components_[i]->create());
			}
	
		}
		return (*this);
	}

	// Constructor initialized with a system
	ForceField::ForceField(System& system)
		:	periodic_boundary(*this)
	{
		bool result = setup(system);

		if (!result) 
		{
			Log.level(LogStream::ERROR) << "Force Field setup failed! " << endl;
			valid_ = false;
		}
	}
	
	// Constructor intialized with a system and a set of options
	ForceField::ForceField(System& system, const Options& new_options)
		:	periodic_boundary(*this)
	{
		bool result = setup(system, new_options);

		if (!result) 
		{
			Log.level(LogStream::ERROR) << "Force Field setup failed! " << endl;
			valid_ = false;
		}
	}
	

	// destructor
	ForceField::~ForceField()
	{
		// clear pointers 

		system_ = 0;

		// remove all force field components from the list after
		// calling their destructors
		vector<ForceFieldComponent*>::iterator  it;
		for (it = components_.begin(); it != components_.end(); ++it)
		{
			delete (*it);
		}
		components_.clear();
	}
		
	// Is the force field valid
	bool  ForceField::isValid()
	{
		return valid_;
	}

	// setup methods
	bool  ForceField::setup(System& system)
	{
		bool  success = true;

		// store the specified system
		system_ = &system;

		// Setup periodic boundary
		if(!periodic_boundary.setup()) 
		{
			Log.error() << "setup of periodic boundary failed" << endl;
			return false;
		}
			
		// collect the atoms of the system in the atoms_ vector
		collectAtoms_(system);
		Size old_size = atoms_.size();

		// force field specific parts
		success = specificSetup();
		if (!success) 
		{
			Log.error() << "Force Field specificSetup failed!" << endl;
			return false;
		}

		// if specificSetup cleared this array, it wants to tell us 
		// that it had to change the system a bit (e.g. CHARMM replacing
	  // hydrogens by united atoms). So, we have to recalculated the vector.
		if (atoms_.size() != old_size)
		{
			collectAtoms_(system);
		}
		// call the setup method for each force field component
		vector<ForceFieldComponent*>::iterator  it;
		for (it = components_.begin(); (it != components_.end()) && success; ++it)
		{
			success = (*(*it)).setup();
			if (!success) 
			{
				Log.error() << "Force Field Component setup of " 
										<< (*it)->name_ <<  " failed!" << endl;
			}
		}


		// if the setup failed, our force field becomes invalid!
		valid_ = success;
		return success;
	}


	// collect all atoms
	void ForceField::collectAtoms_(const System& system)
	{
		// clear existing atoms entries
		atoms_.clear();


		// check for selected atoms
		// if any of the atoms is selected, the atoms_ array holds
		// the selected atoms first (0 < i < number_of_movable_atoms_) 
		use_selection_ = system.containsSelection();
		number_of_movable_atoms_ = 0;
		AtomIterator atom_it = system.beginAtom();
		if (use_selection_ == true)
		{
			for (; +atom_it; ++atom_it)
			{
				if (atom_it->isSelected() == true)
				{
					atoms_.push_back(&(*atom_it));
				}
			}
		} else {
			for (; +atom_it; ++atom_it)
			{
				atoms_.push_back(&(*atom_it));
			}
		}


		number_of_movable_atoms_ = atoms_.size();
		// generate the vector of molecules if periodic boundary is enabled
		if (periodic_boundary.isEnabled())
		{
			periodic_boundary.generateMoleculesVector();
		}

		// force field specific parts
		success = specificSetup();
		if (!success) 
		{
			Log.error() << "Force Field specificSetup failed!" << endl;
			return false;
		}

		// call the setup method for each force field component
		vector<ForceFieldComponent*>::iterator  it;
		for (it = components_.begin(); (it != components_.end()) && success; ++it)
		{
			success = (*(*it)).setup();
			if (!success) 
			{
				Log.error() << "Force Field Component setup of " 
					<< (*it)->name_ <<  " failed!" << endl;
			}
		}

		return success;
	}

	// Setup with a system and a set of options
	bool ForceField::setup(System& system, const Options& new_options)
	{
		options = new_options;
		return setup(system);
	}


	// Returns the name of the force field
	String ForceField::getName() const
	{
		return name_;
	}

	// Sets the name of force field to name
	void ForceField::setName(const String& name)
	{
		name_ = name;
	}


	// Returns the number of atoms stored in the vector atoms_
	Size ForceField::getNumberOfAtoms() const
	{
		return atoms_.size();
	}

	// Returns the number of movable (non-fixed) atoms stored in the vector atoms_
	Size ForceField::getNumberOfMovableAtoms() const
	{
		return number_of_movable_atoms_;
	}

	// Returns the reference of the atom vector atoms_
	const vector<Atom*>& ForceField::getAtoms() const 
	{
		return atoms_;
	}

	// Return a pointer to the system
	System* ForceField::getSystem()
	{
		return system_;
	}

	// Return the parameter use_selection_
	bool ForceField::getUseSelection()
	{
		return use_selection_;
	}

	// Return the parameter use_selection_
	void ForceField::setUseSelection(bool use_selection)
	{
		use_selection_ = use_selection;
	}

	// Return a pointer to the parameter file
	ForceFieldParameters& ForceField::getParameters()
	{
		return parameters_;
	}

	void ForceField::updateForces()
	{
		// check for validity of the force field
		if (!isValid())
		{
			return;
		}

		// Set forces to zero
		for (vector<Atom*>::iterator it = atoms_.begin(); it != atoms_.end(); ++it) 
		{
			(*it)->setForce(RTTI::getDefault<Vector3>());
		}

		// call each component - they will add their forces...
		vector<ForceFieldComponent*>::iterator		component_it = components_.begin();
		for (; component_it != components_.end(); ++component_it)
		{
			(*component_it)->updateForces();
		}
	}

	// Calculate the RMS of the gradient
	float	ForceField::getRMSGradient() const
	{
		float sum = 0;
		vector<Atom*>::const_iterator it = atoms_.begin();
		for (; it != atoms_.end(); ++it)
		{
			sum += (*it)->getForce().getSquareLength();
		}
		sum = sqrt(sum/(3 * (float)atoms_.size()));
		sum *= Constants::AVOGADRO / 1e13;
		return(sum);
	}

	float ForceField::getEnergy() const 
	{
	 return energy_;
	}

	float ForceField::updateEnergy()
	{
		// check for validity of the force field
		if (!isValid())
		{
			return 0.0;
		}

		// clear the total energy
		energy_ = 0;

		// call each component and add their energies
		vector<ForceFieldComponent*>::iterator		it;
		for (it = components_.begin(); it != components_.end(); ++it)
		{
			energy_ += (*it)->updateEnergy();
		}

		// return the resulting energy
		return energy_;
		
	}

	
	Size ForceField::getUpdateFrequency() const
	{
		return 1;
	}

	void ForceField::update()
	{
	}
	

	bool ForceField::specificSetup() 
	{
		return true;
	}

	// Insert a component in the component vector
	void ForceField::insertComponent(ForceFieldComponent* force_field_component)
	{
		components_.push_back(force_field_component);
	}

	// Remove the component 
	void ForceField::removeComponent(const ForceFieldComponent* force_field_component)
	{
		// call each component - test if equal - remove
		vector<ForceFieldComponent*>::iterator		it;
		for (it = components_.begin(); it != components_.end(); ++it)
		{
			if ((*it) == force_field_component) 
			{
				components_.erase(it);
				break ;
			}
		}
	}

	// Remove the component 
	void ForceField::removeComponent(const String& name)
	{
		// call each component - test if equal - remove
		vector<ForceFieldComponent*>::iterator		it;
		for (it = components_.begin(); it != components_.end(); ++it)
		{
			if ((*it)->getName() == name) 
			{
				components_.erase(it);
				break ;
			}
		}		
	}

	// Returns the number of components
	Size ForceField::countComponents() const
	{
		return components_.size();
	}

	// Returns the component with index "index"
	ForceFieldComponent* ForceField::getComponent(const Size index) const
	{
		if (index >= components_.size())
			return 0;

		return components_[index];
	}

	ForceFieldComponent* ForceField::getComponent(const String& name) const
	{
		for (Size i = 0; i < components_.size(); ++i)
		{
			if (components_[i]->getName() == name)
			{
				return components_[i];
			}
		}

		return 0;
	}

	

} // namespace BALL
