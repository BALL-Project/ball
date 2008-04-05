#include <BALL/APPLICATIONS/QSAR_GUI/mainWindow.h>
#include <BALL/QSAR/exception.h>
#include <BALL/APPLICATIONS/QSAR_GUI/exception.h>
#include <BALL/APPLICATIONS/QSAR_GUI/inputDataDialog.h>
#include <BALL/APPLICATIONS/QSAR_GUI/CSVInputDialog.h>
#include <BALL/APPLICATIONS/QSAR_GUI/dottedEdge.h>


#include <QtGui/QMessageBox>
#include <QtGui/QFileDialog>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QDockWidget>
#include <QtCore/QTextStream>
#include <QtCore/QHash>

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

	//path_ = new Path;
	// 	QDir dir;
	// 	std::string path = dir.currentPath().toStdString();
	// 	path_->setDataPath(String(path));

	progress_bar_ = new QProgressBar(this);

	///set the view as central widget
	setCentralWidget(view_);

	///create actions, menus, tool bars, status bar, dock windows and dialogs
	createActions();
	createMenus();	
	createToolBars();
	createStatusBar();
	createDockWindows();
	createDialogs();

	dragged_item = NULL;
	
	///just for style-reasons 
	setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

	///maximize the main window on startup
	showMaximized();
	
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
	String p = QDir::homePath().toStdString();
	p = p+"/.QSARGUI";
	ofstream output(p.c_str());
	cout<<"writing \'"<<last_path_<<"\' ."<<endl;
	output<<last_path_<<endl<<flush;
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

	saveAct_ = new QAction(QIcon("./images/save_desktop.png"),tr("&Save Desktop"), this);
	saveAct_->setShortcut(tr("Ctrl+S"));
	saveAct_->setStatusTip(tr("Saves the pipeline on the desktop"));
	connect(saveAct_, SIGNAL(triggered()), this, SLOT(saveDesktop()));

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

	exportAct_ = new QAction(QIcon("./images/save.png"),tr("Export Pipeline"), this);
	exportAct_->setStatusTip(tr("Exports the Pipeline in a configuration file for the QSAR Pipeline Package"));
	connect(exportAct_, SIGNAL(triggered()), this, SLOT(exportPipeline()));
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
	fileToolBar_->addAction(saveAct_);
	fileToolBar_->addAction(exportAct_);
	fileToolBar_->addAction(restoreAct_);
	fileToolBar_->addAction(executeAct_);
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
	// read information about last used input path
	String s = QDir::homePath().toStdString();
	s = s+"/.QSARGUI";
	ifstream in(s.c_str());
	String path="";
	if(in) 
	{
		in>>path;
		in.close();
	}
	last_path_ = path;
	file_browser_ = new FileBrowser(path.c_str());
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


void MainWindow::restoreDesktop()
{
	QString filename = QFileDialog::getOpenFileName(this, tr("Open File"),"",tr("text (*.txt)"));
		
	try
	{
		restoreDesktop(filename);
	}
	catch(InvalidPipeline)
	{
		QMessageBox::about(this, tr("Error"),tr("Invalid Pipeline"));
	}
}

void MainWindow::exportPipeline()
{
	QString filename = QFileDialog::getSaveFileName(this, tr("Save File as"),"config.txt",tr("text (*.txt)"));
	exportPipeline(filename, false);
}

