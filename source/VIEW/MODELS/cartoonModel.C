// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: cartoonModel.C,v 1.60.16.1 2007/03/25 22:02:29 oliver Exp $
//

#include <BALL/VIEW/MODELS/cartoonModel.h>

#include <BALL/VIEW/PRIMITIVES/tube.h>
#include <BALL/VIEW/PRIMITIVES/disc.h>
#include <BALL/VIEW/PRIMITIVES/mesh.h>
#include <BALL/VIEW/PRIMITIVES/sphere.h>
#include <BALL/VIEW/PRIMITIVES/line.h>
#include <BALL/VIEW/PRIMITIVES/twoColoredTube.h>
#include <BALL/VIEW/KERNEL/common.h>

#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/PTE.h>

#include <BALL/MATHS/matrix44.h>
#include <BALL/STRUCTURE/geometricProperties.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{
		
AddCartoonModel::AddCartoonModel()
	: AddBackboneModel(),
		helix_radius_(2.1),
		helix_height_(0.1),
		helix_width_(1.8),
		arrow_width_(1.0),
		strand_width_(2.2),
		strand_height_(0.4),
		DNA_helix_radius_(0.5),
		DNA_ladder_radius_(0.8),
		DNA_base_radius_(0.2),
		draw_DNA_as_ladder_(false),
		draw_ribbon_(true)
{
	care_for_SS_ = true;
	tube_radius_ = 0.2;
}

AddCartoonModel::AddCartoonModel(const AddCartoonModel& cartoon)
	:	AddBackboneModel(cartoon),
		helix_radius_(cartoon.helix_radius_),
		helix_width_(cartoon.helix_width_),
		arrow_width_(cartoon.arrow_width_),
		strand_width_(cartoon.strand_width_),
		strand_height_(cartoon.strand_height_),
		DNA_helix_radius_(cartoon.DNA_helix_radius_),
		DNA_ladder_radius_(cartoon.DNA_ladder_radius_),
		DNA_base_radius_(cartoon.DNA_base_radius_),
		draw_DNA_as_ladder_(cartoon.draw_DNA_as_ladder_),
		draw_ribbon_(cartoon.draw_ribbon_)
{
	care_for_SS_ = true;
}

AddCartoonModel::~AddCartoonModel()
{
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "Destructing object of class AddCartoonModel" << endl;
	#endif 
}


void AddCartoonModel::calculateModelParts(Protein& protein)
{
	calculateComplementaryBases_(protein);
	AddBackboneModel::calculateModelParts(protein);
}

void AddCartoonModel::assignModelType(ModelPart& part)
{
	Residue& residue = *part.residues[0];
	
	// is this a chain with nucleic acids?
	const String name = residue.getName();
	if (name.size() == 1 &&
			(name == "A" || name == "C" || name == "G" || name == "T" || name == "U"))
	{
		part.type = NUCLEIC_ACID;
		return;
	}

	part.type = TUBE;

	const SecondaryStructure* ss = residue.getAncestor(dummy_ss_);
	if (ss == 0) return;
	
	Position type = (Position) ss->getType();

	if (type == SecondaryStructure::STRAND && part.residues.size() > 1)
	{
		part.type = STRAND;
	}
	else if (ss->getType() == SecondaryStructure::HELIX)
	{
		part.type = HELIX;
	}
}

void AddCartoonModel::createModel_(Position set_pos, Position part_pos)
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
 	if (part.type == RIBBON && part.residues.size() > 3) 
	{
		createRibbon_(set_pos, part_pos);
	}
 	else if (part.type == TUBE) 
	{
 		createTube_(set_pos, part_pos);
	}
 	else if (part.type == STRAND) 
	{
 		createStrand_(set_pos, part_pos);
	}
 	else if (part.type == HELIX) 
	{
 		if (draw_ribbon_) createHelix_(set_pos, part_pos);
		else 							createTubeHelix_(set_pos, part_pos);
	}
 	else if (part.type == NUCLEIC_ACID) 
	{
		if (draw_DNA_as_ladder_) createSimpleNucleicAcid_(set_pos, part_pos);
		else 										 createWatsonCrickModel_(set_pos, part_pos);
	}
}

// --------------------- HELIX ------------------------------------------------
void AddCartoonModel::createTubeHelix_(Position set_pos, Position part_pos)
{
	ModelPart& part = model_parts_[set_pos][part_pos];			
	const Position middle_ribbon = (number_of_ribbons_ - 1) /  2;
	// points on the middle ribbon string
	vector<Vector3>& points = interpolated_points_[set_pos][middle_ribbon];
	Position start_pos = part.first_point;
	Position end_pos = part.last_point;
	vector<Residue*>& residues = part.residues;

	Vector3 first_point = points[start_pos];
	Vector3 last_point = points[end_pos];
	// direction vector of the two current spline points
	Vector3 normal = last_point - first_point;

	// calcluate slices for the helix cylinder according to the C-atoms
	Vector3 last_pos = first_point;
	Vector3 diff = (normal / (residues.size()));

	for (Position p = 0; p < residues.size(); p++)
	{
		Tube* tube = new Tube;
		tube->setRadius(helix_radius_);
		tube->setVertex1(last_pos);
		last_pos += diff;
		tube->setVertex2(last_pos);
		tube->setComposite(residues[p]);
		geometric_objects_.push_back(tube);
	}
		
	// add a disc at the beginning and the end of the cylinder to close it
	Disc* disc = new Disc( Circle3(first_point, -normal, helix_radius_));
	disc->setComposite(residues[0]);
	geometric_objects_.push_back(disc);

	disc = new Disc(Circle3(last_point, normal, helix_radius_));
	disc->setComposite(residues[residues.size() - 1]);
	geometric_objects_.push_back(disc);
}


