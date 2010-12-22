// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_KERNEL_MODULARWIDGET_H
# include <BALL/VIEW/KERNEL/modularWidget.h>
#endif

#ifndef BALL_VIEW_WIDGETS_CANVASWIDGET_H
# include <BALL/VIEW/WIDGETS/canvasWidget.h>
#endif

#ifndef BALL_DATATYPE_REGULARDATA1D_H
# include <BALL/DATATYPE/regularData1D.h>
#endif

#ifndef BALL_VIEW_WIDGETS_DOCKWIDGET_H
# include <BALL/VIEW/WIDGETS/dockWidget.h>
#endif

#include <QtGui/QColor>
//Added by qt3to4:
#include <QContextMenuEvent>
#include <QResizeEvent>

namespace BALL
{
	namespace VIEW
	{
		/** Widget used to display one-dimensional data.
		 		\ingroup ViewWidgets
		 */
		class BALL_VIEW_EXPORT RegularData1DWidget
			: public CanvasWidget,
				public ModularWidget
		{
			Q_OBJECT  
				
			public:
		
			BALL_EMBEDDABLE(RegularData1DWidget, ModularWidget)
			
			/** Detailed constructor
			 */
			RegularData1DWidget(const RegularData1D* data, QWidget *parent = 0);

			/** Destructor
			 */
			~RegularData1DWidget();

			/** Handles messages sent by other registered ConnectionObject instances.
					\param message the pointer to the message that should be processed
		  */
			virtual void onNotify(Message *message);

			public slots: 

			/** Creator of a plot
			 */
			void createPlot();
			
			protected:

			RegularData1DWidget(RegularData1DWidget* widget);

			const RegularData1D* data_;
			QColor diagram_color_;
			QColor background_color_;
			QColor axis_color_;
		}; 


		///
    class BALL_VIEW_EXPORT DockableRegularData1DWidget
      : public DockWidget
    {
 	    Q_OBJECT 

			public:

			///
      DockableRegularData1DWidget(const RegularData1D* data, QWidget *parent=0 );

			///
      ~DockableRegularData1DWidget();

			///
      void plot();
  
		public slots:
		
			///
			virtual void zoomToFit();
		
			///
			virtual void zoomIn();

			///
			virtual void zoomOut();

		protected slots:
			virtual void resizeEvent(QResizeEvent* e);
			virtual void contextMenuEvent(QContextMenuEvent* e);

    protected:
      DockableRegularData1DWidget(const DockableRegularData1DWidget& dockcanwid);
			QSize sizeHint() const;

      RegularData1DWidget canWidget_;
    };
    
	} //end of namespace VIEW
}//end of namespace BALL
