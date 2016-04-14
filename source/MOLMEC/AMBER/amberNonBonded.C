// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/MOLMEC/AMBER/amberNonBonded.h>
#include <BALL/MOLMEC/AMBER/amber.h>
#include <BALL/MOLMEC/COMMON/forceField.h>
#include <BALL/MOLMEC/COMMON/support.h>
#include <BALL/SCORING/COMPONENTS/advElectrostatic.h>
#include <BALL/SYSTEM/path.h>

using namespace std;

// define a macro for the square function
#define SQR(x) ((x) * (x))

#ifdef BALL_HAS_INLINE_TPL_ARGS
#	define BALL_TPL_ARG_INLINE inline
#else
#	define BALL_TPL_ARG_INLINE
#endif

namespace BALL 
{
	bool store_interactions = false;
	AdvancedElectrostatic* advanced_electrostatic = 0;
	using namespace Constants;
	const double AmberNonBonded::ELECTROSTATIC_FACTOR
		= NA * e0 * e0 * 1e7 / (4.0 * PI * VACUUM_PERMITTIVITY);

	// default constructor
	AmberNonBonded::AmberNonBonded()
		
		:	ForceFieldComponent(),
			electrostatic_energy_(0.0),
			vdw_energy_(0.0),
			non_bonded_(),
			is_hydrogen_bond_(),
			number_of_1_4_(0),
			number_of_h_bonds_(0),
			cut_off_(0.0),
			cut_off_vdw_(0.0),
			cut_on_vdw_(0.0),
			cut_off_electrostatic_(0.0),
			cut_on_electrostatic_(0.0),
			inverse_distance_off_on_vdw_3_(0.0),
			inverse_distance_off_on_electrostatic_3_(0.0),
			scaling_vdw_1_4_(0.0),
			scaling_electrostatic_1_4_(0.0),
			use_dist_depend_dielectric_(false),
			algorithm_type_(MolmecSupport::BRUTE_FORCE),
			van_der_waals_(),
			hydrogen_bond_()
	{	
		// set component name
		setName("Amber NonBonded");
	}


	// constructor
	AmberNonBonded::AmberNonBonded(ForceField& force_field)
		
		:	ForceFieldComponent(force_field),
			electrostatic_energy_(0.0),
			vdw_energy_(0.0),
			non_bonded_(),
			is_hydrogen_bond_(),
			number_of_1_4_(0),
			number_of_h_bonds_(0),
			cut_off_(0.0),
			cut_off_vdw_(0.0),
			cut_on_vdw_(0.0),
			cut_off_electrostatic_(0.0),
			cut_on_electrostatic_(0.0),
			inverse_distance_off_on_vdw_3_(0.0),
			inverse_distance_off_on_electrostatic_3_(0.0),
			scaling_vdw_1_4_(0.0),
			scaling_electrostatic_1_4_(0.0),
			use_dist_depend_dielectric_(false),
			algorithm_type_(MolmecSupport::BRUTE_FORCE),
			van_der_waals_(),
			hydrogen_bond_()
	{
		// set component name
		setName("Amber NonBonded");
	}


	// copy constructor
	AmberNonBonded::AmberNonBonded(const AmberNonBonded& component)
		
		:	ForceFieldComponent(component),
			electrostatic_energy_(component.electrostatic_energy_),
			vdw_energy_(component.vdw_energy_),
			non_bonded_(component.non_bonded_),
			is_hydrogen_bond_(component.is_hydrogen_bond_),
			number_of_1_4_(component.number_of_1_4_),
			number_of_h_bonds_(component.number_of_h_bonds_),
			cut_off_(component.cut_off_),
			cut_off_vdw_(component.cut_off_vdw_),
			cut_on_vdw_(component.cut_on_vdw_),
			cut_off_electrostatic_(component.cut_off_electrostatic_),
			cut_on_electrostatic_(component.cut_on_electrostatic_),
			inverse_distance_off_on_vdw_3_(component.inverse_distance_off_on_vdw_3_),
			inverse_distance_off_on_electrostatic_3_(component.inverse_distance_off_on_electrostatic_3_),
			scaling_vdw_1_4_(component.scaling_vdw_1_4_),
			scaling_electrostatic_1_4_(component.scaling_electrostatic_1_4_),
			use_dist_depend_dielectric_(component.use_dist_depend_dielectric_),
			algorithm_type_(component.algorithm_type_),
			van_der_waals_(component.van_der_waals_),
			hydrogen_bond_(component.hydrogen_bond_)
	{
	}


	// destructor
	AmberNonBonded::~AmberNonBonded()
		
	{
		clear();
	}

	const AmberNonBonded& AmberNonBonded::operator = (const AmberNonBonded& anb)
		
	{
		// catch self assignment
		if (&anb == this)
		{
			return *this;
		}

		// call the base class operator
		ForceFieldComponent::operator = (anb);

		// copy members
		electrostatic_energy_ = anb.electrostatic_energy_;
		vdw_energy_ = anb.vdw_energy_;
		non_bonded_ = anb.non_bonded_;
		is_hydrogen_bond_ = anb.is_hydrogen_bond_;
		number_of_1_4_ = anb.number_of_1_4_;
		number_of_h_bonds_ = anb.number_of_h_bonds_;
		cut_off_ = anb.cut_off_;
		cut_off_vdw_ = anb.cut_off_vdw_;
		cut_on_vdw_ = anb.cut_on_vdw_;
		cut_off_electrostatic_ = anb.cut_off_electrostatic_;
		cut_on_electrostatic_ = anb.cut_on_electrostatic_;
		inverse_distance_off_on_vdw_3_ = anb.inverse_distance_off_on_vdw_3_;
		inverse_distance_off_on_electrostatic_3_ = anb.inverse_distance_off_on_electrostatic_3_;
		scaling_vdw_1_4_ = anb.scaling_vdw_1_4_;
		scaling_electrostatic_1_4_ = anb.scaling_electrostatic_1_4_;
		use_dist_depend_dielectric_ = anb.use_dist_depend_dielectric_;
		algorithm_type_ = anb.algorithm_type_;
		van_der_waals_ = anb.van_der_waals_;
		hydrogen_bond_ = anb.hydrogen_bond_;

		return *this;
	}

	void AmberNonBonded::clear()
		
	{
		electrostatic_energy_ = 0.0;
		vdw_energy_ = 0.0;
		non_bonded_.clear();
		is_hydrogen_bond_.clear();
		number_of_1_4_ = 0;
		number_of_h_bonds_ = 0;
		cut_off_ = 0.0;
		cut_off_vdw_ = 0.0;
		cut_on_vdw_ = 0.0;
		cut_off_electrostatic_ = 0.0;
		cut_on_electrostatic_ = 0.0;
		inverse_distance_off_on_vdw_3_ = 0.0;
		inverse_distance_off_on_electrostatic_3_ = 0.0;
		scaling_vdw_1_4_ = 0.0;
		scaling_electrostatic_1_4_ = 0.0;
		use_dist_depend_dielectric_ = false;
		algorithm_type_ = MolmecSupport::BRUTE_FORCE;
		van_der_waals_.clear();
		hydrogen_bond_.clear();
	}


