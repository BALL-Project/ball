#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/FORMAT/dockResultFile.h>
#include <BALL/DOCKING/COMMON/flexibleMolecule.h>
#include <BALL/DOCKING/COMMON/conformation.h>
#include <BALL/DOCKING/COMMON/result.h>
#include <BALL/FORMAT/SDFile.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/bondIterator.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/MATHS/vector3.h>
#include <BALL/STRUCTURE/defaultProcessors.h>
#include <BALL/STRUCTURE/geometricTransformations.h>

#include <iostream>
///////////////////////////

using namespace BALL;
using namespace std;
using BALL::DockResultFile;
using BALL::Ligand;
using BALL::Result;
using BALL::Conformation;

bool compareMolecules(Molecule& mol1, Molecule& mol2)
{
	if(mol1.countAtoms()!=mol2.countAtoms())
		return false;
	if(mol1.countBonds()!=mol2.countBonds())
		return false;

	AtomIterator ai;
	vector<Vector3> pos1;
	vector<float> q1;
	BALL_FOREACH_ATOM(mol1, ai)
	{
		pos1.push_back(ai->getPosition());
		q1.push_back(ai->getCharge());
	}

	vector<Vector3> pos2;
	vector<float> q2;
	BALL_FOREACH_ATOM(mol2, ai)
	{
		pos2.push_back(ai->getPosition());
		q2.push_back(ai->getCharge());
	}

	for(unsigned int i=0;i<pos1.size();i++)
	{
		if( fabs(pos1[i].x-pos2[i].x)>1e-8 )
			return false;
		if( fabs(pos1[i].y-pos2[i].y)>1e-8 )
			return false;
		if( fabs(pos1[i].z-pos2[i].z)>1e-8 )
			return false;
		if( fabs(q1[i]-q2[i])>1e-8 )
			return false;
	}



	AtomBondIterator bi;

	vector<String> pairs1;
	BALL_FOREACH_BOND(mol1, ai, bi)
    {
	Bond* bnd = &(*bi);
	const Atom* at1 = bnd->getFirstAtom();
	const Atom* at2 = bnd->getSecondAtom();
	String pair = at1->getElement().getSymbol()+"-"+String(bnd->getOrder())+"-"+at2->getElement().getSymbol();
		pairs1.push_back(pair);
    }

	vector<String> pairs2;
	BALL_FOREACH_BOND(mol2, ai, bi)
    {
	Bond* bnd = &(*bi);
	const Atom* at1 = bnd->getFirstAtom();
	const Atom* at2 = bnd->getSecondAtom();
	String pair = at1->getElement().getSymbol()+"-"+String(bnd->getOrder())+"-"+at2->getElement().getSymbol();
		pairs2.push_back(pair);
    }

	if(pairs1.size()!=pairs2.size())
		return false;
	for(unsigned int i=0;i<pairs1.size();i++)
		if(pairs1[i]!=pairs2[i])
			return false;

	return true;
}


START_TEST(DockResultFile)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////


CHECK(read/write DockResultFile)



        SDFile f(BALL_TEST_DATA_PATH(QSAR_test.sdf));
		Molecule* mol;
		String tmpfile;
		File::createTemporaryFilename(tmpfile);
		DockResultFile df(tmpfile,File::MODE_OUT);
		vector<Molecule*> mols;
		int id=0;
		while( (mol = f.read()) )
		{
			// skip this molecule because its
			// conformation is contained twice
			// in this set
			if(mol->getName()=="THIOL_4")
				continue;
			mol->setProperty("ID",String(id));
			df.write(*mol);
			mols.push_back(mol);
			id++;
		}
		df.close();



		// now iterate molecule-by-molecule over the file
		DockResultFile dfin(tmpfile,File::MODE_IN);
		Molecule* mol2;
		unsigned int i=0;
		while( (mol2 = dfin.read()) )
		{
			bool cmp = compareMolecules(*mols[i],*mol2);
			TEST_EQUAL(cmp,true)
			TEST_EQUAL(mol2->getProperty("ID").getString(),mols[i]->getProperty("ID").getString())
			delete mol2;
			i++;
		}
		dfin.close();



		DockResultFile dfin2(tmpfile,File::MODE_IN);
		Ligand* lig = dfin2.readLigand();
		TEST_EQUAL(lig->hasConformation("f5fdd18549299b09eedbe0131fb46fbc02b10efb"),true)
		TEST_EQUAL(lig->getConformationId(0),"f5fdd18549299b09eedbe0131fb46fbc02b10efb")
		TEST_EQUAL(lig->getNumberOfConformations(),1)
		delete lig;
		dfin2.close();



		// now iterate ligand-by-ligand over the file
		DockResultFile dfin3(tmpfile,File::MODE_IN);
		i=0;
		while( (lig=dfin3.readLigand()) )
		{
			Molecule* mol = lig->getConformer(0);
			bool cmp = compareMolecules(*mols[i],*mol);
			TEST_EQUAL(cmp,true)

			// there is no second conformer so returned
			// pointer should be NULL
			TEST_EQUAL(lig->getConformer(1),0)
			delete mol;
			delete lig;
			i++;
		}
		dfin3.close();


		File::remove(tmpfile);
		for(unsigned int i=0;i<mols.size();i++)
			delete mols[i];

RESULT



/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
