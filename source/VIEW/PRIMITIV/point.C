// $Id: point.C,v 1.1 1999/08/26 08:02:47 oliver Exp $

#include <BALL/VIEW/PRIMITIV/point.h>

namespace BALL
{

	namespace VIEW
	{


		Point::Point()
			:	GeometricObject(),
				ColorExtension(),
				Vertex()
		{
		}

		Point::Point(const Point& point, bool deep)
			:	GeometricObject(point, deep),
				ColorExtension(point, deep),
				Vertex(point, deep)
		{
		}

		Point::Point(const GeometricObject& geometric_object)
			:	GeometricObject(geometric_object),
				ColorExtension(),
				Vertex()
		{
		}

		Point::~Point()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI<Point>::getName() << endl;
			#endif 

			destroy();
		}

		void Point::clear()
		{
			GeometricObject::clear();
			ColorExtension::clear();
			Vertex::clear();
		}

		void Point::destroy()
		{
			GeometricObject::destroy();
			ColorExtension::destroy();
			Vertex::destroy();
		}

		void Point::set(const Point& point, bool deep)
		{
			GeometricObject::set(point, deep);
			ColorExtension::set(point, deep);
			Vertex::set(point, deep);
		}

		Point& Point::operator =(const Point& point)
		{
			set(point);

			return *this;
		}

		void Point::get(Point& point, bool deep) const
		{
			point.set(*this, deep);
		}

		void Point::swap(Point& point)
		{
			GeometricObject::swap(point);
			ColorExtension::swap(point);
			Vertex::swap(point);
		}

		bool Point::isValid() const
		{
			return (bool)(GeometricObject::isValid() == true
										&& ColorExtension::isValid() == true
										&& Vertex::isValid() == true);
		}

		void Point::dump
			(ostream& s, unsigned long depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			GeometricObject::dump(s, depth + 1);
			ColorExtension::dump(s, depth + 1);
			Vertex::dump(s, depth + 1);

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void Point::read(istream & /* s */)
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void Point::write(ostream & /* s */) const
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		bool Point::export()
		{
			return true;  
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/PRIMITIV/point.iC>
#		endif
	
	} // namespace VIEW

} // namespace BALL
