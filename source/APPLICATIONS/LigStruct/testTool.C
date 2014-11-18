// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/FORMAT/SDFile.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/STRUCTURE/molecularSimilarity.h>
#include <BALL/STRUCTURE/structureMapper.h>
#include <BALL/STRUCTURE/geometricTransformations.h>

#include <openbabel/obconversion.h>
#include <openbabel/mol.h>

#include <vector>
#include <util.h>
#include <algorithm>
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>

using namespace OpenBabel;
using namespace BALL;
using namespace std;

/// ################# H E L P E R #################
/// ------- read in the library as hash-map
void readConnections(boost::unordered_map <String, Molecule* >& con, String& file_name)
{
	con.clear();
	
	SDFile handle(file_name, ios::in); //open the lib file as sdf-file
	
	// read all lib molecules and save them with their key:
	Molecule* tmp_mol;
	tmp_mol = handle.read(); // first entry
	
	int cnt = 0;
	while(tmp_mol)
	{
		BALL::String key = tmp_mol->getProperty("key").getString();
		con[key] = tmp_mol;
		
		tmp_mol = handle.read();
		cnt++;
	}
	
	handle.close();
}


// compare element+bondorder annotations:
bool compare(pair<String,Atom*>& a, pair<String,Atom*>& b)
{
	return a.first < b.first;
}


/// ------- get connection site
/// 'atm' the atom spanning the site and 'partner' the atom
/// on the other side of the bond that is to be formed.
/// 
int getSite(Atom* atm, vector< Atom* >& site, Atom* partner, String& key)
{
	key = atm->getElement().getSymbol();
	
	// insert central atom as first:
	site.push_back(atm);
	
	// determine the key and the order for atoms
	Atom::BondIterator b_it = atm->beginBond();
	vector< pair<String,Atom*> > elements;
	
	for(; b_it != atm->endBond(); b_it++)
	{
		Atom* tmp_atm = b_it->getBoundAtom(*atm); // get neighbors
		
		String elem = tmp_atm->getElement().getSymbol();
		elem += String(b_it->getOrder());
		
		elements.push_back( make_pair( elem, tmp_atm) );
	}
	
	// also add the partner atom for the new bond:
	elements.push_back( make_pair( String(partner->getElement().getSymbol())+ String(1), partner) );
	
	if (elements.size() < 3)
	{
		cout<<"ERROR: cant connect to this atom, only 1 neighbor atom! currently I need at last 2"<<endl;
		exit(EXIT_FAILURE);
	}
	
	// sort identifers, but keep central atom as first:
	sort( elements.begin(), elements.end(), compare);
	
	// set the next two atoms that are NOT the partner
	// as additional points for a 3 point match site:
	int pos = -1;
	for(int i = 0; i < elements.size(); i++)
	{
		Atom* tmp = elements[i].second;
		if(site.size() < 3)
		{
			if(tmp != partner)
				site.push_back(tmp);
		}
		if(tmp == partner)
		{
			pos = i+1;
		}
	}
	
	// create the key:
	vector< pair<String, Atom*> >::iterator el_it = elements.begin();
	for(; el_it !=elements.end(); el_it++)
	{
		key += ((*el_it).first);
	}

	return pos;
}

/// ------- merge two connection templates to a final template
/// the final template will only contain 6 atoms, 3 for each end
/// starting at position 0 and 3 with the molecules that are to be
/// connected, and then the ordered next two neighbors
void mergeTemplates(Molecule* mol1, int pos1, Molecule* mol2, int pos2, vector<Atom*>& result)
{
	result.clear();
	Atom* aTarget = mol1->getAtom(pos1);
	Atom* bTarget = mol2->getAtom(pos2);
	
	// move template 1 to origin (central atom at origin)
	Vector3& ptA1 = mol1->getAtom(0)->getPosition();
	TranslationProcessor translateA(-ptA1);
	mol1->apply(translateA);
	
	Vector3& a = aTarget->getPosition();

	// move template 2 to origin with partner atom at origin
	Vector3& ptB2 = bTarget->getPosition();
	TranslationProcessor translateB(-ptB2);
	mol2->apply(translateB);
	Vector3& b = mol2->getAtom(0)->getPosition();
	
	// calc transformation to map the templates:
	Angle angl = a.getAngle(b); // negative rot-angle
	Vector3 rot_ax = a % b; // rotation axis
	rot_ax.normalize();
	Matrix4x4 rot_matrix;
	rot_matrix.rotate(-angl, rot_ax); // rotation matrix

	// transfrom the 2nd template
  TransformationProcessor transformer(rot_matrix);
	mol2->apply(transformer);

	// Transfer fitted coordinates to 'result' vector
	result.push_back( mol1->getAtom(0) );
	AtomIterator ati = mol1->beginAtom(); ati++;
	for( ; ati != mol1->endAtom() && result.size() < 3; ati++)
	{
		if(*ati != *aTarget)
			result.push_back(&*ati);
	}
	// Transfer fitted coordinates to 'result' vector
	result.push_back( mol2->getAtom(0) );
	ati = mol2->beginAtom(); ati++;
	for( ; ati != mol2->endAtom() && result.size() < 6; ati++)
	{
		if(*ati != *bTarget)
			result.push_back(&*ati);
	}
	// DEBUG:
	// transfer all atoms to mol1 and write to DEBUG file
	Molecule new_mol;
	for(int i = 0; i<result.size(); i++)
	{
		new_mol.insert(*result[i]);
	}
	SDFile outfile("/Users/pbrach/Desktop/tests/rigid-rigid/test1_debug.sdf", ios::out);
	outfile << new_mol;
	outfile.close();
}

