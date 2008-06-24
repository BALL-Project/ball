#include <BALL/APPLICATIONS/QSAR_GUI/inputDataItemIO.h>
#include <BALL/APPLICATIONS/QSAR_GUI/mainWindow.h>

#include <QtGui/QMessageBox>


using namespace BALL::VIEW;


InputDataItemIO::InputDataItemIO(DataItemView* view)
{
	view_ = view;
}

void InputDataItemIO::writeConfigSection(SDFInputDataItem* sd_item, ofstream& out)
{
	String activity_string;
	String tmp;
	SortedList<int> activities = sd_item->activityValues();
	activities.front();
	while(activities.hasNext())
	{
		int a = activities.next();
		activity_string += " "+ String(a);
	}
	out << "[InputReader]" << "\n";
	out << "sd_file = "<< sd_item->filename().toStdString() << "\n";
	out << "read_sd_descriptors = "<< sd_item->useSDProperties() << "\n";
	out << "activity_IDs = "<< activity_string << "\n";
	out << "center_data = "<< sd_item->centerData() << "\n";
	out << "center_response = "<< sd_item->centerY() << "\n";
	
	list<CSVInputDataItem*>* csv_items = sd_item->getConnectedCSVItems();
	for(list<CSVInputDataItem*>::iterator it=csv_items->begin(); it!=csv_items->end(); it++)
	{
		out << "csv_file = " << (*it)->filename().toStdString()<<"\n";
		out << "csv_separator = "<<"\""<<(*it)->getSeperator()<<"\"\n";
		out << "csv_desc_labels = "<<(*it)->getDescriptorLabels()<<"\n";
		out << "csv_compound_labels = "<< (*it)->getCompoundLabels()<<"\n";
		out << "csv_no_response = "<< (*it)->getNoResponseVariables()<<"\n";
		
		written_csv_.insert(*it);
	}
	out << "output = " << sd_item->savedAs().toStdString()  << "\n";
	out<<"\n";
}


void InputDataItemIO::writeConfigSection(CSVInputDataItem* csv_item, ofstream& out)
{
	// if csv_item has been written to a config section because it is connected to a SDFInputDataItem, do nothing!
	if(written_csv_.find(csv_item)!=written_csv_.end()) return;
	
	out << "[InputReader]" << "\n";
	out << "csv_file = " << csv_item->filename().toStdString()<<"\n";
	out << "csv_separator = "<<"\""<<csv_item->getSeperator()<<"\"\n";
	out << "csv_desc_labels = "<<csv_item->getDescriptorLabels()<<"\n";
	out << "csv_compound_labels = "<<csv_item->getCompoundLabels()<<"\n";
	out << "csv_no_response = "<<csv_item->getNoResponseVariables()<<"\n";
	out << "center_data = "<< csv_item->centerData() << "\n";
	out << "center_response = "<< csv_item->centerY() << "\n";
	out << "output = " << csv_item->savedAs().toStdString() << "\n";
	out << "\n";
	
	written_csv_.insert(csv_item);
}


void InputDataItemIO::writeConfigSection(PartitioningItem* item, ofstream& out)
{
	out << "[InputPartitioner]" << "\n";
	out << "input_file = " << item->getInputItem()->savedAs().toStdString()<<"\n";
	out << "ID = "<<item->getID()<<"\n";
	out << "val_fraction = " << item->getValFraction()<<"\n";
	out << "no_folds = "<<item->getNoFolds()<<"\n";
	out << "\n";
}


