// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////
#include <BALL/KERNEL/nucleicAcid.h>
#include <BALL/KERNEL/nucleotide.h>
#include <BALL/CONCEPT/textPersistenceManager.h>
///////////////////////////

START_TEST(NucleicAcid)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
String filename;

NucleicAcid*	r;
CHECK(NucleicAcid() throw())
	r = new NucleicAcid;
	TEST_NOT_EQUAL(r, 0)
RESULT

CHECK(~NucleicAcid() throw())
	delete r;
RESULT

CHECK(NucleicAcid(const NucleicAcid& nucleic_acid, bool deep = true) throw())
	NucleicAcid* na1 = new NucleicAcid;
	na1->setName("testname");
	Nucleotide a("a");
	na1->insert(a);
	NucleicAcid* na2 = new NucleicAcid(*na1, true);
	TEST_NOT_EQUAL(na2, 0)
	if (na2 != 0)
	{
		TEST_EQUAL(na2->getName(), "testname")
		TEST_EQUAL(na2->getNucleotide(0)->getName(), "a")
		delete na2;
	}
	na2 = new NucleicAcid(*na1, false);
	TEST_NOT_EQUAL(na2, 0)
	if (na2 != 0)
	{
		TEST_EQUAL(na2->getName(), "testname")
		delete na2;
	}
	delete na1;
RESULT

CHECK(NucleicAcid(const String& name, const String& id = BALL_NUCLEICACID_DEFAULT_ID) throw())
	NucleicAcid* na1 = new NucleicAcid("na1", "id");
	TEST_NOT_EQUAL(na1, 0)
	if (na1 != 0)
	{
		TEST_EQUAL(na1->getName(), "na1")
		TEST_EQUAL(na1->getID(), "id")
		delete na1;
	}
	NucleicAcid* na2 = new NucleicAcid("na1");
	TEST_NOT_EQUAL(na2, 0)
	if (na2 != 0)
	{
		TEST_EQUAL(na2->getName(), "na1")
		TEST_EQUAL(na2->getID(), "")
		delete na2;
	}
RESULT

CHECK(void clear() throw())
	NucleicAcid na("na1", "id");
	Nucleotide a("a");
	na.insert(a);
	na.clear();
	TEST_EQUAL(na.countNucleotides(), 0)
	TEST_EQUAL(na.getID(), "")
RESULT

CHECK(void destroy() throw())
	NucleicAcid na("na1", "id");
	Nucleotide a("a");
	na.insert(a);
	na.destroy();
	TEST_EQUAL(na.countNucleotides(), 0)
	TEST_EQUAL(na.getID(), "")
RESULT

CHECK(void set(const NucleicAcid& nucleic_acid, bool deep = true) throw())
	NucleicAcid na1("na1");
	Nucleotide a("a");
	na1.insert(a);
	NucleicAcid na2("na2");
	na2.set(na1, false);
	TEST_EQUAL(na2.getName(), "na1")
	TEST_EQUAL(na2.countNucleotides(), 0)
	na2.setName("a");
	na2.set(na1);
	TEST_EQUAL(na2.getName(), "na1")
	TEST_EQUAL(na2.countNucleotides(), 1)
RESULT

CHECK(NucleicAcid& operator = (const NucleicAcid& nucleic_acid) throw())
	NucleicAcid na1("na1");
	Nucleotide a("a");
	na1.insert(a);
	NucleicAcid na2("na2");
	na2 = na1;
	TEST_EQUAL(na2.getName(), "na1")
	TEST_EQUAL(na2.countNucleotides(), 1)
RESULT

CHECK(void get(NucleicAcid& nucleic_acid, bool deep = true) const throw())
	NucleicAcid na1("na1");
	Nucleotide a("a");
	na1.insert(a);
	NucleicAcid na2("na2");
	na1.get(na2, false);
	TEST_EQUAL(na2.getName(), "na1")
	TEST_EQUAL(na2.countNucleotides(), 0)
	na2.setName("a");
	na1.get(na2);
	TEST_EQUAL(na2.getName(), "na1")
	TEST_EQUAL(na2.countNucleotides(), 1)
