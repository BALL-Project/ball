// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/STRUCTURE/peptides.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/protein.h>
#include <BALL/FORMAT/HINFile.h>

///////////////////////////

START_TEST(Peptides)

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
 	
CHECK(IsOneLetterCode(char aa))
	TEST_EQUAL(IsOneLetterCode('b'), false)
	TEST_EQUAL(IsOneLetterCode('?'), false)
	TEST_EQUAL(IsOneLetterCode('a'), true)
	TEST_EQUAL(IsOneLetterCode('A'), true)
	TEST_EQUAL(IsOneLetterCode('C'), true)
	TEST_EQUAL(IsOneLetterCode('D'), true)
	TEST_EQUAL(IsOneLetterCode('E'), true)
	TEST_EQUAL(IsOneLetterCode('F'), true)
	TEST_EQUAL(IsOneLetterCode('G'), true)
	TEST_EQUAL(IsOneLetterCode('H'), true)
	TEST_EQUAL(IsOneLetterCode('I'), true)
	TEST_EQUAL(IsOneLetterCode('K'), true)
	TEST_EQUAL(IsOneLetterCode('L'), true)
	TEST_EQUAL(IsOneLetterCode('M'), true)
	TEST_EQUAL(IsOneLetterCode('N'), true)
	TEST_EQUAL(IsOneLetterCode('P'), true)
	TEST_EQUAL(IsOneLetterCode('Q'), true)
	TEST_EQUAL(IsOneLetterCode('R'), true)
	TEST_EQUAL(IsOneLetterCode('S'), true)
	TEST_EQUAL(IsOneLetterCode('T'), true)
	TEST_EQUAL(IsOneLetterCode('V'), true)
	TEST_EQUAL(IsOneLetterCode('W'), true)
	TEST_EQUAL(IsOneLetterCode('Y'), true)
RESULT
 		
CHECK(IsThreeLetterCode(const String& aa))
	TEST_EQUAL(IsThreeLetterCode("ALAALA"), false)
	TEST_EQUAL(IsThreeLetterCode(""), false)
	TEST_EQUAL(IsThreeLetterCode("alalys"), false)
	TEST_EQUAL(IsThreeLetterCode("ala@arg@"), false)
	TEST_EQUAL(IsThreeLetterCode("A@A"), false)

	TEST_EQUAL(IsThreeLetterCode("ala"), true)
	TEST_EQUAL(IsThreeLetterCode("ALA"), true)
	TEST_EQUAL(IsThreeLetterCode("Ala"), true)
	TEST_EQUAL(IsThreeLetterCode("ARG"), true)
	TEST_EQUAL(IsThreeLetterCode("ASN"), true)
	TEST_EQUAL(IsThreeLetterCode("ASP"), true)
	TEST_EQUAL(IsThreeLetterCode("CYS"), true)
	TEST_EQUAL(IsThreeLetterCode("GLN"), true)
	TEST_EQUAL(IsThreeLetterCode("GLU"), true)
	TEST_EQUAL(IsThreeLetterCode("GLY"), true)
	TEST_EQUAL(IsThreeLetterCode("HIS"), true)
	TEST_EQUAL(IsThreeLetterCode("his"), true)
	TEST_EQUAL(IsThreeLetterCode("ILE"), true)
	TEST_EQUAL(IsThreeLetterCode("LEU"), true)
	TEST_EQUAL(IsThreeLetterCode("LYS"), true)
	TEST_EQUAL(IsThreeLetterCode("MET"), true)
	TEST_EQUAL(IsThreeLetterCode("PHE"), true)
	TEST_EQUAL(IsThreeLetterCode("PRO"), true)
	TEST_EQUAL(IsThreeLetterCode("SER"), true)
	TEST_EQUAL(IsThreeLetterCode("THR"), true)
	TEST_EQUAL(IsThreeLetterCode("TRP"), true)
	TEST_EQUAL(IsThreeLetterCode("TYR"), true)
	TEST_EQUAL(IsThreeLetterCode("VAL"), true)
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

CHECK(GetSequence(const Chain& chain))
	HINFile f(BALL_TEST_DATA_PATH(AlaGlySer__CysHisMet.hin));
	System S;
	f >> S;
	TEST_EQUAL(S.countResidues(), 6)
	ChainIterator chain_it = S.beginChain();
	OneLetterAASequence seq = GetSequence(*chain_it);
	TEST_EQUAL(seq, "AGS")
	chain_it++;
	seq = GetSequence(*chain_it);
	TEST_EQUAL(seq, "CHM")
RESULT


////////////////////////////////////////////////
//         class NameConverter
////////////////////////////////////////////////

NameConverter* converter_p;

CHECK(NameConverter())
	converter_p = new NameConverter();	
	TEST_NOT_EQUAL(converter_p, 0)
RESULT
			
CHECK(~NameConverter())
	delete converter_p;
RESULT
			
