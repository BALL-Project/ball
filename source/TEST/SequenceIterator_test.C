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

START_TEST(SequenceIterator)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

SequenceIterator* itp;

CHECK(SequenceIterator())
	itp= new SequenceIterator();

	TEST_NOT_EQUAL(itp,0);
RESULT

CHECK(~SequenceIterator())
	delete itp;
RESULT	


CHECK(SequenceIterator(Sequence& seq))

	Sequence seq;
	seq.setName("example");

itp= new SequenceIterator(seq);
bool cmp = itp->getSequence() == seq;

TEST_EQUAL(cmp,true);
RESULT

CHECK(SequenceIterator(SequenceIterator& it))
	Sequence seq;

	seq.setName("example");

	itp= new SequenceIterator(seq);

	SequenceIterator* itpcp = new SequenceIterator(*itp);

	bool cmp= itp->getSequence() == itpcp->getSequence();
	TEST_EQUAL(cmp, true);
	TEST_EQUAL(itp->getCounter(), itpcp->getCounter());
	cmp=itp->getCharacter()== itpcp->getCharacter();
	TEST_EQUAL(cmp,true);

RESULT

CHECK(SequenceIterator& operator = (const SequenceIterator& it))

	Sequence seq;
	SequenceCharacter c;
	c.setOrigin(&seq);
	itp->setSequence(seq);
	itp->setChar(c);
	itp->setCounter(4);
	
	SequenceIterator& cp = *itp;

bool cmp= itp->getSequence()== cp.getSequence();
TEST_EQUAL(cmp,true);

cmp= itp->getCharacter() == cp.getCharacter();
TEST_EQUAL(cmp, true);

TEST_EQUAL(itp->getCounter(), cp.getCounter());

RESULT



////////////////////////////////////// set up environment for the test /////////////////////////////////////
	SequenceIterator it;
	
	Protein *p = new Protein();

	Residue* r1 = new Residue("ALA");
	Residue* r2 = new Residue ("GLY");


	p->insert(*r1);
	p->insert(*r2);
	

	Sequence* seqp = new Sequence() ;

	seqp->setOrigin(p);

//////////////////////////////////////////////////////////////////////////////////////

CHECK(bool first())
	it.setSequence(*seqp);
	it.first();
	
	TEST_EQUAL(it.getCounter(),0);

	SequenceCharacter c;
	c.setOrigin(seqp);
	c.setChar('A');
	c.setResidue(r1);

	TEST_EQUAL(it.getCharacter()== c, true);
RESULT

CHECK(void reset())
	it.setSequence(*seqp);
	it.first();

	it.reset();
	TEST_EQUAL(it.getCounter(),0);
//	TEST_EQUAL(it.getSequence(),0);	
	//TODO test ob char auch resetted
RESULT

CHECK(bool last())
	it.setSequence(*seqp);
	it.last();
	
	TEST_EQUAL(it.getCounter(),1);

	SequenceCharacter c;
	c.setOrigin(seqp);
	c.setChar('G');
	c.setResidue(r2);
	
	TEST_EQUAL(it.getCharacter()== c,true);
RESULT


CHECK(bool operator == (SequenceIterator& it))

	it.setSequence(*seqp);
	it.first();

	SequenceIterator tmp = it;

	TEST_EQUAL(tmp == it, true);

	it.last();

	TEST_EQUAL(tmp == it, false);

	tmp.reset();

RESULT

CHECK(bool operator != (SequenceIterator& it))
it.setSequence(*seqp);
it.first();

SequenceIterator tmp = it;

TEST_EQUAL(tmp != it, false);

it.last();

TEST_EQUAL(tmp != it, true);

tmp.reset();

RESULT

CHECK(SequenceIterator& next());

//TODO check wehteher initializing with seq.begin()
	it.setSequence(*seqp);
	it.first();
	it.next();
	
	TEST_EQUAL(it.getCounter(),1);

	SequenceCharacter c;
	c.setOrigin(seqp);
	c.setChar('G');
	c.setResidue(r2);
	
	TEST_EQUAL(it.getCharacter() == c,true)
	
	it.next();

	TEST_EXCEPTION(Exception::IndexOverflow, it.next());

RESULT

CHECK(SequenceIterator& previous())
	it.setSequence(*seqp);
	it.first();

	TEST_EXCEPTION(Exception::IndexUnderflow, it.previous());
	
	it.next();
	it.previous();

	TEST_EQUAL(it.getCounter(),0);

	SequenceCharacter c;
	c.setOrigin(seqp);
	c.setChar('A');
	c.setResidue(r1);	
	TEST_EQUAL(it.getCharacter()== c,true)

RESULT


