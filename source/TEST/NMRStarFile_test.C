// $Id: NMRStarFile_test.C,v 1.10 2001/07/17 00:46:30 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/FORMAT/NMRStarFile.h>
///////////////////////////

using namespace BALL;

START_TEST(String,"$Id: NMRStarFile_test.C,v 1.10 2001/07/17 00:46:30 oliver Exp $")

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

NMRStarFile rs;

CHECK(NMRStarFile::NMRStarFile(filename))
	PRECISION(1e-3)
	rs = NMRStarFile("data/AssignShiftProcessor_test2.str");
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
	TEST_EQUAL(rs.getData()[0]->reference->elements[0]->reference_method, (char)NMRStarFile::INTERNAL_REFERENCE)
	TEST_EQUAL(rs.getData()[0]->reference->elements[0]->reference_type, (char)NMRStarFile::DIRECT_TYPE)
	TEST_REAL_EQUAL(rs.getData()[0]->reference->elements[0]->indirect_shift_ratio, 1)

	/*
	for (Position i = 0;  i < rs.getData()[0]->atom_data.size(); i++)
	{
		cout << *(rs.getData()[0]->atom_data[i]) << endl;
	}
	*/
RESULT

NMRStarFile f2;

CHECK(NMRStarFile::operator == (const NMRStarFile& f))
	NMRStarFile f1;
	TEST_EQUAL(f1 == f2, true)
	f2 = NMRStarFile("data/AssignShiftProcessor_test2.str");
	TEST_EQUAL(f1 == f2, false)
RESULT

CHECK(NMRStarFile::operator != (const NMRStarFile& f))
	NMRStarFile f1;
	TEST_EQUAL(f1 != f1, false)
	TEST_EQUAL(f1 != f2, true)
RESULT

CHECK(NMRStarFile::clear())
	TEST_EQUAL(f2.getData().size(), 1)
	TEST_EQUAL(f2.getNumberOfAtoms(), 1914)
	f2.clear();
	TEST_EQUAL(f2.getData().size(), 0)
	TEST_EQUAL(f2.getNumberOfAtoms(), 0)
RESULT

CHECK(NMRStarFile::BALL_CREATE(NMRStarFile))
	NMRStarFile* v_ptr = (NMRStarFile*)rs.create();
	TEST_NOT_EQUAL(v_ptr, 0)
	TEST_EQUAL(v_ptr->getNumberOfAtoms(), 1914)
	TEST_EQUAL(v_ptr->getData()[0]->reference->elements[0]->mol_common_name, "DSS")
	delete v_ptr;
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
