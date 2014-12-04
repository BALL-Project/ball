#ifndef MATCHER_H
#define MATCHER_H

//#include <BALL/FORMAT/commandlineParser.h>
//#include <BALL/FORMAT/SDFile.h>
//#include <BALL/FORMAT/lineBasedFile.h>
//#include <BALL/SYSTEM/file.h>

//#include <BALL/KERNEL/forEach.h>
//#include <BALL/DATATYPE/string.h>

//#include <BALL/KERNEL/atomContainer.h>
//#include <BALL/KERNEL/molecule.h>
//#include <BALL/KERNEL/bond.h>
//#include <BALL/KERNEL/atom.h>
//#include <BALL/KERNEL/PTE.h>

//#include <BALL/STRUCTURE/UCK.h>
//#include <BALL/STRUCTURE/molecularSimilarity.h>
//#include <BALL/STRUCTURE/structureMapper.h>
//#include <BALL/STRUCTURE/geometricTransformations.h>
//#include <BALL/MATHS/angle.h>
//#include <BALL/MATHS/vector3.h>
//#include <BALL/MATHS/matrix44.h>

//#include <openbabel/obconversion.h>
//#include <openbabel/mol.h>
//#include <openbabel/canon.h>
//#include <openbabel/graphsym.h>

//#include <vector>

//#include <boost/unordered_map.hpp>
//#include <boost/pending/disjoint_sets.hpp>

#include "basic.h"

///####################### M A T C H I N G ##############################
///---------------make atomlists of the fragments canonical---------------------
using namespace OpenBabel;
using namespace BALL;

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
		
		copyMoleculeProperies(**it, *new_mol);
		(*it)->swap(*new_mol);
		
//		cout<<" updated original < "<<endl;
		delete new_mol;
//		cout<<" DONE "<<endl;
	}
}

///-------------------match queryFragments to libFragments----------------------
void matchFragments(boost::unordered_map <BALL::String, Molecule*>& fragmentLib, vector<Molecule*>& fragments)
{
	// get coordinates for rigid fragments
	std::vector< Molecule* >::iterator it2;
	for(it2=fragments.begin(); it2 != fragments.end(); it2++)
	{
		// for all rigid fragments, match these against the lib:
		if( (*it2)->getProperty("isRigid").getBool() )
		{
			UCK keyGen(**it2, true, 5);
			Molecule* templat = fragmentLib[ keyGen.getUCK() ];
			
			if(templat && (templat->countAtoms() == (*it2)->countAtoms()) )
				setCoordinates(*it2, templat);
			else
				cout<<"Warning: could not find a template for "<< (*it2)->getName()<<endl;
		}
	}
}


#endif // MATCHER_H
