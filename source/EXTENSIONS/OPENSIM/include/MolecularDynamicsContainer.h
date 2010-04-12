#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/atomContainer.h>
#include <BALL/VIEW/MODELS/standardColorProcessor.h>
#include <BALL/VIEW/KERNEL/modularWidget.h>
#include <BALL/VIEW/KERNEL/mainControl.h>

#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/VIEW/KERNEL/threads.h>

namespace BALL
{

	namespace VIEW
	{

		class MolecularDynamicsContainer
		{
			

		 protected:

				
			public:

				MolecularDynamicsContainer();


				void minimize();

				void runMDSimulation();

				void stopSimulation();

				void runSinglePointCalculation();

				
		};
		
	}
}


