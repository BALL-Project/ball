// $Id: sphere.C,v 1.8 2000/12/22 19:12:18 amoll Exp $

#include <BALL/VIEW/PRIMITIV/sphere.h>

using namespace std;

namespace BALL
{

	namespace VIEW
	{

		Sphere::Sphere()
			throw()
			:	GeometricObject(),
				ColorExtension(),
				Vertex(),
				Radius()
		{
		}

		Sphere::Sphere(const Sphere& sphere, bool deep)
			throw()
			:	GeometricObject(sphere, deep),
				ColorExtension(sphere, deep),
				Vertex(sphere, deep),
				Radius(sphere, deep)
		{
		}

		Sphere::Sphere(const GeometricObject& geometric_object)
			throw()
			:	GeometricObject(geometric_object),
				ColorExtension(),
				Vertex(),
				Radius()
		{
		}

		Sphere::~Sphere()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<Sphere>() << endl;
			#endif 

			destroy();
		}

		void Sphere::clear()
			throw()
		{
			GeometricObject::clear();
			ColorExtension::clear();
			Vertex::clear();
			Radius::clear();
		}

		void Sphere::destroy()
			throw()
		{
			GeometricObject::destroy();
			ColorExtension::destroy();
			Vertex::destroy();
			Radius::destroy();
		}

		void Sphere::set(const Sphere& sphere, bool deep)
			throw()
		{
			GeometricObject::set(sphere, deep);
			ColorExtension::set(sphere, deep);
			Vertex::set(sphere, deep);
			Radius::set(sphere, deep);
		}

		const Sphere& Sphere::operator = (const Sphere& sphere)
			throw()
		{
			set(sphere);
			return *this;
		}

		void Sphere::get(Sphere& sphere, bool deep) const
			throw()
		{
			sphere.set(*this, deep);
		}

		void Sphere::swap(Sphere& sphere)
			throw()
		{
			GeometricObject::swap(sphere);
			ColorExtension::swap(sphere);
			Vertex::swap(sphere);
			Radius::swap(sphere);
		}

		bool Sphere::isValid() const
			throw()
		{
			return (GeometricObject::isValid() && 
							 ColorExtension::isValid() && 
											 Vertex::isValid() &&
										   Radius::isValid());
		}

		void Sphere::dump(ostream& s, Size depth) const
			throw()
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			GeometricObject::dump(s, depth + 1);
			ColorExtension::dump(s, depth + 1);
			Vertex::dump(s, depth + 1);
			Radius::dump(s, depth + 1);

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void Sphere::read(istream & /* s */)
			throw()
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void Sphere::write(ostream & /* s */) const
			throw()
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		bool Sphere::extract()
			throw()
		{
			return true;  
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/PRIMITIV/sphere.iC>
#		endif

	} // namespace VIEW

} // namespace BALL