void MainWindow::saveDesktop()
{
	QString filename = QFileDialog::getSaveFileName(this, tr("Save File as"),"pipeline_gui.txt",tr("text (*.txt)"));
	exportPipeline(filename, true);
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
			(*it)->setSavedAs(QString(file.c_str()));
			model->saveToFile(file);
		}
	}
	catch(GeneralException e)
	{	
		QMessageBox::about(this, tr("Error"),e.getMessage());
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

void MainWindow::addInputToPipeline(InputDataItem* item)
{
	if (item)
	{
		if (SDFInputDataItem* sdf_item = qgraphicsitem_cast<SDFInputDataItem*>(item))
		{
			sdf_input_pipeline_.insert(sdf_item);
		}
		else if (CSVInputDataItem* csv_item = qgraphicsitem_cast<CSVInputDataItem*>(item))
		{
			csv_input_pipeline_.insert(csv_item);	
		}
	}
}

void MainWindow::addModelToPipeline(ModelItem* item)
{
	if (item != NULL)
	{
		model_pipeline_.insert(item);
	}
}

void MainWindow::addPredictionToPipeline(PredictionItem* item)
{
	if (item != NULL)
	{
		prediction_pipeline_.insert(item);
	}
}

void MainWindow::addFeatureSelectionToPipeline(FeatureSelectionItem* item)
{
	if (item != NULL)
	{
		fs_pipeline_.insert(item);
	}
}

void MainWindow::addValidationToPipeline(ValidationItem* item)
{
	if (item != NULL)
	{
		val_pipeline_.insert(item);
	}
}

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

	///train all models
	for (QSet<ModelItem*>::Iterator it = model_pipeline_.begin(); it != model_pipeline_.end(); it++)
	{
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


bool MainWindow::removeFromPipeline(DataItem* item)
{
	if(disconnected_items_.remove(item))
	{
		return 1;
	}

	else if (SDFInputDataItem* iitem =  qgraphicsitem_cast<SDFInputDataItem*>(item))
	{
		if(sdf_input_pipeline_.remove(iitem))
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}

	else if (CSVInputDataItem* ciitem =  qgraphicsitem_cast<CSVInputDataItem*>(item))
	{
		if(csv_input_pipeline_.remove(ciitem))
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}

	else if (ModelItem* mitem = qgraphicsitem_cast<ModelItem*>(item))
	{
		if(model_pipeline_.remove(mitem))
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}

	else if (PredictionItem* pitem = qgraphicsitem_cast<PredictionItem*>(item))
	{
		if(prediction_pipeline_.remove(pitem))
		{
			return 1;
		}
		else
		{

			return 0;
		}
	}

	else if (ValidationItem* vitem = qgraphicsitem_cast<ValidationItem*>(item))
	{
		if(val_pipeline_.remove(vitem))
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}

	else if (FeatureSelectionItem* fitem = qgraphicsitem_cast<FeatureSelectionItem*>(item))
	{
		if(fs_pipeline_.remove(fitem))
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}

	
	else
	{
		return 0;
// 		if(disconnected_items_.remove(item))
// 		{
// 			std::cout << "disconnected erased" << std::endl;
// 			return 1;
// 		}
// 		else
// 		{
// 			std::cout << "disconnected NOT erased" << std::endl;
// 			return 0;
// 		}
	} 
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
///
///
///
///IMPORT & EXPORT
///
///
///
///

void MainWindow::restoreDesktop(QString filename)
{
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
        	return;
	}

	///flags for the different sections that can occour in a pipeline config file
	bool input_section=0;
	bool model_section=0;
	bool fs_section=0;
	bool val_section=0;	
	bool pred_section=0;

	int num_of_items=0;

	///positions of the items
	float x = 0.;
	float y = 0.;

	///input item parameters
	QString sd_file="";
	SortedList<int> activities;
	bool center_data = 1;
	bool center_response = 1;
	QString data_output="";

	///model item parameters
	vector<double> parameters;
	QString data="";
	int model_no=-1; 
	int kernel_type=0;
	double kernel_par1=0;
	double kernel_par2=-1;
	int k_fold=0;
	bool optimize_model_parameters = 0;
	double grid_search_stepwidth = 0.25;
	int grid_search_steps = 0;
	int grid_search_recursions = 0;
	QString model_output="";

	/// feature selection item parameters
	QString fs_model_input="";
	QString fs_data="";
	int selection_type=-1;
	QString fs_output="";
	int fs_k=-1;
	bool opt_par = 0;

	///validation item parameters
	QString val_model_input="";
	QString val_data="";
	QString ext_val_data="";
	QString val_output="";
	int val_k_folds=0;
	int bootstrap_samples=0;
	int no_of_permutation_tests = 0;
	int val_type = 0;

	///prediction item parameters
	QString pred_model_input="";
	QString pred_data="";
	QString pred_output="";
	bool print_excepted=1;
	QStringList tmp_result;

	QHash<QString, DataItem*> hash;

	QString l = file.readLine().trimmed();
	if (l.startsWith("num of items"))
	{
		QStringList tmp_result = l.split("=");
		bool ok = false;
		num_of_items = tmp_result[tmp_result.size()-1].toInt(&ok);
	}

	int value = 0;
	progress_bar_->setMaximum(num_of_items);

	try
	{
		///read in complete file
		while (!file.atEnd()) 
		{
			/// read the current line and remove whitespaces from start and end
			QString line = file.readLine().trimmed();

			///if line empty: continue with next line
			if (line =="")
			{
				continue;
			}

			///search for section headers
			if (!(input_section || model_section || fs_section || val_section || pred_section))
			{
				///InputReader section
				if(line.startsWith("[InputReader]",Qt::CaseInsensitive))
				{
					input_section=1;
					model_section=0;
					fs_section=0;
					val_section=0;	
					pred_section=0;
				}
	
				///ModelCreator section
				else if(line.startsWith("[ModelCreator]",Qt::CaseInsensitive))
				{
					model_section=1;
					input_section=0;
					fs_section=0;
					val_section=0;	
					pred_section=0;
				}
	
				///FeatureSelection section
				else if(line.startsWith("[FeatureSelector]",Qt::CaseInsensitive))
				{
					fs_section=1;
					input_section=0;
					model_section=0;
					val_section=0;	
					pred_section=0;
				}
	
				///Validator section
				else if(line.startsWith("[Validator]",Qt::CaseInsensitive))
				{
					val_section=1;
					input_section=0;
					model_section=0;
					fs_section=0;
					pred_section=0;
				}
	
				///Predictor
				else if(line.startsWith("[Predictor]",Qt::CaseInsensitive))
				{
					pred_section=1;
					input_section=0;
					model_section=0;
					fs_section=0;
					val_section=0;	
				}
				continue;
			}
			else
			{
			}

			///[INPUT]
			if (input_section == 1)
			{
				QStringList tmp_result;

				///look for parameters
				if(line.startsWith("sd_file"))
				{
					tmp_result = line.split("=");
					sd_file = tmp_result[tmp_result.size()-1].trimmed();
					continue;
				}
			
				else if(line.startsWith("activity_IDs"))
				{
					tmp_result = line.split("=");
					QString act = tmp_result[tmp_result.size()-1];
					tmp_result = act.split(" ");
		
					bool ok;
		
					foreach (QString num, tmp_result)
					{
						int res = num.toInt(&ok);
						if (ok && res >= 0)
						{	
							activities.insert(res);	
						}
					}
					continue;	
				}

				else if(line.startsWith("center_data"))
				{
					tmp_result = line.split("=");
					if (tmp_result[tmp_result.size()-1].trimmed() == "1")
					{
						center_data = true;
					}
					else if (tmp_result[tmp_result.size()-1].trimmed() == "0")
					{
						center_data = false;
					}
					continue;
				}
				else if(line.startsWith("center_response"))
 				{
					tmp_result = line.split("=");
					if (tmp_result[tmp_result.size()-1] == "1")
					{
						center_response = true;
					}
					else if (tmp_result[tmp_result.size()-1] == "0")
					{
						center_response = false;
					}
					continue;
 				}
	
				else if(line.startsWith("x_coordinate"))
				{
					tmp_result = line.split("=");
					bool ok;
					x = tmp_result[tmp_result.size()-1].toFloat(&ok);
					continue;
				}
	
				else if(line.startsWith("y_coordinate"))
				{
					tmp_result = line.split("=");
					bool ok;
					y = tmp_result[tmp_result.size()-1].toFloat(&ok);
				}

				else if(line.startsWith("output"))
				{
					tmp_result = line.split("=");
					data_output = tmp_result[tmp_result.size()-1].trimmed();
					continue;
				}

				else
				{
					continue;
				}
				
				if(sd_file=="")
				{
					QMessageBox::critical(this,"Error","SD file must be specified within config-file!");
					break;
				}

				if(data_output=="")
				{
					QMessageBox::critical(this,"Error","Output file must be specified within config-file!");
					break;
				}
				if(sd_file!="" && activities.size()==0)
				{
					QMessageBox::critical(this,"Error","Activity IDs must be specified within config-file!");
					break;
				}
		

				std::cout << "Create Input" << std::endl;
				try
				{
					SDFInputDataItem* input;
					input = new SDFInputDataItem(sd_file, activities,center_data, center_response, view_);
					input->setPos(x,y);
					view_scene_.addItem(input);
					addInputToPipeline(input);
					hash.insert(data_output, input);	
				}
				catch(InvalidActivityID)
				{
					QMessageBox::critical(this,"Error","Invalid activity ID");
					break;
				}


				activities.clear();
				input_section=0;
				value++;
				emit sendNewValue(value);
			}//if (input_section == 1)

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

			///[MODEL]
			else if (model_section == 1)
			{
				QStringList tmp_result;

				if(line.startsWith("data_file"))
				{
					tmp_result = line.split("=");
					data = tmp_result[tmp_result.size()-1].trimmed();
					continue;
				}
				else if(line.startsWith("output"))
				{
					tmp_result = line.split("=");
					model_output = tmp_result[tmp_result.size()-1].trimmed();
					continue;
				}
				else if(line.startsWith("model_parameters"))
				{
					tmp_result = line.split("=");
					QString par = tmp_result[tmp_result.size()-1];
					tmp_result = par.split(" ");
		
					bool ok;
					double res;
		
					foreach (QString num, tmp_result)
					{
						res = num.toDouble(&ok);
						if (ok && res >= 0)
						{	
							parameters.push_back(res);
						}
					}
					continue;
				}
				else if(line.startsWith("model_no"))
				{
					tmp_result = line.split("=");
					bool ok;
					
					model_no = tmp_result[tmp_result.size()-1].toInt(&ok);
					continue;
				}
				else if(line.startsWith("grid_search_steps"))
				{
					tmp_result = line.split("=");
					bool ok;
					
					grid_search_steps = tmp_result[tmp_result.size()-1].toInt(&ok);
					continue;
				}
				else if(line.startsWith("grid_search_recursions"))
				{
					tmp_result = line.split("=");
					bool ok;
					
					grid_search_recursions = tmp_result[tmp_result.size()-1].toInt(&ok);
					continue;
				}
				else if(line.startsWith("grid_search_stepwidth"))
				{
					tmp_result = line.split("=");
					bool ok;
					grid_search_stepwidth = tmp_result[tmp_result.size()-1].toDouble(&ok);
					continue;

				}
				else if(line.startsWith("k_fold"))
				{
					tmp_result = line.split("=");
					bool ok;
					k_fold = tmp_result[tmp_result.size()-1].toInt(&ok);
					continue;
				}
				else if(line.startsWith("optimize_model_parameters"))
				{
					tmp_result = line.split("=");
					if (tmp_result[tmp_result.size()-1] == "1")
					{
						optimize_model_parameters = true;
					}
					else if (tmp_result[tmp_result.size()-1] == "0")
					{
						optimize_model_parameters = false;
					}
					continue;
				}
				else if(line.startsWith("kernel_type"))
				{
					tmp_result = line.split("=");
					bool ok;
					kernel_type = tmp_result[tmp_result.size()-1].toInt(&ok);
					continue;
				}
				else if(line.startsWith("kernel_par1"))
				{	
					tmp_result = line.split("=");
					bool ok;
					kernel_par1 = tmp_result[tmp_result.size()-1].toDouble(&ok);
					continue;
				}
				else if(line.startsWith("kernel_par2"))
				{
					tmp_result = line.split("=");
					bool ok;
					kernel_par2 = tmp_result[tmp_result.size()-1].toDouble(&ok);
					continue;
				}

				else if(line.startsWith("x_coordinate"))
				{
					tmp_result = line.split("=");
					bool ok;
					
					x = tmp_result[tmp_result.size()-1].toFloat(&ok);
					continue;
				}
	
				else if(line.startsWith("y_coordinate"))
				{
					tmp_result = line.split("=");
					bool ok;
					y = tmp_result[tmp_result.size()-1].toFloat(&ok);
				}

				else
				{
					continue;
				}

				if(data=="")
				{
					QMessageBox::critical(this,"Error","QSARData file must be specified within config-file!");
					break;
				}
				if(model_output=="")
				{
					QMessageBox::critical(this,"Error","Output file must be specified within config-file!");
					break;
				}
				if(model_no==-1)
				{
					QMessageBox::critical(this,"Error","The model type must be specified within config-file!");
					break;
				}

				bool kernel=0;

				DataItem* d_item = hash.value(data);
				InputDataItem* input = qgraphicsitem_cast<InputDataItem*>(d_item);

				ModelItem* model_item = new ModelItem(&reg_->registered_models[model_no], view_);
				
				if(reg_->registered_models[model_no].create!=NULL)
				{
					model_item = model_item->createModel(input);
				}
				else
				{
					if(kernel_type==0 || kernel_par1==0)
					{
						QMessageBox::critical(this,"Error","For kernel based model, kernel-type and kernel-parameter(s) must be specified!");
						break;
					}
		
					model_item = model_item->createModel(input, kernel_type, kernel_par1, kernel_par2);
					kernel=1;
				}
				
				if(parameters.size()>0)
				{
					model_item->model()->setParameters(parameters);
					parameters.clear();
				}
				if(optimize_model_parameters)
				{
					if(k_fold==0)
					{
						QMessageBox::critical(this,"Error","'k_fold' must be set if model parameters are to be optimized!");
						break;
					}
					model_item->model()->optimizeParameters(k_fold);
				}
				if(kernel && grid_search_steps>0)
				{
					if(k_fold==0)
					{
						QMessageBox::critical(this,"Error","'k_fold' must be set if grid search is to be done");
						break;
					}
					if(grid_search_stepwidth==0 && kernel_type!=2)
					{ 
						QMessageBox::critical(this,"Error","'grid_search_stepwidth' must be set if grid search is to be done!");
						break;
					} 
					KernelModel* km = (KernelModel*)(model_item->model());
					km->kernel->gridSearch(grid_search_stepwidth, grid_search_steps,grid_search_recursions,k_fold);
					model_item->setModel(km);
				}
				model_item->setPos(x,y);
				view_scene_.addItem(model_item);
				addModelToPipeline(model_item);
				Edge* edge = new Edge(input, model_item);
				view_scene_.addItem(edge);
				hash.insert(model_output, model_item);
				
				model_section=0;
				value++;
				emit sendNewValue(value);
			}//if (input_section == 1)
	
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			///[FEATURE SELECTION]
			else if (fs_section == 1)
			{
				QStringList tmp_result; 

				if(line.startsWith("model_file"))
				{
					tmp_result = line.split("=");
					fs_model_input = tmp_result[tmp_result.size()-1].trimmed();
					continue;
				}
				else if(line.startsWith("data_file"))
				{
					tmp_result = line.split("=");
					fs_data = tmp_result[tmp_result.size()-1].trimmed();
					continue;
				}
				else if(line.startsWith("feature_selection_type"))
				{
					tmp_result = line.split("=");
					bool ok;
					selection_type = tmp_result[tmp_result.size()-1].toInt(&ok);
					continue;
				}
				else if(line.startsWith("k_fold"))
				{
					tmp_result = line.split("=");
					bool ok;
					fs_k = tmp_result[tmp_result.size()-1].toInt(&ok);
					continue;
				}
				else if(line.startsWith("output"))
				{
					tmp_result = line.split("=");
					fs_output = tmp_result[tmp_result.size()-1].trimmed();
					continue;
				}
		
				else if(line.startsWith("optimize_parameters"))
				{
					tmp_result = line.split("=");
					if (tmp_result[tmp_result.size()-1] == "1")
					{
						opt_par = true;
					}
					else if (tmp_result[tmp_result.size()-1] == "0")
					{
						opt_par = false;
					}
					continue;
				}

				else if(line.startsWith("x_coordinate"))
				{
					tmp_result = line.split("=");
					bool ok;
					
					x = tmp_result[tmp_result.size()-1].toFloat(&ok);
					continue;
				}
	
				else if(line.startsWith("y_coordinate"))
				{
					tmp_result = line.split("=");
					bool ok;
					y = tmp_result[tmp_result.size()-1].toFloat(&ok);
				}

				else
				{
					continue;
				}

				if(fs_data=="")
				{
					QMessageBox::critical(this,"Error","QSARData file must be specified within config-file!");
					break;
				}
				if(fs_model_input=="")
				{
					QMessageBox::critical(this,"Error","Model file must be specified within config-file!");
					break;
				}
				if(fs_output=="")
				{
					QMessageBox::critical(this,"Error","Output file must be specified within config-file!");
					break;
				}
				if(fs_k==-1)
				{
					QMessageBox::critical(this,"Error","k-fold must be specified within config-file!");
					break;
				}

				DataItem* d_item = hash.value(fs_model_input);
				if (d_item == NULL)
				{
					break;
				}
				ModelItem* model_item = qgraphicsitem_cast<ModelItem*>(d_item);

				FeatureSelectionItem* fs_item = new FeatureSelectionItem(selection_type, view_);

				ModelItem* model_copy = new ModelItem(*model_item);

				try
				{
					fs_item->setModelItem(model_copy);
					fs_item->setInputModelItem(model_item);
					fs_item->setK(fs_k);
					fs_item->setOpt(opt_par);

					///model is generated automatically, so it doesn't need to be saved as a textfile
					model_copy->setSaveAttribute(false);
					model_copy->setPos(x,y);

					view_scene_.addItem(model_copy);
					addModelToPipeline(model_copy);
					hash.insert(fs_output, model_copy);

					DataItem* it = hash.value(fs_model_input);
					if (it == NULL)
					{
						break;
					}
					
					view_scene_.addItem(fs_item);
					fs_item->setPos(x,y);

					Edge* edge = new Edge(fs_item, model_copy);
					view_scene_.addItem(edge);
					Edge* edge2 = new Edge(model_item, fs_item);
					view_scene_.addItem(edge2);
					addFeatureSelectionToPipeline(fs_item);
				}

				catch(InvalidFeatureSelectionItem)
				{
					delete model_copy;
				}

				std::cout << "Feature Selection" << std::endl;
				fs_section=0;
				value++;
				emit sendNewValue(value);
			}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			///[VALIDATION]
			else if (val_section == 1)
			{
				QStringList tmp_result; 

				if(line.startsWith("model_file"))
				{
					tmp_result = line.split("=");
					val_model_input = tmp_result[tmp_result.size()-1].trimmed();
					continue;
				}
				else if(line.startsWith("data_file"))
				{
					tmp_result = line.split("=");
					val_data = tmp_result[tmp_result.size()-1].trimmed();
					continue;
				}
				else if(line.startsWith("validation_data_file"))
				{
					continue;
				}
				else if(line.startsWith("k_fold"))
				{
					tmp_result = line.split("=");
					bool ok;
					val_k_folds = tmp_result[tmp_result.size()-1].toInt(&ok);
					continue;
				}
				else if(line.startsWith("bootstrap_samples"))
				{
					tmp_result = line.split("=");
					bool ok;
					bootstrap_samples = tmp_result[tmp_result.size()-1].toInt(&ok);
					continue;
				}
				else if(line.startsWith("no_of_permutation_tests"))
				{
					tmp_result = line.split("=");
					bool ok;
					no_of_permutation_tests = tmp_result[tmp_result.size()-1].toInt(&ok);
					continue;
				}
				else if(line.startsWith("output"))
				{	
					tmp_result = line.split("=");
					val_output = tmp_result[tmp_result.size()-1].trimmed();
					continue;
				}

				else if(line.startsWith("validation_type"))
				{	
					tmp_result = line.split("=");
					bool ok;
					val_type = tmp_result[tmp_result.size()-1].toInt(&ok);
					continue;
				}

				else if(line.startsWith("x_coordinate"))
				{
					tmp_result = line.split("=");
					bool ok;	
					x = tmp_result[tmp_result.size()-1].toFloat(&ok);
					continue;
				}
	
				else if(line.startsWith("y_coordinate"))
				{
					tmp_result = line.split("=");
					bool ok;
					y = tmp_result[tmp_result.size()-1].toFloat(&ok);
				}
				else
				{
					continue;
				}

				if(val_data=="")
				{
					QMessageBox::critical(this,"Error","QSARData file must be specified within config-file!");
					break;
				}
				if(val_model_input=="")
				{
					QMessageBox::critical(this,"Error","Model file must be specified within config-file!");
					break;
				}

				if(val_k_folds==0 && val_type != 1 && val_type!=3)
				{
					QMessageBox::critical(this,"Error","'k_folds' must be specified within config-file!");
					break;
				}

				DataItem* d_item = hash.value(val_model_input);
				ModelItem* model_item = qgraphicsitem_cast<ModelItem*>(d_item);
				
				ValidationItem* val_item = new ValidationItem(val_type, view_);
				val_item->setModelItem(model_item);
				
				switch(val_type)
				{
					case 1:
						break;
					case 2:
						val_item->setK(val_k_folds);
						break;
					case 3:	
						val_item->setNumOfSamples(bootstrap_samples);
						break;
					case 4:	
						val_item->setK(val_k_folds);
						val_item->setNumOfRuns(no_of_permutation_tests);
						break;
					default: 
						throw InvalidValidationItem(__FILE__,__LINE__);
				}

				view_scene_.addItem(val_item);
				val_item->setPos(x,y);

				Edge* edge = new Edge(model_item, val_item);
				view_scene_.addItem(edge);
				addValidationToPipeline(val_item);

				std::cout << "Validation" << std::endl;
				val_section=0;
				value++;
				emit sendNewValue(value);
			}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			///[PREDICTION]
			else if (pred_section == 1)
			{

				if(line.startsWith("model_file"))
				{
					tmp_result = line.split("=");
					pred_model_input = tmp_result[tmp_result.size()-1].trimmed();
					continue;
				}
				else if(line.startsWith("output"))
				{
					tmp_result = line.split("=");
					pred_output = tmp_result[tmp_result.size()-1].trimmed();
					continue;
				}
				else if(line.startsWith("data_file")) 
				{
					tmp_result = line.split("=");
					pred_data = tmp_result[tmp_result.size()-1].trimmed();
					continue;
				}
				else if(line.startsWith("print_excepted")) 
				{
					tmp_result = line.split("=");
					if (tmp_result[tmp_result.size()-1] == "1")
					{
						print_excepted = true;
					}
					else if (tmp_result[tmp_result.size()-1] == "0")
					{
						print_excepted = false;
					}
					continue;
				}
				else if(line.startsWith("x_coordinate"))
				{
					tmp_result = line.split("=");
					bool ok;
					
					x = tmp_result[tmp_result.size()-1].toFloat(&ok);
					continue;
				}
	
				else if(line.startsWith("y_coordinate"))
				{
					tmp_result = line.split("=");
					bool ok;
					y = tmp_result[tmp_result.size()-1].toFloat(&ok);
				}
				else
				{
					continue;
				}

				if(pred_model_input=="")
				{
					QMessageBox::critical(this,"Error","Model file must be specified within config-file!");
					break;
				}
				if(pred_output=="")
				{
					QMessageBox::critical(this,"Error","Output file must be specified within config-file!");
					break;
				}

				if(pred_data=="")
				{
					QMessageBox::critical(this,"Error","'data file must be specified within config-file!");
					break;
				}

				/// search model in hashmap
				DataItem* d_item = hash.value(pred_model_input);
				ModelItem* model_item = qgraphicsitem_cast<ModelItem*>(d_item);
	
				/// search input in hashmap
				d_item = hash.value(pred_data);
				InputDataItem* i_item = qgraphicsitem_cast<InputDataItem*>(d_item);
			
				PredictionItem* pred_item = new PredictionItem(i_item, model_item, view_);

				view_scene_.addItem(pred_item);
				pred_item->setPos(x,y);

				Edge* edge = new Edge(model_item, pred_item);
				view_scene_.addItem(edge);

				Edge* edge2 = new Edge(i_item, model_item);
				view_scene_.addItem(edge2);

				DottedEdge* dedge = new DottedEdge(i_item, pred_item);
				view_scene_.addItem(dedge);

				addPredictionToPipeline(pred_item);

				std::cout << "Prediction" << std::endl;
				pred_section=0;
				value++;
				emit sendNewValue(value);
			}
		
		}//while (!file.atEnd()) 
		file.close();
	}
	catch(BALL::Exception::GeneralException e)
	{
		QMessageBox::warning(this,"Error",e.getMessage());
	}

}


void MainWindow::setLastUsedPath(String path)
{
	last_path_ = path;	
}


void MainWindow::exportPipeline(QString filename, bool ext)
{
	int maximum = sdf_input_pipeline_.size() + csv_input_pipeline_.size() + model_pipeline_.size() + val_pipeline_.size() + prediction_pipeline_.size() + disconnected_items_.size(); //all items - fs-items (model items automatically created by feature selection are not exported)

	if (maximum == 0)
	{
		return;
	}

	int value = 0;
	progress_bar_->setMaximum(maximum);

	QFile file(filename);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		return;
	}

	QTextStream out(&file);

	int model_counter = 0;
	int val_counter = 0;
	int prediction_counter = 0;

	QString name;
	
	String f = filename.toStdString();
	String directory = f.substr(0,f.find_last_of("/")+1);
			
	if (ext)
	{
		QString num;
		out << "num of items = " << num.setNum(maximum) << "\n";
	}

	///Input Items
	for (QSet<SDFInputDataItem*>::Iterator it = sdf_input_pipeline_.begin(); it != sdf_input_pipeline_.end(); it++)
	{
		SDFInputDataItem* item = (*it);
		QString activity_string;
		QString tmp;
		SortedList<int> activities = item->activityValues();
		activities.front();
		while(activities.hasNext())
		{
			int a = activities.next();
			activity_string += " "+ tmp.setNum(a);
		}

		item->setSavedAs(item->name()+".in");

		out << "[InputReader]" << "\n";
		out << "sd_file = "<< item->filename() << "\n";
		out << "read_sd_descriptors = "<< 1 << "\n";
		out << "activity_IDs = "<< activity_string << "\n";
		out << "center_data = "<< item->centerData() << "\n";
		out << "center_response = "<< item->centerY() << "\n";
		out << "output = " << item->savedAs()  << "\n";

		if (ext)
		{
			out << item->writeConfiguration();
		}
		out << "\n";

		value++;
		emit sendNewValue(value);
	}

	///Input Items
	for (QSet<CSVInputDataItem*>::Iterator it = csv_input_pipeline_.begin(); it != csv_input_pipeline_.end(); it++)
	{
		value++;
		emit sendNewValue(value);
	}

	///Model Items
	for (QSet<ModelItem*>::Iterator it = model_pipeline_.begin(); it != model_pipeline_.end(); it++)
	{
		ModelItem* item = (*it); 
		
		// generate file-names for *all* models
		item->setSavedAs(item->name() + name.setNum(model_counter) + ".mod");
		model_counter++;

		// do not write a config-file section for model that are created by feature selection.
		// --> FeatureSelector will create these models; no need to run ModelCreator for these ones!
		if (item->saveAttribute())
		{
			QString parameter_string;
			QString tmp;
			std::vector<double> parameters = item->model_parameters;
			for (unsigned int i = 0; i < parameters.size(); i++)
			{
				parameter_string += " "+ tmp.setNum(parameters[i]);
			}
	
			bool hasKernel = item->getRegistryEntry()->kernel;
	
			out << "[ModelCreator]" << "\n";
			out << "data_file = "<< item->inputDataItem()->savedAs() << "\n";
			out << "model_no = "<< reg_->getModelNo(item->getRegistryEntry()->name_abreviation) << "\n";
			out << "model_parameters = "<< parameter_string << "\n";
	
			if (hasKernel)
			{
				out << "kernel_type = "<< item->kernel_function_type <<"\n";
	
				if (item->kernel_function_type != 4)
				{
					out << "kernel_par1 = "<< item->kernel_parameter1 << "\n";
					if (item->kernel_function_type == 3)
					{
						out << "kernel_par2 = " << item->kernel_parameter2 << "\n";
					}
				}
				out << "grid_search_steps = "<< item->grid_search_steps << "\n";
				out << "grid_search_stepwidth = "<< item->grid_search_stepwidth <<"\n";
				out << "grid_search_recursions = "<< item->grid_search_recursions << "\n";
			}
	
			out << "optimize_model_parameters = "<< item->optimize_model_parameters << "\n";
	
			if (item->optimize_model_parameters)
			{
				out << "k_fold = "<< item->k_fold <<  "\n";
			}
			out << "output = "<< item->savedAs() << "\n";

			if (ext)
			{
				out << item->writeConfiguration();
			}
			out << "\n";

			value++;
			emit sendNewValue(value);

		}
	}
	
	/// save models to files
	saveModels(directory);
	
			
	///Feature Selection Items
	for (QSet<FeatureSelectionItem*>::Iterator it = fs_pipeline_.begin(); it != fs_pipeline_.end(); it++)
	{
		FeatureSelectionItem* item = *it;
		//fs_counter++;
		model_counter++;

		//item->setSavedAs(name.setNum(fs_counter) + ".fs");

		///take the part of the model's name before the " "
		QList<QString> name_strings = item->modelItem()->name().split(" ");
		item->modelItem()->setSavedAs(name_strings[0] + name.setNum(model_counter) + ".mod");

		out << "[FeatureSelector]" << "\n";
		out << "model_file = "<< item->inputModelItem()->savedAs() << "\n";
		out << "data_file = "<< item->inputModelItem()->inputDataItem()->savedAs() << "\n";
		int s = item->getValidationStatistic();
		String stat = item->modelItem()->getRegistryEntry()->getStatName(s);
		if(item->getType()>0)
		{
			out<< "classification_statistic = "<<stat.c_str()<<endl;
			out << "k_fold = "<< item->k() <<  "\n";
			out << "feature_selection_type = "<< item->getType() <<  "\n";
			out << "output = " << item->modelItem()->savedAs() << "\n";
		}
		else
		{
			out<<"remove_correlated_features = 1"<<endl;
			out<<"cor_threshold = "<<item->getCorThreshold()<<endl;
		}

		if (ext)
		{
			out << "optimize_parameters = " << item->opt() << "\n";
			out << item->writeConfiguration();
		}
		out << "\n";

		value++;
		emit sendNewValue(value);
	}	
		
	///Validation Items
	for (QSet<ValidationItem*>::Iterator it = val_pipeline_.begin(); it != val_pipeline_.end(); it++)
	{
		ValidationItem* item = (*it); 
		val_counter++;

		item->setSavedAs(name.setNum(val_counter)+".val");

		out << "[Validator]" << "\n";
		out << "model_file = "<< item->modelItem()->savedAs() << "\n";
		out << "data_file = "<< item->modelItem()->inputDataItem()->savedAs() << "\n";
		int s = item->getValidationStatistic();
		String stat = item->modelItem()->getRegistryEntry()->getStatName(s);
		out<< "classification_statistic = "<<stat.c_str()<<endl;
		out << "validation_data_file = "<< "\n";
		out << "k_fold = "<< item->k() <<  "\n";
		out << "bootstrap_samples = "<< item->numOfSamples() << "\n";
		out << "no_of_permutation_tests = " <<  item->numOfRuns() << "\n";
		out << "output = " << item->savedAs() << "\n";

		if (ext)
		{
			out << "validation_type = " << item->getValidationType() << "\n";
			out << item->writeConfiguration(); 
			
		}
		out << "\n";

		value++;
		emit sendNewValue(value);
	}

	///Prediction Items
	for (QSet<PredictionItem*>::Iterator it = prediction_pipeline_.begin(); it != prediction_pipeline_.end(); it++)
	{
		PredictionItem* item = (*it); 
		prediction_counter++;
	
		item->setSavedAs(name.setNum(prediction_counter) + ".pred");
	
		out << "[Predictor]" << "\n";
		out << "model_file = "<< item->modelItem()->savedAs() << "\n";
		out << "data_file = "<< item->inputDataItem()->savedAs() << "\n";
		out << "print_excepted = "<< 1 << "\n";
		out << "output = " << filename + "_"+item->savedAs() << "\n";

		if (ext)
		{
			out << item->writeConfiguration();
		}
		out << "\n";

		value++;
		emit sendNewValue(value);
	}
	file.close();
	progress_bar_->reset();
}
