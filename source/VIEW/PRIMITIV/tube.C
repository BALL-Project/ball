// $Id: tube.C,v 1.8 2001/02/04 16:14:28 hekl Exp $

#include <BALL/VIEW/PRIMITIV/tube.h>

using namespace std;

namespace BALL
{

	namespace VIEW
	{

		Tube::Tube()
			throw()
			: GeometricObject(),
				Radius(),
			  ColorExtension(),
   			Vertex2()
		{
		}

		Tube::Tube(const Tube& tube, bool deep)
			throw()
			: GeometricObject(tube, deep),
				Radius(tube),
			  ColorExtension(tube),
   			Vertex2(tube)
		{
		}

		Tube::Tube(const GeometricObject& geometric_object)
			throw()
			: GeometricObject(geometric_object),
				Radius(),
			  ColorExtension(),
   			Vertex2()
		{
		}

		Tube::~Tube()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<Tube>() << endl;
			#endif 

			destroy();
		}

		void Tube::clear()
			throw()
		{
			GeometricObject::clear();
			Radius::clear();
			ColorExtension::clear();
			Vertex2::clear();
		}

		void Tube::destroy()
			throw()
		{
			GeometricObject::destroy();
			Radius::destroy();
			ColorExtension::destroy();
			Vertex2::destroy();
		}

		void Tube::set(const Tube& tube, bool deep)
			throw()
		{
			GeometricObject::set(tube, deep);
			Radius::set(tube);
			ColorExtension::set(tube);
			Vertex2::set(tube);
		}

		const Tube& Tube::operator = (const Tube& tube)
			throw()
		{
			set(tube);
			return *this;
		}

		void Tube::get(Tube& tube, bool deep) const
			throw()
		{
			tube.set(*this, deep);
		}

		void Tube::swap(Tube& tube)
			throw()
		{
			GeometricObject::swap(tube);
			Radius::swap(tube);
			ColorExtension::swap(tube);
			Vertex2::swap(tube);
		}

		bool Tube::isValid() const
			throw()
		{
			return (GeometricObject::isValid()	
							&& Vertex2::isValid());
		}

		void Tube::dump(ostream& s, Size depth) const
			throw()
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			GeometricObject::dump(s, depth + 1);
			Radius::dump(s, depth + 1);
			ColorExtension::dump(s, depth + 1);
			Vertex2::dump(s, depth + 1);

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void Tube::read(istream & /* s */)
			throw()
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void Tube::write(ostream & /* s */) const
			throw()
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		bool Tube::extract()
			throw()
		{
			return true;  
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/PRIMITIV/tube.iC>
#		endif

	} // namespace VIEW

} // namespace BALL
