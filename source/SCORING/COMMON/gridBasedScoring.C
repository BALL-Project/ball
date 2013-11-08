// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Marcel Schumann $
// ----------------------------------------------------

#include <BALL/SCORING/COMMON/gridBasedScoring.h>
#include <BALL/SYSTEM/timer.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/residue.h>
#include <BALL/STRUCTURE/structureMapper.h>
#include <BALL/STRUCTURE/residueRotamerSet.h>
#include <BALL/SYSTEM/path.h>

#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>


using namespace BALL;
using namespace std;

const char* GridBasedScoring ::Option::SCOREGRID_RESOLUTION = "scoregrid_resolution";
const char* GridBasedScoring ::Option::SCOREGRID_INTERPOLATION="scoregrid_interpolation";
double GridBasedScoring::Default::SCOREGRID_RESOLUTION = 0.5;
bool GridBasedScoring::Default::SCOREGRID_INTERPOLATION = 0;


GridBasedScoring::GridBasedScoring(AtomContainer& receptor, AtomContainer& ligand, Options& options)
	: ScoringFunction(receptor, ligand, options)
{
	setup();
}


GridBasedScoring::GridBasedScoring(AtomContainer& receptor, Vector3& hashgrid_origin, Options& options)
	: ScoringFunction(receptor, hashgrid_origin, options)
{
	setup();
}


GridBasedScoring::~GridBasedScoring()
{
	for (Size i = 0; i < grid_sets_.size(); i++)
	{
		delete grid_sets_[i];
	}
}


void GridBasedScoring::setup()
{
	combine_operation_ = 0; // default: summing up of grid scores

	scoregrid_resolution_ = options_.setDefaultReal(Option::SCOREGRID_RESOLUTION, Default::SCOREGRID_RESOLUTION);
	scoregrid_interpolation_ = options_.setDefaultBool(Option::SCOREGRID_INTERPOLATION, Default::SCOREGRID_INTERPOLATION);

	// set default types
	atom_types_map_.insert(make_pair("0_ELECTROSTATIC", 0));
	atom_types_map_.insert(make_pair("1_INTERACTIONS", 1));
	atom_types_map_.insert(make_pair("C", 2));
	atom_types_map_.insert(make_pair("H", 3));
	atom_types_map_.insert(make_pair("N", 4));
	atom_types_map_.insert(make_pair("O", 5));
	atom_types_map_.insert(make_pair("P", 6));
	atom_types_map_.insert(make_pair("S", 7));
	atom_types_map_.insert(make_pair("Cl", 8));
	atom_types_map_.insert(make_pair("F", 9));
	atom_types_map_.insert(make_pair("I", 10));

	Vector3 size(hashgrid_->getSizeX(), hashgrid_->getSizeY(), hashgrid_->getSizeZ());
	size = size*resolution_; // = size in units of Angstroem

	size.x /= scoregrid_resolution_; // = size in units of ScoreGrid cells
	size.y /= scoregrid_resolution_;
	size.z /= scoregrid_resolution_;

	defineReceptorGridSet(hashgrid_->getOrigin(), size, 1e10, 1, scoregrid_resolution_);
	flex_gridset_id_ = -1;

	if (!flexible_residues_.empty())
	{
		defineFlexibleResiduesGridSet();
	}
}


void GridBasedScoring::setupReferenceLigand()
{
	Log.level(20)<<"average no of neighboring target atoms for each reference ligand:"<<endl;

// 	Log<<"number of ScoreGridSets: "<<grid_sets_.size()<<endl<<flush;
// 	Log<<grid_sets_[0].out_of_grid_penalty<<endl;
//  	Log<<grid_sets_[0][0].size()<<endl<<flush;
// 	if(grid_sets_.size()>1)
// 	{
// 		Log<<grid_sets_[1][0].size()<<endl<<flush;
// 		Log<<grid_sets_[1].out_of_grid_penalty<<endl;
// 	}

	for (Size set = 0; set < grid_sets_.size(); set++)
	{
		double neighbors = 0;
		int atoms = 0;
		for (AtomIterator it = ligand_->beginAtom(); it != ligand_->endAtom(); it++)
		{
			atoms++;
			Vector3 pos = it->getPosition();
			neighbors += grid_sets_[set]->getGridScore(1, pos, scoregrid_interpolation_);
		}
		grid_sets_[set]->reference_interactions = (Size)(neighbors/atoms);
		Log.level(20)<<"ScoreGridSet "<<set<<" : "<<grid_sets_[set]->reference_interactions<<endl;
	}
}


void GridBasedScoring::setCombineOperation(int i)
{
	if (i > 3 || i < 0)
	{
		throw Exception::GeneralException(__FILE__, __LINE__, "ScoreGridSet::setCombineOperation() error", "Only grid combine operations 0 to 3 are defined!");
	}
	combine_operation_ = i;
}


