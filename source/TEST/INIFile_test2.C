// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////
#include <BALL/FORMAT/INIFile.h>
#include "ItemCollector.h"
///////////////////////////
#include <iostream>
using namespace std;

template<typename DataType>
class MyItemCollector	
: public ItemCollector<DataType>
{
	public:

	Processor::Result operator () (DataType& item)
	{	//cout << *item << endl;
		ItemCollector<DataType>::list_.push_back(&item);
		list2.push_back(*item);
		return Processor::CONTINUE;
	}

	list<String> list2;
};


START_TEST(INIFile)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

INIFile ini;

CHECK(bool deleteSection(const String& section))
	INIFile ini(BALL_TEST_DATA_PATH(INIFile_test.ini));
	ini.read();
	
	TEST_EQUAL(ini.deleteSection(ini.HEADER), true)
  TEST_EQUAL(ini.getNumberOfLines(), 10)

	TEST_EQUAL(ini.deleteSection("Section1"), true)
  TEST_EQUAL(ini.getNumberOfLines(), 9)
	TEST_EQUAL(ini.hasSection("Section1"), false)

	TEST_EQUAL(ini.deleteSection("Section2"), true)
  TEST_EQUAL(ini.getNumberOfLines(), 6)
	TEST_EQUAL(ini.hasSection("Section2"), false)

	INIFile emptyFile;
	TEST_EQUAL(emptyFile.deleteSection(emptyFile.HEADER), true)
	TEST_EQUAL(emptyFile.hasSection(emptyFile.HEADER), true)
	TEST_EQUAL(emptyFile.deleteSection("asd"), false)
RESULT

CHECK(bool appendSection(const String& section))
	INIFile ini(BALL_TEST_DATA_PATH(INIFile_test.ini));
	ini.read();
	TEST_EQUAL(ini.hasSection(ini.HEADER), true)
	CAPTURE_OUTPUT_LEVEL(2000)
		TEST_EQUAL(ini.appendSection(ini.HEADER), false)
		const char* output = "INIFile::appendSection: " BALL_TEST_DATA_PATH(INIFile_test.ini) " , while adding section: '#HEADER!' already exists.\n";
	COMPARE_OUTPUT(output)
  TEST_EQUAL(ini.getNumberOfLines(), 10)

  CAPTURE_OUTPUT_LEVEL(2000)
		TEST_EQUAL(ini.appendSection("Section1"), false)
	COMPARE_OUTPUT("INIFile::appendSection: " BALL_TEST_DATA_PATH(INIFile_test.ini) " , while adding section: 'Section1' already exists.\n")
  TEST_EQUAL(ini.getNumberOfLines(), 10)
	TEST_EQUAL(ini.hasSection("Section1"), true)

	TEST_EQUAL(ini.appendSection("Section5"), true)
  TEST_EQUAL(ini.getNumberOfLines(), 11)
	TEST_EQUAL(ini.hasSection("Section5"), true)
  TEST_EQUAL(*ini.getLine(10), "[Section5]")

	INIFile emptyFile;
  CAPTURE_OUTPUT_LEVEL(2000)
		TEST_EQUAL(emptyFile.appendSection(emptyFile.HEADER), false)
  COMPARE_OUTPUT("INIFile::appendSection:  , while adding section: '#HEADER!' already exists.\n")
	TEST_EQUAL(emptyFile.hasSection(emptyFile.HEADER), true)
	TEST_EQUAL(emptyFile.appendSection("asd"), true)
RESULT

CHECK(bool apply(UnaryProcessor<LineIterator>& processor))
	MyItemCollector<INIFile::LineIterator> myproc;
	INIFile ini(BALL_TEST_DATA_PATH(INIFile_test.ini));
	ini.read();

	TEST_EQUAL(ini.apply(myproc), true)
	TEST_EQUAL(myproc.getSize(), 10)
	TEST_EQUAL(myproc.list2.size(), 10)
	list<String>::iterator it(myproc.list2.begin());
	TEST_EQUAL(*it, "[Section1]")
	it = myproc.list2.end();
	it--;
	TEST_EQUAL(*it, "[Section4]")

	INIFile emptyFile;
	TEST_EQUAL(emptyFile.apply(myproc), true)
RESULT

CHECK(bool operator == (const INIFile& inifile) const)
	INIFile ini(BALL_TEST_DATA_PATH(INIFile_test.ini));
	ini.read();
	INIFile ini2(BALL_TEST_DATA_PATH(INIFile_test.ini));
	ini2.read();
	TEST_EQUAL(ini == ini2, true)
	ini.deleteLine(ini.getLine(2));
	TEST_EQUAL(ini == ini2, false)

	INIFile emptyFile;
	TEST_EQUAL(emptyFile == emptyFile, true)
RESULT

