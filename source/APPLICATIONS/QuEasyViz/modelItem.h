/* modelItem.h
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

#ifndef MODELITEM_H
#define MODELITEM_H

#include <dataItem.h>
#include <inputDataItem.h>
#include <plotter.h>
#include <BALL/QSAR/Model.h>
#include <BALL/QSAR/registry.h>

namespace BALL
{
	namespace VIEW
	{		
		/** @class ModelItem
		 * @brief graphical representation of a QSAR model
		 *
		 * @todo
		*/
		class ModelItem : public DataItem
		{
			Q_OBJECT
			
			public:
				/** @name Constructors and Destructors*/
		
				/** constructor  
				* @param entry pointer to the registry entry of this model
				* @param miv pointer to the item's DataItemView
				*/
				ModelItem(BALL::QSAR::RegistryEntry* entry, DataItemView* miv);

				/** constructor 
				* @param inputdata pointer to the item's InputDataItem
				* @param entry pointer to the RegistryEntry of this model
				* @param miv pointer to the item's DataItemView	
				*/
				ModelItem(InputDataItem* inputdata, BALL::QSAR::RegistryEntry* entry, DataItemView* miv);

	 			/** constructor 
				* @param inputdata pointer to the item's InputDataItem
				* @param entry pointer to the RegistryEntry of this model
				* @param kernelType id of the chosen kernel	
				* @param parameter1 kernel parameter 1	
				* @param parameter2 kernel parameter 2
				* @param miv pointer to the item's DataItemView	
				*/
				ModelItem(InputDataItem* inputdata, BALL::QSAR::RegistryEntry* entry, int kernelType, double parameter1, double parameter2, DataItemView* miv);

				/** constructor 
				* @param inputdata pointer to the item's InputDataItem
				* @param entry pointer to the RegistryEntry of this model
				* @param s1 kernel string 1
				* @param s2 kernel string2
				* @param miv pointer to the item's DataItemView	
				*/
				ModelItem(InputDataItem* inputdata, BALL::QSAR::RegistryEntry* entry, String s1, String s2, DataItemView* miv);

				/** copy constructor 
				* @param item the ModelItem that is to be copied
				*/
				ModelItem(ModelItem& item);
				
				/** Creates a ModelItem from the geiven config-file section, registers it in the view and in the Pipeline-QSets of MainWindow and creates edges */
				ModelItem(String& configfile_section, std::map<String, DataItem*>& filenames_map, list<pair<double,double> >* item_positions, DataItemView* view);
			
				/** creates a model
				* @param inputdata pointer to the item's InputDataItem
				*/ 
				ModelItem* createModel(InputDataItem* inputdata);

				/** creates a model 
				* @param inputdata pointer to the item's InputDataItem
				* @param kernelType id of the chosen kernel	
				* @param parameter1 kernel parameter 1	
				* @param parameter2 kernel parameter 2
				*/
				ModelItem* createModel(InputDataItem* inputdata, int kernelType, double parameter1, double parameter2);

				/** creates a model 
				* @param inputdata pointer to the item's InputDataItem
				* @param kernelType id of the chosen kernel	
				* @param s1 kernel string 1
				* @param s2 kernel string2
				*/
				ModelItem* createModel(InputDataItem* inputdata, String s1, String s2);
		
				/**  destructor */
				~ModelItem();
		
				
				/** @name Accessors */

				/** sets the item's model
			 	* @param model pointer to the model
				*/
				void setModel(BALL::QSAR::Model* model);

				/** sets the item's registry entry*
				* @param entry pointer to the entry
				*/
				void setEntry(BALL::QSAR::RegistryEntry* entry);
				
				/** returns a pointer to the item's Model */
				BALL::QSAR::Model* model();	
				
				/** returns a pointer to the item's InputDataItem */
				InputDataItem* inputDataItem();	
				
				/** returns a pointer to the item's RegistryEntry */
				BALL::QSAR::RegistryEntry* getRegistryEntry();
				
				/** trains the item's model */
				bool execute();
				
				/** optimizes only the model parameters according to the setting of the item and does nothing else (no training); for use by FeatureSelectionItem */
				void optimizeModelParameters();
				
				/** optimizes only the kernel parameters according to the setting of this item and does nothing else (no training); for use by FeatureSelectionItem */
				void optimizeKernelParameters();
				
				void setSaveAttribute(bool save);
				bool saveAttribute();
				void setInputDataItem(InputDataItem* item);
				void addPredictionInputEdge(Edge* edge);
				void deletePredictionInputEdge(Edge* edge);
				void createActions();
				void saveToFile(String file);
				void loadFromFile(String file);

				/** generates the config-file section for the current model and appends it to out */
				void writeConfigSection(ofstream& out);

				void addToPipeline();
				void removeFromPipeline();
				
				void disableTraining();
				void enableTraining();
				
				/** Tests if anything is still to be done for this model. \n
				This function checks whether this ModelItem has already been executed or if it has outgoing edges only to FeatureSelectionItems. In both cases, false is returned. */
				bool isDone();

				/** @name Enums */
				/** 
				* Type of the Item 
				* @see BALL::VIEW::DataItem
				*/
				enum { Type = UserType + 3 };
				int type() const { return Type; };

				/** @name Attributes */
				int kernel_function_type;
				double kernel_parameter1;
				double kernel_parameter2;
				std::vector<double> model_parameters;
				bool optimize_model_parameters;
				bool optimize_kernel_parameters;
				double grid_search_stepwidth;
				int grid_search_steps;
				int grid_search_recursions;
				int k_fold;
				
				void setInput(InputDataItem* new_input);
				
				/** overloads DataItem::change(), so that all plotters that have already been created are deleted */
				void change();

			public slots:
				void saveModel();
				void loadModel();	
				void showProperties();
				void showPlotter();
				void showFeaturePlotter();
				void showLatentVariablePlotter();
				void showLoadingPlotter();
				
			private:							
				
				/** @name Private Attributes*/

				/** pointer to the item's input data item */
				InputDataItem* input_;
				
				ModelItem* descriptor_source_model_;

				/** pointer to the item's model */
				BALL::QSAR::Model* model_;

				/** pointer to the item's RegistryEntry */
				BALL::QSAR::RegistryEntry* entry_;

				/** determines, if the item has to be exported upon saving */
				bool save_attribute_;

				std::set<Edge*> prediction_input_edges_;
				
				Plotter* plotter_;
				
				Plotter* feature_plotter_;
				
				Plotter* latent_variable_plotter_;
				
				Plotter* loading_plotter_;
				
				/** if set to true, training is not done for this model, e.g. because it serves as input for a feature selection only */
				bool no_training_;
				
				void setPixmap();
				
				/** initializes those members for which this can be done identically, no matter from which constructors this item is created*/
				void init();
				
				BALL::String getMouseOverText();
		

				friend class DataItemScene;
		};
	}
}
#endif
