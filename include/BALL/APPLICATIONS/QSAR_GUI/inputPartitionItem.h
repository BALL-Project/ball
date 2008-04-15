#ifndef INPUTPARTDATAITEM_H
#define INPUTPARTDATAITEM_H

#include <BALL/APPLICATIONS/QSAR_GUI/inputDataItem.h>


namespace BALL
{
	namespace VIEW
	{
		class InputPartitionItem : public InputDataItem
		{
			public:
					
				/** creates an InputPartitionItem from a given InputDataItem */
				InputPartitionItem(bool test_partition, DataItemView* view);
				
				
				/** (until now) InputPartitionItems just store&visualize partitions of the input data, they do not do anything themselves! */
				bool execute() {return 0;};
				void removeFromPipeline() {};
				void addToPipeline() {};
			
				
				bool checkForDiscreteY();
				
				enum { Type = UserType + 22 };
				
		
			private:
				/** is this partition a test-partition?!\n
				(If false, it is used for training) */
				bool test_partition_;
				
				void mousePressEvent(QGraphicsSceneMouseEvent *event);
		
		};
	
	}
}

#endif // INPUTPARTDATAITEM_H
