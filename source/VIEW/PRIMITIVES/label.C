// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: label.C,v 1.7 2004/07/14 16:37:39 amoll Exp $
//

#include <BALL/VIEW/PRIMITIVES/label.h>

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

		Label::Label(const Label& label)
			throw()
			:	GeometricObject(label),
				Vertex(label),
				text_(label.text_)
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
			text_.clear();
		}

		void Label::set(const Label& label)
			throw()
		{
			GeometricObject::set(label);
			Vertex::set(label);
			text_ = label.text_;
		}

		const Label& Label::operator = (const Label& label)
			throw()
		{
			set(label);
			return *this;
		}

		void Label::swap(Label& label)
			throw()
		{
			GeometricObject::swap(label);
			Vertex::swap(label);
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
			s << "Label Text: " << text_ << endl;

			GeometricObject::dump(s, depth + 1);
			Vertex::dump(s, depth + 1);

			BALL_DUMP_STREAM_SUFFIX(s);
		}

	} // namespace VIEW
} // namespace BALL
