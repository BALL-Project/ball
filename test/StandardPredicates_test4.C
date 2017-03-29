// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

// insert includes here
#include <BALL/KERNEL/standardPredicates.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/PDBAtom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/residue.h>
#include <BALL/FORMAT/HINFile.h>

///////////////////////////

START_TEST(standardPredicates)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

///  insert tests for each member function here         
///

Atom atom;
atom.setName("Grmpfl.");

// =====================================================
// Ringfinder
// =====================================================

RingFinder* finder = 0;
CHECK(RingFinder() throw())
	finder = new RingFinder;
	TEST_NOT_EQUAL(finder, 0)
RESULT

CHECK(~RingFinder() throw())
	delete finder;
RESULT

CHECK(RingFinder(Size n) throw())
	RingFinder f(6);
RESULT

CHECK(bool operator () (const Atom& atom) throw())
	RingFinder f;
	f(atom);
RESULT

CHECK(const HashSet<const Bond*>& getVisitedBonds() const throw())
	RingFinder f(6);
	TEST_EQUAL(f.getVisitedBonds().size(), 0)
RESULT

CHECK(const std::vector<const Atom*>& getRingAtoms() const throw())
	RingFinder f(6);
	TEST_EQUAL(f.getRingAtoms().size(), 0)
RESULT

CHECK(bool dfs(const Atom& atom, const Size limit) throw())
	RingFinder f(5);
	TEST_EQUAL(f.dfs(atom, 1), false)
RESULT

CHECK(void setRingSize(Size n) throw())
	RingFinder f;
	f.setRingSize(1);
RESULT

// =====================================================
// not to be tested
// =====================================================
CHECK(::std::list<CTPNode*>& getChildren() throw())
// Not to be tested
RESULT

CHECK(CTPNode() throw())
// Not to be tested
RESULT

CHECK(CTPNode(const CTPNode& node) throw())
// Not to be tested
RESULT

CHECK(CTPNode* getParent() const throw())
// Not to be tested
RESULT

CHECK(ConstIterator begin() const throw())
// Not to be tested
RESULT

CHECK(ConstIterator end() const throw())
// Not to be tested
RESULT

CHECK(Iterator begin() throw())
// Not to be tested
RESULT

CHECK(Iterator end() throw())
// Not to be tested
RESULT

CHECK(Size getBondType() const throw())
// Not to be tested
RESULT

CHECK(Size getNumberOfChildren() const throw())
// Not to be tested
RESULT

CHECK(String getSymbol() const throw())
// Not to be tested
RESULT

CHECK(bool isFinished() const throw())
// Not to be tested
RESULT

CHECK(bool isLinked() const throw())
// Not to be tested
RESULT

CHECK(bool operator () (const Atom& atom) const throw())
// Not to be tested
RESULT

CHECK(char getBondTypeChar() const throw())
// Not to be tested
RESULT

CHECK(const HashSet<const CTPNode*>& getLinkSet() const throw())
// Not to be tested
RESULT

CHECK(void addChild(CTPNode* child) throw())
// Not to be tested
RESULT

CHECK(void clear() throw())
// Not to be tested
RESULT

CHECK(void destroy() throw())
// Not to be tested
RESULT

CHECK(void dump() const throw())
// Not to be tested
RESULT

CHECK(void dump(const CTPNode* current) const throw())
// Not to be tested
RESULT

CHECK(void linkWith(CTPNode* partner) throw())
// Not to be tested
RESULT

CHECK(void removeChild(CTPNode* child) throw())
// Not to be tested
RESULT

CHECK(void setArgument(const String& argument) throw())
// Not to be tested
RESULT

CHECK(void setBondType(Size type) throw())
// Not to be tested
RESULT

CHECK(void setBondType(char type) throw())
// Not to be tested
RESULT

CHECK(void setFinished() throw())
// Not to be tested
RESULT

CHECK(void setLinked() throw())
// Not to be tested
RESULT

CHECK(void setParent(CTPNode* parent) throw())
// Not to be tested
RESULT

CHECK(void setSymbol(const String& symbol) throw())
// Not to be tested
RESULT

CHECK(void unsetFinished() throw())
// Not to be tested
RESULT

CHECK(void unsetLinked() throw())
// Not to be tested
RESULT

CHECK(~CTPNode() throw())
// Not to be tested
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
