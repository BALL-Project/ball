// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: backboneModel.C,v 1.25.2.2 2006/07/15 00:35:34 amoll Exp $
//

#include <BALL/VIEW/MODELS/backboneModel.h>
#include <BALL/VIEW/PRIMITIVES/mesh.h>
#include <BALL/VIEW/PRIMITIVES/sphere.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/residue.h>
#include <BALL/KERNEL/chain.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/MATHS/common.h>

#include <BALL/MATHS/matrix44.h>
#include <BALL/VIEW/KERNEL/common.h>

#include <algorithm>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

#define NUCLEIC_ACID_RESIDUE 999

AddBackboneModel::ModelPart::ModelPart()
	: residues(),
		first_guide_point(0),
		last_guide_point(0)
{
}

AddBackboneModel::ModelPart::ModelPart(const ModelPart& m)
	: residues(m.residues),
		type(m.type),
		first_guide_point(m.first_guide_point),
		last_guide_point(m.last_guide_point),
		first_point(m.first_point),
		last_point(m.last_point)
{
}

const AddBackboneModel::ModelPart& AddBackboneModel::ModelPart::operator = (const AddBackboneModel::ModelPart& m)
	throw()
{
	residues = m.residues;
	type = m.type;
	first_guide_point = m.first_guide_point;
	last_guide_point = m.last_guide_point;
	first_point = m.first_point;
	last_point = m.last_point;
	return *this;
}


AddBackboneModel::AddBackboneModel()
	throw()
	: ModelProcessor(),
		tube_radius_((float)0.4),
		interpolation_steps_(9),
		number_of_ribbons_(3),
		ribbon_width_(1),
		ribbon_height_(0.2),
		care_for_SS_(false),
		ribbon_mode_(false)
{
	clear_();
}

AddBackboneModel::AddBackboneModel(const AddBackboneModel& bm)
	throw()
	:	ModelProcessor(bm),
		tube_radius_(bm.tube_radius_),
		interpolation_steps_(bm.interpolation_steps_)
{
}

AddBackboneModel::~AddBackboneModel()
	throw()
{
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "Destructing object of class AddBackboneModel" << endl;
	#endif 
}

void AddBackboneModel::clear()
	throw()
{
	ModelProcessor::clear();
	tube_radius_ = 0.4;
	clear_();
}

Processor::Result AddBackboneModel::operator() (Composite& composite)
{
	Residue* residue = dynamic_cast<Residue*>(&composite);
	if (residue == 0 || !residue->isAminoAcid()) 
	{
		return Processor::CONTINUE;
	}

	residues_to_be_rendered_.insert(residue);

	Protein* protein = (Protein*)residue->getAncestor(dummy_protein_);
	if (protein == 0) return Processor::CONTINUE;
	if (last_protein_ != protein)
	{
		calculateModelParts(*protein);
	}

	return Processor::CONTINUE;
}

bool AddBackboneModel::collectPositions(vector<Residue*> residues)
{
	if (residues.size() < 2) return false;

	AtomConstIterator it;

	vector<Vector3> offsets, backbones;

	String name = residues[0]->getName();
	char c = '?';
	if (name.size() > 0) c = name[0];

	// Secondary Structure Types
	vector<Index> sss;

	// nucleotides?
	if (name.size() == 1 && (c == 'C' || c == 'G' || c == 'T' || c == 'A' || c == 'U'))
	{
		for (Position r = 0; r < residues.size(); r++)
		{
			sss.push_back(NUCLEIC_ACID_RESIDUE);
			char found = 0;
			Residue& residue = *residues[r];
			BALL_FOREACH_ATOM(residue, it)
			{
				if (r == 0)
				{
					if (it->getName() == "O5*")
					{
						backbones.push_back(it->getPosition());
						found ++;
					}
				}
				else
				{
					if (it->getName() == "P")
					{
						backbones.push_back(it->getPosition());
						found ++;
					}
				}

				if (it->getName() == "C1*")
				{
					offsets.push_back(it->getPosition());
					found ++;
				}

				if (found == 2) break;
			}

			if (found != 2)
			{
				logString(String("Warning: not all atoms found for nucleotide backbone/cartoon model!"));
				return false;
			}
		}
	}
	else // proteins
	{
		SecondaryStructure* ss;
		for (Position r = 0; r < residues.size(); r++)
		{
			String batom = "CA";
			Residue& residue = *residues[r];
			ss = residue.getAncestor(dummy_ss_);
			if (ss) sss.push_back(ss->getType());
			else 		sss.push_back(SecondaryStructure::UNKNOWN);
			if (care_for_SS_)
			{
				if (ss && ss->getType() == SecondaryStructure::STRAND)
				{
					batom = "C";
				}
			}

			/*
			String full_name = residue.getFullName();

			// collect CA-Atoms and CH3 atoms in ACE(-N) and NME(-C)
			if (full_name.hasPrefix("ACE") ||
					full_name.hasPrefix("NME"))
			{
				Size found = 0;
				BALL_FOREACH_ATOM(residue, it)
				{
					if (it->getName().hasSubstring("CA") ||
							it->getName().hasSubstring("CH3"))
					{
						found ++;
						splines_.push_back(SplinePoint((*it).getPosition(), &*it));
						if (found == 2) return;
					}
				}

				return;
			}
			*/
			char found = 0;
			bool ca_found = false;
			bool o_found = false;

			// collect CA-Atoms and Os
			BALL_FOREACH_ATOM(residue, it)
			{
				if (it->getName() == batom)
				{
					backbones.push_back(it->getPosition());
					ca_found = true;
					found ++;
				}
				else if (it->getName() == "O")
				{
					offsets.push_back(it->getPosition());
					o_found = true;
					found ++;
				}

				if (found == 2) break;
			}

			if (found != 2)
			{
				logString(String("Warning: not all atoms found for protein backbone/cartoon model!"));
				String rname = residue.getFullName() + String(" ") + String(residue.getID());
				if (!ca_found) logString(String("Could not found CA for ") + rname);
				if (!o_found)  logString(String("Could not found O for ") + rname);
				return false;
			}
		} // all residues
	} // is a protein

	if (backbones.size() == 0 || offsets.size() == 0)
	{
		return false;
	}

	backbone_.push_back(backbones);
	offsets_.push_back(offsets);
	ss_.push_back(sss);

	return true;
}

