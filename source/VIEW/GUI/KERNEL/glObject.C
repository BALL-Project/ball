// $Id: glObject.C,v 1.3 2001/02/04 16:14:26 hekl Exp $

#include <BALL/VIEW/GUI/KERNEL/glObject.h>

using namespace std;

namespace BALL
{

	namespace VIEW
	{

		GLObject::GLObject()
			:	GL_primitive_manager_(NULL)
		{
		}

		GLObject::GLObject(const GLObject& GL_object)
			:	GL_primitive_manager_(GL_object.GL_primitive_manager_)
		{
		}

		GLObject::~GLObject()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<GLObject>() << endl;
			#endif 

			destroy();
		}

		void GLObject::clear()
		{
			GL_primitive_manager_ = 0;
		}

		void GLObject::destroy()
		{
		}

		void GLObject::set(const GLObject& GL_object)
		{
			GL_primitive_manager_ = GL_object.GL_primitive_manager_;
		}

		GLObject& GLObject::operator = (const GLObject& GL_object)
		{
			set(GL_object);

			return *this;
		}

		void GLObject::get(GLObject& GL_object) const
		{
			GL_object.set(*this);
		}

		void GLObject::swap(GLObject& GL_object)
		{
			GLPrimitiveManager *temp = GL_primitive_manager_;
			GL_primitive_manager_ = GL_object.GL_primitive_manager_;
			GL_object.GL_primitive_manager_ = temp;
		}

		bool GLObject::isValid() const
		{
			if (GL_primitive_manager_ != 0)
			{
				return (GL_primitive_manager_->isValid());
			}
			
			return false;
		}

		void GLObject::dump
			(ostream& s, Size depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_DEPTH(s, depth);
			s << "displaylistmanager: " << (void *)GL_primitive_manager_ << endl;

			BALL_DUMP_STREAM_SUFFIX(s);
		}
				
		void GLObject::read(istream & /* s */)
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void GLObject::write(ostream & /* s */) const
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		bool GLObject::draw(bool /* with_names */)
		{
			return true;
		}

		bool GLObject::drawUserDefined()
		{
			return true;
		}

		bool GLObject::extract()
		{
			return true;
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/KERNEL/glObject.iC>
#		endif

	} // namespace VIEW

} // namespace BALL
