// $Id: INIFile_test.C,v 1.7 2001/04/08 23:32:07 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/FORMAT/INIFile.h>
#include "ItemCollector.h"
///////////////////////////

START_TEST(INIFile, "$Id: INIFile_test.C,v 1.7 2001/04/08 23:32:07 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

#include <iostream>
using namespace std;

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


// BAUSTELLE mit header, nur header
CHECK(INIFile::read())
	ini.setFilename("data/INIFile_test.ini");
	TEST_EQUAL(ini.read(), true)
	
	INIFile ini2("data/empty_file.txt");
	TEST_EQUAL(ini2.read(), true)
RESULT


CHECK(INIFile::getLine(Size line_number))
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
RESULT


CHECK(INIFile::getNumberOfLines() const )
	INIFile ini2("data/amber91.ini");
	ini2.read();
	TEST_EQUAL(ini2.getNumberOfLines(), 1379)
  TEST_EQUAL(ini.getNumberOfLines(), 10)
RESULT


CHECK(INIFile::hasSection(const String& section_name) const )
  TEST_EQUAL(ini.hasSection("replace test"), false)
	TEST_EQUAL(ini.hasSection(ini.HEADER), true)
  TEST_EQUAL(ini.hasSection("Section1"), true)  
  TEST_EQUAL(ini.hasSection("Section2"), true)  
  TEST_EQUAL(ini.hasSection("Section3"), true)  
  TEST_EQUAL(ini.hasSection("Section4"), true)  
RESULT

CHECK(INIFile::getSection(const String& section_name) const )
	INIFile::Section_iterator it;
	TEST_EQUAL(ini.getSection("replace test") == 0, true)
	TEST_EQUAL(ini.getSection(ini.HEADER)->getName(), ini.HEADER)
  TEST_EQUAL(ini.getSection("Section1")->getName(), "Section1")
  TEST_EQUAL(ini.getSection("Section2")->getName(), "Section2")
  TEST_EQUAL(ini.getSection("Section3")->getName(), "Section3")
  TEST_EQUAL(ini.getSection("Section4")->getName(), "Section4")
RESULT

CHECK(INIFile::getNumberOfSections() const )
  TEST_EQUAL(ini.getNumberOfSections(), 4)
RESULT


CHECK(INIFile::getSection(Position pos) const )
  TEST_EQUAL(ini.getSection(0)->getName(), ini.HEADER)
  TEST_EQUAL(ini.getSection(1)->getName(), "Section1")
	TEST_EQUAL(ini.getSection(2)->getName(), "Section2")
  TEST_EQUAL(ini.getSection(3)->getName(), "Section3")
  TEST_EQUAL(ini.getSection(4)->getName(), "Section4")
  TEST_EQUAL(ini.getSection(5) == 0, true)
RESULT


CHECK(INIFile::getSectionFirstLine(const String& section_name) const )
	INIFile::LineIterator it;
  TEST_EQUAL(ini.getSectionFirstLine(ini.HEADER) == it, 0)//BAUSTELLE
  TEST_EQUAL(+ini.getSectionFirstLine(ini.HEADER), false)
  TEST_EQUAL(+ini.getSectionFirstLine("Section1"), true)
  TEST_EQUAL(*ini.getSectionFirstLine("Section1"), "[Section1]")

  TEST_EQUAL(+ini.getSectionFirstLine("Section2"), true)
  TEST_EQUAL(*ini.getSectionFirstLine("Section1"), "[Section1]")
  TEST_EQUAL(*ini.getSectionFirstLine("Section2"), "[Section2]")
  TEST_EQUAL(*ini.getSectionFirstLine("Section3"), "[Section3]")
  TEST_EQUAL(*ini.getSectionFirstLine("Section4"), "[Section4]")
  TEST_EQUAL(ini.getSectionFirstLine("") == it, true)
RESULT


CHECK(INIFile::getSectionLastLine(const String& section_name) const )
  TEST_EQUAL(+ini.getSectionLastLine(ini.HEADER), false)
//BAUSTELLE
  TEST_EQUAL(*ini.getSectionLastLine("Section1"), "[Section1]")
  TEST_EQUAL(*ini.getSectionLastLine("Section2"), "! even more comment")
  TEST_EQUAL(*ini.getSectionLastLine("Section3"), "test3 = c")
  TEST_EQUAL(*ini.getSectionLastLine("Section4"), "[Section4]")
  TEST_EQUAL(+ini.getSectionLastLine(""), false)
RESULT


CHECK(INIFile::getSectionLength(const String& section_name) const )
  TEST_EQUAL(ini.getSectionLength(ini.HEADER), 0)
  TEST_EQUAL(ini.getSectionLength("Section1"), 1)
  TEST_EQUAL(ini.getSectionLength("Section2"), 3)
  TEST_EQUAL(ini.getSectionLength("Section3"), 5)
  TEST_EQUAL(ini.getSectionLength("Section4"), 1)
  TEST_EQUAL(ini.getSectionLength("Section5"), INVALID_SIZE)
RESULT


CHECK(INIFile::hasEntry(const String& section, const String& key) const )
  TEST_EQUAL(ini.hasEntry(ini.HEADER, "test"), false)
  TEST_EQUAL(ini.hasEntry("Section1", "test"), false)
  TEST_EQUAL(ini.hasEntry("Section2", "test"), false)
  TEST_EQUAL(ini.hasEntry("Section2", "! even more comment"), false)
  TEST_EQUAL(ini.hasEntry("Section3", "test1"), true)
  TEST_EQUAL(ini.hasEntry("Section3", "test2"), true)
  TEST_EQUAL(ini.hasEntry("Section3", "test3"), true)
  TEST_EQUAL(ini.hasEntry("Section4", ""), false)
RESULT


CHECK(INIFile::getValue(const String& section, const String& key) const )
  TEST_EQUAL(ini.getValue(ini.HEADER, "test"), ini.UNDEFINED)
  TEST_EQUAL(ini.getValue("Section1", "test"), ini.UNDEFINED)
  TEST_EQUAL(ini.getValue("Section2", "test"), ini.UNDEFINED)
  TEST_EQUAL(ini.getValue("Section2", "! even more comment"), ini.UNDEFINED)
  TEST_EQUAL(ini.getValue("Section3", "test1"), "a")
  TEST_EQUAL(ini.getValue("Section3", "test2"), "b")
  TEST_EQUAL(ini.getValue("Section3", "test3"), "c")
  TEST_EQUAL(ini.getValue("Section4", ""), ini.UNDEFINED)
RESULT


CHECK(INIFile::setValue(const String& section, const String& key, const String& value))
  TEST_EQUAL(ini.setValue(ini.HEADER, "test", "setValue_test"), false)
  TEST_EQUAL(ini.setValue("nonsense", "test", "setValue_test"), false)
  TEST_EQUAL(ini.setValue("Section1", "test", "setValue_test"), false)
  TEST_EQUAL(ini.getValue("Section1", "test"), ini.UNDEFINED)
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

// ok basic tests work
// now we are testing insert/delete/set etc...
CHECK(INIFile::deleteLine)
	INIFile ini("data/INIFile_test.ini");
	ini.read();

	INIFile::LineIterator it;
	TEST_EQUAL(ini.deleteLine(it), false)
	
	it = ini.getSectionFirstLine("Section3");
  ++it;
	++it;
	TEST_EQUAL(*it, "test2=b")
	TEST_EQUAL(ini.deleteLine(it), true)

	TEST_EQUAL(ini.hasEntry("Section3", "test2"), false)
  TEST_EQUAL(ini.getValue("Section3", "test2"), ini.UNDEFINED)
  TEST_EQUAL(ini.getSectionLength("Section3"), 4)
RESULT


CHECK(INIFile::appendLine(const String& section_name, const String& line))
	INIFile ini("data/INIFile_test.ini");
	ini.read();
	
	TEST_EQUAL(ini.appendLine("Section9", "GAU"), false)	
	TEST_EQUAL(ini.appendLine("Section2", "[AU"), false)	
	TEST_EQUAL(ini.appendLine("Section3", "test1 = 123"), false)	
	TEST_EQUAL(ini.getNumberOfLines(), 10)

	TEST_EQUAL(ini.appendLine("Section3", "insert1=ok"), true)
	TEST_EQUAL(ini.getNumberOfLines(), 11)
	TEST_EQUAL(ini.getSectionLength("Section3"), 6)
	TEST_EQUAL(ini.hasEntry("Section3", "insert1"), true)
	TEST_EQUAL(ini.getValue("Section3", "insert1"), "ok")
RESULT


CHECK(apply(UnaryProcessor<LineIterator>& processor))
	ItemCollector<INIFile::LineIterator> myproc;
	INIFile ini("data/INIFile_test.ini");
	TEST_EQUAL(ini.apply(myproc), true)

	TEST_EQUAL(myproc.getSize(), 11)
/*
	TEST_EQUAL(*myproc.getPointer(), 1) myproc.forward();
	TEST_EQUAL(*myproc.getPointer(), 2) myproc.forward();
	TEST_EQUAL(*myproc.getPointer(), 3) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), 0)*/
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
