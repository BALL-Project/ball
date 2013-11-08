#include <fstream>
#include <BALL/QSAR/registry.h>
#include <BALL/QSAR/featureSelection.h>
#include <BALL/QSAR/configIO.h>
#include <BALL/SYSTEM/path.h>

#define EXT_MAIN
#include "inputReader.C"
#include "inputPartitioner.C"
#include "modelCreator.C"
#include "featureSelector.C"
#include "validator.C"
#include "predictor.C"

#ifdef BALL_COMPILER_MSVC
#define WINDOWS_LEAN_AND_MEAN
#include <windows.h>
#endif

using namespace BALL::QSAR;
using namespace BALL;


void set_fpu (unsigned int mode)
{
#if defined(BALL_COMPILER_GXX) || defined(BALL_COMPILER_LLVM)
	asm ("fldcw %0" : : "m" (*&mode));
#elif defined(BALL_COMPILER_MSVC)
	// TODO: implement!
	//__asm ("fldcw %0" : : "m" (*&mode));
#endif
}


#ifndef BALL_OS_WINDOWS
int main(int argc, char **argv)
{
#else
int WINAPI WinMain(HINSTANCE, HINSTANCE, PSTR cmd_line, int)
{
	int argc = __argc;
	char** argv = __argv;
#endif
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
	
	set_fpu (0x27F);  /* enforce IEEE754 double-precision */
	
	String line;
	
	QSARData* q = new QSARData; //  try to reload data as seldom as possible...
	String data_filename = "none";

	// -- set data-path if enviroment-variable BALL_DATA_PATH is not set --
	Path p;
	String executable_directory = argv[0];
	String sep = BALL::FileSystem::PATH_SEPARATOR;
	executable_directory = executable_directory.substr(0,executable_directory.find_last_of(sep));
	String file = "QSAR"+sep+"atomic_electron_affinities.data";
	String dir = p.find(file);
	String data_folder = "";
	if(dir=="")
	{
		data_folder = executable_directory+sep+"data"+sep;
	}
	// -----  -----
	
	for(int i=0;!in.eof();i++) // process all sections
	{
		getline(in,line);
		
		try
		{
			if(line.hasPrefix("[InputReader]"))
			{
				ConfigIO::putbackLine(&in,line);
				startInputReading(in,data_folder,q,&data_filename);
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
			std::cout<<e.getName()<<" : "<<e.getMessage()<<std::endl;
			continue;
		}
	}
}
