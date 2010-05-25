/* dataItem.C
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

#include <dataItem.h>
#include <edge.h>
#include <mainWindow.h>

#include <BALL/VIEW/KERNEL/iconLoader.h>

#include <QtGui/QDrag>
#include <QtCore/QMimeData>
#include <QtGui/QApplication>
#include <QtGui/QGraphicsScene>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

		DataItem::DataItem(): 
			view_(NULL),
			width_(50),
			height_(50),
			name_(""),
			saved_as_("")
		{
			QPixmap pm = QPixmap(width_,height_);
			pm.fill(Qt::red);
			setPixmap(pm);
			init();
		}

		DataItem::DataItem(DataItemView* view):
			view_(view),
			width_(50),
			height_(50),
			name_(""),
			saved_as_("")
		{
			QPixmap pm = QPixmap(width_,height_);
			pm.fill(Qt::white);
			setPixmap(pm);
			init();
		}

		DataItem::DataItem(DataItemView* view, QPixmap pm): 
			view_(view),
			width_(50),
			height_(50),
			name_(""),
			saved_as_("")
		{
			setPixmap(pm);
			init();
		}

		void DataItem::init()
		{
			setFlag(ItemIsMovable);
			setFlag(ItemIsSelectable);
			setFlag(ItemIsFocusable);
			setZValue(1);
			setToolTip(name_);
			done_ = 0;
			result_ = "";
			hover_rect_ = NULL;
			setAcceptsHoverEvents(1);
			setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
			//setParent(view_);
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
					if(child->in_edge_list_.size()==0) delete child;
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

		/// at the moment this works only for InputItems (since only ModelItem::input_) is being reset !!
		void DataItem::transferEdges(DataItem* other_item)
		{
			InputDataItem* is_input = dynamic_cast<InputDataItem*>(this);
		// 	ModelItem* is_model = dynamic_cast<ModelItem*>(this);
		// 	PredictionItem* is_prediction = dynamic_cast<PredictionItem*>(this);
		// 	ValidationItem* is_validation = dynamic_cast<ValidationItem*>(this);
			
			if(!is_input /*&&!is_model&&!is_prediction&&!is_prediction*/)
			{
				throw BALL::Exception::GeneralException(__FILE__,__LINE__,"transferEdges Error","transferring edges from the given type of item is not (yet) supported!");
			}
			
			// transfer incoming edges
			for(set<Edge*>::iterator it=other_item->in_edge_list_.begin(); it!=other_item->in_edge_list_.end(); it++)
			{
				(*it)->setDestNode(this);
				addInEdge(*it);
			}
			other_item->in_edge_list_.clear();
			
			// transfer outgoing edges
			for(set<Edge*>::iterator it=other_item->out_edge_list_.begin(); it!=other_item->out_edge_list_.end(); it++)
			{
				(*it)->setSourceNode(this);
				addOutEdge(*it);
				
				uint source_type=(*it)->destNode()->type();
				if(is_input && source_type==ModelItem::Type)
				{
					ModelItem* modelitem = dynamic_cast<ModelItem*>((*it)->destNode());
					modelitem->setInput(is_input);
				}
				if(is_input && source_type==PartitioningItem::Type)
				{
					PartitioningItem* paritem = dynamic_cast<PartitioningItem*>((*it)->destNode());
					paritem->setInput(is_input);
				}
			}
			other_item->out_edge_list_.clear();
		}


		QPainterPath DataItem::shape() const
		{
			QPainterPath path;
			path.addRect(-2,-2, height_+2, width_+2);
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


		void DataItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
		{
			QByteArray itemData;
			QDataStream dataStream(&itemData, QIODevice::WriteOnly);
			QDrag *drag = new QDrag(event->widget());

			// set member-pointer of main_window to the current object in order to gain access to this object from another widget 
			view_->data_scene->main_window->dragged_item = this;
			view_->data_scene->main_window->drag_source = view_->name;
			QMimeData *mimeData = new QMimeData;
			mimeData->setData("application/x-DataItem", itemData);
			drag->setMimeData(mimeData);
			drag->setPixmap(pixmap());
			drag->setHotSpot(QPoint(0, 0));
			drag->start();
			itemChange(ItemPositionChange, pos());
			QGraphicsItem::mousePressEvent(event);
		}

		BALL::String DataItem::getMouseOverText()
		{
			// as default return an empty string, so that no mouse-over help is shown.
			String s="";
			return s;
		}

		void DataItem::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
		{	
			String message=getMouseOverText();
			if(message=="") return;
			
			hover_rect_ = new QGraphicsRectItem(this);
			QGraphicsTextItem label(message.c_str());
			QFont font; font.setPixelSize(10);
			label.setFont(font);
			float width=label.boundingRect().width();
			float x=event->pos().x();
			//cout<<mapToScene(event->pos()).x()+width<<"  "<<view_->data_scene->width()<<endl;
			float f0=mapToScene(event->pos()).x()+width+20;
			if(f0>view_->data_scene->width())
			{
				x-=f0-view_->data_scene->width();
			}
			
			hover_rect_->setPos(x+10,event->pos().y()+10);
			hover_label_ = new QGraphicsTextItem(message.c_str(),hover_rect_);
			hover_label_->setFont(font);
			
			hover_rect_->setRect(0,0,width,hover_label_->boundingRect().height());
			QBrush brush; brush.setColor(QColor(230,230,22,155));
			brush.setStyle(Qt::SolidPattern);
			hover_rect_->setBrush(brush);
			setZValue(100);
		// 	hover_rect_->setZValue(100);
		// 	hover_label_->setZValue(102);
		}

		void DataItem::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
		{
			if(hover_rect_!=NULL) hover_rect_->setPos(event->pos().x()+10,event->pos().x()+10);
		}
				
		void DataItem::hoverLeaveEvent (QGraphicsSceneHoverEvent* /*event*/)
		{
			delete hover_rect_;
			hover_rect_ = NULL;
			setZValue(0);
		}

		void DataItem::setPos(QPointF point)
		{
			setPos(point.x(),point.y());
		}


		void DataItem::setPos(double x, double y)
		{
			QGraphicsPixmapItem::setPos(x,y);
			
			if(view_->name!="view") return; // do only for the pipeline-view
			
			QRectF rect = view_->data_scene->sceneRect();
			double width = rect.width();
			double height = rect.height();
			bool resize=0;
			
			if(x>width-200)
			{
				resize=1;
				width+=300;
			}
			if(y>height-200)
			{
				resize=1;
				height+=300;
			}
			if(resize) 
			{
				rect.setWidth(width);
				rect.setHeight(height);
				view_->data_scene->setSceneRect(rect);
				//view_->ensureVisible(x,y,width,height);
			}	
		}



		void DataItem::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
		{
			if (view_->name == "view")
			{
				QMenu menu(view_);
				for(list<QAction*>::iterator it=context_menu_actions_.begin(); it!=context_menu_actions_.end(); it++)
				{
					menu.addAction(*it);
				}
				menu.exec(event->screenPos());
			}
		}

		QPixmap DataItem::findPixmap(const String& name)
		{
			QIcon const& icon  = IconLoader::instance().getIcon(String("actions/")+name);
			QList<QSize> sizes = icon.availableSizes();

			QPixmap result = icon.pixmap(sizes.last()).scaled(width(), height(), Qt::KeepAspectRatio);

			return result;
		}
	}
}