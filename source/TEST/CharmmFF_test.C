// $Id: CharmmFF_test.C,v 1.3 2000/03/25 22:40:56 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/MOLMEC/CHARMM/charmm.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/STRUCTURE/defaultProcessors.h>
#include <BALL/STRUCTURE/fragmentDB.h>
///////////////////////////

START_TEST(CharmmFF, "$Id: CharmmFF_test.C,v 1.3 2000/03/25 22:40:56 oliver Exp $")

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
		
	// normalize the names and build the bonds
	s.apply(frag_db.normalize_names);
	s.apply(frag_db.build_bonds);

	// setup the force field
	Options options;
	options[CharmmFF::Option::ASSIGN_CHARGES] = "true";
	options[CharmmFF::Option::ASSIGN_TYPENAMES] = "true";
	options[CharmmFF::Option::ASSIGN_TYPES] = "true";
	options[CharmmFF::Option::FILENAME] = "CHARMM/EEF1/param19_eef1.ini";
	CharmmFF eef1(s, options);

	CHECK(energy test 1 (GLY) [EEF1])
		// check whether we got the right number of atoms
		TEST_EQUAL(s.countAtoms(), 8)

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

		eef1.setup(s, options);

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
				// values are taken from CHARMM calculaction
				// (version 27b1 with param19_eef1.inp and toph19_eef1.inp,
				// with modified nonbonded: atom rdiel cutnb 20. cutonnb 11. cutofnb 15.)
				case 0:	force = Vector3( 26.44796,  91.42712, -23.82851); break;
				case 1:	force = Vector3( 14.55307,   1.84307, -19.22059); break;
				case 2:	force = Vector3(-14.56795, -89.42944,  62.77158); break;
				case 3:	force = Vector3(-16.84492, -12.50059,  -5.06838); break;
				case 4:	force = Vector3( -8.71000,   3.45793, -15.72975); break;
				case 5:	force = Vector3( -1.06629, -17.55152,  -0.53075); break;
				case 6:	force = Vector3( 19.60441,  12.80711,  11.19068); break;
				case 7:	force = Vector3(-19.41627,   9.94631,  -9.58428);
			}
			force = force - atom_it->getForce() * Constants::NA * 1e-13 / -4.184;
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
			// values are taken from CHARMM calculaction
			// (version 27b1 with param19_eef1.inp and toph19_eef1.inp,
			// with modified nonbonded: atom rdiel cutnb 20. cutonnb 11. cutofnb 15.)
			case 0:	force = Vector3( 32.09360,  -8.83321,   1.72968); break;
			case 1:	force = Vector3(  0.27122,  -7.58892,  -0.52235); break;
			case 2:	force = Vector3(-21.60332, -81.70418,  44.97363); break;
			case 3:	force = Vector3(-33.70472,  65.80094, -50.27192); break;
			case 4:	force = Vector3( 24.59176,  32.71333,   3.56059); break;
			case 5:	force = Vector3( -4.89956,  20.53095,   2.43037); break;
			case 6:	force = Vector3(  6.60390, -11.26640,   1.32471); break;
			case 7:	force = Vector3( -3.35288,  -9.65251,  -3.22471);
		}
		force = force - atom_it->getForce() * Constants::NA * 1e-13 / -4.184;
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
			// values are taken from CHARMM calculaction
			// (version 27b1 with param19_eef1.inp and toph19_eef1.inp,
			// with modified nonbonded: atom rdiel cutnb 20. cutonnb 11. cutofnb 15.)
			case 0:	force = Vector3(-1.03592,   0.56305,   1.01054); break;
			case 1:	force = Vector3(-0.29165,  -0.83152,  -0.30056); break;
			case 2:	force = Vector3( 2.52092,   0.25830,  -1.49679); break;
			case 3:	force = Vector3(-0.58905,   0.51427,   0.68932); break;
			case 4:	force = Vector3(-2.73842,  -1.08243,   1.15208); break;
			case 5:	force = Vector3( 2.13412,   0.57834,  -1.05459); break;
			case 6:	force = Vector3( 0.00000,   0.00000,   0.00000); break;
			case 7:	force = Vector3( 0.00000,   0.00000,   0.00000);
		}
		force = force - atom_it->getForce() * Constants::NA * 1e-13 / -4.184;
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
			// values are taken from CHARMM calculaction
			// (version 27b1 with param19_eef1.inp and toph19_eef1.inp,
			// with modified nonbonded: atom rdiel cutnb 20. cutonnb 11. cutofnb 15.)
			case 0:	force = Vector3(  0.00000,   0.00000,   0.00000); break;
			case 1:	force = Vector3(  0.00000,   0.00000,   0.00000); break;
			case 2:	force = Vector3(  0.00000,   0.00000,   0.00000); break;
			case 3:	force = Vector3(  0.00000,   0.00000,   0.00000); break;
			case 4:	force = Vector3(  4.98047,   2.50327,  -6.64754); break;
			case 5:	force = Vector3(-16.96076,  -8.20699,  25.84288); break;
			case 6:	force = Vector3(  5.27421,   2.34230, -10.15213); break;
			case 7:	force = Vector3(  6.70608,   3.36142,  -9.04320);
		}
		force = force - atom_it->getForce() * Constants::NA * 1e-13 / -4.184;
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
	eef1.options[CharmmFF::Option::ASSIGN_CHARGES] = "true";
	eef1.options[CharmmFF::Option::ASSIGN_TYPENAMES] = "true";
	eef1.options[CharmmFF::Option::ASSIGN_TYPES] = "true";
	eef1.options[CharmmFF::Option::FILENAME] = "CHARMM/EEF1/param19_eef1.ini";

	eef1.setup(s);
	ClearChargeProcessor clear_charges;
	s.apply(clear_charges);

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
			// values are taken from CHARMM calculaction
			// (version 27b1 with param19_eef1.inp and toph19_eef1.inp,
			// with modified nonbonded: atom rdiel cutnb 20. cutonnb 11. cutofnb 15.)
			case 0:	force = Vector3(-29.88574,  42.58230,  -9.95883); break;
			case 1:	force = Vector3(  0.00347,  -0.15351,   0.08606); break;
			case 2:	force = Vector3(  0.12799,   5.71407,  -0.44357); break;
			case 3:	force = Vector3(  0.11338,  -0.22339,   0.06770); break;
			case 4:	force = Vector3(  0.00000,   0.00000,   0.00000); break;
			case 5:	force = Vector3( 25.47118, -20.67960,  10.22273); break;
			case 6:	force = Vector3(  4.45634, -27.56737,   0.20325); break;
			case 7:	force = Vector3( -0.28662,   0.32750,  -0.17734);
		}
		force = force - atom_it->getForce() * Constants::NA * 1e-13 / -4.184;
		TEST_REAL_EQUAL(force.getLength(), 0.0)
	}
