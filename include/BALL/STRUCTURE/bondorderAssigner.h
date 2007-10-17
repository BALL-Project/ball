// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:

#ifndef BONDORDERASSIGNER_H
#define BONDORDERASSIGNER_H

#ifndef IOSTREAM
#include <iostream>
#endif

#ifndef PAIR_H
#include <pair.h>
#endif

#ifndef VECTOR_H
#include <vector.h>
#endif

#ifndef SET_H
#include <set.h>
#endif

#ifndef LIST_H
#include <list.h>
#endif

#ifndef MATH_H
#include <math.h>
#endif

//////// Kernel Classes
#ifndef BALL_KERNEL_SYSTEM_H 
#include <BALL/KERNEL/system.h>
#endif

#ifndef BALL_KERNEL_MOLECULE_H
#include <BALL/KERNEL/molecule.h>
#endif

#ifndef BALL_KERNEL_RESIDUE_H
#include <BALL/KERNEL/residue.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
#include <BALL/KERNEL/atom.h>
#endif

#ifndef BALL_KERNEL_MOLECULEITERATOR_H
#include <BALL/KERNEL/moleculeIterator.h>
#endif

#ifndef BALL_KERNEL_RESIDUEITERATOR_H
#include <BALL/KERNEL/residueIterator.h>
#endif

#ifndef BALL_KERNEL_ATOMITERATOR_H
#include <BALL/KERNEL/atomIterator.h>
#endif

#ifndef BALL_KERNEL_PTE_H
#include <BALL/KERNEL/PTE.h>
#endif

#ifndef BALL_KERNEL_BOND_H
#include <BALL/KERNEL/bond.h>
#endif

#ifndef BALL_KERNEL_STANDARDPREDICATES_H
#include <BALL/KERNEL/standardPredicates.h>
#endif

#ifndef BALL_QSAR_RINGPERCEPTIONPROCESSOR_H
#include <BALL/QSAR/ringPerceptionProcessor.h>
#endif

//reading and writing of HyperChem files
#ifndef BALL_FORMAT_HINFILE_H
#include <BALL/FORMAT/HINFile.h>
#endif

//#include <BALL/KERNEL/property.h>
//#include <BALL/KERNEL/processor.h>


namespace BALL {

	struct AtomStatus;
	class Atomtypes;
	class TrialAndErrorTester;

	class BondorderAssigner{
		public:
			BondorderAssigner();
			BondorderAssigner(Molecule* mol);
			~BondorderAssigner();
	
			//getter for private variables
			list<int> getAtoms();
			list<int> getCurrentAtoms();
	
			//setter for private variables
			void setCurrentAtoms(list<int> value_list);
			
			//initialize atoms_ with the atomindices of molecule
			void InitializeAtomList(Molecule* mol);
			//update atom properties according to assigned_bondorder
			void AtomPropertyUpdater(Bond*);
			//assign bondorder to special value if condition is fulfilled
			Bond* SpecialBondAssigner(Atom*);
			//assign bondorder of the last unassigend bond of an atom to special value
			Bond* LastBondAssigner(Atom*);
			//assign bondorder to atoms with connectivity 2
			//and atoms that cannnot be assigned with SpecialBondAssigner
			//and LastBondAssigner
			Bond* DoubleBondAssigner(Atom*);
			//test if BondorderAssigner was successfull
			bool SuccessAssigner(Molecule*);
			//assign bondorders of a molecule
			int AssignBondorder(Molecule* mol);
			
		private:
			//list with vector-positions of all atoms to work on
			list<int> atoms_;	
			list<int> current_atoms_;
	};
}

#endif
