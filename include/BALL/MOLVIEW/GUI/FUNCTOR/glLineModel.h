// $Id: glLineModel.h,v 1.2 2000/12/12 16:16:57 oliver Exp $

#ifndef BALL_MOLVIEW_GUI_FUNCTOR_GLLINEMODEL_H
#define BALL_MOLVIEW_GUI_FUNCTOR_GLLINEMODEL_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_GUI_PRIMITIV_GLLINE_H
#	include <BALL/VIEW/GUI/PRIMITIV/glline.h>
#endif

#ifndef BALL_VIEW_GUI_PRIMITIV_GLPOINT_H
#	include <BALL/VIEW/GUI/PRIMITIV/glpoint.h>
#endif

#ifndef BALL_MOLVIEW_FUNCTOR_LINEMODEL_H
#	include <BALL/MOLVIEW/FUNCTOR/lineModel.h>
#endif

#ifndef BALL_MOLVIEW_GUI_PRIMITIV_GLTWOCOLOREDLINE_H
#	include <BALL/MOLVIEW/GUI/PRIMITIV/glTwoColoredLine.h>
#endif

namespace BALL
{

	namespace MOLVIEW
	{
		
		/**
		*/
		class AddGLLineModel
			: public AddLineModel
		{
			public:


			/**	@nameConstructors and Destructors
			*/
			//@{

			AddGLLineModel();

			AddGLLineModel
				(const AddGLLineModel& add_GL_line_model, bool deep = true);

			virtual ~AddGLLineModel()
				throw();
			//@}


			
			private:

			virtual Point* createPoint_();
		 
			virtual Line* createLine_();

			virtual TwoColoredLine* createTwoColoredLine_();
		};

#			ifndef BALL_NO_INLINE_FUNCTIONS
#				include <BALL/MOLVIEW/GUI/FUNCTOR/glLineModel.iC>
#			endif

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_GUI_FUNCTOR_GLLINEMODEL_H
