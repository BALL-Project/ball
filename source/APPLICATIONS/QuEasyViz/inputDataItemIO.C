/* inputDataItemIO.C
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

#include <BALL/APPLICATIONS/QuEasyViz/inputDataItemIO.h>
#include <BALL/APPLICATIONS/QuEasyViz/mainWindow.h>
#include <BALL/QSAR/configIO.h>

#include <QtGui/QMessageBox>


using namespace BALL::VIEW;


InputDataItemIO::InputDataItemIO(DataItemView* view)
{
	view_ = view;
}

void InputDataItemIO::writeConfigSection(SDFInputDataItem* sd_item, ofstream& out, ostringstream& item_positions, const String& directory)
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
	if(sd_item->isDone()) out<<"done = "<<1<<endl;
	
	QDir qdir(directory.c_str());
	String rel_filename = (qdir.relativeFilePath(sd_item->filename())).toStdString();
	out << "sd_file = "<< rel_filename << "\n";
	out << "read_sd_descriptors = "<< sd_item->useSDProperties() << "\n";
	out << "activity_IDs = "<< activity_string << "\n";
	out << "center_data = "<< sd_item->centerData() << "\n";
	out << "center_response = "<< sd_item->centerY() << "\n";
	if(sd_item->getNonNumericClassNames()) out << "nonnumeric_class_names = 1\n";
	
	QPointF pos = sd_item->pos();
	// save position of this item to given stream
	item_positions<<pos.x()<<"  "<<pos.y()<<endl;
	
	list<CSVInputDataItem*>* csv_items = sd_item->getConnectedCSVItems();
	for(list<CSVInputDataItem*>::iterator it=csv_items->begin(); it!=csv_items->end(); it++)
	{
		String rel_filename = (qdir.relativeFilePath((*it)->filename())).toStdString();
		out << "csv_file = " << rel_filename <<"\n";
		out << "csv_separator = "<<"\""<<(*it)->getSeperator()<<"\"\n";
		out << "csv_desc_labels = "<<(*it)->getDescriptorLabels()<<"\n";
		out << "csv_compound_labels = "<< (*it)->getCompoundLabels()<<"\n";
		out << "csv_no_response = "<< (*it)->getNoResponseVariables()<<"\n";
		
		QPointF pos = (*it)->pos();
		// save position of appended CSV-item to given stream
		item_positions<<pos.x()<<"  "<<pos.y()<<endl;
		
		written_csv_.insert(*it);
	}
	out << "output = " << sd_item->savedAs().toStdString()  << "\n";
	out<<"\n";
}


void InputDataItemIO::writeConfigSection(CSVInputDataItem* csv_item, ofstream& out, const String& directory)
{
	// if csv_item has been written to a config section because it is connected to a SDFInputDataItem, do nothing!
	if(written_csv_.find(csv_item)!=written_csv_.end()) return;
	
	QDir qdir(directory.c_str());
	String rel_filename = (qdir.relativeFilePath(csv_item->filename())).toStdString();
	out << "[InputReader]" << "\n";
	if(csv_item->isDone()) out<<"done = "<<1<<endl;
	out << "csv_file = " << rel_filename<<"\n";
	out << "csv_separator = "<<"\""<<csv_item->getSeperator()<<"\"\n";
	out << "csv_desc_labels = "<<csv_item->getDescriptorLabels()<<"\n";
	out << "csv_compound_labels = "<<csv_item->getCompoundLabels()<<"\n";
	out << "csv_no_response = "<<csv_item->getNoResponseVariables()<<"\n";
	if(csv_item->getNonNumericClassNames()) out << "nonnumeric_class_names = 1\n";
	out << "center_data = "<< csv_item->centerData() << "\n";
	out << "center_response = "<< csv_item->centerY() << "\n";
	out << "output = " << csv_item->savedAs().toStdString() << "\n";
	out << "\n";
	
	written_csv_.insert(csv_item);
}


void InputDataItemIO::writeConfigSection(PartitioningItem* item, ofstream& out, ostringstream& item_positions)
{
	out << "[InputPartitioner]" << "\n";
	if(item->isDone()) out<<"done = "<<1<<endl;
	out << "input_file = " << item->getInputItem()->savedAs().toStdString()<<"\n";
	out << "ID = "<<item->getID()<<"\n";
	out << "val_fraction = " << item->getValFraction()<<"\n";
	out << "no_folds = "<<item->getNoFolds()<<"\n";
	out << "\n";
	
	if(item_positions!=NULL)
	{
		item_positions<<item->pos().x()<<" "<<item->pos().y()<<endl;
		for(list<pair<InputPartitionItem*,InputPartitionItem*> >::iterator it= item->folds_.begin(); it!=item->folds_.end(); it++)
		{
			QPointF p = it->first->pos();
			item_positions<<p.x()<<" "<<p.y()<<endl;
			p = it->second->pos();
			item_positions<<p.x()<<" "<<p.y()<<endl;
		}
	}
}


void InputDataItemIO::readPartitionerSection(String& configfile_section, map<String, DataItem*>& filenames_map, list<pair<double,double> >* item_positions)
{
	istringstream input;
	input.str(configfile_section);	
	InputPartitioningConfiguration conf = ConfigIO::readInputPartitioningConfiguration(&input);
		
	InputDataItem* input_item;
	map<String, DataItem*>::iterator it=filenames_map.find(conf.input_file);
	if(it==filenames_map.end())
	{
		throw BALL::Exception::GeneralException(__FILE__,__LINE__,"InputPartitioner reading error","InputItem from which partitions are to be created could not be found!!");
	}
	input_item=(InputDataItem*)it->second;
	PartitioningItem* partitioner = new PartitioningItem(input_item,view_,conf.no_folds,conf.validation_fraction,conf.ID);
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
	
	for(int i=0; i<conf.no_folds;i++)
	{
		InputPartitionItem* train_part = new InputPartitionItem(0,partitioner);
		view_->scene()->addItem(train_part);
		train_part->addToPipeline();
		filenames_map.insert(make_pair(train_part->savedAs().toStdString(),train_part));
		if(item_positions!=0 && item_positions->size()>0)
		{
			pair<double,double> pos = item_positions->front();
			item_positions->pop_front();
			train_part->setPos(pos.first,pos.second);
		}
		Edge* e0 = new Edge(partitioner,train_part);
		view_->scene()->addItem(e0);
		
		InputPartitionItem* test_part = new InputPartitionItem(1,partitioner);
		view_->scene()->addItem(test_part);
		test_part->addToPipeline();
		filenames_map.insert(make_pair(test_part->savedAs().toStdString(),test_part));
		if(item_positions!=0 && item_positions->size()>0)
		{
			pair<double,double> pos = item_positions->front();
			item_positions->pop_front();
			test_part->setPos(pos.first,pos.second);
		}
		Edge* e1 = new Edge(partitioner,test_part);
		view_->scene()->addItem(e1);
		
		partitioner->addFold(make_pair(train_part,test_part));
	}	
}

void InputDataItemIO::readConfigSection(String& configfile_section, map<String, DataItem*>& filenames_map, list<pair<double,double> >* item_positions, const String& directory)
{
	istringstream input;
	input.str(configfile_section);
	
	String line;
	getline(input,line);
	if(line.hasPrefix("[InputPartitioner]"))
	{
		ConfigIO::putbackLine(&input,line);
		readPartitionerSection(configfile_section,filenames_map,item_positions);
		return;
	}
	ConfigIO::putbackLine(&input,line);
	
	InputConfiguration conf;
	try
	{
		conf = ConfigIO::readInputConfiguration(&input);
	}
	catch(BALL::Exception::GeneralException e)
	{
		QMessageBox::critical(view_,"Error reading SD-input",e.getMessage());
	}
	
	SDFInputDataItem* sd_item=0;
	if(conf.sd_file!="")
	{
		try
		{
			QDir qdir(directory.c_str());
			QString abs_filename = qdir.absoluteFilePath(conf.sd_file.c_str());
						
			sd_item = new SDFInputDataItem(abs_filename, conf.activities,conf.center_data, conf.center_y, view_);
			view_->data_scene->addItem(sd_item);
			sd_item->addToPipeline();
			sd_item->setSavedAs(conf.output.c_str());
			sd_item->useSDProperties(conf.read_sd_descriptors);
			sd_item->setNonNumericClassNames(conf.nonnumeric_class_names);
			if(item_positions!=0 && item_positions->size()>0)
			{
				pair<double,double> pos = item_positions->front();
				item_positions->pop_front();
				sd_item->setPos(pos.first,pos.second);
				sd_item->setCenterDataFlag(conf.center_data);
				sd_item->setCenterResponseFlag(conf.center_y);
			}
		}
		catch(BALL::Exception::GeneralException e)
		{
			QMessageBox::critical(view_,"Error reading SD-input",e.getMessage());
		}
		filenames_map.insert(make_pair(conf.output,sd_item));
	}

	try
	{
		uint no=conf.csv_file.size();
		QDir qdir(directory.c_str());
		for(uint i=0; i<no;i++)
		{
			CSVInputDataItem* csv_item;
			if(sd_item!=0)
			{
				csv_item = new CSVInputDataItem(sd_item->data(),view_);	
				csv_item->setAppend(true);
				if(i==no-1)
				{
					csv_item->setCenterDataFlag(conf.center_data);
					csv_item->setCenterResponseFlag(conf.center_y);
				}
				else
				{
					csv_item->setCenterDataFlag(0);
					csv_item->setCenterResponseFlag(0);
				}
				csv_item->setView(view_);
				QString abs_filename = qdir.absoluteFilePath(conf.csv_file[i].c_str());
				csv_item->setFilename(abs_filename.toStdString());
				QStringList list = abs_filename.split("/");
				csv_item->setName(list[list.size()-1]);
			}
			else
			{
				QString abs_filename = qdir.absoluteFilePath(conf.csv_file[i].c_str());	
				csv_item = new CSVInputDataItem(abs_filename,view_);	
				csv_item->setAppend(false);
				csv_item->setCenterDataFlag(conf.center_data);
				csv_item->setCenterResponseFlag(conf.center_y);
				if(i==0) filenames_map.insert(make_pair(conf.output,csv_item));
			}
			csv_item->setNonNumericClassNames(conf.nonnumeric_class_names);
			csv_item->setXLabelFlag(conf.csv_desc_labels[i]);
			csv_item->setYLabelFlag(conf.csv_compound_labels[i]);
			csv_item->setNumOfActivities(conf.csv_no_response[i]);
			csv_item->setSeperator(conf.csv_separator[i]);
			csv_item->setSavedAs(conf.output.c_str());
			
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
