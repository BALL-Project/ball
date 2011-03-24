/* predictor.C
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


void startPrediction(ifstream& in, QSARData* q, String* data_filename)
{
	PredictionConfiguration conf = ConfigIO::readPredictionConfiguration(&in);
		
	if(conf.done) return; // stop processing this section
	
	bool created_data_object=0;
	if(q==NULL || data_filename==NULL || conf.data!=*data_filename)
	{
		if(q==NULL)
		{
			q = new QSARData;
			created_data_object=1;
		}
		q->readFromFile(conf.data);
		if(data_filename) *data_filename = conf.data;
	}
		
	Registry reg;
	Model* m;
	String model_type;
	ifstream model_input(conf.model.c_str()); // read model-abbreviation
	if(!model_input)
	{
		cout<<"Error: Model-file '"<<conf.model<<"' does not exist!!"<<endl;
		return;
	}
	getline(model_input,model_type);
	getline(model_input,model_type);
	model_type = model_type.getField(0,"\t");
	model_input.close();
		
	RegistryEntry* entry = reg.getEntry(model_type);
	bool regression = entry->regression;
					
	if(!entry->kernel)
	{
		m = (*entry->create)(*q);
	}
	else
	{	
		// parameters irrelevant; will be overwritten by those read from file
		m = (*entry->createKernel1)(*q,1,1, -1);
	}
				
	m->readFromFile(conf.model.c_str()); // do NOT train again (done by ModelCreator) !!
	
	m->model_val->selectStat(conf.statistic);
	m->model_val->testInputData(1);	// calculate prediction quality
	m->model_val->setCVRes(m->model_val->getFitRes());
	m->model_val->saveToFile(conf.output);
	
	ofstream out(conf.output.c_str(),ios::app);
	out<<endl<<"[Predictions]"<<endl;
	int no_act = q->getNoResponseVariables();
	int no_cols = no_act;
	if(conf.print_expected)
	{
		no_cols*=2;
		out<<"# format: predition0, expectation0, ..."<<endl;
	}
	out<<"expected_values = "<<conf.print_expected<<endl;
	out<<"dimensions = "<<q->getNoSubstances()<<" "<<no_cols<<endl;
		
	for(int i=0;i<q->getNoSubstances();i++)
	{
		vector<double>* v = q->getSubstance(i); // get UNcentered descriptor-vector of test compound 
			
		Eigen::VectorXd res = m->predict(*v,1); // transform val. data according to centering of training data
		delete v;
		
		vector<double>* exp = q->getActivity(i); // get UNcentered response value vector
		for(int j=1; j<=res.getSize();j++)
		{
			out<<res(j)<<"\t";
			if(conf.print_expected)
			{
				out<<(*exp)[j-1]<<"\t";
			}
		}
		delete exp;	
		out<<endl;
	}
	
	if(created_data_object) delete q;
	delete m;
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
	
	for(int i=0;!in.eof();i++) // read ALL Predictor sections
	{
		for(int j=0;!in.eof();j++) // skip everthing until the beginning of the next Validator-section
		{
			getline(in,line);
			if(!line.hasPrefix("[Predictor]")) continue;
			else break;
		}
		if(!line.hasPrefix("[Predictor]")) break; // there are no (more) Validator-sections!
		ConfigIO::putbackLine(&in,line);
	
		startPrediction(in,NULL,NULL);
	}	
}
#endif
