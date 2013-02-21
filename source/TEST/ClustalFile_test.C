// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Author:
//   Nikola Koch
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/KERNEL/protein.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/system.h>
#include <BALL/STRUCTURE/peptides.h>
#include <BALL/SEQUENCE/sequence.h>
#include <BALL/FORMAT/ClustalFile.h>

///////////////////////////

START_TEST(ClustalFile)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

ClustalFile *fp= new ClustalFile();

//////////////////////////////////////// Constructor and Deconstructor Test ///////////////////////////////////////////////////

CHECK(ClustalFile())
		TEST_NOT_EQUAL(fp,0);
RESULT


CHECK(~ClustalFile())
	delete fp;
RESULT

ClustalFile cp;


//Checks whether Exception is thrown if filename is invalid
CHECK(ClustalFile(const BALL::String& filename,BALL::File::OpenMode open_mode=std::ios::in, bool trim_whitespaces=false))

		TEST_EXCEPTION(Exception::FileNotFound, cp= *(new ClustalFile("x")))
		TEST_EXCEPTION(Exception::FileNotFound, cp= *(new ClustalFile("abscdf")))

RESULT

/////////////////////////////// Block - TESTS ////////////////////////////////////////

CHECK(addSequenceLine(SequenceLine&))

	ClustalFile::Block block;
	
	ClustalFile::SequenceLine line;
	
	line.ident="Line 1";
	line.sequence = "AAA";
	line.length = 3;

	block.addSequenceLine(line);
	
	TEST_EQUAL(block.getSequenceLine(0).ident, line.ident);

RESULT


CHECK(addBlock(const Block& block))

	ClustalFile::Block* block = new ClustalFile::Block();
	
	cp.addBlock(*block);
	TEST_EQUAL(cp.getBlocks().size(),1)

RESULT

 cp.clear();


CHECK(hasValidBlocks())

	ClustalFile::Block block;
	cp.addBlock(block);
	TEST_EQUAL(cp.hasValidBlocks(),true);

RESULT

cp.clear();

CHECK(hasValidBlocks())

	ClustalFile::Block block;
	ClustalFile::SequenceLine line;
	line.ident="Line 1";
	line.sequence = "AAA";
	line.length = 3;

	block.addSequenceLine(line);
	cp.addBlock(block);

	ClustalFile::Block block2;

	ClustalFile::SequenceLine line2;
	line2.ident="Line 1";
	line2.sequence="GGG";
	line2.length = 6;
	block2.addSequenceLine(line2);
	
	cp.addBlock(block2);

	TEST_EQUAL(cp.hasValidBlocks(),true);

RESULT


	cp.clear();

CHECK(hasValidBlocks())

	ClustalFile::Block block;

	ClustalFile::SequenceLine line;
	line.ident="Line 1";
	line.sequence = "AAA";
	line.length = 4;

	block.addSequenceLine(line);

	cp.addBlock(block);

	TEST_EQUAL(cp.hasValidBlocks(),false);

RESULT

cp.clear();


CHECK(hasValidBlocks())

	ClustalFile::Block block;

	ClustalFile::SequenceLine line;
	line.ident="Line 1";
	line.sequence = "AAA";
	line.length = 0;

	block.addSequenceLine(line);
	cp.addBlock(block);

	TEST_EQUAL(cp.hasValidBlocks(),false);

RESULT

cp.clear();

CHECK(hasValidBlocks())

	ClustalFile::Block block;

	ClustalFile::SequenceLine line;
	line.ident="Line 1";
	line.sequence = "AAA";
	line.length = 3;
	block.addSequenceLine(line);
	cp.addBlock(block);

	ClustalFile::Block block2;

	ClustalFile::SequenceLine line2;
	line2.ident="Line 1";
	line2.sequence = "C-C";
	line2.length = 6;

	block2.addSequenceLine(line2);
	cp.addBlock(block2);

//cp.dump();

	TEST_EQUAL(cp.hasValidBlocks(),true);


RESULT

cp.clear();

CHECK(hasValidBlocks())

	ClustalFile::Block block;

	ClustalFile::SequenceLine line;
	line.ident="Line 1";
	line.sequence = "AAA";
	line.length = 3;
	block.addSequenceLine(line);
	cp.addBlock(block);


	ClustalFile::Block block2;
	cp.addBlock(block2);

	ClustalFile::Block block3;

	ClustalFile::SequenceLine line2;
	line2.ident="Line 1";
	line2.sequence = "C-C";
	line2.length = 6;

	block3.addSequenceLine(line2);
	cp.addBlock(block2);