void AddBackboneModel::dump(std::ostream& s, Size depth) const
	throw()
{
	BALL_DUMP_STREAM_PREFIX(s);
	BALL_DUMP_DEPTH(s, depth);
	BALL_DUMP_HEADER(s, this, this);

	ModelProcessor::dump(s, depth + 1);

	BALL_DUMP_STREAM_SUFFIX(s);
}

void AddBackboneModel::createTube_(Position set_pos, Position part_pos)
{
	ModelPart& part = model_parts_[set_pos][part_pos];			
	const Position middle_ribbon = (number_of_ribbons_ - 1) /  2;
	// points on the middle ribbon string
	vector<Vector3>& points = interpolated_points_[set_pos][middle_ribbon];
	// points on the neighbouring ribbon string
 	vector<Vector3>& n_points = interpolated_points_[set_pos][middle_ribbon + 1];
	Position start_pos = part.first_point;
	Position end_pos = part.last_point;

	// if we are not at the end of the interpolated points at the end of the modell we can draw the last point
 	if (end_pos < points.size() - 1) end_pos++;

	vector<Residue*>& residues = part.residues;

	// calculate the number of slides for the circle and the angle in between them
	Size slides = (Size)(8.0 + drawing_precision_ * 8.0);
	Angle slides_angle = Angle(360.0 / slides, false);

	// direction vector of the two current spline points
	Vector3 dir = points[start_pos + 1] - points[start_pos];
	if (!Maths::isZero(dir.getSquareLength())) dir.normalize();

	Vector3 right = n_points[start_pos] - points[start_pos];
	if (!Maths::isZero(right.getSquareLength())) right.normalize();
			
	////////////////////////////////////////////////////////////
	// calculate normal vector r to direction vector dir, with length of radius
	////////////////////////////////////////////////////////////
	Vector3 r = right - (right * dir) * dir;
	if (!Maths::isZero(r.getSquareLength())) r.normalize();
 	r *= tube_radius_;

	////////////////////////////////////////////////////////////
	// initialise a first set of points in a circle around the start position
	////////////////////////////////////////////////////////////
	const Position middle = (Position)(slides / 2.0);
	vector<Vector3> new_points;
	new_points.resize(slides);
	Vector3 x = r;
	new_points[0] = x;
	new_points[middle] = -x;

	Matrix4x4 m;
	m.setRotation(slides_angle, dir);
	// second half of points can be calculated by negating first half
	for (Position i= 1; i < middle; i++)
	{
		x = m * x;
		new_points[i] = x;
		new_points[i + middle] = -x;
	}
	
	////////////////////////////////////////////////////////////
	// same data structures for faster access
	////////////////////////////////////////////////////////////
	Mesh::Triangle t;

	////////////////////////////////////////////////////////////
	// create a new mesh with the points and triangles
	////////////////////////////////////////////////////////////
	// every residue get its own mesh to enable picking for the tube model
	Mesh* mesh = new Mesh();
	mesh->setComposite(residues[0]);
	geometric_objects_.push_back(mesh);

	/////////////////////////////////////////////////
	// create a cap for the start:
	t.v3 = slides;
	for (Position p = 0; p < slides; p++)
	{
		mesh->vertex.push_back(points[start_pos] + new_points[p]);
		mesh->normal.push_back(-dir);

		t.v2 = p;
		if (p < slides - 1)
		{
			t.v1 = p + 1;
		}
		else
		{
			t.v1 = 0;
		}
		mesh->triangle.push_back(t);
	}

	// middle point:
	mesh->vertex.push_back(points[start_pos]);
	mesh->normal.push_back(-dir);

	////////////////////////////////////////////////////////////
	// same data structures for faster access
	////////////////////////////////////////////////////////////
	Size s_old = mesh->vertex.size(); // start position of the last points in the meshs vertices
	Size s_new = 0;  									// start position of the  new points in the meshs vertices

	//////////////////////////////////////////////////
	// add first points again with different normals:
	for (Position p = 0; p < slides; p++)
	{
		mesh->vertex.push_back(points[start_pos] + new_points[p]);
		mesh->normal.push_back(new_points[p]);
	}

	//------------------------------------------------------>
	// iterate over the points in the spline
	//------------------------------------------------------>
	Position residue_pos = 0;
	Vector3 r_new;
	for (Position p = start_pos + 1; p < end_pos; p++)
	{
		// faster access to the current spline point
		const Vector3& point = points[p];
				
		// calculate new direction if not at the last point
 		if (p < end_pos - 1) 
		{
			dir = points[p + 1] - point;
			if (!Maths::isZero(dir.getSquareLength())) dir.normalize();
		
			Vector3 right = n_points[p] - points[p];
			if (!Maths::isZero(right.getSquareLength())) right.normalize();
			r_new = right - (right * dir) * dir;

 			if (!Maths::isZero(r_new.getSquareLength()))
			{
				if (!Maths::isZero(r_new.getSquareLength())) r_new.normalize();
 			}
			r_new *= tube_radius_;
			
			////////////////////////////////////////////////////////////
			// rotate all points of the circle according to new normal
			////////////////////////////////////////////////////////////
			m.setRotation(slides_angle, dir);
			x = r_new;
			new_points[0] = x;

			// second half of points can be calculated by negating first half
			new_points[middle] = -x;
			for (Position i = 1; i < middle; i++)
			{
				x = m * x;
				new_points[i] = x;
				new_points[i + middle] = -x;
			}
		}

		////////////////////////////////////////////////////////////
		// create a new mesh if we have a different atom now
		////////////////////////////////////////////////////////////
		if (p % interpolation_steps_ == 0 && p < end_pos - 1)
		{
			residue_pos++;
			if (residue_pos > residues.size())
			{
				BALLVIEW_DEBUG
				continue;
			}
			const Mesh* old_mesh = mesh;
			mesh = new Mesh();
			mesh->setComposite(residues[residue_pos]);
			geometric_objects_.push_back(mesh);

			// insert the vertices and normals of the last points again into the new mesh
			const Size old_mesh_size = old_mesh->vertex.size();
			for (Position point_pos = old_mesh_size - slides;
										point_pos < old_mesh_size; point_pos++)
			{
				mesh->vertex.push_back(old_mesh->vertex[point_pos]);
				mesh->normal.push_back(old_mesh->normal[point_pos]);
			}

			s_old = 0;
		}
				
		////////////////////////////////////////////////////////////
		// insert only the new points, the old ones are already stored in the mesh
		////////////////////////////////////////////////////////////
		// we will add an other point next, so here we do an off by one :)
		s_new = mesh->vertex.size();

		//------------------------------------------------------>
		// iterate over all points of the circle
		//------------------------------------------------------>
		for (Position point_pos = 0; point_pos < slides ; point_pos++)
		{
			mesh->vertex.push_back(point + new_points[point_pos]);
			mesh->normal.push_back(new_points[point_pos]);

			t.v1 = s_old;			// last lower
			t.v2 = s_old + 1;	// last upper
			t.v3 = s_new;			// new upper
			mesh->triangle.push_back(t);

			t.v1 = s_new;			// new upper
			t.v2 = s_new - 1;	// new lower
			t.v3 = s_old; 		// last lower
			mesh->triangle.push_back(t);

			s_old++;
			s_new++;
		}
	}

	/////////////////////////////////////////////////
	// create a cap for the end:
	end_pos--;
	dir = points[end_pos] - points[end_pos - 1];
	if (!Maths::isZero(dir.getSquareLength())) dir.normalize();

	right = n_points[end_pos] - points[end_pos];
	if (!Maths::isZero(right.getSquareLength())) right.normalize();
	x = right - (right * dir) * dir;
	if (!Maths::isZero(x.getSquareLength())) x.normalize();
	x *= tube_radius_;
	new_points[0] = x;
	new_points[middle] = -x;

	m.setRotation(slides_angle, dir);
	// second half of points can be calculated by negating first half
	for (Position i= 1; i < middle; i++)
	{
		x = m * x;
		new_points[i] = x;
		new_points[i + middle] = -x;
	}
	
	Position start = mesh->vertex.size();
	t.v3 = start + slides;
	for (Position p = 0; p < slides; p++)
	{
		mesh->vertex.push_back(points[end_pos] + new_points[p]);
		mesh->normal.push_back(dir);

		t.v1 = start + p;
		if (p < slides - 1)
		{
			t.v2 = start + p + 1;
		}
		else
		{
			t.v2 = start;
		}
		mesh->triangle.push_back(t);
	}

	// middle point:
	mesh->vertex.push_back(points[end_pos]);
	mesh->normal.push_back(dir);
}


