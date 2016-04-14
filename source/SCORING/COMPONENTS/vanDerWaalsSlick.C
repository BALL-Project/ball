// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: vanDerWaals.C,v 1.4 2006/05/27 09:05:23 anker Exp $


#include <BALL/common.h>
#include <BALL/SCORING/COMPONENTS/vanDerWaalsSlick.h>
#include <BALL/MOLMEC/COMMON/assignTypes.h>
#include <BALL/MOLMEC/COMMON/support.h>
#include <BALL/MOLMEC/PARAMETER/templates.h>
#include <BALL/SYSTEM/path.h>

// define a macro for the square function
#define SQR(x) ((x) * (x))

#ifdef BALL_HAS_INLINE_TPL_ARGS
# define BALL_TPL_ARG_INLINE inline
#else
# define BALL_TPL_ARG_INLINE
#endif

// #define DEBUG 1

namespace BALL
{


	const String VanDerWaalsSlick::Option::VERBOSITY = "verbosity";
	const String VanDerWaalsSlick::Option::VDW_METHOD = "van_der_Waals_method";
	const String VanDerWaalsSlick::Option::VDW_CUT_ON = "van_der_Waals_cut_on";
	const String VanDerWaalsSlick::Option::VDW_CUT_OFF = "van_der_Waals_cut_off";
	const String VanDerWaalsSlick::Option::VDW_SOFTENING_LIMIT = "van_der_Waals_softening_limit";
	const String VanDerWaalsSlick::Option::LENNARD_JONES_FILE = "lennard_jones_file";

	const Size VanDerWaalsSlick::Default::VERBOSITY = 0;
	const Size VanDerWaalsSlick::Default::VDW_METHOD = CALCULATION__FULL_LJ_POTENTIAL;
	const float VanDerWaalsSlick::Default::VDW_CUT_ON = 13.0f;
	const float VanDerWaalsSlick::Default::VDW_CUT_OFF = 15.0f;
	// This option is only in effect if the method supports it
	const float VanDerWaalsSlick::Default::VDW_SOFTENING_LIMIT = 5.0f;
	const String VanDerWaalsSlick::Default::LENNARD_JONES_FILE = "Amber/amber94gly.ini";


	VanDerWaalsSlick::VanDerWaalsSlick()
		:	ScoringComponent()
	{
		// Set the name of this component
		setName("vanDerWaalsSlick");
		// ???
	}


	VanDerWaalsSlick::VanDerWaalsSlick(ScoringComponent& vdw)
		: ScoringComponent(vdw)
	{
		// Set the name of this component
		setName("vanDerWaalsSlick");
		// ???
	}


	VanDerWaalsSlick::VanDerWaalsSlick(ScoringFunction& sf)
		: ScoringComponent(sf)
	{
		// Set the name of this component
		setName("vanDerWaalsSlick");
		// ???
	}


	VanDerWaalsSlick::~VanDerWaalsSlick()
	{
		clear();
	}


	void VanDerWaalsSlick::clear()
	{
		non_bonded_.clear();
	}


	struct SwitchingCutOnOff
	{
		float cutoff_2;
		float cuton_2;
		float inverse_distance_off_on_3;
	};


