#include <BALL/APPLICATIONS/QuEasyViz/CSVInputDataItem.h>
#include <BALL/APPLICATIONS/QuEasyViz/mainWindow.h>

#include <BALL/QSAR/exception.h>
#include <BALL/APPLICATIONS/QuEasyViz/exception.h>
#include <BALL/APPLICATIONS/QuEasyViz/inputDataDialog.h>

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
	String dir = view_->data_scene->main_window->getImageDirectory();
	QPixmap pm = QPixmap((dir+"csv_icon.png").c_str()).scaled(QSize(width(), height()), Qt::KeepAspectRatio,Qt::FastTransformation );
	setPixmap(pm);

	QStringList list = filename_.split("/");
	setName(list[list.size()-1]);
	append_ = 0;

	data_ = new QSARData();
}

CSVInputDataItem::CSVInputDataItem(QSARData* data, DataItemView* view)
{
	view_ = view;
	append_ = true;
	String dir = view_->data_scene->main_window->getImageDirectory();
	QPixmap pm = QPixmap((dir+"csv_icon.png").c_str()).scaled(QSize(width(), height()), Qt::KeepAspectRatio,Qt::FastTransformation );
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
	if(sep=="tabulator") {sep_="	";}
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
	
	done_ = 1;
}

void CSVInputDataItem::appendData()
{
	if(done_) return; // do nothing twice..
	
	string st = filename_.toStdString();

	data_->readCSVFile(st.c_str(), no_y_, x_labels_, y_labels_, sep_.c_str(), 1, nonnumeric_class_names_);
	
	if (center_data_)
	{
		data_->centerData(center_y_);
	}
	
	done_ = 1;
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
	view_->data_scene->main_window->all_items_pipeline_.insert(this);
}

void CSVInputDataItem::removeFromPipeline()
{
	view_->data_scene->main_window->csv_input_pipeline_.erase(this);
	view_->data_scene->main_window->all_items_pipeline_.erase(this);
}


void CSVInputDataItem::replaceItem(InputDataItem* old_item)
{
	transferEdges(old_item);  // steal the old item's edges
	
	// put the new item into the the pipeline at the same position
	CSVInputDataItem* old_csv = dynamic_cast<CSVInputDataItem*>(old_item);
	SDFInputDataItem* old_sdf = dynamic_cast<SDFInputDataItem*>(old_item); 
	if(old_csv)
	{
		Pipeline<CSVInputDataItem*>::iterator csv_it=view_->data_scene->main_window->csv_input_pipeline_.find(old_csv);
		if(csv_it!=view_->data_scene->main_window->csv_input_pipeline_.end())
		{
			view_->data_scene->main_window->csv_input_pipeline_.insert(csv_it,this);
			view_->data_scene->main_window->csv_input_pipeline_.erase(csv_it);
		}
		else view_->data_scene->main_window->csv_input_pipeline_.insert(this);
	}
	else if(old_sdf)
	{
		// delete all features that were read from CSV-files appended to the old SDF-item
		for(list<CSVInputDataItem*>::iterator csv_it=old_sdf->getConnectedCSVItems()->begin(); csv_it!=old_sdf->getConnectedCSVItems()->end(); csv_it++)
		{
			delete *csv_it;
		}
		view_->data_scene->main_window->csv_input_pipeline_.insert(this);
	}
	else view_->data_scene->main_window->csv_input_pipeline_.insert(this);	
	
	Pipeline<DataItem*>::iterator it = view_->data_scene->main_window->all_items_pipeline_.find(old_item);
	if(it!=view_->data_scene->main_window->all_items_pipeline_.end())
	{
		view_->data_scene->main_window->all_items_pipeline_.insert(it,this);
	}
	else
	{
		view_->data_scene->main_window->all_items_pipeline_.insert(this);	
	}
	
	// kill the old item
	delete old_item;	
	
	// finially, make sure that the entire pipeline created by use of this input is reset
	change();
}
