#ifndef BALL_VIEW_WIDGETS_DOCKABLEPIXMAPWIDGET_H
#define BALL_VIEW_WIDGETS_DOCKABLEPIXMAPWIDGET_H

#ifndef BALL_VIEW_WIDGETS_PIXMAPCANVASVIEW_H
# include <BALL/VIEW/WIDGETS/pixmapCanvasView.h>
#endif

#ifndef BALL_VIEW_WIDGETS_DOCKWIDGET_H
# include <BALL/VIEW/WIDGETS/dockWidget.h>
#endif

namespace BALL
{
  namespace VIEW
  {      
    class DockablePixmapWidget
      : public DockWidget
    {
      Q_OBJECT                        //macro for QT messages
	public:

      //Constructor
      DockablePixmapWidget(const QPixmap& pixmap, 
			   int x=0, int y=0, QWidget *parent=0 );
      //   DockablePixmapWidget(int x=0, int y=0, QWidget *parent=0 );

      //Copyconstructor 
      //  DockablePixmapWidget(const  DockablePixmapWidget& dockpixwid);

      //Destructor 
      ~DockablePixmapWidget()
	throw();

      //methods
      // void setPixmap(const QPixmap& pixmap);
      void plot()
	throw();

    protected:
      PixmapCanvasView pixCanView_;

    };//end of class dockablePixmapWidget
    
  }
}


#endif
