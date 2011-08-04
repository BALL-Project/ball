// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/FORMAT/PDBInfo.h>
#include <BALL/KERNEL/system.h>
#include <BALL/FORMAT/PDBFile.h>

///////////////////////////

START_TEST(PDBInfo)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

CHECK(PDBInfo() throw())
  // ???
RESULT

CHECK(~PDBInfo() throw())
  // ???
RESULT

CHECK(PDBInfo(const PDBInfo& pdbi) throw())
  // ???
RESULT

CHECK(PDBRecords& getInvalidRecords())
  // ???
RESULT

CHECK(PDBRecords& getSkippedRecords())
  // ???
RESULT

CHECK(Position getCurrentModel() const)
  // ???
RESULT

CHECK(Position& getCurentModel())
  // ???
RESULT

CHECK(Size getNumberOfATOMRecords() const)
  // ???
RESULT

CHECK(Size getNumberOfHETATMRecords() const)
  // ???
RESULT

CHECK(Size getNumberOfModels() const)
  // ???
RESULT

CHECK(Size getNumberOfRecords() const)
  // ???
RESULT

CHECK(Size& getNumberOfATOMRecords())
  // ???
RESULT

CHECK(Size& getNumberOfHETATMRecords())
  // ???
RESULT

CHECK(Size& getNumberOfModels())
  // ???
RESULT

CHECK(PDBInfo& operator = (const PDBInfo& rhs))
  // ???
RESULT

CHECK(bool operator != (const PDBInfo& rhs) const)
  // ???
RESULT

CHECK(bool operator == (const PDBInfo& rhs) const)
  // ???
RESULT

CHECK(const PDBRecords& getInvalidRecords() const)
  // ???
RESULT

CHECK(const PDBRecords& getSkippedRecords() const)
  // ???
RESULT

CHECK(const String& getFilename() const)
  // ???
RESULT

CHECK(const String& getID() const)
  // ???
RESULT

CHECK(void clear())
  // ???
RESULT

CHECK(void setCurrentModel(Position n))
  // ???
RESULT

CHECK(void setFilename(const String& filename))
  // ???
RESULT

CHECK(void setID(const String& id))
  // ???
RESULT

CHECK(void setNumberOfATOMRecords(Size n))
  // ???
RESULT

CHECK(void setNumberOfHETATMRecords(Size n))
  // ???
RESULT

CHECK(void setNumberOfModels(Size n))
  // ???
RESULT

CHECK(void setNumberOfRecords(Size n))
  // ???
RESULT

CHECK([Extra] reading a PDB file)
	PDBFile f(BALL_TEST_DATA_PATH(PDBInfo_test.pdb));
	System S;
	f.read(S);
	TEST_EQUAL(f.info.getSkippedRecords().size(), 154)

	PDBRecords::ConstIterator it = f.info.getSkippedRecords().begin();
	for (Position i = 0; it != f.info.getSkippedRecords().end(); ++it, ++i)
	{
		STATUS(" - " << *it)
		switch (i)
		{
			case 0:
				TEST_EQUAL(*it, "COMPND    MYOGLOBIN  (FERRIC IRON - METMYOGLOBIN)                       1MBN   4")
				break;
			case 5:
				TEST_EQUAL(*it, "REVDAT  18   12-MAY-83 1MBNQ   3       REMARK SEQRES ATOM               1MBNR  2")
				break;
			case 23:
				TEST_EQUAL(*it, "JRNL        AUTH   H.C.WATSON                                           1MBNG  2")
		}
	}
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
