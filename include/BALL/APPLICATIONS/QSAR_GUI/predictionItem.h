#ifndef PREDICTIONITEM_H
#define PREDICTIONITEM_H

#include <BALL/APPLICATIONS/QSAR_GUI/dataItem.h>
#include <BALL/APPLICATIONS/QSAR_GUI/modelItem.h>
#include <BALL/APPLICATIONS/QSAR_GUI/inputDataItem.h>
#include <BALL/APPLICATIONS/QSAR_GUI/dottedEdge.h>


namespace BALL
{
	namespace VIEW
	{
		/** @class PredictionItem
		* @brief graphical representation of the prediction of activity values for compounds with a QSAR model
		*
		* @todo
		*/
		class PredictionItem : public DataItem
		{
			public:
				/** @name Constructors and Destructors*/
				PredictionItem(InputDataItem* input_item, ModelItem* model, DataItemView* view);

				/** copy constructor */
				PredictionItem(PredictionItem& item);

				/** destructor */
				~PredictionItem();
	
				void connectWithModelItem();
				
				/** @name Accessors */

				InputDataItem* inputDataItem();

				ModelItem* modelItem();

				/** returns the results of the prediction*/
				const QList<RowVector>* results();

				enum { Type = UserType + 6};
				int type() const { return Type; }

				void setDottedEdge(DottedEdge* edge);
				DottedEdge* dottedEdge();

		
			protected:
				void mousePressEvent(QGraphicsSceneMouseEvent *event);

			private:

				/** @name Private Attributes */

				/** results of the prediction */
				QList<RowVector> results_;
				ModelItem* model_item_;
				InputDataItem* input_data_item_;
				DottedEdge* dotted_edge_;
		
		};
	}
}

#endif

