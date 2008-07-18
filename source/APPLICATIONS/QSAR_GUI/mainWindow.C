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
#include <QtGui/QPrinter>
#include <QtGui/QPrintDialog>

#include <sstream>
#include <map>

using namespace BALL::QSAR;
using namespace BALL::QSAR::Exception;
using namespace BALL::VIEW;
using namespace BALL::VIEW::Exception;
using namespace BALL::Exception;

///set up they main window
 MainWindow::MainWindow()
 {
	///set up the main scenes & views
	view_ = new DataItemView(&view_scene_,this);
	view_->name = "view";
	view_->mapToScene(0,0);
	view_->setDragMode(QGraphicsView::RubberBandDrag);

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
	settings.submit_prefix = "qsub -cwd";
	settings.tools_path="";
	settings.send_email=0;
	settings.email_address="";
	if(in) 
	{
		in>>settings.input_data_path;
		if(in) in>>settings.config_path;
		if(in) in>>settings.size_x;
		if(in) in>>settings.size_y;
		if(in) in>>settings.pos_x;
		if(in) in>>settings.pos_y;
		string tmp;
		getline(in,tmp);  // read the rest of the line
		if(in) getline(in,tmp);
		if(tmp!="") settings.submit_prefix=tmp;
		if(in) getline(in,tmp);
		if(tmp!="") settings.tools_path=tmp;
		if(in) in>>settings.send_email;
		getline(in,tmp);  // read the rest of the line
		if(in) getline(in,tmp);
		if(tmp!="") settings.email_address=tmp;
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
	delete model_list_;
	delete fs_list_;
	delete reg_;
	delete file_browser_;
	delete fileMenu_;
	delete editMenu_;
	delete helpMenu_;
	delete windowMenu_;	
	delete fileToolBar_;
	
	clearDesktop();
	delete view_;
	
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
	output<<settings.submit_prefix<<endl;
	output<<settings.tools_path<<endl;
	output<<settings.send_email<<endl;
	output<<settings.email_address<<endl;
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
	QMessageBox::information(this, tr("About QSAR_GUI"), tr("Version 0.67\n2008-07-15"),
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
 
 
// SLOT
void MainWindow::print()
{
	 QPrinter printer(QPrinter::HighResolution);
	 QPrintDialog print_dialog(&printer,this);
	 if (print_dialog.exec() == QDialog::Accepted) 
	 {
		 QPainter painter(&printer);
		 view_scene_.render(&painter);
	 }
}

 
// SLOT
void MainWindow::printToFile()
{
	 QString file = QFileDialog::getSaveFileName(this, tr("Save File as"),(settings.config_path+"pipeline.eps").c_str(),tr("Graphic (*.eps *.ps *.pdf)"));
	 if(file=="") return;
	 QPrinter printer(QPrinter::HighResolution);
	 QPainter painter(&printer);
	 printer.setOutputFileName(file);
	 view_scene_.render(&painter);
}


 void MainWindow::createMenus()
 {
	fileMenu_ = menuBar()->addMenu(tr("&File"));
	fileMenu_->addAction(exitAct_);

	editMenu_ = menuBar()->addMenu(tr("&Edit"));
	QAction* pref = new QAction(tr("&Preferences"), this);
	connect(pref, SIGNAL(triggered()), this, SLOT(preferencesDialog()));
	editMenu_->addAction(pref);

	windowMenu_ = menuBar()->addMenu(tr("&Windows"));

	menuBar()->addSeparator();

	helpMenu_ = menuBar()->addMenu(tr("&Help"));
	helpMenu_->addAction(aboutAct_);
 }
 
 
 // SLOT
 void MainWindow::preferencesDialog()
 {
	QDialog dialog;
	QVBoxLayout main_layout;
	
	QHBoxLayout h_layout;
	QLabel label("Queue submitting command");
	QLineEdit edit;
	edit.setText(settings.submit_prefix.c_str());
	h_layout.addWidget(&label);
	h_layout.addWidget(&edit);
	main_layout.addLayout(&h_layout);
	
	QHBoxLayout h2_layout;
	QLabel label2("Path to QSARPipelinePackage");
	QLineEdit edit2;
	edit2.setText(settings.tools_path.c_str());
	h2_layout.addWidget(&label2);
	h2_layout.addWidget(&edit2);
	main_layout.addLayout(&h2_layout);
	
	QCheckBox checkbox("send email when job is finished?");
	checkbox.setChecked(settings.send_email);
	main_layout.addWidget(&checkbox);
	
	QHBoxLayout h3_layout;
	QLabel label3("email address");
	QLineEdit edit3;
	edit3.setText(settings.email_address.c_str());
	h3_layout.addWidget(&label3);
	h3_layout.addWidget(&edit3);
	main_layout.addLayout(&h3_layout);
	
	QDialogButtonBox buttons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,Qt::Horizontal);
	main_layout.addWidget(&buttons);
	connect(&buttons, SIGNAL(accepted()), &dialog, SLOT(accept()));
	connect(&buttons, SIGNAL(rejected()), &dialog, SLOT(reject()));
	
	dialog.setLayout(&main_layout);
	 
	if(dialog.exec()) // if user clicks "ok"
	{
		settings.submit_prefix = edit.text().toStdString();
		settings.tools_path = edit2.text().toStdString();
		settings.send_email= checkbox.isChecked();
		settings.email_address = edit3.text().toStdString();
	}	 
 } 
 

/*
function for setting up the tool bars
*/
 void MainWindow::createToolBars()
 {	
	fileToolBar_ = addToolBar(tr("File"));
	fileToolBar_->addAction(exitAct_);
	fileToolBar_->addSeparator();
	fileToolBar_->addAction(clearAct_);
	fileToolBar_->addAction(delAct_);
	fileToolBar_->addSeparator();
	fileToolBar_->addAction(exportAct_);
	fileToolBar_->addAction(restoreAct_);
	QAction* print = new QAction(QIcon("./images/printer1.png"),"Print",this);
	fileToolBar_->addAction(print);
	QAction* submit_action = new QAction(QIcon("./images/cluster.png"),"Submit job",this);
	fileToolBar_->addSeparator();
	fileToolBar_->addAction(executeAct_);
	connect(print, SIGNAL(triggered()), this, SLOT(print()));
	

	fileToolBar_->addAction(submit_action);
	connect(submit_action, SIGNAL(triggered()), this, SLOT(submit()));	
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
	QDockWidget* filedock = new QDockWidget(tr("Source Filebrowser"), this);
	filedock->setAllowedAreas(Qt::LeftDockWidgetArea);
	filedock->setWidget(file_browser_);
	addDockWidget(Qt::LeftDockWidgetArea, filedock);
	windowMenu_->addAction(filedock->toggleViewAction());
	model_list_->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	
	
	///create dock widget for listing all available models
	uint row_height=65; uint col_width=65;
	uint x_offset = 20; uint y_offset = 50;
	uint x=x_offset; uint y=y_offset;
	
	QPen pen; pen.setStyle(Qt::SolidLine);
	pen.setWidth(1); pen.setColor(QColor(220,220,220));
	
	uint y0=y_offset;
	for(uint a=0; a<3; a++)
	{
		uint no_models=0;
		QGraphicsTextItem* text0 = new QGraphicsTextItem;
		if(a==0) text0->setPlainText("Linear regression models");
		else if(a==1) text0->setPlainText("Nonlinear regression models");
		else if(a==2) text0->setPlainText("Classification models");
		QGraphicsRectItem* rect0 = new QGraphicsRectItem(0,0,text0->boundingRect().width()+20,text0->boundingRect().height(),text0);
		model_list_scene_.addItem(text0);
		rect0->setPen(pen);
		text0->setPos(x_offset,y-30);
		for(uint i=0; i<reg_->registered_models.size();i++)
		{ 
			if(a==0 && (reg_->registered_models[i].kernel || !reg_->registered_models[i].regression)) continue; // create only lin. model 
			else if(a==1 && (!reg_->registered_models[i].kernel ||  !reg_->registered_models[i].regression)) continue; // create only kernel models
			else if(a==2 && (reg_->registered_models[i].regression)) continue; // create only classification models
			
			ModelItem* item = new ModelItem(&reg_->registered_models[i], model_list_);
			model_list_scene_.addItem(item);
			uint col=no_models%3;
			uint row=no_models/3;
			x=col*col_width+x_offset;
			y=row*row_height+y0;
			item->setPos(x,y);
			no_models++;
		}
		y+=2*y_offset;
		y0=y;
	}
	
// 	int j=0;
// 	for(uint i=0; i<reg_->registered_models.size();i++)
// 	{ 
// 		ModelItem* item = new ModelItem(&reg_->registered_models[i], model_list_);
// 		model_list_scene_.addItem(item);
// 		if (reg_->registered_models[i].kernel)
// 		{
// 			j++;
// 			item->setPos(120,70*(j-1)+20);
// 		}
// 		else
// 		{
// 			item->setPos(20,70*(i-j)+20);
// 		}
// 	}

	QDockWidget* modeldock = new QDockWidget(tr("Models"), this);
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

	///create dock widget for model validation
	for(uint i=1; i<7;i++)
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
		for (QList<QGraphicsItem*>::Iterator it = items.begin(); it != items.end(); ++it)
		{	
			DataItem* item = static_cast<DataItem*>(*it);
			if (disconnected_items_.find(item)!=disconnected_items_.end())
			{
				delete item;
				continue;
			}

			SDFInputDataItem* sdfitem = static_cast<SDFInputDataItem*>(*it);
			if (sdf_input_pipeline_.find(sdfitem)!=sdf_input_pipeline_.end())
			{
				delete sdfitem;
				continue;
			}

			CSVInputDataItem* csvitem = static_cast<CSVInputDataItem*>(*it);
			if (csv_input_pipeline_.find(csvitem)!=csv_input_pipeline_.end())
			{
				delete csvitem;
				continue;
			}

			ModelItem* mitem = static_cast<ModelItem*>(*it);
			if (model_pipeline_.find(mitem)!=model_pipeline_.end())
			{
				delete mitem;
				continue;
			}

			FeatureSelectionItem* fsitem = static_cast<FeatureSelectionItem*>(*it);
			if (fs_pipeline_.find(fsitem)!=fs_pipeline_.end())
			{
				delete fsitem;
				continue;
			}

			ValidationItem* valitem = static_cast<ValidationItem*>(*it);
			if (val_pipeline_.find(valitem)!=val_pipeline_.end())
			{
				delete valitem;
				continue;
			}

			PredictionItem* preditem = static_cast<PredictionItem*>(*it);
			if (prediction_pipeline_.find(preditem)!=prediction_pipeline_.end())
			{
				delete preditem;
				continue;
			}
			
			PartitioningItem* partitem = static_cast<PartitioningItem*>(*it);
			if (partitioning_pipeline_.find(partitem)!=partitioning_pipeline_.end())
			{
				delete partitem;
				continue;
			}
			
			InputPartitionItem* ip_item = static_cast<InputPartitionItem*>(*it);
			if (partition_pipeline_.find(ip_item)!=partition_pipeline_.end())
			{
				delete ip_item;
				continue;
			}
		}
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
	Pipeline<SDFInputDataItem*> sdf_input_pipeline_copy = sdf_input_pipeline_;
	for (Pipeline<SDFInputDataItem*>::iterator it = sdf_input_pipeline_copy.begin(); it != sdf_input_pipeline_copy.end(); ++it)
	{
		delete *it;
	}
 	sdf_input_pipeline_.clear();

	Pipeline<CSVInputDataItem*> csv_input_pipeline_copy = csv_input_pipeline_;
	for (Pipeline<CSVInputDataItem*>::iterator it = csv_input_pipeline_copy.begin(); it != csv_input_pipeline_copy.end(); ++it)
	{
		delete *it;
	}
 	csv_input_pipeline_.clear();
	
	for (Pipeline<DataItem*>::iterator it = disconnected_items_.begin(); it != disconnected_items_.end(); ++it)
	{
		delete *it;
	}
	disconnected_items_.clear();
	
	for (Pipeline<InputPartitionItem*>::iterator it = partition_pipeline_.begin(); it != partition_pipeline_.end(); ++it)
	{
		delete *it;
	}
	partition_pipeline_.clear();
	
	for (Pipeline<PartitioningItem*>::iterator it = partitioning_pipeline_.begin(); it != partitioning_pipeline_.end(); ++it)
	{
		delete *it;
	}
	partitioning_pipeline_.clear();
	
	view_scene_.update();
}

// SLOT
void MainWindow::restoreDesktop()
{
	QString filename = QFileDialog::getOpenFileName(this, tr("Open File"),settings.config_path.c_str(),tr("Pipeline (*.tar.gz *.conf)"));
	if(filename=="") return;
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
BALL::String MainWindow::exportPipeline()
{
	QString filename = QFileDialog::getSaveFileName(this, tr("Save File as"),(settings.config_path+"config.tar.gz").c_str(),tr("Pipeline (*.tar.gz *.conf)"));
	String s = filename.toStdString();
	if(filename=="") return s;
	exportPipeline(filename);
	settings.config_path = s.substr(0,s.find_last_of("/")+1);
	return s;
}


void MainWindow::saveItemsToFiles(String directory, String archive, String configfile)
{
	String files;
	try
	{
		for (Pipeline<SDFInputDataItem*>::iterator it = sdf_input_pipeline_.begin(); it != sdf_input_pipeline_.end(); it++)
		{
			// if input has not been read, there is nothing to be saved
			if(!(*it)->isDone()) continue;
			
			QSARData* data = (*it)->data();
			String f1 = (*it)->savedAs().toStdString();
			if(f1=="")
			{
				throw GeneralException(__FILE__,__LINE__,"SDF data saving error ", "SDF Input must be assigned a file to be saved to!");
			}
			String file = directory+f1;
			files+=f1+" ";
			data->saveToFile(file);
		}
		for (Pipeline<CSVInputDataItem*>::iterator it = csv_input_pipeline_.begin(); it != csv_input_pipeline_.end(); it++)
		{
			// if input has not been read, there is nothing to be saved
			if(!(*it)->isDone() || (*it)->append()) continue;
			
			QSARData* data= (*it)->data();
			String f1 = (*it)->savedAs().toStdString();
			if(f1=="")
			{
				throw GeneralException(__FILE__,__LINE__,"CSV saving error ", "CSV Item must be assigned a file to be saved to!");
			}
			String file = directory+f1;
			files+=f1+" ";
			data->saveToFile(file);
		}
		for (Pipeline<InputPartitionItem*>::iterator it = partition_pipeline_.begin(); it != partition_pipeline_.end(); it++)
		{
			// if input has not been read, there is nothing to be saved
			if(!(*it)->isDone() || (*it)->append()) continue;
			
			QSARData* data= (*it)->data();
			String f1 = (*it)->savedAs().toStdString();
			if(f1=="")
			{
				throw GeneralException(__FILE__,__LINE__,"<Input-partition saving error ", "Item must be assigned a file to be saved to!");
			}
			String file = directory+f1;
			files+=f1+" ";
			data->saveToFile(file);
		}
		for (Pipeline<ModelItem*>::iterator it = model_pipeline_.begin(); it != model_pipeline_.end(); it++)
		{
			// if model has not yet been trained, there is nothing to be saved
			if(!(*it)->isDone()) continue;
			
			String f1 = (*it)->savedAs().toStdString();
			if(f1=="")
			{
				throw GeneralException(__FILE__,__LINE__,"Model saving error ", "Model must be assigned a file to be saved to!");
			}
			String file = directory+f1;
			files+=f1+" ";
			(*it)->saveToFile(file);
		}
	}
	catch(GeneralException e)
	{	
		QMessageBox::warning(this, tr("Error"),e.getMessage());
	}
	
	if(archive!="")
	{
		int index=configfile.find_last_of("/");
		if(index!=string::npos)
		{
			configfile=configfile.substr(index+1); // no path; filename only!
		}		
		String call = "cd "+directory+"; tar -czf "+archive+" "+files+" "+configfile;
		system(call.c_str());	// compress output files
		call = "cd "+directory+"; rm -f "+files+" "+configfile;
		system(call.c_str());	// delete uncompressed files
	}
}


void MainWindow::loadItemsFromFiles(String directory)
{
	try
	{
		for (Pipeline<SDFInputDataItem*>::iterator it = sdf_input_pipeline_.begin(); it != sdf_input_pipeline_.end(); it++)
		{
			String filename=directory+(*it)->savedAs().toStdString();
			ifstream input(filename.c_str());
			if(input) // read only existing input data files
			{
				input.close();
				(*it)->loadFromFile(filename);
			}
		}
		for (Pipeline<CSVInputDataItem*>::iterator it = csv_input_pipeline_.begin(); it != csv_input_pipeline_.end(); it++)
		{
			if((*it)->append()) 
			{
				(*it)->setDone(1);
				continue;
			}
			
			String filename=directory+(*it)->savedAs().toStdString();
			ifstream input(filename.c_str());
			if(input) // read only existing input data files
			{
				input.close();
				(*it)->loadFromFile(filename);
			}
		}
		for (Pipeline<InputPartitionItem*>::iterator it = partition_pipeline_.begin(); it != partition_pipeline_.end(); it++)
		{
			String filename=directory+(*it)->savedAs().toStdString();
			ifstream input(filename.c_str());
			if(input) // read only existing input data files
			{
				input.close();
				(*it)->loadFromFile(filename);
			}			
		}
		for (Pipeline<ModelItem*>::iterator it = model_pipeline_.begin(); it != model_pipeline_.end(); it++)
		{
			String filename=directory+(*it)->savedAs().toStdString();
			ifstream input(filename.c_str());
			if(input) // read only existing models
			{
				//cout<<"model-file= "<<directory+(*it)->savedAs().toStdString()<<endl;
				input.close();
				(*it)->loadFromFile(filename);
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

Pipeline<ModelItem*> MainWindow::getModelPipeline()
{
	return model_pipeline_;
}

void MainWindow::executePipeline()
{
	int maximum = sdf_input_pipeline_.size() + csv_input_pipeline_.size() + model_pipeline_.size() + fs_pipeline_.size() + val_pipeline_.size() + prediction_pipeline_.size();
	
	if (maximum == 0)
	{
		QMessageBox::about(this,"No pipeline","There is no pipeline to be excuted yet!");
		return;
	}

	int value = 0;
	progress_bar_->setMaximum(maximum);
	
	bool done=0;
	
	///read in the input files
	for (Pipeline<SDFInputDataItem*>::iterator it = sdf_input_pipeline_.begin(); it != sdf_input_pipeline_.end(); it++)
	{
		try
		{
			bool b=(*it)->execute();
			if(!done) done=b;	
		}
		catch(BALL::Exception::GeneralException e)
		{
			QMessageBox::warning(this,"Error",e.getMessage());
		}

		value++;
		emit sendNewValue(value);
	}

	for (Pipeline<CSVInputDataItem*>::iterator it = csv_input_pipeline_.begin(); it != csv_input_pipeline_.end(); it++)
	{
		try
		{
			bool b=(*it)->execute();
			if(!done) done=b;
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
	
	/// partition all input data sets (if desired)
	for (Pipeline<PartitioningItem*>::iterator it = partitioning_pipeline_.begin(); it != partitioning_pipeline_.end(); it++)
	{
		try
		{
			bool b=(*it)->execute();
			if(!done) done=b;
		}
		catch(BALL::Exception::GeneralException e)
		{
			QMessageBox::warning(this,"Error",e.getMessage());
		}

		value++;
		emit sendNewValue(value);		
	}
	if (!partitioning_pipeline_.empty())
	{
		statusBar()->showMessage(tr("Partitioning of input data done"));
	}

	///train all models and set their saved as names
	int model_counter=0;
	QString name;
	for (Pipeline<ModelItem*>::iterator it = model_pipeline_.begin(); it != model_pipeline_.end(); it++)
	{
		(*it)->setSavedAs((*it)->name() + name.setNum(model_counter) + ".mod");
		model_counter++;
		try
		{
			bool b=(*it)->execute();
			if(!done) done=b;
		}
		catch (BALL::Exception::GeneralException e)
		{	
			QString error_string = e.getMessage();
			QMessageBox::warning(this,"Error",error_string);
		}
		value++;
		emit sendNewValue(value);
	}

	if (!model_pipeline_.empty())
	{
		statusBar()->showMessage(tr("All models were trained"));
	}

	///feature selection
	for (Pipeline<FeatureSelectionItem*>::iterator it = fs_pipeline_.begin(); it != fs_pipeline_.end(); it++)
	{
		try
		{
			QString num;
			bool b=(*it)->execute();
			if(!done) done=b;
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
		statusBar()->showMessage(tr("Feature Selections done"));
	}
		


	///activity prediction
	for (Pipeline<PredictionItem*>::iterator it = prediction_pipeline_.begin(); it != prediction_pipeline_.end(); it++)
	{
		try
		{
			bool b=(*it)->execute();
			if(!done) done=b;
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
		statusBar()->showMessage(tr("Predictions done"));
	}

	///validation
	for (Pipeline<ValidationItem*>::iterator it = val_pipeline_.begin(); it != val_pipeline_.end(); it++)
	{
		//try
		{
			bool b=(*it)->execute();
			if(!done) done=b;
		}
// 		catch(BALL::Exception::GeneralException e)
// 		{	
// 			QMessageBox::about(this,"Error",e.getMessage());
// 		}
		value++;
		emit sendNewValue(value);	
	}

	progress_bar_->reset();
	if(!done)
	{
		QMessageBox a;
		a.about(this,"Information","Pipeline has not changed,\nso there was nothing to be done!");
	}
	
	view_scene_.update();
	view_->update();
}


void MainWindow::addDisconnectedItem(DataItem* item)
{
	if (item != NULL)
	{
		disconnected_items_.insert(item);
	}
}

Pipeline<DataItem*> MainWindow::disconnectedItems()
{
	return disconnected_items_;
}



///
/// IMPORT & EXPORT
///

void MainWindow::readPartitionPositions(list<pair<double,double> >* item_positions)
{
	if(item_positions==NULL) return;

	for (Pipeline<InputPartitionItem*>::iterator it = partition_pipeline_.begin(); it != partition_pipeline_.end() && item_positions->size()>0; it++)
	{
		pair<double,double> pos = item_positions->front();
		(*it)->setPos(pos.first,pos.second);
		item_positions->pop_front();
		(*it)->adjustEdges();
	}
}


void MainWindow::restoreDesktop(QString filename)
{
	String configfile = filename.toStdString();
	uint s = configfile.find_last_of("/");
	String directory = configfile.substr(0,s+1); // name of config-file folder
	bool archive = 0;
	
	if(configfile.size()>7 && configfile.substr(configfile.size()-7)==".tar.gz")
	{
		archive = 1;
		String call = "cd "+directory+"; tar -xzvf "+configfile+" > archive_contents.tmp";
		system(call.c_str());  // extrace files from archive
		configfile = configfile.substr(0,configfile.size()-7)+".conf"; //config-file within archive will always have extension ".conf"
	}	
	
	ifstream file(configfile.c_str());
	if(!file)
	{
		cout<<"configfile file '"<<configfile<<"' does not exist!!"<<endl;
		return;
	}
	
	try
	{
		bool input_section=0;
		bool model_section=0;
		bool fs_section=0;
		bool val_section=0;	
		bool pred_section=0;
		bool partitioner_section=0;
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
		file.open(configfile.c_str());	
		bool first_model=1;

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
				if(partitioner_section) input_reader.readConfigSection(section,filenames_map,&item_positions);
				if(model_section) 
				{
					if(first_model)
					{
						readPartitionPositions(&item_positions);
						first_model=0;
					}
					new ModelItem(section,filenames_map,&item_positions,view_);
				}
 				if(fs_section) new FeatureSelectionItem(section,filenames_map,&item_positions,view_);
 				if(val_section) new ValidationItem(section,filenames_map,&item_positions,view_);
				if(pred_section) new PredictionItem(section,filenames_map,&item_positions,view_);
				
				input_section=0;model_section=0;fs_section=0;
				val_section=0;pred_section=0;partitioner_section=0;
				if(line.hasPrefix("[InputReader]")) input_section=1;
				else if(line.hasPrefix("[InputPartitioner]")) partitioner_section=1;
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
		if(partitioner_section) input_reader.readConfigSection(section,filenames_map,&item_positions);
 		if(model_section) 
		{
			if(first_model)
			{
				readPartitionPositions(&item_positions);
				first_model=0;
			}
			new ModelItem(section,filenames_map,&item_positions,view_);
		}
 		if(fs_section) new FeatureSelectionItem(section,filenames_map,&item_positions,view_);
 		if(val_section) new ValidationItem(section,filenames_map,&item_positions,view_);
		if(pred_section) new PredictionItem(section,filenames_map,&item_positions,view_);
		
		file.close();
	}
	catch(BALL::Exception::GeneralException e)
	{
		QMessageBox::warning(this,e.getName(),e.getMessage());
	}
	
	view_scene_.update();
	view_->update();
	
	/// read all items if respec. files exist in the folder of the config-file	
	loadItemsFromFiles(directory);
	
	if(archive)
	{	
		string file = directory+"archive_contents.tmp";
		ifstream in(file.c_str());
		String files="";
		while(in)
		{
			String tmp;
			in >> tmp;
			files += tmp+" ";
		}
		String call = "cd "+directory+"; "+"rm -f "+files+" archive_contents.tmp";
		system(call.c_str());   // delete uncompressed files		
	}
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

	InputDataItemIO input_writer(view_);
	int counter = 0;

	QString name;
	String configfile = filename.toStdString();
	String archive="";
	if(configfile.size()>7 && configfile.substr(configfile.size()-7)==".tar.gz")
	{
		archive = configfile;
		configfile = configfile.substr(0,configfile.size()-7)+".conf";
	}
	uint d = configfile.find_last_of(".");
	uint s = configfile.find_last_of("/");
	String file_prefix = configfile.substr(s+1,d-s-1)+"_"; // name of config-file as prefix for output-files
	String directory = configfile.substr(0,s+1); // name of folder
	
	ofstream out(configfile.c_str());
	ostringstream positions;
	positions<<"[ItemPositions]"<<endl;
	
	/// SDFInputItems
	for (Pipeline<SDFInputDataItem*>::iterator it = sdf_input_pipeline_.begin(); it != sdf_input_pipeline_.end(); it++)
	{
		SDFInputDataItem* item = (*it);
		item->setSavedAs(file_prefix.c_str()+item->name()+".dat");
		input_writer.writeConfigSection(item,out);
		positions<<item->x()<<"  "<<item->y()<<endl;
		value++;
		emit sendNewValue(value);
	}

	///CSVInputItems
	for (Pipeline<CSVInputDataItem*>::iterator it = csv_input_pipeline_.begin(); it != csv_input_pipeline_.end(); it++)
	{
		CSVInputDataItem* item = (*it);
		item->setSavedAs(file_prefix.c_str()+item->name()+".dat");
		input_writer.writeConfigSection(item,out);
		positions<<item->x()<<"  "<<item->y()<<endl;
		value++;
		emit sendNewValue(value);
	}
	
	///PartitioningItems
	for (Pipeline<PartitioningItem*>::iterator it = partitioning_pipeline_.begin(); it != partitioning_pipeline_.end(); it++)
	{
		PartitioningItem* item = (*it);
		input_writer.writeConfigSection(item,out);
		positions<<item->x()<<"  "<<item->y()<<endl;
	}

	///InputPartitionItems
	for (Pipeline<InputPartitionItem*>::iterator it = partition_pipeline_.begin(); it != partition_pipeline_.end(); it++)
	{
		InputPartitionItem* item = (*it);
		String n = file_prefix+item->getOutputFilename();
		item->setSavedAs(n.c_str());
		positions<<item->x()<<"  "<<item->y()<<endl;
	}
	
	///Model Items
	counter=0;
	for (Pipeline<ModelItem*>::iterator it = model_pipeline_.begin(); it != model_pipeline_.end(); it++,counter++)
	{
		ModelItem* item = (*it);
		item->setSavedAs(file_prefix.c_str()+item->name() + name.setNum(counter) + ".mod");
		
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
	
	///Feature Selection Items
	counter=0;
	for (Pipeline<FeatureSelectionItem*>::iterator it = fs_pipeline_.begin(); it != fs_pipeline_.end(); it++,counter++)
	{
		FeatureSelectionItem* item = *it;
		item->writeConfigSection(out);
		positions<<item->x()<<"  "<<item->y()<<endl;
		positions<<item->modelItem()->x()<<"  "<<item->modelItem()->y()<<endl;
		value++;
		emit sendNewValue(value);
	}
		
	///Prediction Items
	counter=0;
	for (Pipeline<PredictionItem*>::iterator it = prediction_pipeline_.begin(); it != prediction_pipeline_.end(); it++,counter++)
	{
		PredictionItem* item = (*it); 
		item->setSavedAs(file_prefix.c_str()+name.setNum(counter) + ".pred");
		item->writeConfigSection(out);
		positions<<item->x()<<"  "<<item->y()<<endl;
		value++;
		emit sendNewValue(value);
	}
		
	///Validation Items
	counter=0;
	for (Pipeline<ValidationItem*>::iterator it = val_pipeline_.begin(); it != val_pipeline_.end(); it++,counter++)
	{
		ValidationItem* item = (*it); 
		item->setSavedAs(file_prefix.c_str()+name.setNum(counter)+".val");
		item->writeConfigSection(out);
		positions<<item->x()<<"  "<<item->y()<<endl;
		value++;
		emit sendNewValue(value);
	}
	
	out<<positions.str().c_str()<<endl;
	out.close();
	
	/// save item data to files
	saveItemsToFiles(directory,archive,configfile);
	
	progress_bar_->reset();
}

bool MainWindow::itemExists(DataItem* item)
{
	if(sdf_input_pipeline_.contains((SDFInputDataItem*)item)) return 1;
	if(csv_input_pipeline_.contains((CSVInputDataItem*)item)) return 1;
	if(model_pipeline_.contains((ModelItem*)item)) return 1;
	if(fs_pipeline_.contains((FeatureSelectionItem*)item)) return 1;
	if(val_pipeline_.contains((ValidationItem*)item)) return 1;
	if(prediction_pipeline_.contains((PredictionItem*)item)) return 1;
	if(disconnected_items_.contains(item)) return 1;
	if(partitioning_pipeline_.contains((PartitioningItem*)(item))) return 1;
	if(partition_pipeline_.contains((InputPartitionItem*)item)) return 1;
	
	return 0;
}

// SLOT
void MainWindow::submit()
{
	String configfile = exportPipeline();
	if(configfile!="")
	{
		submitToCluster(configfile);
	}	
}

void MainWindow::submitToCluster(String configfile)
{
	uint d = configfile.find_last_of(".");
	uint s = configfile.find_last_of("/");
	String file = configfile.substr(0,d); // name of config-file as prefix for output-files
	//cout<<"file="<<file<<endl;
	String directory = configfile.substr(0,s+1); // name of folder
	
	String script = file+".csh";
	ofstream out(script.c_str());
	
	String ir=""; String ip=""; String mc="";
	String fs=""; String pr="";
	if(settings.tools_path!="")
	{
		ir=settings.tools_path+"/";
		ip=ir; mc=ir; fs=ir; pr=ir;
	}
	ir.append("InputReader"); ip.append("InputPartitioner");
	mc.append("ModelCreator"); fs.append("FeatureSelector");
	pr.append("Predictor");
		
		
	out<<ir<<" "<<configfile<<endl;
	out<<ip<<" "<<configfile<<endl;
	out<<mc<<" "<<configfile<<endl;
	out<<fs<<" "<<configfile<<endl;
	out<<pr<<" "<<configfile<<endl;
	if(settings.send_email && settings.email_address!="")
	{
		
		out<<"echo -e \"Subject: "<<script<<" is ready!\nProcess '"<<script<<"' is ready!\nTime: `date`\n\n \" | sendmail "<<settings.email_address<<endl;
	}
		
	out.close();
	
	String call = "cd "+directory+"; ";
	call+=settings.submit_prefix+" "+script;
	system(call.c_str());
}

