#include <BALL/APPLICATIONS/QSAR_GUI/mainWindow.h>
#include <BALL/QSAR/exception.h>
#include <BALL/APPLICATIONS/QSAR_GUI/exception.h>
#include <BALL/APPLICATIONS/QSAR_GUI/inputDataDialog.h>
#include <BALL/APPLICATIONS/QSAR_GUI/CSVInputDialog.h>
#include <BALL/APPLICATIONS/QSAR_GUI/dottedEdge.h>
#include <BALL/APPLICATIONS/QSAR_GUI/inputDataItemIO.h>

#include <QtGui/QMessageBox>
#include <QtGui/QFileDialog>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QDockWidget>
#include <QtCore/QTextStream>

#include <sstream>
#include <map>

using namespace BALL::QSAR;
using namespace BALL::QSAR::Exception;
using namespace BALL::VIEW;
using namespace BALL::VIEW::Exception;
using namespace BALL::Exception;

///set up the main window
 MainWindow::MainWindow()
 {
	///set up the main scenes & views
	view_ = new DataItemView(&view_scene_,this);
	view_->name = "view";
	view_->mapToScene(0,0);
	view_->addDropSite();

	model_list_ = new DataItemView(&model_list_scene_,this);
	model_list_->name = "model_list";
	model_list_->setAcceptDrops(false); 

	fs_list_ = new DataItemView(&fs_list_scene_,this);
	fs_list_->name = "fs_list";
	fs_list_->setAcceptDrops(false);

	val_list_ = new DataItemView(&val_list_scene_, this);
	val_list_->name = "val_list";
	val_list_->setAcceptDrops(false);
	
	///create a registry with registered models
	reg_ = new Registry;

	progress_bar_ = new QProgressBar(this);

	///set the view as central widget
	setCentralWidget(view_);
	
	// read information about last used paths
	String s = QDir::homePath().toStdString();
	s = s+"/.QSARGUI";
	ifstream in(s.c_str());
	settings.input_data_path=QDir::homePath().toStdString();
	settings.config_path=QDir::homePath().toStdString();
	settings.size_x=0; settings.size_y=0;
	settings.pos_x=0; settings.pos_y=0;
	if(in) 
	{
		in>>settings.input_data_path;
		if(in) in>>settings.config_path;
		if(in) in>>settings.size_x;
		if(in) in>>settings.size_y;
		if(in) in>>settings.pos_x;
		if(in) in>>settings.pos_y;
		in.close();
	}

	///create actions, menus, tool bars, status bar, dock windows and dialogs
	createActions();
	createMenus();	
	createToolBars();
	createStatusBar();
	createDockWindows();
	createDialogs();

	dragged_item = NULL;
	
	//just for style-reasons 
	setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

	if(settings.size_x==0 || settings.size_y==0) showMaximized();
	else 
	{
		resize(settings.size_x,settings.size_y);
		move(settings.pos_x,settings.pos_y);
		show();
	}
	
	drag_start_time = drag_start_time.now();
	min_drag_time=0.3;

	connect(this, SIGNAL(sendNewValue(int)), progress_bar_, SLOT(setValue(int))); 
 }

MainWindow::~MainWindow()
{
	delete view_;
	delete model_list_;
	delete fs_list_;
	delete reg_;
	delete file_browser_;
	delete fileMenu_;
	delete editMenu_;
	delete helpMenu_;
	delete windowMenu_;	
	delete fileToolBar_;
	
	// save path to last used input file
	String file = QDir::homePath().toStdString();
	file = file+"/.QSARGUI";
	ofstream output(file.c_str());
	output<<settings.input_data_path<<endl<<flush;
	output<<settings.config_path<<endl<<flush;
	QSize s = size();
	output<<s.width()<<"  "<<s.height()<<endl;
	QPoint p = pos();
	output<<p.x()<<"  "<<p.y()<<endl;
	output.close();
}

///create a new SDFInputItem with the given file
SDFInputDataItem* MainWindow::createSDFInput(QString filename)
{
	SDFInputDataItem* input = new SDFInputDataItem(filename, view_);
	bool ok = false;
	int exec = 0;
	SDFInputDialog inputDialog(input);

	///as long as there's no valid input
	while (!ok)	
	{ 
		try
		{
			/// show dialog
			exec = inputDialog.exec();

			///if user accepts
			if(exec==1)
			{
				/// read in the given activity numbers
				inputDialog.getNumbers(); 
				ok= true;
			}
			else
			{
				throw InvalidInputDataItem(__FILE__,__LINE__);
			}
		}

		catch(BALL::QSAR::Exception::InvalidActivityID e)
		{
			ok = false;
			QMessageBox::about(this, tr("Error"),tr("Invalid activity ID"));
		}
	}
	return input;
}

