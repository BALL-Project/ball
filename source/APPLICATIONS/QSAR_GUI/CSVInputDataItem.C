#include <BALL/APPLICATIONS/QSAR_GUI/CSVInputDataItem.h>
#include <BALL/APPLICATIONS/QSAR_GUI/mainWindow.h>

#include <BALL/QSAR/exception.h>
#include <BALL/APPLICATIONS/QSAR_GUI/exception.h>
#include <BALL/APPLICATIONS/QSAR_GUI/inputDataDialog.h>

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
	append_ = 0;

	data_ = new QSARData();
}

CSVInputDataItem::CSVInputDataItem(QSARData* data)	
{
	append_ = true;
	QPixmap pm = QPixmap("./images/csv_icon.png").scaled(QSize(width(), height()), Qt::KeepAspectRatio,Qt::FastTransformation );
	setPixmap(pm);
	data_ = data;
}

CSVInputDataItem::~CSVInputDataItem()
{
	if (view_ && view_->name == "view")
	{
		//if the item was connected to others, delete it from its respective pipeline
		if (!removeDisconnectedItem())
		{
			removeFromPipeline();
		}
	}
}

CSVInputDataItem::CSVInputDataItem(CSVInputDataItem& item):
	InputDataItem(filename_, center_data_, center_y_ ,item.view_)
{
	no_y_ = item. no_y_;
	x_labels_ = item.x_labels_;
	y_labels_ = item.y_labels_;
	append_ = item.append_;
}

void CSVInputDataItem::setSeperator(string sep)
{
	if(sep=="tab") {sep_="	";}
	else sep_=sep;
}


bool CSVInputDataItem::execute()
{
	if(done_) return 0; // do nothing twice...
	
	if(append_) appendData();
	else readData();
	
	return 1;	
}

void CSVInputDataItem::readData()
{
	if(done_) return; // do nothing twice...
	
	string st = filename_.toStdString();
	
	data_->readCSVFile(st.c_str(), no_y_, x_labels_, y_labels_, sep_.c_str(), 0, nonnumeric_class_names_);

	if (center_data_)
	{
		data_->centerData(center_y_);
	}
	cout<<"read CSV file."<<endl;
	
	done_ = 1;
}

void CSVInputDataItem::appendData()
{
	if(done_) return; // do nothing twice..
	
	string st = filename_.toStdString();

	cout<<"trying to append to  "<<data_->getNoSubstances()<<"  "<<data_->getNoDescriptors()<<endl;
	data_->readCSVFile(st.c_str(), no_y_, x_labels_, y_labels_, sep_.c_str(), 1);
	
	if (center_data_)
	{
		data_->centerData(center_y_);
	}
	
	done_ = 1;
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
	if(checked_for_discrete_y_) return discrete_y_;
	
	try
	{
		if(done_) // if data has already been read, check within data-structure
		{
			discrete_y_=data_->checkforDiscreteY();
		}
		else // if data has not been read, check within file
		{
			//discrete_y_=data_->checkforDiscreteY(filename_.toStdString().c_str(), activity_values_);
			return true;
		}
	}
	catch(BALL::Exception::GeneralException e)
	{	
		// e.g. input file has not been found
		QMessageBox::critical(view_,e.getName(),e.getMessage());
		return 0;
	}
	
	checked_for_discrete_y_ = 1;
	return discrete_y_;
}

void CSVInputDataItem::contextMenuEvent(QGraphicsSceneContextMenuEvent* /*event*/)
{
	InputDataDialog inputDataDialog(this);
	inputDataDialog.exec();
}

void CSVInputDataItem::addToPipeline()
{
	view_->data_scene->main_window->csv_input_pipeline_.insert(this);
}

void CSVInputDataItem::removeFromPipeline()
{
	view_->data_scene->main_window->csv_input_pipeline_.erase(this);
}
