// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: baseModelConnector.C,v 1.9 2003/08/26 09:17:59 oliver Exp $

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
				geometric_objects_(0),
				color_processor_(0)
		{
		}

		BaseModelConnector::BaseModelConnector(const BaseModelConnector& connector)
			throw()
			:	Visitor<Composite>(connector),
				PropertyManager(connector),
				geometric_objects_(connector.geometric_objects_),
				color_processor_(connector.color_processor_)
		{
		}

		BaseModelConnector::~BaseModelConnector()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << (void *)this 
									 << " of class " << RTTI::getName<BaseModelConnector>() << std::endl;
			#endif 
		}

		const BaseModelConnector&	BaseModelConnector::operator = (const BaseModelConnector& connector)
			throw()
		{
			set(connector);
			return *this;
		}

		void BaseModelConnector::dump(ostream& s, Size depth) const
			throw()
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_STREAM_SUFFIX(s);
		}

	} // namespace MOLVIEW
} // namespace BALL
