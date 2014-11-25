// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/FORMAT/SDFile.h>
#include <BALL/SYSTEM/file.h>
#include <BALL/FORMAT/lineBasedFile.h>
#include <BALL/DATATYPE/string.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/atomContainer.h>
#include <BALL/CONCEPT/composite.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/STRUCTURE/molecularSimilarity.h>
#include <BALL/STRUCTURE/structureMapper.h>
#include <BALL/STRUCTURE/geometricTransformations.h>

#include <BALL/MATHS/angle.h>
#include <BALL/MATHS/vector3.h>
#include <BALL/MATHS/matrix44.h>

#include <vector>
#include <util.h>
#include <algorithm>
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>

using namespace OpenBabel;
using namespace BALL;
using namespace std;

/// ################# H E L P E R    F U N C T I O N S #################
// Write several result molecules to one file
void writeMolVec(vector<Molecule> &input, SDFile* handle)
{
	for(int i = 0; i < input.size(); i++)
	{
		(*handle) << input[i];
	}
}

/// try all atom pair assignments to get the best rmsd
/// (do not test the central atom, because it is the translation center and
///  thus should always be 0 anyways)
/// ----------------------------------------------------------------------------
/*
 * Bond Order and element should fit before
 * one even tries to get an rmsd???
 * 
 * 
 */
float getBestRMSD(Molecule* li1, Molecule* li2)
{
	float result = 0;
	AtomIterator at1 = li1->beginAtom();
	for (++at1; +at1 ; at1++)
	{
		float smallest = 1000;
		
		AtomIterator at2 = li2->beginAtom();
		for (++at2; +at2; at2++)
		{
			float dist = at1->getDistance(*at2);
			if ( dist < smallest)
				smallest = dist;
		}
		result += smallest;
	}
	result /= li1->countAtoms();
	return result;
}

/// Helper to get a single key component
String getBondName(Atom& atm)
{
	String name = atm.getElement().getSymbol();
	name += atm.beginBond()->getOrder();
	return name;
}

/// Find the atom in 'mol', that has same element and bond order as 'atm'
/// ----------------------------------------------------------------------------
Atom* getMatchingAtom(Molecule* mol, Atom* atm)
{
	String elem = atm->getElement().getSymbol();
	int bo = atm->beginBond()->getOrder(); //should have only one bond

	AtomIterator ati = mol->beginAtom();
	ati++; // first atom is the center atom, which we never want
	for(; +ati; ++ati)
	{
		if(ati->getElement().getSymbol() == elem && ati->beginBond()->getOrder() == bo)
			return &*ati;
	}
	
	// DEBUG:
	ati = mol->beginAtom();
	cout <<endl<< "I am Searching for: " <<elem<<" BO:"<< bo<<endl;
	for( ; +ati; ati++)
	{
		cout<< ati->getElement().getSymbol()<< " "<< ati->beginBond()->getOrder()<<endl;
	}
	cout<<"Key: "<<mol->getProperty("key").getString()<<endl;
	cout<<"Key: "<<((Molecule*)atm->getParent())->getProperty("key").getString()<<endl;
	// DEBUG END
	
	cout<<"ERROR: could not find a partner Atom"<<endl;
	exit(EXIT_FAILURE);
}

bool compatible(Atom* at1,Atom* at2)
{
	return (at1->getElement().getSymbol() == at2->getElement().getSymbol() 
					&& at1->beginBond()->getOrder() == at2->beginBond()->getOrder());
}

/// Structurally align a star-graph molecule (one central atom with any 
/// number of direct neighbors, but no neighbors of dist 2).
/// 
/// Currently we are only interested in the optimal RMSD but one could also
/// change the method to yield the optimal transformation matrix. The problem
/// here is, that for such molecules a unique ordering for the atoms can not
/// always be achived (e.g.: center atoms with 4 identical neighbors and bond 
/// types)
/// 
/// ----------------------------------------------------------------------------
/// My current primitive solution is:
//1.) Catch simple cases of: 1 central atom and 1 neighbor, or 2 neighbors
//    (here we directly know the optimal transformation)
//
//2.) Find unique atoms. If two could be found, solve the simple case
//
//3.) Selection of first neighbor:
//    Check if a single unique atom exists, if so use it otherwise select 
//    the first neighbor atom (second atom entry) 
//
//    Find in the other molecule the first atom with identical Element+BO

