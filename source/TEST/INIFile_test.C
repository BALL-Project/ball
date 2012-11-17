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

INIFile* ini1 = 0;
CHECK(INIFile())
	ini1 = new INIFile;
	TEST_NOT_EQUAL(ini1, 0)
RESULT


CHECK(const String& getFilename() const)
	TEST_EQUAL(ini1->getFilename(), "")
RESULT

CHECK(bool isValid() const)
	TEST_EQUAL(ini1->isValid(), false)
RESULT

CHECK(~INIFile())
  delete ini1;
RESULT

String filename;
NEW_TMP_FILE(filename)
CHECK(INIFile(const String& filename))
	INIFile ini(filename);
	TEST_EQUAL(ini.getFilename(), filename)
RESULT

CHECK(void setFilename(const String& filename))
	INIFile ini;
	ini.setFilename("TEST");
	TEST_EQUAL(ini.getFilename(), "TEST")
	ini.setFilename("");
	TEST_EQUAL(ini.getFilename(), "")
RESULT

CHECK(void clear())
	INIFile ini;
	ini.setFilename("TEST");
	ini.clear();
	TEST_EQUAL(ini.getFilename(), "TEST")
	TEST_EQUAL(ini.getNumberOfLines(), 0)
	TEST_EQUAL(ini.getNumberOfSections(), 0)
RESULT

INIFile ini;

CHECK(bool read())
	ini.setFilename(BALL_TEST_DATA_PATH(INIFile_test.ini));
	TEST_EQUAL(ini.read(), true)
	
	INIFile ini2(BALL_TEST_DATA_PATH(empty_file.txt));
	TEST_EQUAL(ini2.read(), true)
RESULT

CHECK(LineIterator getLine(Size line_number))
	for (int i = 0; i < 10; i++)
	{
	  TEST_EQUAL(+ini.getLine(i), true)
	}

  TEST_EQUAL(*(ini.getLine(0)), "[Section1]")
  TEST_EQUAL(*(ini.getLine(1)), "[Section2]")
  TEST_EQUAL(*(ini.getLine(2)), "# this is a comment =")
  TEST_EQUAL(*(ini.getLine(3)), "! even more comment")
  TEST_EQUAL(*(ini.getLine(4)), "[Section3]")
  TEST_EQUAL(*(ini.getLine(5)), "test1 = a")
  TEST_EQUAL(*(ini.getLine(6)), "test2=b")
  TEST_EQUAL(*(ini.getLine(7)), ";comments over comments")
  TEST_EQUAL(*(ini.getLine(8)), "test3 = c")
  TEST_EQUAL(*(ini.getLine(9)), "[Section4]")
  TEST_EQUAL(+ini.getLine(10), false)

	INIFile emptyFile;
  TEST_EQUAL(+emptyFile.getLine(0), false)
RESULT

CHECK(bool setLine(LineIterator line_it, const String& line))
	INIFile ini(BALL_TEST_DATA_PATH(INIFile_test.ini));
	ini.read();
	INIFile::LineIterator it(ini.getLine(6));
  TEST_EQUAL(ini.setLine(it, "test2=b"), true)
  TEST_EQUAL(ini.setLine(it, "test2=cc"), true)
  TEST_EQUAL(ini.getValue("Section3", "test2"), "cc")
  TEST_EQUAL(ini.setLine(it, "testx=ff"), true)
  TEST_EQUAL(ini.getValue("Section3", "test2"), ini.UNDEFINED)
  TEST_EQUAL(ini.getValue("Section3", "testx"), "ff")

	it = ini.getLine(4);
  TEST_EQUAL(ini.setLine(it, "test"), false)

	INIFile emptyFile;
	it = emptyFile.getLine(0);
  TEST_EQUAL(emptyFile.setLine(it, "test"), false)
RESULT

CHECK(bool deleteLine(LineIterator line_it))
	INIFile ini(BALL_TEST_DATA_PATH(INIFile_test.ini));
	ini.read();

	INIFile::LineIterator it;
	TEST_EQUAL(ini.deleteLine(it), false)
	
	it = ini.getSectionFirstLine("Section3");
  ++it;	++it;
	TEST_EQUAL(+it, true)
	TEST_EQUAL(*it, "test2=b")
	TEST_EQUAL(ini.deleteLine(it), true)

	TEST_EQUAL(ini.hasEntry("Section3", "test2"), false)
  TEST_EQUAL(ini.getValue("Section3", "test2"), ini.UNDEFINED)
  TEST_EQUAL(ini.getSectionLength("Section3"), 4)

	it = ini.getLine(4);
	TEST_EQUAL(ini.deleteLine(it), false)

	INIFile emptyFile;
	it = emptyFile.getLine(0);
  TEST_EQUAL(emptyFile.deleteLine(it), false)
