// $Id: glSurfaceModel.C,v 1.3.4.1 2002/08/16 13:31:21 oliver Exp $

#include <BALL/MOLVIEW/GUI/FUNCTOR/glSurfaceModel.h>

namespace BALL
{

	namespace MOLVIEW
	{

		using VIEW::GLMesh;
			
		AddGLSurfaceModel::AddGLSurfaceModel()
			throw()
			:	AddSurfaceModel()
		{
		}

		AddGLSurfaceModel::AddGLSurfaceModel
			(const AddGLSurfaceModel &add_gl_surface_model,
			 bool deep)
			throw()
			:	AddSurfaceModel(add_gl_surface_model, deep)
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

		Mesh* AddGLSurfaceModel::createMesh_()
		{
			return (Mesh *)(new GLMesh());
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/GUI/FUNCTOR/glSurfaceModel.iC>
#		endif

	} // namespace MOLVIEW

} // namespace BALL
