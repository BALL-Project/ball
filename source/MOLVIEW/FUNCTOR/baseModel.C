// $Id: baseModel.C,v 1.2 1999/12/19 17:14:25 oliver Exp $

#include <BALL/MOLVIEW/FUNCTOR/baseModel.h>


namespace BALL
{

	namespace MOLVIEW
	{

		BaseModelProcessor::BaseModelProcessor()
			:	UnaryProcessor<Atom>(),
				ExtendedPropertyManager(),
				find_geometric_object_(),
				color_calculator_(const_cast<ColorCalculator*>((ColorCalculator*)&RTTI<ColorCalculator>::getDefault()))
		{
			clear_();
		}

		BaseModelProcessor::BaseModelProcessor
			(const BaseModelProcessor& base_model_processor, bool deep)
			:	UnaryProcessor<Atom>(base_model_processor, deep),
				ExtendedPropertyManager(base_model_processor, deep),
				find_geometric_object_(base_model_processor.find_geometric_object_, deep),
				color_calculator_(base_model_processor.color_calculator_)
		{
		}

		BaseModelProcessor::~BaseModelProcessor()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI<baseModelProcessor>::getName() << endl;
			#endif 

			destroy();
		}

		void BaseModelProcessor::clear()
		{
			ExtendedPropertyManager::clear();
			find_geometric_object_.clear();
			color_calculator_ = const_cast<ColorCalculator*>((const ColorCalculator*)&RTTI<ColorCalculator>::getDefault());

			clear_();
		}

		void BaseModelProcessor::destroy()
		{
			clear();
		}

		void BaseModelProcessor::set
			(const BaseModelProcessor& base_model_processor, bool deep)
		{
			clear_();

			ExtendedPropertyManager::set(base_model_processor, deep);
			find_geometric_object_.set(base_model_processor.find_geometric_object_, deep);
			color_calculator_ = base_model_processor.color_calculator_;
		}

		BaseModelProcessor& BaseModelProcessor::operator =
			(const BaseModelProcessor& base_model_processor)
		{
			set(base_model_processor);

			return *this;
		}

		void BaseModelProcessor::get
			(BaseModelProcessor& base_model_processor, bool deep) const
		{
			base_model_processor.set(*this, deep);
		}

		void BaseModelProcessor::swap
			(BaseModelProcessor& base_model_processor)
		{
			ExtendedPropertyManager::swap(base_model_processor);

			find_geometric_object_.swap(base_model_processor.find_geometric_object_);

			ColorCalculator *color_calculator = color_calculator_;
			color_calculator_ = base_model_processor.color_calculator_;
			base_model_processor.color_calculator_ = color_calculator;
		}

		bool BaseModelProcessor::start()
		{
			if (hasProperty(GeometricObject::PROPERTY__OBJECT_DYNAMIC) == true)
			{
				find_geometric_object_.setProperty(GeometricObject::PROPERTY__OBJECT_DYNAMIC);
			}
			else // default
			{
				find_geometric_object_.setProperty(GeometricObject::PROPERTY__OBJECT_STATIC);
			}

			return true;
		}
				
		bool BaseModelProcessor::finish()
		{
			return true;
		}
				
		Processor::Result BaseModelProcessor::operator()(Atom & /* atom */)
		{
			return Processor::CONTINUE;
		}

		bool BaseModelProcessor::isValid() const
		{
			return (bool)(ExtendedPropertyManager::isValid() == true
										&& color_calculator_->isValid() == true
										&& find_geometric_object_.isValid() == true);
		}

		void BaseModelProcessor::dump
			(ostream& s, unsigned long depth) const
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
			s << "use default colorCalculator: " 
					 << ((color_calculator_ == 
								(const_cast<ColorCalculator*>((const ColorCalculator *)(&RTTI<ColorCalculator>::getDefault())))) ? "yes" : "no")
					 << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "use user colorCalculator : " 
					 << ((color_calculator_ != 
								(const_cast<ColorCalculator*>((const ColorCalculator *)(&RTTI<ColorCalculator>::getDefault())))) ? "yes" : "no")
					 << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "generate static model: "
					 << ((hasProperty(GeometricObject::PROPERTY__OBJECT_STATIC) == true) ? "yes" : "no")
					 << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "generate dynamic model: "
					 << ((hasProperty(GeometricObject::PROPERTY__OBJECT_DYNAMIC) == true) ? "yes" : "no")
					 << endl;

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void BaseModelProcessor::read(istream & /* s */)
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void BaseModelProcessor::write(ostream & /* s */) const
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void BaseModelProcessor::dump_
			(ostream& s, unsigned long depth) const
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

			BALL_DUMP_DEPTH(s, depth);
			s << "   dprecision user: " 
					 << BALL_MOLVIEW_PRINT_PROPERTY
											(GeometricObject::PROPERTY__DRAWING_PRECISION_USER_DEFINED) 
					 << endl;

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void BaseModelProcessor::clear_()
		{
			setProperty(GeometricObject::PROPERTY__MODEL_VDW);
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
