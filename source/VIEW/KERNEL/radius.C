// $Id: radius.C,v 1.1 1999/08/26 08:02:46 oliver Exp $

#include <BALL/VIEW/KERNEL/radius.h>

namespace BALL
{

	namespace VIEW
	{

		Radius::Radius()
			:	radius_((Real)1)
		{
		}

		Radius::Radius
			(const Radius& radius, bool /* deep */)
			:	radius_(radius.radius_)
		{
		}

		Radius::~Radius()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI<Radius>::getName() << endl;
			#endif 

			destroy();
		}

		void Radius::clear()
		{
			radius_ = (Real)1;
		}

		void Radius::destroy()
		{
			clear();
		}

		void Radius::set(const Radius& radius, bool /* deep */)
		{
			radius_ = radius.radius_;
		}

		Radius& Radius::operator = (const Radius& radius)
		{
			set(radius);

			return *this;
		}

		void Radius::get(Radius& radius, bool deep) const
		{
			radius.set(*this, deep);
		}

		void Radius::swap(Radius& radius)
		{
			Real temp = radius_;

			radius_ = radius.radius_;

			radius.radius_ = temp;
		}

		bool Radius::isValid() const
		{
			return true;
		}

		void Radius::dump(ostream& s, unsigned long depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_DEPTH(s, depth);
			s << "radius : " << radius_ << endl;

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void Radius::read(istream & /* s */)
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void Radius::write(ostream&  /* s */) const
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/KERNEL/radius.iC>
#		endif 

	} // namespace VIEW

} // namespace BALL
