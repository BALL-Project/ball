// $Id: basesphere.C,v 1.5 2000/12/12 16:18:46 oliver Exp $

#include <BALL/VIEW/PRIMITIV/basesphere.h>

namespace BALL
{

	namespace VIEW
	{

		BaseSphere::BaseSphere()
			:	GeometricObject(),
				ColorExtension(),
				Vertex(),
				Radius()
		{
		}

		BaseSphere::BaseSphere(const BaseSphere& base_sphere, bool deep)
			:	GeometricObject(base_sphere, deep),
				ColorExtension(base_sphere, deep),
				Vertex(base_sphere, deep),
				Radius(base_sphere, deep)
		{
		}

		BaseSphere::BaseSphere(const GeometricObject& geometric_object)
			:	GeometricObject(geometric_object),
				ColorExtension(),
				Vertex(),
				Radius()
		{
		}

		BaseSphere::~BaseSphere()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this << " of class " 
					<< RTTI::getName<BaseSphere>() << endl;
			#endif 

			destroy();
		}

		void BaseSphere::clear()
			throw()
		{
			GeometricObject::clear();
			ColorExtension::clear();
			Vertex::clear();
			Radius::clear();
		}

		void BaseSphere::destroy()
			throw()
		{
			GeometricObject::destroy();
			ColorExtension::destroy();
			Vertex::destroy();
			Radius::destroy();
		}

		void BaseSphere::set(const BaseSphere& base_sphere, bool deep)
		{
			GeometricObject::set(base_sphere, deep);
			ColorExtension::set(base_sphere, deep);
			Vertex::set(base_sphere, deep);
			Radius::set(base_sphere, deep);
		}

		BaseSphere& BaseSphere::operator = (const BaseSphere& base_sphere)
		{
			set(base_sphere);

			return *this;
		}

		void BaseSphere::get(BaseSphere& base_sphere, bool deep) const
		{
			base_sphere.set(*this, deep);
		}

		void BaseSphere::swap(BaseSphere& base_sphere)
		{
			GeometricObject::swap(base_sphere);
			ColorExtension::swap(base_sphere);
			Vertex::swap(base_sphere);
			Radius::swap(base_sphere);
		}

		bool BaseSphere::isValid() const
		{
			return (bool)(GeometricObject::isValid() == true
										&& ColorExtension::isValid() == true
										&& Vertex::isValid() == true
										&& Radius::isValid() == true);
		}

		void BaseSphere::dump
			(ostream& s, Size depth) const
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

		void BaseSphere::read(istream & /* s */)
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void BaseSphere::write(ostream & /* s */) const
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		bool BaseSphere::extract()
		{
			return true;  
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/PRIMITIV/basesphere.iC>
#		endif

	} // namespace VIEW

} // namespace BALL
