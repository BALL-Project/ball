// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: simpleBox.C,v 1.1 2003/08/26 12:38:44 oliver Exp $

#include <BALL/VIEW/PRIMITIV/simpleBox.h>

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

		SimpleBox::SimpleBox(const SimpleBox& box, bool deep)
			throw()
			:	SimpleBox3(box),
				GeometricObject(box, deep)
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

		void SimpleBox::set(const SimpleBox& box, bool deep)
			throw()
		{
			GeometricObject::set(box, deep);
			SimpleBox3::set(box);
		}

		const SimpleBox& SimpleBox::operator = (const SimpleBox& box)
			throw()
		{
			set(box);
			return *this;
		}

		void SimpleBox::get(SimpleBox& box, bool deep) const
			throw()
		{
			box.set(*this, deep);
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