// --------------------- STRAND --------------------------------------------
void AddCartoonModel::createStrand_(Position set_pos, Position part_pos)
{
	ModelPart& part = model_parts_[set_pos][part_pos];			
	const Position middle_ribbon = (number_of_ribbons_ - 1) /  2;
	// points on the middle ribbon string
	vector<Vector3>& points = interpolated_points_[set_pos][middle_ribbon];
	// points on the neighbouring ribbon string
 	vector<Vector3>& n_points = interpolated_points_[set_pos][middle_ribbon + 1];
	Position start_pos = part.first_point;
	Position end_pos = part.last_point;
	if (end_pos < points.size() - 1) end_pos ++;


	vector<Residue*>& residues = part.residues;

	// direction vector of the two current spline points
	Vector3 dir = points[start_pos + 1] - points[start_pos];
			
	Vector3 right = n_points[start_pos] - points[start_pos];
	right.normalize();
	right *= strand_width_;

	// normal in height direction
	Vector3 hn= right % dir;
	hn.normalize();
	hn *= strand_height_;

	// put first four points into the mesh (and first two triangles)
	Vector3 last_points[4];
	last_points[0] = points[start_pos] - (right / 2.0) + (hn / 2.0);
	last_points[1] = last_points[0] - hn;
	last_points[2] = last_points[1] + right;
	last_points[3] = last_points[2] + hn;

	Mesh* mesh = new Mesh;
	mesh->setComposite(residues[0]);
	geometric_objects_.push_back(mesh);

	vector<Vector3>& vertices = mesh->vertex;
	vector<Vector3>& normals  = mesh->normal;

	for (Position i = 0; i < 4; i++)
	{
		vertices.push_back(last_points[i]);
		normals.push_back(dir * -1.);
	}

	Surface::Triangle t;
	t.v1 = 0;
	t.v2 = 1;
	t.v3 = 2;
	mesh->triangle.push_back(t);

	t.v1 = 0;
	t.v2 = 2;
	t.v3 = 3;
	mesh->triangle.push_back(t);
	// we insert all points twice (the first once even thrice!) 
	// in order to get sensible normals for
	// the triangles...
	for (Position p = 0; p < 4; p++)
	{
		vertices.push_back(last_points[p]);
		vertices.push_back(last_points[p]);
	}

	Position last_vertices = 4;

	const Vector3& perpendic = right;

	normals.push_back(-perpendic);
	normals.push_back(-hn);

	normals.push_back(-perpendic);
	normals.push_back(hn);

	normals.push_back(perpendic);
	normals.push_back(hn);

	normals.push_back(perpendic);
	normals.push_back(-hn);

	//------------------------------------------------------>
	// iterate over all points in the spline
	//------------------------------------------------------>
	Position residue_pos = 0;
	for (Position p = start_pos + 1; p < end_pos - interpolation_steps_; p++)
	{
		// faster access to the current spline point
		const Vector3& point = points[p];
				
		// new direction vector: new point - last point
 		const Vector3 dir_new = points[p + 1] - point;

 		Vector3 right = n_points[p] - point;
 		right.normalize();

		// vector in height direction
		Vector3 hn = right % dir_new;
		hn.normalize();

		////////////////////////////////////////////////////////////
		// create a new mesh if we have a different atom now
		////////////////////////////////////////////////////////////
		if (p > start_pos && p % interpolation_steps_ == 0)
		{
			residue_pos++;
			const Mesh* old_mesh = mesh;
			mesh = new Mesh();
			mesh->setComposite(residues[residue_pos]);
	
			geometric_objects_.push_back(mesh);

			// insert the vertices and normals of the last points again into the new mesh
			const Size old_mesh_size = old_mesh->vertex.size();
			for (Position point_pos = old_mesh_size - 8;
										point_pos < old_mesh_size; point_pos++)
			{
				mesh->vertex.push_back(old_mesh->vertex[point_pos]);
				mesh->normal.push_back(old_mesh->normal[point_pos]);
			}
			last_vertices = 0;
		}
				
		////////////////////////////////////////////////////////////
		// insert only the new points, the old ones are already stored in the mesh
		////////////////////////////////////////////////////////////
		// we will add an other point next, so here we do an off by one :)

		drawStrand_(points[p], right, hn, strand_width_, last_vertices, *mesh);
	}


	//////////////////////////////////////
	// finally, we draw the arrow

	Position arrow_start = end_pos - interpolation_steps_;

	float new_arrow_width = arrow_width_ + strand_width_;
	float arrow_width_diff = new_arrow_width / (interpolation_steps_ - 1);

	for (Position j = arrow_start; j < arrow_start + interpolation_steps_; j++)
	{
		Vector3 right = n_points[j] - points[j];

		if (j < end_pos - 1) dir = points[j + 1] - points[j];

		Vector3 hn = right % dir;
		
 		drawStrand_(points[j], right, hn, new_arrow_width, last_vertices, *mesh);

		// interpolate the depth of the box
		new_arrow_width -= arrow_width_diff;
	}
}


void AddCartoonModel::insertTriangle_(Position v1, Position v2, Position v3, Mesh& mesh)
{
	Surface::Triangle t;
	t.v1 = v1;
	t.v2 = v2;
	t.v3 = v3;
	mesh.triangle.push_back(t);

	Vector3 ab = mesh.vertex[v2] - mesh.vertex[v1];
	Vector3 ac = mesh.vertex[v3] - mesh.vertex[v1];
	Vector3 n = ab % ac;
	if (Maths::isZero(n.getSquareLength())) return;

	n.normalize();

	mesh.normal[v1] += n;
	mesh.normal[v2] += n;
	mesh.normal[v3] += n;
}


void AddCartoonModel::drawStrand_(const Vector3& start,
																	Vector3& right,
																	Vector3& hn,
																	float strand_width,
																	Position& last_vertices,
																	Mesh& mesh)
{
	vector<Vector3>& vertices = mesh.vertex;
	vector<Vector3>& normals  = mesh.normal;

	right.normalize();
	hn.normalize();

	right *= strand_width;
	hn *= -strand_height_;

	Vector3 current_points[4];
	// lower left
	current_points[0] = start - (right / 2.0) - (hn / 2.0);
	// upper left
	current_points[1] = current_points[0] + hn;
	// upper right
	current_points[2] = current_points[1] + right;
	// lower right
	current_points[3] = current_points[2] - hn;

	// put the next 4 points and 8 triangles into the mesh
	for (Position p = 0; p < 4; p++)
	{
		vertices.push_back(current_points[p]);
		vertices.push_back(current_points[p]);
	}

	// normal for up and down
	// perpendic for sides
	Position nstart = normals.size();
	normals.push_back(-right);
	normals.push_back(-hn);
	normals.push_back(-right);
	normals.push_back(hn);

	for (Position p = nstart; p < nstart + 4; p++)
	{
		normals.push_back(-normals[p]);
	}

	// one side band
	insertTriangle_(last_vertices +2, last_vertices, last_vertices + 10, mesh);
	insertTriangle_(last_vertices +10, last_vertices, last_vertices +  8, mesh);
	
	// other side band
	insertTriangle_(last_vertices + 4, last_vertices + 12, last_vertices + 14, mesh);
	insertTriangle_(last_vertices + 4, last_vertices + 14, last_vertices +  6, mesh);

	// "upper" band
	insertTriangle_(last_vertices + 5, last_vertices +  3, last_vertices + 13, mesh);
	insertTriangle_(last_vertices + 13, last_vertices + 3, last_vertices + 11, mesh);

	// "lower" band
	insertTriangle_(last_vertices + 1, last_vertices +  7, last_vertices + 15, mesh);
 	insertTriangle_(last_vertices + 15, last_vertices +  9, last_vertices + 1, mesh);

	last_vertices += 8;
}


// draw a helix as ribbon model
void AddCartoonModel::createHelix_(Size set_start, Size part_pos)
{
	createRibbon_(set_start, part_pos);
}


// -----------------------------------------------------------------------
// --------------------- nucleic acids ----------------------------------->
void AddCartoonModel::calculateComplementaryBases_(const Composite& composite)
{
	complementary_bases_.clear();
	Protein& protein = *(Protein*)&composite;
	if (protein.countChains() < 2) return;

	HashMap<Residue*, Vector3> positions;

	ResidueIterator rit = protein.beginResidue();
	for (; +rit; rit++)
	{
		const String name = (*rit).getName();
		if (name.size() > 1) continue;
		
		char c = name[0];
		if (c != 'A' && c != 'C' && c != 'G' && c != 'U' && c != 'T')
		{
			continue;
		}

		GeometricCenterProcessor gcp;
		(*rit).apply(gcp);
		positions[&*rit] = gcp.getCenter();
	}

	if (positions.size() < 2) return;

	Chain& chain1 = *protein.getChain(0);
	Chain& chain2 = *protein.getChain(1);

	ResidueIterator rit1 = chain1.beginResidue();

	HashSet<Residue*> chain2_residues;
	ResidueIterator rit2 = chain2.beginResidue();
	for (; +rit2; ++rit2)
	{
		chain2_residues.insert(&*rit2);
	}

	for (; +rit1; ++rit1)
	{
		float distance = 200;
		if (rit1->getName().size() < 1) continue;
		char c1 = rit1->getName()[0];
		Residue* found_partner = 0;
		HashSet<Residue*>::Iterator rit2 = chain2_residues.begin();
		for (; rit2 != chain2_residues.end(); rit2++)
		{
			String name2 = (**rit2).getName();
			if (name2.size() < 1) continue;
			char c2 = name2[0];

			if ((c1 == 'A' && c2 != 'T' && c2 != 'U') ||
				  (c1 == 'C' && c2 != 'G') ||
					(c1 == 'G' && c2 != 'C') ||
					(c1 == 'T' && c2 != 'A') ||
					(c1 == 'U' && c2 != 'A'))
			{
				continue;
			}

			const float new_distance = ((positions[&*rit1] - positions[*rit2]).getSquareLength());
			if (new_distance > distance) 
			{
				continue;
			}

			found_partner = *rit2;
			distance = new_distance;
		}

		if (found_partner != 0)
		{
			complementary_bases_[&*rit1] =  found_partner;
			complementary_bases_[found_partner]  = &*rit1;
			chain2_residues.erase(found_partner);
		}
		else
		{
		}
	}
}


