#include <BALL/APPLICATIONS/QSAR_GUI/plotter.h>
#include <iostream>
 
using namespace BALL::VIEW;
using namespace std;

Plotter::Plotter(DataItem* item)
{
	item_ = item;
}



Plotter::~Plotter()
{
	std::cout<<"Plotter::~Plotter()"<<std::endl;
}


