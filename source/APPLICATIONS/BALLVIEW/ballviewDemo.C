// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id:

#include "ballviewDemo.h"
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/KERNEL/system.h>
#include <BALL/VIEW/DIALOGS/displayProperties.h>
#include <BALL/VIEW/WIDGETS/molecularStructure.h>
#include <BALL/VIEW/DIALOGS/FDPBDialog.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/DATATYPE/contourSurface.h>
#include <BALL/VIEW/PRIMITIVES/mesh.h>
#include <BALL/STRUCTURE/HBondProcessor.h>
#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/DATATYPE/colorTable.h>
#include <BALL/VIEW/DIALOGS/colorMeshDialog.h>
#include <BALL/VIEW/DIALOGS/molecularFileDialog.h>

#include <qlabel.h>
#include <qpushbutton.h>
#include <qwidgetstack.h>

namespace BALL
{
	namespace VIEW
	{

BALLViewDemo::BALLViewDemo(QWidget* parent, const char* name)
	throw()
	:	BALLViewDemoData(parent, name),
		ModularWidget(name)
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "new BALLViewDemo " << this << std::endl;
#endif
	// register the widget with the MainControl
 	ModularWidget::registerWidget(this);
	hide();
}

BALLViewDemo::~BALLViewDemo()
	throw()
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "deleting BALLViewDemo " << this << std::endl;
#endif
}

void BALLViewDemo::show()
{
	widget_stack->raiseWidget(0);
	DisplayProperties* dp = DisplayProperties::getInstance(0);
	dp->setDrawingPrecision(DRAWING_PRECISION_HIGH);
	dp->enableCreationForNewMolecules(false);
	system_ = new System();

	try
	{
		Path path;
		String file_name(path.getDataPath());
		file_name = file_name.before("data");
		file_name += "data";
		file_name += FileSystem::PATH_SEPARATOR;
		file_name += String("structures");
		file_name += FileSystem::PATH_SEPARATOR;
		file_name += "bpti.pdb";
		MolecularFileDialog* dialog = MolecularFileDialog::getInstance(0);
		system_ = dialog->openFile(file_name);
	}
	catch(Exception::FileNotFound e)
	{
		Log.error() << "Could not open " << e.getFilename() << std::endl;
		return;
	}

	composites_.clear();
	composites_.push_back(system_);
	QDialog::show();
	raise();
}

void BALLViewDemo::onNotify(Message *message)
	throw()
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "BALLViewDemo " << this << " onNotify " << message << std::endl;
#endif

	if (!isVisible()) return;

//   	CompositeMessage* cmsg = RTTI::castTo<CompositeMessage>(*message);
	RepresentationMessage* rmsg = RTTI::castTo<RepresentationMessage>(*message);

	Index id = widget_stack->id(widget_stack->visibleWidget());

	if (id == MODEL_HBONDS + 5)
	{
		if (RTTI::isKindOf<FinishedSimulationMessage>(*message))
		{
			nextStep_();
		}
		return;
	}


	if (id == MODEL_HBONDS + 6)
	{
		RegularData3DMessage* msg = RTTI::castTo<RegularData3DMessage>(*message);
		if (msg != 0 &&
			  ((RegularData3DMessage::RegularDataMessageType)msg->getType()) == RegularDataMessage::NEW)
		{
			grid_ = msg->getData();
			nextStep_();
		}
		return;
	}

	if (//id <= MODEL_HBONDS + 6 &&
		  rmsg != 0 && 
			rmsg->getType() == RepresentationMessage::UPDATE)
	{
		nextStep_();
	}

	
}

void BALLViewDemo::nextStep_()
{
	buttonOk->setEnabled(true);
	widget_stack->raiseWidget(widget_stack->id(widget_stack->visibleWidget()) + 1);
}

