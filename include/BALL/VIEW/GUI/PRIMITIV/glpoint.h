// $Id: glpoint.h,v 1.1 2000/09/24 13:13:16 oliver Exp $

#ifndef BALL_VIEW_GUI_PRIMITIV_GLPOINT_H
#define BALL_VIEW_GUI_PRIMITIV_GLPOINT_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_GUI_KERNEL_GLOBJECT_H
#	include <BALL/VIEW/GUI/KERNEL/glObject.h>
#endif

#ifndef BALL_VIEW_PRIMITIV_POINT_H
#	include <BALL/VIEW/PRIMITIV/point.h>
#endif


namespace BALL
{

	namespace VIEW
	{

		/**
		*/
		class GLPoint
			: public Point,
				public GLObject
		{
			public:
			/**	@name	Constructors and Destructors
			*/
			//@{
			

			GLPoint();

			GLPoint(const GLPoint& point, bool deep = true);

			GLPoint(const GeometricObject& geometric_object);

			virtual ~GLPoint();

			virtual void clear();

			virtual void destroy();
			//@}

			protected:

			virtual bool draw(bool with_names = false);

			virtual bool extract();
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/PRIMITIV/glpoint.iC>
#		endif

	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_GUI_PRIMITIV_GLPOINT_H
