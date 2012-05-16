// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: slickScore.C,v 1.3 2006/05/21 18:15:29 anker Exp $

#include <BALL/SCORING/FUNCTIONS/slickScore.h>
#include <BALL/SCORING/COMPONENTS/vanDerWaalsSlick.h>
#include <BALL/SCORING/COMPONENTS/CHPISlick.h>
#include <BALL/SCORING/COMPONENTS/polarSolvation.h>
#include <BALL/SCORING/COMPONENTS/hydrogenBondSlick.h>

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
	}


	SLICKScore::SLICKScore(Molecule& protein, Molecule& ligand,
			Options& new_options)
		:	ScoringFunction(protein, ligand, new_options)
	{
		setup();
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


	bool SLICKScore::setup()
	{
		setName("SLICKScore");

		// Set VDW to use a cut repulsive potential with a limit of 5 kJ/mol per interaction
		options_.setInteger(VanDerWaalsSlick::Option::VDW_METHOD, VanDerWaalsSlick::CALCULATION__SOFTENED_LJ_POTENTIAL_LOG);
		options_.setReal(VanDerWaalsSlick::Option::VDW_SOFTENING_LIMIT, 5.0f);

		// Use simple (and fast) Coulomb interactions for the scoring
		options_.setInteger(PolarSolvation::Option::POLAR_METHOD, PolarSolvation::CALCULATION__COULOMB);

		// Now extract options_ from options table and set the coefficients of
		// the components accordingly.
		float CONST = options_.setDefaultReal(SLICKScore::Option::CONST, SLICKScore::Default::CONST);
		setIntercept(CONST);

		// register all components of the force field
		registerComponents_();

		// check whether we know which molecule is the protein and which is the
		// ligand in this complex. In case we don't know, we have to guess.
		// This can be wrong if we got two PDB files which will both be tagged
		// as protein.
		if ((getReceptor() == 0) || (getLigand() == 0))
		{
			Log.error() << "I don't know what the protein and the ligand is." 
				<< std::endl;
			return false;
		}

		return true;
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
		const ScoringComponent* component = getComponent("CHPISlick");
		if (component != 0)
		{
			return component->getRawScore();
		} 
		else 
		{
			return 0;
		}
	}


	double SLICKScore::getVDWScore() const
	{
		const ScoringComponent* component = getComponent("vanDerWaalsSlick");
		if (component != 0)
		{
			return component->getRawScore();
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
			return component->getRawScore();
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
			return component->getRawScore();
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
			return component->getRawScore();
		} 
		else 
		{
			return 0;
		}
	}
	
	void SLICKScore::registerComponents_()
	{
		float coeff_CHPI = options_.setDefaultReal(SLICKScore::Option::CHPI, SLICKScore::Default::CHPI);
		float coeff_HB = options_.setDefaultReal(SLICKScore::Option::HB, SLICKScore::Default::HB);
		float coeff_VDW = options_.setDefaultReal(SLICKScore::Option::VDW, SLICKScore::Default::VDW);
		float coeff_PS = options_.setDefaultReal(SLICKScore::Option::POLAR, SLICKScore::Default::POLAR);

		CHPISlick* chpi = new CHPISlick(*this);
		chpi->setCoefficient(coeff_CHPI);
		chpi->setup();
		chpi->setNormalizationParameters(0.0, 0.0);
		insertComponent(chpi);

		HydrogenBondSlick* hb = new HydrogenBondSlick(*this);
		hb->setCoefficient(coeff_HB);
		hb->setup();
		hb->setNormalizationParameters(0.0, 0.0);
		insertComponent(hb);

		VanDerWaalsSlick* vdws = new VanDerWaalsSlick(*this);
		vdws->setCoefficient(coeff_VDW);
		vdws->setup();
		vdws->setNormalizationParameters(0.0, 0.0);
		insertComponent(vdws);

		PolarSolvation* ps = new PolarSolvation(*this);
		ps->setCoefficient(coeff_PS);
		ps->setup();
		ps->setNormalizationParameters(0.0, 0.0);
		insertComponent(ps);
	}

} // namespace BALL
