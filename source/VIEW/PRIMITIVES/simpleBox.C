// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: simpleBox.C,v 1.4.28.1 2007-03-25 22:02:35 oliver Exp $

#include <BALL/VIEW/PRIMITIVES/simpleBox.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

		SimpleBox::SimpleBox()
			:	SimpleBox3(),
				GeometricObject()
		{
		}

		SimpleBox::SimpleBox(const SimpleBox& box)
			:	SimpleBox3(box),
				GeometricObject(box)
		{
		}

		SimpleBox::~SimpleBox()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error << "Destructing object " << (void *)this 
									<< " of class " << RTTI::getName<SimpleBox>() << std::endl;
			#endif 
		}

		void SimpleBox::clear()
		{
			GeometricObject::clear();
			SimpleBox3::clear();
		}

		void SimpleBox::set(const SimpleBox& box)
		{
			GeometricObject::set(box);
			SimpleBox3::set(box);
		}

		const SimpleBox& SimpleBox::operator = (const SimpleBox& box)
		{
			set(box);
			return *this;
		}

		bool SimpleBox::isValid() const
		{
			return (GeometricObject::isValid() && 
				        	 SimpleBox3::isValid());
		}

		void SimpleBox::dump(ostream& s, Size depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			GeometricObject::dump(s, depth + 1);
			SimpleBox3::dump(s, depth + 1);

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void SimpleBox::getVertices(vector<Vector3>& vertices) const
		{
			vertices.push_back(a);
			vertices.push_back(b);
		}

	} // namespace VIEW
} // namespace BALL