///create a new CSVInputItem with the given file
CSVInputDataItem* MainWindow::createCSVInput(QString filename)
{
	CSVInputDataItem* input = new CSVInputDataItem(filename, view_);
	bool ok = false;
	int exec = 0;
	CSVInputDialog csvInputDialog(input);
	///as long as there's no valid input
	while (!ok)	
	{ 
		try
		{
			/// show dialog
			exec = csvInputDialog.exec();

			///if user accepts
			if(exec==1)
			{
				csvInputDialog.readNumY(); 
				ok= true;
			}
			else
			{
				throw InvalidInputDataItem(__FILE__,__LINE__);
			}
		}
		catch(BALL::QSAR::Exception::InvalidActivityID e)
		{
			ok = false;
			QMessageBox::about(this, tr("Error"),tr("Invalid activity ID"));
		}
	}
	return input;
}


///create a new CSVInputItem with the given file
CSVInputDataItem* MainWindow::createCSVInput(QSARData* data)
{
	CSVInputDataItem* input = new CSVInputDataItem(data);
	bool ok = false;
	int exec = 0;
	CSVInputDialog csvInputDialog(input);
	///as long as there's no valid input
	while (!ok)	
	{ 
		try
		{
			/// show dialog
			exec = csvInputDialog.exec();

			///if user accepts
			if(exec==1)
			{
				csvInputDialog.readNumY(); 
				ok= true;
			}
			else
			{
				throw InvalidInputDataItem(__FILE__,__LINE__);
			}
		}
		catch(BALL::QSAR::Exception::InvalidActivityID e)
		{
			ok = false;
			QMessageBox::about(this, tr("Error"),tr("Invalid activity ID"));
		}
	}
	return input;
}


///create a new ModelItem, that is connected to an InputDataItem
ModelItem* MainWindow::createModel(ModelItem* model, InputDataItem* input)
{
	//if the input item is invalid do nothing and return
	if (model==NULL)
	{
		throw InvalidModelItem(__FILE__,__LINE__);
	}

	RegistryEntry* entry = model->getRegistryEntry();
	String name = entry->name_abreviation;
	
	if (input && !entry->regression)
	{
		if (!input->checkForDiscreteY())
		{
			QMessageBox::information(this," ","Some class labels of this input file are not discrete values! Creation of a classification model is therefore not possible.");
			throw InvalidModelItem(__FILE__,__LINE__);	
		}
	}

	modelConfigurationDialog_ = new ModelConfigurationDialog(model, input, this);

	// is there is nothing to be asked of the user, then do not ask him
	if(entry->parameterNames.size()==0 && !entry->kernel)
	{
		modelConfigurationDialog_->createModel();
		model = modelConfigurationDialog_->modelItem();
		if (model == NULL)
		{
			throw InvalidModelItem(__FILE__,__LINE__);	
		}
		else
		{
			return model;
		}
	}
	else if (modelConfigurationDialog_->exec() == 1)
	{	
		model = modelConfigurationDialog_->modelItem();
		if (model == NULL)
		{
			throw InvalidModelItem(__FILE__,__LINE__);	
		}
		else
		{
			return model;
		}
	}
	else
	{
		throw InvalidModelItem(__FILE__,__LINE__);
	}
}

// void MainWindow::changeModelItem(ModelItem* model, InputDataItem* input)
// {
// 	//if the input item is invalid do nothing and return
// 	if (model==NULL || input==NULL )
// 	{
// 		throw InvalidModelItem(__FILE__,__LINE__);
// 	}
// 
// 	RegistryEntry* entry = model->getRegistryEntry();
// 	String name = entry->name_abreviation;
// 	std::cout << "CM " << name.c_str() << std::endl;
// 
// 	if (input && !entry->regression)
// 	{
// 		if (!input->checkForDiscreteY())
// 		{
// 			QMessageBox::information(this," ","Some class labels of this input file are not discrete values! Creation of a classification model is therefore not possible.");
// 			throw InvalidModelItem(__FILE__,__LINE__);	
// 		}
// 	}
// 
// 	//create the model's configuration dialog with the information from the corresponding registry entry for that model
// 	modelConfigurationDialog_ = new ModelConfigurationDialog(model, input, this);
// 
// 	if (modelConfigurationDialog_->exec() == 1)
// 	{	
// 		model = modelConfigurationDialog_->modelItem();
// 		if (model == NULL)
// 		{
// 			std::cout << "CM Invalid Model" << std::endl;
// 			throw InvalidModelItem(__FILE__,__LINE__);	
// 		}
// 	}
// 	else
// 	{
// 		std::cout << "CM Invalid Model" << std::endl;
// 		throw InvalidModelItem(__FILE__,__LINE__);
// 	}
// }

