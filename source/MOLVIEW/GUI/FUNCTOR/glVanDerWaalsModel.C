// $Id: glVanDerWaalsModel.C,v 1.2 2000/12/12 16:19:34 oliver Exp $


#include <BALL/MOLVIEW/GUI/FUNCTOR/glVanDerWaalsModel.h>

namespace BALL
{

	namespace MOLVIEW
	{
		
		using VIEW::GLSphere;

		AddGLVanDerWaalsModel::AddGLVanDerWaalsModel
			()
				:
				AddVanDerWaalsModel()
		{
		}

		AddGLVanDerWaalsModel::AddGLVanDerWaalsModel
			(const AddGLVanDerWaalsModel &__rAddGLVanDerWaalsModel,
			 bool deep)
				:
				AddVanDerWaalsModel(__rAddGLVanDerWaalsModel, deep)
		{
		}

		AddGLVanDerWaalsModel::~AddGLVanDerWaalsModel()
      throw()
 		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
			 << " of class " << getBallClass().getName() << endl;
			#endif 

			AddVanDerWaalsModel::destroy();
		}

		Sphere *
		AddGLVanDerWaalsModel::createSphere_
			()
		{
			return (Sphere *)(new GLSphere());
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/GUI/FUNCTOR/glVanDerWaalsModel.iC>
#		endif

	} // namespace MOLVIEW

} // namespace BALL
