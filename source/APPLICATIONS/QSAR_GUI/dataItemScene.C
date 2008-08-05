#include <BALL/APPLICATIONS/QSAR_GUI/dataItemScene.h>
#include <BALL/APPLICATIONS/QSAR_GUI/edge.h>
#include <BALL/APPLICATIONS/QSAR_GUI/mainWindow.h>
#include <BALL/APPLICATIONS/QSAR_GUI/exception.h>
#include <BALL/APPLICATIONS/QSAR_GUI/inputPartitionItem.h>

#include <QtCore/QMimeData>
#include <QtCore/QUrl>
#include <QtGui/QGraphicsScene>
#include <QtCore/QFileInfo>


using namespace BALL::QSAR;
using namespace BALL::QSAR::Exception;
using namespace BALL::VIEW;
using namespace BALL::VIEW::Exception;

static QPointF default_offset = QPointF(0.,100.);

DataItemScene::DataItemScene()
 {
	setItemIndexMethod(NoIndex);	
 }
 
DataItemScene::~DataItemScene()
{
// 	for(list<QGraphicsItemGroup*>::iterator it=groups_.begin(); it!=groups_.end();it++)
// 	{
// 		delete *it;
// 	}
}

void DataItemScene::setMainWindow(MainWindow* mw)
{
	main_window = mw;
}


QPointF DataItemScene::getOffset(QPointF& origin, DataItem* item)
{
	// InputDataItem created together with a PredictionItem
	QPointF pos = default_offset;
	if(item->type()==SDFInputDataItem::Type || item->type()==CSVInputDataItem::Type || (item->type()==InputPartitionItem::Type && ((InputPartitionItem*)item)->isTestPartition()))
	{
		pos = QPointF(-120,-70);
	}	
	else if(item->type()==ValidationItem::Type) // ValidationItem	
	{
		pos = QPointF(100,0);
	}
	else if(item->type()==PredictionItem::Type) // PredictionItem	
	{
		pos = QPointF(-90,50);
	}
	else if(item->type()==PartitioningItem::Type) // PartitioningItem	
	{
		pos = QPointF(200,-75);
	}	

	for(uint i=0; i<50;i++)
	{
		QPointF p = origin+pos;
		if( qgraphicsitem_cast<SDFInputDataItem*>(itemAt(p)) ||
		    qgraphicsitem_cast<CSVInputDataItem*>(itemAt(p)) ||
		    qgraphicsitem_cast<ModelItem*>(itemAt(p)) ||
		    qgraphicsitem_cast<FeatureSelectionItem*>(itemAt(p)) ||
		    qgraphicsitem_cast<ValidationItem*>(itemAt(p)) ||
		    qgraphicsitem_cast<PredictionItem*>(itemAt(p)) ||
		    qgraphicsitem_cast<PartitioningItem*>(itemAt(p)) ||
		    qgraphicsitem_cast<InputPartitionItem*>(itemAt(p))	)
		{
			pos+=QPointF(200,0);
		}
		else
		{
			break;
		}
	}
	return pos;	
}


void DataItemScene::dropEvent(QGraphicsSceneDragDropEvent* event)
{
	if(view->name != "view") return;
	
	// if the "drag" was very short, is was no real drag at all, so there is nothing being dropped!
	/// -> process mouse clicks instead of drops :
	if(main_window->drag_start_time.now().getSeconds()-main_window->drag_start_time.getSeconds() < main_window->min_drag_time) 
	{
		int type = main_window->dragged_item->type();
		if(type==PredictionItem::Type)
		{
			((PredictionItem*)main_window->dragged_item)->showPredictionPlotter();
		}
		else if(type==SDFInputDataItem::Type || type==CSVInputDataItem::Type || 
		type==InputPartitionItem::Type )
		{
			((InputDataItem*)main_window->dragged_item)->showPlotter();
		}
		else if(type==ModelItem::Type)
		{
			((ModelItem*)main_window->dragged_item)->showPlotter();
		}
// 		else if(type==ValidationItem::Type)
// 		{
// 			((ValidationItem*)main_window->dragged_item)->showPlotter();
// 		}
		return;
	}
	
	QPointF pos = event->scenePos();
	
	/// if more than one item is selected, move all those items and do nothing else
	QList<QGraphicsItem*> sel_items = selectedItems();
	if(sel_items.size()>1)
	{
		QPointF drag_start = main_window->dragged_item->pos();
		QPointF translation = pos-drag_start;
		for(QList<QGraphicsItem*>::iterator it=sel_items.begin(); it!=sel_items.end(); it++)
		{
			(*it)->setPos((*it)->pos()+translation);
			set<Edge*> edges=((DataItem*)(*it))->inEdges();
			for(set<Edge*>::iterator it2=edges.begin(); it2!=edges.end();it2++)
			{
				(*it2)->adjust();
			}
			edges=((DataItem*)(*it))->outEdges();
			for(set<Edge*>::iterator it2=edges.begin(); it2!=edges.end();it2++)
			{
				(*it2)->adjust();
			}
		}
		QGraphicsScene::dropEvent(event);
		update();
		view->update();
		return;			
	}
	
	/// move DataItems within pipeline-scene
	if (event->mimeData()->hasFormat("application/x-csvinputitemdata") || 	
		event->mimeData()->hasFormat("application/x-predictiondata") ||
		event->mimeData()->hasFormat("application/x-predictiondata") ||
		event->mimeData()->hasFormat("application/x-partitemdata") ||
	        event->mimeData()->hasFormat("application/x-inputpartitemdata"))
	{
		main_window->dragged_item->setPos(pos.x(),pos.y());
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
				{
					item = main_window->createModel(item,input_item_at_pos);
					pos = input_item_at_pos->pos();
				}
				else if(csv_input_item_at_pos)
				{
					item = main_window->createModel(item,csv_input_item_at_pos);	
					pos = csv_input_item_at_pos->pos();
				}
					
				item->setView(view);
				addItem(item);
				item->setPos(pos.x(),pos.y());
				item->setPos(pos + getOffset(pos,item));
				if (input_item_at_pos)
				{
					Edge* edge = new Edge(input_item_at_pos, item);
					addItem(edge);
					item->addToPipeline();
				}
				else if(csv_input_item_at_pos)
				{
					Edge* edge = new Edge(csv_input_item_at_pos, item);
					addItem(edge);
					item->addToPipeline();
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
		
				model_copy = new ModelItem(*model_item_at_pos);				
				model_copy->setInputDataItem(model_item_at_pos->inputDataItem());
				item = main_window->createFeatureSelection(item, model_copy, model_item_at_pos);
				pos = model_item_at_pos->pos();
				item->setView(view);
				addItem(item);
				pos += getOffset(pos,item);
				item->setPos(pos);
				
				model_copy->setSaveAttribute(false);
				addItem(model_copy);
				model_copy->addToPipeline();	
				model_copy->setPos(pos + getOffset(pos,model_copy));
		
				Edge* edge = new Edge(item, model_copy);
				addItem(edge);
				Edge* edge2 = new Edge(model_item_at_pos, item);
				addItem(edge2);
				item->addToPipeline();
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
	else if (event->mimeData()->hasFormat("application/x-validationdata") || event->mimeData()->hasFormat("application/x-EXTvalidationdata")) 
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
				if (!model_item_at_pos)
				{
					//item = main_window->createValidation(item);
					//main_window->addDisconnectedItem(item);
					QMessageBox::information(view," ","Please drag the Validation onto a Model within your pipeline!");
					return;
				}
				if(item->getValidationType()==6 && !model_item_at_pos->getRegistryEntry()->regression)
				{
					QMessageBox::information(view," ","Calculation of standard deviations of coefficients can only be done for regression models!");
					return;
				}
								
				item = main_window->createValidation(item, model_item_at_pos);
				item->setView(view);
				pos = model_item_at_pos->pos();
				addItem(item);
				item->setPos(pos + getOffset(pos,item));
				Edge* edge = new Edge(model_item_at_pos, item);
				addItem(edge);
				item->addToPipeline();
				if(event->mimeData()->hasFormat("application/x-EXTvalidationdata"))
				{
				   createExternalValPipeline(model_item_at_pos,item);
				}
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
						
						if(model_item_at_pos) /// create PrecitionItem
						{
							PredictionItem* pred_item = main_window->createPrediction(item,model_item_at_pos);
							addItem(pred_item);
							pos = model_item_at_pos->pos();
							pred_item->setPos(pos+getOffset(pos,pred_item));
							addItem(item);
							QPointF p0 = QPointF(-90,-50);
							item->setPos(pos+p0);
				
							Edge* edge = new Edge(model_item_at_pos, pred_item);
							addItem(edge);
							model_item_at_pos->addPredictionInputEdge(edge);
							Edge* edge2 = new Edge(item, model_item_at_pos);
							addItem(edge2);	
							DottedEdge* dedge = new DottedEdge(item, pred_item);
							addItem(dedge);
							pred_item->setDottedEdge(dedge);
							pred_item->addToPipeline();
						}
						else /// create SDFInputDataItem
						{
							addItem(item);
							item->setPos(pos);
						}
						
						item->addToPipeline();
						
						String p = path.toStdString(); 
						p  = p.substr(0,p.find_last_of("/"));
						main_window->setLastUsedPath(p);
					}
					else if (match_txt.exactMatch(path) || match_csv.exactMatch(path))
					{
						CSVInputDataItem* csv_item;
						SDFInputDataItem* input_item_at_pos = qgraphicsitem_cast<SDFInputDataItem*>(itemAt(pos));
		
						if (input_item_at_pos) /// create CSVInputDataItem and append descriptors
						{
							csv_item = main_window->createCSVInput(path);
							csv_item->setData(input_item_at_pos->data());
							csv_item->setAppend(true);				
							addItem(csv_item);
							pos = input_item_at_pos->pos();
							QPointF p0 = QPointF(input_item_at_pos->width()+10,0);
							csv_item->setPos(pos+p0);
							//QGraphicsItemGroup* group=new QGraphicsItemGroup(input_item_at_pos,this); 
							//groups_.push_back(group);
							//group->addToGroup(csv_item);
							Edge* edge = new Edge(input_item_at_pos, csv_item);
							addItem(edge);
							csv_item->addToPipeline();
							input_item_at_pos->appendCSVDescriptors(csv_item);
						}

						else /// create CSVInputDataItem (no appending of desc.)
						{
							csv_item = main_window->createCSVInput(path);
							addItem(csv_item);
							csv_item->setPos(pos.x(),pos.y());
							csv_item->addToPipeline();
						}
						String p = path.toStdString(); 
						p  = p.substr(0,p.find_last_of("/"));
						main_window->setLastUsedPath(p);
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
 	main_window->updatePipelineScene();
 	view->update();
	
}  // END of  void DataItemScene::dropEvent(QGraphicsSceneDragDropEvent* event)



void DataItemScene::createExternalValPipeline(ModelItem* model_item, ValidationItem* val_item)
{
	list<DataItem*> pipe;
	DataItem* item = model_item;
	while(item!=NULL && item->type()!=SDFInputDataItem::Type && item->type()!=CSVInputDataItem::Type)
	{
		pipe.push_front(item);
		item=(*item->inEdges().begin())->sourceNode();
	}
	if(item->type()!=CSVInputDataItem::Type && item->type()!=SDFInputDataItem::Type)
	{	
		// TODO: convert to exception which is to be caught within dropEvent()
		QMessageBox::critical(view,"Not possible", "This pipeline does not have an input source. Therefore partitioning of input data and subsequent nested cross validation cannot be done!");
		return;
	}
	InputDataItem* data_item = (InputDataItem*)item;
	
	double frac=val_item->getValFraction();
	uint folds = val_item->numOfNCVFolds();
	PartitioningItem* partitioner = new PartitioningItem(data_item,view,folds,frac);
	QPointF p0 = data_item->pos();
	addItem(partitioner);
	partitioner->setPos(p0+getOffset(p0,partitioner));
	partitioner->addToPipeline();
	val_item->setPartitioner(partitioner);
	Edge* e = new Edge(data_item,partitioner);
	addItem(e);
	
	for(uint i=0;i<folds;i++)
	{
		InputPartitionItem* train_part = new InputPartitionItem(0,partitioner); // training-partition of fold i
		addItem(train_part);
		QPointF p0 = partitioner->pos();
		train_part->setPos(p0+getOffset(p0,train_part));
		train_part->addToPipeline();
		Edge* e0 = new Edge(partitioner,train_part);
		addItem(e0);
		
		InputPartitionItem* test_part = new InputPartitionItem(1,partitioner); // test-partition of fold i
		test_part->addToPipeline();
		
		// add fold to PartitioningItem
		partitioner->addFold(make_pair(train_part,test_part));
		
		DataItem* source_item = train_part;
		ModelItem* input_model=0;
		FeatureSelectionItem* new_fs=0;
		bool fs_created=0;
		DataItem* new_item=0;
		ModelItem* target_model=0; // modelItem for which prediction is to be done
		
		for(list<DataItem*>::iterator it=pipe.begin();it!=pipe.end();it++)
		{
			new_item=0;
			item=*it;
			if(item->type()==ModelItem::Type)
			{
				ModelItem* m_item=qgraphicsitem_cast<ModelItem*>(item);
				ModelItem* new_model = new ModelItem(*m_item); // copies model-/kernel-parameters and save_attribute_ 
				if(model_item==m_item) target_model = new_model;
				new_model->setInputDataItem(train_part);
		
				input_model = new_model;
			
				if(fs_created)
				{
					new_fs->setModelItem(new_model); // set output of FS
				}
				new_model->addToPipeline();
				new_item=new_model;
				new_item->change();
				fs_created=0;
			}
			else if(item->type()==FeatureSelectionItem::Type)
			{
				FeatureSelectionItem* fs_item=qgraphicsitem_cast<FeatureSelectionItem*>(item);
				new_fs = new FeatureSelectionItem(*fs_item); // copies FS-parameters
				new_fs->setInputModelItem(input_model);  // set input of FS
				new_fs->change();
				new_item=new_fs;
				new_item->addToPipeline();
				fs_created=1;
			}
			else
			{
				cout<<"item type not found!!"<<endl<<flush;
			}
			QPointF p0 = source_item->pos();
			new_item->setPos(p0+getOffset(p0,new_item));
			addItem(new_item);
			Edge* e = new Edge(source_item,new_item);
			addItem(e);
			source_item=new_item;
		}
		
		addItem(test_part);
		QPointF pos = target_model->pos();
		p0 = QPointF(-90,-50);
		test_part->setPos(pos+p0);
		Edge* e1 = new Edge(partitioner,test_part);
		addItem(e1);
		
		// create PredictionItem using the test-partition of the current external fold
		PredictionItem* pred_item = new PredictionItem(test_part, target_model, view);
		//pred_item->setModelItem(target_model);
		addItem(pred_item);
		pred_item->setPos(pos+getOffset(pos,pred_item));
		Edge* edge = new Edge(target_model, pred_item);
		addItem(edge);
		target_model->addPredictionInputEdge(edge);
		Edge* edge2 = new Edge(test_part, target_model);
		addItem(edge2);	
		DottedEdge* dedge = new DottedEdge(test_part, pred_item);
		addItem(dedge);
		pred_item->setDottedEdge(dedge);
		pred_item->addToPipeline();
		
		// add PredictionItem (which also calculates quality statistic) to nested cross validation item, so that the average predictive quality can later be calculated
		val_item->addExternalFoldValidation(pred_item);
	}
}


// overloading QGraphicsScene::dragMoveEvent() since in older Qt-version it only allows dropping onto items!
void DataItemScene::dragMoveEvent(QGraphicsSceneDragDropEvent* event)
{
	event->accept();
}



// currently only used for left mouse clicks == start of drag
// right clicks are currently being handled by DataItem::contextMenuEvent(QContextMenuEvent *event)
void DataItemScene::mousePressEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
	if(mouseEvent->button()!=Qt::LeftButton) return;

	QPointF p = mouseEvent->scenePos();
	QGraphicsItem* item = itemAt(p);
	if(!item || item->type()<DataItem::Type) return; // if there is no item below mouse cursor, do nothing!

	main_window->drag_start_time = main_window->drag_start_time.now();
	
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
