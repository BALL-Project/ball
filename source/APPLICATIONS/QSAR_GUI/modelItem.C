#include <BALL/APPLICATIONS/QSAR_GUI/modelItem.h>
#include <QtGui/QDrag>
#include <QtCore/QMimeData>
#include <BALL/APPLICATIONS/QSAR_GUI/mainWindow.h>
#include <BALL/QSAR/exception.h>
#include <BALL/APPLICATIONS/QSAR_GUI/exception.h>
#include <QtGui/QMessageBox>

using namespace BALL::QSAR;
using namespace BALL::QSAR::Exception;
using namespace BALL::VIEW;
using namespace BALL::VIEW::Exception;


ModelItem::ModelItem(RegistryEntry* entry,  DataItemView* miv):
	DataItem(miv),
	kernel_function_type(0),
	kernel_parameter1(0.),
	kernel_parameter2(0.),
	optimize_model_parameters(false),
	optimize_kernel_parameters(false),
	grid_search_stepwidth(0.0),
	grid_search_steps(0),
	grid_search_recursions(0),
	k_fold(0),
	input_(NULL),
	model_(NULL), 
	entry_(entry),
	save_attribute_(true)
{
	QPixmap pm;	
	if (entry_->kernel)
	{
		pm = QPixmap("./images/kernel_model.png").scaled(QSize(width(), height()), Qt::KeepAspectRatio,Qt::FastTransformation );
	}
	else 
	{
		pm = QPixmap("./images/model.png").scaled(QSize(width(), height()), Qt::KeepAspectRatio,Qt::FastTransformation );
	}
	setPixmap(pm);
	setName(QString(entry_->name_abreviation.c_str()));
}

ModelItem::ModelItem(InputDataItem* inputdata, RegistryEntry* entry, DataItemView* miv):
	DataItem(miv),	
	kernel_function_type(0),
	kernel_parameter1(0),
	kernel_parameter2(0),
	optimize_model_parameters(false),
	optimize_kernel_parameters(false),
	grid_search_stepwidth(0.0),
	grid_search_steps(0),
	grid_search_recursions(0),
	k_fold(0),
	input_(inputdata),
	entry_(entry),
	save_attribute_(true)
{
	if(!entry_->kernel)
	{
		if (input_ != NULL)
		{
			model_ = (*entry_->create)(*input_->data());
		}
		else
		{
			QSARData q;
			model_ = (*entry_->create)(q);
		}
	}
	QPixmap pm = QPixmap("./images/model.png").scaled(QSize(width(), height()), Qt::KeepAspectRatio,Qt::FastTransformation );
	setPixmap(pm);
	setName(QString(entry_->name_abreviation.c_str()));	
}

ModelItem::ModelItem(InputDataItem* inputdata, RegistryEntry* entry, int kernelType, double parameter1, double parameter2, DataItemView* miv): 
	DataItem(miv),
	kernel_function_type(kernelType),
	kernel_parameter1(parameter1),
	kernel_parameter2(parameter2),
	optimize_model_parameters(false),
	optimize_kernel_parameters(false),
	grid_search_stepwidth(0.0),
	grid_search_steps(0),
	grid_search_recursions(0),
	k_fold(0),
	input_(inputdata),
	entry_(entry),
	save_attribute_(true)
{
	if(entry_->kernel && kernelType < 4)
	{
		if (input_ != NULL)
		{
			model_ = entry_->createKernel1(*input_->data(), kernel_function_type, kernel_parameter1, kernel_parameter2);
		}
		else
		{
			QSARData q;
			model_ = entry_->createKernel1(q, kernel_function_type, kernel_parameter1, kernel_parameter2);
		}
	}
	
	QPixmap pm = QPixmap("./images/kernel_model.png").scaled(QSize(width(), height()), Qt::KeepAspectRatio,Qt::FastTransformation );
	setPixmap(pm);
	setName(QString(entry_->name_abreviation.c_str()));	
}

ModelItem::ModelItem(InputDataItem* inputdata, RegistryEntry* entry, String s1, String s2,DataItemView* miv):
	DataItem(miv),
	kernel_function_type(4),
	kernel_parameter1(0.),
	kernel_parameter2(0.),
	optimize_model_parameters(false),
	optimize_kernel_parameters(false),
	grid_search_stepwidth(0.0),
	grid_search_steps(0),
	grid_search_recursions(0),
	k_fold(0),
	input_(inputdata),
	entry_(entry),
	save_attribute_(true)
{
	if(entry_->kernel)
	{
		if (input_ != NULL)
		{
		model_ = entry_->createKernel2(*input_->data(),s1, s2);
		}
		else 
		{
			QSARData q;
			model_ = entry_->createKernel2(q,s1, s2);
		}
	}
	QPixmap pm = QPixmap("./images/kernel_model.png").scaled(QSize(width(), height()), Qt::KeepAspectRatio,Qt::FastTransformation );
	setPixmap(pm);
	setName(QString(entry_->name_abreviation.c_str()));
}

