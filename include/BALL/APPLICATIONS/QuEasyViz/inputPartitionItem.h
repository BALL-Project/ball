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
