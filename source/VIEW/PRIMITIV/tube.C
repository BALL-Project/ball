// $Id: tube.C,v 1.7 2000/12/21 17:03:46 amoll Exp $

#include <BALL/VIEW/PRIMITIV/tube.h>

using namespace std;

namespace BALL
{

	namespace VIEW
	{

		Tube::Tube()
			throw()
			:	BaseTube(),
				ColorExtension()
		{
		}

		Tube::Tube(const Tube& tube, bool deep)
			throw()
			:	BaseTube(tube, deep),
				ColorExtension(tube, deep)
		{
		}

		Tube::Tube(const GeometricObject& geometric_object)
			throw()
			:	BaseTube(geometric_object),
				ColorExtension()
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
			BaseTube::clear();
			ColorExtension::clear();
		}

		void Tube::destroy()
			throw()
		{
			BaseTube::destroy();
			ColorExtension::destroy();
		}

		void Tube::set(const Tube& tube, bool deep)
			throw()
		{
			BaseTube::set(tube, deep);
			ColorExtension::set(tube, deep);
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
			BaseTube::swap(tube);
			ColorExtension::swap(tube);
		}

		bool Tube::isValid() const
			throw()
		{
			return (BaseTube::isValid()	&& ColorExtension::isValid());
		}

		void Tube::dump(ostream& s, Size depth) const
			throw()
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BaseTube::dump(s, depth + 1);
			ColorExtension::dump(s, depth + 1);

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
