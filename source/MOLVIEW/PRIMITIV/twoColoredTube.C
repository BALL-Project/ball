// $Id: twoColoredTube.C,v 1.1 1999/08/26 08:02:48 oliver Exp $


#include <BALL/MOLVIEW/PRIMITIV/twoColoredTube.h>

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
				ColorExtension2(two_colored_tube, deep)
		{
		}

		TwoColoredTube::TwoColoredTube
			(const VIEW::GeometricObject& geometric_object)
			:	BaseTube(geometric_object),
				ColorExtension2()
		{
		}

		TwoColoredTube::~TwoColoredTube()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI<TwoColoredTube>::getName() << endl;
			#endif 

			destroy();
		}

		void TwoColoredTube::clear()
		{
			BaseTube::clear();
			ColorExtension2::clear();
		}

		void TwoColoredTube::destroy()
		{
			BaseTube::destroy();
			ColorExtension2::destroy();
		}

		void TwoColoredTube::set
			(const TwoColoredTube& two_colored_tube, bool deep)
		{
			BaseTube::set(two_colored_tube, deep);
			ColorExtension2::set(two_colored_tube, deep);
		}

		TwoColoredTube& TwoColoredTube::operator =
			(const TwoColoredTube& two_colored_tube)
		{
			set(two_colored_tube);

			return *this;
		}

		void TwoColoredTube::get
			(TwoColoredTube& two_colored_tube, bool deep) const
		{
			two_colored_tube.set(*this, deep);
		}

		void TwoColoredTube::swap
			(TwoColoredTube& two_colored_tube)
		{
			BaseTube::swap(two_colored_tube);
			ColorExtension2::swap(two_colored_tube);
		}

		bool TwoColoredTube::isValid() const
		{
			return (bool)(BaseTube::isValid() == true
										&& ColorExtension2::isValid() == true);
		}

		void TwoColoredTube::dump
			(ostream& s, unsigned long depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BaseTube::dump(s, depth + 1);
			ColorExtension2::dump(s, depth + 1);

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void TwoColoredTube::read(istream & /* s */)
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void TwoColoredTube::write(ostream & /* s */) const
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		bool TwoColoredTube::export()
		{
			return true;  
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/PRIMITIV/twoColoredTube.iC>
#		endif

	} // namespace MOLVIEW

} // namespace BALL