CHECK(SequenceIterator& operator ++ ())

	it.setSequence(*seqp);
	it.first();

	SequenceIterator tmp2 = it;
	tmp2.last();

	SequenceIterator tmp1 = ++it;
	
	TEST_EQUAL(it==tmp2, true);
	
	it.first();
	TEST_EQUAL(it==tmp1, true);
	++tmp2;
	TEST_EXCEPTION(Exception::IndexOverflow, ++tmp2);

//Postfix


RESULT

CHECK(SequenceIterator& operator -- ())

	it.setSequence(*seqp);
	it.last();

	SequenceIterator tmp2 = it;
	tmp2.first();

	SequenceIterator tmp1 = --it;

	TEST_EQUAL(it == tmp2, true);

	it.last();
	TEST_EQUAL(it == tmp1, true); 
	
	TEST_EXCEPTION(Exception::IndexUnderflow, --tmp2);

//Postfix

RESULT

CHECK(SequenceIterator operator++ (int))

	it.setSequence(*seqp);
	it.first();
	SequenceIterator tmp= it;
	it++;
	tmp.last();
	TEST_EQUAL(tmp== it, true);
	it++;
	TEST_EXCEPTION(Exception::IndexOverflow, it++);


RESULT


CHECK(SequenceIterator operator --(int))

	it.setSequence(*seqp);
	it.last();
	SequenceIterator tmp= it;
	tmp.first();
	it--;
	
	TEST_EQUAL(tmp==it, true);

	TEST_EXCEPTION(Exception::IndexUnderflow, it--);


RESULT

CHECK(SequenceIterator operator + (unsigned int  n))
	
	it.setSequence(*seqp);
	it.first();

	SequenceIterator tmp= it + 1;

	it.last();
	TEST_EQUAL(tmp == it, true);

	TEST_EXCEPTION(Exception::IndexOverflow, it + 3);

//n + a
RESULT

CHECK(SequenceIterator operator - (unsigned int n))
	it.setSequence(*seqp);
	it.last();

	SequenceIterator tmp= it - 1;

	it.first();
	TEST_EQUAL(tmp == it, true);
	TEST_EXCEPTION(Exception::IndexUnderflow, it - 6);

RESULT

SequenceIterator it2;

CHECK(bool operator < (const SequenceIterator& it))
	it.setSequence(*seqp);
	it2.setSequence(*seqp);

	it.first();

	it2.last();

	TEST_EQUAL(it < it2 , true);

	TEST_EQUAL(it2 < it2, false);

	it2.reset();

	it2.setSequence(*(new Sequence()));

	TEST_EXCEPTION(Exception::IncompatibleIterators, it < it2);

RESULT

CHECK(bool operator > (const SequenceIterator& it))
	it.setSequence(*seqp);
	it2.setSequence(*seqp);

	it.first();

	it2.last();
	
	TEST_EQUAL(it2 > it, true);

	TEST_EQUAL(it > it2, false);

	it2.reset();

	it2.setSequence(*(new Sequence()));

	TEST_EXCEPTION(Exception::IncompatibleIterators, it > it2);

RESULT

CHECK(bool operator <= (const SequenceIterator& it))
	it.setSequence(*seqp);
	it2.setSequence(*seqp);

	it.first();

	it2.last();

	TEST_EQUAL(it <= it2 , true);

	TEST_EQUAL(it2 <= it, false);

	it.last();

	TEST_EQUAL(it <= it2, true);

	it2.reset();
	it2.setSequence(*(new Sequence()));

	TEST_EXCEPTION(Exception::IncompatibleIterators, it <= it2);

RESULT

CHECK(bool operator >= (const SequenceIterator& it))

	it.setSequence(*seqp);
	it2.setSequence(*seqp);

	it.first();

	it2.last();

	TEST_EQUAL( it2 >= it, true);

	TEST_EQUAL(it >= it2, false);

	it2.reset();
	it2.setSequence(*(new Sequence()));

	TEST_EXCEPTION(Exception::IncompatibleIterators, it >= it2);

RESULT

CHECK(SequenceIterator& operator += (int i))
	it.setSequence(*seqp);
	it.first();

	SequenceIterator tmp = it;
	tmp.last();

	it+=1;
	TEST_EQUAL(tmp == it, true);

	TEST_EXCEPTION(Exception::IndexOverflow, it += 3)
RESULT

CHECK(SequenceIterator& operator -= (int i))
	it.setSequence(*seqp);
	it.last();

	SequenceIterator tmp= it;
	tmp.first();
	it -= 1;

	TEST_EQUAL(tmp == it, true);
	TEST_EXCEPTION(Exception::IndexUnderflow, it -= 6);


RESULT




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
END_TEST



