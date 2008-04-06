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
		MainWindow* mw = view_->data_scene->main_window;
		mw->removeFromPipeline(this);
	}
}

FeatureSelectionItem::FeatureSelectionItem(FeatureSelectionItem& fs_item):
DataItem(fs_item.view_)
{
	type_ = fs_item.type_;
	k_ = fs_item.k_;
	opt_ = fs_item.opt_;
	model_item_ = fs_item.model_item_;
	name_ = fs_item.name_;
	setPixmap(fs_item.pixmap());
	validation_statistic_ = 0;
	done_ = fs_item.done_;

	if (fs_item.feature_selection_ == NULL)
	{
		feature_selection_ = NULL;
	}
	else
	{
		feature_selection_ = new FeatureSelection(*(model_item_->model()));
	}
}


void FeatureSelectionItem::connectWithModelItem()
{
	if (model_item_ == NULL)
	{
		throw InvalidFeatureSelectionItem(__FILE__,__LINE__);
	}
	
	if(done_) return; // do nothing twice...

	feature_selection_ = new FeatureSelection(*(model_item_->model()));
	if(validation_statistic_>=0)
	{
		feature_selection_->selectStat(validation_statistic_);
	}
	switch(type_)
	{
		case 0:
			feature_selection_->removeHighlyCorrelatedFeatures(cor_threshold_);
			model_item_->model()->readTrainingData();
			model_item_->model()->train();
			break;		
		case 1:	
			feature_selection_->forwardSelection(k_, opt_);
			model_item_->model()->readTrainingData();
			model_item_->model()->train();
			break;
		case 2:
			feature_selection_->backwardSelection(k_, opt_);
			model_item_->model()->readTrainingData();
			model_item_->model()->train();
			break;
		case 3:					
			feature_selection_->stepwiseSelection(k_, opt_);
			model_item_->model()->readTrainingData();
			model_item_->model()->train();
			break;
		default:
			throw InvalidFeatureSelectionItem(__FILE__,__LINE__);
	}
	
	done_ = 1; // ready!
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
	out << "model_file = "<< inputModelItem()->savedAs().toStdString() << "\n";
	out << "data_file = "<< inputModelItem()->inputDataItem()->savedAs().toStdString() << "\n";
	int s = getValidationStatistic();
	String stat = modelItem()->getRegistryEntry()->getStatName(s);
	if(getType()>0)
	{
		out<< "classification_statistic = "<<stat.c_str()<<endl;
		out << "k_fold = "<< k() <<  "\n";
		out << "feature_selection_type = "<< getType() <<  "\n";
		out << "output = " << modelItem()->savedAs().toStdString() << "\n";
	}
	else
	{
		out<<"remove_correlated_features = 1"<<endl;
		out<<"cor_threshold = "<<getCorThreshold()<<endl;
	}
	out << "optimize_parameters = " << opt() << "\n";
	out << "\n";
}
