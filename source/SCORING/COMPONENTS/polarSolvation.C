// $Id: polarSolvation.C,v 1.3 2006/05/27 09:05:23 anker Exp $

#include <BALL/SCORING/COMPONENTS/polarSolvation.h>

#include <BALL/KERNEL/standardPredicates.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/atomIterator.h>
#include <BALL/STRUCTURE/defaultProcessors.h>
#include <BALL/STRUCTURE/geometricProperties.h>
#include <BALL/DATATYPE/hashMap.h>
#include <BALL/ENERGY/distanceCoulomb.h>
#include <BALL/SYSTEM/timer.h>

#include <BALL/FORMAT/PDBFile.h>

using namespace std;

namespace BALL
{

	//
	const char* PolarSolvation::Option::POLAR_METHOD
		= "polar_solvation_method";
	const char* PolarSolvation::Option::POLAR_GB
		= "polar_solvation_use_gb";
	const char* PolarSolvation::Option::POLAR_AVG
		= "polar_solvation_use_averaging";
	const char* PolarSolvation::Option::POLAR_FOCUS_GRID_AROUND_LIGAND
		= "polar_solvation_focus_grid_around_ligand";
	const char* PolarSolvation::Option::UNITE_ATOMS
		= "unite_atoms";
	const char* PolarSolvation::Option::VERBOSITY
		= "verbosity";
	const char* PolarSolvation::Option::GB_SCALING_FILE
		= "gb_scaling_file";
	const char* PolarSolvation::Option::POLAR_OVERWRITE_RADII
		= "polar_overwrite_radii";
	const char* PolarSolvation::Option::POLAR_OVERWRITE_CHARGES
		= "polar_overwrite_charges";
	const char* PolarSolvation::Option::POLAR_RADIUS_RULES
		= "polar_radius_rules";
	const char* PolarSolvation::Option::POLAR_CHARGE_RULES
		= "polar_charge_rules";

	//
	const Size PolarSolvation::Default::POLAR_METHOD
		= CALCULATION__FULL_CYCLE_FOCUSED;
	const bool PolarSolvation::Default::POLAR_GB
		= false;
	const Size PolarSolvation::Default::POLAR_AVG
		= AVERAGING__NONE;
	const bool PolarSolvation::Default::POLAR_FOCUS_GRID_AROUND_LIGAND
		= false;
	const bool PolarSolvation::Default::UNITE_ATOMS
		= false;
	const Size PolarSolvation::Default::VERBOSITY
		= 10;
	const String PolarSolvation::Default::GB_SCALING_FILE
		= "gb_scaling.ini";
	const bool PolarSolvation::Default::POLAR_OVERWRITE_RADII
		= true;
	const bool PolarSolvation::Default::POLAR_OVERWRITE_CHARGES
		= true;
	const String PolarSolvation::Default::POLAR_RADIUS_RULES
		= "solvation/PARSE+ions.rul";
	const String PolarSolvation::Default::POLAR_CHARGE_RULES
		= "solvation/PARSE+ions.rul";


	PolarSolvation::PolarSolvation()
		:	ScoringComponent(),
			calculation_method_(0),
			verbosity_(0),
			fdpb_(),
			gbm_(),
			bulk_water_dc_(0.0),
			vacuum_dc_(0.0)
	{
		// set component name
		setName("Polar Solvation");
	}


	PolarSolvation::PolarSolvation(ScoringFunction& sf)
		:	ScoringComponent(sf),
			calculation_method_(0),
			verbosity_(0),
			fdpb_(),
			gbm_(),
			bulk_water_dc_(0.0),
			vacuum_dc_(0.0)
	{
		// set component name
		setName("Polar Solvation");
	}


	PolarSolvation::PolarSolvation(const PolarSolvation& ps)
		:	ScoringComponent(ps),
			calculation_method_(0),
			verbosity_(0),
			fdpb_(),
			gbm_(),
			bulk_water_dc_(0.0),
			vacuum_dc_(0.0)
	{
	}


	PolarSolvation::~PolarSolvation()
	{
		clear();
	}


