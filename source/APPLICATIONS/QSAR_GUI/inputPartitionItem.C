#include <BALL/APPLICATIONS/QSAR_GUI/inputPartitionItem.h>
#include <BALL/APPLICATIONS/QSAR_GUI/mainWindow.h>
#include <QtGui/QMessageBox>
#include <QtGui/QDrag>


using namespace BALL::VIEW;



InputPartitionItem::InputPartitionItem(bool test_partition, PartitioningItem* partitioner)
{
	test_partition_ = test_partition;
	view_ = partitioner->view_;
	fold_ID_ = partitioner->outEdges().size()/2;
	partitioner_ = partitioner;
	
	String name = partitioner->getInputItem()->name().toStdString();
	int index = name.find_last_of("/");
	if(index!=string::npos)
	{
		name=name.substr(index);
	}
	index = name.find_first_of(".");
	if(index!=string::npos)
	{
		name=name.substr(0,index);
	}
	name_ = name.c_str();
	
	if(partitioner->getID()>0)
	{
		name=name+"_"+partitioner->getID()+"_";
	}
	
	if(test_partition) name+="_TEST";		
	else name+="_TRAIN";
	name += String(fold_ID_)+".dat";
	
	saved_as_ = name.c_str();
	
	if(test_partition) name_ = "validation";
	else name_ = "train";
	//TODO: set pixmap depending on whether test_partition_==1 or not...
}


InputPartitionItem::~InputPartitionItem()
{
	removeFromPipeline();
	
	//check whether partitioner_ has not been deleted yet (which might be the case when clearing entire desktop) 
	Pipeline<PartitioningItem*>& pipe = view_->data_scene->main_window->partitioning_pipeline_;
	if(pipe.find(partitioner_)!=pipe.end())
	{
		partitioner_->removePartition(this);
	}
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


void InputPartitionItem::removeFromPipeline()
{
	view_->data_scene->main_window->partition_pipeline_.erase(this);	
}


void InputPartitionItem::addToPipeline()
{
	view_->data_scene->main_window->partition_pipeline_.insert(this);	
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