void InputDataItemIO::readPartitionerSection(String& configfile_section, map<String, DataItem*>& filenames_map, list<pair<double,double> >* item_positions)
{
	String input_file="";
	double val_fraction=0;
	uint no_folds = 0;
	int ID = -1;
	
	istringstream input;
	input.str(configfile_section);	
	String line;
	while(input)
	{
		getline(input,line);
		line.trimLeft();
		
		if(line=="" || line.hasPrefix("#") || line.hasPrefix("//") || line.hasPrefix("%"))
		{
			continue;
		}
				
		if(line.hasPrefix("input_file"))
		{
			input_file = ((String)line.after("=")).trimLeft();
		}
		else if(line.hasPrefix("val_fraction"))
		{
			val_fraction = ((String)line.after("=")).trimLeft().toDouble();
		}
		else if(line.hasPrefix("no_folds"))
		{
			no_folds = ((String)line.after("=")).trimLeft().toInt();
		}
		else if(line.hasPrefix("ID"))
		{
			ID = ((String)line.after("=")).trimLeft().toInt();
		}		
	}
	
	if(input_file==""||val_fraction==0||no_folds==0||ID==-1)
	{
		throw BALL::Exception::GeneralException(__FILE__,__LINE__,"InputPartitioner reading error","\"input_file\", \"val_fraction\", \"no_folds\" and \"ID\" must be specified within the config section!!");
	}
	
	InputDataItem* input_item;
	map<String, DataItem*>::iterator it=filenames_map.find(input_file);
	if(it==filenames_map.end())
	{
		throw BALL::Exception::GeneralException(__FILE__,__LINE__,"InputPartitioner reading error","InputItem from which partitions are to be created could not be found!!");
	}
	input_item=(InputDataItem*)it->second;
	PartitioningItem* partitioner = new PartitioningItem(input_item,view_,no_folds,val_fraction);
	partitioner->setID(ID);
	view_->scene()->addItem(partitioner);
	partitioner->addToPipeline();
	if(item_positions!=0 && item_positions->size()>0)
	{
		pair<double,double> pos = item_positions->front();
		item_positions->pop_front();
		partitioner->setPos(pos.first,pos.second);
	}
	Edge* e = new Edge(input_item,partitioner);
	view_->scene()->addItem(e);
	
	for(uint i=0; i<no_folds;i++)
	{
		InputPartitionItem* train_part = new InputPartitionItem(0,partitioner);
		view_->scene()->addItem(train_part);
		train_part->addToPipeline();
		filenames_map.insert(make_pair(train_part->savedAs().toStdString(),train_part));
// 		if(item_positions!=0 && item_positions->size()>0)
// 		{
// 			pair<double,double> pos = item_positions->front();
// 			item_positions->pop_front();
// 			train_part->setPos(pos.first,pos.second);
// 		}
		Edge* e0 = new Edge(partitioner,train_part);
		view_->scene()->addItem(e0);
		
		InputPartitionItem* test_part = new InputPartitionItem(1,partitioner);
		view_->scene()->addItem(test_part);
		test_part->addToPipeline();
		filenames_map.insert(make_pair(test_part->savedAs().toStdString(),test_part));
// 		if(item_positions!=0 && item_positions->size()>0)
// 		{
// 			pair<double,double> pos = item_positions->front();
// 			item_positions->pop_front();
// 			test_part->setPos(pos.first,pos.second);
// 		}
		Edge* e1 = new Edge(partitioner,test_part);
		view_->scene()->addItem(e1);
		
		partitioner->addFold(make_pair(train_part,test_part));
	}	
}

