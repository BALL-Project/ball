/* dataItemView.C
 * 
 * Copyright (C) 2009 Marcel Schumann
 * 
 * This file is part of QuEasy -- A Toolbox for Automated QSAR Model
 * Construction and Validation.
 * QuEasy is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 * 
 * QuEasy is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#include <dataItemView.h>
#include <edge.h>
#include <dataItem.h>
#include <mainWindow.h>

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

