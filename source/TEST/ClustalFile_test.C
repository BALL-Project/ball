// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: ClustalFile_test.C,2011/06/07$
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

START_TEST(ClustalFile, "$Id: ClustalFile_test.C,2011/06/07$")

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

	TEST_EQUAL(line.ident , "seq1");

RESULT


//TODO corrupted File read ->Exception?
/*bool read();


ClustalFile& operator >> (System& system);

ClustalFile& operator >>(Alignment& alignment);

ClustalFile& operator << (System& system);

ClustalFile& operator << (Alignment& alignment);

*/

//////////////////////////////////////////////////////////////////// Reading Tests //////////////////////////////////////////////////77

	
	//Molecule *mp;
/*	System *sp;
		
CHECK(operator>> (ClustalFile file, System& system))
	*fp >> *sp;
	
	//TODO evaluate number
  TEST_EQUAL(sp->countProteins(), 3);
RESULT


	delete sp;


	//instantiate new ones

         System *ns;

  */      
/*
CHECK(read(System& system))
	fp->read(ns);
         
        TEST_EQUAL(ns->countDescendants(),3);
RESULT          
*/








/** TODO test the write function
                void write(Protein& protein);
                        void write(Molecule& molecule);
                        void write(System& system);
*/
                ////////////////////////////////////////////////// Misc ////////////////////////////////////////////////////

						

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
