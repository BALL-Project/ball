// $Id: glBackbone.h,v 1.2 2001/01/08 17:29:36 anker Exp $

#ifndef BALL_MOLVIEW_GUI_PRIMITIV_GLBACKBONE_H
#define BALL_MOLVIEW_GUI_PRIMITIV_GLBACKBONE_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_GUI_KERNEL_GLOBJECT_H
#	include <BALL/VIEW/GUI/KERNEL/glObject.h>
#endif

#ifndef BALL_VIEW_GUI_PRIMITIV_GLLINE_H
#	include <BALL/VIEW/GUI/PRIMITIV/glline.h>
#endif

#ifndef BALL_VIEW_GUI_PRIMITIV_GLSPHERE_H
#	include <BALL/VIEW/GUI/PRIMITIV/glsphere.h>
#endif

#ifndef BALL_VIEW_GUI_PRIMITIV_GLTUBE_H
#	include <BALL/VIEW/GUI/PRIMITIV/gltube.h>
#endif

#ifndef BALL_MOLVIEW_PRIMITIV_BACKBONE_H
#	include <BALL/MOLVIEW/PRIMITIV/backbone.h>
#endif


namespace BALL
{
	
	namespace MOLVIEW
	{

		/**
		*/
		class GLBackbone
			: public Backbone,
				public GLObject
		{
			public:

			///	@name	Constructors and Destructors
			//@{

			///
			GLBackbone();

			///
			GLBackbone(const GLBackbone& backbone, bool deep = true);

			///
			GLBackbone(const GeometricObject& geometric_object);

			///
			virtual ~GLBackbone()
				throw();

			///
			virtual void clear()
				throw();

			///
			virtual void destroy()
				throw();

			//@}


			protected:

			// BAUSTELLE: protected method names must have a trailing _
			//_
			virtual bool draw(bool with_names = false);

			//_
			virtual bool drawUserDefined();

			//_
			virtual bool extract()
				throw();


  		private:

			//_
			virtual Sphere* createSphere_();

			//_
			virtual Tube* createTube_();		

			//_
			virtual Line* createLine_();		
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/GUI/PRIMITIV/glBackbone.iC>
#		endif
		
	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_GUI_PRIMITIV_GLBACKBONE_H
