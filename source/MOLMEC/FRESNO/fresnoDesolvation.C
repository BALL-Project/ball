// $Id: fresnoDesolvation.C,v 1.1.2.12 2002/10/22 13:47:25 anker Exp $
// Molecular Mechanics: Fresno force field, desolvation component

#include <BALL/MOLMEC/COMMON/forceField.h>

#include <BALL/KERNEL/standardPredicates.h>
#include <BALL/KERNEL/PTE.h>

#include <BALL/MOLMEC/FRESNO/fresno.h>
#include <BALL/MOLMEC/FRESNO/fresnoDesolvation.h>

#include <BALL/STRUCTURE/defaultProcessors.h>
#include <BALL/STRUCTURE/geometricProperties.h>

#include <BALL/DATATYPE/hashMap.h>
#include <BALL/KERNEL/atomIterator.h>

using namespace std;

namespace BALL
{

	FresnoDesolvation::FresnoDesolvation()
		throw()
		:	ForceFieldComponent(),
			factor_(0.0),
			calculation_method_(0),
			verbosity_(0),
			fdpb_(),
			bulk_water_dc_(0.0),
			vacuum_dc_(0.0)
	{
		// set component name
		setName("Fresno Desolvation");
	}


	FresnoDesolvation::FresnoDesolvation(ForceField& force_field)
		throw()
		:	ForceFieldComponent(force_field),
			factor_(0.0),
			calculation_method_(0),
			verbosity_(0),
			fdpb_(),
			bulk_water_dc_(0.0),
			vacuum_dc_(0.0)
	{
		// set component name
		setName("Fresno Desolvation");
	}


	FresnoDesolvation::FresnoDesolvation(const FresnoDesolvation& fd, bool deep)
		throw()
		:	ForceFieldComponent(fd, deep),
			factor_(0.0),
			calculation_method_(0),
			verbosity_(0),
			fdpb_(),
			bulk_water_dc_(0.0),
			vacuum_dc_(0.0)
	{
	}


	FresnoDesolvation::~FresnoDesolvation()
		throw()
	{
		clear();
	}


	void FresnoDesolvation::clear()
		throw()
	{
		// ?????
		// ForceFieldComponent does not comply with the OCI
		// ForceFieldComponent::clear();
		factor_ = 0.0;
		calculation_method_ = 0;
		verbosity_ = 0;
		// ?????
		// FDPB does not comply with the OCI
		// fdpb_.clear();
	}


