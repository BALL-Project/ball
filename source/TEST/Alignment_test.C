// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//
// Author:
//   Nikola Koch
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>
///////////////////////////

#include <BALL/SEQUENCE/alignment.h>
#include <BALL/SEQUENCE/sequence.h>
///////////////////////////


START_TEST(Alignment)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;


Alignment *alp;

CHECK(Alignment())
	alp= new Alignment();
	TEST_NOT_EQUAL(alp,0)
RESULT

CHECK(~Alignment())
	delete alp;
RESULT

	Alignment al;

CHECK(void addSequence(Sequence& seq))
	Sequence seq;

	Protein *p1 = new Protein();

	Residue* r1 = new Residue("ALA");
	Residue* r2 = new Residue ("GLY");

	p1->insert(*r1);
	p1->insert(*r2);
	seq.setOrigin(p1);
	al.addSequence(seq);

	SequenceCharacter *tmp = new SequenceCharacter('A', &seq, SequenceCharacter::type::CHAR, r1);
	TEST_EQUAL(al.getSequence(0)== seq, true);
	TEST_EQUAL(al.getSeqChar(0,0) == *tmp, true)

	Sequence seq2;

	Protein *p2 = new Protein();

	Residue* r3 = new Residue("GLY");
	p2->insert(*r3);

	seq2.setOrigin(p2);
	al.addSequence(seq2);

	TEST_EQUAL(al.getSequence(1) == seq2, true)

	SequenceCharacter *tmp2 = new SequenceCharacter('-', &seq2, SequenceCharacter::type::GAP, 0);

	TEST_EQUAL(al.getSeqChar(1,1) == *tmp2, true)

RESULT

al.reset();


CHECK(void reset())

	Sequence seq;
	Protein *p1 = new Protein();

	Residue* r1 = new Residue("ALA");
	p1->insert(*r1);

	seq.setOrigin(p1);
	al.addSequence(seq);

	al.reset();

	TEST_EQUAL( (al.cols() == 0) && (al.rows() == 0), true)
	TEST_EQUAL( al.isAligned(), false)
	TEST_EQUAL( al.getScore() , 0)

RESULT

al.reset();



CHECK(bool insertGap(Sequence& seq, int pos))

	Sequence seq;
	Protein *p1 = new Protein();

	Residue* r1 = new Residue("ALA");
	p1->insert(*r1);
	Residue* r2 = new Residue ("GLY");
	p1->insert(*r2);

	seq.setOrigin(p1);
	al.addSequence(seq);

	Sequence seq2;

	Protein *p2 = new Protein();
	Residue* r3 = new Residue("GLY");
	Residue* r4 = new Residue("GLY");

	p2->insert(*r3);
	p2->insert(*r4);

	seq2.setOrigin(p2);
	al.addSequence(seq2);
	al.insertGap(seq,2);

	TEST_EQUAL(al.getSeqChar(0,2).isGap(), true)
	TEST_EQUAL(al.getSeqChar(1,2).isGap(), true)
	TEST_EQUAL(al.cols(), 3)

	al.insertGap(seq, 1);

	TEST_EQUAL(al.getSeqChar(0,1).isGap() ,true)
	TEST_EQUAL(al.getSeqChar(0,0).getChar(),'A')
	TEST_EQUAL(al.getSeqChar(0,2).getChar(), 'G')
	TEST_EQUAL(al.cols(), 3)

	TEST_EQUAL(al.getSeqChar(1,0).getChar(), 'G')
	TEST_EQUAL(al.getSeqChar(1,1).getChar(), 'G')
	TEST_EQUAL(al.getSeqChar(1,2).isGap(), true)

	al.insertGap(seq,0);
	TEST_EQUAL(al.cols(), 4)
	TEST_EQUAL(al.getSeqChar(0,0).isGap() ,true)
	TEST_EQUAL(al.getSeqChar(0,1).getChar(), 'A')
	TEST_EQUAL(al.getSeqChar(0,2).isGap(), true)
	TEST_EQUAL(al.getSeqChar(0,3).getChar(), 'G')

	TEST_EQUAL(al.getSeqChar(1,0).getChar(), 'G')
	TEST_EQUAL(al.getSeqChar(1,1).getChar(), 'G')
	TEST_EQUAL(al.getSeqChar(1,2).isGap(), true)
	TEST_EQUAL(al.getSeqChar(1,3).isGap(),true)