RESULT

CHECK(bool insertLine(LineIterator line_it, const String& line))
	INIFile ini(BALL_TEST_DATA_PATH(INIFile_test.ini));
	ini.read();
	INIFile::LineIterator it(ini.getLine(5));
  TEST_EQUAL(ini.insertLine(it, "insertTest = QQQ"), true)
  TEST_EQUAL(*ini.getLine(6), "insertTest = QQQ")
  TEST_EQUAL(ini.getValue("Section3", "insertTest"), "QQQ")

	it = ini.getLine(10);
	TEST_EQUAL(+it, true)
  TEST_EQUAL(ini.insertLine(it, "insertTest = ZZZ"), true)
  TEST_EQUAL(*ini.getLine(11), "insertTest = ZZZ")
  TEST_EQUAL(ini.getValue("Section4", "insertTest"), "ZZZ")

	it = ini.getLine(0);
	INIFile::LineIterator it2(it);
	it2.toSectionLastLine();
	TEST_EQUAL(*it2, "[Section1]")
	TEST_EQUAL(it.isSectionLastLine(), true)
  TEST_EQUAL(*it, "[Section1]")
  TEST_EQUAL(ini.insertLine(it, "insertTest2 = XXX"), true)
  TEST_EQUAL(*ini.getLine(1), "insertTest2 = XXX")
  TEST_EQUAL(ini.getValue("Section1", "insertTest2"), "XXX")

	it = ini.getLine(0);
  TEST_EQUAL(ini.insertLine(it, "insertTest3"), true)
  TEST_EQUAL(*ini.getLine(0), "[Section1]")
  TEST_EQUAL(*ini.getLine(1), "insertTest3")
  TEST_EQUAL(ini.getValue("Section1", "insertTest3"), ini.UNDEFINED)

	INIFile emptyFile;
	it = emptyFile.getLine(0);
	CAPTURE_OUTPUT_LEVEL(2000)
		TEST_EQUAL(emptyFile.insertLine(it, "test"), false)
	COMPARE_OUTPUT("In INIFile  , error while inserting line: test . Illegal iterator!\n")
RESULT

CHECK(bool appendLine(const String& section_name, const String& line))
	INIFile ini(BALL_TEST_DATA_PATH(INIFile_test.ini));
	ini.read();
	
	CAPTURE_OUTPUT_LEVEL(2000)
		TEST_EQUAL(ini.appendLine("Section9", "GAU"), false)
	COMPARE_OUTPUT("In INIFile " BALL_TEST_DATA_PATH(INIFile_test.ini) " , error while appending line: GAU . Illegal section-name: Section9\n")
	CAPTURE_OUTPUT_LEVEL(2000)
		TEST_EQUAL(ini.appendLine("Section2", "[AU"), false)	
	COMPARE_OUTPUT("In INIFile " BALL_TEST_DATA_PATH(INIFile_test.ini) " , error while appending line: [AU . Illegal section-name: Section2\n")
	TEST_EQUAL(ini.appendLine("Section3", "test1 = 123"), true)	
	TEST_EQUAL(*ini.getLine(9), "test1 = 123")
	TEST_EQUAL(ini.getNumberOfLines(), 11)
  TEST_EQUAL(ini.getValue("Section3", "test1"), "123") 

	TEST_EQUAL(ini.appendLine("Section3", "insert1=ok"), true)
	TEST_EQUAL(ini.getNumberOfLines(), 12)
	TEST_EQUAL(ini.getSectionLength("Section3"), 7)
	TEST_EQUAL(ini.hasEntry("Section3", "insert1"), true)
	TEST_EQUAL(ini.getValue("Section3", "insert1"), "ok")

	INIFile emptyFile;
	TEST_EQUAL(emptyFile.appendLine(emptyFile.HEADER, "insert"), true)
	TEST_EQUAL(+emptyFile.getLine(0), true)
	TEST_EQUAL(*emptyFile.getLine(0), "insert")
RESULT

