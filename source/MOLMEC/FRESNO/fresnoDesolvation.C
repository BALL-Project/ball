// $Id: fresnoDesolvation.C,v 1.1.2.10 2002/09/13 14:18:27 anker Exp $
// Molecular Mechanics: Fresno force field, desolvation component

#include <BALL/MOLMEC/COMMON/forceField.h>

#include <BALL/KERNEL/standardPredicates.h>

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

		// maybe we should set this via options, but for now this will be
		// sufficient
		bulk_water_dc_ = 78.0;
		vacuum_dc_ = 1.0;

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

		// declare a system
		System system;

		fdpb_.options[FDPB::Option::VERBOSITY] = verbosity_;
		fdpb_.options[FDPB::Option::SPACING] = 0.5;
		fdpb_.options[FDPB::Option::BORDER] = 8.0;
		fdpb_.options[FDPB::Option::PROBE_RADIUS] = 1.8;
		fdpb_.options[FDPB::Option::BOUNDARY] = FDPB::Boundary::FOCUSING;
		fdpb_.options[FDPB::Option::DIELECTRIC_SMOOTHING] 
			= FDPB::DielectricSmoothing::HARMONIC;
		fdpb_.options[FDPB::Option::CHARGE_DISTRIBUTION]
			= FDPB::ChargeDistribution::TRILINEAR;

		BoundingBoxProcessor bb_proc;
		if ((calculation_method_ != CALCULATION__FRESNO) 
				&& (calculation_method_ != CALCULATION__KEKSE))
		{
		}

		// declare variables fo rthe different energies
		float dG_reac_system = 0.0;
		float dG_reac_protein = 0.0;
		float dG_reac_ligand = 0.0;
		energy_ = 0.0;

		// now calculate the desolvation energy

		bool result;

		if ((calculation_method_ == CALCULATION__FRESNO)
				|| (calculation_method_ == CALCULATION__FULL_FRESNO))
		{

			// fresno uses just the desolvation energy of the whole ligand as
			// score for their enrgy function

			if (calculation_method_ == CALCULATION__FULL_FRESNO)
			{


				system.clear();
				system.insert(ligand);
				system.insert(protein);

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
				system.insert(protein);
				result = computeEnergyDifference_(system, dG_reac_protein);
				if (result == false) return false;


			}

			system.clear();
			system.insert(ligand);
			result = computeEnergyDifference_(system, dG_reac_ligand);
			if (result == false) return false;

			energy_ = dG_reac_system - dG_reac_protein - dG_reac_ligand;

			// DEBUG
			Log.info() << "DESOLV: score is " << energy_ << endl;
			// /DEBUG

			energy_ *= factor_;
			return true;
		}
		else
		{

			if (calculation_method_ == CALCULATION__FULL_CYCLE)
			{

				// insert our candidates into the system
				system.clear();
				system.insert(protein);
				system.insert(ligand);

				// calculate the bounding box and make sure we alswayz use the same
				// grid.
				system.apply(bb_proc);
				fdpb_.options.setVector(FDPB::Option::BOUNDING_BOX_LOWER,
						bb_proc.getLower());
				fdpb_.options.setVector(FDPB::Option::BOUNDING_BOX_UPPER,
						bb_proc.getUpper());

				// we are computing in bulk water.
				fdpb_.options[FDPB::Option::SOLVENT_DC] = bulk_water_dc_;

				float e;
				if (computeFullCycle_(system, protein, ligand, e) == true)
				{
					energy_ = e;
					Log.info() << "DESOLV: score is " << energy_ << endl;
					energy_ *= factor_;
					return true;
				}
				else return false;

			}
			else
			{
				if (calculation_method_ == CALCULATION__EEF1)
				{
					Log.info() << "Calculating CHARMM EEF1 solvation energy." << endl;

					return true;
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
								Atom* c = new Atom(*it);
								// DEBUG
								Log.info() << "cut_ligand: adding: " << position << " " 
									<< c->getCharge() << " " << c->getFullName() << endl;
								// /DEBUG
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
								Atom* c = new Atom(*it);
								// DEBUG
								Log.info() << "cut_protein: adding: " << position << " " 
									<< c->getCharge() << " " << c->getFullName() << endl;
								// /DEBUG
								cut_protein.insert(*c);
							}
						}

						cut_system.insert(cut_ligand);
						cut_system.insert(cut_protein);

						// DEBUG
						Log.info() << "cut: added " << cut_system.countAtoms() 
							<< " atoms" << endl;
						// /DEBUG

						float e;
						if (computeFullCycle_(cut_system, cut_protein, cut_ligand, e)
							== true)
						{
							energy_ = e;
							Log.info() << "DESOLV: score is " << energy_ << endl;
							energy_ *= factor_;
							return true;
						}
						else return false;

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
			energy = fdpb_.getEnergy();
		}
		else
		{
			Log.error() << "FresnoDesolvation::computeESEnergy_(): "
				<< "could not setup FDPB solver." << endl;
			return false;
		}
	}


	bool FresnoDesolvation::computeFullCycle_(System& system, Molecule&
			protein, Molecule& ligand, float& energy)
		throw()
	{

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

		float dGes_A;
		float dGes_B;
		float dGes_A_cav_B;
		float dGes_B_cav_A;

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
			else Log.error() << "Kapodd" << endl;
			if (charges.has(&*it)) it->setCharge(charges[&*it]);
			else Log.error() << "Kapodd" << endl;
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
			else Log.error() << "Kapodd" << endl;
			if (charges.has(&*it)) it->setCharge(charges[&*it]);
			else Log.error() << "Kapodd" << endl;
		}
		ligand.apply(clear_charges);
		// ligand radii should be correct
		result = computeESEnergy_(system, dGes_A_cav_B);
		if (result == false) return false;
		if (verbosity_ > 0) Log.info() << " " << dGes_A_cav_B << endl;

		// calculate the ES energy of the ligand in presence of a cavity of
		// the protein

		if (verbosity_ > 0) Log.info() << "  calculating dGes_B_cav_A...";
		protein.apply(clear_charges);
		// protein radii should be correct
		for (it = ligand.beginAtom(); +it; ++it)
		{
			if (charges.has(&*it)) it->setCharge(charges[&*it]);
			else Log.error() << "Kapodd" << endl;
		}
		// ligand radii should be correct.
		result = computeESEnergy_(system, dGes_B_cav_A);
		if (result == false) return false;
		if (verbosity_ > 0) Log.info() << " " << dGes_B_cav_A << endl;

		// calculate the changes in solvation energy
		float ddGsolv = dGes_B_cav_A - dGes_B + dGes_A_cav_B - dGes_A;

		energy = ddGsolv;

		return true;

	}

}