void InputDataItemIO::readConfigSection(String& configfile_section, map<String, DataItem*>& filenames_map, list<pair<double,double> >* item_positions)
{
	istringstream input;
	input.str(configfile_section);
		
	String line;
	getline(input,line);
	line.trimLeft();
	if(!line.hasPrefix("[InputReader]"))
	{
		if(line.hasPrefix("[InputPartitioner]"))
		{
			readPartitionerSection(configfile_section, filenames_map, item_positions);
			return;
		}
		throw BALL::Exception::GeneralException(__FILE__,__LINE__,"Input reading error","The given section is no input section!");
	}
	
	String sd_file=""; String output=""; String separate_activity_file="";
	bool center_data=0; bool center_y=0; bool read_sd_descriptors=0;
	double val_fraction=0;
	SortedList<int> activities;
	
	vector<String> csv_file; vector<String> csv_sep;
	vector<bool> csv_desc_labels; vector<bool> csv_compound_labels;
	vector<int> csv_no_response;
	
	
	while(input)
	{
		getline(input,line);
		line.trimLeft();
		
		if(line=="" || line.hasPrefix("#") || line.hasPrefix("//") || line.hasPrefix("%"))
		{
			continue;
		}		
		if(line.hasPrefix("sd_file"))
		{
			sd_file = ((String)line.after("=")).trimLeft();
		}
		else if(line.hasPrefix("output"))
		{
			output = ((String)line.after("=")).trimLeft();
		}
		else if(line.hasPrefix("separate_activity_file"))
		{
			separate_activity_file = ((String)line.after("=")).trimLeft().toBool();
		}
		else if(line.hasPrefix("activity_IDs"))
		{
			String act = ((String)line.after("=")).trimLeft();
			for(uint i=0; i<act.countFields(" ");i++)
			{
				activities.push_back(act.getField(i).toInt());
			}
		}
		else if(line.hasPrefix("center_data"))
		{
			center_data = ((String)line.after("=")).trimLeft().toBool();
		}
		else if(line.hasPrefix("center_response"))
		{
			center_y = ((String)line.after("=")).trimLeft().toBool();
		}
		else if(line.hasPrefix("external_val_fraction"))
		{
			val_fraction = ((String)line.after("=")).trimLeft().toDouble();
		}
		else if(line.hasPrefix("read_sd_descriptors"))
		{
			read_sd_descriptors = ((String)line.after("=")).trimLeft().toBool();
		}
		else if(line.hasPrefix("csv_file"))
		{
			csv_file.push_back(((String)line.after("=")).trimLeft());
		}
		else if(line.hasPrefix("csv_separator"))
		{
			csv_sep.push_back(((String)((String)line.after("=")).after("\"")).before("\""));
		}
		else if(line.hasPrefix("csv_desc_labels"))
		{
			csv_desc_labels.push_back(((String)line.after("=")).trimLeft().toBool());
		}
		else if(line.hasPrefix("csv_no_response"))
		{
			csv_no_response.push_back(((String)line.after("=")).trimLeft().toInt());
		}
		else if(line.hasPrefix("csv_compound_labels"))
		{
			csv_compound_labels.push_back(((String)line.after("=")).trimLeft().toBool());
		}
		else
		{
			String mess = "Configuration command \""+line+"\" unknown!!";
			String name = "InputDataItem reading error";
			throw BALL::Exception::GeneralException(__FILE__,__LINE__,name,mess);
		}	
	}
	
	SDFInputDataItem* sd_item=0;
	if(sd_file!="")
	{
		try
		{
			sd_item = new SDFInputDataItem(sd_file.c_str(), activities,center_data, center_y, view_);
			view_->data_scene->addItem(sd_item);
			sd_item->addToPipeline();
			sd_item->setSavedAs(output.c_str());
			sd_item->useSDProperties(read_sd_descriptors);
			if(item_positions!=0 && item_positions->size()>0)
			{
				pair<double,double> pos = item_positions->front();
				item_positions->pop_front();
				sd_item->setPos(pos.first,pos.second);
				sd_item->setCenterDataFlag(center_data);
				sd_item->setCenterResponseFlag(center_y);
			}
		}
		catch(BALL::Exception::GeneralException e)
		{
			QMessageBox::critical(view_,"Error reading SD-input",e.getMessage());
		}
		filenames_map.insert(make_pair(output,sd_item));
	}
	
	uint no=csv_file.size();
	if(no!=csv_sep.size()||no!=csv_desc_labels.size()||no!=csv_compound_labels.size()||no!=csv_no_response.size())
	{
		QMessageBox::critical(view_,"Error reading CSV-inputs","Some csv-options are missing!");
	}
	try
	{
		for(uint i=0; i<no;i++)
		{
			CSVInputDataItem* csv_item;
			if(sd_item!=0)
			{
				csv_item = new CSVInputDataItem(sd_item->data());	
				csv_item->setAppend(true);
				if(i==no-1)
				{
					csv_item->setCenterDataFlag(center_data);
					csv_item->setCenterResponseFlag(center_y);
				}
				else
				{
					csv_item->setCenterDataFlag(0);
					csv_item->setCenterResponseFlag(0);
				}
				csv_item->setView(view_);
				csv_item->setFilename(csv_file[i]);
			}
			else
			{
				csv_item = new CSVInputDataItem(csv_file[i].c_str(),view_);	
				csv_item->setAppend(false);
				csv_item->setCenterDataFlag(center_data);
				csv_item->setCenterResponseFlag(center_y);
				if(i==0) filenames_map.insert(make_pair(output,csv_item));
			}					
			csv_item->setXLabelFlag(csv_desc_labels[i]);
			csv_item->setYLabelFlag(csv_compound_labels[i]);
			csv_item->setNumOfActivities(csv_no_response[i]);
			csv_item->setSeperator(csv_sep[i]);
			csv_item->setSavedAs(output.c_str());
			
 			view_->data_scene->addItem(csv_item);
			csv_item->addToPipeline();
			if(item_positions!=0 && item_positions->size()>0)
			{
				pair<double,double> pos = item_positions->front();
				item_positions->pop_front();
				csv_item->setPos(pos.first,pos.second);
			}
 			if(sd_item) 
			{
				Edge* edge = new Edge(sd_item, csv_item);
				view_->data_scene->addItem(edge);
				sd_item->appendCSVDescriptors(csv_item);
			}
		}
	}
	catch(BALL::Exception::GeneralException e)
	{
		QMessageBox::critical(view_,"Error reading CSV-input",e.getMessage());
	}
}