//4.) Selection of second neighbor:
//    Check if another unique atom exists among the remaining atoms. If so
//    use it (and the respecctive partner in mol2) and you are done. 
//
//    Otherwise, select any not selected atom from mol1 and try all possible 
//    pairings with the remaining atoms in mol2. Select the pairing that 
//    yields the lowest RMSD.
float star_align(Molecule* mol1, Molecule* mol2)
{
	Vector3 frag2, frag3, tem2, tem3; //the vectors to calc the transformaion
	Matrix4x4 trans_matrix;
	TransformationProcessor transformer;
	
	// set the two center atoms (which are always the first atoms):
	Vector3 frag1 = mol1->getAtom(0)->getPosition();
	Vector3 tem1  = mol2->getAtom(0)->getPosition();
	
	/// 1.) simple solutions for only one or two neighbors
	int num_atoms = mol1->countAtoms();
	if(num_atoms < 4)
	{
		if(num_atoms == 2) /// WARNING: could not yet test this case!!!
		{
			// position 0 has always the center, leaves pos 1 as the only possible partner
			frag2 = mol1->getAtom(1)->getPosition();
			tem2  = mol2->getAtom(1)->getPosition();
			
			// create dummys to use the 3 point matching
			frag3 = Vector3();
			tem3  = Vector3();
		}
		else if(num_atoms == 3)
		{
			// get correct atom assignment:
			Atom *temp_atA2, *temp_atA3, *temp_atB2, *temp_atB3;
			
			temp_atA2 = mol1->getAtom(1);
			temp_atB2 = getMatchingAtom(mol2, temp_atA2);
			
			temp_atA3 = mol1->getAtom(2);
			// the respective remaining atom in mol2 is the one we need for 'temp_atB3'
			temp_atB3 = mol2->getAtom(2);
			if(temp_atB3 == temp_atB2)
				temp_atB3 = mol2->getAtom(1);
			
			// get positions:
			frag2 = temp_atA2->getPosition();
			frag3 = temp_atA3->getPosition();
			tem2  = temp_atB2->getPosition();
			tem3  = temp_atB3->getPosition();
		}
		// got no time to take care of the possible sign errors thus simply use 3-point-matching:
		trans_matrix = StructureMapper::matchPoints(frag1, frag2, frag3, tem1, tem2, tem3);
		transformer.setTransformation(trans_matrix);
		mol1->apply(transformer);
		
		return getBestRMSD(mol1, mol2);
	}
	/// 2.) At least 3 neighbors are given, see if unique atoms exist in mol1
	vector< Atom* > unique_atm;
	AtomIterator atmi = mol1->beginAtom();
	for (atmi++; atmi != mol1->endAtom(); atmi++)
	{
//		cout<<endl;
		bool isUnique = true;
		AtomIterator atmk = mol1->beginAtom();
		for(atmk++; +atmk; atmk++)
		{
//			cout<<atmi->getElement().getSymbol()<<" - "<<atmk->getElement().getSymbol();
//			cout<<" | "<<atmi->beginBond()->getOrder() <<" - "<<atmk->beginBond()->getOrder()<<endl;
			if( atmi->getElement() == atmk->getElement() 
					&& atmi->beginBond()->getOrder() == atmk->beginBond()->getOrder()
					&& &*atmi != &*atmk)
			{
				isUnique = false;
				break;
			}
		}
		if(isUnique)
		{
//			cout<<atmi->getElement().getSymbol()<<" is unique"<<endl;
			unique_atm.push_back(&*atmi);
		}
	}
//	cout<<"num uniques: "<<unique_atm.size()<<endl;
	///2.) ideal case of at lasttwo unique ones. 
	///    Here we have all we need and my return early
	if (unique_atm.size() > 1)
	{
		Atom* selectionA1 = mol1->getAtom(0);
		frag1 = selectionA1->getPosition(); 
		frag2 = unique_atm[0]->getPosition();
		frag3 = unique_atm[1]->getPosition();
		
		Atom* selectionB1 = mol2->getAtom(0);
		tem1  = selectionB1->getPosition();
		tem2 = getMatchingAtom( mol2, unique_atm[0] )->getPosition();
		tem3 = getMatchingAtom( mol2, unique_atm[1] )->getPosition();
		
		// get RMSD:
		trans_matrix = StructureMapper::matchPoints(frag1, frag2, frag3, tem1, tem2, tem3);
		transformer.setTransformation(trans_matrix);
		mol1->apply(transformer);
		
		return getBestRMSD(mol1, mol2);
	}

	///3.) Just one unique atom was found, here we need to test some transformations
	///    to find the best one. Try all possibilities for the third atom:
	if (unique_atm.size() == 1) { 
		// if at least one mol1 atom was unique, it is curcial to use it:
		Atom* uniqueA = unique_atm[0];
		Atom* uniqueB = getMatchingAtom( mol2, uniqueA );
		
		// get next non unique atom from mol1:
		Atom* sel=0;
		AtomIterator ato = mol1->beginAtom();
		for(ato++; +ato; ato++)
		{
			if(&*ato != uniqueA){
				sel = &*ato;
				break;
			}
		}
		
		// test all possible assignments of the selected third atom of mol1
		// to an atom of mol2
		float best_rmsd = 100;
		
		ato = mol2->beginAtom();
		for(ato++; +ato; ato++)
		{
			float rmsd = 1000;
			if(&*ato != uniqueB)
			{
				Atom* selectionA1 = mol1->getAtom(0);Atom* selectionB1 = mol2->getAtom(0);
				frag1 = selectionA1->getPosition(); tem1  = selectionB1->getPosition();
				frag2 = uniqueA->getPosition();
				tem2  = uniqueB->getPosition();
				frag3 = sel->getPosition();
				tem3  = ato->getPosition();
				
				trans_matrix = StructureMapper::matchPoints(frag1, frag2, frag3, tem1, tem2, tem3);
				transformer.setTransformation(trans_matrix);
				mol1->apply(transformer);
				
				rmsd = getBestRMSD(mol1, mol2);
				if (rmsd < best_rmsd)
					best_rmsd = rmsd;
			}
		}
		return best_rmsd;
	}
	///4.) No unique atom was found. Here we have to check every possible
	///    assignment for the two remaining atoms...
	// if at least one mol1 atom was unique, it is curcial to use it:
	float best_rmsd = 100;
	Atom* selectionA1 = mol1->getAtom(0);
	Atom* selectionA2 = mol1->getAtom(1);
	Atom* selectionA3 = mol1->getAtom(2);
	
	Atom* selectionB1 = mol2->getAtom(0);
	Atom* selectionB2 = 0;
	Atom* selectionB3 = 0;
	
	// test all possible assignments to 'selectionA2'
	AtomIterator ati = mol2->beginAtom();
	for(ati++; +ati; ati++)
	{
		selectionB2 = &*ati;
		if( compatible(selectionA2, selectionB2) )
		{
			// test all possible assignments to 'selectionA3'
			AtomIterator ato = mol2->beginAtom();
			for(ato++; +ato; ato++)
			{
				selectionB3 = &*ato;
				if(selectionB2 == selectionB3)
					continue;
				
				float rmsd = 1000;
				frag1 = selectionA1->getPosition(); tem1  = selectionB1->getPosition();
				frag2 = selectionA2->getPosition(); tem2  = selectionB2->getPosition();
				frag3 = selectionA3->getPosition(); tem3  = selectionB3->getPosition();
					
				trans_matrix = StructureMapper::matchPoints(frag1, frag2, frag3, tem1, tem2, tem3);
				transformer.setTransformation(trans_matrix);
				mol1->apply(transformer);
				
				rmsd = getBestRMSD(mol1, mol2);
				if (rmsd < best_rmsd)
					best_rmsd = rmsd;
			}
		}
	}
	return best_rmsd;
}



