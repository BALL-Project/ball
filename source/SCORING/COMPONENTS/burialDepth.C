// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Marcel Schumann $
// ----------------------------------------------------

#include <BALL/SCORING/COMPONENTS/burialDepth.h>
#include <BALL/KERNEL/atom.h>

using namespace BALL;


BurialDepth::BurialDepth(ScoringFunction& sf)
	: ScoringComponent(sf)
{
	gridable_ = false;
	setName("BurialDepth");
}

BurialDepth::~BurialDepth()
{

}

void BurialDepth::update(const vector<std::pair<Atom*, Atom*> >& /*pair_vector*/)
{
	// no need to do anything here
}


double BurialDepth::updateScore()
{
	score_ = scoring_function_->getNoNeighboringReceptorAtoms();

	/*
	scaleScore();
	return score_;
	*/

	return getScaledScore();
}

