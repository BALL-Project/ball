// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: glVanDerWaalsModel.C,v 1.4.2.1 2003/01/07 13:21:30 anker Exp $


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
