#ifndef BALL_VIEW_WIDGETS_DOCKABLEREGULARDATA1DWIDGET_H
#define BALL_VIEW_WIDGETS_DOCKABLEREGULARDATA1DWIDGET_H

#ifndef BALL_VIEW_WIDGETS_REGULARDATA1DWIDGET_H
# include <BALL/VIEW/WIDGETS/regularData1DWidget.h>
#endif

#ifndef BALL_VIEW_WIDGETS_DOCKWIDGET_H
# include <BALL/VIEW/WIDGETS/dockWidget.h>
#endif

namespace BALL
{
  namespace VIEW
  {      

		///
    class BALL_EXPORT DockableRegularData1DWidget
      : public DockWidget
    {
 	    Q_OBJECT 

			public:

			///
      DockableRegularData1DWidget(const RegularData1D* data, QWidget *parent=0 );

			///
      ~DockableRegularData1DWidget()
				throw();

			///
      void plot()
				throw();
  
			///
      RegularData1DWidget& getWidget()
				throw();

		public slots:
		
			///
			virtual void zoomToFit();
		
			///
			virtual void showSize();

			///
			virtual void zoomIn();

			///
			virtual void zoomOut();

		protected slots:
			virtual void contextMenuEvent(QContextMenuEvent* e);

    protected:
      DockableRegularData1DWidget(const DockableRegularData1DWidget& dockcanwid);
			QSize sizeHint() const;

      RegularData1DWidget canWidget_;
    };
    
  }
}
#endif