CHECK(bool isValid(const LineIterator& it) const)
	INIFile ini(BALL_TEST_DATA_PATH(INIFile_test.ini));
	ini.read();

	INIFile::LineIterator it;
	TEST_EQUAL(ini.isValid(it), false)

	it = ini.getLine(9);
	TEST_EQUAL(ini.isValid(it), true)
	it = ini.getLine(10);
	TEST_EQUAL(ini.isValid(it), false)
RESULT

CHECK(bool isValid(const SectionIterator& it) const)
	INIFile ini(BALL_TEST_DATA_PATH(INIFile_test.ini));
	ini.read();
	INIFile::SectionIterator it;
	TEST_EQUAL(ini.isValid(it), true)

	it = ini.getSection(0);
	TEST_EQUAL(ini.isValid(it), true)
	it = ini.getSection("asd");
	TEST_EQUAL(ini.isValid(it), false)
RESULT

CHECK(bool duplicateKeyCheckEnabled() const)
	TEST_EQUAL(ini.duplicateKeyCheckEnabled(), false)
RESULT

CHECK(void setDuplicateKeyCheck(bool mode))
	ini.setDuplicateKeyCheck(true);
	TEST_EQUAL(ini.duplicateKeyCheckEnabled(), true)
	ini.setDuplicateKeyCheck(false);
	TEST_EQUAL(ini.duplicateKeyCheckEnabled(), false)
RESULT

// ===================== Section =================================
CHECK(bool operator == (const Section& section) const)
	INIFile::Section s1, s2;
	TEST_EQUAL(s1 == s2, true)
RESULT

CHECK(friend friend const String& getName() const)
	INIFile::Section s1;
	TEST_EQUAL(s1.getName(), "")
RESULT

// ================ IteratorTraits_ ==============================

INIFile::IteratorTraits_* itt = 0;
CHECK(IteratorTraits_())
	itt = new INIFile::IteratorTraits_;
RESULT

CHECK(~IteratorTraits_())
	delete itt;
RESULT

INIFile inix(BALL_TEST_DATA_PATH(INIFile_test.ini));
inix.read();
INIFile::LineIterator it(ini.getLine(0));
INIFile::LineIterator unbound;

CHECK(BALL_CREATE(IteratorTraits_))
	INIFile::LineIterator* it2 = 0;
	it2 = (INIFile::LineIterator*) it.create();
	TEST_NOT_EQUAL(it2, 0)
	TEST_EQUAL(+*it2, true)
	TEST_EQUAL(**it2, "[Section1]")
	delete it2;
	it2 = (INIFile::LineIterator*)unbound.create();
	TEST_NOT_EQUAL(it2, 0)
	delete it2;
RESULT

CHECK(IteratorTraits_& getSectionNextLine())
	INIFile::LineIterator it2(it);
	it2.getSectionNextLine();
	TEST_EQUAL(+it2, false)
	it2 = ini.getLine(7);
	TEST_EQUAL(*it2, ";comments over comments");
	TEST_EQUAL(+it2.getSectionNextLine(), true)
	TEST_EQUAL(*it2, "test3 = c")
	TEST_EQUAL(+it2.getSectionNextLine(), false)
	unbound.getSectionNextLine();
RESULT

it = ini.getLine(1);
CHECK(IteratorTraits_(const IteratorTraits_& traits))
	INIFile::LineIterator it2(it);
	TEST_EQUAL(+it2, true)
	TEST_EQUAL(*it2, "[Section2]")

	INIFile::LineIterator it4(unbound);
	TEST_EQUAL(+it4, false)
RESULT

CHECK(IteratorTraits_& operator ++ ())
	TEST_EQUAL(*it, "[Section2]")
	++it;
	TEST_EQUAL(+it, true)
	TEST_EQUAL(*it, "# this is a comment =")
	it = ini.getLine(9);
	TEST_EQUAL(+it, true)
	++it;
	TEST_EQUAL(+it, false)
	++unbound;
RESULT

CHECK(IteratorTraits_& operator -- ())
	it = ini.getLine(0);
	--it;
	TEST_EQUAL(+it, false)
	it = ini.getLine(1);
	TEST_EQUAL(*it, "[Section2]")
	--it;
	TEST_EQUAL(*it, "[Section1]")
	--unbound;
RESULT

CHECK(List<String> ::iterator getPosition())
	it = ini.getLine(0);
	TEST_EQUAL(*(it.getPosition()), "[Section1]")
	unbound.getPosition();
RESULT

CHECK(SectionIterator getSection())
	TEST_EQUAL(it.getSection()->getName(), "Section1")
	unbound.getSection();
RESULT

CHECK(bool operator != (const IteratorTraits_& traits) const)
	it = ini.getLine(0);
	INIFile::LineIterator it3;
	TEST_EQUAL(it != it3, true)
	it3 = ini.getLine(1);
	TEST_EQUAL(it != it3, true)
	it3 = ini.getLine(0);
	TEST_EQUAL(it != it3, false)
RESULT

