#include <BALL/QSAR/automaticModelCreator.h>
#include <iostream>

using namespace BALL::QSAR;
using namespace std;


int main(int argc, char* argv[])
{
	if(argc!=4)
	{
		cout<<"Please specify: <data file> <output model file>"<<endl;
		return 0;
	}

	typedef unsigned int Size;

	QSARData input_data;
	input_data.readFromFile(argv[1]);

	AutomaticModelCreator creator(&input_data);
	Model* model = creator.generateModel();

	model->saveToFile(argv[2]);
	cout<<"A model with satisfactory prediction quality has been sucessfully created and saved to file."<<endl;
	delete model;
}

