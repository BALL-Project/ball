// $Id: amber.C,v 1.1 1999/08/26 08:02:44 oliver Exp $
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
 
	const float AmberFF::Default::NONBONDED_CUTOFF = 150.0;
	const float AmberFF::Default::VDW_CUTOFF = 150.0;
	const float AmberFF::Default::ELECTROSTATIC_CUTOFF = 150.0;
	const float AmberFF::Default::SCALING_ELECTROSTATIC_1_4 = 0.5;
	const float AmberFF::Default::SCALING_VDW_1_4 = 0.5;
 
	// Default constructor
	AmberFF::AmberFF() 
			// call the base class constructor
		: ForceField()
	{
		// set the force field name
		setName( "Amber" );

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
		setName( "Amber" );

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
		setName( "Amber" );

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
		String  filename(path.find("AMBER95.ini"));

		if (filename == "") 
		{
			Log.level(LogStream::ERROR) << "cannot open parameter file AMBER95.ini" << endl;
			return false;
		}

		// initialize the force field parameters
		// and retrieve the atom types
		parameters_.setFilename(filename);
		parameters_.init();

		// assign atom types: QUICK AND _VERY_ DIRTY (BAUSTELLLE)
		AssignTypeNameProcessor type_name_proc("/KM/comp-bio/BALL-data/amber_types.dat");
		getSystem()->apply(type_name_proc);
		AssignTypeProcessor type_proc(parameters_.getAtomTypes());
		getSystem()->apply(type_proc);
		
		return true;
	}

} // namespace BALL
