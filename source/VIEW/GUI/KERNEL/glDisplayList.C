// $Id: glDisplayList.C,v 1.1 2000/09/23 13:28:33 hekl Exp $

#include <BALL/VIEW/GUI/KERNEL/glDisplayList.h>
#include <BALL/COMMON/exception.h>

using namespace std;

namespace BALL
{

	namespace VIEW
	{

		const GLDisplayList::GLList 
		GLDisplayList::DISPLAYLIST_NOT_DEFINED = (GLDisplayList::GLList)0;


		GLDisplayList::GLDisplayList()
			:	compile_(true),
				GL_list_(0)
		{
		}

		GLDisplayList::GLDisplayList
			(const GLDisplayList& GL_display_list, bool /* deep */)
			:	compile_(GL_display_list.compile_),
				GL_list_(0)
		{
		}

		GLDisplayList::~GLDisplayList()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<GLDisplayList>() << endl;
			#endif 

			destroy();
		}

		void GLDisplayList::clear()
		{
			compile_ = true;
		}

		void GLDisplayList::destroy()
		{
			if (GL_list_ != 0)
			{
				glDeleteLists(GL_list_, 1);

				GL_list_ = 0;
			}

			clear();
		}

		void GLDisplayList::set
			(const GLDisplayList& GL_display_list, bool /* deep */)
		{
			compile_ = GL_display_list.compile_;
		}

		GLDisplayList& GLDisplayList::operator =
			(const GLDisplayList& GL_display_list)
		{
			set(GL_display_list);

			return *this;
		}

		void GLDisplayList::get
			(GLDisplayList& GL_display_list, bool deep) const
		{
			GL_display_list.set(*this, deep);
		}

		void GLDisplayList::swap(GLDisplayList& GL_display_list)
		{
			bool tmp = compile_;
			compile_ = GL_display_list.compile_;
			GL_display_list.compile_ = tmp;
		}

		void GLDisplayList::startDefinition()
		{
			if (GL_list_ == 0)
			{
				int current_index;

				glGetIntegerv(GL_LIST_INDEX, &current_index);

				BALL_PRECONDITION
					(current_index == 0,
					 BALL_VIEW_GLDISPLAYLIST_ERROR_HANDLER
					 (GLDisplayList::ERROR__NESTED_DISPLAY_LIST_DEFINITION));

				GL_list_ = glGenLists(1);
				
				BALL_PRECONDITION
					(GL_list_ != GLDisplayList::DISPLAYLIST_NOT_DEFINED,
					 BALL_VIEW_GLDISPLAYLIST_ERROR_HANDLER
					 (GLDisplayList::ERROR__NO_DISPLAY_LIST_AVAILABLE));
			}
			else
			{
				BALL_PRECONDITION
					(0,
					 BALL_VIEW_GLDISPLAYLIST_ERROR_HANDLER
					 (GLDisplayList::ERROR__REDECLARATION_OF_DISPLAY_LIST));
			}

			if (compile_ == true)
			{
				glNewList(GL_list_, GL_COMPILE);
			}
			else
			{
				glNewList(GL_list_, GL_COMPILE_AND_EXECUTE);
			}
		}

		void GLDisplayList::endDefinition()
		{
			glEndList();
		}

		void GLDisplayList::draw()
		{
			if (GL_list_ != 0)
			{
				glCallList(GL_list_);
			}
		}

		bool GLDisplayList::isValid() const
		{
			return (bool)(GL_list_ != 0);
		}

		void GLDisplayList::dump
			(ostream& s, Size depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_DEPTH(s, depth);
			s << "display list : " << GL_list_ << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "compile mode : " 
					 << ((compile_ == true) ? "yes" : "no") << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "compile and execute mode : " 
					 << ((compile_ == false) ? "yes" : "no") << endl;

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void GLDisplayList::read(istream & /* s */)
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}
		 
		void GLDisplayList::write(ostream & /* s */) const
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/KERNEL/glDisplayList.iC>
#		endif

	} // namespace VIEW

} // namespace BALL
