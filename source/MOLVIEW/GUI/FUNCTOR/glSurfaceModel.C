// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: glSurfaceModel.C,v 1.5 2002/12/12 10:57:46 oliver Exp $

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

		AddGLSurfaceModel::AddGLSurfaceModel(const AddGLSurfaceModel &add_gl_surface_model, bool deep)
			throw()
			:	AddSurfaceModel(add_gl_surface_model, deep)
		{
		}

		AddGLSurfaceModel::~AddGLSurfaceModel()
      throw()
 		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this << " of class " << RTTI::getName<AddGLSurfaceModel>() << endl;
			#endif 

			AddSurfaceModel::destroy();
		}

		Mesh* AddGLSurfaceModel::createMesh_()
		{
			return (new GLMesh());
		}

	} // namespace MOLVIEW
} // namespace BALL
