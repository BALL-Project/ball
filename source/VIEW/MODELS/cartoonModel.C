// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: cartoonModel.C,v 1.1 2003/08/26 12:38:38 oliver Exp $

#include <BALL/MOLVIEW/FUNCTOR/cartoonModel.h>
#include <BALL/VIEW/GUI/FUNCTOR/colorProcessor.h>
#include <BALL/VIEW/PRIMITIV/tube.h>
#include <BALL/VIEW/PRIMITIV/disc.h>
#include <BALL/VIEW/PRIMITIV/mesh.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/chain.h>
#include <BALL/KERNEL/residue.h>
#include <BALL/KERNEL/secondaryStructure.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>

using namespace std;

namespace BALL
{
	using namespace VIEW;

	namespace MOLVIEW
	{

		AddCartoonModel::AddCartoonModel()
			throw()
			: AddBackboneModel()
		{
		}

		AddCartoonModel::AddCartoonModel(const AddCartoonModel& add_Cartoon)
			throw()
			:	AddBackboneModel(add_Cartoon)
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
			vector<Atom*> c_position;
			vector<Vector3> peptide_normals;
			bool last_residue = false;

			AtomIterator it;
			BALL_FOREACH_ATOM(ss, it)
			{
				if ((it->getName() == "C"))
				{
					c_position.push_back(&*it);
					// now we can find the corresponding nitrogen
					// and oxygen
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

					if (normal.getSquareLength() == 0) 
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
				if (fabs(acos(peptide_normals[i]*peptide_normals[i+1])) >= M_PI/2.)
				{
					peptide_normals[i+1]*=-1.;
				}
			}

			// put first four points into the mesh (and first two triangles)
			Vector3 right = spline_[first_c*9+1] - spline_[first_c*9];

			Vector3 normal = peptide_normals[0];
			normal.normalize();


			right.normalize();
			float box_height_ = 0.6;
			float box_depth_  = 1.5;//3.0;
			Vector3 perpendic = normal % right; // perpendicular to spline

			Vector3 last_points[4];
			Vector3 current_points[4];

			Position last_vertices=0;

			last_points[0] = first->getPosition() - (perpendic * box_depth_/2.) - normal * box_height_/2.;
			last_points[1] = last_points[0] + normal * box_height_;
			last_points[2] = last_points[1] + perpendic * box_depth_;
			last_points[3] = last_points[2] - normal * box_height_;

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


					normal.normalize();
					right.normalize();

					perpendic = normal % right; // perpendicular to spline
					perpendic.normalize();

					current_points[0] = spline_[i*9+j] - (perpendic * box_depth_/2.) - normal * box_height_/2.;
					current_points[1] = current_points[0] + normal * box_height_;
					current_points[2] = current_points[1] + perpendic * box_depth_;
					current_points[3] = current_points[2] - normal * box_height_;

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
				float new_box_depth = 2.5*(1-j*0.95/6.)*box_depth_; 
				
				right  = spline_[i*9+j+1] - spline_[i*9+j];

//				normal =   peptide_normals[i-first_c  ] *(1-j * 0.9/8.) 
//					       + peptide_normals[i-first_c+1] *   j * 0.9/8.;

				normal.normalize();
				right.normalize();

				perpendic = normal % right; // perpendicular to spline
				perpendic.normalize();

				current_points[0] = spline_[i*9+j] - (perpendic * new_box_depth/2.) - normal * box_height_/2.;
				current_points[1] = current_points[0] + normal * box_height_;
				current_points[2] = current_points[1] + perpendic * new_box_depth;
				current_points[3] = current_points[2] - normal * box_height_;

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
				createSplineSegment_(spline_vector_[first_c-1], spline_vector_[first_c]);

			last_point_ = spline_[i*9+6];
			for (Position k=7; k<=9; k++)
				buildGraphicalRepresentation_(spline_[i*9+k], spline_vector_[i].getColor());

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
			AtomIterator it;
			BALL_FOREACH_ATOM(ss, it)
			{
				if (it->getName()=="C")
				{
					if (!first) first = &*it;	
					last = &*it;
				}
			}

			// build tube connection to the last point
			Tube* tube = new Tube;
			if (!tube) throw Exception::OutOfMemory (__FILE__, __LINE__, sizeof(Tube));

			tube->setRadius(2.4);
			tube->setVertex1(first->getPosition());
			tube->setVertex2(last->getPosition());
			ColorRGBA color(1.0,0,0);
			tube->setColor(color);
			geometric_objects_.push_back(tube);

			Disc* disc = new Disc( Circle3(first->getPosition(), Vector3(first->getPosition() - last->getPosition()), 2.4));
			if (!disc) throw Exception::OutOfMemory (__FILE__, __LINE__, sizeof(Disc));
			disc->setColor(color);
			geometric_objects_.push_back(disc);

			disc = new Disc(Circle3(last->getPosition(), Vector3(last->getPosition() - first->getPosition() ), 2.4));
			if (!disc) throw Exception::OutOfMemory (__FILE__, __LINE__, sizeof(Disc));
			disc->setColor(color);
			geometric_objects_.push_back(disc);

			Position p1=0, p2=0;
			for (Position i=0; i<spline_vector_.size(); i++)
			{
				if (spline_vector_[i].getVector() == first->getPosition())
					p1 = i;

				if (spline_vector_[i].getVector() == last->getPosition())
				{
					p2 = i;
					break;
				}
			}


			if (p1 != 0)
			{
				spline_vector_[p1].setTangentialVector(last->getPosition() - first->getPosition());
				createSplineSegment_(spline_vector_[p1-1], spline_vector_[p1]);
			}

			if (p2 < spline_vector_.size()-1 )
			{
				spline_vector_[p2].setTangentialVector(last->getPosition() - first->getPosition());
				createSplineSegment_(spline_vector_[p2], spline_vector_[p2+1]);
			}	

			last_point_ = spline_vector_[p2+1].getVector();
		}

				
		Processor::Result AddCartoonModel::operator () (Composite& composite)
		{
			if (RTTI::isKindOf<Chain>(composite))
			{
				computeSpline_(*RTTI::castTo<Chain>(composite));
				return Processor::CONTINUE;
			}

			if (!RTTI::isKindOf<SecondaryStructure>(composite))  return Processor::CONTINUE;
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
				Vector3 position; // of first C
				AtomIterator it;
				BALL_FOREACH_ATOM(ss, it)
				{
					if (it->getName() == "C")
					{
						position = it->getPosition();	
						break;
					}
				}

				// Calculate start of spline points in the vector for the residues of this SS
				Position index = 0; 
				for (; index<spline_vector_.size(); index++)
				{
					if (spline_vector_[index].getVector() == position) break;
				}

				for (Position i = 0; i < ss.countResidues() - 1; i++)
				{
					for (Position j = 0; j < 9; j++)
					{
						buildGraphicalRepresentation_(
								spline_[(i+index)*9 + j], (j < 5) ? 
								spline_vector_[i+index].getColor() : 
								spline_vector_[i+index+1].getColor());
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

			MolecularModelProcessor::dump(s, depth + 1);

			BALL_DUMP_STREAM_SUFFIX(s);
		}


		void AddCartoonModel::computeSpline_(Chain& chain)
		{
			have_start_point_ = false;
			spline_vector_.clear();
			spline_.clear();
			AtomIterator it;
			BALL_FOREACH_ATOM(chain, it)
			{
				if (it->getName() == "C")
				{
					getColorProcessor()->operator() (&*it);
					SplinePoint spline_point(it->getPosition(), getColorProcessor()->getColor());
					spline_vector_.push_back(spline_point);
				}
			}

			if (spline_vector_.size() == 0) return;

			calculateTangentialVectors_();

			spline_.push_back(spline_vector_[0].getVector());
			
			for (Position i=1; i<spline_vector_.size(); i++)
			{
				createSplineSegment2_(spline_vector_[i-1], spline_vector_[i]);
			}
		}


		void AddCartoonModel::createSplineSegment2_(const SplinePoint &a, const SplinePoint &b)
		{
			int max_step = 9;
			double time = 0.0;
			double step = (double)1 / (double)max_step;

			for (int index = 0; index <= max_step; ++index, time += step)
			{
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

				if (index != 0) spline_.push_back(new_vector);
			}
		}

	} // namespace MOLVIEW
} // namespace BALL
