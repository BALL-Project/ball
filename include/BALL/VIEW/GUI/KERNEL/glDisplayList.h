// $Id: glDisplayList.h,v 1.3 2000/09/23 14:15:03 hekl Exp $

#ifndef BALL_VIEW_GUI_KERNEL_GLDISPLAYLIST_H
#define BALL_VIEW_GUI_KERNEL_GLDISPLAYLIST_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#include <GL/gl.h>

namespace BALL
{

	namespace VIEW
	{

		/**
		*/
		class GLDisplayList
		{
			public:


			/**	@name	Type Definitions
			*/
			//@{

			/**
			*/
			typedef unsigned long GLList;
			//@}

			static const GLList DISPLAYLIST_NOT_DEFINED;

			/**	@name	Constructors and Destructors
			*/
			//@{

			GLDisplayList();

			GLDisplayList(const GLDisplayList& GL_display_list, bool deep = true);

			virtual ~GLDisplayList();

			virtual void clear();

			virtual void destroy();
			//@}

			/**	@name	 Assignment
			*/
			//@{

			void set
				(const GLDisplayList& GL_display_list, bool deep = true);

			GLDisplayList& operator = (const GLDisplayList& GL_display_list);

			void get
				(GLDisplayList& GL_display_list, bool deep = true) const;

			void swap(GLDisplayList& GL_display_list);
			//@}

			/**	@name	Accessors
			*/
			//@{

			void startDefinition();

			void endDefinition();

			void draw();

			void useCompileMode();

			void useCompileAndExecuteMode();
			//@}

			/**	@name	Predicates
			*/
			//@{

			bool isCompileMode() const;

			bool isCompileAndExecuteMode() const;
			//@}

			/**	@name	 Debugging and Diagnostics
			*/
			//@{

			virtual bool isValid() const;

			virtual void dump
				(std::ostream& s = std::cout, Size depth = 0) const;
			//@}

			/**	@name	Storers
			*/
			//@{

			virtual void read(std::istream& s);

			virtual void write(std::ostream& s) const;
			//@}

			
			private:

			/* compile switch */
			bool compile_;

			/* display list */
			GLDisplayList::GLList GL_list_;
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/KERNEL/glDisplayList.iC>
#		endif

	} // namespace VIEW

} // namespace BALL
		

#endif // BALL_VIEW_GUI_KERNEL_GLDISPLAYLIST_H
