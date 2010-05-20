// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////
#include <BALL/FORMAT/NMRStarFile.h>
///////////////////////////

using namespace BALL;

START_TEST(String,"$Id: NMRStarFile_test.C,v 1.14.28.1 2007/03/25 21:47:22 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

NMRStarFile* f;

CHECK(NMRStarFile() )
	f = new NMRStarFile;
	TEST_NOT_EQUAL(f, 0)
RESULT

CHECK(~NMRStarFile() )
	delete(f);
RESULT

NMRStarFile rs;

CHECK(NMRStarFile(const String& file_name) throw(Exception::FileNotFound, Exception::ParseError))
	PRECISION(1e-3)
	rs = NMRStarFile(BALL_TEST_DATA_PATH(AssignShiftProcessor_test2.str));
	TEST_EQUAL(rs.getNMRData().size(), 1)
	TEST_EQUAL(rs.getNumberOfAtoms(), 1914)
	if (rs.getNMRData().size() == 1 && rs.getNumberOfAtoms() == 1914)
	{
		TEST_EQUAL(rs.getNMRData()[0].atom_data[1913].atom_ID, 1914)
		TEST_EQUAL(rs.getNMRData()[0].atom_data[1913].residue_seq_code, 215)
		TEST_EQUAL(rs.getNMRData()[0].atom_data[1913].residue_label, "ILE")
		TEST_EQUAL(rs.getNMRData()[0].atom_data[1913].atom_type, 'N')
		TEST_EQUAL(rs.getNMRData()[0].atom_data[1913].atom_name, "N")
		TEST_REAL_EQUAL(rs.getNMRData()[0].atom_data[1913].shift_value, 123.16)
		TEST_REAL_EQUAL(rs.getNMRData()[0].atom_data[1913].error_value, 0)
		TEST_EQUAL(rs.getNMRData()[0].atom_data[1913].ambiguity_code, 1)
	}

	TEST_EQUAL(rs.getNMRData()[0].name, "assigned_chemical_shifts")

	TEST_EQUAL(rs.getNMRData()[0].condition, "sample_conditions")
	//TEST_EQUAL(rs.getNMRData()[0].condition.temperature, 293)
	//TEST_EQUAL(rs.getNMRData()[0].condition.pH, 7)
	//TEST_EQUAL(rs.getNMRData()[0].condition.pressure, 0)

	//TEST_EQUAL(rs.getNMRData()[0].reference.elements[0].mol_common_name, "DSS")
	//TEST_EQUAL(rs.getNMRData()[0].reference.elements[0].atom_type, 'H')
	//TEST_EQUAL(rs.getNMRData()[0].reference.elements[0].isotope_number, 1)
	//TEST_EQUAL(rs.getNMRData()[0].reference.elements[0].atom_group, "methyl protons")
	//TEST_EQUAL(rs.getNMRData()[0].reference.elements[0].shift_units, "ppm")
	//TEST_REAL_EQUAL(rs.getNMRData()[0].reference.elements[0].shift_value, 0)
	//TEST_EQUAL(rs.getNMRData()[0].reference.elements[0].reference_method, (char)NMRStarFile::INTERNAL_REFERENCE)
	//TEST_EQUAL(rs.getNMRData()[0].reference.elements[0].reference_type, (char)NMRStarFile::DIRECT_TYPE)
	//TEST_REAL_EQUAL(rs.getNMRData()[0].reference.elements[0].indirect_shift_ratio, 1)

	/*
	for (Position i = 0;  i < rs.getNMRData()[0].atom_data.size(); i++)
	{
		cout << *(rs.getNMRData()[0].atom_data[i]) << endl;
	}
	*/
RESULT

NMRStarFile f2;

CHECK(bool operator == (const NMRStarFile& f) )
	NMRStarFile f1;
	TEST_EQUAL(f1 == f2, true)
	f2 = NMRStarFile(BALL_TEST_DATA_PATH(AssignShiftProcessor_test2.str));
	TEST_EQUAL(f1 == f2, false)
RESULT

CHECK(bool operator != (const NMRStarFile& f) )
	NMRStarFile f1;
	TEST_EQUAL(f1 != f1, false)
	TEST_EQUAL(f1 != f2, true)
RESULT

NMRStarFile empty;
CHECK(Size getNumberOfAtoms() const )
	TEST_EQUAL(f2.getNumberOfAtoms(), 1914)
	TEST_EQUAL(empty.getNumberOfAtoms(), 0)
RESULT

CHECK(const std::vector<NMRAtomDataSet>& getNMRData() const )
	TEST_EQUAL(f2.getNMRData().size(), 1)
	TEST_EQUAL(empty.getNMRData().size(), 0)
RESULT

CHECK(void clear() )
	TEST_EQUAL(f2.getNMRData().size(), 1)
	TEST_EQUAL(f2.getNumberOfAtoms(), 1914)
	f2.clear();
	TEST_EQUAL(f2.getNMRData().size(), 0)
	TEST_EQUAL(f2.getNumberOfAtoms(), 0)
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