RESULT

al.reset();


CHECK(insertGap(int row, int column))

	Sequence seq;
	Protein *p1 = new Protein();

	Residue* r1 = new Residue("ALA");
	p1->insert(*r1);
	Residue* r2 = new Residue ("GLY");
	p1->insert(*r2);

	seq.setOrigin(p1);
	al.addSequence(seq);

	Sequence seq2;

	Protein *p2 = new Protein();
	Residue* r3 = new Residue("GLY");
	Residue* r4 = new Residue("GLY");

	p2->insert(*r3);
	p2->insert(*r4);

	seq2.setOrigin(p2);
	al.addSequence(seq2);

	TEST_EQUAL(al.insertGap(0,2), true)

	TEST_EQUAL(al.getSeqChar(0,2).isGap(), true)
	TEST_EQUAL(al.cols(), 3)
	TEST_EQUAL(al.getSeqChar(1,2).isGap(), true)

	TEST_EQUAL(al.insertGap(0, 1), true)

	TEST_EQUAL(al.getSeqChar(0,1).isGap() ,true)
	TEST_EQUAL(al.getSeqChar(0,0).getChar(),'A')
	TEST_EQUAL(al.getSeqChar(0,2).getChar(), 'G')
	TEST_EQUAL(al.cols(), 3)

	TEST_EQUAL(al.getSeqChar(1,0).getChar(), 'G')
	TEST_EQUAL(al.getSeqChar(1,1).getChar(), 'G')
	TEST_EQUAL(al.getSeqChar(1,2).isGap(), true)

	TEST_EQUAL(al.insertGap(0,0), true)

	TEST_EQUAL(al.cols(), 4)
	TEST_EQUAL(al.getSeqChar(0,0).isGap() ,true)
	TEST_EQUAL(al.getSeqChar(0,1).getChar(), 'A')
	TEST_EQUAL(al.getSeqChar(0,2).isGap(), true)
	TEST_EQUAL(al.getSeqChar(0,3).getChar(), 'G')

	TEST_EQUAL(al.getSeqChar(1,0).getChar(), 'G')
	TEST_EQUAL(al.getSeqChar(1,1).getChar(), 'G')
	TEST_EQUAL(al.getSeqChar(1,2).isGap(), true)
	TEST_EQUAL(al.getSeqChar(1,3).isGap(),true)

RESULT

al.reset();


CHECK(bool deleteGap(Sequence& seq, int pos))

	Sequence seq;
	Protein *p1 = new Protein();

	Residue* r1 = new Residue("ALA");
	p1->insert(*r1);
	Residue* r2 = new Residue ("GLY");
	p1->insert(*r2);

	seq.setOrigin(p1);
	al.addSequence(seq);

	Sequence seq2;

	Protein *p2 = new Protein();

	Residue* r3 = new Residue("GLY");

	Residue* r4 = new Residue("GLY");

	p2->insert(*r3);
	p2->insert(*r4);

	seq2.setOrigin(p2);
	al.addSequence(seq2);

	al.insertGap(0,2);
	al.insertGap(0, 1);

	TEST_EQUAL(al.deleteGap(seq,0) , false);


	TEST_EQUAL(al.deleteGap(seq, 1), true)
	TEST_EQUAL(al.getSeqChar(0,2).isGap() ,true)
	TEST_EQUAL(al.getSeqChar(0,0).getChar(),'A')
	TEST_EQUAL(al.getSeqChar(0,1).getChar(), 'G')
	TEST_EQUAL(al.cols(), 3)
	TEST_EQUAL(al.getSeqChar(1,0).getChar(), 'G')
	TEST_EQUAL(al.getSeqChar(1,1).getChar(), 'G')
	TEST_EQUAL(al.getSeqChar(1,2).isGap(), true)


	al.insertGap(1,0);

	TEST_EQUAL(al.deleteGap(seq2,0), true)
	TEST_EQUAL(al.getSeqChar(0,2).isGap() ,true)
	TEST_EQUAL(al.getSeqChar(0,0).getChar(),'A')
	TEST_EQUAL(al.getSeqChar(0,1).getChar(), 'G')
	TEST_EQUAL(al.cols(), 3)
	TEST_EQUAL(al.getSeqChar(1,0).getChar(), 'G')
	TEST_EQUAL(al.getSeqChar(1,1).getChar(), 'G')
	TEST_EQUAL(al.getSeqChar(1,2).isGap(), true)


	TEST_EQUAL(al.deleteGap(seq,2), true)
	TEST_EQUAL(al.getSeqChar(0,2).isGap() ,true)
	TEST_EQUAL(al.getSeqChar(0,0).getChar(),'A')
	TEST_EQUAL(al.getSeqChar(0,1).getChar(), 'G')
	TEST_EQUAL(al.cols(), 3)
	TEST_EQUAL(al.getSeqChar(1,0).getChar(), 'G')
	TEST_EQUAL(al.getSeqChar(1,1).getChar(), 'G')
	TEST_EQUAL(al.getSeqChar(1,2).isGap(), true)

