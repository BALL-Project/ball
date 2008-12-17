#include <BALL/APPLICATIONS/QSAR_GUI/predictionItem.h>
#include <BALL/APPLICATIONS/QSAR_GUI/exception.h>
#include <BALL/APPLICATIONS/QSAR_GUI/mainWindow.h>
#include <BALL/APPLICATIONS/QSAR_GUI/predictionPlotter.h>
#include <BALL/APPLICATIONS/QSAR_GUI/predictionResultDialog.h>
#include <BALL/QSAR/configIO.h>

#include <QtGui/QDrag>
#include <QtCore/QMimeData>

using namespace BALL::QSAR;
using namespace BALL::QSAR::Exception;
using namespace BALL::VIEW;
using namespace BALL::VIEW::Exception;

PredictionItem::PredictionItem(InputDataItem* input_item, ModelItem* model_item, DataItemView* view):
	ValidationItem(7,view),
	input_data_item_(input_item),
	dotted_edge_(NULL)
{
	model_item_ = model_item;
	setPixmap(QPixmap("./images/prediction.png").scaled(QSize(width(), height()), Qt::KeepAspectRatio,Qt::FastTransformation ));
	name_ = "Prediction for " + input_item->name();
	plotter_ = NULL;
	dotted_edge_ = NULL;
	createActions();
}

PredictionItem::PredictionItem(PredictionItem& item)
	: ValidationItem(7,item.view_)
{
	name_ = item.name_;
	setPixmap(item.pixmap());
	model_item_ = item.model_item_;
	results_ = item.results_;
	input_data_item_ = item.input_data_item_;
	plotter_ = item.plotter_;
	dotted_edge_ = NULL;
	createActions();
	
}

PredictionItem::~PredictionItem()
{
	delete dotted_edge_;
	
	if (view_->name == "view")
	{
		//if the item was connected to others, delete it from its respective pipeline
		if (!removeDisconnectedItem())
		{
			removeFromPipeline();
		}
	}
	// everything else is done by base-class destructor!!
}


PredictionItem::PredictionItem(String& configfile_section, map<String, DataItem*>& filenames_map, list<pair<double,double> >* item_positions, DataItemView* view)
	: ValidationItem(7,view)
{
	istringstream input;
	input.str(configfile_section);
	PredictionConfiguration conf = ConfigIO::readPredictionConfiguration(&input);
	validation_statistic_ = conf.statistic;
	
	map<String,DataItem*>::iterator it = filenames_map.find(conf.model);
	if(it==filenames_map.end())
	{
		throw BALL::Exception::GeneralException(__FILE__,__LINE__,"PredictionItem reading error","ModelItem with which the prediction should be done can not be found!");
	}
	model_item_ = (ModelItem*) it->second;
	it = filenames_map.find(conf.data);
	if(it==filenames_map.end())
	{
		throw BALL::Exception::GeneralException(__FILE__,__LINE__,"PredictionItem reading error","InputDataItem for which the prediction should be done can not be found!");
	}
	input_data_item_ = (InputDataItem*) it->second;

	if(item_positions!=0 && item_positions->size()>0)
	{
		pair<double,double> pos = item_positions->front();
		item_positions->pop_front();
		setPos(pos.first,pos.second);
	}
	
	dotted_edge_ = new DottedEdge(input_data_item_,this);
	view_->data_scene->addItem(dotted_edge_);
	Edge* edge = new Edge(input_data_item_, model_item_);
	view_->data_scene->addItem(edge);
	Edge* edge2 = new Edge(model_item_,this);
	view_->data_scene->addItem(edge2);
	
	setPixmap(QPixmap("./images/prediction.png").scaled(QSize(width(), height()), Qt::KeepAspectRatio,Qt::FastTransformation ));
	name_ = "Prediction for " + input_data_item_->name();
	view_->data_scene->addItem(this);
	addToPipeline();
	filenames_map.insert(make_pair(conf.output,this));
	setSavedAs(conf.output.c_str());
	plotter_ = 0;
	done_ = 0;
	createActions();
}


