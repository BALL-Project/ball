#include <BALL/APPLICATIONS/QuEasyViz/predictionItem.h>
#include <BALL/APPLICATIONS/QuEasyViz/exception.h>
#include <BALL/APPLICATIONS/QuEasyViz/mainWindow.h>
#include <BALL/APPLICATIONS/QuEasyViz/predictionPlotter.h>
#include <BALL/APPLICATIONS/QuEasyViz/predictionResultDialog.h>
#include <BALL/QSAR/configIO.h>

#include <QtGui/QDrag>
#include <QtCore/QMimeData>

using namespace BALL::QSAR;
using namespace BALL::QSAR::Exception;
using namespace BALL::VIEW;
using namespace BALL::VIEW::Exception;

PredictionItem::PredictionItem(InputDataItem* input_item, ModelItem* model_item, DataItemView* view):
	ValidationItem(7,view),
	test_data_item_(input_item),
	dotted_edge_(NULL)
{
	model_item_ = model_item;
	String dir = view_->data_scene->main_window->getImageDirectory();
	setPixmap(QPixmap((dir+"prediction.png").c_str()).scaled(QSize(width(), height()), Qt::KeepAspectRatio,Qt::FastTransformation ));
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
	test_data_item_ = item.test_data_item_;
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
	test_data_item_ = (InputDataItem*) it->second;

	if(item_positions!=0 && item_positions->size()>0)
	{
		pair<double,double> pos = item_positions->front();
		item_positions->pop_front();
		setPos(pos.first,pos.second);
	}
	
	dotted_edge_ = new DottedEdge(test_data_item_,this);
	view_->data_scene->addItem(dotted_edge_);
	Edge* edge = new Edge(test_data_item_, model_item_);
	view_->data_scene->addItem(edge);
	Edge* edge2 = new Edge(model_item_,this);
	view_->data_scene->addItem(edge2);
	
	String dir = view_->data_scene->main_window->getImageDirectory();
	setPixmap(QPixmap((dir+"prediction.png").c_str()).scaled(QSize(width(), height()), Qt::KeepAspectRatio,Qt::FastTransformation ));
	name_ = "Prediction for " + test_data_item_->name();
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
	model_item_->model()->setDataSource(test_data_item_->data());
}


bool PredictionItem::execute()
{
	if(done_) return 0;   // do nothing twice !
	
	// predict activities
	for(unsigned int i=0; i<test_data_item_->data()->getNoSubstances();i++)
	{
		vector<double>* substance = test_data_item_->data()->getSubstance(i);
		Vector<double> res = model_item_->model()->predict(*substance,1);
		results_.push_back(res);
		delete substance;
	}
	
	if(test_data_item_->data()->getNoResponseVariables()>0)
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

const list<BALL::Vector<double> >* PredictionItem::results()
{
	return &results_;
}

InputDataItem* PredictionItem::inputDataItem()
{
	return test_data_item_;
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
	if(results_.size()==0)
	{
		QMessageBox::information(view_,"No predictions","No predictions have been done yet that could be displayed!\nTherefore, click \"Execute Pipeline\" first.");
		return;	
	}
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

const QSARData* PredictionItem::getTestData()
{
	return test_data_item_->data();
}

void PredictionItem::saveToFile(String filename)
{
	ValidationItem::saveToFile(filename);
	
	ofstream out(filename.c_str(),ios::app);
	out<<endl<<"[Predictions]"<<endl;
	
	QSARData* test_data = test_data_item_->data();
	bool print_expected = (test_data->getNoResponseVariables()>0);
	int no_act = results_.front().getSize();
	int no_cols = no_act;
	if(print_expected)
	{
		no_cols*=2;
		out<<"# format: predition0, expectation0, ..."<<endl;
	}
			
	out<<"expected_values = "<<print_expected<<endl;
	out<<"dimensions = "<<results_.size()<<" "<<no_cols<<endl;
	list<Vector<double> >::const_iterator it=results_.begin();
	for(uint i=0; it!=results_.end(); i++, it++)
	{	
		vector<double>* e = 0;
 		if(print_expected) e=test_data->getActivity(i);
		for(int act=0; act<no_act; act++)
		{
			out<<((*it)(act+1))<<"\t";
			if(print_expected) out<<(*e)[act]<<"\t";
		}
		delete e;
		out<<endl;
	}	
}

void PredictionItem::loadFromFile(String filename)
{
	try
	{
		model_item_->model()->model_val->readFromFile(filename);
	}
	catch(BALL::Exception::GeneralException e)
	{
		QMessageBox::warning(view_,"Error",e.getMessage());
		return;
	}
	r2_ = model_item_->model()->model_val->getFitRes();
	q2_ = model_item_->model()->model_val->getCVRes();
	if(q2_!=-1) setResultString(q2_);
	
	ifstream in(filename.c_str());
	bool pred_section=0;
	bool expected_values=0;
	uint no_rows = 0;
	uint no_cols = 0;
	while(in)
	{
		String line;
		getline(in,line);
		line.trimLeft();
		if(line=="" || line.hasPrefix("#") || line.hasPrefix("//") || line.hasPrefix("%"))
		{
			continue;
		}
		if(line.hasPrefix("[Predictions]")) pred_section=1;
		else if(pred_section)
		{
			if(line.hasPrefix("expected_values"))
			{
				expected_values = ((String)line.after("=")).trimLeft().toBool();
			}
			if(line.hasPrefix("dimensions"))
			{
				line = ((String)line.after("="));
				no_rows = line.getField(0).toInt();
				no_cols = line.getField(1).toInt();
				break;
			}
		}
	}
	
	int no_act = no_cols;
	if(expected_values) no_act /= 2;
	for(uint i=1;i<=no_rows;i++)
	{
		Vector<double> v(no_act);
		int act=1;
		for(uint j=1; j<=no_cols;j++)
		{
			String s;
			in>>s;
			if(!expected_values || (j%2)==1)
			{
				v(act)=s.toDouble();
				act++;
			}
		}
		results_.push_back(v);
	}
	//getline(in,line); // read the rest of the last matrix-line
	
	if(pred_section) done_=1;	// if predictions were found within file
}

void PredictionItem::change()
{
	results_.clear();
	ValidationItem::change();
}
