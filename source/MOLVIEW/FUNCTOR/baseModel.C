// $Id: baseModel.C,v 1.12 2001/05/13 16:27:25 hekl Exp $

#include <BALL/MOLVIEW/FUNCTOR/baseModel.h>

using namespace std;

namespace BALL
{

	namespace MOLVIEW
	{

		BaseModelProcessor::BaseModelProcessor()
			throw()
			:	UnaryProcessor<Composite>(),
				ExtendedPropertyManager(),
				find_geometric_object_(),
				color_calculator_(const_cast<ColorCalculator*>((ColorCalculator*)&RTTI::getDefault<ColorCalculator>())),
				model_connector_(const_cast<BaseModelConnector*>((BaseModelConnector*)&RTTI::getDefault<BaseModelConnector>()))
		{
			clear_();
		}

		BaseModelProcessor::BaseModelProcessor
			(const BaseModelProcessor& base_model_processor, bool deep)
			throw()
			:	UnaryProcessor<Composite>(),
				ExtendedPropertyManager(base_model_processor, deep),
				find_geometric_object_(base_model_processor.find_geometric_object_, deep),
				color_calculator_(base_model_processor.color_calculator_),
				model_connector_(base_model_processor.model_connector_)
		{
		}

		BaseModelProcessor::~BaseModelProcessor()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<BaseModelProcessor>() << endl;
			#endif 

			destroy();
		}

		void BaseModelProcessor::clear()
			throw()
		{
			ExtendedPropertyManager::clear();
			find_geometric_object_.clear();
			color_calculator_ = const_cast<ColorCalculator*>((ColorCalculator*)&RTTI::getDefault<ColorCalculator>());
			model_connector_ = const_cast<BaseModelConnector*>((BaseModelConnector*)&RTTI::getDefault<BaseModelConnector>());

			clear_();
		}

		void BaseModelProcessor::destroy()
			throw()
		{
		}

		void BaseModelProcessor::set
			(const BaseModelProcessor& base_model_processor, bool deep)
			throw()
		{
			clear_();

			ExtendedPropertyManager::set(base_model_processor, deep);
			find_geometric_object_.set(base_model_processor.find_geometric_object_, deep);
			color_calculator_ = base_model_processor.color_calculator_;
			model_connector_ = base_model_processor.model_connector_;
		}

		const BaseModelProcessor& BaseModelProcessor::operator =
			(const BaseModelProcessor& base_model_processor)
			throw()
		{
			set(base_model_processor);

			return *this;
		}

		void BaseModelProcessor::get
			(BaseModelProcessor& base_model_processor, bool deep) const
			throw()
		{
			base_model_processor.set(*this, deep);
		}

		void BaseModelProcessor::swap
			(BaseModelProcessor& base_model_processor)
			throw()
		{
			ExtendedPropertyManager::swap(base_model_processor);

			find_geometric_object_.swap(base_model_processor.find_geometric_object_);

			ColorCalculator *color_calculator = color_calculator_;
			color_calculator_ = base_model_processor.color_calculator_;
			base_model_processor.color_calculator_ = color_calculator;

			BaseModelConnector *model_connector = model_connector_;
			model_connector_ = base_model_processor.model_connector_;
			base_model_processor.model_connector_ = model_connector;
		}

		bool BaseModelProcessor::start()
		{
			if (hasProperty(GeometricObject::PROPERTY__OBJECT_DYNAMIC) == true)
			{
				getSearcher_().setProperty(GeometricObject::PROPERTY__OBJECT_DYNAMIC);
				getSearcher_().clearProperty(GeometricObject::PROPERTY__OBJECT_STATIC);
				model_connector_->getGeometricObjectSearcher().setProperty(GeometricObject::PROPERTY__OBJECT_DYNAMIC);
				model_connector_->getGeometricObjectSearcher().clearProperty(GeometricObject::PROPERTY__OBJECT_STATIC);
			}
			else // default
			{
				getSearcher_().setProperty(GeometricObject::PROPERTY__OBJECT_STATIC);
				getSearcher_().clearProperty(GeometricObject::PROPERTY__OBJECT_DYNAMIC);
				model_connector_->getGeometricObjectSearcher().setProperty(GeometricObject::PROPERTY__OBJECT_STATIC);
				model_connector_->getGeometricObjectSearcher().clearProperty(GeometricObject::PROPERTY__OBJECT_DYNAMIC);
			}

			getModelConnector()->registerColorCalculator(*getColorCalculator());

			return true;
		}
				
		bool BaseModelProcessor::finish()
		{
			return true;
		}
				
		Processor::Result BaseModelProcessor::operator()(Composite & /* composite */)
		{
			return Processor::CONTINUE;
		}

		bool BaseModelProcessor::isValid() const
			throw()
		{
			return (ExtendedPropertyManager::isValid()
							&& model_connector_->isValid()
							&& find_geometric_object_.isValid());
		}

		void BaseModelProcessor::dump(ostream& s, Size depth) const
			throw()
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_DEPTH(s, depth);
			s << "Properties: -------------------------------" << endl;

			dump_(s, depth + 1);

