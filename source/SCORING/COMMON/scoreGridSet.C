// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Marcel Schumann $
// ----------------------------------------------------

#include <BALL/SCORING/COMMON/scoreGridSet.h>


using namespace BALL;
using namespace std;

ScoreGridSet::ScoreGridSet(GridBasedScoring* gbs, Vector3& v_origin_, Vector3& size, double& res)
{
	origin_ = v_origin_;
	original_origin_ = v_origin_;
	resolution_ = res;
	size_x = (int)size[0];
	size_y = (int)size[1];
	size_z = (int)size[2];
	out_of_grid_penalty_ = 0;
	enforce_grid_boundaries_ = 0;
	reference_interactions = 0;
	T_.setIdentity();
	T_i_.setIdentity();
	transformed_ = 0;
	enabled_ = 1;
	interaction_no_scale_ = gbs->getBurialDepthScale(); // as default use the value of the ScoringFunction that was set via Options by use user

	parent = gbs;
	hashgrid_ = gbs->hashgrid_;
	new_hashgrid_ = 0;

	score_grids_ = new vector<ScoreGrid*>;
	score_grids_->clear(); //set to size of 0

	is_reference_ = 0;
	pharm_constraint_ = 0;
}


ScoreGridSet::ScoreGridSet(GridBasedScoring* gbs)
{
	parent = gbs;

	origin_ = Vector3(0, 0, 0);
	original_origin_ = Vector3(0, 0, 0);
	resolution_ = 0.5;
	size_x = 0;
	size_y = 0;
	size_z = 0;
	out_of_grid_penalty_ = 0;
	enforce_grid_boundaries_ = 0;
	reference_interactions = 0;
	T_.setIdentity();
	T_i_.setIdentity();
	transformed_ = 0;
	enabled_ = 1;
	interaction_no_scale_ = gbs->getBurialDepthScale(); // as default use the value of the ScoringFunction that was set via Options by use user

	hashgrid_ = gbs->hashgrid_;
	new_hashgrid_ = 0;

	score_grids_ = new vector<ScoreGrid*>;
	score_grids_->clear(); //set to size of 0

	is_reference_ = 0;
	pharm_constraint_ = 0;
}


ScoreGridSet::ScoreGridSet(ScoreGridSet* sgs)
{
	parent = sgs->parent;

	origin_ = sgs->origin_;
	original_origin_ = sgs->original_origin_;
	resolution_ = sgs->resolution_;
	size_x = sgs->size_x;
	size_y = sgs->size_y;
	size_z = sgs->size_z;
	out_of_grid_penalty_ = sgs->out_of_grid_penalty_;
	enforce_grid_boundaries_ = sgs->enforce_grid_boundaries_;
	reference_interactions = sgs->reference_interactions;
	interaction_no_scale_ = sgs->interaction_no_scale_;
	T_ = sgs->T_;
	T_i_ = sgs->T_i_;
	transformed_ = sgs->transformed_;
	enabled_ = sgs->enabled_;
	interaction_no_scale_ = sgs->interaction_no_scale_;

	hashgrid_ = sgs->hashgrid_;
	new_hashgrid_ = 0;

	// copy pointer to vector<ScoreGrid*>, thus no need to calculate/save interactions anew!
	score_grids_ = sgs->score_grids_;

	is_reference_ = 1; // <- make sure data is only calculate once and not deleted twice
	pharm_constraint_ = sgs->pharm_constraint_;
}


ScoreGridSet::ScoreGridSet()
{
	parent = 0;
	origin_ = Vector3(0, 0, 0);
	original_origin_ = Vector3(0, 0, 0);
	resolution_ = 0.5;
	size_x = 0;
	size_y = 0;
	size_z = 0;
	out_of_grid_penalty_ = 0;
	enforce_grid_boundaries_ = 0;
	reference_interactions = 0;
	T_.setIdentity();
	T_i_.setIdentity();
	transformed_ = 0;
	enabled_ = 1;
	interaction_no_scale_ = 0;
	hashgrid_ = 0;
	new_hashgrid_ = 0;

	score_grids_ = new vector<ScoreGrid*>;
	score_grids_->clear(); //set to size of 0

	is_reference_ = 0;
	pharm_constraint_ = 0;
}


