#ifndef DATAITEMSCENE_H
#define DATAITEMSCENE_H

//#include <QtGui/QGraphicsView>
#include <QtGui/QGraphicsScene>



namespace BALL
{
	namespace VIEW
	{
		class MainWindow;
		class DataItem;
		class Edge;
		class DataItemView;

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
				//~DataItemScene();

				/** @name Accessors */
			
				/** sets the main window for this scene 
				* @param mw the new main window
				*/
				void setMainWindow(MainWindow* mw);
				void addDropSite();
				
				/** @name Attributes*/

				/** pointer to the window containing ALL widgets */
				MainWindow* main_window;

				/** view connected to this scene*/
				DataItemView* view;
				
			protected:
				//void wheelEvent(QWheelEvent *event);
				//void scaleView(qreal scaleFactor);

				/** QT event handler for drop events on this scene
				*/			
				void dropEvent(QGraphicsSceneDragDropEvent* event);
				//void dragLeaveEvent(QGraphicsSceneDragDropEvent *event);
				//void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
				//void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
				//void mousePressEvent(QMouseEvent *event);
				//void keyPressEvent(QKeyEvent *event);

				
			
				
		};
	}
}

 #endif
