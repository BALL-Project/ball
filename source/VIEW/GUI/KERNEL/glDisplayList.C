// $Id: glDisplayList.C,v 1.3 2001/02/11 13:06:31 hekl Exp $

#include <BALL/VIEW/GUI/KERNEL/glDisplayList.h>
#include <BALL/COMMON/exception.h>

using namespace std;

namespace BALL
{

	namespace VIEW
	{

		const GLDisplayList::GLList 
		GLDisplayList::DISPLAYLIST_NOT_DEFINED = (GLDisplayList::GLList)0;


		GLDisplayList::NestedDisplayList::NestedDisplayList(const char* file, int line)
			:	Exception::GeneralException(file, line, string("NestedDisplayList"), string("display list definition inside another is not allowed."))
		{
		}

		GLDisplayList::NoDisplayListAvailable::NoDisplayListAvailable(const char* file, int line)
			:	Exception::GeneralException(file, line, string("NoDisplayListAvailable"), string("memory allocation for display list failed."))
		{
		}

		GLDisplayList::DisplayListRedeclaration::DisplayListRedeclaration(const char* file, int line)
			:	Exception::GeneralException(file, line, string("DisplayListRedeclaration"), string("display list already defined."))
		{
		}

		GLDisplayList::GLDisplayList()
			:	compile_(true),
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
		}

		void GLDisplayList::startDefinition()
		{
			if (GL_list_ == 0)
			{
				int current_index;

				glGetIntegerv(GL_LIST_INDEX, &current_index);

				// we are already in a display list definition
				if (current_index != 0)
				{
					throw NestedDisplayList(__FILE__, __LINE__);
				}

				GL_list_ = glGenLists(1);

				if (GL_list_ == GLDisplayList::DISPLAYLIST_NOT_DEFINED)
				{
					throw NoDisplayListAvailable(__FILE__, __LINE__);
				}
			}
			else
			{
				throw DisplayListRedeclaration(__FILE__, __LINE__);
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
			return (GL_list_ != 0);
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
