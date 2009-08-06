#include <BALL/APPLICATIONS/QuEasyViz/dataItemView.h>
#include <BALL/APPLICATIONS/QuEasyViz/edge.h>
#include <BALL/APPLICATIONS/QuEasyViz/dataItem.h>
#include <BALL/APPLICATIONS/QuEasyViz/mainWindow.h>

#include <QtGui/QWheelEvent>
#include <QtGui/QMenu>

#include <math.h>

using namespace BALL::VIEW;

DataItemView::DataItemView(DataItemScene* scene, MainWindow* mw):
	timer_id_(0)
 {
	setScene(scene); 
	data_scene = scene;
	setRenderHint(QPainter::Antialiasing);
	setResizeAnchor(AnchorUnderMouse);
	setFocusPolicy(Qt::ClickFocus);
	setAlignment(Qt::AlignLeft | Qt::AlignTop);
	scene->setItemIndexMethod(QGraphicsScene::NoIndex);
	
	scene->setMainWindow(mw);
	scene->view = this;
	
	setAcceptDrops(true);
	setInteractive (true);
 }

 void DataItemView::wheelEvent(QWheelEvent* event)
 {
	if (name == "view")
	{
		centerOn(event->x(),event->y());
		scaleView(pow((double)2, -event->delta() / 240.0));
	}
 }


void DataItemView::scaleView(qreal scaleFactor)
{
	qreal factor = matrix().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
	if (factor < 0.07 || factor > 100)
	{		
		return;
	}
     scale(scaleFactor, scaleFactor);
}


void DataItemView::keyPressEvent(QKeyEvent *event)
{
	DataItem* item = static_cast<DataItem*>(scene()->focusItem());

    if (!item)
	{
		return;
	}

	switch (event->key()) 
	{
     	case Qt::Key_Up:
         	item->moveBy(0, -10);
         	break;
     	case Qt::Key_Down:
         	item->moveBy(0, 10);
        	break;
     	case Qt::Key_Left:
         	item->moveBy(-10, 0);
         	break;
     	case Qt::Key_Right:
         	item->moveBy(10, 0);
         	break;

	default: 
		QGraphicsView::keyPressEvent(event);
	}
}

