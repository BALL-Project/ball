// $Id: glSurfaceModel.C,v 1.2 2000/12/12 16:19:34 oliver Exp $

#include <BALL/MOLVIEW/GUI/FUNCTOR/glSurfaceModel.h>

namespace BALL
{

	namespace MOLVIEW
	{

		using VIEW::GLMesh;

			
		AddGLSurfaceModel::AddGLSurfaceModel
			()
				:
				AddSurfaceModel()
		{
		}

		AddGLSurfaceModel::AddGLSurfaceModel
			(const AddGLSurfaceModel &add_gl_surface_model,
			 bool deep)
				:
				AddSurfaceModel(add_gl_surface_model, deep)
		{
		}

		AddGLSurfaceModel::~AddGLSurfaceModel()
      throw()
 		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<AddGLSurfaceModel>() << endl;
			#endif 

			AddSurfaceModel::destroy();
		}

		Mesh *
		AddGLSurfaceModel::createMesh_
			()
		{
			return (Mesh *)(new GLMesh());
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/GUI/FUNCTOR/glSurfaceModel.iC>
#		endif

	} // namespace MOLVIEW

} // namespace BALL
