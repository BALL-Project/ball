// $Id: baseModelConnector.C,v 1.1 2000/06/25 19:05:02 hekl Exp $

#include <BALL/MOLVIEW/FUNCTOR/baseModelConnector.h>

using namespace std;

namespace BALL
{

	namespace MOLVIEW
	{

		BaseModelConnector::BaseModelConnector()
			:	Visitor<Composite>(),
				PropertyManager(),
				searcher_(),
				color_calculator_(const_cast<ColorCalculator*>((ColorCalculator*)&RTTI::getDefault<ColorCalculator>()))
		{
		}

		BaseModelConnector::BaseModelConnector
			(const BaseModelConnector& connector, bool deep)
			:	Visitor<Composite>(connector),
				PropertyManager(connector, deep),
				searcher_(),
				color_calculator_(connector.color_calculator_)
		{
		}

		BaseModelConnector::~BaseModelConnector()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<BaseModelConnector>() << endl;
			#endif 

			destroy();
		}

		void BaseModelConnector::clear()
		{
			PropertyManager::clear();
			searcher_.clear();
			color_calculator_ 
				= const_cast<ColorCalculator*>((ColorCalculator*)&RTTI::getDefault<ColorCalculator>());
		}

		void BaseModelConnector::destroy()
		{
			clear();
		}

		void BaseModelConnector::set
			(const BaseModelConnector& connector,
			 bool deep)
		{
			PropertyManager::set(connector, deep);
			
			color_calculator_ = connector.color_calculator_;
		}

		BaseModelConnector& 	BaseModelConnector::operator =
			(const BaseModelConnector& connector)
		{
			set(connector);

			return *this;
		}

		void BaseModelConnector::get
			(BaseModelConnector& connector, bool deep) const
		{
			connector.set(*this, deep);
		}

		void BaseModelConnector::swap
			(BaseModelConnector& connector)
		{
			PropertyManager::swap(connector);

			color_calculator_->swap(*connector.color_calculator_);
		}

		void BaseModelConnector::visit(Composite& composite)
		{
		}

		bool BaseModelConnector::isValid() const
		{
			return color_calculator_->isValid();
		}

		void BaseModelConnector::dump
			(ostream& s, Size depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			color_calculator_->dump(s, depth + 1);

			BALL_DUMP_STREAM_SUFFIX(s);
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/FUNCTOR/baseModelConnector.iC>
#		endif

	} // namespace MOLVIEW

} // namespace BALL
