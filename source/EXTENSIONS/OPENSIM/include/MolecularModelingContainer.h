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

		class MolecularModelingContainer
		{
			
			
		 protected:

				enum{
					MMFF94,
					CHARMM,
					AMBER
				};

			public:

				MolecularModelingContainer();

				
			    void saturateFullWithHydrogens();

				void ChangeForceField(Size force_field);


				void runCreateNewMolecule();

					
				
		};
		
	}
}