void AddBackboneModel::clear_()
{
	last_protein_ = 0;
	residues_to_be_rendered_.clear();
	guide_points_.clear();
	interpolated_points_.clear();
	backbone_.clear();
	offsets_.clear();
	model_parts_.clear();
	ss_.clear();
}

bool AddBackboneModel::createGeometricObjects()
	throw()
{
	interpolation_steps_ = 7 + 6 * drawing_precision_;
	refineGuidePoints_();
	interpolate_();
	refineModelParts_();

	if (interpolated_points_.size() != model_parts_.size())
	{
		BALLVIEW_DEBUG
		return false;
	}

	for (Position set_pos = 0; set_pos < model_parts_.size(); set_pos++)
	{
		for (Position part = 0; part < model_parts_[set_pos].size(); part++)
		{
			if (model_parts_[set_pos][part].first_point == 
			    model_parts_[set_pos][part].last_point)
			{
				BALLVIEW_DEBUG
				continue;
			}

			createModel_(set_pos, part);
		}
	}

 	clear_();

	return true;
}

void AddBackboneModel::createModel_(Position set_pos, Position part_pos)
{
	if (set_pos > model_parts_.size() - 1) 
	{
		BALLVIEW_DEBUG
		return;
	}
	
	if (model_parts_[set_pos].size() <= part_pos)
	{
		BALLVIEW_DEBUG
		return;
	}

	ModelPart& part = model_parts_[set_pos][part_pos];
 	if (part.type == RIBBON) 
	{
		createRibbon_(set_pos, part_pos);
	}
 	else if (part.type == TUBE) 
	{
 		createTube_(set_pos, part_pos);
	}
}

