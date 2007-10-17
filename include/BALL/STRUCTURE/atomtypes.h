// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:

#ifndef ATOMTYPES_H
#define ATOMTYPES_H

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

	struct AtomStatus{
		//atom_state:(position,(connectivity,atomic_valence))
		pair<int,pair<int,int> > atom_state;
		int position; 
		int connectivity;
		int atomic_valence;
		//contains indices of partner_atoms concerning the considered molecule mol
		vector<int> partner_atoms;
	};

	class Atomtypes{
		public:
			Atomtypes();	
			Atomtypes(Molecule* mol);
			~Atomtypes();	
			
			//getter for private variables
			int getAllAtomsConnectivity();
			int getAllAtomsAtomicValence();
			vector< vector<int> > getAllPartnerAtoms();
			vector<vector <AtomStatus > >  getSuccessiveUnresolvedAtoms();

			//setter for private variables
			void setAllAtomsConnectivity(int value);
			void setAllAtomsAtomicValence(int value);
			void setAllPartnerAtoms(Molecule* mol);
			void setSuccessiveUnresolvedAtoms(vector<vector <AtomStatus> >);

			//Initialize successive_unresolved_atoms_ with size of given atomlist
			void InitializeSuccessiveUnresolvedAtoms(list<int> atoms);
			//set properties (connectivity, index,..) for every atom in molecule 
			void SetAtomProperties(Molecule* mol);
			//sum up all tuples (connectivity, atomic valence) of every atom 
			pair<int,int> SumOfAllAtomProperties(Molecule* mol);
			//penalize specific valence states
			//only for a given complete structure
			void AtomPenalizer(Molecule* mol);	
			//store AtomStatus at a certain point of time
			void StoreSystemStatus(list<int> atoms, Molecule* mol);
			//set atom and bond properties back to value they had at a certain point of time
			void SetBackProperties(Molecule* mol, vector<AtomStatus > unresolved_atoms);
	
		private:
			int all_atoms_connectivity_;	//sum over connectivity of all atoms
			int all_atoms_atomic_valence_;	//sum over atomic_valence of all atoms
			//vector with partner atoms of all atoms
			//vector index refers to atom index
			vector< vector<int> > all_partner_atoms_; 
			//list of atoms unresolved at a certain point of time
			//the integer time points are indices of vector<vector<AtomStatus> >
			vector<vector <AtomStatus> >  successive_unresolved_atoms_;
				
	};
}
#endif

