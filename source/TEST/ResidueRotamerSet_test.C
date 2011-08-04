// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/STRUCTURE/residueRotamerSet.h>
#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/KERNEL/system.h>

///////////////////////////

START_TEST(RotamerLibrary)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

FragmentDB frag_db("fragments/Fragments.db");

Rotamer* rot_ptr = 0;
CHECK(Rotamer::Rotamer())
	rot_ptr = new Rotamer;
	TEST_NOT_EQUAL(rot_ptr, 0)
	ABORT_IF(rot_ptr == 0)
	TEST_REAL_EQUAL(rot_ptr->P, 0.0)
	TEST_REAL_EQUAL(rot_ptr->chi1, 0.0)
	TEST_REAL_EQUAL(rot_ptr->chi2, 0.0)
	TEST_REAL_EQUAL(rot_ptr->chi3, 0.0)
	TEST_REAL_EQUAL(rot_ptr->chi4, 0.0)
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
	TEST_REAL_EQUAL(r2.chi1, r.chi1)
	TEST_REAL_EQUAL(r2.chi2, r.chi2)
	TEST_REAL_EQUAL(r2.chi3, r.chi3)
	TEST_REAL_EQUAL(r2.chi4, r.chi4)
	TEST_REAL_EQUAL(r2.P, r.P)
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

CHECK(ResidueRotamerSet::ResidueRotamerSet(const ResidueRotamerSet& rotamer_set))
	ResidueRotamerSet r;
	ResidueRotamerSet r2(r);
RESULT

CHECK(ResidueRotamerSet::ResidueRotamerSet(const Residue& residue, Size number_of_torsions))
  Residue ser(*frag_db.getResidue("SER"));
  ResidueRotamerSet r(ser, 1);
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

Residue ser(*frag_db.getResidue("SER"));
ResidueRotamerSet rrs(ser, 1);

CHECK(void ResidueRotamerSet::addRotamer(const Rotamer& rotamer))	
  TEST_EQUAL(rrs.getNumberOfRotamers(), 0)
  Rotamer rotamer(0.2, -80., 0, 0, 0);
  rrs.addRotamer(rotamer);
  Rotamer rotamer2(0.4, 50.2, 0, 0, 0);
  rrs.addRotamer(rotamer2);
  TEST_EQUAL(rrs.getNumberOfRotamers(), 2)
RESULT

CHECK(const ResidueRotamerSet::operator = (const ResidueRotamerSet& rhs))
  ResidueRotamerSet copy_of_rrs;
  TEST_EQUAL(copy_of_rrs.getNumberOfRotamers(), 0)
  copy_of_rrs = rrs;
  TEST_EQUAL(copy_of_rrs.getNumberOfRotamers(), 2)
RESULT


CHECK(Rotamer& ResidueRotamerSet::operator [] (Position index) throw(Exception::IndexOverflow))
  TEST_REAL_EQUAL(rrs[0].chi1, -80.)
	TEST_REAL_EQUAL(rrs[1].P, 0.4)
RESULT

CHECK(const Rotamer& ResidueRotamerSet::getRotamer(Position index))	
  TEST_REAL_EQUAL(rrs.getRotamer(0).chi1, -80)
	TEST_REAL_EQUAL(rrs.getRotamer(1).P, 0.4)
RESULT

CHECK(Rotamer ResidueRotamerSet::getRotamer(const Residue& residue))	
  System S;
  HINFile ags_file(BALL_TEST_DATA_PATH(AlaGlySer.hin));
	ags_file >> S;
	ABORT_IF(S.countResidues() != 3)

	Residue& serine(*++(++S.beginResidue()));
  Rotamer rotamer = rrs.getRotamer(serine);
  TEST_NOT_EQUAL(rotamer.chi1, 0)
RESULT

CHECK(bool ResidueRotamerSet::setRotamer(Residue& residue, const Rotamer& rotamer))	
  Rotamer original(1.0, 75.0, -40.0, 0, 0);
  rrs.setRotamer(ser, original);
  Rotamer set_rotamer = rrs.getRotamer(ser);
  PRECISION(0.001)
  TEST_REAL_EQUAL(set_rotamer.chi1, original.chi1)
  // Serine only has chi1
  TEST_REAL_EQUAL(set_rotamer.chi2, 0.)
