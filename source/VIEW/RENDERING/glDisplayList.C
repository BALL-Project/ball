// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: glDisplayList.C,v 1.7.16.1 2007/03/25 21:57:01 oliver Exp $
//

#include <BALL/VIEW/RENDERING/glDisplayList.h>
#include <BALL/COMMON/exception.h>
#include <BALL/DATATYPE/string.h>
#include <BALL/COMMON/rtti.h>

using namespace std;

//   #define BALL_VIEW_DEBUG

#ifdef BALL_VIEW_DEBUG
#define CHECK_GL_ERROR \
{\
	GLenum e = glGetError(); \
	if (e != GL_NO_ERROR)\
	{\
		Log.error() << "GL Error occurred in " << __FILE__ << " " << __LINE__ << std::endl;\
		switch (e)\
		{\
			case GL_INVALID_VALUE: Log.error() << " GL_INVALID_VALUE" << std::endl;break;\
 			case GL_INVALID_ENUM: Log.error() << " GL_INVALID_ENUM" << std::endl;break;\
			case GL_INVALID_OPERATION: Log.error() << " GL_INVALID_OPERATION" << std::endl;break;\
			case GL_STACK_OVERFLOW: Log.error() << " GL_STACK_OVERFLOW" << std::endl;break;\
			case GL_STACK_UNDERFLOW: Log.error() << " GL_STACK_UNDERFLOW" << std::endl;break;\
			case GL_TABLE_TOO_LARGE: Log.error() << " GL_TABLE_TOO_LARGE" << std::endl;break;\
			default: Log.error() << " UNKNOWN ERROR" << std::endl;\
		}\
	}\
}
#else 
#define CHECK_GL_ERROR
#endif


namespace BALL
{
	namespace VIEW
	{

		const GLDisplayList::GLList 
		GLDisplayList::DISPLAYLIST_NOT_DEFINED = (GLDisplayList::GLList)0;


		GLDisplayList::NestedDisplayList::NestedDisplayList(const char* file, int line)
			:	Exception::GeneralException(file, line, String("NestedDisplayList"), 
																		String("display list definition inside another is not allowed."))
		{
		}

		GLDisplayList::NoDisplayListAvailable::NoDisplayListAvailable(const char* file, int line)
			:	Exception::GeneralException(file, line, String("NoDisplayListAvailable"), 
																		String("memory allocation for display list failed."))
		{
		}

		GLDisplayList::DisplayListRedeclaration::DisplayListRedeclaration(const char* file, int line)
			:	Exception::GeneralException(file, line, String("DisplayListRedeclaration"), 
																		String("display list already defined."))
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
				Log.error() << "Destructing object " << this << " of class GLDisplayList" << endl;
			#endif 

			clear();
		}

		void GLDisplayList::clear()
		{
			if (GL_list_ != 0)
			{
				CHECK_GL_ERROR
				glDeleteLists((GLuint) GL_list_, 1);
				CHECK_GL_ERROR

				GL_list_ = 0;
			}
		}

		void GLDisplayList::startDefinition()
			throw(GLDisplayList::NestedDisplayList, GLDisplayList::NoDisplayListAvailable, 
						GLDisplayList::DisplayListRedeclaration)
		{
			CHECK_GL_ERROR
			if (GL_list_ != 0)
			{
				throw DisplayListRedeclaration(__FILE__, __LINE__);
			}

			GLint current_index = 99;

 			glGetIntegerv(GL_LIST_INDEX, &current_index);
 			CHECK_GL_ERROR

			// we are already in a display list definition
			if (current_index != 0)
			{
				throw NestedDisplayList(__FILE__, __LINE__);
			}

			GL_list_ = glGenLists(1);
			CHECK_GL_ERROR

			if (GL_list_ == GLDisplayList::DISPLAYLIST_NOT_DEFINED)
			{
				throw NoDisplayListAvailable(__FILE__, __LINE__);
			}


			if (compile_)
			{
				glNewList(GL_list_, GL_COMPILE);
				CHECK_GL_ERROR
			}
			else
			{
				glNewList(GL_list_, GL_COMPILE_AND_EXECUTE);
				CHECK_GL_ERROR
			}
		}

		void GLDisplayList::endDefinition()
		{ 
			CHECK_GL_ERROR
			glEndList(); 
		}

		void GLDisplayList::dump(ostream& s, Size depth) const
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
