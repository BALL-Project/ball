#include <BALL/APPLICATIONS/QSAR_GUI/validationItem.h>
#include <BALL/APPLICATIONS/QSAR_GUI/exception.h>
#include <BALL/APPLICATIONS/QSAR_GUI/mainWindow.h>
#include <BALL/APPLICATIONS/QSAR_GUI/validationResultDialog.h>

#include <QtGui/QDrag>
#include <QtCore/QMimeData>

using namespace BALL::QSAR;
using namespace BALL::QSAR::Exception;
using namespace BALL::VIEW;
using namespace BALL::VIEW::Exception;


ValidationItem::ValidationItem(int type, DataItemView* view):
	DataItem(view),
	k_(0),
	num_of_samples_(0),
	num_of_runs_(0),
	model_item_(NULL),
	q2_(0.0),
	r2_(0.0)
{
	setPixmap(QPixmap("./images/validation.png").scaled(QSize(width(), height()), Qt::KeepAspectRatio,Qt::FastTransformation ));
	done_ = 0;
	type_ = type;
	initName();
	result_color_ = QColor(205,225,205);
}

ValidationItem::ValidationItem(ValidationItem& item):
DataItem(item.view_)
{
	name_ = item.name_;
	setPixmap(item.pixmap());
	type_ = item.type_;
	k_ = item.k_;
	num_of_samples_ = item.num_of_samples_;
	num_of_runs_ = item.num_of_runs_;
	model_item_ = item.model_item_;
	q2_ = item.q2_;
	r2_ = item.r2_;
	done_ = item.done_;
	result_color_ = QColor(205,225,205);
}


ValidationItem::ValidationItem(String& configfile_section, std::map<String, DataItem*>& filenames_map, list<pair<double,double> >* item_positions, DataItemView* view)
	: DataItem(view)
{
	result_color_ = QColor(205,225,205);
	
	istringstream input;
	input.str(configfile_section);
		
	String line;
	getline(input,line);
	line.trimLeft();
	if(!line.hasPrefix("[Validator]"))
	{
		throw BALL::Exception::GeneralException(__FILE__,__LINE__,"ValidationItem reading error","The given section is no validation section!");
	}
	
	String mod="";
	String data="";
	String val_data=""; /// TODO: external validation not supported yet
	String output="";
	k_=0;
	num_of_samples_=0;
	num_of_runs_ = 0;
	validation_statistic_=-1;	
	q2_=0;
	r2_=0;
	
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
		else if(line.hasPrefix("validation_data_file"))
		{
			val_data = ((String)line.after("=")).trimLeft();
		}
		else if(line.hasPrefix("k_fold"))
		{
			k_ = ((String)line.after("=")).trimLeft().toInt();
		}
		else if(line.hasPrefix("bootstrap_samples"))
		{
			num_of_samples_ = ((String)line.after("=")).trimLeft().toInt();
		}
		else if(line.hasPrefix("no_of_permutation_tests"))
		{
			num_of_runs_ = ((String)line.after("=")).trimLeft().toInt();
		}
		else if(line.hasPrefix("output"))
		{
			output = ((String)line.after("=")).trimLeft();
		}
		else if(line.hasPrefix("classification_statistic")) // currently unused by regressions!
		{
			String s = ((String)line.after("=")).trimLeft();
			if(s=="average accuracy") validation_statistic_=0;
			else if(s=="weighted average accuracy") validation_statistic_=1;
			else if(s=="overall accuracy") validation_statistic_=2;
			else if(s=="average MCC") validation_statistic_=3;
			else if(s=="overall MCC") validation_statistic_=4;
			else if(s!="")
			{
				String mess ="qualitiy statistic \'"+s+"\' unknown!\n";
				mess+="  possible choices are: \"average accuracy\", \"weighted average accuracy\", \"overall accuracy\", \"average MCC\" and \"overall MCC\"";
				String name = "ValidationItem reading error";
				throw BALL::Exception::GeneralException(__FILE__,__LINE__,name,mess);
			}					
		}
		else
		{
			String mess = "Configuration command \""+line+"\" unknown!!";
			String name = "ValidationItem reading error";
			throw BALL::Exception::GeneralException(__FILE__,__LINE__,name,mess);
		}	
	}
	map<String,DataItem*>::iterator it = filenames_map.find(mod);
	if(it==filenames_map.end())
	{
		throw BALL::Exception::GeneralException(__FILE__,__LINE__,"ValidationItem reading error","ModelItem for wich the validation should be done can not be found!");
	}
	model_item_ = (ModelItem*) it->second;
	
	view_->data_scene->addItem(this);
	addToPipeline();
	if(item_positions!=0 && item_positions->size()>0)
	{
		pair<double,double> pos = item_positions->front();
		item_positions->pop_front();
		setPos(pos.first,pos.second);
	}
	
	Edge* edge = new Edge(model_item_, this);
	view_->data_scene->addItem(edge);

	setPixmap(QPixmap("./images/validation.png").scaled(QSize(width(), height()), Qt::KeepAspectRatio,Qt::FastTransformation ));
	setSavedAs(output.c_str());
	
	///set type of validation to be done:
	if(k_<=0) type_ = 1;
	else if(num_of_samples_<=0) type_ = 2;
	else type_ = 3;
	if(num_of_runs_>0) type_ = 4;
	
	initName();
	done_ = 0;
}


void ValidationItem::initName()
{
	switch(type_)
	{
		case 1:	
			name_ = "test fit to training data";
			break;
		case 2:
			name_ = "cross validation";
			break;
		case 3:
			name_ = "bootstrapping";
			break;

		case 4:
			name_ = "response permutation test";
			break;

		case 5:
			name_ = "nested cross validation";
			//TODO: change pixmap
			break;

			default: throw InvalidFeatureSelectionItem(__FILE__,__LINE__);
			setName("Val");
	}
}


