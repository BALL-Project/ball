// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Marcel Schumann $
// ----------------------------------------------------

#include <BALL/SCORING/COMPONENTS/advElectrostatic.h>
#include <BALL/SCORING/COMMON/scoringFunction.h>
#include <set>


using namespace BALL;


AdvancedElectrostatic::AdvancedElectrostatic(ScoringFunction* sf, Options& options, ForceFieldParameters& forcefield_parameters)
	: Electrostatic(options, forcefield_parameters)
{
	scoring_function_ = sf;
	protein_dielectric_constant_ = 4;
	solvate_dielectric_constant_ = 80;
	amber_nb_->setAdvancedElectrostatic(this);
	setName("Advanced Electrostatics");
	type_name_ = "ES";
}

AdvancedElectrostatic::AdvancedElectrostatic(ScoringFunction* sf, VanDerWaals* vdw)
	: Electrostatic(vdw)
{
	scoring_function_ = sf;
	protein_dielectric_constant_ = 4;
	solvate_dielectric_constant_ = 80;
	amber_nb_->setAdvancedElectrostatic(this);
	setName("Advanced Electrostatics");
	type_name_ = "ES";
}


double AdvancedElectrostatic::calculateDielectricConstant(Vector3& atom1, Vector3& atom2)
{
	const HashGrid3<Atom*>* hashgrid = scoring_function_->getHashGrid();
	///const HashGrid3<Atom*>* static_residues_hashgrid = scoring_function_->static_residues_hashgrid_;

	Vector3 vector = atom2-atom1;
	Size no_steps = (Size)(vector.getLength()/0.5); // stepsize = 0.5 Angstroem
	double solvate_fraction = 1;
	if (no_steps > 0)
	{
		vector /= no_steps;
		Size no_solvated_points = 0;

		for (Size i = 0; i < no_steps; i++)
		{
			const HashGridBox3<Atom*>* box = hashgrid->getBox(atom1+vector*i);
			if (box && box->isEmpty()) // if there are no receptor atoms in this hasgrid-box
			{
				no_solvated_points++;
			}
		}
		solvate_fraction = no_solvated_points/((double)no_steps);
	}
	double di = solvate_fraction*solvate_dielectric_constant_+(1-solvate_fraction)*protein_dielectric_constant_;

	/*
	if (atom2.getDistance(Vector3(26.787001, 2.755, 22.863001)) < 0.6)
	{
		bool show = 0;
		if (atom1.getDistance(Vector3(30.482599, 1.76832, 22.146601)) < 0.6)
		{
			cout<<" --- N ------------- "<<endl;
			show = 1;
		}
		else if (atom1.getDistance(Vector3(32.673199, 5.43216, 21.23430)) < 0.6)
		{
			cout<<" --- H ------------- "<<endl;
			show = 1;
		}
		if (show)
		{
			cout<<"no of solvated cells = "<<no_solvated_cells<<endl;
			cout<<"no of total cells = "<<visited_cells.size()<<endl;
			cout<<"dielectric constanct = "<<di<<endl;
			cout<<endl;
		}
	}*/

	return di;
}
