// $Id: charmm.C,v 1.10 2000/08/07 17:28:57 oliver Exp $
// Molecular Mechanics: Charmm force field class

#include <BALL/MOLMEC/CHARMM/charmm.h>
#include <BALL/MOLMEC/CHARMM/charmmStretch.h>
#include <BALL/MOLMEC/CHARMM/charmmBend.h>
#include <BALL/MOLMEC/CHARMM/charmmTorsion.h>
#include <BALL/MOLMEC/CHARMM/charmmImproperTorsion.h>
#include <BALL/MOLMEC/CHARMM/charmmNonBonded.h>
#include <BALL/MOLMEC/COMMON/assignTypes.h>
#include <BALL/MOLMEC/PARAMETER/templates.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>

using namespace std;

namespace BALL 
{
	const char* CharmmFF::Option::FILENAME = "filename";
	const char* CharmmFF::Option::NONBONDED_CUTOFF = "nonbonded_cutoff";
	const char* CharmmFF::Option::VDW_CUTOFF = "vdw_cutoff";
	const char* CharmmFF::Option::VDW_CUTON = "vdw_cuton";
	const char* CharmmFF::Option::ELECTROSTATIC_CUTOFF = "electrostatic_cutoff";
	const char* CharmmFF::Option::ELECTROSTATIC_CUTON = "electrostatic_cuton";
	const char* CharmmFF::Option::SOLVATION_CUTOFF = "solvation_cutoff";
	const char* CharmmFF::Option::SOLVATION_CUTON = "solvation_cuton";
	const char* CharmmFF::Option::SCALING_VDW_1_4 = "SCAB";
	const char* CharmmFF::Option::SCALING_ELECTROSTATIC_1_4 = "SCEE";
	const char* CharmmFF::Option::DISTANCE_DEPENDENT_DIELECTRIC = "DDDC"; 
	const char* CharmmFF::Option::ASSIGN_CHARGES = "assign_charges"; 
	const char* CharmmFF::Option::ASSIGN_TYPENAMES = "assign_type_names"; 
	const char* CharmmFF::Option::ASSIGN_TYPES = "assign_types"; 
	const char* CharmmFF::Option::OVERWRITE_CHARGES = "overwrite_non-zero_charges"; 
	const char* CharmmFF::Option::OVERWRITE_TYPENAMES = "overwrite_non-empty_typenames"; 
	const char* CharmmFF::Option::USE_EEF1 = "use_EEF1"; 
 
	const char* CharmmFF::Default::FILENAME = "CHARMM/param22.ini";
	const float CharmmFF::Default::NONBONDED_CUTOFF = 20.0;
	const float CharmmFF::Default::VDW_CUTOFF = 9.0;
	const float CharmmFF::Default::VDW_CUTON = 7.0;
	const float CharmmFF::Default::SOLVATION_CUTOFF = 9.0;
	const float CharmmFF::Default::SOLVATION_CUTON = 7.0;
	const float CharmmFF::Default::ELECTROSTATIC_CUTOFF = 9.0;
	const float CharmmFF::Default::ELECTROSTATIC_CUTON = 7.0;
	const float CharmmFF::Default::SCALING_ELECTROSTATIC_1_4 = 2.0;
	const float CharmmFF::Default::SCALING_VDW_1_4 = 1.0;
  const bool  CharmmFF::Default::DISTANCE_DEPENDENT_DIELECTRIC = true;
	const bool	CharmmFF::Default::ASSIGN_CHARGES = true;
	const bool	CharmmFF::Default::ASSIGN_TYPENAMES = true;
	const bool	CharmmFF::Default::ASSIGN_TYPES = true;
	const bool	CharmmFF::Default::OVERWRITE_CHARGES = true;
	const bool	CharmmFF::Default::OVERWRITE_TYPENAMES = false;
	const bool	CharmmFF::Default::USE_EEF1 = true;
 
	// Default constructor
	CharmmFF::CharmmFF() 
		: ForceField(),
			filename_(Default::FILENAME)
	{
		// set the force field name
		setName("CHARMM [" + filename_ + "]");

		// create the component list
		insertComponent(new CharmmStretch(*this));
		insertComponent(new CharmmBend(*this));
		insertComponent(new CharmmTorsion(*this));
		insertComponent(new CharmmImproperTorsion(*this));
		insertComponent(new CharmmNonBonded(*this));
	}

