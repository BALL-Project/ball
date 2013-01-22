/* mainWindow.h
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QDialog>
#include <QtGui/QToolBar>
#include <QtGui/QAction>
#include <QtGui/QContextMenuEvent>
#include <QtGui/QProgressBar>

#include <dataItemView.h>
#include <dataItemScene.h>
#include <dataItem.h>
#include <modelItem.h>
#include <inputDataItem.h>
#include <CSVInputDataItem.h>
#include <featureSelectionItem.h>
#include <validationItem.h>
#include <predictionItem.h>
#include <SDFInputDialog.h>
#include <fileBrowser.h>
#include <modelConfigurationDialog.h>
#include <featureSelectionDialog.h>
#include <validationDialog.h>
#include <partitioningItem.h>
#include <inputPartitionItem.h>
#include <pipeline.h>

#include <BALL/QSAR/registry.h>
#include <BALL/SYSTEM/path.h>


namespace BALL
{
	namespace VIEW
	{
		
		BALL::String valueToString(double value);
		
		
		class ModelConfigurationDialog;

		/** @class MainWindow
		* @brief the main application
		*
		* @todo
		*/
		class MainWindow : public QMainWindow
		{
			Q_OBJECT
			
			public:
				
				/** @name Constructors and Destructors*/
	
				/** constructor*/
				MainWindow();
				
				MainWindow(char* executable_directory);

				/** destructor */
				~MainWindow();
			

				/** creation of items*/
				SDFInputDataItem* createSDFInput(QString filename);
				CSVInputDataItem* createCSVInput(QString filename);
				CSVInputDataItem* createCSVInput(BALL::QSAR::QSARData* data);
				
				ModelItem* createModel(ModelItem* model, InputDataItem* input=NULL);
				FeatureSelectionItem* createFeatureSelection(FeatureSelectionItem* fs, ModelItem* model=0, ModelItem* in_model=0);
				ValidationItem* createValidation(ValidationItem* val, ModelItem* model=0);
				PredictionItem* createPrediction(InputDataItem* input, ModelItem* model);
				
				int chooseValidationStatisticDialog(ModelItem* modelitem);
				
				//void changeModelItem(ModelItem* model, InputDataItem* input);

				BALL::QSAR::Registry* registry();
				Pipeline<ModelItem*> getModelPipeline();

				/**add items to pipeline */
				//void addInputToPipeline(InputDataItem* item);
				//void addModelToPipeline(ModelItem* item);
				//void addPredictionToPipeline(PredictionItem* item);
// 				void addFeatureSelectionToPipeline(FeatureSelectionItem* item);
// 				void addValidationToPipeline(ValidationItem* item);
				void addDisconnectedItem(DataItem* item);
				

				Pipeline<DataItem*> disconnectedItems();
				
				/** submits the pipeline that was saved into configfile to a cluster */
				void submitToCluster(String configfile);
				
				String getDataDirectory();
				
				/** @name Attributes */
				/** pointer to the item that is currently being dragged */
				DataItem* dragged_item;
				
				/** name of the widget from which the current drag originated, i.e. "source_list", "model_list" or "view" */
				string drag_source;
				PreciseTime drag_start_time;
				
				/* shortest desired duration of a mouse drag in units of seconds */
				double min_drag_time;
				
				void setLastUsedPath(String path);
				
				/** checks whether the given item exists in any of this MainWindow's pipelines */
				bool itemExists(DataItem* item);
				
				 /** before calling QGraphicsScene-update, checks for all models whether they are to be disabled and sets pixmaps accordingly */
				void updatePipelineScene();

				Pipeline<SDFInputDataItem*> sdf_input_pipeline_;
				Pipeline<CSVInputDataItem*> csv_input_pipeline_;
				Pipeline<ModelItem*> model_pipeline_;
				Pipeline<FeatureSelectionItem*> fs_pipeline_;
				Pipeline<ValidationItem*> val_pipeline_;
				Pipeline<PredictionItem*> prediction_pipeline_;
				Pipeline<DataItem*> disconnected_items_;
				Pipeline<PartitioningItem*> partitioning_pipeline_;
				Pipeline<InputPartitionItem*> partition_pipeline_;
				
				Pipeline<DataItem*> all_items_pipeline_;
				
				const String* getDescriptorExplanation(String descriptor_name);
				
			
			private slots:
				
				/** @name private slots
				*/
				void about();	
				void clearDesktop();
				void restoreDesktop(QString filename);
				void deleteItem();
				void restoreDesktop();
				void executePipeline();
				void zoomIn();
				void zoomOut();
				
				/** exports the pipeline to files and return the name of the created config-file */
				String exportPipeline();
				
			//	void exportPipeline(QString filename, bool ext);
				void exportPipeline(QString filename);
				void print();
				void printToFile();
				
				/** submits the current pipeline to a cluster */
				void submit();
				
				/** displays a preferences dialog that allow setting the cluster's queue parameters */
				void preferencesDialog();
				
				/** switches to fullscreen and from fullscreen back to normal view */
				void fullscreen();
				
				void showDocumentation();
				
				
			private:
				
				struct Settings
				{			
					String input_data_path;
					String config_path;
					int size_x;
					int size_y;
					int pos_x;
					int pos_y;
					
					/** the prefix for submitting a job to the cluster, \n
					  e.g. "qsub -cwd" or "bsub" or "qsub -q long.q" ... **/
					String submit_prefix;
					
					/** path to the installation of QSARPipelinePackage */
					String tools_path;
					
					/** determines whether an email is to be send after completion of a submitted job */
					bool send_email;
					
					/** determines to which email address a notification of job completion should be send */
					String email_address;
					
					/** a folder where data may be stored temporarily */
					String tmp_folder;
					
					/** The character separating directories in a path. \n
					By default BALL::FileSystem::PATH_SEPARATOR is used */
					String path_separator;
					
					/** names of files containing explanations for descriptors */
					list<String> descriptor_explanation_files;
					
					/** determines whether the documentation is to be shown at start-up */
					bool show_documentation;
					
					MainWindow* main_window;
					
					void saveToFile(String file);
					
					void readFromFile(String file);
				};
				
				 /// the last used paths settings
				Settings settings;
		
				void init();
				void createMenus();
				void createToolBars();
				void createStatusBar();
				void createActions();
				void createDockWindows();
				void createDialogs();
				
				/** saves the data of all InputItems and ModelItems to files.
				@param archive if specified, the created files are compressed and saved as an archive (e.g. *.tar.gz) */
				void saveItemsToFiles(String directory, String archive="", String configfile="");
				
				void loadItemsFromFiles(String directory);
				
				/** check whether the pipeline is not empty. Can be called before saving, submitting, printing the current pipeline.\n
				If no items have been created yet, a message box is displayed 
				@return true if pipelines are empty, false otherwise */
				bool checkForEmptyPipelines();
				
				/** exports the current Pipeline to file(s).
				@param no_immediate_archiving if true, the pipeline is saved as several files even if tar.gz archive was chosen. All created files will later be archived _after_ all calculation on the cluster for this pipeline are ready. */
				String exportPipeline(bool no_immediate_archiving);
				
				void readDescriptorExplanations();

				void setProgressValue(int);
			
				/** @name Private Attributes */

				DataItemView* view_;
				DataItemView* model_list_;
				DataItemView* fs_list_;
				DataItemView* val_list_;

				DataItemScene view_scene_;
				DataItemScene model_list_scene_;
				DataItemScene fs_list_scene_;
				DataItemScene val_list_scene_;
				
				std::map<String,String> descriptor_explanations_;
				bool read_descriptor_explanations_;
				
				/** List of all dock-widget of the mainWindow. It is used for switching to/from fullscreen mode by hiding all dockwidgets */
				list<QDockWidget*> dockwidgets_;
				list<bool> dockwidget_enabled_;
				
				/** is fullscreen currently enabled? */
				bool fullscreen_;
				
				
				/**registry including all available models */	
				BALL::QSAR::Registry* reg_;
				//Path* path_;

				QProgressBar* progress_bar_;
				FileBrowser* file_browser_;
				QDockWidget* documentation_;
		
				ModelConfigurationDialog* modelConfigurationDialog_;

				/** @name Menues
				*/
				QMenu* fileMenu_;
				QMenu* editMenu_;
				QMenu* helpMenu_;
				QMenu* windowMenu_;
			
				/** @name Toolbars
				*/
				QToolBar *fileToolBar_;
			
				/** @name Actions
				*/
				QAction* exitAct_;
				QAction* aboutAct_;
				QAction* clearAct_;
				QAction* delAct_;
				QAction* modelAct_;
				QAction* executeAct_;
				QAction* restoreAct_;
				QAction* exportAct_;
				QAction* fullscreen_action_;
				
				/** The directory of the executable of this program.\n
				It is used to find subfolders for icons and documentation and has to be set from main.C */
				String executable_directory_;
				
				String data_directory_;
				
				friend class ModelItem;
		};
	}
}	

 #endif

