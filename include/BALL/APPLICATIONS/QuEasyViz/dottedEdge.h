#ifndef DEDGE_H
#define DEDGE_H

#include <QtGui/QPainter>

#include <BALL/APPLICATIONS/QuEasyViz/dataItem.h>
#include <BALL/APPLICATIONS/QuEasyViz/edge.h>

namespace BALL
{
	namespace VIEW
	{
		class DataItem;

		/** @class DottedEdge
		* @brief class for representing directed edges
		*
		* @todo
		*/
		class DottedEdge : public Edge
		{
		public:
			/** @name Constructors and Destructors*/
		
			/** constructor
			@param destNode*/
			//DottedEdge(DataItem* destNode);
		
			/** constructor
			@param sourceNode 
			@param destNode*/
			DottedEdge(DataItem* sourceNode, DataItem* destNode);
		
			/** destructor
			*/
			~DottedEdge();
		
			/** @name Accessors*/
		
			/** sets the position of the source
			@param newSourcePosition */
			void setSourcePosition(QPointF newSourcePosition);
		
			/** sets the position of the destination
			@param newDestPosition */
			void setDestPosition(QPointF newDestPosition);
		
			/** returns the source position*/
			QPointF getSourcePosition() const;
		
			/** returns the destination position*/
			QPointF getDestPosition() const;
		
			//void adjust();
		
		
		protected:
			void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
			QRectF boundingRect() const;
		
// 		private:
// 		
// 			/** @name Attributes*/
// 		
// 			/** pointer to the source node of the edge */
// 			DataItem* source_;
// 		
// 			/** pointer to the destinaton node of the edge */
// 			DataItem* dest_;
// 		
// 			QPointF source_point_;
// 			QPointF dest_point_;
// 		
// 			QPointF source_position_;
// 			QPointF dest_position_;
		};
	}
}

 #endif