	bool AmberNonBonded::operator == (const AmberNonBonded& anb)
		
	{
		return (this == &anb);
	}

	// This function determines the most efficient way to calculate all
	// non-bonded atom pairs that depends on the number of atoms of the
	// system. The function return value 0 if the number of atoms is so
	// small that the brute force all against all comparison is the most
	// efficient way. Otherwise it returns 1. 
	MolmecSupport::PairListAlgorithmType
	AmberNonBonded::determineMethodOfAtomPairGeneration()
		
	{
		if (force_field_->getAtoms().size() > 900) 
		{ 
			return MolmecSupport::HASH_GRID;
		} 
		else
		{
			return MolmecSupport::BRUTE_FORCE;
		}
	}

	void AmberNonBonded::update()
		throw(Exception::TooManyErrors)
	{
		if (getForceField() == 0) 
		{
			Log.error() << "AmberNonBonded::update(): component not bound to a force field" << endl;
			return;
		}

		// Calculate all non bonded atom pairs
		ForceField::PairVector atom_pair_vector;

		MolmecSupport::calculateNonBondedAtomPairs
			(atom_pair_vector, getForceField()->getAtoms(), 
			 getForceField()->periodic_boundary.getBox(),
			 cut_off_, force_field_->periodic_boundary.isEnabled(), 
			 algorithm_type_);

		if (getForceField()->getSystem()->containsSelection())
		{
			// eliminate all those pairs where none of the two atoms is selected
			Size number_of_selected_pairs = MolmecSupport::sortNonBondedAtomPairsAfterSelection(atom_pair_vector);
			atom_pair_vector.resize(number_of_selected_pairs);
		}

		// Build the vector "non_bonded_" with the atom pairs and parameters
		buildVectorOfNonBondedAtomPairs(atom_pair_vector, van_der_waals_, hydrogen_bond_);
	}

	bool AmberNonBonded::setup(Options& options, ForceFieldParameters& parameters)
		{
			// clear vector of non-bonded atom pairs
			clear();

			// the cutoffs for the nonbonded pair list and the switching function
			// for vdW and electrostatics
			cut_off_ = options.setDefaultReal(AmberFF::Option::NONBONDED_CUTOFF, AmberFF::Default::NONBONDED_CUTOFF);
			cut_off_electrostatic_ = options.setDefaultReal(AmberFF::Option::ELECTROSTATIC_CUTOFF, AmberFF::Default::ELECTROSTATIC_CUTOFF);
			cut_off_vdw_ = options.setDefaultReal(AmberFF::Option::VDW_CUTOFF, AmberFF::Default::VDW_CUTOFF);
			cut_on_electrostatic_ = options.setDefaultReal(AmberFF::Option::ELECTROSTATIC_CUTON, AmberFF::Default::ELECTROSTATIC_CUTON);
			cut_on_vdw_ = options.setDefaultReal(AmberFF::Option::VDW_CUTON, AmberFF::Default::VDW_CUTON);

			inverse_distance_off_on_vdw_3_ = SQR(cut_off_vdw_) - SQR(cut_on_vdw_);
			inverse_distance_off_on_electrostatic_3_ = SQR(cut_off_electrostatic_) - SQR(cut_on_electrostatic_);

			inverse_distance_off_on_vdw_3_ *= SQR(inverse_distance_off_on_vdw_3_);
			inverse_distance_off_on_electrostatic_3_ *= SQR(inverse_distance_off_on_electrostatic_3_);

			if (inverse_distance_off_on_vdw_3_ <= 0.0)
			{
				Log.warn() << "AmberNonBonded::setup(): "
						<< "vdW cuton value should be smaller than cutoff -- "
						<< "switching function disabled." << endl;
				cut_on_vdw_ = cut_off_vdw_ + 1.0;
			}
			else
			{
				inverse_distance_off_on_vdw_3_ = 1.0 / inverse_distance_off_on_vdw_3_;
			}
			if (inverse_distance_off_on_electrostatic_3_ <= 0.0)
			{
				Log.warn() << "AmberNonBonded::setup(): "
						<< "electrostatic cuton value should be smaller than cutoff." << endl
						<< "Switching function disabled." << endl;
				cut_on_electrostatic_ = cut_off_electrostatic_ + 1.0;
			}
			else
			{
				inverse_distance_off_on_electrostatic_3_ = 1.0 / inverse_distance_off_on_electrostatic_3_;
			}

			scaling_electrostatic_1_4_
					= options.setDefaultReal(AmberFF::Option::SCALING_ELECTROSTATIC_1_4,
					AmberFF::Default::SCALING_ELECTROSTATIC_1_4);
			if (scaling_electrostatic_1_4_ == 0.0)
			{
				Log.warn() << "AmberNonBonded::setup(): "
						<< "illegal - 1-4-electrostatic scaling factor must be non-zero!"
						<< endl << "Resetting to 1.0." << endl;
				scaling_electrostatic_1_4_ = 1.0;
			}
			else
			{
				scaling_electrostatic_1_4_ = 1.0 / scaling_electrostatic_1_4_;
			}

			scaling_vdw_1_4_
					= options.setDefaultReal(AmberFF::Option::SCALING_VDW_1_4,
					AmberFF::Default::SCALING_VDW_1_4);
			if (scaling_vdw_1_4_ == 0.0)
			{
				Log.warn() << "AmberNonBonded::setup(): "
						<< "illegal - 1-4-vdW scaling factor must be non-zero!" << endl
						<< "Resetting to 1.0." << endl;
				scaling_vdw_1_4_ = 1.0;
			}
			else
			{
				scaling_vdw_1_4_ = 1.0 / scaling_vdw_1_4_;
			}

			 // set the option for using a constant dielectric constant (default) or
			// or distance dependent one
			use_dist_depend_dielectric_
					= options.setDefaultBool(AmberFF::Option::DISTANCE_DEPENDENT_DIELECTRIC,
					AmberFF::Default::DISTANCE_DEPENDENT_DIELECTRIC);

			// check whether the parameter file name
			// is set in the options
			string file = AmberFF::Default::FILENAME;
			if (options.has(AmberFF::Option::FILENAME))
			{
				file = options[AmberFF::Option::FILENAME];
				setName("Amber [" + file + "]");
			}
			else
			{
				options[AmberFF::Option::FILENAME] = AmberFF::Default::FILENAME;
			}

			// open parameter file
			Path    path;
			String  filename(path.find(file));

			if (filename == "")
			{
				throw Exception::FileNotFound(__FILE__, __LINE__, filename);
			}

			// initialize the force field parameters
			// and retrieve the atom types
			if (parameters.getFilename() != filename)
			{
				parameters.setFilename(filename);
				parameters.init();
			}

			bool result = van_der_waals_.extractSection(parameters, "LennardJones");
			if (!result)
			{
				Log.error() << "AmberNonBonded::setup(): "
						<< "cannot find section LennardJones in " << getForceField()->getParameters().getFilename() << endl;
				return false;
			}

			result = hydrogen_bond_.extractSection(parameters, "HydrogenBonds");
			if (!result)
			{
				Log.error() << "AmberNonBonded::setup(): "
						<< "cannot find section HydrogenBonds in " << getForceField()->getParameters().getFilename() << endl;
				return false;
			}

			return true;
		}

