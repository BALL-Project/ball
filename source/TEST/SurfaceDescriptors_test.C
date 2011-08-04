// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/QSAR/surfaceDescriptors.h>
#include <BALL/FORMAT/SDFile.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/PTE.h>

///////////////////////////

START_TEST(SimpleDescriptors)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

	SDFile infile(BALL_TEST_DATA_PATH(descriptors_test.sdf));
	System S;
	infile >> S;

	Molecule * molecule;
	Size limit = S.countMolecules();

CHECK(PositiveVdWSurface)
	PositiveVdWSurface ps;
	S.apply(ps);

	PRECISION(0.001)
	double results[9] = {76.8874, 37.9932, 93.4062, 74.3345, 120.657, 109.572, 17.5974, 33.9075};
	for (Size i = 0; i < limit ;++i)
	{
		molecule = S.getMolecule(i);
		TEST_EQUAL(molecule->hasProperty("PositiveVdWSurface"), true)
		if (molecule->hasProperty("PositiveVdWSurface"))
		{
			double value = molecule->getProperty("PositiveVdWSurface").getDouble();
			TEST_REAL_EQUAL(value, results[i])
		}
	}

RESULT


///////////////////////
CHECK(NegativeVdWSurface)
	NegativeVdWSurface ns;
	S.apply(ns);

	PRECISION(0.001)
	double results[9] = {53.359, 74.417, 87.3552, 96.7243, 91.6465, 171.488, 86.8433, 96.2646};
	for (Size i = 0; i < limit ;++i)
	{
		molecule = S.getMolecule(i);
		TEST_EQUAL(molecule->hasProperty("NegativeVdWSurface"), true)
		if (molecule->hasProperty("NegativeVdWSurface"))
		{
			double value = molecule->getProperty("NegativeVdWSurface").getDouble();
			TEST_REAL_EQUAL(value, results[i])
		}
	}

RESULT


/////////////////////////
CHECK(PositivePolarVdWSurface)
	PositivePolarVdWSurface pps;
	S.apply(pps);

	PRECISION(0.001)
	double results[] = {0, 0, 0, 0, 0, 23.8242, 0, 0, 0};
	for (Size i = 0; i < limit ;++i)
	{
		molecule = S.getMolecule(i);
		TEST_EQUAL(molecule->hasProperty("PositivePolarVdWSurface"), true)
		if (molecule->hasProperty("PositivePolarVdWSurface"))
		{
			double value = molecule->getProperty("PositivePolarVdWSurface").getDouble();
			TEST_REAL_EQUAL(value, results[i])
		}
	}

RESULT


/////////////////////////////
CHECK(NegativePolarVdWSurface)
	NegativePolarVdWSurface nps;
	S.apply(nps);

	PRECISION(0.001)
	double results[] = {0, 0, 0, 0, 30.006, 21.3342, 0, 0, 0};
	for (Size i = 0; i < limit ;++i)
	{
		molecule = S.getMolecule(i);
		TEST_EQUAL(molecule->hasProperty("NegativePolarVdWSurface"), true)
		if (molecule->hasProperty("NegativePolarVdWSurface"))
		{
			double value = molecule->getProperty("NegativePolarVdWSurface").getDouble();
			TEST_REAL_EQUAL(value, results[i])
		}
	}

RESULT


///////////////////////////
CHECK(HydrophobicVdWSurface)
	HydrophobicVdWSurface hs;
	S.apply(hs);

	PRECISION(0.01)
	double results[] = {130.246, 112.41, 180.761, 171.059, 182.297, 235.902, 104.441, 130.172, 69.0822};
	for (Size i = 0; i < limit ;++i)
	{
		molecule = S.getMolecule(i);
		TEST_EQUAL(molecule->hasProperty("HydrophobicVdWSurface"), true)
		if (molecule->hasProperty("HydrophobicVdWSurface"))
		{
			double value = molecule->getProperty("HydrophobicVdWSurface").getDouble();
			TEST_REAL_EQUAL(value, results[i])
		}
	}

RESULT


////////////////////////
CHECK(PolarVdWSurface)
	PolarVdWSurface ps;
	S.apply(ps);

	PRECISION(0.001)
	double results[] = {0, 0, 0, 0, 30.006, 45.1583, 0, 0, 0};
	for (Size i = 0; i < limit ;++i)
	{
		molecule = S.getMolecule(i);
		TEST_EQUAL(molecule->hasProperty("PolarVdWSurface"), true)
		if (molecule->hasProperty("PolarVdWSurface"))
		{
			double value = molecule->getProperty("PolarVdWSurface").getDouble();
			TEST_REAL_EQUAL(value, results[i])
		}
	}

RESULT


/////////////////////////////////
CHECK(RelPositiveVdWSurface)
	RelPositiveVdWSurface rps;
	S.apply(rps);

	PRECISION(0.00001)
	double results[] = {0.590323, 0.337987, 0.516737, 0.434555, 0.568323, 0.389854, 0.168491, 0.260482, 0};
	for (Size i = 0; i < limit ;++i)
	{
		molecule = S.getMolecule(i);
		TEST_EQUAL(molecule->hasProperty("RelPositiveVdWSurface"), true)
		if (molecule->hasProperty("RelPositiveVdWSurface"))
		{
			double value = molecule->getProperty("RelPositiveVdWSurface").getDouble();
			TEST_REAL_EQUAL(value, results[i])
		}
	}

RESULT


