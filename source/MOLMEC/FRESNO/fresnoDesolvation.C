// $Id: fresnoDesolvation.C,v 1.1.2.7 2002/04/07 17:42:28 anker Exp $
// Molecular Mechanics: Fresno force field, lipophilic component

#include <BALL/MOLMEC/COMMON/forceField.h>

#include <BALL/KERNEL/standardPredicates.h>

#include <BALL/MOLMEC/FRESNO/fresno.h>
#include <BALL/MOLMEC/FRESNO/fresnoDesolvation.h>

#include <BALL/STRUCTURE/defaultProcessors.h>

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

		System* system = force_field->getSystem();
    Options& options = force_field->options;

		factor_ 
			= options.setDefaultReal(FresnoFF::Option::DESOLV,
					FresnoFF::Default::DESOLV);

		ProteinPredicate isProtein;

		// see above
		Molecule* receptor = &*system->beginProtein();
		Molecule* ligand = system->getMolecule(0);
		if (ligand == receptor) ligand = system->getMolecule(1);

		Molecule temp(*ligand, true);
		System ligand_system;
		ligand_system.insert(temp);
		AssignChargeProcessor charges("charges/amber94.crg");
		ligand_system.apply(charges);

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

		if (fdpb.setup(ligand_system))
		{
			fdpb.solve();
			energy_ = fdpb.getReactionFieldEnergy();
			// DEBUG
			Log.info() << "water " << fdpb.getReactionFieldEnergy() << endl;
			// /DEBUG

			fdpb.options[FDPB::Option::SOLVENT_DC] = 1.0;
			fdpb.setup(ligand_system);
			fdpb.solve();
			energy_ -= fdpb.getReactionFieldEnergy();
			energy_ = - energy_;
			// DEBUG
			Log.info() << "vacuum " << fdpb.getReactionFieldEnergy() << endl;
			// /DEBUG

			// DEBUG
			Log.info() << "DESOLV: score is " << energy_ << endl;
			// /DEBUG
			energy_ *= factor_;
			return true;
		}
		else
		{
			Log.error() << "FresnoDesolvation::setup(): "
				<< "could not setup FDPB solver." << endl;
			return false;
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