RESULT

CHECK(bool ResidueRotamerSet::hasTorsionPhi())
  TEST_EQUAL(rrs.hasTorsionPhi(), false);
RESULT

CHECK(Angle ResidueRotamerSet::getTorsionPhi() const)
  TEST_REAL_EQUAL(rrs.getTorsionPhi().toDegree(), 0.)
RESULT

CHECK(void ResidueRotamerSet::setTorsionPhi(const Angle& phi))
  rrs.setTorsionPhi(Angle(60, false));
  TEST_REAL_EQUAL(rrs.getTorsionPhi().toDegree(), 60.)
RESULT

CHECK(bool ResidueRotamerSet::hasTorsionPsi())
  TEST_EQUAL(rrs.hasTorsionPsi(), false);
RESULT

CHECK(Angle ResidueRotamerSet::getTorsionPsi() const)
  TEST_REAL_EQUAL(rrs.getTorsionPsi().toDegree(), 0.)
RESULT

CHECK(void ResidueRotamerSet::setTorsionPsi(const Angle& psi))
  rrs.setTorsionPsi(Angle(-60, false));
  TEST_REAL_EQUAL(rrs.getTorsionPsi().toDegree(), -60.)
RESULT

CHECK(bool ResidueRotamerSet::setTemplateResidue(const Residue& residue, Size number_of_torsions))
  System S;
  HINFile ags_file(BALL_TEST_DATA_PATH(AlaGlySer.hin));
	ags_file >> S;
	ABORT_IF(S.countResidues() != 3)

	Residue& gly(*++S.beginResidue());
  
  ResidueRotamerSet copy_of_rrs(rrs);
  TEST_EQUAL(copy_of_rrs.getName(), "SER")
  copy_of_rrs.setTemplateResidue(gly, 0);
  TEST_EQUAL(copy_of_rrs.getName(), "GLY")
RESULT

CHECK(void ResidueRotamerSet::deleteRotamer(Iterator loc))
  ResidueRotamerSet copy_of_rrs(rrs);
  TEST_EQUAL(copy_of_rrs.getNumberOfRotamers(), 2)
  copy_of_rrs.deleteRotamer(copy_of_rrs.begin());
  TEST_EQUAL(copy_of_rrs.getNumberOfRotamers(), 1)
RESULT
		
CHECK(void ResidueRotamerSet::deleteRotamers(Iterator begin, Iterator end))
  ResidueRotamerSet copy_of_rrs(rrs);
  TEST_EQUAL(copy_of_rrs.getNumberOfRotamers(), 2)
  copy_of_rrs.deleteRotamers(copy_of_rrs.begin(), copy_of_rrs.end());
  TEST_EQUAL(copy_of_rrs.getNumberOfRotamers(), 0)
RESULT

CHECK(void ResidueRotamerSet::sort())
  ResidueRotamerSet copy_of_rrs(rrs);
  TEST_REAL_EQUAL(copy_of_rrs[0].P, 0.2)
  copy_of_rrs.sort();
  TEST_REAL_EQUAL(copy_of_rrs[0].P, 0.4)
RESULT

CHECK(ResidueRotamerSet::begin())
  Rotamer rotamer = rrs[0];
  ResidueRotamerSet::Iterator iter = rrs.begin();
  TEST_REAL_EQUAL((*iter).P, rotamer.P)
RESULT

CHECK(ResidueRotamerSet::end())
  ResidueRotamerSet::Iterator begin = rrs.begin();
  ResidueRotamerSet::Iterator end = rrs.end();
  TEST_EQUAL(end-begin, 2)
RESULT

CHECK(ResidueRotamerSet::begin() const)
  Rotamer rotamer = rrs[0];
  ResidueRotamerSet::Iterator iter = rrs.begin();
  TEST_REAL_EQUAL((*iter).P, rotamer.P)
RESULT

CHECK(ResidueRotamerSet::end() const)
  ResidueRotamerSet::ConstIterator begin = rrs.begin();
  ResidueRotamerSet::ConstIterator end = rrs.end();
  TEST_EQUAL(end-begin, 2)
RESULT

CHECK(ResidueRotamerSet::ResidueRotamerSet(const ResidueRotamerSet& rotamer_set))
	ResidueRotamerSet r;
	ResidueRotamerSet r2(r);
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
