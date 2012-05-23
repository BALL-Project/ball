#ifndef DATAITEMSCENE_H
#define DATAITEMSCENE_H

//#include <QtGui/QGraphicsView>
#include <QtWidgets/QGraphicsScene>
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
