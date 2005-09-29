// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id:

#include "ballviewDemo.h"
#include "mainframe.h"

#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/VIEW/PRIMITIVES/mesh.h>

#include <BALL/VIEW/DIALOGS/displayProperties.h>
#include <BALL/VIEW/DIALOGS/FDPBDialog.h>
#include <BALL/VIEW/DIALOGS/modifySurfaceDialog.h>
#include <BALL/VIEW/DIALOGS/molecularFileDialog.h>

#include <BALL/VIEW/WIDGETS/molecularStructure.h>
#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/WIDGETS/logView.h>
#include <BALL/VIEW/WIDGETS/pyWidget.h>
#include <BALL/VIEW/WIDGETS/datasetControl.h>

#include <BALL/DATATYPE/contourSurface.h>
#include <BALL/SYSTEM/path.h>

#include <qpushbutton.h>
#include <qwidgetstack.h>
#include <qmessagebox.h>

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
	buttonOk->setEnabled(true);
	DisplayProperties* dp = DisplayProperties::getInstance(0);
	dp->enableCreationForNewMolecules(false);

	((Mainframe*)getMainControl())->reset();

	// open bpti 
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
		if (dialog == 0) return;

		system_ = dialog->openFile(file_name);

		if (system_ == 0)
		{
			String msg("Could not open bpti.pdb. Maybe the file was deleted?\n");
			msg += "It should be found in " + Path().getDataPath() + "bpti.pdb";

			QMessageBox::critical(0, "Error while starting BALLView Demo", msg.c_str(),
					QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
			return;
		}
		
		system_->apply(getFragmentDB().add_hydrogens);
		system_->apply(getFragmentDB().build_bonds);
		getMainControl()->update(*system_, true);
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
	move(20,20);

	// hide some dockwidgets
	if (LogView::getInstance(0) != 0) 			 LogView::getInstance(0)->hide();
	if (DatasetControl::getInstance(0) != 0) DatasetControl::getInstance(0)->hide();
#ifdef BALL_PYTHON_SUPPORT
	if (PyWidget::getInstance(0) != 0) 			 PyWidget::getInstance(0)->hide();
#endif
}


void BALLViewDemo::onNotify(Message *message)
	throw()
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "BALLViewDemo " << this << " onNotify " << message << std::endl;
#endif

	if (!isVisible()) return;

	RepresentationMessage* rmsg = RTTI::castTo<RepresentationMessage>(*message);

	Index id = widget_stack->id(widget_stack->visibleWidget());

	if (id == 12)
	{
		if (RTTI::isKindOf<FinishedSimulationMessage>(*message))
		{
			nextStep_();
		}
	}
	else if (id == 13)
	{
		RegularData3DMessage* msg = RTTI::castTo<RegularData3DMessage>(*message);
		if (msg != 0 &&
			  ((RegularData3DMessage::RegularDataMessageType)msg->getType()) == RegularDataMessage::NEW)
		{
			grid_ = msg->getData();
			nextStep_();
		}
	}
	else if (id == 14)
	{
		SceneMessage* msg = RTTI::castTo<SceneMessage>(*message);
		if (msg != 0 && msg->getType() == SceneMessage::REBUILD_DISPLAY_LISTS)
		{
 			nextStep_();
		}
	}
	else if (rmsg != 0 && 
					 rmsg->getType() == RepresentationMessage::UPDATE)
	{
		nextStep_();
	}
}


void BALLViewDemo::nextStep_()
{
	widget_stack->raiseWidget(widget_stack->id(widget_stack->visibleWidget()) + 1);
	buttonOk->setEnabled(true);
}