	void PolarSolvation::clear()
	{
		calculation_method_ = 0;
		verbosity_ = 0;
		// ?????
		// FDPB does not comply with the OCI
		// fdpb_.clear();
	}


	bool PolarSolvation::setup()
	{
		Timer timer;
		timer.start();

		if (getScoringFunction() == 0)
		{
			Log.error() << "PolarSolvation::setup(): "
				<< "component not bound to scoring function." << endl;
			return false;
		}

		Options* options = getScoringFunction()->getOptionsToModify();

		calculation_method_
			= options->setDefaultInteger(PolarSolvation::Option::POLAR_METHOD,
					PolarSolvation::Default::POLAR_METHOD);

		unite_atoms_
			= options->setDefaultBool(PolarSolvation::Option::UNITE_ATOMS,
					PolarSolvation::Default::UNITE_ATOMS);

		use_gb_
			= options->setDefaultBool(PolarSolvation::Option::POLAR_GB,
					PolarSolvation::Default::POLAR_GB);

		if (use_gb_)
		{
			gbm_.setScalingFactorFile((*options)[PolarSolvation::Option::GB_SCALING_FILE]);
		}

		/*

		disabled for now because focusing does not work this way...

		bool focus_grid_around_ligand
			= options.setDefaultBool(PolarSolvation::Option::POLAR_FOCUS_GRID_AROUND_LIGAND,
					PolarSolvation::Default::POLAR_FOCUS_GRID_AROUND_LIGAND);
		*/

		verbosity_
			= options->setDefaultInteger(PolarSolvation::Option::VERBOSITY,
					PolarSolvation::Default::VERBOSITY);

		verbosity_ = 4;

		averaging_
			= options->setDefaultInteger(PolarSolvation::Option::POLAR_AVG,
					PolarSolvation::Default::POLAR_AVG);

		// ????? hardcoded
		spacing_ = 0.5;
		//	= options.setDefaultInteger(PolarSolvation::Option::SPACING
		//			PolarSolvation::Default::SPACING);

		// maybe we should set this via options, but for now this will be
		// sufficient
		bulk_water_dc_ = 80.0;
		vacuum_dc_ = 1.0;

		if (verbosity_ > 1)
		{
			switch(calculation_method_)
			{
				case CALCULATION__FRESNO:
					if (verbosity_ > 1)
					{
						Log.info() << "Model " << calculation_method_
							<< ": original Fresno." << endl << endl;
						break;
					}

				case CALCULATION__FULL_SLICK:
					if (verbosity_ > 1)
					{
						Log.info() << "Model " << calculation_method_
							<< ": modified Fresno." << endl << endl;
						break;
					}

				case CALCULATION__FULL_CYCLE:
					if (verbosity_ > 1)
					{
						Log.info() << "Model " << calculation_method_
							<< ": full thermodynamic cycle." << endl << endl;
						break;
					}

				case CALCULATION__FULL_CYCLE_FOCUSED:
					if (verbosity_ > 1)
					{
						Log.info() << "Model " << calculation_method_
							<< ": full thermodynamic cycle with focused grid." << endl << endl;
						break;
					}

				case CALCULATION__COULOMB:
					if (verbosity_ > 1)
					{
						Log.info() << "Model " << calculation_method_
							<< ": simple Coulomb." << endl << endl;
						break;
					}

				case CALCULATION__EEF1:
					if (verbosity_ > 1)
					{
						Log.info() << "Model " << calculation_method_
							<< ": CHARMM/EEF1 solvation." << endl << endl;
						break;
					}
			}
		}

		// Get the molecules from the scoring function
		desolv_protein_ = Molecule(*getScoringFunction()->getReceptor(), true);
		desolv_ligand_ = Molecule(*getScoringFunction()->getLigand(), true);

		if (unite_atoms_)
		{
			uniteAtoms_(desolv_protein_);
			uniteAtoms_(desolv_ligand_);
		}

		timer.stop();
		if (verbosity_ > 1)
		{
			Log.info() << "PolarSolvation::setup(): "
				<< timer.getCPUTime() << " s" << std::endl;
		}

		return true;
	}


