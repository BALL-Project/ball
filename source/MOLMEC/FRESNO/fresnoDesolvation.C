// $Id: fresnoDesolvation.C,v 1.1.2.5 2002/04/03 18:52:13 anker Exp $
// Molecular Mechanics: Fresno force field, lipophilic component

#include <BALL/MOLMEC/COMMON/forceField.h>

#include <BALL/KERNEL/standardPredicates.h>

#include <BALL/MOLMEC/FRESNO/fresno.h>
#include <BALL/MOLMEC/FRESNO/fresnoDesolvation.h>

#include <BALL/MOLMEC/COMMON/chargeRuleProcessor.h>

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
		INIFile charge_ini("/home/anker/fresno/BALL/data/solvation/PARSE.rul");
		if (!charge_ini.read())
		{
			Log.error() << "Cannot read ini." << endl;
			return 1;
		}
		ChargeRuleProcessor charges(charge_ini);
		ligand_system.apply(charges);

		FDPB fdpb;

		if (fdpb.setup(ligand_system))
		{
			fdpb.solve();
			energy_ = fdpb.getReactionFieldEnergy();
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
