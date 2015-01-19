// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/FORMAT/SDFile.h>
#include <BALL/FORMAT/lineBasedFile.h>
#include <BALL/SYSTEM/file.h>

#include <BALL/KERNEL/forEach.h>
#include <BALL/DATATYPE/string.h>

#include <BALL/CONCEPT/composite.h>
#include <BALL/KERNEL/atomContainer.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/PTE.h>

#include <BALL/STRUCTURE/UCK.h>
#include <BALL/STRUCTURE/molecularSimilarity.h>
#include <BALL/STRUCTURE/structureMapper.h>
#include <BALL/STRUCTURE/geometricTransformations.h>
#include <BALL/MATHS/angle.h>
#include <BALL/MATHS/vector3.h>
#include <BALL/MATHS/matrix44.h>


#include <vector>
#include <set>
#include <BALL/QSAR/ringPerceptionProcessor.h>

#include "sources/fragmenter.h"

#include <BALL/STRUCTURE/geometricProperties.h>
#include <BALL/MATHS/quaternion.h>
using namespace OpenBabel;
using namespace BALL;
using namespace std;

void rotate(AtomContainer& mol, Angle& angle)
{
	Vector3 ax2 = mol.getAtom(0)->getPosition();
	Vector3 ax1 = mol.getAtom(1)->getPosition();
	
	/** calculate rotation axis
	*/
	Vector3 v = ax1;

	Matrix4x4 transformation(1, 0, 0, -v.x, 0, 1, 0, -v.y, 0, 0, 1, -v.z, 0, 0, 0, 1);

	Vector3 axis = v - ax2;

	/** calculate rotaton matrix
	*/
	Quaternion quat;

      quat.fromAxisAngle(axis, angle);

	Matrix4x4 rotation;

	quat.getRotationMatrix(rotation);

	transformation = rotation * transformation;

	TransformationProcessor tfp(transformation);
	TranslationProcessor tlp(v);
	
	//apply to atom 2:
	mol.getAtom(3)->apply(tfp);
	mol.getAtom(3)->apply(tlp);
}

/// ################# M A I N #################
int main(int argc, char* argv[])
{
	CommandlineParser parpars("queryFragments", " generate query fragments and connections", 0.1, String(__DATE__), "Preparation");
	parpars.registerParameter("i", "input SDF", INFILE, true);
//	parpars.registerParameter("o", "output SDF", OUTFILE, true);
	
//	parpars.registerParameter("c", "location of conf file", INFILE, false);
//	parpars.setSupportedFormats("c","conf");
	
	parpars.setSupportedFormats("i","sdf");
//	parpars.setSupportedFormats("o","sdf");
//	parpars.setOutputFormatSource("o","i");

	String manual = "...just playing...";
	parpars.setToolManual(manual);

	parpars.parse(argc, argv);
	

/// F R A G M E N T I N G
	Log << "Reading molecule, searching ring atoms..."<<endl;
	SDFile in_file(parpars.get("i"), ios::in);
	SDFile outfile("/Users/pbrach/out.sdf", ios::out);
	Molecule* ball_mol = in_file.read();

	while(ball_mol)
	{
		Atom* at1 = ball_mol->getAtom(3);
		Atom* at2 = ball_mol->getAtom(0);
		Atom* at3 = ball_mol->getAtom(1);
		Atom* at4 = ball_mol->getAtom(2);
		
		Angle ang = calculateTorsionAngle(*at1,*at2,*at3,*at4);

		ang -= Angle(60.0, false);
		rotate( *ball_mol, ang);
		cout<<"angle: "<<calculateTorsionAngle(*at1,*at2,*at3,*at4).toDegree()<<endl;
		
		outfile<< *ball_mol;
		
		ball_mol = in_file.read();
	}
//	FragVec rigid, linker;
//	ConnectList connections;
	
//	MoleculeFragmenter mfrag;
	
//	mfrag.setMolecule( *ball_mol );
//	mfrag.getMoleculeFragments(rigid, linker, connections);
	
//	int i = 1;
//	for( Fragment*& f : rigid)
//	{
//		cout<<"Frag "<<i<<endl;
//		cout<<LigBase::printMol(f)<<endl<<endl;
//		f->setProperty("FragNum", i);
//		++i;
//	}

//	rigid[1]->insert( * rigid[2] );
//	rigid[0]->insert( * rigid[1] );
	
//	i = 1;
//	for( Fragment*& f : rigid)
//	{
//		cout<<"Frag "<<i<<" root:"<<((AtomContainer*) &f->getRoot())->getProperty("FragNum").getInt()<<endl;
//		cout<<LigBase::printMol(f)<<endl<<endl;
//		++i;
//	}
//	AtomContainer* root_0 = (AtomContainer*) &rigid[0]->getRoot();
	
//	cout<<"iterating fragments internal to root: Fragment "<< rigid[0]->getProperty("FragNum").getInt()<<endl;
//	for( AtomContainerIterator cit = root_0->beginAtomContainer(); +cit; ++cit)
//	{
//		AtomContainer& ac =  *((AtomContainer*)&*cit);
//		cout<<"frag "<< ac.getProperty("FragNum").getInt()<<endl;
//	}
	
	Log << "......done!"<<endl;
}