	void PolarSolvation::update(const vector<std::pair<Atom*, Atom*> >& pair_vector)
	{
	}


	double PolarSolvation::updateScore()

	{
		// Because we have local copies, we need to update the atom postition
		// for our molecules.
		AtomConstIterator src = getScoringFunction()->getReceptor()->beginAtom();
		AtomIterator dst = desolv_protein_.beginAtom();
		// This for-loop assumes that both systems are still of same size and
		// that atoms are still in the same order. No checking done on this!
		for (; +src && +dst; ++src, ++dst)
		{
			dst->setPosition(src->getPosition());
		}

		src = getScoringFunction()->getLigand()->beginAtom();
		dst = desolv_ligand_.beginAtom();
		for (; +src && +dst; ++src, ++dst)
		{
			dst->setPosition(src->getPosition());
		}

		// initialize
		score_ = 0.0;

		// initialize all those necessary PB options
		fdpb_.options[FDPB::Option::VERBOSITY] = verbosity_;
		fdpb_.options[FDPB::Option::SPACING] = spacing_;
		fdpb_.options[FDPB::Option::BORDER] = 8.0;
		fdpb_.options[FDPB::Option::PROBE_RADIUS] = 1.8;
		fdpb_.options[FDPB::Option::BOUNDARY] = FDPB::Boundary::FOCUSING;
		fdpb_.options[FDPB::Option::DIELECTRIC_SMOOTHING] = FDPB::DielectricSmoothing::HARMONIC;
		fdpb_.options[FDPB::Option::CHARGE_DISTRIBUTION]
			= FDPB::ChargeDistribution::TRILINEAR;

		System system;
		bool result = false;
		BoundingBoxProcessor bb_proc;

		float tmp_energy = 0.0;

		if ((calculation_method_ == CALCULATION__FRESNO)
				|| (calculation_method_ == CALCULATION__FULL_SLICK))
		{

			// declare variables for the different energies
			float dG_reac_system = 0.0;
			float dG_reac_protein = 0.0;
			float dG_reac_ligand = 0.0;

			// fresno uses just the desolvation energy of the whole ligand as
			// score for their energy function

			if (calculation_method_ == CALCULATION__FULL_SLICK)
			{

				system.clear();
				system.insert(*(new Molecule(desolv_ligand_, true)));
				system.insert(*((Molecule*)(desolv_protein_.create(true))));

				// we have to be sure that all systems will be calculated in the
				// same bounding box.

				// If we want to focus the grid around the ligand instead of the
				// whole system, calculate the appropriate bounding box
				/*

				Does not work, FDPB::setupAtomArray() does not allow atoms to be
				outside of the (focusing)grid.

				if (focus_grid_around_ligand == true)
				{
					desolv_ligand_.apply(bb_proc);
				}
				else
				{
					system.apply(bb_proc);
				}
				*/
				system.apply(bb_proc);

				fdpb_.options.setVector(FDPB::Option::BOUNDING_BOX_LOWER,
						bb_proc.getLower());
				fdpb_.options.setVector(FDPB::Option::BOUNDING_BOX_UPPER,
						bb_proc.getUpper());

				result = computeEnergyDifference_(system, dG_reac_system);
				if (!result) return false;

				system.clear();
				system.insert(*((Molecule*)(desolv_protein_.create(true))));
				result = computeEnergyDifference_(system, dG_reac_protein);
				if (!result) return false;

			}

			system.clear();
			system.insert(*((Molecule*)(desolv_ligand_.create(true))));

			if ((calculation_method_ == CALCULATION__FRESNO)
					&& (averaging_ != AVERAGING__NONE))
			{

				system.apply(bb_proc);
				fdpb_.options.setVector(FDPB::Option::BOUNDING_BOX_LOWER,
						bb_proc.getLower());
				fdpb_.options.setVector(FDPB::Option::BOUNDING_BOX_UPPER,
						bb_proc.getUpper());

				if (averaging_ == AVERAGING__STATIC)
				{

					// dG_reac_ligand should still be zero (see definition above)

					result = computeEnergyDifference_(system, tmp_energy);
					if (!result) return false;
					dG_reac_ligand += tmp_energy;

					// ????? hardcoded
					float offset = 0.7;
					Vector3 offset_vector;

					// energy minimium
					float minimal_energy = 1e9;
					// energy maximun
					float maximal_energy = -1e9;

					for (Size i = 1; i <= 8; i++)
					{
						offset_vector = permuteComponentSigns_(Vector3(offset), i);
						fdpb_.options.setVector(FDPB::Option::OFFSET, offset_vector);
						result = computeEnergyDifference_(system, tmp_energy);
						if (!result) return false;
						dG_reac_ligand += tmp_energy;
						if (tmp_energy < minimal_energy) minimal_energy = tmp_energy;
						if (tmp_energy > maximal_energy) maximal_energy = tmp_energy;
					}

					dG_reac_ligand /= 9.0;

					if (verbosity_ > 8)
					{
						Log.info() << "Minimal energy: " << minimal_energy << endl;
						Log.info() << "Maximal energy: " << maximal_energy << endl;
					}

				}
				else
				{
					Log.error() << "Random averaging not yet implmented, aborting"
						<< endl;
					return false;
				}

			}
			else
			{
				result = computeEnergyDifference_(system, dG_reac_ligand);
				if (!result) return false;
			}

			score_ = dG_reac_system - dG_reac_protein - dG_reac_ligand;
		}
		else
		{

			if (calculation_method_ == CALCULATION__FULL_CYCLE)
			{

				// insert our candidates into the system
				system.clear();
				Molecule tmp_ligand(*((Molecule*)(desolv_ligand_.create(true))));
				Molecule tmp_protein(*((Molecule*)(desolv_protein_.create(true))));
				system.insert(tmp_protein);
				system.insert(tmp_ligand);

				// calculate the bounding box and make sure we alswayz use the same
				// grid.
				system.apply(bb_proc);
				fdpb_.options.setVector(FDPB::Option::BOUNDING_BOX_LOWER,
						bb_proc.getLower());
				fdpb_.options.setVector(FDPB::Option::BOUNDING_BOX_UPPER,
						bb_proc.getUpper());

				result = computeFullCycle_(system, tmp_protein, tmp_ligand, tmp_energy);
				if (!result) return false;

				score_ = tmp_energy;

			}
			else
			{
				if (calculation_method_ == CALCULATION__FULL_CYCLE_FOCUSED)
				{
					Molecule cut_ligand(*((Molecule*)(desolv_ligand_.create(true))));
					Molecule cut_protein;
					System cut_system;

					cut_ligand.apply(bb_proc);

					Vector3 lower = bb_proc.getLower() - Vector3(8.0, 8.0, 8.0);
					Vector3 upper = bb_proc.getUpper() + Vector3(8.0, 8.0, 8.0);

					fdpb_.options.setVector(FDPB::Option::BOUNDING_BOX_LOWER,
							lower);
					fdpb_.options.setVector(FDPB::Option::BOUNDING_BOX_UPPER,
							upper);

					AtomIterator it = desolv_ligand_.beginAtom();
					Vector3 position;

					it = desolv_protein_.beginAtom();

					for (; +it; ++it)
					{
						position = it->getPosition();
						if ((position.x > lower.x)
								&& (position.y > lower.y)
								&& (position.z > lower.z)
								&& (position.x < upper.x)
								&& (position.y < upper.y)
								&& (position.z < upper.z))
						{
							Atom* c = new Atom(*it, true);
							if (verbosity_ > 8)
							{
								Log.info() << "cut_protein: adding: " << position << " "
									<< c->getCharge() << " " << c->getElement().getSymbol()
									<< " " << c->getFullName() << endl;
							}
							cut_protein.insert(*c);
						}
					}

					cut_system.insert(cut_protein);
#ifdef DEBUG
					PDBFile intermediate("DS4cutprot.pdb", ios::out);
					intermediate << cut_system;
					intermediate.close();
#endif
					cut_system.insert(cut_ligand);

					if (verbosity_ > 8)
					{
						Log.info() << "cut: added " << cut_system.countAtoms()
							<< " atoms" << endl;
					}

					if (averaging_ == AVERAGING__NONE)
					{

						result = computeFullCycle_(cut_system, cut_protein, cut_ligand,
								tmp_energy);
						if (!result) return false;
						score_ = tmp_energy;
					}
					else
					{
						if (averaging_ == AVERAGING__STATIC)
						{

							float offset = 0.7;
							Vector3 offset_vector;

							score_ = 0.0;

							result = computeFullCycle_(cut_system, cut_protein,
									cut_ligand, tmp_energy);
							if (!result) return false;
							score_ += tmp_energy;

							// energy minimium
							float minimal_energy = 1e9;
							// Maximal energy
							float maximal_energy = -1e9;

							for (Size i = 1; i <= 8; i++)
							{
								offset_vector = permuteComponentSigns_(Vector3(offset), i);
								fdpb_.options.setVector(FDPB::Option::OFFSET, offset_vector);
								result = computeFullCycle_(cut_system, cut_protein,
										cut_ligand, tmp_energy);
								if (!result) return false;
								if (tmp_energy < minimal_energy) minimal_energy = tmp_energy;
								if (tmp_energy > maximal_energy) maximal_energy = tmp_energy;
								score_ += tmp_energy;
							}

							score_ /= 9.0;

							if (verbosity_ > 8)
							{
								Log.info() << "Minimal energy: " << minimal_energy << endl;
								Log.info() << "Maximal energy: " << maximal_energy << endl;
							}

						}
						else
						{
							Log.error() << "Random averaging not yet implemented, aborting"
								<< endl;
							return false;
						}
					}
				}
				else
				{
					if (calculation_method_ == CALCULATION__COULOMB)
					{
						if (verbosity_ > 1) Log.info() << "Calculating Coulomb." << endl;
						system.clear();
						system.insert(*(new Molecule(desolv_ligand_, true)));
						float ligand_coulomb = calculateDistanceCoulomb(system);

						system.clear();
						system.insert(*((Molecule*)(desolv_protein_.create(true))));
						float receptor_coulomb = calculateDistanceCoulomb(system);

						system.clear();
						system.insert(*(new Molecule(desolv_ligand_, true)));
						system.insert(*((Molecule*)(desolv_protein_.create(true))));
						float system_coulomb = calculateDistanceCoulomb(system);

						score_ = system_coulomb - (receptor_coulomb + ligand_coulomb);
					}
					else
					{
						if (calculation_method_ == CALCULATION__EEF1)
						{
							Log.warn() << "Calculating CHARMM EEF1 solvation energy disabled at the moment." << endl;
						}
						else
						{
							Log.error() << "PolarSolvation::setup(): "
								<< "unknown calculation method." << endl;
							return false;
						}
					}
				}
			}
		}

		if (verbosity_ > 0)
		{
			Log.info() << "Polar solvation: energy is "
				<< score_ << endl;
		}

		return score_;
	}


