#include <fstream>
#include <BALL/QSAR/registry.h>
#include <BALL/QSAR/configIO.h>

using namespace BALL::QSAR;
using namespace BALL;


void startInputReading(std::ifstream& in, String data_path, QSARData* q, String* data_filename)
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
	
	if(data_path!="")
	{
		q->setDataFolder(data_path.c_str());
	}
		
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
		unsigned int no_csv = conf.csv_file.size();	
		for(unsigned int i=0; i<no_csv; i++)
		{
			q->readCSVFile(conf.csv_file[i].c_str(),0,conf.csv_desc_labels[i],conf.csv_compound_labels[i],conf.csv_separator[i].c_str(),1,conf.nonnumeric_class_names);
		}
	}
	else // read csv-table only
	{
		if(conf.csv_file.size()==0 || conf.csv_file[0]=="")
		{
			std::cout<<"Error: sd-file and/or csv-table must be specified within config-file!"<< std::endl;
			return;
		}
		q->readCSVFile(conf.csv_file[0].c_str(),conf.csv_no_response[0],conf.csv_desc_labels[0],conf.csv_compound_labels[0],conf.csv_separator[0].c_str(),0,conf.nonnumeric_class_names);
	}
		
	// now obsolete 
	if(conf.validation_fraction>0)
	{
		std::cout<<"conf.validation_fraction is obsolete. Use InputPartitioner(-section) instead!"<<std::endl;
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
		
		startInputReading(in,argv[0], NULL, NULL);
	}
}
#endif
