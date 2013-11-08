// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Marcel Schumann $
// ----------------------------------------------------

#include <BALL/SCORING/COMPONENTS/fragmentationalSolvation.h>
#include <BALL/KERNEL/PTE.h>
#include <map>

using namespace BALL;
using namespace std;


FragmentationalSolvation::FragmentationalSolvation(ScoringFunction& sf)
	: Solvation(sf)
{
	setName("FragmentationalSolvation");
	atom_pairwise_ = 0;
	type_name_ = "fragSolv";
}



void FragmentationalSolvation::update(const AtomPairVector& /*pair_vector*/)
{

}


double FragmentationalSolvation::updateScore()
{
	score_ = 0;

	const HashGrid3<Atom*>* hashgrid = 0;
	bool use_selection = scoring_function_->getLigand()->containsSelection();

	HashGrid3<Atom*>* nonconst_hashgrid = 0;
	if (!isLigandIntraMolecular())
	{
		hashgrid = scoring_function_->getHashGrid();

		nonconst_hashgrid = const_cast<HashGrid3<Atom*>* >(hashgrid);

		// Add contribution of ligand atoms to desolvation of ligand
		for(AtomIterator atom_it=scoring_function_->getLigand()->beginAtom(); !atom_it.isEnd(); atom_it++)
		{
			Vector3 cell = getAtomCell(&*atom_it,hashgrid);
			if(cell[0]>=0)
			{
				nonconst_hashgrid->insert((Position)cell[0],(Position)cell[1],(Position)cell[2],&*atom_it);
			}
		}
	}
	else
	{
		double resolution = 3.0;
		Vector3 center = scoring_function_->getLigandCenter();

		// no of cells on each axis for a resolution of 3 Angstroem
		int size = (int)(ceil(scoring_function_->getLigandRadius()*2/resolution)+1);

		hashgrid = scoring_function_->initializeHashGrid(scoring_function_->getLigand(), center, resolution, size);
	}

	for (AtomIterator atom_it = scoring_function_->getLigand()->beginAtom(); !atom_it.isEnd(); atom_it++)
	{
		if (use_selection && !atom_it->isSelected())
		{
			continue;
		}

		// get the fraction of the volume surrounding the current ligand atom that is occupied by receptor atoms
		double desolv_frac = getDesolvatedFraction(&*atom_it, hashgrid);

		String element = atom_it->getElement().getSymbol();
		map<String, double>::iterator it = solvation_parameters.find(element);
		if (it == solvation_parameters.end())
		{
			throw BALL::Exception::GeneralException(__FILE__, __LINE__, "FragmentationalSolvation::updateScore() error", "No solvation parameter for element "+element+" found!");
		}
		double par = it->second;

		it = solvation_volumes.find(element);
		if (it == solvation_volumes.end())
		{
			throw BALL::Exception::GeneralException(__FILE__, __LINE__, "FragmentationalSolvation::updateScore() error", "No solvation volume for element "+element+" found!");
		}
		double atom_score = par*it->second*desolv_frac;

		if (scoring_function_->storeInteractionsEnabled())
		{
			//double scaled_atom_score = atom_score;
			//scaled_atom_score = scaleScore(scaled_atom_score);
			//atom_it->addInteraction("fragSolv", scaled_atom_score);
			atom_it->addInteraction("fragSolv", scaleScore(atom_score));
		}

		score_ += atom_score;
	}

	if(!isLigandIntraMolecular())
	{
		// remove ligand atoms for hashgrid
		for(AtomIterator atom_it=scoring_function_->getLigand()->beginAtom(); !atom_it.isEnd(); atom_it++)
		{
			Vector3 cell = getAtomCell(&*atom_it,hashgrid);
			if(cell[0]>=0)
			{
				nonconst_hashgrid->remove((Position)cell[0],(Position)cell[1],(Position)cell[2],&*atom_it);
			}
		}
	}
	else
	{
		delete hashgrid;
	}

	/*
	scaleScore();
	return score_;
	*/

	return getScaledScore();
}



double FragmentationalSolvation::getDesolvatedFraction(const Atom* atom, const HashGrid3<Atom*>* hashgrid)
{
	int x_size = (int)hashgrid->getSizeX();
	int y_size = (int)hashgrid->getSizeY();
	int z_size = (int)hashgrid->getSizeZ();

	Vector3 lig_atom_cell = (atom->getPosition() - hashgrid->getOrigin());
	lig_atom_cell[0] /= hashgrid->getUnit()[0];
	lig_atom_cell[1] /= hashgrid->getUnit()[1];
	lig_atom_cell[2] /= hashgrid->getUnit()[2];

	// no desolvation of ligand atom if this atom is outside of grid
	if (lig_atom_cell[0] < 0 || lig_atom_cell[0] > x_size || lig_atom_cell[1] < 0 || lig_atom_cell[1] > y_size || lig_atom_cell[2] < 0 || lig_atom_cell[2] > z_size )
	{
		return 0;
	}

	if (hashgrid->getUnit()[0] > 4 || hashgrid->getUnit()[0] < 3 )
	{
		throw BALL::Exception::GeneralException(__FILE__, __LINE__, "FragmentationalSolvation error", "The ScoringFunction's hashgrid resolution must be between 3 and 4 Angstroem in order to use FragmentationalSolvation!");
	}

	Size radius = 1;

	// indices in HashGrid, where the search for interacting target atoms should begin ( != position of ligand atom)
	int i = static_cast < int > (lig_atom_cell[0]-radius); if (i < 0){i = 0; }
	int j0 = static_cast < int > (lig_atom_cell[1]-radius); if (j0 < 0){j0 = 0; }
	int k0 = static_cast < int > (lig_atom_cell[2]-radius); if (k0 < 0){k0 = 0; }

	Size occupied_cells = 0;

	for (; i <= lig_atom_cell[0]+radius && i < x_size; i++)
	{
		for (int j = j0; j <= lig_atom_cell[1]+radius && j < y_size; j++)
		{
			for (int k = k0; k <= lig_atom_cell[2]+radius && k < z_size; k++)
			{
				const HashGridBox3<Atom*>* box = hashgrid->getBox(i, j, k);

				if (!box->isEmpty())
				{
					// count only if Box contains not only the Atom whose desolvated volume-fraction is to be estimated
					HashGridBox3<Atom*>::ConstDataIterator d_it = box->beginData();
					if (*d_it != atom || ++d_it != box->endData())
					{
						occupied_cells++;
					}
				}
			}
		}
	}

	double no_neighbor_cell = pow((double)(radius*2+1), 3);  // radius of 1 cell == > 3 cells on each axis

	return occupied_cells/no_neighbor_cell;
}


Vector3 FragmentationalSolvation::getAtomCell(Atom* atom, const HashGrid3<Atom*>* hashgrid)
{
	Vector3 cell(-1, -1, -1);

	int x_size = (int)hashgrid->getSizeX();
	int y_size = (int)hashgrid->getSizeY();
	int z_size = (int)hashgrid->getSizeZ();

	cell = (atom->getPosition() - hashgrid->getOrigin());
	cell[0] /= hashgrid->getUnit()[0];
	cell[1] /= hashgrid->getUnit()[1];
	cell[2] /= hashgrid->getUnit()[2];

	if (cell[0] < 0 || cell[0] > x_size || cell[1] < 0 || cell[1] > y_size || cell[2] < 0 || cell[2] > z_size )
	{
		return Vector3(-1, -1, -1);
	}
	return cell;
}
