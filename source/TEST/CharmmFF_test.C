// $Id: CharmmFF_test.C,v 1.1 2000/02/16 19:23:03 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/MOLMEC/CHARMM/charmm.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/STRUCTURE/defaultProcessors.h>
#include <BALL/STRUCTURE/fragmentDB.h>
///////////////////////////

START_TEST(CharmmFF, "$Id: CharmmFF_test.C,v 1.1 2000/02/16 19:23:03 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

CharmmFF* charmm;
CHECK(CharmmFF())
charmm = new CharmmFF();
TEST_NOT_EQUAL(charmm, 0)
RESULT

CHECK(~CharmmFF())
delete charmm;
RESULT

CHECK(CharmmFF(const CharmmFF& force_field, bool clone_deep = true))
CharmmFF a1;
CharmmFF a2(a1);
RESULT
	
CHECK(specificSetup())
CharmmFF a;
a.specificSetup();
RESULT

FragmentDB frag_db;

{
	// read the PDB file containing a single GLY
	PDBFile f("data/CharmmFF_test_1.pdb");	
	System s;
	f >> s;
	f.close();
	// check whether we got the right number of atoms
	TEST_EQUAL(s.countAtoms(), 8)
		
	// normalize the names and build the bonds
	s.apply(frag_db.normalize_names);
	s.apply(frag_db.build_bonds);

	// setup the force field
	Options options;
	options[CharmmFF::Option::FILENAME] = "CHARMM/EEF1/param19_eef1.ini";
	CharmmFF eef1(s, options);

	CHECK(energy test 1 (GLY) [EEF1])
	eef1.updateEnergy();
	#undef PRECISION
	#define PRECISION 5e-3
	TEST_REAL_EQUAL(eef1.getEnergy(), 157.4572251)
	TEST_REAL_EQUAL(eef1.getStretchEnergy(), 30.83411352)
	TEST_REAL_EQUAL(eef1.getBendEnergy(), 307.4544619)
	TEST_REAL_EQUAL(eef1.getProperTorsionEnergy(), 2.11036776)
	TEST_REAL_EQUAL(eef1.getImproperTorsionEnergy(), 5.48237888)
	TEST_REAL_EQUAL(eef1.getVdWEnergy(), 26.64563664)
	TEST_REAL_EQUAL(eef1.getESEnergy(), -53.3215644)
	TEST_REAL_EQUAL(eef1.getSolvationEnergy(), -161.7481263)
	RESULT

	CHECK(gradient test 1 (GLY) [EEF1])
	eef1.updateForces();
	#undef PRECISION
	#define PRECISION 5e-3
	TEST_REAL_EQUAL(eef1.getRMSGradient(), 219.2263702)
	RESULT

	CHECK(force test 1 (GLY, stretch only) [EEF1])
	// remove all components except for stretches
	eef1.removeComponent("CHARMM NonBonded");
	eef1.removeComponent("CHARMM Torsion");
	eef1.removeComponent("CHARMM ImproperTorsion");
	eef1.removeComponent("CHARMM Bend");
	eef1.options[CharmmFF::Option::FILENAME] = "CHARMM/EEF1/param19_eef1.ini";


	eef1.setup(s);

	eef1.updateForces();
	eef1.updateEnergy();

	TEST_REAL_EQUAL(eef1.getEnergy(), 30.83411352)
	TEST_REAL_EQUAL(eef1.getBendEnergy(), 0.0)
	TEST_REAL_EQUAL(eef1.getProperTorsionEnergy(), 0.0)
	TEST_REAL_EQUAL(eef1.getImproperTorsionEnergy(), 0.0)
	TEST_REAL_EQUAL(eef1.getStretchEnergy(), 30.83411352)
	TEST_REAL_EQUAL(eef1.getESEnergy(), 0.0)
	TEST_REAL_EQUAL(eef1.getVdWEnergy(), 0.0)
	TEST_REAL_EQUAL(eef1.getSolvationEnergy(), 0.0)
	TEST_REAL_EQUAL(eef1.getRMSGradient(), 134.2599576)

	Size i = 0;
	Vector3 force;
	AtomIterator atom_it = s.beginAtom();
	for (; +atom_it; ++atom_it, ++i)
	{
		switch (i)
		{
			case 0:	force = Vector3( 26.44796,  91.42712, -23.82851); break;
			case 1:	force = Vector3( 14.55307,   1.84307, -19.22059); break;
			case 2:	force = Vector3(-14.70309, -88.76764,  62.63995); break;
			case 3:	force = Vector3(-16.84492, -12.50059,  -5.06838); break;
			case 4:	force = Vector3( -8.71000,   3.45793, -15.72975); break;
			case 5:	force = Vector3( -1.06629, -17.55152,  -0.53075); break;
			case 6:	force = Vector3( 19.60904,  12.31904,  11.23689); break;
			case 7:	force = Vector3(-19.28575,   9.77259,  -9.49886);
		}
		force = force - atom_it->getForce() * Constants::NA * 1e-13 / -4.184;
		std::cerr << atom_it->getFullName() << " " << atom_it->getTypeName() << " " << atom_it->getType() << " " 
			<< " " << atom_it->countBonds() << atom_it->getForce() * Constants::NA * 1e-13 / -4.184 << std::endl;
		TEST_REAL_EQUAL(force.getLength(), 0.0)
	}
	RESULT

}

CHECK(force test 2 (GLY, bend only) [EEF1])
// read the PDB file containing a single GLY
PDBFile f("data/CharmmFF_test_1.pdb");	
System s;
f >> s;
f.close();
// check whether we got the right number of atoms
TEST_EQUAL(s.countAtoms(), 8)
	
// normalize the names and build the bonds
s.apply(frag_db.normalize_names);
s.apply(frag_db.build_bonds);

CharmmFF eef1;
// remove all components excecpt for bends
eef1.removeComponent("CHARMM Torsion");
eef1.removeComponent("CHARMM ImproperTorsion");
eef1.removeComponent("CHARMM NonBonded");
eef1.removeComponent("CHARMM Stretch");
eef1.options[CharmmFF::Option::ASSIGN_CHARGES] = "true";
eef1.options[CharmmFF::Option::ASSIGN_TYPENAMES] = "true";
eef1.options[CharmmFF::Option::ASSIGN_TYPES] = "true";
eef1.options[CharmmFF::Option::FILENAME] = "CHARMM/EEF1/param19_eef1.ini";

eef1.setup(s);

eef1.updateForces();
eef1.updateEnergy();

TEST_REAL_EQUAL(eef1.getEnergy(), 307.4544619)
TEST_REAL_EQUAL(eef1.getBendEnergy(), 307.4544619)
TEST_REAL_EQUAL(eef1.getProperTorsionEnergy(), 0.0)
TEST_REAL_EQUAL(eef1.getImproperTorsionEnergy(), 0.0)
TEST_REAL_EQUAL(eef1.getStretchEnergy(), 0.0)
TEST_REAL_EQUAL(eef1.getESEnergy(), 0.0)
TEST_REAL_EQUAL(eef1.getVdWEnergy(), 0.0)
TEST_REAL_EQUAL(eef1.getSolvationEnergy(), 0.0)
TEST_REAL_EQUAL(eef1.getRMSGradient(), 123.0344948)

Size i = 0;
Vector3 force;
AtomIterator atom_it = s.beginAtom();
for (; +atom_it; ++atom_it, ++i)
{
	switch (i)
	{
		case 0:	force = Vector3(-29.88574,  42.58230,  -9.95883); break;
		case 1:	force = Vector3(  0.00347,  -0.15351,   0.08606); break;
		case 2:	force = Vector3( -0.00715,   6.37587,  -0.57521); break;
		case 3:	force = Vector3(  0.11338,  -0.22339,   0.06770); break;
		case 4:	force = Vector3(  0.00000,   0.00000,   0.00000); break;
		case 5:	force = Vector3( 25.47118, -20.67960,  10.22273); break;
		case 6:	force = Vector3(  4.46096, -28.05544,   0.24947); break;
		case 7:	force = Vector3( -0.15610,   0.15378,  -0.09192);
	}
	force = force - atom_it->getForce() * Constants::NA * 1e-13 / -4.184;
	std::cerr << atom_it->getName() << " " << atom_it->getForce() * Constants::NA * 1e-13 / -4.184 << std::endl;
	TEST_REAL_EQUAL(force.getLength(), 0.0)
}
RESULT

CHECK(force test 3 (GLY, torsion only) [EEF1])
// read the PDB file containing a single GLY
PDBFile f("data/CharmmFF_test_1.pdb");	
System s;
f >> s;
f.close();
// check whether we got the right number of atoms
TEST_EQUAL(s.countAtoms(), 8)
	
// normalize the names and build the bonds
s.apply(frag_db.normalize_names);
s.apply(frag_db.build_bonds);
CharmmFF eef1;
// remove all components excecpt for proper torsions
eef1.removeComponent("CHARMM Bend");
eef1.removeComponent("CHARMM ImproperTorsion");
eef1.removeComponent("CHARMM NonBonded");
eef1.removeComponent("CHARMM Stretch");
eef1.options[CharmmFF::Option::ASSIGN_CHARGES] = "true";
eef1.options[CharmmFF::Option::ASSIGN_TYPENAMES] = "true";
eef1.options[CharmmFF::Option::ASSIGN_TYPES] = "true";
eef1.options[CharmmFF::Option::FILENAME] = "CHARMM/EEF1/param19_eef1.ini";

eef1.setup(s);

eef1.updateForces();
eef1.updateEnergy();

TEST_REAL_EQUAL(eef1.getEnergy(), 2.11036776)
TEST_REAL_EQUAL(eef1.getBendEnergy(), 0.0)
TEST_REAL_EQUAL(eef1.getProperTorsionEnergy(), 2.11036776)
TEST_REAL_EQUAL(eef1.getImproperTorsionEnergy(), 0.0)
TEST_REAL_EQUAL(eef1.getStretchEnergy(), 0.0)
TEST_REAL_EQUAL(eef1.getESEnergy(), 0.0)
TEST_REAL_EQUAL(eef1.getVdWEnergy(), 0.0)
TEST_REAL_EQUAL(eef1.getSolvationEnergy(), 0.0)
TEST_REAL_EQUAL(eef1.getRMSGradient(), 4.60127032)

Size i = 0;
Vector3 force;
AtomIterator atom_it = s.beginAtom();
for (; +atom_it; ++atom_it, ++i)
{
	switch (i)
	{
		case 0:	force = Vector3(-1.03592,   0.56305,   1.01054); break;
		case 1:	force = Vector3(-0.29165,  -0.83152,  -0.30056); break;
		case 2:	force = Vector3( 2.38577,   0.92010,  -1.62842); break;
		case 3:	force = Vector3(-0.58905,   0.51427,   0.68932); break;
		case 4:	force = Vector3(-2.73842,  -1.08243,   1.15208); break;
		case 5:	force = Vector3( 2.13412,   0.57834,  -1.05459); break;
		case 6:	force = Vector3( 0.00462,  -0.48807,   0.04621); break;
		case 7:	force = Vector3( 0.13052,  -0.17372,   0.08542);
	}
	force = force - atom_it->getForce() * Constants::NA * 1e-13 / -4.184;
	std::cerr << atom_it->getName() << " " << atom_it->getForce() * Constants::NA * 1e-13 / -4.184 << std::endl;
	TEST_REAL_EQUAL(force.getLength(), 0.0)
}
RESULT

CHECK(force test 4 (GLY, improper torsion only) [EEF1])
// read the PDB file containing a single GLY
PDBFile f("data/CharmmFF_test_1.pdb");	
System s;
f >> s;
f.close();
// check whether we got the right number of atoms
TEST_EQUAL(s.countAtoms(), 8)
	
// normalize the names and build the bonds
s.apply(frag_db.normalize_names);
s.apply(frag_db.build_bonds);

CharmmFF eef1;
// remove all components excecpt for proper torsions
eef1.removeComponent("CHARMM Bend");
eef1.removeComponent("CHARMM Torsion");
eef1.removeComponent("CHARMM NonBonded");
eef1.removeComponent("CHARMM Stretch");
eef1.options[CharmmFF::Option::ASSIGN_CHARGES] = "true";
eef1.options[CharmmFF::Option::ASSIGN_TYPENAMES] = "true";
eef1.options[CharmmFF::Option::ASSIGN_TYPES] = "true";
eef1.options[CharmmFF::Option::FILENAME] = "CHARMM/EEF1/param19_eef1.ini";

eef1.setup(s);

eef1.updateForces();
eef1.updateEnergy();

TEST_REAL_EQUAL(eef1.getEnergy(), 5.48237888)
TEST_REAL_EQUAL(eef1.getBendEnergy(), 0.0)
TEST_REAL_EQUAL(eef1.getProperTorsionEnergy(), 0.0)
TEST_REAL_EQUAL(eef1.getImproperTorsionEnergy(), 5.48237888)
TEST_REAL_EQUAL(eef1.getStretchEnergy(), 0.0)
TEST_REAL_EQUAL(eef1.getESEnergy(), 0.0)
TEST_REAL_EQUAL(eef1.getVdWEnergy(), 0.0)
TEST_REAL_EQUAL(eef1.getSolvationEnergy(), 0.0)
TEST_REAL_EQUAL(eef1.getRMSGradient(), 31.6410816)

Size i = 0;
Vector3 force;
AtomIterator atom_it = s.beginAtom();
for (; +atom_it; ++atom_it, ++i)
{
	switch (i)
	{
		case 0:	force = Vector3(  0.00000,   0.00000,   0.00000); break;
		case 1:	force = Vector3(  0.00000,   0.00000,   0.00000); break;
		case 2:	force = Vector3( -0.13514,   0.66180,  -0.13163); break;
		case 3:	force = Vector3(  0.00000,   0.00000,   0.00000); break;
		case 4:	force = Vector3(  4.98047,   2.50327,  -6.64754); break;
		case 5:	force = Vector3(-16.96076,  -8.20699,  25.84288); break;
		case 6:	force = Vector3(  5.27883,   1.85422, -10.10592); break;
		case 7:	force = Vector3(  6.83660,   3.18770,  -8.95778);
	}
	force = force - atom_it->getForce() * Constants::NA * 1e-13 / -4.184;
	std::cerr << atom_it->getName() << " " << atom_it->getForce() * Constants::NA * 1e-13 / -4.184 << std::endl;
	TEST_REAL_EQUAL(force.getLength(), 0.0)
}
RESULT

CHECK(force test 5 (GLY, VdW only) [EEF1])
// read the PDB file containing a single GLY
PDBFile f("data/CharmmFF_test_1.pdb");	
System s;
f >> s;
f.close();
// check whether we got the right number of atoms
TEST_EQUAL(s.countAtoms(), 8)
	
// normalize the names and build the bonds
s.apply(frag_db.normalize_names);
s.apply(frag_db.build_bonds);

CharmmFF eef1;
// remove all components excecpt for VdW
eef1.removeComponent("CHARMM Torsion");
eef1.removeComponent("CHARMM ImproperTorsion");
eef1.removeComponent("CHARMM Bend");
eef1.removeComponent("CHARMM Stretch");
eef1.options[CharmmFF::Option::USE_EEF1] = "false";
eef1.options[CharmmFF::Option::ASSIGN_CHARGES] = "false";
eef1.options[CharmmFF::Option::ASSIGN_TYPENAMES] = "true";
eef1.options[CharmmFF::Option::ASSIGN_TYPES] = "true";
eef1.options[CharmmFF::Option::FILENAME] = "CHARMM/EEF1/param19_eef1.ini";
ClearChargeProcessor clear_charges;
s.apply(clear_charges);

eef1.setup(s);

eef1.updateForces();
eef1.updateEnergy();

TEST_REAL_EQUAL(eef1.getEnergy(), 26.64563664)
TEST_REAL_EQUAL(eef1.getBendEnergy(), 0.0)
TEST_REAL_EQUAL(eef1.getProperTorsionEnergy(), 0.0)
TEST_REAL_EQUAL(eef1.getImproperTorsionEnergy(), 0.0)
TEST_REAL_EQUAL(eef1.getStretchEnergy(), 0.0)
TEST_REAL_EQUAL(eef1.getESEnergy(), 0.0)
TEST_REAL_EQUAL(eef1.getVdWEnergy(), 26.64563664)
TEST_REAL_EQUAL(eef1.getSolvationEnergy(), 0.0)
TEST_REAL_EQUAL(eef1.getRMSGradient(), 59.16799416)

Size i = 0;
Vector3 force;
AtomIterator atom_it = s.beginAtom();
for (; +atom_it; ++atom_it, ++i)
{
	switch (i)
	{
		case 0:	force = Vector3(-29.88574,  42.58230,  -9.95883); break;
		case 1:	force = Vector3(  0.00347,  -0.15351,   0.08606); break;
		case 2:	force = Vector3( -0.00715,   6.37587,  -0.57521); break;
		case 3:	force = Vector3(  0.11338,  -0.22339,   0.06770); break;
		case 4:	force = Vector3(  0.00000,   0.00000,   0.00000); break;
		case 5:	force = Vector3( 25.47118, -20.67960,  10.22273); break;
		case 6:	force = Vector3(  4.46096, -28.05544,   0.24947); break;
		case 7:	force = Vector3( -0.15610,   0.15378,  -0.09192);
	}
	force = force - atom_it->getForce() * Constants::NA * 1e-13 / -4.184;
	std::cerr << atom_it->getName() << " " << atom_it->getForce() * Constants::NA * 1e-13 / -4.184 << std::endl;
	TEST_REAL_EQUAL(force.getLength(), 0.0)
}
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST

