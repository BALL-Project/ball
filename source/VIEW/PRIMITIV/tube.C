// $Id: tube.C,v 1.8.4.2 2002/12/07 02:48:16 amoll Exp $

#include <BALL/VIEW/PRIMITIV/tube.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

		Tube::Tube()
			throw()
			: GeometricObject(),
			  ColorExtension(),
   			Vertex2(),
				radius_(1)
		{
		}

		Tube::Tube(const Tube& tube, bool deep)
			throw()
			: GeometricObject(tube, deep),
			  ColorExtension(tube),
   			Vertex2(tube),
				radius_(1)
		{
		}

		Tube::Tube(const GeometricObject& geometric_object)
			throw()
			: GeometricObject(geometric_object),
			  ColorExtension(),
   			Vertex2(),
				radius_(1)
		{
		}

		Tube::~Tube()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "Destructing object " << (void *)this 
										<< " of class " << RTTI::getName<Tube>() << endl;
			#endif 

			destroy();
		}

		void Tube::clear()
			throw()
		{
			GeometricObject::clear();
			ColorExtension::clear();
			Vertex2::clear();
			radius_ = 1;
		}

		void Tube::destroy()
			throw()
		{
			GeometricObject::destroy();
			ColorExtension::destroy();
			Vertex2::destroy();
		}

		void Tube::set(const Tube& tube, bool deep)
			throw()
		{
			GeometricObject::set(tube, deep);
			ColorExtension::set(tube);
			Vertex2::set(tube);
			radius_ = tube.radius_;
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
			ColorExtension::swap(tube);
			Vertex2::swap(tube);

			Real temp = tube.radius_;
			tube.radius_ = radius_;
			radius_ = temp;
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
			ColorExtension::dump(s, depth + 1);
			Vertex2::dump(s, depth + 1);
			s << "radius : " << radius_ << endl;

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