ScoreGridSet::~ScoreGridSet()
{
	if (!is_reference_)
	{
		for (Size i = 0; i < score_grids_->size(); i++)
		{
			delete (*score_grids_)[i];
		}
		delete score_grids_;
	}
	if (new_hashgrid_)
	{
		delete hashgrid_;
	}
}


void ScoreGridSet::operator += (ScoreGridSet& sgs)
{
	if (*getAtomTypesMap() != *sgs.getAtomTypesMap())
	{
		throw BALL::Exception::GeneralException(__FILE__, __LINE__, "ScoreGridSet::operator += Error", "ScoreGrid to be added must use the same atom types!");
	}

	for (Size x = 0; x < size_x; x++)
	{
		for (Size y = 0; y < size_y; y++)
		{
			for (Size z = 0; z < size_z; z++)
			{
				for (Size grid = 0; grid < score_grids_->size(); grid++)
				{
					Vector3 position((x+0.5)*resolution_, (y+0.5)*resolution_, (z+0.5)*resolution_);
					(*this)[grid][x][y][z] += sgs.getGridScore(grid, position, false);
				}
			}
		}
	}
}


void ScoreGridSet::operator -= (ScoreGridSet& sgs)
{
	if (*getAtomTypesMap() != *sgs.getAtomTypesMap())
	{
		throw BALL::Exception::GeneralException(__FILE__, __LINE__, "ScoreGridSet::operator += Error", "ScoreGrid to be added must use the same atom types!");
	}

	for (Size x = 0; x < size_x; x++)
	{
		for (Size y = 0; y < size_y; y++)
		{
			for (Size z = 0; z < size_z; z++)
			{
				for (Size grid = 0; grid < score_grids_->size(); grid++)
				{
					Vector3 position((x+0.5)*resolution_, (y+0.5)*resolution_, (z+0.5)*resolution_);
					(*this)[grid][x][y][z] -= sgs.getGridScore(grid, position, false);
				}
			}
		}
	}
}

void ScoreGridSet::clearData()
{
	for (Size x = 0; x < size_x; x++)
	{
		for (Size y = 0; y < size_y; y++)
		{
			for (Size z = 0; z < size_z; z++)
			{
				for (Size grid = 0; grid < score_grids_->size(); grid++)
				{
					Vector3 position((x+0.5)*resolution_, (y+0.5)*resolution_, (z+0.5)*resolution_);
					(*this)[grid][x][y][z] = 0;
				}
			}
		}
	}
}

void ScoreGridSet::enable()
{
	enabled_ = 1;
}


void ScoreGridSet::disable()
{
	enabled_ = 0;
}


bool ScoreGridSet::isEnabled()
{
	return enabled_;
}


std::map<BALL::String, int>* ScoreGridSet::getAtomTypesMap()
{
	if (parent) return parent->getAtomTypesMap();
	else return &atom_types_map_no_parent_;
}


BALL::String ScoreGridSet::getGridAtomTypeName(int grid_id)
{
	if (parent) return parent->getGridAtomTypeName(grid_id);

	for (std::map < String, int > ::iterator it = atom_types_map_no_parent_.begin(); it != atom_types_map_no_parent_.end(); it++)
	{
		if (it->second == grid_id) return it->first;
	}

	String mess = "No ScoreGrid with ID="+String(grid_id)+" !!";
	throw BALL::Exception::GeneralException(__FILE__, __LINE__, "ScoreGridSet::getGridAtomTypeName() error", mess);

	return "";
}


void ScoreGridSet::initializeEmptyGrids(int no)
{
	// delete old grids first (if any)
	for (Size i = 0; i < score_grids_->size(); i++)
	{
		delete (*score_grids_)[i];
	}
	score_grids_->clear();

	int no_grids = no;
	if (no == -1)
	{
		if (parent) no_grids = parent->getAtomTypesMap()->size();
		else no_grids = 1;
	}

	score_grids_->resize(no_grids);
	std::vector<double> v1((int)size_z, 0);
	std::vector<vector<double> > v2((int)size_y, v1);
	for (int i = 0; i < no_grids; i++)
	{
		(*score_grids_)[i] = new ScoreGrid((int)size_x, v2);
	}
}



