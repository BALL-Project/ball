// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: representation.C,v 1.50 2004/11/09 21:35:24 amoll Exp $
//

#include <BALL/VIEW/KERNEL/representation.h>
#include <BALL/VIEW/MODELS/modelProcessor.h>
#include <BALL/VIEW/MODELS/colorProcessor.h>
#include <BALL/VIEW/KERNEL/geometricObject.h>

#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/message.h>

#include <BALL/KERNEL/atom.h>

#include <BALL/SYSTEM/timer.h>

#include <qapplication.h>

// #define BALL_BENCHMARKING

namespace BALL
{
	namespace VIEW
	{
		Representation::Representation()
			throw()
				: PropertyManager(),
					drawing_mode_(DRAWING_MODE_SOLID),
					drawing_precision_(DRAWING_PRECISION_HIGH),
					surface_drawing_precision_(-1),
					model_type_(MODEL_UNKNOWN),
					coloring_method_(COLORING_UNKNOWN),
					transparency_(0),
					model_processor_(0),
					color_processor_(0),
					geometric_objects_(),
					composites_(),
					rebuild_(true),
					hidden_(false)
		{
		}

					
		Representation::Representation(const Representation& rp)
			throw()
				: PropertyManager(rp),
					drawing_mode_(rp.drawing_mode_),
					drawing_precision_(rp.drawing_precision_),
					surface_drawing_precision_(rp.drawing_precision_),
					model_type_(rp.model_type_),
					coloring_method_(rp.coloring_method_),
					transparency_(rp.transparency_),
					model_processor_(0),
					color_processor_(0),
					geometric_objects_(),
					composites_(rp.composites_),
					rebuild_(rp.rebuild_),
					hidden_(rp.hidden_)
		{
			if (rp.model_processor_ != 0)
			{
				model_processor_ = new ModelProcessor(*rp.model_processor_);
			}

			if (rp.color_processor_ != 0)
			{
				color_processor_ = new ColorProcessor(*rp.color_processor_);
			}

			GeometricObjectList::ConstIterator it = rp.geometric_objects_.begin();
			for (;it != rp.geometric_objects_.end(); it++)
			{
				GeometricObject* object = new GeometricObject(**it);
				geometric_objects_.push_back(object);
			}
		}


		Representation::Representation(ModelType model_type,
																	 DrawingPrecision drawing_precision,
																	 DrawingMode drawing_mode)
			throw()
				: PropertyManager(),
					drawing_mode_(drawing_mode),
					drawing_precision_(drawing_precision),
					surface_drawing_precision_(-1),
					model_type_(model_type),
					transparency_(0),
					model_processor_(0),
					color_processor_(0),
					geometric_objects_(),
					composites_(),
					rebuild_(true),
					hidden_(false)
		{
		}


