// $Id: vertex2.C,v 1.5 2000/03/15 08:44:38 oliver Exp $

#include <BALL/VIEW/KERNEL/vertex2.h>

using namespace std;

namespace BALL
{

	namespace VIEW
	{

		Vertex2::Vertex2()
			:	vertex1_(),
				vertex2_(),
				vertex1_ptr_(&vertex1_),
				vertex2_ptr_(&vertex2_)
		{
		}

		Vertex2::Vertex2(const Vertex2& v, bool deep)
			:	vertex1_(v.vertex1_),
				vertex2_(v.vertex2_)
		{
			if (deep == true)
			{
				vertex1_ptr_ = v.vertex1_ptr_;
				vertex2_ptr_ = v.vertex2_ptr_;
			}
			else
			{
				vertex1_ptr_ = &vertex1_;
				vertex2_ptr_ = &vertex2_;
			}
		}

		Vertex2::~Vertex2()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<Vertex2>() << endl;
			#endif 

			destroy();
		}

		void Vertex2::clear()
		{
			vertex1_.set(0.0);
			vertex2_.set(0.0);
			vertex1_ptr_ = &vertex1_;
			vertex2_ptr_ = &vertex2_;
		}

		void Vertex2::destroy()
		{
			clear();
		}

		void Vertex2::set(const Vertex2& v, bool deep)
		{
			vertex1_.set(v.vertex1_, deep);
			vertex2_.set(v.vertex2_, deep);
			
			if (deep == true)
			{
				vertex1_ptr_ = v.vertex1_ptr_;
				vertex2_ptr_ = v.vertex2_ptr_;
			}
			else
			{
				vertex1_ptr_ = &vertex1_;
				vertex2_ptr_ = &vertex2_;
			}
		}

		Vertex2& Vertex2::operator = (const Vertex2& v)
		{
			set(v);

			return *this;
		}

		void Vertex2::get(Vertex2& v, bool deep) const
		{
			v.set(*this, deep);
		}

		void Vertex2::swap(Vertex2& v)
		{
			Vector3 *temp_vector_ptr = vertex2_ptr_;

			if (v.vertex1_ptr_ != &v.vertex1_)
			{
				vertex1_ptr_ = v.vertex1_ptr_;
				
				if (temp_vector_ptr != &vertex1_)
				{
					v.vertex1_ptr_ = temp_vector_ptr;
				}
				else
				{
					v.vertex1_ptr_ = &v.vertex1_;
				}
			}
			else if (vertex1_ptr_ != &vertex1_)
			{
				v.vertex1_ptr_ = temp_vector_ptr;
				
				vertex1_ptr_  = &v.vertex1_;
			}  

			if (v.vertex2_ptr_ != &v.vertex2_)
			{
				vertex2_ptr_ = v.vertex2_ptr_;
				
				if (temp_vector_ptr != &vertex2_)
				{
					v.vertex2_ptr_ = temp_vector_ptr;
				}
				else
				{
					v.vertex2_ptr_ = &v.vertex2_;
				}
			}
			else if (vertex2_ptr_ != &vertex2_)
			{
				v.vertex2_ptr_ = temp_vector_ptr;
				
				vertex2_ptr_ = &v.vertex2_;
			}  

			vertex1_.swap(v.vertex1_);
			vertex2_.swap(v.vertex2_);
		}

		bool Vertex2::isValid() const
		{
			return (bool)(vertex1_.isValid() == true
										&& vertex2_.isValid() == true);
		}

		void Vertex2::dump
			(ostream& s, Size depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_DEPTH(s, depth);
			s << "vertex1 : " << vertex1_ << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "*vertex1 : " << (*vertex1_ptr_) << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "vertex2 : " << vertex2_ << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "*vertex2 : " << (*vertex2_ptr_) << endl;

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void Vertex2::read(istream& /* s */)
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void Vertex2::write(ostream& /* s */) const
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/KERNEL/vertex2.iC>
#		endif 

	} // namespace VIEW

} // namespace BALL