///create a new FeatureSelectionItem, that is connected to the given ModelItems
FeatureSelectionItem* MainWindow::createFeatureSelection(FeatureSelectionItem* fs, ModelItem* model, ModelItem* in_model)
{
	//if the input item is invalid do nothing and return
	if (fs==NULL)
	{
		throw InvalidFeatureSelectionItem(__FILE__,__LINE__);
	}

	FeatureSelectionItem* item = new FeatureSelectionItem(*fs);
	FeatureSelectionDialog featureSelectionDialog(item,model);

	bool ok = false;
	int exec = 0;
	while (!ok)	
	{
		try
		{
			exec = featureSelectionDialog.exec();

			if(exec==1)
			{
 				featureSelectionDialog.applyInput(); 
				ok = true;
				item->setModelItem(model);
				item->setInputModelItem(in_model);
				item->setValidationStatistic(featureSelectionDialog.getValidationStatistic());
			}
			else
			{
				ok=true;
				throw InvalidFeatureSelectionItem(__FILE__,__LINE__);	
			}
		}
		catch(InvalidK)
		{
			ok=false;
			QMessageBox::information(this, tr(""),tr("Invalid value"));
		}
	}
	return item;
}


///create a new ValidationItem, that is connected to the given ModelItem
ValidationItem* MainWindow::createValidation(ValidationItem* val, ModelItem* model)
{
	///if the input item is invalid do nothing and return
	if (!val)
	{
		throw InvalidValidationItem(__FILE__,__LINE__);
	}

	ValidationItem* item = new ValidationItem(*val);
	if (val->getValidationType() != 1)
	{
		ValidationDialog validationDialog(item, model);
	
		bool ok = false;
		int exec = 0;

		while (!ok)	
		{
			try
			{
				exec = validationDialog.exec();
	
				if(exec==1)
				{
					validationDialog.applyInput(); 
					ok = true;
					item->setModelItem(model);
					item->setValidationStatistic(validationDialog.getValidationStatistic());
				}
				else
				{
					ok=true;
					throw InvalidValidationItem(__FILE__,__LINE__);		
				}
			}
			catch(InvalidK)
			{
				ok=false;
				QMessageBox::information(this, tr(""),tr("Invalid value"));
			}
		}
	}
	else
	{
		item->setModelItem(model);	
	}
	
	return item;
}

///create a new PredictionItem, that is connected to the given ModelItem
PredictionItem* MainWindow::createPrediction(InputDataItem* input, ModelItem* model)
{
	if (input==NULL || model==NULL)
	{
		throw InvalidPredictionItem(__FILE__,__LINE__);
	}
	PredictionItem* item = new PredictionItem(input, model, view_);
	return item;
}

 void MainWindow::about()
{
	QMessageBox::information(this, tr("About QSAR_GUI"), tr("QSAR_GUI is developed by Lisa Blass<br>lisa@bioinf.uni-sb.de"),
	QMessageBox::Ok);
}

/*
function for creating all needed dialogs in the main window
*/
void MainWindow::createDialogs()
{
	modelConfigurationDialog_ = new ModelConfigurationDialog();
}

/*
function for creating actions for every slot the user should have access to
*/
void MainWindow::createActions()
 {
	//exit action: cloes the application
	exitAct_ = new QAction(QIcon("./images/exit.png"),tr("E&xit"), this);
	exitAct_->setShortcut(tr("Ctrl+Q"));
	exitAct_->setStatusTip(tr("Exit the application"));
	connect(exitAct_, SIGNAL(triggered()), this, SLOT(close()));

	//about action: shows information about the application
	aboutAct_ = new QAction(tr("&About"), this);
	aboutAct_->setStatusTip(tr("Show the application's About box"));
	connect(aboutAct_, SIGNAL(triggered()), this, SLOT(about()));

	clearAct_ = new QAction(QIcon("./images/clear_desktop.png"),tr("&Clear Desktop"), this);
	clearAct_->setShortcut(tr("Ctrl+C"));
	clearAct_->setStatusTip(tr("Clears the desktop (the main view)"));
	connect(clearAct_, SIGNAL(triggered()), this, SLOT(clearDesktop()));

	delAct_ = new QAction(QIcon("./images/delete_item.png"),tr("&Delete Selection"), this);
	delAct_->setStatusTip(tr("Deletes the selected Item from the pipeline"));
	connect(delAct_, SIGNAL(triggered()), this, SLOT(deleteItem()));

	executeAct_ = new QAction(QIcon("./images/run_pipeline.png"),tr("&Execute Pipeline"), this);
	executeAct_->setShortcut(tr("Ctrl+E"));
	executeAct_->setStatusTip(tr("Executes the Pipeline"));
	connect(executeAct_, SIGNAL(triggered()), this, SLOT(executePipeline()));

	restoreAct_ = new QAction(QIcon("./images/restore_desktop.png"),tr("&Restore Desktop"), this);
	restoreAct_->setShortcut(tr("Ctrl+R"));
	restoreAct_->setStatusTip(tr("Restores a Pipeline"));
	connect(restoreAct_, SIGNAL(triggered()), this, SLOT(restoreDesktop()));

	exportAct_ = new QAction(QIcon("./images/save.png"),tr("Save Pipeline"), this);
	exportAct_->setStatusTip(tr("Saves the Pipeline to a configuration file for the QSARPipelinePackage and stores all trained models to files"));
	connect(exportAct_, SIGNAL(triggered()), this, SLOT(exportPipeline()));

	loadModelsAct_ = new QAction(QIcon(),tr("Load Models"), this);
	connect(loadModelsAct_, SIGNAL(triggered()), this, SLOT(loadModels()));
 }

