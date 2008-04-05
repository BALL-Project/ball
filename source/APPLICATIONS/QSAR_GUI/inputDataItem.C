#include <BALL/APPLICATIONS/QSAR_GUI/inputDataItem.h>
#include <BALL/APPLICATIONS/QSAR_GUI/mainWindow.h>

#include <BALL/QSAR/exception.h>
#include <BALL/APPLICATIONS/QSAR_GUI/exception.h>
#include <BALL/APPLICATIONS/QSAR_GUI/inputPlotter.h>

#include <QtGui/QDialog>
#include <QtGui/QDrag>
#include <QtCore/QMimeData>
#include <QtGui/QApplication>
#include <QtGui/QMessageBox>

using namespace BALL::QSAR;
using namespace BALL::QSAR::Exception;
using namespace BALL::VIEW;
using namespace BALL::VIEW::Exception;

InputDataItem::InputDataItem(QString filename, bool center_data, bool center_y, DataItemView* view):
	DataItem(view),
	filename_(filename),
	center_data_(center_data),
	center_y_(center_y)
{
	data_ = new QSARData;
	input_plotter_ = NULL;
}

InputDataItem::InputDataItem(QString filename, DataItemView* view): 
	DataItem(view),
	filename_(filename),
	data_(NULL)
{
	input_plotter_ = NULL;
}

InputDataItem::InputDataItem():
	DataItem(NULL),
	filename_(""),
	center_data_(false),
	center_y_(false),
	data_(NULL)
{
	input_plotter_ = NULL;
}

InputDataItem::~InputDataItem()
{
	delete data_;
	delete input_plotter_;
}

InputDataItem::InputDataItem(InputDataItem& item):
	DataItem(item.view_)
{
	data_ = new QSARData(*item.data_) ;
	filename_ = item.filename_;
	center_data_ = item.center_data_;
	center_y_ = item.center_data_;
	name_ = item.name_;
	setPixmap(item.pixmap());
	center_data_ = item.center_data_;
	center_y_ = item.center_data_;
	input_plotter_ = item.input_plotter_;
}

QSARData* InputDataItem::data()
{
	return data_;
}

QString InputDataItem::filename()
{
	return filename_;
}


bool InputDataItem::centerData()
{
	return center_data_;
}

bool InputDataItem::centerY()
{
	return center_y_;
}

void InputDataItem::setCenterDataFlag(bool cd)
{
	center_data_ = cd;
}

void InputDataItem::setCenterResponseFlag(bool cr)
{
	center_y_ = cr;
}

void InputDataItem::setData(QSARData* data)
{
	data_ = data;
}

void InputDataItem::showPlotter()
{
	if(data_->getNoSubstances()==0)
	{
		QMessageBox::information(view_,"No data","Data must be read before it can be plotted!\nTherefore, click \"Execute Pipeline\" first.");
		return;	
	}
	if(data_->getNoResponseVariables()==0)
	{
		QMessageBox::information(view_,"No response variable", "This input data does not contain a response variable, so that plotting of the response values is not possible.");
		return;
	}
	
	
	if(input_plotter_ == NULL)
	{
		input_plotter_=new InputPlotter(this);
	}
	else
	{
		input_plotter_->show();
	}
}
