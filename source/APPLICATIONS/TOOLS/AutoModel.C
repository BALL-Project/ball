#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/QSAR/automaticModelCreator.h>
#include <iostream>
#include "version.h"

using namespace BALL;
using namespace BALL::QSAR;
using namespace std;


int main(int argc, char* argv[])
{
	CommandlineParser par("AutoModel","automatically find best QSAR model", VERSION, String(__DATE__), "QuEasy (QSAR)");
	par.registerMandatoryInputFile("i", "input data-file");
	par.registerMandatoryOutputFile("o", "output model file");
	par.registerOptionalDoubleParameter("min_quality", "minimal desired quality (default: 0.3)", 0.3);
	par.setParameterRestrictions("min_quality",0,1);

	String man = "This tool tries to automatically find the best QSAR model for a given data set. \n\nIt therefore applies nested validation, including feature selection, for each available model-type. The model with the best nested prediction quality is saved to the specified output file. However, if the best obtained nested prediction quality is smaller than the value specified by '-min_quality', an error will be shown and no model will be saved.";
	par.setToolManual(man);
	par.setSupportedFormats("i","dat");
	par.setSupportedFormats("o","mod");
	par.parse(argc,argv);

	QSARData input_data;
	input_data.readFromFile(par.get("i"));
	double min_quality = 0.3;

	if (input_data.getNoResponseVariables() < 1)
	{
		Log.error() << "[Error:] QSAR data file supplied for automatic model generation does not contain any response variables !!" << std::endl << "Model Creation aborted." << std::endl;
		return 1;
	}

	String s = par.get("min_quality");
	if(s!=CommandlineParser::NOT_FOUND)
	{
		min_quality = s.toDouble();
	}

	AutomaticModelCreator creator(&input_data);
	creator.setMinQuality(min_quality);
	Model* model = creator.generateModel();

	if(model)
	{
		model->saveToFile(par.get("o"));
		Log.level(20) << "A model with satisfactory prediction quality has been sucessfully created and saved to file." << endl;
		delete model;
	}
}
