// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: glDisplayList.C,v 1.6 2004/07/14 12:51:06 amoll Exp $
//

#include <BALL/VIEW/RENDERING/glDisplayList.h>
#include <BALL/COMMON/exception.h>
#include <BALL/DATATYPE/string.h>
#include <BALL/COMMON/rtti.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

		const GLDisplayList::GLList 
		GLDisplayList::DISPLAYLIST_NOT_DEFINED = (GLDisplayList::GLList)0;


		GLDisplayList::NestedDisplayList::NestedDisplayList(const char* file, int line)
			throw()
			:	Exception::GeneralException(file, line, String("NestedDisplayList"), 
																		String("display list definition inside another is not allowed."))
		{
		}

		GLDisplayList::NoDisplayListAvailable::NoDisplayListAvailable(const char* file, int line)
			throw()
			:	Exception::GeneralException(file, line, String("NoDisplayListAvailable"), 
																		String("memory allocation for display list failed."))
		{
		}

		GLDisplayList::DisplayListRedeclaration::DisplayListRedeclaration(const char* file, int line)
			throw()
			:	Exception::GeneralException(file, line, String("DisplayListRedeclaration"), 
																		String("display list already defined."))
		{
		}

		GLDisplayList::GLDisplayList()
			throw()
			:	compile_(true),
				GL_list_(0)
		{
		}

		GLDisplayList::~GLDisplayList()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "Destructing object " << (void *)this 
										<< " of class " << RTTI::getName<GLDisplayList>() << endl;
			#endif 

			clear();
		}

		void GLDisplayList::clear()
			throw()
		{
			if (GL_list_ != 0)
			{
				glDeleteLists(GL_list_, 1);

				GL_list_ = 0;
			}
		}

		void GLDisplayList::startDefinition()
			throw(GLDisplayList::NestedDisplayList, GLDisplayList::NoDisplayListAvailable, 
						GLDisplayList::DisplayListRedeclaration)
		{
			if (GL_list_ != 0)
			{
				throw DisplayListRedeclaration(__FILE__, __LINE__);
			}

			GLint current_index = 99;

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


			if (compile_)
			{
				glNewList(GL_list_, GL_COMPILE);
			}
			else
			{
				glNewList(GL_list_, GL_COMPILE_AND_EXECUTE);
			}
		}

		void GLDisplayList::dump(ostream& s, Size depth) const
			throw()
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_DEPTH(s, depth);
			s << "display list : " << GL_list_ << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "compile mode : " 
					 << ((compile_) ? "yes" : "no") << endl;

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		#	ifdef BALL_NO_INLINE_FUNCTIONS
		#		include <BALL/VIEW/RENDERING/glDisplayList.iC>
		#	endif

	} // namespace VIEW
} // namespace BALL