void AddCartoonModel::createWatsonCrickModel_(Position set_pos, Position part_pos)
{
	createRibbon_(set_pos, part_pos);

	ModelPart& part = model_parts_[set_pos][part_pos];			
 	const Position middle_ribbon = (number_of_ribbons_ - 1) /  2;
	// points on the middle ribbon string
 	vector<Vector3>& points = interpolated_points_[set_pos][middle_ribbon];
	// points on the neighbouring ribbon string
 	Position start_pos = part.first_point;
 	Position end_pos = part.last_point;
	vector<Residue*>& residues = part.residues;

	for (Position p = 0; p < residues.size(); p++)
	{
		Residue* r = residues[p];
		if (r->getName().size() != 1) continue;
		Mesh* mesh;

		vector<Vector3> positions;
		Atom* connection_atom = 0;
		Atom* atoms[11];

		vector<Atom*> hbond_atoms;
		char rname = r->getName()[0];

		if (rname == 'A' ||
				rname == 'G')
		{
			if (rname == 'A')
			{
				String atom_names[10] = {"N9", "C4", "N3", "C2", "N1", "C6", "C5", "N7", "C8", "N6"};
				if (!assignNucleotideAtoms_(*r, 10, atom_names, atoms)) continue;
				hbond_atoms.push_back(atoms[4]);
				hbond_atoms.push_back(atoms[9]);
			}
			else
			{
				String atom_names[11] = {"N9", "C4", "N3", "C2", "N1", "C6", "C5", "N7", "C8", "O6", "N2"};
				if (!assignNucleotideAtoms_(*r, 11, atom_names, atoms)) continue;
				hbond_atoms.push_back(atoms[9]);
				hbond_atoms.push_back(atoms[4]);
				hbond_atoms.push_back(atoms[10]);
			}

			for (Position p = 0; p < 9; p++) positions.push_back(atoms[p]->getPosition());

			mesh = createDoubleRing_(positions);

			connection_atom = atoms[0];

			/*
			Sphere* s = new Sphere;
			s->setComposite(atoms[8]);
			s->setRadius(DNA_base_radius_);
			s->setPosition(atoms[8]->getPosition());
			geometric_objects_.push_back(s);
			*/
		}
		// -------------------------------------------------
		else if (rname == 'C')
		{
			String atom_names[9] = {"N1", "C2", "N3", "C4", "C5", "C6", "N4", "O2", ""};
			if (!assignNucleotideAtoms_(*r, 8, atom_names, atoms)) continue;

			connection_atom = atoms[0];
			hbond_atoms.push_back(atoms[6]);
			hbond_atoms.push_back(atoms[2]);
			hbond_atoms.push_back(atoms[7]);
			for (Position p = 0; p < 6; p++) positions.push_back(atoms[p]->getPosition());
 			mesh = create6Ring_(positions);
		}
		// -------------------------------------------------
		else if (rname == 'T' ||
						 rname == 'U')
		{
			if (rname == 'T')
			{
				String atom_names[9] = {"C2", "N3", "C4", "C5", "C6", "N1", "O4", "", ""};
				if (!assignNucleotideAtoms_(*r, 7, atom_names, atoms)) continue;
			}
			else
			{
				String atom_names[9] = {"C2", "N3", "C4", "C5", "C6", "N1", "O4", "O2", ""};
				if (!assignNucleotideAtoms_(*r, 7, atom_names, atoms)) continue;
			}

			connection_atom = atoms[5];
			hbond_atoms.push_back(atoms[1]);
			hbond_atoms.push_back(atoms[6]);
			for (Position p = 0; p < 6; p++) positions.push_back(atoms[p]->getPosition());
 			mesh = create6Ring_(positions);
		}
		else
		{
			// unknown base, abort for this Residue
			BALLVIEW_DEBUG
			continue;
		}

		
		mesh->setComposite(r);
		// --------------------------------------------
		// draw ribose
		// --------------------------------------------

		String atom_names[9] = {"C1*", "C2*", "C3*", "C4*", "O4*", "", "", "", ""};
		if (!assignNucleotideAtoms_(*r, 5, atom_names, atoms)) continue;

		atoms[5] = atoms[0];

		// calculate middle point
		Vector3 middle;
		
		for (Position p = 0; p < 5; p++)
		{
			middle += atoms[p]->getPosition();
		}

		middle /= 5.0;
		
		/// create an array with the position of the pentagram
		Vector3 pos[6];
		pos[2] = atoms[2]->getPosition();
		pos[3] = atoms[3]->getPosition();

		Vector3 c3_c4 = atoms[3]->getPosition() - atoms[2]->getPosition();
		
		Vector3 axis((atoms[2]->getPosition() - middle) % c3_c4);

		Angle angle = Angle(72.0, false);
		Matrix4x4 m1;
		m1.setRotation(angle, axis);

		Matrix4x4 m2;
		m2.setRotation(-angle, axis);

		// calculate Position of C2 point
		Vector3 c3_c2 = -(m2 * c3_c4);

		pos[1] = atoms[2]->getPosition() + c3_c2;

		// calculate Position of O4 point
		Vector3 c4_o4 = -(m1 * -c3_c4);

		pos[4] = atoms[3]->getPosition() + c4_o4;

		// calculate Position of C1 point
		Vector3 o4_c1 = m1 * (pos[4] - atoms[3]->getPosition());

		pos[0] = pos[4] + o4_c1;

		// first one again
		pos[5] = pos[0];

		for (Position p = 0; p < 5; p++)
		{
			// render outside of ribose, mesh is done below
			drawRiboseAtoms_(atoms[p], atoms[p + 1], pos[p], pos[p + 1]);
		}

		
		TwoColoredTube* tube = new TwoColoredTube;
		tube->setVertex1(pos[0]);
		tube->setVertex2(connection_atom->getPosition());
		tube->setComposite(atoms[0]->getBond(*connection_atom));
		tube->setRadius(DNA_base_radius_);
		geometric_objects_.push_back(tube);

		// draw triangles
		
		if (Maths::isZero(axis.getSquareLength())) axis = Vector3(1,0,0);

		axis.normalize();
		axis *= DNA_base_radius_;

		mesh = new Mesh;
		mesh->setComposite(atoms[0]->getParent());
		geometric_objects_.push_back(mesh);

		Surface::Triangle t;

		mesh->vertex.push_back(middle - axis);
		mesh->vertex.push_back(middle + axis);
		mesh->normal.push_back(-axis);
		mesh->normal.push_back(axis);

		mesh->vertex.push_back(pos[0] - axis);
		mesh->vertex.push_back(pos[0] + axis);
		mesh->normal.push_back(-axis);
		mesh->normal.push_back(axis);

		for (Position p = 0; p < 6; p++)
		{
			mesh->vertex.push_back(pos[p] - axis);
			mesh->vertex.push_back(pos[p] + axis);
			mesh->normal.push_back(-axis);
			mesh->normal.push_back(axis);

			const Size n = mesh->vertex.size();
			
			t.v3 = 0;
			t.v2 = n - 4;
			t.v1 = n - 2;
			mesh->triangle.push_back(t);

			t.v1 = 1;
			t.v2 = n - 3;
			t.v3 = n - 1;
 			mesh->triangle.push_back(t);
		}

		// --------------------------------------------
		// draw connection to backbone from C3*-Atom
		// --------------------------------------------
		// search if we have a spline point near the atom
		const Vector3& c3pos = atoms[2]->getPosition();
		Vector3 spline_pos;
		float distance = 26;
		for (Position spos = start_pos; spos < end_pos; spos++)
		{
			float new_distance = (points[spos] - c3pos).getSquareLength();
			if (new_distance < distance)
			{
				distance = new_distance;
				spline_pos = points[spos];
			}
		}

		if (distance < 26)
		{
			Tube* tube = new Tube;
			tube->setComposite(r);
			tube->setVertex1(c3pos);
			tube->setVertex2(spline_pos);
			tube->setRadius(DNA_base_radius_);
			geometric_objects_.push_back(tube);
		}

/*
		//////////////////////////////////////////////////////////////////
		// draw hydrogen bonds and bonds between the two acids of one pair
		//////////////////////////////////////////////////////////////////
		for (Position p = 0; p < hbond_atoms.size(); p++)
		{
			Atom* a1 = hbond_atoms[p];
			AtomBondIterator bit = a1->beginBond();
			for (; +bit; ++bit)
			{
				if ((*bit).getType() == Bond::TYPE__HYDROGEN)
				{
					Atom* a2 = (*bit).getPartner(*a1);
					Line* line = new Line;
					line->setVertex1(a1->getPosition());
					line->setVertex2(a2->getPosition());
					line->setComposite(&*bit);
					geometric_objects_.push_back(line);
				}
				else
				{
					Atom* a2 = (*bit).getPartner(*a1);

 					if (a1->getElement().getSymbol() == "H" ||
 					    a2->getElement().getSymbol() == "H" ||
					    a1->getName() == "N3" ||
							a1->getName() == "N1" && r->getName() == "G")
					{
						continue;
					}
					
					Sphere* s = new Sphere;
					s->setComposite(a1);
					s->setPosition(a1->getPosition());
					s->setRadius(DNA_base_radius_);
					geometric_objects_.push_back(s);

					TwoColoredTube* tube= new TwoColoredTube;
					tube->setVertex2(a1->getPosition());
					tube->setVertex1(a2->getPosition());
					tube->setRadius(DNA_base_radius_);
					tube->setComposite(&*bit);
					geometric_objects_.push_back(tube);
				}
			} // bonds
		} // hyrogen atoms
		*/
	}	// all residues
}

void AddCartoonModel::drawRiboseAtoms_(const Atom* atom1, const Atom* atom2, const Vector3& v1, const Vector3& v2)
{
	const Bond* bond = atom1->getBond(*atom2);
	if (bond == 0)
	{
	  BALLVIEW_DEBUG
		return;
	}

	TwoColoredTube* tube= new TwoColoredTube;
	tube->setVertex2(v1);
	tube->setVertex1(v2);
	tube->setRadius(DNA_base_radius_);
	tube->setComposite(bond);
	geometric_objects_.push_back(tube);

	Sphere* s = new Sphere;
	s->setComposite(atom1);
	s->setPosition(v1);
	s->setRadius(DNA_base_radius_);
	geometric_objects_.push_back(s);
}

void AddCartoonModel::renderNucleotideOutline_(const vector<Vector3>& positions, Vector3 uv, Mesh& mesh)
{
	Size slides = 4 + drawing_precision_ * 8;
	Angle slides_angle(180. / (float)slides + 1, false);

	vector<Vector3>& vertices = mesh.vertex;
 	vector<Vector3>& normals  = mesh.normal;
	normals.resize(vertices.size() + (positions.size() + 1) * slides);
	Index vpos = vertices.size() - 1;

	// calculate the center point
	Vector3 middle_point;
	for (Position p = 0; p < positions.size(); p++)
	{
		middle_point += positions[p];
	}

	middle_point /= positions.size();

	Matrix4x4 m;
	for (Position p = 0; p < positions.size() + 1; p++)
	{
		// we want to iterate one time around all edges
		Position x = p;
		if (x == positions.size()) x = 0;
		Vector3 v1, v2, v3;
		if (x > 0) v1 = positions[x - 1];
		else       v1 = positions[positions.size() - 1];
		
		v2 = positions[x];

		if (x < positions.size() - 1) v3 = positions[x + 1];
		else 												  v3 = positions[0];

		Vector3 t1 = v2 - v1;
		t1.normalize();

		Vector3 t2 = v2 - v3;
		t2.normalize();

		// vector to the outside
		Vector3 dir = t1 + t2;
		dir.normalize();

		// do we have to flip the outside vector?
		if (middle_point.getSquareDistance(v2 + dir) <
				middle_point.getSquareDistance(v2 - dir))
		{
			dir *= -1;
		}

		// rotate the upvector around an axis perpendicular to outvector
		m.setRotation(slides_angle, (dir % uv));

		Vector3 nv = uv;
		nv *= -DNA_base_radius_;
		Vector3 xv = nv;

		// first corner point: only add vertices
		if (p == 0)
		{
			for (Position s = 0; s < slides; s++)
			{
				if (s == 0) vertices.push_back(v2 + xv);
				else if (s == slides - 1) vertices.push_back(v2 - xv);
				else vertices.push_back(v2 + nv);
 				nv = m * nv;
			}
			vpos += slides;
			continue;
		}

		for (Position s = 0; s < slides; s++)
		{
			if (s == 0) vertices.push_back(v2 + xv);
			else if (s == slides - 1)vertices.push_back(v2 - xv);
			else vertices.push_back(v2 + nv);

			nv = m * nv;
			vpos += 1;

			// first row: only add vertices
			if (s == 0) continue;

   		insertTriangle_(vpos - 1 - slides, vpos - 1, vpos, mesh);
   		insertTriangle_(vpos - slides, vpos - 1 - slides, vpos, mesh);
		} // all slides
	} // all corners
}