		Representation::Representation(const HashSet<const Composite*>& composites, 
																	 ModelProcessor* model_processor)
			throw()
			:	PropertyManager(),
				drawing_mode_(DRAWING_MODE_SOLID),
				drawing_precision_(DRAWING_PRECISION_HIGH),
				surface_drawing_precision_(-1),
				transparency_(0),
				model_processor_(model_processor),
				color_processor_(0),
				geometric_objects_(),
				composites_(composites),
				rebuild_(true),
				hidden_(false)
		{
		}

				
		const Representation& Representation::operator = (const Representation& representation)
			throw()
		{
			clear();

			drawing_mode_= representation.drawing_mode_;
			drawing_precision_= representation.drawing_precision_;
			model_type_ = representation.model_type_;
			coloring_method_ = representation.coloring_method_;
			transparency_ = representation.transparency_;
			surface_drawing_precision_ = representation.surface_drawing_precision_;

			PropertyManager::operator = (representation);

			if (representation.model_processor_ != 0)
			{
				model_processor_ = new ModelProcessor(*representation.model_processor_);
			}
			else
			{
				model_processor_ = 0;
			}

			if (representation.color_processor_ != 0)
			{
				color_processor_ = new ColorProcessor(*representation.color_processor_);
				color_processor_->setTransparency(transparency_);
			}
			else
			{
				color_processor_ = 0;
			}

			GeometricObjectList::ConstIterator it = representation.geometric_objects_.begin();
			for (;it != representation.geometric_objects_.end(); it++)
			{
				GeometricObject* object = new GeometricObject(**it);
				geometric_objects_.push_back(object);
			}

			composites_ = representation.composites_;

			rebuild_ = true;
			hidden_ = representation.hidden_;

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
			clearGeometricObjects();
			composites_.clear();

			if (model_processor_  != 0) delete model_processor_;
			if (color_processor_  != 0) delete color_processor_;
			model_processor_ 	= 0;
			color_processor_ 	= 0;

			drawing_mode_= DRAWING_MODE_SOLID;
			drawing_precision_= DRAWING_PRECISION_HIGH;
			model_type_ = MODEL_UNKNOWN;
			coloring_method_ = COLORING_UNKNOWN;
			transparency_ = 0;
			surface_drawing_precision_ = -1;

			rebuild_ = true;
			hidden_ = false;
		}

		
		void Representation::clearGeometricObjects()
			throw()
		{
			List<GeometricObject*>::Iterator it = geometric_objects_.begin();
			for (; it != geometric_objects_.end(); it++)
			{
				delete *it;
			}
			geometric_objects_.clear();

			if (model_processor_ != 0)
			{
				model_processor_->getGeometricObjects().clear();
			}
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
			s << "coloring type : " << coloring_method_ << std::endl;
			BALL_DUMP_DEPTH(s, depth);
			s << "number of primitives: " << geometric_objects_.size() << std::endl;
			BALL_DUMP_DEPTH(s, depth);
			s << "number of composites: " << composites_.size() << std::endl;
			BALL_DUMP_DEPTH(s, depth);
			s << "model processor: " << model_processor_ << std::endl;
			BALL_DUMP_DEPTH(s, depth);
			s << "hidden: " << hidden_ << std::endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "PropertyManager: " << std::endl;
			PropertyManager::dump(s, depth + 1);

			BALL_DUMP_STREAM_SUFFIX(s);     
		}

		
		bool Representation::isValid() const
			throw()
		{
			if (drawing_precision_  < 0 || drawing_precision_ > BALL_VIEW_MAXIMAL_DRAWING_PRECISION ||
					drawing_mode_ 			< 0 || drawing_mode_ > BALL_VIEW_MAXIMAL_DRAWING_MODE ||
					transparency_ 			> 255)
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

		void Representation::update(bool rebuild)
			throw()
		{
			rebuild_ = rebuild;

#ifdef BALL_QT_HAS_THREADS
			MainControl* mc = MainControl::getInstance(0);
			if (mc != 0)
			{
//   				MainControl* mc = dynamic_cast<MainControl*>(qApp->mainWidget());
				mc->getPrimitiveManager().update_(*this);
				return;
			}
#endif

			update_();
		}
		
		void Representation::update_() 
			throw()
		{
			needs_update_ = false;

#ifdef BALL_BENCHMARKING
	Timer t;
	t.start();
#endif
			// if no ModelProcessor was given, there can only exist 
			// handmade GeometricObjects, which dont need to be updated
			if (model_processor_ != 0 && rebuild_)
			{
				clearGeometricObjects();
				model_processor_->getGeometricObjects().clear();
				model_processor_->clearComposites();
				
				CompositeSet::Iterator it = composites_.begin();
				for (; it!= composites_.end(); it++)
				{
					(const_cast<Composite*>(*it))->apply(*model_processor_);
				}
				model_processor_->createGeometricObjects();
				geometric_objects_ = model_processor_->getGeometricObjects();
				model_build_time_ = PreciseTime::now();
			}

			if (color_processor_ != 0) 
			{
				// make sure, that the atom grid is recomputed for meshes
				if (rebuild_) color_processor_->setComposites(&composites_);
				color_processor_->setTransparency(transparency_);
				color_processor_->setModelType(model_type_);
				geometric_objects_.apply(*color_processor_);
			}

#ifdef BALL_BENCHMARKING
			Log.info() << "Calculating Representation time: " << t.getCPUTime() << std::endl;
			t.stop();
#endif
		}
		

		String Representation::getModelName() const
			throw()
		{
			return VIEW::getModelName(model_type_);
		}

		
		String Representation::getColoringName() const
			throw()
		{
			return VIEW::getColoringName(coloring_method_);
		}


		String Representation::getProperties() const
			throw()
		{
			return String(composites_.size()) + " C, " + String(geometric_objects_.size()) + " P";
		}

		void Representation::setModelProcessor(ModelProcessor* processor)
			throw() 
		{ 
			if (model_processor_ != 0)
			{
				delete model_processor_;
			}
			model_processor_ = processor;
			
			if (model_processor_ != 0) 
			{
				model_processor_->setDrawingPrecision(drawing_precision_);
				model_processor_->setSurfaceDrawingPrecision(surface_drawing_precision_);
			}
		}


		void Representation::setColorProcessor(ColorProcessor* processor)
			throw() 
		{ 
			if (color_processor_ != 0)
			{
				delete color_processor_;
			}
			color_processor_ = processor;
			
			if (color_processor_ != 0)
			{
				color_processor_->setComposites(&composites_);
				color_processor_->setTransparency(transparency_);
			}
		}

		
		void Representation::setDrawingPrecision(DrawingPrecision precision)
			throw() 
		{
			drawing_precision_ = precision;
			if (model_processor_ != 0) model_processor_->setDrawingPrecision(drawing_precision_);
		}

		
		void Representation::setSurfaceDrawingPrecision(float precision)
			throw() 
		{
			surface_drawing_precision_ = precision;
			if (model_processor_ != 0) model_processor_->setSurfaceDrawingPrecision(surface_drawing_precision_);
		}

		
		void Representation::setTransparency(Size value)
			throw()
		{
			transparency_ = value;
			if (transparency_ > 255)
			{
				transparency_ = 255;
			}

			if (color_processor_ != 0)
			{
				color_processor_->setTransparency(transparency_);
			}
		}

		bool Representation::needsUpdate() const
			throw()
		{
			if (needs_update_ || 
					getModelBuildTime() < Atom::getAttributesModificationTime())
			{
				return true;
			}

			CompositeSet::ConstIterator it = composites_.begin();
			for (;it != composites_.end(); it++)
			{
				if (getModelBuildTime() < (*it)->getModificationTime()) return true;
			}

			return false;
		}

		String Representation::toString() const
			throw()
		{
			String result;
			if (getModelType() == MODEL_CLIPPING_PLANE)
			{
				result+= "CP:";
				result+= String(getProperty("AX").getFloat()) + " ";
				result+= String(getProperty("BY").getFloat()) + " ";
				result+= String(getProperty("CZ").getFloat()) + " ";
				result+= String(getProperty("D").getFloat());
				return result;
			}

			result += String(model_type_) + " ";
			result += String(drawing_mode_) + " ";
			result += String(drawing_precision_) + " ";
			result += String(surface_drawing_precision_) + " ";
			result += String(coloring_method_) + " ";
			result += String(transparency_) + " ";

			if (composites_.size() == 0)
			{
				result += "[]";
				return result;
			}

			result += "[";

			const Composite& root = (*composites_.begin())->getRoot();
			HashMap<const Composite*, Position> composite_to_index;
			collectRecursive_(root, composite_to_index);

			CompositesConstIterator it = begin();
			for (; it != end(); it++)
			{
				if (composite_to_index.has(*it))
				{
					result += String(composite_to_index[*it]) + ",";
				}
			}

			result.trimRight(",");
			result += "]";

			if (color_processor_ != 0)
			{
				result += "|";
				String temp;
				result += color_processor_->getDefaultColor();
				result += "|";
			}

			if (isHidden())
			{
				result += "H";
			}

			return result;
		}


		void Representation::collectRecursive_(const Composite& c, 
									HashMap<const Composite*, Position>& hashmap) const
			throw()
		{
			hashmap[&c] = hashmap.size();
			for (Position p = 0; p < c.getDegree(); p++)
			{
				collectRecursive_(*c.getChild(p), hashmap);
			}
		}

	} // namespace VIEW
} // namespace BALL
