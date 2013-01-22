/* featureSelector.C
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

void startFeatureSelection(std::ifstream& in, QSARData* q, String* data_filename)
{
	FeatureSelectionConfiguration conf = ConfigIO::readFeatureSelectionConfiguration(&in);
		
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
		std::cout<<"[FeatureSelector debug-info:] QSARData object for file "<<conf.data_file<<" already in memory; not reading it again."<<std::endl;
	}
	
	Registry reg;
	Model* m;
	String model_type;	
		
	std::ifstream model_input(conf.model.c_str()); // read model-abbreviation
	if(!model_input)
	{
		std::cout<<"Error: Model-file '"<<conf.model<<"' does not exist!!"<<std::endl;
		return;
	}
	std::getline(model_input,model_type);
	std::getline(model_input,model_type);
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
			
	
	std::cout<<"  using "<<conf.statistic_name<<" to assess qualitiy of the model ... "<<std::endl;
	m->model_val->selectStat(conf.statistic);
		
	m->readFromFile(conf.model.c_str());
	FeatureSelection fs(*m);
	if(conf.quality_increase_cutoff!=-1)
	{
		fs.setQualityIncreaseCutoff(conf.quality_increase_cutoff);
	}		
	if(conf.remove_correlated || conf.feat_type==0)
	{
		fs.removeHighlyCorrelatedFeatures(conf.cor_threshold);
	}		
	if(conf.feat_type==1)
	{
		fs.forwardSelection(conf.k_fold,conf.opt);
	}
	else if(conf.feat_type==2)
	{
		fs.backwardSelection(conf.k_fold,conf.opt);
	}
	else if(conf.feat_type==3)
	{
		fs.stepwiseSelection(conf.k_fold,conf.opt);
	}
	else if(conf.feat_type==4)
	{
		fs.removeLowResponseCorrelation(conf.cor_threshold);
	}
	else if(conf.feat_type==6)
	{
		fs.twinScan(conf.k_fold,conf.opt);
	}
	if(conf.opt_model_after_fs)
	{
		m->optimizeParameters(conf.opt_k_fold);
	}
	KernelModel* km = dynamic_cast<KernelModel*>(m);
	if(km && conf.opt_kernel_after_fs)
	{
		/// search locally around current kernel parameters
		try
		{
			// specifing start-values for grid search now obsolete; grid search will automatically search locally around current kernel parameter(s)
			km->kernel->gridSearch(conf.grid_search_stepwidth, conf.grid_search_steps,conf.grid_search_recursions,conf.opt_k_fold,conf.opt/*,start_par1,start_par2*/);
		}
		catch(BALL::Exception::GeneralException e)
		{
			std::cout<<e.getName()<<" : "<<e.getMessage()<<std::endl;
			return;
		}
	}
		
	m->readTrainingData();
	m->train(); 
	m->saveToFile(conf.output);
	
	if(created_data_object) delete q;
	delete m;
}


#ifndef EXT_MAIN
int main(int argc, char* argv[])
{ 
	if(argc<2)
	{
		std::cout<<"Please specify configuration file!"<<std::endl;
		return 0;
	}
	
	std::ifstream in(argv[1]);
	if(!in)
	{
		std::cout<<"Configuration file '"<<argv[1]<<"' not found!"<<std::endl;
		return 0;
	}
	String line;
	
	for(int i=0;!in.eof();i++) // read ALL FeatureSelector sections
	{
		for(int j=0;!in.eof();j++) // skip everthing until the beginning of the next FeatureSelector-section
		{
			std::getline(in,line);
			if(!line.hasPrefix("[FeatureSelector]")) continue;
			else break;
		}
		if(!line.hasPrefix("[FeatureSelector]")) break; // there are no (more) FS-sections!
		ConfigIO::putbackLine(&in,line);
		
		startFeatureSelection(in, NULL, NULL);
	}
}
#endif
