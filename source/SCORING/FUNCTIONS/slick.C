// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: slick.C,v 1.1 2005/11/21 19:27:13 anker Exp $

#include <BALL/SCORING/FUNCTIONS/slick.h>
#include <BALL/SCORING/COMPONENTS/vanDerWaals.h>
#include <BALL/SCORING/COMPONENTS/CHPI.h>
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

	const char* SLICK::Option::CONST = "const";
	const char* SLICK::Option::VDW = "vdw_coefficient";
	const char* SLICK::Option::CHPI = "chpi_coefficient";
	const char* SLICK::Option::NONPOLAR = "nonpolar_coefficient";
	const char* SLICK::Option::POLAR = "polar_coefficient";

	// The following paramteres are derived from regression analysis for
	// sugar lecitn complexes. [anker 2005/08/12]
	const float SLICK::Default::CONST    = -3.80376230f;
	const float SLICK::Default::VDW      =  0.10077183f;
	const float SLICK::Default::CHPI     =  0.68973693f;
	const float SLICK::Default::NONPOLAR =  0.25069990f;
	const float SLICK::Default::POLAR    = -0.05713106f;

	SLICK::SLICK()
		throw()
		:	ScoringFunction()
	{
		// register all components of the force field
		registerComponents_();
	}


	SLICK::SLICK(System& system, Molecule& protein, Molecule& ligand)
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
			Log.error() << "SLICK::SLICK(System&, Molecule&, Molecule&): "
				<< "Setup of scoring function failed! " << std::endl;
		}
	}


	SLICK::SLICK(System& system, Molecule& protein, Molecule& ligand,
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
			Log.error() << "SLICK::SLICK(System&, Molecule&, Molecule&, const Options&): "
				<< "Setup of scoring function failed! " << std::endl;
		}
	}


	SLICK::SLICK(const SLICK& slick)
		throw()
		:	ScoringFunction(slick)
	{
	}


	SLICK::~SLICK()
		throw()
	{
		clear();
	}


	void SLICK::clear()
		throw()
	{
		ScoringFunction::clear();
	}


	bool SLICK::specificSetup()
		throw()
	{

		Size verbosity
			= options.setDefaultInteger(ScoringFunction::Option::VERBOSITY,
					ScoringFunction::Default::VERBOSITY);
		
		options.set(PolarSolvation::Option::POLAR_METHOD,
				PolarSolvation::CALCULATION__FULL_CYCLE_FOCUSED);

		options.set(NonpolarSolvation::Option::NONPOLAR_METHOD,
				NonpolarSolvation::CALCULATION__PCM);

		// check whether the system is assigned
		System* system = getSystem();
		if (system == 0)
		{
			Log.error() << "SLICK::specificSetup(): "
				<< "No system assigned, aborting" << std::endl;
			return false;
		}

		// check whether we have two molecules
		if (system->countMolecules() != 2)
		{
			Log.error() << "SLICK::specificSetup(): "
				<< "SLICK is only defined for systems with 2 molecules, aborting" 
				<< std::endl;
			return false;
		}

		// Now extract options from options table and set the coefficients of
		// the components accordingly.
		float CONST = options.setDefaultReal(SLICK::Option::CONST,
				SLICK::Default::CONST);
		setIntercept(CONST);

		float VDW = options.setDefaultReal(SLICK::Option::VDW,
				SLICK::Default::VDW);
		setCoefficient("van-der-Waals", VDW);

		float CHPI = options.setDefaultReal(SLICK::Option::CHPI,
				SLICK::Default::CHPI);
		setCoefficient("CHPI", CHPI);

		float NPS = options.setDefaultReal(SLICK::Option::NONPOLAR,
				SLICK::Default::NONPOLAR);
		setCoefficient("Nonpolar Solvation", NPS);

		float PS = options.setDefaultReal(SLICK::Option::POLAR,
				SLICK::Default::POLAR);
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


	const SLICK& SLICK::operator = (const SLICK& slick)
		throw()
	{
		// avoid self assignment
		if (&slick != this)
		{
			ScoringFunction::operator = (slick);
		}
		
		return *this;
	}


	double SLICK::getCHPIScore() const
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


	double SLICK::getVDWScore() const
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


	double SLICK::getPolarSolvationScore() const
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


	double SLICK::getNonpolarSolvationScore() const
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

	
	void SLICK::registerComponents_()
		throw()
	{
		// create the component list
		insertComponent(new VanDerWaals(*this));
		insertComponent(new PolarSolvation(*this));
		insertComponent(new NonpolarSolvation(*this));
		insertComponent(new CHPI(*this));
	}

} // namespace BALL
