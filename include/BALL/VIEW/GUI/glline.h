// $Id: glline.h,v 1.3 2000/09/23 14:15:05 hekl Exp $

#ifndef BALL_VIEW_GUI_PRIMITIV_GLLINE_H
#define BALL_VIEW_GUI_PRIMITIV_GLLINE_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_GUI_KERNEL_GLOBJECT_H
#	include <BALL/VIEW/GUI/KERNEL/glObject.h>
#endif

#ifndef BALL_VIEW_PRIMITIV_LINE_H
#	include <BALL/VIEW/PRIMITIV/line.h>
#endif


namespace BALL
{

	namespace VIEW
	{

		/**	
		*/
		class GLLine
			: public Line,
				public GLObject
		{
			public:

			/**	@name	 Constructors and Destructors
			*/
			//@{

			GLLine();

			GLLine(const GLLine& line, bool deep = true);

			GLLine(const GeometricObject& geometric_object);

			virtual ~GLLine();

			virtual void clear();

			virtual void destroy();
			//@}


			protected:

			virtual bool draw(bool with_names = false);

			virtual bool extract();
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/PRIMITIV/glline.iC>
#		endif
		
	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_GUI_PRIMITIV_GLLINE_H 
