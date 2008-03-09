#include <BALL/APPLICATIONS/QSAR_GUI/edge.h>
#include <BALL/APPLICATIONS/QSAR_GUI/dataItem.h>

using namespace BALL::VIEW;
 using namespace std;
 #include <math.h>
#include <algorithm>

static const double Pi = 3.141;
static double TwoPi = 2.0 * Pi;
static qreal arrow_size = 10.0;

 Edge::Edge(DataItem *source_node, DataItem *dest_node)
 {
	setFlag(ItemIsMovable);
	setZValue(1);

	source_ = source_node;
	dest_ = dest_node;

	source_->addOutEdge(this);
	dest_->addInEdge(this);

	source_point_ = mapFromItem(source_, 0, 0);
	dest_point_ = mapFromItem(dest_, 0, 0);	
	adjust();
 }

 Edge::~Edge()
 {
 }

DataItem *Edge::sourceNode()
 {
	return source_;
 }

 void Edge::setSourceNode(DataItem *node)
 {
	source_ = node;
	if (node != NULL)
	{
		adjust();
	}
 }

 DataItem *Edge::destNode()
 {
     return dest_;
 }

 void Edge::setDestNode(DataItem *node)
 {
	dest_ = node;
	if (node != NULL)
	{
		adjust();
	}
 }

 void Edge::adjust()
 {	
	QLineF line = QLineF(mapFromItem(source_, source_->width() +1., source_->height() +1.), mapFromItem(dest_, 0, 0));

	prepareGeometryChange();
	source_point_ = line.p1();
	dest_point_ = line.p2();

// 	prepareGeometryChange();
// 	minDistance();

	return;
 }

 void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
 {
	if (!source_ || !dest_)
	{
		return;
	}
	//adjust();
	QColor color = Qt::black;
	painter->setPen(QPen(color));
	QLineF line(source_point_.x(), source_point_.y(),dest_point_.x(), dest_point_.y());
	painter->drawLine(line);

	// Draw the arrows if there's enough room
	double angle = ::acos(line.dx() / line.length());
	if (line.dy() >= 0)
	{
        angle = TwoPi - angle;
	}

	QPointF destArrowP1 = dest_point_ + QPointF(sin(angle - Pi / 3) * arrow_size, cos(angle - Pi / 3) * arrow_size);
	QPointF destArrowP2 = dest_point_ + QPointF(sin(angle - Pi + Pi / 3) * arrow_size, cos(angle - Pi + Pi / 3) * arrow_size);
	
	painter->setBrush(Qt::black);
	painter->drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2);
 }

QRectF Edge::boundingRect() const
 {
	if (!source_ || !dest_)
	{
         return QRectF();
	}

     qreal penWidth = 1;
     qreal extra = penWidth / 2.0;

     return QRectF(source_point_, QSizeF(dest_point_.x() - source_point_.x(),dest_point_.y() -source_point_.y())).normalized().adjusted(-extra, -extra, extra, extra);
 }

void Edge::setSourcePosition(QPointF newSourcePosition)
{
	source_position_ = newSourcePosition;
}

void Edge::setDestPosition(QPointF newDestPosition)
{
	dest_position_ = newDestPosition;
}

const QPointF Edge::getSourcePosition() const
{
	return source_position_;
}

const QPointF Edge::getDestPosition() const
{
	return dest_position_;
}

// void Edge::minDistance()
// {
// 	vector<int> distances;
// 	QVector<QPointF> source;
// 	QVector<QPointF> dest;
// 
// 	QPointF p1 = mapFromItem(dest_, 0, 0);
// 	QPointF p2 = mapFromItem(dest_, dest_->width()/2, 0);
// 	QPointF p3 = mapFromItem(dest_, 0, dest_->height()/2);
// 	QPointF p4 = mapFromItem(source_, source_->width(), source_->height());
// 	QPointF p5 = mapFromItem(source_, source_->width()/2, source_->height());
// 	QPointF p6 = mapFromItem(source_, source_->width(), source_->height()/2);
// 
// 	source << p4 << p5 << p6;
// 	dest << p1 << p2 << p3;
// 
// 	foreach(QPointF source_point, source)
// 	{
// 		foreach(QPointF dest_point, dest)
// 		{
// 			QLineF line(source_point, dest_point);
// 			distances.push_back((int)(line.length()));	
// 		}
// 	} 
// 
// 	int min = distances.at(0);
// 	QPointF tmp1;
// 	QPointF tmp2;
// 
// 	for (uint i = 0; i< distances.size(); i++)
// 	{	
// 		if (distances[i] < min)
// 		{
// 			min = distances[i];
// 
// 			if (i < 3)
// 			{
// 				tmp1 = p1;
// 				tmp2 = dest.at(i); 
// 			}
// 			else if (3 <= i < 6)
// 			{
// 				tmp1 = p2;
// 				tmp2 = dest.at(i);
// 			}
// 			else
// 			{
// 				tmp1 = p3;
// 				tmp2 = dest.at(i);
// 			}
// 		}
// 	}
// 	source_point_ = tmp2;
// 	dest_point_ = tmp1;
// 
// 	//vector<int>::iterator min_it = min_element(distances.begin(), distances.end());	
// }
