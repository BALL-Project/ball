// $Id: normal.C,v 1.6 2001/05/13 14:28:38 hekl Exp $

#include <BALL/VIEW/KERNEL/normal.h>

using namespace std;

namespace BALL
{

	namespace VIEW
	{

		NormalVector::NormalVector()
			throw()
			:	normal_(1.0, 0.0, 0.0)
		{
		}

		NormalVector::NormalVector(const NormalVector& n)
			throw()
			:	normal_(n.normal_)
		{
		}

		NormalVector::~NormalVector()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<NormalVector>() << endl;
			#endif 

			destroy();
		}

		void NormalVector::clear()
			throw()
		{
			normal_.set(1.0, 0.0, 0.0);
		}

		void NormalVector::destroy()
			throw()
		{
		}

		void NormalVector::set(const NormalVector& n)
			throw()
		{
			normal_ = n.normal_;
		}

		const NormalVector& NormalVector::operator = (const NormalVector& n)
			throw()
		{
			set(n);

			return *this;
		}

		void NormalVector::get(NormalVector& n) const
			throw()
		{
			n.set(*this);
		}

		void NormalVector::swap(NormalVector& n)
			throw()
		{
			normal_.swap(n.normal_);
		}

		bool NormalVector::isValid() const
			throw()
		{
			return normal_.isValid();
		}

		void NormalVector::dump
			(ostream& s, Size depth) const
			throw()
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
			throw()
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void 
		NormalVector::write
			(ostream & /* s */) const
			throw()
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/KERNEL/normal.iC>
#		endif 

	} // namespace VIEW

} // namespace BALL