void ScoreGridSet::setHashGrid(HashGrid3<Atom*>* hashgrid)
{
	hashgrid_ = hashgrid;
	new_hashgrid_ = 1;
}


double ScoreGridSet::getGridScore(Size grid, Vector3 pos, bool interpolation)
{
	if (grid > score_grids_->size())
	{
		String s = "ScoreGrid "; s += String(grid)+" does not exist (yet) !";
		throw Exception::GeneralException(__FILE__, __LINE__, "ScoreGridSet::getGridScore() error", s);
	}

	if (transformed_)
	{
		pos = T_i_*pos;
	}

	int x = (int)((pos.x-original_origin_.x)/resolution_); // indices of cell
	int y = (int)((pos.y-original_origin_.y)/resolution_);
	int z = (int)((pos.z-original_origin_.z)/resolution_);

	if (x < 0 || y < 0 || z < 0 || x >= (int)size_x || y >= (int)size_y || z >= (int)size_z )
	{
		return out_of_grid_penalty_; // (if desired) treat atoms outside of grid as sterical clashes
	}

	if (interpolation)
	{
		// calculate the distance between the point and the center of the assigned cell along each of the 3 axes
		double center_x = (x+0.5)*resolution_+original_origin_.x;
		double center_y = (y+0.5)*resolution_+original_origin_.y;
		double center_z = (z+0.5)*resolution_+original_origin_.z;

		double factor_x = (center_x-pos.x)/resolution_;
		double factor_y = (center_y-pos.y)/resolution_;
		double factor_z = (center_z-pos.z)/resolution_;

		double score = 0;

		int x_neighbor = x;
		if (factor_x < -0.3) x_neighbor += 1;
		else if (factor_x > 0.3) x_neighbor -= 1;

		int y_neighbor = y;
		if (factor_y < -0.3) y_neighbor += 1;
		else if (factor_y > 0.3) y_neighbor -= 1;

		int z_neighbor = z;
		if (factor_z < -0.3) z_neighbor += 1;
		else if (factor_z > 0.3) z_neighbor -= 1;

		if (x_neighbor == x && y_neighbor == y && z_neighbor == z)
		{
			return (*(*score_grids_)[grid])[x][y][z];
		}

		if (x_neighbor >= 0 && x_neighbor < (int)size_x && y_neighbor >= 0 && y_neighbor < (int)size_y
			&& z_neighbor>=0&&z_neighbor<(int)size_z)
		{
			Vector3 center(center_x, center_y, center_z);
			Vector3 neighbor_center((x_neighbor+0.5)*resolution_+original_origin_.x, (y_neighbor+0.5)*resolution_+original_origin_.y, (z_neighbor+0.5)*resolution_+original_origin_.z);

			double dist1 = pos.getDistance(center);
			double dist2 = pos.getDistance(neighbor_center);

			double factor = dist1/(dist1+dist2);

			score = factor*(*(*score_grids_)[grid])[x][y][z]
			+ (1-factor)*(*(*score_grids_)[grid])[x_neighbor][y_neighbor][z_neighbor];

			return score;
		}
		else return (*(*score_grids_)[grid])[x][y][z];
	}

	return (*(*score_grids_)[grid])[x][y][z];
}


HashGrid3<Atom*>* ScoreGridSet::getHashGrid()
{
	return hashgrid_;
}

ScoreGrid& ScoreGridSet::operator[](int i)
{
	return *(*score_grids_)[i];
}


void ScoreGridSet::transform(TMatrix4x4 < float > & T)
{
	TMatrix4x4<float> T_i;
	T.invert(T_i); // store inverse of T in T_i
	T_ *= T;
	T_i_ *= T_i ; // store the inverse of all transformations in T_i_
	transformed_ = 1;

	origin_ = T*origin_;
}


void ScoreGridSet::moveTo(Vector3& destination)
{
	Vector3 translate = destination - getCenter();
	Matrix4x4 T;
	T.setTranslation(translate);
	transform(T);
}


Vector3 ScoreGridSet::getOrigin()
{
	return origin_;
}


