// $Id: glBackboneModel.h,v 1.2 2001/01/08 17:29:33 anker Exp $

#ifndef BALL_MOLVIEW_GUI_FUNCTOR_GLBACKBONEMODEL_H
#define BALL_MOLVIEW_GUI_FUNCTOR_GLBACKBONEMODEL_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_MOLVIEW_GUI_PRIMITIV_GLBACKBONE_H
#	include <BALL/MOLVIEW/GUI/PRIMITIV/glBackbone.h>
#endif

#ifndef BALL_MOLVIEW_FUNCTOR_BACKBONEMODEL_H
#	include <BALL/MOLVIEW/FUNCTOR/backboneModel.h>
#endif



namespace BALL
{

	namespace MOLVIEW
	{

		/**
		*/
		class AddGLBackboneModel: public AddBackboneModel
		{
			public:

			///	@name	Constructors and Destructors
			//@{

			///
			AddGLBackboneModel();

			///
			AddGLBackboneModel
				(const AddGLBackboneModel& add_gl_backbone_model, bool deep = true);

			///
			virtual ~AddGLBackboneModel()
				throw();
			//@}


			private:

			//_
			virtual Backbone* createBackbone_();
		};

#			ifndef BALL_NO_INLINE_FUNCTIONS
#				include <BALL/MOLVIEW/GUI/FUNCTOR/glBackboneModel.iC>
#			endif
		
	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_GUI_FUNCTOR_GLBACKBONEMODEL_H
