// $Id: NucleicAcid_test.C,v 1.1 1999/12/01 13:24:55 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/KERNEL/nucleicAcid.h>
///////////////////////////

START_TEST(NucleicAcid, "$Id: NucleicAcid_test.C,v 1.1 1999/12/01 13:24:55 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

NucleicAcid* na;
CHECK(NucleicAcid::NucleicAcid())
na = new NucleicAcid;
TEST_NOT_EQUAL(na, 0)
RESULT

CHECK(NucleicAcid::~NucleicAcid())
delete na;
RESULT											

CHECK(NucleicAcid::NucleicAcid(const NucleicAcid&, bool))
//BAUSTELLE
RESULT

CHECK(NucleicAcid::NucleicAcid(constr String&, const String&)
//BAUSTELLE
RESULT

CHECK(NucleicAcid::clear())
//BAUSTELLE
RESULT

CHECK(NucleicAcid::destroy())
//BAUSTELLE
RESULT

CHECK(NucleicAcid::persistentWrite())
//BAUSTELLE
RESULT

CHECK(NucleicAcid::persistentRead())
//BAUSTELLE
RESULT

CHECK(NucleicAcid::set(const NucleicAcid&, bool))
//BAUSTELLE
RESULT

CHECK(NucleicAcid::operator = (const NucleicAcid&))
//BAUSTELLE
RESULT

CHECK(NucleicAcid::get(NucleicAcid&, bool))
//BAUSTELLE
RESULT

CHECK(NucleicAcid::swap(NucleicAcid&))
//BAUSTELLE
RESULT

CHECK(NucleicAcid::get3Prime())
//BAUSTELLE
RESULT

CHECK(NucleicAcid::get3Prime() const)
//BAUSTELLE
RESULT

CHECK(NucleicAcid::get5Prime())
//BAUSTELLE
RESULT

CHECK(NucleicAcid::get5Prime() const)
//BAUSTELLE
RESULT

CHECK(NucleicAcid::getID() const)
//BAUSTELLE
RESULT

CHECK(NucleicAcid::setID(const String&))
//BAUSTELLE
RESULT

CHECK(NucleicAcid::countNucleotides())
//BAUSTELLE
RESULT

CHECK(NucleicAcid::isValid())
//BAUSTELLE
RESULT

CHECK(NucleicAcid::dump(ostream&, unsigned long))
//BAUSTELLE
RESULT

CHECK(NucleicAcid::read(istream&))
//BAUSTELLE
RESULT

CHECK(NucleicAcid::write(ostream&) const)
//BAUSTELLE
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
