
// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Marcel Schumann $
// ----------------------------------------------------

#include <BALL/SCORING/COMPONENTS/PB.h>
#include <BALL/SCORING/COMMON/scoringFunction.h>
#include <BALL/SYSTEM/timer.h>
#include <BALL/KERNEL/PTE.h>

using namespace BALL;
using namespace std;

PB::PB(ScoringFunction& sf)
	: ScoringComponent(sf)
{
	pb_solver_ = new FDPB;

	// FDPB uses Atom::getRadius() instead of Element::getVanDerWaalsRadius(), for whatever reason.
	// However, Atom::radius_ is not set by default, so we have to do it here.
	for (AtomIterator it = scoring_function_->getLigand()->beginAtom(); +it; it++)
	{
		it->setRadius(it->getElement().getVanDerWaalsRadius());
	}
	for (AtomIterator it = scoring_function_->getReceptor()->beginAtom(); +it; it++)
	{
		it->setRadius(it->getElement().getVanDerWaalsRadius());
	}
}


PB::~PB()
{
	delete pb_solver_;
}


void PB::setupLigand()
{
	// FDPB uses Atom::getRadius() instead of Element::getVanDerWaalsRadius(), for whatever reason.
	// However, Atom::radius_ is not set by default, so we have to do it here.
	for (AtomIterator it = scoring_function_->getLigand()->beginAtom(); +it; it++)
	{
		it->setRadius(it->getElement().getVanDerWaalsRadius());
	}
}


void PB::update(const vector<std::pair<Atom*, Atom*> >& pair_vector)
{
	// Clear-functions will automatically erase all data that was dynamically allocated by the last call of this function
	receptor_atoms_.clear();
	ligand_atoms_.clear();
	complex_atoms_.clear();

	Molecule* receptor_mol = new Molecule;
	Molecule* ligand_mol = new Molecule;
	Molecule* complex_mol = new Molecule;
	receptor_atoms_.insert(*receptor_mol);
	ligand_atoms_.insert(*ligand_mol);
	complex_atoms_.insert(*complex_mol);

	// At the moment we need to copy the relevant atoms and insert them into a dummy-system, because FDPB can only work on Systems (not an atom-list).
	set<Atom*> copied_atoms;
	for (vector < std::pair < Atom*, Atom* > > ::const_iterator it = pair_vector.begin(); it != pair_vector.end(); it++)
	{
		if (copied_atoms.find(it->first) == copied_atoms.end())
		{
			ligand_mol->insert(*new Atom(*it->first));
			complex_mol->insert(*new Atom(*it->first));
			copied_atoms.insert(it->first);
		}

		if (copied_atoms.find(it->second) == copied_atoms.end())
		{
			receptor_mol->insert(*new Atom(*it->second));
			complex_mol->insert(*new Atom(*it->second));
			copied_atoms.insert(it->second);
		}
	}
}


double PB::updateScore()
{
	Timer timer;
	timer.start();

	Options options = scoring_function_->getOptions();

	// calculate energy for receptor in vacuum
	options.set(FDPB::Option::SOLVENT_DC, 1);
	pb_solver_->setup(receptor_atoms_, options);
	pb_solver_->solve();
	double rec_vac = pb_solver_->getEnergy();

	// calculate energy for solvated receptor
	options.set(FDPB::Option::SOLVENT_DC, 78);
	pb_solver_->setup(receptor_atoms_, options);
	pb_solver_->solve();
	double rec_solv = pb_solver_->getEnergy();

	// calculate energy for ligand in vacuum
	options.set(FDPB::Option::SOLVENT_DC, 1);
	pb_solver_->setup(ligand_atoms_, options);
	pb_solver_->solve();
	double lig_vac = pb_solver_->getEnergy();

	// calculate energy for solvated ligand
	options.set(FDPB::Option::SOLVENT_DC, 78);
	pb_solver_->setup(ligand_atoms_, options);
	pb_solver_->solve();
	double lig_solv = pb_solver_->getEnergy();

	// calculate energy for complex in vacuum
	options.set(FDPB::Option::SOLVENT_DC, 1);
	pb_solver_->setup(complex_atoms_, options);
	pb_solver_->solve();
	double com_vac = pb_solver_->getEnergy();

	// calculate energy for solvated complex
	options.set(FDPB::Option::SOLVENT_DC, 78);
	pb_solver_->setup(complex_atoms_, options);
	pb_solver_->solve();
	double com_solv = pb_solver_->getEnergy();

	score_ = (com_solv-com_vac)-(rec_solv-rec_vac)-(lig_solv-lig_vac);
	//scaleScore();

	cout<<"  rec_vac = "<<rec_vac<<endl;
	cout<<"  rec_solv = "<<rec_solv<<endl;
	cout<<"  lig_vac = "<<lig_vac<<endl;
	cout<<"  lig_solv = "<<lig_solv<<endl;
	cout<<"  com_vac = "<<com_vac<<endl;
	cout<<"  com_solv = "<<com_solv<<endl;
	cout<<"  ----------------------"<<endl;
	cout<<"  total PB score = "<< getScaledScore() <<endl<<flush;

	timer.stop();
	Log<<"Time used by PB::updateScore(): "<<timer.getClockTime()<<endl;

	//return score_;
	return getScaledScore();
}

