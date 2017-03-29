// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/DATATYPE/options.h>
#include <BALL/CONCEPT/textPersistenceManager.h>

START_TEST(Options)

using BALL::Options;
using BALL::Vector3;
using BALL::TextPersistenceManager;
using std::ofstream;
using std::ifstream;

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
	
using namespace BALL;

Options*	options;
CHECK(Options() throw())
	options = new Options;
	TEST_NOT_EQUAL(options, 0)
RESULT

CHECK(~Options() throw())
	delete options;
RESULT

CHECK(void setName(const String& name) throw())
	options = new Options;
	options->setName("ABCDEFG");
RESULT

CHECK(const String& getName() const throw())
	TEST_EQUAL(options->getName(), "ABCDEFG");
RESULT

Options*	options2;
CHECK(Options(const Options& options) throw())
	options2 = new Options(*options);
	TEST_EQUAL(options2->getName(), options->getName())
	TEST_EQUAL(options2->get("ABC"), options->get("ABC"))
RESULT

CHECK(void setBool(const String& key, const bool value) throw())
	options->setBool("BOOL", true);
	TEST_EQUAL(options->getBool("BOOL"), true)
	options->setBool("BOOL", false);
RESULT

CHECK(bool getBool(const String& key) const throw())
	TEST_EQUAL(options->getBool("BOOL"), false)
	TEST_EQUAL(options->getBool("UNDEFINED"), false)
RESULT

CHECK(void setReal(const String& key, const double value) throw())
	options->setReal("REAL", 1.23456);
RESULT

CHECK(double getReal(const String& key) const throw())
	TEST_REAL_EQUAL(1.23456, options->getReal("REAL"))
	TEST_REAL_EQUAL(options->getReal("UNDEFINED"), 0.0)
RESULT

Vector3	vector(1.0, 2.0, 3.0);
CHECK(void setVector(const String& key, const Vector3& value) throw())
	options->setVector("VECTOR", vector);
	TEST_EQUAL(vector, options->getVector("VECTOR"))
RESULT

CHECK(Vector3 getVector(const String& key) const throw())
	Vector3 v(0.0, 0.0, 0.0);
	TEST_EQUAL(options->getVector("UNDEFINED"), v)
RESULT

CHECK(void setInteger(const String& key, const long value) throw())
	options->setInteger("INT", 1234567890);
	TEST_EQUAL(1234567890, options->getInteger("INT"))
RESULT

CHECK(long getInteger(const String& key) const throw())
	TEST_EQUAL(options->getInteger("UNDEFINED"), 0)
RESULT

CHECK([EXTRA]has)
	TEST_EQUAL(options->has("BOOL"), true)
	TEST_EQUAL(options->has("UNDEFINED"), false)
RESULT

CHECK(bool isInteger(const String& key) const throw())
	TEST_EQUAL(options->isInteger("INT"), true)
	TEST_EQUAL(options->isInteger("REAL"), false)
	TEST_EQUAL(options->isInteger("BOOL"), false)
	TEST_EQUAL(options->isBool("VECTOR"), false)
	TEST_EQUAL(options->isInteger("undefined"), false)
RESULT

CHECK(bool isBool(const String& key) const throw())
	TEST_EQUAL(options->isBool("BOOL"), true)
	TEST_EQUAL(options->isBool("UNDEFINED"), false)
	TEST_EQUAL(options->isBool("INT"), false)
	TEST_EQUAL(options->isBool("REAL"), false)
	TEST_EQUAL(options->isBool("VECTOR"), false)
RESULT


CHECK(bool isReal(const String& key) const throw())
	TEST_EQUAL(options->isReal("REAL"), true)
	TEST_EQUAL(options->isReal("INT"), true)
	TEST_EQUAL(options->isReal("BOOL"), false)
	TEST_EQUAL(options->isReal("UNDEFINED"), false)
	TEST_EQUAL(options->isBool("VECTOR"), false)
RESULT


CHECK(bool isVector(const String& key) const throw())
	TEST_EQUAL(options->isVector("INT"), false)
	TEST_EQUAL(options->isVector("REAL"), false)
	TEST_EQUAL(options->isVector("BOOL"), false)
	TEST_EQUAL(options->isVector("UNDEFINED"), false)
	TEST_EQUAL(options->isVector("VECTOR"), true)
	options->set("SVECTOR", "(0.0 1.0 2.0) ");
	TEST_EQUAL(options->isVector("SVECTOR"), true)
RESULT

CHECK(bool isSet(const String& key) const throw())
	TEST_EQUAL(options->isSet("INT"), true)
	TEST_EQUAL(options->isSet("undefined"), false)
RESULT

CHECK(void set(const String& key, const String& value) throw())
	options->set("ABC", "DEF");	
	TEST_EQUAL("DEF", options->get("ABC"))
	TEST_EQUAL(options->get("UNDEFINED"), "")
RESULT

CHECK(String setDefault(const String& key, const String& value) throw())
	options->setDefault("DEF", "default");
	options->setDefault("DEF", "default2");
	TEST_EQUAL(options->get("DEF"), "default")
RESULT

CHECK(long setDefaultInteger(const String& key, const long value) throw())
	options->setDefaultInteger("DEFINT", 123456);
	options->setDefaultInteger("DEFINT", 234567);
	TEST_EQUAL(options->getInteger("DEFINT"), 123456)
RESULT

CHECK(double setDefaultReal(const String& key, const double value) throw())
	options->setDefaultReal("DEFREAL", 1.23456);
	options->setDefaultReal("DEFREAL", 2.34567);
	TEST_REAL_EQUAL(options->getReal("DEFREAL"), 1.23456)
