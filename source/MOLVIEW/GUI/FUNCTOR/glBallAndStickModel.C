// $Id: glBallAndStickModel.C,v 1.1 2000/09/23 15:39:13 hekl Exp $

#include <BALL/MOLVIEW/GUI/FUNCTOR/glBallAndStickModel.h>

namespace BALL
{

	namespace MOLVIEW
	{

		using VIEW::GLSphere;
		using VIEW::GLTube;

			
		AddGLBallAndStickModel::AddGLBallAndStickModel
			()
				:
				AddBallAndStickModel()
		{
		}

		AddGLBallAndStickModel::AddGLBallAndStickModel
			(const AddGLBallAndStickModel &__rAddGLBallAndStickModel,
			 bool deep)
				:
				AddBallAndStickModel(__rAddGLBallAndStickModel, deep)
		{
		}

		AddGLBallAndStickModel::~AddGLBallAndStickModel()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<AddGLBallAndStickModel>() << endl;
			#endif 

			AddBallAndStickModel::destroy();
		}

		Sphere *
		AddGLBallAndStickModel::createSphere_
			()
		{
			return (Sphere *)(new GLSphere());
		}

		Tube *
		AddGLBallAndStickModel::createTube_
			()
		{
			return (Tube *)(new GLTube());
		}

		TwoColoredTube *
		AddGLBallAndStickModel::createTwoColoredTube_
			()
		{
			return (TwoColoredTube *)(new GLTwoColoredTube());
		}


#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/GUI/FUNCTOR/glBallAndStickModel.iC>
#		endif

	} // namespace MOLVIEW

} // namespace BALL
