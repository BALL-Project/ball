// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/QSAR/surfaceDescriptors.h>
#include <BALL/FORMAT/SDFile.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/PTE.h>

///////////////////////////

START_TEST(SimpleDescriptors, "")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

	SDFile infile("data/descriptors_test.sdf");
	System S;
	infile >> S;

	Molecule * molecule;
	Size limit = S.countMolecules();

CHECK(PositiveVdWSurface)
	PositiveVdWSurface ps;
	S.apply(ps);

	PRECISION(0.001)
	double results[9] = {76.8123, 37.9932, 93.4437, 74.297, 120.694, 109.587, 17.5974, 33.9075};
	for (Size i=0;i!=limit;++i)
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
	double results[9] = {53.5374, 74.7739, 87.5337, 96.7243, 91.4622, 171.441, 86.9424, 96.5323};
	for (Size i=0;i!=limit;++i)
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

	PRECISION(0.0001)
	double results[] = {0, 0, 0, 0, 0, 23.9134, 0, 0, 0};
	for (Size i=0;i!=limit;++i)
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

	PRECISION(0.0001)
	double results[] = {0, 0, 0, 0, 29.9585, 21.2831, 0, 0, 0};
	for (Size i=0;i!=limit;++i)
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
	double results[] = {130.35, 112.767, 180.977, 171.021, 182.198, 235.831, 104.54, 130.44, 69.0822};
	for (Size i=0;i!=limit;++i)
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

	PRECISION(0.0001)
	double results[] = {0, 0, 0, 0, 29.9585, 45.1964, 0, 0, 0};
	for (Size i=0;i!=limit;++i)
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
	double results[] = {0.589279, 0.336917, 0.516328, 0.434431, 0.568893, 0.38995, 0.168332, 0.259948, 0};
	for (Size i=0;i!=limit;++i)
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

	PRECISION(0.00001)
	double results[] = {0.410721, 0.663083, 0.483672, 0.565569, 0.431107, 0.61005, 0.831668, 0.740052};
	for (Size i=0;i!=limit;++i)
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
	double results[] = {0, 0, 0, 0, 0, 0.0850928, 0, 0, 0};
	for (Size i=0;i!=limit;++i)
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
	double results[] = {0, 0, 0, 0, 0.141209, 0.0757331, 0, 0, 0, 0};
	for (Size i=0;i!=limit;++i)
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
	double results[] = {1, 1, 1, 1, 0.858791, 0.839174, 1, 1, 1};
	for (Size i=0;i!=limit;++i)
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
	double results[] = {0, 0, 0, 0, 0.141209, 0.160826, 0, 0, 0, 0};
	for (Size i=0;i!=limit;++i)
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
	double results[] = {108.121, 93.6961, 157.27, 147.551, 175.833, 249.007, 85.4923, 113.514, 52.4826};
	for (Size i=0;i!=limit;++i)
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
	double results[] = {0.778405, 0.833693, 0.783483, 0.814594, 1.36007, 1.26775, 2.95618, 1.79723, 2.36069};
	for (Size i=0;i!=limit;++i)
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
	double results[] = {130.35, 112.767, 180.977, 171.021, 212.157, 281.027, 104.54, 130.44, 69.0822};
	for (Size i=0;i!=limit;++i)
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
