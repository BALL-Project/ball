// $Id: glVanDerWaalsModel.C,v 1.4.4.2 2002/11/04 18:15:30 amoll Exp $


#include <BALL/MOLVIEW/GUI/FUNCTOR/glVanDerWaalsModel.h>

namespace BALL
{
	namespace MOLVIEW
	{
		
		using VIEW::GLSphere;

		AddGLVanDerWaalsModel::AddGLVanDerWaalsModel()
			throw()
			: AddVanDerWaalsModel()
		{
		}

		AddGLVanDerWaalsModel::AddGLVanDerWaalsModel
			(const AddGLVanDerWaalsModel& rAddGLVanDerWaalsModel, bool deep)
			throw()
			: AddVanDerWaalsModel(rAddGLVanDerWaalsModel, deep)
		{
		}

		AddGLVanDerWaalsModel::~AddGLVanDerWaalsModel()
      throw()
 		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
			 << " of class " << RTTI::getName<AddGLVanDerWaalsModel>() << endl;
			#endif 

			AddVanDerWaalsModel::destroy();
		}

		Sphere*	AddGLVanDerWaalsModel::createSphere_()
		{
			return (Sphere *)(new GLSphere());
		}

	} // namespace MOLVIEW

} // namespace BALL
