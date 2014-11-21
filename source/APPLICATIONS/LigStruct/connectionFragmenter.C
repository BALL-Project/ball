// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/FORMAT/SDFile.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/DATATYPE/string.h>
#include <BALL/STRUCTURE/molecularSimilarity.h>

#include <openbabel/mol.h>
#include <openbabel/bond.h>
#include <openbabel/atom.h>
#include <openbabel/obconversion.h>

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


// compare element+bondorder annotations:
bool compare(pair<String,Atom*>& a, pair<String,Atom*>& b)
{
	return a.first < b.first;
}
// find all direct neighbours
// 
Molecule* getConnection(OBAtom* atm, Molecule* mol)
{
//	cout<<"working with atom: "<<atm<<" and molecule "<< mol <<endl;
	Molecule * workMol = new Molecule;
	String key = "";
	
	int id_main = atm->GetId();
//	cout<<"copy atom "<<endl;
	Atom* mainAtm = new Atom(*(mol->getAtom(id_main)), true);
	
	key = mainAtm->getElement().getSymbol();

	workMol->insert(*mainAtm);
	
	vector< pair<String,Atom*> > elements;
	OBBondIterator bit = atm->BeginBonds();

	for(; bit != atm->EndBonds(); bit++ )
	{
		OBBond* bnd = *bit;
		unsigned int bnd_od = bnd->GetBondOrder();
		char* elem;
		Atom* tmp;
		if (bnd->GetBeginAtom() != atm)
		{
			elem = bnd->GetBeginAtom()->GetType();
//			cout<<"new atom: "<<bnd->GetBeginAtom()->GetId()<<endl;
			tmp = new Atom(*mol->getAtom(bnd->GetBeginAtom()->GetId()));
		}
		else
		{
//			cout<<"new ENDatom: "<<bnd->GetEndAtom()->GetId()<<endl;
//			cout<<"mol has "<<mol->countAtoms()<<" atoms"<<endl;
//			cout<<"atm pointer: "<< mol->getAtom(bnd->GetEndAtom()->GetId()) <<endl;
			elem = bnd->GetEndAtom()->GetType();
			tmp = new Atom( *mol->getAtom(bnd->GetEndAtom()->GetId()) );
		}

		Bond* tmp_bnd = new Bond();
		tmp_bnd->setOrder(bnd_od);
//		cout<<"creating artificial bond with BO: "<<bnd_od<<endl;
		mainAtm->createBond(*tmp_bnd, *tmp);
		
		elements.push_back( make_pair(String(*elem) + String(bnd_od), tmp) );
	}
	
	// sort identifers
	sort(elements.begin(), elements.end(), compare);
	
	vector< pair<String, Atom*> >::iterator el_it = elements.begin();
	for(; el_it !=elements.end(); el_it++)
	{
		key += ((*el_it).first);
		workMol->insert( *((*el_it).second) );
	}
	
	workMol->setProperty("key", key);
	return workMol;
}

