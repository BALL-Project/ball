// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: representation.C,v 1.31 2004/04/18 17:15:47 amoll Exp $

#include <BALL/VIEW/KERNEL/representation.h>
#include <BALL/VIEW/MODELS/modelProcessor.h>
#include <BALL/VIEW/MODELS/colorProcessor.h>
#include <BALL/VIEW/KERNEL/geometricObject.h>

#include <BALL/VIEW/KERNEL/threads.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/message.h>

#include <BALL/KERNEL/atom.h>

#include <qapplication.h>

namespace BALL
{
	namespace VIEW
	{

#ifdef BALL_QT_HAS_THREADS
		UpdateRepresentationThread* Representation::thread_ = 0;
#endif

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
					composites_()
		{
		}

					
		Representation::Representation(const Representation& rp)
			throw()
				: PropertyManager(rp)
		{
			*this = rp;
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
					composites_()
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
				composites_(composites)
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
			// no need to update hidden representations
			if (hasProperty(PROPERTY__HIDDEN)) 
			{
				needs_update_ = true;
				return;
			}
			else
			{
				needs_update_ = false;
			}
#ifndef BALL_QT_HAS_THREADS
			update_(rebuild);
#else
			MainControl* mc = MainControl::getInstance(0);
			
			// ????? dirty trick to avoid getInstance problem under windows
#ifdef BALL_PLATFORM_WINDOWS
			mc = dynamic_cast<MainControl*>(qApp->mainWidget());
#endif

			if (mc == 0)
			{
				update_(rebuild);
				return;
			}

			if (thread_ != 0)
			{
				thread_->wait();
				delete thread_;
				thread_ = 0;
			}
			thread_ = new UpdateRepresentationThread;
			thread_->setRepresentation(*this);
			thread_->setRebuild(rebuild);
			thread_->start();

			//bool mc_was_muteable = mc->compositesAreMuteable();
			mc->setCompositesMuteable(false);
			
			Position pos = 3;
			String dots;
			while (thread_->running())
			{
				qApp->wakeUpGuiThread();
 				qApp->processEvents();
				if (pos < 40) 
				{
					pos ++;
					dots +="..";
				}
				else 
				{
					pos = 3;
					dots = "...";
				}
				
				mc->setStatusbarText("Creating " + getModelName() + " Model " + dots);
				thread_->wait(500); 
			}

			mc->setStatusbarText("");
			mc->setCompositesMuteable(true);
			
 			if (mc->getPrimitiveManager().has(*this))
			{
				RepresentationMessage* msg = new RepresentationMessage(*this, RepresentationMessage::UPDATE);
				mc->sendMessage(*msg);
				return;
			}

			mc->insert(*this);
 			mc->setStatusbarText("");

#endif
		}

		void Representation::update_(bool rebuild) 
			throw()
		{
			// if no ModelProcessor was given, there can only exist 
			// handmade GeometricObjects, which dont need to be updated
			if (model_processor_ != 0 && rebuild) 
			{
				clearGeometricObjects();
				model_processor_->getGeometricObjects().clear();
				model_processor_->clearComposites();
				
				CompositeSet::Iterator it = composites_.begin();
				for (; it!= composites_.end(); it++)
				{
					(const_cast<Composite*>(*it))->apply(*model_processor_);
				}
				geometric_objects_ = model_processor_->getGeometricObjects();
				model_build_time_ = PreciseTime::now();
			}

			if (color_processor_ != 0) 
			{
				// make sure, that the atom grid is recomputed for meshes
				if (rebuild) color_processor_->setComposites(&composites_);
				color_processor_->setTransparency(transparency_);
				color_processor_->setModelType(model_type_);
				geometric_objects_.apply(*color_processor_);
			}
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

	} // namespace VIEW
} // namespace BALL
