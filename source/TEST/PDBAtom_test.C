// $Id: PDBAtom_test.C,v 1.1 2001/06/24 10:45:25 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/KERNEL/PDBAtom.h>
#include <BALL/KERNEL/chain.h>
#include <BALL/KERNEL/protein.h>
#include <BALL/KERNEL/residue.h>
///////////////////////////

START_TEST(class_name, "$Id: PDBAtom_test.C,v 1.1 2001/06/24 10:45:25 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

// tests for class PDBAtom::

CHECK(PDBAtom::BALL_CREATE_DEEP(PDBAtom))
  //BAUSTELLE
RESULT


CHECK(PDBAtom::bool operator == (const PDBAtom& pdb_atom) const  throw())
  //BAUSTELLE
RESULT


CHECK(PDBAtom::bool operator != (const PDBAtom& pdb_atom) const  throw())
  //BAUSTELLE
RESULT


CHECK(PDBAtom::persistentWrite(PersistenceManager& pm, const char* name = 0) const  throw())
  //BAUSTELLE
RESULT


CHECK(PDBAtom::persistentRead(PersistenceManager& pm) throw())
  //BAUSTELLE
RESULT


CHECK(PDBAtom::set(const PDBAtom& pdb_atom, bool deep = true) throw())
  //BAUSTELLE
RESULT


CHECK(PDBAtom::get(PDBAtom& pdb_atom, bool deep = true) const  throw())
  //BAUSTELLE
RESULT


CHECK(PDBAtom::PDBAtom& operator = (const PDBAtom& pdb_atom) throw())
  //BAUSTELLE
RESULT


CHECK(PDBAtom::swap(PDBAtom& pdb_atom) throw())
  //BAUSTELLE
RESULT


CHECK(PDBAtom::getProtein() throw())
  //BAUSTELLE
RESULT


CHECK(PDBAtom::getProtein() const  throw())
  //BAUSTELLE
RESULT


CHECK(PDBAtom::getChain() throw())
  //BAUSTELLE
RESULT


CHECK(PDBAtom::getChain() const  throw())
  //BAUSTELLE
RESULT


CHECK(PDBAtom::getResidue() throw())
  //BAUSTELLE
RESULT


CHECK(PDBAtom::getResidue() const  throw())
  //BAUSTELLE
RESULT


CHECK(PDBAtom::setBranchDesignator(char branch_designator) throw())
  //BAUSTELLE
RESULT


CHECK(PDBAtom::getBranchDesignator() const  throw())
  //BAUSTELLE
RESULT


CHECK(PDBAtom::setRemotenessIndicator(char remoteness_indicator) throw())
  //BAUSTELLE
RESULT


CHECK(PDBAtom::getRemotenessIndicator() const  throw())
  //BAUSTELLE
RESULT


CHECK(PDBAtom::setAlternateLocationIndicator(char alternate_location_indicator) throw())
  //BAUSTELLE
RESULT


CHECK(PDBAtom::getAlternateLocationIndicator() const  throw())
  //BAUSTELLE
RESULT


CHECK(PDBAtom::setOccupancy(float occupancy) throw())
  //BAUSTELLE
RESULT


CHECK(PDBAtom::getOccupancy() const  throw())
  //BAUSTELLE
RESULT


CHECK(PDBAtom::setTemperatureFactor(float temperature_factor) throw())
  //BAUSTELLE
RESULT


CHECK(PDBAtom::getTemperatureFactor() const  throw())
  //BAUSTELLE
RESULT


CHECK(PDBAtom::dump(std::ostream& s = std::cout, Size depth = 0) const  throw())
  //BAUSTELLE
RESULT


CHECK(PDBAtom::read(std::istream& s) throw())
  //BAUSTELLE
RESULT


CHECK(PDBAtom::write(std::ostream& s) const  throw())
  //BAUSTELLE
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST

