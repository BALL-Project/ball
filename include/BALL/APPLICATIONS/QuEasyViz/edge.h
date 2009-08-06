#ifndef EDGE_H
#define EDGE_H


#include <QtGui/QPainter>
#include <QtGui/QStyleOptionGraphicsItem>

#include <BALL/APPLICATIONS/QuEasyViz/dataItem.h>

#include <iostream>

namespace BALL
{
	namespace VIEW
	{
		class DataItem;


		/** @class Edge
		* @brief class for representing directed edges between two DataItems
		*
		* @todo
		*/
		class Edge : public QGraphicsLineItem
		{
		public:
			/** @name Constructors and Destructors*/

			/** constructor
			@param sourceNode pointer to the source node of the edge
			@param destNode pointer to the the target node of the edge
			@param dotted if true, the new edge is not added to edge-lists of source and destination (as is neccessary for prediction-edges)
			*/
			Edge(DataItem* sourceNode, DataItem* destNode, bool dotted=0);
		
			/** destructor
			*/
			~Edge();
		
			/** @name Accessors*/
		
			/** returns the source node of the edge*/
			DataItem* sourceNode();
		
			/** sets the source node of the edge
			@param node pointer to the source node*/
			void setSourceNode(DataItem* node);
		
			/** returns the destination node of the edge*/
			DataItem* destNode();
		
			/** sets the destination node of the edge
			@param node pointer to the destination node*/
			void setDestNode(DataItem* node);
		
			/** sets the position of the source
			@param newSourcePosition QPointF with the new start position of the edge
			*/
			void setSourcePosition(QPointF newSourcePosition);
		
			/** sets the position of the destination
			@param newDestPosition */
			void setDestPosition(QPointF newDestPosition);
		
			/** returns the source position as a QPointF */
			const QPointF getSourcePosition() const;
		
			/** returns the destination position*/
			const QPointF getDestPosition() const;
		
			enum { Type = UserType + 20 };
		
			/** redraws the edge, whenever its needed (e.g. when one of the items is moved) */ 
			void adjust();
		
		
		protected:
			virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
			QRectF boundingRect() const;
		
			/** @name Attributes*/
		
			/** pointer to the source node of the edge */
			DataItem* source_;
		
			/** pointer to the destinaton node of the edge */
			DataItem* dest_;
		
			QPointF source_point_;
			QPointF dest_point_;
		
			QPointF source_position_;
			QPointF dest_position_;
		};
	}
}

 #endif