bool ValidationItem::execute()
{
	if (model_item_ == NULL)
	{cout<<"model ==NULL!!"<<endl;
		throw InvalidValidationItem(__FILE__,__LINE__);
	}

//	ValidationItem* item = new ValidationItem(type_, view_);
	
	if(done_) return 0; // do nothing twice...
	
	if(validation_statistic_>=0)
	{
		model_item_->model()->model_val->selectStat(validation_statistic_);
	}
	switch(type_)
	{
		case 1:	
			model_item_->model()->model_val->testInputData();
			break;
		case 2:
			model_item_->model()->model_val->crossValidation(k_);
			break;
		case 3:	
			model_item_->model()->model_val->bootstrap(num_of_samples_);
			break;
		case 4:	
			result_of_rand_test_ = model_item_->model()->model_val->yRandomizationTest(num_of_runs_, k_);		
			break;
		case 5: 
			break; //TODO: calculate the mean of the Q^2 values of all PredictionItems
		default:
			throw InvalidValidationItem(__FILE__,__LINE__);
	}
	
	if(type_!=5)
	{
		r2_ = model_item_->model()->model_val->getFitRes();
		q2_ = model_item_->model()->model_val->getCVRes();
	}
	
	if(type_==1) setResultString(r2_);
	else setResultString(q2_);
	
	done_ = 1;
	return 1;
}

void ValidationItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{	
	/// set member-pointer of main_window to the current object in order to gain access to this object from another widget 
	view_->data_scene->main_window->dragged_item = this;
	view_->data_scene->main_window->drag_source = view_->name;

	QByteArray itemData;
	QDataStream dataStream(&itemData, QIODevice::WriteOnly);
	QDrag *drag = new QDrag(event->widget());

	QMimeData *mimeData = new QMimeData;
	if(type_!=5)
	{
		mimeData->setData("application/x-validationdata", itemData);
	}
	else
	{
		mimeData->setData("application/x-EXTvalidationdata", itemData);
	}
	drag->setMimeData(mimeData);
	drag->setPixmap(this->pixmap());
	drag->setHotSpot(QPoint(0, 0));
	drag->start();
	itemChange(ItemPositionChange, pos());
	QGraphicsItem::mousePressEvent(event);
}


ValidationItem::~ValidationItem()
{
	if (view_->name == "view")
	{
		//if the item was connected to others, delete it from its respective pipeline
		if (!removeDisconnectedItem())
		{
			removeFromPipeline();
		}
	}
}
	
BALL::String ValidationItem::getStatName()
{
	String name="";
	if(validation_statistic_==-1) return name;
	
	return model_item_->getRegistryEntry()->getStatName(validation_statistic_);
}

void ValidationItem::setModelItem(ModelItem* model_item)
{
	model_item_ = model_item;
}

int ValidationItem::getValidationType()
{
	return type_;
}

void ValidationItem::setValFraction(double frac)
{
	val_fraction_ = fraction;
}

double ValidationItem::getValFraction()
{
	return val_fraction;	
}

void ValidationItem::setK(int k)
{
	k_ = k;
}
void ValidationItem::setNumOfRuns(int num)
{
	num_of_runs_ = num;
}

void ValidationItem::setNumOfSamples(int num)
{
	num_of_samples_ = num;
}

void ValidationItem::setNumOfNCVFolds(int num)
{
	n_of_ncv_folds_ = num;
}

ModelItem* ValidationItem::modelItem()
{
	return model_item_;
}

double ValidationItem::getQ2()
{
	return q2_;
}

double ValidationItem::getR2()
{
	return r2_;
}

int ValidationItem::numOfRuns()
{
	return num_of_runs_;
}

int ValidationItem::numOfSamples()
{
	return num_of_samples_;
}

int ValidationItem::numOfNCVFolds()
{
	return  n_of_ncv_folds_;
}

int ValidationItem::k()
{
	return k_;
}

Matrix* ValidationItem::resultOfRandTest()
{
	return &result_of_rand_test_;
}

void ValidationItem::contextMenuEvent(QGraphicsSceneContextMenuEvent* /*event*/)
{
	if (view_->name == "view")
	{
		ValidationResultDialog validationResultDialog(this);
		validationResultDialog.exec();
	}
}


void ValidationItem::writeConfigSection(ofstream& out)
{
	out << "[Validator]" << "\n";
	out << "model_file = "<< modelItem()->savedAs().toStdString() << "\n";
	out << "data_file = "<< modelItem()->inputDataItem()->savedAs().toStdString() << "\n";
	int s = getValidationStatistic();
	String stat = modelItem()->getRegistryEntry()->getStatName(s);
	if(stat!="") out<< "classification_statistic = "<<stat.c_str()<<endl;
	//out << "validation_data_file = "<< "\n";    /// TODO: external validation not supported yet
	if(type_>0) out << "k_fold = "<< k() <<  "\n";
	if(num_of_samples_>0) out << "bootstrap_samples = "<< num_of_samples_ << "\n";
	if(num_of_runs_>0) out << "no_of_permutation_tests = " <<  num_of_runs_ << "\n";
	out << "output = " << savedAs().toStdString() << "\n\n";	
}


void ValidationItem::addToPipeline()
{
	view_->data_scene->main_window->val_pipeline_.insert(this);
}

void ValidationItem::removeFromPipeline()
{
	view_->data_scene->main_window->val_pipeline_.erase(this);
}

