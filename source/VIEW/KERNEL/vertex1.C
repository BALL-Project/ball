// $Id: vertex1.C,v 1.3 1999/12/28 18:00:24 oliver Exp $

#include <BALL/VIEW/KERNEL/vertex1.h>

using namespace std;

namespace BALL
{

	namespace VIEW
	{

		Vertex::Vertex()
			:	vertex1_(),
				vertex1_ptr_(&vertex1_)
		{
		}

		Vertex::Vertex(const Vertex& v, bool deep)
			:	vertex1_(v.vertex1_, deep),
				vertex1_ptr_(v.vertex1_ptr_)
		{
		}

		Vertex::~Vertex()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this << " of class " << RTTI<Vertex>::getName() << endl;
			#endif 

			destroy();
		}

		void Vertex::clear()
		{
			vertex1_.set(0.0);
			vertex1_ptr_ = &vertex1_;
		}

		void Vertex::destroy()
		{
			clear();
		}

		void Vertex::set(const Vertex& v, bool deep)
		{
			vertex1_.set(v.vertex1_, deep);
			vertex1_ptr_ = v.vertex1_ptr_;
		}

		Vertex& Vertex::operator = (const Vertex& v)
		{
			set(v);

			return *this;
		}

		void Vertex::get(Vertex& v, bool deep) const
		{
			v.set(*this, deep);
		}

		void Vertex::swap(Vertex& v)
		{
			Vector3 *tmp_vector = vertex1_ptr_;

			if (v.vertex1_ptr_ != &v.vertex1_)
			{
				vertex1_ptr_ = v.vertex1_ptr_;
				
				if (tmp_vector != &vertex1_)
				{
					v.vertex1_ptr_ = tmp_vector;
				}
				else
				{
					v.vertex1_ptr_ = &v.vertex1_;
				}
			}
			else if (vertex1_ptr_ != &vertex1_)
			{
				v.vertex1_ptr_ = tmp_vector;
				
				vertex1_ptr_ = &v.vertex1_;
			}  

			vertex1_.swap(v.vertex1_);
		}

		bool Vertex::isValid() const
		{
			return (bool)(vertex1_.isValid() == true);
		}

		void Vertex::dump
			(ostream& s, Size depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_DEPTH(s, depth);
			s << "vertex1 : " << vertex1_ << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "*vertex1 : " << (*vertex1_ptr_) << endl;

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void Vertex::read(istream & /* s */)
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void Vertex::write(ostream & /* s */) const
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/KERNEL/vertex1.iC>
#		endif 
		
	} // namespace VIEW

} // namespace BALL
