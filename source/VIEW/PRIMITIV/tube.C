// $Id: tube.C,v 1.4 1999/12/30 18:05:52 oliver Exp $

#include <BALL/VIEW/PRIMITIV/tube.h>

using namespace std;

namespace BALL
{

	namespace VIEW
	{

		Tube::Tube()
			:	BaseTube(),
				ColorExtension()
		{
		}

		Tube::Tube(const Tube& tube, bool deep)
			:	BaseTube(tube, deep),
				ColorExtension(tube, deep)
		{
		}

		Tube::Tube(const GeometricObject& geometric_object)
			:	BaseTube(geometric_object),
				ColorExtension()
		{
		}

		Tube::~Tube()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<Tube>() << endl;
			#endif 

			destroy();
		}

		void Tube::clear()
		{
			BaseTube::clear();
			ColorExtension::clear();
		}

		void Tube::destroy()
		{
			BaseTube::destroy();
			ColorExtension::destroy();
		}

		void Tube::set(const Tube& tube, bool deep)
		{
			BaseTube::set(tube, deep);
			ColorExtension::set(tube, deep);
		}

		Tube& Tube::operator = (const Tube& tube)
		{
			set(tube);

			return *this;
		}

		void Tube::get(Tube& tube, bool deep) const
		{
			tube.set(*this, deep);
		}

		void Tube::swap(Tube& tube)
		{
			BaseTube::swap(tube);
			ColorExtension::swap(tube);
		}

		bool Tube::isValid() const
		{
			return (bool)(BaseTube::isValid() == true
										&& ColorExtension::isValid() == true);
		}

		void Tube::dump
			(ostream& s, Size depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BaseTube::dump(s, depth + 1);
			ColorExtension::dump(s, depth + 1);

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void Tube::read(istream & /* s */)
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void Tube::write(ostream & /* s */) const
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		bool Tube::extract()
		{
			return true;  
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/PRIMITIV/tube.iC>
#		endif

	} // namespace VIEW

} // namespace BALL
