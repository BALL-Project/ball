// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: SurfaceProcessor_test.C,v 1.5 2003/05/08 12:05:14 oliver Exp $
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////

// insert includes here
#include <BALL/STRUCTURE/surfaceProcessor.h>
#include <BALL/KERNEL/system.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/DATATYPE/list.h>

///////////////////////////

START_TEST(SurfaceProcessor, "$Id: SurfaceProcessor_test.C,v 1.5 2003/05/08 12:05:14 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

///  insert tests for each member function here         
///

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
	HINFile infile("data/SurfaceProcessor_test_single_atom.hin");
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
	HINFile infile("data/methane.hin");
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

CHECK(SurfaceProcessor / List)
	SurfaceProcessor sp;
	List<Atom*> composites;
	Atom a, b;
	b.setPosition(Vector3(2, 2, 2));
	composites.apply(sp);
	TEST_EQUAL(sp.getSurface().getNumberOfTriangles(), 0)
	TEST_EQUAL(sp.getSurface().getNumberOfVertices(), 0)
	composites.push_back(&a);
	composites.push_back(&b);
	composites.apply(sp);
	TEST_EQUAL(sp.getSurface().getNumberOfTriangles(), 256)
	TEST_EQUAL(sp.getSurface().getNumberOfVertices(), 130)
RESULT

/////////////////////////////////////////////////////////////
END_TEST

