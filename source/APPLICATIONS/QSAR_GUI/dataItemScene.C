#include <BALL/APPLICATIONS/QSAR_GUI/dataItemScene.h>
#include <BALL/APPLICATIONS/QSAR_GUI/edge.h>
#include <BALL/APPLICATIONS/QSAR_GUI/dottedEdge.h>
#include <BALL/APPLICATIONS/QSAR_GUI/inputDataItem.h>
#include <BALL/APPLICATIONS/QSAR_GUI/modelItem.h>
#include <BALL/APPLICATIONS/QSAR_GUI/featureSelectionItem.h>
#include <BALL/APPLICATIONS/QSAR_GUI/mainWindow.h>
#include <BALL/APPLICATIONS/QSAR_GUI/exception.h>
#include <QtCore/QMimeData>
#include <QtCore/QUrl>
#include <QtGui/QGraphicsScene>
#include <QtCore/QFileInfo>


using namespace BALL::QSAR;
using namespace BALL::QSAR::Exception;
using namespace BALL::VIEW;
using namespace BALL::VIEW::Exception;

static QPointF offset = QPointF(100.,100.);

DataItemScene::DataItemScene()
 {
	setItemIndexMethod(NoIndex);	
 }

void DataItemScene::setMainWindow(MainWindow* mw)
{
	main_window = mw;
}

