// $Id: gltube.h,v 1.3 2000/09/23 14:15:06 hekl Exp $

#ifndef BALL_VIEW_GUI_PRIMITIV_GLTUBE_H
#define BALL_VIEW_GUI_PRIMITIV_GLTUBE_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_GUI_KERNEL_GLOBJECT_H
#	include <BALL/VIEW/GUI/KERNEL/glObject.h>
#endif

#ifndef BALL_VIEW_PRIMITIV_TUBE_H
#	include <BALL/VIEW/PRIMITIV/tube.h>
#endif


namespace BALL
{

	namespace VIEW
	{

		/**
		*/
		class GLTube
			: public Tube,
				public GLObject
		{
			public:

			/**	@name Constructors and Destructors
			*/	
			//@{

			GLTube();

			GLTube(const GLTube& tube, bool deep = true);

			GLTube(const GeometricObject& geometric_object);

			virtual ~GLTube();

			virtual void clear();

			virtual void destroy();
			//@}
				


			protected:

			virtual bool draw(bool with_names = false);

			virtual bool drawUserDefined();

			virtual bool extract();
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/PRIMITIV/gltube.iC>
#		endif

	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_GUI_PRIMITIV_GLTUBE_H 
