/* main.C
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

#define EXT_MAIN
#include "inputReader.C"
#include "inputPartitioner.C"
#include "modelCreator.C"
#include "featureSelector.C"
#include "validator.C"
#include "predictor.C"

using namespace BALL::QSAR;
using namespace BALL;


void set_fpu (unsigned int mode)
{
	asm ("fldcw %0" : : "m" (*&mode));
}


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
	
	set_fpu (0x27F);  /* enforce IEEE754 double-precision */
	
	String line;
	
	QSARData* q = new QSARData; //  try to reload data as seldom as possible...
	String data_filename = "none";
	
	for(int i=0;!in.eof();i++) // process all sections
	{
		getline(in,line);
		
		try
		{
			if(line.hasPrefix("[InputReader]"))
			{
				ConfigIO::putbackLine(&in,line);
				startInputReading(in,argv[0],q,&data_filename);
			}
			else if(line.hasPrefix("[InputPartitioner]"))
			{
				ConfigIO::putbackLine(&in,line);
				startInputPartitioning(in,q,&data_filename);
			}
			else if(line.hasPrefix("[ModelCreator]"))
			{
				ConfigIO::putbackLine(&in,line);
				startModelCreation(in,q,&data_filename);
			}
			else if(line.hasPrefix("[FeatureSelector]"))
			{
				ConfigIO::putbackLine(&in,line);
				startFeatureSelection(in,q,&data_filename);
			}
			else if(line.hasPrefix("[Validator]"))
			{
				ConfigIO::putbackLine(&in,line);
				startValidation(in,q,&data_filename);
			}
			else if(line.hasPrefix("[Predictor]"))
			{
				ConfigIO::putbackLine(&in,line);
				startPrediction(in,q,&data_filename);
			}
		}
		catch(BALL::Exception::GeneralException e)
		{
			cout<<e.getName()<<" : "<<e.getMessage()<<endl;
			continue;
		}
	}
}
