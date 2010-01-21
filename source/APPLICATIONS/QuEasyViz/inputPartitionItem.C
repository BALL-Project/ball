/* inputPartitionItem.C
 * 
 * Copyright (C) 2009 Marcel Schumann
 * 
 * This file is part of QuEasy -- A Toolbox for Automated QSAR Model
 * Construction and Validation.
 * QuEasy is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 * 
 * QuEasy is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#include <inputPartitionItem.h>
#include <mainWindow.h>
#include <QtGui/QMessageBox>
#include <QtGui/QDrag>


using namespace BALL::VIEW;



InputPartitionItem::InputPartitionItem(bool test_partition, PartitioningItem* partitioner)
{
	test_partition_ = test_partition;
	view_ = partitioner->view_;
	fold_ID_ = partitioner->outEdges().size()/2;
	partitioner_ = partitioner;
	
	/// set saved_as_, so that it can be used during restoring a pipeline
	String postfix="";
	if(partitioner->getID()>0)
	{
		postfix="_"+String(partitioner->getID());
	}
	if(test_partition) postfix+="_TEST";		
	else postfix+="_TRAIN";
	postfix += String(fold_ID_)+".dat";
	String s = partitioner->getInputItem()->savedAs().toStdString();
	int index2 = s.find_first_of(".");
	if(index2!=(int)string::npos)
	{
		saved_as_ = s.substr(0,index2).c_str();
	}		
	saved_as_+=postfix.c_str();
	
	/// set pixmap and name of this item
	QPixmap pm;
	String dir = view_->data_scene->main_window->getImageDirectory();
	if(test_partition)
	{
		pm = QPixmap((dir+"test_part.png").c_str()).scaled(QSize(width(), height()), Qt::KeepAspectRatio,Qt::FastTransformation);
		name_ = "validation";
	}
	else
	{
		pm = QPixmap((dir+"train_part.png").c_str()).scaled(QSize(width(), height()), Qt::KeepAspectRatio,Qt::FastTransformation);
		name_ = "train";
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

	output_filename_ = partitioner_->getInputItem()->name().toStdString();
	int index = output_filename_.find_last_of("/");
	if(index!=(int)string::npos)
	{
		output_filename_=output_filename_.substr(index);
	}
	index = output_filename_.find_first_of(".");
	if(index!=(int)string::npos)
	{
		output_filename_=output_filename_.substr(0,index);
	}
	
	String postfix="";
	if(partitioner_->getID()>0)
	{
		postfix="_"+String(partitioner_->getID());
	}
	if(test_partition_) postfix+="_TEST";		
	else postfix+="_TRAIN";
	postfix += String(fold_ID_)+".dat";
	
	output_filename_+=postfix;	
	
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
