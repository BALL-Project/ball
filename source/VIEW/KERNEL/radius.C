// $Id: radius.C,v 1.7 2001/05/13 14:28:38 hekl Exp $

#include <BALL/VIEW/KERNEL/radius.h>
#include <BALL/COMMON/exception.h>
using namespace std;

namespace BALL
{

	namespace VIEW
	{

		Radius::Radius()
			throw()
			:	radius_((Real)1)
		{
		}

		Radius::Radius
			(const Radius& radius)
			throw()
			:	radius_(radius.radius_)
		{
		}

		Radius::~Radius()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<Radius>() << endl;
			#endif 

			destroy();
		}

		void Radius::clear()
			throw()
		{
			radius_ = (Real)1;
		}

		void Radius::destroy()
			throw()
		{
		}

		void Radius::set(const Radius& radius)
			throw()
		{
			radius_ = radius.radius_;
		}

		const Radius& Radius::operator = (const Radius& radius)
			throw()
		{
			set(radius);

			return *this;
		}

		void Radius::get(Radius& radius) const
			throw()
		{
			radius.set(*this);
		}

		void Radius::swap(Radius& radius)
			throw()
		{
			Real temp = radius_;

			radius_ = radius.radius_;

			radius.radius_ = temp;
		}

		void Radius::dump(ostream& s, Size depth) const
			throw()
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_DEPTH(s, depth);
			s << "radius : " << radius_ << endl;

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void Radius::read(istream & /* s */)
			throw()
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void Radius::write(ostream&  /* s */) const
			throw()
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/KERNEL/radius.iC>
#		endif 

	} // namespace VIEW

} // namespace BALL
