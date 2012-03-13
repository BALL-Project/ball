// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: forceField.C,v 1.40.26.4 2007/05/18 12:06:00 oliver Exp $
//

#include <BALL/MOLMEC/COMMON/forceField.h>

#include <BALL/MOLMEC/COMMON/forceFieldComponent.h>
#include <BALL/MOLMEC/COMMON/periodicBoundary.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/forEach.h>

#include <limits>

using namespace std;

namespace BALL 
{

	// default constructor
	ForceField::ForceField()
		:	options(),
			periodic_boundary(*this),
			system_(0),
			atoms_(),
			parameters_(),
			valid_(false),
			name_("Force Field"),
			energy_(0.0),
			number_of_movable_atoms_(0),
			use_selection_(false),
			selection_enabled_(true),
			update_time_stamp_(),
			setup_time_stamp_(),
			unassigned_atoms_(),
			max_number_of_errors_(std::numeric_limits<Size>::max()),
			number_of_errors_(0)
	{
	}

	void ForceField::clear()
	{
		options.clear();
		periodic_boundary.clear();

		name_ = "Force Field";
		energy_ = 0.0;
		system_ = 0;
		atoms_.clear();
		number_of_movable_atoms_ = 0;
		parameters_.clear();
		use_selection_ = false;
		selection_enabled_ = true;
		valid_ = true;
		
		update_time_stamp_.clear();
		setup_time_stamp_.clear();

		// remove all force field components from the list after
		// calling their destructors
		vector<ForceFieldComponent*>::iterator  it;
		for (it = components_.begin(); it != components_.end(); ++it)
		{
			delete (*it);
		}
		components_.clear();

		unassigned_atoms_.clear();
		max_number_of_errors_= std::numeric_limits<Size>::max();
		number_of_errors_ = 0;
	}