/// Compare a pair of (int , molecule*) according to the int field
/// ----------------------------------------------------------------------------
bool comparator(pair<int, Molecule*>& a, pair<int, Molecule*>& b)
{
	return b.first < a.first;
}



/// Find Bins depending on an RMSD threshold
/// ----------------------------------------------------------------------------
void getBins(vector< pair<int, Molecule*> >& bins, vector<Molecule*>& mols)
{
	const float THRESHOLD = 0.2;
	/// align each molecule with each other, if they are identical insert to map:
//	boost::unordered_map< Molecule*, int > local_bins;
	map< Molecule*, int > local_bins;
	
	// check if we have more than one template available
	if(mols.size() <2){
		if(mols.size() == 1)
		{
			bins.push_back( make_pair(1, mols[0]) );
		}
		else{
			Log<<"ERROR: got a key without any molecule!"<<endl;
			exit(EXIT_FAILURE);
		}
		return;
	}
	
	vector<Molecule*>::iterator it1, it2;
	for(it1 = mols.begin(); it1 != mols.end(); it1++)
	{
		// do not test molecules AS FISRT molecule that were already replaced
		if( local_bins.find(*it1) != local_bins.end() )
			continue;
		
		// create a 'bin' for this molecule
		local_bins[*it1] += 1;
		
		for(it2 = it1 + 1; it2 != mols.end(); it2++)
		{
			// do not test molecules AS SECOND molecule that were already replaced
			if( local_bins.find(*it2) != local_bins.end() )
				continue;
			
			// align mol2 to match mol1 (special alignment for star-molecules) and get RMSD
			float rmsd = star_align(*it2, *it1);
			if ( rmsd < THRESHOLD)
			{
				delete *it2;
				*it2 = *it1; // replace pointer by the group representative
				
				local_bins[*it1] += 1;
			}
		}
	}

	// transform local bins to result 'bins':
//	boost::unordered_map< Molecule*, int >::iterator it = local_bins.begin();
	map< Molecule*, int >::iterator it = local_bins.begin();
	for(; it != local_bins.end(); it++)
	{
		bins.push_back( make_pair((*it).second, (*it).first) );
	}
}
/// ################# M A I N #################
int main(int argc, char* argv[])
{
	CommandlineParser parpars("connectionLibFilter", "select best templates", 0.1, String(__DATE__), "Preparation");
	parpars.registerParameter("i", "input SDF", INFILE, true);
	parpars.registerParameter("o", "output SDF", OUTFILE, true);
	
	parpars.registerFlag("unique", "only output one fragment for each topology");

	parpars.setSupportedFormats("i","sdf");
	parpars.setSupportedFormats("o","sdf");

	String manual = "For creating a connection library.";
	parpars.setToolManual(manual);

	parpars.parse(argc, argv);
	
/// START of CODE#############################################################

///1.) Read all molecules and sort to their respective connection-class:
	
	map <String, vector<Molecule*> > connection_classes;
	//boost::unordered_map <String, vector<Molecule*> > connection_classes;
	
	Log << "Reading input connections from:"<<endl;
	Log << String(parpars.get("i"))<<endl;
	SDFile infile(parpars.get("i"), ios::in); // OPEN IN-FILE
	Molecule* work_frag =	infile.read();
	while ( work_frag )
	{
		String key = work_frag->getProperty("key").getString();
		connection_classes[key].push_back(work_frag);

		work_frag = infile.read(); // read next
	}
	Log << "Read "<< connection_classes.size()<< " connection classes" << endl;
	
	infile.close();
	Log << "done reading."<<endl;
	
	
///2.) Loop over all classes to find the 'best representative':
///
	SDFile outfile(String(parpars.get("o")), ios::out);// OPEN OUT-FILE
//	boost::unordered_map <String, vector<Molecule*> >::iterator cla_it = connection_classes.begin();
	map <String, vector<Molecule*> >::iterator cla_it = connection_classes.begin();
	for(; cla_it != connection_classes.end(); cla_it++)
	{
		///2.1) Get sets of identical molecules
		vector< pair<int, Molecule*> > bins;
		getBins(bins, (*cla_it).second );
		
		///2.2) Select bin with most frequent molecule
		sort(bins.begin(), bins.end(), comparator);
		
		///2.3) write most frequent molecule to outfile
		Log<<" bin "<<(bins[0].second)->getProperty("key").getString()<<" contains: "<<bins[0].first<<endl;
		outfile<< *(bins[0].second);
		
		///2.4) free memory:
		//connection_classes.erase(cla_it);
		for (int i = 0; i < bins.size(); i++){
			delete bins[i].second;
		}
	}
	Log <<endl<<"wrote " << connection_classes.size()<<" structures"<<endl;
	outfile.close();
}
