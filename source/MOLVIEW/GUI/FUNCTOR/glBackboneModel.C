// $Id: glBackboneModel.C,v 1.3.4.2 2002/12/08 22:35:18 amoll Exp $

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
