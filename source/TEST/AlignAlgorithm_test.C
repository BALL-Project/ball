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

#include <BALL/SEQUENCE/alignAlgorithm.h>
#include <BALL/SEQUENCE/sequence.h>
///////////////////////////


START_TEST(AlignAlgorithm_test.C)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

AlignAlgorithm* algop;

CHECK(AlignAlgorithm())
	algop = new AlignAlgorithm();
	TEST_NOT_EQUAL(algop, 0);
RESULT

CHECK(~AlignAlgorithm())
	delete algop;
RESULT

Alignment al;

CHECK(align(Alignment& alignment))

	Sequence seq;
	Protein *p1 = new Protein();

	Residue* r1 = new Residue("Cys");
	p1->insert(*r1);
	Residue* r2 = new Residue ("Asp");
	p1->insert(*r2);
	Residue* r3 = new Residue("Phe");
	p1->insert(*r3);

	Residue* r4 = new Residue("Gly");
	p1->insert(*r4);

	Residue* r5 = new Residue("His");
	p1->insert(*r5);
	
	Residue* r6 = new Residue("Cys");
	p1->insert(*r6);

	seq.setOrigin(p1);
	al.addSequence(seq);

	
	Sequence seq2;
	
	Protein *p2 = new Protein();

	Residue *r18 = new Residue ("Lys");
	p2->insert(*r18);

	Residue* r7 = new Residue("Cys");
	p2->insert(*r7);

	Residue* r8 = new Residue("Asp");
	p2->insert(*r8);

	Residue* r9 = new Residue("Glu");
	p2->insert(*r9);

	Residue* r10 = new Residue("Phe");
	p2->insert(*r10);

	Residue* r11 = new Residue("Gly");
	p2->insert(*r11);

	Residue* r12 = new Residue("Ala");
	p2->insert(*r12);

	Residue* r13 = new Residue("His");
	p2->insert(*r13);

	Residue* r14 = new Residue("Cys");
	p2->insert(*r14);

	Residue *r15 = new Residue ("Lys");
	p2->insert(*r15);

	Residue *r16 = new Residue ("Lys");
	p2->insert(*r16);


	seq2.setOrigin(p2);
	al.addSequence(seq2);

	AlignAlgorithm algo;

	algo.align(al);


	TEST_EQUAL(al.getSeqChar(0,0).isGap() ,true)
	TEST_EQUAL(al.getSeqChar(0,1).getChar(), 'C')
	TEST_EQUAL(al.getSeqChar(0,2).getChar(), 'D')
	TEST_EQUAL(al.getSeqChar(0,3).isGap(),true)
	TEST_EQUAL(al.getSeqChar(0,4).getChar(), 'F')
	TEST_EQUAL(al.getSeqChar(0,5).getChar(), 'G')
	TEST_EQUAL(al.getSeqChar(0,6).isGap(),true)
	TEST_EQUAL(al.getSeqChar(0,7).getChar(), 'H')
	TEST_EQUAL(al.getSeqChar(0,8).getChar(), 'C')
	TEST_EQUAL(al.getSeqChar(0,9).isGap(),true)
	TEST_EQUAL(al.getSeqChar(0,10).isGap(),true)



	TEST_EQUAL(al.getSeqChar(1,0).getChar(),'K')
	TEST_EQUAL(al.getSeqChar(1,1).getChar(), 'C')
	TEST_EQUAL(al.getSeqChar(1,2).getChar(), 'D')
	TEST_EQUAL(al.getSeqChar(1,3).getChar(), 'E')
	TEST_EQUAL(al.getSeqChar(1,4).getChar(), 'F')
	TEST_EQUAL(al.getSeqChar(1,5).getChar(), 'G')
	TEST_EQUAL(al.getSeqChar(1,6).getChar(), 'A')
	TEST_EQUAL(al.getSeqChar(1,7).getChar(), 'H')
	TEST_EQUAL(al.getSeqChar(1,8).getChar(), 'C')
	TEST_EQUAL(al.getSeqChar(1,9).getChar(), 'K')
	TEST_EQUAL(al.getSeqChar(1,10).getChar(), 'K')

	TEST_EQUAL(al.isAligned(), true);

	TEST_EQUAL(al.getScore(), 1);