CHECK(Size getNumberOfLines() const)
	INIFile ini2(BALL_TEST_DATA_PATH(amber91.ini));
	TEST_EQUAL(ini2.read(), true)
	TEST_EQUAL(ini2.getNumberOfLines(), 1379)
  TEST_EQUAL(ini.getNumberOfLines(), 10)

	INIFile emptyFile;
  TEST_EQUAL(emptyFile.getNumberOfLines(), 0)
RESULT

CHECK(bool hasSection(const String& section_name) const)
  TEST_EQUAL(ini.hasSection("replace test"), false)
	TEST_EQUAL(ini.hasSection(ini.HEADER), true)
  TEST_EQUAL(ini.hasSection("Section1"), true)  
  TEST_EQUAL(ini.hasSection("Section2"), true)  
  TEST_EQUAL(ini.hasSection("Section3"), true)  
  TEST_EQUAL(ini.hasSection("Section4"), true)  

	INIFile emptyFile;
  TEST_EQUAL(emptyFile.hasSection(emptyFile.HEADER), true)
RESULT

CHECK(SectionIterator getSection(const String& section_name))
	TEST_EQUAL(ini.isValid(ini.getSection("replace test")), false)
	TEST_EQUAL(ini.getSection(ini.HEADER)->getName(), ini.HEADER)
  TEST_EQUAL(ini.getSection("Section1")->getName(), "Section1")
  TEST_EQUAL(ini.getSection("Section2")->getName(), "Section2")
  TEST_EQUAL(ini.getSection("Section3")->getName(), "Section3")
  TEST_EQUAL(ini.getSection("Section4")->getName(), "Section4")

	INIFile emptyFile;
  TEST_EQUAL(emptyFile.isValid(emptyFile.getSection(emptyFile.HEADER)), true)
  TEST_EQUAL(emptyFile.getSection(emptyFile.HEADER)->getName(), ini.HEADER)
RESULT

CHECK(SectionIterator getSection(Position pos))
  TEST_EQUAL(ini.getSection(0)->getName(), ini.HEADER)
  TEST_EQUAL(ini.getSection(1)->getName(), "Section1")
	TEST_EQUAL(ini.getSection(2)->getName(), "Section2")
  TEST_EQUAL(ini.getSection(3)->getName(), "Section3")
  TEST_EQUAL(ini.getSection(4)->getName(), "Section4")
  TEST_EQUAL(ini.isValid(ini.getSection(5)), false)

	INIFile emptyFile;
  TEST_EQUAL(emptyFile.getSection(0)->getName(), ini.HEADER)
RESULT

CHECK(Size getNumberOfSections() const)
  TEST_EQUAL(ini.getNumberOfSections(), 4)

	INIFile emptyFile;
  TEST_EQUAL(emptyFile.getNumberOfSections(), 0)
RESULT

CHECK(LineIterator getSectionFirstLine(const String& section_name))
	INIFile::LineIterator it;
  TEST_EQUAL(+ini.getSectionFirstLine(ini.HEADER), false)
  TEST_EQUAL(+ini.getSectionFirstLine("Section1"), true)
  TEST_EQUAL(*ini.getSectionFirstLine("Section1"), "[Section1]")

  TEST_EQUAL(+ini.getSectionFirstLine("Section2"), true)
  TEST_EQUAL(*ini.getSectionFirstLine("Section1"), "[Section1]")
  TEST_EQUAL(*ini.getSectionFirstLine("Section2"), "[Section2]")
  TEST_EQUAL(*ini.getSectionFirstLine("Section3"), "[Section3]")
  TEST_EQUAL(*ini.getSectionFirstLine("Section4"), "[Section4]")
  TEST_EQUAL(+ini.getSectionFirstLine(""), false)

	INIFile emptyFile;
  TEST_EQUAL(+emptyFile.getSectionFirstLine(ini.HEADER), false)
RESULT

CHECK(LineIterator getSectionLastLine(const String& section_name))
  TEST_EQUAL(+ini.getSectionLastLine(ini.HEADER), false)
  TEST_EQUAL(*ini.getSectionLastLine("Section1"), "[Section1]")
  TEST_EQUAL(*ini.getSectionLastLine("Section2"), "! even more comment")
  TEST_EQUAL(*ini.getSectionLastLine("Section3"), "test3 = c")
  TEST_EQUAL(*ini.getSectionLastLine("Section4"), "[Section4]")
  TEST_EQUAL(+ini.getSectionLastLine(""), false)

	INIFile emptyFile;
  TEST_EQUAL(+emptyFile.getSectionLastLine(ini.HEADER), false)
