/* inputReader.C
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

#include <fstream>
#include <BALL/QSAR/registry.h>
#include <BALL/QSAR/configIO.h>

using namespace BALL::QSAR;
using namespace BALL;


void startInputReading(ifstream& in, String data_path, QSARData* q, String* data_filename)
{
	InputConfiguration conf = ConfigIO::readInputConfiguration(&in);
		
	if(conf.done) return; // stop processing this section
		
	bool created_data_object=0;
	if(q==NULL || data_filename==NULL || conf.output!=*data_filename)
	{
		if(q==NULL)
		{
			q = new QSARData;
			created_data_object=1;
		}
		if(data_filename) *data_filename = conf.output;
	}
	
	q->setDataFolder(data_path.c_str());
		
	if(conf.sd_file!="") // read sd-file (and csv-table)
	{
		if(!conf.separate_activity_file)
		{
			q->readSDFile(conf.sd_file.c_str(),conf.activities,conf.read_sd_descriptors,0);
		}
		else
		{
			q->readSDFile(conf.sd_file.c_str());
		}
		uint no_csv = conf.csv_file.size();	
		for(uint i=0; i<no_csv; i++)
		{
			q->readCSVFile(conf.csv_file[i].c_str(),0,conf.csv_desc_labels[i],conf.csv_compound_labels[i],conf.csv_separator[i].c_str(),1,conf.nonnumeric_class_names);
		}
	}
	else // read csv-table only
	{
		if(conf.csv_file.size()==0 || conf.csv_file[0]=="")
		{
			cout<<"Error: sd-file and/or csv-table must be specified within config-file!"<<endl;
			return;
		}
		q->readCSVFile(conf.csv_file[0].c_str(),conf.csv_no_response[0],conf.csv_desc_labels[0],conf.csv_compound_labels[0],conf.csv_separator[0].c_str(),0,conf.nonnumeric_class_names);
	}
		
	// now obsolete 
	if(conf.validation_fraction>0)
	{
		cout<<"conf.validation_fraction is obsolete. Use InputPartitioner(-section) instead!"<<endl;
	}
		
	if(conf.center_data)
	{
		q->centerData(conf.center_y);
	}
	q->saveToFile(conf.output);
	
	if(created_data_object) delete q;
}
	

#ifndef EXT_MAIN
int main(int argc, char* argv[])
{
	if(argc<2)
	{
		cout<<"Please specify configuration file!"<<endl; 
		return 0;
	}
	
	ifstream in(argv[1]);
	if(!in)
	{
		cout<<"Configuration file '"<<argv[1]<<"' not found!"<<endl;
		return 0;
	}
	
	String line;

	// -- set data-path if enviroment-variable BALL_DATA_PATH is not set --
	String data_directory="";
	String executable_directory = argv[0];
	String sep = BALL::FileSystem::PATH_SEPARATOR;
	executable_directory = executable_directory.substr(0,executable_directory.find_last_of(sep));

	char* bdp = getenv("BALL_DATA_PATH");
	String ball_data_path = "";
	if(bdp!=NULL) ball_data_path=String(bdp);
	String path = ball_data_path+sep;
	path+= "QSAR"+sep+"atomic_electron_affinities.data";
	if(!ifstream(path.c_str()))  // use subfolder of executable's directory
	{
		data_directory = executable_directory+sep+"data"+sep;
		string test = data_directory+"atomic_electron_affinities.data";
		if(!fstream(test.c_str()))
		{
			cout<<"[Error:] Data-directory not found !!"<<endl;
		}
	}
	else
	{
		data_directory = ball_data_path+sep+"QSAR";
	}
	// -----  ----- -----  -----
	
	for(int i=0;!in.eof();i++) // read ALL InputReader section
	{		
		for(int j=0;!in.eof();j++) // skip everthing until the beginning of the next InputReader-section
		{
			getline(in,line);
			if(!line.hasPrefix("[InputReader]")) continue;
			else break;
		}
		if(!line.hasPrefix("[InputReader]")) break; // there are no (more) input-sections!
		ConfigIO::putbackLine(&in,line);
		
		startInputReading(in,data_directory);
	}
}
#endif
