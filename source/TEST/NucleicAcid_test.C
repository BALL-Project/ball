// $Id: NucleicAcid_test.C,v 1.4 2000/05/11 23:10:25 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/KERNEL/nucleicAcid.h>
#include <BALL/KERNEL/nucleotide.h>
///////////////////////////

START_TEST(NucleicAcid, "$Id: NucleicAcid_test.C,v 1.4 2000/05/11 23:10:25 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

NucleicAcid*	r;
CHECK(NucleicAcid())
	r = new NucleicAcid;
	TEST_NOT_EQUAL(r, 0)
RESULT

CHECK(~NucleicAcid())
	delete r;
RESULT

CHECK(NucleicAcid(NucleicAcid&, bool))
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

CHECK(NucleicAcid(const String& name,
			  			const String& id = BALL_NUCLEICACID_DEFAULT_ID))
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

CHECK(NucleicAcid::clear())
	NucleicAcid na("na1", "id");
	Nucleotide a("a");
	na.insert(a);
	na.clear();
	TEST_EQUAL(na.countNucleotides(), 0)
	TEST_EQUAL(na.getID(), "")
RESULT

CHECK(NucleicAcid::destroy())
	NucleicAcid na("na1", "id");
	Nucleotide a("a");
	na.insert(a);
	na.destroy();
	TEST_EQUAL(na.countNucleotides(), 0)
	TEST_EQUAL(na.getID(), "")
RESULT

CHECK(NucleicAcid::set(const NucleicAcid& NucleicAcid, bool deep = true))
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

CHECK(NucleicAcid::NucleicAcid& operator = (const NucleicAcid& NucleicAcid))
	NucleicAcid na1("na1");
	Nucleotide a("a");
	na1.insert(a);
	NucleicAcid na2("na2");
	na2 = na1;
	TEST_EQUAL(na2.getName(), "na1")
	TEST_EQUAL(na2.countNucleotides(), 1)
RESULT

CHECK(NucleicAcid::get(NucleicAcid& NucleicAcid, bool deep = true) const )
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

CHECK(NucleicAcid::swap(NucleicAcid&))
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

CHECK(NucleicAcid::get3Prime())
	NucleicAcid na1("na1");
	Nucleotide n1("n1");
	na1.insert(n1);
	na1.get3Prime()->setName("X");
	TEST_EQUAL(na1.get3Prime()->getName(), "X")
RESULT

CHECK(NucleicAcid::get3Prime() const)
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

CHECK(NucleicAcid::get5Prime())
	NucleicAcid na1("na1");
	Nucleotide n1("n1");
	na1.insert(n1);
	na1.get5Prime()->setName("X");
	TEST_EQUAL(na1.get5Prime()->getName(), "X")
RESULT

CHECK(NucleicAcid::get5Prime() const)
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

CHECK(NucleicAcid::getID() const)
	NucleicAcid na1("na1");
	TEST_EQUAL(na1.getID(), BALL_NUCLEICACID_DEFAULT_ID)
	na1.setID("X");
	TEST_EQUAL(na1.getID(), "X")
RESULT

CHECK(NucleicAcid::setID(const String&))
	NucleicAcid na1("na1");
	na1.setID("");
	TEST_EQUAL(na1.getID(), BALL_NUCLEICACID_DEFAULT_ID)
	na1.setID("X");
	TEST_EQUAL(na1.getID(), "X")
RESULT

CHECK(NucleicAcid::countNucleotides())
	NucleicAcid na1("na1");
	TEST_EQUAL(na1.countNucleotides(), 0)
	Nucleotide n1("n1");
	na1.insert(n1);
	TEST_EQUAL(na1.countNucleotides(), 1)
RESULT

CHECK(NucleicAcid::isValid())
	NucleicAcid na1("");
	TEST_EQUAL(na1.isValid(), true)
	na1.setID("");
	TEST_EQUAL(na1.isValid(), true)
	na1.setID("X");
	TEST_EQUAL(na1.isValid(), true)
RESULT

CHECK(NucleicAcid::dump(ostream&, Size))
	NucleicAcid na1("NA1");
	Nucleotide n1("N1");
	na1.insert(n1);
	String filename;
	NEW_TMP_FILE(filename)
	std::ofstream outfile(filename.c_str(), ios::out);
	na1.dump(outfile);
	outfile.close();
	TEST_FILE(filename.c_str(), "data/NucleicAcid_test.txt", true)
RESULT

CHECK(NucleicAcid::read(istream&))
//BAUSTELLE
RESULT

CHECK(NucleicAcid::write(ostream&) const)
//BAUSTELLE
RESULT

CHECK(NucleicAcid::persistentWrite())
//BAUSTELLE
RESULT

CHECK(NucleicAcid::persistentRead())
//BAUSTELLE
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
