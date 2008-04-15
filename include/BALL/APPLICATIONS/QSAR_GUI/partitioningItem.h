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

				enum { Type = UserType + 2703 };
				int type() const { return Type; }
	
				void addToPipeline();
				void removeFromPipeline();

			protected:
				void mousePressEvent(QGraphicsSceneMouseEvent *event);

			private:
				InputDataItem* input_;
				
				uint folds_;
				
				/** the fraction of compounds of the input data set that is to be used as validation set */
				double val_fraction_;
				
			friend class DataItemScene;

		};
	}
}

#endif
