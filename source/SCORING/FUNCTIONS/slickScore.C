// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: slickScore.C,v 1.3 2006/05/21 18:15:29 anker Exp $

#include <BALL/SCORING/FUNCTIONS/slickScore.h>
#include <BALL/SCORING/COMPONENTS/vanDerWaalsSlick.h>
#include <BALL/SCORING/COMPONENTS/CHPI.h>
#include <BALL/SCORING/COMPONENTS/polarSolvation.h>
#include <BALL/SCORING/COMPONENTS/hydrogenBond.h>

#include <BALL/SYSTEM/path.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/standardPredicates.h>
#include <BALL/KERNEL/bondIterator.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/MOLMEC/PARAMETER/templates.h>

namespace BALL
{

	const char* SLICKScore::Option::CONST = "const";
	const char* SLICKScore::Option::HB = "hb_coefficient";
	const char* SLICKScore::Option::CHPI = "chpi_coefficient";
	const char* SLICKScore::Option::VDW = "vdw_coefficient";
	const char* SLICKScore::Option::POLAR = "polar_coefficient";


	// Parameters for the SLICK/score scoring function
	// [anker 2006/02/09]
	const float SLICKScore::Default::CONST    =  0.0f;
	const float SLICKScore::Default::HB       = -2.0f;
	const float SLICKScore::Default::CHPI     = -2.0f;
	const float SLICKScore::Default::VDW      =  0.1f;
	const float SLICKScore::Default::POLAR    =  0.05f;

	SLICKScore::SLICKScore()
		
		:	ScoringFunction()
	{
		// register all components of the force field
		registerComponents_();
	}


	SLICKScore::SLICKScore(Molecule& protein, Molecule& ligand)
		
		:	ScoringFunction()
	{
		setReceptor(protein);
		setLigand(ligand);

		// register all components of the force field
		registerComponents_();

		// set up with the given system
		bool result = setup();

    if (!result)
    {
			Log.error() << "SLICKScore::SLICKScore(Molecule&, Molecule&): "
				<< "Setup of scoring function failed! " << std::endl;
		}
	}


	SLICKScore::SLICKScore(Molecule& protein, Molecule& ligand,
			const Options& new_options)
		
		:	ScoringFunction()
	{
		setReceptor(protein);
		setLigand(ligand);

		// register all components of the force field
		registerComponents_();

		options = new_options;

		bool result = setup(protein, ligand);

    if (!result)
    {
			Log.error() << "SLICKScore::SLICKScore(Molecule&, Molecule&, const Options&): "
				<< "Setup of scoring function failed! " << std::endl;
		}
	}


	SLICKScore::SLICKScore(const SLICKScore& slick)
		
		:	ScoringFunction(slick)
	{
	}


	SLICKScore::~SLICKScore()
		
	{
		clear();
	}


	void SLICKScore::clear()
		
	{
		ScoringFunction::clear();
	}


	bool SLICKScore::specificSetup()
		throw()
	{

		// Set VDW to use a cut repulsive potential with a limit of 5 kJ/mol
		// per interaction
		options.setInteger(VanDerWaals::Option::VDW_METHOD, 
				VanDerWaals::CALCULATION__SOFTENED_LJ_POTENTIAL_LOG);
		options.setReal(VanDerWaals::Option::VDW_SOFTENING_LIMIT, 5.0f);

		// Use simple (and fast) Coulomb interactions for the scoring
		options.setInteger(PolarSolvation::Option::POLAR_METHOD,
				PolarSolvation::CALCULATION__COULOMB);

		// Now extract options from options table and set the coefficients of
		// the components accordingly.
		float CONST = options.setDefaultReal(SLICKScore::Option::CONST,
				SLICKScore::Default::CONST);
		setIntercept(CONST);

		float CHPI = options.setDefaultReal(SLICKScore::Option::CHPI,
				SLICKScore::Default::CHPI);
		setCoefficient("CHPI", CHPI);

		float HB = options.setDefaultReal(SLICKScore::Option::HB,
				SLICKScore::Default::HB);
		setCoefficient("SLICK HydrogenBond", HB);

		float VDW = options.setDefaultReal(SLICKScore::Option::VDW,
				SLICKScore::Default::VDW);
		setCoefficient("van-der-Waals", VDW);

		float PS = options.setDefaultReal(SLICKScore::Option::POLAR,
				SLICKScore::Default::POLAR);
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


	const SLICKScore& SLICKScore::operator = (const SLICKScore& slick)
		
	{
		// avoid self assignment
		if (&slick != this)
		{
			ScoringFunction::operator = (slick);
		}
		
		return *this;
	}


	double SLICKScore::getCHPIScore() const
		
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


	double SLICKScore::getVDWScore() const
		
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


	double SLICKScore::getPolarSolvationScore() const
		
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


	double SLICKScore::getNonpolarSolvationScore() const
		
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

	double SLICKScore::getHydrogenBondScore() const
		
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
	
	void SLICKScore::registerComponents_()
		
	{
		// create the component list
		insertComponent(new CHPI(*this));
		insertComponent(new HydrogenBond(*this));
		insertComponent(new VanDerWaals(*this));
		insertComponent(new PolarSolvation(*this));
	}

} // namespace BALL
