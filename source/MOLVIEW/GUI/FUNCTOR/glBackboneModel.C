// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: glBackboneModel.C,v 1.6 2003/06/06 10:40:58 amoll Exp $

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

		AddGLBackboneModel::AddGLBackboneModel(const AddGLBackboneModel &add_gl_backbone_model)
			throw()
			: AddBackboneModel(add_gl_backbone_model)
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
			throw()
		{
			backbone_ = new GLBackbone();
			return backbone_;
		}

	} // namespace MOLVIEW

} // namespace BALL
