// $Id: Peptides_test.C,v 1.1.2.1 2002/05/14 23:28:15 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/STRUCTURE/peptides.h>

///////////////////////////

START_TEST(Peptides, "$Id: Peptides_test.C,v 1.1.2.1 2002/05/14 23:28:15 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace Peptides;

CHECK(OneLetterCode(const String& aa))
	TEST_EQUAL(OneLetterCode("ALAALA"), '?')
	TEST_EQUAL(OneLetterCode(""), '?')
	TEST_EQUAL(OneLetterCode("alalys"), '?')
	TEST_EQUAL(OneLetterCode("ala@arg@"), '?')
	TEST_EQUAL(OneLetterCode("A@A"), '?')

	TEST_EQUAL(OneLetterCode("ala"), 'A')
	TEST_EQUAL(OneLetterCode("ALA"), 'A')
	TEST_EQUAL(OneLetterCode("Ala"), 'A')
	TEST_EQUAL(OneLetterCode("ARG"), 'R')
	TEST_EQUAL(OneLetterCode("ASN"), 'N')
	TEST_EQUAL(OneLetterCode("ASP"), 'D')
	TEST_EQUAL(OneLetterCode("CYS"), 'C')
	TEST_EQUAL(OneLetterCode("GLN"), 'Q')
	TEST_EQUAL(OneLetterCode("GLU"), 'E')
	TEST_EQUAL(OneLetterCode("GLY"), 'G')
	TEST_EQUAL(OneLetterCode("HIS"), 'H')
	TEST_EQUAL(OneLetterCode("ILE"), 'I')
	TEST_EQUAL(OneLetterCode("LEU"), 'L')
	TEST_EQUAL(OneLetterCode("LYS"), 'K')
	TEST_EQUAL(OneLetterCode("MET"), 'M')
	TEST_EQUAL(OneLetterCode("PHE"), 'F')
	TEST_EQUAL(OneLetterCode("PRO"), 'P')
	TEST_EQUAL(OneLetterCode("SER"), 'S')
	TEST_EQUAL(OneLetterCode("THR"), 'T')
	TEST_EQUAL(OneLetterCode("TRP"), 'W')
	TEST_EQUAL(OneLetterCode("TYR"), 'Y')
	TEST_EQUAL(OneLetterCode("VAL"), 'V')
RESULT											

CHECK(ThreeLetterCode(char aa))
	TEST_EQUAL(ThreeLetterCode('b'), "UNK")
	TEST_EQUAL(ThreeLetterCode('?'), "UNK")
	TEST_EQUAL(ThreeLetterCode('a'), "ALA")
	TEST_EQUAL(ThreeLetterCode('A'), "ALA")
	TEST_EQUAL(ThreeLetterCode('C'), "CYS")
	TEST_EQUAL(ThreeLetterCode('D'), "ASP")
	TEST_EQUAL(ThreeLetterCode('E'), "GLU")
	TEST_EQUAL(ThreeLetterCode('F'), "PHE")
	TEST_EQUAL(ThreeLetterCode('G'), "GLY")
	TEST_EQUAL(ThreeLetterCode('H'), "HIS")
	TEST_EQUAL(ThreeLetterCode('I'), "ILE")
	TEST_EQUAL(ThreeLetterCode('K'), "LYS")
	TEST_EQUAL(ThreeLetterCode('L'), "LEU")
	TEST_EQUAL(ThreeLetterCode('M'), "MET")
	TEST_EQUAL(ThreeLetterCode('N'), "ASN")
	TEST_EQUAL(ThreeLetterCode('P'), "PRO")
	TEST_EQUAL(ThreeLetterCode('Q'), "GLN")
	TEST_EQUAL(ThreeLetterCode('R'), "ARG")
	TEST_EQUAL(ThreeLetterCode('S'), "SER")
	TEST_EQUAL(ThreeLetterCode('T'), "THR")
	TEST_EQUAL(ThreeLetterCode('V'), "VAL")
	TEST_EQUAL(ThreeLetterCode('W'), "TRP")
	TEST_EQUAL(ThreeLetterCode('Y'), "TYR")
RESULT

CHECK(ThreeLetterToOneLetter(const ThreeLetterAASequence& sequence))
	// ???
RESULT

CHECK(OneLetterToThreeLetter(const OneLetterAASequence& sequence))
	// ???
RESULT

CHECK(GetSequence(const Protein& protein))
	// ???
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
