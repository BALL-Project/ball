#include <BALL/APPLICATIONS/QSAR_GUI/modelItem.h>
#include <QtGui/QDrag>
#include <QtCore/QMimeData>
#include <BALL/APPLICATIONS/QSAR_GUI/mainWindow.h>
#include <BALL/QSAR/exception.h>
#include <BALL/APPLICATIONS/QSAR_GUI/exception.h>
#include <QtGui/QMessageBox>
#include <QtGui/QFileDialog>

using namespace BALL::QSAR;
using namespace BALL::QSAR::Exception;
using namespace BALL::VIEW;
using namespace BALL::VIEW::Exception;
using namespace BALL::Exception;


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
	result_color_ = QColor(160,172,182);
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
	createActions();
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
	result_color_ = QColor(160,172,182);
	
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
	createActions();
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
	result_color_ = QColor(160,172,182);
	
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
	createActions();
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
	result_color_ = QColor(160,172,182);
	
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
	createActions();
}

ModelItem::ModelItem(ModelItem& item):
QObject(),
DataItem(item.view_)
{
	result_color_ = item.result_color_;
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
	done_ = item.done_;
	result_ = item.result_;
	
	// do NOT copy from 'item' but connect to the methods of this new object!!
	createActions();

	QSARData q;

	if(!entry_->kernel)
	{
		model_ = (*entry_->create)(q);
	}
	else if(entry_->kernel && kernel_function_type < 4)
	{
		model_ = entry_->createKernel1(q, kernel_function_type, kernel_parameter1, kernel_parameter2);
	}
	else
	{
		Kernel* kernel = ((KernelModel*)item.model())->kernel;
		model_ = entry_->createKernel2(q,kernel->equation1,kernel->equation2);
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



ModelItem::ModelItem(String& configfile_section, std::map<String, DataItem*>& filenames_map, list<pair<double,double> >* item_positions, DataItemView* view)
	: DataItem(view)
{
	result_color_=QColor(160,172,182);
	istringstream input;
	input.str(configfile_section);
		
	String line;
	getline(input,line);
	line.trimLeft();
	if(!line.hasPrefix("[ModelCreator]"))
	{
		throw BALL::Exception::GeneralException(__FILE__,__LINE__,"Model reading error","The given section is no model section!");
	}

	String data_file=""; String output=""; 
	model_parameters.clear();
	int model_no=0;
	
	data_file=""; output=""; model_no=0; grid_search_steps=0; grid_search_recursions=0;
	grid_search_stepwidth=0; k_fold=5; optimize_model_parameters=0;
	optimize_kernel_parameters=0; kernel_function_type=1; kernel_parameter1=0; kernel_parameter2=0;
	
	while(input)
	{
		getline(input,line);
		line.trimLeft();
		if(line=="" || line.hasPrefix("#") || line.hasPrefix("//") || line.hasPrefix("%"))
		{
			continue;
		}
	
		if(line.hasPrefix("data_file"))
		{
			data_file = ((String)line.after("=")).trimLeft();
		}
		else if(line.hasPrefix("output"))
		{
			output = ((String)line.after("=")).trimLeft();
		}
		else if(line.hasPrefix("model_parameters"))
		{
			line = ((String)line.after("=")).trimLeft();
			for(uint i=0; i<line.countFields(" ");i++)
			{
				model_parameters.push_back(line.getField(i).toDouble());
			}
		}
		else if(line.hasPrefix("model_no"))
		{
			model_no = ((String)line.after("=")).trimLeft().toInt();
		}
		else if(line.hasPrefix("grid_search_steps"))
		{
			grid_search_steps = ((String)line.after("=")).trimLeft().toInt();
		}
		else if(line.hasPrefix("grid_search_recursions"))
		{
			grid_search_recursions = ((String)line.after("=")).trimLeft().toInt();
		}
		else if(line.hasPrefix("grid_search_stepwidth"))
		{
			grid_search_stepwidth = ((String)line.after("=")).trimLeft().toDouble();
		}
		else if(line.hasPrefix("k_fold"))
		{
			k_fold = ((String)line.after("=")).trimLeft().toInt();
		}
		else if(line.hasPrefix("optimize_model_parameters"))
		{
			optimize_model_parameters = ((String)line.after("=")).trimLeft().toDouble();
		}
		else if(line.hasPrefix("kernel_type"))
		{
			kernel_function_type = ((String)line.after("=")).trimLeft().toInt();
		}
		else if(line.hasPrefix("kernel_par1"))
		{
			kernel_parameter1 = ((String)line.after("=")).trimLeft().toDouble();
		}
		else if(line.hasPrefix("kernel_par2"))
		{
			kernel_parameter2 = ((String)line.after("=")).trimLeft().toDouble();
		}
		else
		{
			String mess = "Configuration command \""+line+"\" unknown!!";
			String name = "ModelItem reading error";
			throw BALL::Exception::GeneralException(__FILE__,__LINE__,name,mess);
		}
	}

	map<String,DataItem*>::iterator it = filenames_map.find(data_file);
	if(it==filenames_map.end())
	{
		throw BALL::Exception::GeneralException(__FILE__,__LINE__,"Model reading error","InputDataItem for a model does not exist!");
	}
	input_ =  (InputDataItem*) it->second;
	Registry* reg = view_->data_scene->main_window->registry();
	if((uint)model_no>reg->registered_models.size())
	{
		throw BALL::Exception::GeneralException(__FILE__,__LINE__,"Model reading error","The given model-no does not exist!");
	}
	entry_ = &reg->registered_models[model_no];
	if(!entry_->kernel)
	{
		model_ = (entry_->create)(*input_->data());
	}
	else
	{
		model_ = (entry_->createKernel1)(*input_->data(),kernel_function_type,kernel_parameter1,kernel_parameter2);
		if(grid_search_steps>0 && grid_search_stepwidth>0) optimize_kernel_parameters=1;
	}
	
	model_->setParameters(model_parameters);
	
	view_->data_scene->addItem(this);
	addToPipeline();
	if(item_positions!=0 && item_positions->size()>0)
	{
		pair<double,double> pos = item_positions->front();
		item_positions->pop_front();
		setPos(pos.first,pos.second);
	}
	
	Edge* edge = new Edge(input_, this);
	view_->data_scene->addItem(edge);
	
	save_attribute_ = 1;
	QPixmap pm;
	if(entry_->kernel)
	{
		pm = QPixmap("./images/kernel_model.png").scaled(QSize(width(), height()), Qt::KeepAspectRatio,Qt::FastTransformation );
	}
	else
	{
		pm = QPixmap("./images/model.png").scaled(QSize(width(), height()), Qt::KeepAspectRatio,Qt::FastTransformation );
	}
	setPixmap(pm);
	setName(QString(entry_->name_abreviation.c_str()));
	createActions();
	
	filenames_map.insert(make_pair(output,this));
	setSavedAs(output.c_str());
	done_ = 0; // model not trained yet and no trained model read
}



ModelItem::~ModelItem()
{
	if (view_->name == "view")
	{
		//if the item was connected to others, delete it from its respective pipeline
		if (!removeDisconnectedItem())
		{
			removeFromPipeline();
		}
	}
	delete model_;
	delete save_action;
	delete load_action;
	delete properties_action;
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

bool ModelItem::execute()
{
	if(done_) return 0; // do nothing twice...
	
	if (optimize_model_parameters)
	{
		model_->optimizeParameters(k_fold);
	}
	if (optimize_kernel_parameters)
	{
		KernelModel* km = (KernelModel*)model_;
		km->kernel->gridSearch(grid_search_stepwidth, grid_search_steps,grid_search_recursions,k_fold);
		//setModel(km);
	}
	
	model_->readTrainingData();
	model_->train();
	
	setResultString((int)model_->getDescriptorNames()->size());
	done_ = 1; //ready!
	return 1;
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
	if (in_edge_list_.find(edge)!=in_edge_list_.end())
	{
		in_edge_list_.erase(in_edge_list_.find(edge));
	}
}

void ModelItem::deletePredictionInputEdge(Edge* edge)
{
	if (prediction_input_edges_.find(edge)!=prediction_input_edges_.end())
	{
		prediction_input_edges_.erase(prediction_input_edges_.find(edge));
	}
	delete edge;
}

void ModelItem::createActions()
{
	save_action = new QAction(QIcon("./images/save_desktop.png"),tr("Save model"), this);
	connect(save_action, SIGNAL(triggered()), this, SLOT(saveModel()));

	load_action = new QAction(QIcon("./images/save_desktop.png"),tr("Load model"), this);
	connect(load_action, SIGNAL(triggered()), this, SLOT(loadModel()));

	properties_action = new QAction(QIcon("./images/save_desktop.png"),tr("Show Properties"), this);
	connect(properties_action, SIGNAL(triggered()), this, SLOT(showProperties()));
}


// SLOT
void ModelItem::saveModel()
{
	try
	{
		if(done_)
		{
			QString filename = QFileDialog::getSaveFileName(view_, tr("Save File as"),"",tr("text (*.txt)"));
			model_->saveToFile(filename.toStdString());
		}
	}
	catch(GeneralException e)
	{	
		QMessageBox::about(view_, tr("Error"),e.getMessage());
	}	
}

void ModelItem::saveModel(QString file)
{
	try
	{
		if(done_)
		{
			model_->saveToFile(file.toStdString());
		}
	}
	catch(GeneralException e)
	{	
		QMessageBox::about(view_, tr("Error"),e.getMessage());
	}
}

void ModelItem::loadModel()
{
	QString filename = QFileDialog::getOpenFileName(view_, tr("Open Model"),"",tr("models (*.mod)"));
	if (!filename.isEmpty())
	{
		try
		{
			model_->readFromFile(filename.toStdString());
		}
		catch(WrongDataType e)
		{
			QMessageBox::warning(view_,"Error",e.getMessage());
			return;
		}
	}
	
	setResultString((int)model_->getDescriptorNames()->size());
	done_ = 1;
}

void ModelItem::loadFromFile(String file)
{
	try
	{
		model_->readFromFile(file.toStdString());
	}
	catch(WrongDataType e)
	{
		QMessageBox::warning(view_,"Error",e.getMessage());
		return;
	}
	
	if(!save_attribute_) // if this model was created by a feature selection, also set FeatureSelectionItem::done_ to true
	{
		for(set<Edge*>::iterator it=in_edge_list_.begin(); it!=in_edge_list_.end();it++)
		{
			if((*it)->sourceNode()->type()==FeatureSelectionItem::Type)
			{
				(*it)->sourceNode()->setDone(1);
			}
		}
	}
	
	setResultString((int)model_->getDescriptorNames()->size());
	done_ = 1;
}

void ModelItem::showProperties()
{
	ModelConfigurationDialog modelConfigurationDialog(this, view_->data_scene->main_window);
	modelConfigurationDialog.exec();
}

void ModelItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	if (view_->name == "view")
	{
		QMenu menu(view_);
		menu.addAction(save_action);
		menu.addAction(load_action);
		menu.addAction(properties_action);
		menu.exec(event->screenPos());
	}
}

void ModelItem::writeConfigSection(ofstream& out)
{
	String parameter_string;
	String tmp;
	for (uint i = 0; i < model_parameters.size(); i++)
	{
		parameter_string += " "+ String(model_parameters[i]);
	}
	
	bool hasKernel = getRegistryEntry()->kernel;
	
	out << "[ModelCreator]" << "\n";
	out << "data_file = "<< inputDataItem()->savedAs().toStdString() << "\n";
	out << "model_no = "<< view_->data_scene->main_window->reg_->getModelNo(getRegistryEntry()->name_abreviation) << "\n";
	out << "model_parameters = "<< parameter_string << "\n";
	
	if (hasKernel)
	{
		out << "kernel_type = "<< kernel_function_type <<"\n";
	
		if (kernel_function_type != 4)
		{
			out << "kernel_par1 = "<< kernel_parameter1 << "\n";
			if (kernel_function_type == 3)
			{
				out << "kernel_par2 = " << kernel_parameter2 << "\n";
			}
		}
		out << "grid_search_steps = "<< grid_search_steps << "\n";
		out << "grid_search_stepwidth = "<< grid_search_stepwidth <<"\n";
		out << "grid_search_recursions = "<< grid_search_recursions << "\n";
	}
	
	out << "optimize_model_parameters = "<< optimize_model_parameters << "\n";
	
	if (optimize_model_parameters)
	{
		out << "k_fold = "<< k_fold <<  "\n";
	}
	out << "output = "<< savedAs().toStdString() << "\n\n";
}

void ModelItem::removeFromPipeline()
{
	view_->data_scene->main_window->model_pipeline_.erase(this);
}

void ModelItem::addToPipeline()
{
	view_->data_scene->main_window->model_pipeline_.insert(this);
}
