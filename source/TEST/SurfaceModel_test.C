// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: SurfaceModel_test.C,v 1.1.2.3 2007/05/10 20:16:16 amoll Exp $
//
// Author:
//   Andreas Moll
//

#include <BALL/CONCEPT/classTest.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/VIEW/PRIMITIVES/mesh.h>

///////////////////////////
#include <BALL/VIEW/MODELS/surfaceModel.h>
///////////////////////////

using namespace BALL;
using namespace BALL::VIEW;

START_TEST(AddSurfaceModel, "$Id: SurfaceModel_test.C,v 1.1.2.3 2007/05/10 20:16:16 amoll Exp $")

CHECK(CSTR)
	AddSurfaceModel();
RESULT

CHECK(AddSurfaceModel::BALL_CREATE(AddSurfaceModel))
  //BAUSTELLE
RESULT


CHECK(AddSurfaceModel::setProbeRadius(float radius))
	AddSurfaceModel bs;
	bs.setProbeRadius(0.12);
RESULT


CHECK(AddSurfaceModel::getProbeRadius() const  throw())
	AddSurfaceModel bs;
	bs.setProbeRadius(0.12);
	TEST_REAL_EQUAL(bs.getProbeRadius(), 0.12)
RESULT


PDBFile pdb("data/1BNA.pdb");
System system;
pdb >> system;

CHECK(AddSurfaceModel::Processor::Result operator() (Composite& composite))
	AddSurfaceModel bs;
	bool result = bs.operator() (system);
	TEST_EQUAL(result, true)
RESULT

CHECK(AddSurfaceModel::createGeometricObjects() throw())
	AddSurfaceModel bs;
	bs.setProbeRadius(1);
	system.apply(bs);
	bs.createGeometricObjects();
	TEST_EQUAL(bs.getGeometricObjects().size(), 1)
	Mesh* m = dynamic_cast<Mesh*>(*bs.getGeometricObjects().begin());
	TEST_NOT_EQUAL(m, 0)
	PRECISION(0.0001)
	TEST_EQUAL(m->vertex.size(), 33673)
	TEST_EQUAL(m->triangle.size(), 67415)
	TEST_EQUAL(m->normal.size(), 33673)
	TEST_REAL_EQUAL(m->vertex[0].x, 3.42099)
	TEST_REAL_EQUAL(m->vertex[0].y, 23.7674)
	TEST_REAL_EQUAL(m->vertex[0].z, 9.45008)
	TEST_REAL_EQUAL(m->normal[0].x, 0.57071)
	TEST_REAL_EQUAL(m->normal[0].y, 0.526747)
	TEST_REAL_EQUAL(m->normal[0].z, -0.629943)
	TEST_EQUAL(m->triangle[0].v1, 1)
	TEST_EQUAL(m->triangle[0].v2, 3247)
	TEST_EQUAL(m->triangle[0].v3, 3251)
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
