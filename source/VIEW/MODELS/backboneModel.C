// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: backboneModel.C,v 1.17.2.15 2004/12/25 18:45:23 amoll Exp $
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
				
				// create sphere for the point
				start++;
			}

			Sphere* sphere = new Sphere;
			if (!sphere) throw Exception::OutOfMemory (__FILE__, __LINE__, sizeof(Sphere));
			sphere->setRadius(tube_radius_);
			sphere->setPosition(last_point_);
			sphere->setComposite(atoms_of_spline_points_[start]);
			geometric_objects_.push_back(sphere);

			// calculate the number of slides for the circle and the angle in between them
 			float slides = 8.0 + drawing_precision_ * 8.0;
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
			Vector3 n = Vector3(0,1,0);
			// normal vector to direction vector dir, with length of radius
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
			vector<Vector3> points1, points2;
			Matrix4x4 m;
			m.setRotation(slides_angle, n % r);
			Vector3 x = r;
			points1.push_back(x);
			for (Position p = 0; p < slides - 1; p++)
			{
				x = m * x;
				points1.push_back(x);
			}
			// add also a dummy for closing of ring
			points1.push_back(points1[0]);

			points2.resize(points1.size());

			////////////////////////////////////////////////////////////
			// same data structures for faster access
			Mesh::Triangle t;
			vector<Vector3>*  new_points = &points2;
			vector<Vector3>* last_points = &points1;
			vector<Vector3>* dummy = 0;
				
			//------------------------------------------------------>
			// iterate over all spline_points_
			for (Position p = start; p < end -1; p++)
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
				m.setRotation(slides_angle, dir_new);
				x = r_new;
				(*new_points)[0] = x;
				const Position middle = (Position)(slides / 2.0);
				for (Position i= 1; i < middle; i++)
				{
					x = m * x;
					(*new_points)[i] = x;
				}

				// second part of points can be calculated by negating first points
				for (Position i = middle; i < slides; i++)
				{
					(*new_points)[i] = - (*new_points)[i - middle];
				}

				// dont forget the dummy for closing the ring
 				(*new_points)[new_points->size() - 1] = (*new_points)[0];
				////////////////////////////////////////////////////////////

				// create a new mesh with the points and triangles
				Mesh* mesh = new Mesh();
				for (Position point_pos = 0; point_pos < slides; point_pos++)
				{
					mesh->vertex.push_back(last_point_ + (*last_points)[point_pos]);
					mesh->vertex.push_back(			point  + (* new_points)[point_pos]);
					mesh->vertex.push_back(last_point_ + (*last_points)[point_pos + 1]);
					mesh->vertex.push_back(     point  + (* new_points)[point_pos + 1]);

					mesh->normal.push_back((*last_points)[point_pos]);
					mesh->normal.push_back((*new_points)[point_pos]);
					mesh->normal.push_back((*last_points)[point_pos + 1]);
					mesh->normal.push_back((*new_points)[point_pos + 1]);

					const Size s = mesh->vertex.size() -1;
					t.v1 = s - 3;
					t.v2 = s - 1;
					t.v3 = s;
					mesh->triangle.push_back(t);

					t.v1 = s;
					t.v2 = s - 2;
					t.v3 = s - 3;
					mesh->triangle.push_back(t);
				}

				mesh->setComposite(atoms_of_spline_points_[p]);
				geometric_objects_.push_back(mesh);

				// swap between the two point vectors
				dummy = new_points;
				new_points  = last_points;
				last_points = dummy;

				r = r_new;
				last_point_ = point;
			}

			have_start_point_ = true;
			
			// create sphere for the point
			sphere = new Sphere;
			if (!sphere) throw Exception::OutOfMemory (__FILE__, __LINE__, sizeof(Sphere));
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
