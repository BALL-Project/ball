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
	
	output_filename_ = partitioner->getInputItem()->name().toStdString();
	int index = output_filename_.find_last_of("/");
	if(index!=string::npos)
	{
		output_filename_=output_filename_.substr(index);
	}
	index = output_filename_.find_first_of(".");
	if(index!=string::npos)
	{
		output_filename_=output_filename_.substr(0,index);
	}
	
	String postfix="";
	if(partitioner->getID()>0)
	{
		postfix="_"+String(partitioner->getID());
	}
	if(test_partition) postfix+="_TEST";		
	else postfix+="_TRAIN";
	postfix += String(fold_ID_)+".dat";
	
	output_filename_+=postfix;	
	
	if(test_partition) name_ = "validation";
	else name_ = "train";
	
	/// set saved_as_, the name of this item prefixed by the name of the configfile
	String s = partitioner->getInputItem()->savedAs().toStdString();
	int index2 = s.find_first_of(".");
	if(index2!=string::npos)
	{
		saved_as_ = s.substr(0,index2).c_str();
	}		
	saved_as_+=postfix.c_str();
	 
	//cout<<"output_filename_="<<output_filename_<<endl;
	//cout<<"saved_as_="<<saved_as_.toStdString()<<endl<<endl;
	
	QPixmap pm;
	//TODO: set pixmap depending on whether test_partition_==1 or not...
	if(test_partition)
	{
		pm = QPixmap("./images/test_part.png").scaled(QSize(width(), height()), Qt::KeepAspectRatio,Qt::FastTransformation);
	}
	else
	{
		pm = QPixmap("./images/train_part.png").scaled(QSize(width(), height()), Qt::KeepAspectRatio,Qt::FastTransformation);
	}
	setPixmap(pm);
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


BALL::String InputPartitionItem::getOutputFilename()
{

	return output_filename_;
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
