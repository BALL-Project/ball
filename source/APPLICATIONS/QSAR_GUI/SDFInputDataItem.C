#include <BALL/APPLICATIONS/QSAR_GUI/SDFInputDataItem.h>
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


SDFInputDataItem::SDFInputDataItem(QString filename,SortedList<int> act, bool cdv, bool crv, DataItemView* view):
	InputDataItem(filename, cdv, crv, view),
	activity_values_(act)
{
	//set pixmap
	QPixmap pm = QPixmap("./images/sdf_icon.png").scaled(QSize(width(), height()), Qt::KeepAspectRatio,Qt::FastTransformation );
	setPixmap(pm);

	QStringList list = filename_.split("/");
	setName(list[list.size()-1]);

	string st = filename.toStdString();
	
	data_ = new QSARData;

	try
	{
		data_->readSDFile(st.c_str(), act);
	}
	catch(WrongFileFormat)
	{
		throw(InvalidInputDataItem(__FILE__,__LINE__));
	}
		
	///this one occurs if there were some properties missing -> this type of sd-files is not supported at the moment
	catch(PropertyError)
	{
		throw(InvalidInputDataItem(__FILE__,__LINE__));
	}

	center_data_ = cdv;
	center_y_ = crv;

	if (center_data_)
	{
		data_->centerData(center_y_);
	}
}

//create DataItem from a save file
SDFInputDataItem::SDFInputDataItem(QString filename, QString name, DataItemView* view):
	InputDataItem(filename, view)
{
	//set properties for visualisation 
	QPixmap pm = QPixmap(width(),height());
	pm.fill(Qt::blue);
	setPixmap(pm);
	this->setName(name);
	
	data_ = new QSARData;
	data_->readFromFile(filename.toStdString());
}

SDFInputDataItem::SDFInputDataItem(QString filename, DataItemView* view):
	InputDataItem(filename, false, false, view)
{
	QPixmap pm = QPixmap("./images/sdf_icon.png").scaled(QSize(width(), height()), Qt::KeepAspectRatio,Qt::FastTransformation );
	setPixmap(pm);

	QStringList list = filename_.split("/");
	setName(list[list.size()-1]);
	activity_values_.clear();

	data_ = new QSARData;
}

SDFInputDataItem::~SDFInputDataItem()
{
	if (view_->name == "view")
	{
		MainWindow* mw = view_->data_scene->main_window;
		mw->removeFromPipeline(this);
	}
}

SDFInputDataItem::SDFInputDataItem(SDFInputDataItem& item):
	InputDataItem(filename_, center_data_, center_y_ ,item.view_)
{
	data_ = new QSARData(*item.data_) ;
 	activity_values_.assign(item.activity_values_.begin(), item.activity_values_.end() );
	name_ = item.name_;
	setPixmap(item.pixmap());
}

void SDFInputDataItem::readData()
{
	if(done_) return; // do nothing twice...
	
	string st = filename_.toStdString();
	try
	{
		data_->readSDFile(st.c_str(), activity_values_);

// 		foreach(CSVFileInfo* csv, csv_files_)
// 		{
// 			data_->readCSVFile((csv->file.toStdString()).c_str(), csv->no_y, csv->x_labels, csv->y_labels, csv->sep, 1);
// 		}
	}
	catch(WrongFileFormat)
	{
		throw(InvalidInputDataItem(__FILE__,__LINE__));
	}
	///this one occurs if there were some properties missing
	catch(PropertyError)
	{
		throw(InvalidInputDataItem(__FILE__,__LINE__));
	}

	///erst machen, wenn alle Compounds gelesen sind!!!!!!!!!!
	if (center_data_)
	{
		data_->centerData(center_y_);
	}
	
	done_ = 1;
}

void SDFInputDataItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{	
	QPoint startPos = event->pos().toPoint();
	QDrag *drag = new QDrag(event->widget());
	QByteArray itemData;
	QDataStream dataStream(&itemData, QIODevice::WriteOnly);
	/// set member-pointer of main_window to the current object in order to gain access to this object from another widget 
	view_->data_scene->main_window->dragged_item = this;
	
	QMimeData *mimeData = new QMimeData;
	mimeData->setData("application/x-sdfinputitemdata", itemData);
	drag->setMimeData(mimeData);
	drag->setPixmap(this->pixmap());
	drag->setHotSpot(QPoint(0, 0));
	drag->start();	
	itemChange(ItemPositionChange, pos());
	QGraphicsItem::mousePressEvent(event);
}

bool SDFInputDataItem::checkForDiscreteY()
{
	return data_->checkforDiscreteY(filename_.toStdString().c_str(), activity_values_);
}

SortedList<int> SDFInputDataItem::activityValues()
{
	return activity_values_;
}

void SDFInputDataItem::setActivityValues(SortedList<int> act)
{
	activity_values_.assign(act.begin(), act.end());
}

// void SDFSDFInputDataItem::addCSVFile(CSVFileInfo* csv)
// {
// 	if (csv)
// 	{
// 		csv_files_ << csv;
// 	}
// }

