// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Marcel Schumann $
// ----------------------------------------------------

#include <BALL/SCORING/COMPONENTS/rotationalEntropy.h>
#include <BALL/SCORING/COMMON/scoringFunction.h>

using namespace BALL;

RotationalEntropy::RotationalEntropy(ScoringFunction& sf)
	: ScoringComponent(sf)
{
	ligand_intra_molecular_ = 0;
	gridable_ = 0;
	atom_pairwise_ = 0;
	setName("(nRot>14)");
	type_name_ = "nRot";
}


void RotationalEntropy::setLigandIntraMolecular(bool b)
{
	if (b == true)
	{
		std::cout<<"RotationalEntropy ScoringComponent can not be used to compute ligand conformation score !"<<std::endl;
	}
}


void RotationalEntropy::update(const std::vector<std::pair<Atom*, Atom*> >& /*pair_vector*/)
{
	// nothing needs to be done here
}


double RotationalEntropy::updateScore()
{
	score_ = scoring_function_->getRotatableLigandBonds()->size();

	if (score_ == 0 && scoring_function_->getStaticLigandFragments()->size() == 0)
	{
		throw BALL::Exception::GeneralException(__FILE__, __LINE__, "RotationalEntropy::updateScore() error", "Rotatable bonds have not been searched yet; you therefore need to set Option 'use_static_lig_fragments' to true !");
	}

	// prevent score-decrease for small ligands,
	// since we only want to increase the score ( = add penalty) for ligands with very many rotatable bonds
	if (score_ < 14)
	{
		score_ = 0;
		return 0;
	}

	/*
	scaleScore();
	return score_;
	*/

	return getScaledScore();
}