	bool PolarSolvation::computeEnergyDifference_(System& system,
			float& energy)

	{
		float dG;

		if (use_gb_)
		{
			gbm_.setup(system);

			gbm_.setSolventDC(bulk_water_dc_);
			dG = gbm_.calculateSolvationEnergy();

			energy = dG;
			return(true);

		}
		else
		{
			fdpb_.options[FDPB::Option::SOLVENT_DC] = bulk_water_dc_;
			if (fdpb_.setup(system))
			{
				fdpb_.solve();
				dG = fdpb_.getEnergy();
				if (verbosity_ > 1)
				{
					Log.info() << "dG in water: "
						<< fdpb_.getEnergy() << endl;
				}

				fdpb_.options[FDPB::Option::SOLVENT_DC] = vacuum_dc_;
				fdpb_.setup(system);
				fdpb_.solve();
				if (verbosity_ > 1)
				{
					Log.info() << "dG in vacuum: "
						<< fdpb_.getEnergy() << endl;
				}
				dG -= fdpb_.getEnergy();

				energy = dG;
				return true;
			}
			else
			{
				Log.error() << "PolarSolvation::computeEnergyDifference_(): "
					<< "could not setup FDPB solver." << endl;
				return false;
			}
		}
	}


	bool PolarSolvation::computeESEnergy_(System& system, float& energy)

