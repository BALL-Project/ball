/* predictionPlotter.C
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

#include <predictionPlotter.h>
#include <validationItem.h>
#include <predictionItem.h>

#include <qwt_plot_curve.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_zoomer.h>

#include <QColor>

using namespace BALL::QSAR;

namespace BALL
{
	namespace VIEW
	{

		PredictionPlotter::PredictionPlotter(PredictionItem* item)
			: Plotter(item)
		{
			pred_item_ = item;
			val_item_ = NULL;
			data_ = pred_item_->inputDataItem()->data();
			
			plot(1);
			zoomer_ = new QwtPlotZoomer(qwt_plot_->canvas(),this);
			setWindowTitle("Prediction Plotter");
			
			uint no_y = data_->getNoResponseVariables();
			if(no_y>1)
			{
				for(uint i=0; i<no_y;i++)
				{
					String s = "Activity "+String(i);
					activity_combobox_->addItem(s.c_str(),i);
				}
				activity_combobox_->show();
			}
		}


		PredictionPlotter::PredictionPlotter(ValidationItem* item)
			: Plotter(item)
		{
			pred_item_ = NULL;
			val_item_ = item;
			data_ = NULL;
			
			plot(1);
			zoomer_ = new QwtPlotZoomer(qwt_plot_->canvas(),this);
			setWindowTitle("Nested Validation Prediction-Plotter");
			
			uint no_y = val_item_->modelItem()->inputDataItem()->data()->getNoResponseVariables();
			if(no_y>1)
			{
				for(uint i=0; i<no_y;i++)
				{
					String s = "Activity "+String(i);
					activity_combobox_->addItem(s.c_str(),i);
				}
				activity_combobox_->show();
			}
		}


		void PredictionPlotter::plot(bool zoom)
		{
			qwt_plot_->clear();
			if(val_item_ || data_->getNoResponseVariables()!=0)
			{
				if((val_item_ && !val_item_->modelItem()->getRegistryEntry()->regression) || (pred_item_ && !pred_item_->modelItem()->getRegistryEntry()->regression))
				{
					plotConfusion(zoom);
				}
				else plotObservedVsExpected(zoom);
			}
			else
			{
				plotObserved(zoom);
			}	
		}


		void PredictionPlotter::plotObservedVsExpected(bool zoom)
		{
			/// find all PredictionItems whose predictions are to be plotted
			list<PredictionItem*> pred_items;
			if(pred_item_!=NULL) pred_items.push_back(pred_item_);
			else
			{
				for(list<ValidationItem*>::iterator it=val_item_->external_validations_.begin(); it!=val_item_->external_validations_.end();it++)
				{
					PredictionItem* p = dynamic_cast<PredictionItem*>(*it);
					if(p) pred_items.push_back(p);
				}
			}
			
			
			/// now do the plotting for each PredictionItem
			
			double min_y=1e10;
			double max_y=-1e10;
			double min_x=1e10;
			double max_x=-1e10;
			
			int p=0;
			for(list<PredictionItem*>::iterator p_it=pred_items.begin(); p_it!=pred_items.end(); p_it++,p++)
			{
				const QSARData* p_data;
				if(pred_item_) p_data = data_;
				else p_data = (*p_it)->getTestData();
				const list<Vector<double> >* results = (*p_it)->results();
				if(results==0)
				{
					cout<<"Results must be read before plotting can be done!"<<endl;
					return;
				}
				if(p_data->getNoResponseVariables()==0)
				{
					cout<<"There are no response values in the input data to be plotted as 'expected' within an 'observed-vs-expected' plot !"<<endl;
					return;
				}
				QwtSymbol symbol = data_symbol;
				if(val_item_)
				{
					QBrush brush=data_symbol.brush();
					brush.setColor(generateColor(pred_items.size(),p));
					symbol.setBrush(brush);
				}
			
				int i = 0;
				const vector<string>* comp_names = p_data->getSubstanceNames();
				
				for (list<Vector<double> >::const_iterator it = results->begin(); it != results->end(); it++,i++)
				{
					QwtPlotMarker* marker= new QwtPlotMarker;
					marker->setSymbol(symbol);
					double observed = (*it)(selected_activity_+1);
					vector<double>* e = p_data->getActivity(i);
					double expected = (*e)[selected_activity_];
					delete e;
					if(observed<min_y) min_y=observed;
					if(observed>max_y) max_y=observed;
					if(expected<min_x) min_x=expected;
					if(expected>max_x) max_x=expected;
					marker->setValue(expected,observed);
					marker->attach(qwt_plot_); // attached object will be automatically deleted by QwtPlot
					
					if(show_data_labels)
					{
						QString s =(*comp_names)[i].c_str();
						QwtText label(s);
						label.setFont(data_label_font);
						marker->setLabel(label);
						marker->setLabelAlignment(data_label_alignment);
					}
					//names_.push_back(s);
					//marker->setTitle(s);
				}
			}
			QString s1 = "expected";
			QString s2 = "observed";
			qwt_plot_->setAxisTitle(0,s2);
			qwt_plot_->setAxisTitle(2,s1);
			
			double x_border=(max_x-min_x)*0.05;
			double y_border=(max_y-min_y)*0.05;
			min_x-=x_border; min_y-=y_border;
			max_x+=x_border; max_y+=y_border;
			
			QwtPlotCurve* diagonal = new QwtPlotCurve;
			double x[2]; x[0]=min_x; x[1]=max_x;
			double y[2]; y[0]=min_x; y[1]=max_x;
			diagonal->setData(x,y,2);
			QColor c(135,135,135);
			QPen pen(c);
			diagonal->setPen(pen);
			diagonal->attach(qwt_plot_); // attached object will be automatically deleted by QwtPlot
			
			if(zoom)
			{
				qwt_plot_->setAxisScale(0,min_y,max_y);
				qwt_plot_->setAxisScale(2,min_x,max_x);
			}
		}



		void PredictionPlotter::plotObserved(bool zoom)
		{
			if(pred_item_==NULL) return;
			
			const list<Vector<double> >* results = pred_item_->results();
			
			if(results==0)
			{
				cout<<"Results must be read before plotting can be done!"<<endl;
				return;
			}
			
			double min_y=1e10;
			double max_y=-1e10;
			double min_x=1e10;
			double max_x=-1e10;
			
			int i = 0;
			const vector<string>* comp_names = data_->getSubstanceNames();
			for (list<Vector<double> >::const_iterator it = results->begin(); it != results->end(); it++,i++)
			{
				QwtPlotMarker* marker= new QwtPlotMarker;
				marker->setSymbol(data_symbol);
				double value = (*it)(1);
				if(value<min_y) min_y=value;
				if(value>max_y) max_y=value;
				marker->setValue(i,(*it)(1));
				marker->attach(qwt_plot_);
				
				if(show_data_labels)
				{
					QString s =(*comp_names)[i].c_str();
					QwtText label(s);
					label.setFont(data_label_font);
					marker->setLabel(label);
					marker->setLabelAlignment(data_label_alignment);
				}
			}
				
			min_x = 0;
			max_x = results->size();
			QString s1 = "compounds";
			QString s2 = "predicted activity";
			qwt_plot_->setAxisTitle(0,s2);
			qwt_plot_->setAxisTitle(2,s1);
			
			double x_border=(max_x-min_x)*0.05;
			double y_border=(max_y-min_y)*0.05;
			min_x-=x_border; min_y-=y_border;
			max_x+=x_border; max_y+=y_border;
			
			if(zoom)
			{
				qwt_plot_->setAxisScale(0,min_y,max_y);
				qwt_plot_->setAxisScale(2,min_x,max_x);
			}
		}


		void PredictionPlotter::plotConfusion(bool zoom)
		{
			/// find all PredictionItems whose predictions are to be plotted
			list<PredictionItem*> pred_items;
			if(pred_item_!=NULL) pred_items.push_back(pred_item_);
			else
			{
				for(list<ValidationItem*>::iterator it=val_item_->external_validations_.begin(); it!=val_item_->external_validations_.end();it++)
				{
					PredictionItem* p = dynamic_cast<PredictionItem*>(*it);
					if(p) pred_items.push_back(p);
				}
			}
			
			/// now do the plotting for each PredictionItem
			
			vector<int> labels;
			if(pred_item_) labels = ((ClassificationModel*)pred_item_->modelItem()->model())->getClassLabels();
			else if(val_item_) labels = ((ClassificationModel*)val_item_->modelItem()->model())->getClassLabels();
			else
			{
				cout<<"no models found whose confusion matrix could be plotted!"<<endl;
				return;
			}
			
			double min_exp=labels[0];
			double max_exp=labels[labels.size()-1];
			map<int,uint> labels_map; // maps the class-IDs to the index in the above vector
			for(uint i=0; i<labels.size(); i++)
			{
				labels_map[labels[i]] = i;
			}
			vector<int> TP(labels.size(),0);
			vector<int> FP(labels.size(),0);
			vector<int> TN(labels.size(),0);
			vector<int> FN(labels.size(),0);
			
			int p=0;
			for(list<PredictionItem*>::iterator p_it=pred_items.begin(); p_it!=pred_items.end(); p_it++,p++)
			{
				const QSARData* p_data;
				if(pred_item_) p_data = data_;
				else p_data = (*p_it)->getTestData();
				const list<Vector<double> >* results = (*p_it)->results();
				if(results==0)
				{
					cout<<"Results must be read before plotting can be done!"<<endl;
					return;
				}
				if(p_data->getNoResponseVariables()==0)
				{
					cout<<"There are no response values in the input data to be plotted as 'expected' within an 'observed-vs-expected' plot !"<<endl;
					return;
				}
				
				int compound = 0;
				for(list<Vector<double> >::const_iterator it = results->begin(); it != results->end(); it++,compound++)
				{
					int observed = (int)((*it)(selected_activity_+1));
					vector<double>* e = p_data->getActivity(compound);
					int expected = (int)((*e)[selected_activity_]);
					delete e;
					if(observed==expected)
					{
						uint id = labels_map[observed];
						for(uint j=0; j<labels.size();j++)
						{
							if(j==id) TP[j]++;
							else TN[j]++;
						}
					}
					else
					{
						uint obs_id = labels_map[observed];
						uint exp_id = labels_map[expected];
						for(uint j=0; j<labels.size();j++)
						{
							if(j==obs_id) FP[j]++;
							else if(j==exp_id) FN[j]++;
							else TN[j]++;
						}
					}
				}
			}
			
		// 	for(uint a=0; a<4; a++) // for TP,..,FN
		// 	{
		// 		if(a==0) cout<<"TP:  ";
		// 		else if(a==1) cout<<"FP:  ";
		// 		else if(a==2) cout<<"TN:  ";
		// 		else if(a==3) cout<<"FN:  ";
		// 		for(uint i=0; i<TP.size(); i++) // for each class
		// 		{
		// 			if(a==0) cout<<TP[i]<<"  ";
		// 			else if(a==1) cout<<FP[i]<<"  ";
		// 			else if(a==2) cout<<TN[i]<<"  ";
		// 			else if(a==3) cout<<FN[i]<<"  ";
		// 		}
		// 		cout<<endl<<flush;
		// 	}	
		// 	cout<<"sums: "<<TP[0]+FP[0]+TN[0]+FN[0]<<"  "<<TP[1]+FP[1]+TN[1]+FN[1]<<"  "<<TP[2]+FP[2]+TN[2]+FN[2]<<"  "<<TP[3]+FP[3]+TN[3]+FN[3]<<endl<<flush;
			
			int min_y = 999999;
			int max_y = -999999;
			vector<QwtPlotCurve*> curves(4);
			vector<double*> x(4);
			vector<double*> y(4);
			
			for(uint a=0; a<4; a++) // for TP,..,FN
			{
				curves[a] = new QwtPlotCurve;
				x[a] = new double[labels.size()];
				y[a] = new double[labels.size()];
				QColor c(135,135,135);
				if(a==0)
				{ 
					c = QColor(70,170,50); // green
				}
				else if(a==1)
				{
					c = QColor(10,30,195); // blue
				}
				else if(a==2)
				{
					c = QColor(194,195,7); // yellow
				}
				else if(a==3)
				{
					c = QColor(100,50,80); // ?!
				}
				QPen pen(c);
				for(uint i=0; i<labels.size(); i++) // for each class
				{
					x[a][i] = labels[i];
					if(a==0) y[a][i] = TP[i]; 
					else if(a==1) y[a][i] = FP[i]; 
					else if(a==2) y[a][i] = TN[i];
					else if(a==3) y[a][i] = FN[i];
					if(y[a][i]<min_y) min_y=(int)y[a][i];
					if(y[a][i]>max_y) max_y=(int)y[a][i];
					
					QwtPlotMarker* marker = new QwtPlotMarker;
					marker->setValue(x[a][i],y[a][i]);
					QwtSymbol symbol = data_symbol;
					symbol.setPen(pen);
					QBrush brush(c);
					symbol.setBrush(brush);
					//symbol.setSize(0.2,y[a][i]);
					marker->setSymbol(symbol);
					marker->attach(qwt_plot_);
					
					if(show_data_labels)
					{
						String s;
						if(a==0) s="TP";
						else if(a==1) s="FP";
						else if(a==2) s="TN";
						else if(a==3) s="FN";
						//s+=", class "+String(labels[i]);
						QwtText label(s.c_str());
						label.setFont(data_label_font);
						marker->setLabel(label);
						marker->setLabelAlignment(data_label_alignment);
					}
				}
				curves[a]->setData(x[a],y[a],labels.size());
				delete x[a];
				delete y[a];
				if(a==0) pen.setWidth(3);
				curves[a]->setPen(pen);
				curves[a]->attach(qwt_plot_); // attached object will be automatically deleted by QwtPlot
			}
			
			QString s1 = "class ID";
			QString s2 = "number of TP,FP,TN,FN";
			qwt_plot_->setAxisTitle(0,s2);
			qwt_plot_->setAxisTitle(2,s1);
			
			if(zoom)
			{
				double x_border=(max_exp-min_exp)*0.05;
				double y_border=(max_y-min_y)*0.05;
				min_exp-=x_border; min_y-=y_border;
				max_exp+=x_border; max_y+=y_border;
				qwt_plot_->setAxisScale(QwtPlot::xBottom,min_exp,max_exp);
				qwt_plot_->setAxisScale(QwtPlot::yLeft,min_y,max_y);
			}
		}
	}
}