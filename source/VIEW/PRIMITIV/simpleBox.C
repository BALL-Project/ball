// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: simpleBox.C,v 1.9 2002/12/16 12:23:15 sturm Exp $

#include <BALL/VIEW/PRIMITIV/simpleBox.h>

using namespace std;

namespace BALL
{

	namespace VIEW
	{

		SimpleBox::SimpleBox()
			throw()
			:	GeometricObject(),
				ColorExtension(),
				Vertex2()
		{
		}

		SimpleBox::SimpleBox(const SimpleBox& simpleBox, bool deep)
			throw()
			:	GeometricObject(simpleBox, deep),
				ColorExtension(simpleBox),
				Vertex2(simpleBox)
		{
		}

		SimpleBox::SimpleBox(const GeometricObject& geometric_object)
			throw()
			:	GeometricObject(geometric_object),
				ColorExtension(),
				Vertex2()
		{
		}

		SimpleBox::~SimpleBox()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<SimpleBox>() << endl;
			#endif 

			destroy();
		}

		void SimpleBox::clear()
			throw()
		{
			GeometricObject::clear();
			ColorExtension::clear();
			Vertex2::clear();
		}

		void SimpleBox::destroy()
			throw()
		{
			GeometricObject::destroy();
			ColorExtension::destroy();
			Vertex2::destroy();
		}

		void SimpleBox::set(const SimpleBox& simpleBox, bool deep)
			throw()
		{
			GeometricObject::set(simpleBox, deep);
			ColorExtension::set(simpleBox);
			Vertex2::set(simpleBox);
		}

		const SimpleBox& SimpleBox::operator = (const SimpleBox& simpleBox)
			throw()
		{
			set(simpleBox);
			return *this;
		}

		void SimpleBox::get(SimpleBox& simpleBox, bool deep) const
			throw()
		{
			simpleBox.set(*this, deep);
		}

		void SimpleBox::swap(SimpleBox& simpleBox)
			throw()
		{
			GeometricObject::swap(simpleBox);
			ColorExtension::swap(simpleBox);
			Vertex2::swap(simpleBox);
		}

		bool SimpleBox::isValid() const
			throw()
		{
			return (GeometricObject::isValid() && 
							        Vertex2::isValid());
		}

		void SimpleBox::dump(ostream& s, Size depth) const
			throw()
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			GeometricObject::dump(s, depth + 1);
			ColorExtension::dump(s, depth + 1);
			Vertex2::dump(s, depth + 1);

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		bool SimpleBox::extract()
			throw()
		{
			return true;  
		}

	} // namespace VIEW

} // namespace BALL
