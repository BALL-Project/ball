// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: label.C,v 1.1 2003/08/26 12:38:43 oliver Exp $

#include <BALL/VIEW/PRIMITIV/label.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

		Label::Label()
			throw()
			:	GeometricObject(),
				Vertex()
		{
		}

		Label::Label(const Label& label, bool deep)
			throw()
			:	GeometricObject(label, deep),
				Vertex(label)
		{
		}

		Label::~Label()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << (void *)this 
						 			 << " of class " << RTTI::getName<Label>() << std::endl;
			#endif 
		}

		void Label::clear()
			throw()
		{
			GeometricObject::clear();
			Vertex::clear();
		}

		void Label::set(const Label& label, bool deep)
			throw()
		{
			GeometricObject::set(label, deep);
			Vertex::set(label);
		}

		const Label& Label::operator = (const Label& label)
			throw()
		{
			set(label);
			return *this;
		}

		void Label::get(Label& label, bool deep) const
			throw()
		{
			label.set(*this, deep);
		}

		void Label::swap(Label& label)
			throw()
		{
			GeometricObject::swap(label);
			Vertex::swap(label);
		}

	  String Label::getTypeName() const
			throw()
    {
			return String("Label");
    }

		bool Label::isValid() const
			throw()
		{
			return (GeometricObject::isValid() &&
							Vertex::isValid());
		}

		void Label::dump(ostream& s, Size depth) const
			throw()
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_DEPTH(s, depth);
			s << "Label Text: " << getName() << endl;

			GeometricObject::dump(s, depth + 1);
			Vertex::dump(s, depth + 1);

			BALL_DUMP_STREAM_SUFFIX(s);
		}

	} // namespace VIEW
} // namespace BALL
