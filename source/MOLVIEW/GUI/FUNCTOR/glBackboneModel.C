// $Id: glBackboneModel.C,v 1.1 2001/01/07 15:39:53 hekl Exp $

#include <BALL/MOLVIEW/GUI/FUNCTOR/glBackboneModel.h>

namespace BALL
{

	namespace MOLVIEW
	{

		AddGLBackboneModel::AddGLBackboneModel
			()
				:
				AddBackboneModel()
		{
		}

		AddGLBackboneModel::AddGLBackboneModel
			(const AddGLBackboneModel &add_gl_backbone_model,
			 bool deep)
				:
				AddBackboneModel(add_gl_backbone_model, deep)
		{
		}

		AddGLBackboneModel::~AddGLBackboneModel()
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