void GridBasedScoring::setAtomTypeNames(set<String>& types)
{
	atom_types_map_.clear();
	atom_types_map_.insert(make_pair("0_ELECTROSTATIC", 0));
	atom_types_map_.insert(make_pair("1_INTERACTIONS", 1));

	int i = 2;
	for (set < String > ::iterator it = types.begin(); it != types.end(); it++)
	{
		atom_types_map_.insert(make_pair(*it, i));
		i++;
	}
}


BALL::String GridBasedScoring::getGridAtomTypeName(int grid_id)
{
	for (map < String, int > ::iterator it = atom_types_map_.begin(); it != atom_types_map_.end(); it++)
	{
		if (it->second == grid_id) return it->first;
	}

	String mess = "No ScoreGrid with ID="+String(grid_id)+" !!";
	throw BALL::Exception::GeneralException(__FILE__, __LINE__, "GridBasedScoring::precalculateGrids() error", mess);

	return "";
}


std::map<BALL::String, int>* GridBasedScoring::getAtomTypesMap()
{
	return &atom_types_map_;
}


// define ScoreGridsSet according to dimensions of ScoringFunction::hashgrid_
void GridBasedScoring::defineReceptorGridSet(Vector3& origin, Vector3& size, double out_of_grid_penalty, bool enforce_grid_boundaries, double resolution, double interaction_no_scale)
{
	ScoreGridSet* sgs = new ScoreGridSet(this, origin, size, resolution);
	sgs->out_of_grid_penalty_ = out_of_grid_penalty;
	sgs->enforce_grid_boundaries_ = enforce_grid_boundaries;
	sgs->resolution_ = resolution;
	sgs->interaction_no_scale_ = interaction_no_scale;

	// if no reference ligand has been set, reference_neighbors_ == 0
	sgs->reference_interactions = reference_neighbors_;

	grid_sets_.push_back(sgs);
}


void GridBasedScoring::defineGridSet(AtomContainer* sys, Vector3& angstroem, double out_of_grid_penalty, bool enforce_grid_boundaries, double resolution, double interaction_no_scale)
{
	Vector3 center = calculateGeometricalCenter(sys);
	Vector3 origin = center - angstroem/2;
	double size_x = angstroem.x/resolution;
	double size_y = angstroem.y/resolution;
	double size_z = angstroem.z/resolution;
	Vector3 size(size_x, size_y, size_z);
	ScoreGridSet* sgs = new ScoreGridSet(this, origin, size, resolution);
	sgs->out_of_grid_penalty_ = out_of_grid_penalty;
	sgs->enforce_grid_boundaries_ = enforce_grid_boundaries;
	sgs->resolution_ = resolution;
	sgs->interaction_no_scale_ = interaction_no_scale;

	grid_sets_.push_back(sgs);

	// use ScoringFunction::resolution_ as resolution of the new hashgrid (!=resolution of the ScoreGridSet), so that ScoringFunction::createNonbondedPairs() will work correctly
	int no_hash_cells = (int)(angstroem[0]/ScoringFunction::resolution_);
	sgs->setHashGrid(initializeHashGrid(sys, center, ScoringFunction::resolution_, no_hash_cells));
}


void GridBasedScoring::defineGridSet(AtomContainer* sys, double out_of_grid_penalty, bool enforce_grid_boundaries, double resolution, double interaction_no_scale)
{
	Vector3 center = calculateGeometricalCenter(sys);
	double radius = calculateMoleculeRadius(sys, center);
	radius += nonbonded_cutoff_; // make grid large enough to encompass everything within nonbonded_cutoff_ of each atom of sys

	double size_x = 2*radius/resolution;
	double size_y = 2*radius/resolution;
	double size_z = 2*radius/resolution;

	Vector3 size(size_x, size_y, size_z);
	Vector3 v(radius, radius, radius);
	Vector3 origin = center - v;

	ScoreGridSet* sgs = new ScoreGridSet(this, origin, size, resolution);
	sgs->out_of_grid_penalty_ = out_of_grid_penalty;
	sgs->enforce_grid_boundaries_ = enforce_grid_boundaries;
	sgs->resolution_ = resolution;
	sgs->interaction_no_scale_ = interaction_no_scale;

	grid_sets_.push_back(sgs);

	// use ScoringFunction::resolution_ as resolution of the new hashgrid (!=resolution of the ScoreGridSet), so that ScoringFunction::createNonbondedPairs() will work correctly
	int no_hash_cells = (int)((2*radius)/ScoringFunction::resolution_);
	sgs->setHashGrid(initializeHashGrid(sys, center, ScoringFunction::resolution_, no_hash_cells));
}