	// setup the internal datastructures for the component
	bool AmberNonBonded::setup()
		throw(Exception::TooManyErrors)
	{
		if (getForceField() == 0) 
		{
			Log.error() << "AmberNonBonded::setup(): component not bound to a force field" << endl;
			return false;
		}

		// clear vector of non-bonded atom pairs
		clear();
 
 		Options& options = getForceField()->options;
		if (options.has(AMBER_NB_ENABLED))
		{
			if (!options.getBool(AMBER_NB_ENABLED))
			{
				setEnabled(false);
				return true;
			}
			else
			{
				setEnabled(true);
			}
		}

		// the cutoffs for the nonbonded pair list and the switching function
		// for vdW and electrostatics
		cut_off_ = options.setDefaultReal(AmberFF::Option::NONBONDED_CUTOFF, AmberFF::Default::NONBONDED_CUTOFF);
		cut_off_electrostatic_ = options.setDefaultReal(AmberFF::Option::ELECTROSTATIC_CUTOFF, AmberFF::Default::ELECTROSTATIC_CUTOFF);
		cut_off_vdw_ = options.setDefaultReal(AmberFF::Option::VDW_CUTOFF, AmberFF::Default::VDW_CUTOFF);
		cut_on_electrostatic_ = options.setDefaultReal(AmberFF::Option::ELECTROSTATIC_CUTON, AmberFF::Default::ELECTROSTATIC_CUTON);
		cut_on_vdw_ = options.setDefaultReal(AmberFF::Option::VDW_CUTON, AmberFF::Default::VDW_CUTON);

		// when using periodic boundary conditions, all
		// cutoffs must be smaller than the smallest linear extension of
		// the box - we use the minimum image convention!
		if (getForceField()->periodic_boundary.isEnabled())
		{
			SimpleBox3 box = getForceField()->periodic_boundary.getBox();
			float max_cut_off = 0.5 * Maths::min(box.getWidth(), box.getHeight(), box.getDepth());

			if (cut_off_electrostatic_ > max_cut_off)
			{
				Log.error() << "AmberNonBonded::setup(): "
										<< "electrostatic cutoff may not exceed half" << endl
										<< "the box dimension when using periodic boundary conditions!" << endl;
				return false;
			}
			if (cut_off_vdw_ > max_cut_off)
			{
				Log.error() << "AmberNonBonded::setup(): "
										<< "vdW cutoff may not exceed half" << endl
										<< "the box dimension when using periodic boundary conditions!" << endl;
				return false;
			}
		}
				
    inverse_distance_off_on_vdw_3_ = SQR(cut_off_vdw_) - SQR(cut_on_vdw_);
    inverse_distance_off_on_electrostatic_3_ = SQR(cut_off_electrostatic_) - SQR(cut_on_electrostatic_);

    inverse_distance_off_on_vdw_3_ *= SQR(inverse_distance_off_on_vdw_3_);
    inverse_distance_off_on_electrostatic_3_ *= SQR(inverse_distance_off_on_electrostatic_3_);

    if (inverse_distance_off_on_vdw_3_ <= 0.0)
    {
      Log.warn() << "AmberNonBonded::setup(): "
								 << "vdW cuton value should be smaller than cutoff -- "
								 << "switching function disabled." << endl;
      cut_on_vdw_ = cut_off_vdw_ + 1.0;
		}
    else
    {
      inverse_distance_off_on_vdw_3_ = 1.0 / inverse_distance_off_on_vdw_3_;
		}
    if (inverse_distance_off_on_electrostatic_3_ <= 0.0)
    {
      Log.warn() << "AmberNonBonded::setup(): "
								 << "electrostatic cuton value should be smaller than cutoff." << endl
								 << "Switching function disabled." << endl;
      cut_on_electrostatic_ = cut_off_electrostatic_ + 1.0;
		}
    else
    {
      inverse_distance_off_on_electrostatic_3_ = 1.0 / inverse_distance_off_on_electrostatic_3_;
		}
 
		scaling_electrostatic_1_4_ 
			= options.setDefaultReal(AmberFF::Option::SCALING_ELECTROSTATIC_1_4,
					AmberFF::Default::SCALING_ELECTROSTATIC_1_4);
		if (scaling_electrostatic_1_4_ == 0.0)
		{
			Log.warn() << "AmberNonBonded::setup(): "
									<< "illegal - 1-4-electrostatic scaling factor must be non-zero!"
									<< endl << "Resetting to 1.0." << endl;
			scaling_electrostatic_1_4_ = 1.0;
		}
		else 
		{
			scaling_electrostatic_1_4_ = 1.0 / scaling_electrostatic_1_4_;			
		}

		scaling_vdw_1_4_ 
			= options.setDefaultReal(AmberFF::Option::SCALING_VDW_1_4,
					AmberFF::Default::SCALING_VDW_1_4);
		if (scaling_vdw_1_4_ == 0.0)
		{
			Log.warn() << "AmberNonBonded::setup(): "
				<< "illegal - 1-4-vdW scaling factor must be non-zero!" << endl
				<< "Resetting to 1.0." << endl;
			scaling_vdw_1_4_ = 1.0;
		}
		else 
		{
			scaling_vdw_1_4_ = 1.0 / scaling_vdw_1_4_;			
		}

    // set the option for using a constant dielectric constant (default) or
    // or distance dependent one 
		use_dist_depend_dielectric_ 
			= options.setDefaultBool(AmberFF::Option::DISTANCE_DEPENDENT_DIELECTRIC,
					AmberFF::Default::DISTANCE_DEPENDENT_DIELECTRIC);

		// extract the Lennard-Jones parameters
		AmberFF* amber_force_field = dynamic_cast<AmberFF*>(force_field_);
		bool has_initialized_parameters = false;
		if ((amber_force_field != 0) && (amber_force_field->hasInitializedParameters()))
		{
			has_initialized_parameters = true;
		}
			
		if (!has_initialized_parameters)
		{
			bool result = van_der_waals_.extractSection(getForceField()->getParameters(), "LennardJones");

			if (!result) 
			{	
				Log.error() << "AmberNonBonded::setup(): "
					<< "cannot find section LennardJones in " << getForceField()->getParameters().getFilename() << endl;
				return false;
			}
		}

		// extract the hydrogen bond parameters
		if (!has_initialized_parameters)
		{
			bool result = hydrogen_bond_.extractSection(getForceField()->getParameters(), "HydrogenBonds");

			if (!result) 
			{
				Log.error() << "AmberNonBonded::setup(): "
					<< "cannot find section HydrogenBonds in " << getForceField()->getParameters().getFilename() << endl;
				return false;
			}
		}

		// Determine the most efficient way to calculate all non bonded atom pairs
		algorithm_type_ = determineMethodOfAtomPairGeneration();

		// build the nonbonded pairs
		update();

		return true;
	}

