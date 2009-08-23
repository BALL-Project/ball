/* inputPartitionItem.h
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

#ifndef INPUTPARTDATAITEM_H
#define INPUTPARTDATAITEM_H

#include <BALL/APPLICATIONS/QuEasyViz/partitioningItem.h>


namespace BALL
{
	namespace VIEW
	{
		class InputPartitionItem : public InputDataItem
		{
			public:
					
				/** creates an InputPartitionItem from a given InputDataItem */
				InputPartitionItem(bool test_partition,PartitioningItem* input_item);
				
				~InputPartitionItem();
				
				
				/** (until now) InputPartitionItems just store&visualize partitions of the input data, they do not do anything themselves! */
				bool execute() {return 0;};
				
				
				void removeFromPipeline();
				void addToPipeline();
			
				bool isTestPartition() {return test_partition_;}
				
				bool checkForDiscreteY();
				
				enum { Type = UserType + 133 };
				
				int type() const { return Type; }
				
				String getOutputFilename();
				
		
			private:
				/** is this partition a test-partition?!\n
				(If false, it is used for training) */
				bool test_partition_;
				
				/** PartitioningItem that is used to create the data for this object */
				PartitioningItem* partitioner_;
				
				/** the ID of the fold */
				int fold_ID_;
				
				String output_filename_;
		
		};
		
	
	}
}

#endif // INPUTPARTDATAITEM_H