/*
function for setting up the menus
*/
 void MainWindow::createMenus()
 {
	fileMenu_ = menuBar()->addMenu(tr("&File"));
	fileMenu_->addAction(exitAct_);

	editMenu_ = menuBar()->addMenu(tr("&Edit"));

	windowMenu_ = menuBar()->addMenu(tr("&Windows"));

	menuBar()->addSeparator();

	helpMenu_ = menuBar()->addMenu(tr("&Help"));
	helpMenu_->addAction(aboutAct_);
 }

/*
function for setting up the tool bars
*/
 void MainWindow::createToolBars()
 {	
	fileToolBar_ = addToolBar(tr("File"));
	fileToolBar_->addAction(exitAct_);
	fileToolBar_->addAction(clearAct_);
	fileToolBar_->addAction(delAct_);
	fileToolBar_->addAction(exportAct_);
	fileToolBar_->addAction(restoreAct_);
	fileToolBar_->addAction(executeAct_);
	fileToolBar_->addAction(loadModelsAct_);
 }

/*
function for setting up the status bar
*/
 void MainWindow::createStatusBar()
 {
     statusBar()->showMessage(tr(" "));
 }

/*
function for creating the different dock windows
*/
void MainWindow::createDockWindows()
{	
	file_browser_ = new FileBrowser(settings.input_data_path.c_str());
	QDockWidget *filedock = new QDockWidget(tr("Source Filebrowser"), this);
	filedock->setAllowedAreas(Qt::LeftDockWidgetArea);
	filedock->setWidget(file_browser_);
	addDockWidget(Qt::LeftDockWidgetArea, filedock);
	windowMenu_->addAction(filedock->toggleViewAction());

	///create dock widget for listing all available models
	int j=0;
	for(uint i=0; i<reg_->registered_models.size();i++)
	{ 
		ModelItem* item = new ModelItem(&reg_->registered_models[i], model_list_);
		model_list_scene_.addItem(item);
		if (reg_->registered_models[i].kernel)
		{
			j++;
			item->setPos(120,70*(j-1)+20);
		}
		else
		{
			item->setPos(20,70*(i-j)+20);
		}
	}

	QDockWidget *modeldock = new QDockWidget(tr("Models"), this);
	modeldock->setAllowedAreas(Qt::LeftDockWidgetArea);
	modeldock->setWidget(model_list_);
	
	QColor c1(160,172,182);
	QBrush b1(c1,Qt::SolidPattern);
	model_list_->setBackgroundBrush(b1);
	
	addDockWidget(Qt::LeftDockWidgetArea, modeldock);
	windowMenu_->addAction(modeldock->toggleViewAction());

	///create dock widget for listing all available feature selection methods
	for(uint i=0; i<4;i++)
	{ 
		FeatureSelectionItem* item = new FeatureSelectionItem(i, fs_list_);
		fs_list_scene_.addItem(item);
		item->setPos(20,70*i+20);
	}
	QDockWidget* fsdock = new QDockWidget(tr("Feature Selection"), this);
	fsdock->setAllowedAreas(Qt::LeftDockWidgetArea);
	fsdock->setWidget(fs_list_);
	addDockWidget(Qt::LeftDockWidgetArea, fsdock);
	windowMenu_->addAction(fsdock->toggleViewAction());
	
	QColor c2(245,244,180);
	QBrush b2(c2,Qt::SolidPattern);
	fs_list_->setBackgroundBrush(b2);

	///create dock widget for  model validation
	for(uint i=1; i<5;i++)
	{ 
		ValidationItem* item = new ValidationItem(i, val_list_);
		val_list_scene_.addItem(item);
		item->setPos(20,70*(i-1)+20);

	}
	QDockWidget* validationdock = new QDockWidget(tr("Validation"), this);
	validationdock->setAllowedAreas(Qt::LeftDockWidgetArea);
	validationdock->setWidget(val_list_);
	addDockWidget(Qt::LeftDockWidgetArea, validationdock);
	windowMenu_->addAction(validationdock->toggleViewAction());
	
	QColor c3(205,225,205);
	QBrush b3(c3,Qt::SolidPattern);
	val_list_->setBackgroundBrush(b3);

	tabifyDockWidget(modeldock,fsdock);
	tabifyDockWidget(fsdock, validationdock);

	QDockWidget* progressdock = new QDockWidget(tr("Progress"), this);
	progressdock->setAllowedAreas(Qt::LeftDockWidgetArea);
	progressdock->setWidget(progress_bar_);
	addDockWidget(Qt::LeftDockWidgetArea, progressdock);
	windowMenu_->addAction(progressdock->toggleViewAction());	
	
}

