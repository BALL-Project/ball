#include <BALL/APPLICATIONS/QSAR_GUI/predictionPlotter.h>
#include <BALL/APPLICATIONS/QSAR_GUI/validationItem.h>
#include <BALL/APPLICATIONS/QSAR_GUI/predictionItem.h>

#include <qwt_plot_curve.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_zoomer.h>

#include <QColor>

using namespace BALL::VIEW;



PredictionPlotter::PredictionPlotter(PredictionItem* item)
	: Plotter(item)
{
	pred_item_ = item;
	val_item_ = NULL;
	data_ = pred_item_->inputDataItem()->data();
	
	plot(1);
	zoomer_ = new QwtPlotZoomer(qwt_plot_->canvas(),this);
}


PredictionPlotter::PredictionPlotter(ValidationItem* item)
	: Plotter(item)
{
	pred_item_ = NULL;
	val_item_ = item;
	data_ = NULL;
	
	plot(1);
	zoomer_ = new QwtPlotZoomer(qwt_plot_->canvas(),this);
}


void PredictionPlotter::plot(bool zoom)
{
	qwt_plot_->clear();
	if(val_item_ || data_->getNoResponseVariables()!=0)
	{
		plotObservedVsExpected(zoom);
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
		QSARData* p_data;
		if(pred_item_) p_data = data_;
		else p_data = (*p_it)->test_data_;
		const QList<Vector<double> >* results = (*p_it)->results();
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
		
		for (QList<Vector<double> >::ConstIterator it = results->begin(); it != results->end(); it++,i++)
		{
			QwtPlotMarker* marker= new QwtPlotMarker;
			marker->setSymbol(symbol);
			double observed = (*it)(1);
			vector<double>* e = p_data->getActivity(i);
			double expected = (*e)[0];
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
	
	const QList<Vector<double> >* results = pred_item_->results();
	
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
	for (QList<Vector<double> >::ConstIterator it = results->begin(); it != results->end(); it++,i++)
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