void AddBackboneModel::assignModelType(ModelPart& part)
{
	part.type = TUBE;
	if (ribbon_mode_) part.type = RIBBON;
}

void AddBackboneModel::calculateModelParts(Protein& protein)
{
	last_protein_ = &protein;
	ResidueIterator rit = protein.beginResidue();

	if (!+rit) return;

	// create a new set of models:
	model_parts_.resize(model_parts_.size() + 1);

	// we will compare the chains of each two succeeding residues:
	Chain* this_chain = (*rit).getAncestor(dummy_chain_);
	Chain* next_chain = 0;

	// residues for each individual model:
	vector<Residue*> residues;
	// residues for a string of residues:
	vector<Residue*> connected_residues;

	while (+rit)
	{
		Residue& residue = *rit;
		rit++;

		Residue* residue_next = 0;
		if (+rit) 
		{
			residue_next = &*rit;
			next_chain = residue_next->getAncestor(dummy_chain_);
		}
		else
		{
			next_chain = 0;
		}


		if (!residue.isAminoAcid()) 
		{
			this_chain = next_chain;
			continue;
		}

		// this residue connected to next residue?
		bool connected = false;
		
		if (residue_next && next_chain == this_chain)
		{
			connected = residuesAreConnected_(residue, *residue_next);
		}

		// continue this model over the next residue?
		// at least the next residue must be connected to this one
		bool to_continue = connected;

		// do we create a different model for each type of secondary structure?
		if (to_continue && care_for_SS_)
		{
			SecondaryStructure* ss = residue.getAncestor(dummy_ss_);
			SecondaryStructure* ss2 = residue_next->getAncestor(dummy_ss_);
			// prevent seg faults if a residue is not in a SS
			if ((ss == 0) != (ss2 == 0)) to_continue = false;
			else if (ss != 0 && ss2 != 0)
			{
				to_continue = ss->getType() == ss2->getType();
			}
		}
		
		// collect the residues for one ModelPart, e.g. one continous tube model
		residues.push_back(&residue);
		// collect the residues for one continous residue string
		connected_residues.push_back(&residue);

		// if we will not continue this model for the next residue: 
		// create a ModelPart
		if (!to_continue)
		{
			// add a new model to the vector:
			vector<ModelPart>& current_set = model_parts_[model_parts_.size() - 1];
			current_set.resize(current_set.size() + 1);
			ModelPart& part = current_set[current_set.size() - 1];
			part.residues = residues;
			part.first_guide_point = connected_residues.size() - residues.size();
			part.last_guide_point = connected_residues.size() - 1;
			assignModelType(part);
			residues.clear();
		}

		if (!connected)
		{
			bool ok = collectPositions(connected_residues);
			connected_residues.clear();
			if (ok) model_parts_.resize(model_parts_.size() + 1);
			else model_parts_[model_parts_.size() - 1].clear();
		}

		this_chain = next_chain;
	} // all residues

	// remove an empty last set
	if (model_parts_[model_parts_.size() - 1].size() == 0) model_parts_.pop_back();

	calculateGuidePoints_();
}