RESULT

CHECK(Size getSectionLength(const String& section_name) const)
  TEST_EQUAL(ini.getSectionLength(ini.HEADER), 0)
  TEST_EQUAL(ini.getSectionLength("Section1"), 1)
  TEST_EQUAL(ini.getSectionLength("Section2"), 3)
  TEST_EQUAL(ini.getSectionLength("Section3"), 5)
  TEST_EQUAL(ini.getSectionLength("Section4"), 1)
  TEST_EQUAL(ini.getSectionLength("Section5"), INVALID_SIZE)

	INIFile emptyFile;
  TEST_EQUAL(+emptyFile.getSectionLength(ini.HEADER), 0)
RESULT

CHECK(bool hasEntry(const String& section, const String& key) const)
  TEST_EQUAL(ini.hasEntry(ini.HEADER, "test"), false)
  TEST_EQUAL(ini.hasEntry("Section1", "test"), false)
  TEST_EQUAL(ini.hasEntry("Section2", "test"), false)
  TEST_EQUAL(ini.hasEntry("Section2", "! even more comment"), false)
  TEST_EQUAL(ini.hasEntry("Section3", "test1"), true)
  TEST_EQUAL(ini.hasEntry("Section3", "test2"), true)
  TEST_EQUAL(ini.hasEntry("Section3", "test3"), true)
  TEST_EQUAL(ini.hasEntry("Section4", ""), false)

	INIFile emptyFile;
  TEST_EQUAL(emptyFile.hasEntry(ini.HEADER, "test"), false)
RESULT

CHECK(String getValue(const String& section, const String& key) const)
  TEST_EQUAL(ini.getValue(ini.HEADER, "test"), ini.UNDEFINED)
  TEST_EQUAL(ini.getValue("Section1", "test"), ini.UNDEFINED)
  TEST_EQUAL(ini.getValue("Section2", "test"), ini.UNDEFINED)
  TEST_EQUAL(ini.getValue("Section2", "! even more comment"), ini.UNDEFINED)
  TEST_EQUAL(ini.getValue("Section3", "test1"), "a")
  TEST_EQUAL(ini.getValue("Section3", "test2"), "b")
  TEST_EQUAL(ini.getValue("Section3", "test3"), "c")
  TEST_EQUAL(ini.getValue("Section4", ""), ini.UNDEFINED)

	INIFile emptyFile;
  TEST_EQUAL(emptyFile.getValue(emptyFile.HEADER, "test"), ini.UNDEFINED)
RESULT

CHECK(bool setValue(const String& section, const String& key, const String& value))
  TEST_EQUAL(ini.setValue(ini.HEADER, "test", "setValue_test"), false)
  TEST_EQUAL(ini.setValue("nonsense", "test", "setValue_test"), false)
  TEST_EQUAL(ini.setValue("Section1", "test", "setValue_test"), false)
  TEST_EQUAL(ini.getValue("Section1", "test"), ini.UNDEFINED)
  TEST_EQUAL(ini.setValue("Section3", "test1", "YYY"), true)
  TEST_EQUAL(*(ini.getLine(5)), "test1=YYY")
  TEST_EQUAL(ini.getValue("Section3", "test1"), "YYY")
  TEST_EQUAL(ini.setValue("Section3", "test1", "XXX"), true)
RESULT

CHECK(bool insertValue(const String& section, const String& key, const String& value))
	INIFile ini(BALL_TEST_DATA_PATH(INIFile_test.ini));
	ini.read();
	TEST_EQUAL(ini.insertValue("nonsense", "test", "insertValue_test"), false)
	TEST_EQUAL(ini.insertValue("Section1", "test", "insertValue_test"), true)
	TEST_EQUAL(ini.getValue("Section1", "test"), "insertValue_test")
	TEST_EQUAL(*(ini.getLine(1)), "test=insertValue_test")
	TEST_EQUAL(ini.insertValue("Section1", "test", "setValue_test"), false)
RESULT

CHECK(bool write())
	String filename;
	NEW_TMP_FILE(filename)
	ini.setFilename(filename);
	TEST_EQUAL(ini.write(), true)
	TEST_FILE(BALL_TEST_DATA_PATH(INIFile_test2.ini), filename.c_str())

	NEW_TMP_FILE(filename)
	INIFile emptyFile;
	emptyFile.setFilename(filename);
	TEST_EQUAL(emptyFile.write(), true)
RESULT

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