Vector3 ScoreGridSet::getCenter()
{
	Vector3 center = origin_;
	center.x += 0.5*size_x*resolution_;
	center.y += 0.5*size_y*resolution_;
	center.z += 0.5*size_z*resolution_;
	return center;
}


Size ScoreGridSet::sizeX()
{
	return size_x;
}

Size ScoreGridSet::sizeY()
{
	return size_y;
}

Size ScoreGridSet::sizeZ()
{
	return size_z;
}

Size ScoreGridSet::noGrids()
{
	return score_grids_->size();
}


void ScoreGridSet::binaryWrite(std::ostream& outfile)
{
	BinaryFileAdaptor<Size> adapt_size;
	BinaryFileAdaptor<Vector3> adapt_vector3;
	BinaryFileAdaptor<double> adapt_double;
	BinaryFileAdaptor<bool> adapt_bool;
	BinaryFileAdaptor<char> adapt_char;

	// save information about the number of grids
	adapt_size.setData(score_grids_->size());
	outfile << adapt_size;

	// save information about the number of cells on each axis of each grid
	adapt_size.setData(sizeX());
	outfile << adapt_size;
	adapt_size.setData(sizeY());
	outfile << adapt_size;
	adapt_size.setData(sizeZ());
	outfile << adapt_size;

	adapt_double.setData(resolution_);
	outfile << adapt_double;

	adapt_vector3.setData(original_origin_);
	outfile << adapt_vector3;

	adapt_double.setData(out_of_grid_penalty_);
	outfile << adapt_double;

	adapt_bool.setData(enforce_grid_boundaries_);
	outfile << adapt_bool;

	// now save each score-grid
	for (Size g = 0; g < score_grids_->size(); g++)
	{
		String type_name = getGridAtomTypeName(g).c_str();
		Size no_chars = type_name.size();

		// write the number of chars of the type-name
		adapt_size.setData(no_chars);
		outfile << adapt_size;

		// .. and now write the type-name itself
		for (Size t = 0; t < no_chars; t++)
		{
			char c = type_name[t];
			adapt_char.setData(c);
			outfile << adapt_char;
		}

		for (Size i = 0; i < (*score_grids_)[g]->size(); i++)
		{
			for (Size j = 0; j < (*(*score_grids_)[g])[i].size(); j++)
			{
				for (Size k = 0; k < (*(*score_grids_)[g])[i][j].size(); k++)
				{
					adapt_double.setData((*(*score_grids_)[g])[i][j][k]);
					outfile << adapt_double;
				}
			}
		}
	}
}


void ScoreGridSet::binaryRead(std::istream& infile)
{
	BinaryFileAdaptor<Size> adapt_size;
	BinaryFileAdaptor<Vector3> adapt_vector3;
	BinaryFileAdaptor<double> adapt_double;
	BinaryFileAdaptor<bool> adapt_bool;
	BinaryFileAdaptor<char> adapt_char;

	infile >> adapt_size;
	Size no_grids = adapt_size.getData();

	infile >> adapt_size;
	size_x = adapt_size.getData();

	infile >> adapt_size;
	size_y = adapt_size.getData();

	infile >> adapt_size;
	size_z = adapt_size.getData();

	infile >> adapt_double;
	resolution_ = adapt_double.getData();

	infile >> adapt_vector3;
	original_origin_ = adapt_vector3.getData();
	origin_ = original_origin_;

	infile >> adapt_double;
	out_of_grid_penalty_ = adapt_double.getData();

	infile >> adapt_bool;
	enforce_grid_boundaries_ = adapt_bool.getData();

	initializeEmptyGrids(no_grids);

	std::map<String, int>* type_map = getAtomTypesMap();

	bool replace = 0;
	if (!parent || parent->grid_sets_.size() == 1)
	{
		type_map->clear(); // remove old atomTypes from map, if there is only ONE ScoreGridSet (this one)
		replace = 1;
	}

	// now read each score-grid
	for (Size g = 0; g < no_grids; g++)
	{
		// read information about the number of chars of the type-name
		infile >> adapt_size;
		Size no_chars = adapt_size.getData();

		// .. and now read the type-name itself
		String type_name ="";
		for (Size t = 0; t < no_chars; t++)
		{
			infile >> adapt_char;
			char c = adapt_char.getData();
			type_name += c;
		}

		if (replace)
		{
			type_map->insert(make_pair(type_name, g));
		}
		// if there is more than one ScoreGridSet, AtomTypes MUST be identical for all ScoreGridSets !!
		else
		{
			String expected_atomtype = getGridAtomTypeName(g);
			if (type_name != expected_atomtype)
			{
				std::cout<<type_name<<"  "<<expected_atomtype<<std::endl;
				throw BALL::Exception::GeneralException(__FILE__, __LINE__, "ScoreGridSet::readFromFile() error", "If using more than one ScoreGridSet, all ScoreGridSets MUST contain the same AtomTypes!!");
			}
		}

		for (Size i = 0; i < (*score_grids_)[g]->size(); i++)
		{
			for (Size j = 0; j < (*(*score_grids_)[g])[i].size(); j++)
			{
				for (Size k = 0; k < (*(*score_grids_)[g])[i][j].size(); k++)
				{
					infile >> adapt_double;
					(*(*score_grids_)[g])[i][j][k] = adapt_double.getData();
				}
			}
		}
	}
}


