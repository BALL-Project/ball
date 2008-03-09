#include <BALL/APPLICATIONS/QSAR_GUI/dataItem.h>
#include <BALL/APPLICATIONS/QSAR_GUI/edge.h>
#include <BALL/APPLICATIONS/QSAR_GUI/mainWindow.h>

#include <QtGui/QDrag>
#include <QtCore/QMimeData>
#include <QtGui/QApplication>
#include <QtGui/QGraphicsScene>

using namespace BALL::VIEW;

DataItem::DataItem(): 
	view_(NULL),
	width_(50),
	height_(50),
	name_(""),
	saved_as_("")
{
	setFlag(ItemIsMovable);
	setFlag(ItemIsSelectable);
	setFlag(ItemIsFocusable);
	setZValue(1);
	QPixmap pm = QPixmap(width_,height_);
	pm.fill(Qt::red);
	setPixmap(pm);
	setToolTip(name_);
}

DataItem::DataItem(DataItemView* view):
	view_(view),
	width_(50),
	height_(50),
	name_(""),
	saved_as_("")
{
	setFlag(ItemIsMovable);
	setFlag(ItemIsSelectable);
	setFlag(ItemIsFocusable);
	setToolTip(QString(name_));
	setZValue(1);
	QPixmap pm = QPixmap(width_,height_);
	pm.fill(Qt::white);
	setPixmap(pm);
	setToolTip(name_);
}

DataItem::DataItem(DataItemView* view, QPixmap pm): 
	view_(view),
	width_(50),
	height_(50),
	name_(""),
	saved_as_("")
{
	setFlag(ItemIsMovable);
	setFlag(ItemIsSelectable);
	setFlag(ItemIsFocusable);
	setToolTip(QString(name_));
	setZValue(1);
	setPixmap(pm);
	setToolTip(name_);
}

DataItem::~DataItem()
{
	using namespace std;
	
	// delete incoming edges
	for(QSet<Edge*>::iterator it=in_edge_list_.begin(); it!=in_edge_list_.end(); it++)
	{
		delete *it;
	}

	// delete outgoing edges and everthing hanging below these egdes
	for(QSet<Edge*>::iterator it=out_edge_list_.begin(); it!=out_edge_list_.end(); it++)
	{
		view_->data_scene->removeItem(*it);

		DataItem* child = (*it)->destNode();		
		delete *it;
		delete child;
	}
	//view_->data_scene->removeItem(this);
}

void DataItem::addInEdge(Edge* edge)
{
	in_edge_list_.insert(edge);
}
			
void DataItem::addOutEdge(Edge* edge)
{
	out_edge_list_.insert(edge);
}

QSet<Edge*> DataItem::inEdges() const
{
	return in_edge_list_;
}

QSet<Edge*> DataItem::outEdges() const
{
	return out_edge_list_;
}

QPainterPath DataItem::shape() const
{
	QPainterPath path;
	path.addRect(-2,-2, height_+2, width_+2);
	//path.addText (0.0, 0.0,QFont(), name_);
	return path;
}

void DataItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	QFont font;
	font.setBold(true);
	painter->setFont(font);
	//painter->drawRoundRect(-1, -1, width_+1, height_+1);
	painter->drawRect(-1, -1, width_+1, height_+1);
	QGraphicsPixmapItem::paint(painter, option, widget);
	painter->drawText(0,0,width_*10, height_, Qt::AlignLeft || Qt::TextWordWrap, name_);
}

void DataItem::setName(QString name)
{
	this->name_ = name;
}

void DataItem::setSize(int width, int height)
{
	this->width_ = width;
	this->height_ = height;
}

void DataItem::setView(DataItemView* v)
{
	view_ = v;
}


int DataItem::width()
{
	return this->width_;
}

int DataItem::height()
{
	return this->height_;
}

QString DataItem::name()
{
	return this->name_;
}


void DataItem::removeInEdge(Edge* edge)
{
	if (in_edge_list_.contains(edge))
	{
		in_edge_list_.erase(in_edge_list_.find(edge));
	}
}