/// ################# M A I N #################
int main(int argc, char* argv[])
{
	CommandlineParser parpars("attachFragments", " connect two fragments", 0.1, String(__DATE__), "Assembly");
//	parpars.registerParameter("i", "input SDF", INFILE, true);
//	parpars.registerParameter("o", "output SDF", OUTFILE, true);
	parpars.registerParameter("l", "output SDF", INFILE, true);
	
//	parpars.setSupportedFormats("i","sdf");
//	parpars.setSupportedFormats("o","sdf");
	parpars.setSupportedFormats("l","sdf");
//	parpars.setOutputFormatSource("o","i");

	String manual = "...just playing...";
	parpars.setToolManual(manual);

	parpars.parse(argc, argv);
	
///======================read all sturctures/templates =========================
	cout<<"Loading connection lib ";
	boost::unordered_map <String, Molecule* > connections;
	String conLib_name = parpars.get("l"); // "/Users/pbrach/Desktop/tests/rigid-rigid/OUT_connections.sdf";
	cout<<" from: "<<conLib_name<<endl;
	readConnections(connections, conLib_name);
	cout<<" done! got "<< connections.size()<<" templates" <<endl;
	
	// the second fragment is connected to the first one by a single bond
	// between the two first atoms
	SDFile fragments("/Users/pbrach/Desktop/tests/rigid-rigid/test1.sdf", ios::in);
	Molecule* frag1 = fragments.read();
	Molecule* frag2 = fragments.read();
	fragments.close();
	
	Atom* con1 = frag1->getAtom(1);
	Atom* con2 = frag2->getAtom(6);
	// test1, connection between: frag1-atom2 sp2 ; frag2-atom7 sp3

///===========find connection site and match against conection library==========
	cout<<"searching sites..."<<endl<<endl;
	///1) find connection sites from the two atom pointers:
	vector< Atom* > site1, site2;
	String key1, key2;
	int pos1 = getSite(con1, site1, con2, key1);
	int pos2 = getSite(con2, site2, con1, key2);
	
	cout<<"site key1: "<<key1<<endl;
	cout<<"site key2: "<<key2<<endl<<endl;
	cout<<"......done!"<<endl<<endl;
	
	cout<<"searching templates..."<<endl;
	///2) find the corresponding templates
	Molecule* templ1 = connections[key1];
	Molecule* templ2 = connections[key2];
	cout<< "found templates: "<< templ1 <<" "<<templ1->countAtoms() <<" - "<<templ2<<" "<<templ2->countAtoms()<<endl;
	cout<<"......done!"<<endl<<endl;
	
	cout<<"creating final templates..."<<endl;
	///3) connect the two templates to one new template
	vector< Atom* > final_tmp;
	mergeTemplates(templ1, pos1, templ2, pos2, final_tmp);
	cout<<"......done!"<<endl<<endl;
	
	cout<<"transforming fragments to connect..."<<endl;
	///4) rotate both sites to match the template
	// calc transformation so that the connection template matches:
	// template coords to be transformed:
	Vector3 tmp_po0 = final_tmp[0]->getPosition();
	Vector3 tmp_po1 = final_tmp[1]->getPosition();
	Vector3 tmp_po2 = final_tmp[2]->getPosition();
	cout<<"getting site1"<<endl;
	cout<<"1: "<< site1[0]<<" 2: "<< site1[1]<<" 3: "<< site1[2]<<endl;
	// calc transformation for fragment1:
	Vector3 frag_po0 = site1[0]->getPosition();
	Vector3 frag_po1 = site1[1]->getPosition();
	Vector3 frag_po2 = site1[2]->getPosition();
	cout<<"got site1"<<endl;
//	Molecule tes1; tes1.insert(*site1[0]);tes1.insert(*site1[1]);tes1.insert(*site1[2]);
//	SDFile outfile1("/Users/pbrach/Desktop/tests/rigid-rigid/test1_frag1.sdf", ios::out);
//	outfile1 << tes1;
	Matrix4x4 trans1 = StructureMapper::matchPoints(frag_po0, frag_po1, frag_po2,
																										tmp_po0, tmp_po1, tmp_po2);
	
	// calc transformation for fragment2:
	tmp_po0 = final_tmp[3]->getPosition();
	tmp_po1 = final_tmp[4]->getPosition();
	tmp_po2 = final_tmp[5]->getPosition();
	//transformation for fragment1:
	cout<<"getting site2"<<endl;
	frag_po0 = site2[0]->getPosition();
	frag_po1 = site2[1]->getPosition();
	frag_po2 = site2[2]->getPosition();

//	Molecule tes2; tes2.insert(*site2[0]);tes2.insert(*site2[1]);tes2.insert(*site2[2]);
//	SDFile outfile2("/Users/pbrach/Desktop/tests/rigid-rigid/test1_frag2.sdf", ios::out);
//	outfile2 << tes2;
	Matrix4x4 trans2 = StructureMapper::matchPoints(frag_po0, frag_po1, frag_po2,
																										tmp_po0, tmp_po1, tmp_po2);

	// apply transformation to the fragments:
	TransformationProcessor transformer;
	transformer.setTransformation(trans1);
	frag1->apply(transformer);
	
	transformer.setTransformation(trans2);
	frag2->apply(transformer);

	cout<<"......done!"<<endl<<endl;
	
	
	cout<<"writing correct molecule type..."<<endl;
	///5) finishing molecule connection on data-type level:
	// form new bond:
	Bond bnd;
	bnd.setOrder(1);
	con1->createBond( bnd,*(con2) );
			
	// transfer atoms to frag1:
	AtomIterator atit = frag2->beginAtom();
	vector<Atom*> atmList;
	for(; atit != frag2->endAtom(); atit++)
	{
		atmList.push_back(&*atit);
	}
	for(int i=0; i < atmList.size(); i++)
	{
		frag1->append(*atmList[i]);
	}
	cout<<"......done!"<<endl<<endl;
	
	cout<<"writing output..."<<endl;
	///6) write output:
	SDFile outfile("/Users/pbrach/Desktop/tests/rigid-rigid/test1_result.sdf", ios::out);
	outfile << *frag1;
}
