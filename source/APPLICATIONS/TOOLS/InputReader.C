// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/QSAR/registry.h>
#include <BALL/QSAR/configIO.h>
#include <fstream>
#include "version.h"

using namespace BALL::QSAR;
using namespace BALL;
using namespace std;

void startInputReading(InputConfiguration& conf, String data_path, QSARData* q, String* data_filename);


void startInputReading(ifstream& in, String data_path, QSARData* q, String* data_filename)
{
	InputConfiguration conf = ConfigIO::readInputConfiguration(&in);
	if(conf.done) return; // stop processing this section


	startInputReading(conf, data_path, q, data_filename);
}


void startInputReading(InputConfiguration& conf, String data_path, QSARData* q, String* data_filename)
{
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
		#ifndef EXT_MAIN
			if(conf.activity_names.size()==0)
			{
				q->readSDFile(conf.sd_file.c_str(),conf.activities,conf.read_sd_descriptors,0,0);
			}
			else
		#endif
			{
				q->readSDFile(conf.sd_file.c_str(),conf.activity_names,conf.read_sd_descriptors,0,0,1,1);
			}
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
			Log.error()<<"Error: sd-file and/or csv-table must be specified within config-file!"<<endl;
			return;
		}
		q->readCSVFile(conf.csv_file[0].c_str(),conf.csv_no_response[0],conf.csv_desc_labels[0],conf.csv_compound_labels[0],conf.csv_separator[0].c_str(),0,conf.nonnumeric_class_names);
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
	CommandlineParser par("InputReader","generate QSAR data set     ", VERSION, String(__DATE__), "QuEasy (QSAR)");
	par.registerMandatoryInputFile("i", "input sd-file");
	par.registerMandatoryOutputFile("o", "output dat-file");
	par.registerOptionalStringParameter("act","sd-property containing response values");
	par.registerFlag("sdp","use sd-properties as additional descriptors");
	par.registerFlag("no_cd","do not center descriptors");
	par.registerFlag("no_cr","do not center response values");
	par.registerOptionalInputFile("csv","input csv-file w/ additional descriptors");
	par.registerOptionalIntegerParameter("csv_nr","no. of response variables in csv-file");
	par.registerFlag("csv_cl","csv-file has compound (row) labels");
	par.registerFlag("csv_dl","csv-file has descriptor (column) labels");
	par.registerOptionalIntegerParameter("csv_sep","separator symbol in csv-file");

	String man = "This tool reads input from sd-files and generate features for QSAR analysis.\nActivity data (response values) for a training set are taken from sd-properties of the input file; the name of this property can be specified by option '-act'.\nThe following number of features will be automatically created for each molecule in your sd-file:\n\n\
    * 40 atom and bond count descriptors\n\
    * 2 connectivity indices (Balaban and Zagreb index)\n\
    * 4 partial charge descriptors\n\
    * 14 surface descriptors\n\
    * 133 topological descriptors (functional group counts)\n\nIf desired, you can also read additional descriptors from a csv-file; in this case you need to specify the file with the above options.\nOutput of this tool is a data file that can be used by other QuEasy tools (e.g. ModelCreator).";
	par.setToolManual(man);
	par.setSupportedFormats("i","sdf");
	par.setSupportedFormats("csv","csv");
	par.setSupportedFormats("o","dat");
	par.parse(argc,argv);

	InputConfiguration conf;
	conf.sd_file = par.get("i");
	conf.output = par.get("o");
	if(par.get("act")!=CommandlineParser::NOT_FOUND)
	{
		list<String> alist = par.getList("act");
		for(list<String>::iterator it=alist.begin(); it!=alist.end(); it++)
		{
			conf.activity_names.insert(*it);
		}
	}
	conf.read_sd_descriptors = 0;
	String s = par.get("sdp");
	if(s!=CommandlineParser::NOT_FOUND) conf.read_sd_descriptors = s.toBool();
	conf.center_data = 1;
	s = par.get("no_cd");
	if(s!=CommandlineParser::NOT_FOUND) conf.center_data = false;
	conf.center_y = 1;
	s = par.get("no_cr");
	if(s!=CommandlineParser::NOT_FOUND) conf.center_y = false;

	// support for reading one csv-file:
	int csv_no_response = 0;
	s = par.get("csv_nr");
	if(s!=CommandlineParser::NOT_FOUND) csv_no_response = s.toInt();
	bool csv_desc_labels = 0;
	s = par.get("csv_dl");
	if(s!=CommandlineParser::NOT_FOUND) csv_desc_labels = s.toInt();
	bool csv_compound_labels = 0;
	s = par.get("csv_cl");
	if(s!=CommandlineParser::NOT_FOUND) csv_compound_labels = s.toInt();
	String csv_separator = "\t";
	s = par.get("csv_sep");
	if(s!=CommandlineParser::NOT_FOUND) csv_separator = s;
	s = par.get("csv");
	if(s!=CommandlineParser::NOT_FOUND)
	{
		conf.csv_file.push_back(s);
		conf.csv_no_response.push_back(csv_no_response);
		conf.csv_desc_labels.push_back(csv_desc_labels);
		conf.csv_compound_labels.push_back(csv_compound_labels);
		conf.csv_separator.push_back(csv_separator);
	}

	startInputReading(conf,"",0,0);
}
#endif
