// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Marcel Schumann $
// ----------------------------------------------------

#include <BALL/SCORING/FUNCTIONS/rescoring3D.h>
#include <BALL/KERNEL/molecularInteractions.h>


using namespace BALL;
using namespace std;

Rescoring3D::Rescoring3D(AtomContainer& receptor, AtomContainer& reference_ligand, Options& options, String free_energy_label, ScoringFunction* sf)
    : Rescoring(receptor, reference_ligand, options, free_energy_label, sf)
{
    name_ = "Rescoring3D";
    use_calibration_ = 1;
    setup_();
}

void Rescoring3D::setup_()
{
    scoring_function_->enableStoreInteractions(1);

    const HashGrid3<Atom*>* sf_hashgrid = scoring_function_->getHashGrid();
    sizeX_ = sf_hashgrid->getSizeX();
    sizeY_ = sf_hashgrid->getSizeY();
    sizeZ_ = sf_hashgrid->getSizeZ();
    resolution_ = sf_hashgrid->getUnit()[0];
    origin_ = sf_hashgrid->getOrigin();
    reg3D_grid_names_.push_back("rescore3D grid");
}


void Rescoring3D::generateAtomScoreContributions_(const Atom* atom, vector<double>& contributions)
{
    if (!atom)
    {
        contributions.resize(1, 0);
        return;
    }
    if (contributions.size() == 0) contributions.resize(1, 0);
    if (atom->interactions) contributions[0] += atom->interactions->getInteractionEnergy();
}


void Rescoring3D::generateScoreContributions_(Molecule* mol, vector<vector<double> >* matrix, vector<double>* v)
{
    HashGrid3<Atom*> hashgrid(origin_, sizeX_, sizeY_, sizeZ_, resolution_);

    // add all atoms of 'mol' to hashgrid
    Vector3 origin = hashgrid.getOrigin();
    Size hashgrid_size = hashgrid.getSizeX();
    for (AtomIterator it = mol->beginAtom(); +it; it++)
    {
        // position of the current atoms within the HashGrid
        Vector3 atom_pos = it->getPosition()-origin;

        atom_pos[0] /= hashgrid.getUnit()[0];
        atom_pos[1] /= hashgrid.getUnit()[1];
        atom_pos[2] /= hashgrid.getUnit()[2];

        // insert all target atoms that are located within the grid boundaries
        if (atom_pos[0] >= 0 && atom_pos[0] < hashgrid_size && atom_pos[1] >= 0 && atom_pos[1] < hashgrid_size && atom_pos[2] >= 0 && atom_pos[2] < hashgrid_size )
        {
            hashgrid.insert(it->getPosition(), &*it);
        }
    }

    //	sum up the score of each box
    Size cell_no = 0;
    for (Size i = 0; i < hashgrid.getSizeX(); i++)
    {
        for (Size j = 0; j < hashgrid.getSizeY(); j++)
        {
            for (Size k = 0; k < hashgrid.getSizeZ(); k++)
            {
                vector<double> box_scores(0, 0);

                // initializes vector with appropriate size
                generateAtomScoreContributions_(0, box_scores);

                HashGridBox3<Atom*>* box = hashgrid.getBox(i, j, k);
                for (HashGridBox3 < Atom* > ::DataIterator di = box->beginData(); di != box->endData(); di++)
                {
                    generateAtomScoreContributions_(*di, box_scores);
                }
                for (Size s = 0; s < box_scores.size(); s++, cell_no++)
                {
                    if (matrix)
                    {
                        (*matrix)[cell_no].push_back(box_scores[s]);
                    }
                    else if (v)
                    {
                        v->push_back(box_scores[s]);
                    }
                }
            }
        }
    }
}

list<pair<String, RegularData3D*> > Rescoring3D::generateRegularData3DGrids()
{
    if (!model_)
    {
        throw BALL::Exception::GeneralException(__FILE__, __LINE__, "Rescoring3D::generateRegularData3DGrids() Error", "No existing model!");
    }
    const Eigen::MatrixXd* coefficients = model_->getTrainingResult();
    const vector<string>* names = model_->getDescriptorNames();
    if (sizeX_*sizeY_*sizeZ_ < coefficients->rows())
    {
        throw BALL::Exception::GeneralException(__FILE__, __LINE__, "Rescoring3D::generateRegularData3DGrids() Error", "Number of coefficients larger than number of grid cells!");
    }

    list<pair<String, RegularData3D*> > reg3d_list;


    Vector3 resolution(resolution_, resolution_, resolution_);
    Vector3 dimension(resolution_*sizeX_, resolution_*sizeY_, resolution_*sizeZ_);

    vector<RegularData3D*> grids;
    Size no_reg3D_grids = reg3D_grid_names_.size();
    for (Size g = 0; g < no_reg3D_grids; g++)
    {
        RegularData3D* reg3d = new RegularData3D(origin_, dimension, resolution);
        grids.push_back(reg3d);
        reg3d_list.push_back(make_pair(reg3D_grid_names_[g], reg3d));
    }

    vector<string>::const_iterator name_it = names->begin();
    int coeff_index = 1;
    Size cell_no = 0;
    for (Size i = 0; i < sizeX_; i++)
    {
        for (Size j = 0; j < sizeY_; j++)
        {
            for (Size k = 0; k < sizeZ_; k++)
            {
                for (Size g = 0; g < no_reg3D_grids; g++, cell_no++)
                {
                    String name;
                    if (name_it == names->end()) // cell was removed by feature selection
                    {
                        RegularData3D::IndexType index(i, j, k);
                        grids[g]->getData(index) = 0;
                    }
                    else if ((name = *name_it).isDigit())
                    {
                        Size coeff_no = (Size)name.toFloat();
                        RegularData3D::IndexType index(i, j, k);

                        if (coeff_no == cell_no)
                        {
                            grids[g]->getData(index) = (*coefficients)(coeff_index, 1);
                            name_it++;
                            coeff_index++;
                        }
                        else // cell was removed by feature selection
                        {
                            grids[g]->getData(index) = 0;
                        }
                    }
                    else
                    {
                        throw BALL::Exception::GeneralException(__FILE__, __LINE__, "Rescoring3D::generateRegularData3DGrids() Error", "Non-numeric feature label found!");
                    }
                }
            }
        }
    }

    return reg3d_list;
}