al.dump();
	RESULT

	al.reset();

	CHECK(align(Alignment& alignment))

	Sequence seq;
	Protein *p1 = new Protein();

	Residue* r1 = new Residue("Cys");
	p1->insert(*r1);
	Residue* r2 = new Residue ("Asp");
	p1->insert(*r2);
	Residue* r3 = new Residue("Phe");
	p1->insert(*r3);

	Residue* r4 = new Residue("Gly");
	p1->insert(*r4);

	Residue* r5 = new Residue("His");
	p1->insert(*r5);
	
	Residue* r6 = new Residue("Cys");
	p1->insert(*r6);

	seq.setOrigin(p1);
	al.addSequence(seq);

	
	Sequence seq2;
	
	Protein *p2 = new Protein();

	Residue *r17 = new Residue ("Lys");
	p2->insert(*r17);

	Residue *r18 = new Residue ("Lys");
	p2->insert(*r18);

	Residue* r7 = new Residue("Cys");
	p2->insert(*r7);

	Residue* r8 = new Residue("Asp");
	p2->insert(*r8);

	Residue* r9 = new Residue("Glu");
	p2->insert(*r9);

	Residue* r10 = new Residue("Phe");
	p2->insert(*r10);

	Residue* r11 = new Residue("Gly");
	p2->insert(*r11);

	Residue* r12 = new Residue("Ala");
	p2->insert(*r12);

	Residue* r13 = new Residue("His");
	p2->insert(*r13);

	Residue* r14 = new Residue("Cys");
	p2->insert(*r14);

	Residue *r15 = new Residue ("Lys");
	p2->insert(*r15);

	Residue *r16 = new Residue ("Lys");
	p2->insert(*r16);


	seq2.setOrigin(p2);
	al.addSequence(seq2);

	Sequence seq3;
	Protein *p3 = new Protein();

	Residue *r19 = new Residue ("Lys");
	p3->insert(*r19);

	Residue* r20 = new Residue("Cys");
	p3->insert(*r20);


	Residue* r21 = new Residue("Phe");
	p3->insert(*r21);

	Residue* r22 = new Residue("Gly");
	p3->insert(*r22);

	Residue* r23 = new Residue("Ala");
	p3->insert(*r23);

	Residue* r24 = new Residue("His");
	p3->insert(*r24);

	Residue *r25 = new Residue ("Lys");
	p3->insert(*r25);

	Residue *r26 = new Residue ("Lys");
	p3->insert(*r26);

	seq3.setOrigin(p3);
	al.addSequence(seq3);

	Sequence seq4;
	Protein *p4 = new Protein();

	Residue *r27 = new Residue ("Lys");
	p4->insert(*r27);

	Residue *r28 = new Residue ("Lys");
	p4->insert(*r28);

	Residue* r29 = new Residue("Glu");
	p4->insert(*r29);

	Residue* r30 = new Residue("Phe");
	p4->insert(*r30);

	Residue* r31 = new Residue("Gly");
	p4->insert(*r31);

	Residue* r32 = new Residue("Ala");
	p4->insert(*r32);

	Residue* r33 = new Residue("Cys");
	p4->insert(*r33);

	Residue *r34 = new Residue ("Lys");
	p4->insert(*r34);

	Residue *r35 = new Residue ("Lys");
	p4->insert(*r35);


	seq4.setOrigin(p4);
	al.addSequence(seq4);

	AlignAlgorithm algo;

	algo.align(al);

