// $Id: SolventDescriptor_test.C,v 1.1 2000/08/31 16:47:24 anker Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

// insert includes here
#include <BALL/SOLVATION/solventDescriptor.h>

///////////////////////////

START_TEST(class_name, "$Id: SolventDescriptor_test.C,v 1.1 2000/08/31 16:47:24 anker Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

///  insert tests for each member function here         
///
	

CHECK(SolventDescriptor::SolventDescriptor())
  //BAUSTELLE
RESULT


CHECK(SolventDescriptor::SolventDescriptor(const SolventDescriptor& solvent))
  //BAUSTELLE
RESULT


CHECK(SolventDescriptor::SolventDescriptor(const String& name, float number_density, const std::vector<SolventAtomDescriptor>& atom_list))
  //BAUSTELLE
RESULT


CHECK(SolventDescriptor::~SolventDescriptor())
  //BAUSTELLE
RESULT


CHECK(SolventDescriptor::clear())
  //BAUSTELLE
RESULT


CHECK(SolventDescriptor::destroy())
  //BAUSTELLE
RESULT


CHECK(SolventDescriptor::set(const SolventDescriptor& descriptor))
  //BAUSTELLE
RESULT


CHECK(SolventDescriptor::SolventDescriptor& operator = (const SolventDescriptor& descriptor))
  //BAUSTELLE
RESULT


CHECK(SolventDescriptor::setName(const String& name))
  //BAUSTELLE
RESULT


CHECK(SolventDescriptor::getName() const )
  //BAUSTELLE
RESULT


CHECK(SolventDescriptor::setNumberDensity(float number_density))
  //BAUSTELLE
RESULT


CHECK(SolventDescriptor::getNumberDensity() const )
  //BAUSTELLE
RESULT


CHECK(SolventDescriptor::setSolventAtomDescriptorList(const std::vector<SolventAtomDescriptor>& solvent_atoms))
  //BAUSTELLE
RESULT


CHECK(SolventDescriptor::getSolventAtomDescriptorList() const )
  //BAUSTELLE
RESULT


CHECK(SolventDescriptor::getNumberOfAtomTypes() const )
  //BAUSTELLE
RESULT


CHECK(SolventDescriptor::getAtomDescriptor(Position index) const )
  //BAUSTELLE
RESULT



/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
