// $Id: amber.C,v 1.7 1999/09/17 13:39:10 oliver Exp $
// Molecular Mechanics: Amber force field class

#include <BALL/MOLMEC/AMBER/amber.h>
#include <BALL/MOLMEC/AMBER/amberStretch.h>
#include <BALL/MOLMEC/AMBER/amberBend.h>
#include <BALL/MOLMEC/AMBER/amberTorsion.h>
#include <BALL/MOLMEC/AMBER/amberNonBonded.h>
#include <BALL/MOLMEC/COMMON/assignTypes.h>

namespace BALL 
{

	const char* AmberFF::Option::NONBONDED_CUTOFF = "nonbonded_cutoff";
	const char* AmberFF::Option::VDW_CUTOFF = "vdw_cutoff";
	const char* AmberFF::Option::ELECTROSTATIC_CUTOFF = "electrostatic_cutoff";
	const char* AmberFF::Option::SCALING_VDW_1_4 = "SCAB";
	const char* AmberFF::Option::SCALING_ELECTROSTATIC_1_4 = "SCEE";
        const char* AmberFF::Option::DISTANCE_DEPENDENT_DIELECTRIC = "DDDC"; 
 
	const float AmberFF::Default::NONBONDED_CUTOFF = 20.0;
	const float AmberFF::Default::VDW_CUTOFF = 15.0;
	const float AmberFF::Default::ELECTROSTATIC_CUTOFF = 15.0;
	const float AmberFF::Default::SCALING_ELECTROSTATIC_1_4 = 0.5;
	const float AmberFF::Default::SCALING_VDW_1_4 = 0.5;
        const bool  AmberFF::Default::DISTANCE_DEPENDENT_DIELECTRIC = false;   
 
	// Default constructor
	AmberFF::AmberFF() 
			// call the base class constructor
		: ForceField()
	{
		// set the force field name
		setName("Amber");

		// create the component list
		insertComponent(new AmberStretch( this ));
		insertComponent(new AmberBend( this ));
		insertComponent(new AmberTorsion( this ));
		insertComponent(new AmberNonBonded( this ));
	}

  // Constructor initialized with a system
  AmberFF::AmberFF(System& system)
    : ForceField()
  {
		// set the force field name
		setName("Amber");

		// create the component list
		insertComponent(new AmberStretch( this ));
		insertComponent(new AmberBend( this ));
		insertComponent(new AmberTorsion( this ));
		insertComponent(new AmberNonBonded( this ));

    bool result = setup(system);

    if (!result)
    {
      Log.level(LogStream::ERROR) << " Force Field setup failed! " << endl;
      valid_ = false;
		}
	}

  // Constructor intialized with a system and a set of options
  AmberFF::AmberFF(System& system, const Options& new_options)
    : ForceField()
  {
		// set the force field name
		setName("Amber");

		// create the component list
		insertComponent(new AmberStretch( this ));
		insertComponent(new AmberBend( this ));
		insertComponent(new AmberTorsion( this ));
		insertComponent(new AmberNonBonded( this ));

    bool result = setup(system, new_options);

    if (!result)
    {
      Log.level(LogStream::ERROR) << " Force Field setup failed! " << endl;
      valid_ = false;
		}
	}
 
	// copy constructor  
	AmberFF::AmberFF(const AmberFF& force_field, bool clone_deep)
		 :	ForceField( force_field, clone_deep) 
	{
		
	}

	// destructor 
	AmberFF::~AmberFF(void)
	{
	}

	// force field specific setup method BAUSTELLE
	bool AmberFF::specificSetup(void)
	{

		// check whether the system is assigned
		if (getSystem() == 0)
		{
			return false;
		}

 
		// open parameter file
		Path    path;
		String  filename(path.find("Amber/amber94.ini"));

		if (filename == "") 
		{
			Log.level(LogStream::ERROR) << "cannot open parameter file Amber/amber94.ini" << endl;
			return false;
		}

		// initialize the force field parameters
		// and retrieve the atom types
		parameters_.setFilename(filename);
		parameters_.init();

		// assign atom types: QUICK AND _VERY_ DIRTY (BAUSTELLLE)
		AssignTypeNameProcessor type_name_proc("/KM/comp-bio/BALL-data/Amber/amber94.types", false);
		getSystem()->apply(type_name_proc);
		AssignTypeProcessor type_proc(parameters_.getAtomTypes());
		getSystem()->apply(type_proc);
		
		return true;
	}

	float AmberFF::getStretchEnergy() const
	{
		ForceFieldComponent* component = getComponent("Amber Stretch");
		if (component != 0)
		{
			return component->getEnergy();
		} else {
			return 0;
		}
	}

	float AmberFF::getBendEnergy() const
	{
		ForceFieldComponent* component = getComponent("Amber Bend");
		if (component != 0)
		{
			return component->getEnergy();
		} else {
			return 0;
		}
	}

	float AmberFF::getTorsionEnergy() const
	{
		ForceFieldComponent* component = getComponent("Amber Torsion");
		if (component != 0)
		{
			return component->getEnergy();
		} else {
			return 0;
		}
	}

	float AmberFF::getVdWEnergy() const
	{
		const ForceFieldComponent* component = getComponent("Amber NonBonded");
		if (component != 0)
		{
			const AmberNonBonded* nonbonded_component = dynamic_cast<const AmberNonBonded*>(component);
			if (nonbonded_component != 0)
			{
				return nonbonded_component->getVdwEnergy();
			}
		}

		return 0;
	}

	float AmberFF::getESEnergy() const
	{
		const ForceFieldComponent* component = getComponent("Amber NonBonded");
		if (component != 0)
		{
			const AmberNonBonded* nonbonded_component = dynamic_cast<const AmberNonBonded*>(component);
			if (nonbonded_component != 0)
			{
				return nonbonded_component->getElectrostaticEnergy();
			}
		}

		return 0;
	}

	float AmberFF::getNonbondedEnergy() const
	{
		const ForceFieldComponent* component = getComponent("Amber NonBonded");
		if (component != 0)
		{
			return component->getEnergy();
		}

		return 0;
	}

} // namespace BALL