	// copy constructor 
	ForceField::ForceField(const ForceField& force_field)
		:	options(force_field.options),
			periodic_boundary(force_field.periodic_boundary),
			system_(force_field.system_),
			atoms_(force_field.atoms_),
			parameters_(force_field.parameters_),
			valid_(force_field.valid_),
			name_(force_field.name_),
			energy_(force_field.energy_),
			number_of_movable_atoms_(force_field.number_of_movable_atoms_),
			use_selection_(force_field.use_selection_),
			selection_enabled_(force_field.selection_enabled_),
			update_time_stamp_(force_field.update_time_stamp_),
			setup_time_stamp_(force_field.setup_time_stamp_),
			max_number_of_errors_(force_field.max_number_of_errors_),
			number_of_errors_(0)
	{
		// Copy the component vector and its components.
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
			selection_enabled_ = force_field.selection_enabled_;
			valid_ = force_field.valid_;
			max_number_of_errors_= force_field.max_number_of_errors_;
			number_of_errors_ = 0;

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
		:	options(),
			periodic_boundary(*this),
			system_(0),
			atoms_(),
			parameters_(),
			valid_(false),
			name_("Force Field"),
			energy_(0.0),
			number_of_movable_atoms_(0),
			use_selection_(false),
			selection_enabled_(true),
			update_time_stamp_(),
			setup_time_stamp_(),
			unassigned_atoms_(),
			max_number_of_errors_(std::numeric_limits<Size>::max()),
			number_of_errors_(0)
	{
		bool result = setup(system);

		if (!result) 
		{
			Log.error() << "Force Field setup failed! " << endl;
			valid_ = false;
		}
	}
	
	// Constructor intialized with a system and a set of options
	ForceField::ForceField(System& system, const Options& new_options)
		:	options(),
			periodic_boundary(*this),
			system_(0),
			atoms_(),
			parameters_(),
			valid_(false),
			name_("Force Field"),
			energy_(0.0),
			number_of_movable_atoms_(0),
			use_selection_(false),
			selection_enabled_(true),
			update_time_stamp_(),
			setup_time_stamp_(),
			unassigned_atoms_(),
			max_number_of_errors_(std::numeric_limits<Size>::max()),
			number_of_errors_(0)
	{
		bool result = setup(system, new_options);

		if (!result) 
		{
			Log.error() << "Force Field setup failed! " << endl;
			valid_ = false;
		}
	}
	

	// destructor
	ForceField::~ForceField()
	{
		clear();
		valid_ = false;
	}
		
	// Is the force field valid
	bool ForceField::isValid() const
		
	{
		return valid_;
	}

	// setup methods
	bool ForceField::setup(System& system)
	{
		unassigned_atoms_.clear();
		number_of_errors_ = 0;

		// store the specified system
		system_ = &system;

		// Setup periodic boundary
		if (!periodic_boundary.setup()) 
		{
			Log.error() << "setup of periodic boundary failed" << endl;
			return false;
		}
			

		// Update the use_selection_ flag.
		use_selection_ = (selection_enabled_ && system_->containsSelection());

		// collect the atoms of the system in the atoms_vector_
		collectAtoms_(system);
		Size old_size = (Size)atoms_.size();

		// generate the vector of molecules if periodic boundary is enabled
		if (periodic_boundary.isEnabled())
		{
			periodic_boundary.generateMoleculesVector();
		}

		// force field specific parts
		bool success = false;
		try
		{
			success = specificSetup();
		}
		catch(...)
		{
		}

		if (!success) 
		{
			Log.error() << "Force Field specificSetup failed!" << endl;
			return false;
		}

		// If specificSetup cleared this array, it wants to tell us 
		// that it had to change the system a bit (e.g. CHARMM replacing
		// hydrogens by united atoms). So, we have to recalculated the vector.
		if (atoms_.size() != old_size)
		{
			collectAtoms_(system);
		}

		// Call the setup method for each force field component.
		vector<ForceFieldComponent*>::iterator  it;
		for (it = components_.begin(); (it != components_.end()) && success; ++it)
		{
			success = false;
			try
			{
				success = (*(*it)).setup();
			}
			catch(...)
			{
			}

			if (!success) 
			{
				Log.error() << "Force Field Component setup of " << (*it)->name_ <<  " failed!" << endl;
			}
		}

		// ?????
		update();

		// Remember the setup time
		setup_time_stamp_.stamp();

		// If the setup failed, our force field becomes invalid!
		valid_ = success;
		return success;
	}


	// collect all atoms
	void ForceField::collectAtoms_(const System& system)
	{
		// Clear existing atoms entries.
		atoms_.clear();

		// Check for selected atoms.
		// Consider selection only if it has not been disabled
		// and there are selected atoms in the system.
		// If any of the atoms are selected, the atoms_ array holds
		// the selected atoms first (0 < i < number_of_movable_atoms_) 
		number_of_movable_atoms_ = 0;
		AtomConstIterator atom_it;

		if (getUseSelection())
		{
			// We store the selected atoms only!
			// All other atoms will not be considered in the
			// calculation -- they become invisible to the force field.
			AtomConstIterator atom_it = system.beginAtom();
			for (; +atom_it; ++atom_it)
			{
				if (atom_it->isSelected())
				{
					atoms_.push_back(const_cast<Atom*>(&(*atom_it)));
				}
			}
			number_of_movable_atoms_ = (Size)atoms_.size();
		}
		else
		{
			// We store ALL atoms in the atom vector -- selection
			// has been disabled for this purpose.
			for (atom_it = system.beginAtom(); +atom_it; ++atom_it)
			{
				atoms_.push_back(const_cast<Atom*>(&(*atom_it)));
			}
			// Make sure the selected atoms are in the front
			sortSelectedAtomVector_();
		}
	}

	void ForceField::sortSelectedAtomVector_()
	{
		if (system_->containsSelection() && (atoms_.size() > 1))
		{
			// Exchange sort.
			Position first = 0;
			Position last = (Position)(atoms_.size() - 1);
			while (last >= first)
			{	
				while ((first < atoms_.size()) && atoms_[first]->isSelected())
				{
					first++;
				}
				while ((last > 0) && !atoms_[last]->isSelected())
				{
					last--;
				}
				if ((last > 0) && (first < atoms_.size()) && (first < last))
				{
					Atom* tmp = atoms_[first];
					atoms_[first] = atoms_[last];
					atoms_[last] = tmp;
				}
			}
			number_of_movable_atoms_ = first;
		}
		else
		{
			number_of_movable_atoms_ = (Size)atoms_.size();
		}
	}

	// Setup with a system and a set of options
	bool ForceField::setup(System& system, const Options& new_options)
	{
		options = new_options;
		return setup(system);
	}

  ForceFieldParameters& ForceField::getParameters()
	{
		return parameters_;
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

	const TimeStamp& ForceField::getUpdateTime() const
		
	{
		return update_time_stamp_;
	}

	const TimeStamp& ForceField::getSetupTime() const
		
	{
		return setup_time_stamp_;
	}

	// Returns the number of atoms stored in the vector atoms_
	Size ForceField::getNumberOfAtoms() const
	{
		// if the force field is invalid, there's not much we can do
		if (!isValid())
		{
			return 0;
		}

		return (Size)atoms_.size();
	}

	// Returns the number of movable (non-fixed) atoms stored in the vector atoms_
	Size ForceField::getNumberOfMovableAtoms() const
	{
		// if the force field is invalid, there's not much we can do
		if (!isValid())
		{
			return 0;
		}

		// check whether the selection changed since the last call
		// to update and call update otherwise
		if (update_time_stamp_.isOlderThan(system_->getSelectionTime()))
		{
			// Call update if the selection time stamp changed since
			// the last call to update. This ensures consistency of
			// the selection information in pair lists, bond lists, etc.
			// Also make sure the movable atoms are still in the front
			// of the atom vevtor.
			// we have to do a bit of nasty casting to hide this, but that's OK here --
			// it still is *kind of* const :-)
			const_cast<ForceField*>(this)->sortSelectedAtomVector_();
			const_cast<ForceField*>(this)->update();

			// Update the use_selection_ flag.
			// this const_cast is _bad_
			bool* use_ptr = const_cast<bool*>(&use_selection_);
			*use_ptr = (selection_enabled_ && system_->containsSelection());
		}
		
		return number_of_movable_atoms_;
	}

	void ForceField::updateForces()
	{
		// check for validity of the force field
		if (!isValid())
		{
			return;
		}

		// Set forces to zero
		for (AtomVector::Iterator it = atoms_.begin(); it != atoms_.end(); ++it) 
		{
			(*it)->setForce(RTTI::getDefault<Vector3>());
		}

		performRequiredUpdates_();

		// call each component - they will add their forces...
		vector<ForceFieldComponent*>::iterator		component_it = components_.begin();
		for (; component_it != components_.end(); ++component_it)
		{
			if ((**component_it).isEnabled())
			{
				(*component_it)->updateForces();
			}
		}
	}

	// Calculate the RMS of the gradient
	double	ForceField::getRMSGradient() const
	{
		double sum = 0;
		AtomVector::ConstIterator it(atoms_.begin());
		for (; it != atoms_.end(); ++it)
		{
			sum += (*it)->getForce().getSquareLength();
		}
		sum = sqrt(sum/(3 * (double)atoms_.size()));
		sum *= Constants::AVOGADRO / 1e13;
		return(sum);
	}

	double ForceField::getEnergy() const 
	{
	 return energy_;
	}

	double ForceField::updateEnergy()
	{
		// check for validity of the force field
		if (!isValid())
		{
			return 0.0;
		}

		// clear the total energy
		energy_ = 0;

		performRequiredUpdates_();

		// call each component and add their energies
		vector<ForceFieldComponent*>::iterator		it;
		for (it = components_.begin(); it != components_.end(); ++it)
		{
			if (!(**it).isEnabled()) continue;

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
		throw(Exception::TooManyErrors)
	{
		// check for validity of the force field
		if (!valid_)
		{
			return;
		}

		// iterate over all components and 
		// call their update methods
		vector<ForceFieldComponent*>::iterator it;
		for (it = components_.begin(); it != components_.end(); ++it)
		{
			if ((**it).isEnabled())
			{
				(*it)->update();
			}
		}

		// remember the time of the last update
		update_time_stamp_.stamp();
	}
	

	bool ForceField::specificSetup() 
		throw(Exception::TooManyErrors)
	{
		return true;
	}

	// Insert a component in the component vector
	void ForceField::insertComponent(ForceFieldComponent* force_field_component)
	{
		components_.push_back(force_field_component);
		force_field_component->setForceField(*this);
	}

	// Remove the component 
	void ForceField::removeComponent(const ForceFieldComponent* force_field_component)
	{
		// call each component - test if equal - remove
		vector<ForceFieldComponent*>::iterator it;
		for (it = components_.begin(); it != components_.end(); ++it)
		{
			if ((*it) == force_field_component) 
			{
				// Destruct the component -- we are responsible for the mem management.
				delete *it;

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
				// Destruct the component -- we are responsible for the mem management.
				delete *it;

				components_.erase(it);
				break ;
			}
		}		
	}

	// Returns the number of components
	Size ForceField::countComponents() const
	{
		return (Size)components_.size();
	}

	// Returns the component with index "index"
	ForceFieldComponent* ForceField::getComponent(const Size index) const
	{
		if (index >= components_.size())
		{
			return 0;
		}

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

	void ForceField::setMaximumNumberOfErrors(Size nr)
	{
		max_number_of_errors_= nr;
	}
	
	Size ForceField::getMaximumNumberOfErrors() const
	{
		return max_number_of_errors_;
	}

	HashSet<const Atom*>& ForceField::getUnassignedAtoms()
	{
		return unassigned_atoms_;
	}

	std::ostream& ForceField::error() throw(Exception::TooManyErrors)
	{
		number_of_errors_++;
		if (number_of_errors_ > max_number_of_errors_)
		{
			throw Exception::TooManyErrors(__FILE__, __LINE__);
		}
		return Log.error();
	 } 

	void ForceField::performRequiredUpdates_()
	{
		// check whether the selection changed since the last call
		// to update and call update otherwise
		if (update_time_stamp_.isOlderThan(system_->getSelectionTime()))
		{
			// Call update if the selection time stamp changed since
			// the last call to update. This ensures consistency of
			// the selection information in pair lists, bond lists, etc.
			// Also make sure the movable atoms are still in the front
			// of the atom vevtor.
			sortSelectedAtomVector_();
			update();

			// Update the use_selection_ flag.
			use_selection_ = (selection_enabled_ && system_->containsSelection());
		}

		if (setup_time_stamp_.isOlderThan(system_->getModificationTime()))
		{
			// complain if someone meddled with the system while simulating it
			Log.error() << "ForceField::updateForces: Error! System topology was modified (i.e., atoms, residues"
			               "or the like were inserted or removed) after ForceField::Setup was called for the last"
			               "time. The results obtained from this simulation might be erroneous.\n";
		}

	}

# ifdef BALL_NO_INLINE_FUNCTIONS
#   include <BALL/MOLMEC/COMMON/forceField.iC>
# endif
	
} // namespace BALL
