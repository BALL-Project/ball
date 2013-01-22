/* modelCreator.C
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

void startModelCreation(std::ifstream& in, QSARData* q, String* data_filename)
{
	ModelConfiguration conf = ConfigIO::readModelConfiguration(&in);
		
	if(conf.done) return; // stop processing this section
		
	bool created_data_object=0;
	if(q==NULL || data_filename==NULL || conf.data_file!=*data_filename)
	{
		if(q==NULL)
		{
			q = new QSARData;
			created_data_object=1;
		}
		q->readFromFile(conf.data_file);
		if(data_filename) *data_filename = conf.data_file;
	}
	else
	{
		std::cout << "[ModelCreator debug-info:] QSARData object for file "<<conf.data_file<<" already in memory; not reading it again." << std::endl;
	}
	Registry reg;
	Model* model;
	bool kernel=0;
	RegistryEntry* entry = reg.getEntry(conf.model_no);
		
	if(entry->create!=NULL)
	{
		model = (*entry->create)(*q);
	}
	else
	{
		if(conf.kernel_type==0 || conf.kernel_par1==0)
		{
			std::cout<<"For kernel based model, kernel-type and kernel-parameter(s) must be specified!"<< std::endl;
			return;
		}
			
		model = (*entry->createKernel1)(*q,conf.kernel_type,conf.kernel_par1, conf.kernel_par2);
		kernel=1;
	}
		
	if(conf.model_parameters.size()>0)
	{
		model->setParameters(conf.model_parameters);
	}
	if(!conf.no_training && conf.optimize_model_parameters)
	{
		if(conf.k_fold==0)
		{
			std::cout << "'k_fold' must be set if model parameters are to be optimized!" << std::endl;
			return;
		}
		model->optimizeParameters(conf.k_fold);
	}
	if(!conf.no_training && kernel && conf.grid_search_steps>0)
	{
		if(conf.k_fold==0)
		{
			std::cout << "'k_fold' must be set if grid search is to be done!" << std::endl;
			return;
		}
		if(conf.grid_search_stepwidth==0 && conf.kernel_type!=2)
		{ 
			std::cout << "'grid_search_stepwidth' must be set if grid search is to be done!" << std::endl;
			return;
		}
		((KernelModel*)model)->kernel->gridSearch(conf.grid_search_stepwidth, conf.grid_search_steps, conf.grid_search_recursions, conf.k_fold);
	}
		
	model->readTrainingData();
	if(!conf.no_training)
	{
		try
		{
			model->train();
		}
		catch(BALL::Exception::GeneralException e)
		{ 
			std::cout << e.getMessage();
		}
	}
		
	model->saveToFile(conf.output);	
	
	if(created_data_object) delete q;
	delete model;
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
	for(int i=0;!in.eof();i++) // read ALL ModelCreator sections
	{
		for(int j=0;!in.eof();j++) // skip everthing until the beginning of the next model-section
		{
			getline(in,line);
			if(!line.hasPrefix("[ModelCreator]")) continue;
			else break;
		}
		if(!line.hasPrefix("[ModelCreator]")) break; // there are no (more) model-sections!
		ConfigIO::putbackLine(&in,line);
		
		startModelCreation(in,NULL,NULL);		
	}
}
#endif
