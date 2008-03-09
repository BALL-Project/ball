#include <BALL/APPLICATIONS/QSAR_GUI/CSVInputDataItem.h>
#include <BALL/APPLICATIONS/QSAR_GUI/mainWindow.h>

#include <BALL/QSAR/exception.h>
#include <BALL/APPLICATIONS/QSAR_GUI/exception.h>

#include <QtGui/QDialog>
#include <QtGui/QDrag>
#include <QtCore/QMimeData>
#include <QtGui/QApplication>
#include <QtGui/QMessageBox>

using namespace BALL::QSAR;
using namespace BALL::QSAR::Exception;
using namespace BALL::VIEW;
using namespace BALL::VIEW::Exception;

CSVInputDataItem::CSVInputDataItem(QString filename, DataItemView* view):
	InputDataItem(filename, view)
{
	QPixmap pm = QPixmap("./images/csv_icon.png").scaled(QSize(width(), height()), Qt::KeepAspectRatio,Qt::FastTransformation );
	setPixmap(pm);

	QStringList list = filename_.split("/");
	setName(list[list.size()-1]);

	data_ = new QSARData();
}

CSVInputDataItem::CSVInputDataItem(QSARData* data):
	append_(true)
{
	data_ = data;
}

CSVInputDataItem::~CSVInputDataItem()
{
	if (view_->name == "view")
	{
		MainWindow* mw = view_->data_scene->main_window;
		mw->removeFromPipeline(this);
	}
	delete data_;
}

CSVInputDataItem::CSVInputDataItem(CSVInputDataItem& item):
	InputDataItem(filename_, center_data_, center_y_ ,item.view_)
{
	no_y_ = item. no_y_;
	x_labels_ = item.x_labels_;
	y_labels_ = item.y_labels_;
	append_ = item.append_;
}

void CSVInputDataItem::readData()
{
	data_ = new QSARData;
	string st = filename_.toStdString();

	try
	{
		data_->readCSVFile(st.c_str(), no_y_, x_labels_, y_labels_, ",", 0);
	}
	catch(WrongFileFormat)
	{
		throw(InvalidInputDataItem(__FILE__,__LINE__));
	}
	catch(PropertyError)
	{
		throw(InvalidInputDataItem(__FILE__,__LINE__));
	}

	if (center_data_)
	{
		data_->centerData(center_y_);
	}
}

void CSVInputDataItem::appendData()
{
	string st = filename_.toStdString();

	try
	{
		data_->readCSVFile(st.c_str(), no_y_, x_labels_, y_labels_, ",", 1);
	}
	catch(WrongFileFormat)
	{
		throw(InvalidInputDataItem(__FILE__,__LINE__));
	}
}

void CSVInputDataItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{	
	QPoint startPos = event->pos().toPoint();
	QDrag *drag = new QDrag(event->widget());
	QByteArray itemData;
	QDataStream dataStream(&itemData, QIODevice::WriteOnly);
	/// set member-pointer of main_window to the current object in order to gain access to this object from another widget 
	view_->data_scene->main_window->dragged_item = this;
	
	QMimeData *mimeData = new QMimeData;
	mimeData->setData("application/x-csvinputitemdata", itemData);
	drag->setMimeData(mimeData);
	drag->setPixmap(this->pixmap());
	drag->setHotSpot(QPoint(0, 0));
	drag->start();	
	itemChange(ItemPositionChange, pos());
	QGraphicsItem::mousePressEvent(event);
}

void CSVInputDataItem::setXLabelFlag(bool x)
{
	x_labels_ = x;
}

void CSVInputDataItem::setYLabelFlag(bool y)
{
	y_labels_ = y;
}

void CSVInputDataItem::setNumOfActivities(int num)
{
	no_y_ = num;
}

bool CSVInputDataItem::checkForDiscreteY()
{
	return true;
}

void CSVInputDataItem::setAppend(bool append)
{
	append_ = append;
}

bool CSVInputDataItem::append()
{
	return append_;
}
