// $Id: fresnoDesolvation.C,v 1.1.2.8 2002/06/04 11:57:04 anker Exp $
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
		:	ForceFieldComponent()
	{
		// set component name
		setName("Fresno Desolvation");
	}


	FresnoDesolvation::FresnoDesolvation(ForceField& force_field)
		throw()
		:	ForceFieldComponent(force_field)
	{
		// set component name
		setName("Fresno Desolvation");
	}


	FresnoDesolvation::FresnoDesolvation(const FresnoDesolvation& fd, bool deep)
		throw()
		:	ForceFieldComponent(fd, deep)
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

		// fdpb.clear();

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

		// DEBUG
		Log.info() << "Using model " << calculation_method_ << endl;
		// /DEBUG

		// ?????
		// ProteinPredicate isProtein;

		// see above
		Molecule* receptor_ptr = &*original->beginProtein();
		Molecule* ligand_ptr = original->getMolecule(0);
		if (ligand_ptr == receptor_ptr) 
		{
			ligand_ptr = original->getMolecule(1);
		}

		// we need local copies of the molecules
		Molecule ligand(*ligand_ptr, true);
		Molecule receptor(*receptor_ptr, true);

		// now insert those copies into a system
		System system;
		system.insert(ligand);
		system.insert(receptor);

		BoundingBoxProcessor bb_proc;
		system.apply(bb_proc);

		FDPB fdpb;
		fdpb.options[FDPB::Option::SOLVENT_DC] = 80.0;
		fdpb.options[FDPB::Option::SPACING] = 0.5;
		fdpb.options[FDPB::Option::BORDER] = 8.0;
		fdpb.options[FDPB::Option::PROBE_RADIUS] = 1.8;
		fdpb.options[FDPB::Option::BOUNDARY] = FDPB::Boundary::FOCUSING;
		fdpb.options[FDPB::Option::DIELECTRIC_SMOOTHING] 
			= FDPB::DielectricSmoothing::HARMONIC;
		fdpb.options[FDPB::Option::CHARGE_DISTRIBUTION]
			= FDPB::ChargeDistribution::TRILINEAR;

		if (calculation_method_ != CALCULATION__FRESNO)
		{
			fdpb.options.setVector(FDPB::Option::BOUNDING_BOX_LOWER,
					bb_proc.getLower());
			fdpb.options.setVector(FDPB::Option::BOUNDING_BOX_UPPER,
					bb_proc.getUpper());
		}

		double dG_reac_sys = 0.0;
		double dG_reac_rec = 0.0;
		double dG_reac_lig = 0.0;
		energy_ = 0.0;

		// now calculate the desolvation energy

		if ((calculation_method_ == CALCULATION__FRESNO)
				|| (calculation_method_ == CALCULATION__FULL_FRESNO))
		{

			System ligand_system;
			ligand_system.insert(ligand);

			if (fdpb.setup(ligand_system))
			{
				// DEBUG
				// Log.info() << "options after setup of dG_reac(ligand) in water: ";
				// fdpb.options.dump();
				// /DEBUG
				fdpb.solve();
				dG_reac_lig = fdpb.getReactionFieldEnergy();

				// DEBUG
				Log.info() << "dG_reac(ligand) in water: " 
					<< fdpb.getReactionFieldEnergy() << endl;
				// /DEBUG

				fdpb.options[FDPB::Option::SOLVENT_DC] = 1.0;
				fdpb.setup(ligand_system);
				// DEBUG
				// Log.info() << "options after setup of dG_reac(ligand) in vacuum: ";
				// fdpb.options.dump();
				// /DEBUG
				fdpb.solve();
				dG_reac_lig -= fdpb.getReactionFieldEnergy();

				// DEBUG
				Log.info() << "dG_reac(ligand) in vacuum: " 
					<< fdpb.getReactionFieldEnergy() << endl;
				// /DEBUG


			}
			else
			{
				Log.error() << "FresnoDesolvation::setup(): "
					<< "could not setup FDPB solver for the ligand." << endl;
				return false;
			}
			if (calculation_method_ == CALCULATION__FULL_FRESNO)
			{

				Log.info() << "Calculating complete desolvation energy term." << endl;

				System receptor_system;
				receptor_system.insert(receptor);

				fdpb.options[FDPB::Option::SOLVENT_DC] = 80.0;
				if (fdpb.setup(receptor_system))
				{
					// DEBUG
					Log.info() << "options after setup of dG_reac(receptor) in water: ";
					fdpb.options.dump();
					// /DEBUG
					fdpb.solve();
					dG_reac_rec = fdpb.getReactionFieldEnergy();

					// DEBUG
					Log.info() << "dG_reac(receptor) in water: " 
						<< fdpb.getReactionFieldEnergy() << endl;
					// /DEBUG

					fdpb.options[FDPB::Option::SOLVENT_DC] = 1.0;
					fdpb.setup(receptor_system);
					// DEBUG
					Log.info() << "options after setup of dG_reac(receptor) in vacuum: ";
					fdpb.options.dump();
					// /DEBUG
					fdpb.solve();
					dG_reac_rec -= fdpb.getReactionFieldEnergy();

					// DEBUG
					Log.info() << "dG_reac(receptor) in vacuum: " 
						<< fdpb.getReactionFieldEnergy() << endl;
					// /DEBUG

				}
				else
				{
					Log.error() << "FresnoDesolvation::setup(): "
						<< "could not setup FDPB solver for the protein." << endl;
					return false;
				}

				fdpb.options[FDPB::Option::SOLVENT_DC] = 80.0;
				system.clear();
				system.insert(ligand);
				system.insert(receptor);
				if (fdpb.setup(system))
				{
					// DEBUG
					Log.info() << "options after setup of dG_reac(system) in water: ";
					fdpb.options.dump();
					// /DEBUG
					fdpb.solve();
					dG_reac_sys = fdpb.getReactionFieldEnergy();

					// DEBUG
					Log.info() << "dG_reac(system) in water: " 
						<< fdpb.getReactionFieldEnergy() << endl;
					// /DEBUG

					fdpb.options[FDPB::Option::SOLVENT_DC] = 1.0;
					fdpb.setup(system);
					// DEBUG
					Log.info() << "options after setup of dG_reac(system) in vacuum: ";
					fdpb.options.dump();
					// /DEBUG
					fdpb.solve();
					dG_reac_sys -= fdpb.getReactionFieldEnergy();

					// DEBUG
					Log.info() << "dG_reac(system) in vacuum: " 
						<< fdpb.getReactionFieldEnergy() << endl;
					// /DEBUG
				}
				else
				{
					Log.error() << "FresnoDesolvation::setup(): "
						<< "could not setup FDPB solver for the complex." << endl;
					return false;
				}

			}


			energy_ = dG_reac_sys - dG_reac_rec - dG_reac_lig;

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

				Log.info() << "Calculating full solvation cycle." << endl;

				ClearChargeProcessor clear_charges;
				ClearRadiusProcessor clear_radii;

				HashMap<Atom*, float> radii;
				HashMap<Atom*, float> charges;
				HashMap<Atom*, float>::ConstIterator map_it;

				// first save the radii and charges of all atoms
				AtomIterator it = system.beginAtom();
				for (; +it; ++it)
				{
					radii.insert(pair<Atom*, float>(&*it, it->getRadius()));
					charges.insert(pair<Atom*, float>(&*it, it->getCharge()));
				}

				Log.info() << "  calculating dGes_A...";
				ligand.apply(clear_charges);
				ligand.apply(clear_radii);
				// receptor radii and charges are still untouched
				fdpb.setup(system);
				fdpb.solve();
				float dGes_A = fdpb.getReactionFieldEnergy();
				Log.info() << " " << dGes_A << endl;

				Log.info() << "  calculating dGes_B...";
				receptor.apply(clear_charges);
				receptor.apply(clear_radii);
				for (it = ligand.beginAtom(); +it; ++it)
				{
					if (radii.has(&*it)) it->setRadius(radii[&*it]);
					else Log.error() << "Kapodd" << endl;
					if (charges.has(&*it)) it->setCharge(charges[&*it]);
					else Log.error() << "Kapodd" << endl;
				}
				fdpb.setup(system);
				fdpb.solve();
				float dGes_B = fdpb.getReactionFieldEnergy();  
				Log.info() << " " << dGes_B << endl;

				Log.info() << "  calculating dGes_A_cav_B...";
				for (it = receptor.beginAtom(); +it; ++it)
				{
					if (radii.has(&*it)) it->setRadius(radii[&*it]);
					else Log.error() << "Kapodd" << endl;
					if (charges.has(&*it)) it->setCharge(charges[&*it]);
					else Log.error() << "Kapodd" << endl;
				}
				ligand.apply(clear_charges);
				// ligand radii should be correct
				fdpb.setup(system);
				fdpb.solve();
				float dGes_A_cav_B = fdpb.getReactionFieldEnergy();
				Log.info() << " " << dGes_A_cav_B << endl;

				Log.info() << "  calculating dGes_B_cav_A...";
				receptor.apply(clear_charges);
				// receptor radii should be correct
				for (it = ligand.beginAtom(); +it; ++it)
				{
					if (charges.has(&*it)) it->setCharge(charges[&*it]);
					else Log.error() << "Kapodd" << endl;
				}
				// ligand radii should be correct.
				fdpb.setup(system);
				fdpb.solve();
				float dGes_B_cav_A = fdpb.getReactionFieldEnergy();
				Log.info() << " " << dGes_B_cav_A << endl;

				// calculate the changes in solvation energy
				float ddGsolv = dGes_B_cav_A - dGes_B + dGes_A_cav_B - dGes_A;

				energy_ = ddGsolv;
				Log.info() << "DESOLV: score is " << energy_ << endl;
				energy_ *= factor_;
				return true;

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
					Log.error() << "FresnoDesolvation::setup(): "
						<< "unknown calculation method." << endl;
					return false;
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


}