	{
		if (use_gb_)
		{
			gbm_.setup(system);
			energy = gbm_.calculateEnergy();
			return(true);
		}
		else
		{
			if (fdpb_.setup(system))
			{
				fdpb_.solve();
				energy = fdpb_.getEnergy();
				return true;
			}
			else
			{
				Log.error() << "PolarSolvation::computeESEnergy_(): "
					<< "could not setup FDPB solver." << endl;
				return false;
			}
		}
	}


	float PolarSolvation::computeESInteractionEnergy_(const Molecule&
	molecule, const HashMap<const Atom*, float>& p_hash)

	{

		float dGint = 0.0;

		AtomConstIterator atom_it = molecule.beginAtom();

		for (; +atom_it; ++atom_it)
		{
			float charge = atom_it->getCharge();

			if (charge != 0.0f)
			{
				float potential;
				if (use_gb_)
				{
					potential = p_hash[&*atom_it];
				}
				else
				{
					potential
						= fdpb_.phi_grid->getInterpolatedValue(atom_it->getPosition());

					potential *= Constants::NA * 1e-3;
				}

				dGint += charge * potential;
			}
		}

		return(Constants::e0 * dGint);
	}


	bool PolarSolvation::computeFullCycle_(System& system, Molecule&
			protein, Molecule& ligand, float& energy)

