// $Id: point.C,v 1.8 2001/02/04 16:14:28 hekl Exp $

#include <BALL/VIEW/PRIMITIV/point.h>

using namespace std;

namespace BALL
{

	namespace VIEW
	{


		Point::Point()
			throw()
			:	GeometricObject(),
				ColorExtension(),
				Vertex()
		{
		}

		Point::Point(const Point& point, bool deep)
			throw()
			:	GeometricObject(point, deep),
				ColorExtension(point),
				Vertex(point)
		{
		}

		Point::Point(const GeometricObject& geometric_object)
			throw()
			:	GeometricObject(geometric_object),
				ColorExtension(),
				Vertex()
		{
		}

		Point::~Point()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<Point>() << endl;
			#endif 

			destroy();
		}

		void Point::clear()
			throw()
		{
			GeometricObject::clear();
			ColorExtension::clear();
			Vertex::clear();
		}

		void Point::destroy()
			throw()
		{
			GeometricObject::destroy();
			ColorExtension::destroy();
			Vertex::destroy();
		}

		void Point::set(const Point& point, bool deep)
			throw()
		{
			GeometricObject::set(point, deep);
			ColorExtension::set(point);
			Vertex::set(point);
		}

		const Point& Point::operator =(const Point& point)
			throw()
		{
			set(point);
			return *this;
		}

		void Point::get(Point& point, bool deep) const
			throw()
		{
			point.set(*this, deep);
		}

		void Point::swap(Point& point)
			throw()
		{
			GeometricObject::swap(point);
			ColorExtension::swap(point);
			Vertex::swap(point);
		}

		bool Point::isValid() const
			throw()
		{
			return (GeometricObject::isValid() &&
											 Vertex::isValid() );
		}

		void Point::dump(ostream& s, Size depth) const
			throw()
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
			throw()
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void Point::write(ostream & /* s */) const
			throw()
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		bool Point::extract()
			throw()
		{
			return true;  
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/PRIMITIV/point.iC>
#		endif
	
	} // namespace VIEW

} // namespace BALL
