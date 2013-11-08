// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Marcel Schumann $
// ----------------------------------------------------

#include <BALL/SCORING/COMPONENTS/electrostatic.h>
#include <BALL/SCORING/COMPONENTS/vanDerWaals.h>
#include <BALL/SCORING/COMMON/scoringFunction.h>


using namespace BALL;


Electrostatic::Electrostatic(Options& options, ForceFieldParameters& forcefield_parameters)
{
	amber_nb_ = new AmberNonBonded;
	amber_nb_->setup(options, forcefield_parameters);
	do_calculations_ = 1;
	setName("Electrostatics");
	type_name_ = "ES";
}

Electrostatic::Electrostatic(VanDerWaals* vdw)
{
	amber_nb_ = vdw->getAmberNonBonded();
	do_calculations_ = 0;
	ligand_intra_molecular_ = vdw->isLigandIntraMolecular();
	setName("Electrostatics");
	type_name_ = "ES";
}

Electrostatic::~Electrostatic()
{
	if (do_calculations_) delete amber_nb_;
}

void Electrostatic::update(const vector<std::pair<Atom*, Atom*> >& pair_vector)
{
	if (do_calculations_)
	{
		amber_nb_->update(pair_vector);
	}
}


double Electrostatic::updateScore()
{
	if (do_calculations_)
	{
		bool b = scoring_function_->storeInteractionsEnabled();
		amber_nb_->enableStoreInteractions(b);
		amber_nb_->updateEnergy();
	}

	score_ = amber_nb_->getElectrostaticEnergy();

	//scaleScore();

	// use threshold for unrealistically good amber-ES energies due to consideration of only point-charges
	/// TODO: convert this to a value for each ligand atom == each grid cell
	/// else thresholds will be different when using grid-based and non-grid-based scoring
	//double threshold = scoring_function_->getNoLigandAtoms()*-10;
	//if (score < threshold) score_ = threshold;
	//if (score_ < -40) score_ = -40;

	//return score_;
	return getScaledScore();
}


AmberNonBonded* Electrostatic::getAmberNonBonded()
{
	return amber_nb_;
}