	void AmberNonBonded::update(const vector<pair<Atom*, Atom*> >& atom_vector)
	{
		buildVectorOfNonBondedAtomPairs(atom_vector, van_der_waals_, hydrogen_bond_);
	}

	// Build a vector of non-bonded atom pairs with the vdw parameters 
	// The vector starts with 1-4 interactions
	void AmberNonBonded::buildVectorOfNonBondedAtomPairs
		(const vector<pair<Atom*, Atom*> >& atom_vector,
		 const LennardJones& lennard_jones, 
		 const Potential1210& hydrogen_bond)
		throw(Exception::TooManyErrors)
	{
		// throw away the old rubbish
		non_bonded_.clear();
		is_hydrogen_bond_.clear();

		// resize non_bonded_ if necessary
		if (non_bonded_.capacity() < atom_vector.size())
		{
			// reserve the required size plus 20% 
			// to avoid frequent resizing)
			non_bonded_.reserve((Size)((double)atom_vector.size() * 1.2));
			is_hydrogen_bond_.reserve(non_bonded_.capacity());
		}

		// Iterate over all atom pairs in atom_vector and test if the atoms
		// are part of a torsion
		vector<Position> non_torsions;
		non_torsions.reserve(atom_vector.size());

		LennardJones::Data tmp;
		Atom*	atom1;
		Atom* atom2;
		Atom::Type	type_atom1;
		Atom::Type  type_atom2;

		// Iterate and search torsions, fill the atom pairs that have a torsion
		// in non_bonded_
		for (Position i = 0; i < (Size)atom_vector.size(); ++i) 
		{
			atom1 = atom_vector[i].first;
			atom2 = atom_vector[i].second;
			if (!atom1->isVicinal(*atom2)) 
			{
				// store the non-torsions for later appending in the non_torsions
				// vector
				non_torsions.push_back(i);
			}
			else
			{
				type_atom1 = atom1->getType();
				type_atom2 = atom2->getType();
				tmp.atom1 = atom1;
				tmp.atom2 = atom2;

				if (!lennard_jones.assignParameters(tmp.values, type_atom1, type_atom2)) 
				{
					// hydrogen bond parameters are assigned later - do nothing!
					if (!hydrogen_bond.hasParameters(type_atom1, type_atom2))
					{
						getForceField()->error() << "AmberNonBonded::setup(): "
												<< "cannot find vdw parameters for types "
												<< atom1->getTypeName() << "-" << atom2->getTypeName() 
												<< " (" << atom1->getFullName() << "-" << atom2->getFullName() << ")" << endl;
						tmp.values.A = 0;
						tmp.values.B = 0;

						getForceField()->getUnassignedAtoms().insert(atom1);
						getForceField()->getUnassignedAtoms().insert(atom2);
					}
				}

				non_bonded_.push_back(tmp);
			}
		}

		// Determine and set the number of 1-4 interactions (torsions)
		number_of_1_4_ = (Size)non_bonded_.size();

		// Iterate and search non torsions, fill them in the vector non_bonded_
		for (Position i = 0; i < (Size)non_torsions.size(); ++i) 
		{
			atom1 = atom_vector[non_torsions[i]].first;
			atom2 = atom_vector[non_torsions[i]].second;

			type_atom1 = atom1->getType();
			type_atom2 = atom2->getType();
			tmp.atom1 = atom1;
			tmp.atom2 = atom2;

			if (lennard_jones.hasParameters(type_atom1, type_atom2)) 
			{
				lennard_jones.assignParameters(tmp.values, type_atom1, type_atom2);
			} 
			else 
			{
				Log.error() << "AmberNonBonded::setup(): "
					<< "cannot find Lennard Jones parameters for types "
					<< getForceField()->getParameters().getAtomTypes().getTypeName(type_atom1) 
					<< "-"
					<< getForceField()->getParameters().getAtomTypes().getTypeName(type_atom2) 
					<< " (" << atom1->getFullName() << "-" << atom2->getFullName() << ")"
					<< endl;
				tmp.atom1 = atom1;
				tmp.atom2 = atom2;
				tmp.values.A = 0;
				tmp.values.B = 0;
			}
			non_bonded_.push_back(tmp);
		}

		// now check for hydrogen bonds
		// parameters for hydrogen bonds are used, if they exist
		// and the two atoms are not vicinal (1-4).
		// We make sure that the H-bond parameters are all at the
		// end of the pair list.
		Potential1210::Values values;
		number_of_h_bonds_ = 0;
		Position first_h_bond = non_bonded_.size(); 
		for (Position i = number_of_1_4_; i < first_h_bond; )
		{
			// Retrieve the two atom types...
			type_atom1 = non_bonded_[i].atom1->getType();
			type_atom2 = non_bonded_[i].atom2->getType();

			// and figure out whether we have suitable H-bond parameters.
			bool is_hydrogen_bond = hydrogen_bond.hasParameters(type_atom1,	type_atom2);	
			if (is_hydrogen_bond)
			{
				// OK, it's an H-bond pair. Retrieve its parameters and assign them.
				hydrogen_bond.assignParameters(values, type_atom1, type_atom2);
				non_bonded_[i].values.A = values.A;
				non_bonded_[i].values.B = values.B;

				// Note this as an H-bond.
				number_of_h_bonds_++;

				// ...and swap it to the end of the pair list.
				first_h_bond--;
				swap(non_bonded_[i], non_bonded_[first_h_bond]);				
			}	
			else
			{
				// No H-bond, get the next pair.
				is_hydrogen_bond_.push_back(false);
				i++;
			}
		}		
		// Fill the is_hydrogen_bond_ vector with the reamining pairs
		// (H-bonds only)
		for (Position i = first_h_bond; i < non_bonded_.size(); i++)
		{
			is_hydrogen_bond_.push_back(true);
		}
	}

	// Determine the closest pair of images under cubic periodic 
	// boundary conditions.
	BALL_INLINE
	void AMBERcalculateMinimumImage(Vector3& difference, const Vector3& period)
		
	{
		// half the period of the cell
		float half_period_x = period.x * 0.5;
		float half_period_y = period.y * 0.5;
		float half_period_z = period.z * 0.5;

		if (difference.x <= -half_period_x)
		{
			difference.x += period.x;
		}
		else if (difference.x > half_period_x)
		{
			difference.x -= period.x;
		}

		if (difference.y <= -half_period_y)
		{
			difference.y += period.y;
		}
		else if (difference.y > half_period_y)
		{
			difference.y -= period.y;
		}

		if (difference.z <= -half_period_z)
		{
			difference.z += period.z;
		}
		else if (difference.z > half_period_z)
		{
			difference.z -= period.z;
		}
	}

	struct SwitchingCutOnOff
	{
		float cutoff_2;
		float cuton_2;
		float inverse_distance_off_on_3;
	};

	struct NBStruct
	{
		Atom* atom1;
		Atom* atom2;
		float charge_product;
		float A;
		float B;
	};
		
