#ifndef PARTITIONINGITEM_H
#define PARTITIONINGITEM_H

#include <BALL/APPLICATIONS/QSAR_GUI/dataItem.h>
#include <BALL/APPLICATIONS/QSAR_GUI/inputDataItem.h>

namespace BALL
{
	namespace VIEW
	{
		class PartitioningItem : public DataItem
		{
			public:
				PartitioningItem(InputDataItem* input, DataItemView* miv, uint folds, double& fraction);
				~PartitioningItem();

				bool execute();

				enum { Type = UserType + 128 };
				int type() const { return Type; }
	
				void addToPipeline();
				void removeFromPipeline();
				
				InputDataItem* getInputItem();
				
				double getValFraction();
				uint getNoFolds();
				uint getID();
				
				/** Set the ID. Neccessary only after restoring a PartitioningItem from a config-file. */
				void setID(uint ID);

			protected:
				void mousePressEvent(QGraphicsSceneMouseEvent *event);

			private:
				InputDataItem* input_;
				
				uint folds_;
				
				/** the fraction of compounds of the input data set that is to be used as validation set */
				double val_fraction_;
				
				/** the ID of this PartitioningItem with regard to its InputDataItem.
				It is only used during writing the config-file. */
				uint id_;
				
				
			friend class DataItemScene;

		};
	}
}

#endif
