// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/WIDGETS/regularData1DWidget.h>
#include <BALL/VIEW/KERNEL/message.h>

#include <QtGui/QPopupMenu>
#include <QtGui/QApplication>
//Added by qt3to4:
#include <QContextMenuEvent>
#include <QResizeEvent>

namespace BALL
{
	namespace VIEW
	{

		RegularData1DWidget::RegularData1DWidget(const RegularData1D* data, QWidget *parent)
			: CanvasWidget(parent),
				ModularWidget("RegularData1DWidget"),
				data_(data),
				diagram_color_(QColor(Qt::black)),
				background_color_(QColor(Qt::white)),
				axis_color_(QColor(Qt::red))
		{
			registerWidget(this);
		}

		RegularData1DWidget::~RegularData1DWidget()
		{
		}

		// creates a polygon from a given vector RegularData1D * data
		void RegularData1DWidget::createPlot()
		{
			// no data => no polygon
			if (data_ == 0 ||
					data_->size() == 0) 
			{
				return;
			}

			// find the data min and max
			float min = (*data_)[0];
			float max = (*data_)[0];
			float dif_min = min;
			float old = min; //last point

			if (dif_min == 0) dif_min = 1;

			for (int i=0; i<(int)data_->size(); i++)
			{
				if ((*data_)[i] >= max) max = (*data_)[i];
				if ((*data_)[i] <= min) min = (*data_)[i];

				float dif = (*data_)[i] - old;

				if ( (dif > 1e-3) && (dif < dif_min) ) 
				{
					dif_min = dif;
				}

				old = (*data_)[i];
			}

			int height_ = (int)ceil(((max-min) / dif_min) * 5);
		 
			if (height_ > 1000)
			{
				// the resulting picture would be much too large
				height_ = 1000;
				dif_min = (max - min)/200.;
			}	

			// resize the canvas to fit the data
			canvas_.resize(5*(data_->size()+2), height_+10);

			int x_new;
			int y_new;
			int x_old = 5;
			int y_old = height_+5 - (int)((((*data_)[0]-min)/dif_min)*5);
			Q3CanvasLine *ql;

			try
			{
				for (int i=0; i<(int)data_->size(); i++)
				{
					x_new = 5*(i+1);
					y_new = height_+5 - (int)(((data_->getData(i)-min)/dif_min)*5);
					
					ql = new Q3CanvasLine(&canvas_);
					ql->setPen(diagram_color_);
					ql->show();
					ql->setPoints(x_old, y_old, x_new, y_new);
					objects_.push_back(dynamic_cast<Q3CanvasItem*> (ql));

					x_old = x_new;
					y_old = y_new;
				}
			}
			catch(...)
			{
				setStatusbarText("Error: Point in dataset out of grid!");
				Log.error() << "Error: Point in dataset out of grid!" << std::endl;
				return;
			}

			//add the x-axis
			ql = new Q3CanvasLine(&canvas_);
			int startx = 1;
			int starty = height_+5 - (int)/*round*/(((startx-min)/dif_min)*5);
			int endx   = data_->size()*5;
			ql->setPoints(startx, starty, endx, starty);
			ql->setPen(axis_color_);
			ql->show();
			objects_.push_back(dynamic_cast<Q3CanvasItem*> (ql));
				
			//add the y-axis	
			ql = new Q3CanvasLine(&canvas_);
			startx = 4;
			starty = 0;
			int endy   = height_+5 - (int)/*round*/((((*data_)[endx]-min)/dif_min)*5);
			ql->setPoints(startx, starty, startx, endy);
			ql->setPen(QColor(Qt::red));
			ql->show();
			objects_.push_back(dynamic_cast<Q3CanvasItem*> (ql));
		}

		void RegularData1DWidget::onNotify(Message *message)
		{
			if (!RTTI::isKindOf<RegularData1DMessage>(*message))
			{
				return;
			}

			RegularData1DMessage& msg = *(RegularData1DMessage*) message;
			if (msg.getData() != data_) return;

			if ((RegularData1DMessage::RegularDataMessageType) msg.getType() 
						== RegularData1DMessage::UPDATE)
			{
				createPlot();
			}
			else if ((RegularData1DMessage::RegularDataMessageType) msg.getType() 
									== RegularData1DMessage::REMOVE)
			{
				data_ = 0;

				Q3CanvasItemList list = canvas()->allItems();
				Q3CanvasItemList::Iterator it = list.begin();
				for (; it != list.end(); ++it) 
				{
					if ( *it ) delete *it;
				}
			}
		}

		// ==========================================

		DockableRegularData1DWidget::DockableRegularData1DWidget(const RegularData1D* data, QWidget *parent)
			: DockWidget(parent,"Dockable RegularData1D Widget"),
				canWidget_(data, this)
		{
			QSizePolicy sizePolicy (QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);		
			setSizePolicy(sizePolicy);
			setMinimumSize(sizeHint());
			setGuest(canWidget_);  
			canWidget_.createPlot();
			canWidget_.resize(size()); 
		}


		DockableRegularData1DWidget::~DockableRegularData1DWidget()
		{
		}

		void DockableRegularData1DWidget::plot()
		{
			canWidget_.showObjects();
			show();
			canWidget_.zoomToFit();
		}

		void DockableRegularData1DWidget::contextMenuEvent(QContextMenuEvent* e)
		{
			Q3PopupMenu context_menu;
			context_menu.insertItem("ZoomToFit", this, SLOT(zoomToFit()));
			context_menu.insertItem("ZoomIn", this, SLOT(zoomIn()));
			context_menu.insertItem("ZoomOut", this, SLOT(zoomOut()));
			QPoint pos = e->reason() == QContextMenuEvent::Mouse ? e->globalPos() :
				mapToGlobal( QPoint(e->pos().x(), 0) ) + QPoint( width() / 2, height() / 2 );
			context_menu.exec(pos);
			e->accept();
		}

		void DockableRegularData1DWidget::zoomToFit()
		{
			canWidget_.zoomToFit();
		}

		void DockableRegularData1DWidget::zoomIn()
		{
			canWidget_.zoomIn();
		}

		void DockableRegularData1DWidget::zoomOut()
		{
			canWidget_.zoomOut();
		}

		//we need this for SizePolicy
		QSize DockableRegularData1DWidget::sizeHint() const
		{
			return QSize((int)(qApp->mainWidget()->width()/3.)+55, (int)(qApp->mainWidget()->height()/3.)+55);
		}
				
		void DockableRegularData1DWidget::resizeEvent(QResizeEvent* e)
		{
			canWidget_.canvas()->setAllChanged();
			canWidget_.canvas()->update();
			DockWidget::resizeEvent(e);
		}

	} // namespace VIEW

} // namespace BALL