RESULT

CHECK(force test 6 (2 atoms, electrostatic only) [EEF1/CDIEL])
	// read the PDB file containing a single GLY
	PDBFile f("data/CharmmFF_test_2.pdb");	
	System s;
	f >> s;
	f.close();
	// check whether we got the right number of atoms
	TEST_EQUAL(s.countAtoms(), 2)
		
	// normalize the names and build the bonds
	s.apply(frag_db.normalize_names);

	CharmmFF eef1;
	// remove all components excecpt for electrostatics
	eef1.removeComponent("CHARMM Torsion");
	eef1.removeComponent("CHARMM ImproperTorsion");
	eef1.removeComponent("CHARMM Bend");
	eef1.removeComponent("CHARMM Stretch");
	eef1.options[CharmmFF::Option::USE_EEF1] = "false";
	eef1.options[CharmmFF::Option::ASSIGN_CHARGES] = "true";
	eef1.options[CharmmFF::Option::ASSIGN_TYPENAMES] = "true";
	eef1.options[CharmmFF::Option::ASSIGN_TYPES] = "true";
	eef1.options[CharmmFF::Option::DISTANCE_DEPENDENT_DIELECTRIC] = "false";
	eef1.options[CharmmFF::Option::FILENAME] = "CHARMM/EEF1/param19_eef1.ini";
	// turn off vdW interactions
	eef1.options[CharmmFF::Option::VDW_CUTOFF] = 0.1;
	eef1.options[CharmmFF::Option::VDW_CUTON] = 0.05;
	eef1.setup(s);

	eef1.updateForces();
	eef1.updateEnergy();

	TEST_REAL_EQUAL(eef1.getEnergy(), -910.321)
	TEST_REAL_EQUAL(eef1.getBendEnergy(), 0.0)
	TEST_REAL_EQUAL(eef1.getProperTorsionEnergy(), 0.0)
	TEST_REAL_EQUAL(eef1.getImproperTorsionEnergy(), 0.0)
	TEST_REAL_EQUAL(eef1.getStretchEnergy(), 0.0)
	TEST_REAL_EQUAL(eef1.getESEnergy(), -910.321)
	TEST_REAL_EQUAL(eef1.getVdWEnergy(), 0.0)
	TEST_REAL_EQUAL(eef1.getSolvationEnergy(), 0.0)
	TEST_REAL_EQUAL(eef1.getRMSGradient(), 212.569)

	Size i = 0;
	Vector3 force;
	AtomIterator atom_it = s.beginAtom();
	for (; +atom_it; ++atom_it, ++i)
	{
		force = atom_it->getForce() * Constants::NA * 1e-13;
		TEST_REAL_EQUAL(force.getLength(), 368.181)
	}
