#ifndef MATCHER_H
#define MATCHER_H

#ifndef BASIC_H
#include "basic.h"
#endif

///####################### M A T C H I N G ##############################

// cut bonds that are shared with atoms from other fragments:
void clearExternalBonds(AtomContainer* mol)
{
	Atom::BondIterator bit;
	AtomIterator ait;

	BALL_FOREACH_INTERBOND(*mol, ait, bit)
	{
		bit->destroy();
	}
}

/** 
 * Generate a canonical order of atoms to be able to transfer the coordinates
 * 
 */
void canonicalize(std::vector <Fragment*>& fragments)
{
	
	int num_atoms = -1;// obMol.NumAtoms();
//	cout<<"Canonicalising the atomlists..."<<endl;
	std::vector< Fragment* >::iterator it;
	Fragment* new_frag;
	for(it=fragments.begin(); it != fragments.end(); it++)
	{
		num_atoms = (*it)->countAtoms();
//		cout<<(*it)->getName()<< " has #atoms: "<<(*it)->countAtoms()<<endl;
//		cout<<"DEBUG: "<<endl;
		
		/// TODO: remove and find alternative
		clearExternalBonds(*it);
		
		
		
//		cout<<" cut xt-Bonds <"<<endl;
		OBMol* temp = MolecularSimilarity::createOBMol(**it, true);
//		cout<<" made OBMol < "<<endl;
		
		OBGraphSym grsym(temp);
		std::vector<unsigned int> sym;
		grsym.GetSymmetry(sym);
//		cout<<" made graphSym < "<<endl;
		
		std::vector<unsigned int> clabels;
		CanonicalLabels(temp, sym, clabels);
//		cout<<" calculated Labels < "<<endl;
		
		new_frag = new Fragment;
		std::vector <Atom*> aList(num_atoms);
		for(int i=0; i<clabels.size(); i++)
			aList[clabels[i]-1]=( (*it)->getAtom(i) );
//		cout<<" correct atom-List < "<<endl;
		
		for(int i=0; i<clabels.size(); i++)
			new_frag->append(*aList[i]);

//		cout<<" correct molecule < "<<endl;
		(*it)->swap(*new_frag);
		
//		cout<<" updated original < "<<endl;
		delete new_frag;
//		cout<<" DONE "<<endl;
	}
}

///-------------------match queryFragments to libFragments----------------------
void matchRigidFragments(
		boost::unordered_map <BALL::String, TemplateCoord*>& fragmentLib, 
		vector<Fragment*>& fragments)
{
	// get coordinates for rigid fragments
	std::vector< Fragment* >::iterator it2;
	for(it2=fragments.begin(); it2 != fragments.end(); it2++)
	{
		// for all fragments, match these against the lib:
		UCK keyGen(**it2, true, 5);
		TemplateCoord* templat = fragmentLib[ keyGen.getUCK() ];
			
		if(templat && (templat->size() == (*it2)->countAtoms()) )
		{
			templat->transferCoordinates(*it2);
			// DEBUG
			cout<<"Found template for:"<<endl;
			AtomIterator ati = (*it2)->beginAtom();
			for(; +ati; ++ati)
			{
				cout << ati->getElement().getSymbol();
			}
			cout<<endl;
			cout << "key: "<<keyGen.getUCK()<<endl;
			// DEBUG
		}
		else
		{
			cout<<"Warning: could not find a template for ";
			AtomIterator ati = (*it2)->beginAtom();
			for(; +ati; ++ati)
			{
				cout << ati->getElement().getSymbol();
			}
			cout<<endl;
			cout << "key: "<<keyGen.getUCK()<<endl;
		}
	}
}

#endif // MATCHER_H
