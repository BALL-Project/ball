// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

// insert includes here
#include <BALL/STRUCTURE/surfaceProcessor.h>
#include <BALL/KERNEL/system.h>
#include <BALL/FORMAT/HINFile.h>
#include <list>

///////////////////////////

START_TEST(SurfaceProcessor)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

SurfaceProcessor* surface_processor_ptr = 0;

CHECK(default constructor)
	surface_processor_ptr = new SurfaceProcessor;
	TEST_NOT_EQUAL(surface_processor_ptr, 0)
RESULT

CHECK(destructor)
	delete surface_processor_ptr;
RESULT

CHECK(getSurface())
	SurfaceProcessor proc;
	Surface empty_surface;
	TEST_EQUAL((proc.getSurface() == empty_surface), true)
RESULT

// ... all obvious test missing

CHECK(SurfaceProcessor / single atom)
	HINFile infile(BALL_TEST_DATA_PATH(SurfaceProcessor_test_single_atom.hin));
	System system;
	infile >> system;
	infile.close();
	TEST_EQUAL(system.countAtoms(), 1)

	SurfaceProcessor proc;
	system.apply(proc);
	Surface surface = proc.getSurface();
	TEST_EQUAL(surface.getNumberOfTriangles(), 320)
	TEST_EQUAL(surface.getNumberOfVertices(), 162)
RESULT

CHECK(SurfaceProcessor / methane)
	HINFile infile(BALL_TEST_DATA_PATH(methane.hin));
	System system;
	infile >> system;
	infile.close();
	TEST_EQUAL(system.countAtoms(), 5)

	SurfaceProcessor proc;
	system.apply(proc);
	Surface surface = proc.getSurface();
	TEST_EQUAL(surface.getNumberOfTriangles(), 422)
	TEST_EQUAL(surface.getNumberOfVertices(), 213)
RESULT

/////////////////////////////////////////////////////////////
END_TEST