bool AddBackboneModel::residuesAreConnected_(Residue& residue1, Residue& residue2)
{
	AtomConstIterator ait = residue1.beginAtom();
	for (; +ait; ait++)
	{
		AtomBondConstIterator bit = ait->beginBond();
		for (; +bit; ++bit)
		{
			if ((*bit).getType() == Bond::TYPE__HYDROGEN) continue;
			if (bit->getPartner(*ait)->getParent() == &residue2)
			{
				return true;
			}
		}
	}

	return false;
}

// inspired by Raster3D
void AddBackboneModel::calculateGuidePoints_()
{
	// distance of two ribbon strands
	float ribbon_distance = ribbon_width_ / (number_of_ribbons_ - 1);
	float rib2 = (number_of_ribbons_ - 1) * 0.5;

	// some temp variables
	Vector3 ca_to_ca, ca_to_o, plane, ribbon_to_ribbon;
	Vector3 average_r_to_r, offset_vector, old_r_to_r, old_ca_to_ca;

	// for all continueing sets of residues:
	guide_points_.resize(backbone_.size());
	for (Position s = 0; s < backbone_.size(); s++)
	{
		const vector<Vector3>& backbones = backbone_[s];
		const vector<Vector3>& offsets   =  offsets_[s];
		vector<vector<Vector3> >& guide_points =  guide_points_[s];
		guide_points.resize(number_of_ribbons_);
		for (Position ribbon = 0; ribbon < number_of_ribbons_; ribbon++)
		{
			guide_points[ribbon].reserve(backbones.size());
		}

		// for all residues in the current string of residues:
		for (Position r = 0; r < backbones.size(); r++)
		{
			// position of the atom on the backbone, e.g. C_alpha or P in nucleotides
			const Vector3& cap = backbones[r];
			// position of the atom off the backbone, e.g. O in proteins or C1* in nucleotides
			Vector3  op = offsets[r];

			// iterate over all residues but the last
			if (r < backbones.size() - 1)
			{
				ca_to_ca = backbones[r + 1] - cap;
				ca_to_o  = op - cap;

				plane = ca_to_ca % ca_to_o;
				ribbon_to_ribbon = plane % ca_to_ca;
				if (!Maths::isZero(ribbon_to_ribbon.getSquareLength())) ribbon_to_ribbon.normalize();

				if (r == 0) // first residue
				{
					average_r_to_r = ribbon_to_ribbon;
					offset_vector  = Vector3(0.);
				}
				else
				{
					// did we have a flip?
					if (ribbon_to_ribbon * old_r_to_r >= 0.) // no
					{
						average_r_to_r = old_r_to_r + ribbon_to_ribbon;
					}
					else
					{
						average_r_to_r = old_r_to_r - ribbon_to_ribbon;
					}

					offset_vector = old_ca_to_ca - ca_to_ca;
					if (care_for_SS_ && ss_[s][r] == SecondaryStructure::STRAND) offset_vector *= -1;
					if (!Maths::isZero(offset_vector.getSquareLength())) offset_vector.normalize();
				}

				if (!Maths::isZero(average_r_to_r.getSquareLength())) average_r_to_r.normalize();
			}
			else // last residue
			{
				average_r_to_r = old_r_to_r;
				offset_vector  = Vector3(0.);
			}
		
			// now build the guide points
			offset_vector += cap;

			// for all ribbon strands
			for (Position current_ribbon = 0; current_ribbon < number_of_ribbons_; current_ribbon++)
			{
				float fraction = (current_ribbon - rib2) * ribbon_distance;
				guide_points[current_ribbon].push_back(offset_vector + fraction * average_r_to_r);
				
			/*	
				Sphere* sphere = new Sphere();
				sphere->setPosition(guide_points[current_ribbon][r]);
				sphere->setRadius(0.2);
				geometric_objects_.push_back(sphere);
				*/
			}

			old_r_to_r = average_r_to_r;
			old_ca_to_ca = ca_to_ca;
		} // one continuing backbone

		if (ss_[s][0] == NUCLEIC_ACID_RESIDUE)
		{
			Position last = guide_points[0].size() - 1;
			for (Position current_ribbon = 0; current_ribbon < number_of_ribbons_; current_ribbon++)
			{
				Vector3 offset = (guide_points[current_ribbon][last] - guide_points[current_ribbon][last - 1]);
				offset *= .3;
				guide_points[current_ribbon][last] += offset;
			}
		}
	} // all continuous residue strings
}

