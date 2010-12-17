/* componentPlotter.h
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

#ifndef COMP_PLOTTER
#define COMP_PLOTTER

#include  <plotter.h>
#include  <modelItem.h>
#include <QtGui/QComboBox>
#include <BALL/QSAR/latentVariableModel.h>


namespace BALL
{
	namespace VIEW
	{
		class ComponentPlotter : public Plotter
		{
			Q_OBJECT
			
			public:
				ComponentPlotter(ModelItem* model_item, bool plot_loadings);
		
			
			private:
				void plot(bool zoom);
				
				const Matrix<double>* component_matrix_;
				
				ModelItem* model_item_;
				
				void calculateComponents();
				
				double min_response_value_;
				
				double max_response_value_;
				
				/** should the loadings be plotted or the latent variables? */
				bool plot_loadings_;
				
				QComboBox* component_one_combobox_;
				QComboBox* component_two_combobox_;
				
			private slots:
				void selectedCompChanged();
				
			protected slots:
				/** overloads Plotter::activityChange(), in order to make sure to recalculate min and max of the response variable in case of a latent-variable-plotter */
				void activityChange();
				
		};
	}
}

#endif //COMP_PLOTTER



