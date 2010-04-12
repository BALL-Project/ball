#include "../include/MolecularModelingContainer.h"
#include <BALL/VIEW/WIDGETS/editableScene.h>
#include <BALL/VIEW/WIDGETS/molecularStructure.h>


namespace BALL
{
	namespace VIEW
	{

		MolecularModelingContainer::MolecularModelingContainer()
		{
		}

		
		void MolecularModelingContainer::saturateFullWithHydrogens()
		{
		
			try {

					EditableScene * es = EditableScene::getInstance(0);
					if (es != 0 )
						es->saturateWithHydrogens();
			}
			catch(Exception::GeneralException& e)
			{

				String txt = "Saturateion-Full With Hydrogens aborted because of an unexpected exception";
				
				Log.error() << txt << ":" << std::endl;
				Log.error() << e << std::endl;
			}
		}
		
		void MolecularModelingContainer::ChangeForceField(Size force_field)
		{
			try {

					if(force_field == MolecularModelingContainer::MMFF94)
					{

						MolecularStructure * ms = MolecularStructure::getInstance(0);
						if(ms != 0)
							ms->chooseAmberFF();
					}
					else if(force_field == MolecularModelingContainer::CHARMM)
					{
						
						MolecularStructure * ms = MolecularStructure::getInstance(0);
						if(ms != 0)
							ms->chooseCharmmFF();

					}
					else if(force_field == MolecularModelingContainer::MMFF94)
					{

						MolecularStructure * ms = MolecularStructure::getInstance(0);
						if(ms != 0)
							ms->chooseMMFF94();
					}

			}
			catch(Exception::GeneralException& e)
			{

				String txt = " Setting force field  aborted because of an unexpected exception";
				
				Log.error() << txt << ":" << std::endl;
				Log.error() << e << std::endl;
			}
		}

		void MolecularModelingContainer::runCreateNewMolecule()
		{
			
			try {
					EditableScene * es = EditableScene::getInstance(0);
					if (es != 0)
						es->createNewMolecule();
			}
			catch(Exception::GeneralException& e)
			{

				String txt = "  Creating new molecule  aborted because of an unexpected exception";
				
				Log.error() << txt << ":" << std::endl;
				Log.error() << e << std::endl;
			}
		}
	}
}



		