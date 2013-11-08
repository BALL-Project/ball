// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Marcel Schumann $
// ----------------------------------------------------

#include <BALL/SCORING/COMPONENTS/solvation.h>
#include <BALL/KERNEL/PTE.h>

using namespace BALL;
using namespace std;



Solvation::Solvation(ScoringFunction& sf)
	: ScoringComponent(sf)
{
	setName("Solvation");

	// Parameters taken from Autodock4, AD4_PARM99.dat
	solvation_parameters["H"] = 0.00051;
	solvation_parameters["N"] = -0.00162;
	solvation_parameters["C"] = -0.00143;
	solvation_parameters["O"] = -0.00251;
	solvation_parameters["S"] = -0.00214;
	solvation_parameters["Cl"] = -0.00110;
	solvation_parameters["Ca"] = -0.00110;
	solvation_parameters["Mn"] = -0.00110;
	solvation_parameters["Mg"] = -0.00110;
	solvation_parameters["Fe"] = -0.00110;
	solvation_parameters["Zn"] = -0.00110;
	solvation_parameters["Br"] = -0.00110;
	solvation_parameters["I"] = -0.00110;
	solvation_parameters["F"] = -0.00110;
	solvation_parameters["Cs"] = -0.00110;
	solvation_parameters["P"] = -0.00110;

	solvation_volumes["H"] = 0.9;
	solvation_volumes["N"] = 25.42;
	solvation_volumes["C"] = 29.10;
	solvation_volumes["O"] = 19.20;
	solvation_volumes["S"] = 33.51;
	solvation_volumes["Cl"] = 30.96;
	solvation_volumes["Ca"] = 21.06;
	solvation_volumes["Mn"] = 2.1400; // from AD4_parameters.dat
	solvation_volumes["Mg"] = 2.09;
	solvation_volumes["Fe"] = 1.8400; // from AD4_parameters.dat
	solvation_volumes["Zn"] = 5.58;
	solvation_volumes["Br"] = 45.83;
	solvation_volumes["I"] = 54.36;
	solvation_volumes["F"] = 22.45;
	solvation_volumes["Cs"] = 163.91;
	solvation_volumes["P"] = 38.79;

	type_name_ = "solv";
}



void Solvation::update(const AtomPairVector& pair_vector)
{
	pair_vector_ = pair_vector;
}


double Solvation::updateScore()
{
	score_ = 0;

	for (AtomPairVector::iterator it = pair_vector_.begin(); it != pair_vector_.end(); it++)
	{
		const Atom* atom_i = it->first;
		const Atom* atom_j = it->second;

		double distance = atom_i->getPosition().getDistance(atom_j->getPosition());

		String element_i = atom_i->getElement().getSymbol();
		map<String, double>::iterator search_it = solvation_parameters.find(element_i);
		if (search_it == solvation_parameters.end())
		{
			throw BALL::Exception::GeneralException(__FILE__, __LINE__, "Solvation::updateScore() error", "No solvation parameter for element "+element_i+" found!");
		}
		double par_i = search_it->second;

		String element_j = atom_j->getElement().getSymbol();
		search_it = solvation_volumes.find(element_j);
		if (search_it == solvation_volumes.end())
		{
			throw BALL::Exception::GeneralException(__FILE__, __LINE__, "Solvation::updateScore() error", "No solvation volume for element "+element_j+" found!");
		}
		double vol_j = search_it->second;

		score_ += par_i*vol_j*exp((-distance*distance)/(2*3.5*3.5));
	}

	/*
	scaleScore();
	return score_;
	*/

	return getScaledScore();
}


