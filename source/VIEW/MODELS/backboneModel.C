// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: backboneModel.C,v 1.17.2.6 2004/12/21 17:21:15 amoll Exp $
//

#include <BALL/VIEW/MODELS/backboneModel.h>
#include <BALL/VIEW/PRIMITIVES/sphere.h>
#include <BALL/VIEW/PRIMITIVES/tube.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/residue.h>
#include <BALL/KERNEL/forEach.h>

#include <BALL/VIEW/PRIMITIVES/line.h>
#include <BALL/MATHS/analyticalGeometry.h>
#include <BALL/MATHS/matrix44.h>

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
			return atom_ < point.atom_;
		}


		AddBackboneModel::AddBackboneModel()
			throw()
			: ModelProcessor(),
				last_parent_(0),
				tube_radius_((float)0.4)
		{
		}

		AddBackboneModel::AddBackboneModel(const AddBackboneModel& add_Backbone)
			throw()
			:	ModelProcessor(add_Backbone),
				last_parent_(0),
				tube_radius_(add_Backbone.tube_radius_)
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

		void AddBackboneModel::collectAtoms_(const AtomContainer& ai)
			throw()
		{
			AtomIterator it;
			BALL_FOREACH_ATOM(*const_cast<AtomContainer*>(&ai), it)
			{
				const Residue& residue = *dynamic_cast<Residue*> (it->getParent());
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
			sort(spline_vector_.begin(), spline_vector_.end());
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
			Size max_step = 9;
			double time = 0.0;
			double step = (double)1 / (double)max_step;

			for (Size index = 0; index < max_step; ++index, time += step)
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
				atoms_of_spline_points_.push_back((index <= max_step/2) ? a.getAtom() : b.getAtom());
			}
		}


		// builds a graphical representation to this point
		void AddBackboneModel::buildGraphicalRepresentation_(Size start, Size end)
			throw(Exception::OutOfMemory)
		{
have_start_point_ = false;

			if (spline_points_.size() == 0) return;
			if (spline_points_.size() != atoms_of_spline_points_.size())
			{
				Log.error() << "Error in " << __FILE__ << __LINE__ << std::endl;
				return;
			}

			if (end == 0) end = spline_points_.size();

			if (!have_start_point_)
			{
				last_point_ = spline_points_[start];
			}
			else
			{
				/*
				// create sphere for the point
				Sphere* sphere = new Sphere;
				if (!sphere) throw Exception::OutOfMemory (__FILE__, __LINE__, sizeof(Sphere));

				sphere->setRadius(tube_radius_);
				sphere->setPosition(last_point_);
				sphere->setComposite(atoms_of_spline_points_[start]);
				geometric_objects_.push_back(sphere);
				*/
			}

 			float slides = 8.0 + drawing_precision_ * 8.0;
			vector<Vector3> points;
			Vector3 dir = spline_points_[start + 1] - last_point_;
			Vector3 r = dir % Vector3(1,0,0);
			if (Maths::isZero(r.getSquareLength())) 
			{ 
				r = dir % Vector3(0,1,0);
				if (Maths::isZero(r.getSquareLength())) 
				{
					r = dir % Vector3(0,0,1);
				}
			}
			r.normalize();
			r *= tube_radius_;

			Vector3 x(tube_radius_, 0, 0);
			
			Matrix4x4 m;
			m.setRotation(Angle(360.0 / (slides), false), Vector3(0,1,0));
			points.push_back(last_point_ + x);
			// initialise a first set of points in a circle around the start position
			for (float p = 0; p < slides; p++)
			{
				x = m * x;
				points.push_back(last_point_ + x);
			}
			// add also a dummy for closing of ring
			points.push_back(points[0]);

			for (float p = 0; p < points.size() - 1; p++)
			{
				Line* line = new Line();
				line->setVertex1(points[(Size)p]);
				line->setVertex2(points[(Size)p+1]);
				geometric_objects_.push_back(line);
			}

				
			// iterate over all spline_points_
			for (Position p = start + 1; p < end; p++)
			{
				Vector3 point 	= spline_points_[p];
				Vector3 dir_new = spline_points_[p + 1] - point;
				Vector3 r_new = dir_new % Vector3(1,0,0);
				if (Maths::isZero(r.getSquareLength())) 
				{ 
					r_new = dir_new % Vector3(0,1,0);
					if (Maths::isZero(r.getSquareLength())) 
					{
						r_new = dir_new % Vector3(0,0,1);
					}
				}
				r_new.normalize();
				r_new *= tube_radius_;

				Angle angle;
				GetAngle(r, r_new, angle);
				Matrix4x4 m;
				m.setRotation(angle, r);
				r = m * r;


				{
					Line* line = new Line();
					line->setVertex1(spline_points_[p+1]);
					line->setVertex2(point);
 					geometric_objects_.push_back(line);
				}

				/*
				if (!Maths::isZero(r_new.getSquareLength()))
				{
					r.normalize();
					r *= tube_radius_ * 10;
					r = r_new;
					dir = dir_new;
				}
				*/

				/*
				for (float p = 0; p < slides; p++)
				{
					Vector3 rv(r.x * cos((360.0/slides)*p), 
										 r.y * sin((360.0/slides)*p), 
										 0);

					Line* line = new Line();
					line->setVertex1(points[(Size)p]);
					points[(Size)p] = (point + rv);
					line->setVertex2(points[(Size)p]);
 					geometric_objects_.push_back(line);
				}
				*/

				Line* line = new Line();
				line->setVertex1(point);
				line->setVertex2(point + r * 10.0);
 				geometric_objects_.push_back(line);


				/*
				// build tube connection to the last point
				Tube* tube = new Tube;
				if (!tube) throw Exception::OutOfMemory (__FILE__, __LINE__, sizeof(Tube));
				
				tube->setRadius(tube_radius_);
				tube->setVertex1(last_point_);
				tube->setVertex2(spline_points_[p]);
				tube->setComposite(atoms_of_spline_points_[p]);
				geometric_objects_.push_back(tube);

				// create sphere for the point
				Sphere* sphere = new Sphere;
				if (!sphere) throw Exception::OutOfMemory (__FILE__, __LINE__, sizeof(Sphere));

				sphere->setRadius(tube_radius_);
				sphere->setPosition(spline_points_[p]);
				sphere->setComposite(atoms_of_spline_points_[p]);
				geometric_objects_.push_back(sphere);
				last_point_ = spline_points_[p];
				*/
			}

			have_start_point_ = true;
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
