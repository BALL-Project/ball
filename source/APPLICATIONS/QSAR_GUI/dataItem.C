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
	done_ = 0;
	result_ = "";
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
	done_ = 0;
	result_ = "";
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
	done_ = 0;
	result_ = "";
}

DataItem::~DataItem()
{
	using namespace std;
	
	// delete incoming edges
	for(set<Edge*>::iterator it=in_edge_list_.begin(); it!=in_edge_list_.end(); it++)
	{
		(*it)->sourceNode()->out_edge_list_.erase(*it);
		view_->data_scene->removeItem(*it);
		delete *it;	
	}

	// delete outgoing edges and everthing hanging below these egdes
	for(set<Edge*>::iterator it=out_edge_list_.begin(); it!=out_edge_list_.end(); it++)
	{
		view_->data_scene->removeItem(*it);
		DataItem* child = (*it)->destNode();
		if(view_->data_scene->main_window->itemExists(child))
		{
			child->in_edge_list_.erase(*it);
			delete *it;
			delete child;
		}
		else
		{
			cout<<"child does not exist!"<<endl;
		}
	}
	if(view_->name == "view") view_->data_scene->removeItem(this);
}

void DataItem::addInEdge(Edge* edge)
{
	in_edge_list_.insert(edge);
}
			
void DataItem::addOutEdge(Edge* edge)
{
	out_edge_list_.insert(edge);
}

set<Edge*> DataItem::inEdges() const
{
	return in_edge_list_;
}

set<Edge*> DataItem::outEdges() const
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


bool DataItem::isDone()
{
	return done_;
}

void DataItem::change()
{
	done_ = 0;
	result_ = "";
	for(set<Edge*>::iterator it=out_edge_list_.begin(); it!=out_edge_list_.end();it++)
	{
		if(view_->data_scene->main_window->itemExists((*it)->destNode()))
		{
			(*it)->destNode()->change();
		}
		else
		{
			cout<<"found non-existing egde-destination!!"<<endl;
			set<Edge*>::iterator tmp = it; tmp--;
			view_->data_scene->removeItem(*it);
			delete *it;
			out_edge_list_.erase(it);
			it = tmp;
		}			
	}
}


void DataItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	QFont font;
	font.setPixelSize(10.5);
	font.setBold(true);
	painter->setFont(font);
	//painter->drawRoundRect(-1, -1, width_+1, height_+1);
	painter->drawRect(-1, -1, width_+1, height_+1);
	QGraphicsPixmapItem::paint(painter, option, widget);
	painter->drawText(0,0,width_*10, height_, Qt::AlignLeft || Qt::TextWordWrap, name_);
	
	if(result_!="")
	{
		QBrush brush(result_color_,Qt::SolidPattern);
		painter->setBrush(brush);
		painter->drawEllipse(width()-17.5,height()-10, 35,20);
		font.setBold(0);
		painter->setFont(font);
		painter->drawText(width()-17.5,height()-7.5, 35,15, Qt::AlignHCenter,result_);
	}
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
	if (in_edge_list_.find(edge)!=in_edge_list_.end())
	{
		in_edge_list_.erase(in_edge_list_.find(edge));
	}
}

void DataItem::removeOutEdge(Edge* edge)
{
	if (out_edge_list_.find(edge)!=out_edge_list_.end())
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
 
void DataItem::adjustEdges()
{
	itemChange(GraphicsItemChange(),QVariant(1));
}
	 

bool DataItem::removeDisconnectedItem()
{
	if(!view_->data_scene->main_window->disconnected_items_.contains(this))
	{
		return false;
	}
	view_->data_scene->main_window->disconnected_items_.erase(this)!=view_->data_scene->main_window->disconnected_items_.erase(this);
	return true;
}

void DataItem::setResultString(double value)
{
	int v = ceil(value*100);
	result_.setNum(v/100.);
}

void DataItem::setResultString(int value)
{
	String s=value;
	result_ = s.c_str();
}

void DataItem::changeSlot()
{
	change();
	view_->data_scene->update();
}


