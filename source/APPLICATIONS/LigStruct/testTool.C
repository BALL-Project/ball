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
#include <map>

using namespace OpenBabel;
using namespace BALL;
using namespace std;

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

	SDFile handle1("/Users/pbrach/Desktop/tests/rigid-rigid/templ_sp2-sp3.sdf", ios::in);
	Molecule* sp2_sp3_templ = handle1.read();
	handle1.close();
	// template atoms:
	//          2
	//           \
	//            0=1
	//           /
	//          3
	//         /|\
	//        4 5 6
	// sp2-sp3 connection; between atom 0 and 3
	
	// the second fragment is connected to the first one by a single bond
	// between the two first atoms
	SDFile fragments("/Users/pbrach/Desktop/tests/rigid-rigid/test1.sdf", ios::in);
	Molecule* frag1 = fragments.read();
	Molecule* frag2 = fragments.read();
	fragments.close();

	// test1, connection between: frag1-atom2 sp2 ; frag2-atom7 sp3

	// calc transformation so that the connection template matches:
	// template coords to be transformed:
	Vector3 tmp1_po0 = sp2_sp3_templ->getAtom(0)->getPosition();
	Vector3 tmp1_po1 = sp2_sp3_templ->getAtom(1)->getPosition();
	Vector3 tmp1_po2 = sp2_sp3_templ->getAtom(2)->getPosition();
	// calc transformation for fragment1:
	Vector3 frag1_po0 = frag1->getAtom(1)->getPosition();
	Vector3 frag1_po1 = frag1->getAtom(0)->getPosition();
	Vector3 frag1_po2 = frag1->getAtom(4)->getPosition();
	Matrix4x4 trans1 = StructureMapper::matchPoints(frag1_po0, frag1_po1, frag1_po2,
																										tmp1_po0, tmp1_po1, tmp1_po2);
	
	// calc transformation for fragment2:
	tmp1_po0 = sp2_sp3_templ->getAtom(3)->getPosition();
	tmp1_po1 = sp2_sp3_templ->getAtom(4)->getPosition();
	tmp1_po2 = sp2_sp3_templ->getAtom(5)->getPosition();
	// coord for fragment2:
	Vector3 frag2_po0 = frag2->getAtom(6)->getPosition();
	Vector3 frag2_po1 = frag2->getAtom(5)->getPosition();
	Vector3 frag2_po2 = frag2->getAtom(7)->getPosition();

	Matrix4x4 trans2 = StructureMapper::matchPoints(frag2_po0, frag2_po1, frag2_po2,
																										tmp1_po0, tmp1_po1, tmp1_po2);

	// apply transformation to the fragments:
	TransformationProcessor transformer;
	transformer.setTransformation(trans1);
	frag1->apply(transformer);
	
	transformer.setTransformation(trans2);
	frag2->apply(transformer);

	
	// form new bond:
	Bond bnd;
	bnd.setOrder(1);
	frag2->getAtom(6)->createBond( bnd,*(frag1->getAtom(1)) );
			
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
	
	/// write output:
	SDFile outfile("/Users/pbrach/Desktop/tests/rigid-rigid/test1_result.sdf", ios::out);
	outfile << *frag1;
}