void BALLViewDemo::accept()
{
	Index id = widget_stack->id(widget_stack->visibleWidget());
	MolecularStructure* ms = MolecularStructure::getInstance(0);
	bool disable_button = true;

	if (id < MODEL_HBONDS + 7)
	{
		// remove representations
		PrimitiveManager& pm = getMainControl()->getPrimitiveManager();
		Size nr = pm.getNumberOfRepresentations();
		list<Representation*> reps = pm.getRepresentations();
		for (Position p = 0; p < nr; p++)
		{
			getMainControl()->remove(**reps.begin());
			reps.pop_front();
		}
	}

	if (id < MODEL_HBONDS - 1)
	{
		if (((ModelType)id) >= MODEL_SE_SURFACE) id ++;
 		CreateRepresentationMessage* crmsg = new CreateRepresentationMessage(composites_, (ModelType) id, COLORING_ELEMENT);
//   CreateRepresentationMessage* crmsg = new CreateRepresentationMessage(composites_, MODEL_STICK, COLORING_ELEMENT);
		notify_(crmsg);
	}
	else if (id == MODEL_HBONDS - 1)
	{
		CreateRepresentationMessage* crmsg = new CreateRepresentationMessage(composites_, MODEL_STICK, COLORING_ELEMENT);
		notify_(crmsg);
		crmsg = new CreateRepresentationMessage(composites_, MODEL_HBONDS, COLORING_ELEMENT);
		notify_(crmsg);
	}
	else if (id == MODEL_HBONDS)
	{
		CreateRepresentationMessage* crmsg = new CreateRepresentationMessage(composites_, MODEL_VDW, COLORING_TEMPERATURE_FACTOR);
		notify_(crmsg);
	}
	else if (id == MODEL_HBONDS + 1)
	{
		CreateRepresentationMessage* crmsg = new CreateRepresentationMessage(composites_, MODEL_CARTOON, COLORING_SECONDARY_STRUCTURE);
		notify_(crmsg);
	}
	else if (id == MODEL_HBONDS + 2)
	{
		CreateRepresentationMessage* crmsg = new CreateRepresentationMessage(composites_, MODEL_CARTOON, COLORING_RESIDUE_INDEX);
		notify_(crmsg);
	}
	else if (id == MODEL_HBONDS + 3)
	{
		CreateRepresentationMessage* crmsg = new CreateRepresentationMessage(composites_, MODEL_STICK, COLORING_RESIDUE_NAME);
		notify_(crmsg);
	}
	else if (id == MODEL_HBONDS + 4)
	{
		CreateRepresentationMessage* crmsg = new CreateRepresentationMessage(composites_, MODEL_STICK, COLORING_ELEMENT);
		notify_(crmsg);

		ms->chooseAmberFF();
		ms->getMDSimulationDialog().setTimeStep(0.001);
 		ms->getMDSimulationDialog().setNumberOfSteps(30);
		ms->MDSimulation(false);
	}
	else if (id == MODEL_HBONDS + 5) //FDPB
	{
		ms->calculateFDPB();
		ms->getFDPBDialog()->okPressed();
		disable_button = false;
	}
	else if (id == MODEL_HBONDS + 6) // SES colored 
	{
		getMainControl()->getPrimitiveManager().setMultithreadingMode(false);
		CreateRepresentationMessage* crmsg = new CreateRepresentationMessage(composites_, MODEL_SE_SURFACE, COLORING_ELEMENT);
		notify_(crmsg);

		Representation* rep = *getMainControl()->getPrimitiveManager().begin();
		Mesh* mesh = dynamic_cast<Mesh*> (*rep->getGeometricObjects().begin());

		ColorMeshDialog* cdialog = ColorMeshDialog::getInstance(0);
		cdialog->setMesh(mesh, rep);
		cdialog->setGrid(grid_);
		cdialog->setMinValue(-0.7);
		cdialog->setMaxValue(3.0);
		cdialog->applyPressed();

		rep->setColorProcessor(0);
		getMainControl()->getPrimitiveManager().setMultithreadingMode(false);

 		SceneMessage* smsg = new SceneMessage(SceneMessage::REBUILD_DISPLAY_LISTS);
 		notify_(smsg);

		/*
		ContourSurface cs(*grid_, 0.01);
		Mesh* mesh = new Mesh;
		mesh->Surface::operator = (static_cast<Surface&>(cs));

		Vector3 center;
		for (Position i = 0; i < mesh->vertex.size(); i++)
		{
			center += mesh->vertex[i];
		}

		center /= mesh->vertex.size();

		Size nr_of_strange_normals = 0;
		for (Position i = 0; i < mesh->normal.size(); i++)
		{
			if ((mesh->vertex[i] + mesh->normal[i]).getDistance(center) < (mesh->vertex[i] - mesh->normal[i]).getDistance(center))
			{
				nr_of_strange_normals ++;
			}
		}

		if (nr_of_strange_normals > mesh->normal.size() / 2.0)
		{
			for (Position i = 0; i < mesh->normal.size(); i++)
			{
				mesh->normal[i] *= -1;
			}
		}

		Stage stage = *Scene::getInstance(0)->getStage();
		stage.clearLightSources();
		LightSource ls;
		ls.setPosition(Vector3(0,0,4000));
		ls.setDirection(Vector3(0,0,0));
		stage.addLightSource(ls);
		SceneMessage* smsg = new SceneMessage(SceneMessage::UPDATE_CAMERA);
		smsg->setStage(stage);
		notify_(smsg);
		// Create a new representation containing the contour surface.
		Representation* rep = getMainControl()->getPrimitiveManager().createRepresentation();
		rep->insert(*mesh);
		rep->setModelType(MODEL_CONTOUR_SURFACE); 

		// Make sure BALLView knows about the new representation.
		RepresentationMessage* message = new RepresentationMessage(*rep, RepresentationMessage::ADD);
		notify_(message);

*/
 		disable_button = false;
		
	}
	else if (id == MODEL_HBONDS + 7) // last
	{
		CreateRepresentationMessage* crmsg = new CreateRepresentationMessage(composites_, MODEL_STICK, COLORING_ELEMENT);
		notify_(crmsg);
	}

	buttonOk->setEnabled(!disable_button);
}


} } // namespaces
