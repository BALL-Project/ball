// $Id: glSurfaceModel.h,v 1.1 2000/09/23 15:38:11 hekl Exp $

#ifndef BALL_MOLVIEW_GUI_FUNCTOR_GLSURFACEMODEL_H
#define BALL_MOLVIEW_GUI_FUNCTOR_GLSURFACEMODEL_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_GUI_PRIMITIV_GLMESH_H
#	include <BALL/VIEW/GUI/PRIMITIV/glmesh.h>
#endif

#ifndef BALL_MOLVIEW_FUNCTOR_SURFACEMODEL_H
#	include <BALL/MOLVIEW/FUNCTOR/surfaceModel.h>
#endif



namespace BALL
{

	namespace MOLVIEW
	{

		/**
		*/
		class AddGLSurfaceModel: public AddSurfaceModel
		{
			public:

			/**	@name	Constructors and Destructors
			*/
			//@{

			AddGLSurfaceModel();

			AddGLSurfaceModel
				(const AddGLSurfaceModel& add_gl_surface_model, bool deep = true);

			virtual ~AddGLSurfaceModel();
			//@}


			private:

			virtual Mesh* createMesh_();
		};

#			ifndef BALL_NO_INLINE_FUNCTIONS
#				include <BALL/MOLVIEW/GUI/FUNCTOR/glSurfaceModel.iC>
#			endif
		
	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_GUI_FUNCTOR_GLSURFACEMODEL_H
