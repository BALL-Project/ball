// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: backboneModel.C,v 1.22.4.1 2005/05/25 00:54:31 amoll Exp $
//

#include <BALL/VIEW/MODELS/backboneModel.h>
#include <BALL/VIEW/PRIMITIVES/sphere.h>
#include <BALL/VIEW/PRIMITIVES/tube.h>
#include <BALL/VIEW/PRIMITIVES/mesh.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/residue.h>
#include <BALL/KERNEL/chain.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/bond.h>

#include <BALL/VIEW/PRIMITIVES/line.h>
#include <BALL/MATHS/analyticalGeometry.h>
#include <BALL/MATHS/matrix44.h>
#include <BALL/VIEW/KERNEL/common.h>

#include <algorithm>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

		AddBackboneModel::SplinePoint::SplinePoint()
			: point(),
				tangent(),
				atom(0)
		{
		}


		AddBackboneModel::SplinePoint::SplinePoint(const Vector3& new_point, const Atom* new_atom)
			: point(new_point),
				tangent(),
				atom(new_atom)
		{
		}

		bool AddBackboneModel::SplinePoint::operator < (const AddBackboneModel::SplinePoint& point) const
			throw()
		{
			try
			{
				return ((Residue*) atom->getParent())->getID().toUnsignedInt() <=
							 ((Residue*) point.atom->getParent())->getID().toUnsignedInt();
			}
			catch(...)
			{
			}

			return ((Residue*) atom->getParent())->getID() <=
						 ((Residue*) point.atom->getParent())->getID();
		}


		AddBackboneModel::AddBackboneModel()
			throw()
			: ModelProcessor(),
				last_residue_(0),
				tube_radius_((float)0.4),
				interpolation_steps_(9)
		{
		}

		AddBackboneModel::AddBackboneModel(const AddBackboneModel& bm)
			throw()
			:	ModelProcessor(bm),
				last_residue_(0),
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
			interpolation_steps_ = 9;
			tube_radius_ = 0.4;
			clear_();
		}

		bool AddBackboneModel::checkBuildNow_(const Residue& residue)
		{
			// if have already visited some residues and this reside was not in the same
			// chain, build the backbone for the last chain
			// this prevents building backbones between different chains
			if (last_residue_ == 0) 
			{
				return false;
			}

			Chain dummy_chain;

			const Chain* chain1 = residue.getAncestor(dummy_chain);
			const Chain* chain2 = last_residue_->getAncestor(dummy_chain);

			if (chain1 != chain2) return true;

			AtomConstIterator ait = residue.beginAtom();
			for (; +ait; ait++)
			{
				AtomBondConstIterator bit = ait->beginBond();
				for (; +bit; ++bit)
				{
					if ((*bit).getType() == Bond::TYPE__HYDROGEN) continue;
					if (bit->getPartner(*ait)->getParent() == last_residue_)
					{
						return false;
					}
				}
			}

			return true;
		}


		Processor::Result AddBackboneModel::operator() (Composite& composite)
		{
			const Residue* const residue = dynamic_cast<Residue*>(&composite);
			if (residue == 0 || !residue->isAminoAcid()) 
			{
				return Processor::CONTINUE;
			}

			collectAtoms_(*residue);
			return Processor::CONTINUE;
		}

		void AddBackboneModel::collectAtoms_(const Residue& residue)
		{
			String full_name = residue.getFullName();
			String 			name = residue.getName();

			AtomConstIterator it;

			// we collect P atoms in nucleotides
			if (name.size() == 1 && (name == "C" || name == "G" || name == "T" || name == "A"))
			{
				BALL_FOREACH_ATOM(residue, it)
				{
					if (it->getName() == "P")
					{
						splines_.push_back(SplinePoint((*it).getPosition(), &*it));
						return;
					}
				}

				return;
			}

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

			// collect only CA-Atoms
			BALL_FOREACH_ATOM(residue, it)
			{
				if (it->getName().hasSubstring("CA"))
				{
					splines_.push_back(SplinePoint((*it).getPosition(), &*it));
					return;
				}
			}
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


		// create the Representation up to spline point number pos
		void AddBackboneModel::createPart_(Position pos)
		{
			if (pos == 0) return;

			points_.clear();
			atoms_of_points_.clear();

			vector<SplinePoint>::iterator it = splines_.begin();
			for (Position i = 0; i < pos; i++)
			{
				it++;
			}

			// we have to sort the spline points, in case that we create a backbone for single residues
 			sort(splines_.begin(), it);

			createSplinePath_(pos);
			drawPart_(pos);

			it++;
			splines_.erase(splines_.begin(), it);
		}


		// computes the actual spline path through the given support points
		void AddBackboneModel::createSplinePath_(Position last)
		{
			// calculates for every splinepoint the tangential vector
			// first and last spline point have tangential vectors (0,0,0)
			// so lets forget about the first and the last spline point
			for (Position index = 1; index < last; ++index)
			{
				Vector3 tangent;

				tangent.x = (float) 0.8 * (splines_[index + 1].point.x - 
													 splines_[index - 1].point.x);

				tangent.y = (float) 0.8 * (splines_[index + 1].point.y -
													 splines_[index - 1].point.y);

				tangent.z = (float) 0.8 * (splines_[index + 1].point.z -
													 splines_[index - 1].point.z);

				splines_[index].tangent = tangent;
			}

			///////////////////////////////////////////////////////////////
			// create the spline
			///////////////////////////////////////////////////////////////
			for (Position spline_index = 0; spline_index < last; ++spline_index)
			{
				SplinePoint& a = splines_[spline_index];
				SplinePoint& b = splines_[spline_index + 1];
				
				// create a spline segment between two spline points a and b

				double time = 0.0;
				double step = (double)1 / (double)interpolation_steps_;

				for (Size index = 0; index < interpolation_steps_; ++index, time += step)
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

					new_vector.x = (float) (
												 (h1 * a.point.x) + 
												 (h2 * b.point.x) + 
												 (h3 * a.tangent.x) + 
												 (h4 * b.tangent.x));

					new_vector.y = (float) (
												 (h1 * a.point.y) + 
												 (h2 * b.point.y) + 
												 (h3 * a.tangent.y) + 
												 (h4 * b.tangent.y));

					new_vector.z = (float) (
												 (h1 * a.point.z) + 
												 (h2 * b.point.z) + 
												 (h3 * a.tangent.z) + 
												 (h4 * b.tangent.z));

					points_.push_back(new_vector);
					atoms_of_points_.push_back((index <= interpolation_steps_/2) ? a.atom : b.atom);
				} // for interpolation steps
			} // for all splines
		} // method


		void AddBackboneModel::drawPart_(Position last)
		{
			buildGraphicalRepresentation_(0, last, SecondaryStructure::UNKNOWN);
			last_build_ = last;
		}

		void AddBackboneModel::buildGraphicalRepresentation_(Position start, Position end, Position)
		{
			buildTube_(start, end);
		}

		void AddBackboneModel::buildTube_(Position start, Position end)
		{
			if (points_.size() == 0) return;
			if (points_.size() != atoms_of_points_.size())
			{
				Log.error() << "Error in " << __FILE__ << " " << __LINE__ << std::endl;
				return;
			}
			
			start *= interpolation_steps_;
			end   *= interpolation_steps_;
			end 	-= 1;

			// create sphere for the point
			Sphere* sphere = new Sphere;
			sphere->setRadius(tube_radius_);
			sphere->setPosition(points_[start]);
			sphere->setComposite(atoms_of_points_[start]);
 			geometric_objects_.push_back(sphere);

			// calculate the number of slides for the circle and the angle in between them
 			Size slides = (Size)(8.0 + drawing_precision_ * 8.0);
			Angle slides_angle = Angle(360.0 / slides, false);

			// direction vector of the two current spline points
			Vector3 dir = points_[start + 1] - points_[start];
					
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
			if (atoms_of_points_[start] != 0)
			{
				mesh->setComposite(atoms_of_points_[start]->getParent());
			}
			geometric_objects_.push_back(mesh);

			for (Position p = 0; p < slides; p++)
			{
				mesh->vertex.push_back(points_[start] + new_points[p]);
				mesh->normal.push_back(new_points[p]);
			}
			
			////////////////////////////////////////////////////////////
			// same data structures for faster access
			////////////////////////////////////////////////////////////
			Mesh::Triangle t;
			Size s_old = 0;  // start position of the last points in the meshs vertices
			Size s_new = 0;  // start position of the  new points in the meshs vertices
	
			//------------------------------------------------------>
			// iterate over all spline_points_
			//------------------------------------------------------>
			for (Position p = start; p < end; p++)
			{
				// faster access to the current spline point
				const Vector3& point = points_[p];
				
				// new direction vector: new point - last point
 				const Vector3 dir_new = points_[p + 1] - point;

				// new normal vector
				Vector3 r_new = r - (
				           (dir_new.x * r.x       + dir_new.y *       r.y + dir_new.z *       r.z)  /
				           (dir_new.x * dir_new.x + dir_new.y * dir_new.y + dir_new.z * dir_new.z) 
									 * dir_new);
				r_new.normalize();
				r_new *= tube_radius_;

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
				if (atoms_of_points_[p] != 0 &&
						mesh->getComposite() != atoms_of_points_[p]->getParent())
				{
					const Mesh* old_mesh = mesh;
					mesh = new Mesh();
					if (atoms_of_points_[p] != 0)
					{
						mesh->setComposite(atoms_of_points_[p]->getParent());
					}
					geometric_objects_.push_back(mesh);

					// insert the vertices and normals of the last points again into the new mesh
					for (Position point_pos = old_mesh->vertex.size() - slides;
							 					point_pos < old_mesh->vertex.size(); point_pos++)
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

				r = r_new;
			}

			Tube* tube = new Tube();
			tube->setVertex1(points_[end - 1]);
			tube->setVertex2(points_[end]);
			tube->setComposite(atoms_of_points_[end]);
			tube->setRadius(tube_radius_);
 			geometric_objects_.push_back(tube);
			
			// create a sphere as an end cap for the point
			sphere = new Sphere;
			sphere->setRadius(tube_radius_);
			sphere->setPosition(points_[end]);
 			sphere->setComposite(atoms_of_points_[end]);
 			geometric_objects_.push_back(sphere);
		}


		void AddBackboneModel::clear_()
		{
			splines_.clear();
			points_.clear();
			atoms_of_points_.clear();
			last_residue_ = 0;
			last_build_ = 0;
		}

		bool AddBackboneModel::createGeometricObjects()
			throw()
		{
			if (splines_.size() == 0) return true;

			Position pos = 0;
			vector<SplinePoint>::iterator sit = splines_.begin();
			for (; sit != splines_.end(); ++sit)
			{
				if ((*sit).atom == 0)
				{
					Log.error() << "Problem in " << __FILE__ << " " << __LINE__ << std::endl;
					return false;
				}

				const Residue* residue = dynamic_cast<const Residue*>((*sit).atom->getParent());
				if (residue == 0 || checkBuildNow_(*residue))
				{
					createPart_(pos - 1);
					sit = splines_.begin();
					pos = 0;
				}
				last_residue_ = residue;
				pos ++;
			}

			if (splines_.size() != 0)
			{
				createPart_(splines_.size() - 1);
			}

			clear_();
			return true;
		}

		Position AddBackboneModel::getType_(const Residue&)
		{
			return SecondaryStructure::UNKNOWN;
		}

	} // namespace VIEW
} // namespace BALL