	bool VanDerWaalsSlick::setup()
	{
		if (getScoringFunction() == 0)
		{
			Log.error() << "VanDerWaalsSlick::setup(): Not bound to a ScoringFunction"
				<< std::endl;
			return(false);
		}

		if (getScoringFunction()->getReceptor() == 0
				|| getScoringFunction()->getLigand() == 0)
		{
			Log.error() << "VanDerWaalsSlick::setup(): Receptor or ligand missing"
				<< std::endl;
			return(false);
		}

		Options options = getScoringFunction()->getOptions();

		// Set the options for this component from the options stored in the
		// ScoringFunction
		String lj_file_name
			= options.setDefault(VanDerWaalsSlick::Option::LENNARD_JONES_FILE,
					VanDerWaalsSlick::Default::LENNARD_JONES_FILE);
		calculation_method_
			= options.setDefaultInteger(VanDerWaalsSlick::Option::VDW_METHOD,
					VanDerWaalsSlick::Default::VDW_METHOD);
		cut_off_vdw_
			= options.setDefaultReal(VanDerWaalsSlick::Option::VDW_CUT_OFF,
					VanDerWaalsSlick::Default::VDW_CUT_OFF);
		cut_on_vdw_
			= options.setDefaultReal(VanDerWaalsSlick::Option::VDW_CUT_ON,
					VanDerWaalsSlick::Default::VDW_CUT_ON);
		scaling_vdw_1_4_ = 2.0;
		softening_limit_
			= options.setDefaultReal(VanDerWaalsSlick::Option::VDW_SOFTENING_LIMIT,
					VanDerWaalsSlick::Default::VDW_SOFTENING_LIMIT);
		verbosity_ = options.setDefaultInteger(VanDerWaalsSlick::Option::VERBOSITY,
				VanDerWaalsSlick::Default::VERBOSITY);

		// Copy Receptor and ligand into a system for later reference
		vdw_receptor_
			= new Molecule(*(getScoringFunction()->getReceptor()), true);
		vdw_system_.insert(*vdw_receptor_);
		vdw_ligand_
			= new Molecule(*(getScoringFunction()->getLigand()), true);
		vdw_system_.insert(*vdw_ligand_);

		// Find the parameter file
		Path path;
		String tmp = path.find(lj_file_name);
		if (tmp != "") lj_file_name = tmp;

		// Read parameters and initialise them
		ForceFieldParameters parameters(lj_file_name);
		parameters.init();

		tmp = path.find("Amber/amber94.types");
		if (tmp == "") tmp = "Amber/amber94.types";
		AssignTypeNameProcessor assign_type_names(tmp, false);
		vdw_system_.apply(assign_type_names);

		AssignTypeProcessor type_proc(parameters.getAtomTypes());
		vdw_system_.apply(type_proc);

		Templates templates;
		templates.extractSection(parameters, "ChargesAndTypeNames");
		// Assign force field parameters without overwriting existing types
		templates.assign(vdw_system_, true, false);

		// Save unassigned atoms for creating meaningful error reports
		HashSet<const Atom*>::ConstIterator unassigned_it
			= type_proc.getUnassignedAtoms().begin();
		for (; unassigned_it != type_proc.getUnassignedAtoms().end();
				unassigned_it++)
		{
			getScoringFunction()->getUnassignedAtoms().insert(*unassigned_it);
		}

		// HashSet<const Atom*>::ConstIterator unassigned_it
		unassigned_it
			= templates.getUnassignedAtoms().begin();
		for (; unassigned_it != templates.getUnassignedAtoms().end(); unassigned_it++)
		{
			if (verbosity_ > 1)
			{
				Log.warn() << "VanDerWaalsSlick::setup(): unassigned atom "
					<< (*unassigned_it)->getFullName() << " with type "
					<< (*unassigned_it)->getTypeName()
					<<  " (" << (*unassigned_it)->getType() << ")"
					<< std::endl;
			}
			getScoringFunction()->getUnassignedAtoms().insert(*unassigned_it);
		}

		// Read the parameters for the VDW calculation
		bool result = lennard_jones_.extractSection(parameters, "LennardJones");
		if (result == false)
		{
			return(false);
		}
		result = hydrogen_bond_.extractSection(parameters, "HydrogenBonds");
		if (result == false)
		{
			return(false);
		}

		if (verbosity_ > 20)
		{
			AtomIterator it = vdw_system_.beginAtom();
			for (; +it; ++it)
			{
				Log.info() << "type of "
					<< it->getFullName() << " is "
					<< it->getTypeName() <<  " (" << it->getType() << ")"
					<< std::endl;
			}
		}

		return(true);
	}


