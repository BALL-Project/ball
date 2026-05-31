// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/FORMAT/HINFile.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/STRUCTURE/residueChecker.h>
#include <BALL/NMR/spectrum.h>
#include <BALL/NMR/peak.h>
#include <BALL/KERNEL/system.h>
///////////////////////////

START_TEST(Spectrum1D)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace std;

Spectrum1D* p;

CHECK(Spectrum1D::Spectrum1D())
	p = new Spectrum1D;
	TEST_NOT_EQUAL(p, 0);
RESULT

CHECK(Spectrum1D::~Spectrum1D())
	delete p;
RESULT

// BUG-601: Spectrum<RegularData2D, Peak2D>::computeAllMoments was a no-op (its
// whole body commented out since commit b499647), so the moment vectors stayed
// empty. This CHECK builds a deterministic 2x2 grid (flat values 1,2,3,4),
// computes the moments, and asserts them against values cross-checked by
// mirroring the verified 1D algorithm. Red against the no-op (vectors empty),
// green against the re-implementation. The public moment vectors
// (normal_moments / central_moments / standardized_moments) are read directly,
// the same access path the 1D getNormalMomentsDifference uses.
CHECK(Spectrum2D::computeAllMoments() [BUG-601])
	// 2x2 grid, unit spacing/dimension. data_[index.x + size.x*index.y].
	RegularData2D::IndexType grid_size(2, 2);
	RegularData2D data2d(grid_size,
	                     RegularData2D::CoordinateType(0.0f, 0.0f),
	                     RegularData2D::CoordinateType(1.0f, 1.0f));
	// fill flat positions 0..3 with 1,2,3,4
	data2d[(BALL::Position)0] = 1.0f;
	data2d[(BALL::Position)1] = 2.0f;
	data2d[(BALL::Position)2] = 3.0f;
	data2d[(BALL::Position)3] = 4.0f;

	Spectrum2D spec2d(data2d);

	// no-op state: moment vectors are empty before the call
	TEST_EQUAL(spec2d.normal_moments.size(), 0)

	spec2d.computeAllMoments(4);

	// the vectors must now be populated (this alone fails against the no-op)
	TEST_EQUAL(spec2d.normal_moments.size(), 4)
	TEST_EQUAL(spec2d.central_moments.size(), 4)
	TEST_EQUAL(spec2d.standardized_moments.size(), 4)

	// expected values (mirrored from the 1D algorithm; average = 10):
	// normal:  [10, 2, 5, 13.4]
	// central: [10, 0, 1, -0.6]
	// std (sd=1): [10, 0, 1, -0.6]
	PRECISION(1e-3)
	TEST_REAL_EQUAL(spec2d.normal_moments[0], 10.0)
	TEST_REAL_EQUAL(spec2d.normal_moments[1], 2.0)
	TEST_REAL_EQUAL(spec2d.normal_moments[2], 5.0)
	TEST_REAL_EQUAL(spec2d.normal_moments[3], 13.4)

	TEST_REAL_EQUAL(spec2d.central_moments[0], 10.0)
	TEST_REAL_EQUAL(spec2d.central_moments[1], 0.0)
	TEST_REAL_EQUAL(spec2d.central_moments[2], 1.0)
	TEST_REAL_EQUAL(spec2d.central_moments[3], -0.6)

	TEST_REAL_EQUAL(spec2d.standardized_moments[0], 10.0)
	TEST_REAL_EQUAL(spec2d.standardized_moments[2], 1.0)
	TEST_REAL_EQUAL(spec2d.standardized_moments[3], -0.6)
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
