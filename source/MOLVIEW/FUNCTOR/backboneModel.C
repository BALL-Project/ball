// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: backboneModel.C,v 1.8 2003/06/10 21:30:30 amoll Exp $

#include <BALL/MOLVIEW/FUNCTOR/backboneModel.h>
#include <BALL/MOLVIEW/PRIMITIV/backbone.h>
#include <BALL/VIEW/PRIMITIV/sphere.h>
#include <BALL/VIEW/PRIMITIV/tube.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/residue.h>
#include <BALL/KERNEL/forEach.h>

using namespace std;

namespace BALL
{
	using VIEW::Tube;
	using VIEW::Sphere;

	namespace MOLVIEW
	{

		AddBackboneModel::AddBackboneModel()
			throw()
			: BaseModelProcessor(),
				last_parent_(0),
				root_(0),
				backbone_(0)
		{
		}

		AddBackboneModel::AddBackboneModel(const AddBackboneModel& add_Backbone)
			throw()
			:	BaseModelProcessor(add_Backbone),
				last_parent_(0),
				root_(0),
				backbone_(0)
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
			BaseModelProcessor::clear();
			spline_vector_.clear();
			last_parent_ = 0;
			backbone_ = 0;
			root_ = 0;
		}

		bool AddBackboneModel::start()
		{
			return BaseModelProcessor::start();
		}
				
		bool AddBackboneModel::finish()
		{
			if (root_ == 0 || 
					spline_vector_.size() == 0) return false;

			buildBackbone_();
			root_->appendChild(*backbone_);
			spline_vector_.clear();
			last_parent_ = 0;
			backbone_ = 0;
			root_ = 0;
			return true;
		}
				
		Processor::Result AddBackboneModel::operator () (Composite& composite)
		{
			if (root_ == 0)
			{
				root_ = &composite.getRoot();
			}

			if (!RTTI::isKindOf<Residue>(composite))  return Processor::CONTINUE;
			Residue& residue(*RTTI::castTo<Residue>(composite));
			if (!residue.isAminoAcid()) return Processor::CONTINUE;
			
			// if have already visited some residues and this reside was not in the same
			// chain, build the backbone for the last chain
			if (last_parent_ != 0 &&
					residue.getParent() != last_parent_ &&
					spline_vector_.size() > 0) 
			{
				buildBackbone_();
			}
			
			last_parent_ = residue.getParent();
			AtomIterator it;
			BALL_FOREACH_ATOM(residue, it)
			{
				// collect only CA-Atoms
				if ((it->getName().hasSubstring(String("CA"))) ||
				    (it->getName().hasSubstring(String("CH3")) &&
						(residue.getFullName() == "ACE" ||
						 residue.getFullName() == "ACE-N" ||
						 residue.getFullName() == "NME" ||
						 residue.getFullName() == "NME-C" )
						))
				{
					it->host(*getColorCalculator());
					SplinePoint spline_point;
					spline_point.setVector((*it).getPosition());
					spline_point.setColor(getColorCalculator()->getColor());
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

			BaseModelProcessor::dump(s, depth + 1);

			BALL_DUMP_STREAM_SUFFIX(s);
		}


		// creates the backbone representation from the given atom and atom colors list
		void AddBackboneModel::buildBackbone_()
			throw()
		{
			if (backbone_ == 0) createBackbone_();
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
					(new_vector, ((index <= max_step/2) ? a.getColor() : b.getColor()));
			}
		}


		// builds a graphical representation to this point
		void AddBackboneModel::buildGraphicalRepresentation_(const Vector3 &point, 
																												 const ColorRGBA &color)
		{
			if (have_start_point_)
			{
				if (point == last_point_)
				{
					return;
				}
				
				// build tube connection to the last point
				Tube* tube = backbone_->createTube_();
				tube->setRadius(0.4);
				tube->setVertex1(last_point_);
				tube->setVertex2(point);
				tube->setColor(color);
				backbone_->appendChild(*tube);
				tube->setProperty(GeometricObject::PROPERTY__OBJECT_STATIC);
				tube->PropertyManager::set(*this);
			}
			else
			{
				have_start_point_ = true;
			}

			last_point_ = point;

			// create sphere for the point
			Sphere* sphere = backbone_->createSphere_();
			sphere->setRadius(0.4);
			sphere->setVertex(point);
			sphere->setColor(color);
			backbone_->appendChild(*sphere);
			sphere->setProperty(GeometricObject::PROPERTY__OBJECT_STATIC);
			sphere->PropertyManager::set(*this);
			
			Line* line = backbone_->createLine_();
			line->PropertyManager::set(*this);
			line->setProperty(GeometricObject::PROPERTY__OBJECT_DYNAMIC);
			line->setVertex1(last_point_);
			line->setVertex2(point);
			line->setColor(color);
			backbone_->appendChild(*line);
		}

		Backbone* AddBackboneModel::createBackbone_()
			throw()
		{
			backbone_ = new Backbone;
			return backbone_;
		}

	} // namespace MOLVIEW
} // namespace BALL