CHECK(setDefaultOptions())
	NameConverter converter;
	converter.setDefaultOptions();
	TEST_EQUAL(converter.options.get(NameConverter::Option::INIFile), NameConverter::Default::INIFile)		
	TEST_EQUAL(converter.options.get(NameConverter::Option::PSEUDO_ATOMS_INIFile), NameConverter::Default::PSEUDO_ATOMS_INIFile)		
RESULT

CHECK(supportsNamingScheme(const String& scheme_name) const)	
	NameConverter converter;
	TEST_EQUAL(converter.supportsNamingScheme("BMRB"), true)
	TEST_EQUAL(converter.supportsNamingScheme("bmrb"), true)
	TEST_EQUAL(converter.supportsNamingScheme("SC"), true)
	TEST_EQUAL(converter.supportsNamingScheme("PDB"), true)
	TEST_EQUAL(converter.supportsNamingScheme("UCSF"), true)
	TEST_EQUAL(converter.supportsNamingScheme("MSI"), true)
	TEST_EQUAL(converter.supportsNamingScheme("xplor"), true)
	TEST_EQUAL(converter.supportsNamingScheme("SYBYL"), true)
	TEST_EQUAL(converter.supportsNamingScheme("MIDAS"), true)
	TEST_EQUAL(converter.supportsNamingScheme("DIANA"), true)
	TEST_EQUAL(converter.supportsNamingScheme("NMRSTAR"), true)
RESULT

CHECK(supportsPseudoAtomNamingScheme(const String& scheme_name) const)	
	NameConverter converter;
	TEST_EQUAL(converter.supportsPseudoAtomNamingScheme("BMRB"), true)
	TEST_EQUAL(converter.supportsPseudoAtomNamingScheme("bmrb"), true)
	TEST_EQUAL(converter.supportsPseudoAtomNamingScheme("UCSF"), true)
	TEST_EQUAL(converter.supportsPseudoAtomNamingScheme("xplor"), true)
	TEST_EQUAL(converter.supportsPseudoAtomNamingScheme("NMRSTAR"), true)
	TEST_EQUAL(converter.supportsPseudoAtomNamingScheme("PDB"), true)
RESULT

CHECK(convertName(const String& amino_acid, const String& old_atom_name, const String& old_naming_scheme, const String& new_naming_scheme) const)
	NameConverter converter;
	TEST_EQUAL(converter.supportsNamingScheme("BMRB"), true)
	TEST_EQUAL(converter.supportsNamingScheme("PDB"), true)
	if (converter.supportsNamingScheme("BMRB") && converter.supportsNamingScheme("PDB") )
	{
		TEST_EQUAL(converter.convertName("LYS", "HB2", "BMRB", "PDB"), "1HB")
		TEST_EQUAL(converter.convertName("K", "HB2", "BMRB", "PDB"), "1HB")
		TEST_EQUAL(converter.convertName("k", "1hb", "PDB", "BMRB"), "1hb")
	}
RESULT


CHECK(resolvePseudoAtoms(const String& amino_acid, const String& atom_name, 
			                   const String& old_naming_scheme, const String& new_naming_scheme) const)
	NameConverter converter;
	std::vector<String> true_result1(1);
	true_result1[0] = "X";

	std::vector<String> result1 = converter.resolvePseudoAtoms("A", "X", "BMRB", "UCSF");

	TEST_EQUAL(result1.size(), true_result1.size())
	for (Position i=0; i<result1.size(); ++i)
		TEST_EQUAL(result1[i], true_result1[i])

	std::vector<String> true_result2(2);
	true_result2[0] = "1HB";
	true_result2[1] = "2HB";

	std::vector<String> result2 = converter.resolvePseudoAtoms("R", "HB", "NMRSTAR", "PDB");

	TEST_EQUAL(result2.size(), true_result2.size())
	for (Position i=0; i<result2.size(); ++i)
		TEST_EQUAL(result2[i], true_result2[i])
RESULT



CHECK(matches(const String& amino_acid, const String& old_atom_name,
											 const String& old_naming_scheme, const String& new_atom_name,
				               const String& new_naming_scheme) const)

	NameConverter converter;	
	TEST_EQUAL(converter.supportsNamingScheme("NMRSTAR"), true)
	TEST_EQUAL(converter.supportsNamingScheme("BMRB"), true)
	TEST_EQUAL(converter.supportsNamingScheme("PDB"), true)

	if (   converter.supportsNamingScheme("BMRB") && converter.supportsNamingScheme("PDB")
			&& converter.supportsNamingScheme("NMRSTAR") )
	{
		TEST_EQUAL(converter.matches("LYS", "HB2", "BMRB", "1HB", "PDB"), true)
		TEST_EQUAL(converter.matches("K",   "HB2", "BMRB", "1HB", "PDB"), true)
		TEST_EQUAL(converter.matches("k",   "1hb", "PDB",  "HB2", "BMRB"), false)

		TEST_EQUAL(converter.matches("T", "1HG2", "PDB",     "HG2", "NMRSTAR"), true)
		TEST_EQUAL(converter.matches("A", "HB",   "NMRSTAR", "1HB", "PDB"), true)
	}

RESULT





/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
