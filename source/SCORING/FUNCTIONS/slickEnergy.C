// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: slickEnergy.C,v 1.1 2006/02/21 16:27:17 anker Exp $

#include <BALL/SCORING/FUNCTIONS/slickEnergy.h>
#include <BALL/SCORING/COMPONENTS/CHPI.h>
#include <BALL/SCORING/COMPONENTS/hydrogenBond.h>
#include <BALL/SCORING/COMPONENTS/vanDerWaals.h>
#include <BALL/SCORING/COMPONENTS/nonpolarSolvation.h>
#include <BALL/SCORING/COMPONENTS/polarSolvation.h>

#include <BALL/SYSTEM/path.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/standardPredicates.h>
#include <BALL/KERNEL/bondIterator.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/MOLMEC/PARAMETER/templates.h>

namespace BALL
{

	const char* SLICKEnergy::Option::CONST = "const";
	const char* SLICKEnergy::Option::HB = "hb_coefficient";
	const char* SLICKEnergy::Option::VDW = "vdw_coefficient";
	const char* SLICKEnergy::Option::CHPI = "chpi_coefficient";
	const char* SLICKEnergy::Option::NONPOLAR = "nonpolar_coefficient";
	const char* SLICKEnergy::Option::POLAR = "polar_coefficient";

	// Parameters for the new function containing HB contributions.
	// Calibration for the complete set. [anker 2006/02/21]
	// Calibration set: 1j4u 5cna 1gic 1qdo 1qdc 1ona 1dgl 1axz 1ax0 1ax1
	// 1ax2 2bqp 1bqp 1qf3 2pel 1ehh 1en2 1k7u
	const float SLICKEnergy::Default::CONST    = -3.04303892;
	const float SLICKEnergy::Default::HB       = -1.18389617;
	const float SLICKEnergy::Default::CHPI     = -0.97534598;
	const float SLICKEnergy::Default::VDW      =  0.02562972;
	const float SLICKEnergy::Default::NONPOLAR =  0.48077471;
	const float SLICKEnergy::Default::POLAR    = -0.15589680;

	SLICKEnergy::SLICKEnergy()
		throw()
		:	ScoringFunction()
	{
		// register all components of the force field
		registerComponents_();
	}


	SLICKEnergy::SLICKEnergy(System& system, Molecule& protein, Molecule& ligand)
		throw()
		:	ScoringFunction()
	{
		setSystem(system);
		setReceptor(protein);
		setLigand(ligand);

		// register all components of the force field
		registerComponents_();

		// set up with the given system
		bool result = setup();

    if (!result)
    {
			Log.error() << "SLICKEnergy::SLICKEnergy(System&, Molecule&, Molecule&): "
				<< "Setup of scoring function failed! " << std::endl;
		}
	}


	SLICKEnergy::SLICKEnergy(System& system, Molecule& protein, Molecule& ligand,
			const Options& new_options)
		throw()
		:	ScoringFunction()
	{
		setSystem(system);
		setReceptor(protein);
		setLigand(ligand);

		// register all components of the force field
		registerComponents_();

		options = new_options;

		bool result = setup(system, protein, ligand);

    if (!result)
    {
			Log.error() << "SLICKEnergy::SLICKEnergy(System&, Molecule&, Molecule&, const Options&): "
				<< "Setup of scoring function failed! " << std::endl;
		}
	}


	SLICKEnergy::SLICKEnergy(const SLICKEnergy& slick)
		throw()
		:	ScoringFunction(slick)
	{
	}


	SLICKEnergy::~SLICKEnergy()
		throw()
	{
		clear();
	}


	void SLICKEnergy::clear()
		throw()
	{
		ScoringFunction::clear();
	}


