// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/QSAR/simpleDescriptors.h>
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

//////
CHECK(NumberOfBonds)	
	NumberOfBonds nb;
	S.apply(nb);
	double results[] = {18, 12, 24, 21, 22, 32, 4, 12, 6};
	
	for(Size i=0;i!=limit;++i)
	{
		molecule = S.getMolecule(i);
		TEST_EQUAL(molecule->hasProperty("NumberOfBonds"), true)
		if (molecule->hasProperty("NumberOfBonds"))
		{
			double value = molecule->getProperty("NumberOfBonds").getDouble();
			TEST_EQUAL(value, results[i])
		}
	}

RESULT

///////
CHECK(MolecularWeight)
	MolecularWeight mw;
	S.apply(mw);

	PRECISION(0.01)
	double results[] = {84.1616, 78.1138, 123.219, 120.195, 239.145, 315.679, 252.731, 204.01, 123.895};
	for (Size i=0;i!=limit;++i)
	{
		molecule = S.getMolecule(i);
		TEST_EQUAL(molecule->hasProperty("MolecularWeight"), true)
		if (molecule->hasProperty("MolecularWeight"))
		{
			double value = molecule->getProperty("MolecularWeight").getDouble();
			TEST_REAL_EQUAL(value, results[i])
		}
	}

RESULT


///////////
CHECK(NumberOfSingleBonds)
	NumberOfSingleBonds nsb;
	S.apply(nsb);

	double results[] = {18, 6, 24, 15, 15, 24, 4, 6, 6};
	for (Size i=0;i!=limit;++i)
	{
		molecule = S.getMolecule(i);
		TEST_EQUAL(molecule->hasProperty("NumberOfSingleBonds"), true)
		if (molecule->hasProperty("NumberOfSingleBonds"))
		{
			double value = molecule->getProperty("NumberOfSingleBonds").getDouble();
			TEST_EQUAL(value, results[i])
		}
	}

RESULT


///////////////
CHECK(NumberOfDoubleBonds)
	NumberOfDoubleBonds ndb;
	S.apply(ndb);

	double results[] = {0, 0, 0, 0, 1, 1, 0, 0, 0};
	for (Size i=0;i!=limit;++i)
	{
		molecule = S.getMolecule(i);
		TEST_EQUAL(molecule->hasProperty("NumberOfDoubleBonds"), true)
		if (molecule->hasProperty("NumberOfDoubleBonds"))
		{
			double value = molecule->getProperty("NumberOfDoubleBonds").getDouble();
			TEST_EQUAL(value, results[i])
		}
	}

RESULT


////////////////////////
CHECK(NumberOfTripleBonds)
	NumberOfTripleBonds ntb;
	S.apply(ntb);

	double results[] = {0, 0, 0, 0, 0, 1, 0, 0, 0};
	for (Size i=0;i!=limit;++i)
	{
		molecule = S.getMolecule(i);
		TEST_EQUAL(molecule->hasProperty("NumberOfTripleBonds"), true)
		if (molecule->hasProperty("NumberOfTripleBonds"))
		{
			double value = molecule->getProperty("NumberOfTripleBonds").getDouble();
			ABORT_IF(i > 10)
			TEST_EQUAL(value, results[i])
		}
	}

RESULT


/////////////////////////
//CHECK(NumberOfQuadrupleBonds)


/////////////////////
CHECK(NumberOfAromaticBonds)
	NumberOfAromaticBonds nab;
	S.apply(nab);

	double results[] = {0, 6, 0, 6, 6, 6, 0, 6, 0};
	for (Size i=0;i!=limit;++i)
	{
		molecule = S.getMolecule(i);
		TEST_EQUAL(molecule->hasProperty("NumberOfAromaticBonds"), true)
		if (molecule->hasProperty("NumberOfAromaticBonds"))
		{
			double value = molecule->getProperty("NumberOfAromaticBonds").getDouble();
			TEST_EQUAL(value, results[i])
		}
	}

RESULT


//////////////
CHECK(NumberOfAtoms)
	NumberOfAtoms na;
	S.apply(na);

	double results[] = {18, 12, 24, 21, 22, 30, 5, 12, 4};
	for (Size i=0;i!=limit;++i)
	{
		molecule = S.getMolecule(i);
		TEST_EQUAL(molecule->hasProperty("NumberOfAtoms"), true)
		if (molecule->hasProperty("NumberOfAtoms"))
		{
			double value = molecule->getProperty("NumberOfAtoms").getDouble();
			TEST_EQUAL(value, results[i])
		}
	}

