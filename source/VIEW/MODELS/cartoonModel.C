// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: cartoonModel.C,v 1.28 2004/07/20 21:58:39 amoll Exp $

#include <BALL/VIEW/MODELS/cartoonModel.h>
#include <BALL/VIEW/PRIMITIVES/tube.h>
#include <BALL/VIEW/PRIMITIVES/disc.h>
#include <BALL/VIEW/PRIMITIVES/mesh.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/chain.h>
#include <BALL/KERNEL/residue.h>
#include <BALL/KERNEL/secondaryStructure.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/MATHS/matrix44.h>

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
				arrow_height_(0.4)
		{
		}

		AddCartoonModel::AddCartoonModel(const AddCartoonModel& cartoon)
			throw()
			:	AddBackboneModel(cartoon),
				last_chain_(0),
				spline_vector_position_(-1),
				helix_radius_(cartoon.helix_radius_),
				arrow_width_(cartoon.arrow_width_),
				arrow_height_(cartoon.arrow_height_)
		{
		}

		AddCartoonModel::~AddCartoonModel()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "Destructing object " << (void *)this 
										<< " of class " << RTTI::getName<AddCartoonModel>() << endl;
			#endif 
		}

		bool AddCartoonModel::finish()
		{
			return true;
		}


		void AddCartoonModel::clear_()
			throw()
		{
			spline_vector_.clear();
			last_parent_ = 0;
			have_start_point_ = false;

			spline_.clear();
			last_chain_ = 0;
			spline_vector_position_ = -1;
		}

		// -----------------------------------------------------------------
		void AddCartoonModel::drawStrand_(SecondaryStructure& ss)
			throw()
		{
			vector<Vector3> peptide_normals;
			bool last_residue = false;
			bool is_terminal_segment = false;
			bool first_residue = true;

			std::vector<Vector3> spline_backup = spline_;
			std::vector<SplinePoint> spline_vector_backup = spline_vector_;

			// we want to compute the spline ourselves for better smoothing properties
			spline_vector_.clear();
			spline_.clear();

			ResidueIterator ri;
			BALL_FOREACH_RESIDUE(ss, ri)
			{
				Atom* 	 CA = 0;
				Atom* 	  N = 0;
				Atom*		  C = 0;
				Atom* nextN = 0;
				Atom*  	  O = 0;

				// first find the CA of this residue
				AtomIterator ai;
				BALL_FOREACH_ATOM(*ri, ai)
				{
					if (ai->getName() == "CA")
					{
						CA = &(*ai);
						break;
					}
				};

				if (!CA)
				{
					Log.error() << "Could not draw strand: residue contains no C_alpha!" << std::endl;
					return;
				}

				// now find the remaining atoms
				AtomBondIterator bi;
				BALL_FOREACH_ATOM_BOND(*CA, bi)
				{
					Atom* partner = bi->getPartner(*CA);
					if (partner->getName() == "N") 
					{
						N = partner;
						continue;
					};

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

				if (!nextN) // we are at the last residue
				{
					last_residue = true;
					if (ri->isTerminal()) is_terminal_segment = true;
					continue;
				}

				Vector3 normal =   (O->getPosition() - C->getPosition()) 
												 % (nextN->getPosition()  - C->getPosition()) ;

				if (Maths::isZero(normal.getSquareLength())) 
				{
					Log.error() << "Could not draw cartoon style: degenerate peptide bond!" << std::endl;
					return;
				}
				peptide_normals.push_back(normal.normalize());

				// we have computed the normal. now compute the two spline points corresponding to this
				// amino acid: we take the point between the current N (N) and the C_alpha (CA) and
				// the point between the C_alpha and C
				
				// if this is the first residue, we also add the position of the N-atom to prevent
				// large "holes" in the representation. we also reuse the same normal as for the
				// first "real" spline point
				if (first_residue)
				{
					SplinePoint sp(CA->getPosition(), CA);
					spline_vector_.push_back(sp);
					peptide_normals.push_back(normal.normalize());
					first_residue = false;
				}

				Vector3 sv = C->getPosition() + (nextN->getPosition() - C->getPosition())*1./2.;
				SplinePoint sp(sv, N);
				spline_vector_.push_back(sp);	

				if (last_residue)
				{
					if (peptide_normals.size() == 0) 
					{
						Log.error() << "Could not draw cartoon style: no peptide bonds found!" << std::endl;
						return;
					}

					peptide_normals[peptide_normals.size()-1] = peptide_normals[peptide_normals.size()-2];
				}
			}

			calculateTangentialVectors_();
			spline_.push_back(spline_vector_[0].getVector());
			for (Position i=1; i<spline_vector_.size(); i++)
			{
				createSplineSegment2_(spline_vector_[i-1], spline_vector_[i]);
			}
	
			
			// if two adjacent normals differ by more than 90 degrees, we
			// flip them to ensure a smooth strand representation
			for (Position i=0; i<peptide_normals.size()-1; i++)
			{
				Angle current(fabs(acos(peptide_normals[i]*peptide_normals[i+1])));
				if ((current <= (float)Constants::PI*3./2.)&&(current >= (float)Constants::PI/2.))
				{
					Vector3 rotaxis = (peptide_normals[i]%peptide_normals[i+1]).normalize();
					Matrix4x4 rotmat;
					rotmat.rotate(Angle(M_PI), rotaxis);
					peptide_normals[i+1] = rotmat * peptide_normals[i+1];
				}
			}
			
			// an additional smoothing run...
			for (Position i=0; i<peptide_normals.size()-1; i++)
			{
				// To smooth the strand representation a bit, we iterate over all normal
				// vectors and compute the angle in between them.
				// Then we reduce the angle by an appropriate rotation to a third of the
				// original angle.
				Vector3 rotaxis = (peptide_normals[i]%peptide_normals[i+1]);

				if (rotaxis.getSquareLength() > 1e-2)
				{
					Angle current(fabs(acos(peptide_normals[i]*peptide_normals[i+1])));
					Angle new_angle = Angle(2./3.*current);

					Angle diff_angle = new_angle - current;
					Matrix4x4 rotmat;

					rotmat.rotate(diff_angle, rotaxis);

					peptide_normals[i+1] = rotmat * peptide_normals[i+1];
				}
			}
			
			// put first four points into the mesh (and first two triangles)
			Vector3 right = spline_[1] - spline_[0];
			Vector3 normal = peptide_normals[0];

			// maybe these cases should not happen, but they do...
			if (!Maths::isZero(normal.getSquareLength())) normal.normalize();
			if (!Maths::isZero(right.getSquareLength()))  right.normalize();

			Vector3 perpendic = normal % right; // perpendicular to spline
			Vector3 last_points[4];

			last_points[0] = spline_[0] - (perpendic * arrow_width_/2.) - normal * arrow_height_/2.;
			last_points[1] = last_points[0] + normal * arrow_height_;
			last_points[2] = last_points[1] + perpendic * arrow_width_;
			last_points[3] = last_points[2] - normal * arrow_height_;

			Mesh* mesh = new Mesh;
			if (mesh == 0) throw Exception::OutOfMemory(__FILE__, __LINE__, sizeof(Mesh));
			mesh->colorList.clear();
			mesh->colorList.push_back(ColorRGBA(0,1.,0));
			mesh->setComposite(ss.getResidue(0));

			vector<Vector3>* vertices = &mesh->vertex;
			vector<Vector3>* normals  = &mesh->normal;

			for (Position i = 0; i < 4; i++)
			{
				vertices->push_back(last_points[i]);
				normals->push_back(right * -1);
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
				vertices->push_back(last_points[p]);
				vertices->push_back(last_points[p]);
			}

			Position last_vertices = 4;

			normals->push_back(perpendic*-1.);
			normals->push_back(normal*-1.);

			normals->push_back(perpendic*-1.);
			normals->push_back(normal);

			normals->push_back(perpendic);
			normals->push_back(normal);

			normals->push_back(perpendic);
			normals->push_back(normal*-1.);

			Mesh* last_mesh = 0;
			// iterate over all but the last amino acid (last amino acid becomes the arrow)
			Position res;
			for (res=0; res<spline_vector_.size() - 2; res++)
			{
				if (res != 0) 
				{
					mesh = new Mesh();
					vertices = &mesh->vertex;
					normals  = &mesh->normal;
					mesh->setComposite(ss.getResidue(res));

					// copy last 8 points from the old mesh
					for (Position p = 0; p < 8; p++)
					{
						vertices->push_back(last_mesh->vertex[last_mesh->vertex.size() - 8 + p]);
						normals->push_back(last_mesh->normal[last_mesh->normal.size() - 8 + p]);
					}
					last_vertices = 0;
				}

				// iterate over the spline points between two amino acids
				for (Position j=0; j<=8; j++)
				{
					right  = spline_[res*9+j+1] - spline_[res*9+j];

					normal =   peptide_normals[res  ] *(1-j * 0.9/8.) 
						       + peptide_normals[res+1] *   j * 0.9/8.;

					drawStrand_(spline_[res*9+j], normal, right, arrow_width_, last_vertices, *mesh);
				}

				last_mesh = mesh;
				geometric_objects_.push_back(mesh);
			}

			// finally, we draw the arrow
			for (Index j=-1; j<=6; j++)
			{
				// interpolate the depth of the box
				float new_arrow_width = 2*(1-j*0.95/6.)*arrow_width_; 
				
				right  = spline_[res*9+j+1] - spline_[res*9+j];

				drawStrand_(spline_[res*9+j], normal, right, new_arrow_width, last_vertices, *mesh);
			}

			last_spline_point_ = spline_vector_[spline_vector_.size()-1];
			last_point_ = last_spline_point_.getVector();
			have_start_point_ = true;
			if (!is_terminal_segment)
			{
				for (Position k=7; k<=9; k++)
				{
		 			buildGraphicalRepresentation_(spline_[res*9+k], spline_vector_[res].getAtom()); 
				}
			}
			else
			{
		 		buildGraphicalRepresentation_(spline_[res*9], spline_vector_[res].getAtom()); 
			}

			// restore the old spline vectors
			spline_vector_ = spline_vector_backup;
			spline_        = spline_backup;
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

			List<const Atom*>::ConstIterator lit = catoms.begin();
			lit++;
			Vector3 normal = last->getPosition() - first->getPosition();

			// calcluate slices for the helix cylinder according to the C-atoms
			Vector3 last_pos = first->getPosition();
			Vector3 diff = (normal / (catoms.size() ));

			for (Position p = 0; p < catoms.size() -1; p++)
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
			if (!disc) throw Exception::OutOfMemory (__FILE__, __LINE__, sizeof(Disc));
			disc->setComposite(first);
			geometric_objects_.push_back(disc);

			disc = new Disc(Circle3(last_pos, normal, helix_radius_));
			if (!disc) throw Exception::OutOfMemory (__FILE__, __LINE__, sizeof(Disc));
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

			if (p2 == 0 || p2 == spline_vector_.size()-1) return;

			spline_vector_[p2-2].setVector(last_pos-diff);
			spline_vector_[p2-2].setTangentialVector(normal);
			
			spline_vector_[p2-1].setVector(last_pos);
			spline_vector_[p2-1].setTangentialVector(normal);

			calculateTangentialVectors_();
 			createSplineSegment_(spline_vector_[p2-1], spline_vector_[p2]);

 			last_spline_point_ = spline_vector_[p2];
			last_point_ = last_spline_point_.getVector();
			have_start_point_ = true;
		}

		// -------------------------------------------------------------------	
		Processor::Result AddCartoonModel::operator () (Composite& composite)
		{
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
				 drawHelix_(ss);
			}
			else if ((ss.getType() == SecondaryStructure::STRAND) && (ss.countResidues() > 3))
			{
 			 	drawStrand_(ss);
			}
			else
			{
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

			Vector3 c1_position; // of first C atom
			AtomIterator it;
			BALL_FOREACH_ATOM(ss, it)
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

			for (; index<spline_vector_.size(); index++)
			{
				if (spline_vector_[index].getVector() == c1_position) 
				{
					spline_vector_position_ = index;
					break;
				}
			}

			Position nr_of_residues = ss.countResidues();
			Position max = index + nr_of_residues;
			if (max > spline_vector_.size() -1 ) max = spline_vector_.size() -1;

			for (Position pos = index; pos < max ; pos++)
			{
				for (Position j = 0; j < 9; j++)
				{
					buildGraphicalRepresentation_(
						spline_[pos*9 + j], (j < 5) ? 
						spline_vector_[pos].getAtom() : 
						spline_vector_[pos+1].getAtom());
				}
			}

		 	have_start_point_ = false;
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
					SplinePoint spline_point(it->getPosition(), &*it);
					spline_vector_.push_back(spline_point);
				}
			}

			if (spline_vector_.size() < 2) return;

			calculateTangentialVectors_();

			spline_.push_back(spline_vector_[0].getVector());
			
			for (Position i=1; i<spline_vector_.size(); i++)
			{
				createSplineSegment2_(spline_vector_[i-1], spline_vector_[i]);
			}
		}


		void AddCartoonModel::createSplineSegment2_(const SplinePoint &a, const SplinePoint &b)
		{
			Size max_step = 9;
			double time = 0.0;
			double step = (double)1 / (double)max_step;

			for (Position index = 0; index <= max_step; ++index, time += step)
			{
				if (index == 0) continue;

				double t_2 = time * time;
				double t_3 = t_2 * time;
				double m2_t_3 = 2.0 * t_3;
				double m3_t_2 = 3.0 * t_2;

				double h1 = m2_t_3 - m3_t_2 + 1.0;
				double h2 = - m2_t_3 + m3_t_2;
				double h3 = t_3 - 2.0 * t_2 + time;
				double h4 = t_3 - t_2;

				Vector3 new_vector;

				new_vector.x = (h1 * a.getVector().x) + 
											 (h2 * b.getVector().x) + 
											 (h3 * a.getTangentialVector().x) + 
											 (h4 * b.getTangentialVector().x);

				new_vector.y = (h1 * a.getVector().y) + 
											 (h2 * b.getVector().y) + 
											 (h3 * a.getTangentialVector().y) + 
											 (h4 * b.getTangentialVector().y);

				new_vector.z = (h1 * a.getVector().z) + 
											 (h2 * b.getVector().z) + 
											 (h3 * a.getTangentialVector().z) + 
											 (h4 * b.getTangentialVector().z);

				spline_.push_back(new_vector);
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
			current_points[0] = start - (perpendic * arrow_width/2.) - normal * arrow_height_/2.;
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
			normals->push_back(normal* -1.);

			normals->push_back(perpendic);
			normals->push_back(normal* -1.);

			normals->push_back(perpendic *  1);
			normals->push_back(normal*  -1.);

			normals->push_back(perpendic *  1);
			normals->push_back(normal*  -1.);

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

	} // namespace VIEW
} // namespace BALL
