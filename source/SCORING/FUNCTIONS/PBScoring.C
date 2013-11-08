// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Marcel Schumann $
// ----------------------------------------------------

#include <BALL/SCORING/FUNCTIONS/PBScoring.h>
#include <BALL/SCORING/COMPONENTS/PB.h>

using namespace BALL;


PBScoring::PBScoring(AtomContainer& receptor, AtomContainer& ligand, Options& options)
	: ScoringFunction(receptor, ligand, options)
{
	setup();
}


PBScoring::PBScoring(AtomContainer& receptor, Vector3& hashgrid_origin, Options& options)
	: ScoringFunction(receptor, hashgrid_origin, options)
{
	setup();
}


void PBScoring::setup()
{
	ScoringFunction::setName("PBScoring");
	PB* pb = new PB(*this);
	pb->setName("PB");
	pb->setScoringFunction(*this);
	pb->setLigandIntraMolecular(0);
	scoring_components_.push_back(pb);
}


double PBScoring::getES()
{
	double es = 0;
	return es;
}