  // Constructor initialized with a system
  CharmmFF::CharmmFF(System& system)
    : ForceField(),
			filename_(Default::FILENAME)
  {
		// create the component list
		insertComponent(new CharmmStretch(*this));
		insertComponent(new CharmmBend(*this));
		insertComponent(new CharmmTorsion(*this));
		insertComponent(new CharmmImproperTorsion(*this));
		insertComponent(new CharmmNonBonded(*this));

    bool result = setup(system);

		// set the force field name
		setName("CHARMM [" + filename_ + "]");

    if (!result)
    {
      Log.level(LogStream::ERROR) << " Force Field setup failed! " << endl;
      valid_ = false;
		}
	}

  // Constructor intialized with a system and a set of options
  CharmmFF::CharmmFF(System& system, const Options& new_options)
    : ForceField(),
			filename_(Default::FILENAME)
  {
		// create the component list
		insertComponent(new CharmmStretch(*this));
		insertComponent(new CharmmBend(*this));
		insertComponent(new CharmmTorsion(*this));
		insertComponent(new CharmmImproperTorsion(*this));
		insertComponent(new CharmmNonBonded(*this));

    bool result = setup(system, new_options);

		// set the force field name (this has to be done after(!) setup,
		// otherwise filename_ is not yet set if it is used in options)
		setName("CHARMM [" + filename_ + "]");

    if (!result)
    {
      Log.level(LogStream::ERROR) << " Force Field setup failed! " << endl;
      valid_ = false;
		}
	}
 
	// copy constructor  
	CharmmFF::CharmmFF(const CharmmFF& force_field, bool clone_deep)
		:	ForceField( force_field, clone_deep),
			filename_(force_field.filename_)
	{
	}

	// destructor 
	CharmmFF::~CharmmFF()
	{
	}

	// force field specific setup method
	bool CharmmFF::specificSetup()
	{
		// check whether the system is assigned
		if (getSystem() == 0)
		{
			return false;
		}
 
		// check whether the parameter file name
		// is set in the options
		if (options.has(Option::FILENAME))
		{
			filename_ = options[Option::FILENAME];
			setName("Charmm [" + filename_ + "]");
		} 
		else 
		{
			options[Option::FILENAME] = filename_;
		}

		// open parameter file
		Path    path;
		String  filename(path.find(filename_));

		if (filename == "") 
		{
			throw Exception::FileNotFound(__FILE__, __LINE__, filename_);
		}

		// initialize the force field parameters
		// and retrieve the atom types
		if (parameters_.getFilename() != filename)
		{
			parameters_.setFilename(filename);
			parameters_.init();

			// this is the first time, parameters was initialized
			// tell all components about it
			parameters_initialized_ = false;
		} 
		else	
		{
			// parameters_ are already initialized, tell all components about it
			parameters_initialized_ = true;
		}

		// check the options whether types, type names, or charges 
		// have to be (re)asigned
		options.setDefaultBool(Option::ASSIGN_CHARGES, Default::ASSIGN_CHARGES);
		bool assign_charges = options.getBool(Option::ASSIGN_CHARGES);
		options.setDefaultBool(Option::ASSIGN_TYPES, Default::ASSIGN_TYPES);
		bool assign_types = options.getBool(Option::ASSIGN_TYPES);
		options.setDefaultBool(Option::ASSIGN_TYPENAMES, Default::ASSIGN_TYPENAMES);
		bool assign_type_names = options.getBool(Option::ASSIGN_TYPENAMES);
		options.setDefaultBool(Option::OVERWRITE_TYPENAMES, Default::OVERWRITE_TYPENAMES);
		bool overwrite_type_names = options.getBool(Option::OVERWRITE_TYPENAMES);
		options.setDefaultBool(Option::OVERWRITE_CHARGES, Default::OVERWRITE_CHARGES);
		bool overwrite_charges = options.getBool(Option::OVERWRITE_CHARGES);
		
		bool remove_hydrogens = true;

		// extract template section (containing charges and atom types)
		if (assign_charges || assign_type_names || remove_hydrogens)
		{
			Templates templates;
			templates.extractSection(parameters_, "ChargesAndTypeNames");
			
			// remove all hydrogens bound to extended atom types
			if (remove_hydrogens)
			{
				HashSet<Atom*> atoms_to_delete;
				AtomIterator it = getSystem()->beginAtom();
				for (; +it; ++it)
				{
					if (it->getElement() != PTE[Element::H])
					{
	 					if (templates.has(it->getFullName()))
						{
							String type_name = templates.getTypeName(it->getFullName());
							bool extended = getParameters().getAtomTypes().getValue(type_name, "extended").toBool();
							if (extended) 
							{
								// check for hydrogen atoms and insert them into the hash set
								// We use a hash set just in case - usually a hydrogen shouldn`t be 
								// bound to two atoms!
								Atom::BondIterator bond_it = it->beginBond();
								for (; +bond_it; ++bond_it)
								{
									if (bond_it->getPartner(*it)->getElement() == PTE[Element::H])
									{
										// if the atom is a hydrogen atom, store its pointer
										// for removal
										atoms_to_delete.insert(bond_it->getPartner(*it));
									}
								}
							}
						}
					}
				}

				if (atoms_to_delete.size() > 0)
				{
					Log.info() << "CharmmFF::setup: deleted " << atoms_to_delete.size() << " hydrogen atoms (using CHARMM united atoms instead)." << endl;
					HashSet<Atom*>::Iterator hydrogen_it = atoms_to_delete.begin();
					for (; hydrogen_it != atoms_to_delete.end(); ++hydrogen_it)
					{
						delete *hydrogen_it;
					}
						
					// tell the ForceField class to recalculate the atoms_ vector 
					// - we deleted things!
					atoms_.clear();
				}
			}

			if (assign_charges && assign_type_names)
			{
				templates.assign(*getSystem(), overwrite_type_names, overwrite_charges);
			} 
			else 
			{
				if (assign_type_names)
				{
					templates.assignTypeNames(*getSystem(), overwrite_type_names);
				} 
				else 
				{
					templates.assignCharges(*getSystem(), overwrite_charges);
				}
			}
		}

		// assign the types (type names should have been set already)
		if (assign_types)
		{
			// convert the type names to types
			AssignTypeProcessor type_proc(parameters_.getAtomTypes());
			getSystem()->apply(type_proc);			
		}
		
		return true;
	}

