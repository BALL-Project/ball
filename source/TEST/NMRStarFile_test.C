// $Id: NMRStarFile_test.C,v 1.7 2000/10/03 18:55:35 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/FORMAT/NMRStarFile.h>
///////////////////////////

using namespace BALL;

START_TEST(String,"$Id: NMRStarFile_test.C,v 1.7 2000/10/03 18:55:35 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

NMRStarFile* f;

CHECK(NMRStarFile::NMRStarFile())
	f = new NMRStarFile;
	TEST_NOT_EQUAL(f, 0)
RESULT

CHECK(NMRStarFile::~NMRStarFile())
	delete(f);
RESULT

CHECK(NMRStarFile::NMRStarFile(const NMRStarFile& f))
	NMRStarFile f1;
	NMRStarFile* f2 = new NMRStarFile(f1);
	TEST_NOT_EQUAL(f2, 0)
RESULT

CHECK(NMRStarFile::operator = (const NMRStarFile& f))
	NMRStarFile f1;
	NMRStarFile f2 = f1;
RESULT

CHECK(NMRStarFile::NMRStarFile(filename))
	PRECISION(1e-3)
	NMRStarFile rs("data/bmr4318.str");
	TEST_EQUAL(rs.getData().size(), 1)
	TEST_EQUAL(rs.getNumberOfAtoms(), 1914)
	if (rs.getData().size() == 1 && rs.getNumberOfAtoms() == 1914)
	{
		TEST_EQUAL(rs.getData()[0]->atom_data[1913]->atom_ID, 1914)
		TEST_EQUAL(rs.getData()[0]->atom_data[1913]->residue_seq_code, 215)
		TEST_EQUAL(rs.getData()[0]->atom_data[1913]->residue_label, "ILE")
		TEST_EQUAL(rs.getData()[0]->atom_data[1913]->atom_type, 'N')
		TEST_EQUAL(rs.getData()[0]->atom_data[1913]->atom_name, "N")
		TEST_REAL_EQUAL(rs.getData()[0]->atom_data[1913]->shift_value, 123.16)
		TEST_REAL_EQUAL(rs.getData()[0]->atom_data[1913]->error_value, 0)
		TEST_EQUAL(rs.getData()[0]->atom_data[1913]->ambiguity_code, 1)
	}

	TEST_EQUAL(rs.getData()[0]->name, "assigned_chemical_shifts")

	TEST_EQUAL(rs.getData()[0]->condition->name, "sample_conditions")
	TEST_EQUAL(rs.getData()[0]->condition->temperature, 293)
	TEST_EQUAL(rs.getData()[0]->condition->pH, 7)
	TEST_EQUAL(rs.getData()[0]->condition->pressure, 0)

	TEST_EQUAL(rs.getData()[0]->reference->elements[0]->mol_common_name, "DSS")
	TEST_EQUAL(rs.getData()[0]->reference->elements[0]->atom_type, 'H')
	TEST_EQUAL(rs.getData()[0]->reference->elements[0]->isotope_number, 1)
	TEST_EQUAL(rs.getData()[0]->reference->elements[0]->atom_group, "methyl protons")
	TEST_EQUAL(rs.getData()[0]->reference->elements[0]->shift_units, "ppm")
	TEST_REAL_EQUAL(rs.getData()[0]->reference->elements[0]->shift_value, 0)
	TEST_EQUAL(rs.getData()[0]->reference->elements[0]->reference_method, NMRStarFile::INTERNAL_REFERENCE)
	TEST_EQUAL(rs.getData()[0]->reference->elements[0]->reference_type, NMRStarFile::DIRECT_TYPE)
	TEST_REAL_EQUAL(rs.getData()[0]->reference->elements[0]->indirect_shift_ratio, 1)

	/*
	for (Position i = 0;  i < rs.getData()[0]->atom_data.size(); i++)
	{
		cout << *(rs.getData()[0]->atom_data[i]) << endl;
	}
	*/
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