	BALL_TPL_ARG_INLINE float distanceDependentCoulomb(float inverse_square_distance, float charge_product)
	{
		return charge_product * inverse_square_distance;
	}

	BALL_TPL_ARG_INLINE float coulomb(float inverse_square_distance, float charge_product)
	{
		return charge_product * sqrt(inverse_square_distance);
	}

	BALL_TPL_ARG_INLINE float vdwSixTwelve(float inverse_square_distance, float A, float B)
	{
		float inv_dist_6(inverse_square_distance * inverse_square_distance * inverse_square_distance);
		return (inv_dist_6 * (inv_dist_6 * A - B)); 
	}

	BALL_TPL_ARG_INLINE float vdwTenTwelve(float inverse_square_distance, float A, float B)
	{
		float inv_dist_10 = inverse_square_distance * inverse_square_distance;
		inv_dist_10 *= inv_dist_10 * inverse_square_distance;
		return (inv_dist_10 * (inverse_square_distance * A - B)); 
	}

	typedef float (*ESEnergyFunction) (float square_dist, float q1_q2);
	typedef float (*VdwEnergyFunction) (float square_dist, float A, float B);
	typedef float (*SwitchingFunction) (double square_distance, const SwitchingCutOnOff& cutoffs);

	template <ESEnergyFunction ESEnergy, 
						VdwEnergyFunction VdwEnergy,
						SwitchingFunction Switch>
	BALL_INLINE void AmberNBEnergy
		(LennardJones::Data* ptr, LennardJones::Data* end_ptr, 
		 double& es_energy, double& vdw_energy, 
		 const SwitchingCutOnOff& switching_es, const SwitchingCutOnOff& switching_vdw)
	{
		/* original BALL code (before CADDSuite merging)
		// iterate over all pairs
		for (; ptr != end_ptr; ++ptr)
		{
			// compute the square distance
			double square_distance(ptr->atom1->getPosition().getSquareDistance(ptr->atom2->getPosition()));
			double inverse_square_distance(1.0 / square_distance);
			es_energy += ESEnergy(inverse_square_distance, ptr->atom1->getCharge() * ptr->atom2->getCharge()) * Switch(square_distance, switching_es);
			vdw_energy += VdwEnergy(inverse_square_distance, ptr->values.A, ptr->values.B) * Switch(square_distance, switching_vdw);
		}
		*/
		for (; ptr != end_ptr; ++ptr)
		{
			// compute the square distance
			double square_distance(ptr->atom1->getPosition().getSquareDistance(ptr->atom2->getPosition()));
			double inverse_square_distance(1.0 / square_distance);
			double es_p = ESEnergy(inverse_square_distance, ptr->atom1->getCharge() * ptr->atom2->getCharge()) * Switch(square_distance, switching_es);
			double vdw_p = VdwEnergy(inverse_square_distance, ptr->values.A, ptr->values.B) * Switch(square_distance, switching_vdw);

			if(advanced_electrostatic)
			{
				es_p /= advanced_electrostatic->calculateDielectricConstant(ptr->atom1->getPosition(),ptr->atom2->getPosition());
			}
			if(store_interactions)
			{
				double vdw_scaled = vdw_p*0.1;
				ptr->atom1->addInteraction(ptr->atom2,"vdW",vdw_scaled);
				ptr->atom2->addInteraction(ptr->atom1,"vdW",vdw_scaled);

				double es_scaled = AmberNonBonded::ELECTROSTATIC_FACTOR * es_p;
				es_scaled *= 0.01;
				ptr->atom1->addInteraction(ptr->atom2,"ES",es_scaled);
				ptr->atom2->addInteraction(ptr->atom1,"ES",es_scaled);
			}

			es_energy += es_p;
			vdw_energy += vdw_p;
		}
	}

	
	BALL_TPL_ARG_INLINE float cubicSwitch(double square_distance, const SwitchingCutOnOff& cutoffs)
	{
		float below_off = ((square_distance < cutoffs.cutoff_2) ? 1.0 : 0.0);
		float below_on = ((square_distance < cutoffs.cuton_2) ? 1.0 : 0.0);
		return below_off * (below_on + (1.0 - below_on) * SQR(cutoffs.cutoff_2 - square_distance) 
						* (cutoffs.cutoff_2 + 2.0 * square_distance - 3.0 * cutoffs.cuton_2)
						* cutoffs.inverse_distance_off_on_3);
	}

	template <ESEnergyFunction ESEnergyFct, 
						VdwEnergyFunction VdwEnergyFct,
						SwitchingFunction SwitchFct>
	BALL_INLINE 
	void AmberNBEnergyPeriodic
		(LennardJones::Data* ptr, LennardJones::Data* end_ptr, 
		 double& es_energy, double& vdw_energy, 
		 SwitchingCutOnOff es_switching, SwitchingCutOnOff vdw_switching,
		 const Vector3& period)
	{
		/*
		// iterate over all pairs
		Vector3 difference;
		for (; ptr != end_ptr; ++ptr)
		{
			difference = ptr->atom1->getPosition() - ptr->atom2->getPosition();
			AMBERcalculateMinimumImage(difference, period);

			// compute the square distance and correct for periodic boundary if necessary
			double square_distance(difference.getSquareLength());
			double inverse_square_distance(1.0 / square_distance);

			es_energy += ESEnergyFct(inverse_square_distance, ptr->atom1->getCharge() * ptr->atom2->getCharge()) * SwitchFct(square_distance, es_switching);
			vdw_energy += VdwEnergyFct(inverse_square_distance, ptr->values.A, ptr->values.B) * SwitchFct(square_distance, vdw_switching);
		}
		*/

		// iterate over all pairs
		Vector3 difference;
		for (; ptr != end_ptr; ++ptr)
		{
			difference = ptr->atom1->getPosition() - ptr->atom2->getPosition();
			AMBERcalculateMinimumImage(difference, period);

			// compute the square distance and correct for periodic boundary if necessary
			double square_distance(difference.getSquareLength());
			double inverse_square_distance(1.0 / square_distance);

			double es_p = ESEnergyFct(inverse_square_distance, ptr->atom1->getCharge() * ptr->atom2->getCharge()) * SwitchFct(square_distance, es_switching);
			double vdw_p = VdwEnergyFct(inverse_square_distance, ptr->values.A, ptr->values.B) * SwitchFct(square_distance, vdw_switching);

			if(advanced_electrostatic)
			{
				es_p /= advanced_electrostatic->calculateDielectricConstant(ptr->atom1->getPosition(),ptr->atom2->getPosition());
			}
			if(store_interactions)
			{
				double vdw_scaled = vdw_p*0.1;
				ptr->atom1->addInteraction(ptr->atom2,"vdW",vdw_scaled);
				ptr->atom2->addInteraction(ptr->atom1,"vdW",vdw_scaled);

				// correct for the additional factor 1/4 in the distance dependent case
				double es_scaled = AmberNonBonded::ELECTROSTATIC_FACTOR*0.25 * es_p;
				es_scaled *= 0.01;
				ptr->atom1->addInteraction(ptr->atom2,"ES",es_scaled);
				ptr->atom2->addInteraction(ptr->atom1,"ES",es_scaled);
			}

			es_energy += es_p;
			vdw_energy += vdw_p;
		}
	}

	
	

