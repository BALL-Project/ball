// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: glBackboneModel.C,v 1.4 2002/02/27 12:23:46 sturm Exp $

#include <BALL/MOLVIEW/GUI/FUNCTOR/glBackboneModel.h>

namespace BALL
{

	namespace MOLVIEW
	{

		AddGLBackboneModel::AddGLBackboneModel()
			throw()
				:
				AddBackboneModel()
		{
		}

		AddGLBackboneModel::AddGLBackboneModel
			(const AddGLBackboneModel &add_gl_backbone_model,
			 bool deep)
			throw()
				:
				AddBackboneModel(add_gl_backbone_model, deep)
		{
		}

		AddGLBackboneModel::~AddGLBackboneModel()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<AddGLBackboneModel>() << endl;
			#endif 

			AddBackboneModel::destroy();
		}

		Backbone *
		AddGLBackboneModel::createBackbone_
			()
		{
			return (Backbone *)(new GLBackbone());
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/GUI/FUNCTOR/glBackboneModel.iC>
#		endif

	} // namespace MOLVIEW

} // namespace BALL
