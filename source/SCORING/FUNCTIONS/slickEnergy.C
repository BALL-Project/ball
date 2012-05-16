// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: slickEnergy.C,v 1.4 2006/05/21 18:15:29 anker Exp $

#include <BALL/SCORING/FUNCTIONS/slickEnergy.h>
#include <BALL/SCORING/COMPONENTS/CHPISlick.h>
#include <BALL/SCORING/COMPONENTS/hydrogenBondSlick.h>
#include <BALL/SCORING/COMPONENTS/vanDerWaalsSlick.h>
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
	// Calibration for the complete set. [anker 2006/02/24]
	// Calibration set: 1j4u 5cna 1gic 1qdo 1qdc 1ona 1dgl 1axz 1ax0 1ax1
	// 1ax2 2bqp 1bqp 1qf3 2pel 1ehh 1en2 1k7u
	// const float SLICKEnergy::Default::CONST    = -2.70722038;
	// const float SLICKEnergy::Default::HB       = -1.31258320;
	// const float SLICKEnergy::Default::CHPI     = -0.74188312;
	// const float SLICKEnergy::Default::VDW      =  0.02207132;
	// const float SLICKEnergy::Default::NONPOLAR =  0.49981964;
	// const float SLICKEnergy::Default::POLAR    = -0.12498055;

	// Parameters for the whole calibration set, this time with Bondi
	// observed mean radii in the nonpolar term and the log-softened VDW
	// component
	// [anker 2006/04/26]
// 	const float SLICKEnergy::Default::CONST    = -5.23774531;
// 	const float SLICKEnergy::Default::HB       = -0.09077441;
// 	const float SLICKEnergy::Default::CHPI     = -0.50244683;
// 	const float SLICKEnergy::Default::VDW      =  0.01559796;
// 	const float SLICKEnergy::Default::NONPOLAR =  0.36638941;
// 	const float SLICKEnergy::Default::POLAR    = -0.10725374;

	// Recalibrated after finding inconsistencies
	// [anker 2008-Dec-15]
	const float SLICKEnergy::Default::CONST    = -5.303867;
	const float SLICKEnergy::Default::HB       =  0.160806;
	const float SLICKEnergy::Default::CHPI     = -0.655133;
	const float SLICKEnergy::Default::VDW      =  0.006934;
	const float SLICKEnergy::Default::NONPOLAR =  0.406553;
	const float SLICKEnergy::Default::POLAR    = -0.099546;

	SLICKEnergy::SLICKEnergy()
		:	ScoringFunction()
	{
	}


	SLICKEnergy::SLICKEnergy(Molecule& protein, Molecule& ligand,
				 Options& new_options)
		:	ScoringFunction(protein, ligand, new_options)
	{
		setup();
	}


	SLICKEnergy::SLICKEnergy(const SLICKEnergy& slick)
		:	ScoringFunction(slick)
	{
	}


	SLICKEnergy::~SLICKEnergy()
	{
		clear();
	}


	void SLICKEnergy::clear()
		
	{
		ScoringFunction::clear();
	}


	bool SLICKEnergy::setup()
	{
		setName("SLICKEnergy");

		options_.set(NonpolarSolvation::Option::NONPOLAR_METHOD, NonpolarSolvation::CALCULATION__PCM);
		options_.setInteger(PolarSolvation::Option::POLAR_METHOD, PolarSolvation::CALCULATION__FULL_CYCLE_FOCUSED);
		options_.setInteger(VanDerWaalsSlick::Option::VDW_METHOD, VanDerWaalsSlick::CALCULATION__SOFTENED_LJ_POTENTIAL_LOG);
		options_.setReal(VanDerWaalsSlick::Option::VDW_SOFTENING_LIMIT, 5.0f);

		float CONST = options_.setDefaultReal(SLICKEnergy::Option::CONST, SLICKEnergy::Default::CONST);
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

		return true ;
	}


	const SLICKEnergy& SLICKEnergy::operator = (const SLICKEnergy& slick)
	{
		// avoid self assignment
		if (&slick != this)
		{
			ScoringFunction::operator = (slick);
		}
		
		return *this;
	}


	double SLICKEnergy::getHydrogenBondScore() const
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
	

	double SLICKEnergy::getCHPIScore() const
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


	double SLICKEnergy::getVDWScore() const
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


	double SLICKEnergy::getPolarSolvationScore() const
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


	double SLICKEnergy::getNonpolarSolvationScore() const
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

	
	void SLICKEnergy::registerComponents_()
		
	{
		float coeff_CHPI = options_.setDefaultReal(SLICKEnergy::Option::CHPI, SLICKEnergy::Default::CHPI);
		float coeff_HB = options_.setDefaultReal(SLICKEnergy::Option::HB, SLICKEnergy::Default::HB);
		float coeff_VDW = options_.setDefaultReal(SLICKEnergy::Option::VDW, SLICKEnergy::Default::VDW);
		float coeff_NPS = options_.setDefaultReal(SLICKEnergy::Option::NONPOLAR, SLICKEnergy::Default::NONPOLAR);
		float coeff_PS = options_.setDefaultReal(SLICKEnergy::Option::POLAR, SLICKEnergy::Default::POLAR);

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

		NonpolarSolvation* nps = new NonpolarSolvation(*this);
		nps->setCoefficient(coeff_NPS);
		nps->setup();
		nps->setNormalizationParameters(0.0, 0.0);
		insertComponent(nps);
	}

} // namespace BALL
