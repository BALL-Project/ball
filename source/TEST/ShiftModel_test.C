// $Id: ShiftModel_test.C,v 1.2 2000/09/18 21:38:22 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/NMR/shiftModel.h>

///////////////////////////

START_TEST(ShiftModel, "$Id: ShiftModel_test.C,v 1.2 2000/09/18 21:38:22 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;


ShiftModel* sm = 0;
CHECK(ShiftModel::ShiftModel() throw())
  sm = new ShiftModel;
	TEST_NOT_EQUAL(sm, 0)
RESULT


CHECK(ShiftModel::~ShiftModel() throw())
  delete sm;
RESULT


CHECK(ShiftModel::isValid() const  throw())
  ShiftModel sm;
	TEST_EQUAL(sm.isValid(), false)
RESULT


CHECK(ShiftModel::isRegistered(const String& name) const  throw())
  ShiftModel sm;
	TEST_EQUAL(sm.isRegistered("UNNAMED MODULE X"), false)
	TEST_EQUAL(sm.isRegistered(""), false)
	TEST_EQUAL(sm.isRegistered("JohnsonBovey"), true)
	TEST_EQUAL(sm.isRegistered("HaighMallion"), true)
RESULT


CHECK(ShiftModel::getParameters() throw())
  ShiftModel sm;
	TEST_NOT_EQUAL(&sm.getParameters(), 0)
	TEST_EQUAL(sm.getParameters().isValid(), false)
RESULT


CHECK(ShiftModel::getModuleList() throw())
	ShiftModel sm;
	TEST_EQUAL(sm.getModuleList().size(), 0)
RESULT


CHECK(ShiftModel::setFilename(const String& filename) throw(Exception::FileNotFound))
	ShiftModel sm;
	TEST_EXCEPTION(Exception::FileNotFound, sm.setFilename("XXXXXX"))
	TEST_EQUAL(sm.isValid(), false)
	TEST_EQUAL(sm.getParameters().isValid(), false)
	sm.setFilename("data/ShiftModel_test.ini");
	TEST_EQUAL(sm.isValid(), true)
	TEST_EQUAL(sm.getParameters().isValid(), true)
RESULT


CHECK(ShiftModel::getFilename() const  throw())
	ShiftModel sm;
	TEST_EXCEPTION(Exception::FileNotFound, sm.setFilename("XXXXXX"))
	TEST_EQUAL(sm.isValid(), false)
	TEST_EQUAL(sm.getFilename(), "XXXXXX")
	sm.setFilename("data/ShiftModel_test.ini");
	TEST_EQUAL(sm.isValid(), true)
	TEST_EQUAL(sm.getFilename(), "data/ShiftModel_test.ini")
RESULT


CHECK(ShiftModel::registerModule(const String& name, CreateMethod method) throw(Exception::NullPointer))
	ShiftModel sm;
	using namespace RTTI;
	ShiftModel::CreateMethod m = getNew<ShiftModule>;
	TEST_EQUAL(sm.isRegistered("TEST"), false)
	sm.registerModule("TEST", m);
	TEST_EQUAL(sm.isRegistered("TEST"), true)
	TEST_EQUAL(sm.isRegistered("TEST2"), false)
	TEST_EXCEPTION(Exception::NullPointer, sm.registerModule("TEST2", 0))
	TEST_EQUAL(sm.isRegistered("TEST2"), false)
RESULT


CHECK(ShiftModel::unregisterModule(const String& name) throw())
	ShiftModel sm;
	using namespace RTTI;
	ShiftModel::CreateMethod m = getNew<ShiftModule>;
	TEST_EQUAL(sm.isRegistered("TEST"), false)
	sm.registerModule("TEST", m);
	TEST_EQUAL(sm.isRegistered("TEST"), true)
	sm.unregisterModule("TEST");
	TEST_EQUAL(sm.isRegistered("TEST"), false)
	sm.unregisterModule("TEST");
	TEST_EQUAL(sm.isRegistered("TEST"), false)	
RESULT


CHECK(ShiftModel::ShiftModel(const String& filename) throw())
  ShiftModel sm("data/ShiftModel_test.ini");
	TEST_EQUAL(sm.isValid(), true)
	ShiftModel::ModuleList mod_list = sm.getModuleList();
	TEST_EQUAL(mod_list.size(), 2)
	if (mod_list.size() > 0)
	{
		TEST_EQUAL((*mod_list.begin())->getName(), "JB_ring_current")
	}
RESULT


CHECK(ShiftModel::clear() throw())
  ShiftModel sm("data/ShiftModel_test.ini");
	sm.clear();
	TEST_EQUAL(sm.isValid(), false)
	TEST_EQUAL(sm.getFilename(), "")
	TEST_EQUAL(sm.getModuleList().size(), 0)
RESULT


CHECK(ShiftModel::ShiftModel(const ShiftModel& model) throw())
  //BAUSTELLE
RESULT


CHECK(ShiftModel::ShiftModel& operator = (const ShiftModel& model) throw())
  //BAUSTELLE
RESULT


CHECK(ShiftModel::ShiftModel& operator = (const String& filename) throw())
  //BAUSTELLE
RESULT


											

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