	Size VanDerWaalsSlick::createNonBondedList_(const ForceField::PairVector& atom_pair_vector)
	{
		// The following piece of code is stolen and adapted from
		// AmberNonBonded::buildVectorOfNonBondedAtomPairs
		non_bonded_.clear();
		non_bonded_.reserve(atom_pair_vector.size());
		is_hydrogen_bond_.clear();
		is_hydrogen_bond_.reserve(atom_pair_vector.size());

		vector<Position> non_torsions;
		non_torsions.reserve(atom_pair_vector.size());

		LennardJones::Data lj_tmp;
		Atom*	atom1;
		Atom* atom2;
		Atom::Type	type_atom1;
		Atom::Type  type_atom2;

		// Iterate and search torsions, fill the atom pairs that have a torsion
		// in non_bonded_
		for (Position i = 0; i < (Size)atom_pair_vector.size(); ++i)
		{
			atom1 = atom_pair_vector[i].first;
			atom2 = atom_pair_vector[i].second;
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
				lj_tmp.atom1 = atom1;
				lj_tmp.atom2 = atom2;

				if (!lennard_jones_.assignParameters(lj_tmp.values, type_atom1, type_atom2))
				{
					// hydrogen bond parameters are assigned later - do nothing!
					if (!hydrogen_bond_.hasParameters(type_atom1, type_atom2))
					{
						Log.error() << "VanDerWaalsSlick::setup(): "
							<< "cannot find vdw parameters for types "
							<< atom1->getTypeName() << "-" << atom2->getTypeName()
							<< " (" << atom1->getFullName() << "-"
							<< atom2->getFullName() << ")" << std::endl;

						lj_tmp.values.A = 0;
						lj_tmp.values.B = 0;

						getScoringFunction()->getUnassignedAtoms().insert(atom1);
						getScoringFunction()->getUnassignedAtoms().insert(atom2);
					}
				}

				non_bonded_.push_back(lj_tmp);
			}
		}

		// Determine and set the number of 1-4 interactions (torsions)
		number_of_1_4_ = (Size)non_bonded_.size();

