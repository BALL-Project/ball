// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: regularData2DWidget.h,v 1.4 2004/01/13 12:39:38 anhi Exp $
//

#ifndef BALL_VIEW_KERNEL_MESSAGE_H
# include <BALL/VIEW/KERNEL/message.h>
#endif

#ifndef BALL_VIEW_KERNEL_MODULARWIDGET_H
# include <BALL/VIEW/KERNEL/modularWidget.h>
#endif

#ifndef BALL_VIEW_WIDGETS_CANVASWIDGET_H
# include <BALL/VIEW/WIDGETS/canvasWidget.h>
#endif

#ifndef BALL_DATATYPE_REGULARDATA1D_H
# include <BALL/DATATYPE/regularData2D.h>
#endif

#include <qlabel.h>
#include <qlayout.h>
#include <qcanvas.h>
#include <qpointarray.h>
#include <qcolor.h>

namespace BALL
{
	namespace VIEW
	{
		//  class regularData2DWidget used to plot one-dimensional data
		class MainControl;

		/**  New RegularData1D Message Classic.
				\ingroup ViewWidgets
		 */
		class UpdateRegularData2DMessage 
			: public Message
		{
		 public:
			 
			/**  @name Constructors and Destructors
			 */
			//@{
			
			///
			UpdateRegularData2DMessage();

			///
			UpdateRegularData2DMessage(const UpdateRegularData2DMessage& message);

			///
			virtual ~UpdateRegularData2DMessage()
				throw();

			///
			void setData(RegularData2D* data)
				throw() { data_ = data;}

			///
			RegularData2D* getData() 
				throw() { return data_;}

		 protected:

			RegularData2D* data_;
			
			//@}
		};


		/** Widget used to display one-dimensional data.
		 \ingroup ViewWidgets
		 */
		class RegularData2DWidget
			:public CanvasWidget 
		{
			Q_OBJECT  //macro for QT messages
				
			public:
		
			//			BALL_EMBEDDABLE(RegularData1DWidget) //macro for BALL messages
			
			/** Detailed constructor
			 */
			RegularData2DWidget(const RegularData2D& data, QWidget *parent = 0)
				throw();

			/** Copy constructor
			 */
			/*	RegularData2DWidget(RegularData2DWidget* widget)
					throw() ; */

			/** Destructor
			 */
			~RegularData2DWidget()
				throw();

			public slots: //catches signals from messages

			/** Creator of a plot
			 */
			void createPixmap()
				throw();
			
		protected:
			RegularData2D data_;
			QColor diagram_color_;
			QColor background_color_;
			Size height_; // is it neccessary? 
			bool line_style_;
			

		}; //end of class RegularData1DWidget

	} //end of namespace VIEW

}//end of namespace BALL
