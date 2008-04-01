#include <BALL/APPLICATIONS/QSAR_GUI/dataItemView.h>
#include <BALL/APPLICATIONS/QSAR_GUI/edge.h>
#include <BALL/APPLICATIONS/QSAR_GUI/dataItem.h>
#include <BALL/APPLICATIONS/QSAR_GUI/mainWindow.h>

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

 void DataItemView::wheelEvent(QWheelEvent *event)
 {
	if (name == "view")
	{
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
	QList<QGraphicsItem*> itemlist = scene()->items(); 
	DataItem* item = static_cast<DataItem*>(scene()->focusItem ());

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

void DataItemView::addDropSite()
{
	data_scene->addDropSite();
}


/*
void DataItemView::timerEvent(QTimerEvent *event)
{
	std::cout << "timer event" << std::endl;

	Q_UNUSED(event);

	QList<DataItem* > dataitems;
	foreach (QGraphicsItem *item, scene()->items()) 
	{
		if (DataItem* it = static_cast<DataItem *>(item))
		{
			std::cout << "timer event: item added" << std::endl;
			dataitems << it;
		}
		else
		{
			std::cout << "timer event: item not added" << std::endl;
		}
	}

	foreach (DataItem* item, dataitems)
	{
		item->calculateForces();
	}

	bool itemsMoved = false;

	foreach (DataItem* item, dataitems) 
	{
		if (item->advance())
		{
			itemsMoved = true;
		}
	}

	if (!itemsMoved) 
	{
		killTimer(timer_id_);
		timer_id_ = 0;
	}
}

void DataItemView::itemMoved()
{
	if (!timer_id_)
	{
		std::cout << "start timer" << std::endl;
		timer_id_ = startTimer(50);
	}
}
*/
