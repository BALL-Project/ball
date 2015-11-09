// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/VIEW/MODELS/cartoonModel.h>
#include <BALL/VIEW/PRIMITIVES/mesh.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/STRUCTURE/secondaryStructureProcessor.h>

///////////////////////////

START_TEST(class_name)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace BALL::VIEW;

CHECK(AddCartoonModel::AddCartoonModel() throw())
	AddCartoonModel();
RESULT


CHECK(AddCartoonModel::AddCartoonModel(const AddCartoonModel& cartoon_model) throw())
	AddCartoonModel acm;
	AddCartoonModel acm2(acm);
RESULT


CHECK(AddCartoonModel::~AddCartoonModel() throw())
	AddCartoonModel();
RESULT

AddCartoonModel cm;

CHECK(AddCartoonModel::setHelixRadius(float radius) throw())
	cm.setHelixRadius(2.11);
RESULT

CHECK(AddCartoonModel::getHelixRadius())
	TEST_REAL_EQUAL(cm.getHelixRadius(), 2.11)
RESULT


CHECK(AddCartoonModel::setArrowWidth(float width) throw())
	cm.setArrowWidth(3.12);
RESULT

CHECK(AddCartoonModel::getArrowWidth())
	TEST_REAL_EQUAL(cm.getArrowWidth(), 3.12)
RESULT

CHECK(AddCartoonModel::setStrandHeight(float heigth) throw())
	cm.setStrandHeight(4.13);
RESULT

CHECK(AddCartoonModel::getStrandHeight())
	TEST_REAL_EQUAL(cm.getStrandHeight(), 4.13)
RESULT

CHECK(AddCartoonModel::setStrandWidth(float w) throw())
	cm.setStrandWidth(5.14);
RESULT

CHECK(AddCartoonModel::getStrandWidth())
	TEST_REAL_EQUAL(cm.getStrandWidth(), 5.14)
RESULT

CHECK(AddCartoonModel::setDNABaseRadius(float r) throw())
	cm.setDNABaseRadius(6.15);
RESULT

CHECK(AddCartoonModel::getDNABaseRadius())
	TEST_REAL_EQUAL(cm.getDNABaseRadius(), 6.15)
RESULT

CHECK(AddCartoonModel::setDNAHelixRadius(float r) throw())
	cm.setDNAHelixRadius(7.16);
RESULT

CHECK(AddCartoonModel::getDNAHelixRadius())
	TEST_REAL_EQUAL(cm.getDNAHelixRadius(), 7.16)
RESULT

CHECK(AddCartoonModel::setDNALadderRadius(float r) throw())
	cm.setDNALadderRadius(8.17);
RESULT

CHECK(AddCartoonModel::getDNALadderRadius())
	TEST_REAL_EQUAL(cm.getDNALadderRadius(), 8.17)
RESULT

CHECK(AddCartoonModel::setDrawDNAAsLadderModel(bool state) throw())
	cm.setDrawDNAAsLadderModel(true);
RESULT

CHECK(AddCartoonModel::drawDNAAsLadderModel())
	TEST_EQUAL(cm.drawDNAAsLadderModel(), true)
	cm.setDrawDNAAsLadderModel(false);
	TEST_EQUAL(cm.drawDNAAsLadderModel(), false)
RESULT

CHECK(AddCartoonModel::enableRibbons(bool state) throw())
	cm.enableRibbons(true);
RESULT

CHECK(AddCartoonModel::ribbonsEnabled())
	TEST_EQUAL(cm.ribbonsEnabled(), true)
	cm.enableRibbons(false);
	TEST_EQUAL(cm.ribbonsEnabled(), false)
RESULT

HINFile hin(BALL_TEST_DATA_PATH(NMRSpectrum_test.hin));
System system;
hin >> system;

PDBFile pdb(BALL_TEST_DATA_PATH(1BNA.pdb));
System system2;
pdb >> system2;


CHECK(AddCartoonModel::createGeometricObjects() throw())
	AddCartoonModel cm;
	system.apply(cm);
	cm.createGeometricObjects();
	TEST_EQUAL(cm.getGeometricObjects().size(), 130)
	GeometricObject* go = *cm.getGeometricObjects().begin();
    TEST_EQUAL(RTTI::isKindOf<Mesh>(go), true)
	Mesh* mesh = dynamic_cast<Mesh*>(go);
	TEST_EQUAL(mesh->vertex.size(), 481)
	TEST_EQUAL(mesh->normal.size(), 481)
	TEST_EQUAL(mesh->triangle.size(), 888)
	PRECISION(0.0001)
	TEST_REAL_EQUAL(mesh->vertex[0].x, 25.0073)
	TEST_REAL_EQUAL(mesh->vertex[0].y, 7.203)
	TEST_REAL_EQUAL(mesh->vertex[0].z, 3.19359)
	TEST_REAL_EQUAL(mesh->normal[0].getLength(), 1)

	cm.clear();
	system2.apply(cm);
	cm.createGeometricObjects();
	// System has no bonds!
	TEST_EQUAL(cm.getGeometricObjects().size(), 0)

	HINFile hin2(BALL_TEST_DATA_PATH(1BNA.hin));
	hin2 >> system2;

	cm.clear();
	system2.apply(cm);
	cm.createGeometricObjects();
	// System has invalid atom names!
	TEST_EQUAL(cm.getGeometricObjects().size(), 0)
RESULT

CHECK(DNA model)
	System system;
	HINFile hin3(BALL_TEST_DATA_PATH(1BNA_2.hin));
	hin3 >> system;

	cm.clear();
	system.apply(cm);
	cm.createGeometricObjects();
	TEST_EQUAL(cm.getGeometricObjects().size(), 345)
	GeometricObject* go = *cm.getGeometricObjects().begin();
    TEST_EQUAL(RTTI::isKindOf<Mesh>(go), true)
	Mesh* mesh = dynamic_cast<Mesh*>(go);
	TEST_EQUAL(mesh->vertex.size(), 241)
	TEST_EQUAL(mesh->normal.size(), 241)
	TEST_EQUAL(mesh->triangle.size(), 408)
	TEST_REAL_EQUAL(mesh->vertex[0].x, 20.123)
	TEST_REAL_EQUAL(mesh->vertex[0].y, 33.4408)
	TEST_REAL_EQUAL(mesh->vertex[0].z, 22.593)
	TEST_REAL_EQUAL(mesh->normal[0].getLength(), 1)

	cm.setDrawDNAAsLadderModel(true);
	cm.clear();
	system.apply(cm);
	cm.createGeometricObjects();
	TEST_EQUAL(cm.getGeometricObjects().size(), 92)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
