#ifndef MATCHER_H
#define MATCHER_H

#include "basic.h"

///####################### M A T C H I N G ##############################
///---------------make atomlists of the fragments canonical---------------------
using namespace OpenBabel;
using namespace BALL;

/** Generate a canonical order of atoms
 * 
 */
void canonicalize(std::vector <Molecule*>& fragments)
{
	
	int num_atoms = -1;// obMol.NumAtoms();
//	cout<<"Canonicalising the atomlists..."<<endl;
	std::vector< Molecule* >::iterator it;
	Molecule* new_mol;
	for(it=fragments.begin(); it != fragments.end(); it++)
	{
		num_atoms = (*it)->countAtoms();
//		cout<<(*it)->getName()<< " has #atoms: "<<(*it)->countAtoms()<<endl;
//		cout<<"DEBUG: "<<endl;
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
		
		new_mol = new Molecule;
		std::vector <Atom*> aList(num_atoms);
		for(int i=0; i<clabels.size(); i++)
			aList[clabels[i]-1]=( (*it)->getAtom(i) );
//		cout<<" correct atom-List < "<<endl;
		
		for(int i=0; i<clabels.size(); i++)
			new_mol->append(*aList[i]);

//		cout<<" correct molecule < "<<endl;
		(*it)->swap(*new_mol);
		
//		cout<<" updated original < "<<endl;
		delete new_mol;
//		cout<<" DONE "<<endl;
	}
}

///-------------------match queryFragments to libFragments----------------------
void matchRigidFragments(
		boost::unordered_map <BALL::String, TemplateCoord*>& fragmentLib, 
		vector<Molecule*>& fragments)
{
	// get coordinates for rigid fragments
	std::vector< Molecule* >::iterator it2;
	for(it2=fragments.begin(); it2 != fragments.end(); it2++)
	{
		// for all fragments, match these against the lib:
		UCK keyGen(**it2, true, 5);
		TemplateCoord* templat = fragmentLib[ keyGen.getUCK() ];
			
		if(templat && (templat->size() == (*it2)->countAtoms()) )
			setCoordinates(*it2, templat);
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


/// Old Matcher................
void setCoordinates(Molecule* query, Molecule* templat)
{
	AtomIterator qit = query->beginAtom();
	AtomIterator tit = templat->beginAtom();
	for (; qit != query->endAtom(); qit++, tit++)
	{
		qit->setPosition( tit->getPosition() );
	}
}

/// Old Matcher................
void matchRigidFragments(boost::unordered_map <BALL::String, Molecule*>& fragmentLib, vector<Molecule*>& fragments)
{
	// get coordinates for rigid fragments
	std::vector< Molecule* >::iterator it2;
	for(it2=fragments.begin(); it2 != fragments.end(); it2++)
	{
		// for all fragments, match these against the lib:
		UCK keyGen(**it2, true, 5);
		Molecule* templat = fragmentLib[ keyGen.getUCK() ];
		
		if(templat && (templat->countAtoms() == (*it2)->countAtoms()) )
			setCoordinates(*it2, templat);
		else
			cout<<"Warning: could not find a template for "<< (*it2)->getName()<<endl;
		
	}
}

#endif // MATCHER_H