void GridBasedScoring::defineFlexibleResiduesGridSet()
{
	// GridSet for flexible residues has already been created
	if (flex_gridset_id_ >= 0) return;

	if (grid_sets_.empty())
	{
		throw BALL::Exception::GeneralException(__FILE__, __LINE__, "GridBasedScoring::defineFlexibleResiduesGridSet() error", "A ScoreGridSet for the receptor must exist before one for flexible residues can be created.");
	}

	double radius = (grid_sets_[0]->size_x*grid_sets_[0]->resolution_)/2;
	Size d_size = (2*radius)/grid_sets_[0]->resolution_;
	Vector3 origin = grid_sets_[0]->origin_;

	ScoreGridSet* rec_gridset = grid_sets_[0];
	Vector3 size(d_size, d_size, d_size);
	ScoreGridSet* sgs = new ScoreGridSet(this, origin, size, rec_gridset->resolution_);

	sgs->out_of_grid_penalty_ = 0;
	sgs->enforce_grid_boundaries_ = false;
	sgs->interaction_no_scale_ = 0;
	sgs->hashgrid_ = flexible_residues_hashgrid_;
	grid_sets_.push_back(sgs);
	flex_gridset_id_ = grid_sets_.size()-1;
}


// set the desired type of the given atom.
// This function can be overloaded when setting the atom's type is done in a different way, like for example for Gehlhaar's ScoringFunction
void GridBasedScoring::setAtomType(Atom* atom, const String& type_name)
{
	int type = getAtomTypes().getType(type_name);
	if (type == -1)
	{
		String mess = "AtomType for \'"+type_name+"\' unknown!!";
		throw BALL::Exception::GeneralException(__FILE__, __LINE__, "GridBasedScoring::precalculateGrids() error", mess);
	}
	atom->setType(type);
	atom->setTypeName(type_name);
	atom->setElement(PTE_::getElement(type_name));
}



void GridBasedScoring::precalculateGrids(bool ony_flexRes_grids)
{
	double stdddev_backup = exp_energy_stddev_;
	exp_energy_stddev_ = 0;

	AtomContainer* backup_ligand = ligand_;

	Atom* atom = new Atom;
	atom->setCharge(1);
	Molecule m;
	m.insert(*atom);

	int ES_grid = -1;
	int NB_grid = -1;
	map<String, int>::iterator it = atom_types_map_.find("0_ELECTROSTATIC");
	if (it != atom_types_map_.end()) ES_grid = it->second;
	it = atom_types_map_.find("1_INTERACTIONS");
	if (it != atom_types_map_.end()) NB_grid = it->second;

	 // use AtomContainer containing only one test atom for precalculation of ScoreGridSets
	setLigand(m);

	Timer t_0;
	t_0.start();
	HashGrid3<Atom*>* hashgrid_backup = hashgrid_;

	int start = 0;
	int end = grid_sets_.size();
	if (ony_flexRes_grids)
	{
		start = flex_gridset_id_;
		end = start+1;
	}
	for (int set = start; set < end; set++) // for each defined grid-set (e.g. different binding pocket descriptions)
	{
		if (grid_sets_.size() > 1)
		{
			Log.level(20)<<"---- Calculation for ScoreGridSet "<<set<<" :  --------------"<<endl;
		}

		if (grid_sets_[set]->getPharmacophoreConstraint())
		{
			enableInteractionComponents_(*grid_sets_[set]->getPharmacophoreConstraint()->getInteractionTypes());
		}

		if (set == flex_gridset_id_ || grid_sets_[set]->getPharmacophoreConstraint())
		{
			// this is necessary in order to make sure that all ScoringComponents that directly use the hashgrid (e.g. adv. ES, frag. solv.), use the correct one!
			hashgrid_ = grid_sets_[set]->hashgrid_;
		}
		grid_sets_[set]->initializeEmptyGrids();

		Vector3& origin = grid_sets_[set]->origin_;
		double& resolution = grid_sets_[set]->resolution_;
		bool first_grid = 1;

		for (int a = 0; a < (int)grid_sets_[set]->noGrids(); a++) // once for each atom-type
		{
			Timer t_a;
			t_a.start();

			String type_name = getGridAtomTypeName(a);

			//if (a < 2) continue;
			if (a == ES_grid || a == NB_grid) continue;

			Log.level(20)<<"calculating ScoreGrid for atom type "<<type_name<<" ... " << endl;
			Log.flush();

			setAtomType(atom, type_name);
			setLigand(m); // make sure to setup ligand again, which is important for components that depend on atom-types (e.g. HB)

			int n = 1;
			Size stepsize = (Size)ceil(grid_sets_[set]->sizeX()/20.0);
			for (Size i = 0; i < grid_sets_[set]->sizeX(); i++)
			{
				if (i == (stepsize*n))
				{
					Log.level(20) << String(n*5) << "% ";
					Log.flush();
					n++;
				}

				for (Size j = 0; j < grid_sets_[set]->sizeY(); j++)
				{
					for (Size k = 0; k < grid_sets_[set]->sizeZ(); k++)
					{
						Vector3 position(origin.x+(i+0.5)*resolution, origin.y+(j+0.5)*resolution, origin.z+(k+0.5)*resolution);
						atom->setPosition(position);

						updatePrecalculatedScore(set);

						if (first_grid)
						{
							if (ES_grid >= 0)
							{
								(*grid_sets_[set])[ES_grid][i][j][k] = getES();
							}
							if (NB_grid >= 0)
							{
								(*grid_sets_[set])[NB_grid][i][j][k] = neighboring_target_atoms_;
							}
						}

						(*grid_sets_[set])[a][i][j][k] = getScore()-getES();
					}
				}
			}
			t_a.stop();
			if (a > 1)
			{
				Log.level(20)<<"100%   done in "<<convertTime(t_a.getClockTime())<<endl;
			}

			first_grid = 0;
			if (grid_sets_.size() > 1) Log<<endl;
		}

		if (set == flex_gridset_id_ || grid_sets_[set]->getPharmacophoreConstraint())
		{
			if (grid_sets_[set]->getPharmacophoreConstraint()) enableAllComponents_();
			hashgrid_ = hashgrid_backup;
		}
	}
	Log.level(20)<<"---------------------------"<<endl;
	t_0.stop();
	Log.level(20)<<"calculated all ScoreGridSets in "<<convertTime(t_0.getClockTime())<<endl;

	setLigand(*backup_ligand); // restore the ligand;

	// estimate burial of reference ligand
	if (ligand_ != NULL) setupReferenceLigand();

	exp_energy_stddev_ = stdddev_backup;
}


