// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: cartoonModel.C,v 1.57.4.11 2005/07/14 10:36:16 amoll Exp $
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
#include <BALL/KERNEL/chain.h>
#include <BALL/KERNEL/residue.h>
#include <BALL/KERNEL/secondaryStructure.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/protein.h>

#include <BALL/MATHS/matrix44.h>
#include <BALL/STRUCTURE/geometricProperties.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

AddCartoonModel::AddCartoonModel()
	throw()
	: AddBackboneModel(),
		last_chain_(0),
		helix_radius_(2.4),
		arrow_width_(2.2),
		arrow_height_(0.4),
		DNA_helix_radius_(0.5),
		DNA_ladder_radius_(0.8),
		DNA_base_radius_(0.2),
		ribbon_width_(1.8),
		ribbon_radius_(0.1),
		draw_DNA_as_ladder_(false),
		draw_ribbon_(true),
		use_two_colors_(true),
		no_ss_(false)
{
}

AddCartoonModel::AddCartoonModel(const AddCartoonModel& cartoon)
	throw()
	:	AddBackboneModel(cartoon),
		last_chain_(0),
		helix_radius_(cartoon.helix_radius_),
		arrow_width_(cartoon.arrow_width_),
		arrow_height_(cartoon.arrow_height_),
		DNA_helix_radius_(cartoon.DNA_helix_radius_),
		DNA_ladder_radius_(cartoon.DNA_ladder_radius_),
		DNA_base_radius_(cartoon.DNA_base_radius_),
		ribbon_width_(cartoon.ribbon_width_),
		ribbon_radius_(cartoon.ribbon_radius_),
		draw_DNA_as_ladder_(cartoon.draw_DNA_as_ladder_),
		draw_ribbon_(cartoon.draw_ribbon_),
		use_two_colors_(true),
		no_ss_(false)
{
}

AddCartoonModel::~AddCartoonModel()
	throw()
{
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "Destructing object of class AddCartoonModel" << endl;
	#endif 
}

void AddCartoonModel::clear_()
	throw()
{
	AddBackboneModel::clear_();
	last_chain_ = 0;
	ss_to_spline_start_.clear();
	ss_nr_splines_.clear();
}


Processor::Result AddCartoonModel::operator() (Composite& composite)
{
	// speedup, save some RTTI calls
	if (RTTI::isKindOf<Atom>(composite)) return Processor::CONTINUE;

	if (RTTI::isKindOf<Protein>(composite))
	{
		calculateComplementaryBases_(composite);
		return Processor::CONTINUE;
	}

	if (!RTTI::isKindOf<SecondaryStructure>(composite))  return Processor::CONTINUE;
	SecondaryStructure& ss = *RTTI::castTo<SecondaryStructure>(composite);

	if (ss.countResidues() == 0) return Processor::CONTINUE;

	collectAtoms_(ss);

	return Processor::CONTINUE;
}


void AddCartoonModel::drawPart_(Position last)
{
	last_build_ = 0;

	if (last == 0) return;
	
	// lets see, if we have a valid spline
	if (points_.size() != (splines_.size() - 1) * interpolation_steps_ ||
			points_.size() != atoms_of_points_.size())
	{
   #ifdef BALL_VIEW_DEBUG
		logString(String("Invalid spline: ") +
			          " nr of splines for SS; "  + String(splines_.size()) +
								" spline points and " + String(points_.size()) +
								" interpolated points in " +
								 String(__FILE__) + " " + String(__LINE__) + "\n");
   #endif
	}

	const Residue* residue = dynamic_cast<const Residue*>(splines_[0].atom->getParent());
	if (residue == 0) return;

	Position last_type = getType_(*residue);
	for (Position pos = 1; pos < last; pos++)
	{
		residue = dynamic_cast<const Residue*>(splines_[pos].atom->getParent());
		if (residue == 0) continue;

		Position type = getType_(*residue);

		if (last_type != type)
		{
			if (last_build_ != 0)
			{
				// draw connection
				Position pos1 = (pos - 1) * interpolation_steps_;
				Position pos2 = (pos - 1) * interpolation_steps_ - 1;

				Position apos1 = pos1;
				Position apos2 = pos2;

				if (atoms_of_points_.size() > apos1 + interpolation_steps_)
				{
					apos1 += interpolation_steps_;
				}

				if (atoms_of_points_.size() > apos2 + interpolation_steps_)
				{
					apos2 += interpolation_steps_;
				}

				Sphere* sphere = new Sphere;
				sphere->setRadius(tube_radius_);
				sphere->setPosition(points_[pos1]);
				sphere->setComposite(atoms_of_points_[apos1]);
				geometric_objects_.push_back(sphere);
 
				Tube* tube = new Tube();
				tube->setVertex1(points_[pos1]);
				tube->setVertex2(points_[pos2]);
				tube->setComposite(atoms_of_points_[apos1]);
				tube->setRadius(tube_radius_);
				geometric_objects_.push_back(tube);
				
				sphere = new Sphere;
				sphere->setRadius(tube_radius_);
				sphere->setPosition(points_[pos2]);
				sphere->setComposite(atoms_of_points_[apos2]);
				geometric_objects_.push_back(sphere);
				
			}

		 	buildGraphicalRepresentation_(last_build_, pos - 1, last_type);
			last_build_ = pos - 1;
		}

		last_type = type;
	}

	buildGraphicalRepresentation_(last_build_, last, last_type);
}
	

Position AddCartoonModel::getType_(const Residue& residue)
{
	const Composite* parent = residue.getParent();
	if (parent == 0) return SecondaryStructure::UNKNOWN;

	const SecondaryStructure* ss = dynamic_cast<const SecondaryStructure*>(parent);
	if (ss == 0) return SecondaryStructure::UNKNOWN;
	
	Position type = (Position) ss->getType();

	if (type == SecondaryStructure::STRAND && ss->countResidues() > 3)
	{
		return SecondaryStructure::STRAND;
	}

	else if (ss->getType() == SecondaryStructure::HELIX)
	{
		return SecondaryStructure::HELIX;
	}
	else
	{
		// is this a chain with nucleic acids?
		const String name = residue.getName();
		if (name.size() == 1 &&
				(name == "A" || name == "C" || name == "G" || name == "T" || name == "U"))
		{
			return SecondaryStructure::UNKNOWN + 1;
		}

		return SecondaryStructure::UNKNOWN;
	}
}


void AddCartoonModel::buildGraphicalRepresentation_(Position start, Position end, Position type)
{
	if 			(type == (Position) SecondaryStructure::UNKNOWN + 1)
	{
		buildTube_(start, end);

		if (draw_DNA_as_ladder_)
		{
			buildDNA_(start, end);
		}
		else
		{
			buildWatsonCrickModel_(start, end);
		}
	}
	else if (type == (Position) SecondaryStructure::STRAND)
	{
		buildStrand_(start, end);
	}
	else if (type == (Position) SecondaryStructure::HELIX)
	{
		if (draw_ribbon_)
		{
			buildRibbon_(start, end);
		}
		else
		{
			buildHelix_(start, end);
		}
	}
	else
	{
		List<GeometricObject*>::Iterator lit = geometric_objects_.end();
		lit--;

		buildTube_(start, end);

		if (atoms_of_points_.size() < end + 1) return;
	
		// prevent problems for SS coloring
		lit++;

		for (Position p = 0; p < 2; p++)
		{
			if (lit == geometric_objects_.end()) return;
			
		 	(*lit)->setComposite(splines_[start +1].atom->getParent());
			lit++;
		}
	}
}


// ===========================================================
void AddCartoonModel::collectAtoms_(SecondaryStructure& ss)
{
	Size old_nr_splines = splines_.size();
	ResidueIterator rit = ss.beginResidue();
	AtomIterator ait;

	/////////////////////////////////////
	// is this an nucleic acid?
	Residue* r = ss.getResidue(0);
	if ((r->getName().size() == 1) &&
			(r->getName() == "A" ||
			 r->getName() == "C" ||
			 r->getName() == "G" ||
			 r->getName() == "T" ||
			 r->getName() == "U"))
	{
		// add O5* atom for 1. Residue
		BALL_FOREACH_ATOM(*rit, ait)
		{
			if (ait->getName() == "O5*")
			{
 				splines_.push_back(SplinePoint((*ait).getPosition(), &*ait));
				break;
			}
		}

		// collect the P atoms for the backbone spline
		for(; +rit; ++rit)
		{
			BALL_FOREACH_ATOM(*rit, ait)
			{
				if (ait->getName() == "P")
				{
					splines_.push_back(SplinePoint((*ait).getPosition(), &*ait));
					break;
				}
			}
		}
		
		// add O3* atom for last Residue
		BALL_FOREACH_ATOM(*ss.getCTerminal(), ait)
		{
			if (ait->getName() == "O3*")
			{
				splines_.push_back(SplinePoint((*ait).getPosition(), &*ait));
				break;
			}
		}
	}
	
	/////////////////////////////////////
	// no, this is a protein!
	/////////////////////////////////////
	else if (ss.getType() == SecondaryStructure::STRAND)
	{
		for (; +rit; ++rit)
		{
			Atom* C = 0;
			Atom* nextN = 0;		

			BALL_FOREACH_ATOM(*rit, ait)
			{
				if ((*ait).getName() == "C")
				{
					C = &*ait;
					AtomBondIterator bi;
					BALL_FOREACH_ATOM_BOND(*ait, bi)
					{
						Atom* partner = bi->getPartner(*ait);
						if (partner->getName() == "N") 
						{
							nextN = partner;
							break;
						}
					}

					// now compute the spline point for this residue
					Vector3 sv;
					if (!nextN) 
					{
						sv = C->getPosition();
					}
					else
					{
						// we take the point between the current C (C) and the next N (nextN)
						sv = C->getPosition() + (nextN->getPosition() - C->getPosition()) * 0.5;
					}

					splines_.push_back(SplinePoint(sv, C));	
					break;
				}
			}
		}
	}

	/////////////////////////////////////
	// ok, draw this as tube!
	else
	{
		ResidueIterator ri;
		BALL_FOREACH_RESIDUE(ss, ri)
		{
			AddBackboneModel::collectAtoms_(*ri);
		}
	}

	if (splines_.size() > old_nr_splines)
	{
		ss_to_spline_start_[&ss] = old_nr_splines;
		ss_nr_splines_[&ss] = splines_.size() - old_nr_splines;
	}
	else
	{ 
		ss_to_spline_start_[&ss] = 0;
		ss_nr_splines_[&ss] = 0;
	}
}


// --------------------- HELIX ------------------------------------------------
void AddCartoonModel::buildHelix_(Position first, Position last)
{
	const Atom* first_atom = splines_[first].atom;
	const Atom*  last_atom = splines_[last].atom;

	Vector3 normal = last_atom->getPosition() - first_atom->getPosition();

	// calcluate slices for the helix cylinder according to the C-atoms
	Vector3 last_pos = first_atom->getPosition();
	Vector3 diff = (normal / (last - first));

	for (Position p = first; p < last; p++)
	{
		Tube* tube = new Tube;
		tube->setRadius(helix_radius_);
		tube->setVertex1(last_pos);
		last_pos += diff;
		tube->setVertex2(last_pos);
		tube->setComposite(splines_[p].atom);
		geometric_objects_.push_back(tube);
	}
		
	// add a disc at the beginning and the end of the cylinder to close it
	Disc* disc = new Disc( Circle3(first_atom->getPosition(), -normal, helix_radius_));
	disc->setComposite(first_atom);
	geometric_objects_.push_back(disc);

	disc = new Disc(Circle3(last_atom->getPosition(), normal, helix_radius_));
	disc->setComposite(last_atom);
	geometric_objects_.push_back(disc);
}


// --------------------- STRAND --------------------------------------------
void AddCartoonModel::buildStrand_(Position first, Position last)
{
	// calculate the normals of the peptide bonds for all residues of the SecondaryStructure
	vector<Vector3> 			peptide_normals;
	for (Position spline_pos = first; spline_pos < last; spline_pos++)
	{
		const Residue* residue = (const Residue*) splines_[spline_pos].atom->getParent();
		// we have to find the following atoms:
		const Atom* 	 CA = 0;
		const Atom* 	  N = 0;
		const Atom*		  C = 0;
		const Atom* nextN = 0;
		const Atom*  	  O = 0;

		// find the CA of this residue
		AtomConstIterator ai;
		BALL_FOREACH_ATOM(*residue, ai)
		{
			if (ai->getName() == "CA")
			{
				CA = &(*ai);
				break;
			}
		}

		if (!CA)
		{
			Log.error() << "Could not draw strand: residue contains no C_alpha!" << std::endl;
			return;
		}

		// now find the neighbouring atoms of the CA's: N + C
		AtomBondConstIterator bi;
		BALL_FOREACH_ATOM_BOND(*CA, bi)
		{
			Atom* partner = bi->getPartner(*CA);
			if (partner->getName() == "N") 
			{
				N = partner;
				continue;
			}

			if ((partner->getName() == "C"))
			{
				C = partner;
				continue;
			}
		}

		if (!C || !N)
		{
			Log.error() << "Could not draw strand: residue contains no C or no N!" << std::endl;
			return;	
		}

		// finally, we need the position of the oxygen and of the nitrogen of the next residue 
		BALL_FOREACH_ATOM_BOND(*C, bi)
		{
			Atom* partner = bi->getPartner(*C);
			if (partner->getName() == "N") 
			{
				nextN = partner;
				continue;
			}
			if (partner->getName() == "O")
			{
				O = partner;
				continue;
			}
		}

		if (!O) 
		{
			Log.error() << "Could not draw cartoon style: no oxygen in peptide bond!" << std::endl;
			return;
		}	

		// if we are at the last residue break
		if (!nextN) 
		{
			if (!residue->isTerminal())
			{
				Log.error() << "Could not find next N atom, aborting..." << std::endl;
				return;
			}

			break;
		}

		// calculate the normal of this peptide bond
		Vector3 normal =   (O->getPosition() 			- C->getPosition()) 
										 % (nextN->getPosition()  - C->getPosition());

		if (Maths::isZero(normal.getSquareLength())) 
		{
			Log.error() << "Could not draw cartoon style: degenerated peptide bond!" << std::endl;
			return;
		}

		if (!Maths::isZero(normal.getSquareLength())) normal.normalize();

		peptide_normals.push_back(normal);

	} // iteration over all residues of secondary structure


	// abort if we have no normals
	if (peptide_normals.size() == 0) return;


	Matrix4x4 rotmat;
	Angle angle_pi(M_PI);

	// if two adjacent normals differ by more than 90 degrees, we
	// flip them to ensure a smooth strand representation
	for (Position i = 0; i < peptide_normals.size() - 1; i++)
	{
		const Angle current(fabs(acos(peptide_normals[i]*peptide_normals[i+1])));
		if ((current <= (float)Constants::PI * 1.5) && (current >= (float)Constants::PI / 2.0))
		{
			rotmat.rotate(angle_pi, (peptide_normals[i] % peptide_normals[i + 1]));
			peptide_normals[i + 1] = rotmat * peptide_normals[i + 1];
		}
	}
	
	// an additional smoothing run...
	for (Position i = 0; i < peptide_normals.size() - 1; i++)
	{
		// To smooth the strand representation a bit, we iterate over all normal
		// vectors and compute the angle in between them.
		// Then we reduce the angle by an appropriate rotation to a third of the
		// original angle.
		const Vector3 rotaxis = (peptide_normals[i] % peptide_normals[i+1]);

		if (rotaxis.getSquareLength() > 1e-3)
		{
			const Angle current(fabs(acos(peptide_normals[i]*peptide_normals[i+1])));
			rotmat.rotate(Angle(1.0 / 3.0 * current) - current, rotaxis);
			peptide_normals[i + 1] = rotmat * peptide_normals[i + 1];
		}
	}

	// start of points_ of this SS
	const Position start = last_build_ * interpolation_steps_;

	// put first four points into the mesh (and first two triangles)
	Vector3 right = points_[start + 1] - points_[start];
	Vector3 normal = peptide_normals[0];

	// maybe these cases should not happen, but they do...
	if (!Maths::isZero(normal.getSquareLength())) normal.normalize();
	if (!Maths::isZero( right.getSquareLength()))  right.normalize();

	Vector3 perpendic = normal % right; // perpendicular to spline
	Vector3 last_points[4];

	last_points[0] = points_[start] - (perpendic * arrow_width_/2.) - normal * arrow_height_/2.;
	last_points[1] = last_points[0] + normal * arrow_height_;
	last_points[2] = last_points[1] + perpendic * arrow_width_;
	last_points[3] = last_points[2] - normal * arrow_height_;

	Mesh* mesh = new Mesh;
	mesh->setComposite(splines_[first].atom->getParent());

	// prevent coloring problems, when coloring by SS
	if (splines_.size() > first + 1)
	{
		mesh->setComposite(splines_[first + 1].atom->getParent());
	}

	vector<Vector3>& vertices = mesh->vertex;
	vector<Vector3>& normals  = mesh->normal;

	for (Position i = 0; i < 4; i++)
	{
		vertices.push_back(last_points[i]);
		normals.push_back(right * -1);
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

	normals.push_back(-perpendic);
	normals.push_back(-normal);

	normals.push_back(-perpendic);
	normals.push_back(normal);

	normals.push_back(perpendic);
	normals.push_back(normal);

	normals.push_back(perpendic);
	normals.push_back(-normal);

	Mesh* last_mesh = 0;

	// iterate over all but the last amino acid (last amino acid becomes the arrow)
	Position spline_point_nr = start;
	Position res = 0;
	const Position nr_res = last - first - 1;
	for (; res < nr_res; res++)
	{
		if (res != 0) 
		{
			mesh = new Mesh();

			if (spline_point_nr > atoms_of_points_.size() - 1) 
			{
				Log.error() << "Error in " << __FILE__ << " " << __LINE__ << std::endl; 
				break;
			}

			mesh->setComposite(atoms_of_points_[spline_point_nr]->getParent());

			// copy last 8 points from the old mesh
			const Size copy_start = last_mesh->vertex.size() - 8;
			for (Position p = 0; p < 8; p++)
			{
				mesh->vertex.push_back(last_mesh->vertex[copy_start + p]);
				mesh->normal.push_back(last_mesh->normal[copy_start + p]);
			}
			last_vertices = 0;
		}

		// iterate over the spline points between two amino acids
		for (Position j = 0; j < interpolation_steps_; j++)
		{
			right  = points_[spline_point_nr + 1] - points_[spline_point_nr];
			if (right.getSquareLength() == 0)
			{
				Log.error() << "Error in " << __FILE__ << " " << __LINE__ << std::endl; 
				spline_point_nr++;
				continue;
			}

			normal =   peptide_normals[res    ] *(1 - j * 0.9 / 8.0) 
							 + peptide_normals[res + 1] *     j * 0.9 / 8.0;

			drawStrand_(points_[spline_point_nr], normal, right, arrow_width_, last_vertices, *mesh);
			spline_point_nr ++;
		}

		last_mesh = mesh;
		geometric_objects_.push_back(mesh);
	}

	// make sure atoms of arrow head are found for coloring
	mesh->setComposite(atoms_of_points_[spline_point_nr]->getParent()->getParent());

	// finally, we draw the arrow
	for (Index j = -1; j <= 6; j++)
	{
		// interpolate the depth of the box
		const float new_arrow_width = 2 * (1 - j * 0.95 / 6.0) * arrow_width_; 
		
		right = points_[spline_point_nr + 1] - points_[spline_point_nr];

		drawStrand_(points_[spline_point_nr], normal, right, new_arrow_width, last_vertices, *mesh);

		spline_point_nr++;
	}
}


void AddCartoonModel::insertTriangle_(Position v1, Position v2, Position v3, Mesh& mesh)
{
	Surface::Triangle t;
	t.v1 = v1;
	t.v2 = v2;
	t.v3 = v3;
	mesh.triangle.push_back(t);
}


void AddCartoonModel::drawStrand_(const Vector3& start,
																	Vector3& normal,
																	Vector3& right,
																	float arrow_width,
																	Position& last_vertices,
																	Mesh& mesh)
{
	vector<Vector3>* vertices = &mesh.vertex;
	vector<Vector3>* normals  = &mesh.normal;

	// maybe this cases should not happen, but they do...
	if (!Maths::isZero(normal.getSquareLength())) normal.normalize();
	if (!Maths::isZero(right.getSquareLength())) right.normalize();

	Vector3 perpendic = normal % right; // perpendicular to spline
	if (!Maths::isZero(perpendic.getSquareLength())) perpendic.normalize();

	Vector3 current_points[4];
	current_points[0] = start - (perpendic * arrow_width   / 2.0) 
														- (normal    * arrow_height_ / 2.0);
	current_points[1] = current_points[0] + normal * arrow_height_;
	current_points[2] = current_points[1] + perpendic * arrow_width;
	current_points[3] = current_points[2] - normal * arrow_height_;

	// put the next 4 points and 8 triangles into the mesh
	for (Position p = 0; p < 4; p++)
	{
		vertices->push_back(current_points[p]);
		vertices->push_back(current_points[p]);
	}

	normals->push_back(perpendic);
	normals->push_back(-normal);

	normals->push_back(perpendic);
	normals->push_back(-normal);

	normals->push_back(perpendic);
	normals->push_back(-normal);

	normals->push_back(perpendic);
	normals->push_back(-normal);

	// one side band
	insertTriangle_(last_vertices    , last_vertices +  2, last_vertices + 10, mesh);
	insertTriangle_(last_vertices    , last_vertices + 10, last_vertices +  8, mesh);
	
	// other side band
	insertTriangle_(last_vertices + 4, last_vertices + 12, last_vertices + 14, mesh);
	insertTriangle_(last_vertices + 4, last_vertices + 14, last_vertices +  6, mesh);

	// "upper" band
	insertTriangle_(last_vertices + 3, last_vertices +  5, last_vertices + 13, mesh);
	insertTriangle_(last_vertices + 3, last_vertices + 13, last_vertices + 11, mesh);
	
	// "lower" band
	insertTriangle_(last_vertices + 1, last_vertices +  7, last_vertices + 15, mesh);
	insertTriangle_(last_vertices + 1, last_vertices +  15, last_vertices + 9, mesh);

	last_vertices+=8;
}


// draw a helix as ribbon model
void AddCartoonModel::buildRibbon_(Size start, Size end)
{
	start *= interpolation_steps_;
	end   *= interpolation_steps_;

	// overall direction of the helix
	Vector3 helix_dir(points_[end] - points_[start]);
	if (!Maths::isZero(helix_dir.getSquareLength()))
	{ 
		helix_dir.normalize();
	}

	// distance difference vector for growing/shrinking of helix at start/end
	const Vector3 helix_step = helix_dir * ribbon_width_ / 12;

	// distance vector changes dynamicaly later by helix_step
	Vector3 tube_diff;
	
	// calculate the number of slides for the circles and the angle in between them
	Size slides = (Size)(8 + drawing_precision_ * 4);
	Angle slides_angle = Angle(360.0 / slides, false);

	// direction vector of the two current spline points
	Vector3 dir;
	Vector3 last_point(points_[start]);

	// prevent problems if last point is the same as the start point
	while (true)
	{
		dir = points_[start] - last_point;
		if (Maths::isZero(dir.getSquareLength()))
		{
			start++;
		}
		else
		{
			break;
		}
	}

	if (start >= atoms_of_points_.size()) return;
			
	////////////////////////////////////////////////////////////
	// calculate normal vector r to direction vector dir, with length of radius
	////////////////////////////////////////////////////////////
	Vector3 n = Vector3(0,1,0);
	Vector3 r = dir % n;
	if (Maths::isZero(r.getSquareLength())) 
	{ 
		r = dir % n;
		if (Maths::isZero(r.getSquareLength())) 
		{
			n = Vector3(0,0,1);
			r = dir % n;
		}
	}

	if (!Maths::isZero(r.getSquareLength())) r.normalize();

	r *= ribbon_radius_;

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
	m.setRotation(slides_angle, n % r);
	// second half of points can be calculated by negating first half
	for (Position i= 1; i < middle; i++)
	{
		x = m * x;
		new_points[i] = x;
		new_points[i + middle] = -x;
	}

	////////////////////////////////////////////////////////////
	// create two new mesh with the points and triangles
	////////////////////////////////////////////////////////////
	// every residue get its own meshes to enable picking for the tube model
	Mesh* mesh1 = new Mesh(); // the two tubes
	Mesh* mesh2 = new Mesh(); // connection between tubes 1
	Mesh* mesh3 = new Mesh(); // connection between tubes 2

	Position astart = start;
	if (atoms_of_points_.size() > start + 1)
	{
		astart += interpolation_steps_;
	}

	if (atoms_of_points_[start] != 0)
	{
		mesh1->setComposite(atoms_of_points_[start]->getParent());
 		mesh2->setComposite(atoms_of_points_[start]->getParent());

		if (!use_two_colors_)
		{
			mesh3->setComposite(atoms_of_points_[start]->getParent());
		}
		else
		{
			mesh3->setComposite(&composite_to_be_ignored_for_colorprocessors_);
		}
	}
 	geometric_objects_.push_back(mesh1);
 	geometric_objects_.push_back(mesh2);
	geometric_objects_.push_back(mesh3);

	// distance vector between the two connections
	if (!Maths::isZero(r.getSquareLength())) r.normalize();

	Vector3 diff = r * 0.1;
	
	// insert connection between tubes
	mesh2->vertex.push_back(last_point + tube_diff + diff);
	mesh2->vertex.push_back(last_point - tube_diff + diff);
	Vector3 vn(-(dir % helix_dir));
	mesh2->normal.push_back(vn);
	mesh2->normal.push_back(vn);
	
	// insert connection between tubes
	mesh3->vertex.push_back(last_point + tube_diff - diff);
	mesh3->vertex.push_back(last_point - tube_diff - diff);
	mesh3->normal.push_back(-vn);
	mesh3->normal.push_back(-vn);


	for (Position p = 0; p < slides; p++)
	{
		mesh1->vertex.push_back(last_point + tube_diff + new_points[p]);
		mesh1->vertex.push_back(last_point - tube_diff + new_points[p]);
		mesh1->normal.push_back(new_points[p]);
		mesh1->normal.push_back(new_points[p]);
	}
	
	// same data structures for faster access
	Mesh::Triangle t;
	Size s_old = 0;  // start position of the last points of the mesh1 vertices

	//------------------------------------------------------>
	// iterate over all points_
	//------------------------------------------------------>
	for (Position p = start; p < end - 1; p++)
	{
		// faster access to the current spline point
		const Vector3 point = points_[p];
		
		// new direction vector: new point - last point
		const Vector3 dir_new = point - last_point;

		// new normal vector
		Vector3 r_new = r - (
							 (dir_new.x * r.x       + dir_new.y *       r.y + dir_new.z *       r.z)  /
							 (dir_new.x * dir_new.x + dir_new.y * dir_new.y + dir_new.z * dir_new.z) 
							 * dir_new);
		
		if (!Maths::isZero(r_new.getSquareLength())) r_new.normalize();

		r_new *= ribbon_radius_;

		////////////////////////////////////////////////////////////
		// rotate all points of the circle according to new normal
		m.setRotation(slides_angle, dir_new);
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

		// prevent problems with SS coloring
		if (p <= start + interpolation_steps_ / 2)
		{
			const Composite* c = atoms_of_points_[astart]->getParent();
			mesh1->setComposite(c);
			mesh2->setComposite(c);
			if (!use_two_colors_)
			{
				mesh3->setComposite(c);
			}
		}

		////////////////////////////////////////////////////////////
		// create two new meshes if we have a different residue
		////////////////////////////////////////////////////////////
		if (atoms_of_points_[p] != 0 &&
				mesh1->getComposite() != atoms_of_points_[p]->getParent())
		{
			const Mesh* old_mesh1 = mesh1;
			mesh1 = new Mesh();
			// insert the vertices and normals of the last points again into the new mesh
			for (Position point_pos = old_mesh1->vertex.size() - (slides * 2);
										point_pos < old_mesh1->vertex.size(); point_pos++)
			{
				mesh1->vertex.push_back(old_mesh1->vertex[point_pos]);
				mesh1->normal.push_back(old_mesh1->normal[point_pos]);
			}

			// first band
			const Mesh* old_mesh2 = mesh2;
			mesh2 = new Mesh();
			const Size vs = old_mesh2->vertex.size() - 2;
			mesh2->vertex.push_back(old_mesh2->vertex[vs]);
			mesh2->vertex.push_back(old_mesh2->vertex[vs + 1]);
			mesh2->normal.push_back(old_mesh2->normal[vs]);
			mesh2->normal.push_back(old_mesh2->normal[vs + 1]);

			// second band
			const Mesh* old_mesh3 = mesh3;
			mesh3 = new Mesh();
			mesh3->vertex.push_back(old_mesh3->vertex[vs]);
			mesh3->vertex.push_back(old_mesh3->vertex[vs + 1]);
			mesh3->normal.push_back(old_mesh3->normal[vs]);
			mesh3->normal.push_back(old_mesh3->normal[vs + 1]);

			geometric_objects_.push_back(mesh1);
			geometric_objects_.push_back(mesh2);
			geometric_objects_.push_back(mesh3);

			if (atoms_of_points_[p] != 0)
			{
				mesh1->setComposite(atoms_of_points_[p]->getParent());
				mesh2->setComposite(atoms_of_points_[p]->getParent());
				if (!use_two_colors_)
				{
					mesh3->setComposite(atoms_of_points_[p]->getParent());
				}
				else
				{
					mesh3->setComposite(&composite_to_be_ignored_for_colorprocessors_);
				}
			}

			s_old = 0;
		}
		
		////////////////////////////////////////////////////////////
		// insert connection between tubes
		////////////////////////////////////////////////////////////
		if (p < start + 10)
		{
			tube_diff += helix_step;
		}
		else if (p > end - 12)
		{
			tube_diff -= helix_step;
		}

		// distance between the two bands

		// first band
		Vector3 vn(-(dir_new % helix_dir));
		if (!Maths::isZero(vn.getSquareLength())) vn.normalize();

		Vector3 diff = vn * -0.1;

		mesh2->vertex.push_back(point + tube_diff + diff);
		mesh2->normal.push_back(vn);
		mesh2->vertex.push_back(point - tube_diff + diff);
		mesh2->normal.push_back(vn);

		const Size sn = mesh2->vertex.size() - 1;
		t.v1 = sn - 1;
		t.v2 = sn;
		t.v3 = sn - 2;
 		mesh2->triangle.push_back(t);

		t.v1 = sn - 1;
		t.v2 = sn - 2;
		t.v3 = sn - 3;
		mesh2->triangle.push_back(t);
		
		// second band
		mesh3->vertex.push_back(point + tube_diff - diff);
		mesh3->normal.push_back(vn);
		mesh3->vertex.push_back(point - tube_diff - diff);
		mesh3->normal.push_back(vn);

		t.v1 = sn - 1;
		t.v2 = sn;
		t.v3 = sn - 2;
 		mesh3->triangle.push_back(t);

		t.v1 = sn - 1;
		t.v2 = sn - 2;
		t.v3 = sn - 3;
		mesh3->triangle.push_back(t);


		////////////////////////////////////////////////////////////
		// insert the points of the two new circles
		////////////////////////////////////////////////////////////
		// we will add an other point next, so here we do an off by one :)
		Size s_new = mesh1->vertex.size();

		//------------------------------------------------------>
		// iterate over all points of the circle
		//------------------------------------------------------>
		for (Position point_pos = 0; point_pos < slides ; point_pos++)
		{
			mesh1->vertex.push_back(point + tube_diff + new_points[point_pos]);
			mesh1->normal.push_back(new_points[point_pos]);

			t.v1 = s_old;			// last lower
			t.v2 = s_old + 2;	// last upper
			t.v3 = s_new;			// new upper
 			mesh1->triangle.push_back(t);

			t.v1 = s_new;			// new upper
			t.v2 = s_new - 2;	// new lower
			t.v3 = s_old; 		// last lower
 			mesh1->triangle.push_back(t);

			s_old++;
			s_new++;

			mesh1->vertex.push_back(point - tube_diff + new_points[point_pos]);
			mesh1->normal.push_back(new_points[point_pos]);

			t.v1 = s_old;			// last lower
			t.v2 = s_old + 2;	// last upper
			t.v3 = s_new;			// new upper
 			mesh1->triangle.push_back(t);

			t.v1 = s_new;			// new upper
			t.v2 = s_new - 2;	// new lower
			t.v3 = s_old; 		// last lower
 			mesh1->triangle.push_back(t);

			s_old++;
			s_new++;
		}

		r = r_new;
		last_point = point;
	}
}

// -----------------------------------------------------------------------
// --------------------- DNA -------------------------------------------->
void AddCartoonModel::calculateComplementaryBases_(const Composite& composite)
	throw()
{
	complementary_bases_.clear();
	Protein& protein = *(Protein*)&composite;
	if (protein.countChains() < 2) return;

	HashMap<Residue*, Vector3> positions;

	ResidueIterator rit = protein.beginResidue();
	for (; +rit; rit++)
	{
		if ((*rit).getName().size() > 1 ||
				((*rit).getName() != "A" &&
				 (*rit).getName() != "C" &&
				 (*rit).getName() != "G" &&
				 (*rit).getName() != "T"))
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
		Residue* found_partner = 0;
		HashSet<Residue*>::Iterator rit2 = chain2_residues.begin();
		for (; rit2 != chain2_residues.end(); rit2++)
		{
			if (((*rit1).getName() == "A" && 
				 ((**rit2).getName() != "T" && (**rit2).getName() != "U")) ||
					((*rit1).getName() == "C" && (**rit2).getName() != "G") ||
					((*rit1).getName() == "G" && (**rit2).getName() != "C") ||
					((*rit1).getName() == "T" && (**rit2).getName() != "A") ||
					((*rit1).getName() == "U" && (**rit2).getName() != "A"))
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


void AddCartoonModel::createTriangle_(Mesh& mesh, const Atom& a1, const Atom& a2, const Atom& a3,
																									const Atom* sa1, const Atom* sa2, const Atom* sa3)
	throw()
{
	vector<Vector3>& vertices = mesh.vertex;
	vector<Vector3>& normals  = mesh.normal;
	vector<Surface::Triangle>& triangles = mesh.triangle;

	Vector3 p1 = a1.getPosition();
	Vector3 p2 = a2.getPosition();
	Vector3 p3 = a3.getPosition();

	Vector3 v1 = p1 - p2;
	Vector3 v2 = p3 - p2;
	Vector3 normal = v1 % v2;

	if (!Maths::isZero(normal.getSquareLength())) normal.normalize();

	normal *= -DNA_base_radius_;

	vertices.push_back(p1 + normal);
	vertices.push_back(p2 + normal);
	vertices.push_back(p3 + normal);
	normals.push_back(normal);
	normals.push_back(normal);
	normals.push_back(normal);

	Surface::Triangle t;
	t.v1 = vertices.size() - 3;
	t.v2 = vertices.size() - 2;
	t.v3 = vertices.size() - 1;
	triangles.push_back(t);

	// lower side
	vertices.push_back(p1 - normal);
	vertices.push_back(p2 - normal);
	vertices.push_back(p3 - normal);
	normals.push_back(normal);
	normals.push_back(normal);
	normals.push_back(normal);

	t.v1 = vertices.size() - 3;
	t.v2 = vertices.size() - 2;
	t.v3 = vertices.size() - 1;
	triangles.push_back(t);

	if (sa1 == 0) return;

	TwoColoredTube* tube = new TwoColoredTube;
	if (sa1->getBond(*sa2)->getFirstAtom() == sa1)
	{
		tube->setVertex1(sa1->getPosition());
		tube->setVertex2(sa2->getPosition());
	}
	else
	{
		tube->setVertex1(sa2->getPosition());
		tube->setVertex2(sa1->getPosition());
	}
	tube->setRadius(DNA_base_radius_);
	tube->setComposite(sa1->getBond(*sa2));
	geometric_objects_.push_back(tube);

	Sphere* s1 = new Sphere;
	s1->setPosition(sa1->getPosition());
	s1->setRadius(DNA_base_radius_);
	s1->setComposite(sa1);
	geometric_objects_.push_back(s1);

	s1 = new Sphere;
	s1->setPosition(sa2->getPosition());
	s1->setRadius(DNA_base_radius_);
	s1->setComposite(sa2);
	geometric_objects_.push_back(s1);

	if (sa3 == 0) return;

	tube = new TwoColoredTube;
	if (sa2->getBond(*sa3)->getFirstAtom() == sa2)
	{
		tube->setVertex1(sa2->getPosition());
		tube->setVertex2(sa3->getPosition());
	}
	else
	{
		tube->setVertex1(sa3->getPosition());
		tube->setVertex2(sa2->getPosition());
	}
	tube->setRadius(DNA_base_radius_);
	tube->setComposite(sa2->getBond(*sa3));
	geometric_objects_.push_back(tube);
}


void AddCartoonModel::buildWatsonCrickModel_(Position first, Position)
{
	SecondaryStructure dummy_ss;
	const SecondaryStructure& ss = *splines_[first].atom->getAncestor(dummy_ss);

	vector<Vector3>::iterator old_spline_point = points_.begin();
	Position nr_of_residues = ss.countResidues();
	for (Position pos = 0; pos < nr_of_residues; pos++)
	{
		Residue* r = (Residue*) ss.getResidue(pos);
		Mesh* mesh = new Mesh;
		mesh->setComposite(r);
		geometric_objects_.push_back(mesh);

		Vector3 connection_point;
		Atom* atoms[9];
		for (Position p = 0; p < 9; p++) atoms[p] = 0;

		vector<Atom*> hbond_atoms;

		if (r->getName() == "A" ||
				r->getName() == "G")
		{
			if (r->getName() == "A")
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

			connection_point = atoms[0]->getPosition();
			createTriangle_(*mesh, *atoms[1], *atoms[0], *atoms[8], atoms[1], atoms[0], atoms[8]); 	// C4,N9,C8
			createTriangle_(*mesh, *atoms[6], *atoms[1], *atoms[8], 0, 0, 0); 										 	// C5,C4,C8
			createTriangle_(*mesh, *atoms[6], *atoms[7], *atoms[8], atoms[6], atoms[7], atoms[8]); 	// C5,N7,C8
			createTriangle_(*mesh, *atoms[2], *atoms[3], *atoms[4], atoms[2], atoms[3], atoms[4]); 	// N3,C2,N1
			createTriangle_(*mesh, *atoms[1], *atoms[2], *atoms[4], atoms[1], atoms[2], 0); 				// C4,N3,N1
			createTriangle_(*mesh, *atoms[1], *atoms[4], *atoms[5], atoms[4], atoms[5], 0); 				// C4,N1,C6
			createTriangle_(*mesh, *atoms[1], *atoms[5], *atoms[6], atoms[5], atoms[6], 0); 				// C4,C6,C5

			Sphere* s = new Sphere;
			s->setComposite(atoms[8]);
			s->setRadius(DNA_base_radius_);
			s->setPosition(atoms[8]->getPosition());
			geometric_objects_.push_back(s);
		}
		// -------------------------------------------------
		else if (r->getName() == "C")
		{
			String atom_names[9] = {"N1", "C2", "N3", "C4", "C5", "C6", "N4", "O2", ""};
			if (!assignNucleotideAtoms_(*r, 8, atom_names, atoms)) continue;

			connection_point = atoms[0]->getPosition();
			createTriangle_(*mesh, *atoms[1], *atoms[2], *atoms[3], atoms[1], atoms[2], atoms[3]); 	// C2,N3,C4
			createTriangle_(*mesh, *atoms[0], *atoms[1], *atoms[3], atoms[0], atoms[1], 0); 			  // N1,C2,C4
			createTriangle_(*mesh, *atoms[0], *atoms[3], *atoms[4], atoms[3], atoms[4], 0); 				// N1,C4,C5
			createTriangle_(*mesh, *atoms[0], *atoms[5], *atoms[4], atoms[0], atoms[5], atoms[4]); 	// N1,C6,C5
			hbond_atoms.push_back(atoms[6]);
			hbond_atoms.push_back(atoms[2]);
			hbond_atoms.push_back(atoms[7]);
		}
		// -------------------------------------------------
		else if (r->getName() == "T")
		{
			String atom_names[9] = {"C2", "N3", "C4", "C5", "C6", "N1", "O4", "", ""};
			if (!assignNucleotideAtoms_(*r, 7, atom_names, atoms)) continue;

			connection_point = atoms[5]->getPosition();
			createTriangle_(*mesh, *atoms[1], *atoms[2], *atoms[3], atoms[1], atoms[2], atoms[3]); 	// N3,C4,C5
			createTriangle_(*mesh, *atoms[0], *atoms[1], *atoms[3], atoms[0], atoms[1], 0); 			  // C2,N3,C5
			createTriangle_(*mesh, *atoms[0], *atoms[3], *atoms[4], atoms[3], atoms[4], 0); 				// C2,C5,C6
			createTriangle_(*mesh, *atoms[0], *atoms[5], *atoms[4], atoms[0], atoms[5], atoms[4]); 	// C2,N1,C6
			hbond_atoms.push_back(atoms[1]);
			hbond_atoms.push_back(atoms[6]);
		}
		// -------------------------------------------------
		else if (r->getName() == "U")
		{
			String atom_names[9] = {"C2", "N3", "C4", "C5", "C6", "N1", "O4", "O2", ""};
			if (!assignNucleotideAtoms_(*r, 7, atom_names, atoms)) continue;

			connection_point = atoms[5]->getPosition();
			createTriangle_(*mesh, *atoms[1], *atoms[2], *atoms[3], atoms[1], atoms[2], atoms[3]); 	// N3,C4,C5
			createTriangle_(*mesh, *atoms[0], *atoms[1], *atoms[3], atoms[0], atoms[1], 0); 			  // C2,N3,C5
			createTriangle_(*mesh, *atoms[0], *atoms[3], *atoms[4], atoms[3], atoms[4], 0); 				// C2,C5,C6
			createTriangle_(*mesh, *atoms[0], *atoms[5], *atoms[4], atoms[0], atoms[5], atoms[4]); 	// C2,N1,C6
			hbond_atoms.push_back(atoms[1]);
			hbond_atoms.push_back(atoms[6]);
		}
		else
		{
			// unknown base, abort for this Residue
			continue;
		}

		// --------------------------------------------
		// draw connection to backbone
		// --------------------------------------------
		float distance = 256;
		vector<Vector3>::iterator sit = old_spline_point;
		for (; sit != points_.end(); sit++)
		{
			float new_distance = ((*sit) - connection_point).getSquareLength();
			if (new_distance < distance)
			{
				distance = new_distance;
				old_spline_point = sit;
			}
		}

		if (distance < 256)
		{
			Tube* tube = new Tube;
			tube->setComposite(r);
			tube->setVertex1(connection_point);
			tube->setVertex2(*old_spline_point);
			tube->setRadius(DNA_base_radius_);
			geometric_objects_.push_back(tube);
		}
		else
		{
			Log.error() << "Could not draw connection to backbone of DNA in " 
									<< __FILE__ << " " << __LINE__ << std::endl;
		}

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
					if (a1->getName() == "N3" ||
							(a1->getName() == "N1" && r->getName() == "G"))
					{
						continue;
					}
					
					Atom* a2 = (*bit).getPartner(*a1);
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
			}
		}

		// done for Residue r
	}
}


bool AddCartoonModel::assignNucleotideAtoms_(Residue& r, Size nr_atoms, 
																						 String atom_names[9], Atom* atoms[9])
	throw()
{
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


void AddCartoonModel::buildDNA_(Position first, Position)
{
	SecondaryStructure dummy_ss;
	const SecondaryStructure& ss = *splines_[first].atom->getAncestor(dummy_ss);

	tube_radius_ = DNA_helix_radius_;

	HashMap<const Residue*, const Atom*> base_atoms;

	Position start = ss_to_spline_start_[&ss];
	for (Position p = start; p < start + ss_nr_splines_[&ss]; p++)
	{
		const Atom* atom = (Atom*) splines_[p].atom;
		base_atoms[(const Residue*)atom->getParent()] = atom;
	}

	ResidueConstIterator rit;
	for (rit = ss.beginResidue(); +rit; ++rit)
	{
		const Residue* r = &*rit;

		// prevent double drawing for complementary bases
		if (complementary_bases_.has(r) &&
				complementary_bases_[r] < r) 
		{
			continue;
		}

		const Atom* base_atom = base_atoms[r];
		if (base_atom == 0) 
		{
			continue;
		}

		AtomConstIterator ait;

		// ================ draw unpaired bases ===============
		if (!complementary_bases_.has(r))
		{
			String atom_name;
			if (r->getName() == "A" ||
					r->getName() == "G")
			{
				atom_name = "C6";
			}
			else if (r->getName() == "C") atom_name = "C4";
			else if (r->getName() == "T") atom_name = "N3";
				
			const Atom* end_atom = 0;
			BALL_FOREACH_ATOM(*r, ait)
			{
				if (ait->getName() == atom_name)
				{
					end_atom = &*ait;
					break;
				}
			}

			if (end_atom == 0) continue;

			Tube* tube = new Tube;
			tube->setVertex1(base_atom->getPosition());
			tube->setVertex2(end_atom->getPosition());
			tube->setComposite(r);
			tube->setRadius(DNA_ladder_radius_);
 			geometric_objects_.push_back(tube);

			Sphere* sphere1 = new Sphere;
			sphere1->setPosition(end_atom->getPosition());
			sphere1->setRadius(DNA_ladder_radius_);
			sphere1->setComposite(r);
			geometric_objects_.push_back(sphere1);

			Circle3 c(base_atom->getPosition(), -(end_atom->getPosition() - base_atom->getPosition()), DNA_ladder_radius_);
			Disc* disc = new Disc(c);
			disc->setComposite(r);
			geometric_objects_.push_back(disc);

			continue;
		}

		// ================ draw paired bases ===============
		const Residue* partner = complementary_bases_[r];
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
			BALL_FOREACH_ATOM(*partner, ait)
			{
				if (ait->getName() == "O5*") 
				{
					partner_base = &*ait;
					break;
				}
			}

			if (partner_base == 0) 
			{
				continue;
			}
		}

		Vector3 v = base_atom->getPosition() - partner_base->getPosition();
		v /= 2.5;

		Tube* tube = new Tube;
		tube->setVertex1(base_atom->getPosition());
		tube->setVertex2(base_atom->getPosition() - v);
		tube->setComposite(r);
		tube->setRadius(DNA_ladder_radius_);
		geometric_objects_.push_back(tube);

		Sphere* sphere1 = new Sphere;
		sphere1->setPosition(base_atom->getPosition() -v);
		sphere1->setRadius(DNA_ladder_radius_);
		sphere1->setComposite(r);
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


		Circle3 c(base_atom->getPosition(), v, DNA_ladder_radius_);
		Disc* disc = new Disc(c);
		disc->setComposite(r);
		geometric_objects_.push_back(disc);

		Circle3 c2(partner_base->getPosition(), -v, DNA_ladder_radius_);
		disc = new Disc(c2);
		disc->setComposite(partner);
		geometric_objects_.push_back(disc);
	}
}


	} // namespace VIEW
} // namespace BALL
