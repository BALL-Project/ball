// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: glBackboneModel.C,v 1.3.2.1 2003/01/07 13:21:29 anker Exp $

#include <BALL/MOLVIEW/GUI/FUNCTOR/glBackboneModel.h>

namespace BALL
{

	namespace MOLVIEW
	{

		AddGLBackboneModel::AddGLBackboneModel()
			throw()
			: AddBackboneModel()
		{
		}

		AddGLBackboneModel::AddGLBackboneModel(const AddGLBackboneModel &add_gl_backbone_model, bool deep)
			throw()
			: AddBackboneModel(add_gl_backbone_model, deep)
		{
		}

		AddGLBackboneModel::~AddGLBackboneModel()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "Destructing object " << (void *)this 
										<< " of class " << RTTI::getName<AddGLBackboneModel>() << endl;
			#endif 

			AddBackboneModel::destroy();
		}

		Backbone* AddGLBackboneModel::createBackbone_()
		{
			return (Backbone *)(new GLBackbone());
		}

	} // namespace MOLVIEW

} // namespace BALL
