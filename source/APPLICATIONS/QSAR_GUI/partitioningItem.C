#include <BALL/APPLICATIONS/QSAR_GUI/partitioningItem.h>
#include <BALL/APPLICATIONS/QSAR_GUI/exception.h>
#include <BALL/APPLICATIONS/QSAR_GUI/mainWindow.h>
#include <BALL/APPLICATIONS/QSAR_GUI/inputPartitionItem.h>

#include <QtGui/QDrag>
#include <QtCore/QMimeData>

using namespace BALL::QSAR;
using namespace BALL::QSAR::Exception;
using namespace BALL::VIEW;
using namespace BALL::VIEW::Exception;


PartitioningItem::PartitioningItem(InputDataItem* input, DataItemView* miv, uint folds, double& fraction):
	DataItem(miv),
	input_(input)
{
	setPixmap(QPixmap("./images/partitioning.png").scaled(QSize(width(), height()), Qt::KeepAspectRatio,Qt::FastTransformation ));
	name_ = "Partitioning of "+ input_->name();
	folds_ = folds;
	val_fraction_ = fraction;
	id_ = input->no_partitioner_;
	input->no_partitioner_++;
}


PartitioningItem::~PartitioningItem()
{
	removeFromPipeline();
}

// void addTrainingsPartition(InputPartitionItem* item)
// {
// 	trainings_partitions_.push_back(item);
// }
// 
// void addTestPartition(InputPartitionItem* item);
// {
// 	test_partitions_.push_back(item);
// }

bool PartitioningItem::execute()
{
	if(done_) return 0;
	
	if(out_edge_list_.size()!=(int)folds_*2)
	{
		throw BALL::Exception::GeneralException(__FILE__,__LINE__,"PartitioningItem:::execute() error","Wrong number of connected InputPartitionItems!");
	}
	
	QSet<Edge*>::iterator it=out_edge_list_.begin();
	for(uint i=0; i<folds_;i++)
	{	cout<<i<<endl;
		QSARData* data = ((InputDataItem*)(*in_edge_list_.begin())->sourceNode())->data();
		vector<QSARData*> sets = data->generateExternalSet(val_fraction_); // length==2
		int a=0;
		for(uint j=0; j<2;j++)
		{
			//if((*it)->destNode()->type()==InputPartitionItem::Type)
			{
				InputPartitionItem* part=(InputPartitionItem*)((*it)->destNode());
				part->setData(sets[a]);
				cout<<"size="<<sets[a]->getNoSubstances()<<"  #features="<<sets[a]->getNoDescriptors()<<endl;
				
				// all children of the InputPartitionItems must be executed again,
				// but InputPartitionItems themselves are already done (data has been loaded!)
				part->change();
				part->setDone(1);
				
				a++;
			}
// 			else
// 			{cout<<"else!!"<<endl;
// 				it++;
// 				j--;
// 				continue;
// 			}
			it++;
		}
	}
	
	done_ = 1;
	return 0;
}

void PartitioningItem::addToPipeline()
{
	view_->data_scene->main_window->partitioning_pipeline_.insert(this);
}

void PartitioningItem::removeFromPipeline()
{
	view_->data_scene->main_window->partitioning_pipeline_.remove(this);
}


InputDataItem* PartitioningItem::getInputItem()
{
	return input_;
}


double PartitioningItem::getValFraction()
{
	return val_fraction_;
}

uint PartitioningItem::getNoFolds()
{
	return folds_;
}

uint PartitioningItem::getID()
{
	return id_;
}

void PartitioningItem::setID(uint ID)
{
	id_ = ID;
}

void PartitioningItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QByteArray itemData;
	QDataStream dataStream(&itemData, QIODevice::WriteOnly);
	QDrag *drag = new QDrag(event->widget());

	/// set member-pointer of main_window to the current object in order to gain access to this object from another widget 
	view_->data_scene->main_window->dragged_item = this;
	view_->data_scene->main_window->drag_source = view_->name;

	QMimeData *mimeData = new QMimeData;
	mimeData->setData("application/x-partitemdata", itemData);
	drag->setMimeData(mimeData);
	drag->setPixmap(pixmap());
	drag->setHotSpot(QPoint(0, 0));
	drag->start();
	itemChange(ItemPositionChange, pos());
	QGraphicsItem::mousePressEvent(event);
}
