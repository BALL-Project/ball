// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: vertex1.C,v 1.1 2003/08/26 12:38:09 oliver Exp $

#include <BALL/VIEW/KERNEL/vertex1.h>

using namespace std;

namespace BALL
{

	namespace VIEW
	{

		Vertex::Vertex()
			throw()
			:	vertex1_(),
				vertex1_ptr_(&vertex1_)
		{
		}

		Vertex::Vertex(const Vertex& v)
			throw()
			:	vertex1_(v.vertex1_),
				vertex1_ptr_(v.vertex1_ptr_)
		{
		}

		Vertex::~Vertex()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this << " of class " << RTTI::getName<Vertex>() << endl;
			#endif 
		}

		void Vertex::clear()
			throw()
		{
			vertex1_.set(0.0);
			vertex1_ptr_ = &vertex1_;
		}

		void Vertex::set(const Vertex& v)
			throw()
		{
			vertex1_.set(v.vertex1_);
			vertex1_ptr_ = v.vertex1_ptr_;
		}

		const Vertex& Vertex::operator = (const Vertex& v)
			throw()
		{
			set(v);

			return *this;
		}

		void Vertex::get(Vertex& v) const
			throw()
		{
			v.set(*this);
		}

		void Vertex::swap(Vertex& v)
			throw()
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
			throw()
		{
			return vertex1_.isValid();
		}

		void Vertex::dump(ostream& s, Size depth) const
			throw()
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

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/KERNEL/vertex1.iC>
#		endif 
		
	} // namespace VIEW
} // namespace BALL
