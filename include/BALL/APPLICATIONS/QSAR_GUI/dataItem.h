#ifndef DATAITEM_H
#define DATAITEM_H

#include <QtGui/QGraphicsPixmapItem>
#include <QtGui/QPainter>
#include <QtGui/QStyleOptionGraphicsItem>
#include <QtGui/QGraphicsSceneMouseEvent>
#include <QtCore/QSet>

#include <BALL/APPLICATIONS/QSAR_GUI/edge.h>
#include <BALL/APPLICATIONS/QSAR_GUI/dataItemView.h>

#include <QtCore/QPointer>
#include <QtCore/QTextStream>

namespace BALL
{
	namespace VIEW
	{
		class Edge;

		/** @class DataItem
		* @brief general base class for representations of parts of a QSAR pipeline 
		*
		* @todo
		*/
		class DataItem : public QGraphicsPixmapItem
		{
		public:
		
			/** @name Constructors and Destructors*/
			/**standard constructor */
			DataItem();
		
			/** constructor
			@param view DatItemView the item is assigned to
			*/
			DataItem(DataItemView* view);
		
			/** constructor
			@param view the item's DataItemView
			@param pm the item's pixmap/icon
			*/
			DataItem(DataItemView* view, QPixmap pm);
		
			/** destructor
			*/
			virtual ~DataItem();
		
			/**
			* QT's paint method of the item which determines it's appearance
			*/
			void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
		
			/** @name Accessors */
			
			/** returns the ingoing edges of the item*/
			QSet<Edge*> inEdges() const;

			/** returns the outgoing edges of the item*/
			QSet<Edge*> outEdges() const;

			/** QT function; returns the shape of the item*/
			QPainterPath shape() const;
		
			/** returns the height of the item's pixmap*/
			int height();
		
			/** returns the width of the item's  pixmap*/
			int width();
		
			/** returns the name of the item*/
			QString name();
		
			/** add an ingoing edge to the item's edge list
			@param edge
			*/
			void addInEdge(Edge* edge);
			
			/** add an outgoing edge to the item's edge list
			@param edge
			*/
			void addOutEdge(Edge* edge);

			/** remove an ingoing edge from the item's edge list
			@param edge
			*/
			void removeInEdge(Edge* edge);

			/** remove an outgoing edge from the item's edge list
			@param edge
			*/
			void removeOutEdge(Edge* edge);
		
			/**set the item's name
			*@param name
			*/
			void setName(QString name);

			void setSavedAs(QString name);

			QString savedAs();
		
			/**set the item's size
			*@param width in pixel
			*@param height in pixel
			*/
			void setSize(int width, int height); 
			
			/**set the view of the item
			*@param v new view
			*/
			void setView(DataItemView* v);
			DataItemView* view();
			
			bool isDone();
			void setDone(bool b) {done_=b;}
			
			/** Call this function if a change of parameters and/or input has occured.\n
			done_ of this item and *all of its children* is thus set to false. */
			void change();
		
			enum { Type = UserType + 1 };
		
			/** Returns the type of the item as an int. This type information is used by qgraphicsitem_cast() to distinguish between types. */
			int type() const { return Type; }

			bool removeDisconnectedItem();
			
			/** executes the DataItem, i.e. its starts reading of input, training of the model and so on, depending on the specific type of DataItem.\n
			This function must be implemented by each final class derived from DataItem! 
			@return returns a bool that specifies whether anything had to be done.*/
			virtual bool execute() = 0;
			
			/** removes the respective DataItem from its MainWindow-Pipeline.\n
			This function  must be implemented by each final class derived from DataItem! */
			virtual void removeFromPipeline() = 0;
			
			/** adds the respective DataItem to its MainWindow-Pipeline.\n
			This function  must be implemented by each final class derived from DataItem! */
			virtual void addToPipeline() = 0;
			
			/** set result_ to a given value */
			void setResultString(double value);
			void setResultString(int value);
		
		
		protected:
			/** @name Protected Attributes */

			QVariant itemChange(GraphicsItemChange change, const QVariant &value);

			void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
			
			
			/**the item's view*/
			DataItemView* view_;
			
			/** the item's width*/
			int width_;
					
			/** the item's height*/
			int height_;

			/**the item's name*/
			QString name_;
		
			/** list of ingoing edges*/
			QSet<Edge*>in_edge_list_;
			
			/** list of outgoing edges*/
			QSet<Edge*>out_edge_list_;

			QString saved_as_;
			
			/** has the data already been processed by this DataItem? */
			bool done_;
			
			/** the result that is to be shown on the screen next to this item's pixmap */
			QString result_;
			
			/** background color for the ellipse around result-string */
			QColor result_color_;
			
			
		private:
		
			/** @name Private Attributes*/
			QPointF newPos_;
		};
	}
}

#endif