void AddBackboneModel::createRibbon_(Position set_pos, Position part_pos)
{
	ModelPart& part = model_parts_[set_pos][part_pos];			
	const Position middle_ribbon = (number_of_ribbons_ - 1) /  2;
	// points on the middle ribbon string
	vector<Vector3>& points = interpolated_points_[set_pos][middle_ribbon];
	// points on the neighbouring ribbon string
	vector<Vector3>& n_points = interpolated_points_[set_pos][middle_ribbon + 1];
	Position start_pos = part.first_point;
	Position end_pos = part.last_point;

	// if we are not at the end of the interpolated points at the end of the modell we can draw the last point
	if (end_pos < points.size() - 1) end_pos++;

	vector<Residue*>& residues = part.residues;
	
	if (set_pos >= ss_.size() ||
			end_pos >= points.size())
	{
		Log.error() << "Set: " << set_pos << " ss size: " << ss_.size() << " end pos: " << end_pos << " size " << points.size() << std::endl;
		BALLVIEW_DEBUG
		return;
	}


	bool nucleotide = ss_[set_pos][start_pos / interpolation_steps_] == NUCLEIC_ACID_RESIDUE;

	// calculate the number of slides for the circle and the angle in between them
	Size slides = (Size)(8.0 + drawing_precision_ * 8.0);
	Angle slides_angle = Angle(360.0 / slides, false);

	// direction vector of the first two spline points
	Vector3 dir = points[start_pos + 1] - points[start_pos];
			
	////////////////////////////////////////////////////////////
	// calculate normal vector xn: in width direction
	////////////////////////////////////////////////////////////
 	Vector3 xn = n_points[start_pos] - points[start_pos];
	if (!Maths::isZero(xn.getSquareLength())) xn.normalize();

	////////////////////////////////////////////////////////////
	// initialise a first set of points in a circle around the start position
	////////////////////////////////////////////////////////////
	const Position middle = (Position)(slides / 2.0);
	vector<float> xs(slides), ys(slides);
	Position temp = 0;
	float step = 4. / slides;
	for (float i = -1.0; i < 1.0; i+= step)
	{
		xs[temp] = i * ribbon_width_;
		ys[temp] = sqrt((1-i*i / ribbon_width_ * ribbon_width_) * (ribbon_height_ * ribbon_height_));

		xs[temp + middle] = -xs[temp];
		ys[temp + middle] = -ys[temp];
		temp++;
	}

	Vector3 yn = dir % xn;
	if (!Maths::isZero(yn.getSquareLength())) yn.normalize();

	vector<Vector3> new_points(slides);
	for (Position p = 0; p < slides; p++)
	{
		new_points[p] = xs[p] * xn + ys[p] * yn;
	}

	////////////////////////////////////////////////////////////
	// create a new mesh with the points and triangles
	////////////////////////////////////////////////////////////
	// every residue get its own mesh to enable picking for the tube model
	Mesh* mesh = new Mesh();
	mesh->setComposite(residues[0]);
	geometric_objects_.push_back(mesh);

	Mesh::Triangle t;
	/////////////////////////////////////////////////
	// create a cap for the start:
	t.v3 = slides;
	for (Position p = 0; p < slides; p++)
	{
		mesh->vertex.push_back(points[start_pos] + new_points[p]);
		mesh->normal.push_back(-dir);

		t.v1 = p;
		if (p < slides - 1)
		{
			t.v2 = p + 1;
		}
		else
		{
			t.v2 = 0;
		}
		mesh->triangle.push_back(t);
	}

	// middle point:
	mesh->vertex.push_back(points[start_pos]);
	mesh->normal.push_back(-dir);

	Size s_old = mesh->vertex.size();  // start position of the last points in the meshs vertices

	////////////////////////////////////////
	// add first points again
	for (Position p = 0; p < slides; p++)
	{
		mesh->vertex.push_back(points[start_pos] + new_points[p]);
	}

	///////////////////////////////////////
	// set the normals:
	mesh->normal.push_back(-xn);
	for (Position p = 1; p < middle; p++)
	{
		mesh->normal.push_back((new_points[p - 1] - new_points[p + 1]) % dir);
	}

	Position npos = mesh->normal.size() - middle;
	for (Position p = npos; p < npos + middle; p++)
	{
		mesh->normal.push_back(- mesh->normal[p]);
	}

	Size s_new = 0;  // start position of the  new points in the meshs vertices

	Size vertex_off = 0;
	if (nucleotide) vertex_off = interpolation_steps_ / 2;

	//------------------------------------------------------>
	// iterate over all points in the spline
	//------------------------------------------------------>
	Position residue_pos = 0;
	for (Position p = start_pos + 1; p < end_pos; p++)
	{
		// faster access to the current spline point
		const Vector3& point = points[p];
				
 		Vector3 xn = n_points[p] - point;
		// new direction vector: new point - last point
 		if (p < end_pos - 1) 
		{
			dir = points[p + 1] - point;
		}

		xn = n_points[p] - point;
		if (!Maths::isZero(xn.getSquareLength())) xn.normalize();

		////////////////////////////////////////////////////////////
		// set the points of the ellipse
		////////////////////////////////////////////////////////////
		// normal vector to x normal and direction:
		Vector3 yn = dir % xn;
		if (!Maths::isZero(yn.getSquareLength())) yn.normalize();

		for (Position i = 0; i < slides; i++)
		{
			new_points[i] = xs[i] * xn + ys[i] * yn;
		}

		////////////////////////////////////////////////////////////
		// create a new mesh if we have a different atom now
		////////////////////////////////////////////////////////////
		if (p > vertex_off && 
				(p - vertex_off)% interpolation_steps_ == 0 &&
				residue_pos < residues.size() - 1)
		{
			residue_pos ++;
			const Mesh* old_mesh = mesh;
			mesh = new Mesh();
			mesh->setComposite(residues[residue_pos]);
			geometric_objects_.push_back(mesh);

			// insert the vertices and normals of the last points again into the new mesh
			const Size old_mesh_size = old_mesh->vertex.size();
			for (Position point_pos = old_mesh_size - slides;
										point_pos < old_mesh_size; point_pos++)
			{
				mesh->vertex.push_back(old_mesh->vertex[point_pos]);
				mesh->normal.push_back(old_mesh->normal[point_pos]);
			}

			s_old = 0;
		}
				
		////////////////////////////////////////////////////////////
		// insert only the new points, the old ones are already stored in the mesh
		////////////////////////////////////////////////////////////
		// we will add an other point next, so here we do an off by one :)
		s_new = mesh->vertex.size();

		//------------------------------------------------------>
		// iterate over all points of the circle
		//------------------------------------------------------>
		for (Position point_pos = 0; point_pos < slides ; point_pos++)
		{
			mesh->vertex.push_back(point + new_points[point_pos]);

			t.v1 = s_old;			// last lower
			t.v3 = s_old + 1;	// last upper
			t.v2 = s_new;			// new upper
			mesh->triangle.push_back(t);

			t.v1 = s_new;			// new upper
			t.v3 = s_new - 1;	// new lower
			t.v2 = s_old; 		// last lower
			mesh->triangle.push_back(t);

			s_old++;
			s_new++;
		}

		///////////////////////////////////////
		// set the normals:
		mesh->normal.push_back(-xn);
		for (Position p = 1; p < middle; p++)
		{
			mesh->normal.push_back((new_points[p - 1] - new_points[p + 1]) % dir);
		}

		Position npos = mesh->normal.size() - middle;
		for (Position p = npos; p < npos + middle; p++)
		{
			mesh->normal.push_back(- mesh->normal[p]);
		}
	}
	
	/////////////////////////////////////////////////
	// create a cap for the end:
	end_pos--;
	Position start = mesh->vertex.size();
	t.v3 = start + slides;
	for (Position p = 0; p < slides; p++)
	{
		mesh->vertex.push_back(points[end_pos] + new_points[p]);
		mesh->normal.push_back(dir);

		t.v2 = start + p;
		if (p < slides - 1)
		{
			t.v1 = start + p + 1;
		}
		else
		{
			t.v1 = start;
		}
		mesh->triangle.push_back(t);
	}

	// middle point:
	mesh->vertex.push_back(points[end_pos]);
	mesh->normal.push_back(dir);
}
			
