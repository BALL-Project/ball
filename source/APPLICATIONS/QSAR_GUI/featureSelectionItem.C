#include <BALL/QSAR/exception.h>

#include <BALL/APPLICATIONS/QSAR_GUI/exception.h>
#include <BALL/APPLICATIONS/QSAR_GUI/featureSelectionItem.h>
#include <BALL/APPLICATIONS/QSAR_GUI/mainWindow.h>

#include <QtGui/QDrag>
#include <QtCore/QMimeData>

using namespace BALL::QSAR;
using namespace BALL::QSAR::Exception;
using namespace BALL::VIEW;
using namespace BALL::VIEW::Exception;

FeatureSelectionItem::FeatureSelectionItem(int type, DataItemView* miv):
	DataItem(miv)
{
	setPixmap(QPixmap("./images/feature_selection.png").scaled(QSize(width(), height()), Qt::KeepAspectRatio,Qt::FastTransformation ));

	feature_selection_ = NULL;
	validation_statistic_ = 0;
	done_= 0;
	k_ = 0;
	cor_threshold_ = 0;
	quality_increase_cutoff_ = -1;
	opt_ = 0;
	post_optimization_model_par_ = 0;
	post_optimization_kernel_par_ = 0;

	switch(type)
	{
		case 0:	
			name_ = "Remove Colinear Features";
			type_ = type;
			break;
		case 1:	
			name_ = "Forward Selection";
			type_ = type;
			break;
		case 2:
			name_ = "Backward Selection";
			type_ = type;
			break;
		case 3:
			name_ = "Stepwise Selection";
			type_ = type;
			break;

		default: throw InvalidFeatureSelectionItem(__FILE__,__LINE__);
	}
}

FeatureSelectionItem::~FeatureSelectionItem()
{	
	if (view_ && view_->name == "view")
	{
		//if the item was connected to others, delete it from its respective pipeline
		if (!removeDisconnectedItem())
		{
			removeFromPipeline();
		}
	}
}

FeatureSelectionItem::FeatureSelectionItem(FeatureSelectionItem& fs_item):
DataItem(fs_item.view_)
{
	type_ = fs_item.type_;
	k_ = fs_item.k_;
	opt_ = fs_item.opt_;
	model_item_ = NULL;
	name_ = fs_item.name_;
	setPixmap(fs_item.pixmap());
	validation_statistic_ = fs_item.validation_statistic_;
	done_ = 0;
	result_ = "";
	quality_increase_cutoff_ = fs_item.quality_increase_cutoff_;
	cor_threshold_ = fs_item.cor_threshold_;
	feature_selection_ = NULL;
	post_optimization_model_par_ = fs_item.post_optimization_model_par_;
	post_optimization_kernel_par_ = fs_item.post_optimization_kernel_par_;
}


