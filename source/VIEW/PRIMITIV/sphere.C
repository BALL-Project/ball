// $Id: sphere.C,v 1.9.4.3 2002/12/08 16:50:04 oliver Exp $

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
				radius_(1)
		{
		}

		Sphere::Sphere(const Sphere& sphere, bool deep)
			throw()
			:	GeometricObject(sphere, deep),
				ColorExtension(sphere),
				Vertex(sphere),
				radius_(sphere.radius_)
		{
		}

		Sphere::Sphere(const GeometricObject& geometric_object)
			throw()
			:	GeometricObject(geometric_object),
				ColorExtension(),
				Vertex(),
				radius_(1)
		{
		}

		Sphere::~Sphere()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.erro() << "Destructing object " << (void *)this 
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
			radius_ = 1;
		}

		void Sphere::destroy()
			throw()
		{
			GeometricObject::destroy();
			ColorExtension::destroy();
			Vertex::destroy();
		}

		void Sphere::set(const Sphere& sphere, bool deep)
			throw()
		{
			GeometricObject::set(sphere, deep);
			ColorExtension::set(sphere);
			Vertex::set(sphere);
			radius_ = sphere.radius_;
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
			Real temp = sphere.radius_;
			sphere.radius_ = radius_;
			radius_ = temp;
		}

		bool Sphere::isValid() const
			throw()
		{
			return (GeometricObject::isValid() && 
											 Vertex::isValid());
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
			s << "radius : " << radius_ << endl;

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
		
	} // namespace VIEW

} // namespace BALL