void DataItemScene::dropEvent(QGraphicsSceneDragDropEvent* event)
{
	QPointF pos = event->scenePos();
	
	/// move SDFInputDataItem
	if (event->mimeData()->hasFormat("application/x-sdfinputitemdata")) 
	{
		SDFInputDataItem* source_item = (SDFInputDataItem*)main_window->dragged_item;
		source_item->setPos(pos.x(),pos.y());
	}

	/// move CSVInputDataItem
	if (event->mimeData()->hasFormat("application/x-csvinputitemdata")) 
	{
		CSVInputDataItem* source_item = (CSVInputDataItem*)main_window->dragged_item;
		source_item->setPos(pos.x(),pos.y());
	}

	/// move PredictionItem
	else if (event->mimeData()->hasFormat("application/x-predictiondata")) 
	{
		PredictionItem* source_item = (PredictionItem*)main_window->dragged_item;
		source_item->setPos(pos.x(),pos.y());
	}

	/// create or move ModelItem
	else if (event->mimeData()->hasFormat("application/x-modelitemdata")) 
	{
		if(view->name!="view")
		{
			return;
		}

		ModelItem* item;
		SDFInputDataItem* input_item_at_pos = qgraphicsitem_cast<SDFInputDataItem *>(itemAt(pos));
		CSVInputDataItem* csv_input_item_at_pos = qgraphicsitem_cast<CSVInputDataItem *>(itemAt(pos));
		
		/// create a new model that is to be placed into the view-widget
		if(main_window->drag_source=="model_list")
		{
			item = (ModelItem*)main_window->dragged_item;
			
			try
			{
				
				if(!input_item_at_pos && !csv_input_item_at_pos)
				{
					//main_window->addDisconnectedItem(item);
					QMessageBox::information(view," ","Please drag the Model onto a SD- or CSV-item within your pipeline!");
					return;	
				}
				
				if(input_item_at_pos)
				item = main_window->createModel(item,input_item_at_pos);
				else if(csv_input_item_at_pos)
				item = main_window->createModel(item,csv_input_item_at_pos);	
					
				item->setView(view);
				addItem(item);
				item->setPos(pos.x(),pos.y());
				item->setPos(pos + offset);
				if (input_item_at_pos)
				{
					Edge* edge = new Edge(input_item_at_pos, item);
					addItem(edge);
					main_window->addModelToPipeline(item);
				}
				else if(csv_input_item_at_pos)
				{
					Edge* edge = new Edge(csv_input_item_at_pos, item);
					addItem(edge);
					main_window->addModelToPipeline(item);
				}
			}
			catch(InvalidModelItem)
			{
			}
		}
		
		/// if a ModelItem is to be moved WITHIN the view-widget:
		else if(main_window->drag_source=="view")
		{
			item = (ModelItem*)main_window->dragged_item;
			item->setPos(pos.x(),pos.y());
		}
		main_window->drag_source = "view"; // set back to "default"
	}

	/// create or move feature selection item
	else if (event->mimeData()->hasFormat("application/x-fsitemdata")) 
	{
		if(view->name!="view")
		{
			return;
		}

		FeatureSelectionItem* item;
		ModelItem* model_item_at_pos = qgraphicsitem_cast<ModelItem *>(itemAt(pos));
		
		// create a new feature selection item that is to be placed into the view-widget
		if(main_window->drag_source=="fs_list")
		{
			item = (FeatureSelectionItem*)main_window->dragged_item;
			
			//create a FeatureSelectionItem that is connected to a ModelItem
			ModelItem* model_copy = NULL;
			try
			{
				if (!model_item_at_pos)
				{
					//item = main_window->createFeatureSelection(item);
					//main_window->addDisconnectedItem(item);
					QMessageBox::information(view," ","Please drag the FeatureSelection onto a Model within your pipeline!");
					return;	
				}
				if (model_item_at_pos)
				{
					model_copy = new ModelItem(*model_item_at_pos);
					item = main_window->createFeatureSelection(item, model_copy, model_item_at_pos);
					model_copy->setSaveAttribute(false);
					
					addItem(model_copy);
					main_window->addModelToPipeline(model_copy);

					Edge* edge = new Edge(item, model_copy);
					addItem(edge);
					Edge* edge2 = new Edge(model_item_at_pos, item);
					addItem(edge2);
					main_window->addFeatureSelectionToPipeline(item);
					
					model_copy->setPos(pos + 2*offset);
				}

				item->setView(view);
				addItem(item);
				item->setPos(pos.x(),pos.y());
				item->setPos(pos + offset);
			}

			catch(InvalidFeatureSelectionItem)
			{
				delete model_copy;
			}	
		}

		else if(main_window->drag_source=="view")
		{
			item = (FeatureSelectionItem*)main_window->dragged_item;
			item->setPos(pos.x(),pos.y());
		}
		main_window->drag_source = "view"; // set back to "default"
     	}

	/// create or move validation item
	else if (event->mimeData()->hasFormat("application/x-validationdata")) 
	{
         	if(view->name!="view")
		{
			return;
		}

		ValidationItem* item;
		ModelItem* model_item_at_pos = qgraphicsitem_cast<ModelItem *>(itemAt(pos));
		
		// create a new validation item that is to be placed into the view-widget
		if(main_window->drag_source=="val_list")
		{
			item = (ValidationItem*)main_window->dragged_item;
				
			try
			{
				if (model_item_at_pos)
				{
					item = main_window->createValidation(item, model_item_at_pos);
					Edge* edge = new Edge(model_item_at_pos, item);
					addItem(edge);
					main_window->addValidationToPipeline(item);
				}
				else
				{
					//item = main_window->createValidation(item);
					//main_window->addDisconnectedItem(item);
					QMessageBox::information(view," ","Please drag the Validation onto a Model within your pipeline!");
					return;
				}
				item->setView(view);
				addItem(item);
				item->setPos(pos);
				item->setPos(pos + offset);
			}
			catch(InvalidValidationItem)
			{
// 				delete item;
			}

		}
		
		else if(main_window->drag_source=="view")
		{
			item = (ValidationItem*)main_window->dragged_item;
			item->setPos(pos.x(),pos.y());
		}
		
		main_window->drag_source = "view"; // set back to "default"
     	} 

	/// drag and drop from filebrowser to mainview
	else if ((event->mimeData()->hasUrls()))
	{
		if(view->name!="view")
		{
			return;
		}

		QList<QUrl> urlList = event->mimeData()->urls();
		QString path;
		
		QRegExp match_sd("*.sdf");
		QRegExp match_txt("*.txt");
		QRegExp match_csv("*.csv");
		match_sd.setPatternSyntax(QRegExp::Wildcard);
		match_txt.setPatternSyntax(QRegExp::Wildcard);
		match_csv.setPatternSyntax(QRegExp::Wildcard);

		foreach (QUrl url, urlList)
		{
			path = url.toLocalFile();
			QFileInfo file_info(path);
			if (file_info.isFile())
			{
				try
				{			
					if (match_sd.exactMatch(path))
					{
						SDFInputDataItem* item; 
					 	item = main_window->createSDFInput(path);

						ModelItem* model_item_at_pos = qgraphicsitem_cast<ModelItem*>(itemAt(pos)); 

						if (model_item_at_pos)
						{
							item->setPos(pos.x(),pos.y());
							PredictionItem* pred_item = main_window->createPrediction(item,model_item_at_pos);
							addItem(pred_item);
							pred_item->setPos(pos.x(),pos.y());
		
							Edge* edge = new Edge(model_item_at_pos, pred_item);
							addItem(edge);
							model_item_at_pos->addPredictionInputEdge(edge);
							Edge* edge2 = new Edge(item, model_item_at_pos);
							addItem(edge2);	
							DottedEdge* dedge = new DottedEdge(item, pred_item);
							addItem(dedge);
							pred_item->setDottedEdge(dedge);
							main_window->addPredictionToPipeline(pred_item);
						}
						addItem(item);
						item->setPos(pos.x(),pos.y());
						main_window->addInputToPipeline(item);

					}
					else if (match_txt.exactMatch(path) || match_csv.exactMatch(path))
					{
						CSVInputDataItem* csv_item;
						SDFInputDataItem* input_item_at_pos = qgraphicsitem_cast<SDFInputDataItem*>(itemAt(pos));
		
						if (input_item_at_pos)
						{
							csv_item = main_window->createCSVInput(path);
							csv_item->setData(input_item_at_pos->data());
							csv_item->setAppend(true);
							addItem(csv_item);
							Edge* edge = new Edge(input_item_at_pos, csv_item);
							addItem(edge);
							main_window->addInputToPipeline(csv_item);
							input_item_at_pos->additional_descriptors_ = 1;
						}

						else
						{
							csv_item = main_window->createCSVInput(path);
							addItem(csv_item);
							csv_item->setPos(pos.x(),pos.y());
							main_window->addInputToPipeline(csv_item);
						}
					}

				}
				catch(InvalidInputDataItem)
				{
				}
				catch(InvalidPredictionItem)
				{
				}
			}
		} 
	}
	else 
	{	
		event->ignore();
	}
	QGraphicsScene::dropEvent(event);
 	update();
 	view->update();
}