//cp.dump();

	TEST_EQUAL(cp.hasValidBlocks(),false);


RESULT


cp.clear();
	
cp.open(BALL_TEST_DATA_PATH(ClustalFile_test.aln));

CHECK(bool read())

	cp.read();

	TEST_EQUAL(cp.getBlocks().size(), 2);

	ClustalFile::Block block = cp.getBlocks().at(0);
	ClustalFile::SequenceLine line= block.getSequenceLine(0);
	ClustalFile::SequenceLine line2 = block.getSequenceLine(1);
	ClustalFile::SequenceLine line3 = block.getSequenceLine(2);

	TEST_EQUAL(line.ident , "seq1")
	TEST_EQUAL(line.sequence, "-----------------------KSKERYKDENGGNYFQLREDWWDANRETVWKAITCNA")
	TEST_EQUAL(line.length, 60)

	TEST_EQUAL(line2.ident, "seq2")
	TEST_EQUAL(line2.sequence, "----------------YEGLTTANGKEYYQDKNGGNFFKLREDWWTANRETVWKAITCGA")
	TEST_EQUAL(line2.length, 60)

	TEST_EQUAL(line3.ident, "seq3")
	TEST_EQUAL(line3.sequence, "----KRIYKKIFKEIHSGLSTKNGVKDRYQN-DGDNYFQLREDWWTANRSTVWKALTCSD")
	TEST_EQUAL(line3.length, 60)

	block = cp.getBlocks().at(1);
	line= block.getSequenceLine(0);
	line2 = block.getSequenceLine(1);
	line3 = block.getSequenceLine(2);

	TEST_EQUAL(line.ident , "seq1")
	TEST_EQUAL(line.sequence, "--GGGKYFRNTCDG--GQNPTETQNNCRCIG----------ATVPTYFDYVPQYLRWSDE")
	TEST_EQUAL(line.length, 120)

	TEST_EQUAL(line2.ident, "seq2")
	TEST_EQUAL(line2.sequence, "P-GDASYFHATCDSGDGRGGAQAPHKCRCDG---------ANVVPTYFDYVPQFLRWPEE")
	TEST_EQUAL(line2.length, 120)

	TEST_EQUAL(line3.ident, "seq3")
	TEST_EQUAL(line3.sequence, "KLSNASYFRATC--SDGQSGAQANNYCRCNGDKPDDDKP-NTDPPTYFDYVPQYLRWSEE")
	TEST_EQUAL(line3.length,120)


RESULT

CHECK(operator >>(System& system))

	BALL::System s;
	cp>>s;
	TEST_EQUAL(s.countProteins(), 3)
	TEST_EQUAL(s.countResidues(), 289)

RESULT

cp.clear();

CHECK(operator << (Alignment& alignment))

	Alignment al;

	Sequence seq;
	Protein *p1 = new Protein();

	Residue* r1 = new Residue("ALA");
	p1->insert(*r1);
	Residue* r2 = new Residue ("GLY");
	p1->insert(*r2);

	seq.setOrigin(p1);
	seq.setName("blib");
	al.addSequence(seq);

	Sequence seq2;
	seq2.setName("blub");
	Protein *p2 = new Protein();

	Residue* r3 = new Residue("GLY");

	Residue* r4 = new Residue("GLY");

	p2->insert(*r3);
	p2->insert(*r4);

	seq2.setOrigin(p2);
	al.addSequence(seq2);

	al.insertGap(0,2);
	al.setAligned(true);
	cp << al;

	ClustalFile::SequenceLine line1 = cp.getBlocks().at(0).getSequenceLine(0);
	ClustalFile::SequenceLine line2 = cp.getBlocks().at(0).getSequenceLine(1);

	TEST_EQUAL(cp.getBlocks().size(),1)
	TEST_EQUAL(cp.getBlocks().at(0).seqs.size(),2)

	TEST_EQUAL(line1.ident, "blib")
	TEST_EQUAL(line1.sequence , "AG-")
	TEST_EQUAL(line1.length, 3)

	TEST_EQUAL(line2.ident, "blub")
	TEST_EQUAL(line2.sequence, "GG-")
	TEST_EQUAL(line2.length, 3)

RESULT

//TODO Check mehrere Blöcke

END_TEST