RESULT

CHECK(force test 7 (2 atoms, electrostatic only) [EEF1/RDIEL])
	// read the PDB file containing a single GLY
	PDBFile f("data/CharmmFF_test_2.pdb");	
	System s;
	f >> s;
	f.close();
	// check whether we got the right number of atoms
	TEST_EQUAL(s.countAtoms(), 2)
		
	// normalize the names and build the bonds
	s.apply(frag_db.normalize_names);

	CharmmFF eef1;
	// remove all components excecpt for electrostatics
	eef1.removeComponent("CHARMM Torsion");
	eef1.removeComponent("CHARMM ImproperTorsion");
	eef1.removeComponent("CHARMM Bend");
	eef1.removeComponent("CHARMM Stretch");
	eef1.options[CharmmFF::Option::USE_EEF1] = "false";
	eef1.options[CharmmFF::Option::ASSIGN_CHARGES] = "true";
	eef1.options[CharmmFF::Option::ASSIGN_TYPENAMES] = "true";
	eef1.options[CharmmFF::Option::ASSIGN_TYPES] = "true";
	eef1.options[CharmmFF::Option::FILENAME] = "CHARMM/EEF1/param19_eef1.ini";
	// turn off vdW interactions
	eef1.options[CharmmFF::Option::VDW_CUTOFF] = 0.1;
	eef1.options[CharmmFF::Option::VDW_CUTON] = 0.05;
	eef1.options[CharmmFF::Option::DISTANCE_DEPENDENT_DIELECTRIC] = "true";
	eef1.setup(s);

	eef1.updateForces();
	eef1.updateEnergy();

	TEST_REAL_EQUAL(eef1.getEnergy(), -368.181)
	TEST_REAL_EQUAL(eef1.getBendEnergy(), 0.0)
	TEST_REAL_EQUAL(eef1.getProperTorsionEnergy(), 0.0)
	TEST_REAL_EQUAL(eef1.getImproperTorsionEnergy(), 0.0)
	TEST_REAL_EQUAL(eef1.getStretchEnergy(), 0.0)
	TEST_REAL_EQUAL(eef1.getESEnergy(), -368.181)
	TEST_REAL_EQUAL(eef1.getVdWEnergy(), 0.0)
	TEST_REAL_EQUAL(eef1.getSolvationEnergy(), 0.0)
	TEST_REAL_EQUAL(eef1.getRMSGradient(), 171.948)

	Size i = 0;
	Vector3 force, a1, a2;
	AtomIterator atom_it = s.beginAtom();
	for (; +atom_it; ++atom_it, ++i)
	{
		force = atom_it->getForce() * Constants::NA * 1e-13;
		TEST_REAL_EQUAL(force.getLength(), 297.823)
	}
