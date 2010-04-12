#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/atomContainer.h>
#include <BALL/VIEW/MODELS/standardColorProcessor.h>
#include <BALL/VIEW/KERNEL/modularWidget.h>
#include <BALL/VIEW/KERNEL/mainControl.h>

#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/VIEW/KERNEL/threads.h>
#include <BALL/SYSTEM/mutex.h>


namespace BALL
{
	namespace VIEW
	{

		class MolecularStructureContainer
		{
			friend class OpenSimPlugin;
			
			public:

				MolecularStructureContainer();

				AtomContainer* getAtomContainer();
				
				Index addAtom(const String& Element, const Vector3& position, 
				              float radius, const ColorRGBA& color);

				Index addBondByAtomIndex(Index atom1, Index atom2, Size order);

				
				void removeAtomByIndex(Index atom_index);

			
				void removeBondByAtomIndex( Index bond_index,
											Index atom_index_one, 
											Index atom_index_two);

				void updateAtomByIndex( Index index, const String& element,
										const Vector3& position);

			
				void updateBondByAtomIndex( Index atom_index_one, 
											Index atom_index_two,Size order);

			
				void selectAtom(Index atom_index);
				void deselectAtom(Index atom_index);

				void selectBond(Index atom_index_one,Index atom_index_two);
				void deselectBond(Index atom_index_one,Index atom_index_two);

		 protected:
				HashMap<Index, Handle> index_to_atom_;
				HashMap<Handle, Index> atom_to_index_;

				HashMap<Handle, Atom *> handle_to_atom_;

				HashMap<Index, Handle> index_to_bond_;
				HashMap<Handle, Index> bond_to_index_;

				HashMap<Handle, Bond *> handle_to_bond_;

								
				Index next_atom_index_;
				Index next_bond_index_;

				ReadWriteLock readWriteLock_;
		};
		
	}
}


