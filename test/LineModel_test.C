// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/VIEW/PRIMITIVES/twoColoredLine.h>
#include <BALL/VIEW/PRIMITIVES/point.h>
#include <BALL/KERNEL/extractors.h>

///////////////////////////
#include <BALL/VIEW/MODELS/lineModel.h>
///////////////////////////

using namespace BALL;
using namespace BALL::VIEW;

START_TEST(AddLineModel)

CHECK(CSTR)
	AddLineModel();
RESULT

CHECK(AddLineModel::BALL_CREATE(AddLineModel))
  //BAUSTELLE
RESULT


PDBFile pdb(BALL_TEST_DATA_PATH(1BNA.pdb));
System system;
pdb >> system;

CHECK(AddLineModel::Processor::Result operator() (Composite& composite))
	AddLineModel bs;
	bool result = bs.operator() (system);
	TEST_EQUAL(result, true)
RESULT

Size nr_atoms = system.countAtoms();
Size nr_bonds = system.countBonds();
Size nr_double_bonds = 0;

AtomBondIterator abit;
AtomIterator ait;
BALL_FOREACH_INTRABOND(system, ait, abit)
{
	if (abit->getType() == Bond::TYPE__HYDROGEN)
	{
		nr_bonds--;
	}
	 
	if (abit->getOrder() == Bond::ORDER__DOUBLE)
	{
		nr_double_bonds++;
	}
}

CHECK(AddLineModel::createGeometricObjects() throw())
	AddLineModel bs;
	system.apply(bs);
	bs.createGeometricObjects();
	TEST_EQUAL(bs.getGeometricObjects().size(), nr_atoms + nr_bonds)
	Point* m = dynamic_cast<Point*>(*bs.getGeometricObjects().begin());
	TEST_NOT_EQUAL(m, 0)
	PRECISION(0.0001)
	Atom* atom = *atoms(system).begin();
	TEST_EQUAL(m->getVertex(), atom->getPosition())
	TEST_EQUAL(m->getComposite(), atom)
	list<GeometricObject*>::iterator it = bs.getGeometricObjects().begin();
	bool found = false;
	for (; it != bs.getGeometricObjects().end(); it++)
	{	
		TwoColoredLine* m = dynamic_cast<TwoColoredLine*>(*it);
		if (m != 0)
		{
			const Bond* bond = dynamic_cast<const Bond*>((*it)->getComposite());
			Bond* bond2 = (Bond*) bond;
			TEST_EQUAL(m->getVertex1(), bond2->getFirstAtom()->getPosition())
			TEST_EQUAL(m->getVertex2(), bond2->getSecondAtom()->getPosition())
			found = true;
			break;
		}
	}

	TEST_EQUAL(found, true)
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