//////////////////////////////7
CHECK(RelNegativeVdWSurface)
	RelNegativeVdWSurface rns;
	S.apply(rns);

	PRECISION(0.0001)
	double results[] = {0.409677, 0.662013, 0.483263, 0.565445, 0.431677, 0.61005, 0.831509, 0.739518, 0};
	for (Size i = 0; i < limit ;++i)
	{
		molecule = S.getMolecule(i);
		TEST_EQUAL(molecule->hasProperty("RelNegativeVdWSurface"), true)
		if (molecule->hasProperty("RelNegativeVdWSurface"))
		{
			double value = molecule->getProperty("RelNegativeVdWSurface").getDouble();
			TEST_REAL_EQUAL(value, results[i])
		}
	}

RESULT


///////////////////////////
CHECK(RelPositivePolarVdWSurface)
	RelPositivePolarVdWSurface rpps;
	S.apply(rpps);

	PRECISION(0.00001)
	double results[] = {0, 0, 0, 0, 0, 0.0847653, 0, 0, 0};
	for (Size i = 0; i < limit ;++i)
	{
		molecule = S.getMolecule(i);
		TEST_EQUAL(molecule->hasProperty("RelPositivePolarVdWSurface"), true)
		if (molecule->hasProperty("RelPositivePolarVdWSurface"))
		{
			double value = molecule->getProperty("RelPositivePolarVdWSurface").getDouble();
			TEST_REAL_EQUAL(value, results[i])
		}
	}

RESULT


//////////////////////////////
CHECK(RelNegativePolarVdWSurface)
	RelNegativePolarVdWSurface rnps;
	S.apply(rnps);

	PRECISION(0.00001)
	double results[] = {0, 0, 0, 0, 0.141335, 0.0759059, 0, 0, 0, 0};
	for (Size i = 0; i < limit ;++i)
	{
		molecule = S.getMolecule(i);
		TEST_EQUAL(molecule->hasProperty("RelNegativePolarVdWSurface"), true)
		if (molecule->hasProperty("RelNegativePolarVdWSurface"))
		{
			double value = molecule->getProperty("RelNegativePolarVdWSurface").getDouble();
			TEST_REAL_EQUAL(value, results[i])
		}
	}

RESULT


////////////////////////
CHECK(RelHydrophobicVdWSurface)
	RelHydrophobicVdWSurface rhs;
	S.apply(rhs);

	PRECISION(0.0001)
	double results[] = {1, 1, 1, 1, 0.858665, 0.839329, 1, 1, 1};
	for (Size i = 0; i < limit ;++i)
	{
		molecule = S.getMolecule(i);
		TEST_EQUAL(molecule->hasProperty("RelHydrophobicVdWSurface"), true)
		if (molecule->hasProperty("RelHydrophobicVdWSurface"))
		{
			double value = molecule->getProperty("RelHydrophobicVdWSurface").getDouble();
			TEST_REAL_EQUAL(value, results[i])
		}
	}

RESULT


///////////////////////////
CHECK(RelPolarVdWSurface)
	RelPolarVdWSurface rps;
	S.apply(rps);

	PRECISION(0.00001)
	double results[] = {0, 0, 0, 0, 0.141335, 0.160671, 0, 0, 0, 0};
	for (Size i = 0; i < limit ;++i)
	{
		molecule = S.getMolecule(i);
		TEST_EQUAL(molecule->hasProperty("RelPolarVdWSurface"), true)
		if (molecule->hasProperty("RelPolarVdWSurface"))
		{
			double value = molecule->getProperty("RelPolarVdWSurface").getDouble();
			TEST_REAL_EQUAL(value, results[i])
		}
	}

RESULT


//////////////////////////////
CHECK(VdWVolume)
	VdWVolume vol;
	S.apply(vol);

	PRECISION(0.01)
	double results[] = {107.95, 93.5151, 157.119, 147.588, 175.931, 248.93, 85.4505, 113.348, 52.4826};
	for (Size i = 0; i < limit ;++i)
	{
		molecule = S.getMolecule(i);
		TEST_EQUAL(molecule->hasProperty("VdWVolume"), true)
		if (molecule->hasProperty("VdWVolume"))
		{
			double value = molecule->getProperty("VdWVolume").getDouble();
			TEST_REAL_EQUAL(value, results[i])
		}
	}

RESULT


////////////////////////////////
CHECK(Density)
	Density d;
	S.apply(d);

	PRECISION(0.0001)
	double results[] = {0.779635, 0.835307, 0.784235, 0.814392, 1.35931, 1.26814, 2.95763, 1.79985, 2.36069};
	for (Size i = 0; i < limit ;++i)
	{
		molecule = S.getMolecule(i);
		TEST_EQUAL(molecule->hasProperty("Density"), true)
		if (molecule->hasProperty("Density"))
		{
			double value = molecule->getProperty("Density").getDouble();
			TEST_REAL_EQUAL(value, results[i])
		}
	}

RESULT


////////////////////////
CHECK(VdWSurface)
	VdWSurface s;
	S.apply(s);

	PRECISION(0.01)
	double results[] = {130.246, 112.41, 180.761, 171.059, 212.303, 281.06, 104.441, 130.172, 69.0822};
	for (Size i = 0; i < limit ;++i)
	{
		molecule = S.getMolecule(i);
		TEST_EQUAL(molecule->hasProperty("VdWSurface"), true)
		if (molecule->hasProperty("VdWSurface"))
		{
			double value = molecule->getProperty("VdWSurface").getDouble();
			TEST_REAL_EQUAL(value, results[i])
		}
	}

RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