void MainWindow::deleteItem()
{
	QList<QGraphicsItem*> items = view_scene_.selectedItems();
	if (items.size() > 0)
	{
		QList<QGraphicsItem*>::Iterator it = items.begin();
	
		DataItem* item = static_cast<DataItem *>(*it);
		//deleteItem(item);
		delete item;
	}
	else
	{
		QMessageBox::information(this, tr(" "),tr("No item selected"));
	}
	view_scene_.update(); // remove junk from screen
}

///delete everything on the desktop
void MainWindow::clearDesktop()
{
	QSet<SDFInputDataItem*> sdf_input_pipeline_copy = sdf_input_pipeline_;
	for (QSet<SDFInputDataItem*>::iterator it = sdf_input_pipeline_copy.begin(); it != sdf_input_pipeline_copy.end(); ++it)
	{
		delete *it;
	}
 	sdf_input_pipeline_.clear();

	QSet<CSVInputDataItem*> csv_input_pipeline_copy = csv_input_pipeline_;
	for (QSet<CSVInputDataItem*>::iterator it = csv_input_pipeline_copy.begin(); it != csv_input_pipeline_copy.end(); ++it)
	{
		delete *it;
	}
 	csv_input_pipeline_.clear();
	
	QSet<DataItem*> disconnected_items_copy = disconnected_items_;

	for (QSet<DataItem*>::iterator it = disconnected_items_copy.begin(); it != disconnected_items_copy.end(); ++it)
	{
		delete *it;
	}
	
	view_scene_.update();
}

// SLOT
void MainWindow::restoreDesktop()
{
	QString filename = QFileDialog::getOpenFileName(this, tr("Open File"),settings.config_path.c_str(),tr("text (*.txt)"));
	String s = filename.toStdString();
	settings.config_path = s.substr(0,s.find_last_of("/")+1);
	try
	{
		restoreDesktop(filename);
	}
	catch(InvalidPipeline)
	{
		QMessageBox::about(this, tr("Error"),tr("Invalid Pipeline"));
	}
}


// SLOT
void MainWindow::exportPipeline()
{
	QString filename = QFileDialog::getSaveFileName(this, tr("Save File as"),(settings.config_path+"config.txt").c_str(),tr("text (*.txt)"));
	exportPipeline(filename);
	String s = filename.toStdString();
	settings.config_path = s.substr(0,s.find_last_of("/")+1);
}


void MainWindow::saveModels(String directory)
{
	try
	{
		for (QSet<ModelItem*>::Iterator it = model_pipeline_.begin(); it != model_pipeline_.end(); it++)
		{
			// if model has not yet been trained, there is nothing to be saved
			if(!(*it)->isDone()) continue;
			
			Model* model = (*it)->model();
			String file = directory;
			String f1 = (*it)->savedAs().toStdString();
			if(f1=="")
			{
				throw GeneralException(__FILE__,__LINE__,"Model saving error ", "model must be assigned a file to be saved to!");
			}
			file += f1;
			model->saveToFile(file);
		}
	}
	catch(GeneralException e)
	{	
		QMessageBox::warning(this, tr("Error"),e.getMessage());
	}
}

void MainWindow::loadModels()
{
	try
	{
		for (QSet<ModelItem*>::Iterator it = model_pipeline_.begin(); it != model_pipeline_.end(); it++)
		{
			String filename=directory+(*it)->savedAs().toStdString();
			ifstream input(filename.c_str());
			// read only existing models
			if(input)
			{
				input.close();
				(*it)->loadModel(filename);
			}
		}
	}
	catch(GeneralException e)
	{
		QMessageBox::warning(this, tr("Error"),e.getMessage());
	}
}

Registry* MainWindow::registry()
{
	return reg_;
}

QSet<ModelItem*> MainWindow::getModelPipeline()
{
	return model_pipeline_;
}

// void MainWindow::addInputToPipeline(InputDataItem* item)
// {
// 	if (item)
// 	{
// 		if (SDFInputDataItem* sdf_item = qgraphicsitem_cast<SDFInputDataItem*>(item))
// 		{
// 			sdf_input_pipeline_.insert(sdf_item);
// 		}
// 		else if (CSVInputDataItem* csv_item = qgraphicsitem_cast<CSVInputDataItem*>(item))
// 		{
// 			csv_input_pipeline_.insert(csv_item);	
// 		}
// 	}
// }