RESULT

al.reset();

CHECK(bool deleteGap(int row, int column))

	Sequence seq;
	Protein *p1 = new Protein();

	Residue* r1 = new Residue("ALA");
	p1->insert(*r1);
	Residue* r2 = new Residue ("GLY");
	p1->insert(*r2);

	seq.setOrigin(p1);
	al.addSequence(seq);

	Sequence seq2;
	
	Protein *p2 = new Protein();

	Residue* r3 = new Residue("GLY");

	Residue* r4 = new Residue("GLY");

	p2->insert(*r3);
	p2->insert(*r4);
		
	seq2.setOrigin(p2);
	al.addSequence(seq2);

	al.insertGap(0,2);
	al.insertGap(0, 1);

	TEST_EQUAL(al.deleteGap(0,0) , false);


	TEST_EQUAL(al.deleteGap(0, 1), true)
	TEST_EQUAL(al.getSeqChar(0,2).isGap() ,true)
	TEST_EQUAL(al.getSeqChar(0,0).getChar(),'A')
	TEST_EQUAL(al.getSeqChar(0,1).getChar(), 'G')
	TEST_EQUAL(al.cols(), 3)
	TEST_EQUAL(al.getSeqChar(1,0).getChar(), 'G')
	TEST_EQUAL(al.getSeqChar(1,1).getChar(), 'G')
	TEST_EQUAL(al.getSeqChar(1,2).isGap(), true)


	al.insertGap(1,0);

	TEST_EQUAL(al.deleteGap(1,0), true)
	TEST_EQUAL(al.getSeqChar(0,2).isGap() ,true)
	TEST_EQUAL(al.getSeqChar(0,0).getChar(),'A')
	TEST_EQUAL(al.getSeqChar(0,1).getChar(), 'G')
	TEST_EQUAL(al.cols(), 3)
	TEST_EQUAL(al.getSeqChar(1,0).getChar(), 'G')
	TEST_EQUAL(al.getSeqChar(1,1).getChar(), 'G')
	TEST_EQUAL(al.getSeqChar(1,2).isGap(), true)


	TEST_EQUAL(al.deleteGap(0,2), true)
	TEST_EQUAL(al.getSeqChar(0,2).isGap() ,true)
	TEST_EQUAL(al.getSeqChar(0,0).getChar(),'A')
	TEST_EQUAL(al.getSeqChar(0,1).getChar(), 'G')
	TEST_EQUAL(al.cols(), 3)
	TEST_EQUAL(al.getSeqChar(1,0).getChar(), 'G')
	TEST_EQUAL(al.getSeqChar(1,1).getChar(), 'G')
	TEST_EQUAL(al.getSeqChar(1,2).isGap(), true)
RESULT

al.reset();



