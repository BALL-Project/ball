// $Id: Selectable_test.C,v 1.1 2000/08/19 13:58:52 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/CONCEPT/selectable.h>
#include <BALL/CONCEPT/persistenceManager.h>
#include <BALL/CONCEPT/textPersistenceManager.h>

///////////////////////////

START_TEST(class_name, "$Id: Selectable_test.C,v 1.1 2000/08/19 13:58:52 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;


Selectable* s;
CHECK(Selectable::Selectable())
	s = new Selectable();
	TEST_NOT_EQUAL(s, 0)
RESULT

CHECK(Selectable::~Selectable())
	delete s;
RESULT

Selectable se, se2;

CHECK(Selectable::Selectable(const Selectable& selectable, bool deep = true))
	se.select();
	Selectable* s2 = new Selectable(se) ;
	TEST_EQUAL(s2->isSelected(), true)
RESULT

CHECK(Selectable::clear())
	se2.clear();
	TEST_EQUAL(se2.isSelected(), false)
RESULT

CHECK(Selectable::destroy())
	Selectable* s2;	
	s2->select();
	s2->destroy();
	TEST_EQUAL(s2->isSelected(), false)
RESULT

CHECK(Selectable::set(const Selectable& selectable, bool deep = true))
	se2.set(se);
	TEST_EQUAL(se2.isSelected(), true)
	se2.clear();
RESULT

CHECK(Selectable::Selectable& operator = (const Selectable& selectable))
	se2 = se;
	TEST_EQUAL(se2.isSelected(), true)
	se2.clear();
RESULT

CHECK(Selectable::get(Selectable& selectable, bool deep = true) const )
	se.get(se2);
	TEST_EQUAL(se2.isSelected(), true)
	se2.clear();
RESULT

CHECK(Selectable::swap(Selectable& selectable))
  se.swap(se2);
	TEST_EQUAL(se.isSelected(), false)
	TEST_EQUAL(se2.isSelected(), true)
  se.swap(se2);
RESULT

CHECK(Selectable::select())
  se2.select();
	TEST_EQUAL(se2.isSelected(), true)
  se2.select();
	TEST_EQUAL(se2.isSelected(), true)
RESULT

CHECK(Selectable::deselect())
  se2.deselect();
	TEST_EQUAL(se2.isSelected(), false)
  se2.deselect();
	TEST_EQUAL(se2.isSelected(), false)
RESULT

CHECK(Selectable::isSelected() const )
  se2.select();
	TEST_EQUAL(se2.isSelected(), true)
  se2.deselect();
	TEST_EQUAL(se2.isSelected(), false)
RESULT

String filename;
using std::ofstream;
using std::ios;
using namespace RTTI;
TextPersistenceManager pm;

CHECK(Selectable::friend::std::istream& operator >> (::std::istream& s, Selectable& selectable))
	std::ifstream instr("data/Selectable_test.txt");
	se2.clear();
	instr >> se2;
	instr.close();
	TEST_EQUAL(se2.isSelected(), true)
RESULT

CHECK(Selectable::friend::std::ostream& operator << (::std::ostream& s, const Selectable& selectable))
	NEW_TMP_FILE(filename)
	std::ofstream outstr(filename.c_str(), File::OUT);
	outstr << se;
	outstr.close();
	TEST_FILE(filename.c_str(), "data/Selectable_test.txt", true)
RESULT

CHECK(Selectable::read(PersistenceManager& pm))
	ifstream  ifile("data/Selectable_test2.txt");
	pm.setIstream(ifile);
	PersistentObject* ptr;
	ptr = pm.readObject();
	ifile.close();
	TEST_NOT_EQUAL(ptr, 0)
	if (ptr != 0)
	{
		TEST_EQUAL(isKindOf<Selectable>(*ptr), true)
		if (isKindOf<Selectable>(*ptr))
		{
			Selectable* v_ptr = castTo<Selectable>(*ptr);
			TEST_EQUAL(v_ptr->isSelected(), true)
		}
	}
RESULT

CHECK(Selectable::write(PersistenceManager& pm) const )
	NEW_TMP_FILE(filename)
	ofstream  ofile(filename.c_str(), File::OUT);
	pm.setOstream(ofile);
	//pm.registerClass(getStreamName<Selectable>(), Selectable::createDefault);
	se.write(pm);
	ofile.close();	
	TEST_FILE(filename.c_str(), "data/Selectable_test2.txt", true)
RESULT

CHECK(Selectable::dump(::std::ostream& s = std::cout, Size depth = 0L) const )
  String filename;
	NEW_TMP_FILE(filename)
	std::ofstream outfile(filename.c_str(), File::OUT);
	se.dump(outfile);
	outfile.close();
	TEST_FILE(filename.c_str(), "data/Selectable_test3.txt", true)
RESULT	

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