void DataItem::removeOutEdge(Edge* edge)
{
	if (out_edge_list_.contains(edge))
	{
		out_edge_list_.erase(out_edge_list_.find(edge));
	}
}

void DataItem::setSavedAs(QString name)
{
	saved_as_ = name;
}

QString DataItem::savedAs()
{
	return saved_as_;
}

QString DataItem::writeConfiguration()
{
	QString a;
	QString b;
	qreal xc = x();
	qreal yc = y();
	QString config = "x_coordinate = " + a.setNum(xc) + QString("\n") + "y_coordinate = " + b.setNum(yc) + QString("\n");
	return config;
}

DataItemView* DataItem::view()
{
	return view_;
}

void DataItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	update();
	QGraphicsItem::mouseReleaseEvent(event);
}


QVariant DataItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
	switch (change) 
	{
		case ItemPositionChange:
			foreach (Edge *edge, out_edge_list_)
			{
				edge->adjust();
			}
			foreach (Edge *edge, in_edge_list_)
			{
				edge->adjust();
			}
			break;
		default:
			break;
	}
	return QGraphicsItem::itemChange(change, value);
 }


// void DataItem::calculateForces()
// {	
// 	if (!scene() || scene()->mouseGrabberItem() == this) 
// 	{
// 		std::cout << "calculate forces 1" << std::endl;
// 		newPos_ = pos();
// 		return;
// 	}
// 
// 	// Sum up all forces pushing this item away
// 	qreal xvel = 0;
// 	qreal yvel = 0;
// 
// 	foreach (QGraphicsItem* item, scene()->items()) 
// 	{
// 		DataItem* dataitem = static_cast<DataItem *>(item);
// 		if (!dataitem)
// 		{
// 			std::cout << "calculate forces: no item" << std::endl;
// 			continue;
// 		}
// 
// 		QLineF line(mapFromItem(dataitem, 0, 0), QPointF(0, 0));
// 		qreal dx = line.dx();
// 		qreal dy = line.dy();
// 		double l = 2.0 * (dx * dx + dy * dy);
// 		if (l > 0) 
// 		{
//         	xvel += (dx * 150.0) / l;
// 			yvel += (dy * 150.0) / l;
// 		}
// 	}
// 
// 	// Now subtract all forces pulling items together
// 	
// 	double weight = 1;
// // 	if (in_edge_list_.isEmpty() && out_edge_list_.isEmpty())
// // 	{
// // 		 weight = (in_edge_list_.size() + out_edge_list_.size() + 2) * 10.0 +0.1;
// // 	}
// 	std::cout << "bla" << std::endl;
// 	if (out_edge_list_.size() > 0)
// 	{
// 		std::cout << "Blubb" << std::endl;
// 		foreach (Edge* edge, out_edge_list_) 
// 		{
// 			QPointF pos;
// 			if (edge->sourceNode() == this)
// 			{	
// 				pos = mapFromItem(edge->destNode(), 0, 0);
// 			}
// 			else
// 			{
// 				pos = mapFromItem(edge->sourceNode(), 0, 0);
// 			}
// 			xvel += pos.x() / weight;
// 			yvel += pos.y() / weight;
// 		}
// 	}
// 
// 	std::cout << "Blubb2" << std::endl;
// 
// 	if (qAbs(xvel) < 0.1 && qAbs(yvel) < 0.1)
// 	{
// 		xvel = 0;
// 		yvel = 0;
// 	}
// 
// 	QRectF sceneRect = scene()->sceneRect();
// 	newPos_ = pos() + QPointF(xvel, yvel);
// 	newPos_.setX(qMin(qMax(newPos_.x(), sceneRect.left() + 10), sceneRect.right() - 10));
// 	newPos_.setY(qMin(qMax(newPos_.y(), sceneRect.top() + 10), sceneRect.bottom() - 10));
// }
// 
// bool DataItem::advance()
// {
// 	std::cout << "advance" << std::endl;
// 	if (newPos_ == pos())
// 	{
//          return false;
// 	}
// 	setPos(newPos_);
// 	return true;
// }
// 


