// $Id: INIFile_test.C,v 1.4 2001/03/09 20:49:36 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/FORMAT/INIFile.h>
///////////////////////////

START_TEST(INIFile, "$Id: INIFile_test.C,v 1.4 2001/03/09 20:49:36 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

//#include <iostream>
//using namespace std;

using namespace BALL;

INIFile* ini1 = 0;
CHECK(INIFile::INIFile())
	ini1 = new INIFile;
	TEST_NOT_EQUAL(ini1, 0)
RESULT


CHECK(INIFile::getFilename() const )
	TEST_EQUAL(ini1->getFilename(), "")
RESULT


CHECK(INIFile::isValid() const )
	TEST_EQUAL(ini1->isValid(), false)
RESULT


CHECK(INIFile::~INIFile())
  delete ini1;
RESULT


String filename;
NEW_TMP_FILE(filename)
CHECK(INIFile::INIFile(const String& filename))
	INIFile ini(filename);
	TEST_EQUAL(ini.getFilename(), filename)
RESULT


CHECK(INIFile::setFilename(const String& filename))
	INIFile ini;
	ini.setFilename("TEST");
	TEST_EQUAL(ini.getFilename(), "TEST")
	ini.setFilename("");
	TEST_EQUAL(ini.getFilename(), "")
RESULT


CHECK(INIFile::destroy())
	INIFile ini;
	ini.setFilename("TEST");
	ini.destroy();
	TEST_EQUAL(ini.getFilename(), "")
RESULT


CHECK(INIFile::clear())
	INIFile ini;
	ini.setFilename("TEST");
	ini.clear();
	TEST_EQUAL(ini.getFilename(), "TEST")
RESULT


INIFile ini;

CHECK(INIFile::read())
	ini.setFilename("data/INIFile_test.ini");
	TEST_EQUAL(ini.read(), true)
RESULT


CHECK(INIFile::getLine(Size line_number))
  TEST_EQUAL(*(ini.getLine(0)), "[Section1]")
  TEST_EQUAL(*(ini.getLine(1)), "[Section2]")
  TEST_EQUAL(*(ini.getLine(10)), "[Section4]")
RESULT


CHECK(INIFile::setLine(Size line_number, const String& line))
  TEST_EQUAL(ini.setLine(2, "replace test"), true)
  TEST_EQUAL(*(ini.getLine(2)), "replace test")
  TEST_EQUAL(ini.setLine(11, "replace test"), false)
RESULT


CHECK(INIFile::getNumberOfLines() const )
	INIFile ini2("data/amber91.ini");
	ini2.read();
	TEST_EQUAL(ini2.getNumberOfLines(), 1379)
  TEST_EQUAL(ini.getNumberOfLines(), 11)
RESULT


CHECK(INIFile::hasSection(const String& section_name) const )
  TEST_EQUAL(ini.hasSection("replace test"), false)
  TEST_EQUAL(ini.hasSection("Section1"), true)  
  TEST_EQUAL(ini.hasSection("Section2"), true)  
  TEST_EQUAL(ini.hasSection("Section3"), true)  
  TEST_EQUAL(ini.hasSection("Section4"), true)  
RESULT

CHECK(INIFile::getNumberOfSections() const )
  TEST_EQUAL(ini.getNumberOfSections(), 4)
RESULT


CHECK(INIFile::getSectionName(Position pos) const )
  TEST_EQUAL(*(ini.getSectionName(0)), "Section1")
	TEST_EQUAL(*(ini.getSectionName(1)), "Section2")
  TEST_EQUAL(*(ini.getSectionName(2)), "Section3")
  TEST_EQUAL(*(ini.getSectionName(3)), "Section4")  
  TEST_EQUAL(ini.getSectionName(4), 0)  
RESULT


CHECK(INIFile::getSectionFirstLine(const String& section_name) const )
  TEST_EQUAL(ini.getSectionFirstLine("Section1"), INVALID_SIZE)
  TEST_EQUAL(ini.getSectionFirstLine("Section2"), 2)
  TEST_EQUAL(ini.getSectionFirstLine("Section3"), 6)
  TEST_EQUAL(ini.getSectionFirstLine("Section4"), INVALID_SIZE)
  TEST_EQUAL(ini.getSectionFirstLine(""), INVALID_SIZE)
RESULT


CHECK(INIFile::getSectionLastLine(const String& section_name) const )
  TEST_EQUAL(ini.getSectionLastLine("Section1"), INVALID_SIZE)
  TEST_EQUAL(ini.getSectionLastLine("Section2"), 4)
  TEST_EQUAL(ini.getSectionLastLine("Section3"), 9)
  TEST_EQUAL(ini.getSectionLastLine("Section4"), INVALID_SIZE)
  TEST_EQUAL(ini.getSectionLastLine(""), INVALID_SIZE)
RESULT


CHECK(INIFile::getSectionLength(const String& section_name) const )
  TEST_EQUAL(ini.getSectionLength("Section1"), 0)
  TEST_EQUAL(ini.getSectionLength("Section2"), 3)
  TEST_EQUAL(ini.getSectionLength("Section3"), 4)
  TEST_EQUAL(ini.getSectionLength("Section4"), 0)
  TEST_EQUAL(ini.getSectionLength("Section5"), INVALID_SIZE)
RESULT


CHECK(INIFile::hasEntry(const String& section, const String& key) const )
  TEST_EQUAL(ini.hasEntry("Section1", "test"), false)
  TEST_EQUAL(ini.hasEntry("Section2", "test"), false)
  TEST_EQUAL(ini.hasEntry("Section2", "! even more comment"), false)
  TEST_EQUAL(ini.hasEntry("Section3", "test1"), true)
  TEST_EQUAL(ini.hasEntry("Section3", "test2"), true)
  TEST_EQUAL(ini.hasEntry("Section3", "test3"), true)
  TEST_EQUAL(ini.hasEntry("Section4", ""), false)
RESULT


CHECK(INIFile::getValue(const String& section, const String& key) const )
  TEST_EQUAL(ini.getValue("Section1", "test"), "")
  TEST_EQUAL(ini.getValue("Section2", "test"), "")
  TEST_EQUAL(ini.getValue("Section2", "! even more comment"), "")
  TEST_EQUAL(ini.getValue("Section3", "test1"), "a")
  TEST_EQUAL(ini.getValue("Section3", "test2"), "b")
  TEST_EQUAL(ini.getValue("Section3", "test3"), "c")
  TEST_EQUAL(ini.getValue("Section4", ""), "")
RESULT


CHECK(INIFile::setValue(const String& section, const String& key, const String& value))
  TEST_EQUAL(ini.setValue("Section1", "test", "blabla"), true)
  TEST_EQUAL(ini.getValue("Section1", "test"), "blabla")
  TEST_EQUAL(ini.setValue("Section3", "test1", "XXX"), true)
  TEST_EQUAL(ini.getValue("Section3", "test1"), "XXX")
RESULT

CHECK(INIFile::write())
	String filename;
	NEW_TMP_FILE(filename)
	ini.setFilename(filename);
	TEST_EQUAL(ini.write(), true)
	TEST_FILE("data/INIFile_test2.ini", filename.c_str(), false)
RESULT

/*
	StringHashMap<Size>::Iterator it = ini.section_key_map_.begin();
	TEST_EQUAL((*it).first, "");
	TEST_EQUAL((*it).second, 0);
	++it;
	TEST_EQUAL((*it).first, "");
	TEST_EQUAL((*it).second, 0);
	++it;
	TEST_EQUAL((*it).first, "");
	TEST_EQUAL((*it).second, 0);
*/

/*
	for (int i = 0; i < ini.section_names_.size() ; i++)
	{ 
		cout << endl<< ini.section_names_[i] << endl;
		cout << ini.line_section_index_[i] << endl;
		cout << ini.section_start_[i] << endl;
		cout << ini.section_end_[i] << endl;
		cout << ini.section_key_map_[i] << endl;
		cout << ini.section_index_[i] << endl;
		cout << endl;
	}
*/
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
