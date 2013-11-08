// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Marcel Schumann $
// ----------------------------------------------------

#include <BALL/SCORING/COMPONENTS/vanDerWaals.h>
#include <BALL/SCORING/COMPONENTS/electrostatic.h>
#include <BALL/SCORING/COMMON/scoringFunction.h>


using namespace BALL;


VanDerWaals::VanDerWaals(Options& options, ForceFieldParameters& forcefield_parameters)
{
	amber_nb_ = new AmberNonBonded;
	amber_nb_->setup(options, forcefield_parameters);
	do_calculations_ = 1;
	setName("vdW");
	type_name_ = "vdW";
}


VanDerWaals::VanDerWaals(Electrostatic* es)
{
	amber_nb_ = es->getAmberNonBonded();
	do_calculations_ = 0;
	ligand_intra_molecular_ = es->isLigandIntraMolecular();
	setName("vdW");
	type_name_ = "vdW";
}

VanDerWaals::~VanDerWaals()
{
	if (do_calculations_) delete amber_nb_;
}

void VanDerWaals::update(const vector<std::pair<Atom*, Atom*> >& pair_vector)
{
	if (do_calculations_)
	{
		amber_nb_->update(pair_vector);
	}
}


double VanDerWaals::updateScore()
{
	if (do_calculations_)
	{
		bool b = scoring_function_->storeInteractionsEnabled();
		amber_nb_->enableStoreInteractions(b);
		amber_nb_->updateEnergy();
	}

	score_ = amber_nb_->getVdwEnergy();

	/*
	scaleScore();
	return score_;
	*/

	return getScaledScore();
}


AmberNonBonded* VanDerWaals::getAmberNonBonded()
{
	return amber_nb_;
}
