// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: cartoonModel.C,v 1.15 2003/12/10 14:05:01 anhi Exp $

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
				helix_radius_(2.4),
				arrow_width_(2.6),
				arrow_height_(0.5)
		{
		}

		AddCartoonModel::AddCartoonModel(const AddCartoonModel& cartoon)
			throw()
			:	AddBackboneModel(cartoon),
				last_chain_(0),
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

		void AddCartoonModel::clear()
			throw()
		{
			AddBackboneModel::clear();
		}

		bool AddCartoonModel::start()
		{
			return AddBackboneModel::start();
		}
				
		bool AddCartoonModel::finish()
		{
			spline_vector_.clear();
			spline_.clear();

			return true;
		}

		void AddCartoonModel::drawStrand_(SecondaryStructure& ss)
		{
			vector<Atom*> 	c_position;
			vector<Vector3> peptide_normals;
			vector<bool>    is_proline; // needed for the smoothing
			bool last_residue = false;

			AtomIterator it;
			BALL_FOREACH_ATOM(ss, it)
			{
				if ((it->getName() == "C"))
				{
					// first determine if we are in a proline residue
					if (it->getResidue()->getName() == "PRO")
						is_proline.push_back(true);
					else
						is_proline.push_back(false);

					c_position.push_back(&*it);
					// now we can find the corresponding nitrogen and oxygen
					Atom* N = 0;
					Atom* O = 0;

					AtomBondIterator bi;
					BALL_FOREACH_ATOM_BOND(*it, bi)
					{
						Atom* partner = bi->getPartner(*it);
						if (partner->getName() == "N") 
						{
							N = partner;
						}
						if (partner->getName() == "O")
						{
							O = partner;
						}
					}
					if (!N) // we are at the last residue
					{
						last_residue = true;
						continue;
					}
					if (!O) 
					{
						Log.error() << "Could not draw cartoon style: no oxygen in peptide bond!" << std::endl;
						return;
					}	

					Vector3 normal =   (O->getPosition()  - it->getPosition()) 
						               % (N->getPosition()  - it->getPosition()) ;

					if (Maths::isZero(normal.getSquareLength())) 
					{
						Log.error() << "Could not draw cartoon style: degenerate peptide bond!" << std::endl;
						return;
					}
					peptide_normals.push_back(normal.normalize());
				}


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

			if (c_position.size() == 0) 
			{
				return;
			}
			Atom* first = c_position[0];
			Atom* last  = c_position[c_position.size() - 1];

			Position first_c=0, last_c=0; // positions of the first and last C in the spline vector
			for (Position i=0; i<spline_vector_.size(); i++)
			{
				if (spline_vector_[i].getVector() == first->getPosition()) first_c = i;

				if (spline_vector_[i].getVector() == last->getPosition())
				{
					last_c = i;
					break;
				}
			}

			// if two adjacent normals differ by more than 90 degrees, we
			// flip them to ensure a smooth strand representation
			for (Position i=0; i<peptide_normals.size()-1; i++)
			{
			//	if (!is_proline[i+1])
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
			}

			// put first four points into the mesh (and first two triangles)
			Vector3 right = spline_[first_c*9+1] - spline_[first_c*9];

			Vector3 normal = peptide_normals[0];

			// maybe these cases should not happen, but they do...
			if (!Maths::isZero(normal.getSquareLength())) normal.normalize();
			if (!Maths::isZero(right.getSquareLength()))  right.normalize();

			Vector3 perpendic = normal % right; // perpendicular to spline

			Vector3 last_points[4];
			Vector3 current_points[4];

			Position last_vertices=0;

			arrow_height_ = 0.5;
			arrow_width_  = 0.8;

			last_points[0] = first->getPosition() - (perpendic * arrow_width_/2.) - normal * arrow_height_/2.;
			last_points[1] = last_points[0] + normal * arrow_height_;
			last_points[2] = last_points[1] + perpendic * arrow_width_;
			last_points[3] = last_points[2] - normal * arrow_height_;

			Mesh* mesh = new Mesh;
			if (mesh == 0) throw Exception::OutOfMemory(__FILE__, __LINE__, sizeof(Mesh));
			mesh->colorList.clear();
			mesh->colorList.push_back(ColorRGBA(0,1.,0));

			for (Position i = 0; i < 4; i++)
			{
				mesh->vertex.push_back(last_points[i]);
				mesh->normal.push_back(right * -1);
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

			vector<Vector3>& vertices   = mesh->vertex;
			vector<Vector3>& normals    = mesh->normal;

			// we insert all points twice (the first once even thrice!) 
			// in order to get sensible normals for
			// the triangles...
			last_vertices = 4;

			vertices.push_back(last_points[0]);
			normals.push_back(perpendic*-1.);
			vertices.push_back(last_points[0]);
			normals.push_back(normal*-1.);

			vertices.push_back(last_points[1]);
			normals.push_back(perpendic*-1.);
			vertices.push_back(last_points[1]);
			normals.push_back(normal);

			vertices.push_back(last_points[2]);
			normals.push_back(perpendic);
			vertices.push_back(last_points[2]);
			normals.push_back(normal);

			vertices.push_back(last_points[3]);
			normals.push_back(perpendic);
			vertices.push_back(last_points[3]);
			normals.push_back(normal*-1.);

			// error checking for debugging (just to be sure we dont get a nasty core dump)
			if ((last_c-1)*9+8+1 >= spline_.size())
			{
				Log.error() << "Calculation of strand stop because of error in " 
										<< __FILE__ << __LINE__ << std::endl;
				return;
			}

			// iterate over all but the last amino acid (last amino acid becomes the arrow)
			Position i;
			for (i=first_c; i<last_c - 1; i++)
			{

				// iterate over the spline points between two amino acids
				for (Position j=0; j<=8; j++)
				{
					right  = spline_[i*9+j+1] - spline_[i*9+j];

					normal =   peptide_normals[i-first_c  ] *(1-j * 0.9/8.) 
						       + peptide_normals[i-first_c+1] *   j * 0.9/8.;

					// maybe this cases should not happen, but they do...
					if (!Maths::isZero(normal.getSquareLength())) normal.normalize();
					if (!Maths::isZero(right.getSquareLength())) right.normalize();

					perpendic = normal % right; // perpendicular to spline
					if (!Maths::isZero(perpendic.getSquareLength())) perpendic.normalize();

					current_points[0] = spline_[i*9+j] - (perpendic * arrow_width_/2.) - normal * arrow_height_/2.;
					current_points[1] = current_points[0] + normal * arrow_height_;
					current_points[2] = current_points[1] + perpendic * arrow_width_;
					current_points[3] = current_points[2] - normal * arrow_height_;

					// put the next 4 points and 8 triangles into the mesh
					vertices.push_back(current_points[0]);
					normals.push_back(perpendic*-1.);
					vertices.push_back(current_points[0]);
					normals.push_back(normal*-1.);

					vertices.push_back(current_points[1]);
					normals.push_back(perpendic*-1.);
					vertices.push_back(current_points[1]);
					normals.push_back(normal);

					vertices.push_back(current_points[2]);
					normals.push_back(perpendic);
					vertices.push_back(current_points[2]);
					normals.push_back(normal);

					vertices.push_back(current_points[3]);
					normals.push_back(perpendic);
					vertices.push_back(current_points[3]);
					normals.push_back(normal*-1.);

					insertTriangle_(last_vertices    , last_vertices +  2, last_vertices + 10, *mesh);
					insertTriangle_(last_vertices    , last_vertices + 10, last_vertices +  8, *mesh);
					insertTriangle_(last_vertices + 4, last_vertices + 12, last_vertices + 14, *mesh);
					insertTriangle_(last_vertices + 4, last_vertices +  6, last_vertices + 14, *mesh);
					insertTriangle_(last_vertices + 3, last_vertices +  5, last_vertices + 13, *mesh);
					insertTriangle_(last_vertices + 3, last_vertices + 11, last_vertices + 13, *mesh);
					insertTriangle_(last_vertices + 1, last_vertices +  7, last_vertices + 15, *mesh);
					insertTriangle_(last_vertices + 1, last_vertices +  9, last_vertices + 15, *mesh);

					last_vertices+=8;
				}
			}

			// finally, we draw the arrow
			for (Index j=-1; j<=6; j++)
			{
				// interpolate the depth of the box
				float new_arrow_width = 2*(1-j*0.95/6.)*arrow_width_; 
				
				right  = spline_[i*9+j+1] - spline_[i*9+j];

//				normal =   peptide_normals[i-first_c  ] *(1-j * 0.9/8.) 
//					       + peptide_normals[i-first_c+1] *   j * 0.9/8.;

				if (!Maths::isZero(normal.getSquareLength())) normal.normalize();
				if (!Maths::isZero(right.getSquareLength())) right.normalize();

				perpendic = normal % right; // perpendicular to spline
				if (!Maths::isZero(perpendic.getSquareLength())) perpendic.normalize();

				current_points[0] = spline_[i*9+j] - (perpendic * new_arrow_width/2.) - normal * arrow_height_/2.;
				current_points[1] = current_points[0] + normal * arrow_height_;
				current_points[2] = current_points[1] + perpendic * new_arrow_width;
				current_points[3] = current_points[2] - normal * arrow_height_;

				// put the next 4 points and 8 triangles into the mesh
				vertices.push_back(current_points[0]);
				normals.push_back(perpendic*-1.);
				vertices.push_back(current_points[0]);
				normals.push_back(normal*-1.);

				vertices.push_back(current_points[1]);
				normals.push_back(perpendic*-1.);
				vertices.push_back(current_points[1]);
				normals.push_back(normal);

				vertices.push_back(current_points[2]);
				normals.push_back(perpendic);
				vertices.push_back(current_points[2]);
				normals.push_back(normal);

				vertices.push_back(current_points[3]);
				normals.push_back(perpendic);
				vertices.push_back(current_points[3]);
				normals.push_back(normal*-1.);

				insertTriangle_(last_vertices    , last_vertices +  2, last_vertices + 10, *mesh);
				insertTriangle_(last_vertices    , last_vertices + 10, last_vertices +  8, *mesh);
				insertTriangle_(last_vertices + 4, last_vertices + 12, last_vertices + 14, *mesh);
				insertTriangle_(last_vertices + 4, last_vertices +  6, last_vertices + 14, *mesh);
				insertTriangle_(last_vertices + 3, last_vertices +  5, last_vertices + 13, *mesh);
				insertTriangle_(last_vertices + 3, last_vertices + 11, last_vertices + 13, *mesh);
				insertTriangle_(last_vertices + 1, last_vertices +  7, last_vertices + 15, *mesh);
				insertTriangle_(last_vertices + 1, last_vertices +  9, last_vertices + 15, *mesh);

				last_vertices+=8;
			}

			geometric_objects_.push_back(mesh);

			// now we just need to connect the strand to the rest of the backbone
			if (first_c >= 1)
			{
				createSplineSegment_(spline_vector_[first_c-1], spline_vector_[first_c]);
			}

			last_point_ = spline_[i*9+6];
			if (!((Residue*)last->getParent())->isTerminal())//ss.countResidues() > 2)
			{
				for (Position k=7; k<=9; k++)
				{
					buildGraphicalRepresentation_(spline_[i*9+k], spline_vector_[i].getAtom()); 
				}
			}
			else
			{
				buildGraphicalRepresentation_(spline_[i*9], spline_vector_[i].getAtom()); 
			}

			last_point_ = last->getPosition();
		}


		void AddCartoonModel::insertTriangle_(Position v1, Position v2, Position v3, Mesh& mesh)
		{
			Surface::Triangle t;
			t.v1 = v1;
			t.v2 = v2;
			t.v3 = v3;
			mesh.triangle.push_back(t);
		}

		void AddCartoonModel::drawHelix_(SecondaryStructure& ss)
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
			Disc* disc = new Disc( Circle3(first->getPosition(), normal, helix_radius_));
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
				createSplineSegment_(spline_vector_[p1-1], spline_vector_[p1]);
			}

			if (p2 == 0 || p2 == spline_vector_.size()-1) return;

			spline_vector_[p2-2].setVector(last_pos-diff);
			spline_vector_[p2-2].setTangentialVector(normal);
			
			spline_vector_[p2-1].setVector(last_pos);
			spline_vector_[p2-1].setTangentialVector(normal);

			calculateTangentialVectors_();
			createSplineSegment_(spline_vector_[p2-1], spline_vector_[p2]);

			spline_.clear();
			spline_.push_back(spline_vector_[0].getVector());
			
			for (Position i=1; i<spline_vector_.size(); i++)
			{
				createSplineSegment2_(spline_vector_[i-1], spline_vector_[i]);
			}
			
 			last_point_ = spline_vector_[p2].getVector();
			have_start_point_ = true;
		}

				
		Processor::Result AddCartoonModel::operator () (Composite& composite)
		{
			if (RTTI::isKindOf<Chain>(composite))
			{
				last_chain_ = &composite;
				computeSpline_(*RTTI::castTo<Chain>(composite));
				return Processor::CONTINUE;
			}

			if (!RTTI::isKindOf<SecondaryStructure>(composite))  return Processor::CONTINUE;

			// if called for a SS and no calculation done for the parent chain
			if (last_chain_ != composite.getParent())
			{
				if (composite.getParent() == 0)
				{
					computeSpline_(*RTTI::castTo<SecondaryStructure>(composite));
				}

				last_chain_ = composite.getParent();
				computeSpline_(*RTTI::castTo<AtomContainer>(composite));
			}

			SecondaryStructure& ss(*RTTI::castTo<SecondaryStructure>(composite));
			if (ss.hasProperty(SecondaryStructure::PROPERTY__HELIX))
			{
				drawHelix_(ss);
			}
			else if (ss.hasProperty(SecondaryStructure::PROPERTY__STRAND))
			{
				drawStrand_(ss);
			}
			else
			{
				if (spline_vector_.size() == 0) return Processor::CONTINUE;

				Vector3 c1_position; // of first C
				AtomIterator it;
				BALL_FOREACH_ATOM(ss, it)
				{
					if (it->getName() == "C")
					{
						c1_position = it->getPosition();	
						break;
					}
				}

				Position index = 0; // start of spline points in the vector for the residues of this SS
				for (; index<spline_vector_.size(); index++)
				{
					if (spline_vector_[index].getVector() == c1_position) break;
				}

				Position nr_of_residues = ss.countResidues() -1;
				for (Position res = 0; res < nr_of_residues; res++)
				{
					Position pos = res + index;
					for (Position j = 0; j < 9; j++)
					{
						buildGraphicalRepresentation_(
								spline_[pos*9 + j], (j < 5) ? 
								spline_vector_[pos].getAtom() : 
								spline_vector_[pos+1].getAtom());
					}
				}
			}
			return Processor::CONTINUE;
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
			have_start_point_ = false;
			spline_vector_.clear();
			spline_.clear();
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

	} // namespace VIEW
} // namespace BALL