// void MainWindow::addModelToPipeline(ModelItem* item)
// {
// 	if (item != NULL)
// 	{
// 		model_pipeline_.insert(item);
// 	}
// }

// void MainWindow::addPredictionToPipeline(PredictionItem* item)
// {
// 	if (item != NULL)
// 	{
// 		prediction_pipeline_.insert(item);
// 	}
// }

// void MainWindow::addFeatureSelectionToPipeline(FeatureSelectionItem* item)
// {
// 	if (item != NULL)
// 	{
// 		fs_pipeline_.insert(item);
// 	}
// }

// void MainWindow::addValidationToPipeline(ValidationItem* item)
// {
// 	if (item != NULL)
// 	{
// 		val_pipeline_.insert(item);
// 	}
// }

void MainWindow::executePipeline()
{
	int maximum = sdf_input_pipeline_.size() + csv_input_pipeline_.size() + model_pipeline_.size() + fs_pipeline_.size() + val_pipeline_.size() + prediction_pipeline_.size();
	
	if (maximum == 0)
	{
		return;
	}

	int value = 0;
	progress_bar_->setMaximum(maximum);
	
	///read in the input files
	for (QSet<SDFInputDataItem*>::Iterator it = sdf_input_pipeline_.begin(); it != sdf_input_pipeline_.end(); it++)
	{
		try
		{
			(*it)->readData();	
		}
		catch(BALL::Exception::GeneralException e)
		{
			QMessageBox::warning(this,"Error",e.getMessage());
		}

		value++;
		emit sendNewValue(value);
	}

	for (QSet<CSVInputDataItem*>::Iterator it = csv_input_pipeline_.begin(); it != csv_input_pipeline_.end(); it++)
	{
		try
		{
			if ((*it)->append())
			{
				(*it)->appendData();
			}
			else 
			{
				(*it)->readData();
			}
		}
		catch(BALL::Exception::GeneralException e)
		{
			QMessageBox::warning(this,"Error",e.getMessage());
		}

		value++;
		emit sendNewValue(value);
	}
	
	if (!sdf_input_pipeline_.empty() || csv_input_pipeline_.empty())
	{
		statusBar()->showMessage(tr("All input files were read"));
	}

	///train all models and set their saved as names
	int model_counter=0;
	QString name;
	for (QSet<ModelItem*>::Iterator it = model_pipeline_.begin(); it != model_pipeline_.end(); it++)
	{
		(*it)->setSavedAs((*it)->name() + name.setNum(model_counter) + ".mod");
		model_counter++;
		try
		{
			(*it)->model()->setDataSource((*it)->inputDataItem()->data());
		
			if ((*it)->optimize_model_parameters)
			{
				(*it)->model()->optimizeParameters((*it)->k_fold);
			}
			if ((*it)->optimize_kernel_parameters)
			{
				KernelModel* km = (KernelModel*)((*it)->model());
				km->kernel->gridSearch((*it)->grid_search_stepwidth, (*it)->grid_search_steps,(*it)->grid_search_recursions,(*it)->k_fold);
				(*it)->setModel(km);
			}
			(*it)->trainModel();
		}
		catch (ModelTrainingError e)
		{	
			QString error_string = e.getMessage();
			QMessageBox::warning(this,"Error",error_string);
/*
			(*it)->setEntry(reg_->getRegistryEntry(String("RR")));
			changeModelItem(*it,(*it)->inputDataItem());
			(*it)->setName(QString((*it)->getRegistryEntry()->name_abreviation.c_str()));
			(*it)->setSaveAttribute(true);

 			ModelItem* old_model = new ModelItem(**it);
			(*it) = new ModelItem(reg_->getRegistryEntry(BALL::String("RR")), view_);
			view_scene_.addItem(*it);
			(*it)->setEntry(reg_->getRegistryEntry(BALL::String("RR")));
			(*it) = createModel(*it,old_model->inputDataItem());
			delete old_model;

			(*it)->setModel(NULL);
			(*it)->setEntry(reg_->getRegistryEntry(BALL::String("RR")));
			(*it) = createModel(*it,(*it)->inputDataItem());	
			(*it)->setName(QString((*it)->getRegistryEntry()->name_abreviation.c_str()));	

			changeModelItem(*it,(*it)->inputDataItem());
			std::cout << "MW, entry: " << reg_->getModelNo((*it)->getRegistryEntry()->name_abreviation) << std::endl;
			std::cout <<  "MW, model name: " << (*it)->name().toStdString() << std::endl;*/
		}
		catch(WrongDataType e)
		{
			QMessageBox::warning(this," ",e.getMessage());	
		}
		catch(ParseError)
		{
			QMessageBox::warning(this," ","The individual kernel functions were invalid");	
		}
		value++;
		emit sendNewValue(value);
	}

	if (!model_pipeline_.empty())
	{
		statusBar()->showMessage(tr("All models were trained"));
	}

	///feature selection
	for (QSet<FeatureSelectionItem*>::Iterator it = fs_pipeline_.begin(); it != fs_pipeline_.end(); it++)
	{	
		try
		{
			QString num;
			(*it)->connectWithModelItem();	
			(*it)->modelItem()->trainModel();
		}
		catch(BALL::Exception::GeneralException e)
		{
			QMessageBox::warning(this,"Error",e.getMessage());
		}
		value++;
		emit sendNewValue(value);
	}
	if (!fs_pipeline_.empty())
	{
		statusBar()->showMessage(tr("Feature Selection"));
	}
		


	///activity prediction
	for (QSet<PredictionItem*>::Iterator it = prediction_pipeline_.begin(); it != prediction_pipeline_.end(); it++)
	{
		try
		{
			(*it)->connectWithModelItem();
		}
		catch(BALL::Exception::GeneralException e)
		{	
			QMessageBox::about(this,"Error",e.getMessage());
		}
		catch(BaseException e)
		{	
			QMessageBox::about(this,"Error",e.what());
		}
		value++;
		emit sendNewValue(value);
	}
	if (!prediction_pipeline_.empty())
	{
		statusBar()->showMessage(tr("Prediction"));
	}

	///validation
	for (QSet<ValidationItem*>::Iterator it = val_pipeline_.begin(); it != val_pipeline_.end(); it++)
	{
		try
		{
			(*it)->connectWithModelItem();
		}
		catch(InconsistentUsage)
		{
			QString error_string = "The model " + QString(((*it)->modelItem()->model()->getType()->c_str()))+ "  Model must be trained before its fit to the input data can be evaluated.";
			QMessageBox::about(this,"Error",error_string);
		}
		value++;
		emit sendNewValue(value);	
	}

	progress_bar_->reset();
}