ModelItem::ModelItem(ModelItem& item):
DataItem(item.view_)
{
	view_ = item.view_;
	name_ = item.name_;
	setPixmap(item.pixmap());
	input_ = item.input_;
	entry_ = item.entry_;
	kernel_function_type = item.kernel_function_type;
	kernel_parameter1 = item.kernel_parameter1;
	kernel_parameter2 = item.kernel_parameter2;
	model_parameters = item.model_parameters;
	optimize_model_parameters = item.optimize_model_parameters;
	optimize_kernel_parameters = item.optimize_kernel_parameters;
	grid_search_stepwidth = item.grid_search_stepwidth;
	grid_search_steps = item.grid_search_steps;
	grid_search_recursions = item.grid_search_recursions;
	k_fold = item.k_fold;
	save_attribute_ = item.save_attribute_;
	prediction_input_edges_ = item.prediction_input_edges_;

	QSARData q;

	if(!entry_->kernel)
	{
		model_ = (*entry_->create)(q);
	}
	else if(entry_->kernel && kernel_function_type < 4)
	{
		model_ = entry_->createKernel1(q, 1, 1.0, 1.0);
	}
	else
	{
		model_ = entry_->createKernel2(q,"", "");
	}

	if (item.model_ != NULL)
	{
		(*model_).operator=(*item.model_);
	}
	else
	{
		model_ = NULL;
	}
	
}

ModelItem::~ModelItem()
{

	if (view_->name == "view")
	{
		MainWindow* mw = view_->data_scene->main_window;
		mw->removeFromPipeline(this);
	}
	delete model_;
}

void ModelItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{	
	QByteArray itemData;
	QDataStream dataStream(&itemData, QIODevice::WriteOnly);
	QDrag *drag = new QDrag(event->widget());

	/// set member-pointer of main_window to the current object in order to gain access to this object from another widget 
	view_->data_scene->main_window->dragged_item = this;
	view_->data_scene->main_window->drag_source = view_->name;

	QMimeData *mimeData = new QMimeData;
	mimeData->setData("application/x-modelitemdata", itemData);
	drag->setMimeData(mimeData);
	drag->setPixmap(pixmap());
	drag->setHotSpot(QPoint(0, 0));
	drag->start();
	itemChange(ItemPositionChange, pos());
	QGraphicsItem::mousePressEvent(event);
}

ModelItem* ModelItem::createModel(InputDataItem* inputdata)
{
	ModelItem* item = new ModelItem(inputdata, entry_, view_);
	return item;
}

ModelItem* ModelItem::createModel(InputDataItem* inputdata, int kernelType, double parameter1, double parameter2)
{
	ModelItem* item = new ModelItem(inputdata, entry_, kernelType, parameter1, parameter2,  view_);

	item->grid_search_stepwidth = grid_search_stepwidth;
	item->grid_search_steps = grid_search_steps;
	item->grid_search_recursions = grid_search_recursions;
	item->k_fold = k_fold;

	return item;
}
ModelItem* ModelItem::createModel(InputDataItem* inputdata, String s1, String s2)
{
	ModelItem* item = new ModelItem(inputdata, entry_, s1, s2,  view_);
	item->grid_search_stepwidth = grid_search_stepwidth;
	item->grid_search_steps = grid_search_steps;
	item->grid_search_recursions = grid_search_recursions;
	item->k_fold = k_fold;

	return item;
}

void ModelItem::setModel(Model* model)
{
	model_ = model;
}

void  ModelItem::setEntry(RegistryEntry* entry)
{
	entry_ = entry;
}

RegistryEntry* ModelItem::getRegistryEntry()
{
	return entry_;
}

Model* ModelItem::model()
{	
	return model_;
}

InputDataItem* ModelItem::inputDataItem()
{
	return input_;
}

void ModelItem::trainModel()
{
	model_->readTrainingData();
	try 
	{
		model_->train();
	}
	catch (SingularMatrixError)
	{
		throw ModelTrainingError(__FILE__,__LINE__);
	}
}

void  ModelItem::setSaveAttribute(bool save)
{
	save_attribute_ = save;
}

bool  ModelItem::saveAttribute()
{
	return save_attribute_;
}

void ModelItem::setInputDataItem(InputDataItem* item)
{
	input_ = item;
}

void ModelItem::addPredictionInputEdge(Edge* edge)
{
	prediction_input_edges_.insert(edge);
	if (in_edge_list_.contains(edge))
	{
		in_edge_list_.erase(in_edge_list_.find(edge));
	}
}

void ModelItem::deletePredictionInputEdge(Edge* edge)
{
	if (prediction_input_edges_.contains(edge))
	{
		prediction_input_edges_.erase(prediction_input_edges_.find(edge));
	}
	delete edge;
}