// inspired by Raster3D
void AddBackboneModel::evaluateBSpline(const Vector3& v1, const Vector3& v2, 
																			 const Vector3& v3, float x, Vector3& result)
{
	float frac3 = 0.5*x*x;
	float frac1 = 0.5 * (1. - x) * (1. - x);
	float frac2 = 1. - (frac1 + frac3);
	
	result = frac1 * v1 + frac2 * v2 + frac3 * v3;	
}

void AddBackboneModel::interpolate_()
{
	float x_increment = 1./((float)interpolation_steps_);
	interpolated_points_.resize(guide_points_.size());

	for (Position s = 0; s < guide_points_.size(); s++)
	{
		interpolated_points_[s].resize(number_of_ribbons_);

		for (Position ribbon = 0; ribbon < number_of_ribbons_; ribbon++)
		{
			// the guide and spline points for this ribbon string:
			vector<Vector3>& guide_points = guide_points_[s][ribbon];
			vector<Vector3>& spline_points = interpolated_points_[s][ribbon];
			Size nr_guides = guide_points.size();

			if (nr_guides == 0)
			{
				BALLVIEW_DEBUG
				continue;
			}

			// calculate the number of interpolated points:
			const Size number_points = (nr_guides - 0) * interpolation_steps_;
			spline_points.resize(number_points);

			// position in the interpolated points vector:
			Position current_point = 0;
			
			// now add the end segments
			float x = 0.;
			Vector3 p1_2 = guide_points[1] - guide_points[0];
			Vector3 dummy = guide_points[0] - p1_2 / 2.0;
			for (Position i = 0; i < interpolation_steps_; i++)
			{
 				evaluateBSpline(dummy, guide_points[0], 
 												guide_points[1], x,
 												spline_points[current_point]);
				x += x_increment;
				current_point++;
			}

			for (Position residue = 1; residue < nr_guides - 1; residue++)
			{
				float x = 0.;
				for (Position i = 0; i < interpolation_steps_; i++)
				{
					evaluateBSpline(guide_points[residue - 1], guide_points[residue], 
													guide_points[residue + 1], x,
													spline_points[current_point]);
					x += x_increment;
					current_point++;
				}
			}

			// now add the end segments
			x = 0.;
			const Vector3& pi = guide_points[nr_guides - 2];
			const Vector3& pj = guide_points[nr_guides - 1];
			p1_2 = pi - pj;
			dummy = pj - p1_2 / 2.0;
			for (Position i = 0; i < interpolation_steps_; i++)
			{
 				evaluateBSpline(pi, pj, dummy, x, spline_points[current_point]);
				x += x_increment;
				current_point++;
			}
		}	

		
		/*
		for (Position y = 0; y < interpolated_points_.size(); y++)
		{
			for (Position x = 0; x < number_of_ribbons_; x++)
			{
				for (Position z = 0; z < interpolated_points_[y][x].size(); z++)
				{
				Sphere* sphere = new Sphere();
				sphere->setPosition(interpolated_points_[y][x][z]);
				sphere->setRadius(0.1);
				geometric_objects_.push_back(sphere);
				}
			}
		}
		*/
	
		// assign the points to the individial model parts:
		for (Position set = 0; set < interpolated_points_.size(); set++)
		{
			vector<ModelPart>& parts = model_parts_[set];
			// end position in interpolated_points_ of last model:
			Position last = 0;
			// maximum position in interpolated_points_:
			Size max = interpolated_points_[set][0].size() - 1;
			// iterate over all models:
			for (Position p = 0; p < parts.size(); p++)
			{
				ModelPart& part = parts[p];
				part.first_point = last;
				last += part.residues.size() * interpolation_steps_;
				if (last <= max)
				{
					part.last_point = last;
				}
				else
				{
					part.last_point = max;
				}
			}
		}
	}
}