void MainWindow::addDisconnectedItem(DataItem* item)
{
	if (item != NULL)
	{
		disconnected_items_.insert(item);
	}
}

QSet<DataItem*> MainWindow::disconnectedItems()
{
	return disconnected_items_;
}



///
/// IMPORT & EXPORT
///

void MainWindow::restoreDesktop(QString filename)
{
	ifstream file(filename.toStdString().c_str());
	if(!file)
	{
		cout<<"file does not exist!!"<<endl;
		return;
	}
	try
	{
		bool input_section=0;
		bool model_section=0;
		bool fs_section=0;
		bool val_section=0;	
		bool pred_section=0;
		String section="";
		map<String, DataItem*> filenames_map;
		InputDataItemIO input_reader(view_);
		
		/// first of all, read [ItemPositions] section:
		list<pair<double,double> > item_positions;
		bool within_pos_section=0;
		for(int i=0;!file.eof();i++)
		{
			String line="";
			getline(file,line);
			if(line=="" || line.hasPrefix("#") || line.hasPrefix("//") || line.hasPrefix("%"))
			{
				continue;
			}
			if(line.hasPrefix("["))
			{
				if(within_pos_section) break;
				else if(line.hasPrefix("[ItemPositions]")) within_pos_section=1;
			}
			else if(within_pos_section)
			{
				double x = line.getField(0).toDouble(); double y = line.getField(1).toDouble();
				item_positions.push_back(make_pair(x,y));
			}
		}
		file.close();
		file.open(filename.toStdString().c_str());	

		/// read all other sections
		for(int i=0;!file.eof();i++)
		{
			String line="";
			getline(file,line);
			
			if(line=="" || line.hasPrefix("#") || line.hasPrefix("//") || line.hasPrefix("%"))
			{
				continue;
			}
			if(line.hasPrefix("["))
			{
				if(input_section) input_reader.readConfigSection(section,filenames_map,&item_positions);
 				if(model_section) new ModelItem(section,filenames_map,&item_positions,view_);
 				if(fs_section) new FeatureSelectionItem(section,filenames_map,&item_positions,view_);
 				if(val_section) new ValidationItem(section,filenames_map,&item_positions,view_);
				if(pred_section) new PredictionItem(section,filenames_map,&item_positions,view_);
				
				input_section=0;model_section=0;fs_section=0;
				val_section=0;pred_section=0;
				if(line.hasPrefix("[InputReader]")) input_section=1;
				else if(line.hasPrefix("[ModelCreator]")) model_section=1;
				else if(line.hasPrefix("[FeatureSelector]")) fs_section=1;
				else if(line.hasPrefix("[Validator]")) val_section=1;
				else if(line.hasPrefix("[Predictor]")) pred_section=1;
				
				section=line+"\n"; // save section header
				continue;
			}
			
			section+=line+"\n"; // store line of current section
		}
 		if(input_section) input_reader.readConfigSection(section,filenames_map,&item_positions);
 		if(model_section) new ModelItem(section,filenames_map,&item_positions,view_);
 		if(fs_section) new FeatureSelectionItem(section,filenames_map,&item_positions,view_);
 		if(val_section) new ValidationItem(section,filenames_map,&item_positions,view_);
		if(pred_section) new PredictionItem(section,filenames_map,&item_positions,view_);
		
		file.close();
	}
	catch(BALL::Exception::GeneralException e)
	{
		QMessageBox::warning(this,"Error",e.getMessage());
	}
	
	view_scene_.update();
	view_->update();
	
	/// read all model if respec. files exist in the folder of the config-file
	String f = filename.toStdString();
	String directory = f.substr(0,f.find_last_of("/")+1);
	loadModels(directory);
}


