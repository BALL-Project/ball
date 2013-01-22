/* featurePlotter.h
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

#ifndef FEATURE_PLOTTER
#define FEATURE_PLOTTER

#include  <plotter.h>
#include  <modelItem.h>
#include <QtGui/QComboBox>


namespace BALL
{
	namespace VIEW
	{
		class FeaturePlotter : public Plotter
		{
			Q_OBJECT
			
			public:
				FeaturePlotter(ModelItem* model_item);
				
				/** fetch new results if the model was retrained and update display */
				void update();
		
			
			private:
				ModelItem* model_item_;
				
				QComboBox* feature_combobox_;
				
				QPushButton* delete_feature_button_;
				
				void plot(bool zoom);
				
				/** user has confirmed deleting a feature; do not show the dialog again if any more features are to be deleted */ 
				bool deletion_confirmed_;
				
				std::map<String,unsigned int> map_names_to_ID_;

			
				
			private slots:
				void selectedFeatureChanged();
				
				void deleteCurrentFeature();
		
			
			
		};
	}
}

#endif //FEATURE_PLOTTER


