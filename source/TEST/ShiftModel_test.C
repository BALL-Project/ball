// $Id: ShiftModel_test.C,v 1.6 2000/09/25 19:08:16 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/NMR/shiftModel.h>
#include <BALL/NMR/haighMallionShiftProcessor.h>
#include <BALL/NMR/randomCoilShiftProcessor.h>
#include <BALL/NMR/EFShiftProcessor.h>
#include <BALL/NMR/anisotropyShiftProcessor.h>
#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/FORMAT/PDBFile.h>

///////////////////////////

START_TEST(ShiftModel, "$Id: ShiftModel_test.C,v 1.6 2000/09/25 19:08:16 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace std;

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
	TEST_EQUAL(sm.isRegistered("Anisotropy"), true)
	TEST_EQUAL(sm.isRegistered("RandomCoil"), true)
	TEST_EQUAL(sm.isRegistered("ElectricField"), true)
RESULT

CHECK(ShiftModel::getParameters() throw())
  ShiftModel sm;
	TEST_NOT_EQUAL(&sm.getParameters(), 0)
	TEST_EQUAL(sm.getParameters().isValid(), false)
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

CHECK(ShiftModel::getModuleList() throw())
	ShiftModel sm;
	TEST_EQUAL(sm.getModuleList().size(), 0)
	
	sm.setFilename("data/ShiftModel_test.ini");
	TEST_EQUAL(sm.getModuleList().size(), 4)

	if (sm.getModuleList().size() == 4)
	{
		List<ShiftModule*>::const_iterator list_it = sm.getModuleList().begin();
		for (; list_it != sm.getModuleList().end(); ++list_it)
		{
			TEST_EQUAL((*list_it)->isValid(), true)
		}
	}
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
	TEST_EQUAL(mod_list.size(), 4)
	if (mod_list.size() > 0)
	{
		TEST_EQUAL((*mod_list.begin())->getName(), "ring_current")
	}
RESULT

CHECK(ShiftModel::clear() throw())
  ShiftModel sm("data/ShiftModel_test.ini");
	sm.clear();
	TEST_EQUAL(sm.isValid(), false)
	TEST_EQUAL(sm.getFilename(), "")
	TEST_EQUAL(sm.getModuleList().size(), 0)
RESULT

const ShiftModel smx("data/ShiftModel_test.ini");

CHECK(ShiftModel::ShiftModel(const ShiftModel& model) throw())
	ShiftModel sm(smx);
	TEST_EQUAL(sm.isValid(), true)
	ShiftModel::ModuleList mod_list = sm.getModuleList();
	TEST_EQUAL(mod_list.size(), 4)
	if (mod_list.size() > 0)
	{
		TEST_EQUAL((*mod_list.begin())->getName(), "ring_current")
	}
RESULT

CHECK(ShiftModel::ShiftModel& operator = (const ShiftModel& model) throw())
	ShiftModel sm = smx;
	TEST_EQUAL(sm.isValid(), true)
	ShiftModel::ModuleList mod_list = sm.getModuleList();
	TEST_EQUAL(mod_list.size(), 4)
	if (mod_list.size() > 0)
	{
		TEST_EQUAL((*mod_list.begin())->getName(), "ring_current")
	}
RESULT

CHECK(ShiftModel::ShiftModel& operator = (const String& filename) throw())
	ShiftModel sm = String("data/ShiftModel_test.ini");
	TEST_EQUAL(sm.isValid(), true)
	ShiftModel::ModuleList mod_list = sm.getModuleList();
	TEST_EQUAL(mod_list.size(), 4)
	if (mod_list.size() > 0)
	{
		TEST_EQUAL((*mod_list.begin())->getName(), "ring_current")
	}
RESULT		

PRECISION(0.0001)
CHECK(chemical shift/BPTI)
	PDBFile f("data/ShiftModel_test.pdb");
	System S;
	f >> S;
	TEST_EQUAL(S.countAtoms(), 892)

	FragmentDB frag_db;
	bool result = S.apply(frag_db.build_bonds);
	TEST_EQUAL(result, true)
	
	ShiftModel sm("data/ShiftModel_test.ini");
	TEST_EQUAL(sm.isValid(), true)
	S.apply(sm);

  StringHashMap<float> shifts;
  ifstream infile("data/ShiftModel_test.dat");
  String name;
  float shift;
  while (infile.good())
  {
    infile >> name >> shift;
    if (name != "")
    {
      shifts.insert(name, shift);
		}
	}
  TEST_EQUAL(shifts.size(), 438)
 
	Size i = 0;
	AtomIterator atom = S.beginAtom();
	for (; +atom; ++atom)
	{
		if (atom->hasProperty(ShiftModule::PROPERTY__SHIFT))
		{
			float shift = atom->getProperty(ShiftModule::PROPERTY__SHIFT).getFloat();
			String name;
			Residue* residue = dynamic_cast<Residue*>(atom->getFragment());
			if (residue != 0)
			{
				name = residue->getID();
			}
			name += ":" + atom->getFullName();
			
			STATUS(name << ": " << shift)
			TEST_EQUAL(shifts.has(name), true)
			TEST_REAL_EQUAL(shift, shifts[name])
			i++;
		}
	}
  TEST_EQUAL(i, 438)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