	// This  function calculates the force vector
	// resulting from non-bonded interactions between two atoms 
	BALL_INLINE
	void AMBERcalculateNBForce
		(LennardJones::Data& LJ_data, 
		 Vector3& period,
		 const double cut_off_vdw_2, 
		 const double cut_on_vdw_2, 
		 const double inverse_distance_off_on_vdw_3,
		 const double cut_off_electrostatic_2,
		 const double cut_on_electrostatic_2, 
		 const double inverse_distance_off_on_electrostatic_3,
		 const double e_scaling_factor, 
		 const double vdw_scaling_factor, 
     bool is_hydrogen_bond, 
		 bool use_periodic_boundary, 
		 bool use_dist_depend,
		 bool use_selection)
		
	{
    // calculate the difference vector between the two atoms
		// useful aliases
		Atom* atom1 = LJ_data.atom1;
		Atom* atom2 = LJ_data.atom2;

    Vector3 direction(atom1->getPosition() - atom2->getPosition());

    // choose the nearest image if period boundary is enabled 
    if (use_periodic_boundary == true)
		{
			AMBERcalculateMinimumImage(direction, period); 
		}

    double distance_2 = direction.getSquareLength(); 

		// we multiply the normalized direction with this factor
		// to get the force for each atom
		double factor = 0.0;
		if (distance_2 != 0.0)
		{
			using namespace Constants;
			double inverse_distance_2 = 1 / distance_2;
			if (distance_2 <= cut_off_electrostatic_2) 
			{ 
				// the product of the charges
				double q1q2 = atom1->getCharge() * atom2->getCharge();
				factor = q1q2 * inverse_distance_2 * e_scaling_factor;
				// distinguish between constant and distance dependent dielectric 
				if (use_dist_depend)
				{
					// distance dependent dielectric:  epsilon = 4 * r_ij
					// 4 reduces to 2 (due to derivation of the energy)
					factor *= 0.5 * inverse_distance_2;
				} 
				else 
				{
					// distance independent dielectric constant
					factor *= sqrt(inverse_distance_2);
				}

				// we have to use the switching  function (cuton <= distance <= cutoff)
				if (distance_2 > cut_on_electrostatic_2)
				{

					// the switching function is defined as follows:
					//         (r_{off}^2 - R^2)^2 (r_{off}^2 + 2 R^2 - 3r_{on}^2)
					// sw(R) = ---------------------------------------------------
					//                    (r_{off}^2 - r_{on}^2)^3
					//
					// [Brooks et al., J. Comput. Chem., 4:191 (1983)]
					//
					// the derivative has the following form:
					//
					//                      (r_{off}^2 - R^2)(r_{on}^2 - R^2)
					//   d/dR sw(R) = 12 R -----------------------------------
					//                           (r_{off}^2 - r_{on}^2)^3
					//

					double difference_to_off = cut_off_electrostatic_2 - distance_2;
					double difference_to_on = cut_on_electrostatic_2 - distance_2;

					// First, multiply the current force with the switching function
					factor *= SQR(difference_to_off) 
									* (cut_off_electrostatic_2 
										 + 2.0 * distance_2 
										 - 3.0 * cut_on_electrostatic_2)
									* inverse_distance_off_on_electrostatic_3;

					// Second, we add the product of the energy and the derivative
					// of the switching function (the total force is the derivative of
					// a product of functions)
					// we save the multiplication by distance to avoid the normalization
					// of the direction vector
					// In fact, we calculate the negative energy, since we de not
					// calculate the force, but the derivative of the energy above.

					double derivative_of_switch 
						= 12.0
							* difference_to_off 
							* difference_to_on
							* inverse_distance_off_on_electrostatic_3;

					// calculate the electrostatic energy

					double inverse_distance = sqrt(inverse_distance_2);
					double dist_depend_factor = (double)(use_dist_depend == true);
					dist_depend_factor = 0.25 * inverse_distance * dist_depend_factor 
						+ (1.0 - dist_depend_factor);
					double electrostatic_energy = - e_scaling_factor * dist_depend_factor 
						* inverse_distance * q1q2;
					factor += derivative_of_switch * electrostatic_energy;
				}
			}

			if (distance_2 <= cut_off_vdw_2) 
			{
				double tmp = 1e13 / Constants::NA * inverse_distance_2;
				double inverse_distance_6 = inverse_distance_2 * inverse_distance_2 * inverse_distance_2;

				if (!is_hydrogen_bond)
				{
					// conversion from kJ/(mol*A) -> J/m
					//    1e3 (kJ -> J)
					//    1e10 (A -> m)
					//    1/NA (J/mol -> J)
					tmp *= inverse_distance_6 * vdw_scaling_factor 
							   * (12 * LJ_data.values.A * inverse_distance_6 - 6 * LJ_data.values.B);
				} 
				else 
				{
					double inverse_distance_12 = inverse_distance_6 * inverse_distance_6;
					// conversion from kJ/(mol*A) -> J/m
					//    1e3 (kJ -> J)
					//    1e10 (A -> m)
					//    1/NA (J/mol -> J)
					tmp *= inverse_distance_12 *  (12 * LJ_data.values.A * inverse_distance_2 - 10 * LJ_data.values.B); 
				}

				// we have to use the switching function (cuton <= distance <= cutoff)
				if (distance_2 > cut_on_vdw_2)
				{
					// the switching function is the same function as for
					// electrostatics (see above)
					double difference_to_off = cut_off_vdw_2 - distance_2;
					double difference_to_on = cut_on_vdw_2 - distance_2;

					// First, multiply the current force with the switching function
					tmp *= SQR(difference_to_off)
								* (cut_off_vdw_2 + 2.0 * distance_2 - 3.0 * cut_on_vdw_2)
								* inverse_distance_off_on_vdw_3;

					// Second, we add the product of the energy and the derivative
					// of the switching function (the total force is the derivative of
					// a product of functions)
					double derivative_of_switch = 12.0 * difference_to_off * difference_to_on
																				 * inverse_distance_off_on_vdw_3;


					// calculate the vdW or hydrogen bond energy
					double energy = - 1e13 / AVOGADRO * vdw_scaling_factor;
					if (!is_hydrogen_bond)
					{
						// calculate the vdW energy
						energy *= inverse_distance_6 * (inverse_distance_6 * LJ_data.values.A - LJ_data.values.B);
					}
					else 
					{
						// calculate the hydrogen bond energy
						energy *= SQR(inverse_distance_2) * inverse_distance_6 * (LJ_data.values.A * inverse_distance_2 - LJ_data.values.B );
					}
					tmp += derivative_of_switch * energy;
				}

				// add the vdW contributions to the force factor
				factor += tmp;
			}
		}

		// now apply the force to the atoms
		Vector3 force = (float)factor * direction; 

		if (!use_selection || atom1->isSelected()) 
		{
			atom1->getForce() += force;
		}
		if (!use_selection || atom2->isSelected())
		{
			atom2->getForce() -= force;
		}
	} // end of function 	AMBERcalculateNBForce()


