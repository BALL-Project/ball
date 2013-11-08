// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Marcel Schumann $
// ----------------------------------------------------

#include <BALL/SCORING/FUNCTIONS/MMScoring.h>
#include <BALL/MOLMEC/COMMON/forceFieldComponent.h>
#include <BALL/MOLMEC/AMBER/amberNonBonded.h>

#include <BALL/SCORING/COMPONENTS/aromaticRingStacking.h>
#include <BALL/SCORING/COMPONENTS/solvation.h>
#include <BALL/SCORING/COMPONENTS/fragmentationalSolvation.h>
#include <BALL/SCORING/COMPONENTS/advElectrostatic.h>
#include <BALL/SCORING/COMPONENTS/electrostatic.h>
#include <BALL/SCORING/COMPONENTS/vanDerWaals.h>
#include <BALL/SCORING/COMPONENTS/hydrogenBond.h>
#include <BALL/SCORING/COMPONENTS/rotationalEntropy.h>
#include <BALL/SCORING/COMPONENTS/lipophilic.h>
#include <BALL/SCORING/COMPONENTS/burialDepth.h>


using namespace BALL;


MMScoring::MMScoring(AtomContainer& receptor, AtomContainer& reference_ligand, Options& options)
	: DifferentiableScoringFunction(receptor, reference_ligand, options)
{
	setup();
}


MMScoring::MMScoring(AtomContainer& receptor, Vector3& hashgrid_origin, Options& options)
	: DifferentiableScoringFunction(receptor, hashgrid_origin, options)
{
	setup();
}


void MMScoring::setup()
{
	ScoringFunction::setName("MMScoring");

	AdvancedElectrostatic* es = new AdvancedElectrostatic(this, options_, forcefield_parameters_);
	es->setCoefficient(0.01);
	scoring_components_.push_back(es);

	VanDerWaals* vdw = new VanDerWaals(es);
	vdw->setCoefficient(0.1);
	vdw->setScoringFunction(*this);
	scoring_components_.push_back(vdw);

	FragmentationalSolvation* solv = new FragmentationalSolvation(*this);
	solv->setCoefficient(19.823265);
	scoring_components_.push_back(solv);

	HydrogenBond* hb = new HydrogenBond(*this, HydrogenBond::ALL_HYDROGENS);
	hb->selectBaseFunction("fermi");
	hb->setCoefficient(3);
	hb->setup();
	scoring_components_.push_back(hb);

	//add ( = penalize) 1kJ/mol for each rotatable bond exceeding 14
	RotationalEntropy* re = new RotationalEntropy(*this);
	re->setNormalizationParameters(1, 14);
	re->setCoefficient(1);
	scoring_components_.push_back(re);
}


double MMScoring::getES()
{
	double es_score = 0;

	for (vector<ScoringComponent*> ::iterator it = scoring_components_.begin(); it != scoring_components_.end(); ++it)
	{
		Electrostatic* es = dynamic_cast<Electrostatic*>(*it);
		if (!(*it)->isLigandIntraMolecular() && (*it)->isEnabled() && es != NULL)
		{
			es_score += es->getScaledScore();
		}
	}

	return es_score;
}