///this function allows the dropping of an item anywhere on the scene
void DataItemScene::addDropSite()
{
	///set the scene rect on the maximal possible value
	setSceneRect(0,0, 16777215, 16777215);
	
	///add a transparent rect item with maximal size onto this scene and allow drops onto it 
	QGraphicsRectItem* rect = addRect(QRectF(0, 0, 16777215, 16777215),QPen(QColor(Qt::transparent)));
	rect->setAcceptDrops(true);
}


void DataItemScene::mousePressEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
	QPointF p = mouseEvent->scenePos();
	QGraphicsItem* item = itemAt(p);
	if(!item) return; // if there is no item below mouse cursor, do nothing!
	
	/// if not doing this, Qt cannot reliably cast to ModelItem*, so that the type of the created ModelItem will be incorrect!
	if(strcmp(view->name.c_str(),"model_list")==0)
	{
  		ModelItem* model_item = static_cast<ModelItem*>(item);
  		model_item->mousePressEvent(mouseEvent);
	}
	else if(strcmp(view->name.c_str(),"fs_list")==0)
	{
		FeatureSelectionItem* fs_item = static_cast<FeatureSelectionItem*>(item);
		fs_item->mousePressEvent(mouseEvent);
	}
	else if(strcmp(view->name.c_str(),"val_list")==0)
	{
		ValidationItem* val_item = static_cast<ValidationItem*>(item);
		val_item->mousePressEvent(mouseEvent);
	}	
	else
	{
		QGraphicsScene::mousePressEvent(mouseEvent);
	}
}