void MainWindow::setLastUsedPath(String path)
{
	settings.input_data_path = path;	
}


void MainWindow::exportPipeline(QString filename)
{
	int maximum = sdf_input_pipeline_.size() + csv_input_pipeline_.size() + model_pipeline_.size() + val_pipeline_.size() + prediction_pipeline_.size() + disconnected_items_.size(); //all items - fs-items (model items automatically created by feature selection are not exported)

	if (maximum == 0)
	{
		return;
	}
	
	int value = 0;
	progress_bar_->setMaximum(maximum);

	ofstream out(filename.toStdString().c_str());
	InputDataItemIO input_writer(view_);
	int counter = 0;

	QString name;
	String f = filename.toStdString();
	String directory = f.substr(0,f.find_last_of("/")+1);
	
	ostringstream positions;
	positions<<"[ItemPositions]"<<endl;
	
	/// SDFInputItems
	for (QSet<SDFInputDataItem*>::Iterator it = sdf_input_pipeline_.begin(); it != sdf_input_pipeline_.end(); it++)
	{
		SDFInputDataItem* item = (*it);
		item->setSavedAs(item->name()+".dat");
		input_writer.writeConfigSection(item,out);
		positions<<item->x()<<"  "<<item->y()<<endl;
		value++;
		emit sendNewValue(value);
	}

	///CSVInputItems
	for (QSet<CSVInputDataItem*>::Iterator it = csv_input_pipeline_.begin(); it != csv_input_pipeline_.end(); it++)
	{
		CSVInputDataItem* item = (*it);
		item->setSavedAs(item->name()+".dat");
		input_writer.writeConfigSection(item,out);
		positions<<item->x()<<"  "<<item->y()<<endl;
		value++;
		emit sendNewValue(value);
	}

	///Model Items
	counter=0;
	for (QSet<ModelItem*>::Iterator it = model_pipeline_.begin(); it != model_pipeline_.end(); it++,counter++)
	{
		ModelItem* item = (*it); 
		item->setSavedAs(item->name() + name.setNum(counter) + ".mod");

		// do not write a config-file section for models that are created by feature selection.
		// --> FeatureSelector will create these models; no need to run ModelCreator for these ones!
		if (item->saveAttribute())
		{
			item->writeConfigSection(out);
			positions<<item->x()<<"  "<<item->y()<<endl;
			value++;
			emit sendNewValue(value);
		}
	}
	
	/// save models to files
	saveModels(directory);
	
	///Feature Selection Items
	counter=0;
	for (QSet<FeatureSelectionItem*>::Iterator it = fs_pipeline_.begin(); it != fs_pipeline_.end(); it++,counter++)
	{
		FeatureSelectionItem* item = *it;
		item->writeConfigSection(out);
		positions<<item->x()<<"  "<<item->y()<<endl;
		positions<<item->modelItem()->x()<<"  "<<item->modelItem()->y()<<endl;
		value++;
		emit sendNewValue(value);
	}	
		
	///Validation Items
	counter=0;
	for (QSet<ValidationItem*>::Iterator it = val_pipeline_.begin(); it != val_pipeline_.end(); it++,counter++)
	{
		ValidationItem* item = (*it); 
		item->setSavedAs(name.setNum(counter)+".val");
		item->writeConfigSection(out);
		positions<<item->x()<<"  "<<item->y()<<endl;
		value++;
		emit sendNewValue(value);
	}

	///Prediction Items
	counter=0;
	for (QSet<PredictionItem*>::Iterator it = prediction_pipeline_.begin(); it != prediction_pipeline_.end(); it++,counter++)
	{
		PredictionItem* item = (*it); 
		item->setSavedAs(name.setNum(counter) + ".pred");
		item->writeConfigSection(out);
		positions<<item->x()<<"  "<<item->y()<<endl;
		value++;
		emit sendNewValue(value);
	}
	
	out<<positions.str().c_str()<<endl;
	out.close();
	progress_bar_->reset();
}



