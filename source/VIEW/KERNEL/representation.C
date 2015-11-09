// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: representation.C,v 1.66.16.3 2007/04/20 14:19:52 amoll Exp $
//


#include <BALL/VIEW/KERNEL/representation.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/KERNEL/geometricObject.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/PRIMITIVES/sphere.h>

#include <BALL/VIEW/PRIMITIVES/mesh.h>
#include <BALL/VIEW/PRIMITIVES/label.h>
#include <BALL/VIEW/DATATYPE/vertex1.h>
#include <BALL/VIEW/DATATYPE/vertex2.h>

#include <BALL/KERNEL/atom.h>
#include <BALL/SYSTEM/timer.h>

#include <BALL/CONCEPT/textPersistenceManager.h>

//     #define BALL_BENCHMARKING

namespace BALL
{
	namespace VIEW
	{
		MolecularInformation Representation::information_ = MolecularInformation();
		ModelInformation     Representation::model_information_ = ModelInformation();

		Representation::Representation()
				: PropertyManager(),
					drawing_mode_(DRAWING_MODE_SOLID),
					drawing_precision_(DRAWING_PRECISION_HIGH),
					surface_drawing_precision_(3.5),
					model_type_(MODEL_UNKNOWN),
					coloring_method_(COLORING_UNKNOWN),
					transparency_(0),
					model_processor_(0),
					color_processor_(0),
					composites_(),
					model_build_time_(PreciseTime(99999, 9)),
					needs_update_(false),
					rebuild_(true),
					changed_color_processor_(true),
					hidden_(false),
					geometric_objects_(),
					model_update_enabled_(true),
					coloring_update_enabled_(true),
					custom_model_information_(0)
		{
		}

					
		Representation::Representation(const Representation& rp)
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
					model_build_time_(rp.model_build_time_),
					needs_update_(rp.needs_update_),
					rebuild_(rp.rebuild_),
					changed_color_processor_(true),
					hidden_(rp.hidden_),
					geometric_objects_(),
					model_update_enabled_(rp.model_update_enabled_),
					coloring_update_enabled_(rp.coloring_update_enabled_),
					name_(rp.name_),
					custom_model_information_(rp.custom_model_information_)
		{
			if (rp.model_processor_ != 0)
			{
				model_processor_ = (ModelProcessor*) rp.model_processor_->create();
			}

			if (rp.color_processor_ != 0)
			{
				color_processor_ = (ColorProcessor*) rp.color_processor_->create();
			}

			GeometricObjectList::const_iterator it = rp.getGeometricObjects().begin();
			for (;it != rp.getGeometricObjects().end(); it++)
			{
				GeometricObject* object = (GeometricObject*)(**it).create();
				getGeometricObjects().push_back(object);
			}
		}


		Representation::Representation(ModelType model_type,
																	 DrawingPrecision drawing_precision,
																	 DrawingMode drawing_mode)
				: PropertyManager(),
					drawing_mode_(drawing_mode),
					drawing_precision_(drawing_precision),
					surface_drawing_precision_(3.5),
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
					coloring_update_enabled_(true),
					custom_model_information_(0)
		{
		}


		bool Representation::operator == (const Representation& representation) const
		{
			return drawing_mode_== representation.drawing_mode_ &&
							drawing_precision_== representation.drawing_precision_ &&
							model_type_ == representation.model_type_ &&
							coloring_method_ == representation.coloring_method_ &&
							transparency_ == representation.transparency_ &&
							surface_drawing_precision_ == representation.surface_drawing_precision_ &&
							hidden_ == representation.hidden_ &&
							name_ 	== representation.name_ &&
							PropertyManager::operator == (representation);
		}

		const Representation& Representation::operator = (const Representation& representation)
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

