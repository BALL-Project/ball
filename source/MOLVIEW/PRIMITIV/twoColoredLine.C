// $Id: twoColoredLine.C,v 1.9 2001/05/13 15:02:41 hekl Exp $

#include <BALL/MOLVIEW/PRIMITIV/twoColoredLine.h>

using namespace std;

namespace BALL
{

	namespace MOLVIEW
	{
		
		TwoColoredLine::TwoColoredLine()
			throw()
			:	GeometricObject(),
				ColorExtension2(),
				Vertex2()
		{
		}

		TwoColoredLine::TwoColoredLine
			(const TwoColoredLine& two_colored_line, bool deep)
			throw()
			:	GeometricObject(two_colored_line, deep),
				ColorExtension2(two_colored_line),
				Vertex2(two_colored_line)
		{
		}

		TwoColoredLine::TwoColoredLine
			(const GeometricObject& geometric_object)
			throw()
			:	GeometricObject(geometric_object),
				ColorExtension2(),
				Vertex2()
		{
		}

		TwoColoredLine::~TwoColoredLine()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<TwoColoredLine>() << endl;
			#endif 

			destroy();
		}

		void TwoColoredLine::clear()
			throw()
		{
			GeometricObject::clear();
			ColorExtension2::clear();
			Vertex2::clear();
		}

		void TwoColoredLine::destroy()
			throw()
		{
			GeometricObject::destroy();
			ColorExtension2::destroy();
			Vertex2::destroy();
		}

		void TwoColoredLine::set(const TwoColoredLine& two_colored_line, bool deep)
			throw()
		{
			GeometricObject::set(two_colored_line, deep);
			ColorExtension2::set(two_colored_line);
			Vertex2::set(two_colored_line);
		}

		const TwoColoredLine& TwoColoredLine::operator = (const TwoColoredLine &two_colored_line)
			throw()
		{
			set(two_colored_line);
			return *this;
		}

		void TwoColoredLine::get(TwoColoredLine& two_colored_line, bool deep) const
			throw()
		{
			two_colored_line.set(*this, deep);
		}

		void TwoColoredLine::swap(TwoColoredLine& two_colored_line)
			throw()
		{
			GeometricObject::swap(two_colored_line);
			ColorExtension2::swap(two_colored_line);
			Vertex2::swap(two_colored_line);
		}

		bool TwoColoredLine::isValid() const
			throw()
		{
			return (GeometricObject::isValid());
		}

		void TwoColoredLine::dump(ostream& s, Size depth) const
			throw()
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			GeometricObject::dump(s, depth + 1);
			ColorExtension2::dump(s, depth + 1);
			Vertex2::dump(s, depth + 1);

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void TwoColoredLine::read(istream & /* s */)
			throw()
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void TwoColoredLine::write(ostream & /* s */) const
			throw()
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		bool TwoColoredLine::extract()
			throw()
		{
			return true;  
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/PRIMITIV/twoColoredLine.iC>
#		endif

	} // namespace MOLVIEW

} // namespace BALL
