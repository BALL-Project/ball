// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: backboneModel.C,v 1.17.2.26 2004/12/28 15:14:54 amoll Exp $
//

#include <BALL/VIEW/MODELS/backboneModel.h>
#include <BALL/VIEW/PRIMITIVES/sphere.h>
#include <BALL/VIEW/PRIMITIVES/tube.h>
#include <BALL/VIEW/PRIMITIVES/mesh.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/residue.h>
#include <BALL/KERNEL/forEach.h>

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
			: point_(),
				tangent_(),
				atom_(0)
		{
		}


		AddBackboneModel::SplinePoint::SplinePoint(const Vector3& point, const Atom* atom)
			: point_(point),
				tangent_(),
				atom_(atom)
		{
		}

		bool AddBackboneModel::SplinePoint::operator < (const SplinePoint::SplinePoint& point) const
			throw()
		{
			return ((Residue*)       atom_->getParent())->getID() <
						 ((Residue*) point.atom_->getParent())->getID();
		}


		AddBackboneModel::AddBackboneModel()
			throw()
			: ModelProcessor(),
				last_parent_(0),
				tube_radius_((float)0.4),
				interpolation_steps_(9)
		{
		}

		AddBackboneModel::AddBackboneModel(const AddBackboneModel& bm)
			throw()
			:	ModelProcessor(bm),
				last_parent_(0),
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

		Processor::Result AddBackboneModel::operator() (Composite& composite)
		{
			if (!RTTI::isKindOf<Residue>(composite))  return Processor::CONTINUE;
			Residue& residue(*RTTI::castTo<Residue>(composite));
			if (!residue.isAminoAcid()) return Processor::CONTINUE;
			
			// if have already visited some residues and this reside was not in the same
			// chain, build the backbone for the last chain
			// this prevents building backbones between different chains
			if (last_parent_ != 0 &&
					residue.getParent()->getParent() != last_parent_ &&
					spline_vector_.size() > 0) 
			{
				createBackbone_();
				clear_();
			}
			
			last_parent_ = residue.getParent()->getParent();
			collectAtoms_(residue);

			return Processor::CONTINUE;
		}

		void AddBackboneModel::collectAtoms_(Residue& residue)
			throw()
		{
			AtomIterator it;
			BALL_FOREACH_ATOM(residue, it)
			{
				// collect only CA-Atoms and CH3 atoms in ACE and NME
				if (((it->getName().hasSubstring("CA")) ||
				    (it->getName().hasSubstring("CH3") &&
						(residue.getFullName() == "ACE" 	||
						 residue.getFullName() == "ACE-N" ||
						 residue.getFullName() == "NME" 	||
						 residue.getFullName() == "NME-C" )
						)) || (
						// or we collect P atoms in nucleotides
						residue.getName().size() == 1 &&
						it->getName() == "P" 					&&
						(
						 residue.getName() == "C" ||
						 residue.getName() == "G" ||
						 residue.getName() == "T" ||
						 residue.getName() == "A"
						)))
				{
					SplinePoint spline_point((*it).getPosition(), &*it);
					spline_vector_.push_back(spline_point);
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


		// creates the backbone representation from the given atom 
		void AddBackboneModel::createBackbone_()
			throw()
		{
			have_start_point_ = false;
//   			sort(spline_vector_.begin(), spline_vector_.end());

			calculateTangentialVectors_();
			createSplinePath_();
			buildGraphicalRepresentation_();
		}


		// calculates for every splinepoint the tangential vector
		void AddBackboneModel::calculateTangentialVectors_()
		{
			// first and last spline point have tangential vectors (0,0,0)
			// so lets forget about the first and the last spline point
			for (Position index = 1; index < spline_vector_.size() - 1; ++index)
			{
				Vector3 tangent;

				tangent.x = (float) 0.8 * (spline_vector_[index + 1].getVector().x - 
													 spline_vector_[index - 1].getVector().x);

				tangent.y = (float) 0.8 * (spline_vector_[index + 1].getVector().y -
													 spline_vector_[index - 1].getVector().y);

				tangent.z = (float) 0.8 * (spline_vector_[index + 1].getVector().z -
													 spline_vector_[index - 1].getVector().z);

				spline_vector_[index].setTangentialVector(tangent);
			}
		}
		

		// computes the actual spline path through the given support points
		// in the splinepoint array
		void AddBackboneModel::createSplinePath_()
		{
			// creates the spline
			for (Position index = 0; index < spline_vector_.size() - 1; ++index)
			{
				createSplineSegment_(spline_vector_[index], spline_vector_[index + 1]);
			}
		}


		// create a spline segment between two spline points a and b
		void AddBackboneModel::createSplineSegment_(const SplinePoint &a, const SplinePoint &b)
		{
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
											 (h1 * a.getVector().x) + 
											 (h2 * b.getVector().x) + 
											 (h3 * a.getTangentialVector().x) + 
											 (h4 * b.getTangentialVector().x));

				new_vector.y = (float) (
											 (h1 * a.getVector().y) + 
											 (h2 * b.getVector().y) + 
											 (h3 * a.getTangentialVector().y) + 
											 (h4 * b.getTangentialVector().y));

				new_vector.z = (float) (
											 (h1 * a.getVector().z) + 
											 (h2 * b.getVector().z) + 
											 (h3 * a.getTangentialVector().z) + 
											 (h4 * b.getTangentialVector().z));

				spline_points_.push_back(new_vector);
				atoms_of_spline_points_.push_back((index <= interpolation_steps_/2) ? a.getAtom() : b.getAtom());
			}
		}


		// builds a graphical representation to this point
		void AddBackboneModel::buildGraphicalRepresentation_(Size start, Size end)
			throw(Exception::OutOfMemory)
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
			new_points.resize(slides + 1);
			Vector3 x = r;
			new_points.push_back(x);
			new_points[middle] = -x;
			new_points[slides] = x; // dummy for closing of ring

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
			mesh->setComposite(atoms_of_spline_points_[start]->getParent());
			geometric_objects_.push_back(mesh);

			for (Position p = 0; p < slides + 1; p++)
			{
				mesh->vertex.push_back(last_point_ + new_points[p]);
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
				r_new *= tube_radius_;

				////////////////////////////////////////////////////////////
				// rotate all points of the circle according to new normal
				////////////////////////////////////////////////////////////
				m.setRotation(slides_angle, dir_new);
				x = r_new;
				new_points[0] = x;
				new_points[slides] = x; // dummy

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
				if (mesh->getComposite() != atoms_of_spline_points_[p]->getParent())
				{
					const Mesh* old_mesh = mesh;
					mesh = new Mesh();
					mesh->setComposite(atoms_of_spline_points_[p]->getParent());
					geometric_objects_.push_back(mesh);

					// insert the vertices and normals of the last points again into the new mesh
					for (Position point_pos = s_new - slides - 1;
							 					point_pos < old_mesh->vertex.size() - 1; point_pos++)
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
				for (Position point_pos = 0; point_pos < slides; point_pos++)
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


		void AddBackboneModel::clear_()
			throw()
		{
			spline_vector_.clear();
			last_parent_ = 0;
			have_start_point_ = false;

			spline_points_.clear();
			atoms_of_spline_points_.clear();
		}

		bool AddBackboneModel::createGeometricObjects()
			throw()
		{
			if (spline_vector_.size() == 0) return true;
			createBackbone_();
			clear_();
			return true;
		}

	} // namespace VIEW
} // namespace BALL
