// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: backboneModel.C,v 1.9 2003/12/15 01:10:15 amoll Exp $
//

#include <BALL/VIEW/MODELS/backboneModel.h>
#include <BALL/VIEW/PRIMITIVES/sphere.h>
#include <BALL/VIEW/PRIMITIVES/tube.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/residue.h>
#include <BALL/KERNEL/forEach.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

		AddBackboneModel::SplinePoint::SplinePoint(const Vector3& point, const Atom* atom)
			: point_(point),
				tangent_(),
				atom_(atom)
		{}

		AddBackboneModel::AddBackboneModel()
			throw()
			: ModelProcessor(),
				last_parent_(0),
				tube_radius_(0.4)
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
				Log.error() << "Destructing object " << (void *)this 
										<< " of class " << RTTI::getName<AddBackboneModel>() << endl;
			#endif 
		}

		void AddBackboneModel::clear()
			throw()
		{
			ModelProcessor::clear();
			spline_vector_.clear();
			last_parent_ = 0;
			tube_radius_ = 0.4;
		}

		bool AddBackboneModel::finish()
		{
			if (spline_vector_.size() == 0) return false;

			createBackbone_();
			return true;
		}

		Processor::Result AddBackboneModel::operator () (Composite& composite)
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
			}
			
			last_parent_ = residue.getParent()->getParent();
			AtomIterator it;
			BALL_FOREACH_ATOM(residue, it)
			{
				// collect only CA-Atoms and CH3 atoms in ACE and NME
				if ((it->getName().hasSubstring("CA")) ||
				    (it->getName().hasSubstring("CH3") &&
						(residue.getFullName() == "ACE" 	||
						 residue.getFullName() == "ACE-N" ||
						 residue.getFullName() == "NME" 	||
						 residue.getFullName() == "NME-C" )
						))
				{
					SplinePoint spline_point((*it).getPosition(), &*it);
					spline_vector_.push_back(spline_point);
				}
			}

			return Processor::CONTINUE;
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
			calculateTangentialVectors_();
			createSplinePath_();
			spline_vector_.clear();
		}


		// calculates to every splinepoint the tangential vector
		void AddBackboneModel::calculateTangentialVectors_()
		{
			// first and last spline point have tangential vectors (0,0,0)
			// so lets forget about the first and the last spline point
			for (Position index = 1; index < spline_vector_.size() - 1; ++index)
			{
				Vector3 tangent;

				tangent.x = 0.8 * (spline_vector_[index + 1].getVector().x - 
													 spline_vector_[index - 1].getVector().x);

				tangent.y = 0.8 * (spline_vector_[index + 1].getVector().y -
													 spline_vector_[index - 1].getVector().y);

				tangent.z = 0.8 * (spline_vector_[index + 1].getVector().z -
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

				// build the objects
				buildGraphicalRepresentation_
					(new_vector, ((index <= max_step/2) ? a.getAtom() : b.getAtom()));
			}
		}


		// builds a graphical representation to this point
		void AddBackboneModel::buildGraphicalRepresentation_(const Vector3 &point, 
																												 const Atom* atom)
		{
			if (have_start_point_)
			{
				if (point == last_point_) return;
				
				// build tube connection to the last point
				Tube* tube = new Tube;
				if (!tube) throw Exception::OutOfMemory (__FILE__, __LINE__, sizeof(Tube));
				
				tube->setRadius(tube_radius_);
				tube->setVertex1(last_point_);
				tube->setVertex2(point);
				tube->setComposite(atom);
				geometric_objects_.push_back(tube);
			}

			have_start_point_ = true;
			last_point_ = point;

			// create sphere for the point
			Sphere* sphere = new Sphere;
			if (!sphere) throw Exception::OutOfMemory (__FILE__, __LINE__, sizeof(Sphere));

			sphere->setRadius(tube_radius_);
			sphere->setPosition(point);
			sphere->setComposite(atom);
			geometric_objects_.push_back(sphere);
		}

		
	} // namespace VIEW
} // namespace BALL
