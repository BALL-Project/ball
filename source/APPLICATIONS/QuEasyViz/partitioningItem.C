/* partitioningItem.C
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

#include <partitioningItem.h>
#include <exception.h>
#include <mainWindow.h>
#include <inputPartitionItem.h>


#include <QtGui/QDrag>
#include <QtCore/QMimeData>

using namespace BALL::QSAR;
using namespace BALL::Exception;

namespace BALL
{
	namespace VIEW
	{

		PartitioningItem::PartitioningItem(InputDataItem* input, DataItemView* miv, uint folds, double& fraction, int ID):
			DataItem(miv),
			input_(input)
		{
			String dir = view_->data_scene->main_window->getImageDirectory();
			setPixmap(QPixmap((dir+"partitioning.png").c_str()).scaled(QSize(width(), height()), Qt::KeepAspectRatio,Qt::FastTransformation ));
			name_ = "Partitioning of "+ input_->name();
			no_folds_ = folds;
			val_fraction_ = fraction;
			
			if(ID==-1) // if this item is _not_ been restored from an archive (i.e. no ID was set for this item)
			{
				if(!input->partitioner_IDs_.empty()) id_=input->partitioner_IDs_.back()+1;
				else id_=0;
			}
			else
			{
				id_ = ID;
			}
			input->partitioner_IDs_.push_back(id_);
		}


		PartitioningItem::~PartitioningItem()
		{
			removeFromPipeline();
		}


		bool PartitioningItem::execute()
		{
			if(isDone()) return 0;
			
			if(out_edge_list_.size()!=no_folds_*2 || out_edge_list_.size()!=2*folds_.size())
			{
				throw BALL::Exception::GeneralException(__FILE__,__LINE__,"PartitioningItem:::execute() error","Wrong number of connected InputPartitionItems!");
			}
			
			set<Edge*>::iterator it=out_edge_list_.begin();
			for(list<pair<InputPartitionItem*,InputPartitionItem*> >::iterator it=folds_.begin(); it!=folds_.end(); it++)
			{	
				QSARData* data = ((InputDataItem*)(*in_edge_list_.begin())->sourceNode())->data();
				vector<QSARData*> sets = data->generateExternalSet(val_fraction_); // length==2
				
				if(data->isDataCentered())
				{
					bool center_y = data->isResponseCentered();
					sets[0]->centerData(center_y); // train-partition
					sets[1]->centerData(center_y); // test-partition
				}
				delete it->first->data();  // delete old data-objetcs (if any)
				delete it->second->data(); 
				it->first->setData(sets[0]);
				it->second->setData(sets[1]);
				
				// all children of the InputPartitionItems must be executed again,
				// but InputPartitionItems themselves are already done (data has been loaded!)
				it->first->change();
				it->first->setDone(1);
				it->second->change();
				it->second->setDone(1);	
			}
			done_ = 1;
			return 1;
		}

		bool PartitioningItem::isDone()
		{
			if(done_) return 1;
			
			bool all_children_done=1;
			for(list<pair<InputPartitionItem*,InputPartitionItem*> >::iterator it=folds_.begin(); it!=folds_.end(); it++)
			{
				if(!it->first->isDone())
				{
					all_children_done = 0;
					break;
				}
				if(!it->second->isDone())
				{
					all_children_done = 0;
					break;
				}
			}
			
			return all_children_done;	
		}

		void PartitioningItem::addToPipeline()
		{
			view_->data_scene->main_window->partitioning_pipeline_.insert(this);
			view_->data_scene->main_window->all_items_pipeline_.insert(this);
		}

		void PartitioningItem::removeFromPipeline()
		{
			view_->data_scene->main_window->partitioning_pipeline_.erase(this);
			view_->data_scene->main_window->all_items_pipeline_.erase(this);
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
			return no_folds_;
		}

		uint PartitioningItem::getID()
		{
			return id_;
		}

		void PartitioningItem::setID(uint ID)
		{
			id_ = ID;
		}

		void PartitioningItem::addFold(pair<InputPartitionItem*,InputPartitionItem*> fold)
		{
			folds_.push_back(fold);
		}


		void PartitioningItem::removePartition(InputPartitionItem* partition)
		{
			for(list<pair<InputPartitionItem*,InputPartitionItem*> >::iterator it= folds_.begin(); it!=folds_.end(); it++)
			{
				if(it->first==partition)
				{
					InputPartitionItem* test_part = it->second;
					it = folds_.erase(it);
					no_folds_--;
					delete test_part;
					break;
				}
				if(it->second==partition)
				{
					InputPartitionItem* train_part = it->first;
					it = folds_.erase(it);
					no_folds_--;
					delete train_part;	
					break;
				}			
			}	
		}


		BALL::String PartitioningItem::getMouseOverText()
		{
			String s="Random partitioning of input\n";
			s+="  fraction of data set aside for validation="+valueToString(val_fraction_);
			return s;
		}

		void PartitioningItem::setInput(InputDataItem* new_input)
		{
			input_= new_input;
		}
	}
}