//--CD--FG-HC--
	TEST_EQUAL(al.getSeqChar(0,0).isGap(),true)
	TEST_EQUAL(al.getSeqChar(0,1).isGap() ,true)
	TEST_EQUAL(al.getSeqChar(0,2).getChar(), 'C')
	TEST_EQUAL(al.getSeqChar(0,3).getChar(), 'D')
	TEST_EQUAL(al.getSeqChar(0,4).isGap(),true)
	TEST_EQUAL(al.getSeqChar(0,5).getChar(), 'F')
	TEST_EQUAL(al.getSeqChar(0,6).getChar(), 'G')
	TEST_EQUAL(al.getSeqChar(0,7).isGap(),true)
	TEST_EQUAL(al.getSeqChar(0,8).getChar(), 'H')
	TEST_EQUAL(al.getSeqChar(0,9).getChar(), 'C')
	TEST_EQUAL(al.getSeqChar(0,10).isGap(),true)
	TEST_EQUAL(al.getSeqChar(0,11).isGap(),true)


//KKCDEFGAHCKK
	TEST_EQUAL(al.getSeqChar(1,0).getChar(),'K')
	TEST_EQUAL(al.getSeqChar(1,1).getChar() ,'K')
	TEST_EQUAL(al.getSeqChar(1,2).getChar(), 'C')
	TEST_EQUAL(al.getSeqChar(1,3).getChar(), 'D')
	TEST_EQUAL(al.getSeqChar(1,4).getChar(), 'E')
	TEST_EQUAL(al.getSeqChar(1,5).getChar(), 'F')
	TEST_EQUAL(al.getSeqChar(1,6).getChar(), 'G')
	TEST_EQUAL(al.getSeqChar(1,7).getChar(), 'A')
	TEST_EQUAL(al.getSeqChar(1,8).getChar(), 'H')
	TEST_EQUAL(al.getSeqChar(1,9).getChar(), 'C')
	TEST_EQUAL(al.getSeqChar(1,10).getChar(), 'K')
	TEST_EQUAL(al.getSeqChar(1,11).getChar(), 'K')

//-KC--FGA-HKK
	TEST_EQUAL(al.getSeqChar(2,0).isGap(),true)
	TEST_EQUAL(al.getSeqChar(2,1).getChar() , 'K')
	TEST_EQUAL(al.getSeqChar(2,2).getChar(), 'C')
	TEST_EQUAL(al.getSeqChar(2,3).isGap(), true)
	TEST_EQUAL(al.getSeqChar(2,4).isGap(), true)
	TEST_EQUAL(al.getSeqChar(2,5).getChar(), 'F')
	TEST_EQUAL(al.getSeqChar(2,6).getChar(), 'G')
	TEST_EQUAL(al.getSeqChar(2,7).getChar(), 'A')
	TEST_EQUAL(al.getSeqChar(2,8).isGap(), true)
	TEST_EQUAL(al.getSeqChar(2,9).getChar(), 'H')
	TEST_EQUAL(al.getSeqChar(2,10).getChar(), 'K')
	TEST_EQUAL(al.getSeqChar(2,11).getChar(), 'K')

//KKE--FGA-CKK
	TEST_EQUAL(al.getSeqChar(3,0).getChar(),'K')
	TEST_EQUAL(al.getSeqChar(3,1).getChar() ,'K')
	TEST_EQUAL(al.getSeqChar(3,2).getChar(), 'E')
	TEST_EQUAL(al.getSeqChar(3,3).isGap(), true)
	TEST_EQUAL(al.getSeqChar(3,4).isGap(), true)
	TEST_EQUAL(al.getSeqChar(3,5).getChar(),'F')
	TEST_EQUAL(al.getSeqChar(3,6).getChar(), 'G')
	TEST_EQUAL(al.getSeqChar(3,7).getChar(), 'A')
	TEST_EQUAL(al.getSeqChar(3,8).isGap(), true)
	TEST_EQUAL(al.getSeqChar(3,9).getChar(), 'C')
	TEST_EQUAL(al.getSeqChar(3,10).getChar(), 'K')
	TEST_EQUAL(al.getSeqChar(3,11).getChar(), 'K')

	TEST_EQUAL(al.isAligned(), true);

	RESULT


END_TEST
