// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: glVanDerWaalsModel.C,v 1.5 2002/02/27 12:23:47 sturm Exp $


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
