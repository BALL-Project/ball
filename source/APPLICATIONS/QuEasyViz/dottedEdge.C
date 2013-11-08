#include <dottedEdge.h>
#include <dataItem.h>

namespace BALL
{
	namespace VIEW
	{

		DottedEdge::DottedEdge(DataItem *source_node, DataItem *dest_node)
			: Edge(source_node,dest_node,1)
		 {
		 }

		DottedEdge::~DottedEdge()
		 {
		 }

		//  void DottedEdge::adjust()
		//  {
		// 	QPointF start = mapFromItem(source_, source_position_);
		// 	QPointF stop = mapFromItem(dest_, dest_position_);	
		// 	QLineF line;
		// 
		// 	line = QLineF(mapFromItem(source_, source_->width() +1., source_->height() +1.), mapFromItem(dest_, 0, 0));
		// 
		// 	//prepareGeometryChange();
		// 	source_point_ = line.p1();
		//  	dest_point_ = line.p2();
		// 	return;
		// 
		//  }

		 void DottedEdge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
		{
			adjust();
			QColor color = Qt::darkGray;
			painter->setPen(QPen(color, 1, Qt::DotLine));
			QLineF line(source_point_.x(), source_point_.y(),dest_point_.x(), dest_point_.y());
			painter->drawLine(line);
		}

		QRectF DottedEdge::boundingRect() const
		 {
				 if (!source_ || !dest_)
						 return QRectF();

				 qreal penWidth = 1;
				 qreal extra = penWidth / 2.0;

				 return QRectF(source_point_, QSizeF(dest_point_.x() - source_point_.x(),dest_point_.y() -source_point_.y())).normalized().adjusted(-extra, -extra, extra, extra);
		 }

		void DottedEdge::setSourcePosition(QPointF newSourcePosition)
		{
			source_position_ = newSourcePosition;
		}

		void DottedEdge::setDestPosition(QPointF newDestPosition)
		{
			dest_position_ = newDestPosition;
		}

		QPointF DottedEdge::getSourcePosition() const
		{
			return source_position_;
		}

		QPointF DottedEdge::getDestPosition() const
		{
			return dest_position_;
		}
	}
}