			BALL_DUMP_DEPTH(s, depth);
			s << "-----------------------------------------------------" << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "default colorCalculator registered: " 
					 << ((color_calculator_ == 
								(const_cast<ColorCalculator*>((const ColorCalculator *)(&RTTI::getDefault<ColorCalculator>())))) ? "yes" : "no")
					 << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "user colorCalculator registered: " 
					 << ((color_calculator_ != 
								(const_cast<ColorCalculator*>((const ColorCalculator *)(&RTTI::getDefault<ColorCalculator>())))) ? "yes" : "no")
					 << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "generate static model: "
					 << ((hasProperty(GeometricObject::PROPERTY__OBJECT_STATIC)) ? "yes" : "no")
					 << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "generate dynamic model: "
					 << ((hasProperty(GeometricObject::PROPERTY__OBJECT_DYNAMIC)) ? "yes" : "no")
					 << endl;

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void BaseModelProcessor::dump_
			(ostream& s, Size depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);

			BALL_DUMP_DEPTH(s, depth);
			s << "transparent object: " 
					 << BALL_MOLVIEW_PRINT_PROPERTY
											(GeometricObject::PROPERTY__OBJECT_TRANSPARENT) 
					 << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "           visible: " 
					 << BALL_MOLVIEW_PRINT_PROPERTY
											(GeometricObject::PROPERTY__OBJECT_VISIBLE) 
					 << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "            opened: " 
					 << BALL_MOLVIEW_PRINT_PROPERTY
											(GeometricObject::PROPERTY__OBJECT_OPENED) 
					 << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "        dmode dots: " 
					 << BALL_MOLVIEW_PRINT_PROPERTY
											(GeometricObject::PROPERTY__DRAWING_MODE_DOTS) 
					 << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "   dmode wireframe: " 
					 << BALL_MOLVIEW_PRINT_PROPERTY
											(GeometricObject::PROPERTY__DRAWING_MODE_WIREFRAME) 
					 << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "       dmode solid: " 
					 << BALL_MOLVIEW_PRINT_PROPERTY
											(GeometricObject::PROPERTY__DRAWING_MODE_SOLID) 
					 << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "    dprecision low: " 
					 << BALL_MOLVIEW_PRINT_PROPERTY
											(GeometricObject::PROPERTY__DRAWING_PRECISION_LOW) 
					 << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << " dprecision medium: " 
					 << BALL_MOLVIEW_PRINT_PROPERTY
											(GeometricObject::PROPERTY__DRAWING_PRECISION_MEDIUM) 
					 << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "   dprecision high: " 
					 << BALL_MOLVIEW_PRINT_PROPERTY
											(GeometricObject::PROPERTY__DRAWING_PRECISION_HIGH) 
					 << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "  dprecision ultra: " 
					 << BALL_MOLVIEW_PRINT_PROPERTY
											(GeometricObject::PROPERTY__DRAWING_PRECISION_ULTRA) 
					 << endl;

			BALL_DUMP_STREAM_SUFFIX(s);
		}


  	bool BaseModelProcessor::isModel_(GeometricObject& geometric_object)
			throw()
    {
			if (geometric_object.hasProperty(PROPERTY__MODEL_BALL_AND_STICK))
			{
				return true;
			}
			else if (geometric_object.hasProperty(PROPERTY__MODEL_DOTS))
			{
				return true;
			}
			else if (geometric_object.hasProperty(PROPERTY__MODEL_LINES))
			{
				return true;
			}
			else if (geometric_object.hasProperty(PROPERTY__MODEL_VDW))
			{
				return true;
			}

			return false;
    }

	  void BaseModelProcessor::removeGeometricObjects_(Composite& composite, bool only_models)
			throw()
    {
			// get geometric objects
			composite.applyChild(getSearcher_());

			// if geometric objects found, delete them
			if (getSearcher_().geometricObjectsFound())
			{
				// get geometric objects
				List<VIEW::GeometricObject*>::Iterator it = getSearcher_().getGeometricObjects().begin();
				
				for(; it != getSearcher_().getGeometricObjects().end(); ++it)
				{
					if (only_models)
					{
						if (!isModel_(**it))
						{
							continue;
						}
					}

					Composite *composite_ptr = RTTI::castTo<Composite>(**it);
					
					// remove geometric object from composite
					if(composite_ptr != 0)
					{
						// delete geometric object
						delete composite_ptr;
					}
				}
			}
   }

		void BaseModelProcessor::clear_()
			throw()
		{
			setProperty(GeometricObject::PROPERTY__OBJECT_STATIC);
			setProperty(GeometricObject::PROPERTY__OBJECT_OPAQUE);
			setProperty(GeometricObject::PROPERTY__OBJECT_VISIBLE);
			setProperty(GeometricObject::PROPERTY__OBJECT_CLOSED);
			setProperty(GeometricObject::PROPERTY__DRAWING_MODE_SOLID);
			setProperty(GeometricObject::PROPERTY__DRAWING_PRECISION_HIGH);
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/FUNCTOR/baseModel.iC>
#		endif

	} // namespace MOLVIEW

} // namespace BALL
