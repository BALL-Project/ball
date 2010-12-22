// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_WIDGETS_CANVASWIDGET_H
#define BALL_VIEW_WIDGETS_CANVASWIDGET_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#include <QCanvasWidget>
#include <QtGui/QPixmap>
#include <QtGui/QPainter>
#include <vector>

namespace BALL
{
  namespace VIEW
  {      
		///
    class BALL_VIEW_EXPORT CanvasWidget
      : public QCanvasView
    {
      Q_OBJECT    

      public:
      
			///
      class BALL_VIEW_EXPORT PixmapItem
				: public QCanvasRectangle
      {
				public:
					
				PixmapItem(QCanvas* canvas, const QPixmap& pixmap);
				
				PixmapItem(const PixmapItem& pixitem);

				virtual ~PixmapItem();
	
				QPixmap& getPixmap();

				protected:

				void drawShape(QPainter& p);

				QPixmap pixmap_;
	
      }; //end of class PixmapItem

			///
      CanvasWidget (QWidget *parent  = 0, 
										const char* name = 0, 
										Qt::WFlags f         = 0);
                

      //Destructor
      virtual ~CanvasWidget();  
      
      ///
      void showObjects()	 
				throw();
      
    public slots:

      virtual void zoomIn()
        throw();

      virtual void zoomOut()
				throw();

      virtual void zoom(float xfactor,float yfactor)
				throw();

      virtual void zoomToFit()
				throw();

    protected:  

      //Copyconstructor doesn't work because of the QT private copy constructors
			CanvasWidget(const CanvasWidget& /*cw*/)
				throw()
				: QCanvasView() {};

      QCanvas canvas_;
      std::vector<QCanvasItem*> objects_; //e.g PixItem, Polygonzuege
      
    };//end of class CanvasWidget

  }
}

#endif
