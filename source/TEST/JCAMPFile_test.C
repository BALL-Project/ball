// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: JCAMPFile_test.C,v 1.2 2003/06/01 17:06:01 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/FORMAT/JCAMPFile.h>

///////////////////////////

START_TEST(class_name, "$Id: JCAMPFile_test.C,v 1.2 2003/06/01 17:06:01 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

JCAMPFile* ptr = 0;
CHECK(JCAMPFile() throw())
	ptr = new JCAMPFile;
	TEST_NOT_EQUAL(ptr, 0)
RESULT

CHECK(virtual ~JCAMPFile() throw())
	delete ptr;
RESULT

CHECK(JCAMPFile(const String& name, OpenMode open_mode = std::ios::in) throw(Exception::FileNotFound))
	// ????
RESULT

CHECK(JCAMPFile(const JCAMPFile& file) throw(Exception::FileNotFound))
	// ????
RESULT

CHECK(void read() throw(Exception::ParseError))
	JCAMPFile jcamp(String("data") + FileSystem::PATH_SEPARATOR + "JCAMPFile_test.dat");
	jcamp.read();
RESULT

CHECK(void write())
	// ????
RESULT

CHECK(JCAMPFile::HeaderMap& getHeader() throw())
	// ????
RESULT

CHECK(const JCAMPFile::HeaderMap& getHeader() const throw())
	// ????
RESULT

CHECK(JCAMPFile::EntryMap& getEntries() throw())
	// ????
RESULT

CHECK(const JCAMPFile::EntryMap& getEntries() const throw())
	// ????
RESULT

CHECK(bool has(const String& name) const)
	// ????
RESULT
											

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
