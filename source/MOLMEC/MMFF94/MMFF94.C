// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94.C,v 1.1.4.1 2006/05/21 22:26:08 amoll Exp $
//
// Molecular Mechanics: MMFF94 force field class
//

#include <BALL/SYSTEM/path.h>
#include <BALL/SYSTEM/fileSystem.h>
#include <BALL/MOLMEC/MMFF94/MMFF94.h>
#include <BALL/MOLMEC/MMFF94/MMFF94StretchBend.h>
#include <BALL/MOLMEC/MMFF94/MMFF94Torsion.h>
#include <BALL/MOLMEC/MMFF94/MMFF94NonBonded.h>
#include <BALL/MOLMEC/MMFF94/MMFF94OutOfPlaneBend.h>
#include <BALL/QSAR/ringPerceptionProcessor.h>
#include <BALL/QSAR/aromaticityProcessor.h>

//    #define BALL_DEBUG_MMFF
#define BALL_DEBUG_TEST

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

		insertComponents_();
	}

  // Constructor initialized with a system
  MMFF94::MMFF94(System& system)
    : ForceField(),
			folder_(Default::FOLDER),
			parameters_initialized_(false)
  {
		insertComponents_();

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
		insertComponents_();

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

	void MMFF94::insertComponents_()
	{
 		insertComponent(new MMFF94StretchBend(*this));
 		insertComponent(new MMFF94Torsion(*this));
 		insertComponent(new MMFF94NonBonded(*this));
 		insertComponent(new MMFF94OutOfPlaneBend(*this));
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

		collectBonds_();
		collectRings_();

		if (!parameters_initialized_)
		{
			String prefix = folder + FileSystem::PATH_SEPARATOR;
			atom_types_.readParameters(prefix + "MMFFPROP.PAR");
			equivalences_.readParameters(prefix + "MMFFDEF.PAR");
			bond_parameters_.readParameters(prefix + "MMFFBOND.PAR");
			bond_parameters_.readEmpericalParameters(prefix + "MMFFBNDK.PAR");
			parameters_initialized_ = true;

			es_parameters_.readParameters(prefix + "MMFFCHG.PAR");
			es_parameters_.readEmpericalParameters(prefix + "MMFFPBCI.PAR");

			atom_typer_.setup(prefix + "TYPES.PAR");
			atom_typer_.setupHydrogenTypes(prefix + "MMFFHDEF.PAR");
			atom_typer_.setupSymbolsToTypes(prefix + "MFFSYMB.PAR");
			atom_typer_.setupAromaticTypes(prefix + "MMFFAROM.PAR");
			atom_typer_.collectHeteroAtomTypes(atom_types_);

			charge_processor_.setup(prefix + "CHARGES.PAR");
			charge_processor_.setESParameters(es_parameters_);
		}

		/////////////////////////////////////////////////////////
		// atom types 
		/*
		atom_typer_.setAromaticRings(getAromaticRings());
		atom_typer_.assignTo(*system_);

		// for future use mark the invalid atoms:
		for (Position p = 0; p < atoms_.size() ; p++)
		{
			if (!checkAtomType(*atoms_[p]))
			{
				atoms_[p]->setType(0);
				atoms_[p]->setTypeName("Any");
			}
		}

		if (unassigned_atoms_.size() > 0)
		{
			error() << "Could not assign atom types for " << unassigned_atoms_.size() << " atoms." << std::endl;
		}
		*/

		/////////////////////////////////////////////////////////
		// bond types
		assignBondTypes_();

		/////////////////////////////////////////////////////////
		// atom charges
		charge_processor_.setAromaticRings(getAromaticRings());
		getSystem()->apply(charge_processor_);

		if (charge_processor_.getUnassignedAtoms().size() > 0)
		{
			error() << "Could not assign partial charges for all atoms" << std::endl;
			HashSet<Atom*>::ConstIterator it = charge_processor_.getUnassignedAtoms().begin();
			for (;+it; ++it)
			{
				getUnassignedAtoms().insert(*it);
			}
		}

		return true;
	}


	Size MMFF94::getUpdateFrequency() const
	{
		return 20;
	}

	double MMFF94::getStretchEnergy() const
	{
		ForceFieldComponent* component = getComponent("MMFF94 StretchBend");
		if (component != 0)
		{
			return ((MMFF94StretchBend*)component)->getStretchEnergy();
		} 
		else 
		{
			return 0.0;
		}
	}

	double MMFF94::getBendEnergy() const
	{
		ForceFieldComponent* component = getComponent("MMFF94 StretchBend");
		if (component != 0)
		{
			return ((MMFF94StretchBend*)component)->getBendEnergy();
		} 
		else 
		{
			return 0;
		}
	}

	double MMFF94::getStretchBendEnergy() const
	{
		ForceFieldComponent* component = getComponent("MMFF94 StretchBend");
		if (component != 0)
		{
			return ((MMFF94StretchBend*)component)->getStretchBendEnergy();
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
				return nonbonded_component->getVDWEnergy();
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
				return nonbonded_component->getESEnergy();
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
	

	bool MMFF94::assignMMFF94BondType(Bond& bond) const
	{
		MMFF94StretchParameters::StretchMap::ConstIterator it;
		it = bond_parameters_.getParameters(bond.getFirstAtom()->getType(),
																				bond.getSecondAtom()->getType());

		if (!+it) return false;

		const MMFF94StretchParameters::BondData& data = it->second;

#ifdef BALL_DEBUG_TEST
		if (!data.sbmb_exists && !data.standard_bond_exists)
		{
			Log.error() << "Error in " << __FILE__ << " " << __LINE__ << std::endl;
			return false;
		}
#endif

		bool is_sbmb = false;
		if (data.sbmb_exists && ! data.standard_bond_exists)
		{
			is_sbmb = true;
		}

		else if (!data.sbmb_exists && data.standard_bond_exists)
		{
			is_sbmb = false;
		}

		else
		{
			const Atom& atom1 = *(Atom*)bond.getFirstAtom();
			const Atom& atom2 = *(Atom*)bond.getSecondAtom();

			// ok, this bond can be a sbmb or a standard bond
			// it is sbmb if :

			// is the bond order == 1 ?
			// are both atoms sp or sp2 hypridised?
			// both atoms are not in one aromatic ring
			const bool in_one_ring = isInOneAromaticRing(bond);

			is_sbmb = bond.getOrder() == Bond::ORDER__SINGLE  &&
								!in_one_ring;

			// check if both atoms are in different aromatic rings
			if (!is_sbmb && !in_one_ring)
			{
				bool a1 = false;
				bool a2 = false;

				for (Position pos = 0; pos < aromatic_rings_.size(); pos++)
				{
					if (aromatic_rings_[pos].has((Atom*)&atom1)) a1 = true;
					if (aromatic_rings_[pos].has((Atom*)&atom2)) a2 = true;
				}
			
			  if (a1 && a2) is_sbmb = true;
			}
#ifdef BALL_DEBUG_MMFF
Log.info() << atom1.getName() << " " << atom2.getName() << "  order single: "
					 << (bond.getOrder() == Bond::ORDER__SINGLE) << "  sp: " 
					 << !isInOneAromaticRing(bond) << std::endl;
#endif
		}

		if (is_sbmb)
		{
			bond.setProperty("MMFF94SBMB");
		}
		else
		{
			bond.clearProperty("MMFF94SBMB");
		}

		return true;
	}

	bool MMFF94::isInOneAromaticRing(const Bond& bond) const
	{
		Atom* atom1 = (Atom*) bond.getFirstAtom();
		Atom* atom2 = (Atom*) bond.getSecondAtom();

		for (Position pos = 0; pos < aromatic_rings_.size(); pos++)
		{
			if (aromatic_rings_[pos].has(atom1) &&
					aromatic_rings_[pos].has(atom2))
			{
				return true;
			}
		}
		
		return false;
	}

	void MMFF94::collectBonds_()
	{
		bonds_.clear();

		Atom::BondIterator bond_iterator;
		AtomVector::ConstIterator atom_it = getAtoms().begin();
		for ( ; atom_it != getAtoms().end(); ++atom_it)
		{
			for (Atom::BondIterator it = (*atom_it)->beginBond(); +it ; ++it) 
			{
				// collect all bonds only once!
				if (*atom_it != it->getFirstAtom()) continue;
				
				Bond&	bond = const_cast<Bond&>(*it);

				// Ignore hydrogen bonds!
				if (bond.getType() == Bond::TYPE__HYDROGEN) continue;

				if (getUseSelection() &&
						(!bond.getFirstAtom()->isSelected() ||
						 !bond.getSecondAtom()->isSelected()))
				{
					continue;
				}

				bonds_.push_back(&bond);
			}
		}
	}

	void MMFF94::assignBondTypes_()
	{
		vector<Bond*>::iterator bit = bonds_.begin();
		for (; bit != bonds_.end(); bit++)
		{
			assignMMFF94BondType(**bit);
		}
	}
		
	
	void MMFF94::collectRings_()
	{
		///////////////////////////////////////
		/// calculate all rings in the molecule
		rings_.clear();
		vector<vector<Atom*> > rings;

		RingPerceptionProcessor rpp;
		rpp.calculateSSSR(rings, *getSystem());

		for (Position i = 0; i < rings.size(); i++)
		{
			// all 3er and 4er rings are copied below
 			if (rings[i].size() == 3 || rings[i].size() == 4) continue;

			HashSet<Atom*> set;
			for (Position j = 0; j < rings[i].size(); j++)
			{
				set.insert(rings[i][j]);
			}
			rings_.push_back(set);
		}

		vector<vector<Atom*> > rings2 = rpp.getAllSmallRings();

		// copy 3er and 4er rings
		for (Position i = 0; i < rings2.size(); i++)
		{
			HashSet<Atom*> set;
			for (Position j = 0; j < rings2[i].size(); j++)
			{
				set.insert(rings2[i][j]);
			}
			rings_.push_back(set);
		}


		///////////////////////////////////////
		/// calculate all aromatic rings in the molecule
		aromatic_rings_.clear();

		AromaticityProcessor ap;
		ap.aromatizeSimple(rings);
	
		for (Position i = 0; i < rings.size(); i ++)
		{
			HashSet<Atom*> set;
			for (Position j = 0; j < rings[i].size(); j++)
			{
				set.insert(rings[i][j]);
			}
			aromatic_rings_.push_back(set);
		}

#ifdef BALL_DEBUG_MMFF
		Log.info() << "MMFF94: Found " << rings_.size() << " rings: ";
		for (Position pos = 0; pos < rings_.size(); pos++)
		{
			Log.info() << rings_[pos].size() << " ";
		}
		Log.info() << std::endl;

		Log.info() << "MMFF94: Found " << aromatic_rings_.size() << " aromatic rings: ";
		for (Position pos = 0; pos < aromatic_rings_.size(); pos++)
		{
			Log.info() << aromatic_rings_[pos].size() << " ";
		}
		Log.info() << std::endl;
#endif

	}

	bool MMFF94::areInOneRing(vector<Atom*> v, Size ring_size) const
	{
		for (Position i = 0; i < rings_.size(); i++)
		{
			if (ring_size != 0 && ring_size != rings_[i].size()) continue;

			bool found = true;
			for (Position j = 0; j < v.size(); j++)
			{
				if (!rings_[i].has(v[j])) 
				{
					found = false;
					break;
				}
			}

			if (found) return true;
		}

		return false;
	}

	bool MMFF94::areInOneAromaticRing(const vector<Atom*>& v, Size ring_size) const
	{
		for (Position i = 0; i < aromatic_rings_.size(); i++)
		{
			if (ring_size != 0 && ring_size != aromatic_rings_[i].size()) continue;

			bool found = true;
			for (Position j = 0; j < v.size(); j++)
			{
				if (!aromatic_rings_[i].has(v[j])) 
				{
					found = false;
					break;
				}
			}

			if (found) return true;
		}

		return false;
	}

	bool MMFF94::checkAtomType(Atom& atom)
	{
		const Index atom_type = atom.getType();

		if (atom_type < 1 || 
				atom_type > (Index) MMFF94_number_atom_types - 1  ||
 				!atom_types_.getAtomTypes()[atom_type].valid)
		{
			if (unassigned_atoms_.has(&atom)) return false;

			unassigned_atoms_.insert(&atom);
			error() << "Invalid atom type: " << atom.getFullName() << " " << atom_type << std::endl;
			return false;
		}

		return true;
	}

} // namespace BALL