	// Compute the non-bonded energy (i.e. electrostatic, vdW, and H-bonds)
	double AmberNonBonded::updateEnergy()
		
	{
		// Calculate squared cut_off values
		double cut_off_electrostatic_2 = SQR(cut_off_electrostatic_);
		double cut_off_vdw_2 = SQR(cut_off_vdw_);
		double cut_on_electrostatic_2 = SQR(cut_on_electrostatic_);
		double cut_on_vdw_2 = SQR(cut_on_vdw_);

		SwitchingCutOnOff cutoffs_es 
			= { (float)cut_off_electrostatic_2, (float)cut_on_electrostatic_2, (float)inverse_distance_off_on_electrostatic_3_ };
		SwitchingCutOnOff cutoffs_vdw 
			= { (float)cut_off_vdw_2, (float)cut_on_vdw_2, (float)inverse_distance_off_on_vdw_3_ };

		// Define the different components of the non-bonded energy
		double vdw_energy = 0.0;
		double vdw_energy_1_4 = 0.0;
		double hbond_energy = 0.0;
		double electrostatic_energy = 0.0;
		double electrostatic_energy_1_4 = 0.0;

		static Vector3 period;

		bool use_periodic_boundary = false;
		if (force_field_!=NULL)
		{
			use_periodic_boundary = force_field_->periodic_boundary.isEnabled();
		}

		if (use_periodic_boundary)
		{
			// calculate the box period (half of the box period)
			const SimpleBox3& box = force_field_->periodic_boundary.getBox();
			period = box.b - box.a;
		}

		// Compute the individual contributions to the non-bonded energy
		// (in that order):
		//  - all 1-4 interactions (vdW + electrostatics)
		//  - all 1-4 vdW pairs + electrostatics
		//  - all H-bonds and electrostatics
		//
		// The non-bonded pairs are stored in that order in the non-bonded vector.
		// We only have to call the templatized energy functions.
		// For the functional form, we have to consider four major cases,
		// depending on the presence of periodic boundary conditions
		// and the use of a distance-dependent dielectric constant.
		// The first results in the use of AmberNBEnergyPeriodic
		// instead of AmberNBEnergy, the latter in the use of distanceDependentCoulomb
		// instead of coulomb for the electrostatic energy.
		if (!non_bonded_.empty())
		{
			if (!use_periodic_boundary && !use_dist_depend_dielectric_)
			{
				// no periodic boundary, constant dielectric
					AmberNBEnergy<coulomb, vdwSixTwelve, cubicSwitch>
						(&non_bonded_[0], &non_bonded_[0] + number_of_1_4_, electrostatic_energy_1_4, vdw_energy_1_4,
						 cutoffs_es, cutoffs_vdw);
					AmberNBEnergy<coulomb, vdwSixTwelve, cubicSwitch>
						(&non_bonded_[0] + number_of_1_4_, &non_bonded_[0] + non_bonded_.size() - number_of_h_bonds_, electrostatic_energy, vdw_energy, 
						 cutoffs_es, cutoffs_vdw);
					AmberNBEnergy<coulomb, vdwTenTwelve, cubicSwitch>
						(&non_bonded_[0] + non_bonded_.size() - number_of_h_bonds_, &non_bonded_[0] + non_bonded_.size(), electrostatic_energy, hbond_energy, 
						 cutoffs_es, cutoffs_vdw);
			}
			else if (!use_periodic_boundary && use_dist_depend_dielectric_)
			{
				// no periodic boundary, distance-dependent dielectric constant
					AmberNBEnergy<distanceDependentCoulomb, vdwSixTwelve, cubicSwitch >
						(&non_bonded_[0], &non_bonded_[0] + number_of_1_4_, electrostatic_energy_1_4, vdw_energy_1_4,
						 cutoffs_es, cutoffs_vdw);
					AmberNBEnergy<distanceDependentCoulomb, vdwSixTwelve, cubicSwitch >
						(&non_bonded_[0] + number_of_1_4_, &non_bonded_[0] + non_bonded_.size() - number_of_h_bonds_, electrostatic_energy, vdw_energy, 
						 cutoffs_es, cutoffs_vdw);
					AmberNBEnergy<distanceDependentCoulomb, vdwTenTwelve, cubicSwitch >
						(&non_bonded_[0] + non_bonded_.size() - number_of_h_bonds_, &non_bonded_[0] + non_bonded_.size(), electrostatic_energy, hbond_energy, 
						 cutoffs_es, cutoffs_vdw);
			}
			else if (use_periodic_boundary && !use_dist_depend_dielectric_)
			{
				// periodic boundary, constant dielectric
					AmberNBEnergyPeriodic<coulomb, vdwSixTwelve, cubicSwitch >
						(&non_bonded_[0], &non_bonded_[0] + number_of_1_4_, electrostatic_energy_1_4, vdw_energy_1_4, 
						 cutoffs_es, cutoffs_vdw, period);
					AmberNBEnergyPeriodic<coulomb, vdwSixTwelve, cubicSwitch >
						(&non_bonded_[0] + number_of_1_4_, &non_bonded_[0] + non_bonded_.size() - number_of_h_bonds_, electrostatic_energy, vdw_energy, 
						 cutoffs_es, cutoffs_vdw, period);
					AmberNBEnergyPeriodic<coulomb, vdwTenTwelve, cubicSwitch >
						(&non_bonded_[0] + non_bonded_.size() - number_of_h_bonds_, &non_bonded_[0] + non_bonded_.size(), electrostatic_energy, hbond_energy, 
						 cutoffs_es, cutoffs_vdw, period);
			}
			else
			{
				// periodic boundary, distance-dependent dielectric constant
					AmberNBEnergyPeriodic<distanceDependentCoulomb, vdwSixTwelve, cubicSwitch >
						(&non_bonded_[0], &non_bonded_[0] + number_of_1_4_, electrostatic_energy_1_4, vdw_energy_1_4, 
						 cutoffs_es, cutoffs_vdw, period);
					AmberNBEnergyPeriodic<distanceDependentCoulomb, vdwSixTwelve, cubicSwitch >
						(&non_bonded_[0] + number_of_1_4_, &non_bonded_[0] + non_bonded_.size() - number_of_h_bonds_, electrostatic_energy, vdw_energy, 
						 cutoffs_es, cutoffs_vdw, period);
					AmberNBEnergyPeriodic<distanceDependentCoulomb, vdwTenTwelve, cubicSwitch >
						(&non_bonded_[0] + non_bonded_.size() - number_of_h_bonds_, &non_bonded_[0] + non_bonded_.size(), electrostatic_energy, hbond_energy, 
						 cutoffs_es, cutoffs_vdw, period);
			}
		}

		// calculate the total energy and its contributions
		vdw_energy_ = ((vdw_energy + hbond_energy) + scaling_vdw_1_4_ * vdw_energy_1_4);

		using namespace Constants;
		double electrostatic_factor 
						= NA * e0 * e0 * 1e7 / (4.0 * PI * VACUUM_PERMITTIVITY);

		if (use_dist_depend_dielectric_)
		{
			// correct for the additional factor 1/4 in the distance dependent case
			electrostatic_factor *= 0.25;
		}

		electrostatic_energy_ = electrostatic_factor * 
			(electrostatic_energy + scaling_electrostatic_1_4_ * electrostatic_energy_1_4);

		// sum up all energies
		energy_ =  vdw_energy_ + electrostatic_energy_;

		return energy_; 
  } // end of AmberNonBonded::updateEnergy 
	