void AddBackboneModel::refineModelParts_()
{
//   	return;
	vector<vector<ModelPart> > temp_models = model_parts_;

	for (Position set = 0; set < temp_models.size(); set++)
	{
		model_parts_[set].clear();
		vector<ModelPart>& models = temp_models[set];
		for (Position part_pos = 0; part_pos < models.size(); part_pos++)
		{
			ModelPart& part = models[part_pos];
			vector<Residue*>& residues = part.residues;
			vector<Position> new_residues;
			Position guide_start_offset = 0;
			for (Position residue_pos = 0; residue_pos < residues.size(); residue_pos++)
			{
				bool to_render = residues_to_be_rendered_.has(residues[residue_pos]);

				// current residue to be rendered: store it
				if (to_render)
				{
					new_residues.push_back(residue_pos);
				}
				else
				{
					// current residue not to be rendered and no residues stored: do nothing
					if (new_residues.size() == 0) 
					{
						guide_start_offset++;
						continue;
					}
				}

				if (to_render && residue_pos < residues.size() - 1)
				{
					continue;
				}

				// current residue not to be rendered and we have other residues to be rendered: OR
				// we are at the last residue: 
				// create a new ModelPart
				Size msize = model_parts_[set].size() + 1;
				model_parts_[set].resize(msize);
				ModelPart& new_part = model_parts_[set][msize - 1];

				for (Position r = 0; r < new_residues.size(); r++)
				{
					new_part.residues.push_back(residues[new_residues[r]]);
				}
				new_part.first_guide_point = guide_start_offset + part.first_guide_point;
				new_part.last_guide_point = new_part.first_guide_point + new_part.residues.size() - 1;
				new_part.first_point = new_part.first_guide_point * interpolation_steps_;
				Size max = interpolated_points_[set][0].size() - 1;
				// for the last point: we make an off by one, so that we get a connection to the next model
				// but prevent a segfault for the last point in the current set:
				new_part.last_point = BALL_MIN(max, (new_part.last_guide_point + 1) * interpolation_steps_);
				new_part.type = part.type;

				new_residues.clear();
			}
		}
	}
}

	} // namespace VIEW
} // namespace BALL