RESULT

CHECK(force test 8 (2 atoms, ES switching function) [EEF1/RDIEL])
	// read the PDB file containing two atoms
	PDBFile f("data/CharmmFF_test_2.pdb");	
	System s;
	f >> s;
	f.close();
	// check whether we got the right number of atoms
	TEST_EQUAL(s.countAtoms(), 2)
		
	// normalize the names and build the bonds
	s.apply(frag_db.normalize_names);

	CharmmFF eef1;
	// remove all components excecpt for electrostatics
	eef1.removeComponent("CHARMM Torsion");
	eef1.removeComponent("CHARMM ImproperTorsion");
	eef1.removeComponent("CHARMM Bend");
	eef1.removeComponent("CHARMM Stretch");
	eef1.options[CharmmFF::Option::USE_EEF1] = "false";
	eef1.options[CharmmFF::Option::ASSIGN_CHARGES] = "true";
	eef1.options[CharmmFF::Option::ASSIGN_TYPENAMES] = "true";
	eef1.options[CharmmFF::Option::ASSIGN_TYPES] = "true";
	eef1.options[CharmmFF::Option::FILENAME] = "CHARMM/EEF1/param19_eef1.ini";
	// turn off vdW interactions
	eef1.options[CharmmFF::Option::VDW_CUTOFF] = 0.1;
	eef1.options[CharmmFF::Option::VDW_CUTON] = 0.05;
	eef1.options[CharmmFF::Option::ELECTROSTATIC_CUTON] = 2.0;
	eef1.options[CharmmFF::Option::ELECTROSTATIC_CUTOFF] = 4.0;
	eef1.options[CharmmFF::Option::DISTANCE_DEPENDENT_DIELECTRIC] = "true";
	eef1.setup(s);

	Size i = 0;
	Vector3 force;
	Atom* a1;
	Atom* a2;
	AtomIterator atom_it = s.beginAtom();
	for (; +atom_it; ++atom_it, ++i)
	{

		switch (i)
		{
			case 0:	a1 = &*atom_it; break;
			case 1:	a2 = &*atom_it; break;
		}
	}
	TEST_NOT_EQUAL(a1, 0)
	TEST_NOT_EQUAL(a2, 0)

	// set one atom to the origin
	a1->setPosition(Vector3(0.0));
	
	i = 0;
	for (double d = 1.2; d < 6.0; d += 0.2, i++)
	{
		a2->setPosition(Vector3(d, 0.0, 0.0));
		eef1.updateForces();
		double energy = eef1.updateEnergy();
		double force = a1->getForce().x * Constants::NA / 1e13;
		switch (i)
		{
			case 0:	
				TEST_REAL_EQUAL(force, 2605.04)
				TEST_REAL_EQUAL(energy, -1563.02)
				break;
			case 1:	
				TEST_REAL_EQUAL(force, 1640.49)
				TEST_REAL_EQUAL(energy, -1148.34)
				break;
			case 2:	
				TEST_REAL_EQUAL(force, 1099)
				TEST_REAL_EQUAL(energy, -879.201)
				break;
			case 3:	
				TEST_REAL_EQUAL(force, 771.864)
				TEST_REAL_EQUAL(energy, -694.677)
				break;
			case 4:	
				TEST_REAL_EQUAL(force, 562.688)
				TEST_REAL_EQUAL(energy, -562.688)
				break;
			case 5:	
				TEST_REAL_EQUAL(force, 483.433)
				TEST_REAL_EQUAL(energy, -458.515)
				break;
			case 6:	
				TEST_REAL_EQUAL(force, 424.043)
				TEST_REAL_EQUAL(energy, -368.004)
				break;
			case 7:	
				TEST_REAL_EQUAL(force, 375.014)
				TEST_REAL_EQUAL(energy, -288.214)
				break;
			case 8:	
				TEST_REAL_EQUAL(force, 330.421)
				TEST_REAL_EQUAL(energy, -217.707)
				break;
			case 9:	
				TEST_REAL_EQUAL(force, 286.361)
				TEST_REAL_EQUAL(energy, -156.013)
				break;
			case 10:	
				TEST_REAL_EQUAL(force, 240.127)
				TEST_REAL_EQUAL(energy, -103.309)
				break;
			case 11:	
				TEST_REAL_EQUAL(force, 189.744)
				TEST_REAL_EQUAL(energy, -60.2396)
				break;
			case 12:	
				TEST_REAL_EQUAL(force, 133.7)
				TEST_REAL_EQUAL(energy, -27.7899)
				break;
			case 13:	
				TEST_REAL_EQUAL(force, 70.7883)
				TEST_REAL_EQUAL(energy, -7.21769)
				break;
			default:	// above cutoff -> everything equals zero
				TEST_REAL_EQUAL(force, 0.0)
				TEST_REAL_EQUAL(energy, 0.0)
		}
	}