	bool FresnoDesolvation::setup()
		throw()
	{
		ForceField* force_field = getForceField();
		if (force_field == 0)
		{
			Log.error() << "FresnoDesolvation::setup(): "
				<< "component not bound to force field." << endl;
			return false;
		}

		// ?????
		// we should check whether force_field is a FresnoFF, because we need
		// the fresno types

		System* original = force_field->getSystem();
    Options& options = force_field->options;

		factor_ 
			= options.setDefaultReal(FresnoFF::Option::DESOLV,
					FresnoFF::Default::DESOLV);

		calculation_method_ 
			= options.setDefaultInteger(FresnoFF::Option::DESOLV_METHOD,
					FresnoFF::Default::DESOLV_METHOD);

		verbosity_
			= options.setDefaultInteger(FresnoFF::Option::VERBOSITY,
					FresnoFF::Default::VERBOSITY);

		averaging_
			= options.setDefaultInteger(FresnoFF::Option::DESOLV_AVG,
					FresnoFF::Default::DESOLV_AVG);

		// ????? hardcoded
		spacing_ = 0.5;
		//	= options.setDefaultInteger(FresnoFF::Option::DESOLV_AVG,
		//			FresnoFF::Default::DESOLV_AVG);

		// maybe we should set this via options, but for now this will be
		// sufficient
		bulk_water_dc_ = 80.0;
		vacuum_dc_ = 1.0;
		float tmp_energy = 0.0;

		if (verbosity_ > 0)
		{
			switch(calculation_method_)
			{
				case CALCULATION__FRESNO:
					Log.info() << "Model " << calculation_method_ 
						<< ": original Fresno." << endl << endl;
					break;

				case CALCULATION__FULL_FRESNO:
					Log.info() << "Model " << calculation_method_ 
						<< ": modified Fresno." << endl << endl;
					break;

				case CALCULATION__FULL_CYCLE:
					Log.info() << "Model " << calculation_method_ 
						<< ": full thermodynamic cycle." << endl << endl;
					break;

				case CALCULATION__KEKSE:
					Log.info() << "Model " << calculation_method_ 
						<< ": full thermodynamic cycle with focused grid." << endl << endl;
					break;

				case CALCULATION__EEF1:
					Log.info() << "Model " << calculation_method_ 
						<< ": CHARMM/EEF1 solvation." << endl << endl;
					break;

			}
		}

		// we need local copies of the molecules
		Molecule protein(*((FresnoFF*)force_field)->getProtein(), true);
		Molecule ligand(*((FresnoFF*)force_field)->getLigand(), true);

		// declare a system and some more or less globally neede variables
		System system;
		bool result = false;
		BoundingBoxProcessor bb_proc;
		float single_energy = 0.0;

		energy_ = 0.0;

		// initialize all those necessary PB options
		fdpb_.options[FDPB::Option::VERBOSITY] = verbosity_;
		fdpb_.options[FDPB::Option::SPACING] = spacing_;
		fdpb_.options[FDPB::Option::BORDER] = 8.0;
		fdpb_.options[FDPB::Option::PROBE_RADIUS] = 1.8;
		fdpb_.options[FDPB::Option::BOUNDARY] = FDPB::Boundary::FOCUSING;
		fdpb_.options[FDPB::Option::DIELECTRIC_SMOOTHING] 
			= FDPB::DielectricSmoothing::HARMONIC;
		fdpb_.options[FDPB::Option::CHARGE_DISTRIBUTION]
			= FDPB::ChargeDistribution::TRILINEAR;

		// now calculate the desolvation energy

		if ((calculation_method_ == CALCULATION__FRESNO)
				|| (calculation_method_ == CALCULATION__FULL_FRESNO))
		{

			// declare variables for the different energies
			float dG_reac_system = 0.0;
			float dG_reac_protein = 0.0;
			float dG_reac_ligand = 0.0;

			// fresno uses just the desolvation energy of the whole ligand as
			// score for their energy function

			if (calculation_method_ == CALCULATION__FULL_FRESNO)
			{

				system.clear();
				system.insert(*(new Molecule(ligand, true)));
				system.insert(*((Molecule*)(protein.create(true))));

				// we have to be sure that all systems will be calculated in the
				// same bounding box.
				system.apply(bb_proc);
				fdpb_.options.setVector(FDPB::Option::BOUNDING_BOX_LOWER,
						bb_proc.getLower());
				fdpb_.options.setVector(FDPB::Option::BOUNDING_BOX_UPPER,
						bb_proc.getUpper());

				result = computeEnergyDifference_(system, dG_reac_system);
				if (result == false) return false;

				system.clear();
				system.insert(*((Molecule*)(protein.create(true))));
				result = computeEnergyDifference_(system, dG_reac_protein);
				if (result == false) return false;


			}

			system.clear();
			system.insert(*((Molecule*)(ligand.create(true))));

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
					if (result == false) return false;
					Log.info() << "no perm: " << tmp_energy << endl;
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
						if (result == false) return false;
						Log.info() << "perm " << i << ": " << tmp_energy << endl;
						dG_reac_ligand += tmp_energy;
						if (tmp_energy < minimal_energy) minimal_energy = tmp_energy;
						if (tmp_energy > maximal_energy) maximal_energy = tmp_energy;
					}

					dG_reac_ligand /= 9.0;

					if (verbosity_ > 0)
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
				if (result == false) return false;
			}

