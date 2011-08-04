// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////
#include <BALL/VIEW/KERNEL/representationManager.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/clippingPlane.h>
#include <BALL/VIEW/DIALOGS/displayProperties.h>
#include <BALL/KERNEL/system.h>
///////////////////////////

/////////////////////////////////////////////////////////////
START_TEST(RepresentationManager)
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace BALL::VIEW;
using namespace std;

char* argv = "asd";
int args = 0;

// need a valid X-Context for the following code:
//   QApplication app(args, &argv);
//   MainControl mc;
//   DisplayProperties* dp = new DisplayProperties(&mc);
//   dp->initializeWidget(mc);
//   dp->initializePreferencesTab(*mc.getPreferences());

CHECK(CSTR)
	RepresentationManager rm;
RESULT

CHECK(RepresentationManager::BALL_CREATE(RepresentationManager))
	// no copying support
RESULT


CHECK(RepresentationManager::RepresentationManager& operator = (const RepresentationManager& pm) throw())
	// no copying support
RESULT

/*
CHECK(RepresentationManager::bool operator == (const RepresentationManager& pm) const  throw())
	RepresentationManager r1(&mc), r2(&mc);
	TEST_EQUAL(r1 == r2, true)
	Representation* rep = new Representation();
	r1.insert(*rep);
	TEST_EQUAL(r1 == r2, false)
RESULT

RepresentationManager rm(&mc);
*/
RepresentationManager rm;

CHECK(RepresentationManager::clear() throw())
	rm.insert(*new Representation());
	TEST_EQUAL(rm.getNumberOfRepresentations(), 1)
	rm.clear();
	TEST_EQUAL(rm.getNumberOfRepresentations(), 0)
RESULT


CHECK(RepresentationManager::remove(Representation& representation, bool send_message = true) throw())
	Representation* rep = new Representation();
	rm.insert(*rep);
	TEST_EQUAL(rm.getNumberOfRepresentations(), 1)
	rm.remove(*rep);
	TEST_EQUAL(rm.getNumberOfRepresentations(), 0)
RESULT


CHECK(RepresentationManager::insert(Representation& representation, bool send_message = true) throw())
	Representation* rep = new Representation();
	rm.insert(*rep);
	TEST_EQUAL(rm.getNumberOfRepresentations(), 1)
	rm.clear();
RESULT


CHECK(RepresentationManager::getRepresentations() const  throw())
	Representation* rep = new Representation();
	rm.clear();
	rm.insert(*rep);
	TEST_EQUAL(*rm.getRepresentations().begin(), rep)
	rm.clear();
	TEST_EQUAL(rm.getRepresentations().size(), 0)
RESULT


CHECK(RepresentationManager::createRepresentation() throw())
	Representation* rep = rm.createRepresentation();
	TEST_EQUAL(*rm.getRepresentations().begin(), rep);
RESULT


CHECK(RepresentationManager::has(const Representation& representation) const  throw())
	Representation* rep = new Representation;
	TEST_EQUAL(rm.has(*rep), false)
	rm.clear();
	rm.insert(*rep);
	TEST_EQUAL(rm.has(*rep), true)
RESULT


CHECK(RepresentationManager::dump(std::ostream& s, Size depth) const  throw())
	rm.clear();
	Representation* rep = new Representation;
	rm.insert(*rep);
  String filename;
	NEW_TMP_FILE(filename)
	std::ofstream outfile(filename.c_str(), std::ios::out);
	rm.dump(outfile);
	outfile.close();
	TEST_FILE_REGEXP(filename.c_str(), BALL_TEST_DATA_PATH(RepresentationManager.txt))
RESULT


CHECK(RepresentationManager::begin() throw())
	rm.clear();
	TEST_EQUAL(rm.begin() == rm.getRepresentations().end(), true)
 	Representation* rep = rm.createRepresentation();
	TEST_EQUAL(*rm.begin(), rep)
RESULT


CHECK(RepresentationManager::end() throw())
	rm.clear();
	TEST_EQUAL(rm.begin() == rm.getRepresentations().end(), true)
RESULT

Composite composite;
List<const Composite*> cl;
cl.push_back(&composite);

CHECK(RepresentationManager::removedComposite(const Composite& composite, bool update = true) throw())
	rm.clear();
 	Representation* rep = rm.createRepresentation();
	rep->setComposites(cl);
 	Representation* rep2 = rm.createRepresentation();
	TEST_EQUAL(rm.getNumberOfRepresentations(), 2)
	rm.removedComposite(composite);
	TEST_EQUAL(rm.getNumberOfRepresentations(), 1)
	TEST_EQUAL(*rm.begin(), rep2);
RESULT


