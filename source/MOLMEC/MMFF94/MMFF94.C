// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94.C,v 1.1.8.2 2007/03/26 13:47:50 amoll Exp $
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

// #define BALL_DEBUG_MMFF
// #define BALL_DEBUG_TEST

using namespace std;

namespace BALL
{
	const char* MMFF94::Option::FOLDER = "folder";
	const char* MMFF94::Option::ASSIGN_CHARGES = "assign_charges"; 
	const char* MMFF94::Option::ASSIGN_TYPENAMES = "assign_type_names"; 
	const char* MMFF94::Option::ASSIGN_TYPES = "assign_types"; 
	const char* MMFF94::Option::OVERWRITE_CHARGES = "overwrite_non-zero_charges"; 
	const char* MMFF94::Option::OVERWRITE_TYPENAMES = "overwrite_non-empty_typenames"; 
	const char* MMFF94::Option::NONBONDED_CUTOFF = "nonbonded_cutoff";
	const char* MMFF94::Option::VDW_CUTOFF = "vdw_cutoff";
	const char* MMFF94::Option::VDW_CUTON = "vdw_cuton";
	const char* MMFF94::Option::ELECTROSTATIC_CUTOFF = "electrostatic_cutoff";
	const char* MMFF94::Option::ELECTROSTATIC_CUTON = "electrostatic_cuton";
	const char* MMFF94::Option::DISTANCE_DEPENDENT_DIELECTRIC = "distance_dependend_dc";

	const char* MMFF94::Default::FOLDER = "MMFF94";
	const bool  MMFF94::Default::ASSIGN_CHARGES = true; 
	const bool  MMFF94::Default::ASSIGN_TYPENAMES = true; 
	const bool  MMFF94::Default::ASSIGN_TYPES = true; 
	const bool  MMFF94::Default::OVERWRITE_CHARGES = true; 
	const bool  MMFF94::Default::OVERWRITE_TYPENAMES = true; 
	const float MMFF94::Default::NONBONDED_CUTOFF = 16.0;
	const float MMFF94::Default::VDW_CUTOFF = 15.0;
	const float MMFF94::Default::VDW_CUTON = 13.0;
	const float MMFF94::Default::ELECTROSTATIC_CUTOFF = 15.0;
	const float MMFF94::Default::ELECTROSTATIC_CUTON = 13.0;
	const bool  MMFF94::Default::DISTANCE_DEPENDENT_DIELECTRIC = false; 

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
		
	{
		ForceField::clear();
		parameters_initialized_ = false;
	}

	const MMFF94& MMFF94::operator = (const MMFF94& force_field)
		
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
		if (getSystem() == 0) return false;
 
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

		options.setDefaultBool(Option::ASSIGN_CHARGES, Default::ASSIGN_CHARGES);
		options.setDefaultBool(Option::ASSIGN_TYPES, Default::ASSIGN_TYPES);

		// open parameter file
		Path    path;
		String  folder(path.find(folder_));

		if (folder == "") 
		{
			throw Exception::FileNotFound(__FILE__, __LINE__, folder_);
		}

		collectBonds_();
		collectRings_();

		/////////////////////////////////////////////
		// Kekulise all aromatic bonds:

		// workaround for differend set classes:
		vector<set<Atom*> > arings, rings;
		
		HashSet<Atom*>::Iterator hit;

		arings.resize(aromatic_rings_.size());
		for (Position p = 0; p < aromatic_rings_.size(); p++)
		{
			for ( hit=aromatic_rings_[p].begin(); +hit;++hit)
			{
				arings[p].insert(*hit);
			}
		}

		rings.resize(rings_.size());
		for (Position p = 0; p < rings_.size(); p++)
		{
			for ( hit=rings_[p].begin(); +hit;++hit)
			{
				rings[p].insert(*hit);
			}
		}
		// workaround end

		kekuliser_.setAromaticRings(arings);
		kekuliser_.setRings(rings);
		MoleculeIterator mit = system_->beginMolecule();
		for (; +mit; ++mit)
		{
			kekuliser_.setup(*mit);
		}

		const vector<Bond*>& ubonds = kekuliser_.getUnassignedBonds();
		for (Position p = 0; p < ubonds.size(); p++)
		{
			Atom* a1 = (Atom*)ubonds[p]->getFirstAtom();
			Atom* a2 = (Atom*)ubonds[p]->getSecondAtom();
			unassigned_atoms_.insert(a1);
			unassigned_atoms_.insert(a2);
			error() << "Cannot kekulise bond " 
							<< a1->getFullName(Atom::ADD_RESIDUE_ID) << " -> " 
							<< a2->getFullName(Atom::ADD_RESIDUE_ID) << std::endl;
		}

		kekuliser_.clear();
		collectRings_();

		/////////////////////////////////////////////
		// initialise all parameter sets from the corresponding files:
		if (!parameters_initialized_)
		{
			String prefix = folder + FileSystem::PATH_SEPARATOR;
			Parameters p(prefix + "mmff94.ini");
			atom_types_.readParameters(p, "AtomTypeProperties");
			equivalences_.readParameters(p, "Equivalences");
			bond_parameters_.readParameters(p, "BondStretch");
			bond_parameters_.readEmpiricalParameters(p, "EmpiricalBondParams");
			parameters_initialized_ = true;

			es_parameters_.readParameters(p, "PartialCharges");
			es_parameters_.readEmpiricalParameters(p, "PartialBondCharges");

			atom_typer_.setup(prefix + "TYPES.PAR");
			atom_typer_.setupHydrogenTypes(p, "HydrogenTypes");
			atom_typer_.setupSymbolsToTypes(p, "Symbols");
			atom_typer_.setupAromaticTypes(p, "Aromatic");
			atom_typer_.collectHeteroAtomTypes(atom_types_);

			charge_processor_.setup("Charges");
			charge_processor_.setESParameters(es_parameters_);
		}

		/////////////////////////////////////////////////////////
		// atom types 
		
		bool assign_types = options.getBool(Option::ASSIGN_TYPES);

		if (assign_types)
 		{
			atom_typer_.setAromaticRings(getAromaticRings());
			atom_typer_.setRings(getRings());
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
		}
			

		/////////////////////////////////////////////////////////
		// bond types
		assignBondTypes_();

		/////////////////////////////////////////////////////////
		// atom charges
		bool assign_charges = options.getBool(Option::ASSIGN_CHARGES);
		if (assign_charges)
		{
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
		return 0.0;
	}

	double MMFF94::getBendEnergy() const
	{
		ForceFieldComponent* component = getComponent("MMFF94 StretchBend");
		if (component != 0)
		{
			return ((MMFF94StretchBend*)component)->getBendEnergy();
		} 
		return 0;
	}

	double MMFF94::getStretchBendEnergy() const
	{
		ForceFieldComponent* component = getComponent("MMFF94 StretchBend");
		if (component != 0)
		{
			return ((MMFF94StretchBend*)component)->getStretchBendEnergy();
		} 
		return 0;
	}

