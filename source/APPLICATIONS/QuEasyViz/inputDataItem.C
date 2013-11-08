/* TRANSLATOR BALL::QSAR

		Necessary for lupdate.
*/

#include <inputDataItem.h>
#include <mainWindow.h>

#include <BALL/QSAR/exception.h>
#include <exception.h>
#include <inputPlotter.h>

#include <QtGui/QDialog>
#include <QtGui/QDrag>
#include <QtCore/QMimeData>
#include <QtGui/QApplication>
#include <QtGui/QMessageBox>


using namespace BALL::QSAR;
using namespace BALL::Exception;
using namespace BALL::QSAR::Exception;

namespace BALL
{
	namespace VIEW
	{

		InputDataItem::InputDataItem(QString filename, bool center_data, bool center_y, DataItemView* view):
			DataItem(view),
			filename_(filename),
			center_data_(center_data),
			center_y_(center_y)
		{
			data_ = new QSARData;
			input_plotter_ = NULL;
			append_ = 0;
			checked_for_discrete_y_ = 0;
			discrete_y_ = 0;
			partitioner_IDs_.clear();
			createActions();
		}

		InputDataItem::InputDataItem(QString filename, DataItemView* view): 
			DataItem(view),
			data_(NULL),
			filename_(filename)
		{
			input_plotter_ = NULL;
			append_ = 0;
			checked_for_discrete_y_ = 0;
			discrete_y_ = 0;
			partitioner_IDs_.clear();
			createActions();
		}

		InputDataItem::InputDataItem():
			DataItem(NULL),
			data_(NULL),
			filename_(""),
			center_data_(false),
			center_y_(false)
		{
			input_plotter_ = NULL;
			append_ = 0;
			checked_for_discrete_y_ = 0;
			discrete_y_ = 0;
			partitioner_IDs_.clear();
			createActions();
		}

		InputDataItem::~InputDataItem()
		{
			if(!append_) delete data_;
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
			append_ = item.append_;
			checked_for_discrete_y_ = item.checked_for_discrete_y_;
			discrete_y_ = item.discrete_y_;
			partitioner_IDs_ = item.partitioner_IDs_;
			createActions();
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

		void InputDataItem::loadFromFile(String file)
		{
			try
			{
				if(data_==NULL)
				{
					//throw BALL::Exception::GeneralException(__FILE__,__LINE__,"InputDataItem error","InputDataItem not connected to a QSARData object!!");		
					data_ = new QSARData;
				}
				data_->readFromFile(file);
			}
			catch(WrongDataType e)
			{
				QMessageBox::warning(view_,"Error",e.getMessage());
				return;
			}
			done_ = 1;
		}

		void InputDataItem::showPlotter()
		{
			if(data_==0 || data_->getNoSubstances()==0)
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
			input_plotter_->show();
		}

		void InputDataItem::setAppend(bool append)
		{
			append_ = append;
		}

		bool InputDataItem::append()
		{
			return append_;
		}

		BALL::String InputDataItem::getMouseOverText()
		{
			String s="";
			if(data_!=NULL)
			{
				if(data_->getNoSubstances()>0)
				{
					s=String(data_->getNoSubstances())+" compounds\n";
					s+=String(data_->getNoDescriptors())+" features\n";
					int no_y=data_->getNoResponseVariables();
					s+=String(no_y)+" response variable";
					if(no_y!=1) s+="s";
				}
			}
			return s;
		}

		void InputDataItem::createActions()
		{
			QAction* plot_action = new QAction(QIcon(""),tr("Plot data"), this);
			connect(plot_action, SIGNAL(triggered()), this, SLOT(showPlotter()));
			context_menu_actions_.push_back(plot_action);
		}
	}
}
