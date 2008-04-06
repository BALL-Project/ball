#ifndef VALIDATIONITEM_H
#define VALIDATIONITEM_H

#include <BALL/APPLICATIONS/QSAR_GUI/dataItem.h>
#include <BALL/APPLICATIONS/QSAR_GUI/modelItem.h>

namespace BALL
{
	namespace VIEW
	{
		/** @class ModelItem
		* @brief graphical representation of a model validation 
		*
		* @todo
		*/
		class ValidationItem : public DataItem
		{
			public:
				/** @name Constructors and Destructors*/

				/**
				* constructor.
				* @param type the id of the validation method \n
				* 1: test quality of fit \n
				* 2: test predictive power with cross validation \n
				* 3: test predictive power with bootstrapping \n
				* 4: response permutation test 

				* @param view pointer to the item's view
				*/
				ValidationItem(int type, DataItemView* view);

				/** copy constructor
				* @param item ValidadionItem that is to be copied
				*/
				ValidationItem(ValidationItem& item);

				/** standard destructor
				*/
				~ValidationItem();

				/**
				* carries out the chosen valdidation method and asks for parameters (if needed)
				* @param modelitem pointer to the ModelItem to which this item should be connected	
				*/
				void connectWithModelItem();
		
				/** @name Enums */
				ModelItem* modelItem();
				void setModelItem(ModelItem* model_item);
				
				void setValidationStatistic(int s) {validation_statistic_=s;};
				int getValidationStatistic() {return validation_statistic_;};
				
				String getStatName();

				/** 
				* Type of the Item 
				* @see BALL::VIEW::DataItem
				*/
				enum { Type = UserType + 5 };

				/** @name Accessors */

				/**
				* returns the item's type
				*/
				int type() const { return Type; }
				int getValidationType();
				int k();
				void setK(int k);
				void setNumOfRuns(int num);
				void setNumOfSamples(int num);
				int numOfRuns();
				int numOfSamples();

				Matrix* resultOfRandTest();

				double getQ2();
				double getR2();
				
				/** generates the config-file section for the current model and appends it to out */
				void writeConfigSection(ofstream& out);
		
			protected:
			
				/** @name Protected Event Handlers */

				/**
				* QT event handler \n
				* defines how the item deals with mouse clicks
				*/
				void mousePressEvent(QGraphicsSceneMouseEvent *event);
				void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
		
				/** @name Private Attributes */
	
				/**
				*type of the chosen validation method
				*/
				int type_;
				
				bool is_classification_;
		
				int k_;
			
				int num_of_samples_;
			
				int num_of_runs_;

				/**
				* the model item which is connected to this validation item and whose model is validated
				*/
				ModelItem* model_item_;

				Matrix result_of_rand_test_;

				double q2_;
				
				double r2_;
				
				int validation_statistic_;
				
				bool done_;
				
				
				friend class DataItemScene;
		
		};
	}
}

#endif

