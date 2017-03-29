// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/bond.h>

///////////////////////////
#include <BALL/VIEW/MODELS/ballAndStickModel.h>
///////////////////////////

using namespace BALL;
using namespace BALL::VIEW;

START_TEST(AddBallAndStickModel)

CHECK(CSTR)
	AddBallAndStickModel();
RESULT

CHECK(AddBallAndStickModel::BALL_CREATE(AddBallAndStickModel))
  //TODO
RESULT


CHECK(AddBallAndStickModel::setBallRadius(const float radius) throw(Exception::OutOfRange))
	AddBallAndStickModel bs;
	bs.setBallRadius(0.11);
RESULT


CHECK(AddBallAndStickModel::getBallRadius() const  throw())
	AddBallAndStickModel bs;
	bs.setBallRadius(0.11);
	TEST_REAL_EQUAL(bs.getBallRadius(), 0.11)
RESULT


CHECK(AddBallAndStickModel::setStickRadius(const float radius) throw(Exception::OutOfRange))
	AddBallAndStickModel bs;
	bs.setStickRadius(0.12);
RESULT


CHECK(AddBallAndStickModel::getStickRadius() const  throw())
	AddBallAndStickModel bs;
	bs.setStickRadius(0.12);
	TEST_REAL_EQUAL(bs.getStickRadius(), 0.12)
RESULT


CHECK(AddBallAndStickModel::enableBallAndStickModel() throw())
	AddBallAndStickModel bs;
	bs.enableBallAndStickModel();
	TEST_EQUAL(bs.isBallAndStickModel(), true)
RESULT


CHECK(AddBallAndStickModel::enableStickModel() throw())
	AddBallAndStickModel bs;
	bs.enableStickModel();
	TEST_EQUAL(bs.isStickModel(), true)
RESULT


CHECK(AddBallAndStickModel::isBallAndStickModel() const  throw())
	AddBallAndStickModel bs;
	bs.enableBallAndStickModel();
	TEST_EQUAL(bs.isBallAndStickModel(), true)
RESULT


CHECK(AddBallAndStickModel::isStickModel() const  throw())
	AddBallAndStickModel bs;
	bs.enableStickModel();
	TEST_EQUAL(bs.isStickModel(), true)
RESULT

CHECK(AddBallAndStickModel::Processor::Result operator() (Composite& composite))
  //TODO
RESULT



CHECK(AddBallAndStickModel::dashedBondsEnabled() const  throw())
	AddBallAndStickModel bs;
	bs.enableDashedBonds(true);
	TEST_EQUAL(bs.dashedBondsEnabled(), true)
	bs.enableDashedBonds(false);
	TEST_EQUAL(bs.dashedBondsEnabled(), false)
RESULT


CHECK(AddBallAndStickModel::dump(std::ostream& s = std::cout, Size depth = 0) const  throw())
	AddBallAndStickModel bs;
  String filename;
	NEW_TMP_FILE(filename)
	std::ofstream outfile(filename.c_str(), std::ios::out);
	bs.dump(outfile);
	outfile.close();
	TEST_FILE_REGEXP(filename.c_str(), BALL_TEST_DATA_PATH(BallAndStickModel.txt))
RESULT

PDBFile pdb(BALL_TEST_DATA_PATH(1BNA.pdb));
System system;
pdb >> system;

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

CHECK(AddBallAndStickModel::Processor::Result operator() (Composite& composite))
	AddBallAndStickModel bs;
	bool result = bs.operator() (system);
	TEST_EQUAL(result, true)
RESULT

CHECK(AddBallAndStickModel::createGeometricObjects() throw())
	AddBallAndStickModel bs;
	bs.enableStickModel();
	bs.enableDashedBonds(false);
	system.apply(bs);
	bs.createGeometricObjects();
	TEST_EQUAL(bs.getGeometricObjects().size(), nr_atoms + nr_bonds)

	bs.clear();
	bs.enableBallAndStickModel();
	bs.enableDashedBonds(false);
	system.apply(bs);
	bs.createGeometricObjects();
	TEST_EQUAL(bs.getGeometricObjects().size(), nr_atoms + nr_bonds + nr_double_bonds)
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
