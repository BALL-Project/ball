// $Id: normal.C,v 1.5 2001/02/04 16:14:27 hekl Exp $

#include <BALL/VIEW/KERNEL/normal.h>

using namespace std;

namespace BALL
{

	namespace VIEW
	{

		NormalVector::NormalVector()
			:	normal_(1.0, 0.0, 0.0)
		{
		}

		NormalVector::NormalVector(const NormalVector& n)
			:	normal_(n.normal_)
		{
		}

		NormalVector::~NormalVector()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<NormalVector>() << endl;
			#endif 

			destroy();
		}

		void NormalVector::clear()
		{
			normal_.set(1.0, 0.0, 0.0);
		}

		void NormalVector::destroy()
		{
		}

		void NormalVector::set(const NormalVector& n)
		{
			normal_ = n.normal_;
		}

		NormalVector& NormalVector::operator = (const NormalVector& n)
		{
			set(n);

			return *this;
		}

		void NormalVector::get(NormalVector& n) const
		{
			n.set(*this);
		}

		void NormalVector::swap(NormalVector& n)
		{
			normal_.swap(n.normal_);
		}

		bool NormalVector::isValid() const
		{
			return normal_.isValid();
		}

		void NormalVector::dump
			(ostream& s, Size depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_DEPTH(s, depth);
			s << "normal: " << normal_ << endl;

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void 
		NormalVector::read
			(istream & /* s */)
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void 
		NormalVector::write
			(ostream & /* s */) const
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/KERNEL/normal.iC>
#		endif 

	} // namespace VIEW

} // namespace BALL
