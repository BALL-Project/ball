// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: representation.C,v 1.62.4.18 2005/10/17 14:50:02 amoll Exp $
//


#include <BALL/VIEW/KERNEL/representation.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/KERNEL/geometricObject.h>
#include <BALL/VIEW/KERNEL/mainControl.h>

#include <BALL/VIEW/PRIMITIVES/mesh.h>
#include <BALL/VIEW/PRIMITIVES/label.h>

#include <BALL/KERNEL/atom.h>
#include <BALL/SYSTEM/timer.h>

//     #define BALL_BENCHMARKING

namespace BALL
{
	namespace VIEW
	{
		MolecularInformation Representation::information_ = MolecularInformation();

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
					composites_(),
					model_build_time_(PreciseTime(99999, 9)),
					rebuild_(true),
					changed_color_processor_(true),
					hidden_(false),
					geometric_objects_(),
					model_update_enabled_(true),
					coloring_update_enabled_(true)
		{
		}

					
		Representation::Representation(const Representation& rp)
			throw()
				: PropertyManager(rp),
					drawing_mode_(rp.drawing_mode_),
					drawing_precision_(rp.drawing_precision_),
					surface_drawing_precision_(rp.surface_drawing_precision_),
					model_type_(rp.model_type_),
					coloring_method_(rp.coloring_method_),
					transparency_(rp.transparency_),
					model_processor_(0),
					color_processor_(0),
					composites_(rp.composites_),
					model_build_time_(PreciseTime(99999, 9)),
					rebuild_(rp.rebuild_),
					changed_color_processor_(true),
					hidden_(rp.hidden_),
					geometric_objects_(rp.geometric_objects_),
					model_update_enabled_(rp.model_update_enabled_),
					coloring_update_enabled_(rp.coloring_update_enabled_)
		{
			if (rp.model_processor_ != 0)
			{
				model_processor_ = (ModelProcessor*) rp.model_processor_->create();
			}

			if (rp.color_processor_ != 0)
			{
				color_processor_ = (ColorProcessor*) rp.color_processor_->create();
			}

			GeometricObjectList::ConstIterator it = rp.getGeometricObjects().begin();
			for (;it != rp.getGeometricObjects().end(); it++)
			{
				GeometricObject* object = (GeometricObject*)(**it).create();
				getGeometricObjects().push_back(object);
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
					composites_(),
					model_build_time_(PreciseTime(99999, 9)),
					rebuild_(true),
					changed_color_processor_(true),
					hidden_(false),
					geometric_objects_(),
					model_update_enabled_(true),
					coloring_update_enabled_(true)
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

			GeometricObjectList::ConstIterator it = representation.getGeometricObjects().begin();
			for (;it != representation.getGeometricObjects().end(); it++)
			{
				GeometricObject* object = new GeometricObject(**it);
				getGeometricObjects().push_back(object);
			}

			composites_ = representation.composites_;

			rebuild_ = true;
			hidden_ = representation.hidden_;
			changed_color_processor_ = true;
	
			model_update_enabled_ = true;
			coloring_update_enabled_ = true;

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
			composites_.clear();

			if (model_processor_  != 0) delete model_processor_;
			if (color_processor_  != 0) delete color_processor_;
			model_processor_ 	= 0;
			color_processor_ 	= 0;

			clearGeometricObjects();

			drawing_mode_= DRAWING_MODE_SOLID;
			drawing_precision_= DRAWING_PRECISION_HIGH;
			model_type_ = MODEL_UNKNOWN;
			coloring_method_ = COLORING_UNKNOWN;
			transparency_ = 0;
			surface_drawing_precision_ = -1;

			rebuild_ = true;
			hidden_ = false;

			model_update_enabled_ = true;
			coloring_update_enabled_ = true;
		}

		
		void Representation::clearGeometricObjects()
			throw()
		{
			List<GeometricObject*>::Iterator it = getGeometricObjects().begin();
			for (; it != getGeometricObjects().end(); it++)
			{
				delete *it;
			}
			getGeometricObjects().clear();

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
			s << "surface drawing precision: " << surface_drawing_precision_<< std::endl;
			BALL_DUMP_DEPTH(s, depth);
			s << "model type : " << model_type_ << std::endl;
			BALL_DUMP_DEPTH(s, depth);
			s << "coloring type : " << coloring_method_ << std::endl;
			BALL_DUMP_DEPTH(s, depth);
			s << "number of primitives: " << getGeometricObjects().size() << std::endl;
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
			if (!isSurfaceModel(model_type_))
			{
				if (drawing_precision_  < 0 || drawing_precision_ > BALL_VIEW_MAXIMAL_DRAWING_PRECISION)
				{
					return false;
				}
			}
			else
			{
				if (surface_drawing_precision_ < 0.1) return false;
			}
				
			if (drawing_precision_  < 0 || drawing_precision_ > BALL_VIEW_MAXIMAL_DRAWING_PRECISION ||
					drawing_mode_ 			< 0 || drawing_mode_ > BALL_VIEW_MAXIMAL_DRAWING_MODE ||
					transparency_ 			> 255)
			{
				return false;
			}

#ifdef BALL_VIEW_DEBUG
			GeometricObjectList::ConstIterator it = getGeometricObjects().begin();
			for (; it != getGeometricObjects().end(); it++)
			{
				if (!(*it)->isValid()) return false;
			}
#endif

			if (model_processor_ != 0 && !model_processor_->isValid()) return false; 

			return true;
		}

	
		void Representation::update_() 
			throw()
		{
			if (isHidden())
			{
				needs_update_ = true;
				return;
			}

			needs_update_ = false;

#ifdef BALL_BENCHMARKING
			Timer t;
			t.start();
#endif
			// if no ModelProcessor was given, there can only exist 
			// handmade GeometricObjects, which dont need to be updated
			if (model_update_enabled_ && model_processor_ != 0)
			{
				// just to be sure we control the composites if they changed after last model.
				// this shouldnt happen, but maybe someone send the false message after changing the Composite tree
				// (CompositeMessage::CHANGED_COMPOSITE instead of CHANGED_COMPOSITE_HIERARCHY)
				if (!rebuild_)
				{
					List<const Composite*>::const_iterator it = composites_.begin();
					for (; it!= composites_.end(); it++)
					{
						if ((*it)->getModificationTime() > model_build_time_) 
						{
							rebuild_ = true;
							break;
						}
					}
				}

				if (rebuild_)
				{
					clearGeometricObjects();
					model_processor_->clearComposites();
					
					List<const Composite*>::const_iterator it = composites_.begin();
					for (; it!= composites_.end(); it++)
					{
						(const_cast<Composite*>(*it))->apply(*model_processor_);
					}
					model_processor_->createGeometricObjects();

#ifdef BALL_BENCHMARKING
					t.stop();
					logString("Calculating Model time: " + String(t.getClockTime()));
					t.reset();
					t.start();
#endif
				}
			}

			if (coloring_update_enabled_ && color_processor_ != 0)
			{
				bool apply_color_processor = 
						(changed_color_processor_ || rebuild_ || color_processor_->updateAlwaysNeeded());

				// we have to apply the ColorProcessor anyhow if the selection changed since the last model build
				if (!apply_color_processor)
				{
					List<const Composite*>::const_iterator it = composites_.begin();
					for (; it!= composites_.end(); it++)
					{
						if ((*it)->getSelectionTime() > model_build_time_) 
						{
							apply_color_processor = true;
							break;
						}
					}
				}

				if (apply_color_processor)
				{
					// make sure, that the atom grid is recomputed for meshes
					if (rebuild_) color_processor_->setComposites(&getComposites());
					color_processor_->setTransparency(transparency_);
					color_processor_->setModelType(model_type_);
					getGeometricObjects().apply(*color_processor_);
					changed_color_processor_ = false;
#ifdef BALL_BENCHMARKING
					t.stop();
					logString("Calculating Coloring time: " + String(t.getClockTime()));
#endif
				}
			}

			if (model_update_enabled_)
			{
				changed_color_processor_ = false;
				model_build_time_ = PreciseTime::now();
				rebuild_ = false;
			}

#ifndef BALL_QT_HAS_THREADS
			// if multithreaded, the PrimitiveManager will send the Update message, otherwise do it here...
			MainControl* mc = getMainControl();
			if (mc != 0)
			{
				mc->sendMessage(*new RepresentationMessage(*this, RepresentationMessage::UPDATE));
			}
#endif
		}
		

		String Representation::getProperties() const
			throw()
		{
			String prop;
			if (VIEW::isSurfaceModel(model_type_))
			{
				GeometricObjectList::ConstIterator it = getGeometricObjects().begin();
				Size triangles = 0;
				for (;it != getGeometricObjects().end(); it++)
				{
					if (RTTI::isKindOf<Mesh>(**it))
					{ 
						triangles += dynamic_cast<Mesh*>(*it)->triangle.size();
					}
				}
						
				prop = String(triangles) + " T";
			}
			else if (model_type_ == MODEL_LABEL)
			{
				if (getGeometricObjects().size() > 0)
				{
					prop += ((Label*)*getGeometricObjects().begin())->getText();
				}
			}
			else
			{
				prop = String(getGeometricObjects().size()) + " P";
			}

			if (getTransparency() != 0)
			{
				prop += " ";
				prop += String((Size)(getTransparency() / 2.55)) + " % Transparent ";
			}

			if (getDrawingMode() == DRAWING_MODE_WIREFRAME)
			{
				prop += " Wireframe";
			}
			else if (getDrawingMode() == DRAWING_MODE_DOTS)
			{
				prop += " Dots";
			}

			return prop;
		}

		void Representation::setModelProcessor(ModelProcessor* processor)
			throw() 
		{ 
			if (processor == 0 && model_processor_ == 0) return;

			GeometricObjectList::ConstIterator it = geometric_objects_.begin();
			for (;it != geometric_objects_.end(); it++)
			{
				delete *it;
			}
			geometric_objects_.clear();

			// if modelprocessor is removed, copy the geometric object pointers to the Representation's own list
			if (processor == 0 && model_processor_ != 0)
			{
				GeometricObjectList::ConstIterator it = model_processor_->getGeometricObjects().begin();
				for (;it != model_processor_->getGeometricObjects().end(); it++)
				{
					geometric_objects_.push_back(*it);
				}

				model_processor_->getGeometricObjects().clear();
			}

			if (model_processor_ != 0) delete model_processor_;

			model_processor_ = processor;
			
			if (model_processor_ != 0) 
			{
				if (surface_drawing_precision_ >= 0.1)
				{
					model_processor_->setSurfaceDrawingPrecision(surface_drawing_precision_);
				}
				else
				{
					model_processor_->setDrawingPrecision(drawing_precision_);
				}
			}

			changed_color_processor_ = true;
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
				color_processor_->setComposites(&getComposites());
				color_processor_->setTransparency(transparency_);
			}

			changed_color_processor_ = true;
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

 			changed_color_processor_ = true;
		}