FeatureSelectionItem::FeatureSelectionItem(String& configfile_section, std::map<String, DataItem*>& filenames_map, list<pair<double,double> >* item_positions, DataItemView* view)
	: DataItem(view)
{
	istringstream input;
	input.str(configfile_section);
		
	String line;
	getline(input,line);
	line.trimLeft();
	if(!line.hasPrefix("[FeatureSelector]"))
	{
		throw BALL::Exception::GeneralException(__FILE__,__LINE__,"FeatureSelection reading error","The given section is no feature selection section!");
	}
	
	String mod="";
	String data="";
	type_=-1;
	String output="";
	k_=-1;
	String accuracy="average accuracy";
	validation_statistic_=0;
	bool remove_correlated=0; // == type_=0
	cor_threshold_=-10;
	quality_increase_cutoff_ = -1;
	quality_increase_cutoff_=-1;
	opt_ = 0;
	feature_selection_ = NULL;
	post_optimization_model_par_ = 0;
	post_optimization_kernel_par_ = 0;
	
	while(input)
	{
		getline(input,line);
		line.trimLeft();
		if(line=="" || line.hasPrefix("#") || line.hasPrefix("//") || line.hasPrefix("%"))
		{
			continue;
		}
		
		if(line.hasPrefix("model_file"))
		{
			mod = ((String)line.after("=")).trimLeft();
		}
		else if(line.hasPrefix("data_file"))
		{
			data = ((String)line.after("=")).trimLeft();
		}
		else if(line.hasPrefix("feature_selection_type"))
		{
			type_ = ((String)line.after("=")).trimLeft().toInt();
			if(type_==0) name_="Remove Colinear Features";
			else if(type_==1) name_="Forward Selection";
			else if(type_==2) name_="Backward Selection";
			else if(type_==3) name_="Stepwise Selection";
		}
		else if(line.hasPrefix("k_fold"))
		{
			k_ = ((String)line.after("=")).trimLeft().toInt();
		}
		else if(line.hasPrefix("quality_increase_cutoff"))
		{
			quality_increase_cutoff_=((String)line.after("=")).trimLeft().toDouble();				
		}
		else if(line.hasPrefix("output"))
		{
			output = ((String)line.after("=")).trimLeft();
		}
		else if(line.hasPrefix("remove_correlated_features"))
		{
			remove_correlated = ((String)line.after("=")).trimLeft().toBool();
			type_=0;name_="Remove Colinear Features";
		}
		else if(line.hasPrefix("cor_threshold"))
		{
			cor_threshold_ = ((String)line.after("=")).trimLeft().toDouble();
		}
		else if(line.hasPrefix("opt_model_par_after_fs"))
		{
			post_optimization_model_par_ = ((String)line.after("=")).trimLeft().toBool();
		}
		else if(line.hasPrefix("opt_kernel_par_after_fs"))
		{
			post_optimization_kernel_par_ = ((String)line.after("=")).trimLeft().toBool();
		}
		else if(line.hasPrefix("grid_search_steps"))
		{
			// ignore this line; it is used for the command-line programs only
		}
		else if(line.hasPrefix("grid_search_recursions"))
		{
			// ignore this line; it is used for the command-line programs only
		}
		else if(line.hasPrefix("grid_search_stepwidth"))
		{
			// ignore this line; it is used for the command-line programs only
		}
		else if(line.hasPrefix("opt_k_fold"))
		{
			// ignore this line; it is used for the command-line programs only
		}
// 		else if(line.hasPrefix("optimize_parameters"))
// 		{
// 			opt_ = ((String)line.after("=")).trimLeft().toBool();
// 		}
		else if(line.hasPrefix("classification_statistic")) // currently unused by regressions!
		{
			String s = ((String)line.after("=")).trimLeft();
			accuracy = s;
			if(s=="average accuracy") validation_statistic_=0;
			else if(s=="weighted average accuracy") validation_statistic_=1;
			else if(s=="overall accuracy") validation_statistic_=2;
			else if(s=="average MCC") validation_statistic_=3;
			else if(s=="overall MCC") validation_statistic_=4;
			else if(s!="")
			{
				String mess ="qualitiy statistic \'"+s+"\' unknown!\n";
				mess+="  possible choices are: \"average accuracy\", \"weighted average accuracy\", \"overall accuracy\", \"average MCC\" and \"overall MCC\"";
				String name = "FeatureSelection reading error";
				throw BALL::Exception::GeneralException(__FILE__,__LINE__,name,mess);
			}				
		}
		else if(line.hasPrefix("done"))
		{
			// ignore this line; it is used for the command-line programms only
		}
		else
		{
			String mess = "Configuration command \""+line+"\" unknown!!";
			String name = "FeatureSelectionItem reading error";
			throw BALL::Exception::GeneralException(__FILE__,__LINE__,name,mess);
		}
	}
	
	map<String,DataItem*>::iterator it = filenames_map.find(mod);
	if(it==filenames_map.end())
	{
		throw BALL::Exception::GeneralException(__FILE__,__LINE__,"Feature selection reading error","ModelItem to which the feature selection should be applied can not be found!");
	}
	setInputModelItem((ModelItem*) it->second);	
	model_item_ = new ModelItem(*input_model_item_);
	model_item_->setInputDataItem(input_model_item_->inputDataItem());
	model_item_->setSaveAttribute(false);
	model_item_->setSavedAs(output.c_str());
	view_->data_scene->addItem(model_item_);
	model_item_->addToPipeline();
	
	feature_selection_ = new FeatureSelection(*model_item_->model());
	if(quality_increase_cutoff_!=-1)
	{
		feature_selection_->setQualityIncreaseCutoff(quality_increase_cutoff_);
	}
	addToPipeline();
	view_->data_scene->addItem(this);
	if(item_positions!=0 && item_positions->size()>0)
	{
		pair<double,double> pos = item_positions->front();
		item_positions->pop_front();
		setPos(pos.first,pos.second);
		if(item_positions->size()>0)
		{
			pair<double,double> pos = item_positions->front();
			item_positions->pop_front();
			model_item_->setPos(pos.first,pos.second);
		}
	}		
	
	Edge* edge1 = new Edge(input_model_item_, this);
	Edge* edge2 = new Edge(this, model_item_);
	view_->data_scene->addItem(edge1);
	view_->data_scene->addItem(edge2);
	
	setSavedAs(output.c_str());
	filenames_map.insert(make_pair(output,model_item_));
	setPixmap(QPixmap("./images/feature_selection.png").scaled(QSize(width(), height()), Qt::KeepAspectRatio,Qt::FastTransformation ));
	
	// TODO : save/load this information
	post_optimization_model_par_ = 0;
	post_optimization_kernel_par_ = 0;
	
	done_ = 0;
}

