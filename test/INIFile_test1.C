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
  TEST_EQUAL(ini.getSectionLength("Section5"), INVALID_Size)

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


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
