// $Id: DCDFile_test.C,v 1.1 2000/12/19 13:25:13 anker Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

// insert includes here
#include <BALL/FORMAT/DCDFile.h>
#include <BALL/MOLMEC/MDSIMULATION/molecularDynamics.h>

///////////////////////////

START_TEST(class_name, "$Id: DCDFile_test.C,v 1.1 2000/12/19 13:25:13 anker Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

///  insert tests for each member function here         
///

CHECK(DCDHeader::DCDHeader())
	// BAUSTELLE
RESULT

CHECK(DCDHeader::operator << (os, header))
	// BAUSTELLE
RESULT

CHECK(DCDHeader::operator >> (os, header))
	// BAUSTELLE
RESULT

CHECK(DCDFile::DCDFile())
	// BAUSTELLE
RESULT

CHECK(DCDFile::DCDFile(const DCDFile& file))
	// BAUSTELLE
RESULT

CHECK(DCDFile::DCDFile(const String& name, File::OpenMode open_mode))
	// BAUSTELLE
RESULT

CHECK(DCDFile::~DCDFile)
	// BAUSTELLE
RESULT

CHECK(DCDFile::operator = (const DCDFile& file))
	// BAUSTELLE
RESULT

CHECK(DCDFile::clear())
	// BAUSTELLE
RESULT

CHECK(DCDFile::operator == (const DCDFile& file))
	// BAUSTELLE
RESULT

CHECK(DCDFile::readHeader())
	// BAUSTELLE
RESULT

CHECK(DCDFile::updateHeader(const SnapShotManager& manager))
	// BAUSTELLE
RESULT

CHECK(DCDFile::writeHeader())
	// BAUSTELLE
RESULT

CHECK(DCDFile::read(SnapShotManager& manager))
	// BAUSTELLE
RESULT

CHECK(DCDFile::write(const SnapShotManager& manager))
	// BAUSTELLE
RESULT

CHECK(DCDFile::append(const SnapShot& snapshot))
	// BAUSTELLE
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