bool FeatureSelectionItem::execute()
{
	if (model_item_ == NULL)
	{
		throw InvalidFeatureSelectionItem(__FILE__,__LINE__);
	}
	
	if(done_) return 0; // do nothing twice...

	model_item_->setInputDataItem(input_model_item_->inputDataItem());
	delete feature_selection_;
	feature_selection_ = new FeatureSelection(*(model_item_->model()));
	if(validation_statistic_>=0)
	{
		feature_selection_->selectStat(validation_statistic_);
	}
	if(quality_increase_cutoff_>=0)
	{
		feature_selection_->setQualityIncreaseCutoff(quality_increase_cutoff_);
	}
	
	switch(type_)
	{
		case 0:
			feature_selection_->removeHighlyCorrelatedFeatures(cor_threshold_);
			break;		
		case 1:	
			feature_selection_->forwardSelection(k_, opt_);
			break;
		case 2:
			feature_selection_->backwardSelection(k_, opt_);
			
			break;
		case 3:					
			feature_selection_->stepwiseSelection(k_, opt_);
			break;
		default:
			throw InvalidFeatureSelectionItem(__FILE__,__LINE__);
			break;
	}

	cout<<"opt's : "<<post_optimization_model_par_<<"  "<<post_optimization_kernel_par_<<endl;
	if(post_optimization_model_par_) model_item_->optimizeModelParameters();
	if(post_optimization_kernel_par_) model_item_->optimizeKernelParameters();

	model_item_->model()->readTrainingData();
	model_item_->model()->train();
	model_item_->setResultString((int)model_item_->model()->getDescriptorIDs()->size());

	model_item_->setDone(1); // set done_ to 1, so that model will be saved to file when exporting the pipeline
	done_ = 1; // ready!
	return 1;
}

void FeatureSelectionItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{	
	/// set member-pointer of main_window to the current object in order to gain access to this object from another widget 
	view_->data_scene->main_window->dragged_item = this;
	view_->data_scene->main_window->drag_source = view_->name;

	QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
	QDrag *drag = new QDrag(event->widget());

	QMimeData *mimeData = new QMimeData;
	mimeData->setData("application/x-fsitemdata", itemData);
	drag->setMimeData(mimeData);
	drag->setPixmap(this->pixmap());
	drag->setHotSpot(QPoint(0, 0));
	drag->start();
	itemChange(ItemPositionChange, pos());
	QGraphicsItem::mousePressEvent(event);
}

int FeatureSelectionItem::numberOfFeatures()
{
	return model_item_->model()->getDescriptorIDs()->size();
}


int FeatureSelectionItem::getType()
{
	return type_;
}

FeatureSelection* FeatureSelectionItem::featureSelection()
{
	return feature_selection_;
}

void FeatureSelectionItem::setK(int k)
{
	k_ = k;
}

int FeatureSelectionItem::k()
{
	return k_;
}

void FeatureSelectionItem::setOpt(bool opt)
{
	opt_ = opt;
}

void FeatureSelectionItem::setModelItem(ModelItem* model_item)
{
	model_item_ = model_item;
}

void FeatureSelectionItem::setInputModelItem(ModelItem* model_item)
{
	input_model_item_ = model_item;	
}

bool FeatureSelectionItem::opt()
{
	return opt_;
}

ModelItem* FeatureSelectionItem::modelItem()
{
	return model_item_;
}

ModelItem* FeatureSelectionItem::inputModelItem()
{
	return input_model_item_;
}


void FeatureSelectionItem::writeConfigSection(ofstream& out)
{
	out << "[FeatureSelector]" << "\n";
	if(done_) out<<"done = "<<done_<<endl;
	out << "model_file = "<< inputModelItem()->savedAs().toStdString() << "\n";
	out << "data_file = "<< inputModelItem()->inputDataItem()->savedAs().toStdString() << "\n";
	int s = getValidationStatistic();
	String stat = modelItem()->getRegistryEntry()->getStatName(s);
	if(getType()>0)
	{
		if(!input_model_item_->getRegistryEntry()->regression)
		{
			out<< "classification_statistic = "<<stat.c_str()<<endl;
		}
		out << "k_fold = "<< k() <<  "\n";
		out << "feature_selection_type = "<< getType() <<  "\n";
		if(quality_increase_cutoff_>0) out<<"quality_increase_cutoff = "<<quality_increase_cutoff_<<"\n";
	}
	else
	{
		out<<"remove_correlated_features = 1"<<endl;
		out<<"cor_threshold = "<<getCorThreshold()<<endl;
	}
	if(opt_) out << "optimize_parameters = " << opt() << "\n";
	
	bool b=0;
	if(post_optimization_model_par_) 
	{
		out << "opt_model_par_after_fs = "<< 1 << "\n";
		out << "opt_k_fold = "<<model_item_->k_fold<< "\n";
		b=1;
	}
	if(post_optimization_kernel_par_)
	{
		out << "opt_kernel_par_after_fs = "<< 1 << "\n";
		out << "grid_search_steps = "<<model_item_->grid_search_steps << "\n";
		out << "grid_search_recursions = "<<model_item_->grid_search_recursions << "\n";
		out << "grid_search_stepwidth = " <<model_item_->grid_search_stepwidth << "\n";
		if(!b) out << "opt_k_fold = "<<model_item_->k_fold << "\n";
	}
	out << "output = " << modelItem()->savedAs().toStdString() << "\n";
	out << "\n";
}

void FeatureSelectionItem::addToPipeline()
{
	view_->data_scene->main_window->fs_pipeline_.insert(this);
}

void FeatureSelectionItem::removeFromPipeline()
{
	view_->data_scene->main_window->fs_pipeline_.erase(this);
}

void FeatureSelectionItem::setQualityIncreaseCutoff(double cutoff)
{
	quality_increase_cutoff_ = cutoff;
}
