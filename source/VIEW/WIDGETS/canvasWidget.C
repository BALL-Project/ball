#include <BALL/VIEW/WIDGETS/canvasWidget.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <qlabel.h>

namespace BALL
{
  namespace VIEW
  {
    
    //---------------------Class PixmapItem
 
    CanvasWidget::PixmapItem::PixmapItem(QCanvas* canvas, const QPixmap& pixmap)
      : QCanvasRectangle(canvas),
	pixmap_(pixmap)
    {
    }

    CanvasWidget::PixmapItem::PixmapItem(const CanvasWidget::PixmapItem& pixitem)
      : QCanvasRectangle((QCanvasRectangle)pixitem),
	pixmap_(pixitem.pixmap_)
    {
    }

    CanvasWidget::PixmapItem::~PixmapItem()
    {
    }

    void CanvasWidget::PixmapItem::drawShape(QPainter& p)
    {
      p.drawPixmap(int (x()),int (y()), pixmap_);
    }

    QPixmap& CanvasWidget::PixmapItem::getPixmap()
    {
      return pixmap_;
    }

    //-----------------------class CanvasWidget
    
    //Constructor
    CanvasWidget::CanvasWidget(QWidget *parent, int x, int y, 
			       const char* name, WFlags f)
      : QCanvasView(parent, name, f),
				canvas_(0,0),//800, 600),
				x_(x),
				y_(y)
    {
      setCanvas(&canvas_);
    }

    //Destructor
    CanvasWidget::~CanvasWidget()
    {
	// we are responsible for destrucing the items
      for (int i=0; i<(int)objects_.size(); i++)
	{
		if (objects_[i] != 0)
			delete (objects_[i]);
	}
     
    }
   
    
    //methods:
    void CanvasWidget::showObjects()
      throw() 
    {
      for(int i=0; i<(int)objects_.size(); i++)
      {
//	objects_[i].move(0,0);
	objects_[i]->show();
      }
    }
    

    void CanvasWidget::zoomIn()
      throw()
    {
Log.info() << "ZoomIn" << std::endl;
      QWMatrix m = this->worldMatrix();
      m.scale(2.0,2.0);
      this->setWorldMatrix(m);
    }
  
    void CanvasWidget::zoomOut()
      throw()
    {
      QWMatrix m = this->worldMatrix();
      m.scale(0.5, 0.5);
      this->setWorldMatrix(m);
    }


    void CanvasWidget::zoom(float xfactor, float yfactor)
      throw()
    {
      QWMatrix m = this->worldMatrix();
      m.scale(xfactor, yfactor);
      this->setWorldMatrix(m);
    }

    void CanvasWidget::zoomToFit()
      throw()
    {      
      float xfactor = 1.;
      float yfactor = 1.;
      QWMatrix m  = worldMatrix();

      int conWidth  = viewport()->width();  //content of scrollview
      int conHeight = viewport()->height(); //content of scrollview

      int visHeight = canvas_.height();  // height of canvas
      int visWidth  = canvas_.width();   // width of canvas


      if((visWidth !=0.) && (visHeight!=0.))
      {
      	xfactor = (float)conWidth/(float)visWidth;
				yfactor = (float)conHeight/(float)visHeight;
      }
			QWMatrix m2(xfactor, m.m12(), m.m21(), yfactor, m.dx(), m.dy());
Log.info() << xfactor << " " << yfactor << std::endl;
      setWorldMatrix(m2);
    }


  }// End of namespace VIEW
}//End of namespae BALL