		// Iterate and search non torsions, fill them in the vector non_bonded_
		for (Position i = 0; i < (Size)non_torsions.size(); ++i)
		{
			atom1 = atom_pair_vector[non_torsions[i]].first;
			atom2 = atom_pair_vector[non_torsions[i]].second;

			type_atom1 = atom1->getType();
			type_atom2 = atom2->getType();
			lj_tmp.atom1 = atom1;
			lj_tmp.atom2 = atom2;

			if (lennard_jones_.hasParameters(type_atom1, type_atom2))
			{
				lennard_jones_.assignParameters(lj_tmp.values, type_atom1, type_atom2);
			#ifdef DEBUGDEFUNCT
				std::cout << "Assigning: " << type_atom1 << "/" << type_atom2
					<< " --> A = " << lj_tmp.values.A << ", B = " << lj_tmp.values.B
					<< std::endl;
			#endif
			}
			else
			{
				Log.error() << "AmberNonBonded::setup(): "
				<< "cannot find Lennard Jones parameters for types "
				<< " (" << atom1->getFullName() << "-" << atom2->getFullName() << ")"
				<< std::endl;

				lj_tmp.atom1 = atom1;
				lj_tmp.atom2 = atom2;
				lj_tmp.values.A = 0;
				lj_tmp.values.B = 0;
			}

			non_bonded_.push_back(lj_tmp);
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
			bool is_hydrogen_bond = hydrogen_bond_.hasParameters(type_atom1,
			type_atom2);
			if (is_hydrogen_bond)
			{
				// OK, it's an H-bond pair. Retrieve its parameters and assign
				// them.
				hydrogen_bond_.assignParameters(values, type_atom1, type_atom2);
				non_bonded_[i].values.A = values.A;
				non_bonded_[i].values.B = values.B;

				// Note this as an H-bond.
				number_of_h_bonds_++;

				// ...and swap it to the end of the pair list.
				first_h_bond--;
				std::swap(non_bonded_[i], non_bonded_[first_h_bond]);
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

		return(getScoringFunction()->getUnassignedAtoms().size());
	}


	// This is the standard version of the Lennard-Jones potential.
	//
	// BALL_TPL_ARG_INLINE float vdwSixTwelve(float inverse_square_distance,
	float vdwSixTwelve(float inverse_square_distance,
			float A, float B, float /* limit */)
	{
		float inv_dist_6(inverse_square_distance
				* inverse_square_distance * inverse_square_distance);

#ifdef DEBUG
		/*
		std::cout << "S: ir6 = " << inv_dist_6
			<< ", dist = " << sqrt(1.0f/inverse_square_distance)
			<< ", lim = " << pow(A/B, 1.0f/6.0f)
			<< ", A = " << A << ", B = " << B
			<< std::endl;
			*/
		float e = (inv_dist_6 * (inv_dist_6 * A - B));
		std::cout << "e = " << e << std::endl;
		if (fabs(e) > 100.0f)
		{
			std::cout << "ACHTUNG!" << std::endl;
		}

#endif

		return (inv_dist_6 * (inv_dist_6 * A - B));
	}


	// This is the simple softened version of the Lennard-Jones potential.
	// This function will simply return a constant (defined by
	// softening_limit) if the energy should rise above that constant value.
	//
	// BALL_TPL_ARG_INLINE float vdwSixTwelve(float inverse_square_distance,
	BALL_TPL_ARG_INLINE float vdwSixTwelveSoftSimple(float inverse_square_distance,
			float A, float B, float limit)
	{
		float inv_dist_6(inverse_square_distance
				* inverse_square_distance * inverse_square_distance);

#ifdef DEBUG
		/*
		std::cout << "S: ir6 = " << inv_dist_6
			<< ", dist = " << sqrt(1.0f/inverse_square_distance)
			<< ", lim = " << pow(A/B, 1.0f/6.0f)
			<< ", A = " << A << ", B = " << B
			<< std::endl;
		*/
		float e = (inv_dist_6 * (inv_dist_6 * A - B));
		std::cout << "e = " << e << std::endl;
		if (fabs(e) > 100.0f)
		{
			std::cout << "ACHTUNG!" << std::endl;
		}

#endif

		float energy = inv_dist_6 * (inv_dist_6 * A - B);
		//  if (energy > limit) energy = limit;
		if (energy > limit) energy = limit;
		return (energy);
	}

	// This is the somewhat more sophisitcated softened version of the
	// Lennard-Jones potential.
	// If the energy contribution of one atom pair is above the softening
	// limit, it returns the value of softening limit plus the logarithm of
	// the energy.
	//
	// BALL_TPL_ARG_INLINE float vdwSixTwelve(float inverse_square_distance,
	BALL_TPL_ARG_INLINE float vdwSixTwelveSoftLog(float inverse_square_distance,
			float A, float B, float limit)
	{
		float inv_dist_6(inverse_square_distance
				* inverse_square_distance * inverse_square_distance);

#ifdef DEBUG
		/*
		std::cout << "S: ir6 = " << inv_dist_6
			<< ", dist = " << sqrt(1.0f/inverse_square_distance)
			<< ", lim = " << pow(A/B, 1.0f/6.0f)
			<< ", A = " << A << ", B = " << B
			<< std::endl;
		*/
		float e = (inv_dist_6 * (inv_dist_6 * A - B));
		std::cout << "e = " << e << std::endl;
		if (fabs(e) > 100.0f)
		{
			std::cout << "ACHTUNG!" << std::endl;
		}

#endif

		float energy = inv_dist_6 * (inv_dist_6 * A - B);
		//  if (energy > limit) energy = limit;
		if (energy > limit) energy = limit + log(energy);
		return (energy);
	}


  BALL_TPL_ARG_INLINE float vdwTenTwelve(float inverse_square_distance,
			float A, float B, float /* limit */)
	{
		float inv_dist_10 = inverse_square_distance *
			inverse_square_distance;
		inv_dist_10 *= inv_dist_10 * inverse_square_distance;
		return (inv_dist_10 * (inverse_square_distance * A - B));
	}


  BALL_TPL_ARG_INLINE float vdwTenTwelveSoftSimple(float inverse_square_distance,
			float A, float B, float limit)
	{
		float inv_dist_10 = inverse_square_distance *
			inverse_square_distance;
		inv_dist_10 *= inv_dist_10 * inverse_square_distance;
		float energy = inv_dist_10 * (inverse_square_distance * A - B);
		if (energy > limit) energy = limit;
		return(energy);
	}


  BALL_TPL_ARG_INLINE float vdwTenTwelveSoftLog(float inverse_square_distance,
			float A, float B, float limit)
	{
		float inv_dist_10 = inverse_square_distance *
			inverse_square_distance;
		inv_dist_10 *= inv_dist_10 * inverse_square_distance;
		float energy = inv_dist_10 * (inverse_square_distance * A - B);
		if (energy > limit) energy = limit + log(energy);
		return(energy);
	}


	typedef float (*VdwEnergyFunction) (float square_dist, float A, float B,
			float limit);
	typedef float (*SwitchingFunction) (double square_distance,
			const SwitchingCutOnOff& cutoffs);

	template <VdwEnergyFunction VdwEnergy, SwitchingFunction Switch>
	BALL_INLINE void AmberVDWEnergy
		(LennardJones::Data* ptr, LennardJones::Data* end_ptr,
		double& vdw_energy, const SwitchingCutOnOff& switching_vdw,
		float softening_limit)
	{
		// iterate over all pairs
		for (; ptr != end_ptr; ++ptr)
		{
			// compute the square distance
			double square_distance(ptr->atom1->getPosition().getSquareDistance(ptr->atom2->getPosition()));
			double inverse_square_distance(1.0 / square_distance);

			vdw_energy += VdwEnergy(inverse_square_distance, ptr->values.A,
					ptr->values.B, softening_limit)
				* Switch(square_distance, switching_vdw);
		}
	}


	// ??? There seems to be something wrong with this switching function.

  // BALL_TPL_ARG_INLINE float cubicSwitch(double square_distance,
  /*float cubicSwitch(double square_distance,
			const SwitchingCutOnOff& cutoffs)
  {
    float below_off = ((square_distance < cutoffs.cutoff_2) ? 1.0 : 0.0);
    float below_on = ((square_distance < cutoffs.cuton_2) ? 1.0 : 0.0);
		return below_off * (below_on + (1.0 - below_on)
				* SQR(cutoffs.cutoff_2 - square_distance)
				* (cutoffs.cutoff_2 + 2.0 * square_distance - 3.0 * cutoffs.cuton_2)
				* cutoffs.inverse_distance_off_on_3);
	}
*/

	float noSwitch(double /* square_distance */, const SwitchingCutOnOff& /* cutoffs */)
	{
		return 1.0f;
	}


	double VanDerWaalsSlick::calculateVDWEnergy_(const AtomVector& atom_vector)

	{
		// NOTE: The following is NOT the AMBER definition of van der Waals.
		// This is the 6-12 interaction of non-bonded atoms ONLY, there are no
		// hydrogen bonds and no 1-4 terms.

		// Calculate all non bonded atom pairs
		// Brute force algorithm from MolmecSupport without cutoff
		ForceField::PairVector atom_pair_vector;
		MolmecSupport::calculateNonBondedAtomPairs(atom_pair_vector,
				atom_vector, SimpleBox3(), 20.0f, false,
				MolmecSupport::BRUTE_FORCE);

		// Size no_unassigned_atoms = createNonBondedList_(atom_pair_vector);
		createNonBondedList_(atom_pair_vector);

		double cut_off_vdw_2 = SQR(cut_off_vdw_);
		double cut_on_vdw_2 = SQR(cut_on_vdw_);
		double inverse_distance_off_on_vdw_3 = cut_off_vdw_2 - cut_on_vdw_2;
		inverse_distance_off_on_vdw_3 *= SQR(inverse_distance_off_on_vdw_3);

		SwitchingCutOnOff cutoffs_vdw
			= { (float)cut_off_vdw_2, (float)cut_on_vdw_2, (float)inverse_distance_off_on_vdw_3 };

		double vdw_energy_1_4 = 0.0;
		double vdw_energy = 0.0;
		double hbond_energy = 0.0;

		// ??? Disabled switching for the moment, because there seems to some
		// error. Using s/noSwitch/cubicSwitch/ will turn it on again.
		if (calculation_method_ == CALCULATION__FULL_LJ_POTENTIAL)
		{
		AmberVDWEnergy<vdwSixTwelve, noSwitch>
			(&non_bonded_[0],
			 &non_bonded_[number_of_1_4_],
			 vdw_energy_1_4, cutoffs_vdw, softening_limit_);
		AmberVDWEnergy<vdwSixTwelve, noSwitch>
			(&non_bonded_[number_of_1_4_],
			 &non_bonded_[non_bonded_.size() - number_of_h_bonds_],
			 vdw_energy, cutoffs_vdw, softening_limit_);
		AmberVDWEnergy<vdwTenTwelve, noSwitch>
			(&non_bonded_[non_bonded_.size() - number_of_h_bonds_],
			 &non_bonded_[non_bonded_.size()],
			 hbond_energy, cutoffs_vdw, softening_limit_);
		}
		else
		{
			if (calculation_method_ == CALCULATION__SOFTENED_LJ_POTENTIAL_SIMPLE)
			{
				AmberVDWEnergy<vdwSixTwelveSoftSimple, noSwitch>
					(&non_bonded_[0],
					 &non_bonded_[number_of_1_4_],
					 vdw_energy_1_4, cutoffs_vdw, softening_limit_);
				AmberVDWEnergy<vdwSixTwelveSoftSimple, noSwitch>
					(&non_bonded_[number_of_1_4_],
					 &non_bonded_[non_bonded_.size() - number_of_h_bonds_],
					 vdw_energy, cutoffs_vdw, softening_limit_);
				AmberVDWEnergy<vdwTenTwelveSoftSimple, noSwitch>
					(&non_bonded_[non_bonded_.size() - number_of_h_bonds_],
					 &non_bonded_[non_bonded_.size()],
					 hbond_energy, cutoffs_vdw, softening_limit_);
			}
			else
			{
				if (calculation_method_ == CALCULATION__SOFTENED_LJ_POTENTIAL_LOG)
				{
					AmberVDWEnergy<vdwSixTwelveSoftLog, noSwitch>
						(&non_bonded_[0],
						 &non_bonded_[number_of_1_4_],
						 vdw_energy_1_4, cutoffs_vdw, softening_limit_);
					AmberVDWEnergy<vdwSixTwelveSoftLog, noSwitch>
						(&non_bonded_[number_of_1_4_],
						 &non_bonded_[non_bonded_.size() - number_of_h_bonds_],
						 vdw_energy, cutoffs_vdw, softening_limit_);
					AmberVDWEnergy<vdwTenTwelveSoftLog, noSwitch>
						(&non_bonded_[non_bonded_.size() - number_of_h_bonds_],
						 &non_bonded_[non_bonded_.size()],
						 hbond_energy, cutoffs_vdw, softening_limit_);
				}
				else
				{
					Log.error() << "Unknown calculation method for VDW model"
						<< std::endl;
				}
			}
		}

		double energy = scaling_vdw_1_4_ * vdw_energy_1_4 + vdw_energy
			+ hbond_energy;

#ifdef DEBUG
		std::cout << "S: " << scaling_vdw_1_4_ * vdw_energy_1_4 << " + "
			<< vdw_energy << " + " << hbond_energy << " = " << energy << std::endl;
#endif


		return(energy);
	}


	void VanDerWaalsSlick::update(const vector<std::pair<Atom*, Atom*> >& pair_vector)
	{
	}


	double VanDerWaalsSlick::updateScore()
	{
		// Because we have local copies, we need to update the atom postition
		// for our molecules.
		AtomConstIterator src = getScoringFunction()->getReceptor()->beginAtom();
		AtomIterator dst = vdw_receptor_->beginAtom();
		// This for-loop assumes that both systems are still of same size and
		// that atoms are still in the same order. No checking done on this!
		for (; +src && +dst; ++src, ++dst)
		{
			dst->setPosition(src->getPosition());
		}

		src = getScoringFunction()->getLigand()->beginAtom();
		dst = vdw_ligand_->beginAtom();
		for (; +src && +dst; ++src, ++dst)
		{
			dst->setPosition(src->getPosition());
		}


		// Collect atoms
		AtomVector atom_vector;

		AtomIterator it = vdw_system_.beginAtom();
		for (; +it; ++it) atom_vector.push_back(&*it);

		double complex_energy = calculateVDWEnergy_(atom_vector);
		if (verbosity_ > 1)
		{
			std::cout << "VDW energy of complex: " << complex_energy << std::endl;
		}

		// Receptor:

		// Collect atoms
		atom_vector.clear();

		it = vdw_receptor_->beginAtom();
		for (; +it; ++it) atom_vector.push_back(&*it);

		double receptor_energy = calculateVDWEnergy_(atom_vector);
		if (verbosity_ > 1)
		{
			Log.info() << "VDW energy of receptor: " << receptor_energy << std::endl;
		}

		// Ligand:

		// Collect atoms
		atom_vector.clear();

		it = vdw_ligand_->beginAtom();
		for (; +it; ++it) atom_vector.push_back(&*it);

		double ligand_energy = calculateVDWEnergy_(atom_vector);
		if (verbosity_ > 1)
		{
			Log.info() << "VDW energy of ligand: " << ligand_energy << std::endl;
		}

		score_ = complex_energy - (receptor_energy + ligand_energy);

		if (verbosity_ > 1)
		{
			Log.info() << "VanDerWaalsSlick::calculateScore(): score_ is "
				<< score_ << std::endl;
		}

		return score_;
	}
}