	#define FORCE_PARAMETERS\
		period,\
		cut_off_vdw_2,\
		cut_on_vdw_2,\
		inverse_distance_off_on_vdw_3_,\
		cut_off_electrostatic_2,\
		cut_on_electrostatic_2,\
		inverse_distance_off_on_electrostatic_3_

	// This method AMBERcalculates the current forces resulting from
	// van-der-Waals and electrostatic interactions 
	void AmberNonBonded::updateForces()
		
	{
		if (getForceField() == 0)
		{
			return;
		}

		// Define variables for the squared cut_offs, the unit factors and so on
		double	cut_off_electrostatic_2 = SQR(cut_off_electrostatic_);
		double	cut_off_vdw_2 = SQR(cut_off_vdw_);
		double	cut_on_electrostatic_2 = SQR(cut_on_electrostatic_);
		double	cut_on_vdw_2 = SQR(cut_on_vdw_);

		// e_scaling_factor contains the unit conversions und the constants
		// appearing in Coulomb's law:
		//
		//               1        q1 * e0 * q2 * e0
		//     F = ------------- ------------------
		//         4 PI epsilon0       r * r
		//
		// Conversion factors are 1e-10 for Angstrom -> m
		// and Constants::e0 for the proton charge

		const double e_scaling_factor = Constants::e0 * Constants::e0 
																		/ (4 * Constants::PI * Constants::VACUUM_PERMITTIVITY * 1e-20); 
		const double e_scaling_factor_1_4 = e_scaling_factor * scaling_electrostatic_1_4_;
		const double vdw_scaling_factor = 1.0;
		double vdw_scaling_factor_1_4 = vdw_scaling_factor * scaling_vdw_1_4_;

		Size i;
		vector<LennardJones::Data>::iterator it;  
		Vector3 period; 

		bool use_periodic_boundary = force_field_->periodic_boundary.isEnabled(); 
		bool use_selection = getForceField()->getUseSelection() && getForceField()->getSystem()->containsSelection();

		// calculate forces arising from 1-4 interaction pairs
		// and remaining non-bonded interaction pairs

		if ((use_periodic_boundary == true) && (use_dist_depend_dielectric_ == true))
		{
			// periodic boundary is enabled; use a distance dependent dielectric
			// constant 

			// Calculate periods and half periods
			SimpleBox3 box = force_field_->periodic_boundary.getBox();
			period = box.b - box.a; 

			// first deal with 1-4 non-bonded pairs 
			for (i = 0, it = non_bonded_.begin(); i < number_of_1_4_; i++, ++it) 
			{
				AMBERcalculateNBForce
					(*it, FORCE_PARAMETERS,
					 e_scaling_factor_1_4, vdw_scaling_factor_1_4, false, true, true, use_selection);
			}

			// now deal with 'real' non-bonded pairs (in the same vector non_bonded_) 
			for (i = 0; it != non_bonded_.end(); i++, ++it) 
			{
				AMBERcalculateNBForce
					(*it, FORCE_PARAMETERS, e_scaling_factor, 
					 vdw_scaling_factor, (bool)(is_hydrogen_bond_[i] != 0), true, true, use_selection);
			}
		}
		else
		{
			if ((use_periodic_boundary == true)
					&& (use_dist_depend_dielectric_ == false))
			{
				// periodic boundary is enabled; use a distance dependent
				// dielectric constant 

				// Calculate periods and half periods
				SimpleBox3 box = force_field_->periodic_boundary.getBox();
				period = box.b - box.a; 

				// first deal with 1-4 non-bonded pairs
				for (i = 0, it = non_bonded_.begin(); i < number_of_1_4_; i++, ++it) 
				{
						AMBERcalculateNBForce
							(*it, FORCE_PARAMETERS, e_scaling_factor_1_4, 
							 vdw_scaling_factor_1_4, false, true, false, use_selection);
				}

				// now deal with 'real' non-bonded pairs (in the same vector
				// non_bonded_) 
				for (i = 0; it != non_bonded_.end(); i++, ++it) 
				{
					AMBERcalculateNBForce
						(*it, FORCE_PARAMETERS, e_scaling_factor, 
						 vdw_scaling_factor, (is_hydrogen_bond_[i] != 0), true, false, use_selection);
				}
			}
			else
			{
				if ((use_periodic_boundary == false) 
						&& (use_dist_depend_dielectric_ == true))
				{
					// periodic boundary not enabled; use a distance dependent
					// dielectric constant 

					// first deal with 1-4 non-bonded pairs
					for (i = 0, it = non_bonded_.begin(); i < number_of_1_4_; i++, ++it) 
					{
						AMBERcalculateNBForce
							(*it, FORCE_PARAMETERS, e_scaling_factor_1_4, 
							 vdw_scaling_factor_1_4, false, false, true, use_selection);
					}

					// now deal with 'real' non-bonded pairs (in the same vector
					// non_bonded_)
					for (i = 0; it != non_bonded_.end(); i++, ++it) 
					{
						AMBERcalculateNBForce
							(*it, FORCE_PARAMETERS, e_scaling_factor, 
							 vdw_scaling_factor, (is_hydrogen_bond_[i] != 0), false, true, use_selection);
					}
				}
				else
				{
					// periodic boundary is not enabled; use a constant dielectric 

					// first deal with 1-4 non-bonded pairs
					for (i = 0, it = non_bonded_.begin(); i < number_of_1_4_; i++, it++) 
					{
						AMBERcalculateNBForce
							(*it, FORCE_PARAMETERS, e_scaling_factor_1_4, 
							 vdw_scaling_factor_1_4, false, false, false, use_selection);
					}

					// now deal with 'real' non-bonded pairs (in the same vector
					// non_bonded_)
					for (i = 0; it != non_bonded_.end(); i++, ++it) 
					{
						AMBERcalculateNBForce
							(*it, FORCE_PARAMETERS, e_scaling_factor, 
							 vdw_scaling_factor, (is_hydrogen_bond_[i] != 0), false, false, use_selection);
					}
				}
			}
		}
	} // end of method AmberNonBonded::updateForces()

	double AmberNonBonded::getElectrostaticEnergy() const
		
	{
		return electrostatic_energy_;
	}

	double AmberNonBonded::getVdwEnergy() const
		
	{
		return vdw_energy_;
	}

	void AmberNonBonded::enableStoreInteractions(bool b)
	{
		store_interactions = b;
	}

	void AmberNonBonded::setAdvancedElectrostatic(AdvancedElectrostatic* advES)
	{
		advanced_electrostatic = advES;
	}
} // namespace BALL