RESULT

CHECK(bool setDefaultBool(const String& key, const bool value) throw())
	options->setDefaultBool("DEFBOOL", true);
	options->setDefaultBool("DEFBOOL", false);
	TEST_EQUAL(options->getBool("DEFBOOL"), true)
RESULT

CHECK(bool readOptionFile(const String& filename) throw())
	Options o;
	TEST_EQUAL(o.readOptionFile(BALL_TEST_DATA_PATH(OptionsFile1.txt)), true)
	TEST_EQUAL(o.getBool("BOOL"), true)
	TEST_EQUAL(o.getBool("BOOL2"), false)
	TEST_REAL_EQUAL(o.getReal("REAL"), 1.23456)
	TEST_EQUAL(o.getVector("VECTOR"), vector)
	TEST_EQUAL(o.getInteger("INT"), 1234567890)

	Options o2;
	TEST_EQUAL(o2.readOptionFile(BALL_TEST_DATA_PATH(OptionsFile2.txt)), true)
	TEST_EQUAL(o2.get("ABC"), "DEF")
	TEST_EQUAL(o2.getBool("BOOL"), false)
	TEST_EQUAL(o2.get("DEF"), "default")
	TEST_EQUAL(o2.getBool("DEFBOOL"), true)
	TEST_EQUAL(o2.getInteger("DEFINT"), 123456)
	TEST_REAL_EQUAL(o2.getReal("DEFREAL"), 1.234560)
	TEST_EQUAL(o2.getInteger("INT"), 1234567890)
	TEST_REAL_EQUAL(o2.getReal("REAL"), 1.234560)
	Vector3 v(0.0, 1.0, 2.0);
	TEST_EQUAL(o2.getVector("SVECTOR"), v)
	TEST_EQUAL(o2.getVector("VECTOR"), vector)

	//Test an options file with arbitrary deleimiters
	Options o3;
	TEST_EQUAL(o3.readOptionFile(BALL_TEST_DATA_PATH(OptionsFile4.txt)), true)
	TEST_EQUAL(o3.getBool("BOOL"), true)
	TEST_EQUAL(o3.getBool("BOOL2"), false)
	TEST_REAL_EQUAL(o3.getReal("REAL"), 1.23456)
	TEST_EQUAL(o3.getVector("VECTOR"), vector)
	TEST_EQUAL(o3.getInteger("INT"), 1234567890)
RESULT

String filename;
CHECK(bool writeOptionFile(const String& filename) const throw())
	NEW_TMP_FILE(filename)
	options->writeOptionFile(filename);
	TEST_FILE_REGEXP(filename.c_str(), BALL_TEST_DATA_PATH(OptionsFile2.txt))
RESULT


TextPersistenceManager pm;
Options o2;
CHECK(bool read(PersistenceManager& pm) throw())
	ifstream ifile(BALL_TEST_DATA_PATH(OptionsFile3.txt));
	pm.setIstream(ifile);

	bool success = o2.read(pm);
	ifile.close();
	TEST_EQUAL(success, true)
	TEST_EQUAL(o2.get("ABC"), "DEF")
	TEST_EQUAL(o2.getBool("BOOL"), false)
	TEST_EQUAL(o2.get("DEF"), "default")
	TEST_EQUAL(o2.getBool("DEFBOOL"), true)
	TEST_EQUAL(o2.getInteger("DEFINT"), 123456)
	TEST_REAL_EQUAL(o2.getReal("DEFREAL"), 1.234560)
	TEST_EQUAL(o2.getInteger("INT"), 1234567890)
	TEST_REAL_EQUAL(o2.getReal("REAL"), 1.234560)
	Vector3 v(0.0, 1.0, 2.0);
	TEST_EQUAL(o2.getVector("SVECTOR"), v)
	TEST_EQUAL(o2.getVector("VECTOR"), vector)
RESULT

String tmpname;
CHECK(void write(PersistenceManager& pm) const throw())
	NEW_TMP_FILE(tmpname)
	ofstream  ofile(tmpname.c_str(), std::ios::out);
	pm.setOstream(ofile);
	o2.write(pm);
	ofile.close();	
	TEST_FILE_REGEXP(tmpname.c_str(), BALL_TEST_DATA_PATH(OptionsFile3.txt))
RESULT

CHECK(void dump(std::ostream& s = std::cout, Size depth = 0) const throw())
	using std::ofstream;
	using std::ios;
	NEW_TMP_FILE(filename)
	std::ofstream outfile(filename.c_str(), std::ios::out);
	options->dump(outfile);
	outfile.close();
	TEST_FILE_REGEXP(filename.c_str(), BALL_TEST_DATA_PATH(Options_test.txt))
RESULT
delete options;
delete options2;


Options o;
o.setInteger("INT", 1234);
CHECK(String get(const String& key) const throw())
	TEST_EQUAL("1234", o.get("INT"))
	TEST_EQUAL("", o.get(""))
RESULT

CHECK(bool operator != (const Options& option) const throw())
	Options o2;
	TEST_EQUAL(o2 != o, true)
	o2 = o;
	TEST_EQUAL(o2 != o, false)
RESULT

CHECK(bool operator == (const Options& option) const throw())
	Options o2;
	TEST_EQUAL(o2 == o, false)
	o2 = o;
	TEST_EQUAL(o2 == o, true)
RESULT

CHECK(const Options& operator = (const Options& options) throw())
	Options o2(o);
	TEST_EQUAL(o2 == o, true)
RESULT

CHECK(void clear() throw())
	o.clear();
	Options o2;
	TEST_EQUAL(o2 == o, true)
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
