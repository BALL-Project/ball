// $Id: glBallAndStickModel.h,v 1.2 2000/12/12 16:16:57 oliver Exp $

#ifndef BALL_MOLVIEW_GUI_FUNCTOR_GLBALLANDSTICKMODEL_H
#define BALL_MOLVIEW_GUI_FUNCTOR_GLBALLANDSTICKMODEL_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_GUI_PRIMITIV_GLSPHERE_H
#	include <BALL/VIEW/GUI/PRIMITIV/glsphere.h>
#endif

#ifndef BALL_VIEW_GUI_PRIMITIV_GLTUBE_H
#	include <BALL/VIEW/GUI/PRIMITIV/gltube.h>
#endif

#ifndef BALL_MOLVIEW_FUNCTOR_BALLANDSTICKMODEL_H
#	include <BALL/MOLVIEW/FUNCTOR/ballAndStickModel.h>
#endif

#ifndef BALL_MOLVIEW_GUI_PRIMITIV_GLTWOCOLOREDTUBE_H
#	include <BALL/MOLVIEW/GUI/PRIMITIV/glTwoColoredTube.h>
#endif


namespace BALL
{

	namespace MOLVIEW
	{

		/**
		*/
		class AddGLBallAndStickModel: public AddBallAndStickModel
		{
			public:

			/**	@name	Constructors and Destructors
			*/
			//@{

			AddGLBallAndStickModel();

			AddGLBallAndStickModel
				(const AddGLBallAndStickModel& add_GL_ball_and_stick_model, bool deep = true);

			virtual ~AddGLBallAndStickModel()
				throw();
			//@}


			private:

			virtual Sphere* createSphere_();
		 
			virtual Tube* createTube_();
		 
			virtual TwoColoredTube* createTwoColoredTube_();
		};

#			ifndef BALL_NO_INLINE_FUNCTIONS
#				include <BALL/MOLVIEW/GUI/FUNCTOR/glBallAndStickModel.iC>
#			endif
		
	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_GUI_FUNCTOR_GLBALLANDSTICKMODEL_H
