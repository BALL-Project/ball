// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/STRUCTURE/rotamerLibrary.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/STRUCTURE/residueChecker.h>
#include <BALL/STRUCTURE/geometricProperties.h>
#include <BALL/STRUCTURE/defaultProcessors.h>
#include <BALL/KERNEL/system.h>

///////////////////////////

START_TEST(RotamerLibrary)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

FragmentDB frag_db("fragments/Fragments.db");

RotamerLibrary* rl_ptr = 0;
CHECK(RotamerLibrary::RotamerLibrary())	
	rl_ptr = new RotamerLibrary;
	TEST_NOT_EQUAL(rl_ptr, 0)
  ABORT_IF(rl_ptr == 0)
	TEST_EQUAL(rl_ptr->getNumberOfRotamers(), 466829)
RESULT


CHECK(RotamerLibrary::RotamerLibrary(const String& filename, const FragmentDB& fragment_db))
	RotamerLibrary rl("rotamers/bbind99.Aug.lib", frag_db);

	TEST_EQUAL(rl.getNumberOfRotamers(), 320)
RESULT

CHECK(RotamerLibrary::RotamerLibrary(const FragmentDB& fragment_db))
	RotamerLibrary rl(frag_db);
	TEST_EQUAL(rl.getNumberOfRotamers(), 0)
RESULT



RotamerLibrary rl_ind("rotamers/bbind99.Aug.lib", frag_db);
CHECK(RotamerLibrary::RotamerLibrary(const RotamerLibrary& rotamer_library))	
	RotamerLibrary rl2(rl_ind);
	TEST_EQUAL(rl2.getNumberOfRotamers(), 320)
	// Make sure the old stuff hasn't changed
	TEST_EQUAL(rl_ind.getNumberOfRotamers(), 320)
RESULT

CHECK(RotamerLibrary& RotamerLibrary::operator = (const RotamerLibrary& rotamer_library))
	RotamerLibrary rl2;
	rl2 = rl_ind;
	TEST_EQUAL(rl2.getNumberOfRotamers(), 320)
	// Make sure the old stuff hasn't changed
	TEST_EQUAL(rl_ind.getNumberOfRotamers(), 320)
RESULT

CHECK(ResidueRotamerSet* getRotamerSet(const String& name))
  ResidueRotamerSet* rrs = rl_ind.getRotamerSet("LYS");
  TEST_NOT_EQUAL(rrs, 0)
  ABORT_IF(rrs == 0)
  TEST_EQUAL(rrs->getNumberOfRotamers(), 81)
RESULT

CHECK(ResidueRotamerSet* getRotamerSet(const String& name, float phi, float psi))
  ResidueRotamerSet* rrs = rl_ptr->getRotamerSet("LYS", 180, 60);
  TEST_NOT_EQUAL(rrs, 0)
  ABORT_IF(rrs == 0)
  TEST_EQUAL(rrs->getNumberOfRotamers(), 81)
RESULT

CHECK(ResidueRotamerSet* getRotamerSet(const Residue& residue))
  System S;
	HINFile ags_file(BALL_TEST_DATA_PATH(AlaGlySer.hin));
	ags_file >> S;
	ABORT_IF(S.countResidues() != 3)

	Residue& ser(*++(++S.beginResidue()));
  ResidueRotamerSet* rrs = rl_ptr->getRotamerSet(ser);
  TEST_NOT_EQUAL(rrs, 0)
  ABORT_IF(rrs == 0)
  TEST_EQUAL(rrs->getNumberOfRotamers(), 3)
RESULT


CHECK(Size RotamerLibrary::getNumberOfRotamers() const)
	TEST_EQUAL(rl_ind.getNumberOfRotamers(), 320)
RESULT

CHECK(Size RotamerLibrary::getNumberOfRotamers(const String& name) const)
	TEST_EQUAL(rl_ind.getNumberOfRotamers("LYS"), 81)
RESULT

CHECK(Size RotamerLibrary::getNumberOfRotamerSets() const)
	TEST_EQUAL(rl_ind.getNumberOfRotamerSets(), 18)
RESULT

CHECK(void RotamerLibrary::addRotamer(const String& name, const Rotamer& rotamer, Size number_of_torsions, Index phi, Index psi))
	TEST_EQUAL(rl_ptr->getNumberOfRotamers(), 466829)
  Rotamer rotamer;
  rl_ptr->addRotamer("SER", rotamer, 1, -60, 80);
	TEST_EQUAL(rl_ptr->getNumberOfRotamers(), 466830)
RESULT

CHECK(void RotamerLibrary::addRotamer(const String& name, const Rotamer& rotamer, Size number_of_torsions))
	TEST_EQUAL(rl_ind.getNumberOfRotamers(), 320)
  Rotamer rot;
  rl_ind.addRotamer("LYS", rot, 4);
	TEST_EQUAL(rl_ind.getNumberOfRotamers(), 321)
