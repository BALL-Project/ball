/* bayesPlotter.h
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

#ifndef BAYES_PLOTTER
#define BAYES_PLOTTER

#include <qwt_plot_spectrogram.h>

#include  <BALL/APPLICATIONS/QuEasyViz/plotter.h>
#include  <BALL/APPLICATIONS/QuEasyViz/modelItem.h>
#include <qwt_raster_data.h>

namespace BALL
{
	namespace VIEW
	{
		class BayesPlotter;
		
		class SpectrogramData : public QwtRasterData
		{
			public:
				SpectrogramData(BayesModel* snb,  const double& max_x, const double& min_y, const double& max_y, const double& y_resolution, BayesPlotter* plotter, bool copy=0);

				virtual QwtRasterData* copy() const;
				virtual QwtDoubleInterval range() const;

				virtual double value(double x, double y) const;
		
				/** calculates the probabilities for a specified class 
				@param class_index if plotter_::class_to_use_==-1, the class with the highest probability will be used for each cell of the heatmap */
				void precalculateProb();
			private:
				double min_x_;
				double max_x_;
				double min_y_;
				double max_y_;
				double min_z_;
				double max_z_;
				double y_resolution_;
				BayesModel* snb_;
				BayesPlotter* plotter_;
		};
		
		class BayesPlotter : public Plotter
		{
			public:
				BayesPlotter(ModelItem* model_item);
		
			
			private:
				ModelItem* model_item_;
				
				void plot(bool zoom);
				
				QwtPlotSpectrogram* spectrogram_;
				
				/** precalculated probabilities that are to be plotted */
				vector<vector<double> > data_;
				
				int class_to_use_;
		
				friend class SpectrogramData;
		};
	}
}

#endif //BAYES_PLOTTER
