// $Id: glsimpleBox.h,v 1.2 2000/12/12 16:15:42 oliver Exp $

#ifndef BALL_VIEW_GUI_PRIMITIV_GLSIMPLEBOX_H
#define BALL_VIEW_GUI_PRIMITIV_GLSIMPLEBOX_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_GUI_KERNEL_GLOBJECT_H
#	include <BALL/VIEW/GUI/KERNEL/glObject.h>
#endif

#ifndef BALL_VIEW_PRIMITIV_SIMPLEBOX_H
#	include <BALL/VIEW/PRIMITIV/simpleBox.h>
#endif


namespace BALL
{
	
	namespace VIEW
	{

		/**
		*/
		class GLSimpleBox
			: public SimpleBox,
				public GLObject
		{
			public:

			/**	@name	Constructors and Destructors
			*/	
			//@{

			GLSimpleBox();

			GLSimpleBox(const GLSimpleBox& SimpleBox, bool deep = true);

			GLSimpleBox(const GeometricObject& geometric_object);

			virtual ~GLSimpleBox()
				throw();

			virtual void clear()
				throw();

			virtual void destroy()
				throw();
			//@}


			protected:

			virtual bool draw(bool with_names = false);

			virtual bool drawUserDefined();

			virtual bool extract();
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/PRIMITIV/glsimpleBox.iC>
#		endif
		
	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_GUI_PRIMITIV_GLSIMPLEBOX_H
