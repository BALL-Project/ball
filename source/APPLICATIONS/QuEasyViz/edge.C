/* edge.C
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

#include <BALL/APPLICATIONS/QuEasyViz/edge.h>
#include <BALL/APPLICATIONS/QuEasyViz/dataItem.h>

using namespace BALL::VIEW;
 using namespace std;
 #include <math.h>
#include <algorithm>

static const double Pi = 3.141;
static double TwoPi = 2.0 * Pi;
static qreal arrow_size = 10.0;

 
Edge::Edge(DataItem *source_node, DataItem *dest_node, bool dotted)
{
	setFlag(ItemIsMovable);
	setZValue(1);
	
	source_ = source_node;
	dest_ = dest_node;

	if(!dotted)
	{
		source_->addOutEdge(this);
		dest_->addInEdge(this);
	}
	
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
	double s_w = source_->width()/2;
	double s_h = source_->height()/2;
	double d_w = dest_->width()/2;
	double d_h = dest_->height()/2;
	QPointF s1 = mapFromItem(source_, 0, 0);
	QPointF s2 = mapFromItem(source_,0,source_->height());
	QPointF d1 = mapFromItem(dest_, 0, 0);
	QPointF d2 = mapFromItem(dest_, 0,dest_->height());
	
	source_point_ = s1;
	dest_point_ = d1;
	
	if(d1.y()>s2.y())   // dest below source
	{
		QPointF s21 = mapFromItem(source_,s_w,source_->height());
		source_point_ = s21;
		
		if(s2.y()+source_->height()>d1.y()) 
		{
			if(d1.x()>s2.x()+source_->width()) // dest to the immediate lower right of source
			{
				dest_point_ = mapFromItem(dest_,0,d_h);
				return;
			}
			if(d1.x()+dest_->width()<s1.x()) // dest to the immediate lower left of source
			{
				dest_point_ = mapFromItem(dest_,dest_->width(),d_h);
				return;
			}
		}
		
		dest_point_ = mapFromItem(dest_, d_w,0);
		return;
	}
	
	QPointF d3 = mapFromItem(dest_,dest_->width(),dest_->height());
	// dest above source
	if(d2.y()<s1.y() && d2.x()>s1.x()-source_->width() && d3.x()<s1.x()+2*source_->width())
	{
		source_point_ = mapFromItem(source_,s_w,0);
		dest_point_ = mapFromItem(dest_, d_w,dest_->height());
		return;
	}
	
	QPointF s3 = mapFromItem(source_,source_->width(),source_->height());
	if(d1.x()>s3.x()) // dest to the right of source
	{
		source_point_ = mapFromItem(source_,source_->width(),s_h);
		dest_point_ = mapFromItem(dest_, 0,d_h);
		return;
	}
	else // dest dest to the left of source
	{
		source_point_ = mapFromItem(source_,0,s_h);
		dest_point_ = mapFromItem(dest_, dest_->width(),d_h);
		return;
	}
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
