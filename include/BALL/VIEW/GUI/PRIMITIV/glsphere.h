// $Id: glsphere.h,v 1.1 2000/09/24 13:13:17 oliver Exp $

#ifndef BALL_VIEW_GUI_PRIMITIV_GLSPHERE_H
#define BALL_VIEW_GUI_PRIMITIV_GLSPHERE_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_GUI_KERNEL_GLOBJECT_H
#	include <BALL/VIEW/GUI/KERNEL/glObject.h>
#endif

#ifndef BALL_VIEW_PRIMITIV_SPHERE_H
#	include <BALL/VIEW/PRIMITIV/sphere.h>
#endif


namespace BALL
{
	
	namespace VIEW
	{

		/**
		*/
		class GLSphere
			: public Sphere,
				public GLObject
		{
			public:

			/**	@name	Constructors and Destructors
			*/	
			//@{

			GLSphere();

			GLSphere(const GLSphere& sphere, bool deep = true);

			GLSphere(const GeometricObject& geometric_object);

			virtual ~GLSphere();

			virtual void clear();

			virtual void destroy();
			//@}


			protected:

			virtual bool draw(bool with_names = false);

			virtual bool drawUserDefined();

			virtual bool extract();
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/PRIMITIV/glsphere.iC>
#		endif
		
	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_GUI_PRIMITIV_GLSPHERE_H