CHECK(bool operator == (const IteratorTraits_& traits) const)
  it = ini.getLine(0);
	INIFile::LineIterator it3;
	TEST_EQUAL(it == it3, false)
	it3 = ini.getLine(1);
	TEST_EQUAL(it == it3, false)
	it3 = ini.getLine(0);
	TEST_EQUAL(it == it3, true)
RESULT

INIFile::LineIterator it3;
CHECK(bool operator + () const)
	TEST_EQUAL(+it, true)
	TEST_EQUAL(+it3, false)
RESULT

CHECK(const IteratorTraits_& operator = (const IteratorTraits_ &traits))
	it3 = ini.getLine(1);
	TEST_EQUAL(*it3, "[Section2]")
	INIFile::LineIterator it4;
	it4 = unbound;
RESULT

CHECK(const String& operator * () const)
	TEST_EQUAL(*it3, "[Section2]")
RESULT

CHECK(bool isSectionEnd() const)
	TEST_EQUAL(it3.isSectionEnd(), false)
	it3 = ini.getLine(3);
	TEST_EQUAL(it3.isSectionEnd(), false)
	it3 = ini.getLine(10);
	TEST_EQUAL(it3.isSectionEnd(), false)
	++it3;
	TEST_EQUAL(it3.isSectionEnd(), false)
	TEST_EQUAL(unbound.isSectionEnd(), false)
RESULT

CHECK(bool isSectionFirstLine() const)
	it = ini.getLine(0);
	TEST_EQUAL(it.isSectionFirstLine(), true)
	it = ini.getLine(2);
	TEST_EQUAL(it.isSectionFirstLine(), false)
	TEST_EQUAL(unbound.isSectionFirstLine(), false)
RESULT

CHECK(bool isSectionLastLine() const)
	it = ini.getLine(0);
	TEST_EQUAL(it.isSectionLastLine(), true)
	it = ini.getLine(3);
	TEST_EQUAL(it.isSectionLastLine(), true)
	TEST_EQUAL(+it, true)
	TEST_EQUAL(unbound.isSectionLastLine(), false)
RESULT

CHECK(void toEnd())
	it.toEnd();
	TEST_EQUAL(+it, false)
	--it;
	TEST_EQUAL(+it, true)
	TEST_EQUAL(it.getSection()->getName(), "Section4")
	TEST_EQUAL(*it, "[Section4]")
	unbound.toEnd();
RESULT

CHECK(void toFirstLine())
	it = ini.getLine(3);
	TEST_EQUAL(+it, true)
	it.toFirstLine();
	TEST_EQUAL(it.getSection()->getName(), "Section1")
	TEST_EQUAL(+it, true)
	TEST_EQUAL(*it, "[Section1]")
	unbound.toFirstLine();
	TEST_EQUAL(+unbound, false)
RESULT

CHECK(void toLastLine())
	it = ini.getLine(3);
	it.toLastLine();
	TEST_EQUAL(*it, "[Section4]")
	it.toLastLine();
	TEST_EQUAL(+it, true)
	TEST_EQUAL(*it, "[Section4]")
	unbound.toLastLine();
	TEST_EQUAL(+unbound, false)
RESULT

CHECK(void toSectionEnd())
	it = ini.getLine(3);
	it.toSectionEnd();
	TEST_EQUAL(it.isSectionEnd(), true)
	TEST_EQUAL(it.getSection()->getName(), "Section2")
	--it;
	TEST_EQUAL(+it, true)
	TEST_EQUAL(*it, "! even more comment")
	it = ini.getLine(9);
	it.toSectionEnd();
	--it;
	TEST_EQUAL(*it, "[Section4]")
	unbound.toSectionEnd();
	TEST_EQUAL(+unbound, false)
RESULT

CHECK(void toSectionFirstLine())
	it = ini.getLine(3);
	it.toSectionFirstLine();
	TEST_EQUAL(+it, true)
	TEST_EQUAL(*it, "[Section2]")
	it = ini.getLine(0);
	it.toSectionFirstLine();
	TEST_EQUAL(+it, true)
	TEST_EQUAL(*it, "[Section1]")
	unbound.toSectionFirstLine();
	TEST_EQUAL(+unbound, false)
RESULT

CHECK(void toSectionLastLine())
  it = ini.getLine(3);
	it.toSectionLastLine();
	TEST_EQUAL(+it, true)
	TEST_EQUAL(*it, "! even more comment")
	it = ini.getLine(0);
	it.toSectionLastLine();
	TEST_EQUAL(+it, true)
	TEST_EQUAL(*it, "[Section1]")
	unbound.toSectionLastLine();
	TEST_EQUAL(+unbound, false)
RESULT

CHECK([EXTRA] large_enry)
	INIFile ini(BALL_TEST_DATA_PATH(INIFile_test3.ini));
	ini.read();
	TEST_EQUAL(ini.hasEntry("Section1", "PythonHotkeys"), true)
	String res = ini.getValue("Section1", "PythonHotkeys");
	TEST_EQUAL(res.size(), 9999)
RESULT



/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
