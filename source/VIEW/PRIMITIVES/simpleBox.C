// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: simpleBox.C,v 1.4 2003/08/29 15:38:00 amoll Exp $

#include <BALL/VIEW/PRIMITIVES/simpleBox.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

		SimpleBox::SimpleBox()
			throw()
			:	SimpleBox3(),
				GeometricObject()
		{
		}

		SimpleBox::SimpleBox(const SimpleBox& box)
			throw()
			:	SimpleBox3(box),
				GeometricObject(box)
		{
		}

		SimpleBox::~SimpleBox()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error << "Destructing object " << (void *)this 
									<< " of class " << RTTI::getName<SimpleBox>() << std::endl;
			#endif 
		}

		void SimpleBox::clear()
			throw()
		{
			GeometricObject::clear();
			SimpleBox3::clear();
		}

		void SimpleBox::set(const SimpleBox& box)
			throw()
		{
			GeometricObject::set(box);
			SimpleBox3::set(box);
		}

		const SimpleBox& SimpleBox::operator = (const SimpleBox& box)
			throw()
		{
			set(box);
			return *this;
		}

		bool SimpleBox::isValid() const
			throw()
		{
			return (GeometricObject::isValid() && 
				        	 SimpleBox3::isValid());
		}

		void SimpleBox::dump(ostream& s, Size depth) const
			throw()
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			GeometricObject::dump(s, depth + 1);
			SimpleBox3::dump(s, depth + 1);

			BALL_DUMP_STREAM_SUFFIX(s);
		}

	} // namespace VIEW
} // namespace BALL