void GridBasedScoring::validateGridSets()
{
	double diff_sum = 0;
	Size no_atoms = 0;

	// test each atom seperatly for easier analysis
	for (AtomIterator it = ligand_->beginAtom(); +it; it++, no_atoms++)
	{
		it->select();

		// use DiffGridBasedScoring-functions with precalculated grids
		update();
		double score_w_grids = updateScore();

		// use base-class functions ( == no precalculated grids)
		ScoringFunction::update();
		double score_wo_grids = ScoringFunction::updateScore();

		Log<<score_w_grids<<"  "<<score_wo_grids<<endl;
		diff_sum += pow(score_w_grids-score_wo_grids, 2);

		it->deselect();
	}

	double rmse = sqrt(diff_sum/no_atoms);
	Log<<"RMSE = "<<rmse<<endl;

	// use DiffGridBasedScoring-functions with precalculated grids
	update();
	double score_w_grids = updateScore();

	// use base-class functions ( == no precalculated grids)
	ScoringFunction::update();
	double score_wo_grids = ScoringFunction::updateScore();
	Log<<"\nscores for entire ligand:"<<endl;
	Log<<score_w_grids<<"  "<<score_wo_grids<<endl;
}



void GridBasedScoring::saveGridSetsToFile(String file, String receptor_name)
{
	bool compress = false;
	bool binary = false;
	if (file.hasSuffix(".grd"))
	{
		binary = false;
	}
	else if (file.hasSuffix(".grd.gz"))
	{
		compress = true;
	}
	else if (file.hasSuffix(".bngrd"))
	{
		binary = true;
	}
	else if (file.hasSuffix(".bngrd.gz"))
	{
		compress = true;
		binary = true;
	}
	else
	{
		// Extension unknown, will use grd-tgz format
		Log.level(10) << "ScoreGridSet format unknown, will use grd.tgz" << endl;
		compress = true;
	}

	if (grid_sets_.size() == 0)
	{
		Log.error()<<"Error in GridBasedScoring::saveGridsToFile() : there are no precalculated ScoreGridSets, so that there is nothing to be saved!!"<<endl;
		return;
	}

	ofstream filestream;
	ostream* output = &filestream;
	boost::iostreams::filtering_ostream boost_filter;

	if (binary)
	{
		filestream.open(file.c_str(), ios::binary);
	}
	else
	{
		filestream.open(file.c_str());
	}
	if (compress)
	{
		boost_filter.push(boost::iostreams::gzip_compressor());
		boost_filter.push(filestream);
		output = &boost_filter;
	}

	// save each ScoreGridSet
	for (Size set = 0; set < grid_sets_.size(); set++)
	{
		if (!binary)
		{
			String name;
			if (set == 0) name = receptor_name;
			else name = String("PharmacophoreContraint ") + String(set);
			grid_sets_[set]->saveToFile(*output, name);
		}
		else
		{
			grid_sets_[set]->binaryWrite(*output);
		}
	}

	if (compress) boost_filter.pop();
}


