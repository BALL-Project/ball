// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: representation.C,v 1.2 2003/08/26 09:18:42 oliver Exp $

#include <BALL/VIEW/GUI/KERNEL/representation.h>
#include <BALL/VIEW/GUI/FUNCTOR/modelProcessor.h>
#include <BALL/VIEW/KERNEL/geometricObject.h>
#include <BALL/VIEW/COMMON/global.h>
#include <BALL/MATHS/common.h>

namespace BALL
{
	namespace VIEW
	{
		Representation::Representation()
			throw()
				: PropertyManager(),
					drawing_mode_(DRAWING_MODE_SOLID),
					drawing_precision_(DRAWING_PRECISION_HIGH),
					model_type_(0),
					model_processor_(0),
					geometric_objects_(),
					composites_()
		{
		}

		Representation::Representation(const Representation& rp)
			throw()
				: PropertyManager(rp),
					drawing_mode_(rp.drawing_mode_),
					drawing_precision_(rp.drawing_precision_),
					model_type_(rp.model_type_),
					model_processor_(new ModelProcessor(*rp.model_processor_)),
					geometric_objects_(),
					composites_(rp.composites_)
		{
		}


		Representation::Representation(Index model_type,
																	 Index drawing_precision,
																	 Index drawing_mode)
			throw()
				: PropertyManager(),
					drawing_mode_(drawing_mode),
					drawing_precision_(drawing_precision),
					model_type_(model_type),
					model_processor_(0),
					geometric_objects_(),
					composites_()
		{
		}


		Representation::Representation(const HashSet<const Composite*>& composites, 
																	 ModelProcessor* model_processor)
			throw()
			:	PropertyManager(),
				drawing_mode_(DRAWING_MODE_SOLID),
				drawing_precision_(DRAWING_PRECISION_HIGH),
				model_processor_(model_processor),
				geometric_objects_(),
				composites_(composites)
		{
			// drawing properties form processors
		}

				
		const Representation& Representation::operator = (const Representation& representation)
			throw()
		{
			drawing_mode_= representation.drawing_mode_;
			drawing_precision_= representation.drawing_precision_;
			model_type_ = representation.model_type_;

			PropertyManager::operator = (representation);

			if (representation.model_processor_ != 0)
			{
				model_processor_ = new ModelProcessor(*representation.model_processor_);
			}
			else
			{
				model_processor_ = 0;
			}

			GeometricObjectList::ConstIterator it = representation.geometric_objects_.begin();
			for (;it != representation.geometric_objects_.end(); it++)
			{
				GeometricObject* object = new GeometricObject(**it);
				geometric_objects_.push_back(object);
			}

			CompositeSet::ConstIterator comp_it = representation.composites_.begin();
			for (;comp_it != representation.composites_.end(); comp_it++)
			{
				composites_.insert(*comp_it);
			}

			return *this;
		}


		Representation::~Representation()
			throw()
		{
			clear();
		}

		
		void Representation::clear()
			throw()
		{
			clearGeometricObjects_();
			composites_.clear();

			if (model_processor_  != 0) delete model_processor_;
			model_processor_ 	= 0;

			drawing_mode_= DRAWING_MODE_SOLID;
			drawing_precision_= DRAWING_PRECISION_HIGH;
			model_type_ = 0;
		}

		
		void Representation::clearGeometricObjects_()
			throw()
		{
			List<GeometricObject*>::Iterator it = geometric_objects_.begin();
			for (; it != geometric_objects_.end(); it++)
			{
				delete *it;
			}
			geometric_objects_.clear();
		}
					

		void Representation::dump(std::ostream& s, Size depth) const
			throw()
		{
			BALL_DUMP_STREAM_PREFIX(s);

			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_DEPTH(s, depth);
			s << "drawing mode: " << drawing_mode_<< std::endl;
			BALL_DUMP_DEPTH(s, depth);
			s << "drawing precision: " << drawing_precision_<< std::endl;
			BALL_DUMP_DEPTH(s, depth);
			s << "model type : " << model_type_ << std::endl;
			BALL_DUMP_DEPTH(s, depth);
			s << "number of primitives: " << geometric_objects_.size() << std::endl;
			BALL_DUMP_DEPTH(s, depth);
			s << "number of composites: " << composites_.size() << std::endl;
			BALL_DUMP_DEPTH(s, depth);
			s << "model processor: " << model_processor_ << std::endl;
			BALL_DUMP_DEPTH(s, depth);
			s << "PropertyManager: " << std::endl;

			PropertyManager::dump(s, depth + 1);

			BALL_DUMP_STREAM_SUFFIX(s);     
		}

		
		bool Representation::isValid() const
			throw()
		{
			if (drawing_precision_  < 0 || drawing_precision_ > BALL_VIEW_MAXIMAL_DRAWING_PRECISION ||
					drawing_mode_ 			< 0 || drawing_mode_ > BALL_VIEW_MAXIMAL_DRAWING_MODE)
			{
				return false;
			}

#ifdef BALL_VIEW_DEBUG
			GeometricObjectList::ConstIterator it = geometric_objects_.begin();
			for (; it != geometric_objects_.end(); it++)
			{
				if (!(*it)->isValid()) return false;
			}
#endif

			if (model_processor_ != 0 && !model_processor_->isValid()) return false; 

			return true;
		}

		
		void Representation::update() 
			throw()
		{
			// if no modelprocessor was given, there can only exist 
			// handmade GeometricObjects, which dont need to be updated
			if (model_processor_ != 0) 
			{
				clearGeometricObjects_();
				model_processor_->getGeometricObjects().clear();
				
				CompositeSet::Iterator it = composites_.begin();
				if (model_processor_ != 0)
				{
					for (; it!= composites_.end(); it++)
					{
						(const_cast<Composite*>(*it))->apply(*model_processor_);
					}
				}
				geometric_objects_ = model_processor_->getGeometricObjects();
			}
		}

		String Representation::getModelName() const
			throw()
		{
			switch (model_type_)
			{
				case 0:
					return "Line";
				case 1:
					return "Stick";
				case 2:
					return "BallandStick";
				case 3:
					return "VDW";
				case 4:
					return "Surface";
				case 5:
					return "Backbone";
				case 6:
					return "Cartoon";
				case 7:
					return "Label";
			}
			return "Unknown";
		}

		String Representation::getProperties() const
			throw()
		{
			return String(composites_.size()) + " C, " + String(geometric_objects_.size()) + " P";
		}

		void Representation::setModelProcessor(ModelProcessor* processor)
			throw() 
		{ 
			model_processor_ = processor;
			if (model_processor_ != 0) model_processor_->setDrawingPrecision(drawing_precision_);
		}

		void Representation::setDrawingPrecision(Index precision)
			throw() 
		{
			drawing_precision_ = precision;
			if (model_processor_ != 0) model_processor_->setDrawingPrecision(drawing_precision_);
		}

	} // namespace VIEW
} // namespace BALL
