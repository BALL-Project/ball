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

				Index hashAtom(Atom* new_atom);
				Index hashBond(Bond* new_bond);

				Index getAtomIndex(Atom const* atom) const;
				Index getBondIndex(Bond const* bond) const;

				Index addBondByAtomIndex(Index atom1, Index atom2, Size order);

				
				void unhashAtom(Index atom_index);
				void unhashBond(Index bond_index);

				bool needsUpdate(Index atom_index);

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
				HashMap<Index, Atom*> index_to_atom_;
				HashMap<Atom*, Index> atom_to_index_;

				HashMap<Index, Bond*> index_to_bond_;
				HashMap<Bond*, Index> bond_to_index_;

				Index next_atom_index_;
				Index next_bond_index_;

				HashMap<Index, PreciseTime> atom_hash_times_;

				mutable ReadWriteLock readWriteLock_;
		};
		
	}
}
