// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: bounding_box.C,v 1.7.2.1 2004/05/15 10:57:13 amoll Exp $
// BALLView tutorial example
// ------------------------
// visualise the bounding box of a molecular structure

// BALL includes
#include <BALL/common.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/KERNEL/system.h>
#include <BALL/VIEW/KERNEL/representation.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/WIDGETS/geometricControl.h>
#include <BALL/VIEW/WIDGETS/molecularControl.h>
#include "../APPLICATIONS/BALLVIEW/mainframe.h"

#include <qapplication.h>

// include our new processor
#include "glBoundingBoxModel.h"

#include <qapplication.h>

using namespace BALL;
using namespace BALL::VIEW;
using namespace std;

class MyMainframe
	: public MainControl
{
	public:
		MyMainframe(QWidget* parent = 0, const char* name = 0)
			:	MainControl(parent, name)
		{
			Scene* scene_ = new Scene(this, "3D View");
			scene_->setMinimumSize(10, 10);
			setCentralWidget(scene_);

			GeometricControl* geometric_control_ = new GeometricControl(this, "Representations");
			CHECK_PTR(geometric_control_);

			MolecularControl* molecular_control_ = new MolecularControl(this, "Molecules");
			CHECK_PTR(molecular_control_);
		}	
};

int main(int argc, char **argv)
{
  QApplication application(argc, argv);
  MyMainframe mainframe;
  application.setMainWidget(&mainframe);
  // start the application
  mainframe.show();

  // read a molecule from a file
  HINFile infile("bounding_box_example.hin") ;
  System* system = new System();
  infile >> *system;
  infile.close();
  mainframe.insert(*system);

  // apply the bounding box processor
  GLBoundingBoxModel bb_processor;
	bb_processor.setColor(ColorRGBA(255,255,255));
  system->apply(bb_processor);

  Representation* rep = new Representation();
	List<GeometricObject*>::Iterator it = bb_processor.getGeometricObjects().begin();
	for (;it != bb_processor.getGeometricObjects().end(); ++it)
	{
		rep->insert(**it);
	}
  mainframe.insert(*rep);

	SceneMessage* msg = new SceneMessage(SceneMessage::UPDATE_CAMERA);
	Camera camera;
	camera.setViewPoint(Vector3(0,0,0));	
	camera.setLookAtPosition(Vector3(-3,2,-2));
	msg->setCamera(camera);
	mainframe.sendMessage(*msg);

	return application.exec();
}