void GridBasedScoring::readAdditionalGridSetFromFile(String file)
{

	ScoreGridSet* sgs = new ScoreGridSet(this);
	grid_sets_.push_back(sgs);

	ifstream input(file.c_str());
	sgs->readFromFile(input);
	/// TODO : save+restore interaction_no_scale from file

	// estimate burial of reference ligand
	if (ligand_ != NULL) setupReferenceLigand();
}


void GridBasedScoring::loadFlexibleResidueScoreGrids(list<pair<const Residue*, const Rotamer*> > residue_list)
{
	// Check whether 'flex_gridsets_' contains all the residues found in 'residue_list'.
	// If this is not the case, erase data from 'grid_sets_[flex_gridset_id_]' and reload the grids for *all* given rotamers.
	bool clear_flexgrid = 0;
	for (list < pair < const Residue*, const Rotamer* > > ::iterator it = residue_list.begin(); it != residue_list.end(); it++)
	{
		map<const Residue*, ScoreGridSet*>::iterator map_it = flex_gridsets_.find(it->first);
		if (map_it == flex_gridsets_.end())
		{
			ScoreGridSet* sgs = new ScoreGridSet(this);
			flex_gridsets_.insert(make_pair(it->first, sgs));
			clear_flexgrid = 1;
		}
	}
	if (clear_flexgrid && flex_gridset_id_ >= 0)
	{
		grid_sets_[flex_gridset_id_]->clearData();
	}


	// If no ScoreGridSet for the flexible residues has been defined yet, then do it now.
	if (flex_gridset_id_ < 0) defineFlexibleResiduesGridSet();

	Path path;
	for (list < pair < const Residue*, const Rotamer* > > ::iterator it = residue_list.begin(); it != residue_list.end(); it++)
	{
		String filename = it->first->getName()+"_";
		filename += String((int)it->second->chi1)+"_";
		filename += String((int)it->second->chi2)+"_";
		filename += String((int)it->second->chi3)+"_";
		filename += String((int)it->second->chi4);

		ScoreGridSet* sgs;
		map<const Residue*, ScoreGridSet*>::iterator map_it = flex_gridsets_.find(it->first);
		if (map_it == flex_gridsets_.end())
		{
			throw BALL::Exception::GeneralException(__FILE__, __LINE__, "GridBasedScoring::loadFlexibleResidueScoreGrids() error", "Missing residue map!");
		}
		else
		{
			sgs = map_it->second;

			// Remove old score contribution from ScoreGridSet for flexible residues
			if (!clear_flexgrid) *grid_sets_[flex_gridset_id_] -= *sgs;
		}

		sgs->hashgrid_ = NULL;
		sgs->disable();
		String file = path.find("sidechain-grids/"+filename+".bngrd");
		if (file == "")
		{
			throw BALL::Exception::GeneralException(__FILE__, __LINE__, "GridBasedScoring::loadFlexibleResidueScoreGrids() error", "ScoreGridSets for flexible residues could not be loaded from file!");
		}
		ifstream input(file.c_str());
		sgs->binaryRead(input);

		/// Transform sgs by mapping backbone atoms used for precalculation to the ones of the given residue !
		file = path.find("sidechain-grids/"+filename+".pdb");
		if (file == "")
		{
			throw BALL::Exception::GeneralException(__FILE__, __LINE__, "GridBasedScoring::loadFlexibleResidueScoreGrids() error", "PDB-file for rotamer could not be found !");
		}
		PDBFile pdb(file);
		Molecule* template_residue = pdb.read();
		Vector3 template_C, template_CA, template_N;
		Vector3 current_C, current_CA, current_N;
		bool found_C = 0; bool found_CA = 0; bool found_N = 0;
		for (AtomConstIterator atom_it = template_residue->beginAtom(); +atom_it; atom_it++)
		{
			String name = atom_it->getName();
			if (name == "N")
			{
				if (found_N)
				{
					throw BALL::Exception::GeneralException(__FILE__, __LINE__, "GridBasedScoring::loadFlexibleResidueScoreGrids() error", "More than one atom of a template residue is named 'N', thus backbone atoms could not be uniquely identified!");
				}
				template_N = atom_it->getPosition();
				found_N = true;
			}
			else if (name == "C")
			{
				if (found_C)
				{
					throw BALL::Exception::GeneralException(__FILE__, __LINE__, "GridBasedScoring::loadFlexibleResidueScoreGrids() error", "More than one atom of a template residue is named 'C', thus backbone atoms could not be uniquely identified!");
				}
				template_C = atom_it->getPosition();
				found_C = true;
			}
			else if (name == "CA")
			{
				if (found_CA)
				{
					throw BALL::Exception::GeneralException(__FILE__, __LINE__, "GridBasedScoring::loadFlexibleResidueScoreGrids() error", "More than one atom of a template residue is named 'CA', thus backbone atoms could not be uniquely identified!");
				}
				template_CA = atom_it->getPosition();
				found_CA = true;
			}
		}
		if (!found_N || !found_CA || !found_C)
		{
			throw BALL::Exception::GeneralException(__FILE__, __LINE__, "GridBasedScoring::loadFlexibleResidueScoreGrids() error", "Some backbone atoms are missing in the template residue!");
		}

		found_C = 0; found_CA = 0; found_N = 0;
		for (AtomConstIterator atom_it = it->first->beginAtom(); +atom_it; atom_it++)
		{
			String name = atom_it->getName();
			if (name == "N")
			{
				if (found_N)
				{
					throw BALL::Exception::GeneralException(__FILE__, __LINE__, "GridBasedScoring::loadFlexibleResidueScoreGrids() error", "More than one atom of a residue is named 'N', thus backbone atoms could not be uniquely identified!");
				}
				current_N = atom_it->getPosition();
				found_N = true;
			}
			else if (name == "C")
			{
				if (found_C)
				{
					throw BALL::Exception::GeneralException(__FILE__, __LINE__, "GridBasedScoring::loadFlexibleResidueScoreGrids() error", "More than one atom of a residue is named 'C', thus backbone atoms could not be uniquely identified!");
				}
				current_C = atom_it->getPosition();
				found_C = true;
			}
			else if (name == "CA")
			{
				if (found_CA)
				{
					throw BALL::Exception::GeneralException(__FILE__, __LINE__, "GridBasedScoring::loadFlexibleResidueScoreGrids() error", "More than one atom of a residue is named 'CA', thus backbone atoms could not be uniquely identified!");
				}
				current_CA = atom_it->getPosition();
				found_CA = true;
			}
		}
		if (!found_N || !found_CA || !found_C)
		{
			throw BALL::Exception::GeneralException(__FILE__, __LINE__, "GridBasedScoring::loadFlexibleResidueScoreGrids() error", "Some backbone atoms are missing in a residue!");
		}

		delete template_residue;

		Matrix4x4 T = StructureMapper::matchPoints
		(template_C, template_CA, template_N, current_C, current_CA, current_N);
		sgs->transform(T);

		if (current_CA.getDistance(T*template_CA) > 0.2)
		{
			throw BALL::Exception::GeneralException(__FILE__, __LINE__, "GridBasedScoring::loadFlexibleResidueScoreGrids() error", "Backbone atoms of current residue could not be mapped sufficiently to template!");
		}
		/// --- end of transforming ---

		// Add new score contribution to ScoreGridSet for flexible residues
		*grid_sets_[flex_gridset_id_] += *sgs;
	}
}