	double MMFF94::getTorsionEnergy() const
	{
		ForceFieldComponent* component = getComponent("MMFF94 Torsion");
		if (component != 0) return component->getEnergy();
		return 0;
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

	double MMFF94::getPlaneEnergy() const
	{
		const ForceFieldComponent* component = getComponent("MMFF94 OutOfPlaneBend");
		if (component != 0)
		{
			const MMFF94OutOfPlaneBend* ocomponent = dynamic_cast<const MMFF94OutOfPlaneBend*>(component);
			if (ocomponent!= 0)
			{
				return ocomponent->getEnergy();
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
		if (component != 0) return component->getEnergy();

		return 0;
	}

	bool MMFF94::hasInitializedParameters() const
	{
		return parameters_initialized_;
	}

	String MMFF94::getResults() const
		
	{
		String result = String("\n")
		+ "MMFF94 Energy:\n";

		result += String(" - bends             : ") +String(getBendEnergy())+  " kJ/mol\n" ;
		result += String(" - stretches         : ") +String(getStretchEnergy())+  " kJ/mol\n";
		result += String(" - stretchbends      : ") +String(getStretchBendEnergy())+  " kJ/mol\n";
		result += String(" - out of plane      : ") +String(getPlaneEnergy())+  " kJ/mol\n";
		result += String(" - torsions          : ") +String(getTorsionEnergy())+  " kJ/mol\n";
		result += String(" - van der Waals     : ") +String(getVdWEnergy())+  " kJ/mol\n";
		result += String(" - electrostatics    : ") +String(getESEnergy())+  " kJ/mol\n";
		result = result + "---------------------------------------\n" 
										+ "  total energy       : " +String(getEnergy()) + " kJ/mol\n";
		return result;
	}
	

	bool MMFF94::assignMMFF94BondType(Bond& bond) const
	{
		static MMFF94StretchParameters::BondData data;
		MMFF94StretchParameters::StretchMap::ConstIterator it;
		if (!bond_parameters_.assignParameters(bond.getFirstAtom()->getType(),
																				bond.getSecondAtom()->getType(), data))
		{
			return false;
		}

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
			if (bond.getOrder() == Bond::ORDER__SINGLE)
			{
				const Atom& atom1 = *(Atom*)bond.getFirstAtom();
				const Atom& atom2 = *(Atom*)bond.getSecondAtom();

				// are both atoms aromatic?
				bool atom1_is_aromatic = false;
				for (Position pos = 0; pos < aromatic_rings_.size(); pos++)
				{
					if (aromatic_rings_[pos].has((Atom*)&atom1)) 
					{
						atom1_is_aromatic = true;
						break;
					}
				}

				bool atom2_is_aromatic = false;
				for (Position pos = 0; pos < aromatic_rings_.size(); pos++)
				{
					if (aromatic_rings_[pos].has((Atom*)&atom2)) 
					{
						atom2_is_aromatic = true;
						break;
					}
				}

				// ok, this bond can be a sbmb or a standard bond
				
				// it is set to a non-standard bond type, iff
				//   (a)  it is a single bond (we checked that already)
				//     (b1) between non-aromatic atoms
				//     (c1) both of which have sbmb in their type definition
				// or  
				//     (b2) between aromatic atoms
				//     (c2) in different aromatic rings
				
				if (!(atom1_is_aromatic || atom2_is_aromatic))
				{
					if (    atom_types_.getAtomTypes()[atom1.getType()].sbmb 
							 && atom_types_.getAtomTypes()[atom2.getType()].sbmb)
					{
						is_sbmb = true;
					}
				}
				else if (atom1_is_aromatic && atom2_is_aromatic)
				{
					if (!isInOneAromaticRing(bond))
					{
						is_sbmb = true;
					}
				}
#ifdef BALL_DEBUG_MMFF
				Log.info() << atom1.getName() << " " << atom2.getName() << "  order single: "
					<< (bond.getOrder() == Bond::ORDER__SINGLE) << "  sp: " 
					<< !isInOneAromaticRing(bond) << std::endl;
#endif
			}
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
		return aromatic_bonds_.has((Bond*)&bond);
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

	void MMFF94::transformAromaticBonds_()
	{
		HashSet<Bond*> xbonds;
		typedef HashMap<Atom*, Size> AtomMap;
		AtomMap atom_map;
		AtomMap::Iterator it;

		// collect all unspecified and aromatic bonds
		vector<Bond*>::iterator bit = bonds_.begin();
		for (; bit != bonds_.end(); bit++)
		{
			// ??? is it ok to let the aromatic ring bonds keep their aromatic type?
			if (isInOneAromaticRing(**bit)) continue;

			if ((**bit).getOrder() < Bond::ORDER__SINGLE ||
					(**bit).getOrder() > Bond::ORDER__QUADRUPLE)
			{
				xbonds.insert(*bit);
				Atom* atom1 = (Atom*)(**bit).getFirstAtom();
				Atom* atom2 = (Atom*)(**bit).getFirstAtom();
				it = atom_map.find(atom1);
				if (it != atom_map.end())
				{
					it->second ++;
				}
				else
				{
					atom_map[atom1] = 1;
				}

				it = atom_map.find(atom2);
				if (it != atom_map.end())
				{
					it->second ++;
				}
				else
				{
					atom_map[atom2] = 1;
				}
			}
		}

		// if an atom has two aromatic bonds, make one a single and one a double bond
		vector<Bond*> bonds;
		it = atom_map.begin();
		for (; it != atom_map.end(); ++it)
		{
			if (it->second < 2) continue;

			// collect the aromatic bonds of one atom
			bonds.clear();
			AtomBondIterator abit = it->first->beginBond();
			for (; +abit; ++abit)
			{
				if (abit->getOrder() == Bond::ORDER__AROMATIC)
				{
					bonds.push_back(&*abit);
				}
			}

			// set the new bond orders
			Position p = 0;
			Index i = -1;
			for (; p < bonds.size(); p++)
			{
				if (i == -1) bonds[p]->setOrder(Bond::ORDER__SINGLE);
				else 				 bonds[p]->setOrder(Bond::ORDER__DOUBLE);
				i *= -1;
				xbonds.erase(bonds[p]);
			}
		}

		// set all remaining bonds to single bonds
		HashSet<Bond*>::Iterator hit = xbonds.begin();
		for (; +hit; ++hit)
		{
			(**hit).setOrder(Bond::ORDER__SINGLE);
		}
	}


	void MMFF94::assignBondTypes_()
	{
		vector<Bond*>::iterator bit = bonds_.begin();
		bit = bonds_.begin();
		for (; bit != bonds_.end(); bit++)
		{
			assignMMFF94BondType(**bit);
		}
	}
		
	
	void MMFF94::collectRings_()
	{
		///////////////////////////////////////
		/// calculate all rings in the molecule
		aromatic_bonds_.clear();
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

		// store all aromatic bonds
		for (Position p = 0; p < bonds_.size(); p++)
		{
			Bond& bond = *bonds_[p];

			Atom* atom1 = (Atom*) bond.getFirstAtom();
			Atom* atom2 = (Atom*) bond.getSecondAtom();

			for (Position pos = 0; pos < aromatic_rings_.size(); pos++)
			{
				if (aromatic_rings_[pos].has(atom1) &&
						aromatic_rings_[pos].has(atom2))
				{
					aromatic_bonds_.insert(&bond);
				}
			}
		}
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
			error() << "Invalid atom type: " 
							<< atom.getFullName(Atom::ADD_RESIDUE_ID) 
							<< " " << atom_type << std::endl;
			return false;
		}

		return true;
	}

} // namespace BALL
