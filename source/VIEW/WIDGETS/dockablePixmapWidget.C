#include <BALL/VIEW/WIDGETS/dockablePixmapWidget.h>


namespace BALL
{
  namespace VIEW
  {
    //---------------------Class dockablePixmapWidget

    //Constructor
    DockablePixmapWidget::DockablePixmapWidget(const QPixmap& pixmap, 
			    int x, int y, QWidget *parent)
      : DockWidget(parent,"Dockable Pixmap Widget"),
        pixCanView_(pixmap, x, y, this)
    {
       setGuest(pixCanView_);
    }

    /*  DockablePixmapWidget::DockablePixmapWidget(int x, int y, QWidget *parent)
      : DockWidget(parent)
    {
    }
    */

   //Destructor
    DockablePixmapWidget::~DockablePixmapWidget()
      throw()
    {
    }
   //methods:

    void DockablePixmapWidget::plot()
      throw()
    {
      //    setGuest(pixCanView_);
      pixCanView_.plot();
    }

  }// End of namespace VIEW
}//End of namespae BALL