	bool SLICKEnergy::specificSetup()
		throw()
	{

		Size verbosity
			= options.setDefaultInteger(ScoringFunction::Option::VERBOSITY,
					ScoringFunction::Default::VERBOSITY);
		
		options.set(NonpolarSolvation::Option::NONPOLAR_METHOD,
				NonpolarSolvation::CALCULATION__PCM);

		options.setInteger(PolarSolvation::Option::POLAR_METHOD,
				PolarSolvation::CALCULATION__FULL_CYCLE_FOCUSED);

		options.setInteger(VanDerWaals::Option::VDW_METHOD, 
				VanDerWaals::CALCULATION__SOFTENED_LJ_POTENTIAL_SIMPLE);
		options.setReal(VanDerWaals::Option::VDW_SOFTENING_LIMIT, 0.0f);

		// Flag for activating GB calculations. Bad results for finding binding
		// pockets, so commented out here
		// 
		// options.setBool(PolarSolvation::Option::POLAR_GB,
		//		true);

		// check whether the system is assigned
		System* system = getSystem();
		if (system == 0)
		{
			Log.error() << "SLICKEnergy::specificSetup(): "
				<< "No system assigned, aborting" << std::endl;
			return false;
		}

		// check whether we have two molecules
		if (system->countMolecules() != 2)
		{
			Log.error() << "SLICKEnergy::specificSetup(): "
				<< "SLICKEnergy is only defined for systems with 2 molecules, aborting" 
				<< std::endl;
			return false;
		}

		// Now extract options from options table and set the coefficients of
		// the components accordingly.
		float CONST = options.setDefaultReal(SLICKEnergy::Option::CONST,
				SLICKEnergy::Default::CONST);
		setIntercept(CONST);

		float CHPI = options.setDefaultReal(SLICKEnergy::Option::CHPI,
				SLICKEnergy::Default::CHPI);
		setCoefficient("CHPI", CHPI);

		float HB = options.setDefaultReal(SLICKEnergy::Option::HB,
				SLICKEnergy::Default::HB);
		setCoefficient("SLICK HydrogenBond", HB);

		float VDW = options.setDefaultReal(SLICKEnergy::Option::VDW,
				SLICKEnergy::Default::VDW);
		setCoefficient("van-der-Waals", VDW);

		float NPS = options.setDefaultReal(SLICKEnergy::Option::NONPOLAR,
				SLICKEnergy::Default::NONPOLAR);
		setCoefficient("Nonpolar Solvation", NPS);

		float PS = options.setDefaultReal(SLICKEnergy::Option::POLAR,
				SLICKEnergy::Default::POLAR);
		setCoefficient("Polar Solvation", PS);

		// check whether we know which molecule is the protein and which is the
		// ligand in this complex. In case we don't know, we have to guess.
		// This can be wrong if we got two PDB files which will both be tagged
		// as protein.
		if ((getReceptor() == 0) || (getLigand() == 0))
		{
			Log.error() << "I don't know what the protein and the ligand is." 
				<< std::endl;
			return(false);
		}

		return(true);

	}


	const SLICKEnergy& SLICKEnergy::operator = (const SLICKEnergy& slick)
		throw()
	{
		// avoid self assignment
		if (&slick != this)
		{
			ScoringFunction::operator = (slick);
		}
		
		return *this;
	}


	double SLICKEnergy::getHydrogenBondScore() const
		throw()
	{
		const ScoringComponent* component = getComponent("SLICK HydrogenBond");
		if (component != 0)
		{
			return component->getScore();
		} 
		else 
		{
			return 0;
		}
	}
	

	double SLICKEnergy::getCHPIScore() const
		throw()
	{
		const ScoringComponent* component = getComponent("CHPI");
		if (component != 0)
		{
			return component->getScore();
		} 
		else 
		{
			return 0;
		}
	}


	double SLICKEnergy::getVDWScore() const
		throw()
	{
		const ScoringComponent* component = getComponent("van-der-Waals");
		if (component != 0)
		{
			return component->getScore();
		} 
		else 
		{
			return 0;
		}
	}


	double SLICKEnergy::getPolarSolvationScore() const
		throw()
	{
		const ScoringComponent* component = getComponent("Polar Solvation");
		if (component != 0)
		{
			return component->getScore();
		} 
		else 
		{
			return 0;
		}
	}


	double SLICKEnergy::getNonpolarSolvationScore() const
		throw()
	{
		const ScoringComponent* component = getComponent("Nonpolar Solvation");
		if (component != 0)
		{
			return component->getScore();
		} 
		else 
		{
			return 0;
		}
	}

	
	void SLICKEnergy::registerComponents_()
		throw()
	{
		// create the component list
		insertComponent(new CHPI(*this));
		insertComponent(new HydrogenBond(*this));
		insertComponent(new VanDerWaals(*this));
		insertComponent(new PolarSolvation(*this));
		insertComponent(new NonpolarSolvation(*this));
	}

} // namespace BALL
