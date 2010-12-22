// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/WIDGETS/regularData2DWidget.h> 
#include <BALL/VIEW/DATATYPE/colorMap.h>
#include <BALL/VIEW/KERNEL/message.h>

#include <QtGui/QPointArray>
#include <QtGui/QPainter>
#include <QtGui/QLayout>
#include <QtGui/QPopupMenu>
#include <QtGui/QApplication>
//Added by qt3to4:
#include <QContextMenuEvent>
#include <QPixmap>
#include <QResizeEvent>

namespace BALL
{
	namespace VIEW
	{

		RegularData2DWidget::RegularData2DWidget(const RegularData2D* data, QWidget *parent)
			: CanvasWidget(parent),
				ModularWidget("RegularData2DWidget"),
				data_(data),
				diagram_color_(QColor(Qt::black)),
				background_color_(QColor(Qt::white)),
				axis_color_(QColor(Qt::red))
		{
			registerWidget(this);
		}

		RegularData2DWidget::~RegularData2DWidget()
		{
		}

		// creates a polygon from a given vector RegularData1D * data
		void RegularData2DWidget::createPlot()
		{
			// no data => no polygon
			if (data_ == 0 ||
					data_->size() == 0) 
			{
				return;
			}

			// set up the ColorMap... TODO: This should be done by a dialog or something similar
			
			ColorRGBA colorList[3];
			ColorMap color_table_;  
			colorList[0] = ColorRGBA(1.,0.,0.,1.);
			colorList[1] = ColorRGBA(0.,1.,0.,1.);
			colorList[2] = ColorRGBA(0.,0.,1.,1.);

			color_table_.setBaseColors(colorList, 3);
			color_table_.setNumberOfColors(100);
			
			// determine the minimal and maximal values in data_
			float min = (*data_)[0];
			float max = (*data_)[0];

			for (Position i=1; i < (*data_).size(); i++)
			{
				if      ((*data_)[i] < min) min = (*data_)[i];
				else if ((*data_)[i] > max) max = (*data_)[i];
			}			
			color_table_.setRange(min, max);
			color_table_.createMap();
			
			//maximal number of Lines and Columns
			Size max_x = (*data_).getSize().x;
			Size max_y = (*data_).getSize().y;

			// Draw the points
			QPixmap pixmap;								
			pixmap.resize(max_x, max_y);
			pixmap.fill();           			// delete the old picture
			QPainter paint;     
			paint.begin(&pixmap);         // set the Painter 

			try
			{
				QColor pCol;        
				for (Position y=0; y< max_y; y++) 
				{
					for (Position x=0; x< max_x; x++) 
					{
						ColorRGBA mapcolor = color_table_.map(data_->getData(x + y * max_x));
						pCol = QColor(mapcolor.getRed(), mapcolor.getGreen(), mapcolor.getBlue());
						paint.setPen(pCol);
						paint.drawPoint(x, y);
					}
				}
			}
			catch(...)
			{
				setStatusbarText("Error: Point in dataset out of grid!");
				Log.error() << "Error: Point in dataset out of grid!" << std::endl;
				return;
			}

			paint.end();

			//put the pixmapItem into objects
			PixmapItem* pixItem = new PixmapItem(&canvas_, pixmap);
			pixItem->show();
			objects_.push_back(dynamic_cast<Q3CanvasItem*> (pixItem)); 

			// resize the canvas to fit the data
			canvas_.resize(max_x, max_y);
		}


		void RegularData2DWidget::onNotify(Message *message)
		{
			if (!RTTI::isKindOf<RegularData2DMessage>(*message))
			{
				return;
			}

			RegularData2DMessage& msg = *(RegularData2DMessage*) message;
			if (msg.getData() != data_) return;

			if ((RegularData2DMessage::RegularDataMessageType) msg.getType() 
						== RegularData2DMessage::UPDATE)
			{
				createPlot();
			}
			else if ((RegularData2DMessage::RegularDataMessageType) msg.getType() 
									== RegularData2DMessage::REMOVE)
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

		DockableRegularData2DWidget::DockableRegularData2DWidget(const RegularData2D* data, QWidget *parent)
			: DockWidget(parent,"Dockable RegularData2D Widget"),
				canWidget_(data, this)
		{
			QSizePolicy sizePolicy (QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);		
			setSizePolicy(sizePolicy);
			setMinimumSize(sizeHint());
			setGuest(canWidget_);  
			canWidget_.createPlot();
			canWidget_.resize(size()); 
		}


		DockableRegularData2DWidget::~DockableRegularData2DWidget()
		{
		}

		void DockableRegularData2DWidget::plot()
		{
			canWidget_.showObjects();
			show();
			canWidget_.zoomToFit();
		}

		void DockableRegularData2DWidget::contextMenuEvent(QContextMenuEvent* e)
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

		void DockableRegularData2DWidget::zoomToFit()
		{
			canWidget_.zoomToFit();
		}

		void DockableRegularData2DWidget::zoomIn()
		{
			canWidget_.zoomIn();
		}

		void DockableRegularData2DWidget::zoomOut()
		{
			canWidget_.zoomOut();
		}

		QSize DockableRegularData2DWidget::sizeHint() const
		{
			return QSize((int)(qApp->mainWidget()->width()/3.)+55, (int)(qApp->mainWidget()->height()/3.)+55);
		}

		void DockableRegularData2DWidget::resizeEvent(QResizeEvent* e)
		{
			canWidget_.canvas()->setAllChanged();
			canWidget_.canvas()->update();
			DockWidget::resizeEvent(e);
		}

	}//end of namespace VIEW
}//end of namespace BALL
