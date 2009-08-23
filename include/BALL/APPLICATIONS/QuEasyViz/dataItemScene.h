/* dataItemScene.h
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

#ifndef DATAITEMSCENE_H
#define DATAITEMSCENE_H

//#include <QtGui/QGraphicsView>
#include <QtGui/QGraphicsScene>
#include <qobject.h>




namespace BALL
{
	namespace VIEW
	{
		class MainWindow;
		class DataItem;
		class Edge;
		class DataItemView;
		class ModelItem;
		class ValidationItem;
		
		/** @class DataItemScene
		* @brief class for storing all DataItems
		*
		* @todo
		*/
		class DataItemScene : public QGraphicsScene
		{
			Q_OBJECT
			
			public:
			
				/** @name Constructors and Destructors */

				/** standard constructor */
				DataItemScene();
				
				~DataItemScene();

				/** @name Accessors */
			
				/** sets the main window for this scene 
				* @param mw the new main window
				*/
				void setMainWindow(MainWindow* mw);
				
				/** @name Attributes*/

				/** pointer to the window containing ALL widgets */
				MainWindow* main_window;

				/** view connected to this scene*/
				DataItemView* view;
				
				void mousePressEvent(QGraphicsSceneMouseEvent* mouseEvent);
				
				QPointF getOffset(QPointF& origin,DataItem* item);
				
			protected:
				//void wheelEvent(QWheelEvent *event);
				//void scaleView(qreal scaleFactor);
				


				/** QT event handler for drop events on this scene
				*/			
				void dropEvent(QGraphicsSceneDragDropEvent* event);
				
				void dragMoveEvent(QGraphicsSceneDragDropEvent* event);
				
				void createExternalValPipeline(ModelItem* model_item, ValidationItem* val_item);
				//void dragLeaveEvent(QGraphicsSceneDragDropEvent *event);
				//void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
				//void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
				//void mousePressEvent(QMouseEvent *event);
				//void keyPressEvent(QKeyEvent *event);
				
				
			private:
				
				std::list<QGraphicsItemGroup*> groups_;

				
			
				
		};
	}
}

 #endif
