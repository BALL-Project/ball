#include <BALL/APPLICATIONS/QSAR_GUI/inputPlotter.h>

#include <qwt_plot_marker.h>


using namespace BALL::VIEW;


InputPlotter::InputPlotter(InputDataItem* item)
	: Plotter(item)
{
	input_item_ = item;
	data_ = item->data();	
	plot();
}


void InputPlotter::plot()
{
	qwt_plot_->clear();
	plotActivity();
}


void InputPlotter::plotActivity()
{
	const vector<string>* names = data_->getSubstanceNames();
	uint no = data_->getNoResponseVariables();
	
	if(no==0)
	{
		cout<<"Activities must be read before they can be plotted!!"<<endl;
		return;
	}
	if(names->size()==0) show_data_labels=0;
	
	double min_y=1e10;
	double max_y=-1e10;
	for(uint i=0; i<data_->getNoSubstances();i++)
	{
		QwtPlotMarker* marker= new QwtPlotMarker;
		marker->setSymbol(data_symbol);
		vector<double>* y0 = data_->getActivity(i);
		double y = (*y0)[0];
		delete y0;
		if(y<min_y) min_y=y;
		if(y>max_y) max_y=y;
		marker->setValue(i,y);
		marker->attach(qwt_plot_); // attached object will be automatically deleted by QwtPlot
		
		if(show_data_labels)
		{
			QString s =(*names)[i].c_str();
			QwtText label(s);
			label.setFont(data_label_font);
			marker->setLabel(label);
			marker->setLabelAlignment(data_label_alignment);
		}		
	}
	
	QString s1 = "compounds";
	QString s2 = "activity";
	qwt_plot_->setAxisTitle(0,s2);
	qwt_plot_->setAxisTitle(2,s1);
	
	double x_border=data_->getNoSubstances()*0.05;
	double y_border=(max_y-min_y)*0.05;
	double min_x=0-x_border; min_y-=y_border;
	double max_x=data_->getNoSubstances()+x_border; max_y+=y_border;
	
	qwt_plot_->setAxisScale(0,min_y,max_y);
	qwt_plot_->setAxisScale(2,min_x,max_x);
	qwt_plot_->replot();	
}
