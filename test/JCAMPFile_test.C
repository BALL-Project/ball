// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/FORMAT/JCAMPFile.h>

///////////////////////////

START_TEST(class_name)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

JCAMPFile* ptr = 0;
CHECK(JCAMPFile() throw())
	ptr = new JCAMPFile;
	TEST_NOT_EQUAL(ptr, 0)
RESULT

CHECK(~JCAMPFile() throw())
	delete ptr;
RESULT

CHECK(JCAMPFile(const String& name, OpenMode open_mode = std::ios::in) throw(Exception::FileNotFound))
	TEST_EXCEPTION(Exception::FileNotFound, JCAMPFile("asddddddddddddaaaaaaaacasdd"))
	JCAMPFile jcamp(BALL_TEST_DATA_PATH(JCAMPFile_test.dat));
RESULT

CHECK(void read() throw(Exception::ParseError))
	JCAMPFile jcamp(BALL_TEST_DATA_PATH(JCAMPFile_test.dat));
	jcamp.read();

	/*
	StringHashMap<JCAMPFile::JCAMPValue>::Iterator it = jcamp.getEntries().begin();
	for(;+it;++it)
	{
		Log.error() << it->first << "  " << it->second.string_value << "         ";
		if (it->second.numeric_value.size() > 0) Log.error() << it->second.numeric_value[0] << std::endl;
		else Log.error() << std::endl;
	}
	*/
	TEST_EQUAL(jcamp.hasEntry("SYMM"), true)
	TEST_EQUAL(jcamp.getIntValue("SYMM"), 0)
	TEST_EQUAL(jcamp.hasEntry("ML3"), true)
	TEST_REAL_EQUAL(jcamp.getDoubleValue("ML3"), 0.00542953585106775)

	JCAMPFile empty;
	TEST_EXCEPTION(Exception::ParseError, empty.read())
RESULT

CHECK(bool write() throw(File::CannotWrite))
	JCAMPFile jcamp(BALL_TEST_DATA_PATH(JCAMPFile_test.dat));
	jcamp.read();
	jcamp.close();
	String filename;
	NEW_TMP_FILE(filename);
	JCAMPFile out(filename, File::MODE_OUT);
	out.getHeader() = jcamp.getHeader();
	out.getEntries() = jcamp.getEntries();
	out.write();

	JCAMPFile empty;
	TEST_EXCEPTION(File::CannotWrite, empty.write())
RESULT

CHECK(HeaderMap& getHeader() throw())
	// ????
RESULT

CHECK(const HeaderMap& getHeader() const throw())
	// ????
RESULT

CHECK(EntryMap& getEntries() throw())
	// ????
RESULT

CHECK(const EntryMap& getEntries() const throw())
	// ????
RESULT

CHECK(bool hasEntry(const String& name) const throw())
	// ????
RESULT
											
CHECK(Index getIntValue(const String& name) const throw(Exception::InvalidFormat))
	// ????
RESULT

CHECK(bool hasHeader(const String& name) const throw())
	// ????
RESULT

CHECK(const JCAMPValue& operator [] (const String& name) const)
	// ????
RESULT

CHECK(double getDoubleValue(const String& name) const throw(Exception::InvalidFormat))
	// ????
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