		bool Representation::needsUpdate() const
			throw()
		{
			if (needs_update_ || 
					changed_color_processor_ ||
					getModelBuildTime() < Atom::getAttributesModificationTime())
			{
				return true;
			}

			List<const Composite*>::const_iterator it = composites_.begin();
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

			List<const Composite*>::const_iterator it = composites_.begin();
			for (; it != composites_.end(); it++)
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

		void Representation::update(bool rebuild)
			throw()
		{
			rebuild_ |= rebuild;

#ifdef BALL_QT_HAS_THREADS
			MainControl* mc = getMainControl();
			if (mc != 0)
			{
				mc->getPrimitiveManager().update_(*this);
				return;
			}
#endif

			update_();
		}

		String Representation::getName() const
			throw()
		{
			if (hasProperty(Representation::PROPERTY__IS_COORDINATE_SYSTEM))
			{
				return "Coordinate System";
			}

			String name = getModelName().c_str();

			if (getComposites().size() == 0) return name;

			const Composite* c_ptr = *getComposites().begin();

			String composite_name;
			if (RTTI::isKindOf<Atom>(*c_ptr))
			{
				if (c_ptr->getParent() != 0)
				{
					((Composite*)c_ptr->getParent())->host(information_);
					composite_name = information_.getName();
					composite_name += ":";
					composite_name += ((const Atom*) c_ptr)->getName();
				}
				else
				{
					composite_name = ((const Atom*) c_ptr)->getFullName();
				}
			}
			else
			{
				((Composite*)c_ptr)->host(information_);
				composite_name = information_.getName();
			}

			name = name + "  " + composite_name;

			if (getComposites().size() > 1) name += "...";

			return name;
		}

		void Representation::setComposites(const List<const Composite*>& composites)
			throw()
		{
			composites_ = composites;
		}
			
  #	ifdef BALL_NO_INLINE_FUNCTIONS
  #		include <BALL/VIEW/KERNEL/representation.iC>
  #	endif

	} // namespace VIEW
} // namespace BALL
