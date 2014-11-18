// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/FORMAT/SDFile.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/molecule.h>
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
void readConnections(boost::unordered_map <String, Molecule* > con, String file_name)
{
	con.clear();
	
	SDFile handle(file_name, ios::in); //open the lib file as sdf-file
	
	// read all lib molecules and save them with their key:
	Molecule* tmp_mol;
	tmp_mol = handle.read(); // first entry
	
	while(tmp_mol)
	{
		BALL::String key = tmp_mol->getProperty("key").getString();
		con[key] = tmp_mol;
		
		tmp_mol = handle.read();
	}
	
	handle.close();
}

/// ------- get connection site
String getSite(Atom* atn, vector< Atom* > site)
{
	//TODO
	return String("wurst");
}

/// ------- merge two connection templates to a final template
/// the final template will only contain 6 atoms, 3 for each end
/// starting at position 0 and 3 with the molecules that are to be
/// connected, and then the ordered next two neighbors
void mergeTemplates(Molecule* mol1, Molecule* mol2, vector<Atom*> result)
{
	result.clear();
	//TODO
}

/// ################# M A I N #################
int main(int argc, char* argv[])
{
//	CommandlineParser parpars("attachFragments", " connect two fragments", 0.1, String(__DATE__), "Assembly");
//	parpars.registerParameter("i", "input SDF", INFILE, true);
//	parpars.registerParameter("o", "output SDF", OUTFILE, true);
	
//	parpars.setSupportedFormats("i","sdf");
//	parpars.setSupportedFormats("o","sdf");
//	parpars.setOutputFormatSource("o","i");

//	String manual = "...just playing...";
//	parpars.setToolManual(manual);

//	parpars.parse(argc, argv);
	
///======================read all sturctures/templates =========================
	cout<<"Loading connection lib...";
	boost::unordered_map <String, Molecule* > connections;
	String conLib_name = "/Users/pbrach/Desktop/tests/rigid-rigid/templ_sp2-sp3.sdf";
	readConnections(connections, conLib_name);
	cout<<" done!"<<endl;
	
	// the second fragment is connected to the first one by a single bond
	// between the two first atoms
	SDFile fragments("/Users/pbrach/Desktop/tests/rigid-rigid/test1.sdf", ios::in);
	Molecule* frag1 = fragments.read();
	Molecule* frag2 = fragments.read();
	fragments.close();
	Atom* con1 = frag2->getAtom(1);
	Atom* con2 = frag2->getAtom(6);
	// test1, connection between: frag1-atom2 sp2 ; frag2-atom7 sp3

///===========find connection site and match against conection library==========
	
	///1) find connection sites from the two atom pointers:
	vector< Atom* > site1, site2;
	String key1 = getSite(con1, site1);
	String key2 = getSite(con2, site2);
	
	///2) find the corresponding templates
	Molecule* templ1 = connections[key1];
	Molecule* templ2 = connections[key2];
	
	///3) connect the two templates to one new template
	vector< Atom* > final_tmp;
	mergeTemplates(templ1, templ2, final_tmp);
	
	///4) rotate both sites to match the template
	
	// calc transformation so that the connection template matches:
	// template coords to be transformed:
	Vector3 tmp_po0 = final_tmp[0]->getPosition();
	Vector3 tmp_po1 = final_tmp[1]->getPosition();
	Vector3 tmp_po2 = final_tmp[2]->getPosition();
	// calc transformation for fragment1:
	Vector3 frag_po0 = site1[0]->getPosition();
	Vector3 frag_po1 = site1[1]->getPosition();
	Vector3 frag_po2 = site1[2]->getPosition();
	Matrix4x4 trans1 = StructureMapper::matchPoints(frag_po0, frag_po1, frag_po2,
																										tmp_po0, tmp_po1, tmp_po2);
	
	// calc transformation for fragment2:
	tmp_po0 = final_tmp[3]->getPosition();
	tmp_po1 = final_tmp[4]->getPosition();
	tmp_po2 = final_tmp[5]->getPosition();
	//transformation for fragment1:
	frag_po0 = site1[0]->getPosition();
	frag_po1 = site1[1]->getPosition();
	frag_po2 = site1[2]->getPosition();

	Matrix4x4 trans2 = StructureMapper::matchPoints(frag_po0, frag_po1, frag_po2,
																										tmp_po0, tmp_po1, tmp_po2);

	// apply transformation to the fragments:
	TransformationProcessor transformer;
	transformer.setTransformation(trans1);
	frag1->apply(transformer);
	
	transformer.setTransformation(trans2);
	frag2->apply(transformer);

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
	
	///6) write output:
	SDFile outfile("/Users/pbrach/Desktop/tests/rigid-rigid/test1_result.sdf", ios::out);
	outfile << *frag1;
}
