// $Id: glsimpleBox.h,v 1.1 2000/09/23 12:50:12 hekl Exp $

#ifndef BALL_VIEW_OPENGL_PRIMITIV_GLSIMPLEBOX_H
#define BALL_VIEW_OPENGL_PRIMITIV_GLSIMPLEBOX_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_OPENGL_KERNEL_GLOBJECT_H
#	include <BALL/VIEW/OPENGL/KERNEL/glObject.h>
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

			virtual ~GLSimpleBox();

			virtual void clear();

			virtual void destroy();
			//@}


			protected:

			virtual bool draw(bool with_names = false);

			virtual bool drawUserDefined();

			virtual bool extract();
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/OPENGL/PRIMITIV/glsimpleBox.iC>
#		endif
		
	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_OPENGL_PRIMITIV_GLSIMPLEBOX_H