CHECK(RepresentationManager::getRepresentationsOf(const Composite& composite) throw())
	rm.clear();
 	Representation* rep = rm.createRepresentation();
	rep->setComposites(cl);
 	Representation* rep2 = rm.createRepresentation();
	TEST_EQUAL(rm.getRepresentationsOf(composite).size(), 1)
	TEST_EQUAL(*rm.getRepresentationsOf(composite).begin(), rep)
	rm.remove(*rep2);
RESULT


CHECK(RepresentationManager::rebuildAllRepresentations() throw())
	rm.clear();
 	Representation* rep = rm.createRepresentation();
	rm.rebuildAllRepresentations();
	TEST_EQUAL(rm.willBeUpdated(*rep), true)
 	Representation* rep2 = rm.createRepresentation();
	TEST_EQUAL(rm.willBeUpdated(*rep2), false)
RESULT


CHECK(RepresentationManager::getClippingPlanes() const )
	TEST_EQUAL(rm.getClippingPlanes().size(), 0)
RESULT


CHECK(removeClippingPlane(ClippingPlane*))
	ClippingPlane pl;
	TEST_EQUAL(rm.removeClippingPlane(&pl), false)
RESULT

CHECK(insertClippingPlane(ClippingPlane*))
	ClippingPlane* plane = new ClippingPlane();
	rm.insertClippingPlane(plane);
	TEST_EQUAL(*rm.getClippingPlanes().begin(), plane)
	rm.removeClippingPlane(plane);
	TEST_EQUAL(rm.getClippingPlanes().size(), 0)
RESULT

/*
Representation rrep;
ClippingPlane  rplane;
RepresentationManager& mrm = mc.getRepresentationManager();
System* system = new System();

CHECK(storeRepresentations(INIFile&))
	mc.insert(*system);
	List<const Composite*> cl;
	cl.push_back(system);

	mrm.clear();
 	Representation* rep = mrm.createRepresentation();
	rep->setComposites(cl);
	rep->setModelType(MODEL_SE_SURFACE);
	rep->setColoringMethod(COLORING_TEMPERATURE_FACTOR);
	rep->setTransparency(100);
	rep->setDrawingMode(DRAWING_MODE_WIREFRAME);
	rep->setHidden(true);
	rrep = *rep;

	rplane.setPoint(Vector3(1,2,3));
	rplane.setNormal(Vector3(4,5,6));
	rplane.setCappingEnabled(true);
	rplane.setActive(true);
	mrm.insertClippingPlane(new ClippingPlane(rplane));

	String filename;
	NEW_TMP_FILE(filename)
	INIFile file(filename);
	mrm.storeRepresentations(file);
	file.write();
	TEST_FILE(filename.c_str(), BALL_TEST_DATA_PATH(RepresentationManager2.txt))
RESULT
			
CHECK(restoreRepresentations(const INIFile& in, const vector<const Composite*>& new_systems))
	mrm.clear();
	INIFile infile(BALL_TEST_DATA_PATH(RepresentationManager2.txt));
	infile.read();
	vector<const Composite*> new_systems;
	new_systems.push_back(system);
	mrm.clear();
	TEST_EQUAL(mrm.getNumberOfRepresentations(), 0);
	mrm.restoreRepresentations(infile, new_systems);
	TEST_EQUAL(mrm.getNumberOfRepresentations(), 1);
	if (mrm.getNumberOfRepresentations() == 1)
	{
		Representation& rep = **mrm.begin();
		TEST_EQUAL(rep.getModelType(), rrep.getModelType())
		TEST_EQUAL(rep.getColoringMethod(), rrep.getColoringMethod())
		TEST_EQUAL(rep.getTransparency(), rrep.getTransparency())
		TEST_EQUAL(rep.getDrawingMode(), rrep.getDrawingMode())
		TEST_EQUAL(rep.isHidden(), rrep.isHidden())
	}

	TEST_EQUAL(mrm.getClippingPlanes().size(), 1)
	if (mrm.getClippingPlanes().size() == 1)
	{
		ClippingPlane* tp = *mrm.getClippingPlanes().begin();
		TEST_EQUAL(tp->getNormal(), rplane.getNormal())
		TEST_EQUAL(tp->getPoint(), rplane.getPoint())
		TEST_EQUAL(tp->cappingEnabled(), rplane.cappingEnabled())
	}
RESULT
*/

CHECK(focusRepresentation(const Representation&))
RESULT

CHECK(willBeUpdated(const Representation&) const)
RESULT

CHECK(updateRunning())
RESULT

CHECK(startedRendering(Representation*))
RESULT

CHECK(finishedRendering(Representation*))
RESULT
			
CHECK(isBeeingRendered(const Representation*) const)
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST

