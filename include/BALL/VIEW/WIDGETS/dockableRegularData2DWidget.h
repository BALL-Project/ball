#ifndef BALL_VIEW_WIDGETS_DOCKABLEREGULARDATA2DWIDGET_H
#define BALL_VIEW_WIDGETS_DOCKABLEREGULARDATA2DWIDGET_H

#ifndef BALL_VIEW_WIDGETS_REGULARDATA2DWIDGET_H
# include <BALL/VIEW/WIDGETS/regularData2DWidget.h>
#endif

#ifndef BALL_VIEW_WIDGETS_DOCKWIDGET_H
# include <BALL/VIEW/WIDGETS/dockWidget.h>
#endif

namespace BALL
{
  namespace VIEW
  {      
    class DockableRegularData2DWidget
      : public DockWidget
    {
 	     Q_OBJECT                        //macro for QT messages
			public:

      //Constructor
      DockableRegularData2DWidget(const RegularData2D& data, QWidget *parent=0 );

      //   DockableRegularData2DWidget(QWidget *parent=0 );

      //Copyconstructor 
      //  DockableRegularData2DWidget(const  DockableRegularData2DWidget& dockcanwid);

      //Destructor 
      ~DockableRegularData2DWidget()
				throw();

      //methods
      void plot()
	throw();
  
      RegularData2DWidget& getWidget()
	throw();

		public slots:
			virtual void zoomToFit();
			virtual void showSize();
			virtual void zoomIn();
			virtual void zoomOut();


    protected:
      RegularData2DWidget canWidget_;

		protected slots:
			virtual void contextMenuEvent(QContextMenuEvent* e);
			virtual void reactToMessages(int number);
    };//end of class dockableRegularData2DWidget
    
  }
}


#endif
