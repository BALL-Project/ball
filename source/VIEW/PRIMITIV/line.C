// $Id: line.C,v 1.1 1999/08/26 08:02:47 oliver Exp $

#include <BALL/VIEW/PRIMITIV/line.h>


namespace BALL
{

	namespace VIEW
	{

		Line::Line()
			:	GeometricObject(),
				ColorExtension(),
				Vertex2()
		{
		}

		Line::Line(const Line& line, bool deep)
			:	GeometricObject(line, deep),
				ColorExtension(line, deep),
				Vertex2(line, deep)
		{
		}

		Line::Line(const GeometricObject& geometric_object)
			:	GeometricObject(geometric_object),
				ColorExtension(),
				Vertex2()
		{
		}

		Line::~Line()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI<Line>::getName() << endl;
			#endif 

			destroy();
		}

		void Line::clear()
		{
			GeometricObject::clear();
			ColorExtension::clear();
			Vertex2::clear();
		}

		void Line::destroy()
		{
			GeometricObject::destroy();
			ColorExtension::destroy();
			Vertex2::destroy();
		}

		void Line::set(const Line& line, bool deep)
		{
			GeometricObject::set(line, deep);
			ColorExtension::set(line, deep);
			Vertex2::set(line, deep);
		}

		Line& Line::operator = (const Line& line)
		{
			set(line);

			return *this;
		}

		void Line::get(Line& line, bool deep) const
		{
			line.set(*this, deep);
		}

		void Line::swap(Line& line)
		{
			GeometricObject::swap(line);
			ColorExtension::swap(line);
			Vertex2::swap(line);
		}

		bool Line::isValid() const
		{
			return (bool)(GeometricObject::isValid() == true
										&& ColorExtension::isValid() == true
										&& Vertex2::isValid() == true);
		}

		void Line::dump
			(ostream& s, unsigned long depth) const
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
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void Line::write(ostream & /* s */) const
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		bool Line::export()
		{
			return true;  
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/PRIMITIV/line.iC>
#		endif

	} // namespace VIEW

} // namespace BALL