RESULT

CHECK(force test 9 (2 atoms, ES switching function) [EEF1/CDIEL])
	// read the PDB file containing two atoms
	PDBFile f("data/CharmmFF_test_2.pdb");	
	System s;
	f >> s;
	f.close();
	// check whether we got the right number of atoms
	TEST_EQUAL(s.countAtoms(), 2)
		
	// normalize the names and build the bonds
	s.apply(frag_db.normalize_names);

	CharmmFF eef1;
	// remove all components except for electrostatics
	eef1.removeComponent("CHARMM Torsion");
	eef1.removeComponent("CHARMM ImproperTorsion");
	eef1.removeComponent("CHARMM Bend");
	eef1.removeComponent("CHARMM Stretch");
	eef1.options[CharmmFF::Option::USE_EEF1] = "false";
	eef1.options[CharmmFF::Option::ASSIGN_CHARGES] = "true";
	eef1.options[CharmmFF::Option::ASSIGN_TYPENAMES] = "true";
	eef1.options[CharmmFF::Option::ASSIGN_TYPES] = "true";
	eef1.options[CharmmFF::Option::FILENAME] = "CHARMM/EEF1/param19_eef1.ini";
	// turn off vdW interactions
	eef1.options[CharmmFF::Option::VDW_CUTOFF] = 0.1;
	eef1.options[CharmmFF::Option::VDW_CUTON] = 0.05;
	eef1.options[CharmmFF::Option::ELECTROSTATIC_CUTON] = 2.0;
	eef1.options[CharmmFF::Option::ELECTROSTATIC_CUTOFF] = 4.0;
	eef1.options[CharmmFF::Option::DISTANCE_DEPENDENT_DIELECTRIC] = "false";
	eef1.setup(s);

	Size i = 0;
	Vector3 force;
	Atom* a1;
	Atom* a2;
	AtomIterator atom_it = s.beginAtom();
	for (; +atom_it; ++atom_it, ++i)
	{
		switch (i)
		{
			case 0:	a1 = &*atom_it; break;
			case 1:	a2 = &*atom_it; break;
		}
	}
	TEST_NOT_EQUAL(a1, 0)
	TEST_NOT_EQUAL(a2, 0)

	// set one atom to the origin
	a1->setPosition(Vector3(0.0));
	
	i = 0;
	for (double d = 1.2; d < 6.0; d += 0.2, i++)
	{
		a2->setPosition(Vector3(d, 0.0, 0.0));
		eef1.updateForces();
		double energy = eef1.updateEnergy();
		double force = a1->getForce().x * Constants::NA / 1e13;
		switch (i)
		{
			case 0:	
				TEST_REAL_EQUAL(force, 1563.02)
				TEST_REAL_EQUAL(energy, -1875.625)
				break;
			case 1:	
				TEST_REAL_EQUAL(force, 1148.34)
				TEST_REAL_EQUAL(energy, -1607.68)
				break;
			case 2:	
				TEST_REAL_EQUAL(force, 879.201)
				TEST_REAL_EQUAL(energy, -1406.72)
				break;
			case 3:	
				TEST_REAL_EQUAL(force, 694.678)
				TEST_REAL_EQUAL(energy, -1250.415)
				break;
			case 4:	
				TEST_REAL_EQUAL(force, 562.689)
				TEST_REAL_EQUAL(energy, -1125.375)
				break;
			case 5:	
				TEST_REAL_EQUAL(force, 605.039)
				TEST_REAL_EQUAL(energy, -1008.73)
				break;
			case 6:	
				TEST_REAL_EQUAL(force, 649.699)
				TEST_REAL_EQUAL(energy, -883.211)
				break;
			case 7:	
				TEST_REAL_EQUAL(force, 686.823)
				TEST_REAL_EQUAL(energy, -749.357)
				break;
			case 8:	
				TEST_REAL_EQUAL(force, 707.471)
				TEST_REAL_EQUAL(energy, -609.581)
				break;
			case 9:	
				TEST_REAL_EQUAL(force, 703.071)
				TEST_REAL_EQUAL(energy, -468.038)
				break;
			case 10:	
				TEST_REAL_EQUAL(force, 665.098)
				TEST_REAL_EQUAL(energy, -330.59)
				break;
			case 11:	
				TEST_REAL_EQUAL(force, 584.89)
				TEST_REAL_EQUAL(energy, -204.814)
				break;
			case 12:	
				TEST_REAL_EQUAL(force, 453.532)
				TEST_REAL_EQUAL(energy, -100.043)
				break;
			case 13:	
				TEST_REAL_EQUAL(force, 261.778)
				TEST_REAL_EQUAL(energy, -27.4272)
				break;
			default:	// above cutoff -> everything equals zero
				TEST_REAL_EQUAL(force, 0.0)
				TEST_REAL_EQUAL(energy, 0.0)
		}
	}