			energy_ = dG_reac_system - dG_reac_protein - dG_reac_ligand;

		}
		else
		{

			if (calculation_method_ == CALCULATION__FULL_CYCLE)
			{

				// insert our candidates into the system
				system.clear();
				Molecule tmp_ligand(*((Molecule*)(ligand.create(true))));
				Molecule tmp_protein(*((Molecule*)(protein.create(true))));
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
				if (result == false) return false;

				energy_ = tmp_energy;

			}
			else
			{
				if (calculation_method_ == CALCULATION__EEF1)
				{
					Log.info() << "Calculating CHARMM EEF1 solvation energy." << endl;
				}
				else
				{
					if (calculation_method_ == CALCULATION__KEKSE)
					{
						Molecule cut_ligand;
						Molecule cut_protein;
						System cut_system;

						ligand.apply(bb_proc);

						Vector3 lower = bb_proc.getLower() - Vector3(8.0, 8.0, 8.0);
						Vector3 upper = bb_proc.getUpper() + Vector3(8.0, 8.0, 8.0);

						fdpb_.options.setVector(FDPB::Option::BOUNDING_BOX_LOWER,
								lower);
						fdpb_.options.setVector(FDPB::Option::BOUNDING_BOX_UPPER,
								upper);

						AtomIterator it = ligand.beginAtom();
						Vector3 position;

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
									Log.info() << "cut_ligand: adding: " << position << " " 
										<< c->getCharge() << " " << c->getElement().getSymbol()
										<< " " << c->getFullName() << endl;
								}
								cut_ligand.insert(*c);
							}
						}

						it = protein.beginAtom();

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

						cut_system.insert(*((Molecule*)(cut_ligand.create(true))));
						cut_system.insert(*((Molecule*)(cut_protein.create(true))));

						// DEBUG
						Log.info() << "cut: added " << cut_system.countAtoms() 
							<< " atoms" << endl;
						// /DEBUG

						if (averaging_ == AVERAGING__NONE)
						{

							result = computeFullCycle_(cut_system, cut_protein, cut_ligand,
									tmp_energy);
							if (result == false) return false;

							// DEBUG
							Log.info() << "DEBUG: " << tmp_energy << endl;
							result = computeFullCycle_(cut_system, cut_protein, cut_ligand, 
									tmp_energy);
							if (result == false) return false;
							Log.info() << "DEBUG: " << tmp_energy << endl;
							result = computeFullCycle_(cut_system, cut_protein, cut_ligand, 
									tmp_energy);
							if (result == false) return false;
							Log.info() << "DEBUG: " << tmp_energy << endl;
							// /DEBUG

							energy_ = tmp_energy;


						}
						else
						{
							if (averaging_ == AVERAGING__STATIC)
							{

								float offset = 0.7;
								Vector3 offset_vector;

								energy_ = 0.0;

								result = computeFullCycle_(cut_system, cut_protein,
										cut_ligand, tmp_energy);
								if (result == false) return false;
								Log.info() << "no perm: " << tmp_energy << endl;
								energy_ += tmp_energy;

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
									if (result == false) return false;
									if (tmp_energy < minimal_energy) minimal_energy = tmp_energy;
									if (tmp_energy > maximal_energy) maximal_energy = tmp_energy;
									Log.info() << "perm " << i << ": " << tmp_energy << endl;
									energy_ += tmp_energy;
								}

								energy_ /= 9.0;

								if (verbosity_ > 0)
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
						Log.error() << "FresnoDesolvation::setup(): "
							<< "unknown calculation method." << endl;
						return false;
					}
				}
			}
		}

		// DEBUG
		Log.info() << "DESOLV: score is " << energy_ << endl;
		// /DEBUG

		energy_ *= factor_;
		return true;
	}


	double FresnoDesolvation::updateEnergy()
		throw()
	{
		// DEBUG
		cout << "DESOLV: energy is " << energy_ << endl;
		// /DEBUG
		return energy_;
	}


	void FresnoDesolvation::updateForces()
		throw()
	{
	}


	bool FresnoDesolvation::computeEnergyDifference_(System& system,
			float& energy)
		throw()
	{
		float dG_reac;

		fdpb_.options[FDPB::Option::SOLVENT_DC] = bulk_water_dc_;
		if (fdpb_.setup(system))
		{
			fdpb_.solve();
			if (verbosity_ > 0)
			{
				Log.info() << "dG_reac in water: " 
					<< fdpb_.getReactionFieldEnergy() << endl;
			}
			dG_reac = fdpb_.getReactionFieldEnergy();

			fdpb_.options[FDPB::Option::SOLVENT_DC] = vacuum_dc_;
			fdpb_.setup(system);
			fdpb_.solve();
			if (verbosity_ > 0)
			{
				Log.info() << "dG_reac in vacuum: " 
					<< fdpb_.getReactionFieldEnergy() << endl;
			}
			dG_reac -= fdpb_.getReactionFieldEnergy();

			energy = dG_reac;
			return true;
		}
		else
		{
			Log.error() << "FresnoDesolvation::computeEnergyDifference_(): "
				<< "could not setup FDPB solver." << endl;
			return false;
		}
	}


	bool FresnoDesolvation::computeESEnergy_(System& system, float& energy)
		throw()
	{
		if (fdpb_.setup(system))
		{
			fdpb_.solve();
			energy = fdpb_.getReactionFieldEnergy();
			return true;
		}
		else
		{
			Log.error() << "FresnoDesolvation::computeESEnergy_(): "
				<< "could not setup FDPB solver." << endl;
			return false;
		}
	}


	float FresnoDesolvation::computeESInteractionEnergy_(const Molecule& molecule)
		throw()
	{

		float dGint = 0.0;

		AtomConstIterator atom_it = molecule.beginAtom();

		for (; +atom_it; ++atom_it)
		{
			dGint += atom_it->getCharge() 
				* fdpb_.phi_grid->getInterpolatedValue(atom_it->getPosition());
		}

		return Constants::e0 * Constants::NA * 1e-3 * dGint;
	}


	bool FresnoDesolvation::computeFullCycle_(System& system, Molecule&
			protein, Molecule& ligand, float& energy)
		throw()
	{

		// we are computing in bulk water.
		fdpb_.options[FDPB::Option::SOLVENT_DC] = bulk_water_dc_;

		ClearChargeProcessor clear_charges;
		ClearRadiusProcessor clear_radii;

		HashMap<Atom*, float> radii;
		HashMap<Atom*, float> charges;
		HashMap<Atom*, float>::ConstIterator map_it;

		// first save the radii and charges of all atoms

		if (verbosity_ > 0) Log.info() << "Saving radii and charges...";
		AtomIterator it = system.beginAtom();
		for (; +it; ++it)
		{
			radii.insert(pair<Atom*, float>(&*it, it->getRadius()));
			charges.insert(pair<Atom*, float>(&*it, it->getCharge()));
		}
		if (verbosity_ > 0) Log.info() << "done" << endl; 

		float dGes_A = 0.0;
		float dGes_B = 0.0;
		float dGes_A_cav_B = 0.0;
		float dGes_B_cav_A = 0.0;
		float dGint_AB = 0.0;
		float dGint_BA = 0.0;

		// calculate the ES energy of the protein

		if (verbosity_ > 0) Log.info() << "  calculating dGes_A... ";
		ligand.apply(clear_charges);
		ligand.apply(clear_radii);
		// protein radii and charges are still untouched
		bool result = computeESEnergy_(system, dGes_A);
		if (result == false) return false;
		if (verbosity_ > 0) Log.info() << dGes_A << endl;

		// calculate the ES energy of the ligand

		if (verbosity_ > 0) Log.info() << "  calculating dGes_B... ";
		protein.apply(clear_charges);
		protein.apply(clear_radii);
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
		result = computeESEnergy_(system, dGes_B);
		if (result == false) return false;
		if (verbosity_ > 0) Log.info() << dGes_B << endl;

		// calculate the ES energy of the protein in presence of a cavity
		// of the ligand

		if (verbosity_ > 0) Log.info() << "  calculating dGes_A_cav_B...";
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
		ligand.apply(clear_charges);
		// ligand radii should be correct
		result = computeESEnergy_(system, dGes_A_cav_B);
		if (result == false) return false;

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
		dGint_AB = computeESInteractionEnergy_(ligand);

		if (verbosity_ > 0) Log.info() << " " << dGes_A_cav_B << endl;

		// calculate the ES energy of the ligand in presence of a cavity of
		// the protein

		if (verbosity_ > 0) Log.info() << "  calculating dGes_B_cav_A...";
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
		if (result == false) return false;

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
		dGint_BA = computeESInteractionEnergy_(protein);

		if (verbosity_ > 0) Log.info() << " " << dGes_B_cav_A << endl;

		// calculate the changes in solvation energy
		float ddGsolv = dGes_B_cav_A - dGes_B + dGes_A_cav_B - dGes_A;
		// DEBUG
		Log.info() << "dGes_A = " << dGes_A << endl;
		Log.info() << "dGes_B = " << dGes_B << endl;
		Log.info() << "dGes_A_cav_B = " << dGes_A_cav_B << endl;
		Log.info() << "dGes_B_cav_A = " << dGes_B_cav_A << endl;
		Log.info() << "ddGsolv = " << ddGsolv << endl;
		// /DEBUG

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

		energy = ddGsolv;

		return true;

	}


	Vector3 FresnoDesolvation::permuteComponentSigns_(Vector3 vector, 
			Size permutation)
		throw()
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

}
