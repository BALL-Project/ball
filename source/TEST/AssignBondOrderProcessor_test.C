// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/STRUCTURE/assignBondOrderProcessor.h>
#include <BALL/FORMAT/HINFile.h>
///////////////////////////
using namespace BALL;

START_TEST(PDBAtom, "$Id:$")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////


CHECK(AssignBondOrderProcessor())
  //TODO
RESULT

CHECK(AssignBondOrderProcessor(const AssignBondOrderProcessor& abop))
  //TODO
RESULT

CHECK(~AssignBondOrderProcessor())
  //TODO
RESULT

CHECK(Processor::Result operator () (AtomContainer& ac))
  //TODO
RESULT

CHECK(getNumberOfBondOrdersSet())
  //TODO
RESULT

CHECK(getNumberOfComputedSolutions())
  //TODO
RESULT

CHECK(apply(Position i))
  //TODO
RESULT

CHECK(computeNextSolution())
  //TODO
RESULT

CHECK(AssignBondOrderProcessor& operator = (const AssignBondOrderProcessor& abop))
  //TODO
RESULT

CHECK(setDefaultOptions())
  //TODO
RESULT

CHECK(evaluatePenalty(AtomContainer* ac))
  //TODO
RESULT