RESULT

CHECK(force test 10 (2 atoms, vdW switching function) [EEF1])
	// read the PDB file containing two atoms
	PDBFile f("data/CharmmFF_test_2.pdb");	
	System s;
	f >> s;
	f.close();
	// check whether we got the right number of atoms
	TEST_EQUAL(s.countAtoms(), 2)
		
	// normalize the names and build the bonds
	s.apply(frag_db.normalize_names);

	CharmmFF eef1;
	// remove all components except for vdW
	eef1.removeComponent("CHARMM Torsion");
	eef1.removeComponent("CHARMM ImproperTorsion");
	eef1.removeComponent("CHARMM Bend");
	eef1.removeComponent("CHARMM Stretch");
	eef1.options[CharmmFF::Option::USE_EEF1] = "false";
	eef1.options[CharmmFF::Option::ASSIGN_CHARGES] = "true";
	eef1.options[CharmmFF::Option::ASSIGN_TYPENAMES] = "true";
	eef1.options[CharmmFF::Option::ASSIGN_TYPES] = "true";
	eef1.options[CharmmFF::Option::FILENAME] = "CHARMM/EEF1/param19_eef1.ini";
	// turn off ES interactions
	eef1.options[CharmmFF::Option::VDW_CUTOFF] = 6.0;
	eef1.options[CharmmFF::Option::VDW_CUTON] = 4.0;
	eef1.options[CharmmFF::Option::ELECTROSTATIC_CUTON] = 0.05;
	eef1.options[CharmmFF::Option::ELECTROSTATIC_CUTOFF] = 0.1;
	eef1.setup(s);

	Size i = 0;
	Vector3 force;
	Atom* a1;
	Atom* a2;
	AtomIterator atom_it = s.beginAtom();
	for (; +atom_it; ++atom_it, ++i)
	{
		switch (i)
		{
			case 0:	a1 = &*atom_it; break;
			case 1:	a2 = &*atom_it; break;
		}
	}
	TEST_NOT_EQUAL(a1, 0)
	TEST_NOT_EQUAL(a2, 0)

	// set one atom to the origin
	a1->setPosition(Vector3(0.0));
	
	i = 0;
	for (double d = 3.2; d < 8.0; d += 0.2, i++)
	{
		a2->setPosition(Vector3(d, 0.0, 0.0));
		eef1.updateForces();
		double energy = eef1.updateEnergy();
		double force = a1->getForce().x * Constants::NA / 1e13;
		switch (i)
		{
			case 0:	
				TEST_REAL_EQUAL(force, -8.81132)
				TEST_REAL_EQUAL(energy, 0.658678)
				break;
			case 1:	
				TEST_REAL_EQUAL(force, -2.74153)
				TEST_REAL_EQUAL(energy, -0.398596)
				break;
			case 2:	
				TEST_REAL_EQUAL(force, -0.496799)
				TEST_REAL_EQUAL(energy, -0.685084)
				break;
			case 3:	
				TEST_REAL_EQUAL(force, 0.281582)
				TEST_REAL_EQUAL(energy, -0.692204)
				break;
			case 4:	
				TEST_REAL_EQUAL(force, 0.496839)
				TEST_REAL_EQUAL(energy, -0.608901)
				break;
			case 5:	
				TEST_REAL_EQUAL(force, 0.589907)
				TEST_REAL_EQUAL(energy, -0.497289)
				break;
			case 6:	
				TEST_REAL_EQUAL(force, 0.559986)
				TEST_REAL_EQUAL(energy, -0.380997)
				break;
			case 7:	
				TEST_REAL_EQUAL(force, 0.479373)
				TEST_REAL_EQUAL(energy, -0.276597)
				break;
			case 8:	
				TEST_REAL_EQUAL(force, 0.384436)
				TEST_REAL_EQUAL(energy, -0.190162)
				break;
			case 9:	
				TEST_REAL_EQUAL(force, 0.292368)
				TEST_REAL_EQUAL(energy, -0.12261)
				break;
			case 10:	
				TEST_REAL_EQUAL(force, 0.210405)
				TEST_REAL_EQUAL(energy, -0.0725306)
				break;
			case 11:	
				TEST_REAL_EQUAL(force, 0.14078)
				TEST_REAL_EQUAL(energy, -0.0376211)
				break;
			case 12:	
				TEST_REAL_EQUAL(force, 0.0833467)
				TEST_REAL_EQUAL(energy, -0.0154035)
				break;
			case 13:	
				TEST_REAL_EQUAL(force, 0.0369267)
				TEST_REAL_EQUAL(energy, -0.00354749)
				break;
			default:	// above cutoff -> everything equals zero
				TEST_REAL_EQUAL(force, 0.0)
				TEST_REAL_EQUAL(energy, 0.0)
		}
	}