void GridBasedScoring::replaceGridSetFromFile(String file)
{
	for (Size i = 0; i < grid_sets_.size(); i++)
	{
		delete grid_sets_[i];
	}
	grid_sets_.clear();

	String postfix = "";
	String prefix = "";
	bool uncompress = false;
	bool binary = false;
	if (file.hasSuffix(".grd"))
	{
		uncompress = false;
	}
	else if (file.hasSuffix(".grd.gz"))
	{
		uncompress = true;
	}
	else if (file.hasSuffix(".bngrd"))
	{
		binary = true;
	}
	else if (file.hasSuffix(".bngrd.gz"))
	{
		uncompress = true;
		binary = true;
	}
	else
	{
		// Extension unknown, will use grd-tgz format
		Log.level(10) << "ScoreGridSet format unknown, will try grd.gz" << endl;
		uncompress = true;
	}

	ifstream filestream;
	istream* input = &filestream;
	boost::iostreams::filtering_istream boost_filter;

	if (binary)
	{
		filestream.open(file.c_str(), ios::binary | ios::in);
	}
	else
	{
		filestream.open(file.c_str(), ios::in);
	}
	if (uncompress)
	{
		boost_filter.push(boost::iostreams::gzip_decompressor());
		boost_filter.push(filestream);
		input = &boost_filter;
	}

	list<PharmacophoreConstraint*> pharm_constraints;
	for (list < Constraint* > ::iterator it = constraints.begin(); it != constraints.end(); it++)
	{
		PharmacophoreConstraint* phc = dynamic_cast<PharmacophoreConstraint*>(*it);
		if (phc) pharm_constraints.push_back(phc);
	}

	if (pharm_constraints.size() == 0 && !hasFlexibleResidues() && !uncompress)
	{
		ScoreGridSet* sgs = new ScoreGridSet(this);
		grid_sets_.push_back(sgs);
		if (!binary) sgs->readFromFile(*input);
		else sgs->binaryRead(*input);
	}
	else
	{
		// read the first ScoreGridSet, i.e. the one for the entire receptor
		Size set = 0;
		ScoreGridSet* sgs = new ScoreGridSet(this);
		grid_sets_.push_back(sgs);
		Log.level(10)<<"reading score grids "<<" ... "<<endl;
		Log.flush();
		if (!binary) sgs->readFromFile(*input);
		else sgs->binaryRead(*input);
		set++;

		if (hasFlexibleResidues())
		{
			// read one ScoreGridSet for the flexible residues
			ScoreGridSet* sgs2 = new ScoreGridSet;
			sgs2->hashgrid_ = flexible_residues_hashgrid_;
			grid_sets_.push_back(sgs2);
			Log.level(10)<<"reading score grids for flexible residues ... "<<endl<<flush;
			if (!binary) sgs2->readFromFile(*input);
			else sgs2->binaryRead(*input);
			flex_gridset_id_ = 1;
			set++;
		}

		// read one ScoreGridSet for each PharmacophoreConstraint
		for (list < PharmacophoreConstraint* > ::iterator it = pharm_constraints.begin(); it != pharm_constraints.end(); it++, set++)
		{
			Vector3 size_in_angstroem(grid_sets_[0]->sizeX()*resolution_, grid_sets_[0]->sizeY()*resolution_, grid_sets_[0]->sizeZ()*resolution_);
			defineGridSet((*it)->getGridContainer(), size_in_angstroem, 0, 0, resolution_, 0);
			grid_sets_[grid_sets_.size()-1]->setPharmacophoreConstraint(*it);

			Log.level(10)<<"reading score grids for pharm. constraint " << (*it)->getName() << " ... " << endl;
			Log.flush();
			if (!binary)
			{
				grid_sets_[grid_sets_.size()-1]->readFromFile(*input);
			}
			else grid_sets_[grid_sets_.size()-1]->binaryRead(*input);
		}
	}

	/// TODO : save+restore interaction_no_scale from file

	// estimate burial of reference ligand
	if (ligand_ != NULL) setupReferenceLigand();
}


