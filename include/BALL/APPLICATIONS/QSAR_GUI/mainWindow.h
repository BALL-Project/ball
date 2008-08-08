#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QDialog>
#include <QtGui/QToolBar>
#include <QtGui/QAction>
#include <QtGui/QContextMenuEvent>
#include <QtGui/QProgressBar>

#include <BALL/APPLICATIONS/QSAR_GUI/dataItemView.h>
#include <BALL/APPLICATIONS/QSAR_GUI/dataItemScene.h>
#include <BALL/APPLICATIONS/QSAR_GUI/dataItem.h>
#include <BALL/APPLICATIONS/QSAR_GUI/modelItem.h>
#include <BALL/APPLICATIONS/QSAR_GUI/inputDataItem.h>
#include <BALL/APPLICATIONS/QSAR_GUI/CSVInputDataItem.h>
#include <BALL/APPLICATIONS/QSAR_GUI/featureSelectionItem.h>
#include <BALL/APPLICATIONS/QSAR_GUI/validationItem.h>
#include <BALL/APPLICATIONS/QSAR_GUI/predictionItem.h>
#include <BALL/APPLICATIONS/QSAR_GUI/SDFInputDialog.h>
#include <BALL/APPLICATIONS/QSAR_GUI/fileBrowser.h>
#include <BALL/APPLICATIONS/QSAR_GUI/modelConfigurationDialog.h>
#include <BALL/APPLICATIONS/QSAR_GUI/featureSelectionDialog.h>
#include <BALL/APPLICATIONS/QSAR_GUI/validationDialog.h>
#include <BALL/APPLICATIONS/QSAR_GUI/partitioningItem.h>
#include <BALL/APPLICATIONS/QSAR_GUI/inputPartitionItem.h>
#include <BALL/APPLICATIONS/QSAR_GUI/pipeline.h>

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

				/** destructor */
				~MainWindow();
			

				/** creation of items*/
				SDFInputDataItem* createSDFInput(QString filename);
				CSVInputDataItem* createCSVInput(QString filename);
				CSVInputDataItem* createCSVInput(QSARData* data);
				
				ModelItem* createModel(ModelItem* model, InputDataItem* input=NULL);
				FeatureSelectionItem* createFeatureSelection(FeatureSelectionItem* fs, ModelItem* model=0, ModelItem* in_model=0);
				ValidationItem* createValidation(ValidationItem* val, ModelItem* model=0);
				PredictionItem* createPrediction(InputDataItem* input, ModelItem* model);
				
				//void changeModelItem(ModelItem* model, InputDataItem* input);

				Registry* registry();
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

				/** sets for fast search of Items: */
				Pipeline<SDFInputDataItem*> sdf_input_pipeline_;
				Pipeline<CSVInputDataItem*> csv_input_pipeline_;
				Pipeline<ModelItem*> model_pipeline_;
				Pipeline<FeatureSelectionItem*> fs_pipeline_;
				Pipeline<ValidationItem*> val_pipeline_;
				Pipeline<PredictionItem*> prediction_pipeline_;
				Pipeline<DataItem*> disconnected_items_;
				Pipeline<PartitioningItem*> partitioning_pipeline_;
				Pipeline<InputPartitionItem*> partition_pipeline_;

			signals:
				void sendNewValue(int);
			
			private slots:
				
				/** @name private slots
				*/
				void about();	
				void clearDesktop();
				void restoreDesktop(QString filename);
				void deleteItem();
				void restoreDesktop();
				void executePipeline();
				
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
				};
				
				 /// the last used paths settings
				Settings settings;
		
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
				
				void readPartitionPositions(list<pair<double,double> >* item_positions);
				
				/** check whether the pipeline is not empty. Can be called before saving, submitting, printing the current pipeline.\n
				If no items have been created yet, a message box is displayed 
				@return true if pipelines are empty, false otherwise */
				bool checkForEmptyPipelines();
			
				/** @name Private Attributes */

				DataItemView* view_;
				DataItemView* model_list_;
				DataItemView* fs_list_;
				DataItemView* val_list_;

				DataItemScene view_scene_;
				DataItemScene model_list_scene_;
				DataItemScene fs_list_scene_;
				DataItemScene val_list_scene_;
				
				
				/**registry including all available models */	
				Registry* reg_;
				//Path* path_;

				QProgressBar* progress_bar_;
				FileBrowser* file_browser_;
		
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
				QAction* loadModelsAct_;
				
				friend class ModelItem;
		};
	}
}	

 #endif

