/* validationItem.h
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

#ifndef VALIDATIONITEM_H
#define VALIDATIONITEM_H

#include <dataItem.h>
#include <modelItem.h>
#include <predictionPlotter.h>
#include <partitioningItem.h>

namespace BALL
{
	namespace VIEW
	{
		/** @class ValidationItem
		* @brief graphical representation of a model validation 
		*
		* @todo
		*/
		class ValidationItem : public DataItem
		{
			Q_OBJECT

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
				
				/** Creates a ValidationItem from the geiven config-file section, registers it in the view and in the Pipeline-QSets of MainWindow and creates edges */
				ValidationItem(String& configfile_section, std::map<String, DataItem*>& filenames_map, list<pair<double,double> >* item_positions, DataItemView* view);

				/** standard destructor
				*/
				~ValidationItem();

				/** start the validation */
				bool execute();
		
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
				void setNumOfNCVFolds(int num);
				int numOfRuns();
				int numOfSamples();
				int numOfNCVFolds();
				
				void setValFraction(double frac);
				double getValFraction();

				Eigen::MatrixXd* resultOfRandTest();
				
				void addExternalFoldValidation(ValidationItem* item);
				
				/** returns the number of nested cross validation folds in case of nested cross validation (else returns 0) */
				int getNoExternalFolds();

				double getQ2();
				double getR2();
				
				/** generates the config-file section for the current model and appends it to out */
				void writeConfigSection(ofstream& out);
				
				void setPartitioner(PartitioningItem* partitioner);

				virtual void addToPipeline();
				virtual void removeFromPipeline();
				
				BALL::String getMouseOverText();
				
				/** overloads DataItem::change(), so that all plotters that have already been created are deleted */
				void change();
				
				/** save the result of the applied validation methods to a file */
				void saveToFile(String filename);
				
				/** restore validation-results from a file */
				void loadFromFile(String filename);
				
		
			protected:
			
				/** @name Protected Event Handlers */

				void initName();
				virtual void setValidationInput();
				
				/** created context menu actions and adds them to member-list context_menu_actions_ */
				virtual void createActions();
			
		
				/** @name Private Attributes */
	
				/**
				*type of the chosen validation method
				*/
				int type_;
				
				bool is_classification_;
		
				int k_;
			
				int num_of_samples_;
			
				int num_of_runs_;

				int n_of_ncv_folds_;

				/**
				* the model item which is connected to this validation item and whose model is validated
				*/
				ModelItem* model_item_;

				Eigen::MatrixXd result_of_rand_test_;
				
				Eigen::MatrixXd coeff_stddev_;

				double q2_;
				
				double r2_;
				
				double coeff_stderr_ratio_;
				
				/** in case of nested cross validation: the fraction of the input data that should be set aside for validation */
				double val_fraction_;
				
				/** in case of nested cross validation this member will contain pointers to the validations of the nested cross validation folds, so that the average quality statistic can be calculated */
				list<ValidationItem*> external_validations_;
				
				/** in case of nested cross validation this member will contain a pointer to the PartitioningItem used to create training- and validation-partitions */
				PartitioningItem* partitioner_;
				
				/** in case of a item that holds the results of validation of _one_ nested cross validation fold, this member points to the item that should display the _average_ predictive quality of all external folds */
				ValidationItem* nested_val_item_;
				
				int validation_statistic_;
				
				PredictionPlotter* plotter_;
				
				/** processes the validation results and updates the result-display */
				void processResults();
				
			
			protected slots:
				virtual void showPredictionDialog();
				
				/** calls DataItem::change and in case of nested validation also calls change() for each connected PartitioningItem */
				void changeSlot();
				
				void showPlotter();
				
			private:
				void init();
				
				
				friend class DataItemScene;
				friend class PredictionPlotter;
		
		};
	}
}

#endif