void PredictionItem::setValidationInput()
{
	model_item_->model()->setDataSource(test_data_);
}


bool PredictionItem::execute()
{
	if(done_) return 0;   // do nothing twice !
	
	// predict activities
	for(unsigned int i=0; i<input_data_item_->data()->getNoSubstances();i++)
	{
		vector<double>* substance = input_data_item_->data()->getSubstance(i);
		Vector<double> res = model_item_->model()->predict(*substance,1);
		results_ << res;
		delete substance;
	}
	
	// if expected activity values are available, calculate Q^2
	test_data_ = ((InputDataItem*)dotted_edge_->sourceNode())->data();
	
	if(test_data_->getNoResponseVariables()>0)
	{
		const QSARData* train_data_backup =  model_item_->model()->data;
		double r2_backup = r2_;
		
		// ValidationItem::execute() will set r2_; but since we are using _external_ data for the prediction and are assesing the quality of fit to it (and not the training-data), we save the obtained quality statistic in q2_ insteadfset
		
		//model_item_->model()->setDataSource(test_data); // now done by setValidationInput(), called from ValidationItem::execute()
		ValidationItem::execute(); // calculate Q^2
		model_item_->model()->setDataSource(train_data_backup); // reset the model's data
		q2_ = r2_;
		r2_ = r2_backup;
	}
	
	done_ = 1;
	return 1;
}

ModelItem* PredictionItem::modelItem()
{
	return model_item_;
}

const QList<BALL::Vector<double> >* PredictionItem::results()
{
	return &results_;
}

InputDataItem* PredictionItem::inputDataItem()
{
	return input_data_item_;
}


void PredictionItem::setDottedEdge(DottedEdge* edge)
{
	dotted_edge_ = edge;
}

DottedEdge* PredictionItem::dottedEdge()
{
	return dotted_edge_;
}

void PredictionItem::showPredictionPlotter()
{
	if(results_.size()==0)
	{
		QMessageBox::information(view_,"No predictions","No predictions have been done yet that could be plotted!\nTherefore, click \"Execute Pipeline\" first.");
		return;	
	}
	if(plotter_ == NULL)
	{
		plotter_=new PredictionPlotter(this);
	}
	plotter_->show();
}


void PredictionItem::showPredictionDialog()
{
	PredictionResultDialog predictionResultDialog(this);
	predictionResultDialog.exec();
}

void PredictionItem::createActions()
{
	QAction* show_plotter = new QAction(QIcon(""),tr("Plot predictions"), this);
	connect(show_plotter, SIGNAL(triggered()), this, SLOT(showPredictionPlotter()));
	context_menu_actions_.push_back(show_plotter);
}


void PredictionItem::writeConfigSection(ofstream& out)
{
	out << "[Predictor]" << "\n";
	out << "model_file = "<< modelItem()->savedAs().toStdString() << "\n";
	out << "data_file = "<< inputDataItem()->savedAs().toStdString() << "\n";
	out << "print_expected = "<< 1 << "\n";
	int s = getValidationStatistic();
	if(s>=0)
	{
		String stat = modelItem()->getRegistryEntry()->getStatName(s);
		if(!model_item_->getRegistryEntry()->regression) out<< "classification_statistic = "<<stat.c_str()<<endl;
		else out<< "regression_statistic = "<<stat.c_str()<<endl;
	}
	out << "output = " << savedAs().toStdString() << "\n\n";
}

void PredictionItem::addToPipeline()
{
	view_->data_scene->main_window->prediction_pipeline_.insert(this);
	view_->data_scene->main_window->all_items_pipeline_.insert(this);
}

void PredictionItem::removeFromPipeline()
{
	view_->data_scene->main_window->prediction_pipeline_.erase(this);
	view_->data_scene->main_window->all_items_pipeline_.erase(this);
}

