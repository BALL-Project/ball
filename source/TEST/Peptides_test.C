// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: Peptides_test.C,v 1.3.34.1 2007/03/25 21:47:31 oliver Exp $
#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/STRUCTURE/peptides.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/protein.h>
#include <BALL/FORMAT/HINFile.h>

///////////////////////////

START_TEST(Peptides, "$Id: Peptides_test.C,v 1.3.34.1 2007/03/25 21:47:31 oliver Exp $")

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
	ThreeLetterAASequence seq;
	seq.push_back("UNK");
	seq.push_back("ala");
	seq.push_back("ser");
	seq.push_back("GLY");
	TEST_EQUAL(ThreeLetterToOneLetter(seq), "?ASG")
	seq.clear();
	TEST_EQUAL(ThreeLetterToOneLetter(seq), "");
RESULT

CHECK(OneLetterToThreeLetter(const OneLetterAASequence& sequence))
	OneLetterAASequence s("");
	ThreeLetterAASequence seq = OneLetterToThreeLetter("");
	TEST_EQUAL(seq.size(), 0)
	seq = OneLetterToThreeLetter("AGZ?");
	TEST_EQUAL(seq.size(), 4)
	ABORT_IF(seq.size() != 4)
	ThreeLetterAASequence::iterator it = seq.begin();
	TEST_EQUAL(*it, "ALA")
	it++;
	TEST_EQUAL(*it, "GLY")
	it++;
	TEST_EQUAL(*it, "UNK")
	it++;
	TEST_EQUAL(*it, "UNK")
RESULT

CHECK(GetSequence(const Protein& protein))
	HINFile f(BALL_TEST_DATA_PATH(AlaGlySer.hin));
	System S;
	f >> S;
	TEST_EQUAL(S.countResidues(), 3)
	OneLetterAASequence seq = GetSequence(*S.beginProtein());
	TEST_EQUAL(seq, "AGS")
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
