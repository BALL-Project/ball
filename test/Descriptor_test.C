// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//
#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/QSAR/descriptor.h>
#include <BALL/FORMAT/SDFile.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/molecule.h>

///////////////////////////
START_TEST(Descriptor)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

SDFile infile(BALL_TEST_DATA_PATH(descriptors_test.sdf));
System S;
infile >> S;
infile.close();

Molecule * molecule;
Size limit = S.countMolecules();

CHECK(Descriptor)	

	Descriptor desc;
	const String name = "the fancy name";
	const String unit = "a heavy unit";

	// test default values
	TEST_EQUAL(desc.getName(), "generic descriptor")
	TEST_EQUAL(desc.getUnit(), "")
	
	desc.setName(name);
	desc.setUnit(unit);
	S.apply(desc);

	for (Size i=0;i!=limit;++i)
  {
		molecule = S.getMolecule(i);
		TEST_EQUAL(molecule->hasProperty(name), true)
		TEST_EQUAL(molecule->getProperty(name).getDouble(), 0)
		TEST_EQUAL(desc.getName(), name)
		TEST_EQUAL(desc.getUnit(), unit)
	}

		
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
