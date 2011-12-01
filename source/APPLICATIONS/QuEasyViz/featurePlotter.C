/* featurePlotter.C
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

#include <BALL/VIEW/KERNEL/iconLoader.h>

#include <featurePlotter.h>
#include <mainWindow.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_zoomer.h>

#include <QColor>

#include <set>

using namespace BALL::QSAR;

namespace BALL
{
	namespace VIEW
	{

		FeaturePlotter::FeaturePlotter(ModelItem* model_item)
			: Plotter(model_item)
		{
			model_item_ = model_item;
			feature_combobox_ = new QComboBox(this);
			buttonsLayout_->addWidget(feature_combobox_);
			connect(feature_combobox_,SIGNAL(currentIndexChanged(int)),this,SLOT(selectedFeatureChanged()));
			QIcon icon(IconLoader::instance().getIcon("actions/delete_item"));
			delete_feature_button_ = new QPushButton(icon,"",this);
			buttonsLayout_->addWidget(delete_feature_button_);
			connect(delete_feature_button_,SIGNAL(pressed()),this,SLOT(deleteCurrentFeature()));
			
			deletion_confirmed_ = 0;
			plot(1);
			zoomer_ = new QwtPlotZoomer(qwt_plot_->canvas(),this);
		}


		// SLOT
		void FeaturePlotter::selectedFeatureChanged()
		{
			delete zoomer_;
			zoomer_ = NULL;
			plot(1);
			zoomer_ = new QwtPlotZoomer(qwt_plot_->canvas(),this); // if not creating a new zoomer, zooming will not work correctly
			
			if(feature_combobox_->currentIndex()==0 || feature_combobox_->count()<=2)
			{
				delete_feature_button_->setEnabled(0);
			}
			else delete_feature_button_->setEnabled(1);
		}


		// SLOT
		void FeaturePlotter::deleteCurrentFeature()
		{
			bool ok=0;
			if(!deletion_confirmed_)
			{
				QMessageBox box;
				box.setText("Are you sure you want to delete this feature from the model?!\nIf you choose to do so, the model and all depending items need to be retrained!");
				box.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
				box.button(QMessageBox::Ok)->setText("Delete");
				box.setWindowTitle("Delete feature?");
				box.setDefaultButton(QMessageBox::Cancel);
				ok = (box.exec()==QMessageBox::Ok);
				if(ok) deletion_confirmed_ = 1;
			}
			else ok=1;
			
			if(ok)
			{
				unsigned int index = feature_combobox_->currentIndex();
			
				map<String,unsigned int>::iterator it=map_names_to_ID_.find(feature_combobox_->currentText().toStdString());
				if(it==map_names_to_ID_.end())
				{
					cout<<"error: ID of feature to be deleted not found!"<<endl;
					return;
				}		
				
				unsigned int feature_ID = it->second;
				
				std::multiset<unsigned int>* features = model_item_->model()->getDescriptorIDs();
				if(features->size()==0)
				{
					for(unsigned int i=0; i<feature_combobox_->count()-1; i++)
					{
						if(i!=feature_ID) features->insert(i);
					}
				}
				else
				{
					features->erase(feature_ID);
				}
				
				cout<<"deleting a feature: ID="<<feature_ID<<", name="<<feature_combobox_->currentText().toStdString()<<endl;
				
				feature_combobox_->removeItem(index);
				
				model_item_->change();
			}
		}


		void FeaturePlotter::update()
		{
			feature_combobox_->clear();
			plot(1);	
		}

		void FeaturePlotter::plot(bool zoom)
		{
			qwt_plot_->clear();
			
			Model* model = model_item_->model();
			const Eigen::MatrixXd* descriptor_matrix = model->getDescriptorMatrix();
			const Eigen::MatrixXd* Y = model->getY();
			
			// try to read data
			if(descriptor_matrix->Ncols()==0 || Y->Ncols()==0)
			{
				model->setDataSource(model_item_->inputDataItem()->data());
				model->readTrainingData();
			}
			
			if(descriptor_matrix->Ncols()==0)
			{
				cout<<"Data must be read into model before features can be plotted!"<<endl;
				return;
			}
			if(Y->Ncols()==0)
			{
				cout<<"Response variables must be read into model before features can be plotted!"<<endl;
				return;
			}
			
			const vector<string>* feature_names;
			if(!model_item_->getRegistryEntry()->kernel)
			{
				feature_names = model->getDescriptorNames();
			}
			else
			{
				feature_names = model->getSubstanceNames();
			}
			const vector<string>* compound_names = model->getSubstanceNames();
			
			double min_y=1e10;
			double max_y=-1e10;
			double min_x=1e10;
			double max_x=-1e10;
			const unsigned int no_compounds = descriptor_matrix->Nrows();
			const unsigned int no_features = descriptor_matrix->Ncols();
			
			if(feature_combobox_->count()==0) // if combobox has not yet been set up
			{
				feature_combobox_->addItem("All features",0);
				map_names_to_ID_.clear();
				
				std::multiset<unsigned int>* features = model_item_->model()->getDescriptorIDs();

				if(features->size()>0) // features have already been selected
				{
					std::multiset<unsigned int>::iterator f_it = features->begin();

					for(unsigned int i=0;f_it != features->end();i++, ++f_it)
					{
						feature_combobox_->addItem((*feature_names)[i].c_str(),i+1);
						map_names_to_ID_.insert(make_pair((*feature_names)[i],*f_it));
					}
				}
				else
				{
					for(unsigned int i=0;i<no_features;i++)
					{
						feature_combobox_->addItem((*feature_names)[i].c_str(),i+1);
						map_names_to_ID_.insert(make_pair((*feature_names)[i],i));
					}
				}
					
				feature_combobox_->setCurrentIndex(0);
			}
			
			unsigned int first_index=1;
			unsigned int last_index=no_features;
			bool one_feature=0;
			if(feature_combobox_->currentIndex()>0)
			{
				first_index = feature_combobox_->currentIndex();
				last_index = first_index;
				one_feature=1;
			}
			
			/// create plot-curve(s)
			for(unsigned int i=first_index; i<=last_index; i++)
			{
				unsigned int feature_index = feature_combobox_->itemData(i).toInt();
				if(feature_index==0) continue;
				
				// sort ascendingly according to activity value
				std::multiset<pair<double,pair<double,unsigned int> > > values;
				for(unsigned int j=1; j<=no_compounds; j++)
				{
					double feature_value, response_value;
					model->getUnnormalizedFeatureValue(j,feature_index,feature_value);
					model->getUnnormalizedResponseValue(j,1,response_value);
					values.insert(make_pair(feature_value,make_pair(response_value,j)));
				}
				
				std::multiset<pair<double, pair<double, unsigned int> > >::iterator v_it = values.begin();
				QwtPlotCurve* curve_i = new QwtPlotCurve;
				double* x = new double[no_compounds];
				double* y = new double[no_compounds];
				
				for(unsigned int j=1; j<=no_compounds; j++, ++v_it)
				{
					const pair<double,pair<double,unsigned int> >& p = *v_it;
					double x_ji = p.first;
					double y_j = p.second.first;
					x[j-1] = x_ji;
					y[j-1] = y_j;	// TODO: enable more than 1 resp. variable
					
					if(one_feature)
					{
						QwtPlotMarker* marker= new QwtPlotMarker;
						marker->setSymbol(data_symbol);
						marker->setValue(x_ji,y_j);
						if(show_data_labels)
						{
							QwtText label((*compound_names)[p.second.second-1].c_str());
							label.setFont(data_label_font);
							marker->setLabel(label);
							marker->setLabelAlignment(data_label_alignment);
						}
						marker->attach(qwt_plot_); // attached object will be automatically deleted by QwtPlot
					}
					
					if(x_ji<min_x) min_x=x_ji;
					if(x_ji>max_x) max_x=x_ji;
					if(y_j<min_y) min_y=y_j;
					if(y_j>max_y) max_y=y_j;
				}
			
				curve_i->setData(x,y,no_compounds);
				delete x;
				delete y;
				QColor c(135,135,135);
				if(i==1)
				{ 
					c = QColor(190,10,10); // red
				}
				else if(i==2)
				{
					c = QColor(10,30,195); // blue
				}
				else if(i==3)
				{
					c = QColor(194,195,7); // yellow
				}
				else
				{
					c = QColor(rand()%255,rand()%255,rand()%255); // random color
				}
				QPen pen(c);
				curve_i->setPen(pen);
				curve_i->attach(qwt_plot_); // attached object will be automatically deleted by QwtPlot
			}		
			
			
			/// use feature-name or feature explanation (if not too long) as label for X-axis
			QwtText s1("feature values");
			s1.setFont(qwt_plot_->axisTitle(0).font());
			
			if(feature_combobox_->currentIndex()>0)
			{
				unsigned int feature_index = feature_combobox_->itemData(feature_combobox_->currentIndex()).toInt();
				const vector<string>* names = model_item_->model()->getDescriptorNames();
			
				const String* expl = model_item_->view()->data_scene->main_window->getDescriptorExplanation((*names)[feature_index-1]);
				if(expl!=NULL)
				{
					QFont font = s1.font();
					s1 = QwtText(expl->c_str());
				
					unsigned int max_width=width()-100;
					unsigned int size=font.pointSize();
					unsigned int i=0;
					for(; s1.textSize(font).width()>max_width && i<6; i++) 
					{
						size--;
						font.setPointSize(size);
					}
					if(i==6) // feature explanation is too long, use feature name instead
					{
						s1=QwtText((*names)[feature_index-1].c_str());
						font = qwt_plot_->axisTitle(0).font();
					}
					s1.setFont(font);
				}
				else 
				{
					s1=QwtText((*names)[feature_index-1].c_str());
					s1.setFont(qwt_plot_->axisTitle(0).font());
				}
			}
			
			QString s2 = "response values";
			qwt_plot_->setAxisTitle(0,s2);
			qwt_plot_->setAxisTitle(2,s1);
			
			double x_border=(max_x-min_x)*0.05;
			double y_border=(max_y-min_y)*0.05;
			min_x-=x_border; min_y-=y_border;
			max_x+=x_border; max_y+=y_border;
			
			QwtPlotCurve* zero_line = new QwtPlotCurve;
			double x[2]; x[0]=min_x; x[1]=max_x;
			double y[2]; y[0]=0; y[1]=0;
			zero_line->setData(x,y,2);
			QColor c(135,135,135); // grey
			QPen pen(c);
			zero_line->setPen(pen);
			zero_line->attach(qwt_plot_);

			if(zoom)
			{
				qwt_plot_->setAxisScale(0,min_y,max_y);
				qwt_plot_->setAxisScale(2,min_x,max_x);
			}
		}
	}
}