// gather infos about the occuring bond lengths
void addToLenghts(OBBond* bnd, boost::unordered_map <String, vector<double> >& lengths)
{
	String key1 ="";
	String key2 ="";
	char a = *(bnd->GetBeginAtom()->GetType());
	char b = *(bnd->GetEndAtom()->GetType());
	key1 += a;
	key1 += b;
	
	key2 += b;//bnd->GetEndAtom()->GetType() ;
	key2 += a;//bnd->GetBeginAtom()->GetType() ;
	double len = bnd->GetLength();
	
	if(len < 0.6 || len > 3.5)
		cout<< "WARNING found dist of: "<< len << " for "<< key1<<endl;
	
	if( (lengths.find(key1) == lengths.end()) && (lengths.find(key2) == lengths.end()) )
	{
		vector<double> tmp(4,0);
		tmp[0] = len; // sum
		tmp[1] = len; // min
		tmp[2] = len; // max
		tmp[3] = 1.;  // number of entries
		lengths[key1]=tmp;
	}
	else
	{
		vector<double>* tmp;
		
		if(lengths.find(key1) != lengths.end())
			tmp = &(lengths[key1]);
		else
			tmp = &(lengths[key2]);
			
		tmp->at(3) = tmp->at(3) + 1.;
		tmp->at(0) = tmp->at(0) + len;
		if(len < (*tmp)[1])
			(*tmp)[1] = len;
		if(len > (*tmp)[2])
			(*tmp)[2] = len;
	}
}
/// ################# M A I N #################
int main(int argc, char* argv[])
{
	CommandlineParser parpars("libFragmenter", "cut a molecule along its rotable bonds, generating fragments", 0.1, String(__DATE__), "Preparation");
	parpars.registerParameter("i", "input SDF", INFILE, true);
	parpars.registerParameter("o", "output SDF", OUTFILE, true);
	
	parpars.registerFlag("unique", "only output one fragment for each topology");

	parpars.setSupportedFormats("i","sdf");
	parpars.setSupportedFormats("o","sdf");

	String manual = "For creating a connection library.";
	parpars.setToolManual(manual);

	parpars.parse(argc, argv);
	
	// START of CODE#############################################################
	
	OBMol obMol; // working molecule
	
	// Read open-babel molecule as input:
	OBConversion conv;
	OBFormat *format = conv.FormatFromExt(parpars.get("i"));
	
	if (!format || !conv.SetInFormat(format))
		cout << "Could not find input format for file " << parpars.get("i") << endl;

	ifstream ifs(parpars.get("i")); // open file
	
	if (!ifs)
		cout << "Could not open " << parpars.get("i") << " for reading." << endl;
	
	// open output file:
	String outfile_name = String(parpars.get("o"));
	SDFile outfile(outfile_name, ios::out);
	
	File bondFile(outfile_name+"bondlen.txt", ios::out);
	
	
	vector<OBBond*> rotors;
	vector<Molecule> connections;
	Molecule* ball_mol;
	int cntr=0;
	boost::unordered_set< String > used; // used fragment keys
	boost::unordered_map <String, vector<double> > lengths;
	
	// Read all molecules.
	while ( conv.Read(&obMol, &ifs) )
	{
		// init ring and rotable information:
		obMol.FindRingAtomsAndBonds();
		
		rotors.clear();
//		connections.clear();
		
		// get only rotor-bonds for connections
		FOR_BONDS_OF_MOL(b, obMol){
			if (b->IsRotor()) 
				rotors.push_back(&(*b));
		}
		
//		ball_mol = MolecularSimilarity::createMolecule(obMol, false);
		vector<OBBond*>::iterator it;
		for(it=rotors.begin(); it!=rotors.end(); ++it)
		{
			//1) get the two sub-mols per bond
			OBBond* bnd = (*it);
			
			addToLenghts(bnd, lengths);

//			Molecule* new_mol1 = getConnection(bnd->GetBeginAtom(), ball_mol);
//			Molecule* new_mol2 = getConnection(bnd->GetEndAtom(), ball_mol);
			
//			String key1 = new_mol1->getProperty("key").getString();
//			String key2 = new_mol2->getProperty("key").getString();
			
//			//3) filter out if already known connections
//			if(parpars.has("unique"))
//			{
//				if( used.find(key1) == used.end() )
//				{
//					//4) create the connections
//					used.insert(key1);
//					connections.push_back(*new_mol1);
//				}
//				else{
//					delete new_mol1;
//				}
				
//				if( used.find(key2) == used.end() )
//				{
//					//4) create the connections
//					used.insert(key2);
//					connections.push_back(*new_mol2);
//				}
//				else{
//					delete new_mol2;
//				}
//			}
//			else
//			{
//				//4) create the connections
//				connections.push_back(*new_mol1);
//				connections.push_back(*new_mol2);
//			}
		}
//		//5) write all connections out
/// write to output-------------------------------------------------------------
		
//		writeMolVec(connections, &outfile);
//		delete ball_mol;
//		obMol.Clear();
//		cntr++;
	}
	outfile.close();
	ifs.close();
	Log << "read "<< cntr<<" input structures, wrote fragments to: " << outfile_name << endl;
	
	// print results of standard lengths calculation:
	boost::unordered_map< String, vector<double> >::iterator mit = lengths.begin();
	for(; mit != lengths.end(); mit++)
	{
		// type-ids, avg-length, min-length, max-length, number-of-occurences
		bondFile <<mit->first<<" "<< (mit->second[0] / mit->second[3]);
		bondFile <<" "<< mit->second[1]<<" " << mit->second[2];
		bondFile << " "<< mit->second[3]<<endl;
	}
	bondFile.close();
}
