#include <BALL/APPLICATIONS/QuEasyViz/dataItemScene.h>
#include <BALL/APPLICATIONS/QuEasyViz/edge.h>
#include <BALL/APPLICATIONS/QuEasyViz/mainWindow.h>
#include <BALL/APPLICATIONS/QuEasyViz/exception.h>
#include <BALL/APPLICATIONS/QuEasyViz/inputPartitionItem.h>

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
		pos = QPointF(200,-200);
	}	

	for(uint i=0; i<50;i++)
	{
		QPointF p = origin+pos;
		if((dynamic_cast<DataItem*>(itemAt(p))))
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
	uint type = 0;
	if(main_window->dragged_item!=NULL) type=main_window->dragged_item->type();
	
	if(view->name!="view" || ((main_window->dragged_item==NULL||type<QVariant::UserType) && !event->mimeData()->hasUrls())) 
	{
		main_window->dragged_item=NULL;
		return;
	}

	// if the "drag" was very short, is was no real drag at all, so there is nothing being dropped!
	/// -> process mouse clicks instead of drops :
	if(main_window->drag_start_time.now().getSeconds()-main_window->drag_start_time.getSeconds() < main_window->min_drag_time) 
	{
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
		main_window->dragged_item=NULL;
		return;
	}
	
	QPointF pos = event->scenePos();
	
	/// move DataItems within pipeline-scene (if no drop from another DataItemScene)
	if (main_window->dragged_item!=NULL && main_window->drag_source==view->name)
	{
		// if more than one item is selected, move all those items and do nothing else
		QList<QGraphicsItem*> sel_items = selectedItems();
		if(sel_items.size()>1)
		{
			QPointF drag_start = main_window->dragged_item->pos();
			QPointF translation = pos-drag_start;
			for(QList<QGraphicsItem*>::iterator it=sel_items.begin(); it!=sel_items.end(); it++)
			{
				DataItem* item = (DataItem*)(*it);
				item->setPos((*it)->pos()+translation);
				set<Edge*> edges=item->inEdges();
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
		else  // move only one time
		{
			main_window->dragged_item->setPos(pos.x(),pos.y());
		}
		main_window->dragged_item=NULL;
	}
	
	/// create SDFInputDataItem or CSVInputDataItem
	else if ((event->mimeData()->hasUrls()))
	{
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
			try
			{
				
				path = url.toLocalFile();
				QFileInfo file_info(path);
				if (file_info.isFile())
				{
					SDFInputDataItem* existing_sdf_item = dynamic_cast<SDFInputDataItem*>(itemAt(pos));
					CSVInputDataItem* existing_csv_item = dynamic_cast<CSVInputDataItem*>(itemAt(pos));
					
					if (match_sd.exactMatch(path))
					{
						SDFInputDataItem* item=NULL;
	
						/// replace input-item by new SD-item
						if(existing_sdf_item!=NULL || (existing_csv_item!=NULL&&existing_csv_item->append()==0))
						{
							InputDataItem* existing_input_item;
							if(existing_sdf_item!=NULL) existing_input_item=existing_sdf_item;
							else existing_input_item = existing_csv_item;
							
							QMessageBox box;
							box.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
							box.button(QMessageBox::Ok)->setText("Replace");
							box.setText("Do you want to replace the input data of this pipeline?!");
							box.setWindowTitle("Replace input data?");
							box.setDefaultButton(QMessageBox::Cancel);
							bool replace = (box.exec()==QMessageBox::Ok);
							
							if(replace)
							{
								item = main_window->createSDFInput(path);
								QPointF pos = existing_input_item->pos();
								addItem(item);
								item->setPos(pos);
								item->replaceItem(existing_input_item);
							}
							else 
							{
								return; // "cancel" was chosen, so do nothing!
							}
						}
						
						else
						{
							item = main_window->createSDFInput(path);
							ModelItem* model_item_at_pos = qgraphicsitem_cast<ModelItem*>(itemAt(pos)); 
							PredictionItem* pred_item = NULL;
							
							if(model_item_at_pos) /// create PrecitionItem
							{
								pred_item = main_window->createPrediction(item,model_item_at_pos);
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
								
								int stat = main_window->chooseValidationStatisticDialog(model_item_at_pos);
								pred_item->setValidationStatistic(stat);
							}
							else /// create SDFInputDataItem
							{
								addItem(item);
								item->setPos(pos);
							}
							item->addToPipeline();
							if(pred_item) pred_item->addToPipeline();
						}
	
						String p = path.toStdString(); 
						p  = p.substr(0,p.find_last_of("/"));
						main_window->setLastUsedPath(p);
					}
					else if (match_txt.exactMatch(path) || match_csv.exactMatch(path))
					{
						CSVInputDataItem* csv_item=0;
						
						// replace input-item by new CSV-item
						bool replace=0; 
						if(existing_sdf_item)
						{
							QMessageBox box;
							box.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
							box.button(QMessageBox::Ok)->setText("Replace");
							box.setText("Do you want to replace the input data of this pipeline or do you want to append descriptors to the current data set?!");
							box.button(QMessageBox::Cancel)->setText("Append");
							box.setWindowTitle("Replace input data?");
							box.setDefaultButton(QMessageBox::Cancel);
							replace = (box.exec()==QMessageBox::Ok);
							
							if(replace)
							{
								QPointF pos=existing_sdf_item->pos();
								csv_item = main_window->createCSVInput(path);
								addItem(csv_item);
								csv_item->setPos(pos);
								csv_item->replaceItem(existing_sdf_item);
							}
						}
						else if(existing_csv_item)
						{
							// existing_csv_item is appended to a sdf-item
							if(existing_csv_item->append()==1) return;
							
							QMessageBox box;
							box.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
							box.button(QMessageBox::Ok)->setText("Replace");
							box.setText("Do you want to replace the input data of this pipeline?!");
							box.setWindowTitle("Replace input data?");
							box.setDefaultButton(QMessageBox::Cancel);
							replace = (box.exec()==QMessageBox::Ok);
							
							if(replace)
							{
								QPointF pos=existing_csv_item->pos();
								csv_item = main_window->createCSVInput(path);
								addItem(csv_item);
								csv_item->setPos(pos);
								csv_item->replaceItem(existing_csv_item);
							}
							else 
							{
								return; // "cancel" was chosen, so do nothing!
							}
						}
						
						if(!replace)
						{	
							csv_item=main_window->createCSVInput(path);
							ModelItem* model_item_at_pos = dynamic_cast<ModelItem*>(itemAt(pos)); 
							PredictionItem* pred_item = NULL;
							
							if (existing_sdf_item) /// create CSVInputDataItem and append descriptors
							{
								csv_item->setData(existing_sdf_item->data());
								csv_item->setAppend(true);				
								addItem(csv_item);
								pos = existing_sdf_item->pos();
								QPointF p0 = QPointF(existing_sdf_item->width()+10,0);
								csv_item->setPos(pos+p0);
								
								Edge* edge = new Edge(existing_sdf_item, csv_item);
								addItem(edge);
								
								// place the new CSV-item into the pipeline at the correct position, i.e. after the SD-item or after the last CSV-item already appended to it
								DataItem* d;
								if(existing_sdf_item->additional_descriptors_.size()>0)
								{
									d = existing_sdf_item->additional_descriptors_.back();
								}
								else d = existing_sdf_item;
								main_window->all_items_pipeline_.insertAfter(csv_item,d);
								main_window->csv_input_pipeline_.insert(csv_item);
								
								existing_sdf_item->appendCSVDescriptors(csv_item);
							}
							else if(model_item_at_pos) /// create PrecitionItem
							{
								pred_item = main_window->createPrediction(csv_item,model_item_at_pos);
								addItem(pred_item);
								pos = model_item_at_pos->pos();
								pred_item->setPos(pos+getOffset(pos,pred_item));
								addItem(csv_item);
								QPointF p0 = QPointF(-90,-50);
								csv_item->setPos(pos+p0);
					
								Edge* edge = new Edge(model_item_at_pos, pred_item);
								addItem(edge);
								model_item_at_pos->addPredictionInputEdge(edge);
								Edge* edge2 = new Edge(csv_item, model_item_at_pos);
								addItem(edge2);	
								DottedEdge* dedge = new DottedEdge(csv_item, pred_item);
								addItem(dedge);
								pred_item->setDottedEdge(dedge);
								
								int stat = main_window->chooseValidationStatisticDialog(model_item_at_pos);
								pred_item->setValidationStatistic(stat);
								
								csv_item->addToPipeline();
								pred_item->addToPipeline();
							}
							else /// create CSVInputDataItem (no appending of desc.)
							{
								addItem(csv_item);
								csv_item->setPos(pos.x(),pos.y());
								csv_item->addToPipeline();
							}
						}
						String p = path.toStdString(); 
						p  = p.substr(0,p.find_last_of("/"));
						main_window->setLastUsedPath(p);
					}
				}
			}
			catch(InvalidInputDataItem) {}
			catch(InvalidPredictionItem) {} // if 'cancel' has been clicked within the dialogs
		}
	}

	/// create ModelItem
	else if (type==ModelItem::Type) 
	{
		if(main_window->drag_source!="model_list") return;
		
		ModelItem* item;
		SDFInputDataItem* input_item_at_pos = qgraphicsitem_cast<SDFInputDataItem*>(itemAt(pos));
		CSVInputDataItem* csv_input_item_at_pos = qgraphicsitem_cast<CSVInputDataItem*>(itemAt(pos));
		ModelItem* model_item_at_pos = qgraphicsitem_cast<ModelItem*>(itemAt(pos));
		
		item = (ModelItem*)main_window->dragged_item;
		
		DataItem* source = dynamic_cast<DataItem*>(itemAt(pos));
		
		try
		{
			if(!input_item_at_pos && !csv_input_item_at_pos && !model_item_at_pos)
			{
				QMessageBox::information(view," ","Please drag the Model onto a SD-item, a CSV-item or anto another model within your pipeline!");
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
			else if(model_item_at_pos)
			{
				
				item = main_window->createModel(item,model_item_at_pos->inputDataItem());
				
				// ModelItem::execute() will copy the descriptors before training 'item'
				item->descriptor_source_model_ = model_item_at_pos;
				pos = model_item_at_pos->pos();
			}
	
			item->setView(view);
			addItem(item);
			item->setPos(pos.x(),pos.y());
			item->setPos(pos + getOffset(pos,item));
			
			Edge* edge = new Edge(source, item);
			addItem(edge);
			item->addToPipeline();
		}
		catch(InvalidModelItem) {}  // if 'cancel' has been clicked within the dialogs

		main_window->drag_source = "view"; // set back to "default"
	}

	/// create feature selection item
	else if (type==FeatureSelectionItem::Type) 
	{
		FeatureSelectionItem* item;
		ModelItem* model_item_at_pos = dynamic_cast<ModelItem*>(itemAt(pos));
		
		if(main_window->drag_source!="fs_list") return;
		
		item = (FeatureSelectionItem*)main_window->dragged_item;
		
		//create a FeatureSelectionItem that is connected to a ModelItem
		ModelItem* model_copy = NULL;
		try
		{
			if (!model_item_at_pos)
			{
				QMessageBox::information(view," ","Please drag the FeatureSelection onto a Model within your pipeline!");
				return;	
			}
			if(item->getType()==4 && !model_item_at_pos->getRegistryEntry()->regression)
			{
				QMessageBox::information(view," ","Removal of features uncorrelated with reponse variable\ncan only be done for regression models!");
				return;
			}
			if(item->getType()==5 && (!model_item_at_pos->getRegistryEntry()->regression||model_item_at_pos->getRegistryEntry()->kernel))
			{
				QMessageBox::information(view," ","Removal of insignificant coefficients\ncan only be done for linear regression models!");
				return;
			}
			
			model_copy = new ModelItem(*model_item_at_pos);				
			model_copy->setInputDataItem(model_item_at_pos->inputDataItem());
			item = main_window->createFeatureSelection(item, model_copy, model_item_at_pos);
			pos = model_item_at_pos->pos();
			item->setView(view);
			addItem(item);
			item->addToPipeline();
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
		}
		catch(InvalidFeatureSelectionItem)
		{
			delete model_copy;
		}
		main_window->drag_source = "view"; // set back to "default"
     	}

	/// create validation item
	else if (type==ValidationItem::Type) 
	{
		ValidationItem* item;
		ModelItem* model_item_at_pos = dynamic_cast<ModelItem*>(itemAt(pos));
		
		if(main_window->drag_source!="val_list") return;
		
		item = (ValidationItem*)main_window->dragged_item;
		// create a new validation item that is to be placed into the view-widget	
		try
		{
			if (!model_item_at_pos)
			{
				QMessageBox::information(view," ","Please drag the Validation onto a Model within your pipeline!");
				return;
			}
			if(item->getValidationType()==6 && (!model_item_at_pos->getRegistryEntry()->regression || model_item_at_pos->getRegistryEntry()->kernel))
			{
				QMessageBox::information(view," ","Calculation of standard deviations of coefficients can only be done for linear regression models!");
				return;
			}
							
			item = main_window->createValidation(item, model_item_at_pos);
			item->setView(view);
			pos = model_item_at_pos->pos();
			addItem(item);
			item->setPos(pos + getOffset(pos,item));
			Edge* edge = new Edge(model_item_at_pos, item);
			addItem(edge);
			if(item->getValidationType()==5)
			{
				createExternalValPipeline(model_item_at_pos,item);
			}
			item->addToPipeline();
		}
		catch(InvalidValidationItem)
		{
			//delete item;
		}

		main_window->drag_source = "view"; // set back to "default"
     	} 

	else 
	{	
		event->ignore();
	}
	QGraphicsScene::dropEvent(event);
 	main_window->updatePipelineScene();
 	view->update();
	main_window->dragged_item=NULL;
}  // END of  void DataItemScene::dropEvent(QGraphicsSceneDragDropEvent* event)



void DataItemScene::createExternalValPipeline(ModelItem* model_item, ValidationItem* val_item)
{
	list<DataItem*> pipe;
	DataItem* item = model_item;
	DataItem* first_model = NULL;
	while(item!=NULL && item->type()!=SDFInputDataItem::Type && item->type()!=CSVInputDataItem::Type)
	{
		pipe.push_front(item);
		first_model = item;
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
	QPointF p0 = first_model->pos();
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
		DataItem* last_item=0;
		
		for(list<DataItem*>::iterator it=pipe.begin();it!=pipe.end();it++)
		{
			last_item=new_item;
			new_item=0;
			item=*it;
			if(item->type()==ModelItem::Type)
			{
				ModelItem* m_item=dynamic_cast<ModelItem*>(item);
				ModelItem* new_model = new ModelItem(*m_item); // copies model-/kernel-parameters and save_attribute_ 
				if(model_item==m_item) target_model = new_model;
				new_model->setInputDataItem(train_part);
		
				input_model = new_model;
			
				if(fs_created)
				{
					new_fs->setModelItem(new_model); // set output of FS
				}
				
				if(m_item->descriptor_source_model_!=NULL && last_item!=NULL && last_item->type()==ModelItem::Type)
				{
					// connect new ModelItem to previously created ModelItem if descriptors are to be copied...
					new_model->descriptor_source_model_ = dynamic_cast<ModelItem*>(last_item);
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
		pred_item->setValidationStatistic(val_item->getValidationStatistic());
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