	{

		// we are computing in bulk water.
		fdpb_.options[FDPB::Option::SOLVENT_DC] = bulk_water_dc_;

		// We need something for clearing the charges of a molecule (for the
		// so-called "ghost" complexes
		ClearChargeProcessor clear_charges;
		ClearRadiusProcessor clear_radii;

		HashMap<Atom*, float> radii;
		HashMap<Atom*, float> charges;
		HashMap<Atom*, float>::ConstIterator map_it;

		// first save the radii and charges of all atoms

		if (verbosity_ > 1) Log.info() << "Saving radii and charges...";
		AtomIterator it = system.beginAtom();
		for (; +it; ++it)
		{
			radii.insert(pair<Atom*, float>(&*it, it->getRadius()));
			charges.insert(pair<Atom*, float>(&*it, it->getCharge()));
		}
		if (verbosity_ > 1) Log.info() << "done" << endl;

		float dGes_A = 0.0;
		float dGes_B = 0.0;
		float dGes_A_cav_B = 0.0;
		float dGes_B_cav_A = 0.0;
		float dGint_AB = 0.0;
		float dGint_BA = 0.0;

		// 1. Calculate the ES energy of the protein

		// Therefore "remove" the ligand by setting its radii and charges to 0
		ligand.apply(clear_charges);
		ligand.apply(clear_radii);

		// Protein radii and charges are still untouched, so we can instantly
		// calculate the ES energy
		bool result = computeESEnergy_(system, dGes_A);
		if (!result) return false;
		if (verbosity_ > 1) Log.info() << "dGes_A = " << dGes_A << endl;

		// 2. Calculate the ES energy of the ligand

		// Therefore "remove" the protein by setting its radii and charges to 0
		protein.apply(clear_charges);
		protein.apply(clear_radii);

		// Retrieve the ligand's radii and charges
		for (it = ligand.beginAtom(); +it; ++it)
		{
			if (radii.has(&*it)) it->setRadius(radii[&*it]);
			else
			{
				Log.error() << "Lost radius of ligand atom pointer "
					<< &*it << ", aborting." << endl;
				return false;
			}
			if (charges.has(&*it)) it->setCharge(charges[&*it]);
			else
			{
				Log.error() << "Lost charge of ligand atom pointer "
					<< &*it << ", aborting." << endl;
				return false;
			}
		}

		// Now compute the ES energy
		result = computeESEnergy_(system, dGes_B);
		if (!result) return false;
		if (verbosity_ > 1) Log.info() << "dGes_B = " << dGes_B << endl;

		// 3. Calculate the ES energy of the protein in presence of a cavity
		// of the ligand

		// Retrieve charges and radii of the protein
		for (it = protein.beginAtom(); +it; ++it)
		{
			if (radii.has(&*it)) it->setRadius(radii[&*it]);
			else
			{
				Log.error() << "Lost radius of protein atom pointer "
					<< &*it << ", aborting." << endl;
				return false;
			}
			if (charges.has(&*it)) it->setCharge(charges[&*it]);
			else
			{
				Log.error() << "Lost charge of protein atom pointer "
					<< &*it << ", aborting." << endl;
				return false;
			}
		}

		// Clear charges of the ligand in order to create the "ghost" particle,
		// i. e. an uncharged cavity formed like the ligand
		// Ligand radii should still be correct
		ligand.apply(clear_charges);

		// Comoute the electrostatic energy
		result = computeESEnergy_(system, dGes_A_cav_B);
		if (!result) return false;
		if (verbosity_ > 1) Log.info() << "dGes_A_cav_B = " << dGes_A_cav_B << endl;

		// 5. (a) Compute the ES interaction energies of the complex partners.
		// This is done by computing the energy of the ligand in the potential
		// of the protein and vice versa
		HashMap<const Atom*, float> p_hash;
		if (use_gb_)
		{
			gbm_.calculatePotential(p_hash);
		}

		for (it = ligand.beginAtom(); +it; ++it)
		{
			if (charges.has(&*it)) it->setCharge(charges[&*it]);
			else
			{
				Log.error() << "Lost charge of ligand atom pointer "
					<< &*it << ", aborting." << endl;
				return false;
			}
		}
		dGint_AB = computeESInteractionEnergy_(ligand, p_hash);
		if (verbosity_ > 1) Log.info() << "dGint_AB = " << dGint_AB << endl;

		// 4. Calculate the ES energy of the ligand in presence of a cavity of
		// the protein

		if (verbosity_ > 1) Log.info() << "  calculating dGes_B_cav_A...";
		protein.apply(clear_charges);
		// protein radii should be correct
		for (it = ligand.beginAtom(); +it; ++it)
		{
			if (charges.has(&*it)) it->setCharge(charges[&*it]);
			else
			{
				Log.error() << "Lost charge of ligand atom pointer "
					<< &*it << ", aborting." << endl;
				return false;
			}
		}
		// ligand radii should be correct.
		result = computeESEnergy_(system, dGes_B_cav_A);
		if (!result) return false;
		if (verbosity_ > 1) Log.info() << "dGes_B_cav_A = " << dGes_B_cav_A << endl;

		// 5. (a) Compute the ES interaction energies of the complex partners.
		// This is done by computing the energy of the ligand in the potential
		// of the protein and vice versa
		if (use_gb_)
		{
			gbm_.calculatePotential(p_hash);
		}

		for (it = protein.beginAtom(); +it; ++it)
		{
			if (charges.has(&*it)) it->setCharge(charges[&*it]);
			else
			{
				Log.error() << "Lost charge of protein atom pointer "
					<< &*it << ", aborting." << endl;
				return false;
			}
		}
		dGint_BA = computeESInteractionEnergy_(protein, p_hash);
		if (verbosity_ > 1) Log.info() << "dGint_BA = " << dGint_BA << endl;

		// calculate the changes in solvation energy
		float ddGsolv = dGes_B_cav_A - dGes_B + dGes_A_cav_B - dGes_A;
		float dGele = ddGsolv + ((dGint_AB + dGint_BA) / 2.0);

		if (verbosity_ > 0)
		{
			Log.info() << "dGes_A = " << dGes_A << endl;
			Log.info() << "dGes_B = " << dGes_B << endl;
			Log.info() << "dGes_A_cav_B = " << dGes_A_cav_B << endl;
			Log.info() << "dGes_B_cav_A = " << dGes_B_cav_A << endl;
			Log.info() << "dGint_AB = " << dGint_AB << endl;
			Log.info() << "dGint_BA = " << dGint_BA << endl;
			Log.info() << "ddGsolv = " << ddGsolv << endl;
			Log.info() << "dGint = " << (dGint_AB + dGint_BA) / 2.0 << endl;
			Log.info() << "dGele = " << dGele << endl;
		}

		// restore radii and charges of the whole system for further usage
		it = system.beginAtom();
		for (; +it; ++it)
		{
			if (radii.has(&*it)) it->setRadius(radii[&*it]);
			else
			{
				Log.error() << "Lost radius of system atom pointer "
					<< &*it << ", aborting." << endl;
				return false;
			}
			if (charges.has(&*it)) it->setCharge(charges[&*it]);
			else
			{
				Log.error() << "Lost charge of system atom pointer "
					<< &*it << ", aborting." << endl;
				return false;
			}
		}

		energy = dGele;

		return true;

	}


