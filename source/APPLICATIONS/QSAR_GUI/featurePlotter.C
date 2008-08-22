#include <BALL/APPLICATIONS/QSAR_GUI/featurePlotter.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_zoomer.h>

#include <QColor>

using namespace BALL::VIEW;



FeaturePlotter::FeaturePlotter(ModelItem* model_item)
	: Plotter(model_item)
{
	model_item_ = model_item;
	plot(1);
	zoomer_ = new QwtPlotZoomer(qwt_plot_->canvas(),this);
}



void FeaturePlotter::plot(bool zoom)
{
	qwt_plot_->clear();
	
	Model* model = model_item_->model();
	const Matrix* descriptor_matrix = model->getDescriptorMatrix();
	const Matrix* Y = model->getY();
	
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
	
	double min_y=1e10;
	double max_y=-1e10;
	double min_x=1e10;
	double max_x=-1e10;
	const uint no_compounds = descriptor_matrix->Nrows();
	const uint no_features = descriptor_matrix->Ncols();
	
	// sort ascendingly according to activity value
	vector<SortedList<pair<double,double> > > values(no_features);
	for(int i=1; i<=no_features; i++)
	{
		for(uint j=1; j<=no_compounds; j++)
		{
			values[i-1].insert(make_pair((*descriptor_matrix)(j,i),(*Y)(j,1)));
		}
	}	
	
	for(int i=1; i<=no_features; i++)
	{
		values[i-1].front();
		QwtPlotCurve* curve_i = new QwtPlotCurve;
		double* x = new double[no_compounds];
		double* y = new double[no_compounds];
		
		for(uint j=1; j<=no_compounds; j++)
		{
			//QwtPlotMarker* marker= new QwtPlotMarker;
			//marker->setSymbol(data_symbol);
			pair<double,double> p = values[i-1].next();
			double x_ji = p.first;
			double y_j = p.second;
			x[j-1] = x_ji;
			y[j-1] = y_j;	// TODO: enable more than 1 resp. variable
			
			if(x_ji<min_x) min_x=x_ji;
			if(x_ji>max_x) max_x=x_ji;
			if(y_j<min_y) min_y=y_j;
			if(y_j>max_y) max_y=y_j;
			
			//marker->setValue(x_ji,y_j);
			//marker->attach(qwt_plot_); // attached object will be automatically deleted by QwtPlot
			
// 			if(show_data_labels)
// 			{
// 				QString s =(*feature_names)[i-1].c_str();
// 				QwtText label(s);
// 				label.setFont(data_label_font);
// 				marker->setLabel(label);
// 				marker->setLabelAlignment(data_label_alignment);
// 			}
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
		
	
	QString s1 = "features values";
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
