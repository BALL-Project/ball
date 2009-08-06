 
#include <BALL/APPLICATIONS/QuEasyViz/componentPlotter.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_zoomer.h>
#include <qwt_scale_widget.h>
#include <qwt_color_map.h>

#include <qwt_legend.h>
#include <qwt_legend_item.h>

#include <QColor>

using namespace BALL::VIEW;



ComponentPlotter::ComponentPlotter(ModelItem* model_item, bool plot_loadings)
	: Plotter(model_item)
{
	zoomer_ = NULL;
	model_item_ = model_item;
	plot_loadings_ = plot_loadings;
	component_one_combobox_ = new QComboBox(this);
	component_two_combobox_ = new QComboBox(this);
	component_matrix_ = NULL;
	qwt_plot_->enableAxis(QwtPlot::yLeft);
	if(!plot_loadings_) qwt_plot_->enableAxis(QwtPlot::yRight,1);
	else qwt_plot_->enableAxis(QwtPlot::yRight,0);
	
	data_symbol.setSize(6,6);
	print_data_symbol.setSize(5,5);
	
	plot(1);
	zoomer_ = new QwtPlotZoomer(qwt_plot_->canvas(),this);

	buttonsLayout_->addWidget(component_one_combobox_);
	buttonsLayout_->addWidget(component_two_combobox_);
	connect(component_one_combobox_,SIGNAL(currentIndexChanged(int)),this,SLOT(selectedCompChanged()));
	connect(component_two_combobox_,SIGNAL(currentIndexChanged(int)),this,SLOT(selectedCompChanged()));
	
	if(plot_loadings_) setWindowTitle("Loadings Plotter");
	else setWindowTitle("Latent Variable Plotter");
	
	calculateComponents();
	if(component_matrix_==NULL || component_matrix_->Ncols()==0)
	{
		throw BALL::Exception::GeneralException(__FILE__,__LINE__,"ComponentPlotter error","No input data available, thus plot cannot be created!");
	}
	
	// loading-plot is identical for all response variables ...
	if(!plot_loadings_)
	{
		int no_y = model_item_->inputDataItem()->data()->getNoResponseVariables();
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
}


void ComponentPlotter::selectedCompChanged()
{
	delete zoomer_;
	zoomer_ = NULL;
	plot(1);
	zoomer_ = new QwtPlotZoomer(qwt_plot_->canvas(),this); // if not creating a new zoomer, zooming will not work correctly
}


void ComponentPlotter::calculateComponents()
{
	RegressionModel* model = (RegressionModel*)model_item_->model();
	
	if(!model_item_->getRegistryEntry()->latent_variables)
	{
		cout<<"Plotting of components can only be done for models that create principle components or partial least squares components!"<<endl;
		return;
	}
	if(model_item_->inputDataItem()->data()==NULL)
	{
		cout<<"No input data! Thus creation of PCR/PLS components for plotting is not possible!"<<endl;
		return;
	}
	if(!plot_loadings_ && model_item_->inputDataItem()->data()->getNoResponseVariables()==0)
	{
		cout<<"There should be response variable(s) for plotting of PLS/PCR components"<<endl;
		return;
	}
	
	uint no_components=0;
	
	LatentVariableModel* lv_model = dynamic_cast<LatentVariableModel*>(model_item_->model());
	
	if(!plot_loadings_)
	{
		component_matrix_ = lv_model->getLatentVariables();
		if(component_matrix_->Ncols()==0||component_matrix_->Nrows()==0)
		{
			model->setDataSource(model_item_->inputDataItem()->data());
			model->readTrainingData();
			model->train(); 
			component_matrix_ = lv_model->getLatentVariables();
		}
		
		no_components=component_matrix_->Ncols();
		if(no_components<2) return;
		
		uint size = model_item_->model()->data->getNoSubstances();
		min_response_value_ = 1e10;
		max_response_value_ = -1e10;
		const QSARData* data = model_item_->inputDataItem()->data();
		for(uint j=0; j<size; j++)
		{
			vector<double>* resp = data->getActivity(j);
			double response_value = (*resp)[selected_activity_];
			delete resp;
			if(response_value<min_response_value_) min_response_value_=response_value;
			if(response_value>max_response_value_) max_response_value_=response_value;
		}
	}
	else
	{
		component_matrix_ = lv_model->getLoadings();
		if(component_matrix_->Ncols()==0||component_matrix_->Nrows()==0)
		{
			model->setDataSource(model_item_->inputDataItem()->data());
			model->readTrainingData();
			model->train();
			component_matrix_ = lv_model->getLoadings();
		}
	}
	
	no_components=component_matrix_->Ncols();
	if(no_components<2) return;

	if(component_one_combobox_->count()==0)
	{
		for(uint i=1;i<=no_components;i++)
		{
			String tmp;
			if(!plot_loadings_) tmp="component ";
			else tmp="loading ";
			tmp+=String(i);
			component_one_combobox_->addItem(tmp.c_str(),i);
			component_two_combobox_->addItem(tmp.c_str(),i);
		}
		component_one_combobox_->setCurrentIndex(0);
		component_two_combobox_->setCurrentIndex(1);	
	}
}


// SLOT
void ComponentPlotter::activityChange()
{
	component_matrix_=NULL;  // make sure to recalculate min and max of the response variable
	Plotter::activityChange();	
}


void ComponentPlotter::plot(bool zoom)
{
	qwt_plot_->clear();
	
	if(component_matrix_==NULL||component_matrix_->Ncols()==0) calculateComponents();
	if(component_matrix_==NULL||component_matrix_->Ncols()<2) return; // only 1 component	
	
	double min_y=1e10;
	double max_y=-1e10;
	double min_x=1e10;
	double max_x=-1e10;
	const vector<string>* feature_names;
	if(!plot_loadings_) feature_names = model_item_->model()->getSubstanceNames();
	else feature_names = model_item_->model()->getDescriptorNames();
	const uint size = feature_names->size();
	
	uint comp_one=component_one_combobox_->itemData(component_one_combobox_->currentIndex()).toInt();
	uint comp_two=component_two_combobox_->itemData(component_two_combobox_->currentIndex()).toInt();

	QwtLinearColorMap color_map;
	color_map.setColorInterval(QColor(255,0,0),QColor(0,255,0));
	QwtDoubleInterval interval(min_response_value_,max_response_value_);
	
	const QSARData* data = model_item_->inputDataItem()->data();
	
	for(uint j=1; j<=size; j++)
	{
		QwtPlotMarker* marker= new QwtPlotMarker;
		QwtSymbol symbol = data_symbol;
		
		if(!plot_loadings_)
		{
			vector<double>* resp = data->getActivity(j-1);
			double response_value = (*resp)[selected_activity_];
			delete resp;
			QBrush b(QColor(color_map.rgb(interval,response_value)),Qt::SolidPattern);
			symbol.setBrush(b);
		}
	
		double x_j = (*component_matrix_)(j,comp_one);
		double y_j = (*component_matrix_)(j,comp_two);
		if(y_j<min_y) min_y=y_j;
		if(y_j>max_y) max_y=y_j;
		if(x_j<min_x) min_x=x_j;
		if(x_j>max_x) max_x=x_j;
		marker->setSymbol(symbol);
		marker->setValue(x_j,y_j);
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

	QString s1 = component_one_combobox_->itemText(component_one_combobox_->currentIndex());
	QString s2 = component_two_combobox_->itemText(component_two_combobox_->currentIndex());
	LatentVariableModel* lv_model = dynamic_cast<LatentVariableModel*>(model_item_->model());
	const Matrix<double>* weights = lv_model->getWeights();
	
	if(!plot_loadings_)
	{
		s1 += "  c="; s1+=String((*weights)(comp_one,1)).c_str();
		s2 += "  c="; s2+=String((*weights)(comp_two,1)).c_str();
	}
	
	qwt_plot_->setAxisTitle(QwtPlot::yLeft,s2);
	qwt_plot_->setAxisTitle(QwtPlot::xBottom,s1);
	
	double x_border=(max_x-min_x)*0.05;
	double y_border=(max_y-min_y)*0.05;
	min_x-=x_border; min_y-=y_border;
	max_x+=x_border; max_y+=y_border;
	
	QwtPlotCurve* zero_line = new QwtPlotCurve;
	QwtPlotCurve* zero_line2 = new QwtPlotCurve;
	double x[2]; x[0]=min_x; x[1]=max_x;
	double y[2]; y[0]=0; y[1]=0;
	zero_line->setData(x,y,2);
	x[0]=0; x[1]=0;
	y[0]=min_y; y[1]=max_y;
	zero_line2->setData(x,y,2);
	QColor c(135,135,135); // grey
	QPen pen(c);
	zero_line->setPen(pen);
	zero_line->attach(qwt_plot_);
	zero_line2->setPen(pen);
	zero_line2->attach(qwt_plot_);
	
	if(zoom)
	{
		qwt_plot_->setAxisScale(QwtPlot::yLeft,min_y,max_y);
		qwt_plot_->setAxisScale(QwtPlot::xBottom,min_x,max_x);
	}
	
	if(!plot_loadings_)
	{	
		QwtScaleWidget* rightAxis = qwt_plot_->axisWidget(QwtPlot::yRight);
		rightAxis->setTitle("response value");
		rightAxis->setColorBarEnabled(true);
		rightAxis->setColorMap(interval,color_map);
		qwt_plot_->setAxisScale(QwtPlot::yRight,min_response_value_,max_response_value_);
	}
}