CHECK(bool deleteSequence(Sequence& seq))


	Sequence seq;
	Protein *p1 = new Protein();

	Residue* r1 = new Residue("ALA");
	p1->insert(*r1);
	Residue* r2 = new Residue ("GLY");
	p1->insert(*r2);

	seq.setOrigin(p1);
	al.addSequence(seq);

	Sequence seq2;
	
	Protein *p2 = new Protein();

	Residue* r3 = new Residue("GLY");

	Residue* r4 = new Residue("GLY");

	p2->insert(*r3);
	p2->insert(*r4);
		
	seq2.setOrigin(p2);

	TEST_EQUAL(al.deleteSequence (seq2), false)

	al.addSequence(seq2);

	TEST_EQUAL(al.deleteSequence(seq), true)
	TEST_EQUAL(al.rows(),1)
	TEST_EQUAL(al.getSeqChar(0,0).getChar(), 'G')
	TEST_EQUAL(al.getSeqChar(0,1).getChar(), 'G')
	
RESULT

al.reset();

CHECK(bool deleteSequence(int row))

	Sequence seq;
	Protein *p1 = new Protein();

	Residue* r1 = new Residue("ALA");
	p1->insert(*r1);
	Residue* r2 = new Residue ("GLY");
	p1->insert(*r2);

	seq.setOrigin(p1);
	al.addSequence(seq);

	Sequence seq2;
	
	Protein *p2 = new Protein();

	Residue* r3 = new Residue("GLY");

	Residue* r4 = new Residue("GLY");

	p2->insert(*r3);
	p2->insert(*r4);
		
	seq2.setOrigin(p2);
	al.addSequence(seq2);
	TEST_EQUAL(al.deleteSequence (4), false)

	TEST_EQUAL(al.deleteSequence(0), true)
	TEST_EQUAL(al.rows(),1)
	TEST_EQUAL(al.getSeqChar(0,0).getChar(), 'G')
	TEST_EQUAL(al.getSeqChar(0,1).getChar(), 'G')
	
RESULT


al.reset();

CHECK(bool insertSeqChar(Sequence& seq, SequenceCharacter& c, int pos=0))

	Sequence seq;
	Protein *p1 = new Protein();

	Residue* r1 = new Residue("ALA");
	p1->insert(*r1);
	Residue* r2 = new Residue ("GLY");
	p1->insert(*r2);
		
	seq.setOrigin(p1);
	al.addSequence(seq);

	Sequence seq2;
	
	Protein *p2 = new Protein();

	Residue* r4 = new Residue("GLY");

	Residue* r5 = new Residue("GLY");

	p2->insert(*r4);
	p2->insert(*r5);
		
	seq2.setOrigin(p2);
	al.addSequence(seq2);
	
	Residue* r3 = new Residue("ALA");
	
	SequenceCharacter c;
	c.setOrigin(&seq);
	c.setResidue(r3);
	c.setChar('A');
	c.setType(SequenceCharacter::type::CHAR);
	p1->insert(*r3);

	TEST_EQUAL(al.insertSeqChar(seq2, c), false)

	TEST_EQUAL(al.insertSeqChar(seq,c), true)

	TEST_EQUAL(al.cols(), 3)
	TEST_EQUAL(al.getSeqChar(0,1).getChar(),'A')
	TEST_EQUAL(al.getSeqChar(0,0).getChar(),'A')
	TEST_EQUAL(al.getSeqChar(0,2).getChar(), 'G')


	TEST_EQUAL(al.getSeqChar(1,0).getChar(), 'G')
	TEST_EQUAL(al.getSeqChar(1,1).getChar(), 'G')
	TEST_EQUAL(al.getSeqChar(1,2).isGap(), true)

	al.insertGap(seq,3);
	al.insertSeqChar(seq,c,3);
	
	TEST_EQUAL(al.cols(), 4)

	TEST_EQUAL(al.getSeqChar(0,1).getChar(),'A')
	TEST_EQUAL(al.getSeqChar(0,0).getChar(),'A')
	TEST_EQUAL(al.getSeqChar(0,2).getChar(),'G')
	TEST_EQUAL(al.getSeqChar(0,3).getChar(),'A')

	TEST_EQUAL(al.getSeqChar(1,0).getChar(), 'G')
	TEST_EQUAL(al.getSeqChar(1,1).getChar(), 'G')
	TEST_EQUAL(al.getSeqChar(1,2).isGap(), true)
	TEST_EQUAL(al.getSeqChar(1,3).isGap(), true)

	al.insertSeqChar(seq,c,2);

	TEST_EQUAL(al.cols(), 5)
	TEST_EQUAL(al.getSeqChar(0,1).getChar(), 'A')
	TEST_EQUAL(al.getSeqChar(0,0).getChar(),'A')
	TEST_EQUAL(al.getSeqChar(0,2).getChar(),'A')
	TEST_EQUAL(al.getSeqChar(0,3).getChar(),'G')
	TEST_EQUAL(al.getSeqChar(0,4).getChar(),'A')
	
	TEST_EQUAL(al.getSeqChar(1,0).getChar(), 'G')
	TEST_EQUAL(al.getSeqChar(1,1).getChar(), 'G')
	TEST_EQUAL(al.getSeqChar(1,2).isGap(), true)
	TEST_EQUAL(al.getSeqChar(1,3).isGap(),true)
	TEST_EQUAL(al.getSeqChar(1,4).isGap(),true)

