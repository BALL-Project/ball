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
      void showObjects();
      
    public slots:

      virtual void zoomIn();

      virtual void zoomOut();

      virtual void zoom(float xfactor,float yfactor);

      virtual void zoomToFit();

    protected:  

      //Copyconstructor doesn't work because of the QT private copy constructors
			CanvasWidget(const CanvasWidget& /*cw*/)
				: QCanvasView() {};

      QCanvas canvas_;
      std::vector<QCanvasItem*> objects_; //e.g PixItem, Polygonzuege
      
    };//end of class CanvasWidget

  }
}

#endif
