#include <BALL/APPLICATIONS/QSAR_GUI/validationItem.h>
#include <BALL/APPLICATIONS/QSAR_GUI/exception.h>
#include <BALL/APPLICATIONS/QSAR_GUI/mainWindow.h>

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
	
	switch(type)
	{
		case 1:	
			name_ = "test quality of fit";
			type_ = type;
			break;
		case 2:
			name_ = "test predictive power (cross validation)";
			type_ = type;
			break;
		case 3:
			name_ = "test predictive power (bootstrapping)";
			type_ = type;
			break;

		case 4:
			name_ = "response permutation test";
			type_ = type;
			break;

		default: throw InvalidFeatureSelectionItem(__FILE__,__LINE__);
		setName("Val");
	}
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
}

ValidationItem* ValidationItem::connectWithModelItem()
{
	if (model_item_ == NULL)
	{
		throw InvalidValidationItem(__FILE__,__LINE__);
	}

	ValidationItem* item = new ValidationItem(type_, view_);
	
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
		default:
			throw InvalidValidationItem(__FILE__,__LINE__);
	}
	
	r2_ = model_item_->model()->model_val->getFitRes();
	q2_ = model_item_->model()->model_val->getCVRes();

	return item;
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
	mimeData->setData("application/x-validationdata", itemData);
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
		MainWindow* mw = view_->data_scene->main_window;
		mw->removeFromPipeline(this);
	}
}	

void ValidationItem::setModelItem(ModelItem* model_item)
{
	model_item_ = model_item;
}

int ValidationItem::getValidationType()
{
	return type_;
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

int ValidationItem::k()
{
	return k_;
}

Matrix* ValidationItem::resultOfRandTest()
{
	return &result_of_rand_test_;
}