			GeometricObjectList::const_iterator it = representation.getGeometricObjects().begin();
			for (;it != representation.getGeometricObjects().end(); it++)
			{
				GeometricObject* object = (GeometricObject*)(**it).create();
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
		{
			clear();
		}

		
		void Representation::clear()
		{
			composites_.clear();

			clearGeometricObjects();

			if (model_processor_  != 0) delete model_processor_;
			if (color_processor_  != 0) delete color_processor_;
			model_processor_ 	= 0;
			color_processor_ 	= 0;

			drawing_mode_= DRAWING_MODE_SOLID;
			drawing_precision_= DRAWING_PRECISION_HIGH;
			model_type_ = MODEL_UNKNOWN;
			coloring_method_ = COLORING_UNKNOWN;
			transparency_ = 0;
			surface_drawing_precision_ = 3.5;

			rebuild_ = true;
			hidden_ = false;

			model_update_enabled_ = true;
			coloring_update_enabled_ = true;
		}

		
		void Representation::clearGeometricObjects()
		{
			list<GeometricObject*>::iterator it = getGeometricObjects().begin();
			for (; it != getGeometricObjects().end(); it++)
			{
				delete *it;
			}

			getGeometricObjects().clear();
		}
					

		void Representation::dump(std::ostream& s, Size depth) const
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
		{
			if (surface_drawing_precision_ < 0.1) return false;
				
			if (drawing_precision_  < 0 || drawing_precision_ > BALL_VIEW_MAXIMAL_DRAWING_PRECISION ||
					drawing_mode_ 			< 0 || drawing_mode_ > BALL_VIEW_MAXIMAL_DRAWING_MODE ||
					transparency_ 			> 255)
			{
				return false;
			}

#ifdef BALL_VIEW_DEBUG
			GeometricObjectList::const_iterator it = getGeometricObjects().begin();
			for (; it != getGeometricObjects().end(); it++)
			{
				if (!(*it)->isValid()) return false;
			}
#endif

			if (model_processor_ != 0 && !model_processor_->isValid()) return false; 

			return true;
		}

	
		void Representation::update_() 
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
					list<const Composite*>::const_iterator it = composites_.begin();
					for (; it!= composites_.end(); it++)
					{
						if ((*it)->getModificationTime() > model_build_time_)
						{
							rebuild_ = true;
							break;
						}
					}
				}

				// rebuild if the selection color changed
				// slow this way, but do it anyhow...
				rebuild_ |= BALL_SELECTED_COLOR_CHANGE_TIME > model_build_time_;

				if (rebuild_)
				{
					clearGeometricObjects();
					model_processor_->clearComposites();
					
					list<const Composite*>::const_iterator it = composites_.begin();
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
					list<const Composite*>::const_iterator it = composites_.begin();
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

					if (color_processor_->start())
					{
						bool no_error = true;
						for (std::list<GeometricObject*>::iterator it = getGeometricObjects().begin(); it != getGeometricObjects().end(); ++it)
						{
							if ((*color_processor_)(*it) <= Processor::BREAK)
							{
								no_error = false;
								break;
							}
						}

						if(no_error)
						{
							color_processor_->finish();
						}
					}

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

			// in multithreaded, the RepresentationManager will send the Update message
		}
		

