#include <BALL/APPLICATIONS/QSAR_GUI/SDFInputDataItem.h>
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



/// TODO: set additional_descriptors_ and center data only if ==0 !!!
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
/// TODO: set additional_descriptors_ and center data only if ==0 !!!
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
	
	// base class destructor will delete the connected QSARData object,
	// so make sure that CSVInputDataItems that use the same QSARData object do not delete it a second time!
	for(list<CSVInputDataItem*>::iterator it=additional_descriptors_.begin();it!=additional_descriptors_.end();it++)
	{
		(*it)->setData(NULL);
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
		Path p;
		if(p.find("QSAR/atomic_electron_affinities.data")=="")
		{    	// use subfolder of executable location...
			data_->setDataFolder("./data");
		}
		data_->readSDFile(st.c_str(), activity_values_);
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

	/// center data only if desired by the user and if there are no additional descriptors being read from a csv-file. In the latter case, centering is left to be done by the CSVInputDataItem.
	if (additional_descriptors_.size()==0 && center_data_)
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

void SDFInputDataItem::contextMenuEvent(QGraphicsSceneContextMenuEvent* /*event*/)
{
	InputDataDialog inputDataDialog(this);
	inputDataDialog.exec();
}

void SDFInputDataItem::appendCSVDescriptors(CSVInputDataItem* item)
{
	/// make sure that centering is ONLY done by the last CSVInputDataItem!
	if(additional_descriptors_.size()!=0)
	{
		CSVInputDataItem* csv_item = additional_descriptors_.back();
		csv_item->setCenterDataFlag(0);
		csv_item->setCenterResponseFlag(0);
	}
	additional_descriptors_.push_back(item);
}


list<CSVInputDataItem*>* SDFInputDataItem::getConnectedCSVItems()
{
	return &additional_descriptors_;
}