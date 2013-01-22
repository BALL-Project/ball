/* TRANSLATOR BALL::QSAR

		Necessary for lupdate.
*/

/* mainWindow.C
 * 
 * Copyright (C) 2009 Marcel Schumann
 * 
 * This file is part of QuEasy -- A Toolbox for Automated QSAR Model
 * Construction and Validation.
 * QuEasy is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 * 
 * QuEasy is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#include <mainWindow.h>
#include <BALL/QSAR/exception.h>
#include <exception.h>
#include <inputDataDialog.h>
#include <CSVInputDialog.h>
#include <dottedEdge.h>
#include <inputDataItemIO.h>
#include <BALL/SYSTEM/timer.h>
#include <BALL/FORMAT/INIFile.h>
#include <BALL/VIEW/KERNEL/iconLoader.h>

#include <QtGui/QMessageBox>
#include <QtGui/QFileDialog>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QDockWidget>
#include <QtCore/QTextStream>
#include <QtGui/QPrinter>
#include <QtGui/QPrintDialog>
#include <QtGui/QShortcut>
#include <QtGui/QTextBrowser>
#include <QtGui/QApplication>

#include <sstream>
#include <map>

using namespace BALL::QSAR;
using namespace BALL::Exception;


namespace BALL
{
	namespace VIEW
	{

		///set up they main window
		MainWindow::MainWindow()
		 {
			init();
		 }
		 
		 MainWindow::MainWindow(char* executable_directory)
		 {
			 executable_directory_ = executable_directory;
			 init();
		 }

		 
		void MainWindow::init()
		{
			Path p;
			IconLoader::instance().appendIconPath(p.find("graphics/QuEasy"));
			
			fullscreen_ = 0;
			settings.main_window = this;
			 
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
			s = s+"/.QuEasyViz";
			settings.input_data_path=QDir::homePath().toStdString();
			settings.config_path=QDir::homePath().toStdString();
			settings.size_x=0; settings.size_y=0;
			settings.pos_x=0; settings.pos_y=0;
			settings.submit_prefix = "qsub -cwd";
			settings.tools_path="";
			settings.send_email=0;
			settings.email_address="";
			settings.tmp_folder="";
			settings.path_separator = BALL::FileSystem::PATH_SEPARATOR;
			settings.show_documentation = true;
			settings.descriptor_explanation_files.push_back("dragon_descriptors.txt");
			settings.readFromFile(s);
			
			if(executable_directory_!="")
			{
				executable_directory_ = executable_directory_.substr(0,executable_directory_.find_last_of(settings.path_separator));
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

			if (settings.size_x==0 || settings.size_y==0) 
				showMaximized();
			
			drag_start_time = drag_start_time.now();
			min_drag_time=0.3;
			
			String file = "QSAR" + settings.path_separator + "atomic_electron_affinities.data";
			String dir = p.find(file);
			if (dir=="")
			{
				data_directory_ = executable_directory_ + settings.path_separator + "data"+settings.path_separator;
			}
			else
			{
				data_directory_ = "QSAR"+settings.path_separator;
			}
			
			read_descriptor_explanations_=0;
			documentation_= 0;
			if (settings.show_documentation) 
				showDocumentation();
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
			file = file+"/.QuEasyViz";
			
			settings.saveToFile(file);
		}


		void MainWindow::Settings::saveToFile(String file)
		{
			INIFile out(file);
			out.appendSection("QuEasyViz");
			out.insertValue("QuEasyViz","input_data_path",input_data_path);
			out.insertValue("QuEasyViz","config_path",config_path);
			QSize s = main_window->size();
			String size = String(s.width())+" "+String(s.height());
			out.insertValue("QuEasyViz","window_size",size);
			QPoint p = main_window->pos();
			String pos = String(p.x())+"  "+String(p.y());
			out.insertValue("QuEasyViz","window_position",pos);
			out.insertValue("QuEasyViz","submit_prefix",submit_prefix);
			out.insertValue("QuEasyViz","tools_path",tools_path);
			out.insertValue("QuEasyViz","tmp_folder",tmp_folder);
			out.insertValue("QuEasyViz","send_email",send_email);
			out.insertValue("QuEasyViz","email_address",email_address);
			out.insertValue("QuEasyViz","path_separator",path_separator);
			out.insertValue("QuEasyViz","show_documentation",show_documentation);
			out.write();
		}

		void MainWindow::Settings::readFromFile(String file)
		{
			ifstream test_existence(file.c_str());
			if(!test_existence) return;
			test_existence.close();	
			
			try
			{
				INIFile ini(file);
				ini.read();
				if (ini.hasEntry("QuEasyViz","input_data_path")) input_data_path = ini.getValue("QuEasyViz","input_data_path");
				if (ini.hasEntry("QuEasyViz","config_path")) config_path = ini.getValue("QuEasyViz","config_path");
				if (ini.hasEntry("QuEasyViz","window_size")) 
				{
					String s = ini.getValue("QuEasyViz","window_size");
					size_x=s.getField(0).toInt(); size_y=s.getField(1).toInt();
					main_window->resize(size_x,size_y);
				}
				if (ini.hasEntry("QuEasyViz","window_position"))
				{
					String position = ini.getValue("QuEasyViz","window_position");
					pos_x=position.getField(0).toInt(); pos_y=position.getField(1).toInt();
					main_window->move(pos_x,pos_y);
				}
				if (ini.hasEntry("QuEasyViz","submit_prefix")) 
					submit_prefix = ini.getValue("QuEasyViz","submit_prefix");
				if (ini.hasEntry("QuEasyViz","tools_path")) 
					tools_path = ini.getValue("QuEasyViz","tools_path");
				if (ini.hasEntry("QuEasyViz","send_email")) 
					send_email = ini.getValue("QuEasyViz","email").toBool();
				if (ini.hasEntry("QuEasyViz","email_address")) 
					email_address = ini.getValue("QuEasyViz","email_address");
				if (ini.hasEntry("QuEasyViz","tmp_folder")) 
					tmp_folder = ini.getValue("QuEasyViz","tmp_folder");
				if (ini.hasEntry("QuEasyViz","path_separator")) 
					path_separator = ini.getValue("QuEasyViz","path_separator");
				if (ini.hasEntry("QuEasyViz","show_documentation")) 
					show_documentation = ini.getValue("QuEasyViz","show_documentation").toBool();
			}
			// if there are unacceptable entries in the ini-file, ignore the file
			catch (BALL::Exception::GeneralException e) 
			{
				// do nothing
		 	}
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
					if (exec==1)
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
				catch (BALL::QSAR::Exception::InvalidActivityID e)
				{
					ok = false;
					QMessageBox::about(this, tr("Error"), tr("Invalid activity ID"));
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
					QMessageBox::about(this, tr("Error"), tr("Invalid activity ID"));
				}
			}
			return input;
		}


		///create a new CSVInputItem with the given file
		CSVInputDataItem* MainWindow::createCSVInput(QSARData* data)
		{
			CSVInputDataItem* input = new CSVInputDataItem(data, view_);
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
					if (exec==1)
					{
						csvInputDialog.readNumY(); 
						ok= true;
					}
					else
					{
						throw InvalidInputDataItem(__FILE__,__LINE__);
					}
				}
				catch (BALL::QSAR::Exception::InvalidActivityID e)
				{
					ok = false;
					QMessageBox::about(this, tr("Error"), tr("Invalid activity ID"));
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
					QMessageBox::information(this, " ", 
							tr("Some class labels of this input file are not discrete values! Creation of a classification model is therefore not possible."));
					throw InvalidModelItem(__FILE__,__LINE__);	
				}
			}

			modelConfigurationDialog_ = new ModelConfigurationDialog(model, input, this);

			if (entry->parameterNames.size()==0 && !entry->kernel)
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
					QMessageBox::information(this, tr(""), tr("Invalid value"));
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
					catch (InvalidK)
					{
						ok=false;
						QMessageBox::information(this, tr(""), tr("Invalid value"));
					}
				}
			}
			else
			{
				item->setModelItem(model);
				int stat = chooseValidationStatisticDialog(model);
				if(stat>=0) item->setValidationStatistic(stat);	
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
		//	String s = "Version "+String(QUEASY_VERSION)+"\nbuild "+String(__DATE__);
			String s = "Version "+String(BALL_RELEASE_STRING)+"\nbuild "+String(__DATE__);
			QMessageBox::information(this, tr("About QuEasyViz"), tr(s.c_str()),
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
			exitAct_ = new QAction(QIcon(IconLoader::instance().getIcon("actions/exit")), tr("Close"), this);
			exitAct_->setShortcut(tr("Ctrl+Q"));
			exitAct_->setStatusTip(tr("Exit the application"));
			connect(exitAct_, SIGNAL(triggered()), this, SLOT(close()));

			aboutAct_ = new QAction(tr("&About"), this);
			aboutAct_->setStatusTip(tr("Show the application's About box"));
			connect(aboutAct_, SIGNAL(triggered()), this, SLOT(about()));

			clearAct_ = new QAction(QIcon(IconLoader::instance().getIcon("actions/clear_desktop")), tr("&Clear Desktop"), this);
			clearAct_->setShortcut(tr("Ctrl+C"));
			clearAct_->setStatusTip(tr("Clear the desktop"));
			connect(clearAct_, SIGNAL(triggered()), this, SLOT(clearDesktop()));

			delAct_ = new QAction(QIcon(IconLoader::instance().getIcon("actions/delete_item")), tr("&Delete Selection"), this);
			delAct_->setStatusTip(tr("Delete the selected Item from the pipeline"));
			delAct_->setShortcut(tr("Ctrl+D"));
			connect(delAct_, SIGNAL(triggered()), this, SLOT(deleteItem()));
			
			QShortcut* del_shortcut = new QShortcut(QKeySequence::Delete,this);
			connect(del_shortcut,SIGNAL(activated()),this,SLOT(deleteItem()));

			executeAct_ = new QAction(QIcon(IconLoader::instance().getIcon("actions/run_pipeline")),tr("&Execute Pipeline"), this);
			executeAct_->setShortcut(tr("Ctrl+E"));
			executeAct_->setStatusTip(tr("Execute the Pipeline"));
			connect(executeAct_, SIGNAL(triggered()), this, SLOT(executePipeline()));

			restoreAct_ = new QAction(QIcon(IconLoader::instance().getIcon("actions/restore_desktop")),tr("&Restore Pipeline"), this);
			restoreAct_->setShortcut(tr("Ctrl+R"));
			restoreAct_->setStatusTip(tr("Restore a Pipeline"));
			connect(restoreAct_, SIGNAL(triggered()), this, SLOT(restoreDesktop()));

			exportAct_ = new QAction(QIcon(IconLoader::instance().getIcon("actions/save")),tr("Save Pipeline"), this);
			exportAct_->setStatusTip(tr("Save the Pipeline"));
			exportAct_->setShortcut(tr("Ctrl+S"));
			connect(exportAct_, SIGNAL(triggered()), this, SLOT(exportPipeline()));
		 }
		 
		// SLOT
		void MainWindow::zoomIn()
		{
			view_->scaleView(1.41);
		}

		// SLOT
		void MainWindow::zoomOut()
		{
			view_->scaleView(0.7);
		}

		// SLOT
		void MainWindow::print()
		{
			if (checkForEmptyPipelines()) 
				return;
			
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
			if(checkForEmptyPipelines()) 
				return;
			
			QString file = QFileDialog::getSaveFileName(this,
			                  tr("Save File as"),
												(settings.config_path+"pipeline.eps").c_str(), 
												tr("Graphic") + " (*.eps *.ps *.pdf)");
			if (file=="") 
				return;
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
			pref->setShortcut(tr("Ctrl+P"));
			connect(pref, SIGNAL(triggered()), this, SLOT(preferencesDialog()));
			editMenu_->addAction(pref);

			windowMenu_ = menuBar()->addMenu(tr("&Windows"));

			menuBar()->addSeparator();

			helpMenu_ = menuBar()->addMenu(tr("&Help"));
			QAction* doc = new QAction(tr("&Documentation"), this);
			QList<QKeySequence> list;
			list.push_back(QKeySequence("Ctrl+H")); list.push_back(QKeySequence::HelpContents);
			doc->setShortcuts(list);  // F1-key or Ctrl+H
			connect(doc, SIGNAL(triggered()), this, SLOT(showDocumentation()));
			helpMenu_->addAction(doc);
			helpMenu_->addAction(aboutAct_);
		 }
		 
		 
		 // SLOT
		 void MainWindow::preferencesDialog()
		 {
			QDialog dialog;
			QVBoxLayout main_layout;
			
			QVBoxLayout* layout1 = new QVBoxLayout;
			QGroupBox general_settins(tr("General settings"));
			
			QHBoxLayout h4_layout;
			QLabel label4("tmp folder");
			QLineEdit edit4;
			edit4.setText(settings.tmp_folder.c_str());
			h4_layout.addWidget(&label4);
			h4_layout.addWidget(&edit4);
			layout1->addLayout(&h4_layout);
			
			QCheckBox doc_checkbox(tr("Show documentation at start-up?"));
			doc_checkbox.setChecked(settings.show_documentation);
			layout1->addWidget(&doc_checkbox);
			
			general_settins.setLayout(layout1);
			main_layout.addWidget(&general_settins);
			
			
			QVBoxLayout* layout2 = new QVBoxLayout;
			QGroupBox cluster_settins(tr("Cluster submit settings"));
			
			QHBoxLayout h_layout;
			QLabel label(tr("Queue submitting command"));
			QLineEdit edit;
			edit.setText(settings.submit_prefix.c_str());
			h_layout.addWidget(&label);
			h_layout.addWidget(&edit);
			layout2->addLayout(&h_layout);
			
			QHBoxLayout h2_layout;
			QLabel label2(tr("Path to QuEasyRun"));
			QLineEdit edit2;
			edit2.setText(settings.tools_path.c_str());
			h2_layout.addWidget(&label2);
			h2_layout.addWidget(&edit2);
			layout2->addLayout(&h2_layout);
			
			QCheckBox checkbox(tr("Send email when job is finished?"));
			checkbox.setChecked(settings.send_email);
			layout2->addWidget(&checkbox);
			
			QHBoxLayout h3_layout;
			QLabel label3(tr("Email address"));
			QLineEdit edit3;
			edit3.setText(settings.email_address.c_str());
			h3_layout.addWidget(&label3);
			h3_layout.addWidget(&edit3);
			layout2->addLayout(&h3_layout);
			
			cluster_settins.setLayout(layout2);
			main_layout.addWidget(&cluster_settins);
			
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
				settings.tmp_folder = edit4.text().toStdString();
				settings.show_documentation = doc_checkbox.isChecked();
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
			QAction* print = new QAction(QIcon(IconLoader::instance().getIcon("actions/printer1")),tr("Print"),this);
			fileToolBar_->addAction(print);
			
			fileToolBar_->addSeparator();

			fullscreen_action_ = new QAction(QIcon(IconLoader::instance().getIcon("actions/window_fullscreen")),"Fullscreen",this);
			fileToolBar_->addAction(fullscreen_action_);
			fullscreen_action_->setShortcut(tr("Ctrl+F"));
			connect(fullscreen_action_, SIGNAL(triggered()), this, SLOT(fullscreen()));
			QAction* zoom_in_action = new QAction(QIcon(IconLoader::instance().getIcon("actions/zoom_in")),tr("zoom in"),this);
			zoom_in_action->setShortcut(tr("+"));
			connect(zoom_in_action, SIGNAL(triggered()), this, SLOT(zoomIn()));
			fileToolBar_->addAction(zoom_in_action);
			QAction* zoom_out_action = new QAction(QIcon(IconLoader::instance().getIcon("actions/zoom_out")),tr("zoom out"),this);
			zoom_out_action->setShortcut(tr("-"));
			connect(zoom_out_action, SIGNAL(triggered()), this, SLOT(zoomOut()));
			fileToolBar_->addAction(zoom_out_action);

			fileToolBar_->addSeparator();
			
			QAction* submit_action = new QAction(QIcon(IconLoader::instance().getIcon("actions/cluster")),tr("Submit job"),this);
			fileToolBar_->addAction(executeAct_);
			connect(print, SIGNAL(triggered()), this, SLOT(print()));
			fileToolBar_->addAction(submit_action);
			connect(submit_action, SIGNAL(triggered()), this, SLOT(submit()));
		 }
		 
		 
		// SLOT 
		void MainWindow::fullscreen()
		{
			fullscreen_ = !fullscreen_;
			if(dockwidget_enabled_.size()!=dockwidgets_.size()) dockwidget_enabled_.resize(dockwidgets_.size(),0);
			
			list<bool>::iterator b_it=dockwidget_enabled_.begin();
			for(list<QDockWidget*>::iterator it=dockwidgets_.begin(); it!=dockwidgets_.end(); it++, b_it++)
			{
				if(fullscreen_) 
				{
					if((*it)->isVisible()) *b_it=1;
					(*it)->setVisible(0);
				}
				else
				{
					if(*b_it==1) 
					{
						(*it)->setVisible(1);
						*b_it=0;
					}
				}		
			}
			if(fullscreen_) fullscreen_action_->setIcon(QIcon(IconLoader::instance().getIcon("actions/window_nofullscreen")));
			else fullscreen_action_->setIcon(QIcon(IconLoader::instance().getIcon("actions/window_fullscreen")));
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
			file_browser_ = new FileBrowser(settings.input_data_path.c_str(),this);
			QDockWidget* filedock = new QDockWidget(tr("Source Filebrowser"), this);
			dockwidgets_.push_back(filedock);
			filedock->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
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
				if (a==0) text0->setPlainText(tr("Linear regression models"));
				else if(a==1) text0->setPlainText(tr("Nonlinear regression models"));
				else if(a==2) text0->setPlainText(tr("Classification models"));
				QGraphicsRectItem* rect0 = new QGraphicsRectItem(0,0,text0->boundingRect().width()+20,text0->boundingRect().height(),text0);
				model_list_scene_.addItem(text0);
				rect0->setPen(pen);
				text0->setPos(x_offset,y-30);
				uint i=0;
				for(RegistryEntryIterator it=reg_->beginEntry(); it!=reg_->endEntry(); it++,i++)
				{ 
					if(a==0 && (it->second.kernel || !it->second.regression)) continue; // create only lin. model 
					else if(a==1 && (!it->second.kernel ||  !it->second.regression)) continue; // create only kernel models
					else if(a==2 && (it->second.regression)) continue; // create only classification models
					
					ModelItem* item = new ModelItem(&(it->second), model_list_);
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


			QDockWidget* modeldock = new QDockWidget(tr("Models"), this);
			dockwidgets_.push_back(modeldock);
			modeldock->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
			modeldock->setWidget(model_list_);
			
			QColor c1(160,172,182);
			QBrush b1(c1,Qt::SolidPattern);
			model_list_->setBackgroundBrush(b1);
			
			addDockWidget(Qt::RightDockWidgetArea, modeldock);
			windowMenu_->addAction(modeldock->toggleViewAction());

			///create dock widget for listing all available feature selection methods
			for(uint i=0; i<=6;i++)
			{ 
				FeatureSelectionItem* item = new FeatureSelectionItem(i, fs_list_);
				fs_list_scene_.addItem(item);
				item->setPos(20,70*i+20);
			}
			QDockWidget* fsdock = new QDockWidget(tr("Feature Selection"), this);
			dockwidgets_.push_back(fsdock);
			fsdock->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
			fsdock->setWidget(fs_list_);
			addDockWidget(Qt::RightDockWidgetArea, fsdock);
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
			dockwidgets_.push_back(validationdock);
			validationdock->setAllowedAreas(Qt::RightDockWidgetArea|Qt::RightDockWidgetArea);
			validationdock->setWidget(val_list_);
			addDockWidget(Qt::LeftDockWidgetArea, validationdock);
			windowMenu_->addAction(validationdock->toggleViewAction());
			
			QColor c3(205,225,205);
			QBrush b3(c3,Qt::SolidPattern);
			val_list_->setBackgroundBrush(b3);

			tabifyDockWidget(modeldock,fsdock);
			tabifyDockWidget(fsdock, validationdock);
			modeldock->raise();

			QDockWidget* progressdock = new QDockWidget(tr("Progress"), this);
			dockwidgets_.push_back(progressdock);
			progressdock->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
			progressdock->setWidget(progress_bar_);
			addDockWidget(Qt::LeftDockWidgetArea, progressdock);
			windowMenu_->addAction(progressdock->toggleViewAction());
		}

		void MainWindow::showDocumentation()
		{
			if(!documentation_)
			{
				documentation_ = new QDockWidget(tr("Documentation"),this);
				QTextBrowser* browser = new QTextBrowser(documentation_);
				documentation_->setWidget(browser);
				
				String file=".."+settings.path_separator+"doc"+settings.path_separator+"QuEasyViz"+settings.path_separator+"index.html";
				Path p;
				file = p.find(file);
				if(file=="") // use subfolder of executable's directory
				{
					file = executable_directory_+settings.path_separator+"doc"+settings.path_separator+"QuEasyViz"+settings.path_separator+"index.html";
				}		
				
				QUrl qurl = QUrl::fromLocalFile(file.c_str());
				browser->setSource(qurl);
				addDockWidget(Qt::LeftDockWidgetArea, documentation_);
				documentation_->setFloating(1);
				
				//documentation_->move((int)(pos().x()+width()*0.2),(int)(pos().y()+height()*0.1));
				int x0=(width()-750)/2;
				if(x0<0) x0=0;
				documentation_->move(pos().x()+x0,pos().y());
				documentation_->resize(750,height());
				
				//documentation_->resize(500,500);
				dockwidgets_.push_back(documentation_);
			}
			else
			{
				documentation_->setVisible(1);
			}	
		}

		void MainWindow::deleteItem()
		{
			QList<QGraphicsItem*> items = view_scene_.selectedItems();

			if (items.size() > 0)
			{
				for (QList<QGraphicsItem*>::Iterator it = items.begin(); it != items.end(); ++it)
				{	
					DataItem* item = (DataItem*)(*it);
					if(itemExists(item)) // just as backup to make sure that the item has not already been deleted
					{
						delete item;
					}
				}
				
			}
			else
			{
				QMessageBox::information(this, tr(" "), tr("No item selected"));
			}
			updatePipelineScene(); // remove junk from screen
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
			
			updatePipelineScene();
		}


		void MainWindow::updatePipelineScene()
		{
			for (Pipeline<ModelItem*>::iterator it = model_pipeline_.begin(); it != model_pipeline_.end(); it++)
			{
				(*it)->isDone(); // checks whether model is to be disabled and sets pixmap accordingly
			}
			view_scene_.update();
		}
			

		// SLOT
		void MainWindow::restoreDesktop()
		{
			QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), settings.config_path.c_str(), tr("Pipeline") + " (*.tar.gz *.conf)");
			if (filename=="") 
				return;
			String s = filename.toStdString();
			settings.config_path = s.substr(0,s.find_last_of(settings.path_separator)+1);
			try
			{
				restoreDesktop(filename);
			}
			catch(InvalidPipeline)
			{
				QMessageBox::about(this, tr("Error"), tr("Invalid Pipeline"));
			}
		}


		// SLOT
		BALL::String MainWindow::exportPipeline()
		{
			return exportPipeline(0);
		}


		BALL::String MainWindow::exportPipeline(bool no_immediate_archiving)
		{
			if (checkForEmptyPipelines()) return "";
			
			QString filename = QFileDialog::getSaveFileName(this, tr("Save File as"),(settings.config_path+"config.tar.gz").c_str(), tr("Pipeline") + " (*.tar.gz *.conf)");
			String s = filename.toStdString();
			if (filename=="") return s;
			
			if (no_immediate_archiving && s.hasSuffix(".tar.gz"))
			{
				String tmp = s.before(".tar.gz");
				filename = tmp.c_str();
				filename += ".conf";
			}
			
			exportPipeline(filename);
			settings.config_path = s.substr(0,s.find_last_of(settings.path_separator)+1);
			return s;
		}


		void MainWindow::saveItemsToFiles(String directory, String archive, String configfile)
		{
			bool use_tmp=0;
			if (archive!="" && settings.tmp_folder!="") 
				use_tmp=1;
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
						throw GeneralException(__FILE__,__LINE__, "SDF data saving error ", "SDF Input must be assigned a file to be saved to!");
					}
					String file = directory+f1;
					if(use_tmp) file = settings.tmp_folder+settings.path_separator+f1;
					files+=f1+" ";
					data->saveToFile(file);
				}
				for (Pipeline<CSVInputDataItem*>::iterator it = csv_input_pipeline_.begin(); it != csv_input_pipeline_.end(); it++)
				{
					// if input has not been read, there is nothing to be saved
					if (!(*it)->isDone() || (*it)->append()) 
						continue;
					
					QSARData* data= (*it)->data();
					String f1 = (*it)->savedAs().toStdString();
					if (f1=="")
					{
						throw GeneralException(__FILE__,__LINE__, "CSV saving error ", "CSV Item must be assigned a file to be saved to!");
					}
					String file = directory+f1;
					if (use_tmp) 
						file = settings.tmp_folder+settings.path_separator+f1;
					files+=f1+" ";
					data->saveToFile(file);
				}
				for (Pipeline<InputPartitionItem*>::iterator it = partition_pipeline_.begin(); it != partition_pipeline_.end(); it++)
				{
					// if input has not been read, there is nothing to be saved
					if (!(*it)->isDone() || (*it)->append()) 
						continue;
					
					QSARData* data= (*it)->data();
					String f1 = (*it)->savedAs().toStdString();
					if (f1=="")
					{
						throw GeneralException(__FILE__,__LINE__, "Input-partition saving error ", "Item must be assigned a file to be saved to!");
					}
					String file = directory+f1;
					if(use_tmp) file = settings.tmp_folder+settings.path_separator+f1;
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
						throw GeneralException(__FILE__,__LINE__, "Model saving error ", "Model must be assigned a file to be saved to!");
					}
					String file = directory+f1;
					if(use_tmp) file = settings.tmp_folder+settings.path_separator+f1;
					files+=f1+" ";
					(*it)->saveToFile(file);
				}
				for (Pipeline<ValidationItem*>::iterator it = val_pipeline_.begin(); it != val_pipeline_.end(); it++)
				{
					// if validation has not yet been done, there is nothing to be saved
					if(!(*it)->isDone()) continue;
					
					String f1 = (*it)->savedAs().toStdString();
					if(f1=="")
					{
						throw GeneralException(__FILE__,__LINE__, "Validation saving error ", "Validation must be assigned a file to be saved to!");
					}
					String file = directory+f1;
					if(use_tmp) file = settings.tmp_folder+settings.path_separator+f1;
					files+=f1+" ";
					(*it)->saveToFile(file);
				}
				for (Pipeline<PredictionItem*>::iterator it = prediction_pipeline_.begin(); it != prediction_pipeline_.end(); it++)
				{
					// if validation has not yet been done, there is nothing to be saved
					if(!(*it)->isDone()) continue;
					
					String f1 = (*it)->savedAs().toStdString();
					if(f1=="")
					{
						throw GeneralException(__FILE__,__LINE__, "Prediction saving error ", "Prediction must be assigned a file to be saved to!");
					}
					String file = directory+f1;
					if(use_tmp) file = settings.tmp_folder+settings.path_separator+f1;
					files+=f1+" ";
					(*it)->saveToFile(file);
				}
			}
			catch(GeneralException e)
			{	
				QMessageBox::warning(this, "Error",e.getMessage());
			}
			
			if(archive!="")
			{
				int index=configfile.find_last_of(settings.path_separator);
				if(index!=(int)string::npos)
				{
					configfile=configfile.substr(index+1); // no path; filename only!
				}
				String call1;
				String call2;
				if(!use_tmp)
				{
					call1 = "cd "+directory+"; tar -czf "+archive+" "+files+" "+configfile;
					call2 = "cd "+directory+"; rm -f "+files+" "+configfile;
				}
				else
				{
					call1 = "cd "+settings.tmp_folder+"; tar -czf "+archive+" "+files+" "+configfile;
					call2 = "cd "+settings.tmp_folder+"; rm -f "+files+" "+configfile;
				}
				system(call1.c_str());	// compress output files (and move archive)
				system(call2.c_str());	// delete uncompressed files
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
					// if dat-file has already been read by connected SDFInputItem, do not read it again
					if((*it)->isDone()) continue;
					
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
						input.close();
						(*it)->loadFromFile(filename);
					}
				}
				for (Pipeline<ValidationItem*>::iterator it = val_pipeline_.begin(); it != val_pipeline_.end(); it++)
				{
					String filename=directory+(*it)->savedAs().toStdString();
					ifstream input(filename.c_str());
					if(input) // read only existing validation-files
					{
						input.close();
						(*it)->loadFromFile(filename);
					}
				}
				for (Pipeline<PredictionItem*>::iterator it = prediction_pipeline_.begin(); it != prediction_pipeline_.end(); it++)
				{
					String filename=directory+(*it)->savedAs().toStdString();
					ifstream input(filename.c_str());
					if(input) // read only existing validation-files
					{
						input.close();
						(*it)->loadFromFile(filename);
					}
				}
				
				// now just display the average predictive quality for all nested-validation items (if all necessary predictions are already done)
				// --> no time-consuming calculation is done here
				for (Pipeline<ValidationItem*>::iterator it = val_pipeline_.begin(); it != val_pipeline_.end(); it++)
				{
					if((*it)->getValidationType()==5)
					{
						(*it)->execute();
					}
				}
			}
			catch(GeneralException e)
			{
				QMessageBox::warning(this, "Error",e.getMessage());
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
				QMessageBox::about(this,"No pipeline", "There is no pipeline to be excuted yet!");
				return;
			}

			int value = 0;
			progress_bar_->setMaximum(maximum);
			
			bool done=0;
			Timer timer; timer.start();
			
			for(Pipeline<DataItem*>::iterator it = all_items_pipeline_.begin(); it != all_items_pipeline_.end(); it++)
			{
				try
				{
					bool b=(*it)->execute();
					if(!done) done=b;
				}
				catch(BALL::Exception::GeneralException e)
				{
					QMessageBox::warning(this,e.getName(),e.getMessage());
				}

				value++;
				setProgressValue(value);
			}
			
			timer.stop(); 
			cout<<"Time for executing pipeline: "<<timer.getClockTime()<<endl;

			progress_bar_->reset();
			if (!done)
			{
				QMessageBox a;
				a.about(this, "Warning:", "Pipeline has not changed,\nso there was nothing to be done!");
			}
			
			updatePipelineScene();
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


		void MainWindow::restoreDesktop(QString filename)
		{
			String configfile = filename.toStdString();
			int s = configfile.find_last_of(settings.path_separator);
			String directory = configfile.substr(0,s+1); // name of config-file folder
			bool archive = 0;
			String input_directory = directory;
			
			try
			{
				if (configfile.size()>7 && configfile.substr(configfile.size()-7)==".tar.gz")
				{
					archive = 1;
					String call;
					if (settings.tmp_folder=="") 
						call = "cd "+directory;
					else call = "cd "+settings.tmp_folder;
					call+="; tar -xzvf "+configfile+" > archive_contents.tmp";
					system(call.c_str());  // extrace files from archive
					
					configfile = configfile.substr(0,configfile.size()-7)+".conf"; //config-file within archive will always have extension ".conf"
				
					if (settings.tmp_folder!="")
					{
						int s = configfile.find_last_of(settings.path_separator);
						configfile = settings.tmp_folder + settings.path_separator + String(configfile.substr(s+1));
						directory = settings.tmp_folder+settings.path_separator;
					}
					
					if (!ifstream(configfile.c_str())) // find config-file if archive has been renamed
					{
						ifstream archive_contents;
						string contents_file = directory+"/archive_contents.tmp";
						archive_contents.open(contents_file.c_str());
						
						uint no_conf_files=0;
						while (archive_contents)
						{
							String item;
							archive_contents >> item;
							if (item.hasSuffix(".conf")) 
							{
								configfile=directory+settings.path_separator+item;
								no_conf_files++;
							}
						}
						if (no_conf_files==0)
						{
							throw BALL::Exception::GeneralException(__FILE__,__LINE__,"Pipeline-archive reading error","No conf-file found in archive!");
						}
						else if (no_conf_files>1)
						{
							throw BALL::Exception::GeneralException(__FILE__,__LINE__,"Pipeline-archive reading error","More than one conf-file found in archive!");
						}
					}
				}	
				
				ifstream file(configfile.c_str());
				if(!file)
				{
					string txt =  "config-file '";
					txt        += configfile+"' can not be found!";
					throw BALL::Exception::GeneralException(__FILE__,__LINE__,"Pipeline reading error" ,txt.c_str());
				}
			
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
				for (int i=0;!file.eof();i++)
				{
					String line="";
					getline(file,line);
					if (line=="" || line.hasPrefix("#") || line.hasPrefix("//") || line.hasPrefix("%"))
					{
						continue;
					}
					if (line.hasPrefix("["))
					{
						if (within_pos_section) break;
						else if (line.hasPrefix("[ItemPositions]")) within_pos_section=1;
					}
					else if (within_pos_section)
					{
						double x = line.getField(0).toDouble(); double y = line.getField(1).toDouble();
						item_positions.push_back(make_pair(x,y));
					}
				}
				
				file.close();
				file.open(configfile.c_str());

				/// read all other sections
				for (int i=0;!file.eof();i++)
				{
					String line="";
					getline(file,line);
					
					if (line=="" || line.hasPrefix("#") || line.hasPrefix("//") || line.hasPrefix("%"))
					{
						continue;
					}
					if (line.hasPrefix("["))
					{
						if (input_section) input_reader.readConfigSection(section,filenames_map,&item_positions,input_directory);
						if (partitioner_section) input_reader.readConfigSection(section,filenames_map,&item_positions,input_directory);
						if (model_section) new ModelItem(section,filenames_map,&item_positions,view_);
 						if (fs_section) new FeatureSelectionItem(section,filenames_map,&item_positions,view_);
 						if (val_section) new ValidationItem(section,filenames_map,&item_positions,view_);
						if (pred_section) new PredictionItem(section,filenames_map,&item_positions,view_);
						
						input_section=0;model_section=0;fs_section=0;
						val_section=0;pred_section=0;partitioner_section=0;
						if (line.hasPrefix("[InputReader]")) input_section=1;
						else if (line.hasPrefix("[InputPartitioner]")) partitioner_section=1;
						else if (line.hasPrefix("[ModelCreator]")) model_section=1;
						else if (line.hasPrefix("[FeatureSelector]")) fs_section=1;
						else if (line.hasPrefix("[Validator]")) val_section=1;
						else if (line.hasPrefix("[Predictor]")) pred_section=1;
						
						section=line+"\n"; // save section header
						continue;
					}
					
					section+=line+"\n"; // store line of current section
				}
 				if (input_section) input_reader.readConfigSection(section,filenames_map,&item_positions,input_directory);
				if (partitioner_section) input_reader.readConfigSection(section,filenames_map,&item_positions,input_directory);
 				if (model_section) new ModelItem(section,filenames_map,&item_positions,view_);
 				if (fs_section) new FeatureSelectionItem(section,filenames_map,&item_positions,view_);
 				if (val_section) new ValidationItem(section,filenames_map,&item_positions,view_);
				if (pred_section) new PredictionItem(section,filenames_map,&item_positions,view_);
				
				file.close();
			}
			catch(BALL::Exception::GeneralException e)
			{
				QMessageBox::warning(this,e.getName(),e.getMessage());
				if (archive)
				{	
					string file = directory+"archive_contents.tmp";
					ifstream in(file.c_str());
					String files="";
					while (in)
					{
						String tmp;
						in >> tmp;
						files += tmp+" ";
					}
					String call = "cd "+directory+"; "+"rm -f "+files+" archive_contents.tmp";
					system(call.c_str());   // delete uncompressed files
				}
				return;
			}
			
			updatePipelineScene();
			view_->update();
			
			/// read all items if respec. files exist in the folder of the config-file	
			loadItemsFromFiles(directory);
			
			if (archive)
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
			int value = 0;
			progress_bar_->setMaximum(maximum);

			InputDataItemIO input_writer(view_);

			QString name;
			String configfile = filename.toStdString(); // has full path
			String archive = "";
			if (configfile.size()>7 && configfile.substr(configfile.size()-7)==".tar.gz")
			{
				archive = configfile;
				configfile = configfile.substr(0,configfile.size()-7)+".conf";
			}
			int d = configfile.find_last_of(".");
			int s = configfile.find_last_of(settings.path_separator);
			String file_prefix = configfile.substr(s+1,d-s-1)+"_"; // name of config-file as prefix for output-files
			String directory = configfile.substr(0,s+1); // name of folder
			
			if (archive!="" && settings.tmp_folder!="") 
				configfile = settings.tmp_folder + settings.path_separator + String(configfile.substr(s+1));
			ofstream out(configfile.c_str());
			
			ostringstream positions;
			positions<<"[ItemPositions]"<<endl;
			
			int counter = 0;
			
			/// First, set all output filenames!
			for (Pipeline<SDFInputDataItem*>::iterator it = sdf_input_pipeline_.begin(); it != sdf_input_pipeline_.end(); it++)
			{
				SDFInputDataItem* item = (*it);
				item->setSavedAs(file_prefix.c_str()+item->name()+".dat");
			}

			for (Pipeline<CSVInputDataItem*>::iterator it = csv_input_pipeline_.begin(); it != csv_input_pipeline_.end(); it++)
			{
				CSVInputDataItem* item = (*it);
				item->setSavedAs(file_prefix.c_str()+item->name()+".dat");
			}

			for (Pipeline<InputPartitionItem*>::iterator it = partition_pipeline_.begin(); it != partition_pipeline_.end(); it++)
			{
				InputPartitionItem* item = (*it);
				String n = file_prefix+item->getOutputFilename();
				item->setSavedAs(n.c_str());
			}
			
			counter=0;
			for (Pipeline<ModelItem*>::iterator it = model_pipeline_.begin(); it != model_pipeline_.end(); it++,counter++)
			{
				ModelItem* item = (*it);
				item->setSavedAs(file_prefix.c_str()+item->name() + name.setNum(counter) + ".mod");
			}

			counter=0;
			for (Pipeline<PredictionItem*>::iterator it = prediction_pipeline_.begin(); it != prediction_pipeline_.end(); it++,counter++)
			{
				PredictionItem* item = (*it); 
				item->setSavedAs(file_prefix.c_str()+name.setNum(counter) + ".pred");
			}
				
			counter=0;
			for (Pipeline<ValidationItem*>::iterator it = val_pipeline_.begin(); it != val_pipeline_.end(); it++,counter++)
			{
				ValidationItem* item = (*it); 
				item->setSavedAs(file_prefix.c_str()+name.setNum(counter)+".val");
			}
			
			/// Write all configfile-sections in the order in which the items were created!!
			for (Pipeline<DataItem*>::iterator it = all_items_pipeline_.begin(); it != all_items_pipeline_.end(); it++)
			{
				DataItem* item = *it;
				int type = item->type();
				
				if (type==CSVInputDataItem::Type)
				{
					CSVInputDataItem* csv_item = (CSVInputDataItem*) item;
					if (csv_item->append()) 
						continue;
					input_writer.writeConfigSection(csv_item,out,directory);
				}
				else if (type==SDFInputDataItem::Type)
				{
					SDFInputDataItem* sdf_item = (SDFInputDataItem*) item;
					input_writer.writeConfigSection(sdf_item,out,positions,directory);
					continue;
				}
				else if (type==PartitioningItem::Type)
				{
					PartitioningItem* partitioner = (PartitioningItem*) item;
					input_writer.writeConfigSection(partitioner,out,positions);
					continue;
				}
				else
				{
					if (type==ModelItem::Type)
					{
						ModelItem* model_item = (ModelItem*) item;
						if (!model_item->saveAttribute()) 
							continue;
					}
					item->writeConfigSection(out);
				}
				
				// save the item's own position!
				positions<<item->x()<<"  "<<item->y()<<endl;
				
				if(type==FeatureSelectionItem::Type) 
				{
					FeatureSelectionItem* fs_item = (FeatureSelectionItem*) item;
					positions<<fs_item->modelItem()->x()<<"  "<<fs_item->modelItem()->y()<<endl;
				}
				
				value++;
				setProgressValue(value);
			}
			
			out<<positions.str().c_str()<<endl;
			out.close();
			
			/// save item data to files
			saveItemsToFiles(directory,archive,configfile);
			
			progress_bar_->reset();
		}


		bool MainWindow::itemExists(DataItem* item)
		{
			if (sdf_input_pipeline_.contains((SDFInputDataItem*)item)) return 1;
			if (csv_input_pipeline_.contains((CSVInputDataItem*)item)) return 1;
			if (model_pipeline_.contains((ModelItem*)item)) return 1;
			if (fs_pipeline_.contains((FeatureSelectionItem*)item)) return 1;
			if (val_pipeline_.contains((ValidationItem*)item)) return 1;
			if (prediction_pipeline_.contains((PredictionItem*)item)) return 1;
			if (disconnected_items_.contains(item)) return 1;
			if (partitioning_pipeline_.contains((PartitioningItem*)(item))) return 1;
			if (partition_pipeline_.contains((InputPartitionItem*)item)) return 1;
			
			return 0;
		}

		// SLOT
		void MainWindow::submit()
		{
			if(checkForEmptyPipelines()) return;
			
			String configfile = exportPipeline(1);
			if(configfile!="")
			{
				submitToCluster(configfile);
			}	
		}

		void MainWindow::submitToCluster(String configfile)
		{
			int d = configfile.find_last_of(".");
			int s = configfile.find_last_of(settings.path_separator);
			String file_prefix = configfile.substr(0,d); // prefix for output-files
			String short_file_prefix = configfile.substr(s+1,d-s); // prefix without folders
			
			String directory = configfile.substr(0,s+1); // name of folder
			bool archive = configfile.hasSuffix(".tar.gz");
			if (archive)
			{
				file_prefix = configfile.before(".tar.gz");
				short_file_prefix = configfile.substr(s+1);
				short_file_prefix = short_file_prefix.before(".tar.gz");
				configfile = file_prefix+".conf";
			}
			
			String script = file_prefix+".csh";
			ofstream out(script.c_str());
			
			String prog="";
			if (settings.tools_path!="")
			{
				prog=settings.tools_path+settings.path_separator;
			}
			prog.append("QPipeStarter");
				
			if (settings.send_email && settings.email_address!="")
			{	
				out<<"setenv start_time `date`"<<endl;
			}
			out<<"cd "<<directory<<endl;
			out<<prog<<" "<<configfile<<endl<<endl;
			
			if (archive)
			{
				out<<"tar -cz "<<short_file_prefix<<"* -f "<<short_file_prefix<<".tar.gz"<<endl;
				out<<"rm -f "<<short_file_prefix<<"*.dat "<<short_file_prefix<<"*.mod "<<short_file_prefix<<"*.conf "<<script<<endl<<endl;	
			}
			if (settings.send_email && settings.email_address!="")
			{
				out<<"echo \"Subject: "<<short_file_prefix<<" is ready!\\"<<endl<<"Process '"<<script<<"' is ready!\\"<<endl<<"Start Time: $start_time\\"<<endl<<"End time: `date`\\"<<endl<<"\\"<<endl<<" \" | sendmail "<<settings.email_address<<endl;
			}
				
			out.close();
			
			String call = "cd "+directory+"; ";
			call+=settings.submit_prefix+" "+script+" &";
			system(call.c_str());
		}


		bool MainWindow::checkForEmptyPipelines()
		{
			int maximum = sdf_input_pipeline_.size() + csv_input_pipeline_.size() + model_pipeline_.size() + val_pipeline_.size() + prediction_pipeline_.size() + disconnected_items_.size(); //all items - fs-items (model items automatically created by feature selection are not exported)
			if (maximum == 0)
			{
				QMessageBox::about(this,"No pipeline","There is no pipeline yet!");
				return 1;
			}
			return 0;
		}


		int MainWindow::chooseValidationStatisticDialog(ModelItem* modelitem)
		{
			const map<uint,String>* statistics = modelitem->getRegistryEntry()->getStatistics();
			
			// if there is just one registered statistic, don't bother the user with a useless question! 
			if (statistics->size()==1)
			{
				return statistics->begin()->first;
			}
				
			QDialog dialog;
			QVBoxLayout main_layout;
			
			QHBoxLayout h_layout;
			QLabel label(tr("Desired quality statistic"));
			QComboBox statistic_box;
				
			for (map<uint,String>::const_iterator it=statistics->begin(); it!=statistics->end(); ++it)
			{
				statistic_box.addItem(it->second.c_str(),it->first);
			}
					
			h_layout.addWidget(&label);h_layout.addWidget(&statistic_box);
			main_layout.addLayout(&h_layout);
			dialog.setLayout(&main_layout);
			QDialogButtonBox buttons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,Qt::Horizontal);
			main_layout.addWidget(&buttons);
			connect(&buttons, SIGNAL(accepted()), &dialog, SLOT(accept()));
			connect(&buttons, SIGNAL(rejected()), &dialog, SLOT(reject()));
			
			bool ok = dialog.exec();
			if (ok) 
				return statistic_box.currentIndex();
			else return 0;	
		}

		BALL::String MainWindow::getDataDirectory()
		{
			return data_directory_;
		}


		BALL::String valueToString(double value)
		{
			BALL::String t(value);
			int index = (uint)t.find_last_not_of("0");
			if (index!=(int)string::npos)
			{
				if (index+1<=(int)t.size() && (t[index]!='.')) 
					index++;
				t =t.substr(0,index);
			}
			return t;
		}


		void MainWindow::readDescriptorExplanations()
		{
			Path p;
			
			for (list<String>::iterator it = settings.descriptor_explanation_files.begin();
				it!=settings.descriptor_explanation_files.end(); it++)
			{
				String filename = data_directory_+settings.path_separator+*it;
				String abs_filename = p.find(filename);
				if (abs_filename=="") 
					abs_filename=filename;
			
				ifstream in(abs_filename.c_str());
				if (!in)
				{
					cout << "Error: feature-description file '" << abs_filename << "' not found!" << endl;
					return;
				}
				string name;
				string explanation;
				while(in)
				{
					String line; 
					getline(in,line);
					if (line==""||line=="\n") 
						break;
					
					name = line.before("\t");
					explanation = line.after("\t");
					descriptor_explanations_.insert(make_pair(name,explanation));
				}
			}
			
			read_descriptor_explanations_ = 1;
		}


		const BALL::String* MainWindow::getDescriptorExplanation(String descriptor_name)
		{
			if (!read_descriptor_explanations_) 
				readDescriptorExplanations();
			
			map<String,String>::iterator it = descriptor_explanations_.find(descriptor_name);
			
			if (it!=descriptor_explanations_.end()) 
				return &it->second;
			else return NULL;
		}

		void MainWindow::setProgressValue(int value)
		{
			if (progress_bar_) progress_bar_->setValue(value);
			QApplication::instance()->processEvents(QEventLoop::AllEvents, 500);
		}
	}
}
