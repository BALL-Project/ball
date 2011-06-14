// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//


#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

//////////////////////////////////////////////////////////////////////////////////////////

#include <BALL/SEQUENCE/sequenceCharacter.h>
#include <BALL/COMMON/exception.h>

////////////////////////////////////////////////////////////////////////////
using namespace BALL;

START_TEST(SequenceCharacter, "SequenceCharacter Test 2011/05/31 Author: nikoch")
PRECISION(1e-5)


SequenceCharacter* seqcharp;

//test the constructor
CHECK(SequenceCharacter())
	TEST_NOT_EQUAL(seqcharp,0);

RESULT
//test the Deconstructor
CHECK(~SequenceCharacter());
	delete seqcharp;
RESULT

////////////////////////// Test Getter and Setter //////////////////////////////////
 
SequenceCharacter seqchar;


CHECK(setChar(char character))
        seqchar.setChar('D');
	TEST_EQUAL(seqchar.getChar(), 'D');
RESULT

String* s =new String("AA");

CHECK(setType(String& type))
	seqchar.setType(*s);
	
	TEST_EQUAL(seqchar.getType, *s);
RESULT

//
CHECK(setSequence(Sequence& origin))
	Sequence* s = new Sequence();
	seqchar.setOrigin(*s);
	TEST_EQUAL(seqchar.getOrigin(),*s);
RESULT


////////////////////////////////////////////// Test Exceptions /////////////////////////////////////////////////////7

//checks whether an exception is thrown if character is invalid
CHECK(setChar(char character))
	TEST_EXCEPTION(Exception::InvalidArgument,seqchar.setChar('B'));
RESULT

//checks whether an exception is thrown if type is invalid
	*s="foo";
CHECK(setType(String type))
	TEST_EXCEPTION(Exception::InvalidArgument, seqchar.setType(*s));
RESULT

/**checks whether an exception is thrown if origin is invalid
CHECK(setOrigin(Sequence& seq))
	TEST_EXCEPTION(Exception::InvaldiArgument,seqchar.setOrigin("somethingstupid"));
RESULT
*/

//checks whether isGap returns false if type is no GAP
CHECK(isGAP())
	TEST_EQUAL(seqchar.isGAP(), false);
RESULT

//checks whether isGap returns true if type is GAP
CHECK(isGap())
	seqchar.setChar('-');
	*s="GAP";
	seqchar.setType(*s);

	TEST_EQUAL(seqchar.isGap, true);
RESULT

//checks whether an Exception is thrown if type is GAP but character is no GAP

CHECK(isGap())
	seqchar.setChar('A');
	TEST_EXCEPTION(Exception::GeneralException,seqchar.isGap());
RESULT

//checks whether an Exception is thrown if type is not GAP but hte character is a GAP
CHECK(isGap())
	seq.setChar('-');
	*s= "AA";
	seq.setType(*s);
	TEST_EXCEPTION(Exception::GeneralException, seqchar.isGap());
RESULT


END_TEST
