#include "../include/MolecularDynamicsContainer.h"

#include <BALL/VIEW/WIDGETS/editableScene.h>
#include <BALL/VIEW/WIDGETS/molecularStructure.h>

namespace BALL
{
	namespace VIEW
	{

		MolecularDynamicsContainer::MolecularDynamicsContainer()
		{
		}

		void MolecularDynamicsContainer::minimize()
		{
			// Note
			// There is another function called minimization
			// MolecularStructure::runMinimization
			

		
			try{

					EditableScene * es = EditableScene::getInstance(0);
					if (es != 0)
						es->optimizeStructure();
			}
			catch(Exception::GeneralException& e)
			{

				String txt = " Minimization  aborted because of an unexpected exception";
				
				Log.error() << txt << ":" << std::endl;
				Log.error() << e << std::endl;
			}

			
			
			
		}

		
		void MolecularDynamicsContainer::runMDSimulation()
		{

			try{
					MolecularStructure * ms = MolecularStructure::getInstance(0);
					
					if(ms != 0)
						ms->MDSimulation();
			}
			catch(Exception::GeneralException& e)
			{

				String txt = " MD Simulation  aborted because of an unexpected exception";
				
				Log.error() << txt << ":" << std::endl;
				Log.error() << e << std::endl;
			}

		}


		void MolecularDynamicsContainer::stopSimulation()
		{
			
			try{

				MainControl * mc =  getMainControl();
				
				if((!mc->isBusy()) &&(mc!= 0 ) )
					mc->stopSimulation();
			}
			catch(Exception::GeneralException& e)
			{

				String txt = " Stopping simulation  aborted because of an unexpected exception";
				
				Log.error() << txt << ":" << std::endl;
				Log.error() << e << std::endl;
			}



		}


		void MolecularDynamicsContainer::runSinglePointCalculation()
		{
			
			try {

					MolecularStructure * ms = MolecularStructure::getInstance(0);
					
					
					if(ms != 0)
						ms->calculateForceFieldEnergy();
			}
			catch(Exception::GeneralException& e)
			{

				String txt = " Single Point Calculation  aborted because of an unexpected exception";
				
				Log.error() << txt << ":" << std::endl;
				Log.error() << e << std::endl;
			}

		}
		

	}

}


