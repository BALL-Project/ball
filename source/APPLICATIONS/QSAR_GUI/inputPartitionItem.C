#include <BALL/APPLICATIONS/QSAR_GUI/inputPartitionItem.h>
#include <BALL/APPLICATIONS/QSAR_GUI/mainWindow.h>
#include <QtGui/QMessageBox>
#include <QtGui/QDrag>


using namespace BALL::VIEW;



InputPartitionItem::InputPartitionItem(bool test_partition, DataItemView* view)
{
	test_partition_ = test_partition;
	view_ = view;
	//TODO: set pixmap depending on whether test_partition_==1 or not...
}


bool InputPartitionItem::checkForDiscreteY()
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


void InputPartitionItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QByteArray itemData;
	QDataStream dataStream(&itemData, QIODevice::WriteOnly);
	QDrag *drag = new QDrag(event->widget());

	/// set member-pointer of main_window to the current object in order to gain access to this object from another widget 
	view_->data_scene->main_window->dragged_item = this;
	view_->data_scene->main_window->drag_source = view_->name;

	QMimeData *mimeData = new QMimeData;
	mimeData->setData("application/x-inputpartitemdata", itemData);
	drag->setMimeData(mimeData);
	drag->setPixmap(pixmap());
	drag->setHotSpot(QPoint(0, 0));
	drag->start();
	itemChange(ItemPositionChange, pos());
	QGraphicsItem::mousePressEvent(event);
}
