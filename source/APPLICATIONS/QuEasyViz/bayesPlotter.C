/* bayesPlotter.C
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

#include <bayesPlotter.h>
#include <qwt_plot_zoomer.h>
#include <qwt_scale_widget.h>

#include <QColor>

using namespace BALL::QSAR;

namespace BALL
{
	namespace VIEW
	{

		SpectrogramData::SpectrogramData(BayesModel* snb, const double& max_x, const double& min_y, const double& max_y, const double& y_resolution, BayesPlotter* plotter, bool copy):
			QwtRasterData(QRectF(0, min_y, max_x+1, max_y-min_y))
		{
			min_x_=0; min_y_=min_y;
			max_x_=max_x;	
			max_y_=max_y;
			min_z_=999999; max_z_=-999999;
			y_resolution_ = y_resolution;
			snb_ = snb;
			plotter_ = plotter;
			if(!copy) precalculateProb();
		}

		QwtRasterData* SpectrogramData::copy() const
		{
			SpectrogramData* spec = new SpectrogramData(snb_,max_x_,min_y_,max_y_,y_resolution_,plotter_,1);
			spec->min_z_=min_z_;
			spec->max_z_=max_z_;
			return spec;
		}

		QwtDoubleInterval SpectrogramData::range() const
		{
			if(plotter_->class_to_use_>-1) return QwtDoubleInterval(0,1);
			else return QwtDoubleInterval(min_z_,max_z_);
		}

		double SpectrogramData::value(double x, double y) const
		{
			int x_index = (int)(x-min_x_);
			int y_index = (int)((y-min_y_)/y_resolution_);
			if(x_index<0) x_index=0;
			else if(x_index>=(int)plotter_->data_.size()) x_index=plotter_->data_.size()-1;
			if(y_index<0) x_index=0;
			else if(y_index>=(int)plotter_->data_[0].size()) y_index=plotter_->data_[0].size()-1;
			return plotter_->data_[x_index][y_index];
		}
				    
		void SpectrogramData::precalculateProb()
		{
			int x_size = (int) (max_x_-min_x_+1);
			plotter_->data_.resize(x_size);
			int y_size = (int) ((max_y_-min_y_)/y_resolution_);
			
			for(int i=0; i<x_size;i++)
			{
				plotter_->data_[i].resize(y_size);
				double feature_value=min_y_;
  				for(int j=0; j<y_size;j++)
				{
					vector<double> probabilities = snb_->calculateProbabilities(plotter_->selected_activity_,i,feature_value);
					double prob;
					if(plotter_->class_to_use_>-1) prob=probabilities[plotter_->class_to_use_];
					else 
					{
						// find class with highest probability 
						double max_prob=0;
						int max_index=0; 
						for(unsigned int a=0;a<probabilities.size();a++) 
						{	
							if(probabilities[a]>max_prob)
							{
								max_prob=probabilities[a];
								max_index=a;
							}
						}
						if(max_prob==0) prob = -1; // use -1 to to indicate "no class"
						else prob = max_index;
					}			
					
					if(prob<min_z_) min_z_ = prob;
					if(prob>max_z_) max_z_ = prob;
					plotter_->data_[i][j] = prob;
					feature_value+=y_resolution_;
 				}
			}
		}




		BayesPlotter::BayesPlotter(ModelItem* model_item)
			: Plotter(model_item)
		{
			model_item_ = model_item;
			spectrogram_ = NULL;
			class_to_use_ = -1;
					
			if(*model_item_->model()->getType()!="snB" && *model_item_->model()->getType()!="nB")
			{
				throw BALL::Exception::GeneralException(__FILE__,__LINE__,"BayesPlotter error","BayesPlotter can only be created for a Bayes model!");
			}
			
			qwt_plot_->setAxisTitle(QwtPlot::xBottom,"Features");
			qwt_plot_->enableAxis(QwtPlot::yRight);	
			plot(1);
			zoomer_ = new QwtPlotZoomer(qwt_plot_->canvas(),this);
			
			unsigned int no_y = ((BayesModel*)model_item_->model())->getNoResponseVariables();
			if(no_y>1)
			{
				for(unsigned int i=0; i<no_y;i++)
				{
					String s = "Activity "+String(i);
					activity_combobox_->addItem(s.c_str(),i);
				}
				activity_combobox_->show();
			}
		}




		void BayesPlotter::plot(bool zoom)
		{
			delete spectrogram_;
			BayesModel* snb = (BayesModel*) model_item_->model();
			spectrogram_ = new QwtPlotSpectrogram();
			double y0=-3; double y1=3; double res=0.1;
			
			spectrogram_->setData(SpectrogramData(snb,snb->getDescriptorNames()->size()-1,y0,y1,res,this));
			spectrogram_->attach(qwt_plot_);
			
			QwtScaleWidget* rightAxis = qwt_plot_->axisWidget(QwtPlot::yRight);
			if(class_to_use_>-1) 
			{
				String title = "Probability for class "+String(class_to_use_);
				rightAxis->setTitle(title.c_str());
			}
			else
			{
				rightAxis->setTitle("Most likely class");
			}
			rightAxis->setColorBarEnabled(true);
			rightAxis->setColorMap(spectrogram_->data().range(),spectrogram_->colorMap());
			qwt_plot_->setAxisScale(QwtPlot::yRight,spectrogram_->data().range().minValue(), spectrogram_->data().range().maxValue());
			
			if(zoom)
			{
				qwt_plot_->setAxisScale(QwtPlot::xBottom,0,snb->getDescriptorNames()->size());
				qwt_plot_->setAxisScale(QwtPlot::yLeft,y0,y1);
			}
		}
	}
}