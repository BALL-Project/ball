// $Id: INIFile_test.C,v 1.2 2000/08/23 15:45:18 anker Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/FORMAT/INIFile.h>

///////////////////////////

START_TEST(INIFile, "$Id: INIFile_test.C,v 1.2 2000/08/23 15:45:18 anker Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

INIFile* ini = 0;
CHECK(INIFile::INIFile())
	ini = new INIFile;
	TEST_NOT_EQUAL(ini, 0)
RESULT

CHECK(INIFile::getFilename() const )
	TEST_EQUAL(ini->getFilename(), "")
RESULT

CHECK(INIFile::isValid() const )
	TEST_EQUAL(ini->isValid(), true)
RESULT


CHECK(INIFile::~INIFile())
  delete ini;
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
  //BAUSTELLE
RESULT


CHECK(INIFile::clear())
  //BAUSTELLE
RESULT


CHECK(INIFile::read())
  //BAUSTELLE
RESULT


CHECK(INIFile::write())
  //BAUSTELLE
RESULT


CHECK(INIFile::getLine(Size line_number))
  //BAUSTELLE
RESULT


CHECK(INIFile::setLine(Size line_number, const String& line))
  //BAUSTELLE
RESULT


CHECK(INIFile::getNumberOfLines() const )
	INIFile ini("data/amber91.ini");
	ini.read();
	TEST_EQUAL(ini.getNumberOfLines(), 1379)
  //BAUSTELLE
RESULT


CHECK(INIFile::hasSection(const String& section_name) const )
  //BAUSTELLE
RESULT


CHECK(INIFile::getSectionFirstLine(const String& section_name) const )
  //BAUSTELLE
RESULT


CHECK(INIFile::getSectionLastLine(const String& section_name) const )
  //BAUSTELLE
RESULT


CHECK(INIFile::getSectionLength(const String& section_name) const )
  //BAUSTELLE
RESULT


CHECK(INIFile::hasEntry(const String& section, const String& key) const )
  //BAUSTELLE
RESULT


CHECK(INIFile::getValue(const String& section, const String& key) const )
  //BAUSTELLE
RESULT


CHECK(INIFile::setValue(const String& section, const String& key, const String& value))
  //BAUSTELLE
RESULT


											

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
