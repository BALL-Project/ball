// $Id: SolventDescriptor_test.C,v 1.4 2001/07/16 15:37:12 sturm Exp $
#include <BALL/CONCEPT/classTest.h>
 
///////////////////////////
#include <BALL/SOLVATION/solventDescriptor.h>
///////////////////////////
	  
START_TEST(SolventDescriptor, "$Id: SolventDescriptor_test.C,v 1.4 2001/07/16 15:37:12 sturm Exp $")
		 
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
		  
using namespace BALL;
			 
SolventDescriptor* s_ptr;
CHECK(Cstr)
	s_ptr = new SolventDescriptor;
	TEST_NOT_EQUAL(s_ptr, 0)
	TEST_EQUAL(s_ptr->getName(), "")
	TEST_EQUAL(s_ptr->getNumberDensity(), 0.0)
	TEST_EQUAL(s_ptr->getNumberOfAtomTypes(), 0)
	TEST_EXCEPTION(Exception::IndexOverflow, s_ptr->getAtomDescriptor(99))
	TEST_EQUAL(s_ptr->isValid(), false)
RESULT


CHECK(Destructor)
	delete(s_ptr);
RESULT

vector<SolventAtomDescriptor> SADVector;
SolventAtomDescriptor SAD;
SAD.element_symbol="L";
SAD.radius=1.999;
SADVector.push_back(SAD);
CHECK(SolventDescriptor(const String& name, float number_density, const std::vector<SolventAtomDescriptor>& atom_list))
	s_ptr = new SolventDescriptor("SolvD1",1.234,SADVector);
	TEST_NOT_EQUAL(s_ptr, 0)
	TEST_EQUAL(s_ptr->getName(), "SolvD1")
	TEST_EQUAL(s_ptr->getNumberDensity(), 1.234)
	TEST_EQUAL(s_ptr->getNumberOfAtomTypes(), 1)
	TEST_EQUAL(s_ptr->isValid(), true)
RESULT

SolventDescriptor* s_ptr2;
CHECK(Cpy-Cstr)
	s_ptr2 = new SolventDescriptor(*s_ptr);
	TEST_NOT_EQUAL(s_ptr2, 0)
	TEST_EQUAL(s_ptr2->getName(), "SolvD1")
	TEST_EQUAL(s_ptr2->getNumberDensity(), 1.234)
	TEST_EQUAL(s_ptr2->getNumberOfAtomTypes(), 1)
	TEST_EQUAL(s_ptr2->isValid(), true)
RESULT

CHECK(SolventDescriptor::clear())
	s_ptr2->clear();
	TEST_NOT_EQUAL(s_ptr2, 0)
	TEST_EQUAL(s_ptr2->getName(), "")
	TEST_EQUAL(s_ptr2->getNumberDensity(),0.0)
	TEST_EQUAL(s_ptr2->getNumberOfAtomTypes(), 0)
	TEST_EQUAL(s_ptr->isValid(), false)
RESULT


CHECK(SolventDescriptor::setName(const String& name) throw())
  s_ptr2->setName("SolvD1");
	TEST_EQUAL(s_ptr2->getName(), "SolvD1");
RESULT

CHECK(SolventDescriptor::setNumberDensity(float number_density) throw())
  s_ptr2->setNumberDensity(1.234);
	TEST_EQUAL(s_ptr2->getNumberDensity(),1.234);
RESULT

CHECK(SolventDescriptor::setSolventAtomDescriptorList(const std::vector<SolventAtomDescriptor>& solvent_atoms) throw())
	s_ptr2->setSolventAtomDescriptorList(SADVector);	
	TEST_EQUAL(s_ptr2->getNumberOfAtomTypes(), 1)
RESULT


CHECK(SolventDescriptor::getSolventAtomDescriptorList() const  throw())
	TEST_EQUAL(s_ptr2->getSolventAtomDescriptorList().size(),1)
RESULT


CHECK(SolventDescriptor::getAtomDescriptor(Position index) const  throw())
  SolventAtomDescriptor SAD2 = s_ptr2->getAtomDescriptor(0);
	TEST_EQUAL(SAD2.radius,1.999)
RESULT

CHECK(SolventDescriptor::bool operator == (const SolventDescriptor& descriptor) const  throw())
  TEST_EQUAL(*s_ptr==*s_ptr2,true)
RESULT

///////////////////////////////
END_TEST
