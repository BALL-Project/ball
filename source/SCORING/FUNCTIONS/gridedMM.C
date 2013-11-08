
// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Marcel Schumann $
// ----------------------------------------------------

#include <BALL/SCORING/FUNCTIONS/gridedMM.h>

#include <BALL/SCORING/COMPONENTS/advElectrostatic.h>
#include <BALL/SCORING/COMPONENTS/aromaticRingStacking.h>
#include <BALL/SCORING/COMPONENTS/solvation.h>
#include <BALL/SCORING/COMPONENTS/fragmentationalSolvation.h>
#include <BALL/SCORING/COMPONENTS/vanDerWaals.h>
#include <BALL/SCORING/COMPONENTS/hydrogenBond.h>
#include <BALL/SCORING/COMPONENTS/rotationalEntropy.h>
#include <BALL/SCORING/COMPONENTS/lipophilic.h>


using namespace BALL;


GridedMM::GridedMM(AtomContainer& receptor, AtomContainer& ligand, Options& options)
	: DiffGridBasedScoring(receptor, ligand, options)
{
	setup();
}


GridedMM::GridedMM(AtomContainer& receptor, Vector3& hashgrid_origin, Options& options)
	: DiffGridBasedScoring(receptor, hashgrid_origin, options)
{
	setup();
}


void GridedMM::setup()
{
	setName("GridedMM");

	AdvancedElectrostatic* es = new AdvancedElectrostatic(this, options_, forcefield_parameters_);
	es->setCoefficient(0.01);
	es->setScoringFunction(*this);
	scoring_components_.push_back(es);

	VanDerWaals* vdw = new VanDerWaals(es);
	vdw->setCoefficient(0.1);
	vdw->setScoringFunction(*this);
	scoring_components_.push_back(vdw);

	FragmentationalSolvation* solv = new FragmentationalSolvation(*this);
	solv->setCoefficient(19.823265);
	scoring_components_.push_back(solv);

	// HydrogenBond for h-bonds between receptor-hydrogens and ligand-acceptors.
	// The scores for this component will be precalculated and stored in the grid.
	HydrogenBond* hb = new HydrogenBond(*this, HydrogenBond::RECEPTOR_HYDROGENS);
	hb->selectBaseFunction("fermi");
	hb->setCoefficient(3);
	hb->setup();
	hb->setName("HB_rh");
	scoring_components_.push_back(hb);

	// HydrogenBond for h-bonds between ligand-hydrogens and receptor-acceptors.
	// The scores for this component cannot be precalculated and are calculated on-the-fly during docking.
	HydrogenBond* hb2 = new HydrogenBond(*this, HydrogenBond::LIGAND_HYDROGENS);
	hb2->selectBaseFunction("fermi");
	hb2->setCoefficient(3);
	hb2->setup();
	hb2->setName("HB_lh");
	scoring_components_.push_back(hb2);

	//RotationalEntropy will not be precalculated, thus we can use normalization
	//add ( = penalize) 1kJ/mol for each rotatable bond exceeding 14
	RotationalEntropy* re = new RotationalEntropy(*this);
	re->setNormalizationParameters(1, 14);
	re->setCoefficient(1);
	scoring_components_.push_back(re);

	/*
	// The following two components are used to penalize bad ligand conformations.
	// The score for a good conformation is scaled down according to option 'conformation_scale'.
	Electrostatic* lig_es = new Electrostatic(options_, forcefield_parameters_);
	lig_es->setLigandIntraMolecular(true);
	lig_es->setName("ES_lig");
	lig_es->setCoefficient(0.1);
	lig_es->setScoringFunction(*this);
	scoring_components_.push_back(lig_es);

	VanDerWaals* lig_vdw = new VanDerWaals(lig_es);
	lig_es->setLigandIntraMolecular(true);
	lig_vdw->setName("vdW_lig");
	lig_vdw->setCoefficient(0.1);
	lig_vdw->setScoringFunction(*this);
	scoring_components_.push_back(lig_vdw);
	*/
}


BALL::AtomTypes& GridedMM::getAtomTypes()
{
	return forcefield_parameters_.getAtomTypes();
}


// used during grid-calculation in order to fetch the ES-score
double GridedMM::getES()
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
