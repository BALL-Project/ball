// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: regularData2DWidget.h,v 1.9 2004/06/10 17:06:01 amoll Exp $
//

#ifndef BALL_VIEW_KERNEL_MODULARWIDGET_H
# include <BALL/VIEW/KERNEL/modularWidget.h>
#endif

#ifndef BALL_VIEW_WIDGETS_CANVASWIDGET_H
# include <BALL/VIEW/WIDGETS/canvasWidget.h>
#endif

#ifndef BALL_DATATYPE_REGULARDATA2D_H
# include <BALL/DATATYPE/regularData2D.h>
#endif

#include <qcolor.h>

namespace BALL
{
	namespace VIEW
	{
		/** Widget used to display two-dimensional data.
				\ingroup ViewWidgets
		 */
		class BALL_EXPORT RegularData2DWidget
			: public CanvasWidget,
		 		public ModularWidget
		{
			Q_OBJECT  
				
			public:
		
			BALL_EMBEDDABLE(RegularData2DWidget, ModularWidget) 
			
			///
			RegularData2DWidget(const RegularData2D* data, QWidget *parent = 0)
				throw();

			///
			~RegularData2DWidget()
				throw();

			/** Handles messages sent by other registered ConnectionObject instances.
					\param message the pointer to the message that should be processed
		  */
			virtual void onNotify(Message *message)
				throw();

			public slots: 

			/// Creator of a plot
			void createPlot()
				throw();
			
		protected:

			RegularData2DWidget(RegularData2DWidget* widget)
				throw();

			const RegularData2D* data_;
			QColor diagram_color_;
			QColor background_color_;
			QColor axis_color_;
		}; 

	} //end of namespace VIEW	
}//end of namespace BALL
