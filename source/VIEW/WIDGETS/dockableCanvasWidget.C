#include <BALL/VIEW/WIDGETS/dockableCanvasWidget.h>


namespace BALL
{
  namespace VIEW
  {
    //---------------------Class dockableCanvasWidget

    //Constructor
    DockableCanvasWidget::DockableCanvasWidget( QWidget *parent)
      : DockWidget(parent,"Dockable Canvas Widget"),
        canWidget_()
    {
       setGuest(canWidget_);
    }

    /*  DockableCanvasWidget::DockableCanvasWidget(QWidget *parent)
      : DockWidget(parent)
    {
    }
    */

   //Destructor
    DockableCanvasWidget::~DockableCanvasWidget()
      throw()
    {
    }
   //methods:

    void DockableCanvasWidget::plot()
      throw()
    {
      //    setGuest(canWidget_);
      canWidget_.showObjects();
    }

  }// End of namespace VIEW
}//End of namespae BALL

