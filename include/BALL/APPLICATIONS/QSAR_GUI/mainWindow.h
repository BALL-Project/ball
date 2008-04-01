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



#include <BALL/QSAR/registry.h>

#include <BALL/SYSTEM/path.h>


namespace BALL
{
	namespace VIEW
	{
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
				QSet<ModelItem*> getModelPipeline();

				/**add items to pipeline */
				void addInputToPipeline(InputDataItem* item);
				void addModelToPipeline(ModelItem* item);
				void addPredictionToPipeline(PredictionItem* item);
				void addFeatureSelectionToPipeline(FeatureSelectionItem* item);
				void addValidationToPipeline(ValidationItem* item);
				bool removeFromPipeline(DataItem* item);
				void addDisconnectedItem(DataItem* item);

				QSet<DataItem*> disconnectedItems();

				/** @name Attributes */
				/** pointer to the item that is currently being dragged */
				DataItem* dragged_item;
				
				/** name of the widget from which the current drag originated, i.e. "source_list", "model_list" or "view" */
				string drag_source;
				PreciseTime drag_start_time;
				
				/* shortest desired duration of a mouse drag in units of seconds */
				double min_drag_time;
				
				void setLastUsedPath(String path);

			signals:
				void sendNewValue(int);
			
			private slots:
				
				/** @name private slots
				*/
				void about();	
				void clearDesktop();
				void saveDesktop();
				void restoreDesktop(QString filename);
				void deleteItem();
				void restoreDesktop();
				void executePipeline();
				void exportPipeline();
				void exportPipeline(QString filename, bool ext);
				void saveModels();

			private:
		
				void createMenus();
				void createToolBars();
				void createStatusBar();
				void createActions();
				void createDockWindows();
				void createDialogs();
			
				/** @name Private Attributes */

				DataItemView* view_;
				DataItemView* model_list_;
				DataItemView* fs_list_;
				DataItemView* val_list_;

				DataItemScene view_scene_;
				DataItemScene model_list_scene_;
				DataItemScene fs_list_scene_;
				DataItemScene val_list_scene_;
				
				// path of the last used input
				String last_path_;
				
				/**registry including all available models */	
				Registry* reg_;
				//Path* path_;

				QProgressBar* progress_bar_;
				FileBrowser* file_browser_;

				QSet<SDFInputDataItem*> sdf_input_pipeline_;
				QSet<CSVInputDataItem*> csv_input_pipeline_;
				QSet<ModelItem*> model_pipeline_;
				QSet<FeatureSelectionItem*> fs_pipeline_;
				QSet<ValidationItem*> val_pipeline_;
				QSet<PredictionItem*> prediction_pipeline_;
				QSet<DataItem*> disconnected_items_;
			
				/** @name Dialogs
				*/
				ModelConfigurationDialog* modelConfigurationDialog_;

				/** @name Menues
				*/
				QMenu *fileMenu_;
				QMenu *editMenu_;
				QMenu *helpMenu_;
				QMenu *windowMenu_;
			
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
				QAction* saveAct_;
				QAction* executeAct_;
				QAction* restoreAct_;
				QAction* exportAct_;
				QAction* saveModelsAct_;
				
				friend class ModelItem;
		};
	}
}	

 #endif

