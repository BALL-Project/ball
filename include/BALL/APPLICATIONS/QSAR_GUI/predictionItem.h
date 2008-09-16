#ifndef PREDICTIONITEM_H
#define PREDICTIONITEM_H

#include <BALL/APPLICATIONS/QSAR_GUI/validationItem.h>
#include <BALL/APPLICATIONS/QSAR_GUI/modelItem.h>
#include <BALL/APPLICATIONS/QSAR_GUI/inputDataItem.h>
#include <BALL/APPLICATIONS/QSAR_GUI/dottedEdge.h>
#include <BALL/APPLICATIONS/QSAR_GUI/inputPartitionItem.h>


namespace BALL
{
	namespace VIEW
	{
		/** @class PredictionItem
		* @brief graphical representation of the prediction of activity values for compounds with a QSAR model
		*
		* @todo
		*/
		
		class PredictionPlotter;
		
		class PredictionItem : public ValidationItem
		{
			public:
				/** @name Constructors and Destructors*/
				PredictionItem(InputDataItem* input_item, ModelItem* model, DataItemView* view);

				/** copy constructor */
				PredictionItem(PredictionItem& item);
				
				/** Creates a PredictionItem from the geiven config-file section, registers it in the view and in the Pipeline-QSets of MainWindow and creates edges */
				PredictionItem(String& configfile_section, std::map<String, DataItem*>& filenames_map, list<pair<double,double> >* item_positions, DataItemView* view);

				/** destructor */
				~PredictionItem();
	
				bool execute();
				
				/** @name Accessors */

				InputDataItem* inputDataItem();

				ModelItem* modelItem();

				/** returns the results of the prediction*/
				const QList<Vector<double> >* results();

				enum { Type = UserType + 6};
				int type() const { return Type; }

				void setDottedEdge(DottedEdge* edge);
				DottedEdge* dottedEdge();

				void showPredictionPlotter();
				
				/** generates the config-file section for the current model and appends it to out */
				void writeConfigSection(ofstream& out);
				
				void addToPipeline();
				void removeFromPipeline();
				
				void addTrainingsPartition(InputPartitionItem* item);
				void addTestPartition(InputPartitionItem* item);
				
		
			protected:
				void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
				
				//overlaods ValidationItem::setValidationInput()
				void setValidationInput();

				
			private:

				/** @name Private Attributes */

				/** results of the prediction */
				QList<Vector<double> > results_;
				//ModelItem* model_item_;
				InputDataItem* input_data_item_;
				DottedEdge* dotted_edge_;
				
				PredictionPlotter* pred_plotter_;
				
				QSARData* test_data_;
				
				list<InputPartitionItem*> trainings_partitions_;
				list<InputPartitionItem*> test_partitions_;		
		};
	}
}

#endif

