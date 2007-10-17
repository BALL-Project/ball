
#include <BALL/STRUCTURE/atomtypes.h>

#ifndef BONDORDERASSIGNER_H
#include <BALL/STRUCTURE/bondorderAssigner.h>
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
	
	Atomtypes::Atomtypes(){
	}
		
	Atomtypes::Atomtypes(Molecule* mol){
		all_atoms_connectivity_ = 100;
		all_atoms_atomic_valence_ = 100;

		//initialize all_partner_atoms_: size = molecule_size
		int molecule_size = mol->countAtoms();
		vector<int> tmp;
		for(int i = 0; i < molecule_size;++i){
			all_partner_atoms_.push_back(tmp);
		}
	}

	Atomtypes::~Atomtypes(){
	}

	//getter for private variables
	int Atomtypes::getAllAtomsConnectivity(){
		return all_atoms_connectivity_;
	}
	
	int Atomtypes::getAllAtomsAtomicValence(){
		return all_atoms_atomic_valence_;
	}

	vector< vector<int> > Atomtypes::getAllPartnerAtoms(){
		return all_partner_atoms_;
	}

	vector<vector <AtomStatus> >  Atomtypes::getSuccessiveUnresolvedAtoms(){
		return successive_unresolved_atoms_;
	}

	//setter for private variables
	void Atomtypes::setAllAtomsConnectivity(int value){
		all_atoms_connectivity_ = value;
	}
	
	void Atomtypes::setAllAtomsAtomicValence(int value){
		all_atoms_atomic_valence_ = value;
	}

	void Atomtypes::setAllPartnerAtoms(Molecule* mol){
		BondorderAssigner bondorderAssigner = BondorderAssigner(mol);
		list<int> atoms = bondorderAssigner.getCurrentAtoms();
		AtomIterator atom_it = mol->beginAtom();
		//get all partner atoms of atom_it
		for(;+atom_it;++atom_it){
			int atom_index = atom_it->getProperty("index").getInt();
			Atom::BondIterator bond_it = atom_it->beginBond();
			for(;+bond_it;++bond_it){
				const Atom* atom_const = &(*atom_it);
				Atom* partnerAtom = bond_it->getPartner(*atom_const);
				int partner_index = partnerAtom->getProperty("index").getInt();
				//store index of partnerAtom in vector partner_atoms
				all_partner_atoms_[atom_index].push_back(partner_index);	
			}
		}	
		cout << "setAllPartnerAtoms" << endl;
	}

	void Atomtypes::setSuccessiveUnresolvedAtoms(vector<vector <AtomStatus> > successive_unresolved_atoms){
		successive_unresolved_atoms_ = successive_unresolved_atoms;
	}


	//Initialize successive_unresolved_atoms_ with size of given atomlist
	void Atomtypes::InitializeSuccessiveUnresolvedAtoms(list<int> atoms){
		int numberOfAtoms = atoms.size();	
		vector<vector<AtomStatus> > init_unresolved_atoms;
		vector<AtomStatus> tmp;
		for(int i = 0 ;i < numberOfAtoms;++i){
			init_unresolved_atoms.push_back(tmp);
		}
		setSuccessiveUnresolvedAtoms(init_unresolved_atoms);
	}


	//set properties for every atom in molecule 
	void Atomtypes::SetAtomProperties(Molecule* mol){
			
		//iterates over all atoms/bonds in the observed molecule
		AtomIterator atom_it;
		Atom::BondIterator bond_it;
		
		atom_it = mol->beginAtom();	
	
		//for all atoms in Molecule i
		for(;+atom_it; ++atom_it){
	
			//initialize
			int connectivity = atom_it->countBonds();
			int atomic_valence =0;
			
			// atomproperty connectivity_
			atom_it->setProperty("connectivity", (int) connectivity);
			
			bond_it = atom_it->beginBond();
			int bondorder = 0;
			//for all bonds of this atom
			for (;bond_it != atom_it->endBond();++bond_it){
				bondorder = bond_it->getOrder();
				atomic_valence = atomic_valence + bondorder;
			}
			
			//set atomproperty atomic valence
			atom_it->setProperty("atomic valence", (int) atomic_valence);
		}
	}
	int atomic_penalty_score = 100;
	
	//sum up all tuples (connectivity_, atomic valence) of every atom 
	pair<int,int> Atomtypes::SumOfAllAtomProperties(Molecule* mol){
		int all_atoms_connectivity = 0;
		int all_atoms_atomic_valence = 0;
	
		//iterates over all atoms in the observed molecule
		AtomIterator atom_it;	
		atom_it = mol->beginAtom();
	
		//for all atoms: sum connectivity_ and atomic valence at a time
		for(;+atom_it;++atom_it){
			all_atoms_connectivity = all_atoms_connectivity + (atom_it->getProperty("connectivity").getInt());
			all_atoms_atomic_valence = all_atoms_atomic_valence + (atom_it->getProperty("atomic valence").getInt());			
		}
		pair<int,int> atom_properties;
		atom_properties.first = all_atoms_connectivity;
		atom_properties.second = all_atoms_atomic_valence;

		cout << "SetAtomProperties" << endl;		

		return atom_properties;
	}
			
	
	//penalize specific valence states
	//only for a given complete structure
	void Atomtypes::AtomPenalizer(Molecule* mol){
	
		const Atom* partnerAtom;	
		const Atom* secPartnerAtom;	
		const Atom* otherPartnerAtom;	
		AtomIterator atom_it;	//iterates over all atoms in the observed molecule
		Bond bond;	//bond of the observed atom
		Bond nextBond;	//another bond of the observed atom
		Bond partnerBond;	//bond of the PartnerAtom
		Bond nextPartnerBond;	//next bond of the PartnerAtom	
		Atom::BondIterator bond_it;	//iterate over all bonds of one atom
		Atom::BondIterator bond_it2;
		RingPerceptionProcessor(ringPP);	//marks all atoms in a ring structure with property "InRing"
	
		//TODO be carefull because of init value 0!!!
		//sum of all atom penalties
		int total_penalty_score = 0;
	
		//mark all atoms in a ring structure with property "InRing"
		mol->apply(ringPP);	
		atom_it = mol->beginAtom();	
	
		//set AtomProperties in molecule
		SetAtomProperties(mol);
	
		//assign to each atom in molecule a penalty score if necessary
		for(;+atom_it;++atom_it){
			//Oxygen
			if (atom_it->getElement().getName()=="Oxygen"){
				//O(x1)
				if (atom_it->getProperty("connectivity").getInt() == 1){
					bond_it = atom_it->beginBond();
					partnerAtom = bond_it->getBoundAtom(*atom_it);
					//cout << "partnerAtom:" << partnerAtom->getElement().getName() << endl;
					//O(x1) in pyridine-1-oxide
					if ((partnerAtom->getElement().getName()=="Nitrate" or partnerAtom->getElement().getName()=="Nitrogen") and partnerAtom->getProperty("InRing").getBool()){
						if (atom_it->getProperty("atomic valence").getInt()== 1){
							atomic_penalty_score = 0;
						}
						else if (atom_it->getProperty("atomic valence").getInt()== 2){
							atomic_penalty_score = 1;
						}
					}
					else if (atom_it->getProperty("atomic valence").getInt()== 1){
						atomic_penalty_score = 1;
					}
					else if (atom_it->getProperty("atomic valence").getInt()== 2){
						atomic_penalty_score = 0;
					}
					else if (atom_it->getProperty("atomic valence").getInt()== 3){
						atomic_penalty_score = 64;
					}
				}
				//O(x2)
				else if (atom_it->getProperty("connectivity").getInt() == 2){
					if (atom_it->getProperty("atomic valence").getInt()== 1){
						atomic_penalty_score= 32;					
					}
					else if (atom_it->getProperty("atomic valence").getInt()== 2){
						atomic_penalty_score = 0;
					}
					else if (atom_it->getProperty("atomic valence").getInt()== 3){
						atomic_penalty_score = 64;
					}	
				}
				atom_it->setProperty("atomic penalty score", (int) atomic_penalty_score);
				total_penalty_score = total_penalty_score + atomic_penalty_score;
			}
			
			//H, F, CL, Br, I
			if (atom_it->getElement().getName()=="Hydrogen" or atom_it->getElement().getName()=="Fluorine" or 		atom_it->getElement().getName()=="Iodine " or atom_it->getElement().getName()=="Chlorine" or 	atom_it->getElement().getName()=="Bromine" ){
					if (atom_it->getProperty("atomic valence").getInt()== 1){
						atomic_penalty_score = 0;
					}
					else if (atom_it->getProperty("atomic valence").getInt()== 0){
						atomic_penalty_score = 64;
					}
					else if (atom_it->getProperty("atomic valence").getInt()== 2){
						atomic_penalty_score = 64;
					}
					atom_it->setProperty("atomic penalty score", (int) atomic_penalty_score);
					total_penalty_score = total_penalty_score + atomic_penalty_score;
			}
			
				
			//Carbon
			if (atom_it->getElement().getName()=="Carbon"){
				if (atom_it->getProperty("connectivity").getInt() ==1){
					//C in CN-R
					bond_it = atom_it->beginBond();
					partnerAtom = bond_it->getBoundAtom(*atom_it);
					//cout << "partnerAtom:" << partnerAtom->getElement().getName() << endl;
					if (partnerAtom->getElement().getName()=="Nitrate" or partnerAtom->getElement().getName()=="Nitrogen"){
						cout << "nitrate" << endl;
						if((atom_it->beginBond())->getOrder()==3){
							if(partnerAtom->countBonds()==2){ 
								if (atom_it->getProperty("atomic valence").getInt()== 3){
									atomic_penalty_score= 0;
								}
								else if (atom_it->getProperty("atomic valence").getInt()== 4){
									atomic_penalty_score = 1;
								}
								else if (atom_it->getProperty("atomic valence").getInt()== 5){
									atomic_penalty_score = 32;
								}
							}						
						}
					}
					//C(x1)		
					else if (atom_it->getProperty("atomic valence").getInt()== 3){
						atomic_penalty_score = 1;
					}
					else if (atom_it->getProperty("atomic valence").getInt()== 4){
						atomic_penalty_score = 0;
					}
					else if (atom_it->getProperty("atomic valence").getInt()== 5){
						atomic_penalty_score = 32;
					}
				}
				//C in COO-
				bond_it = atom_it->beginBond();
				partnerAtom = bond_it->getBoundAtom(*atom_it);
				//cout << "partnerAtom:" << partnerAtom->getElement().getName() << endl;
				if (partnerAtom->getElement().getName() =="Oxygen"){
					secPartnerAtom = (++bond_it)->getBoundAtom(*atom_it);
					if (secPartnerAtom->getElement().getName() =="Oxygen"){
						if (partnerAtom->getCharge()<0.0 and secPartnerAtom->getCharge()==0.0){
							if (atom_it->getProperty("atomic valence").getInt()== 5){
								atomic_penalty_score = 0;
							}
							else if (atom_it->getProperty("atomic valence").getInt()== 4){
								atomic_penalty_score = 32;
							}
							else if (atom_it->getProperty("atomic valence").getInt()== 6){
								atomic_penalty_score = 32;
							}	
						}
						else if (partnerAtom->getCharge()==0.0 and secPartnerAtom->getCharge()<0.0){
							if (atom_it->getProperty("atomic valence").getInt()== 5){
								atomic_penalty_score = 0;
							}
							else if (atom_it->getProperty("atomic valence").getInt()== 4){
								atomic_penalty_score = 32;	
							}
							else if (atom_it->getProperty("atomic valence").getInt()== 6){
								atomic_penalty_score = 32;
							}	
						}					
					}
				}
				//if atom type still not found
				if(atomic_penalty_score ==100){
					//C
					if (atom_it->getProperty("atomic valence").getInt()== 2){
						atomic_penalty_score = 64;
					}
					else if (atom_it->getProperty("atomic valence").getInt()== 3){
						atomic_penalty_score = 32;
					}
					else if (atom_it->getProperty("atomic valence").getInt()== 4){
						atomic_penalty_score = 0;
					}
					else if (atom_it->getProperty("atomic valence").getInt()== 5){
						atomic_penalty_score = 32;
					}
					else if (atom_it->getProperty("atomic valence").getInt()== 6){
						atomic_penalty_score = 64;
					}
				}		
				atom_it->setProperty("atomic penalty score", (int) atomic_penalty_score);
				total_penalty_score = total_penalty_score + atomic_penalty_score;
			}
			
			// Si
			if (atom_it->getElement().getName()== "Silicium"){
				if (atom_it->getProperty("atomic valence").getInt()== 4){
					atomic_penalty_score = 0;
				}
				atom_it->setProperty("atomic penalty score", (int) atomic_penalty_score);
				total_penalty_score = total_penalty_score + atomic_penalty_score;
			}
			
			
				
			//N(x1) in N=N=R
			if ((atom_it->getElement().getName()=="Nitrate" or atom_it->getElement().getName()=="Nitrogen") and atom_it->getProperty("connectivity").getInt()==1){
				bond = *(atom_it->beginBond());
				partnerAtom = bond.getBoundAtom(*atom_it);
				if (partnerAtom->getElement().getName()=="Nitrate" or partnerAtom->getElement().getName()=="Nitrogen"){
					if(bond.getOrder()==2 and partnerAtom->countBonds()==2){  
						partnerBond = *(partnerAtom->beginBond());
						nextPartnerBond = *(++(partnerAtom->beginBond()));
						if(partnerBond.getOrder()==2 and nextPartnerBond.getOrder()==2){
							if (atom_it->getProperty("atomic valence").getInt()==2 or atom_it->getProperty("atomic valence").getInt()==3){
								atomic_penalty_score = 0;
							}
						}
					}
				}
				//N(x1)
				if(atomic_penalty_score ==100){
					if (atom_it->getProperty("atomic valence").getInt()==2){
						atomic_penalty_score = 3;
					}
					else if (atom_it->getProperty("atomic valence").getInt()==3){
						atomic_penalty_score = 0;
					}
					else if (atom_it->getProperty("atomic valence").getInt()==2){
						atomic_penalty_score = 32;
					}
				}			
				atom_it->setProperty("atomic penalty score", (int) atomic_penalty_score);
				total_penalty_score = total_penalty_score + atomic_penalty_score;
			}
			//N(x2) in N=N=R
			if ((atom_it->getElement().getName()=="Nitrate" or atom_it->getElement().getName()=="Nitrogen") and atom_it->getProperty("connectivity").getInt()==2){
				bond = *(atom_it->beginBond());
				nextBond = *(++(atom_it->beginBond()));
				partnerAtom = bond.getBoundAtom(*atom_it);
				otherPartnerAtom = nextBond.getBoundAtom(*atom_it);
				//if one of the neighbours is a Nitrate
				if (partnerAtom->getElement().getName()=="Nitrate" or partnerAtom->getElement().getName()=="Nitrogen"){
					//check bonds (x1 of N)
					if(bond.getOrder()==2 and partnerAtom->countBonds()==1){ 
						partnerBond = *(partnerAtom->beginBond());
						nextPartnerBond = *(++(partnerAtom->beginBond()));
						if (atom_it->getProperty("atomic valence").getInt()==3){
							atomic_penalty_score = 1;
						}
						else if (atom_it->getProperty("atomic valence").getInt()==4){
							atomic_penalty_score = 0;
						}										
					}
				}
				else if(otherPartnerAtom->getElement().getName()=="Nitrate" or otherPartnerAtom->getElement().getName()=="Nitrogen"){
				//check bonds (x2 of N)
					if(nextBond.getOrder()==2 and otherPartnerAtom->countBonds()==1){
					partnerBond = *(otherPartnerAtom->beginBond());
						if (atom_it->getProperty("atomic valence").getInt()==3){
							atomic_penalty_score = 1;
						}
						else if (atom_it->getProperty("atomic valence").getInt()==4){
							atomic_penalty_score = 0;
						}
					} 
				}
				//N(x2)
				if(atomic_penalty_score ==100){
					if (atom_it->getProperty("atomic valence").getInt()==2){
						atomic_penalty_score = 4;
					}
					else if (atom_it->getProperty("atomic valence").getInt()==3){
						atomic_penalty_score = 0;
					}
					else if (atom_it->getProperty("atomic valence").getInt()==4){
						atomic_penalty_score = 2;
					}
				}
				atom_it->setProperty("atomic penalty score", (int) atomic_penalty_score);
				total_penalty_score = total_penalty_score + atomic_penalty_score;
			}
			//N(x3) in nitro
			if ((atom_it->getElement().getName()=="Nitrate" or atom_it->getElement().getName()=="Nitrogen") and atom_it->getProperty("connectivity").getInt()==3){
				bond_it = atom_it->beginBond();
				int countOxy= 0;
				for(;bond_it != atom_it->endBond();++bond_it){
					partnerAtom = bond_it->getBoundAtom(*atom_it);
					if(partnerAtom->getElement().getName()=="Oxygen" and partnerAtom->countBonds()==1 /*and partnerAtom->getCharge()==0*/){
						++countOxy; 
					}
				}
				if(countOxy==2){
					if (atom_it->getProperty("atomic valence").getInt()==3){
						atomic_penalty_score = 64;
					}	
					else if (atom_it->getProperty("atomic valence").getInt()==4){
						atomic_penalty_score = 32;
					}
					else if (atom_it->getProperty("atomic valence").getInt()==5){
						atomic_penalty_score = 0;
					}
					else if (atom_it->getProperty("atomic valence").getInt()==6){
						atomic_penalty_score = 32;
					}
				}
				//N(x3) in pyridine-1-oxide
				else if (atom_it->getCharge()>0){
					bond_it2 = atom_it->beginBond();
					int countOxy=0;
					int countCarb=0;
					for(;bond_it2 != atom_it->endBond();++bond_it2){
						partnerAtom = bond_it2->getBoundAtom(*atom_it);
						if(partnerAtom->getElement().getName()=="Carbon" and partnerAtom->getProperty("InRing").getName()=="InRing"){
							++countCarb;
						}
						if(partnerAtom->getElement().getName()=="Oxygen"){
							++countOxy;
						}
					}
					if (countOxy==1 and countCarb==2){
						if(atom_it->getProperty("atomic valence").getInt()==3){	
							atomic_penalty_score = 1;
						}
						else if (atom_it->getProperty("atomic valence").getInt()==4){
							atomic_penalty_score = 0;
						}
					}
				}
				//N(x3)
				if(atomic_penalty_score ==100){
					if (atom_it->getProperty("atomic valence").getInt()==2){
						atomic_penalty_score = 32;
					}
					else if (atom_it->getProperty("atomic valence").getInt()==3){
						atomic_penalty_score = 0;
					}
					else if (atom_it->getProperty("atomic valence").getInt()==4){
						atomic_penalty_score = 1;
					}
					else if (atom_it->getProperty("atomic valence").getInt()==5){
						atomic_penalty_score = 2;
					}	
				}
				atom_it->setProperty("atomic penalty score", (int) atomic_penalty_score);
				total_penalty_score = total_penalty_score + atomic_penalty_score;	
			}
			//N(x4)
			if ((atom_it->getElement().getName()=="Nitrate" or atom_it->getElement().getName()=="Nitrogen") and atom_it->getProperty("connectivity").getInt()==4){
				if (atom_it->getProperty("atomic valence").getInt()==2){
					atomic_penalty_score = 64;
				}
				else if (atom_it->getProperty("atomic valence").getInt()==2){
						atomic_penalty_score = 64;
				}
				else if (atom_it->getProperty("atomic valence").getInt()==3){
						atomic_penalty_score = 0;
				}
				else if (atom_it->getProperty("atomic valence").getInt()==4){
						atomic_penalty_score = 64;
				}
				atom_it->setProperty("atomic penalty score", (int) atomic_penalty_score);	
				total_penalty_score = total_penalty_score + atomic_penalty_score;
			}
			
			
			//P(x1)
			if (atom_it->getElement().getName()=="Phosphorus" and atom_it->getProperty("connectivity").getInt()==1){
				if (atom_it->getProperty("atomic valence").getInt()==2){
						atomic_penalty_score = 2;
				}	
				else if (atom_it->getProperty("atomic valence").getInt()==3){
						atomic_penalty_score = 0;
				}
				else if (atom_it->getProperty("atomic valence").getInt()==4){
						atomic_penalty_score = 32;
				}
				atom_it->setProperty("atomic penalty score", (int) atomic_penalty_score);
				total_penalty_score = total_penalty_score + atomic_penalty_score;
			}
			//P(x2)
			if (atom_it->getElement().getName()=="Phosphorus" and atom_it->getProperty("connectivity").getInt()==2){
				if (atom_it->getProperty("atomic valence").getInt()==2){
						atomic_penalty_score = 4;
				}
				else if (atom_it->getProperty("atomic valence").getInt()==3){
						atomic_penalty_score = 0;
				}
				else if (atom_it->getProperty("atomic valence").getInt()==4){
						atomic_penalty_score = 2;
				}
				atom_it->setProperty("atomic penalty score", (int) atomic_penalty_score);
				total_penalty_score = total_penalty_score + atomic_penalty_score;
			}
			//P(x3)
			if (atom_it->getElement().getName()=="Phosphorus" and atom_it->getProperty("connectivity").getInt()==3){
				if (atom_it->getProperty("atomic valence").getInt()==2){
						atomic_penalty_score = 32;
				}
				else if (atom_it->getProperty("atomic valence").getInt()==3){
						atomic_penalty_score = 0;
				}
				else if (atom_it->getProperty("atomic valence").getInt()==4){
						atomic_penalty_score = 1;
				}
				else if (atom_it->getProperty("atomic valence").getInt()==5){
						atomic_penalty_score = 2;
				}
				atom_it->setProperty("atomic penalty score", (int) atomic_penalty_score);
				total_penalty_score = total_penalty_score + atomic_penalty_score;
			}
			//P(x4) 
			if (atom_it->getElement().getName()=="Phosphorus" and atom_it->getProperty("connectivity").getInt()==4){
				bond_it = atom_it->beginBond();
				int countOxy = 0;
				int countSul = 0;
				for(;bond_it != atom_it->endBond(); ++bond_it){
					partnerAtom = bond_it->getBoundAtom(*atom_it);
					if (partnerAtom->getElement().getName()=="Oxygen" and partnerAtom->countBonds()== 1){
						++countOxy;
					}
					else if ((partnerAtom->getElement().getName()=="Sulphur" or  partnerAtom->getElement().getName()=="Sulfur") and partnerAtom->countBonds()== 1){
						++countSul;
					}					
				}
				//bonded to 2 O(x1) or 2 S(x1)
				if (countOxy==2 or countSul==2){
					if (atom_it->getProperty("atomic valence").getInt()==5){
						atomic_penalty_score = 32;
					}
					else if (atom_it->getProperty("atomic valence").getInt()==6){
						atomic_penalty_score = 0;
					}
					else if (atom_it->getProperty("atomic valence").getInt()==7){
						atomic_penalty_score = 32;
					}
				}	
				//bonded to 3 O(x1) or 3 S(x1)
				if(countOxy==3 or countSul==3){
					if (atom_it->getProperty("atomic valence").getInt()==6){
						atomic_penalty_score = 32;
					}
					else if (atom_it->getProperty("atomic valence").getInt()==7){
						atomic_penalty_score = 0;
					}	
				}
				//P(x4)
				if(atomic_penalty_score ==100){
					if (atom_it->getProperty("atomic valence").getInt()==3){
						atomic_penalty_score = 64;
					}
					else if (atom_it->getProperty("atomic valence").getInt()==4){
						atomic_penalty_score = 1;
					}
					else if (atom_it->getProperty("atomic valence").getInt()==5){
						atomic_penalty_score = 0;
					}
					else if (atom_it->getProperty("atomic valence").getInt()==6){
						atomic_penalty_score = 32;
					}
				}
				atom_it->setProperty("atomic penalty score", (int) atomic_penalty_score);
				total_penalty_score = total_penalty_score + atomic_penalty_score;
			}
					
			
			//S(x1)
			if ((atom_it->getElement().getName()=="Sulphur" or  atom_it->getElement().getName()=="Sulfur") and atom_it->getProperty("connectivity").getInt()==1){
				bond_it = atom_it->beginBond();
				partnerAtom = bond_it->getBoundAtom(*atom_it);
				//S(x1) in pyridine-1-thiol anion
				if ((partnerAtom->getElement().getName()=="Nitrate" or partnerAtom->getElement().getName()=="Nitrogen") and partnerAtom->getProperty("InRing").getName() =="InRing"){
					if(partnerAtom->getCharge()>0 and atom_it->getCharge()<0){
						if (atom_it->getProperty("atomic valence").getInt()==1){
							atomic_penalty_score = 0;
						}
						else if (atom_it->getProperty("atomic valence").getInt()==2){
							atomic_penalty_score = 1;
						}
					}
				}
				//S(x1)
				if(atomic_penalty_score ==100){
					if (atom_it->getProperty("atomic valence").getInt()==1){
							atomic_penalty_score = 2;
					}								
					else if (atom_it->getProperty("atomic valence").getInt()==2){
						atomic_penalty_score = 0;
					}
					else if (atom_it->getProperty("atomic valence").getInt()==3){
							atomic_penalty_score = 64;
					}
				}
				atom_it->setProperty("atomic penalty score", (int) atomic_penalty_score);
				total_penalty_score = total_penalty_score + atomic_penalty_score;
			}
			
			//S(x2)
			if ((atom_it->getElement().getName()=="Sulphur" or  atom_it->getElement().getName()=="Sulfur") and atom_it->getProperty("connectivity").getInt()==2){
				if (atom_it->getProperty("atomic valence").getInt()==1){
						atomic_penalty_score = 32;
				}
				else if (atom_it->getProperty("atomic valence").getInt()==2){
						atomic_penalty_score = 0;
				}
				else if (atom_it->getProperty("atomic valence").getInt()==3){
						atomic_penalty_score = 32;
				}
				else if (atom_it->getProperty("atomic valence").getInt()==4){
						atomic_penalty_score = 1;
				}
				atom_it->setProperty("atomic penalty score", (int) atomic_penalty_score);
				total_penalty_score = total_penalty_score + atomic_penalty_score;
			}
			
			//S(x3)
			if ((atom_it->getElement().getName()=="Sulphur" or  atom_it->getElement().getName()=="Sulfur") and atom_it->getProperty("connectivity").getInt()==3){
				if (atom_it->getProperty("atomic valence").getInt()==3){
						atomic_penalty_score = 1;
				}
				else if (atom_it->getProperty("atomic valence").getInt()==4){
						atomic_penalty_score = 0;
				}
				else if (atom_it->getProperty("atomic valence").getInt()==5){
						atomic_penalty_score = 2;
				}
				else if (atom_it->getProperty("atomic valence").getInt()==6){
						atomic_penalty_score = 2;
				}
				atom_it->setProperty("atomic penalty score", (int) atomic_penalty_score);
				total_penalty_score = total_penalty_score + atomic_penalty_score;
			}
			
			
			
			//S(x4)
			if ((atom_it->getElement().getName()=="Sulphur" or  atom_it->getElement().getName()=="Sulfur") and atom_it->getProperty("connectivity").getInt()==4){
				bond_it = atom_it->beginBond();
				int countOxy = 0;
				int countSul = 0;
				for(;bond_it != atom_it->endBond(); ++bond_it){
					partnerAtom = bond_it->getBoundAtom(*atom_it);
					if (partnerAtom->getElement().getName()=="Oxygen" and partnerAtom->countBonds()== 1){
						++countOxy;
					}
					else if ((partnerAtom->getElement().getName()=="Sulphur" or  partnerAtom->getElement().getName()=="Sulfur") and partnerAtom->countBonds()== 1){
						++countSul;
					}					
				}
				//bonded to 2 O(x1) or S(x1)
				if (countOxy ==2 or countSul==2){
					if (atom_it->getProperty("atomic valence").getInt()==6){
						atomic_penalty_score = 0;
					}
					else if (atom_it->getProperty("atomic valence").getInt()==7){
						atomic_penalty_score = 32;
					}
				}	
				//bonded to 3 O(x1) or S(x1)
				if(countOxy== 3 or countSul==3){
					if (atom_it->getProperty("atomic valence").getInt()==6){
						atomic_penalty_score = 32;
					}
					else if (atom_it->getProperty("atomic valence").getInt()==7){
						atomic_penalty_score = 0;
					}	
				}
				//bonded to 4 O(x1) or S(x1)
				if(countOxy==4 or countSul==4){
					if (atom_it->getProperty("atomic valence").getInt()==6){
						atomic_penalty_score = 32;
					}
					else if (atom_it->getProperty("atomic valence").getInt()==7){
						atomic_penalty_score = 0;
					}		
				}		
				//S(x4)
				if(atomic_penalty_score ==100){
					if (atom_it->getProperty("atomic valence").getInt()==4){
						atomic_penalty_score = 4;
					}
					else if (atom_it->getProperty("atomic valence").getInt()==5){
						atomic_penalty_score = 2;
					}
					else if (atom_it->getProperty("atomic valence").getInt()==6){
						atomic_penalty_score = 0;
					}
				}	
				atom_it->setProperty("atomic penalty score", (int) atomic_penalty_score);
				total_penalty_score = total_penalty_score + atomic_penalty_score;
			}
		}
		mol->setProperty("total penalty score",(int) total_penalty_score);
		cout << "total penalty score:" << total_penalty_score << endl;
		cout << endl;
	}

	//store AtomStatus at a certain point of time
	void Atomtypes::StoreSystemStatus(list<int> current_atoms, Molecule* mol){
		cout << "in StoreSystemStatus" << endl;
		InitializeSuccessiveUnresolvedAtoms(current_atoms);
		cout << "successive_unresolved_atoms:" << successive_unresolved_atoms_.size() << endl;
		for(list<int>::iterator index = current_atoms.begin(); index != current_atoms.end(); ++index){
			//get Atom in molecule on the basis of index 
			Atom* atom = mol->getAtom(*index);
			//store position of atom in AtomStatus
			int position = *index;
			AtomStatus atomStatus;
			atomStatus.position = position;
			//store connectivity and atomic_valence of atom in AtomStatus
			int connectivity = atom->getProperty("connectivity").getInt();
			int atomic_valence = atom->getProperty("atomic valence").getInt();
			atomStatus.connectivity = connectivity;
			atomStatus.atomic_valence = atomic_valence;
			pair<int,int> atom_property;
			atom_property.first = connectivity;
			atom_property.second = atomic_valence;
			atomStatus.atom_state.first = position;
			atomStatus.atom_state.second = atom_property;
			//vector of partnerAtoms
			Atom::BondIterator bond_it = atom->beginBond();
			for(;+bond_it;++bond_it){
				const Atom* atom_const = atom;
				Atom* partnerAtom = bond_it->getPartner(*atom_const);
				int index = partnerAtom->getProperty("index").getInt();
				//store index of partnerAtom in vector partner_atoms
				atomStatus.partner_atoms.push_back(index);	
			}
			//TODO position to push_back refers to TrialAndErrorTest estimatedBonds vector!!!
			successive_unresolved_atoms_[0].push_back(atomStatus);
		}
		cout << "StoreAtomStatus" << endl;
	}


	//set atom and bond properties back to value they had at a certain point of time
	void Atomtypes::SetBackProperties(Molecule* mol, vector<AtomStatus> unresolved_atoms){
		for(vector<AtomStatus >::iterator status_it = unresolved_atoms.begin(); status_it!= unresolved_atoms.end(); ++status_it){
			AtomStatus atomStatus;
			atomStatus = *status_it;
			atomStatus.atom_state = status_it->atom_state;
			pair<int,int> atom_properties = atomStatus.atom_state.second;
			//get atom on the basis of its positionindex in atom_status
			Atom* atom = mol->getAtom(atomStatus.atom_state.first);
			//set atom attached properties back to the properties stored in atom_status
			int connectivity = atom->getProperty("connectivity").getInt();
			connectivity= atomStatus.connectivity;
			atom->setProperty("connectivity", (int) connectivity);
			int atomic_valence = atom->getProperty("atomic valence").getInt() ;
			atomic_valence = atomStatus.atomic_valence;
			atom->setProperty("atomic valence", (int) atomic_valence);

			Bond* bond;
			//set all bonds of atom back to 0
			for(vector<int>::iterator partner_it = atomStatus.partner_atoms.begin();partner_it != atomStatus.partner_atoms.end();++partner_it){
				//if partner_atom of atom in unresolved_atoms: set the corresponding bond to 0
				for(vector<AtomStatus>::iterator status_it2 = unresolved_atoms.begin(); status_it2 != unresolved_atoms.end();++status_it2){
					AtomStatus atomStatus2;
					atomStatus2 = *status_it2;
					if(*partner_it == atomStatus2.position){
						//get partner_atom on the basis of its positionindex in molecule
						const Atom* partner_atom = mol->getAtom(*partner_it);
						bond = atom->getBond(*partner_atom);
						//if bond not assigned by TrialAndErrorTester
						if(!(bond->getProperty("trial and error test").getBool())){
							int bondorder = 0;
							bond->setProperty("assigned bondorder", (int) bondorder);
						}
					}
				}
				
			}
		}	
		cout << "SetBackProperties" << endl;	
	}


}