RESULT


////////////////////////
CHECK(NumberOfBoron)
	NumberOfBoron nb;
	S.apply(nb);

	double results[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
	for (Size i=0;i!=limit;++i)
	{
		molecule = S.getMolecule(i);
		TEST_EQUAL(molecule->hasProperty("NumberOfBoron"), true)
		if (molecule->hasProperty("NumberOfBoron"))
		{
			double value = molecule->getProperty("NumberOfBoron").getDouble();
			TEST_EQUAL(value, results[i])
		}
	}

RESULT

////////////////////////
CHECK(NumberOfCarbon)
	NumberOfCarbon nc;
	S.apply(nc);

	double results[] = {6, 6, 9, 9, 9, 14, 1, 6, 0};
	for (Size i=0;i!=limit;++i)
	{
		molecule = S.getMolecule(i);
		TEST_EQUAL(molecule->hasProperty("NumberOfCarbon"), true)
		if (molecule->hasProperty("NumberOfCarbon"))
		{
			double value = molecule->getProperty("NumberOfCarbon").getDouble();
			TEST_EQUAL(value, results[i])
		}
	}

RESULT

////////////////////////
CHECK(NumberOfNitrogen)
	NumberOfNitrogen nn;
	S.apply(nn);

	double results[] = {0, 0, 0, 0, 1, 1, 0, 0, 0};
	for (Size i=0;i!=limit;++i)
	{
		molecule = S.getMolecule(i);
		TEST_EQUAL(molecule->hasProperty("NumberOfNitrogen"), true)
		if (molecule->hasProperty("NumberOfNitrogen"))
		{
			double value = molecule->getProperty("NumberOfNitrogen").getDouble();
			TEST_EQUAL(value, results[i])
		}
	}

RESULT


////////////////////////
CHECK(NumberOfOxygen)
	NumberOfOxygen no;
	S.apply(no);

	double results[] = {0, 0, 0, 0, 1, 2, 0, 0, 0};
	for (Size i=0;i!=limit;++i)
	{
		molecule = S.getMolecule(i);
		TEST_EQUAL(molecule->hasProperty("NumberOfOxygen"), true)
		if (molecule->hasProperty("NumberOfOxygen"))
		{
			double value = molecule->getProperty("NumberOfOxygen").getDouble();
			TEST_EQUAL(value, results[i])
		}
	}

RESULT

////////////////////////
CHECK(NumberOfFlourine)
	NumberOfFlourine nf;
	S.apply(nf);

	double results[] = {0, 0, 0, 0, 5, 3, 0, 0, 0};
	for (Size i=0;i!=limit;++i)
	{
		molecule = S.getMolecule(i);
		TEST_EQUAL(molecule->hasProperty("NumberOfFlourine"), true)
		if (molecule->hasProperty("NumberOfFlourine"))
		{
			double value = molecule->getProperty("NumberOfFlourine").getDouble();
			TEST_EQUAL(value, results[i])
		}
	}

RESULT


////////////////////////
CHECK(NumberOfPhosphorus)
	NumberOfPhosphorus np;
	S.apply(np);

	double results[] = {0, 0, 0, 0, 0, 0, 0, 0, 4};
	for (Size i=0;i!=limit;++i)
	{
		molecule = S.getMolecule(i);
		TEST_EQUAL(molecule->hasProperty("NumberOfPhosphorus"), true)
		if (molecule->hasProperty("NumberOfPhosphorus"))
		{
			double value = molecule->getProperty("NumberOfPhosphorus").getDouble();
			TEST_EQUAL(value, results[i])
		}
	}

RESULT


////////////////////////
CHECK(NumberOfSulfur)
	NumberOfSulfur ns;
	S.apply(ns);

	double results[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
	for (Size i=0;i!=limit;++i)
	{
		molecule = S.getMolecule(i);
		TEST_EQUAL(molecule->hasProperty("NumberOfSulfur"), true)
		if (molecule->hasProperty("NumberOfSulfur"))
		{
			double value = molecule->getProperty("NumberOfSulfur").getDouble();
			TEST_EQUAL(value, results[i])
		}
	}

RESULT

////////////////////////
CHECK(NumberOfChlorine)
	NumberOfChlorine nc;
	S.apply(nc);

	double results[] = {0, 0, 0, 0, 0, 1, 0, 0, 0};
	for (Size i=0;i!=limit;++i)
	{
		molecule = S.getMolecule(i);
		TEST_EQUAL(molecule->hasProperty("NumberOfChlorine"), true)
		if (molecule->hasProperty("NumberOfChlorine"))
		{
			double value = molecule->getProperty("NumberOfChlorine").getDouble();
			TEST_EQUAL(value, results[i])
		}
	}

RESULT

////////////////////////
CHECK(NumberOfBromine)
	NumberOfBromine nb;
	S.apply(nb);

	double results[] = {0, 0, 0, 0, 0, 0, 3, 0, 0};
	for (Size i=0;i!=limit;++i)
	{
		molecule = S.getMolecule(i);
		TEST_EQUAL(molecule->hasProperty("NumberOfBromine"), true)
		if (molecule->hasProperty("NumberOfBromine"))
		{
			double value = molecule->getProperty("NumberOfBromine").getDouble();
			TEST_EQUAL(value, results[i])
		}
	}

RESULT

////////////////////////
CHECK(NumberOfIodine)
	NumberOfIodine ni;
	S.apply(ni);

	double results[] = {0, 0, 0, 0, 0, 0, 0, 1, 0};
	for (Size i=0;i!=limit;++i)
	{
		molecule = S.getMolecule(i);
		TEST_EQUAL(molecule->hasProperty("NumberOfIodine"), true)
		if (molecule->hasProperty("NumberOfIodine"))
		{
			double value = molecule->getProperty("NumberOfIodine").getDouble();
			TEST_EQUAL(value, results[i])
		}
	}

RESULT

////////////////////////
CHECK(NumberOfHydrogen)
	NumberOfHydrogen nh;
	S.apply(nh);

	double results[] = {12, 6, 15, 12, 6, 9, 1, 5, 0};
	for (Size i=0;i!=limit;++i)
	{
		molecule = S.getMolecule(i);
		TEST_EQUAL(molecule->hasProperty("NumberOfHydrogen"), true)
		if (molecule->hasProperty("NumberOfHydrogen"))
		{
			double value = molecule->getProperty("NumberOfHydrogen").getDouble();
			TEST_EQUAL(value, results[i])
		}
	}

RESULT


////////////////////////
CHECK(FormalCharge)
	FormalCharge fc;
	S.apply(fc);

	double results[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
	for (Size i=0;i!=limit;++i)
	{
		molecule = S.getMolecule(i);
		TEST_EQUAL(molecule->hasProperty("FormalCharge"), true)
		if (molecule->hasProperty("FormalCharge"))
		{
			double value = molecule->getProperty("FormalCharge").getDouble();
			TEST_EQUAL(value, results[i])
		}
	}

RESULT


////////////////////////
CHECK(NumberOfHeavyAtoms)
	NumberOfHeavyAtoms nh;
	S.apply(nh);

	double results[] = {6, 6, 9, 9, 16, 21, 4, 7, 4};
	for (Size i=0;i!=limit;++i)
	{
		molecule = S.getMolecule(i);
		TEST_EQUAL(molecule->hasProperty("NumberOfHeavyAtoms"), true)
		if (molecule->hasProperty("NumberOfHeavyAtoms"))
		{
			double value = molecule->getProperty("NumberOfHeavyAtoms").getDouble();
			TEST_EQUAL(value, results[i])
		}
	}

RESULT


////////////////////////
CHECK(MeanAtomInformationContent)
	MeanAtomInformationContent maic;
	S.apply(maic);

	PRECISION(0.00001)
	double results[] = {0.918296, 1, 0.954434, 0.985228, 1.92994, 1.95398, 1.37095, 1.32501, 0};
	for (Size i=0;i!=limit;++i)
	{
		molecule = S.getMolecule(i);
		TEST_EQUAL(molecule->hasProperty("MeanAtomInformationContent"), true)
		if (molecule->hasProperty("MeanAtomInformationContent"))
		{
			double value = molecule->getProperty("MeanAtomInformationContent").getDouble();
			TEST_REAL_EQUAL(value, results[i])
		}
	}

RESULT


////////////////////////
CHECK(AtomInformationContent)
	AtomInformationContent aic;
	S.apply(aic);

	PRECISION(0.0001)
	double results[] = {16.5293, 12, 22.9064, 20.6898, 42.4588, 58.6195, 6.85475, 15.9001, 0};
	for (Size i=0;i!=limit;++i)
	{
		molecule = S.getMolecule(i);
		TEST_EQUAL(molecule->hasProperty("AtomInformationContent"), true)
		if (molecule->hasProperty("AtomInformationContent"))
		{
			double value = molecule->getProperty("AtomInformationContent").getDouble();
			TEST_REAL_EQUAL(value, results[i])
		}
	}

RESULT


////////////////////////
CHECK(NumberOfRotatableSingleBonds)
	NumberOfRotatableSingleBonds nrsb;
	S.apply(nrsb);

	double results[] = {0, 0, 3, 3, 4, 3, 0, 0, 0};
	for (Size i=0;i!=limit;++i)
	{
		molecule = S.getMolecule(i);
		TEST_EQUAL(molecule->hasProperty("NumberOfRotatableSingleBonds"), true)
		if (molecule->hasProperty("NumberOfRotatableSingleBonds"))
		{
			double value = molecule->getProperty("NumberOfRotatableSingleBonds").getDouble();
			TEST_EQUAL(value, results[i])
		}
	}

RESULT


////////////////////////
CHECK(RelNumberOfRotatableSingleBonds)
	RelNumberOfRotatableSingleBonds rnrsb;
	S.apply(rnrsb);

	PRECISION(0.001)
	double results[] = {0, 0, 0.125, 0.142857, 0.181818, 0.09375, 0, 0, 0};
	for (Size i=0;i!=limit;++i)
	{
		molecule = S.getMolecule(i);
		TEST_EQUAL(molecule->hasProperty("RelNumberOfRotatableSingleBonds"), true)
		if (molecule->hasProperty("RelNumberOfRotatableSingleBonds"))
		{
			double value = molecule->getProperty("RelNumberOfRotatableSingleBonds").getDouble();
			ABORT_IF(i > 10)
			TEST_REAL_EQUAL(value, results[i])
		}
	}

RESULT


////////////////////////
CHECK(NumberOfAromaticAtoms)
	NumberOfAromaticAtoms naa;
	S.apply(naa);

	double results[] = {0, 6, 0, 6, 6, 6, 0, 6, 0};
	for (Size i=0;i!=limit;++i)
	{
		molecule = S.getMolecule(i);
		TEST_EQUAL(molecule->hasProperty("NumberOfAromaticAtoms"), true)
		if (molecule->hasProperty("NumberOfAromaticBonds"))
		{
			double value = molecule->getProperty("NumberOfAromaticAtoms").getDouble();
			ABORT_IF(i > 10)
			TEST_EQUAL(value, results[i])
		}
	}

RESULT


////////////////////////
CHECK(NumberOfHeavyBonds)
	NumberOfHeavyBonds nhb;
	S.apply(nhb);

	double results[] = {6, 6, 9, 9, 16, 23, 3, 7, 6};
	for (Size i=0;i!=limit;++i)
	{
		molecule = S.getMolecule(i);
		TEST_EQUAL(molecule->hasProperty("NumberOfHeavyBonds"), true)
		if (molecule->hasProperty("NumberOfHeavyBonds"))
		{
			double value = molecule->getProperty("NumberOfHeavyBonds").getDouble();
			ABORT_IF(i > 10)
			TEST_EQUAL(value, results[i])
		}
	}

RESULT


////////////////////////
CHECK(NumberOfRotatableBonds)
	NumberOfRotatableBonds nrb;
	S.apply(nrb);

	double results[] = {0, 0, 3, 3, 5, 4, 0, 0, 0};
	for (Size i=0;i!=limit;++i)
	{
		molecule = S.getMolecule(i);
		TEST_EQUAL(molecule->hasProperty("NumberOfRotatableBonds"), true)
		if (molecule->hasProperty("NumberOfRotatableBonds"))
		{
			double value = molecule->getProperty("NumberOfRotatableBonds").getDouble();
			ABORT_IF(i > 10)
			TEST_EQUAL(value, results[i])
		}
	}

RESULT


////////////////////////
CHECK(RelNumberOfRotatableBonds)
	RelNumberOfRotatableBonds rnrb;
	S.apply(rnrb);

	PRECISION(0.00001)
	double results[] = {0, 0, 0.125, 0.142857, 0.227273, 0.125, 0, 0, 0};
	for (Size i=0;i!=limit;++i)
	{
		molecule = S.getMolecule(i);
		TEST_EQUAL(molecule->hasProperty("RelNumberOfRotatableBonds"), true)
		if (molecule->hasProperty("RelNumberOfRotatableBonds"))
		{
			double value = molecule->getProperty("RelNumberOfRotatableBonds").getDouble();
			TEST_REAL_EQUAL(value, results[i])
		}
	}

RESULT


////////////////////////
CHECK(VertexAdjacency)
	VertexAdjacency va;
	S.apply(va);

	PRECISION(0.00001)
	double results[] = {3.58496, 3.58496, 4.16993, 4.16993, 5.0, 5.52356, 2.58496, 3.80735, 3.58496};
	for (Size i=0;i!=limit;++i)
	{
		molecule = S.getMolecule(i);
		TEST_EQUAL(molecule->hasProperty("VertexAdjacency"), true)
		if (molecule->hasProperty("VertexAdjacency"))
		{
			double value = molecule->getProperty("VertexAdjacency").getDouble();
			TEST_REAL_EQUAL(value, results[i])
		}
	}

RESULT


////////////////////////
CHECK(VertexAdjacencyEquality)
	VertexAdjacencyEquality vae;
	S.apply(vae);

	PRECISION(0.00001)
	double results[] = {0.918296, 0.918296, 0.764205, 0.764205, 0.543564, 0.482506, 0.954435, 0.863121, 0.811278};
	for (Size i=0;i!=limit;++i)
	{
		molecule = S.getMolecule(i);
		TEST_EQUAL(molecule->hasProperty("VertexAdjacencyEquality"), true)
		if (molecule->hasProperty("VertexAdjacencyEquality"))
		{
			double value = molecule->getProperty("VertexAdjacencyEquality").getDouble();
			TEST_REAL_EQUAL(value, results[i])
		}
	}

RESULT



////////////////////////
CHECK(NumberOfHydrogenBondAcceptors)
	NumberOfHydrogenBondAcceptors nhba;
	S.apply(nhba);

	double results[] = {0, 0, 0, 0, 5, 4, 0, 0, 0};
	for (Size i=0;i!=limit;++i)
	{
		molecule = S.getMolecule(i);
		TEST_EQUAL(molecule->hasProperty("NumberOfHydrogenBondAcceptors"), true)
		if (molecule->hasProperty("NumberOfHydrogenBondAcceptors"))
		{
			double value = molecule->getProperty("NumberOfHydrogenBondAcceptors").getDouble();
			TEST_EQUAL(value, results[i])
		}
	}

RESULT


////////////////////////
CHECK(NumberOfHydrogenBondsDonors)
	NumberOfHydrogenBondDonors nhbd;
	S.apply(nhbd);

	double results[] = {0, 0, 0, 0, 0, 1, 0, 0, 0};
	for (Size i=0;i!=limit;++i)
	{
		molecule = S.getMolecule(i);
		TEST_EQUAL(molecule->hasProperty("NumberOfHydrogenBondDonors"), true)
		if (molecule->hasProperty("NumberOfHydrogenBondDonors"))
		{
			double value = molecule->getProperty("NumberOfHydrogenBondDonors").getDouble();
			TEST_EQUAL(value, results[i])
		}
	}

RESULT


////////////////////////
CHECK(NumberOfHydrophobicAtoms)
	NumberOfHydrophobicAtoms nha;
	S.apply(nha);

	double results[] = {6, 6, 9, 9, 2, 9, 0, 5, 0};
	for (Size i=0;i!=limit;++i)
	{
		molecule = S.getMolecule(i);
		TEST_EQUAL(molecule->hasProperty("NumberOfHydrophobicAtoms"), true)
		if (molecule->hasProperty("NumberOfHydrophobicAtoms"))
		{
			double value = molecule->getProperty("NumberOfHydrophobicAtoms").getDouble();
			ABORT_IF(i > 10)
			TEST_EQUAL(value, results[i])
		}
	}

RESULT


////////////////////////
CHECK(BondPolarizabilities)
	BondPolarizabilities bp;
	S.apply(bp);

	PRECISION(0.0001)
	double results[] = {13.1185, 6.55924, 16.3981, 13.1185, 14.4902, 17.4019, 4.96321, 9.05603, 0};
	for (Size i=0;i!=limit;++i)
	{
		molecule = S.getMolecule(i);
		TEST_EQUAL(molecule->hasProperty("BondPolarizabilities"), true)
		if (molecule->hasProperty("BondPolarizabilities"))
		{
			double value = molecule->getProperty("BondPolarizabilities").getDouble();
			ABORT_IF(i > 10)
			TEST_REAL_EQUAL(value, results[i])
		}
	}

RESULT


////////////////////////
CHECK(AtomicPolarizabilities)
	AtomicPolarizabilities ap;
	S.apply(ap);

	PRECISION(0.01)
	double results[] = {18.5615, 14.5608, 25.8419, 23.8415, 24.5278, 37.1961, 11.5768, 19.244, 14.52};
	for (Size i=0;i!=limit;++i)
	{
		molecule = S.getMolecule(i);
		TEST_EQUAL(molecule->hasProperty("AtomicPolarizabilities"), true)
		if (molecule->hasProperty("AtomicPolarizabilities"))
		{
			double value = molecule->getProperty("AtomicPolarizabilities").getDouble();
			ABORT_IF(i > 10)
			TEST_REAL_EQUAL(value, results[i])
		}
	}

RESULT


////////////////////////
CHECK(SizeOfSSSR)
	SizeOfSSSR ss;
	S.apply(ss);

	double results[] = {1, 1, 1, 1, 1, 3, 0, 1, 3};
	for (Size i=0;i!=limit;++i)
	{
		molecule = S.getMolecule(i);
		TEST_EQUAL(molecule->hasProperty("SizeOfSSSR"), true)
		if (molecule->hasProperty("SizeOfSSSR"))
		{
			double value = molecule->getProperty("SizeOfSSSR").getDouble();
			TEST_REAL_EQUAL(value, results[i])
		}
	}

RESULT


////////////////////////
CHECK(PrincipalMomentOfInertia)
	PrincipalMomentOfInertia pmi;
	S.apply(pmi);

	PRECISION(0.01)
	double results[] = {219.81, 179.678, 556.085, 516.221, 2080.95, 3232.86, 616.099, 488.358, 54.6024};
	for (Size i=0;i!=limit;++i)
	{
		molecule = S.getMolecule(i);
		TEST_EQUAL(molecule->hasProperty("PrincipalMomentOfInertia"), true)
		if (molecule->hasProperty("PrincipalMomentOfInertia"))
		{
			double value = molecule->getProperty("PrincipalMomentOfInertia").getDouble();
			TEST_REAL_EQUAL(value, results[i])
		}
	}

RESULT


////////////////////////
CHECK(PrincipalMomentOfInertiaX)
	PrincipalMomentOfInertiaX pmix;
	S.apply(pmix);

	PRECISION(0.01)
	double results[] = {101.989, 89.8286, 247.445, 228.849, 1451.82, 1340.25, 494.688, 335.529, 27.3012};
	for (Size i=0;i!=limit;++i)
	{
		molecule = S.getMolecule(i);
		TEST_EQUAL(molecule->hasProperty("PrincipalMomentOfInertiaX"), true)
		if (molecule->hasProperty("PrincipalMomentOfInertiaX"))
		{
			double value = molecule->getProperty("PrincipalMomentOfInertiaX").getDouble();
			TEST_REAL_EQUAL(value, results[i])
		}
	}

RESULT


////////////////////////
CHECK(PrincipalMomentOfInertiaY)
	PrincipalMomentOfInertiaY pmiy;
	S.apply(pmiy);

	PRECISION(0.01)
	double results[] = {101.615, 89.8493, 281.897, 273.478, 566.259, 1447.67, 121.411, 152.829, 27.3012};
	for (Size i=0;i!=limit;++i)
	{
		molecule = S.getMolecule(i);
		TEST_EQUAL(molecule->hasProperty("PrincipalMomentOfInertiaY"), true)
		if (molecule->hasProperty("PrincipalMomentOfInertiaY"))
		{
			double value = molecule->getProperty("PrincipalMomentOfInertiaY").getDouble();
			TEST_REAL_EQUAL(value, results[i])
		}
	}

RESULT


////////////////////////
CHECK(PrincipalMomentOfInertiaZ)
	PrincipalMomentOfInertiaZ pmiz;
	S.apply(pmiz);

	PRECISION(0.01)
	double results[] = {16.206, 0, 26.7422, 13.8964, 62.8705, 444.94, 0, 0, 0};
	for (Size i=0;i!=limit;++i)
	{
		molecule = S.getMolecule(i);
		TEST_EQUAL(molecule->hasProperty("PrincipalMomentOfInertiaZ"), true)
		if (molecule->hasProperty("PrincipalMomentOfInertiaZ"))
		{
			double value = molecule->getProperty("PrincipalMomentOfInertiaZ").getDouble();
			TEST_REAL_EQUAL(value, results[i])
		}
	}

RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST

