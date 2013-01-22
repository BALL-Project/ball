/* inputPartitioner.C
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
#include <BALL/QSAR/featureSelection.h>
#include <BALL/QSAR/configIO.h>

using namespace BALL::QSAR;
using namespace BALL;

void startInputPartitioning(std::ifstream& in, QSARData* q, String* data_filename)
{
	InputPartitioningConfiguration conf = ConfigIO::readInputPartitioningConfiguration(&in);
		
	if(conf.done) return; // stop processing this section
		
	bool created_data_object=0;
	if(q==NULL || data_filename==NULL || conf.input_file!=*data_filename)
	{
		if(q==NULL)
		{
			q = new QSARData;
			created_data_object=1;
		}
		q->readFromFile(conf.input_file);
		if(data_filename) *data_filename = conf.input_file;
	}
		
	// -- create prefix for output-files ---
	String output_prefix = conf.input_file;
	int index = output_prefix.find_first_of(".");
	if(index!=(int)string::npos)
	{
		output_prefix=output_prefix.substr(0,index);
	}
	if(conf.ID>0)
	{
		output_prefix+="_"+String(conf.ID);
	}
	// -----------
		
	for(int i=0; i<conf.no_folds; i++)
	{	
		vector<QSARData*> sets = q->generateExternalSet(conf.validation_fraction); // length==2
			
		if(q->isDataCentered())
		{
			bool center_y = q->isResponseCentered();
			sets[0]->centerData(center_y); // train-partition
			sets[1]->centerData(center_y); // test-partition
		}
	
		String train_name = output_prefix+"_TRAIN"+String(i)+".dat";
		String test_name = output_prefix+"_TEST"+String(i)+".dat";
		sets[0]->saveToFile(train_name);
		sets[1]->saveToFile(test_name);
	}
	
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
	
	for(int i=0;!in.eof();i++) // read ALL InputReader section
	{		
		for(int j=0;!in.eof();j++) // skip everthing until the beginning of the next InputReader-section
		{
			getline(in,line);
			if(!line.hasPrefix("[InputPartitioner]")) continue;
			else break;
		}
		if(!line.hasPrefix("[InputPartitioner]")) break; // there are no (more) input-sections!
		ConfigIO::putbackLine(&in,line);
		
		startInputPartitioning(in,NULL,NULL);
	}
}
#endif
