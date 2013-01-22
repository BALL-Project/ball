/* featureSelectionItem.h
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

#ifndef FEATURESELECTIONITEM_H
#define FEATURESELECTIONITEM_H

#include <dataItem.h>
#include <modelItem.h>
#include <BALL/QSAR/featureSelection.h>

namespace BALL
{
	namespace VIEW
	{
		/**
 		* graphical representation of a feature selection
 		*/

		/** @class FeatureSelectionItem
		* @brief class for the representation of a feature selection
		*
		* @todo
		*/
		class FeatureSelectionItem : public DataItem
		{
			public:
				/** @name Constructors and Destructors*/
				FeatureSelectionItem(int type, DataItemView* miv);
				~FeatureSelectionItem();
			
				FeatureSelectionItem(FeatureSelectionItem& fs_item); 
				
				/** Creates a FeatureSelectionItem from the geiven config-file section, registers it in the view and in the Pipeline-QSets of MainWindow and creates edges */
				FeatureSelectionItem(String& configfile_section, std::map<String, DataItem*>& filenames_map, std::list<std::pair<double,double> >* item_positions, DataItemView* view);

				/** start features selection */
				bool execute();

				BALL::QSAR::FeatureSelection* featureSelection();
				int getType();
				void setK(int k);
				bool opt();
				void setOpt(bool opt);
				void setModelItem(ModelItem* model_item);
				void setInputModelItem(ModelItem* model_item);
				int numberOfFeatures();
				ModelItem* modelItem();
				ModelItem* inputModelItem();	
				
				void setValidationStatistic(int s) {validation_statistic_=s;};
				int getValidationStatistic() {return validation_statistic_;};	
				double getCorThreshold() {return cor_threshold_;};
				
				int k();

				enum { Type = UserType + 4 };
				int type() const { return Type; }
				
				/** generates the config-file section for the current model and appends it to out */
				void writeConfigSection(std::ofstream& out);
				
				void addToPipeline();
				void removeFromPipeline();
				void setQualityIncreaseCutoff(double cutoff);		
				
				BALL::String getMouseOverText();
				
				bool getPostFSModelParOpt() {return post_optimization_model_par_;}
				bool getPostFSKernelParOpt() {return post_optimization_kernel_par_;}
				
				/** overloads DataItem::change(), so that the data-source for the model_item_ is updated (neccessary if input data was replaced) */
				void change();
				
			private:
			
				BALL::QSAR::FeatureSelection* feature_selection_;
				ModelItem* model_item_;
				ModelItem* input_model_item_;
				int type_;
				int k_;
				bool opt_;
				int validation_statistic_;
				double cor_threshold_;
				
				/** indicates whether the model parameters of the reduced model should be optimized. If true, this will be done in the same way as for the original/unreduced model */
				bool post_optimization_model_par_;
				
				/** indicates whether the kernel parameters of the reduced model should be optimized. If true, this will be done in the same way as for the original/unreduced model */
				bool post_optimization_kernel_par_;
				
				double quality_increase_cutoff_;
				
				
				friend class DataItemScene;
				friend class FeatureSelectionDialog;
		};
	}
}

#endif
