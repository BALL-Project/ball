// $Id: line.C,v 1.8 2001/02/04 16:14:28 hekl Exp $

#include <BALL/VIEW/PRIMITIV/line.h>

using namespace std;

namespace BALL
{

	namespace VIEW
	{

		Line::Line()
			throw()
			:	GeometricObject(),
				ColorExtension(),
				Vertex2()
		{
		}

		Line::Line(const Line& line, bool deep)
			throw()
			:	GeometricObject(line, deep),
				ColorExtension(line),
				Vertex2(line)
		{
		}

		Line::Line(const GeometricObject& geometric_object)
			throw()
			:	GeometricObject(geometric_object),
				ColorExtension(),
				Vertex2()
		{
		}

		Line::~Line()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<Line>() << endl;
			#endif 

			destroy();
		}

		void Line::clear()
			throw()
		{
			GeometricObject::clear();
			ColorExtension::clear();
			Vertex2::clear();
		}

		void Line::destroy()
			throw()
		{
			GeometricObject::destroy();
			ColorExtension::destroy();
			Vertex2::destroy();
		}

		void Line::set(const Line& line, bool deep)
			throw()
		{
			GeometricObject::set(line, deep);
			ColorExtension::set(line);
			Vertex2::set(line);
		}

		const Line& Line::operator = (const Line& line)
			throw()
		{
			set(line);
			return *this;
		}

		void Line::get(Line& line, bool deep) const
			throw()
		{
			line.set(*this, deep);
		}

		void Line::swap(Line& line)
			throw()
		{
			GeometricObject::swap(line);
			ColorExtension::swap(line);
			Vertex2::swap(line);
		}

		bool Line::isValid() const
			throw()
		{
			return (GeometricObject::isValid() &&
										  Vertex2::isValid());
		}

		void Line::dump(ostream& s, Size depth) const
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

		void Line::read(istream & /* s */)
			throw()
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void Line::write(ostream & /* s */) const
			throw()
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		bool Line::extract()
			throw()
		{
			return true;  
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/PRIMITIV/line.iC>
#		endif

	} // namespace VIEW

} // namespace BALL
