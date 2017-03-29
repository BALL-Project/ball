// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

// insert includes here
#include <BALL/SYSTEM/path.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/FORMAT/INIFile.h>
#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/MOLMEC/COMMON/radiusRuleProcessor.h>
#include <BALL/MOLMEC/COMMON/chargeRuleProcessor.h>
#include <BALL/STRUCTURE/defaultProcessors.h>

///////////////////////////

START_TEST(class_name)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

HINFile hinfile;
PDBFile pdbfile;
System system;
System ref_system;
FragmentDB db("");

Path path;
String tmp = path.find("solvation/PARSE.rul");
INIFile PARSE_rule_file(tmp);
PARSE_rule_file.read();
ChargeRuleProcessor charges(PARSE_rule_file);
RadiusRuleProcessor radii(PARSE_rule_file);
AtomConstIterator atom_it;
AtomConstIterator ref_atom_it;
float total_charge;
ClearChargeProcessor clear_charges;

CHECK("AlaGlySer: -CONH-, -OH, COO(-), NH3(+), aliphatic carbons with hydrogens")
	hinfile.open(BALL_TEST_DATA_PATH(AlaGlySer.hin));
	hinfile >> system;
	hinfile.close();
	system.apply(db.normalize_names);
	system.apply(clear_charges);
	system.apply(charges);
	system.apply(radii);
	for (atom_it = system.beginAtom(), total_charge = 0.0f; +atom_it; ++atom_it)
	{
		total_charge += atom_it->getCharge();
	}
	TEST_REAL_EQUAL(total_charge, 0.0)
	hinfile.open(BALL_TEST_DATA_PATH(AlaGlySer_PARSE_charges.hin));
	hinfile >> ref_system;
	hinfile.close();
	for (atom_it = system.beginAtom(), ref_atom_it = ref_system.beginAtom();
			+atom_it; ++atom_it, ++ref_atom_it)
	{
		TEST_REAL_EQUAL(atom_it->getCharge(), ref_atom_it->getCharge())
	}
RESULT

CHECK("AspGluAsnGlnArg: -COOH -CONH2 -CONH- -CNC-(NH2)2")
	system.clear();
	hinfile.open(BALL_TEST_DATA_PATH(AspGluAsnGlnArg.hin));
	hinfile >> system;
	hinfile.close();
	system.apply(db.normalize_names);
	system.apply(clear_charges);
	system.apply(charges);
	system.apply(radii);

	for (atom_it = system.beginAtom(), total_charge = 0.0f; +atom_it; ++atom_it)
	{
		total_charge += atom_it->getCharge();
	}
	TEST_REAL_EQUAL(total_charge, -0.0)
	ref_system.clear();
	hinfile.open(BALL_TEST_DATA_PATH(AspGluAsnGlnArg_PARSE_charges.hin));
	hinfile >> ref_system;
	hinfile.close();
	for (atom_it = system.beginAtom(), ref_atom_it = ref_system.beginAtom();
			+atom_it; ++atom_it, ++ref_atom_it)
	{
		TEST_REAL_EQUAL(atom_it->getCharge(), ref_atom_it->getCharge())
	}
RESULT

CHECK("PheTyrTrpHisLys+.hin")
	system.clear();
	hinfile.open(BALL_TEST_DATA_PATH(PheTyrTrpHisLys+.hin));
	hinfile >> system;
	hinfile.close();
	system.apply(db.normalize_names);
	system.apply(clear_charges);
	system.apply(charges);
	system.apply(radii);

	for (atom_it = system.beginAtom(), total_charge = 0.0f; +atom_it; ++atom_it)
	{
		total_charge += atom_it->getCharge();
	}
	TEST_REAL_EQUAL(total_charge, 1.0)
	ref_system.clear();
	hinfile.open(BALL_TEST_DATA_PATH(PheTyrTrpHisLys+_PARSE_charges.hin));
	hinfile >> ref_system;
	hinfile.close();
	for (atom_it = system.beginAtom(), ref_atom_it = ref_system.beginAtom();
			+atom_it; ++atom_it, ++ref_atom_it)
	{
		TEST_REAL_EQUAL(atom_it->getCharge(), ref_atom_it->getCharge())
	}
RESULT

CHECK("Cys-Asp-Glu-Tyr-His+Arg+.hin")
	system.clear();
	hinfile.open(BALL_TEST_DATA_PATH(Cys-Asp-Glu-Tyr-His+Arg+.hin));
	hinfile >> system;
	hinfile.close();
	system.apply(db.normalize_names);
	system.apply(clear_charges);
	system.apply(charges);
	system.apply(radii);

	for (atom_it = system.beginAtom(), total_charge = 0.0f; +atom_it; ++atom_it)
	{
		total_charge += atom_it->getCharge();
	}
	TEST_REAL_EQUAL(total_charge, -2.0)
	ref_system.clear();
	hinfile.open(BALL_TEST_DATA_PATH(Cys-Asp-Glu-Tyr-His+Arg+_PARSE_charges.hin));
	hinfile >> ref_system;
	hinfile.close();
	for (atom_it = system.beginAtom(), ref_atom_it = ref_system.beginAtom();
			+atom_it; ++atom_it, ++ref_atom_it)
	{
		TEST_REAL_EQUAL(atom_it->getCharge(), ref_atom_it->getCharge())
	}
RESULT


CHECK("SerThrLysCysMet.hin")
	system.clear();
	hinfile.open(BALL_TEST_DATA_PATH(SerThrLysCysMet.hin));
	hinfile >> system;
	hinfile.close();
	system.apply(db.normalize_names);
	system.apply(clear_charges);
	system.apply(charges);
	system.apply(radii);

	for (atom_it = system.beginAtom(), total_charge = 0.0f; +atom_it; ++atom_it)
	{
		total_charge += atom_it->getCharge();
	}
	TEST_REAL_EQUAL(total_charge, 0.0)
	ref_system.clear();
	hinfile.open(BALL_TEST_DATA_PATH(SerThrLysCysMet_PARSE_charges.hin));
	hinfile >> ref_system;
	hinfile.close();
	for (atom_it = system.beginAtom(), ref_atom_it = ref_system.beginAtom();
			+atom_it; ++atom_it, ++ref_atom_it)
	{
		TEST_REAL_EQUAL(atom_it->getCharge(), ref_atom_it->getCharge())
	}
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
