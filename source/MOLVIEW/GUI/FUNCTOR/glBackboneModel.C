// $Id: glBackboneModel.C,v 1.3.4.1 2002/10/18 14:48:26 amoll Exp $

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

	} // namespace MOLVIEW

} // namespace BALL