void ScoreGridSet::saveToFile(std::ostream& out, String receptor_name)
{
	out<<"ScoreGridSet for receptor "<<receptor_name<<endl;
	out<<"no of grids: "<<score_grids_->size()<<endl;
	out<<"no of grid boxes: "<<sizeX()<<" "<<sizeY()<<" "<<sizeZ()<<endl;
	out<<"resolution_ (in Angstroem): "<<resolution_<<endl;
	out<<"origin_: "<<original_origin_[0]<<" "<<original_origin_[1]<<" "<<original_origin_[2]<<endl;
	out<<"out_of_grid_penalty_: "<<out_of_grid_penalty_<<endl;
	out<<"enforce_grid_boundaries_?: "<<enforce_grid_boundaries_<<endl;

	//cout<<filename<<" "<<score_grids_->size()<<"  "<<getAtomTypesMap()->size()<<endl;

	for (Size grid = 0; grid < score_grids_->size(); grid++)
	{
		out<<endl<<"===========================================\n\n";
		//out<<"ScoreGrid for AtomType: "<< it->first << endl; it++;
		out<<"ScoreGrid for AtomType: "<< getGridAtomTypeName(grid) << endl;

		int no_overlaps = 0;
		for (Size i = 0; i < size_x; i++)
		{
			for (Size j = 0; j < size_y; j++)
			{
				for (Size k = 0; k < size_z; k++)
				{
					if ((*(*score_grids_)[grid])[i][j][k] >= 1e10)
					{
						no_overlaps++;
					}
					else
					{
						// write information about impossible positions for ligands atoms in compressed format
						if (no_overlaps > 0)
						{
							if (no_overlaps > 1)
							{
								out<<"NA "<<no_overlaps<<"\t";
							}
							else
							{
								out<<out_of_grid_penalty_<<"\t";
							}
							no_overlaps = 0;
						}
						out <<(*(*score_grids_)[grid])[i][j][k]<<"\t";
					}
				}
				if (no_overlaps == 0) out<<endl;
			}
		}
		if (no_overlaps > 1)
		{
			out<<"NA "<<no_overlaps<<"\t" << endl;
		}
		else if (no_overlaps == 1)
		{
			out<<out_of_grid_penalty_<<"\t" << endl;
		}
	}
}


