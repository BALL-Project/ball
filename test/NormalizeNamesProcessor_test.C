// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/KERNEL/system.h>

///////////////////////////

START_TEST(FragmentDB::NormalizeNamesProcessor)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

FragmentDB frag_db("");

FragmentDB::NormalizeNamesProcessor* nnp = 0;
CHECK(NormalizeNamesProcessor())
	nnp = new FragmentDB::NormalizeNamesProcessor;
	TEST_NOT_EQUAL(nnp, 0)
RESULT

CHECK(~NormalizeNamesProcessor())
	delete nnp;
RESULT

CHECK(NormalizeNamesProcessor(FragmentDB& db))
	FragmentDB::NormalizeNamesProcessor nnp(frag_db);
	// ???
RESULT

CHECK(Processor::Result operator () (Fragment& fragment))
  // ???
RESULT

CHECK(bool finish())
  // ???
RESULT

CHECK((bool matchName(String& res_name, String& atom_name, const StringHashMap<String>* map) const))
  // ???
RESULT

CHECK(bool start())
  // ???
RESULT

CHECK(const String& getNamingStandard())
  // ???
RESULT

CHECK(void setFragmentDB(FragmentDB& db))
  // ???
RESULT

CHECK(void setNamingStandard(const String& naming_standard))
  // ???
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
