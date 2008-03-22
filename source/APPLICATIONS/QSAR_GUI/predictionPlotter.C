#include <BALL/APPLICATIONS/QSAR_GUI/predictionPlotter.h>

#include <qwt_plot_curve.h>
#include <qwt_plot_marker.h>
#include <qwt_symbol.h>
#include <qwt_plot_zoomer.h>


using namespace BALL::VIEW;



PredictionPlotter::PredictionPlotter(PredictionItem* item)
	: Plotter(item)
{
	pred_item_ = item;
	data_ = pred_item_->inputDataItem()->data();
	
	if(data_->getNoResponseVariables()!=0)
	{
		plotObservedVsExpected();
	}
	else
	{
		plotObserved();
	}
}


void PredictionPlotter::plotObservedVsExpected()
{
	const QList<RowVector>* results = pred_item_->results();
	
	if(results==0)
	{
		cout<<"Results must be read before plotting can be done!"<<endl;
		return;
	}
	if(data_->getNoResponseVariables()==0)
	{
		cout<<"There are no response values in the input data to be plotted as 'expected' within an 'observed-vs-expected' plot !"<<endl;
		return;
	}
	
	QwtSymbol sym;
	sym.setStyle(QwtSymbol::Ellipse);
	sym.setSize(5,5);
	
	double min_y=1e10;
	double max_y=-1e10;
	double min_x=1e10;
	double max_x=-1e10;
;	
	int i = 0;
	const vector<string>* comp_names = data_->getSubstanceNames();
	for (QList<RowVector>::ConstIterator it = results->begin(); it != results->end(); it++,i++)
	{
		QwtPlotMarker* marker= new QwtPlotMarker;
		marker->setSymbol(sym);
		double observed = (*it)(1);
		double expected = (*data_->getActivity(i))[0];
		if(observed<min_y) min_y=observed;
		if(observed>max_y) max_y=observed;
		if(expected<min_x) min_x=expected;
		if(expected>max_x) max_x=expected;
		marker->setValue(expected,observed);
		marker->attach(this);
		QString s =(*comp_names)[i].c_str();
		//names_.push_back(s);
		marker->setTitle(s);
	}
	QString s1 = "expected";
	QString s2 = "observed";
	setAxisTitle(0,s2);
	setAxisTitle(2,s1);
	
	replot();
	setAxisScale(0,min_y-1,max_y+1);
	setAxisScale(2,min_x-1,max_x+1);
	
	QwtPlot::resize(600,400);
	QwtPlot::show();
}



void PredictionPlotter::plotObserved()
{
	const QList<RowVector>* results = pred_item_->results();
	
	if(results==0)
	{
		cout<<"Results must be read before plotting can be done!"<<endl;
		return;
	}
	
	QwtSymbol sym;
	sym.setStyle(QwtSymbol::Ellipse);
	sym.setSize(5,5);
	
	double min_y=1e10;
	double max_y=-1e10;
	double min_x=1e10;
	double max_x=-1e10;
	
	int i = 0;
	for (QList<RowVector>::ConstIterator it = results->begin(); it != results->end(); it++,i++)
	{
		QwtPlotMarker* marker= new QwtPlotMarker;
		marker->setSymbol(sym);
		double value = (*it)(1);
		if(value<min_y) min_y=value;
		if(value>max_y) max_y=value;
		marker->setValue(i,(*it)(1));
		marker->attach(this);
	}
		
	min_x = 0;
	max_x = results->size();
	QString s1 = "compounds";
	QString s2 = "predicted activity";
	setAxisTitle(0,s2);
	setAxisTitle(2,s1);
	
	replot();
	setAxisScale(0,min_y-1,max_y+1);
	setAxisScale(2,min_x-1,max_x+1);
	
	QwtPlot::resize(600,400);
	QwtPlot::show();
}
