// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: glObject.C,v 1.4.2.1 2003/01/07 13:23:25 anker Exp $

#include <BALL/VIEW/GUI/KERNEL/glObject.h>

using namespace std;

namespace BALL
{

	namespace VIEW
	{

		GLObject::GLObject()
			throw()
			:	GL_primitive_manager_(NULL)
		{
		}

		GLObject::GLObject(const GLObject& GL_object)
			throw()
			:	GL_primitive_manager_(GL_object.GL_primitive_manager_)
		{
		}

		GLObject::~GLObject()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<GLObject>() << endl;
			#endif 

			destroy();
		}

		void GLObject::clear()
			throw()
		{
			GL_primitive_manager_ = 0;
		}

		void GLObject::destroy()
			throw()
		{
		}

		void GLObject::set(const GLObject& GL_object)
			throw()
		{
			GL_primitive_manager_ = GL_object.GL_primitive_manager_;
		}

		const GLObject& GLObject::operator = (const GLObject& GL_object)
			throw()
		{
			set(GL_object);

			return *this;
		}

		void GLObject::get(GLObject& GL_object) const
			throw()
		{
			GL_object.set(*this);
		}

		void GLObject::swap(GLObject& GL_object)
			throw()
		{
			GLPrimitiveManager *temp = GL_primitive_manager_;
			GL_primitive_manager_ = GL_object.GL_primitive_manager_;
			GL_object.GL_primitive_manager_ = temp;
		}

		bool GLObject::isValid() const
			throw()
		{
			if (GL_primitive_manager_ != 0)
			{
				return (GL_primitive_manager_->isValid());
			}
			
			return false;
		}

		void GLObject::dump
			(ostream& s, Size depth) const
			throw()
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_DEPTH(s, depth);
			s << "displaylistmanager: " << (void *)GL_primitive_manager_ << endl;

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		bool GLObject::draw(bool /* with_names */)
			throw()
		{
			return true;
		}

		bool GLObject::drawUserDefined()
			throw()
		{
			return true;
		}

		bool GLObject::extract()
			throw()
		{
			return true;
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/KERNEL/glObject.iC>
#		endif

	} // namespace VIEW

} // namespace BALL
