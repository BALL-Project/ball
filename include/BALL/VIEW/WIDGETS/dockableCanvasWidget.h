#ifndef BALL_VIEW_WIDGETS_DOCKABLECANVASWIDGET_H
#define BALL_VIEW_WIDGETS_DOCKABLECANVASWIDGET_H

#ifndef BALL_VIEW_WIDGETS_CANVASWIDGET_H
# include <BALL/VIEW/WIDGETS/canvasWidget.h>
#endif

#ifndef BALL_VIEW_WIDGETS_DOCKWIDGET_H
# include <BALL/VIEW/WIDGETS/dockWidget.h>
#endif

namespace BALL
{
  namespace VIEW
  {      
    class DockableCanvasWidget
      : public DockWidget
    {
      Q_OBJECT                        //macro for QT messages
    public:

      //Constructor
      DockableCanvasWidget(QWidget *parent=0 );

      //   DockableCanvasWidget(QWidget *parent=0 );

      //Copyconstructor 
      //  DockableCanvasWidget(const  DockableCanvasWidget& dockcanwid);

      //Destructor 
      ~DockableCanvasWidget()
	throw();

      //methods
      void plot()
	throw();
  
    protected:
      CanvasWidget canWidget_;

    };//end of class dockableCanvasWidget
    
  }
}


#endif
