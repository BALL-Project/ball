// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: HBondModel_test.C,v 1.1.2.1 2007/04/09 21:29:36 amoll Exp $
//
// Author:
//   Andreas Moll
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/VIEW/PRIMITIVES/disc.h>
#include <BALL/VIEW/PRIMITIVES/tube.h>
#include <BALL/KERNEL/extractors.h>

///////////////////////////
#include <BALL/VIEW/MODELS/HBondModel.h>
///////////////////////////

using namespace BALL;
using namespace BALL::VIEW;

START_TEST(HBondModelProcessor, "$Id: HBondModel_test.C,v 1.1.2.1 2007/04/09 21:29:36 amoll Exp $")

CHECK(CSTR)
	HBondModelProcessor();
RESULT

CHECK(HBondModelProcessor::BALL_CREATE(HBondModelProcessor))
  //BAUSTELLE
RESULT


PDBFile pdb(BALL_TEST_DATA_PATH(1BNA.pdb));
System system;
pdb >> system;

CHECK(HBondModelProcessor::Processor::Result operator() (Composite& composite))
	HBondModelProcessor bs;
	bool result = bs.operator() (system);
	TEST_EQUAL(result, true)
RESULT

CHECK(HBondModelProcessor::createGeometricObjects() throw())
	HBondModelProcessor bs;
	system.apply(bs);
	bs.createGeometricObjects();
	TEST_EQUAL(bs.getGeometricObjects().size(), 96 * 3 )
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
