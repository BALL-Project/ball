// $Id: twoColoredTube.C,v 1.8 2001/02/04 16:19:04 hekl Exp $

#include <BALL/MOLVIEW/PRIMITIV/twoColoredTube.h>

using namespace std;

namespace BALL
{

	namespace MOLVIEW
	{

		TwoColoredTube::TwoColoredTube()
			:	BaseTube(),
				ColorExtension2()
		{
		}

		TwoColoredTube::TwoColoredTube
			(const TwoColoredTube& two_colored_tube, bool deep)
			:	BaseTube(two_colored_tube, deep),
				ColorExtension2(two_colored_tube)
		{
		}

		TwoColoredTube::TwoColoredTube
			(const VIEW::GeometricObject& geometric_object)
			:	BaseTube(geometric_object),
				ColorExtension2()
		{
		}

		TwoColoredTube::~TwoColoredTube()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<TwoColoredTube>() << endl;
			#endif 

			destroy();
		}

		void TwoColoredTube::clear()
			throw()
		{
			BaseTube::clear();
			ColorExtension2::clear();
		}

		void TwoColoredTube::destroy()
			throw()
		{
			BaseTube::destroy();
			ColorExtension2::destroy();
		}

		void TwoColoredTube::set(const TwoColoredTube& two_colored_tube, bool deep)
		{
			BaseTube::set(two_colored_tube, deep);
			ColorExtension2::set(two_colored_tube);
		}

		const TwoColoredTube& TwoColoredTube::operator = (const TwoColoredTube& two_colored_tube)
		{
			set(two_colored_tube);
			return *this;
		}

		void TwoColoredTube::get(TwoColoredTube& two_colored_tube, bool deep) const
		{
			two_colored_tube.set(*this, deep);
		}

		void TwoColoredTube::swap(TwoColoredTube& two_colored_tube)
		{
			BaseTube::swap(two_colored_tube);
			ColorExtension2::swap(two_colored_tube);
		}

		bool TwoColoredTube::isValid() const
			throw()
		{
			return (BaseTube::isValid());
		}

		void TwoColoredTube::dump(ostream& s, Size depth) const
			throw()
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BaseTube::dump(s, depth + 1);
			ColorExtension2::dump(s, depth + 1);

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void TwoColoredTube::read(istream & /* s */)
			throw()
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void TwoColoredTube::write(ostream & /* s */) const
			throw()
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		bool TwoColoredTube::extract()
			throw()
		{
			return true;  
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/PRIMITIV/twoColoredTube.iC>
#		endif

	} // namespace MOLVIEW

} // namespace BALL