	Vector3 PolarSolvation::permuteComponentSigns_(Vector3 vector,
			Size permutation)

	{
		switch (permutation)
		{

			case 1:
				// + + +
				return Vector3( vector.x,  vector.y,  vector.z);
				break;

			case 2:
				// - + +
				return Vector3(-vector.x,  vector.y,  vector.z);
				break;

			case 3:
				// - - +
				return Vector3(-vector.x, -vector.y,  vector.z);
				break;

			case 4:
				// - - -
				return Vector3(-vector.x, -vector.y, -vector.z);
				break;

			case 5:
				// - + -
				return Vector3(-vector.x,  vector.y, -vector.z);
				break;

			case 6:
				// + + -
				return Vector3( vector.x,  vector.y, -vector.z);
				break;

			case 7:
				// + - -
				return Vector3( vector.x, -vector.y, -vector.z);
				break;

			case 8:
				// + - +
				return Vector3( vector.x, -vector.y,  vector.z);
				break;

			default:
				Log.warn() << "Permutation index wrong, returning original vector."
					<< endl;
				return vector;

		}
	}

	Size PolarSolvation::uniteAtoms_(Molecule& molecule)

	{
		// Go through the atoms of the molecule and find those charged
		// hydrogens which have to be condensed on heavy atoms, because they
		// bear partial charges without havin a radius.

		// Note: Condensing hydrogens may lead to single atoms (NHx)!

		// Store the atoms that have to be removed after condensation in a hash
		// set.
		HashSet<Atom*> remove_me;

		AtomIterator it = molecule.beginAtom();
		for (; +it; ++it)
		{
			if (it->getRadius() == 0.0 && it->getCharge() != 0.0)
			{
				if (it->getElement() != PTE[Element::H])
				{
					Log.error() << "Found non-hydrogen atom with non-zero charge and zero radius, aborting."
						<< endl;
					Log.error() << it->getResidue()->getID() << ":" << it->getFullName()
						<< "@" << it->getPosition() << " (r=" << it->getRadius() << ", c="
						<< it->getCharge() << ", e=" << it->getElement().getSymbol() << ")"
						<< endl;
					return(1);
				}
				else
				{
					if (verbosity_ > 1)
					{
						Log.info() << "Found charged hydrogen with zero radius: "
							<< it->getFullName() << endl;
					}
					Atom* hydrogen_partner = it->getBond(0)->getPartner(*it);
					if (verbosity_ > 1)
					{
						Log.info() << "Condensing it onto the bound heavy atom "
							<< hydrogen_partner->getFullName() << endl;
					}
					hydrogen_partner->setCharge(hydrogen_partner->getCharge()
							+ it->getCharge());
					it->setCharge(0.0);
					remove_me.insert(&*it);
				}
			}
		}

		// Now remove the atoms from the system
		HashSet<Atom*>::Iterator remove_it = remove_me.begin();
		for (; +remove_it; ++remove_it)
		{
			Atom* ptr = *remove_it;
			delete(ptr);
		}
		if (verbosity_ > 1)
		{
			Log.info() << "United atoms: Removed " << remove_me.size()
				<< " atoms from the system" << endl;
		}

		return(remove_me.size());

	}

}