RESULT

al.reset();


CHECK(bool insertSeqChar(int row, int column, SequenceCharacter& c))

	Sequence seq;
	Protein *p1 = new Protein();

	Residue* r1 = new Residue("ALA");
	p1->insert(*r1);
	Residue* r2 = new Residue ("GLY");
	p1->insert(*r2);
		
	seq.setOrigin(p1);
	al.addSequence(seq);

	Sequence seq2;
	
	Protein *p2 = new Protein();

	Residue* r4 = new Residue("GLY");

	Residue* r5 = new Residue("GLY");

	p2->insert(*r4);
	p2->insert(*r5);
		
	seq2.setOrigin(p2);
	al.addSequence(seq2);
	
	Residue* r3 = new Residue("ALA");
	
	SequenceCharacter c;
	c.setOrigin(&seq);
	c.setResidue(r3);
	c.setChar('A');
	c.setType(SequenceCharacter::type::CHAR);
	p1->insert(*r3);

	TEST_EQUAL(al.insertSeqChar(1,0,c),false)


	TEST_EQUAL(al.insertSeqChar(0,0,c), true)

	TEST_EQUAL(al.cols(), 3)
	
	TEST_EQUAL(al.getSeqChar(0,1).getChar(),'A')
	TEST_EQUAL(al.getSeqChar(0,0).getChar(),'A')
	TEST_EQUAL(al.getSeqChar(0,2).getChar(), 'G')
	TEST_EQUAL(al.cols(), 3)
	TEST_EQUAL(al.getSeqChar(1,0).getChar(), 'G')
	TEST_EQUAL(al.getSeqChar(1,1).getChar(), 'G')
	TEST_EQUAL(al.getSeqChar(1,2).isGap(), true)

	al.insertGap(0,3);
	al.insertSeqChar(0,3, c);
	TEST_EQUAL(al.cols(), 4)
	TEST_EQUAL(al.getSeqChar(0,1).getChar(),'A')
	TEST_EQUAL(al.getSeqChar(0,0).getChar(),'A')
	TEST_EQUAL(al.getSeqChar(0,2).getChar(),'G')
	TEST_EQUAL(al.getSeqChar(0,3).getChar(),'A')
	TEST_EQUAL(al.getSeqChar(1,0).getChar(), 'G')
	TEST_EQUAL(al.getSeqChar(1,1).getChar(), 'G')
	TEST_EQUAL(al.getSeqChar(1,2).isGap(), true)
	TEST_EQUAL(al.getSeqChar(1,3).isGap(), true)

	al.insertSeqChar(0,2, c);
	TEST_EQUAL(al.cols(), 5)
	TEST_EQUAL(al.getSeqChar(0,1).getChar(), 'A')
	TEST_EQUAL(al.getSeqChar(0,0).getChar(),'A')
	TEST_EQUAL(al.getSeqChar(0,2).getChar(),'A')
	TEST_EQUAL(al.getSeqChar(0,3).getChar(),'G')
	TEST_EQUAL(al.getSeqChar(0,4).getChar(),'A')
	TEST_EQUAL(al.getSeqChar(1,0).getChar(), 'G')
	TEST_EQUAL(al.getSeqChar(1,1).getChar(), 'G')
	TEST_EQUAL(al.getSeqChar(1,2).isGap(), true)
	TEST_EQUAL(al.getSeqChar(1,3).isGap(),true)
	TEST_EQUAL(al.getSeqChar(1,4).isGap(),true)

