// $Id: basetube.C,v 1.7 2000/12/21 17:03:46 amoll Exp $

#include <BALL/VIEW/PRIMITIV/basetube.h>

using namespace std;

namespace BALL
{

	namespace VIEW
	{

		BaseTube::BaseTube()
			throw()
			:	GeometricObject(),
				Radius(),
				Vertex2()
		{
		}

		BaseTube::BaseTube(const BaseTube& base_tube, bool deep)
			throw()
			:	GeometricObject(base_tube, deep),
				Radius(base_tube, deep),
				Vertex2(base_tube, deep)
		{
		}

		BaseTube::BaseTube(const GeometricObject& geometric_object)
			throw()
			:	GeometricObject(geometric_object),
				Radius(),
				Vertex2()
		{
		}

		BaseTube::~BaseTube()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<BaseTube>() << endl;
			#endif 

			destroy();
		}

		void BaseTube::clear()
			throw()
		{
			GeometricObject::clear();
			Vertex2::clear();
			Radius::clear();
		}

		void BaseTube::destroy()
			throw()
		{
			GeometricObject::destroy();
			Vertex2::destroy();
			Radius::destroy();
		}

		void BaseTube::set(const BaseTube& base_tube, bool deep)
			throw()
		{
			GeometricObject::set(base_tube, deep);
			Vertex2::set(base_tube, deep);
			Radius::set(base_tube, deep);
		}

		const BaseTube& BaseTube::operator = (const BaseTube& base_tube)
			throw()
		{
			set(base_tube);
			return *this;
		}

		void BaseTube::get(BaseTube& base_tube, bool deep) const
			throw()
		{
			base_tube.set(*this, deep);
		}

		void BaseTube::swap(BaseTube& base_tube)
			throw()
		{
			GeometricObject::swap(base_tube);
			Vertex2::swap(base_tube);
			Radius::swap(base_tube);
		}

		bool BaseTube::isValid() const
			throw()
		{
			return (GeometricObject::isValid() &&
										  Vertex2::isValid() && 
										   Radius::isValid());
		}

		void BaseTube::dump(ostream& s, Size depth) const
			throw()
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			GeometricObject::dump(s, depth + 1);
			Vertex2::dump(s, depth + 1);
			Radius::dump(s, depth + 1);

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void BaseTube::read(istream & /* s */)
			throw()
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void BaseTube::write(ostream & /* s */) const
			throw()
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		bool BaseTube::extract()
			throw()
		{
			return true;  
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/PRIMITIV/basetube.iC>
#		endif

	} // namespace VIEW

} // namespace BALL