void ScoreGridSet::readFromFile(istream& input)
{
	String tmp;
	Size no_grids = 0;
	double x_origin = 0; double y_origin = 0; double z_origin = 0;

	getline(input, tmp); // skip first line containing the name of the receptor

	input>>tmp; input>>tmp; input>>tmp;
	input>>no_grids; 	// read the number of energy grids ( == no of atom types)

	input>>tmp; input>>tmp; input>>tmp; input>>tmp;
	input >> size_x; input >> size_y; input>>size_z;

	initializeEmptyGrids(no_grids);

	input>>tmp;  input>>tmp; input>>tmp;
	input>>resolution_; // read resolution_ of grids

	input>>tmp;
	input>>x_origin; input>>y_origin; input>>z_origin;
	origin_ = Vector3(x_origin, y_origin, z_origin);
	original_origin_ = origin_;

	input>>tmp; input>>out_of_grid_penalty_;
	input>>tmp; input>>enforce_grid_boundaries_;

	map<String, int>* type_map = getAtomTypesMap();

	bool replace = 0;
	if (!parent || parent->grid_sets_.size() == 1)
	{
		type_map->clear(); // remove old atomTypes from map, if there is only ONE ScoreGridSet (this one)
		replace = 1;
	}

	// empty line is skipped automatically
	for (Size a = 0; a < no_grids; a++)
	{
		input>>tmp; // skip partitioner ("======")
		// empty line is skipped automatically
		input>>tmp; input>>tmp; input>>tmp;
		String atomtype;
		input >> atomtype;

		if (replace)
		{
			type_map->insert(make_pair(atomtype, a));
		}
		// if there is more than one ScoreGridSet, AtomTypes MUST be identical for all ScoreGridSets !!
		else
		{
			String expected_atomtype = getGridAtomTypeName(a);
			if (atomtype != expected_atomtype)
			{
				cout<<atomtype<<"  "<<expected_atomtype<<endl;
				throw BALL::Exception::GeneralException(__FILE__, __LINE__, "ScoreGridSet::readFromFile() error", "If using more than one ScoreGridSet, all ScoreGridSets MUST contain the same AtomTypes!!");
			}
		}
		int no_overlaps = 0;

		for (Size i = 0; i < (*score_grids_)[a]->size(); i++)
		{
			for (Size j = 0; j < (*(*score_grids_)[a])[i].size(); j++)
			{
				for (Size k = 0; k < (*(*score_grids_)[a])[i][j].size(); k++)
				{
					if (no_overlaps > 0)
					{
						(*(*score_grids_)[a])[i][j][k] = 1e10;
						no_overlaps--;
					}
					else
					{
						// read information about impossible positions for ligands atoms in compressed format
						String v;
						double value = 1e10;
						input>>v;
						if (v == "NA")
						{
							input>>no_overlaps;
							no_overlaps--;
						}
						else
						{
							value = v.toFloat();
						}
						(*(*score_grids_)[a])[i][j][k] = value;
					}
				}
				// empty line is skipped automatically
			}
			// empty line is skipped automatically
		}
		// the following line is also skipped automatically
		//cout<<(*all_energy_grids_[a])[x_boxes-1][y_boxes-1][z_boxes-1]<<endl;
	}

	// read just past the end of the last line
	getline(input, tmp);
}


list<pair<String, RegularData3D*> > ScoreGridSet::convertToRegularData3DGrids()
{
	list<pair<String, RegularData3D*> > reg3d_list;

	for (Size g = 0; g < noGrids(); g++)
	{
		Vector3 resolution(resolution_, resolution_, resolution_);
		Vector3 dimension(resolution_*sizeX(), resolution_*sizeY(), resolution_*sizeZ());
		RegularData3D* reg3d = new RegularData3D(origin_, dimension, resolution);

		ScoreGrid& scoregrid((*this)[g]);
		for (Size i = 0; i < sizeX(); i++)
		{
			for (Size j = 0; j < sizeY(); j++)
			{
				for (Size k = 0; k < sizeZ(); k++)
				{
					RegularData3D::IndexType index(i, j, k);
					reg3d->getData(index) = scoregrid[i][j][k];
				}
			}
		}

		String name = getGridAtomTypeName(g);
		reg3d_list.push_back(make_pair(name, reg3d));
	}

	return reg3d_list;
}


PharmacophoreConstraint* ScoreGridSet::getPharmacophoreConstraint()
{
	return pharm_constraint_;
}

void ScoreGridSet::setPharmacophoreConstraint(PharmacophoreConstraint* phc)
{
	pharm_constraint_ = phc;
}

void ScoreGridSet::setParameters(bool enforce_grid_boundaries, double out_of_grid_penalty, double interaction_no_scale)
{
	enforce_grid_boundaries_ = enforce_grid_boundaries;
	out_of_grid_penalty_ = out_of_grid_penalty;
	interaction_no_scale_ = interaction_no_scale;
}
