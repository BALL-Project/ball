#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/FORMAT/molDB.h>
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

#include <iostream>
///////////////////////////

using namespace BALL;
using namespace std;

bool compareMolecules(Molecule& mol1, Molecule& mol2)
{
	if(mol1.countAtoms()!=mol2.countAtoms())
		return false;
	if(mol1.countBonds()!=mol2.countBonds())
		return false;

	AtomIterator ai;
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


START_TEST(MolDB)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////






CHECK(getTopology buildTopology)
        SDFile f(BALL_TEST_DATA_PATH(QSAR_test.sdf));
		Molecule* mol;
		while( (mol = f.read()) )
		{
			QByteArray topology;
			QBuffer topobuffer(&topology);
			topobuffer.open(QIODevice::WriteOnly);
			QDataStream topo_ds(&topobuffer);
			MolDB::getTopology(*mol, topo_ds);
			Molecule rec_mol;
			MolDB::buildTopology(rec_mol,topology);
			TEST_EQUAL(compareMolecules(*mol,rec_mol),true)
			delete mol;
		}
RESULT

CHECK(getCharges buildCharges)
        SDFile f(BALL_TEST_DATA_PATH(QSAR_test.sdf));
		Molecule* mol = f.read();

		// set charges on atoms
		float i = 0.0;
		AtomIterator ai;
		BALL_FOREACH_ATOM(*mol,ai)
		{
			ai->setCharge(i);
			i+=1.0;
		}
		QByteArray charges;
		QBuffer chargebuffer(&charges);
		chargebuffer.open(QIODevice::WriteOnly);
		QDataStream charge_ds(&chargebuffer);
		MolDB::getCharges(*mol, charge_ds);

		Molecule rec_mol(*mol);
		ClearChargeProcessor clear_charge;
		rec_mol.apply(clear_charge);

		MolDB::buildCharges(rec_mol,charges);

		// check reconstructed charges on atom
		i = 0.0;
		BALL_FOREACH_ATOM(rec_mol,ai)
		{
			TEST_REAL_EQUAL(ai->getCharge(),i);
			i+=1.0;
		}
		delete mol;
RESULT

CHECK(getCoordinates buildCoordinates)
        SDFile f(BALL_TEST_DATA_PATH(QSAR_test.sdf));
		Molecule* mol = f.read();

		// set coords on atoms
		float i = 0.0;
		AtomIterator ai;
		BALL_FOREACH_ATOM(*mol,ai)
		{
			Vector3 pos;
			pos.x=i;
			i+=1.0;
			pos.y=i;
			i+=1.0;
			pos.z=i;
			i+=1.0;
			ai->setPosition(pos);
		}

		QByteArray coords;
		QBuffer coordsbuffer(&coords);
		coordsbuffer.open(QIODevice::WriteOnly);
		QDataStream coords_ds(&coordsbuffer);
		MolDB::getCoordinates(*mol, coords_ds);

		Molecule rec_mol(*mol);

		// clear coords on rec_mol
		BALL_FOREACH_ATOM(*mol,ai)
		{
			Vector3 pos;
			pos.x=0.0;
			pos.y=0.0;
			pos.z=0.0;
			ai->setPosition(pos);
		}

		MolDB::buildCoordinates(rec_mol,coords);

		// check reconstructed coords on atom
		i = 0.0;
		BALL_FOREACH_ATOM(rec_mol,ai)
		{
			Vector3 pos = ai->getPosition();
			TEST_REAL_EQUAL(pos.x,i);
			i+=1.0;
			TEST_REAL_EQUAL(pos.y,i);
			i+=1.0;
			TEST_REAL_EQUAL(pos.z,i);
			i+=1.0;
		}
		delete mol;
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
