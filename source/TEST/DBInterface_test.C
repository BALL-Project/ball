// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

// NOTE: This test *cannot* be run automatically unless you provide login credentials
// for the database in your home directory (~/.ballrc). Please make sure that this
// file is readable for *YOU ALONE*, as it will contain the login credentials in
// plain text! See the documentation of BALL/include/BALL/FORMAT/DBInterface.h for
// details.
// If no .ballrc is found, the test will abort without failing!
//
// There is a default database (BALL_DBINTERFACE_TEST) required to exist.
// you can easily create it by modifying the name in the first two lines
// of BALL/source/APPLICATIONS/create_database.sql and executing that
// SQL script.

///////////////////////////

#include <BALL/FORMAT/DBInterface.h>

#include <BALL/FORMAT/MOLFile.h>
#include <BALL/FORMAT/INIFile.h>

#include <QtGui/QApplication>

///////////////////////////

START_TEST(DBInterface)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

QApplication qapp(argc, argv, false);

DBInterface* dbi_ptr = 0;
CHECK(DBInterface())
	dbi_ptr = new DBInterface;
	TEST_NOT_EQUAL(dbi_ptr, 0)
RESULT

CHECK(~DBInterface())
	delete dbi_ptr;
RESULT


// Make sure we can connect by default to a database named STRUCTURE_TEST

String filename = DBInterface::BALL_DEFAULT_DBRCFILE;
FileSystem::canonizePath(filename);
bool have_access = true;
if (File::isAccessible(filename) && File::isReadable(filename))
{
	INIFile rc(filename);
	rc.read();
	String user = "";
	String password = "";
	String host = DBInterface::BALL_DEFAULT_DATABASE_HOST;
	Size port = DBInterface::BALL_DEFAULT_DATABASE_PORT;
	String driver = DBInterface::BALL_DEFAULT_DATABASE_DRIVER;

	String database = "BALL_DBINTERFACE_TEST";

	if (rc.hasEntry(DBInterface::BALL_DEFAULT_DATABASE_SECTIONNAME, "host"))
	{
		host = rc.getValue(DBInterface::BALL_DEFAULT_DATABASE_SECTIONNAME, "host");
	}
	if (rc.hasEntry(DBInterface::BALL_DEFAULT_DATABASE_SECTIONNAME, "driver"))
	{
		driver = rc.getValue(DBInterface::BALL_DEFAULT_DATABASE_SECTIONNAME, "driver");
	}
	if (rc.hasEntry(DBInterface::BALL_DEFAULT_DATABASE_SECTIONNAME, "port"))
	{
		port = (Size)rc.getValue(DBInterface::BALL_DEFAULT_DATABASE_SECTIONNAME, "port").toInt();
	}
	if (rc.hasEntry(DBInterface::BALL_DEFAULT_DATABASE_SECTIONNAME, "user")
			&& rc.hasEntry(DBInterface::BALL_DEFAULT_DATABASE_SECTIONNAME, "password"))
	{
		user = rc.getValue(DBInterface::BALL_DEFAULT_DATABASE_SECTIONNAME, "user");
		password = rc.getValue(DBInterface::BALL_DEFAULT_DATABASE_SECTIONNAME, "password");
	}
	else
	{
		// user/password entries are mandatory -- exit if they are not given
    have_access = false;
		STATUS("Database access: no login/pw available.")
	}
	if (have_access)
	{
		DBInterface dbi;
		bool success = dbi.connect(user, password, database, host, port, driver);
		have_access &= success;
		STATUS("Database access: result of connect(...) = " << success)
	}
}
else
{
	STATUS("Database access: no credentials file.")
}

STATUS("Database access: " << (have_access ? "yes" : "no"))

// Execute the remaining tests only if we could access the database.
if (have_access)
{
	CHECK(bool connect())
		// ???
	RESULT

	CHECK((bool connect(const String& user, const String& password, const String& database_name = "structures", const String& host = "diclofenac.informatik.uni-tuebingen.de", Size port = 3306, const String& driver = "QMYSQL3")))
		// ???
	RESULT



	CHECK(ConformationMethod getConformationMethod(ID method_id))
		// ???
	RESULT

	CHECK(ErrorCode getError() const)
		// ???
	RESULT

	CHECK((ID getConformationMethod(const String& method, const String& parameters)))
		// ???
	RESULT

	CHECK(ID lastInsertedID())
		// ???
	RESULT

	CHECK((ID newConformationMethod(const String& method, const String& parameters)))
		// ???
	RESULT

	CHECK((ID newTopology(const System& system, const String& name, const String& source_id, ID id = 0)))
		// ???
	RESULT

	CHECK((ID storeConformation(ID topology, ID method_ID, const System& system)))
		// ???
	RESULT

	CHECK(IDVector getConformationList(ID structure))
		// ???
	RESULT

	CHECK((IDVector getConformationList(ID topology_id, ID method_id)))
		// ???
	RESULT

	CHECK(IDVector getConformationMethods())
		// ???
	RESULT

	CHECK((QSqlQuery& executeQuery() throw(InvalidQuery, NotConnected)))
		// ???
	RESULT

	CHECK((QSqlQuery& executeQuery(const String& query_string) throw(InvalidQuery, NotConnected)))
		// ???
	RESULT

	CHECK(QSqlQuery& query())
		// ???
	RESULT

	CHECK(QVariant value(Position k))
		// ???
	RESULT

	CHECK(Size size())
		// ???
	RESULT

	CHECK(String executedQuery())
		// ???
	RESULT

	CHECK(bool first())
		// ???
	RESULT

	CHECK((bool getMOLFile(ID id, String& file_text, String& name, String& source_id)))
		// ???
	RESULT

	CHECK(bool good() const)
		// ???
	RESULT

	CHECK(bool last())
		// ???
	RESULT

	CHECK(bool next())
		// ???
	RESULT

	CHECK(bool prev())
		// ???
	RESULT

	CHECK(void addBindValue(const QVariant& v))
		// ???
	RESULT

	CHECK((void addBindValue(const QVariant& v1, const QVariant& v2)))
		// ???
	RESULT

	CHECK((void addBindValue(const QVariant& v1, const QVariant& v2, const QVariant& v3)))
		// ???
	RESULT

	CHECK((void addBindValue(const QVariant& v1, const QVariant& v2, const QVariant& v3, const QVariant& v4)))
		// ???
	RESULT

	CHECK((void addMOLFileToSystem(const String& molfile, System& system)))
		// ???
	RESULT

	CHECK((void getTopology(ID topology, System& system)))
		// ???
	RESULT

	CHECK((void loadConformation(const ID conformation, System& system)))
		// ???
	RESULT

	CHECK(void prepare(const String& s))
		// ???
	RESULT

	CHECK(void setError(ErrorCode error))
		// ???
	RESULT

	CHECK((void setTopology(ID topology, const System& system)))
		// ???
	RESULT
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