RESULT

CHECK(bool RotamerLibrary::isBackboneDependent() const)
  TEST_EQUAL(rl_ind.isBackboneDependent(), false)
  TEST_EQUAL(rl_ptr->isBackboneDependent(), true)
RESULT

CHECK(void RotamerLibrary::setBackboneDependent(bool dependent))
  TEST_EQUAL(rl_ind.isBackboneDependent(), false)
  rl_ind.setBackboneDependent(true);
  TEST_EQUAL(rl_ind.isBackboneDependent(), true)
  rl_ind.setBackboneDependent(false);
  TEST_EQUAL(rl_ind.isBackboneDependent(), false)
RESULT

CHECK(bool RotamerLibrary::hasRotamers(const String& name) const)
  TEST_EQUAL(rl_ind.hasRotamers("GLY"), false)
  TEST_EQUAL(rl_ind.hasRotamers("TRP"), true)
RESULT

CHECK(bool RotamerLibrary::validate())
  TEST_EQUAL(rl_ind.validate(), true)
  TEST_EQUAL(rl_ptr->validate(), true)
RESULT

CHECK(void RotamerLibrary::sort())
	RotamerLibrary rl("rotamers/bbdep02.May.lib", frag_db);
  ResidueRotamerSet* rrs = rl.getRotamerSet("SER", -180, -140);
  TEST_NOT_EQUAL(rrs, 0)
  ABORT_IF(rrs == 0)
  Rotamer rotamer = rrs->getRotamer(0);
  TEST_REAL_EQUAL(rotamer.P, 0.292117)
  rl.sort();
  rrs = rl.getRotamerSet("SER", -180, -140);
  TEST_NOT_EQUAL(rrs, 0)
  ABORT_IF(rrs == 0)
  rotamer = rrs->getRotamer(0);
  TEST_REAL_EQUAL(rotamer.P, 0.700818)
RESULT

CHECK(void RotamerLibrary::clear())
	RotamerLibrary rl(rl_ind);
	TEST_EQUAL(rl.getNumberOfRotamers(), 321)
  rl.clear();
	TEST_EQUAL(rl.getNumberOfRotamers(), 0)
RESULT

CHECK(RotamerLibrary::~RotamerLibrary())
	delete rl_ptr;
RESULT

ResidueChecker rc(frag_db);
rc.disable(ResidueChecker::OVERLAPPING_ATOMS);