void BALLViewDemo::accept()
{
	Index id = widget_stack->id(widget_stack->visibleWidget());

	if (id == 16) // last page
	{
		hide();
		return;
	}

	MolecularStructure* ms = MolecularStructure::getInstance(0);
	bool disable_button = true;

	// remove representations
	PrimitiveManager& pm = getMainControl()->getPrimitiveManager();
	Size nr = pm.getNumberOfRepresentations();
	list<Representation*> reps = pm.getRepresentations();
	for (Position p = 0; p < nr; p++)
	{
		getMainControl()->remove(**reps.begin());
		reps.pop_front();
	}

	if (id < 7)
	{
		ModelType type = (ModelType) id;
		if (type >= MODEL_SA_SURFACE)
		{
			type = (ModelType)((Index)type + 1);
		}
		notify_(new CreateRepresentationMessage(composites_, type, COLORING_ELEMENT));
	}
	else if (id == 7)
	{
		getMainControl()->getMolecularControlSelection().clear();
		getMainControl()->getMolecularControlSelection().push_back(system_);
		ms->calculateHBonds();
   	notify_(new CreateRepresentationMessage(composites_, MODEL_STICK, COLORING_ELEMENT));
 		notify_(new CreateRepresentationMessage(composites_, MODEL_HBONDS, COLORING_ELEMENT));

	}
	else if (id == 8)
	{
		notify_(new CreateRepresentationMessage(composites_, MODEL_VDW, COLORING_TEMPERATURE_FACTOR));
	}
	else if (id == 9)
	{
		notify_(new CreateRepresentationMessage(composites_, MODEL_CARTOON, COLORING_SECONDARY_STRUCTURE));
	}
	else if (id == 10)
	{
		notify_(new CreateRepresentationMessage(composites_, MODEL_CARTOON, COLORING_RESIDUE_INDEX));
	}
	else if (id == 11)
	{
		notify_(new CreateRepresentationMessage(composites_, MODEL_STICK, COLORING_RESIDUE_NAME));
	}
	else if (id == 12)
	{
		notify_(new CreateRepresentationMessage(composites_, MODEL_STICK, COLORING_ELEMENT));

		ms->chooseAmberFF();
		ms->getMDSimulationDialog().setTimeStep(0.001);
 		ms->getMDSimulationDialog().setNumberOfSteps(30);
		ms->MDSimulation(false);
	}
	else if (id == 13) //FDPB
	{
		ms->calculateFDPB();
		ms->getFDPBDialog()->okPressed();
		disable_button = false;
	}
	else if (id == 14) // SES colored 
	{
		getMainControl()->getPrimitiveManager().setMultithreadingMode(false);
		notify_(new CreateRepresentationMessage(composites_, MODEL_SE_SURFACE, COLORING_ELEMENT));

		getMainControl()->getPrimitiveManager().setMultithreadingMode(false);

		Representation* rep = *getMainControl()->getPrimitiveManager().begin();

		ModifySurfaceDialog* cdialog = ModifySurfaceDialog::getInstance(0);
		cdialog->setRepresentation(rep);
		cdialog->setGrid(grid_);
		cdialog->setMinValue(-0.7);
		cdialog->setMaxValue(3.0);
		cdialog->applyPressed();

		rep->setColorProcessor(0);

 		SceneMessage smsg(SceneMessage::REBUILD_DISPLAY_LISTS);
 		getMainControl()->sendMessage(smsg);
		disable_button = false;
	}
	else if (id == 15)
	{
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
			if ((mesh->vertex[i] + mesh->normal[i]).getDistance(center) < 
					(mesh->vertex[i] - mesh->normal[i]).getDistance(center))
			{
				nr_of_strange_normals ++;
			}
		}

		if (nr_of_strange_normals < mesh->normal.size() / 2.0)
		{
			for (Position i = 0; i < mesh->normal.size(); i++)
			{
				mesh->normal[i] *= -1;
			}
		}

		// Create a new representation containing the contour surface.
		Representation* rep = getMainControl()->getPrimitiveManager().createRepresentation();
		rep->insert(*mesh);
		rep->setModelType(MODEL_CONTOUR_SURFACE); 

		notify_(new RepresentationMessage(*rep, RepresentationMessage::ADD));

   	notify_(new CreateRepresentationMessage(composites_, MODEL_STICK, COLORING_ELEMENT));
	}

	buttonOk->setEnabled(!disable_button);
}


} } // namespaces
