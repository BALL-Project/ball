// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: RotamerLibrary_test.C,v 1.5 2003/04/28 19:21:27 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/STRUCTURE/rotamerLibrary.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/SYSTEM/file.h>
#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/STRUCTURE/residueChecker.h>
#include <BALL/STRUCTURE/geometricProperties.h>
#include <BALL/STRUCTURE/defaultProcessors.h>
#include <BALL/KERNEL/system.h>

///////////////////////////

START_TEST(RotamerLibrary, "$Id: RotamerLibrary_test.C,v 1.5 2003/04/28 19:21:27 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

FragmentDB frag_db;

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
	r.chi1 = 2.1;
	r.chi2 = 3.2;
	r.chi3 = 4.3;
	r.chi4 = 5.4;

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

RotamerLibrary* rl_ptr = 0;
CHECK(RotamerLibrary::RotamerLibrary())	
	rl_ptr = new RotamerLibrary;
	TEST_NOT_EQUAL(rl_ptr, 0)
RESULT

CHECK(RotamerLibrary::~RotamerLibrary())
	delete rl_ptr;
RESULT

CHECK(RotamerLibrary::RotamerLibrary(const String& filename, const FragmentDB& fragment_db))
	// ???
RESULT

CHECK(RotamerLibrary::RotamerLibrary(const RotamerLibrary& rotamer_library))	
	// ????
RESULT

CHECK(const RotamerLibrary& RotamerLibrary::operator = (const RotamerLibrary& rotamer_library))
	// ???
RESULT

CHECK(bool RotamerLibrary::readSQWRLLibraryFile(const String& filename, const FragmentDB& fragment_db))
	// ???
RESULT

CHECK(ResidueRotamerSet* getRotamerSet(const String& name))
	// ???
RESULT

CHECK(Size RotamerLibrary::getNumberOfVariants() const)
	// ???
RESULT

CHECK(Size RotamerLibrary::getNumberOfRotamers() const)
	// ???
RESULT


ResidueChecker rc(frag_db);
rc.disable(ResidueChecker::OVERLAPPING_ATOMS);

CHECK(Side chain positions for Ser)
	System S;
	HINFile ags_file("data/AlaGlySer.hin");
	ags_file >> S;
	ABORT_IF(S.countResidues() != 3)

	Residue& ala(*S.beginResidue());
	Residue& gly(*++S.beginResidue());
	Residue& ser(*++(++S.beginResidue()));

	TEST_EQUAL(ala.countAtoms(), 12)
	TEST_EQUAL(gly.countAtoms(), 7)
	TEST_EQUAL(ser.countAtoms(), 12)

	ResidueRotamerSet rrs(ala, 1);
	TEST_EQUAL(rrs.isValid(), true)
	TEST_EQUAL(rrs.getNumberOfTorsions(), 0)
	TEST_EQUAL(rrs.getNumberOfRotamers(), 0)

	ResidueRotamerSet rrs_gly(gly, 1);
	TEST_EQUAL(rrs_gly.isValid(), true)
	TEST_EQUAL(rrs_gly.getNumberOfTorsions(), 0)
	TEST_EQUAL(rrs_gly.getNumberOfRotamers(), 0)

	ResidueRotamerSet rrs_ser(ser, 1);
	TEST_EQUAL(rrs_ser.isValid(), true)
	TEST_EQUAL(rrs_ser.getNumberOfTorsions(), 1)
	TEST_EQUAL(rrs_ser.getNumberOfRotamers(), 0)
	ABORT_IF(rrs_ser.isValid() == false)

	Rotamer r = rrs_ser.getRotamer(ser);
	TEST_EQUAL(r.P, 1.0)
	PRECISION(1E-3)
	TEST_REAL_EQUAL(r.chi1, 3.14062)
	TEST_REAL_EQUAL(r.chi2, 0.0)
	TEST_REAL_EQUAL(r.chi3, 0.0)
	TEST_REAL_EQUAL(r.chi4, 0.0)
	S.apply(rc);
	TEST_EQUAL(rc.getStatus(), true)

	Rotamer r1;
	r1.chi1 = Angle(0.0);
	
	rrs_ser.setRotamer(ser, r1);
	Rotamer r2 = rrs_ser.getRotamer(ser);
	TEST_REAL_EQUAL(r2.chi1, 0.0)
	Angle a = calculateTorsionAngle(*ser.getAtom("N"), *ser.getAtom("CA"), *ser.getAtom("CB"), *ser.getAtom("OG"));
	TEST_REAL_EQUAL(a, 0.0)
	S.apply(rc);
	TEST_EQUAL(rc.getStatus(), true)		
RESULT

CHECK(Side chain positions for Pro)
	System S;
	HINFile ags_file("data/GPG.hin");
	ags_file >> S;
	ABORT_IF(S.countResidues() != 3)

	Residue& gly1(*S.beginResidue());
	Residue& pro(*++S.beginResidue());
	Residue& gly2(*++(++S.beginResidue()));

	TEST_EQUAL(gly1.countAtoms(), 9)
	TEST_EQUAL(pro.countAtoms(), 14)
	TEST_EQUAL(gly2.countAtoms(), 8)

	ResidueRotamerSet rrs_gly1(gly1, 1);
	TEST_EQUAL(rrs_gly1.isValid(), true)
	TEST_EQUAL(rrs_gly1.getNumberOfTorsions(), 0)
	TEST_EQUAL(rrs_gly1.getNumberOfRotamers(), 0)

	ResidueRotamerSet rrs_gly2(gly2, 1);
	TEST_EQUAL(rrs_gly2.isValid(), true)
	TEST_EQUAL(rrs_gly2.getNumberOfTorsions(), 0)
	TEST_EQUAL(rrs_gly2.getNumberOfRotamers(), 0)

	ResidueRotamerSet rrs_pro(pro, 2);
	TEST_EQUAL(rrs_pro.isValid(), true)
	TEST_EQUAL(rrs_pro.getNumberOfTorsions(), 2)
	TEST_EQUAL(rrs_pro.getNumberOfRotamers(), 0)
	ABORT_IF(rrs_pro.isValid() == false)

	Rotamer r = rrs_pro.getRotamer(pro);
	TEST_EQUAL(r.P, 1.0)
	PRECISION(1E-3)
	TEST_REAL_EQUAL(r.chi1,  0.557364)
	TEST_REAL_EQUAL(r.chi2, -0.637879)
	TEST_REAL_EQUAL(r.chi3,  0.0)
	TEST_REAL_EQUAL(r.chi4,  0.0)

	S.apply(rc);
	TEST_EQUAL(rc.getStatus(), true)		

	Rotamer r1;
	r1.chi1 = Angle( 25.9, false);
	r1.chi2 = Angle(-44.0, false);
	
	rrs_pro.setRotamer(pro, r1);
	Rotamer r1_r = rrs_pro.getRotamer(pro);
	TEST_REAL_EQUAL(r1_r.chi1, r1.chi1)
	TEST_REAL_EQUAL(r1_r.chi2, r1.chi2)
	Angle c1 = calculateTorsionAngle(*pro.getAtom("N"), *pro.getAtom("CA"), *pro.getAtom("CB"), *pro.getAtom("CG"));
	Angle c2 = calculateTorsionAngle(*pro.getAtom("CA"), *pro.getAtom("CB"), *pro.getAtom("CG"), *pro.getAtom("CD"));
	TEST_REAL_EQUAL(c1, r1.chi1)
	TEST_REAL_EQUAL(c2, r1.chi2)		
	S.apply(rc);
	TEST_EQUAL(rc.getStatus(), true)		
	String outfile_name;
	NEW_TMP_FILE(outfile_name)


	float x = 0;
	HINFile outfile(outfile_name, std::ios::out);
	outfile << S;
	outfile.close();
	

	Rotamer r2;
	r2.chi1 = Angle(-23.7, false);
	r2.chi2 = Angle( 39.0, false);
	
	rrs_pro.setRotamer(pro, r2);
	Rotamer r2_r = rrs_pro.getRotamer(pro);
	TEST_REAL_EQUAL(r2_r.chi1, r2.chi1)
	TEST_REAL_EQUAL(r2_r.chi2, r2.chi2)
	c1 = calculateTorsionAngle(*pro.getAtom("N"), *pro.getAtom("CA"), *pro.getAtom("CB"), *pro.getAtom("CG"));
	c2 = calculateTorsionAngle(*pro.getAtom("CA"), *pro.getAtom("CB"), *pro.getAtom("CG"), *pro.getAtom("CD"));
	TEST_REAL_EQUAL(c1, r2.chi1)
	TEST_REAL_EQUAL(c2, r2.chi2)		
	S.apply(rc);
	TEST_EQUAL(rc.getStatus(), true)		

	NEW_TMP_FILE(outfile_name)
	outfile.open(outfile_name, std::ios::out);
	outfile << S;

RESULT

CHECK(side-chain conformations for Dunbrack library)
	System S;
	HINFile infile("data/all_amino.hin");
	infile >> S;
	infile.close();
	TEST_EQUAL(S.countResidues(), 20)
	ClearChargeProcessor cc;

	// Remove all charges -- they are somewhat bogus and would upset the residue checker.
	S.apply(cc);

	RotamerLibrary rl("rotamers/bbind99.Aug.lib", frag_db);
	TEST_EQUAL(rl.getNumberOfVariants(), 57)
	ABORT_IF((rl.getNumberOfVariants() != 57) || (S.countResidues() != 20))

	ResidueIterator res_it(S.beginResidue());
	for (; +res_it; ++res_it)
	{
		STATUS("checking rotamers for " << res_it->getName())
		ResidueRotamerSet* rss = rl.getRotamerSet(res_it->getName());
		if (rss != 0)
		{

			STATUS("  - " << rss->getNumberOfRotamers() << " rotamers")
			for (Position i = 0; i < rss->getNumberOfRotamers(); i++)
			{
				rss->setRotamer(*res_it, rss->getRotamer(i));
				res_it->apply(rc);
				TEST_EQUAL(rc.getStatus(), true)

				// Store suspicious conformations for subsequent debugging.
				if (rc.getStatus() == false)
				{
					String outfile_name;
					NEW_TMP_FILE(outfile_name)
					HINFile outfile(outfile_name, std::ios::out);
					outfile << S;
				}
			}
		}
	}
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