RESULT

CHECK(force test 11 (2 atoms, solvation switching function) [EEF1])
	// read the PDB file containing two atoms
	PDBFile f("data/CharmmFF_test_2.pdb");	
	System s;
	f >> s;
	f.close();
	// check whether we got the right number of atoms
	TEST_EQUAL(s.countAtoms(), 2)
		
	// normalize the names and build the bonds
	s.apply(frag_db.normalize_names);

	CharmmFF eef1;
	// remove all components except for vdW
	eef1.removeComponent("CHARMM Torsion");
	eef1.removeComponent("CHARMM ImproperTorsion");
	eef1.removeComponent("CHARMM Bend");
	eef1.removeComponent("CHARMM Stretch");
	eef1.options[CharmmFF::Option::USE_EEF1] = "true";
	eef1.options[CharmmFF::Option::ASSIGN_CHARGES] = "true";
	eef1.options[CharmmFF::Option::ASSIGN_TYPENAMES] = "true";
	eef1.options[CharmmFF::Option::ASSIGN_TYPES] = "true";
	eef1.options[CharmmFF::Option::FILENAME] = "CHARMM/EEF1/param19_eef1.ini";
	// turn off vdW and ES interactions
	eef1.options[CharmmFF::Option::VDW_CUTON] = 0.05;
	eef1.options[CharmmFF::Option::VDW_CUTOFF] = 0.1;
	eef1.options[CharmmFF::Option::ELECTROSTATIC_CUTON] = 0.05;
	eef1.options[CharmmFF::Option::ELECTROSTATIC_CUTOFF] = 0.1;
	eef1.options[CharmmFF::Option::SOLVATION_CUTON] = 2.0;
	eef1.options[CharmmFF::Option::SOLVATION_CUTOFF] = 4.0;
	eef1.setup(s);

	Size i = 0;
	Vector3 force;
	Atom* a1;
	Atom* a2;
	AtomIterator atom_it = s.beginAtom();
	for (; +atom_it; ++atom_it, ++i)
	{
		switch (i)
		{
			case 0:	a1 = &*atom_it; break;
			case 1:	a2 = &*atom_it; break;
		}
	}
	TEST_NOT_EQUAL(a1, 0)
	TEST_NOT_EQUAL(a2, 0)

	// set one atom to the origin
	a1->setPosition(Vector3(0.0));
	
	i = 0;
	for (double d = 1.2; d < 6.0; d += 0.2, i++)
	{
		a2->setPosition(Vector3(d, 0.0, 0.0));
		eef1.updateForces();
		double energy = eef1.updateEnergy();
		double force = a1->getForce().x * Constants::NA / 1e13;
		switch (i)
		{
			case 0:	
				TEST_REAL_EQUAL(force, -20.9295)
				TEST_REAL_EQUAL(energy, -70.9525)
				break;
			case 1:	
				TEST_REAL_EQUAL(force, -13.2985)
				TEST_REAL_EQUAL(energy, -74.298)
				break;
			case 2:	
				TEST_REAL_EQUAL(force, -8.98882)
				TEST_REAL_EQUAL(energy, -76.4889)
				break;
			case 3:	
				TEST_REAL_EQUAL(force, -6.36919)
				TEST_REAL_EQUAL(energy, -78.0044)
				break;
			case 4:	
				TEST_REAL_EQUAL(force, -4.68368)
				TEST_REAL_EQUAL(energy, -79.0981)
				break;
			case 5:	
				TEST_REAL_EQUAL(force, -4.03846)
				TEST_REAL_EQUAL(energy, -79.967)
				break;
			case 6:	
				TEST_REAL_EQUAL(force, -3.53859)
				TEST_REAL_EQUAL(energy, -80.723)
				break;
			case 7:	
				TEST_REAL_EQUAL(force, -3.11062)
				TEST_REAL_EQUAL(energy, -81.3872)
				break;
			case 8:	
				TEST_REAL_EQUAL(force, -2.71063)
				TEST_REAL_EQUAL(energy, -81.9691)
				break;
			case 9:	
				TEST_REAL_EQUAL(force, -2.31205)
				TEST_REAL_EQUAL(energy, -82.4715)
				break;
			case 10:	
				TEST_REAL_EQUAL(force, -1.89916)
				TEST_REAL_EQUAL(energy, -82.893)
				break;
			case 11:	
				TEST_REAL_EQUAL(force, -1.46339)
				TEST_REAL_EQUAL(energy, -83.2297)
				break;
			case 12:	
				TEST_REAL_EQUAL(force, -1.00115)
				TEST_REAL_EQUAL(energy, -83.4766)
				break;
			case 13:	
				TEST_REAL_EQUAL(force, -0.512456)
				TEST_REAL_EQUAL(energy, -83.6283)
				break;
			default:	
				// above cutoff -> everything equals zero
				// energy equals to the sum of dG_ref
				TEST_REAL_EQUAL(force, 0.0)
				TEST_REAL_EQUAL(energy, -83.68)
		}
	}
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST

