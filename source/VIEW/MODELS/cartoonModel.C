// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: cartoonModel.C,v 1.54.2.14 2005/01/05 15:29:27 amoll Exp $
//

#include <BALL/VIEW/MODELS/cartoonModel.h>

#include <BALL/VIEW/PRIMITIVES/tube.h>
#include <BALL/VIEW/PRIMITIVES/disc.h>
#include <BALL/VIEW/PRIMITIVES/mesh.h>
#include <BALL/VIEW/PRIMITIVES/box.h>
#include <BALL/VIEW/PRIMITIVES/sphere.h>
#include <BALL/VIEW/PRIMITIVES/line.h>
#include <BALL/VIEW/PRIMITIVES/twoColoredTube.h>

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
		spline_vector_position_(-1),
		helix_radius_(2.4),
		arrow_width_(2),
		arrow_height_(0.4),
		DNA_helix_radius_(0.5),
		DNA_ladder_radius_(0.8),
		DNA_base_radius_(0.2),
		ribbon_width_(1.8),
		ribbon_radius_(0.1),
		draw_DNA_as_ladder_(false),
		draw_ribbon_(true)
{
}

AddCartoonModel::AddCartoonModel(const AddCartoonModel& cartoon)
	throw()
	:	AddBackboneModel(cartoon),
		last_chain_(0),
		spline_vector_position_(-1),
		helix_radius_(cartoon.helix_radius_),
		arrow_width_(cartoon.arrow_width_),
		arrow_height_(cartoon.arrow_height_),
		DNA_helix_radius_(cartoon.DNA_helix_radius_),
		DNA_ladder_radius_(cartoon.DNA_ladder_radius_),
		DNA_base_radius_(cartoon.DNA_base_radius_),
		ribbon_width_(cartoon.ribbon_width_),
		ribbon_radius_(cartoon.ribbon_radius_),
		draw_DNA_as_ladder_(cartoon.draw_DNA_as_ladder_),
		draw_ribbon_(cartoon.draw_ribbon_)
{
}

AddCartoonModel::~AddCartoonModel()
	throw()
{
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "Destructing object of class AddCartoonModel" << endl;
	#endif 
}

bool AddCartoonModel::createGeometricObjects()
	throw()
{
	return true;
}


void AddCartoonModel::clear_()
	throw()
{
	AddBackboneModel::clear_();
	last_chain_ = 0;
	spline_vector_position_ = -1;
}


void AddCartoonModel::drawStrand_(SecondaryStructure& ss)
	throw()
{
	// we want to compute the spline ourselves for better smoothing properties
	vector<Vector3> 			spline_backup 				= spline_points_;
	vector<SplinePoint> 	spline_vector_backup 	= spline_vector_;
	vector<const Atom*> 	atoms_backup 					= atoms_of_spline_points_;
	spline_vector_.clear();
	spline_points_.clear();
	atoms_of_spline_points_.clear();

	bool is_terminal_segment = false;
	bool first_residue = true;
	bool last_residue = false;

	vector<Vector3> 			peptide_normals;

	ResidueIterator ri;
	BALL_FOREACH_RESIDUE(ss, ri)
	{
		// we have to find the following atoms:
		Atom* 	 CA = 0;
		Atom* 	  N = 0;
		Atom*		  C = 0;
		Atom* nextN = 0;
		Atom*  	  O = 0;

		// find the CA of this residue
		AtomIterator ai;
		BALL_FOREACH_ATOM(*ri, ai)
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
		AtomBondIterator bi;
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
			last_residue = true;
			if (ri->isTerminal()) is_terminal_segment = true;
			continue;
		}

		// calculate the normal of this peptide bond
		Vector3 normal =   (O->getPosition() - C->getPosition()) 
										 % (nextN->getPosition()  - C->getPosition()) ;

		if (Maths::isZero(normal.getSquareLength())) 
		{
			Log.error() << "Could not draw cartoon style: degenerated peptide bond!" << std::endl;
			return;
		}

		normal.normalize();
		peptide_normals.push_back(normal);

		// we have computed the normal. now compute the two spline points corresponding to this
		// amino acid: we take the point between the current N (N) and the C_alpha (CA) and
		// the point between the C_alpha and C
		
		// if this is the first residue, we also add the position of the N-atom to prevent
		// large "holes" in the representation. we also reuse the same normal as for the
		// first "real" spline point
		if (first_residue)
		{
			spline_vector_.push_back(SplinePoint(CA->getPosition(), CA));
			peptide_normals.push_back(normal);
			first_residue = false;
		}

		Vector3 sv = C->getPosition() + (nextN->getPosition() - C->getPosition()) * 0.5;
		spline_vector_.push_back(SplinePoint(sv, N));	

		if (last_residue)
		{
			if (peptide_normals.size() == 0) 
			{
				Log.error() << "Could not draw cartoon style: no peptide bonds found!" << std::endl;
				return;
			}

			peptide_normals[peptide_normals.size() - 1] = 
			peptide_normals[peptide_normals.size() - 2];
		}
	} // iteration over all residues of secondary structure


	calculateTangentialVectors_();
	createSplinePath_();
	
	// if two adjacent normals differ by more than 90 degrees, we
	// flip them to ensure a smooth strand representation
	for (Position i = 0; i < peptide_normals.size() - 1; i++)
	{
		Angle current(fabs(acos(peptide_normals[i]*peptide_normals[i+1])));
		if ((current <= (float)Constants::PI * 1.5) && (current >= (float)Constants::PI / 2.0))
		{
			Vector3 rotaxis = (peptide_normals[i] % peptide_normals[i + 1]).normalize();
			Matrix4x4 rotmat;
			rotmat.rotate(Angle(M_PI), rotaxis);
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
		Vector3 rotaxis = (peptide_normals[i] % peptide_normals[i+1]);

		if (rotaxis.getSquareLength() > 1e-2)
		{
			Angle current(fabs(acos(peptide_normals[i]*peptide_normals[i+1])));
			Angle new_angle = Angle(2.0 / 3.0 * current);

			Angle diff_angle = new_angle - current;
			Matrix4x4 rotmat;
			rotmat.rotate(diff_angle, rotaxis);

			peptide_normals[i + 1] = rotmat * peptide_normals[i + 1];
		}
	}
	
	// put first four points into the mesh (and first two triangles)
	Vector3 right = spline_points_[1] - spline_points_[0];
	Vector3 normal = peptide_normals[0];

	// maybe these cases should not happen, but they do...
	if (!Maths::isZero(normal.getSquareLength())) normal.normalize();
	if (!Maths::isZero( right.getSquareLength()))  right.normalize();

	Vector3 perpendic = normal % right; // perpendicular to spline
	Vector3 last_points[4];

	last_points[0] = spline_points_[0] - (perpendic * arrow_width_/2.) - normal * arrow_height_/2.;
	last_points[1] = last_points[0] + normal * arrow_height_;
	last_points[2] = last_points[1] + perpendic * arrow_width_;
	last_points[3] = last_points[2] - normal * arrow_height_;

	Mesh* mesh = new Mesh;
	mesh->colorList.clear();
	mesh->colorList.push_back(ColorRGBA(0.0, 1.0, 1.0, 1.0));
	mesh->setComposite(ss.getResidue(0));

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
	Position res;
	Position spline_point_nr = 0;
	for (res = 0; res < spline_vector_.size() - 2; res++)
	{
		if (res != 0) 
		{
			mesh = new Mesh();
			mesh->setComposite(ss.getResidue(res));

			// copy last 8 points from the old mesh
			for (Position p = 0; p < 8; p++)
			{
				mesh->vertex.push_back(last_mesh->vertex[last_mesh->vertex.size() - 8 + p]);
				mesh->normal.push_back(last_mesh->normal[last_mesh->normal.size() - 8 + p]);
			}
			last_vertices = 0;
		}

		// iterate over the spline points between two amino acids
		for (Position j = 0; j < interpolation_steps_; j++)
		{
			right  = spline_points_[spline_point_nr + 1] - spline_points_[spline_point_nr];
			if (right.getSquareLength() == 0)
			{
				spline_point_nr++;
				continue;
			}

			normal =   peptide_normals[res    ] *(1 - j * 0.9 / 8.0) 
							 + peptide_normals[res + 1] *     j * 0.9 / 8.0;

			drawStrand_(spline_points_[spline_point_nr], normal, right, arrow_width_, 
									last_vertices, *mesh);
			spline_point_nr ++;
		}

		last_mesh = mesh;
		geometric_objects_.push_back(mesh);
	}

	// finally, we draw the arrow
	for (Index j = -1; j <= 6; j++)
	{
		// interpolate the depth of the box
		float new_arrow_width = 2 * (1 - j * 0.95 / 6.0) * arrow_width_; 
		
		right  = spline_points_[res * interpolation_steps_ + j + 1] - spline_points_[res * 9 + j];

		drawStrand_(spline_points_[res * interpolation_steps_ + j], normal, right, new_arrow_width, last_vertices, *mesh);
	}

	last_point_ 				= spline_points_[spline_points_.size() - 1];
	have_start_point_ 	= true;

	// restore the old spline vectors
	spline_vector_ 					= spline_vector_backup;
	spline_points_        	= spline_backup;
	atoms_of_spline_points_ = atoms_backup;
}


// ---------------------------------------------------------------------
void AddCartoonModel::drawHelix_(SecondaryStructure& ss)
	throw()
{
	Atom* first = 0;
	Atom* last = 0;
	List<const Atom*> catoms;
	AtomIterator it;
	BALL_FOREACH_ATOM(ss, it)
	{
		if (it->getName()=="C")
		{
			if (first == 0) first = &*it;	
			last = &*it;
			catoms.push_back(&*it);
		}
	}

	if (catoms.size() == 0) return;


	if (have_start_point_)
	{
		// build tube connection to the last point
		Tube* tube = new Tube;
		tube->setRadius(tube_radius_);
		tube->setVertex1(last_point_);
		tube->setVertex2(first->getPosition());
		tube->setComposite(first);
		geometric_objects_.push_back(tube);

		// create sphere for the point
		Sphere* sphere = new Sphere;
		sphere->setRadius(tube_radius_);
		sphere->setPosition(last_point_);
		sphere->setComposite(first);
		geometric_objects_.push_back(sphere);
		
		// create sphere for the point
		sphere = new Sphere;
		sphere->setRadius(tube_radius_);
		sphere->setPosition(first->getPosition());
		sphere->setComposite(first);
		geometric_objects_.push_back(sphere);
	}

	List<const Atom*>::ConstIterator lit = catoms.begin();
	Vector3 normal = last->getPosition() - first->getPosition();

	// calcluate slices for the helix cylinder according to the C-atoms
	Vector3 last_pos = first->getPosition();
	Vector3 diff = (normal / (catoms.size() ));

	for (Position p = 0; p < catoms.size(); p++)
	{
		Tube* tube = new Tube;
		tube->setRadius(helix_radius_);
		tube->setVertex1(last_pos);
		last_pos += diff;
		tube->setVertex2(last_pos);
		tube->setComposite(*lit);
		geometric_objects_.push_back(tube);
		lit++;
	}
		
	// add a disc at the beginning and the end of the cylinder to close it
	Disc* disc = new Disc( Circle3(first->getPosition(), -normal, helix_radius_));
	disc->setComposite(first);
	geometric_objects_.push_back(disc);

	disc = new Disc(Circle3(last_pos, normal, helix_radius_));
	disc->setComposite(last);
	geometric_objects_.push_back(disc);

	// calculate start and end position in the spline_vector_ for 
	// first and last C-atom
	Position p1=0, p2=0; 
	for (Position i=0; i<spline_vector_.size(); i++)
	{
		if (spline_vector_[i].getVector() == first->getPosition())
		{
			p1 = i;
		}

		if (spline_vector_[i].getVector() == last->getPosition())
		{
			p2 = i;
			break;
		}
	}

	// smooth the normals of the neighbouring spline segment to the value of the cylinder normal
	if (p1 != 0)
	{
		spline_vector_[p1].setTangentialVector(normal);
	}

	if (p2 == 0 || p2 == spline_vector_.size() - 1) return;

	spline_vector_[p2 - 2].setVector(last_pos-diff);
	spline_vector_[p2 - 2].setTangentialVector(normal);
	
	spline_vector_[p2 - 1].setVector(last_pos);
	spline_vector_[p2 - 1].setTangentialVector(normal);

	calculateTangentialVectors_();
	createSplineSegment_(spline_vector_[p2 - 1], spline_vector_[p2]);

	last_point_ = last->getPosition();
	have_start_point_ = true;
}


// -------------------------------------------------------------------	
Processor::Result AddCartoonModel::operator() (Composite& composite)
{
	if (RTTI::isKindOf<Protein>(composite))
	{
		calculateComplementaryBases_(composite);
		return Processor::CONTINUE;
	}

	if (RTTI::isKindOf<Chain>(composite))
	{
		clear_();
		last_chain_ = &composite;
		computeSpline_(*RTTI::castTo<Chain>(composite));
		have_start_point_ = false;
		return Processor::CONTINUE;
	}

	if (!RTTI::isKindOf<SecondaryStructure>(composite))  return Processor::CONTINUE;
	SecondaryStructure& ss = *RTTI::castTo<SecondaryStructure>(composite);

	// if called for a SS and no calculation done for the parent chain
	if (last_chain_ != ss.getParent())
	{
		clear_();
		computeSpline_(ss);
	}

	if (ss.getType() == SecondaryStructure::HELIX)
	{
		if (draw_ribbon_)
		{
			Position nr_of_residues = ss.countResidues();
			Position max = getStartPosition_(ss) + nr_of_residues;
			if (max > spline_vector_.size() - 1) max = spline_vector_.size() - 1;

			drawRibbon_(spline_vector_position_ * interpolation_steps_, 
																			max * interpolation_steps_);
		}
		else
		{
		 drawHelix_(ss);
		}
	}
	else if ((ss.getType() == SecondaryStructure::STRAND) && (ss.countResidues() > 3))
	{
		drawStrand_(ss);
	}
	else
	{
		if (ss.countResidues() == 0) return Processor::CONTINUE;

		Residue* r = ss.getResidue(0);
		if ((r->getName().size() == 1) &&
				(r->getName() == "A" ||
				 r->getName() == "C" ||
				 r->getName() == "G" ||
				 r->getName() == "T" ||
				 r->getName() == "U"))
		{
			drawDNA_(ss);
			return Processor::CONTINUE;
		}

		drawTube_(ss) ;
	}

	return Processor::CONTINUE;
}


// -----------------------------------------------------------------
void AddCartoonModel::drawTube_(SecondaryStructure& ss)
	throw()
{
	if (spline_vector_.size() == 0) 
	{
		have_start_point_ = false;
		return;
	}

	Position nr_of_residues = ss.countResidues();
	Position max = getStartPosition_(ss) + nr_of_residues;
	if (max > spline_vector_.size() - 1) max = spline_vector_.size() - 1;

	buildGraphicalRepresentation_(spline_vector_position_ * interpolation_steps_, 
																										max * interpolation_steps_);
}


Size AddCartoonModel::getStartPosition_(const SecondaryStructure& ss)
	throw()
{
	// find first C atom
	Vector3 c1_position; 
	AtomIterator it;
	SecondaryStructure& ss2 = *(SecondaryStructure*) &ss;
	BALL_FOREACH_ATOM(ss2, it)
	{
		if (it->getName() == "C")
		{
			c1_position = it->getPosition();	
			break;
		}
	}

	// start of spline points in the vector for the residues of this SS
	Position index = 0; 

	// speed up search for current position in spline vector
	if (spline_vector_position_ != -1)
	{
		index = spline_vector_position_ + 1;
	}

	for (; index < spline_vector_.size(); index++)
	{
		if (spline_vector_[index].getVector() == c1_position) 
		{
			spline_vector_position_ = index;
			break;
		}
	}

	return index;
}

void AddCartoonModel::dump(std::ostream& s, Size depth) const
	throw()
{
	BALL_DUMP_STREAM_PREFIX(s);
	
	BALL_DUMP_DEPTH(s, depth);
	BALL_DUMP_HEADER(s, this, this);

	AddBackboneModel::dump(s, depth + 1);

	BALL_DUMP_STREAM_SUFFIX(s);
}


void AddCartoonModel::computeSpline_(AtomContainer& ac)
{
	AtomIterator it;
	BALL_FOREACH_ATOM(ac, it)
	{
		if (it->getName() == "C")
		{
			spline_vector_.push_back(SplinePoint(it->getPosition(), &*it));
		}
	}

	if (spline_vector_.size() < 2) return;

	calculateTangentialVectors_();

	spline_points_.push_back(spline_vector_[0].getVector());
	atoms_of_spline_points_.push_back(spline_vector_[0].getAtom());
	createSplinePath_();
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
void AddCartoonModel::drawRibbon_(Size start, Size end)
	throw()
{
	if (spline_points_.size() == 0) return;
	if (spline_points_.size() != atoms_of_spline_points_.size() ||
												end >= atoms_of_spline_points_.size() ||
											start >= atoms_of_spline_points_.size() )
	{
		Log.error() << "Error in " << __FILE__ << __LINE__ << std::endl;
		return;
	}

	if (end == 0) end = spline_points_.size();

	if (!have_start_point_)
	{
		last_point_ = spline_points_[start];
		start++;
	}

	Vector3 helix_dir = spline_points_[end] - spline_points_[start];
	helix_dir.normalize();
	helix_dir *= (ribbon_width_ * 0.5);

	// create sphere for the point
	Sphere* sphere = new Sphere;
	sphere->setRadius(tube_radius_);
	sphere->setPosition(last_point_);
	sphere->setComposite(atoms_of_spline_points_[start]);
	geometric_objects_.push_back(sphere);

	// calculate the number of slides for the circle and the angle in between them
	Size slides = (Size)(8.0 + drawing_precision_ * 8.0);
	Angle slides_angle = Angle(360.0 / slides, false);

	// direction vector of the two current spline points
	Vector3 dir;

	// prevent problems if last point is the same as the start point
	while (true)
	{
		dir = spline_points_[start] - last_point_;
		if (Maths::isZero(dir.getSquareLength()))
		{
			start++;
		}
		else
		{
			break;
		}
	}

	if (start >= atoms_of_spline_points_.size()) return;
			
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
	r.normalize();
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
	// create a new mesh with the points and triangles
	////////////////////////////////////////////////////////////
	// every residue get its own mesh to enable picking for the tube model
	Mesh* mesh = new Mesh();
	if (atoms_of_spline_points_[start] != 0)
	{
		mesh->setComposite(atoms_of_spline_points_[start]->getParent());
	}
	geometric_objects_.push_back(mesh);
	
	// insert connection between tubes
	mesh->vertex.push_back(last_point_ + helix_dir);
	mesh->normal.push_back(r);
	mesh->vertex.push_back(last_point_ - helix_dir);
	mesh->normal.push_back(r);

	for (Position p = 0; p < slides; p++)
	{
		mesh->vertex.push_back(last_point_ + helix_dir + new_points[p]);
		mesh->normal.push_back(new_points[p]);
		mesh->vertex.push_back(last_point_ - helix_dir + new_points[p]);
		mesh->normal.push_back(new_points[p]);
	}
	
	////////////////////////////////////////////////////////////
	// same data structures for faster access
	////////////////////////////////////////////////////////////
	Mesh::Triangle t;
	Size s_old = 2;  // start position of the last points in the meshs vertices
	Size s_new = 2;  // start position of the  new points in the meshs vertices

	//------------------------------------------------------>
	// iterate over all spline_points_
	//------------------------------------------------------>
	for (Position p = start; p < end - 1; p++)
	{
		// faster access to the current spline point
		const Vector3 point = spline_points_[p];
		
		// new direction vector: new point - last point
		const Vector3 dir_new = point - last_point_;

		// new normal vector
		Vector3 r_new = r - (
							 (dir_new.x * r.x       + dir_new.y *       r.y + dir_new.z *       r.z)  /
							 (dir_new.x * dir_new.x + dir_new.y * dir_new.y + dir_new.z * dir_new.z) 
							 * dir_new);
		r_new.normalize();
		r_new *= ribbon_radius_;

		////////////////////////////////////////////////////////////
		// rotate all points of the circle according to new normal
		////////////////////////////////////////////////////////////
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

		////////////////////////////////////////////////////////////
		// create a new mesh if we have a different atom now
		////////////////////////////////////////////////////////////
		if (atoms_of_spline_points_[p] != 0 &&
				mesh->getComposite() != atoms_of_spline_points_[p]->getParent())
		{
			const Mesh* old_mesh = mesh;
			mesh = new Mesh();
			if (atoms_of_spline_points_[p] != 0)
			{
				mesh->setComposite(atoms_of_spline_points_[p]->getParent());
			}
			geometric_objects_.push_back(mesh);

			// insert the vertices and normals of the last points again into the new mesh
			for (Position point_pos = old_mesh->vertex.size() - (slides * 2 + 2);
										point_pos < old_mesh->vertex.size(); point_pos++)
			{
				mesh->vertex.push_back(old_mesh->vertex[point_pos]);
				mesh->normal.push_back(old_mesh->normal[point_pos]);
			}

			s_old = 2;
		}
		
		
		// insert connection between tubes
		mesh->vertex.push_back(point + helix_dir);
		mesh->normal.push_back(r_new);
		mesh->vertex.push_back(point - helix_dir);
		mesh->normal.push_back(r_new);

		const Size sn = mesh->vertex.size() - 2;
		const Size so = mesh->vertex.size() - 2 - slides * 2 - 2;
		t.v1 = sn;
		t.v2 = so + 1;
		t.v3 = so;
 		mesh->triangle.push_back(t);

		t.v1 = sn;
		t.v2 = sn + 1;
		t.v3 = so + 1;
 		mesh->triangle.push_back(t);


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
			mesh->vertex.push_back(point + helix_dir + new_points[point_pos]);
			mesh->normal.push_back(new_points[point_pos]);

			t.v1 = s_old;			// last lower
			t.v2 = s_old + 2;	// last upper
			t.v3 = s_new;			// new upper
 			mesh->triangle.push_back(t);

			t.v1 = s_new;			// new upper
			t.v2 = s_new - 2;	// new lower
			t.v3 = s_old; 		// last lower
 			mesh->triangle.push_back(t);

			s_old++;
			s_new++;

			mesh->vertex.push_back(point - helix_dir + new_points[point_pos]);
			mesh->normal.push_back(new_points[point_pos]);

			t.v1 = s_old;			// last lower
			t.v2 = s_old + 2;	// last upper
			t.v3 = s_new;			// new upper
 			mesh->triangle.push_back(t);

			t.v1 = s_new;			// new upper
			t.v2 = s_new - 2;	// new lower
			t.v3 = s_old; 		// last lower
 			mesh->triangle.push_back(t);

			s_old++;
			s_new++;
		}

		r = r_new;
		last_point_ = point;
	}

	have_start_point_ = true;
	
	// create a sphere as an end cap for the point
	sphere = new Sphere;
	sphere->setRadius(tube_radius_);
	sphere->setPosition(last_point_);
	sphere->setComposite(atoms_of_spline_points_[end - 1]);
	geometric_objects_.push_back(sphere);
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
				 ((**rit2).getName() != "T" || (**rit2).getName() != "U")) ||
					((*rit1).getName() == "C" && (**rit2).getName() != "G") ||
					((*rit1).getName() == "G" && (**rit2).getName() != "C") ||
					((*rit1).getName() == "T" && (**rit2).getName() != "A") ||
					((*rit1).getName() == "U" && (**rit2).getName() != "A"))
			{
				continue;
			}

			float new_distance = ((positions[&*rit1] - positions[*rit2]).getSquareLength());
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
	normal.normalize();
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


void AddCartoonModel::drawWatsonCrickModel_(const SecondaryStructure& ss)
	throw()
{
	vector<Vector3>::iterator old_spline_point = spline_points_.begin();
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
		for (; sit != spline_points_.end(); sit++)
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

void AddCartoonModel::drawDNA_(SecondaryStructure& ss)
	throw()
{
	ResidueIterator rit = ss.beginResidue();
	AtomIterator ait;
	HashMap<Residue*, Atom*> base_atoms;

	// add O5* atom for 1. Residue
	BALL_FOREACH_ATOM(*rit, ait)
	{
		if (ait->getName() == "O5*")
		{
			spline_vector_.push_back(SplinePoint((*ait).getPosition(), &*ait));
			base_atoms[&*rit] = &*ait;
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
				spline_vector_.push_back(SplinePoint((*ait).getPosition(), &*ait));
				base_atoms[&*rit] = &*ait;
				break;
			}
		}
	}
	
	// add O3* atom for last Residue
	BALL_FOREACH_ATOM(*ss.getCTerminal(), ait)
	{
		if (ait->getName() == "O3*")
		{
			spline_vector_.push_back(SplinePoint((*ait).getPosition(), &*ait));
			break;
		}
	}

	tube_radius_ = DNA_helix_radius_;
	createBackbone_();

	if (!draw_DNA_as_ladder_)
	{
		drawWatsonCrickModel_(ss);
		return;
	}

	for (rit = ss.beginResidue(); +rit; ++rit)
	{
		Residue* r = &*rit;

		// prevent double drawing for complementary bases
		if (complementary_bases_.has(r) &&
				complementary_bases_[r] < r) 
		{
			continue;
		}

		Atom* base_atom = base_atoms[r];
		if (base_atom == 0) 
		{
			continue;
		}

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
				
			Atom* end_atom = 0;
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

			continue;
		}

		// ================ draw paired bases ===============
		Residue* partner = complementary_bases_[r];
		Atom* partner_base = 0;
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
	}

	have_start_point_ = false;
}



	} // namespace VIEW
} // namespace BALL
