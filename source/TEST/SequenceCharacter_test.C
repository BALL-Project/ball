// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//


#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

//////////////////////////////////////////////////////////////////////////////////////////

#include <BALL/SEQUENCE/sequenceCharacter.h>
#include <BALL/COMMON/exception.h>
#include <BALL/SEQUENCE/sequence.h>
////////////////////////////////////////////////////////////////////////////
using namespace BALL;

START_TEST(SequenceCharacter)
PRECISION(1e-5)


SequenceCharacter* seqcharp = new SequenceCharacter();

//test the constructor
CHECK(SequenceCharacter())
	TEST_NOT_EQUAL(seqcharp,0);
RESULT

Sequence* s = new Sequence();
	
seqcharp->setOrigin(s);


//test the Deconstructor
CHECK(~SequenceCharacter());
	delete seqcharp;
	TEST_NOT_EQUAL(s,0);
RESULT



////////////////////////// Test Getter and Setter //////////////////////////////////
 
SequenceCharacter* seqchar= new SequenceCharacter();


CHECK(setChar(char character))
        seqchar->setChar('D');
	TEST_EQUAL(seqchar->getChar(), 'D');
RESULT

SequenceCharacter::type t = SequenceCharacter::type::CHAR ;

CHECK(setType(SequenceCharacter::type ty))
	
	seqchar->setType(t);
	 
	TEST_EQUAL(seqchar->getType(), t);
RESULT

//template für GAP
SequenceCharacter::type gap = SequenceCharacter::type::GAP;

//templat für CHAR
SequenceCharacter::type ch = SequenceCharacter::type::CHAR;

////////////////////////////////////////////// Test Exceptions /////////////////////////////////////////////////////7

//checks whether an exception is thrown if character is invalid
CHECK(setChar(char character))
	TEST_EXCEPTION(Exception::InvalidArgument,seqchar->setChar('B'));
RESULT

//checks whether isGap returns false if type is no GAP
CHECK(isGAP())
	TEST_EQUAL(seqchar->isGap(), false);
RESULT

//CHECKS whether setting the char to '-'sets also the type to GAP
CHECK(setChar(char character))
	seqchar->setChar('-');
	TEST_EQUAL(seqchar->getType(),gap);
RESULT
	
//CHECKS whether setting it back to a character also changes the type again
CHECK(setChar(char character))
	seqchar->setChar('C');
	TEST_EQUAL(seqchar->getType(), ch);
RESULT
	
//CHECKS whether changing the type to GAP does also change the character
CHECK(setType(SequenceCharacter::type ty))
	seqchar->setType(gap);
	TEST_EQUAL(seqchar->getChar(), '-');
RESULT

//CHECKS wether setting the type back to CHAR throws an Exception
CHECK(setType(SequenceCharacter::type ty))
	TEST_EXCEPTION(Exception::InvalidArgument,seqchar->setType(ch));
RESULT


END_TEST