bool AddCartoonModel::assignNucleotideAtoms_(Residue& r, Size nr_atoms, 
																						 String atom_names[9], Atom* atoms[9])
{
	for (Position p = 0; p < 9; p++) atoms[p] = 0;

	AtomIterator it;
	BALL_FOREACH_ATOM(r, it)
	{
		for (Position p = 0; p < nr_atoms; p++)
		{
			if (it->getName() == atom_names[p])
			{
				atoms[p] = &*it;
				break;
			}
		}
	}

	bool error = false;
	for (Position p = 0; p < nr_atoms; p++)
	{
		if (atoms[p] == 0)
		{
			error = true;
			break;
		}
	}

	return !error;
}


void AddCartoonModel::createSimpleNucleicAcid_(Position set_pos, Position part_pos)
{
	float bak_radius = tube_radius_;
	tube_radius_ = DNA_ladder_radius_;
	createTube_(set_pos, part_pos);
	tube_radius_ = bak_radius;

	HashMap<const Residue*, Vector3> base_positions;

	ModelPart& part = model_parts_[set_pos][part_pos];			
	vector<Residue*>& residues = part.residues;
	// points on the neighbouring ribbon string
 	Position start_pos = part.first_point;
 	Position end_pos = part.last_point;
 	const Position middle_ribbon = (number_of_ribbons_ - 1) /  2;
	// points on the middle ribbon string
	vector<Vector3>& points = interpolated_points_[set_pos][middle_ribbon];

	for (Position r = 0; r < residues.size(); r++)
	{
		Residue& residue = *residues[r];
		AtomIterator ait = residue.beginAtom();
		for (;+ait; ++ait)
		{
			if (ait->getName() != "P") continue;
			
			Vector3 c3pos = ait->getPosition();

			Vector3 spline_pos;
			float distance = 16;
			for (Position spos = start_pos; spos < end_pos; spos++)
			{
				float new_distance = (points[spos] - c3pos).getSquareLength();
				if (new_distance < distance)
				{
					distance = new_distance;
					spline_pos = points[spos];
				}
			}

			if (distance < 16)
			{
				base_positions[&residue] = spline_pos;
			}

			break;
		}
	}

	for (Position r = 0; r < residues.size(); r++)
	{
		const Residue* res = residues[r];

		// prevent double drawing for complementary bases
		if (complementary_bases_.has(res) &&
				complementary_bases_[res] < res) 
		{
			continue;
		}

		if (!base_positions.has(res))
		{
			continue;
		}

		Vector3 base_position = base_positions[res];

		AtomConstIterator ait;

		// ================ draw unpaired bases ===============
		if (!complementary_bases_.has(res))
		{
			String atom_name;
			if (res->getName() == "A" ||
					res->getName() == "G")
			{
				atom_name = "C6";
			}
			else if (res->getName() == "C") atom_name = "C4";
			else if (res->getName() == "T") atom_name = "N3";
				
			const Atom* end_atom = 0;
			BALL_FOREACH_ATOM(*res, ait)
			{
				if (ait->getName() == atom_name)
				{
					end_atom = &*ait;
					break;
				}
			}

			if (end_atom == 0) continue;

			Tube* tube = new Tube;
			tube->setVertex1(base_position);
			tube->setVertex2(end_atom->getPosition());
			tube->setComposite(res);
			tube->setRadius(DNA_ladder_radius_);
 			geometric_objects_.push_back(tube);

			Sphere* sphere1 = new Sphere;
			sphere1->setPosition(end_atom->getPosition());
			sphere1->setRadius(DNA_ladder_radius_);
			sphere1->setComposite(res);
			geometric_objects_.push_back(sphere1);

			Circle3 c(base_position, -(end_atom->getPosition() - base_position), 
								DNA_ladder_radius_);
			Disc* disc = new Disc(c);
			disc->setComposite(res);
			geometric_objects_.push_back(disc);

			continue;
		}


		// ================ draw paired bases ===============
		const Residue* partner = complementary_bases_[res];
		const Atom* partner_base = 0;
		BALL_FOREACH_ATOM(*partner, ait)
		{
			if (ait->getName() == "P") 
			{	
				partner_base = &*ait;
				break;
			}
		}

		if (partner_base == 0)
		{
			/*
			BALL_FOREACH_ATOM(*partner, ait)
			{
				if (ait->getName() == "O5*") 
				{
					partner_base = &*ait;
					break;
				}
			}
			*/

			if (partner_base == 0) 
			{
				continue;
			}
		}

		Vector3 v = base_position - partner_base->getPosition();
		v /= 2.5;

		Tube* tube = new Tube;
		tube->setVertex1(base_position);
		tube->setVertex2(base_position - v);
		tube->setComposite(res);
		tube->setRadius(DNA_ladder_radius_);
		geometric_objects_.push_back(tube);

		Sphere* sphere1 = new Sphere;
		sphere1->setPosition(base_position -v);
		sphere1->setRadius(DNA_ladder_radius_);
		sphere1->setComposite(res);
		geometric_objects_.push_back(sphere1);

		Tube* tube2 = new Tube;
		tube2->setVertex1(partner_base->getPosition());
		tube2->setVertex2(partner_base->getPosition() + v);
		tube2->setComposite(partner);
		tube2->setRadius(DNA_ladder_radius_);
		geometric_objects_.push_back(tube2);

		Sphere* sphere2 = new Sphere;
		sphere2->setPosition(partner_base->getPosition() + v);
		sphere2->setRadius(DNA_ladder_radius_);
		sphere2->setComposite(partner);
		geometric_objects_.push_back(sphere2);


		Circle3 c(base_position, v, DNA_ladder_radius_);
		Disc* disc = new Disc(c);
		disc->setComposite(res);
		geometric_objects_.push_back(disc);

		Circle3 c2(base_position, -v, DNA_ladder_radius_);
		disc = new Disc(c2);
		disc->setComposite(partner);
		geometric_objects_.push_back(disc);
	}
}

