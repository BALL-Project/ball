// $Id: glBallAndStickModel.C,v 1.2 2000/12/12 16:19:34 oliver Exp $

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
      throw()
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
