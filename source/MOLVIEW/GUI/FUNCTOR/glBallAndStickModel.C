// $Id: glBallAndStickModel.C,v 1.4.4.1 2002/10/18 14:48:26 amoll Exp $

#include <BALL/MOLVIEW/GUI/FUNCTOR/glBallAndStickModel.h>

namespace BALL
{

	namespace MOLVIEW
	{

		using VIEW::GLSphere;
		using VIEW::GLTube;

			
		AddGLBallAndStickModel::AddGLBallAndStickModel()
			throw()
			: AddBallAndStickModel()
		{
		}

		AddGLBallAndStickModel::AddGLBallAndStickModel
			(const AddGLBallAndStickModel& rAddGLBallAndStickModel, bool deep)
			throw()
			: AddBallAndStickModel(rAddGLBallAndStickModel, deep)
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

		Sphere* AddGLBallAndStickModel::createSphere_()
		{
			return (Sphere *)(new GLSphere());
		}

		Tube* AddGLBallAndStickModel::createTube_()
		{
			return (Tube *)(new GLTube());
		}

		TwoColoredTube* AddGLBallAndStickModel::createTwoColoredTube_()
		{
			return (TwoColoredTube *)(new GLTwoColoredTube());
		}


	} // namespace MOLVIEW

} // namespace BALL