RESULT

al.reset();
CHECK(void read(Protein& protein))

	Protein *p1 = new Protein();

	Residue* r1 = new Residue("ALA");
	p1->insert(*r1);
	Residue* r2 = new Residue ("GLY");
	p1->insert(*r2);

	al.read(*p1);

	TEST_EQUAL(al.getSeqChar(0,0).getChar(),'A')
	TEST_EQUAL(al.getSeqChar(0,1).getChar(), 'G')
	TEST_EQUAL(al.cols(), 2)
	TEST_EQUAL(al.rows(),1)
	TEST_EQUAL(al.isAligned(), false)


	Protein *p2 = new Protein();
	Residue* r4 = new Residue("GLY");
	Residue* r5 = new Residue("GLY");

	p2->insert(*r4);
	p2->insert(*r5);

	al.read(*p2);

	TEST_EQUAL(al.getSeqChar(0,0).getChar(),'A')
	TEST_EQUAL(al.getSeqChar(0,1).getChar(), 'G')
	TEST_EQUAL(al.cols(), 2)
	TEST_EQUAL(al.rows(),2)
	TEST_EQUAL(al.getSeqChar(1,0).getChar(), 'G')
	TEST_EQUAL(al.getSeqChar(1,1).getChar(), 'G')
	TEST_EQUAL(al.isAligned(), false)
RESULT

al.reset();
CHECK (void read(System& system))

	Protein *p1 = new Protein();

	Residue* r1 = new Residue("ALA");
	p1->insert(*r1);
	Residue* r2 = new Residue ("GLY");
	p1->insert(*r2);
		
	
	Protein *p2 = new Protein();

	Residue* r4 = new Residue("GLY");

	Residue* r5 = new Residue("GLY");

	p2->insert(*r4);
	p2->insert(*r5);
		

	System sys;
	sys.append(*p1);
	sys.append(*p2);

	al.read(sys);

	TEST_EQUAL(al.getSeqChar(0,0).getChar(),'A')
	TEST_EQUAL(al.getSeqChar(0,1).getChar(), 'G')
	TEST_EQUAL(al.cols(), 2)
	TEST_EQUAL(al.rows(),2)
	TEST_EQUAL(al.getSeqChar(1,0).getChar(), 'G')
	TEST_EQUAL(al.getSeqChar(1,1).getChar(), 'G')
	TEST_EQUAL(al.isAligned(), false)


RESULT

al.reset();
CHECK(bool deleteGapColumn(unsigned int col))

	Sequence seq;
	Protein *p1 = new Protein();

	Residue* r1 = new Residue("ALA");
	p1->insert(*r1);
	Residue* r2 = new Residue ("GLY");
	p1->insert(*r2);
		
	seq.setOrigin(p1);
	al.addSequence(seq);

	Sequence seq2;
	
	Protein *p2 = new Protein();

	Residue* r4 = new Residue("GLY");

	Residue* r5 = new Residue("GLY");

	p2->insert(*r4);
	p2->insert(*r5);
		
	seq2.setOrigin(p2);
	al.addSequence(seq2);

	al.insertGap(seq,2);
	
	al.deleteGapColumn(2);

	TEST_EQUAL(al.getSeqChar(0,0).getChar(),'A')
	TEST_EQUAL(al.getSeqChar(0,1).getChar(), 'G')
	TEST_EQUAL(al.cols(), 2)
	TEST_EQUAL(al.rows(),2)
	TEST_EQUAL(al.getSeqChar(1,0).getChar(), 'G')
	TEST_EQUAL(al.getSeqChar(1,1).getChar(), 'G')
	TEST_EQUAL(al.isAligned(), false)


	al.insertGap(seq,1);
	al.insertGap(seq2,1);

	al.deleteGapColumn(1);

RESULT





/*			bool computeScore(ScoringFunction& scoring_function);
	bool align(AlignAlgorithm& algorithm);
	{
*/
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
