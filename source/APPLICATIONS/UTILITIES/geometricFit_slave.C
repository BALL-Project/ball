/** This should be spawned by geometricFit **/
#include <BALL/DOCKING/geometricFit.h>

#include <iostream>

using namespace BALL;
using namespace std;

int main(int argc, char** argv)
{
	printf("Woke up...\n");
	// this is simple...
	GeometricFit gf;
	gf.MPI_Slave_start(argc, argv);

	return 0;
}
