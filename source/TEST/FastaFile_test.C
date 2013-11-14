// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>


///////////////////////////

#include <BALL/KERNEL/protein.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/system.h>
#include <BALL/STRUCTURE/peptides.h>
#include <BALL/SEQUENCE/sequence.h>
#include <BALL/FORMAT/fastaFile.h>
#include <iostream>
///////////////////////////

START_TEST(FastaFile)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

FastaFile *fp= new FastaFile();


CHECK(FastaFile())
	TEST_NOT_EQUAL(fp, 0);
RESULT


CHECK(~FastaFile())
	delete fp;
RESULT


CHECK(FastaFile(const BALL::String& filename,BALL::File::OpenMode open_mode=std::ios::in, bool trim_whitespaces=false))
	FastaFile fasta(BALL_TEST_DATA_PATH(FastaFile_test1.fasta));
	TEST_EQUAL(fasta.isValid(), true)
	fasta.close();

	FastaFile fasta2(BALL_TEST_DATA_PATH(FastaFile_test2.fasta));
	TEST_EQUAL(fasta2.isValid(), true)
	fasta2.close();
RESULT

String rs("AGTACGTAGTAGCTGCTGCTACGTGCGCTAGCTAGTACGTCACGACGTAGATGCTAGCTGACTCGATGC");


CHECK( read(Protein& protein) )
	FastaFile fasta;
	fasta.open(BALL_TEST_DATA_PATH(FastaFile_test1.fasta));
	Protein prot;
	fasta.read(prot);
	fasta.close();

	String seq = Peptides::GetSequence(prot);
	TEST_EQUAL(seq, rs);

	fasta.open(BALL_TEST_DATA_PATH(FastaFile_test2.fasta));
	Protein prot2;
	fasta.read(prot2);
	fasta.close();
	TEST_EQUAL(prot2.countChains(), 2)
RESULT


CHECK(read(System& system))
	FastaFile fasta;
	fasta.open(BALL_TEST_DATA_PATH(FastaFile_test1.fasta));
	System system;
	fasta.read(system);
	fasta.close();
	TEST_EQUAL(system.countResidues(), rs.length())
	TEST_EQUAL(system.countProteins(), 1)

	fasta.open(BALL_TEST_DATA_PATH(FastaFile_test2.fasta));
	System system2;
	fasta.read(system2);
	TEST_EQUAL(system2.countChains(), 2)
	TEST_EQUAL(system2.countProteins(), 1)
	fasta.close();

	fasta.open(BALL_TEST_DATA_PATH(FastaFile_test2.fasta));
	fasta.read(system);
	TEST_EQUAL(system.countProteins(), 2)
	fasta.close();
RESULT


CHECK(operator>>(Protein& protein))
	FastaFile fasta(BALL_TEST_DATA_PATH(FastaFile_test1.fasta));
  Protein prot;
	fasta >> prot;
	fasta.close();
  TEST_EQUAL(prot.countResidues(), rs.length())
	TEST_EQUAL(Peptides::GetSequence(prot), rs)
	TEST_EQUAL(prot.countChains(), 1)
RESULT


CHECK(operator>>(System& system))
  System s;
	FastaFile fasta(BALL_TEST_DATA_PATH(FastaFile_test2.fasta));
	fasta >> s;
	TEST_EQUAL(s.countChains(), 2)
RESULT


CHECK(operator == (const FastaFile& f))
	FastaFile fasta1(BALL_TEST_DATA_PATH(FastaFile_test1.fasta));
	FastaFile fasta2(BALL_TEST_DATA_PATH(FastaFile_test1.fasta));
  TEST_EQUAL(fasta1 == fasta2, true)
RESULT


CHECK(operator != (const FastaFile& f))
	FastaFile fasta1(BALL_TEST_DATA_PATH(FastaFile_test1.fasta));
	FastaFile fasta3(BALL_TEST_DATA_PATH(FastaFile_test2.fasta));
	TEST_EQUAL(fasta1 != fasta3, true)
RESULT


String filename;
NEW_TMP_FILE(filename)
CHECK( write(Protein& protein))
	Protein* p = new Protein("TESTPROTEIN AGT");
	Chain* chain = new Chain("TESTCHAIN AGT");

	Residue* res1 = new Residue("ALA",0);
	Residue* res2 = new Residue("GLY",1);
	Residue* res3 = new Residue("THR",2);

	res1->setProperty(Residue::PROPERTY__AMINO_ACID);
	res2->setProperty(Residue::PROPERTY__AMINO_ACID);
	res3->setProperty(Residue::PROPERTY__AMINO_ACID);

	chain->insert(*res1);
	chain->insert(*res2);
	chain->insert(*res3);

	p->insert(*chain);
	TEST_EQUAL(p->countChains(),1)
	TEST_EQUAL(p->countResidues(),3)
	FastaFile f(filename, std::ios::out);
	f.write(*p);
	f.close();
	TEST_FILE(filename.c_str(), BALL_TEST_DATA_PATH(FastaFile_test3.fasta))
RESULT


CHECK( write(System& system))
	System S;
	Protein* p = new Protein("TESTPROTEIN AGT");
	Chain* chain = new Chain("TESTCHAIN AGT");

	Residue* res1 = new Residue("ALA",0);
	Residue* res2 = new Residue("GLY",1);
	Residue* res3 = new Residue("THR",2);

	res1->setProperty(Residue::PROPERTY__AMINO_ACID);
	res2->setProperty(Residue::PROPERTY__AMINO_ACID);
	res3->setProperty(Residue::PROPERTY__AMINO_ACID);

	chain->insert(*res1);
	chain->insert(*res2);
	chain->insert(*res3);

	p->insert(*chain);
	TEST_EQUAL(p->countChains(),1)
	TEST_EQUAL(p->countResidues(),3)
	S.insert(*p);
	FastaFile f(filename, std::ios::out);
	f.write(S);
	f.close();
	TEST_FILE(filename.c_str(), BALL_TEST_DATA_PATH(FastaFile_test3.fasta))
RESULT


CHECK( write(Molelcule& mol))
	Molecule* m = new Protein("TESTPROTEIN AGT");
	Chain* chain = new Chain("TESTCHAIN AGT");

	Residue* res1 = new Residue("ALA",0);
	Residue* res2 = new Residue("GLY",1);
	Residue* res3 = new Residue("THR",2);

	res1->setProperty(Residue::PROPERTY__AMINO_ACID);
	res2->setProperty(Residue::PROPERTY__AMINO_ACID);
	res3->setProperty(Residue::PROPERTY__AMINO_ACID);

	chain->insert(*res1);
	chain->insert(*res2);
	chain->insert(*res3);

	m->insert(*chain);

	//TEST_EQUAL(m->countChains(),1)
	//TEST_EQUAL(m->countResidues(),3)

	FastaFile f(filename, std::ios::out);
	f.write(*m);
	f.close();
	TEST_FILE(filename.c_str(), BALL_TEST_DATA_PATH(FastaFile_test3.fasta))

RESULT

END_TEST
