#include <BALL/STRUCTURE/bondorderAssigner.h>

#ifndef ATOMTYPES_H
#include <BALL/STRUCTURE/atomtypes.h>
#endif

#ifndef VSGENERATOR_H
#include <BALL/STRUCTURE/vSgenerator.h>
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

#ifndef ALGORITHM
#include <algorithm>
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
	BondorderAssigner::BondorderAssigner(){
	}
	
	BondorderAssigner::BondorderAssigner(Molecule* mol){
		int molecule_size = mol->countAtoms();
		//initialize atoms_ with the atomindices of molecule
		//and set current_atoms_ on the same value
		for(int i=0; i< molecule_size; ++i){
			atoms_.push_back(i);
			//store index as atom property for later access
			Atom* atom = mol->getAtom(i);
			atom->setProperty("index", (int) i);
		}
		setCurrentAtoms(atoms_);
	}

	BondorderAssigner::~BondorderAssigner(){
	}
	
	//getter for private variables
	list<int> BondorderAssigner::getAtoms(){
		return atoms_;
	}

	list<int> BondorderAssigner::getCurrentAtoms(){
		return current_atoms_;
	}

	//setter for private variables
	void BondorderAssigner::setCurrentAtoms(list<int> value_list){
		current_atoms_ = value_list;
	}


	//initialize atoms_ with the atomindices of molecule
	void BondorderAssigner::InitializeAtomList(Molecule* mol){
		int molecule_size = mol->countAtoms();
		for(int i=0; i< molecule_size; ++i){
			atoms_.push_back(i);
			//store index as atom property for later access
			Atom* atom = mol->getAtom(i);
			atom->setProperty("index", (int) i);
		}
		setCurrentAtoms(atoms_);
		cout << "InitializeAtomList" << endl;
	}



	//update atom properties according to assigned_bondorder
	void BondorderAssigner::AtomPropertyUpdater(Bond* bond){
		cout << "in AtomPropertyUpdater" << endl;

		Atom* first_atom = const_cast<Atom*> (bond->getFirstAtom());
		Atom* second_atom = const_cast<Atom*> (bond->getSecondAtom());

		//if bondorder is already determined and within permitted range (1-4)
		if(bond->getProperty("assigned bondorder").getInt()!= 0 and bond->getProperty("assigned bondorder").getInt()< 5){
			cout << "bondorder:" << bond->getProperty("assigned bondorder").getInt() << endl;
			

			//update connectivity and atomic valence of first_atom
			int connectivity = first_atom->getProperty("connectivity").getInt();
			int atomic_valence = first_atom->getProperty("atomic valence").getInt();
			cout << "first_atom:" << first_atom->getElement().getName() << endl;
			cout << "index:" << first_atom->getProperty("index").getInt() << endl;
			cout << "first_connectivity_before:" << connectivity << endl;
			cout << "first_atomic_valence_before:" << atomic_valence << endl;	
			//if bonds of one Atom in bond_it still unassigned
			//if current atom is not already finished
			if(connectivity!=0){
				connectivity = connectivity -1;	
				//set properties for first_atom in bond
				first_atom->setProperty("connectivity",(int) connectivity);
				cout << "first_connectivity:" << connectivity << endl;
			}
			atomic_valence = first_atom->getProperty("atomic valence").getInt();
			//difference not allowed to be null or negative
			if(atomic_valence >= (bond->getProperty("assigned bondorder").getInt())){
				//save current value for atomic_valence in variable former_atomic_valence
				atomic_valence = atomic_valence - (bond->getProperty("assigned bondorder").getInt());
				//set properties for first_atom in bond
				first_atom->setProperty("atomic valence", (int) atomic_valence);
				cout << "first_atomic_valence:" << atomic_valence << endl;	
			}
				
			//update connectivity and atomic valence of second_atom
			connectivity = second_atom->getProperty("connectivity").getInt();
			atomic_valence = second_atom->getProperty("atomic valence").getInt();
			cout << "second_atom:" << second_atom->getElement().getName() << endl;
			cout << "index:" << second_atom->getProperty("index").getInt() << endl;
			cout << "before_second_connectivity:" << connectivity << endl;
			cout << "before_second_atomic_valence:" << atomic_valence << endl;	
			if(connectivity!=0){
				connectivity = connectivity -1;	
				//set properties for current atom in bond_it										
				second_atom->setProperty("connectivity",(int) connectivity);
				cout << "second_connectivity:" << connectivity << endl;
			}
			atomic_valence = second_atom->getProperty("atomic valence").getInt();
			if(atomic_valence >= bond->getProperty("assigned bondorder").getInt()){
				atomic_valence = atomic_valence - (bond->getProperty("assigned bondorder").getInt());
				//set properties for current Atom in bond_it
				second_atom->setProperty("atomic valence", (int) atomic_valence);	
				cout << "second_atomic_valence:" << atomic_valence << endl;	
			}
		}
		cout << "AtomPropertyUpdater" << endl;
	}
	
	//assign bondorder to special value if condition is fulfilled
	Bond* BondorderAssigner::SpecialBondAssigner(Atom* atom){
		cout << "in SpecialBondAssigner" << endl;
		Atom::BondIterator bond_it = atom->beginBond();
		Bond* bond;

		cout << "index" << atom->getProperty("index").getInt() << endl;
		cout << "connectivity:" << atom->getProperty("connectivity").getInt() << endl;
		cout << "atomic valence" << atom->getProperty("atomic valence").getInt() << endl;

		//if connectivity == atomic_valence
		for(;+bond_it;++bond_it){
			bond = &(*bond_it);
			//take care that bond still unassigned
			if(atom->getProperty("connectivity").getInt() == atom->getProperty("atomic valence").getInt() and bond_it->getProperty("assigned bondorder").getInt()==0 ){
				bond = &(*bond_it);
				bond->setProperty("assigned bondorder", (int) 1);
				cout << "assigned_bondorder:" << bond->getProperty("assigned bondorder").getInt() << endl;	
				break;
			}
		}
		cout << "SpecialBondAssigner" << endl;
		return bond;
	}
	

	//assign bondorder of the last unassigend bond of an atom to special value
	Bond* BondorderAssigner::LastBondAssigner(Atom* atom){
		cout << "in LastBondAssigner" << endl;
		Bond* bond;
		if(atom->getProperty("connectivity").getInt()==1){
			Atom::BondIterator bond_it = atom->beginBond();
			bond = &(*bond_it);
			//assignment of the last unassigned bond
			for(;+bond_it; ++bond_it){
				//if bond is still unassigned					
				if(bond_it->getProperty("assigned bondorder").getInt()==0){
					//set assigned_bondorder to atomic_valence
					bond = &(*bond_it);	
					int assigned_bondorder = atom->getProperty("atomic valence").getInt();
					bond->setProperty("assigned bondorder", (int) assigned_bondorder);
					cout << "assigned_bondorder:" << bond->getProperty("assigned bondorder").getInt() << endl;
				}
			}				
		}
		cout << "LastBondAssigner" << endl;
		return bond;
	}


	//assign bondorder to atoms with connectivity 2
	//and atoms that cannnot be assigned with SpecialBondAssigner
	//and LastBondAssigner
	Bond* BondorderAssigner::DoubleBondAssigner(Atom* atom){
		cout << "in DoubleBondAssigner" << endl;
		Atom::BondIterator bond_it = atom->beginBond();
		Bond* bond = &(*bond_it);
		if(atom->getProperty("connectivity").getInt()==2){
			cout << "connectivity = 2"<< endl;
			if(atom->getProperty("atomic valence").getInt()== 3 or atom->getProperty("atomic valence").getInt() == 4){
				cout << "atomic valence = 3 oder 4" << endl;
				for(;+bond_it;++bond_it){
					if(bond_it->getProperty("assigned bondorder").getInt()==0){	
						bond = &(*bond_it);
						bond->setProperty("assigned bondorder", (int) 2);
						break;			
					}
				}
			}
		}
		cout << "DoubleBondAssigner" << endl;
		return bond;
	}


	//test if BondorderAssigner was successfull
	bool BondorderAssigner::SuccessAssigner(Molecule* mol){
		Atomtypes atomtypes = Atomtypes(mol);
		//sum up all connectivity and atomic_valence values of all atoms
		pair<int,int> atom_properties = atomtypes.SumOfAllAtomProperties(mol);
		atomtypes.setAllAtomsConnectivity(atom_properties.first);
		atomtypes.setAllAtomsAtomicValence(atom_properties.second);
		int all_atoms_connectivity = atomtypes.getAllAtomsConnectivity();
		int all_atoms_atomic_valence = atomtypes.getAllAtomsAtomicValence();
		//if all bonds assigned ->success!
		if(all_atoms_atomic_valence ==0 and all_atoms_connectivity ==0){
			cout << "SuccessAssigner" << endl;
			return 1;
		}
		else {
			cout << "SuccessAssigner" << endl;
			return 0;		
			}
	}

	
	//assign bondorders of a molecule
	int BondorderAssigner::AssignBondorder(Molecule* mol){
		Atomtypes atomtypes = Atomtypes(mol);
		Bond* bond;
		//set properties of all atoms
		atomtypes.SetAtomProperties(mol);
		//assign atomic penalty scores for different atomtypes
		atomtypes.AtomPenalizer(mol);

		//for all atoms: sum up connectivity and atomic valence at a time
		pair <int,int> atom_property = atomtypes.SumOfAllAtomProperties(mol);
		cout <<"(connectivity, atomic valence)=" << "(" << atom_property.first << "," << atom_property.second << ")" << endl;
		
		InitializeAtomList(mol);
		list<int> atoms = getAtoms();

		cout << "atoms.size():" << atoms.size() << endl; 

		int size = 0;
		/////assign bondorder
		//try to assign bonds at first go
		while(!(size==atoms.size() )){
			size = atoms.size();
			for(list<int>::iterator pos_it= atoms.begin();pos_it!= atoms.end();){
				bool moved = false;
				Atom* atom = mol->getAtom(*pos_it);
				if(atom->getProperty("connectivity").getInt() == atom->getProperty("atomic valence").getInt()){	
					bond = SpecialBondAssigner(atom);
					AtomPropertyUpdater(bond);
				}
				if(atom->getProperty("connectivity").getInt()==1){
					bond = LastBondAssigner(atom);
					AtomPropertyUpdater(bond);
				}
				//if all bonds of atom assigned
				if(atom->getProperty("connectivity").getInt()==0 and atom->getProperty("atomic 	valence").getInt()==0 ){	
						pos_it = atoms.erase(pos_it);
						moved = true;
				}
				if(!moved){
				++pos_it;	
				}
			}
		}
			
		
		//update atoms_
		setCurrentAtoms(atoms);
		
		if(atoms.empty()){
			cout << "atoms empty" << endl;
			if(SuccessAssigner(mol)){
			//SUCCESS!!!
			//TODO return value for boaf
			cout << "ROCKAZ BABY!!!" << endl;
			return 1;
			}
			//TODO fehlerbehandlung!
			else cout << "ERROOOAAAR!" << endl;
		}
		else{
			cout << "atoms not empty" << endl;
			atomtypes.StoreSystemStatus(current_atoms_, mol);
			
			//special bond setting 
			while(!atoms.empty()){
				for(list<int>::iterator pos_it =atoms.begin();pos_it !=atoms.end();++pos_it){
					Atom* atom = mol->getAtom(*pos_it);
					if(atom->getProperty("connectivity").getInt() ==2){
						if(atom->getProperty("atomic valence").getInt() ==3 or atom->getProperty("atomic valence").getInt() ==4){
							Bond* bond = DoubleBondAssigner(atom);
							AtomPropertyUpdater(bond);
							cout << "setted bond to2" << endl;
							break;
						}
					}
				}	

				cout << "atoms.size():" << atoms.size() << endl;
				//try to assign bonds
				for(list<int>::iterator pos_it= atoms.begin();pos_it!= atoms.end();){
					bool moved = false;
					cout << "pos_it:" << *pos_it << endl;
					Atom* atom = mol->getAtom(*pos_it);
					cout << "atom:" << atom->getElement().getName() << endl;
					if(atom->getProperty("connectivity").getInt() == atom->getProperty("atomic valence").getInt()){	
						bond = SpecialBondAssigner(atom);
						AtomPropertyUpdater(bond);
					}
					if(atom->getProperty("connectivity").getInt()==1){
						bond = LastBondAssigner(atom);
						AtomPropertyUpdater(bond);
					}
					//if all bonds of atom assigned
					if(atom->getProperty("connectivity").getInt()==0 and atom->getProperty("atomic valence").getInt()==0 ){
							cout << "the killer"<< endl;
							pos_it = atoms.erase(pos_it);
							moved = true;
					}
					if(!moved){
						++pos_it;	
					}
				}
			}
			if(atoms.empty()){
				if(SuccessAssigner(mol)){
				//SUCCESS!!!
				//TODO return value for boaf
				cout << "ROCKAZ BABY!!!" << endl;
				}	
			}
			else{
				cout << "Trial and Error Test inevitable ;-( " << endl;
				return 0;
			}
		}
		return 1;
	}


}
