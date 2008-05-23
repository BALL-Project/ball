#include <BALL/APPLICATIONS/QSAR_GUI/coefficientPlotter.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_zoomer.h>

#include <QColor>

using namespace BALL::VIEW;



CoefficientPlotter::CoefficientPlotter(ModelItem* model_item)
	: Plotter(model_item)
{
	model_item_ = model_item;
	plot();
}



void CoefficientPlotter::plot()
{
	qwt_plot_->clear();
	
	RegressionModel* model = (RegressionModel*)model_item_->model();
	const Matrix* coefficient_matrix = model->getTrainingResult();
	
	if(coefficient_matrix->Ncols()==0)
	{
		cout<<"Model must be trained before coefficients can be plotted!"<<endl;
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
	double min_x=1;
	double max_x=coefficient_matrix->Nrows();
	const uint size = coefficient_matrix->Nrows();
	
	for(int i=1; i<=coefficient_matrix->Ncols(); i++)
	{
		QwtPlotCurve* curve_i = new QwtPlotCurve;
		double* x = new double[size];
		double* y = new double[size];
		
		for(uint j=1; j<=size; j++)
		{
			QwtPlotMarker* marker= new QwtPlotMarker;
			marker->setSymbol(data_symbol);
			double coefficient_j = (*coefficient_matrix)(j,i);
			x[j-1] = j;
			y[j-1] = coefficient_j;	
			
			if(coefficient_j<min_y) min_y=coefficient_j;
			if(coefficient_j>max_y) max_y=coefficient_j;
			
			marker->setValue(j,coefficient_j);
			marker->attach(qwt_plot_); // attached object will be automatically deleted by QwtPlot
			
			if(show_data_labels)
			{
				QString s =(*feature_names)[j-1].c_str();
				QwtText label(s);
				label.setFont(data_label_font);
				marker->setLabel(label);
				marker->setLabelAlignment(data_label_alignment);
			}
		}
	
		curve_i->setData(x,y,size);
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
		
	
	QString s1 = "features";
	QString s2 = "coefficient values";
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
	
	qwt_plot_->setAxisScale(0,min_y,max_y);
	qwt_plot_->setAxisScale(2,min_x,max_x);
	qwt_plot_->replot();
}
