// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:

#ifndef VSGENERATOR_H
#define VSGENERATOR_H

#ifndef ATOMTYPES_H
#include "atomtypes.h"
#endif

#ifndef BONDORDERASSIGNER_H
#include "bondorderAssigner.h"
#endif

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
	class VSgenerator{
	
		public:
			VSgenerator();
			VSgenerator(Molecule* mol);
			~VSgenerator();
			
			//setter for private variables
			void setValenceStates(vector<pair<int, Molecule*> >);
			//getter for private variables
			vector<pair<int, Molecule* > > getValenceStates();
			//stores for every atom its possible atomic valences 
			//and the corresponding possible atomic penalty scores in a vector 
			//the vector then is stored in an other vector at the atom's position in the molecule
			void GenerateAPS(Molecule* mol);
			//prepare all conditions for the generation process
			void SetupGenerator(Molecule* mol);
			//create all valence states within a certain cutoff 
			vector<pair<int,Molecule* > > GenerateValenceStates(Molecule* mol);

			//vector with vectors of pairs out of the possible atomic valences 
			//and its corresponding atomic penalty scores
			//the vector index refers to the atom's position in the molecule
			vector<vector<pair<int,int> > > atomic_penalty_scores_;
			
		private:
			
			
			//vector with all generated valence states of molecule
			//integer value = total penalty score of the corresponding valence state
			vector<pair<int, Molecule* > > valence_states_;
				
				
	};
}
#endif

