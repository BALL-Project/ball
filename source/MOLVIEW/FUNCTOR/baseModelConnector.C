// $Id: baseModelConnector.C,v 1.5 2001/05/13 16:27:25 hekl Exp $

#include <BALL/MOLVIEW/FUNCTOR/baseModelConnector.h>

using namespace std;

namespace BALL
{

	namespace MOLVIEW
	{

		BaseModelConnector::BaseModelConnector()
			throw()
			:	Visitor<Composite>(),
				PropertyManager(),
				searcher_(),
				color_calculator_(const_cast<ColorCalculator*>((ColorCalculator*)&RTTI::getDefault<ColorCalculator>()))
		{
		}

		BaseModelConnector::BaseModelConnector
			(const BaseModelConnector& connector, bool deep)
			throw()
			:	Visitor<Composite>(connector),
				PropertyManager(connector, deep),
				searcher_(),
				color_calculator_(connector.color_calculator_)
		{
		}

		BaseModelConnector::~BaseModelConnector()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<BaseModelConnector>() << endl;
			#endif 

			destroy();
		}

		void BaseModelConnector::clear()
			throw()
		{
			PropertyManager::clear();
			searcher_.clear();
			color_calculator_ 
				= const_cast<ColorCalculator*>((ColorCalculator*)&RTTI::getDefault<ColorCalculator>());
		}

		void BaseModelConnector::destroy()
			throw()
		{
		}

		void BaseModelConnector::set
			(const BaseModelConnector& connector,
			 bool deep)
			throw()
		{
			PropertyManager::set(connector, deep);
			
			color_calculator_ = connector.color_calculator_;
		}

		const BaseModelConnector&	BaseModelConnector::operator =
			(const BaseModelConnector& connector)
			throw()
		{
			set(connector);

			return *this;
		}

		void BaseModelConnector::get
			(BaseModelConnector& connector, bool deep) const
			throw()
		{
			connector.set(*this, deep);
		}

		void BaseModelConnector::swap
			(BaseModelConnector& connector)
			throw()
		{
			PropertyManager::swap(connector);

			color_calculator_->swap(*connector.color_calculator_);
		}

		void BaseModelConnector::visit(Composite& /* composite */)
		{
		}

		void BaseModelConnector::dump
			(ostream& s, Size depth) const
			throw()
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
