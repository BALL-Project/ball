// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: regularData1DWidget.h,v 1.9 2004/06/10 16:09:35 amoll Exp $
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

#include <qcolor.h>

namespace BALL
{
	namespace VIEW
	{
		/**  RegularData1D Message
				\ingroup ViewWidgets
		 */
		class BALL_EXPORT RegularData1DMessage 
			: public Message
		{
		 public:
			 
			/**  @name Constructors and Destructors
			 */
			//@{
			
			///
			RegularData1DMessage()
				throw()
			{}

			///
			RegularData1DMessage(const RegularData1DMessage& message)
				throw(){}

			///
			virtual ~RegularData1DMessage()
				throw() {}

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
		 		WARNING: This code is still under heavy development, no warranty given.
		 		\ingroup ViewWidgets
		 */
		class BALL_EXPORT RegularData1DWidget
			: public CanvasWidget,
				public ModularWidget
		{
			Q_OBJECT  
				
			public:
		
			BALL_EMBEDDABLE(RegularData1DWidget, ModularWidget)
			
			/** Detailed constructor
			 */
			RegularData1DWidget(const RegularData1D* data, QWidget *parent = 0)
				throw();

			/** Copy constructor
			 */
			RegularData1DWidget(RegularData1DWidget* widget)
				throw() ;

			/** Destructor
			 */
			~RegularData1DWidget()
				throw();

			/** Handles messages sent by other registered ConnectionObject instances.
					\param message the pointer to the message that should be processed
		  */
			virtual void onNotify(Message *message)
				throw();

			public slots: 

			/** Creator of a plot
			 */
			void createPolygon()
				throw();
			
		protected:
			const RegularData1D* data_;
			QColor diagram_color_;
			QColor background_color_;
			QColor axis_color_;
		}; //end of class RegularData1DWidget

	} //end of namespace VIEW
}//end of namespace BALL