// connectivity in a 6 ring:
Position rs6[] =  
{
	0, 4, 5,
	0, 3, 4,
	0, 1, 3,
	1, 2, 3
};

Mesh* AddCartoonModel::create6Ring_(vector<Vector3> positions)
{
	Mesh* mesh = new Mesh();
	mesh->normal.reserve(mesh->normal.size() + 12);
	Vector3 normal = (positions[1] - positions[0]) % (positions[1] - positions[2]);
	normal.normalize();
	Vector3 up = normal * DNA_base_radius_;

	/////////////////////////////////////////////////////
	// lower side
	for (Position p = 0; p < positions.size(); p++)
	{
		mesh->vertex.push_back(positions[p] - up);
		mesh->normal.push_back(-normal);
	}

	Mesh::Triangle tri;
	for (Position vpos = 0; vpos <= 9; vpos += 3)
	{
		tri.v1 = rs6[vpos];
		tri.v2 = rs6[vpos + 1];
		tri.v3 = rs6[vpos + 2];
		mesh->triangle.push_back(tri);
	}

	/////////////////////////////////////////////////////
	// upper side
	for (Position p = 0; p < positions.size(); p++)
	{
		mesh->vertex.push_back(positions[p] + up);
		mesh->normal.push_back(normal);
	}

	for (Index vpos = 11; vpos >= 2; vpos -= 3)
	{
		tri.v1 = 6 + rs6[vpos];
		tri.v2 = 6 + rs6[vpos - 1];
		tri.v3 = 6 + rs6[vpos - 2];
 		mesh->triangle.push_back(tri);
	}

 	renderNucleotideOutline_(positions, -normal, *mesh);
	geometric_objects_.push_back(mesh);
	return mesh;
}


