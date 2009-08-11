#ifndef PREDICTIONITEM_H
#define PREDICTIONITEM_H

#include <BALL/APPLICATIONS/QuEasyViz/validationItem.h>
#include <BALL/APPLICATIONS/QuEasyViz/modelItem.h>
#include <BALL/APPLICATIONS/QuEasyViz/inputDataItem.h>
#include <BALL/APPLICATIONS/QuEasyViz/dottedEdge.h>
#include <BALL/APPLICATIONS/QuEasyViz/inputPartitionItem.h>


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
			Q_OBJECT
			
			public:
				/** @name Constructors and Destructors*/
				/** Constructor.
				@param input_item The InputDataItem which contains the data-source for the predictions (NOT the training data) */
				PredictionItem(InputDataItem* test_data_item, ModelItem* model, DataItemView* view);

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
				const list<Vector<double> >* results();

				enum { Type = UserType + 6};
				int type() const { return Type; }

				void setDottedEdge(DottedEdge* edge);
				DottedEdge* dottedEdge();

				/** generates the config-file section for the current model and appends it to out */
				void writeConfigSection(ofstream& out);
				
				void addToPipeline();
				void removeFromPipeline();
				
				void addTrainingsPartition(InputPartitionItem* item);
				void addTestPartition(InputPartitionItem* item);
				
				const QSARData* getTestData();
				
				/** save the prediction results to a file */
				void saveToFile(String filename);
				
				/** restore prediction-results from a file */
				void loadFromFile(String filename);
				
				/** overlaods ValidationItem::change() in order to make sure that results_ is cleared */
				void change();
				
				
		
			protected:
				//overlaods ValidationItem::setValidationInput()
				void setValidationInput();
				
			protected slots:
				void showPredictionDialog();
				
			public slots:
				void showPredictionPlotter();
				
			private:
				void createActions();

				/** @name Private Attributes */

				/** results of the prediction */
				list<Vector<double> > results_;
				//ModelItem* model_item_;
				
				/** The InputDataItem which contains the data-source for the predictions (NOT the training data) */
				InputDataItem* test_data_item_;
				
				DottedEdge* dotted_edge_;
				
				list<InputPartitionItem*> trainings_partitions_;
				list<InputPartitionItem*> test_partitions_;	
					
			friend class PredictionPlotter;
		};
	}
}

#endif