CHECK(Side chain positions for Ser)
	System S;
	HINFile ags_file(BALL_TEST_DATA_PATH(AlaGlySer.hin));
	ags_file >> S;
	ABORT_IF(S.countResidues() != 3)

	Residue& ala(*S.beginResidue());
	Residue& gly(*++S.beginResidue());
	Residue& ser(*++(++S.beginResidue()));

	TEST_EQUAL(ala.countAtoms(), 12)
	TEST_EQUAL(gly.countAtoms(), 7)
	TEST_EQUAL(ser.countAtoms(), 12)

	ResidueRotamerSet rrs(ala, 1);
	TEST_EQUAL(rrs.getNumberOfTorsions(), 0)
	TEST_EQUAL(rrs.getNumberOfRotamers(), 0)

	ResidueRotamerSet rrs_gly(gly, 1);
	TEST_EQUAL(rrs_gly.getNumberOfTorsions(), 0)
	TEST_EQUAL(rrs_gly.getNumberOfRotamers(), 0)

	ResidueRotamerSet rrs_ser(ser, 1);
	TEST_EQUAL(rrs_ser.getNumberOfTorsions(), 1)
	TEST_EQUAL(rrs_ser.getNumberOfRotamers(), 0)

	Rotamer r = rrs_ser.getRotamer(ser);
	PRECISION(1E-1)
	TEST_REAL_EQUAL(r.P, 1.0)
	TEST_REAL_EQUAL(r.chi1, 179.941)
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
	HINFile ags_file(BALL_TEST_DATA_PATH(GPG.hin));
	ags_file >> S;
	ABORT_IF(S.countResidues() != 3)

	Residue& gly1(*S.beginResidue());
	Residue& pro(*++S.beginResidue());
	Residue& gly2(*++(++S.beginResidue()));

	TEST_EQUAL(gly1.countAtoms(), 9)
	TEST_EQUAL(pro.countAtoms(), 14)
	TEST_EQUAL(gly2.countAtoms(), 8)

	ResidueRotamerSet rrs_gly1(gly1, 1);
	TEST_EQUAL(rrs_gly1.getNumberOfTorsions(), 0)
	TEST_EQUAL(rrs_gly1.getNumberOfRotamers(), 0)

	ResidueRotamerSet rrs_gly2(gly2, 1);
	TEST_EQUAL(rrs_gly2.getNumberOfTorsions(), 0)
	TEST_EQUAL(rrs_gly2.getNumberOfRotamers(), 0)

	ResidueRotamerSet rrs_pro(pro, 2);
	TEST_EQUAL(rrs_pro.getNumberOfTorsions(), 2)
	TEST_EQUAL(rrs_pro.getNumberOfRotamers(), 0)

	Rotamer r = rrs_pro.getRotamer(pro);
	PRECISION(1E-3)
	TEST_REAL_EQUAL(r.P, 1.0)
	TEST_REAL_EQUAL(r.chi1,  Angle(0.557364).toDegree())
	TEST_REAL_EQUAL(r.chi2, Angle(-0.637879).toDegree())
	TEST_REAL_EQUAL(r.chi3,  0.0)
	TEST_REAL_EQUAL(r.chi4,  0.0)

	S.apply(rc);
	TEST_EQUAL(rc.getStatus(), true)		

	Rotamer r1;
	r1.chi1 = 25.9;
	r1.chi2 = -44.0;
	
	rrs_pro.setRotamer(pro, r1);
	Rotamer r1_r = rrs_pro.getRotamer(pro);
	TEST_REAL_EQUAL(r1_r.chi1, r1.chi1)
	TEST_REAL_EQUAL(r1_r.chi2, r1.chi2)
	Angle c1 = calculateTorsionAngle(*pro.getAtom("N"), *pro.getAtom("CA"), *pro.getAtom("CB"), *pro.getAtom("CG"));
	Angle c2 = calculateTorsionAngle(*pro.getAtom("CA"), *pro.getAtom("CB"), *pro.getAtom("CG"), *pro.getAtom("CD"));
	TEST_REAL_EQUAL(c1.toDegree(), r1.chi1)
	TEST_REAL_EQUAL(c2.toDegree(), r1.chi2)		
	S.apply(rc);
	TEST_EQUAL(rc.getStatus(), true)		
	String outfile_name;
	NEW_TMP_FILE(outfile_name)


	HINFile outfile(outfile_name, std::ios::out);
	outfile << S;
	outfile.close();
	

	Rotamer r2;
	r2.chi1 = -23.7;
	r2.chi2 = 39.0;
	
	rrs_pro.setRotamer(pro, r2);
	Rotamer r2_r = rrs_pro.getRotamer(pro);
	TEST_REAL_EQUAL(r2_r.chi1, r2.chi1)
	TEST_REAL_EQUAL(r2_r.chi2, r2.chi2)
	c1 = calculateTorsionAngle(*pro.getAtom("N"), *pro.getAtom("CA"), *pro.getAtom("CB"), *pro.getAtom("CG"));
	c2 = calculateTorsionAngle(*pro.getAtom("CA"), *pro.getAtom("CB"), *pro.getAtom("CG"), *pro.getAtom("CD"));
	TEST_REAL_EQUAL(c1.toDegree(), r2.chi1)
	TEST_REAL_EQUAL(c2.toDegree(), r2.chi2)		
	S.apply(rc);
	TEST_EQUAL(rc.getStatus(), true)		

	NEW_TMP_FILE(outfile_name)
	outfile.open(outfile_name, std::ios::out);
	outfile << S;

RESULT

CHECK(Side chain conformations for Dunbrack library)
	System S;
	HINFile infile(BALL_TEST_DATA_PATH(all_amino.hin));
	infile >> S;
	infile.close();
	TEST_EQUAL(S.countResidues(), 20)
	ClearChargeProcessor cc;

	// Remove all charges -- they are somewhat bogus and would upset the residue checker.
	S.apply(cc);

	RotamerLibrary rl("rotamers/bbind99.Aug.lib", frag_db);
	TEST_EQUAL(rl.getNumberOfRotamers(), 320)
	ABORT_IF((rl.getNumberOfRotamers() != 320) || (S.countResidues() != 20))

	ResidueIterator res_it(S.beginResidue());
	for (; +res_it; ++res_it)
	{
		STATUS("checking rotamers for " << res_it->getName())
		ResidueRotamerSet* rss = rl.getRotamerSet(*res_it);
		if (rss != 0)
		{
			// needed if the Residue is different from standard from fragment DB
			rss->setTemplateResidue(*res_it, rss->getNumberOfTorsions());

			STATUS("  - " << rss->getNumberOfRotamers() << " rotamers")
			for (Position i = 0; i < rss->getNumberOfRotamers(); i++)
			{
				rss->setRotamer(*res_it, rss->getRotamer(i));
				res_it->apply(rc);
				TEST_EQUAL(rc.getStatus(), true)

				// Store suspicious conformations for subsequent debugging.
				if (!rc.getStatus())
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
