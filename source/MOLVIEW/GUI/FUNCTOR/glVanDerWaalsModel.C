// $Id: glVanDerWaalsModel.C,v 1.3 2001/01/26 00:43:53 amoll Exp $


#include <BALL/MOLVIEW/GUI/FUNCTOR/glVanDerWaalsModel.h>

namespace BALL
{
	namespace MOLVIEW
	{
		
		using VIEW::GLSphere;

		AddGLVanDerWaalsModel::AddGLVanDerWaalsModel()
			: AddVanDerWaalsModel()
		{
		}

		AddGLVanDerWaalsModel::AddGLVanDerWaalsModel
			(const AddGLVanDerWaalsModel& rAddGLVanDerWaalsModel, bool deep)
			: AddVanDerWaalsModel(rAddGLVanDerWaalsModel, deep)
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

		Sphere*	AddGLVanDerWaalsModel::createSphere_()
		{
			return (Sphere *)(new GLSphere());
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/GUI/FUNCTOR/glVanDerWaalsModel.iC>
#		endif

	} // namespace MOLVIEW

} // namespace BALL
