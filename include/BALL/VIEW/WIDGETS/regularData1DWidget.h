// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: regularData1DWidget.h,v 1.6 2003/12/19 19:47:09 oliver Exp $
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
# include <BALL/DATATYPE/regularData1D.h>
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
		//  class regularData1DWidget used to plot one-dimensional data
		class MainControl;

		/**  New RegularData1D Message Classic.
				\ingroup ViewWidgets
		 */
		class UpdateRegularData1DMessage 
			: public Message
		{
		 public:
			 
			/**  @name Constructors and Destructors
			 */
			//@{
			
			///
			UpdateRegularData1DMessage();

			///
			UpdateRegularData1DMessage(const UpdateRegularData1DMessage& message);

			///
			virtual ~UpdateRegularData1DMessage()
				throw();

			///
			void setData(RegularData1D* data)
				throw() { data_ = data;}

			///
			RegularData1D* getData() 
				throw() { return data_;}

		 protected:

			RegularData1D* data_;
			
			//@}
		};


		/** Widget used to display one-dimensional data.
		 \ingroup ViewWidgets
		 */
		class RegularData1DWidget
			:public CanvasWidget 
		{
			Q_OBJECT  //macro for QT messages
				
			public:
		
			//			BALL_EMBEDDABLE(RegularData1DWidget) //macro for BALL messages
			
			/** Detailed constructor
			 */
			RegularData1DWidget(const RegularData1D& data, QWidget *parent = 0)
				throw();

			/** Copy constructor
			 */
			/*	RegularData1DWidget(RegularData1DWidget* widget)
					throw() ; */

			/** Destructor
			 */
			~RegularData1DWidget()
				throw();

			public slots: //catches signals from messages

			/** Creator of a plot
			 */
			void createPolygon()
				throw();
			
		protected:
			RegularData1D data_;
			//QPointArray point_data_; //to create the polygon
			//			QCanvasPolygon *polygon_;
			//QCanvasSpline *polygon_;
			QColor diagram_color_;
			QColor background_color_;
			Size height_; // is it neccessary? 
			bool line_style_;
			

		}; //end of class RegularData1DWidget

	} //end of namespace VIEW

}//end of namespace BALL
