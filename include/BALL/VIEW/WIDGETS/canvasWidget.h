#ifndef BALL_VIEW_WIDGETS_CANVASWIDGET_H
#define BALL_VIEW_WIDGETS_CANVASWIDGET_H

#include <qcanvas.h>
#include <qpixmap.h>
#include <qpainter.h>
#include <vector>

namespace BALL
{
  namespace VIEW
  {      
    class CanvasWidget
      : public QCanvasView
    {
      Q_OBJECT                        //macro for QT messages
       public:
      
      class PixmapItem
	: public QCanvasRectangle
      {
      public:
	//Constructor
	PixmapItem(QCanvas* canvas, const QPixmap& pixmap);
	//Copyconstructor
	PixmapItem(const PixmapItem& pixitem);
	//Destructor
	virtual ~PixmapItem();
	
      protected:
	void drawShape(QPainter& p);
	QPixmap pixmap_;
	
      }; //end of class PixmapItem
      
      //Constructor
      CanvasWidget (QWidget *parent  = 0, 
		    int x=0, int y=0,  
		    const char* name = 0, 
		    WFlags f         = 0);
                

      //Copyconstructor doesn't work because of the QT private copy constructors

      //Destructor
      virtual ~CanvasWidget();  
      
      //methods
      void showObjects()	 
	throw();
      
      /*
      void enlarge()
        throw();
      void shrink()
        throw();
      void rotateClockwise()
        throw();
      void rotateCounterClockwise()
        throw(); 
      */

      void zoomIn()
        throw();
      void zoomOut()
	throw();
      void zoom(float xfactor,float yfactor)
	throw();
      void zoomToFit()
	throw();
      
      /*
      void mirror()
        throw();
      void moveL()
        throw();
      void moveR()
        throw();
      void moveU()
        throw();
      void moveD()
      throw();
	 
      void print()
        throw();
	 
      void toggleDoubleBuffer()
        throw();
      */


    protected:  
      QCanvas canvas_;
      std::vector<QCanvasItem*> objects_; //z.B PixItem, Polygonzuege
      int x_; //width
      int y_; //heights
      
    };//end of class CanvasWidget
    
  }
}


#endif
