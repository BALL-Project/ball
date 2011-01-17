// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/WIDGETS/canvasWidget.h>
#include <QPixmap>

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

    // ======================= class CanvasWidget =======================
    
    CanvasWidget::CanvasWidget(QWidget *parent, const char* name, Qt::WFlags f)
      : QCanvasView(parent, name, f),
				canvas_(0,0)
    {
			//Sets the QCanvas upon which the canvas item is to be drawn to c. 
      setCanvas(&canvas_);  
    }


    CanvasWidget::~CanvasWidget()
    {
			// we are responsible for destrucing the items
      for (int i=0; i<(int)objects_.size(); i++)
			{
				if (objects_[i] != 0) delete (objects_[i]);
			}
    }
		
    void CanvasWidget::showObjects()
    {
      for(int i=0; i<(int)objects_.size(); i++)
      {
				objects_[i]->show();
      }
    }
    
    void CanvasWidget::zoomIn()
    {
      QMatrix m = this->worldMatrix();
      m.scale(2.0,2.0);
      this->setWorldMatrix(m);
    }
  
    void CanvasWidget::zoomOut()
    {
      QMatrix m = this->worldMatrix();
      m.scale(0.5, 0.5);
      this->setWorldMatrix(m);
    }


    void CanvasWidget::zoom(float xfactor, float yfactor)
    {
      QMatrix m = this->worldMatrix();
      m.scale(xfactor, yfactor);
      this->setWorldMatrix(m);
    }

    void CanvasWidget::zoomToFit()
    {      
      float xfactor = 1.;
      float yfactor = 1.;
      QMatrix m  = worldMatrix();

			// in order to prevend rounding errors we minimize the dividend
			// -> subtract 15	
      int conWidth  = viewport()->width() - 15;  //content of scrollview
      int conHeight = viewport()->height()- 15;  //content of scrollview

      int visHeight = canvas_.height();  // height of canvas
      int visWidth  = canvas_.width();   // width of canvas

      if ((visWidth != 0) && (visHeight != 0))
      {
      	xfactor = (float)conWidth/(float)visWidth;
				yfactor = (float)conHeight/(float)visHeight;
      }

		  //originally we have a 3x3 matrix
			//m.m12 and m.m21 are the old matrixvalues
			//m.dx and m.dy are the old translation values	
			QMatrix m2(xfactor, m.m12(), m.m21(), yfactor, m.dx(), m.dy());
      setWorldMatrix(m2);
    }

  }// End of namespace VIEW
}//End of namespae BALL
