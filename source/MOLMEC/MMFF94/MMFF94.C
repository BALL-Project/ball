// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94.C,v 1.1.2.1 2005/03/17 13:48:24 amoll Exp $
//
// Molecular Mechanics: MMFF94 force field class
//

#include <BALL/SYSTEM/path.h>
#include <BALL/MOLMEC/MMFF94/MMFF94.h>
#include <BALL/MOLMEC/MMFF94/MMFF94Stretch.h>
#include <BALL/MOLMEC/MMFF94/MMFF94Bend.h>
#include <BALL/MOLMEC/MMFF94/MMFF94Torsion.h>
#include <BALL/MOLMEC/MMFF94/MMFF94NonBonded.h>
#include <BALL/MOLMEC/COMMON/assignTypes.h>
#include <BALL/MOLMEC/PARAMETER/templates.h>

using namespace std;

namespace BALL 
{
	const char* MMFF94::Option::FOLDER = "folder";

	const char* MMFF94::Default::FOLDER = "MMFF94";

	// Default constructor
	MMFF94::MMFF94() 
		: ForceField(),
			folder_(Default::FOLDER),
			parameters_initialized_(false)
	{
		// set the force field name
		setName("MMFF94");

		// create the component list
		insertComponent(new MMFF94Stretch(*this));
		insertComponent(new MMFF94Bend(*this));
		insertComponent(new MMFF94Torsion(*this));
		insertComponent(new MMFF94NonBonded(*this));
	}

  // Constructor initialized with a system
  MMFF94::MMFF94(System& system)
    : ForceField(),
			folder_(Default::FOLDER),
			parameters_initialized_(false)
  {
		// create the component list
		insertComponent(new MMFF94Stretch(*this));
		insertComponent(new MMFF94Bend(*this));
		insertComponent(new MMFF94Torsion(*this));
		insertComponent(new MMFF94NonBonded(*this));

    bool result = setup(system);

		// set the force field name
		setName("MMFF94");

    if (!result)
    {
      Log.error() << " Force Field setup failed! " << endl;
      valid_ = false;
		}
	}

  // Constructor intialized with a system and a set of options
  MMFF94::MMFF94(System& system, const Options& new_options)
    : ForceField(),
			folder_(Default::FOLDER),
			parameters_initialized_(false)
  {
		// create the component list
		insertComponent(new MMFF94Stretch(*this));
		insertComponent(new MMFF94Bend(*this));
		insertComponent(new MMFF94Torsion(*this));
		insertComponent(new MMFF94NonBonded(*this));

    bool result = setup(system, new_options);

		// set the force field name (this has to be done after(!) setup,
		// otherwise filename_ is not yet set if it is used in options)
		setName("MMFF94");

    if (!result)
    {
      Log.error() << " Force Field setup failed! " << endl;
      valid_ = false;
		}
	}
 
	// copy constructor  
	MMFF94::MMFF94(const MMFF94& force_field)
		:	ForceField(force_field),
			folder_(force_field.folder_),
			parameters_initialized_(false)
	{
	}

	// destructor 
	MMFF94::~MMFF94()
	{
	}

	void MMFF94::clear()
		throw()
	{
		ForceField::clear();
		parameters_initialized_ = false;
	}

	const MMFF94& MMFF94::operator = (const MMFF94& force_field)
		throw()
	{
		// avoid self assignment
		if (&force_field != this)
		{
			ForceField::operator = (force_field);
			parameters_initialized_ = force_field.parameters_initialized_;
		}
		
		return *this;
	}

	bool MMFF94::specificSetup()
		throw(Exception::TooManyErrors)
	{
		// check whether the system is assigned
		if (getSystem() == 0)
		{
			return false;
		}
 
		// check whether the parameter file name
		// is set in the options
		if (options.has(Option::FOLDER))
		{
			folder_ = options[Option::FOLDER];
			setName("MMFF94");
		} 
		else 
		{
			options[Option::FOLDER] = folder_;
		}

		// open parameter file
		Path    path;
		String  folder(path.find(folder_));

		if (folder == "") 
		{
			throw Exception::FileNotFound(__FILE__, __LINE__, folder_);
		}

		return true;
	}

	Size MMFF94::getUpdateFrequency() const
	{
		return 20;
	}

	double MMFF94::getStretchEnergy() const
	{
		ForceFieldComponent* component = getComponent("MMFF94 Stretch");
		if (component != 0)
		{
			return component->getEnergy();
		} 
		else 
		{
			return 0.0;
		}
	}

	double MMFF94::getBendEnergy() const
	{
		ForceFieldComponent* component = getComponent("MMFF94 Bend");
		if (component != 0)
		{
			return component->getEnergy();
		} 
		else 
		{
			return 0;
		}
	}

	double MMFF94::getTorsionEnergy() const
	{
		ForceFieldComponent* component = getComponent("MMFF94 Torsion");
		if (component != 0)
		{
			return component->getEnergy();
		} 
		else 
		{
			return 0;
		}
	}

	double MMFF94::getVdWEnergy() const
	{
		const ForceFieldComponent* component = getComponent("MMFF94 NonBonded");
		if (component != 0)
		{
			const MMFF94NonBonded* nonbonded_component = dynamic_cast<const MMFF94NonBonded*>(component);
			if (nonbonded_component != 0)
			{
				return nonbonded_component->getVdwEnergy();
			}
		}

		return 0;
	}

	double MMFF94::getESEnergy() const
	{
		const ForceFieldComponent* component = getComponent("MMFF94 NonBonded");
		if (component != 0)
		{
			const MMFF94NonBonded* nonbonded_component = dynamic_cast<const MMFF94NonBonded*>(component);
			if (nonbonded_component != 0)
			{
				return nonbonded_component->getElectrostaticEnergy();
			}
		}

		return 0;
	}

	double MMFF94::getNonbondedEnergy() const
	{
		const ForceFieldComponent* component = getComponent("MMFF94 NonBonded");
		if (component != 0)
		{
			return component->getEnergy();
		}

		return 0;
	}

	bool MMFF94::hasInitializedParameters() const
	{
		return parameters_initialized_;
	}

	String MMFF94::getResults() const
		throw()
	{
		String result = String("\n")
		+ "MMFF94 Energy:\n"
		+ " - electrostatic     : " +String(getESEnergy())+  " kJ/mol\n" 
		+ " - van der Waals     : " +String(getVdWEnergy())+  " kJ/mol\n"
		+ " - bond stretch      : " +String(getStretchEnergy())+  " kJ/mol\n"
		+ " - angle bend        : " +String(getBendEnergy())+  " kJ/mol\n" 
		+ " - torsion           : " +String(getTorsionEnergy())+  " kJ/mol\n" 
		+ "---------------------------------------\n" 
		+ "  total energy       : " +String(getEnergy()) + " kJ/mol\n";
		return result;
	}
	
} // namespace BALL