		String Representation::getProperties() const
		{
			String prop;
			if (getModelInformation().isSurfaceModel(model_type_))
			{
				GeometricObjectList::const_iterator it = getGeometricObjects().begin();
				Size triangles = 0;
				for (;it != getGeometricObjects().end(); it++)
				{
                    if (RTTI::isKindOf<Mesh>(*it))
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
		{ 
			clearGeometricObjects();

			if (model_processor_ != 0) delete model_processor_;

			model_processor_ = processor;
			
			if (model_processor_ != 0) 
			{
				model_processor_->setSurfaceDrawingPrecision(surface_drawing_precision_);
				model_processor_->setDrawingPrecision(drawing_precision_);
			}

			changed_color_processor_ = true;
		}


		void Representation::setColorProcessor(ColorProcessor* processor)
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
			else
			{
				Size alpha = 255 - transparency_;
				GeometricObjectList::iterator it = geometric_objects_.begin();
				for (; it != geometric_objects_.end(); ++it)
				{
					MultiColorExtension* mce = dynamic_cast<MultiColorExtension*>(*it);
					if (mce != 0)
					{
						mce->setAlphas(alpha);
						continue;
					}

					(**it).getColor().setAlpha(alpha);
				}
			}

 			changed_color_processor_ = true;
		}

		bool Representation::needsUpdate() const
		{
			if (needs_update_ || changed_color_processor_)
			{
				return true;
			}

			list<const Composite*>::const_iterator it = composites_.begin();
			for (;it != composites_.end(); it++)
			{
				if (getModelBuildTime() < (*it)->getModificationTime()) return true;
			}

			return false;
		}

		String Representation::toString() const
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

			list<const Composite*>::const_iterator it = composites_.begin();
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

			// now, add all the NamedProperties of the object that can be serialized
			if (countProperties() > 0)
			{
				result += "\\";
				for (Position i=0; i<countProperties(); ++i) 
				{
					std::ostringstream serialized_properties;
					TextPersistenceManager tpm(serialized_properties);

					NamedProperty prop = getNamedProperty(i);
					tpm << prop;

					String serialized_properties_str = serialized_properties.str();
					result += serialized_properties_str.encodeBase64();
					
					if (i != countProperties()-1)
						result += "*";
				}
			}

			return result;
		}


		void Representation::collectRecursive_(const Composite& c, 
									HashMap<const Composite*, Position>& hashmap) const
		{
			Size i = hashmap.size();
			hashmap[&c] = i;
			for (Position p = 0; p < c.getDegree(); p++)
			{
				collectRecursive_(*c.getChild(p), hashmap);
			}
		}

		void Representation::update(bool rebuild)
		{
			rebuild_ |= rebuild;

			MainControl* mc = getMainControl();
			if (mc == 0) 
			{
				update_();
				return;
			}
			
			mc->getRepresentationManager().update_(*this);
		}

		void Representation::setName(const String& name)
		{
			name_ = name;
      setProperty("REPRESENTATION_NAME", name);
		}

		String Representation::getName() const
		{
			if (hasProperty("REPRESENTATION_NAME")) return getProperty("REPRESENTATION_NAME").getString();

			if (hasProperty(Representation::PROPERTY__IS_COORDINATE_SYSTEM))
			{
				return "Coordinate System";
			}

			String name = getModelInformation().getModelName(model_type_) + " " + getCompositeName();

			return name;
		}

		String Representation::getCompositeName() const
		{
			if (getComposites().size() == 0) return "";

			const Composite* c_ptr = *getComposites().begin();

			String composite_name;
            if (RTTI::isKindOf<Atom>(c_ptr))
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

			if (getComposites().size() > 1) composite_name += "...";

			return composite_name;
		}

		void Representation::setComposites(const list<const Composite*>& composites)
		{
			composites_ = composites;
			needs_update_ = true;
		}

		void Representation::setComposite(const Composite* composite)
		{
			composites_.clear();
			composites_.push_back(composite);
			needs_update_ = true;
		}

		void Representation::enableModelUpdate(bool state) 
		{ 
			model_update_enabled_ = state;
			if (state) return;

			// disable the repositioning of Vertex objects (e.g. Stick model)
			GeometricObjectList::iterator it = getGeometricObjects().begin();
			Vector3 v1, v2;
			for (; it != getGeometricObjects().end(); it++)
			{
                if (RTTI::isKindOf<Vertex>(*it))
				{
					Vertex* v = dynamic_cast<Vertex*>(*it);
					v1 = v->getVertex();
					v->setDefaultVertexAddress();
					v->setVertex(v1);
				}

                if (RTTI::isKindOf<Vertex2>(*it))
				{
					Vertex2* v = dynamic_cast<Vertex2*>(*it);
					v1 = v->getVertex1();
					v->setDefaultVertex1Address();
					v->setVertex1(v1);
					v2 = v->getVertex2();
					v->setDefaultVertex2Address();
					v->setVertex2(v2);
				}

                if (RTTI::isKindOf<Sphere>(*it))
				{
					Sphere* v = dynamic_cast<Sphere*>(*it);
					v1 = v->getPosition();
					v->setDefaultPositionAddress();
					v->setPosition(v1);
				}
			}
		}

		void Representation::setModelInformation(const ModelInformation& mi)
		{
			custom_model_information_ = &mi;
		}

		const ModelInformation& Representation::getModelInformation() const
		{
			if (custom_model_information_ != 0) return *custom_model_information_;

			return model_information_;
		}
			
  #	ifdef BALL_NO_INLINE_FUNCTIONS
  #		include <BALL/VIEW/KERNEL/representation.iC>
  #	endif

	} // namespace VIEW
} // namespace BALL