// connectivity in a double ring:
Position dps[] =  
{
	0, 6, 1,
	1, 6, 5,
	1, 5, 2,
	2, 5, 4,
	2, 4, 3,
	0, 7, 6,
	0, 8, 7
};

Mesh* AddCartoonModel::createDoubleRing_(const vector<Vector3>& positions)
{
	Mesh* mesh = new Mesh();
	mesh->normal.reserve(mesh->normal.size() + 18);
	Vector3 normal = (positions[1] - positions[0]) % (positions[1] - positions[2]);
	normal.normalize();
	Vector3 up = normal * DNA_base_radius_;

	/////////////////////////////////////////////////////
	// lower side
	for (Position p = 0; p < positions.size(); p++)
	{
		mesh->vertex.push_back(positions[p] - up);
		mesh->normal.push_back(-normal);
	}

	Mesh::Triangle tri;
	for (Position vpos = 0; vpos <= 18; vpos += 3)
	{
		tri.v1 = dps[vpos];
		tri.v2 = dps[vpos + 1];
		tri.v3 = dps[vpos + 2];
		mesh->triangle.push_back(tri);
	}

	/////////////////////////////////////////////////////
	// upper side
	for (Position p = 0; p < positions.size(); p++)
	{
		mesh->vertex.push_back(positions[p] + up);
		mesh->normal.push_back(normal);
	}

	for (Index vpos = 20; vpos >= 2; vpos -= 3)
	{
		tri.v1 = 9 + dps[vpos];
		tri.v2 = 9 + dps[vpos - 1];
		tri.v3 = 9 + dps[vpos - 2];
 		mesh->triangle.push_back(tri);
	}

 	renderNucleotideOutline_(positions, normal, *mesh);
	geometric_objects_.push_back(mesh);
	return mesh;
}

void AddCartoonModel::refineGuidePoints_()
{
	return;
	for (Position set_pos = 0; set_pos < model_parts_.size(); set_pos++)
	{
		for (Position ribbon = 0; ribbon < number_of_ribbons_; ribbon++)
		{
			vector<Vector3>& guide_points = guide_points_[set_pos][ribbon];
			for (Position part_pos = 0; part_pos < model_parts_[set_pos].size(); part_pos++)
			{
				ModelPart& part = model_parts_[set_pos][part_pos];
				if (part.type != STRAND) continue;

				Position guide_start = part.first_guide_point;
				Position guide_end =  part.last_guide_point;

				Vector3 strand_dir = guide_points[guide_end] - guide_points[guide_start];
				Vector3 nstrand = strand_dir / (guide_end - guide_start);

				Vector3 start = guide_points[guide_start];
				for (Position g = guide_start + 1; g < guide_end; g++)
				{
					Vector3 new_pos = start + nstrand* (float)(g - guide_start) + guide_points[g];
					new_pos /= 2.;
					guide_points[g] = new_pos;
				}
			}
		} // all ribbons
	} // all strings of models
}

	} // namespace VIEW
} // namespace BALL
