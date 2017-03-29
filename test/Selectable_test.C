// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/CONCEPT/selectable.h>
#include <BALL/CONCEPT/persistenceManager.h>
#include <BALL/CONCEPT/textPersistenceManager.h>

///////////////////////////

START_TEST(Selectable)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace std;

Selectable* s;
CHECK(Selectable() throw())
	s = new Selectable();
	TEST_NOT_EQUAL(s, 0)
RESULT

CHECK(~Selectable() throw())
	delete s;
RESULT

Selectable se, se2;

CHECK(Selectable(const Selectable& selectable, bool deep = true) throw())
	se.select();
	Selectable* s2 = new Selectable(se) ;
	TEST_EQUAL(s2->isSelected(), true)
	delete s2;
RESULT

CHECK(void clear() throw())
	se2.select();
	se2.clear();
	TEST_EQUAL(se2.isSelected(), false)
RESULT

CHECK(void set(const Selectable& selectable, bool deep = true) throw())
	se2.set(se);
	TEST_EQUAL(se2.isSelected(), true)
	se2.clear();
RESULT

CHECK(const Selectable& operator = (const Selectable& selectable) throw())
	se2 = se;
	TEST_EQUAL(se2.isSelected(), true)
	se2.clear();
RESULT

CHECK(void get(Selectable& selectable, bool deep = true) const throw())
	se.get(se2);
	TEST_EQUAL(se2.isSelected(), true)
	se2.clear();
RESULT

CHECK(void swap(Selectable& selectable) throw())
  se.swap(se2);
	TEST_EQUAL(se.isSelected(), false)
	TEST_EQUAL(se2.isSelected(), true)
  se.swap(se2);
RESULT

CHECK(void select() throw())
  se2.select();
	TEST_EQUAL(se2.isSelected(), true)
  se2.select();
	TEST_EQUAL(se2.isSelected(), true)
RESULT

CHECK(void deselect() throw())
  se2.deselect();
	TEST_EQUAL(se2.isSelected(), false)
  se2.deselect();
	TEST_EQUAL(se2.isSelected(), false)
RESULT

CHECK(bool isSelected() const throw())
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

CHECK(bool read(PersistenceManager& pm) throw())
	ifstream ifile(BALL_TEST_DATA_PATH(Selectable_test2.txt));
	pm.setIstream(ifile);
	se2.clear();
	TEST_EQUAL(se2.read(pm), true)
	TEST_EQUAL(se2.isSelected(), true)
	ifile.close();
RESULT

CHECK(void write(PersistenceManager& pm) const throw())
	NEW_TMP_FILE(filename)
	ofstream  ofile(filename.c_str(), std::ios::out);
	pm.setOstream(ofile);
	se.write(pm);
	ofile.close();	
	TEST_FILE_REGEXP(filename.c_str(), BALL_TEST_DATA_PATH(Selectable_test2.txt))
RESULT

CHECK(void dump(::std::ostream& s = std::cout, Size depth = 0) const throw())
  String filename;
	NEW_TMP_FILE(filename)
	std::ofstream outfile(filename.c_str(), std::ios::out);
	se.dump(outfile);
	outfile.close();
	TEST_FILE_REGEXP(filename.c_str(), BALL_TEST_DATA_PATH(Selectable_test3.txt))
RESULT	

CHECK(bool operator != (const Selectable& selectable) const throw())
	// ???
RESULT

CHECK(bool operator == (const Selectable& selectable) const throw())
	// ???
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
