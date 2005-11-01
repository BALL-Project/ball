// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: bounding_box.C,v 1.11.4.1 2005/11/01 23:42:00 amoll Exp $
//
// BALLView tutorial example
// ------------------------
// visualise the bounding box of a molecular structure

// BALL includes
#include <BALL/common.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/KERNEL/system.h>
#include <BALL/VIEW/KERNEL/representation.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/WIDGETS/geometricControl.h>
#include <BALL/VIEW/WIDGETS/molecularControl.h>

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

 		BALL_EMBEDDABLE(MyMainframe, MainControl)

		MyMainframe(QWidget* parent = 0, const char* name = 0)
			:	MainControl(parent, name)
		{
			Scene* scene_ = new Scene(this, "3D View");
			scene_->setMinimumSize(10, 10);
			setCentralWidget(scene_);

			new GeometricControl(this, "Representations");
			new MolecularControl(this, "Molecules");
		}	

};

int main(int argc, char **argv)
{
  QApplication application(argc, argv);

  MyMainframe mainframe;
  application.setMainWidget(&mainframe);
	mainframe.setIdentifier("MAIN");
	mainframe.registerThis();
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
  mainframe.update(*rep);

	Stage stage;
	stage.getCamera().setViewPoint(Vector3(0,0,0));	
	stage.getCamera().setLookAtPosition(Vector3(-3,2,-2));

	SceneMessage* msg = new SceneMessage(SceneMessage::UPDATE_CAMERA);
	msg->setStage(stage);
	mainframe.sendMessage(*msg);

	return application.exec();
}

