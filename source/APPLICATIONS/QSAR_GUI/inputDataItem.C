#include <BALL/APPLICATIONS/QSAR_GUI/inputDataItem.h>
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

InputDataItem::InputDataItem(QString filename, bool center_data, bool center_y, DataItemView* view):
	DataItem(view),
	filename_(filename),
	center_data_(center_data),
	center_y_(center_y)
{
	data_ = new QSARData;
}

InputDataItem::InputDataItem(QString filename, DataItemView* view): 
	DataItem(view),
	filename_(filename)
{
}

InputDataItem::InputDataItem():
	DataItem(NULL),
	filename_(""),
	center_data_(false),
	center_y_(false),
	data_(NULL)
{
}

InputDataItem::~InputDataItem()
{
	if (view_->name == "view")
	{
		MainWindow* mw = view_->data_scene->main_window;
		mw->removeFromPipeline(this);
	}
	delete data_;
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