	double CharmmFF::getStretchEnergy() const
	{
		ForceFieldComponent* component = getComponent("CHARMM Stretch");
		if (component != 0)
		{
			return component->getEnergy();
		} 
		else 
		{
			return 0;
		}
	}

	double CharmmFF::getBendEnergy() const
	{
		ForceFieldComponent* component = getComponent("CHARMM Bend");
		if (component != 0)
		{
			return component->getEnergy();
		} 
		else 
		{
			return 0;
		}
	}

	double CharmmFF::getImproperTorsionEnergy() const
	{
		ForceFieldComponent* component = getComponent("CHARMM ImproperTorsion");
		if (component != 0)
		{
			return component->getEnergy();
		} 
		else 
		{
			return 0;
		}
	}

	double CharmmFF::getProperTorsionEnergy() const
	{
		ForceFieldComponent* component = getComponent("CHARMM Torsion");
		if (component != 0)
		{
			return component->getEnergy();
		} 
		else 
		{
			return 0;
		}
	}

	double CharmmFF::getTorsionEnergy() const
	{
		double energy = 0;
		
		// get the energy of the proper torsions
		ForceFieldComponent* component = getComponent("CHARMM Torsion");
		if (component != 0)
		{
			energy += component->getEnergy();
		} 
		// and add the energy of the improper torsions
		component = getComponent("CHARMM ImproperTorsion");
		if (component != 0)
		{
			energy += component->getEnergy();
		} 
		
		return energy;
	}

	double CharmmFF::getVdWEnergy() const
	{
		double energy = 0;
		const ForceFieldComponent* component = getComponent("CHARMM NonBonded");
		if (component != 0)
		{
			const CharmmNonBonded* nonbonded_component = dynamic_cast<const CharmmNonBonded*>(component);
			if (nonbonded_component != 0)
			{
				energy = nonbonded_component->getVdwEnergy();
			}
		}

		return energy;
	}

	double CharmmFF::getESEnergy() const
	{
		double energy = 0;
		const ForceFieldComponent* component = getComponent("CHARMM NonBonded");
		if (component != 0)
		{
			const CharmmNonBonded* nonbonded_component = dynamic_cast<const CharmmNonBonded*>(component);
			if (nonbonded_component != 0)
			{
				energy = nonbonded_component->getElectrostaticEnergy();
			}
		}

		return energy;
	}

	double CharmmFF::getSolvationEnergy() const
	{
		double energy = 0;
		const ForceFieldComponent* component = getComponent("CHARMM NonBonded");
		if (component != 0)
		{
			const CharmmNonBonded* nonbonded_component = dynamic_cast<const CharmmNonBonded*>(component);
			if (nonbonded_component != 0)
			{
				energy = nonbonded_component->getSolvationEnergy();
			}
		}

		return energy;
	}

	double CharmmFF::getNonbondedEnergy() const
	{
		double energy = 0;
		const ForceFieldComponent* component = getComponent("CHARMM NonBonded");
		if (component != 0)
		{
			energy += component->getEnergy();
		}

		return energy;
	}

	bool CharmmFF::hasInitializedParameters() const
	{
		return parameters_initialized_;
	}
	
} // namespace BALL
