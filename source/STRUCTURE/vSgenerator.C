
#include <BALL/STRUCTURE/vSgenerator.h>

#ifndef BONDORDERASSIGNER_H
#include <BALL/STRUCTURE/bondorderAssigner.h>
#endif

#ifndef ATOMTYPES_H
#include <BALL/STRUCTURE/atomtypes.h>
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

	VSgenerator::VSgenerator(){
	}


	VSgenerator::VSgenerator(Molecule* mol){
		//TODO init

		//initialize penalty_scores
		
	}


	VSgenerator::~VSgenerator(){
	}	


	//sort pairs in ascending order concerning the second component
	bool PairSorted( const pair<int,int> pair1, const pair<int,int> pair2){
		if(pair1.second <= pair2.second){
			return true;
		}
		else return false;
	}


	//stores for every atom its possible atomic valences 
	//and the corresponding possible atomic penalty scores in a vector 
	//the vector then is stored in an other vector at the atom's position in the molecule
	void VSgenerator::GenerateAPS(Molecule* mol){
	
		const Atom* partnerAtom;	
		const Atom* secPartnerAtom;	
		const Atom* otherPartnerAtom;	
		AtomIterator atom_it;	//iterates over all atoms in the observed molecule
		Bond* bond;	//bond of the observed atom
		Bond* nextBond;	//another bond of the observed atom
		Bond* partnerBond;	//bond of the PartnerAtom
		Bond* nextPartnerBond;	//next bond of the PartnerAtom	
		Atom::BondIterator bond_it;	//iterate over all bonds of one atom
		Atom::BondIterator bond_it2;
		RingPerceptionProcessor(ringPP);	//marks all atoms in a ring structure with property "InRing"
		int atomic_penalty_score;
		pair <int, int > atomicScore;	//pair of atomic valence and corresponding atomic penalty score
		vector <pair <int, int > > aps_vector;	//vector of atomicScores
	
		//TODO be carefull because of init value 0!!!
		//sum of all atom penalties
		int total_penalty_score = 0;
	
		//mark all atoms in a ring structure with property "InRing"
		mol->apply(ringPP);	
		atom_it = mol->beginAtom();	

		Atomtypes atomtypes = Atomtypes(mol);	
		//set AtomProperties in molecule
		atomtypes.SetAtomProperties(mol);
	
		//assign to each atom in molecule a penalty score if necessary
		for(;+atom_it;++atom_it){
			aps_vector.clear();
			//Oxygen
			if (atom_it->getElement().getName()=="Oxygen"){
				//O(x1)
				if (atom_it->getProperty("connectivity").getInt() == 1){
					bond_it = atom_it->beginBond();
					partnerAtom = bond_it->getBoundAtom(*atom_it);
					//O(x1) in pyridine-1-oxide
					if ((partnerAtom->getElement().getName()=="Nitrate" or partnerAtom->getElement().getName()=="Nitrogen") and partnerAtom->getProperty("InRing").getBool()){
						atomicScore.first = 1;
						atomicScore.second = 0;
						aps_vector.push_back(atomicScore);
				
						atomicScore.first = 2;
						atomicScore.second = 1;
						aps_vector.push_back(atomicScore);
					}
					else {
						atomicScore.first = 1;
						atomicScore.second = 1;
						aps_vector.push_back(atomicScore);
				
						atomicScore.first = 2;
						atomicScore.second = 0;
						aps_vector.push_back(atomicScore);

						atomicScore.first = 3;
						atomicScore.second = 64;
						aps_vector.push_back(atomicScore);
					}
				}
				//O(x2)
				else if (atom_it->getProperty("connectivity").getInt() == 2){
					atomicScore.first = 1;
					atomicScore.second = 32;
					aps_vector.push_back(atomicScore);

					atomicScore.first = 2;
					atomicScore.second = 0;
					aps_vector.push_back(atomicScore);

					atomicScore.first = 3;
					atomicScore.second = 64;
					aps_vector.push_back(atomicScore);
				}
			}
			
			//H, F, CL, Br, I
			if (atom_it->getElement().getName()=="Hydrogen" or atom_it->getElement().getName()=="Fluorine" or 		atom_it->getElement().getName()=="Iodine " or atom_it->getElement().getName()=="Chlorine" or 	atom_it->getElement().getName()=="Bromine" ){
					atomicScore.first = 1;
					atomicScore.second = 0;
					aps_vector.push_back(atomicScore);

					atomicScore.first = 0;
					atomicScore.second = 64;
					aps_vector.push_back(atomicScore);

					atomicScore.first = 2;
					atomicScore.second = 64;
					aps_vector.push_back(atomicScore);
			}
			
				
			//Carbon
			if (atom_it->getElement().getName()=="Carbon"){
				if (atom_it->getProperty("connectivity").getInt() ==1){
					//C in CN-R
					bond_it = atom_it->beginBond();
					partnerAtom = bond_it->getBoundAtom(*atom_it);
					if (partnerAtom->getElement().getName()=="Nitrate" or partnerAtom->getElement().getName()=="Nitrogen"){
						cout << "nitrate" << endl;
						if(partnerAtom->countBonds() == 2){
							// TODO getOrder!!! not always possible!!!
							if((atom_it->beginBond())->getOrder()==3){
								if(partnerAtom->countBonds()==2){ 
									atomicScore.first = 3;
									atomicScore.second = 0;
									aps_vector.push_back(atomicScore);
	
									atomicScore.first = 4;
									atomicScore.second = 1;
									aps_vector.push_back(atomicScore);

									atomicScore.first = 5;
									atomicScore.second = 32;
									aps_vector.push_back(atomicScore);
								}	
							}				
						}
					}
					//C(x1)		
					else {
						atomicScore.first = 3;
						atomicScore.second = 1;
						aps_vector.push_back(atomicScore);

						atomicScore.first = 4;
						atomicScore.second = 0;
						aps_vector.push_back(atomicScore);
					
						atomicScore.first = 5;
						atomicScore.second = 32;
						aps_vector.push_back(atomicScore);
					}
				}
				//C in COO-
				bond_it = atom_it->beginBond();
				partnerAtom = bond_it->getBoundAtom(*atom_it);
				if (partnerAtom->getElement().getName() =="Oxygen"){
					secPartnerAtom = (++bond_it)->getBoundAtom(*atom_it);
					if (secPartnerAtom->getElement().getName() =="Oxygen"){
						//TODO getCharge!!! not always possible!!!
						if (partnerAtom->getCharge()<0.0 and secPartnerAtom->getCharge()==0.0){
							atomicScore.first = 5;
							atomicScore.second = 0;
							aps_vector.push_back(atomicScore);

							atomicScore.first = 4;
							atomicScore.second = 32;
							aps_vector.push_back(atomicScore);

							atomicScore.first = 6;
							atomicScore.second = 32;
							aps_vector.push_back(atomicScore);
						}
						else if (partnerAtom->getCharge()==0.0 and secPartnerAtom->getCharge()<0.0){
							atomicScore.first = 5;
							atomicScore.second = 0;
							aps_vector.push_back(atomicScore);

							atomicScore.first = 4;
							atomicScore.second = 32;
							aps_vector.push_back(atomicScore);

							atomicScore.first = 6;
							atomicScore.second = 32;
							aps_vector.push_back(atomicScore);
						}					
					}
				}
				//if atom type still not found
				if(aps_vector.empty()){
					//C
					atomicScore.first = 2;
					atomicScore.second = 64;
					aps_vector.push_back(atomicScore);
	
					atomicScore.first = 3;
					atomicScore.second = 32;
					aps_vector.push_back(atomicScore);

					atomicScore.first = 4;
					atomicScore.second = 0;
					aps_vector.push_back(atomicScore);

					atomicScore.first = 5;
					atomicScore.second = 32;
					aps_vector.push_back(atomicScore);

					atomicScore.first = 6;
					atomicScore.second = 64;
					aps_vector.push_back(atomicScore);
				}		
			}
			
			// Si
			if (atom_it->getElement().getName()== "Silicium"){
				atomicScore.first = 4;
				atomicScore.second = 0;
				aps_vector.push_back(atomicScore);
			}
			
			
				
			//N(x1) in N=N=R
			if ((atom_it->getElement().getName()=="Nitrate" or atom_it->getElement().getName()=="Nitrogen") and atom_it->getProperty("connectivity").getInt()==1){
				bond = &(*(atom_it->beginBond()));
				partnerAtom = bond->getBoundAtom(*atom_it);
				if (partnerAtom->getElement().getName()=="Nitrate" or partnerAtom->getElement().getName()=="Nitrogen"){
					//TODO getOrder!!! not always possible!!!
					if(bond->getOrder()==2 and partnerAtom->countBonds()==2){  
						partnerBond = const_cast <Bond*> (&(*(partnerAtom->beginBond())));
						nextPartnerBond = const_cast <Bond*> (&(*(++(partnerAtom->beginBond()))));
						if(partnerBond->getOrder()==2 and nextPartnerBond->getOrder()==2){
							atomicScore.first = 2;
							atomicScore.second = 0;
							aps_vector.push_back(atomicScore);

							atomicScore.first = 3;
							atomicScore.second = 0;
							aps_vector.push_back(atomicScore);
						}
					}
				}
				//N(x1)
				if(aps_vector.empty()){
					atomicScore.first = 2;
					atomicScore.second = 3;
					aps_vector.push_back(atomicScore);

					atomicScore.first = 3;
					atomicScore.second = 0;
					aps_vector.push_back(atomicScore);
		
					atomicScore.first = 4;
					atomicScore.second = 32;
					aps_vector.push_back(atomicScore);
				}			
			}
			//N(x2) in N=N=R
			if ((atom_it->getElement().getName()=="Nitrate" or atom_it->getElement().getName()=="Nitrogen") and atom_it->getProperty("connectivity").getInt()==2){
				bond = &(*(atom_it->beginBond()));
				nextBond = const_cast<Bond*>(&(*(++(atom_it->beginBond()))));
				partnerAtom = bond->getBoundAtom(*atom_it);
				otherPartnerAtom = nextBond->getBoundAtom(*atom_it);
				//if one of the neighbours is a Nitrate
				if (partnerAtom->getElement().getName()=="Nitrate" or partnerAtom->getElement().getName()=="Nitrogen"){
					//check bonds (x1 of N)
					if(bond->getOrder()==2 and partnerAtom->countBonds()==1){ 
						partnerBond = const_cast<Bond*>(&(*(partnerAtom->beginBond())));
						nextPartnerBond = const_cast<Bond*>(&(*(++(partnerAtom->beginBond()))));

						atomicScore.first = 3;
						atomicScore.second = 1;
						aps_vector.push_back(atomicScore);

						atomicScore.first = 4;
						atomicScore.second = 0;
						aps_vector.push_back(atomicScore);							
					}
				}
				else if(otherPartnerAtom->getElement().getName()=="Nitrate" or otherPartnerAtom->getElement().getName()=="Nitrogen"){
				//check bonds (x2 of N)
					if(nextBond->getOrder()==2 and otherPartnerAtom->countBonds()==1){
						partnerBond = const_cast<Bond*>(&(*(otherPartnerAtom->beginBond())));
						
						atomicScore.first = 3;
						atomicScore.second = 1;
						aps_vector.push_back(atomicScore);

						atomicScore.first = 4;
						atomicScore.second = 0;
						aps_vector.push_back(atomicScore);
					} 
				}
				//N(x2)
				if(aps_vector.empty()){
					atomicScore.first = 2;
					atomicScore.second = 4;
					aps_vector.push_back(atomicScore);

					atomicScore.first = 3;
					atomicScore.second = 0;
					aps_vector.push_back(atomicScore);

					atomicScore.first = 4;
					atomicScore.second = 2;
					aps_vector.push_back(atomicScore);
				}
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
					atomicScore.first = 3;
					atomicScore.second = 64;
					aps_vector.push_back(atomicScore);

					atomicScore.first = 4;
					atomicScore.second = 32;
					aps_vector.push_back(atomicScore);

					atomicScore.first = 5;
					atomicScore.second = 0;
					aps_vector.push_back(atomicScore);

					atomicScore.first = 6;
					atomicScore.second = 32;
					aps_vector.push_back(atomicScore);
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
						atomicScore.first = 3;
						atomicScore.second = 1;
						aps_vector.push_back(atomicScore);

						atomicScore.first = 4;
						atomicScore.second = 0;
						aps_vector.push_back(atomicScore);
					}
				}
				//N(x3)
				if(aps_vector.empty()){
					atomicScore.first = 2;
					atomicScore.second =32;
					aps_vector.push_back(atomicScore);

					atomicScore.first = 3;
					atomicScore.second = 0;
					aps_vector.push_back(atomicScore);
			
					atomicScore.first = 4;
					atomicScore.second = 1;
					aps_vector.push_back(atomicScore);

					atomicScore.first = 5;
					atomicScore.second = 2;
					aps_vector.push_back(atomicScore);
				}	
			}
			//N(x4)
			if ((atom_it->getElement().getName()=="Nitrate" or atom_it->getElement().getName()=="Nitrogen") and atom_it->getProperty("connectivity").getInt()==4){
				atomicScore.first = 2;
				atomicScore.second = 64;
				aps_vector.push_back(atomicScore);		

				atomicScore.first = 3;
				atomicScore.second = 0;
				aps_vector.push_back(atomicScore);

				atomicScore.first = 4;
				atomicScore.second = 64;
				aps_vector.push_back(atomicScore);
			}
			
			
			//P(x1)
			if (atom_it->getElement().getName()=="Phosphorus" and atom_it->getProperty("connectivity").getInt()==1){
				atomicScore.first = 2;
				atomicScore.second = 2;
				aps_vector.push_back(atomicScore);

				atomicScore.first = 3;
				atomicScore.second = 0;
				aps_vector.push_back(atomicScore);

				atomicScore.first = 4;
				atomicScore.second = 32;
				aps_vector.push_back(atomicScore);
			}
			//P(x2)
			if (atom_it->getElement().getName()=="Phosphorus" and atom_it->getProperty("connectivity").getInt()==2){
				atomicScore.first = 2;
				atomicScore.second = 4;
				aps_vector.push_back(atomicScore);

				atomicScore.first = 3;
				atomicScore.second = 0;
				aps_vector.push_back(atomicScore);

				atomicScore.first = 4;
				atomicScore.second = 2;
				aps_vector.push_back(atomicScore);
			}
			//P(x3)
			if (atom_it->getElement().getName()=="Phosphorus" and atom_it->getProperty("connectivity").getInt()==3){
				atomicScore.first = 2;
				atomicScore.second = 32;
				aps_vector.push_back(atomicScore);

				atomicScore.first = 3;
				atomicScore.second = 0;
				aps_vector.push_back(atomicScore);

				atomicScore.first = 4;
				atomicScore.second = 1;
				aps_vector.push_back(atomicScore);

				atomicScore.first = 5;
				atomicScore.second = 2;
				aps_vector.push_back(atomicScore);
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
					atomicScore.first = 5;
					atomicScore.second = 32;
					aps_vector.push_back(atomicScore);

					atomicScore.first = 6;
					atomicScore.second = 0;
					aps_vector.push_back(atomicScore);

					atomicScore.first = 7;
					atomicScore.second = 32;
					aps_vector.push_back(atomicScore);
				}	
				//bonded to 3 O(x1) or 3 S(x1)
				if(countOxy==3 or countSul==3){
					atomicScore.first = 6;
					atomicScore.second = 32;
					aps_vector.push_back(atomicScore);

					atomicScore.first = 7;
					atomicScore.second = 0;
					aps_vector.push_back(atomicScore);
				}
				//P(x4)
				if(aps_vector.empty()){
					atomicScore.first = 3;
					atomicScore.second = 64;
					aps_vector.push_back(atomicScore);

					atomicScore.first = 4;
					atomicScore.second = 1;
					aps_vector.push_back(atomicScore);

					atomicScore.first = 5;
					atomicScore.second = 0;
					aps_vector.push_back(atomicScore);

					atomicScore.first = 6;
					atomicScore.second = 32;
					aps_vector.push_back(atomicScore);
				}
			}
					
			
			//S(x1)
			if ((atom_it->getElement().getName()=="Sulphur" or  atom_it->getElement().getName()=="Sulfur") and atom_it->getProperty("connectivity").getInt()==1){
				bond_it = atom_it->beginBond();
				partnerAtom = bond_it->getBoundAtom(*atom_it);
				//S(x1) in pyridine-1-thiol anion
				if ((partnerAtom->getElement().getName()=="Nitrate" or partnerAtom->getElement().getName()=="Nitrogen") and partnerAtom->getProperty("InRing").getName() =="InRing"){
					if(partnerAtom->getCharge()>0 and atom_it->getCharge()<0){
						atomicScore.first = 1;
						atomicScore.second = 0;
						aps_vector.push_back(atomicScore);
			
						atomicScore.first = 2;
						atomicScore.second = 1;
						aps_vector.push_back(atomicScore);
					}
				}
				//S(x1)
				if(aps_vector.empty()){
					atomicScore.first = 1;
					atomicScore.second = 2;
					aps_vector.push_back(atomicScore);

					atomicScore.first = 2;
					atomicScore.second = 0;
					aps_vector.push_back(atomicScore);

					atomicScore.first = 3;
					atomicScore.second = 64;
					aps_vector.push_back(atomicScore);
				}
			}
			
			//S(x2)
			if ((atom_it->getElement().getName()=="Sulphur" or  atom_it->getElement().getName()=="Sulfur") and atom_it->getProperty("connectivity").getInt()==2){
				atomicScore.first = 1;
				atomicScore.second = 32;
				aps_vector.push_back(atomicScore);

				atomicScore.first = 2;
				atomicScore.second = 0;
				aps_vector.push_back(atomicScore);

				atomicScore.first = 3;
				atomicScore.second = 32;
				aps_vector.push_back(atomicScore);

				atomicScore.first = 4;
				atomicScore.second = 1;
				aps_vector.push_back(atomicScore);
			}
			
			//S(x3)
			if ((atom_it->getElement().getName()=="Sulphur" or  atom_it->getElement().getName()=="Sulfur") and atom_it->getProperty("connectivity").getInt()==3){
				atomicScore.first = 3;
				atomicScore.second = 1;
				aps_vector.push_back(atomicScore);

				atomicScore.first = 4;
				atomicScore.second = 0;
				aps_vector.push_back(atomicScore);

				atomicScore.first = 5;
				atomicScore.second = 2;
				aps_vector.push_back(atomicScore);

				atomicScore.first = 6;
				atomicScore.second = 2;
				aps_vector.push_back(atomicScore);
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
					atomicScore.first = 6;
					atomicScore.second = 0;
					aps_vector.push_back(atomicScore);

					atomicScore.first = 7;
					atomicScore.second = 32;
					aps_vector.push_back(atomicScore);
				}	
				//bonded to 3 O(x1) or S(x1)
				if(countOxy== 3 or countSul==3){
					atomicScore.first = 6;
					atomicScore.second = 32;
					aps_vector.push_back(atomicScore);

					atomicScore.first = 7;
					atomicScore.second = 0;
					aps_vector.push_back(atomicScore);
				}
				//bonded to 4 O(x1) or S(x1)
				if(countOxy==4 or countSul==4){
					atomicScore.first = 6;
					atomicScore.second = 32;
					aps_vector.push_back(atomicScore);

					atomicScore.first = 7;
					atomicScore.second = 0;
					aps_vector.push_back(atomicScore);
				}		
				//S(x4)
				if(aps_vector.empty()){
					atomicScore.first = 4;
					atomicScore.second = 4;
					aps_vector.push_back(atomicScore);

					atomicScore.first = 5;
					atomicScore.second = 2;
					aps_vector.push_back(atomicScore);

					atomicScore.first = 6;
					atomicScore.second = 0;
					aps_vector.push_back(atomicScore);
				}	
			}
			//sort aps_vector in ascending order
			//sort(aps_vector.begin(),aps_vector.end(),PairSorted);
			atomic_penalty_scores_.push_back(aps_vector);
		}
	}



	//prepare all conditions for the generation process
	void VSgenerator::SetupGenerator(Molecule* mol){
		BondorderAssigner bondorderAssigner = BondorderAssigner(mol);
		list<int> current_atoms = bondorderAssigner.getCurrentAtoms();

		GenerateAPS(mol);
		int index = 0;
		//delete atom indices out of atomlist 
		//if atomic penalty score list has only one element
		for(vector<vector<pair<int,int> > >::iterator vec_it = atomic_penalty_scores_.begin(); vec_it != atomic_penalty_scores_.end();++vec_it){
			for(list<int>::iterator atom_it = current_atoms.begin(); atom_it != current_atoms.end();++atom_it){
				vector<pair<int,int> > apslist = *vec_it;
				if(apslist.size() < 2){
					current_atoms.erase(atom_it);
				}
			}
		}

	}



	//create all valence states within a certain cutoff 
	vector<pair<int,Molecule* > > VSgenerator::GenerateValenceStates(Molecule* mol){
		int total_penalty_score = -100;
		//"best case" (take the minimum for every atom)
		for(vector<vector<pair<int,int> > >::iterator atom_it = atomic_penalty_scores_.begin(); atom_it != atomic_penalty_scores_.end();++atom_it){
			for(int index = 0;index <= atomic_penalty_scores_.size(); ++index){
				vector<pair<int,int> > atomic_scores = *atom_it;
				pair<int,int> property_pair = *(atomic_scores.begin());
				int atomic_valence = property_pair.first;
				int atomic_penalty_score = property_pair.second;
				//set atom properties on current property_pair
				Atom* atom = mol->getAtom(index);
				atom->setProperty("atomic valence", (int) atomic_valence);
				atom->setProperty("atomic penalty score",(int) atomic_penalty_score);
				total_penalty_score = total_penalty_score + atomic_penalty_score;
			}
		}
		pair<int,Molecule*> valence_status;
		valence_status.first = total_penalty_score;
		valence_status.second = mol;
		valence_states_.push_back(valence_status);

		return valence_states_;

	}




}


