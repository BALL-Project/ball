// $Id: glVanDerWaalsModel.h,v 1.2 2000/12/12 16:16:57 oliver Exp $

#ifndef BALL_MOLVIEW_GUI_FUNCTOR_GLVANDERWAALSMODEL_H
#define BALL_MOLVIEW_GUI_FUNCTOR_GLVANDERWAALSMODEL_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_GUI_PRIMITIV_GLSPHERE_H
#	include <BALL/VIEW/GUI/PRIMITIV/glsphere.h>
#endif

#ifndef BALL_MOLVIEW_FUNCTOR_VANDERWAALSMODEL_H
#	include <BALL/MOLVIEW/FUNCTOR/vanDerWaalsModel.h>
#endif


namespace BALL
{
	
	namespace MOLVIEW
	{

		/**
		*/
		class AddGLVanDerWaalsModel
			: public AddVanDerWaalsModel
		{
			public:

			/**	@name	Constructors and Destructors
			*/
			//@{

			AddGLVanDerWaalsModel();

			AddGLVanDerWaalsModel
				(const AddGLVanDerWaalsModel& add_Gl_van_der_waals_model, bool deep = true);

			virtual ~AddGLVanDerWaalsModel()
				throw();
			//@}


			private:

			virtual Sphere* createSphere_();
		};

#			ifndef BALL_NO_INLINE_FUNCTIONS
#				include <BALL/MOLVIEW/GUI/FUNCTOR/glVanDerWaalsModel.iC>
#			endif

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_GUI_FUNCTOR_GLVANDERWAALSMODEL_H