RESULT

CHECK(void swap(NucleicAcid& nucleic_acid) throw())
	NucleicAcid na1("na1");
	NucleicAcid na2("na2");
	Nucleotide n1("n1");
	Nucleotide n2("n2");
	na1.insert(n1);
	na2.insert(n2);
	na1.swap(na2);
	TEST_EQUAL(na1.getName(), "na2")
	TEST_EQUAL(na1.getNucleotide(0), &n2)
	TEST_EQUAL(na2.getName(), "na1")
	TEST_EQUAL(na2.getNucleotide(0), &n1)
RESULT

CHECK(Nucleotide* get3Prime() throw())
	NucleicAcid na1("na1");
	Nucleotide n1("n1");
	na1.insert(n1);
	na1.get3Prime()->setName("X");
	TEST_EQUAL(na1.get3Prime()->getName(), "X")
RESULT

CHECK(const Nucleotide* get3Prime() const throw())
	NucleicAcid na1("na1");
	TEST_EQUAL(na1.get3Prime(), 0)
	Nucleotide n1("n1");
	Nucleotide n2("n2");
	Nucleotide n3("n3");
	na1.insert(n1);
	TEST_EQUAL(na1.get3Prime(), &n1)
	na1.insert(n2);
	TEST_EQUAL(na1.get3Prime(), &n1)
	na1.prepend(n3);
	TEST_EQUAL(na1.get3Prime(), &n3)
RESULT

CHECK(Nucleotide* get5Prime() throw())
	NucleicAcid na1("na1");
	Nucleotide n1("n1");
	na1.insert(n1);
	na1.get5Prime()->setName("X");
	TEST_EQUAL(na1.get5Prime()->getName(), "X")
RESULT

CHECK(const Nucleotide* get5Prime() const throw())
	NucleicAcid na1("na1");
	TEST_EQUAL(na1.get5Prime(), 0)
	Nucleotide n1("n1");
	Nucleotide n2("n2");
	Nucleotide n3("n3");
	na1.insert(n1);
	TEST_EQUAL(na1.get5Prime(), &n1)
	na1.prepend(n2);
	TEST_EQUAL(na1.get5Prime(), &n1)
	na1.insert(n3);
	TEST_EQUAL(na1.get5Prime(), &n3)
RESULT

CHECK(const String& getID() const throw())
	NucleicAcid na1("na1");
	TEST_EQUAL(na1.getID(), BALL_NUCLEICACID_DEFAULT_ID)
	na1.setID("X");
	TEST_EQUAL(na1.getID(), "X")
RESULT

CHECK(void setID(const String& id) throw())
	NucleicAcid na1("na1");
	na1.setID("");
	TEST_EQUAL(na1.getID(), BALL_NUCLEICACID_DEFAULT_ID)
	na1.setID("X");
	TEST_EQUAL(na1.getID(), "X")
RESULT

CHECK(Size countNucleotides() const throw())
	NucleicAcid na1("na1");
	TEST_EQUAL(na1.countNucleotides(), 0)
	Nucleotide n1("n1");
	na1.insert(n1);
	TEST_EQUAL(na1.countNucleotides(), 1)
RESULT

CHECK([EXTRA]NucleicAcid::isValid())
	NucleicAcid na1("");
	TEST_EQUAL(na1.isValid(), true)
	na1.setID("");
	TEST_EQUAL(na1.isValid(), true)
	na1.setID("X");
	TEST_EQUAL(na1.isValid(), true)
RESULT

CHECK([EXTRA]NucleicAcid::dump(ostream&, Size))
	NucleicAcid na1("NA1");
	Nucleotide n1("N1");
	na1.insert(n1);
	NEW_TMP_FILE(filename)
	std::ofstream outfile(filename.c_str(), std::ios::out);
	na1.dump(outfile);
	outfile.close();
	TEST_FILE_REGEXP(filename.c_str(), BALL_TEST_DATA_PATH(NucleicAcid_test.txt))
RESULT

TextPersistenceManager pm;
using namespace RTTI;
NEW_TMP_FILE(filename)
CHECK(void persistentWrite(PersistenceManager& pm, const char* name = 0) const throw(Exception::GeneralException))
	std::ofstream	ofile(filename.c_str(), std::ios::out);
	NucleicAcid* f1 = new NucleicAcid("name1");
	Nucleotide* f2= new Nucleotide("name2");
	Nucleotide* f3= new Nucleotide("name3");
	f1->insert(*f2);
	f1->insert(*f3);
	pm.setOstream(ofile);
	*f1 >> pm;
	ofile.close();
	delete f1;
RESULT

CHECK(void persistentRead(PersistenceManager& pm) throw(Exception::GeneralException))
	std::ifstream	ifile(filename.c_str());
	pm.setIstream(ifile);
	PersistentObject*	ptr = pm.readObject();
	TEST_NOT_EQUAL(ptr, 0)
	if (ptr != 0)
	{
        TEST_EQUAL(isKindOf<NucleicAcid>(ptr), true)
		NucleicAcid* f1 = castTo<NucleicAcid>(*ptr);
		TEST_EQUAL(f1->getName(), "name1")
		TEST_EQUAL(f1->countNucleotides(), 2)
		TEST_EQUAL(f1->getNucleotide(0)->getName(), "name2")
		TEST_EQUAL(f1->getNucleotide(1)->getName(), "name3")
		delete f1;
	} 
	else 
	{
		throw Exception::NullPointer(__FILE__, __LINE__);
	}
RESULT

CHECK(bool operator == (const NucleicAcid& nucleic_acid) const throw())
	NucleicAcid b1;
	NucleicAcid b2;
	TEST_EQUAL(b1 == b2, false)
	b1 = b2;
	TEST_EQUAL(b1 == b1, true)
RESULT

CHECK(bool operator != (const NucleicAcid& nucleic_acid) const throw())
	NucleicAcid b1;
	NucleicAcid b2;
	TEST_EQUAL(b1 != b2, true)
	b1 = b2;
	TEST_EQUAL(b1 != b1, false)
RESULT

NucleicAcid na1("na1");
Nucleotide n1("n1");
Nucleotide n2("n2");
na1.insert(n1);
na1.insert(n2);

CHECK(BALL_CREATE_DEEP(NucleicAcid))
	NucleicAcid* na2 = (NucleicAcid*) na1.create(false, true);
	TEST_EQUAL(na2->getName(), "")
	TEST_EQUAL(na2->countNucleotides(), 0)
	delete na2;
	na2 = (NucleicAcid*) na1.create(true, false);
	TEST_EQUAL(na2->getName(), "na1")
	TEST_EQUAL(na2->countNucleotides(), 2)
	delete na2;
RESULT

CHECK(Nucleotide* getNucleotide(Position position) throw())
	na1.getNucleotide(0)->setName("na1");
	na1.getNucleotide(1)->setName("na2");
	TEST_EQUAL(n1.getName(), "na1")
	TEST_EQUAL(n2.getName(), "na2")
RESULT

CHECK(const Nucleotide* getNucleotide(Position position) const throw())
	const NucleicAcid& na = na1;
	TEST_EQUAL(na.getNucleotide(0)->getName(), "na1")
	TEST_EQUAL(na.getNucleotide(1)->getName(), "na2")
	TEST_EQUAL(na.getNucleotide(2), 0)
RESULT

CHECK([EXTRA] iterators)
	NucleicAcid na;
	Nucleotide n1,n2;
	na.insert(n1);
	na.insert(n2);

	TEST_EQUAL(&*na.beginNucleotide(), &n1)
	TEST_EQUAL(&*--na.endNucleotide(), &n2)
	TEST_EQUAL(&*na.rbeginNucleotide(), &n2)	
RESULT
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