void GridBasedScoring::GridSetsResult::setup(Size no_gridSets)
{
	gridSet_scores.clear();
	no_neighbors.clear();
	no_out_of_grid.clear();
	gridSet_scores.resize(no_gridSets, 0);
	no_neighbors.resize(no_gridSets, 0);
	no_out_of_grid.resize(no_gridSets, 0);
}


double GridBasedScoring::calculateGridScore()
{
	vector<int> neighbors(grid_sets_.size(), 0);
	int atoms = 0;
	double grid_score = 0;
	gridsets_result_.setup(grid_sets_.size());
	bool use_selection = ligand_->containsSelection();

	int ES_grid = -1;
	int NB_grid = -1;
	map<String, int>::iterator it = atom_types_map_.find("0_ELECTROSTATIC");
	if (it != atom_types_map_.end()) ES_grid = it->second;
	it = atom_types_map_.find("1_INTERACTIONS");
	if (it != atom_types_map_.end()) NB_grid = it->second;

	/// add up the scores for each ligand atom
	for (AtomIterator it = ligand_->beginAtom(); it != ligand_->endAtom(); it++)
	{
		if (use_selection && !it->isSelected()) continue;

		atoms++;
		String type = it->getTypeName();
		map<String, int>::iterator type_it = atom_types_map_.find(type);
		int g = -1;

		if (type_it != atom_types_map_.end())
		{
			g = type_it->second; // ID of the respective ScoreGrid
		}
		if (g == -1)
		{
			String type0 = type;
			while (type.size() > 1)
			{
				type = type.getSubstring(0, type.size()-1);
				type_it = atom_types_map_.find(type);
				if (type_it != atom_types_map_.end())
				{
					break;
				}
			}
			if (type_it != atom_types_map_.end())
			{
				Log.warn()<<"No precalculated EnergyGrid for AtomType "<<type0<<" found, using "<<type<<" instead"<<endl;
				g = type_it->second;
			}
			else
			{
				Log.warn()<<"No precalculated EnergyGrid for AtomType "<<type0<<" and for no similar AtomType found! Skipping this ligand atom!"<<endl;
				continue;
			}
		}

		Vector3 pos = it->getPosition();
		bool valid_pose = 1;
		vector<double> tmp_scores(grid_sets_.size(), 0); // one value for each ScoreGridSet

		/// calculate one score value for each ScoreGridSet
		for (Size set = 0; valid_pose && set < grid_sets_.size(); set++)
		{
			if (grid_sets_[set]->enabled_ == 0) continue; // use only enabled ScoreGridSets

			// vdW repectively h-bonds
			double value = grid_sets_[set]->getGridScore(g, pos, scoregrid_interpolation_);
			tmp_scores[set] += value;

			if (grid_sets_[set]->out_of_grid_penalty_ != 0 && value >= grid_sets_[set]->out_of_grid_penalty_)
			{
				if (grid_sets_[set]->enforce_grid_boundaries_)
				{
					valid_pose = 0;
				}
				else
				{
					neighbors[set] += (int)grid_sets_[set]->getGridScore(1, pos, scoregrid_interpolation_);
					gridsets_result_.no_out_of_grid[set]++;
				}
				continue; // if atom is lying outside of grid, add penalty once and continue
				         // (no need to add electrostatic contribution)
			}
			if (!valid_pose) // no need to check remaining ScoreGridSets
			{
				break;
			}

			if (ES_grid >= 0) // if there is a grid for Electrostatic interaction
			{
				 // electrostatic contribution
				tmp_scores[set] += grid_sets_[set]->getGridScore(ES_grid, pos, scoregrid_interpolation_)*it->getCharge();
				gridsets_result_.gridSet_scores[set] += tmp_scores[set];
			}

			if (NB_grid >= 0) // if there is a grid containing the number of neighboring receptor atoms
			{
				// the number of neighboring atoms within a small radius
				neighbors[set] += (int)grid_sets_[set]->getGridScore(NB_grid, pos, scoregrid_interpolation_);
			}
		}

		/// combine the values calculated above as desired by the user
		double atom_score = 0; // score for the current atom
		if (combine_operation_ == 2) atom_score = 1e100; // for minimum calculation
		if (combine_operation_ == 3) atom_score = -1e100; // for maximum calculation
		int no_active_gridSets = 0;
		for (Size i = 0; i < grid_sets_.size(); i++)
		{
			if (grid_sets_[i]->enabled_) no_active_gridSets++;
			else continue;

			// allow different operations: sum/average/min/max of ScoreGridSets
			if (combine_operation_ <= 1) atom_score += tmp_scores[i];
			if (combine_operation_ == 2)
			{
				if (tmp_scores[i] < atom_score) atom_score = tmp_scores[i];
			}
			if (combine_operation_ == 3)
			{
				if (tmp_scores[i] > atom_score) atom_score = tmp_scores[i];
			}
		}
		if (combine_operation_ == 1) // for average calculation
		{
			atom_score /= no_active_gridSets;
		}

		grid_score += atom_score;

	} // end of iteration over ligand atoms


	///If current ligand is buried less deeply then reference ligand, penalize this linearly.
	for (int set = grid_sets_.size()-1; set >= 0; set--)
	{
		gridsets_result_.no_neighbors[set] = ((double)neighbors[set])/atoms;

		// Take flexible residues, which are not part of the receptor score grid, into account as well.
		if (set == 0 && !flexible_residues_.empty() && grid_sets_.size() > 1)
		{
			gridsets_result_.no_neighbors[0] += gridsets_result_.no_neighbors[1];
		}

		// Estimate how deeply buried the current ligand is by looking at the number its of interactions per atom.
		if (grid_sets_[set]->reference_interactions != 0 && grid_sets_[set]->interaction_no_scale_ != 0 && grid_sets_[set]->enabled_)
		{
			double factor;
			if (neighbors[set] != 0)
			{
				// the lower 'factor', the harder the penalization will be
				factor = ((((double)neighbors[set])/atoms) /grid_sets_[set]->reference_interactions);
			}
			else  // if there are no neighboring receptor atoms ...
			{
				factor =  0.5/grid_sets_[set]->reference_interactions;
			}

			if (factor < 1)
			{
				factor /= grid_sets_[set]->interaction_no_scale_;

				if (grid_score > 0)
				{
					grid_score /= factor;
				}
				else if (grid_score < 0)
				{
					grid_score *= factor;
				}
			}
		}
	}

	return grid_score;
}


//  make sure to use calculateGridScore() BEFORE using this function !
int GridBasedScoring::getNoNeighboringReceptorAtoms()
{
	return (int)gridsets_result_.no_neighbors[0];
}


void GridBasedScoring::printResult()
{
	Size no_gridSets = grid_sets_.size();
	for (Size i = 0; i < no_gridSets; i++)
	{
		/*if (no_gridSets > 1)*/ Log.level(20)<<"GridSet "<<i<<" :"<<endl;
		Log.level(20)<<"\t"<<"interaction score: "<<gridsets_result_.gridSet_scores[i]<<endl;
		Log.level(20)<<"\t"<<"av. no. of neighbors: "<<gridsets_result_.no_neighbors[i]<<endl;
		Log.level(20)<<"\t"<<"#atoms out-of-GridSet: "<<gridsets_result_.no_out_of_grid[i]<<endl;
	}
	ScoringFunction::printResult();
}


vector<ScoreGridSet*>* GridBasedScoring::getScoreGridSets()
{
	return &grid_sets_;
}
