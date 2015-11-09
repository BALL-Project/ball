// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
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

#include <QtWidgets/QApplication>

// include our new processor
#include "glBoundingBoxModel.h"

using namespace BALL;
using namespace BALL::VIEW;
using namespace std;

GLBoundingBoxModel::GLBoundingBoxModel() 
	: color_(),
		new_start_(true),
		start_composite_(0),
		bbox_()
{
}

GLBoundingBoxModel::~GLBoundingBoxModel()
{
}

void GLBoundingBoxModel::setColor(const ColorRGBA &color)
{ 
	color_ = color; 
}

bool GLBoundingBoxModel::start()
{
	new_start_ = true;
	start_composite_ = 0;

	return ModelProcessor::start();
}


bool GLBoundingBoxModel::finish()
{
	Composite *root = &(start_composite_->getRoot());

	// if the bounding box is degenerated or if the root of the composite is no
	// System, do not create a bounding box
        if (bbox_.a == bbox_.b || !RTTI::isKindOf<System>(root))
	{
		return false;
	}

	// use a MolecularInformation visitor to gain information about the
	// molecular structure wa are processing at the moment
	MolecularInformation molecular_information;
	start_composite_->host(molecular_information);

	// create a SimpleBox with the boundaries of the calculated box and set
	// its properties with help from MolecularInformation
	SimpleBox* pbox = new SimpleBox();
	pbox->a = bbox_.a ;
	pbox->b = bbox_.b;
	pbox->setColor(color_);

	geometric_objects_.push_back(pbox);

	// TODO
	// move that to the main program
	Log.info() << "Bounding Box: " << bbox_.a << " " << bbox_.b << endl;

	return true;
}


Processor::Result GLBoundingBoxModel::operator() (Composite &composite)
{
	// store the composite we started at because we will need it when finishing
	if (start_composite_ == 0)
	{
		start_composite_ = &composite;
	}

	// if we do not encounter an atom, skip it
        if (!RTTI::isKindOf<Atom>(&composite))
	{
		return Processor::CONTINUE;
	}

	// cast the current item to Atom in order to use Atom related methods
	Atom *atom = RTTI::castTo<Atom>(composite);

	// Create a three dimensional Box defined by the atom position
	SimpleBox3 bbox(atom->getPosition(), atom->getPosition());

	// if we just started, use the just generated box as starting point, else
	// join it with the previously calculated one to create a box containing all
	// atoms we encountered until now
	if (new_start_)
	{
		bbox_ = bbox;
		new_start_ = false;
	}
	else
	{
		bbox_.join(bbox);
	}

	return Processor::CONTINUE;
}




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
	list<GeometricObject*>::Iterator it = bb_processor.getGeometricObjects().begin();
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

