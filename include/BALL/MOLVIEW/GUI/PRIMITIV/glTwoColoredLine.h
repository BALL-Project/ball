// $Id: glTwoColoredLine.h,v 1.2 2000/12/12 16:16:57 oliver Exp $

#ifndef BALL_MOLVIEW_GUI_PRIMITIV_GLTWOCOLOREDLINE_H
#define BALL_MOLVIEW_GUI_PRIMITIV_GLTWOCOLOREDLINE_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_GUI_KERNEL_GLOBJECT_H
#	include <BALL/VIEW/GUI/KERNEL/glObject.h>
#endif

#ifndef BALL_MOLVIEW_PRIMITIV_TWOCOLOREDLINE_H
#	include <BALL/MOLVIEW/PRIMITIV/twoColoredLine.h>
#endif


namespace BALL
{

	namespace MOLVIEW
	{
			
		using VIEW::GLObject;
		
		/**
		*/
		class GLTwoColoredLine
			: public TwoColoredLine,
				public GLObject
		{
			public:
			/**	@name	Constructors and Destructors
			*/
			//@{

			GLTwoColoredLine();

			GLTwoColoredLine
				(const GLTwoColoredLine& GL_two_colored_line, bool deep = true);

			GLTwoColoredLine(const VIEW::GeometricObject& geometric_object);

			virtual ~GLTwoColoredLine()
				throw();

			virtual void clear()
				throw();

			virtual void destroy()
				throw();
			//@}


			protected:

			virtual bool draw(bool with_names = false);

			virtual bool extract();
		};

#			ifndef BALL_NO_INLINE_FUNCTIONS
#				include <BALL/MOLVIEW/GUI/PRIMITIV/glTwoColoredLine.iC>
#			endif

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_GUI_PRIMITIV_GLTWOCOLOREDLINE_H 
