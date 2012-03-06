// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: Sequence_test.C,2011/06/06 $
//
// Author:
//   Nikola Koch
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/SEQUENCE/sequence.h>
#include <BALL/KERNEL/atomContainer.h>
#include <BALL/SEQUENCE/sequenceIterator.h>
///////////////////////////

START_TEST(Sequence)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

///  insert tests for each member function here         
///


Sequence* seqp;

	
CHECK(Sequence())	
 seqp= new Sequence();
	TEST_NOT_EQUAL(seqp,0);
RESULT

CHECK(~Sequence())
	delete seqp;
RESULT


// Test detailed Constructor
AtomContainer *ori1= new AtomContainer();
String s1= "test1";

CHECK(Sequence(String name, AtomContainer& origin))

	Sequence *seqp= new Sequence(s1,ori1);
	TEST_EQUAL( seqp->getName(), s1);

	TEST_EQUAL( seqp->getOrigin(), ori1);

RESULT




//////////////////////////////////////////////////////////// Test Getter and Setter ///////////////////////////////////////////////////////

String *s2= new String("test2");

CHECK(setName(BALL::String name))
	seqp->setName(*s2);
	TEST_EQUAL(seqp->getName(),*s2);
RESULT

CHECK(setOrigin(AtomContainer* origin))
	AtomContainer *ori2 = new AtomContainer();
	seqp->setOrigin(ori2);
//	TEST_EQUAL(seqp->getOrigin(),*ori2);
RESULT



CHECK (operator== (Sequence& seq))
	TEST_EQUAL(seqp==seqp,true)
RESULT

CHECK(operator!= (Sequence& seq))
	TEST_NOT_EQUAL(seqp!=seqp,true)
RESULT

CHECK(operator= (Sequence& seq))
	Sequence& seq2 = *seqp;
	TEST_EQUAL(seq2==*seqp, true)
RESULT



CHECK(getStringSequence())
	
	Protein *p = new Protein();

	Residue* r1 = new Residue("ALA");
	Residue* r2 = new Residue ("GLY");


	p->insert(*r1);
	p->insert(*r2);
	

	Sequence* seq2 = new Sequence() ;

	seq2->setOrigin(p);

	TEST_EQUAL(seq2->getStringSequence(), "AG")
RESULT

CHECK(getArraySequence())
	
	Protein *p = new Protein();

	Residue* r1 = new Residue("ALA");
	Residue* r2 = new Residue ("GLY");


	p->insert(*r1);
	p->insert(*r2);
	

	Sequence* seq2 = new Sequence() ;

	seq2->setOrigin(p);

	Eigen::Array<SequenceCharacter ,2,1> test = seq2->getArraySequence();

	SequenceCharacter c;
	c.setOrigin(seq2);
	c.setType(SequenceCharacter::type::CHAR);
	c.setChar('A');

	TEST_EQUAL(test.size(), 2);

	TEST_EQUAL(test(0,0)==c, true)

	c.setChar('G');

	TEST_EQUAL (test(1,0)==c, true)

RESULT

////////////////////////////////////////////////Test Exceptions ///////////////////////////////////////////


END_TEST

