// $Id: SolventDescriptor_test.C,v 1.3 2001/07/15 17:32:41 amoll Exp $
#include <BALL/CONCEPT/classTest.h>
 
///////////////////////////
#include <BALL/SOLVATION/solventDescriptor.h>
///////////////////////////
	  
START_TEST(SolventDescriptor, "$Id: SolventDescriptor_test.C,v 1.3 2001/07/15 17:32:41 amoll Exp $")
		 
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
		  
using namespace BALL;
			 
SolventDescriptor* s_ptr;
CHECK(Cstr)
	s_ptr = new SolventDescriptor;
	TEST_NOT_EQUAL(s_ptr, 0)
	TEST_EQUAL(s_ptr->getName(), "")
	TEST_EQUAL(s_ptr->getNumberDensity(), 0)
	TEST_EQUAL(s_ptr->getSolventAtomDescriptorList().size(), 0)
	TEST_EQUAL(s_ptr->getNumberOfAtomTypes(), 0)
	SolventAtomDescriptor sad;
	TEST_EXCEPTION(Exception::IndexOverflow, s_ptr->getAtomDescriptor(99))
	TEST_EQUAL(s_ptr->isValid(), false)
RESULT


CHECK(Destructor)
	delete(s_ptr);
RESULT


CHECK(Cpy-Cstr)
	//
RESULT


CHECK(SolventDescriptor(const String& name, float number_density, const std::vector<SolventAtomDescriptor>& atom_list))
	//
RESULT

CHECK(SolventDescriptor::BALL_CREATE(SolventDescriptor))
  //BAUSTELLE
RESULT


CHECK(SolventDescriptor::setName(const String& name) throw())
  //BAUSTELLE
RESULT


CHECK(SolventDescriptor::getName() const  throw())
  //BAUSTELLE
RESULT


CHECK(SolventDescriptor::setNumberDensity(float number_density) throw())
  //BAUSTELLE
RESULT


CHECK(SolventDescriptor::getNumberDensity() const  throw())
  //BAUSTELLE
RESULT


CHECK(SolventDescriptor::setSolventAtomDescriptorList(const std::vector<SolventAtomDescriptor>& solvent_atoms) throw())
  //BAUSTELLE
RESULT


CHECK(SolventDescriptor::getSolventAtomDescriptorList() const  throw())
  //BAUSTELLE
RESULT


CHECK(SolventDescriptor::getNumberOfAtomTypes() const  throw())
  //BAUSTELLE
RESULT


CHECK(SolventDescriptor::getAtomDescriptor(Position index) const  throw())
  //BAUSTELLE
RESULT


CHECK(SolventDescriptor::isValid() const  throw())
  //BAUSTELLE
RESULT


CHECK(SolventDescriptor::bool operator == (const SolventDescriptor& descriptor) const  throw())
  //BAUSTELLE
RESULT

///////////////////////////////
END_TEST
