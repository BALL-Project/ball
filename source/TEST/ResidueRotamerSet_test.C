// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: ResidueRotamerSet_test.C,v 1.1.2.1 2007/04/02 21:02:08 bertsch Exp $
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////

//#include <BALL/STRUCTURE/rotamerLibrary.h>
#include <BALL/STRUCTURE/residueRotamerSet.h>
//#include <BALL/FORMAT/HINFile.h>
//#include <BALL/SYSTEM/file.h>
//#include <BALL/STRUCTURE/fragmentDB.h>
//#include <BALL/STRUCTURE/residueChecker.h>
//#include <BALL/STRUCTURE/geometricProperties.h>
//#include <BALL/STRUCTURE/defaultProcessors.h>
//#include <BALL/KERNEL/system.h>

///////////////////////////

START_TEST(RotamerLibrary, "$Id: ResidueRotamerSet_test.C,v 1.1.2.1 2007/04/02 21:02:08 bertsch Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

FragmentDB frag_db("fragments/Fragments.db");

Rotamer* rot_ptr = 0;
CHECK(Rotamer::Rotamer())
	rot_ptr = new Rotamer;
	TEST_NOT_EQUAL(rot_ptr, 0)
	ABORT_IF(rot_ptr == 0)
	TEST_EQUAL(rot_ptr->P, 0.0)
	TEST_EQUAL(rot_ptr->chi1, 0.0)
	TEST_EQUAL(rot_ptr->chi2, 0.0)
	TEST_EQUAL(rot_ptr->chi3, 0.0)
	TEST_EQUAL(rot_ptr->chi4, 0.0)
RESULT

CHECK(Rotamer::~Rotamer())
	delete rot_ptr;
RESULT											

CHECK(Rotamer::Rotamer(const Rotamer& rotamer))
	Rotamer r;
	r.P = 1.0;
	r.chi1 = Angle(2.1).toDegree();
	r.chi2 = Angle(3.2).toDegree();
	r.chi3 = Angle(4.3).toDegree();
	r.chi4 = Angle(5.4).toDegree();

	Rotamer r2(r);
	TEST_EQUAL(r2.chi1, r.chi1)
	TEST_EQUAL(r2.chi2, r.chi2)
	TEST_EQUAL(r2.chi3, r.chi3)
	TEST_EQUAL(r2.chi4, r.chi4)
	TEST_EQUAL(r2.P, r.P)
RESULT


CHECK(Rotamer::Rotamer(float P, float, chi1, float, chi2, float chi3, float chi4))
	Rotamer r(1.0, 2.0, 3.0, 4.0, 5.0);
	TEST_REAL_EQUAL(r.chi1, 2.0)
	TEST_REAL_EQUAL(r.chi2, 3.0)
	TEST_REAL_EQUAL(r.chi3, 4.0)
	TEST_REAL_EQUAL(r.chi4, 5.0)
	TEST_REAL_EQUAL(r.P, 1.0)
RESULT

ResidueRotamerSet* rrs_ptr = 0;
CHECK(ResidueRotamerSet::ResidueRotamerSet())	
	rrs_ptr = new ResidueRotamerSet;
	TEST_NOT_EQUAL(rrs_ptr, 0)
RESULT

CHECK(ResidueRotamerSet::~ResidueRotamerSet())
	delete rrs_ptr;
RESULT

CHECK(ResidueRotamerSet::ResidueRotamerSet(const Residue& residue, Size number_of_torsions))
	ResidueRotamerSet r;
	ResidueRotamerSet r2(r);
RESULT

CHECK(bool ResidueRotamerSet::isValid())
	ResidueRotamerSet r;
	TEST_EQUAL(r.isValid(), false)
RESULT

CHECK(const String& ResidueRotamerSet::getName() const)
	ResidueRotamerSet r;
	TEST_EQUAL(r.getName(), "")
RESULT

CHECK(void ResidueRotamerSet::setName(const String& name))
	ResidueRotamerSet r;
	TEST_EQUAL(r.getName(), "")
	r.setName("asdf");
	TEST_EQUAL(r.getName(), "asdf")
	r.setName("");
	TEST_EQUAL(r.getName(), "")
RESULT

CHECK(Size ResidueRotamerSet::getNumberOfRotamers() const)
	ResidueRotamerSet r;
	TEST_EQUAL(r.getNumberOfRotamers(), 0)
RESULT

CHECK(Size ResidueRotamerSet::getNumberOfTorsions() const)
	ResidueRotamerSet r;
	TEST_EQUAL(r.getNumberOfTorsions(), 0)
RESULT

CHECK(void ResidueRotamerSet::setNumberOfTorsions(Size number_of_torsions) throw(Exception::IndexOverflow))
	ResidueRotamerSet r;
	TEST_EQUAL(r.getNumberOfTorsions(), 0)
	r.setNumberOfTorsions(1);
	TEST_EQUAL(r.getNumberOfTorsions(), 1)
	r.setNumberOfTorsions(2);
	TEST_EQUAL(r.getNumberOfTorsions(), 2)
	r.setNumberOfTorsions(3);
	TEST_EQUAL(r.getNumberOfTorsions(), 3)
	r.setNumberOfTorsions(4);
	TEST_EQUAL(r.getNumberOfTorsions(), 4)
	r.setNumberOfTorsions(0);
	TEST_EQUAL(r.getNumberOfTorsions(), 0)
	TEST_EXCEPTION(Exception::IndexOverflow, r.setNumberOfTorsions(5))
	TEST_EXCEPTION(Exception::IndexOverflow, r.setNumberOfTorsions(6))
RESULT

/*
CHECK(const Residue& ResidueRotamerSet::getResidue() const)
	ResidueRotamerSet r;
	const ResidueRotamerSet& c_r(r);
	TEST_NOT_EQUAL(&(c_r.getResidue()), 0)
	TEST_EQUAL(c_r.getResidue().countAtoms(), 0)
RESULT

CHECK(Residue& ResidueRotamerSet::getResidue())
	ResidueRotamerSet r;
	TEST_EQUAL(r.getResidue().countAtoms(), 0)
	r.getResidue().insert(*new PDBAtom);
	TEST_EQUAL(r.getResidue().countAtoms(), 1)
RESULT
*/

CHECK(const ResidueRotamerSet::operator = (const ResidueRotamerSet& rhs))
	// ????
RESULT

CHECK(Rotamer& ResidueRotamerSet::operator [] (Position index) throw(Exception::IndexOverflow))
	// ????
RESULT

CHECK(bool setRotamer(Residue& residue, const Rotamer& rotamer))	
	// ????
RESULT

CHECK(Rotamer getRotamer(const Residue& residue))	
	// ????
RESULT

CHECK(const Rotamer& getRotamer(Position index))	
	// ????
RESULT

CHECK(void addRotamer(const Rotamer& rotamer))	
	// ????
RESULT

CHECK(ResidueRotamerSet::begin())
	// ???
RESULT

CHECK(ResidueRotamerSet::end())
	// ???
RESULT

CHECK(ResidueRotamerSet::begin() const)
	// ???
RESULT

CHECK(ResidueRotamerSet::end() const)
	// ???
RESULT

CHECK(ResidueRotamerSet::ResidueRotamerSet(const ResidueRotamerSet& rotamer_set))
	ResidueRotamerSet r;
	ResidueRotamerSet r2(r);
RESULT

CHECK(Residue* ResidueRotamerSet::buildRotamer(const Rotamer& rotamer))
	// ???
RESULT

CHECK(void ResidueRotamerSet::resetResidue())
	// ????
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
