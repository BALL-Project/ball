// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id:

#include "demoTutorialDialog.h"
#include "mainframe.h"

#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/VIEW/PRIMITIVES/mesh.h>
#include <BALL/VIEW/KERNEL/message.h>

#include <BALL/VIEW/DIALOGS/displayProperties.h>
#include <BALL/VIEW/DIALOGS/FDPBDialog.h>
#include <BALL/VIEW/DIALOGS/modifySurfaceDialog.h>
#include <BALL/VIEW/DIALOGS/molecularFileDialog.h>

#include <BALL/VIEW/WIDGETS/molecularStructure.h>
#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/WIDGETS/logView.h>
#include <BALL/VIEW/WIDGETS/pyWidget.h>
#include <BALL/VIEW/WIDGETS/datasetControl.h>
#include <BALL/VIEW/WIDGETS/molecularControl.h>
#include <BALL/VIEW/WIDGETS/geometricControl.h>
#include <BALL/VIEW/WIDGETS/helpViewer.h>

#include <BALL/DATATYPE/contourSurface.h>
#include <BALL/SYSTEM/path.h>

#include <qpushbutton.h>
#include <qmessagebox.h>
#include <qtextbrowser.h>

namespace BALL
{
	namespace VIEW
	{

DemoTutorialDialog::DemoTutorialDialog(QWidget* parent, const char* name)
	throw()
	:	DemoTutorialDialogData(parent, name),
		ModularWidget(name)
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "new DemoTutorialDialog " << this << std::endl;
#endif
	// register the widget with the MainControl
 	ModularWidget::registerWidget(this);
	hide();
}

DemoTutorialDialog::~DemoTutorialDialog()
	throw()
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "deleting DemoTutorialDialog " << this << std::endl;
#endif
}

void DemoTutorialDialog::initDemo_()
{
	setCaption("BALLView Demo");

	prefix_ = getBaseDir_() + "demo";

	next_button->setEnabled(true);

	DisplayProperties* dp = DisplayProperties::getInstance(0);

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

		dp->enableCreationForNewMolecules(false);
		system_ = dialog->openFile(file_name);
		dp->enableCreationForNewMolecules(true);

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

String DemoTutorialDialog::getBaseDir_()
{
	return String(getDataPath()) + ".." +
					FileSystem::PATH_SEPARATOR + 
					"doc" + 
					FileSystem::PATH_SEPARATOR +
					"internal" +
					FileSystem::PATH_SEPARATOR;
}

void DemoTutorialDialog::initTutorial_()
{
	setCaption("BALLView Tutorial");
	
	prefix_ = getBaseDir_() + "tutorial";

	next_button->setEnabled(false);

	((Mainframe*)getMainControl())->reset();

	BALL_VIEW_DOCKWINDOWS_SHOW_LABELS = true;

	Scene::getInstance(0)->show();
	MolecularControl::getInstance(0)->show();
	MolecularControl::getInstance(0)->dock();
	MolecularControl::getInstance(0)->applyPreferences();
	DatasetControl::getInstance(0)->show();
	DatasetControl::getInstance(0)->applyPreferences();
	DatasetControl::getInstance(0)->dock();
	GeometricControl::getInstance(0)->show();
	GeometricControl::getInstance(0)->applyPreferences();
	GeometricControl::getInstance(0)->dock();

	LogView::getInstance(0)->hide();

}

void DemoTutorialDialog::show()
{
	current_step_ = 1;

	if (demo_mode_)
	{
		initDemo_();
	}
	else
	{
		initTutorial_();
	}

	text_browser->setSource(String(prefix_ + "01.html").c_str());

	resize(270, 500);
	DemoTutorialDialogData::show();
	raise();
}

void DemoTutorialDialog::onNotify(Message* message)
	throw()
{
	if (!isVisible()) return;

	if (demo_mode_)
	{
		onNotifyDemo_(message);
	}
	else
	{
		onNotifyTutorial_(message);
	}
}

void DemoTutorialDialog::onNotifyDemo_(Message *message)
	throw()
{
	RepresentationMessage* rmsg = RTTI::castTo<RepresentationMessage>(*message);

	if (current_step_ == 14)
	{
		if (!RTTI::isKindOf<FinishedSimulationMessage>(*message)) return;
	}
	else if (current_step_ == 15)
	{
		RegularData3DMessage* msg = RTTI::castTo<RegularData3DMessage>(*message);
		if (msg == 0 ||
			  ((RegularData3DMessage::RegularDataMessageType)msg->getType()) != RegularDataMessage::NEW)
		{
			return;
		}

		grid_ = msg->getData();
	}
	else if (current_step_ == 16)
	{
		SceneMessage* msg = RTTI::castTo<SceneMessage>(*message);
		if (msg == 0 || msg->getType() != SceneMessage::REBUILD_DISPLAY_LISTS)
		{
			return;
		}
	}
	else if (rmsg == 0 ||
					 rmsg->getType() != RepresentationMessage::UPDATE)
	{
		return;
	}

	enableNextStep_();
}


void DemoTutorialDialog::enableNextStep_()
{
	next_button->setEnabled(true);
}


void DemoTutorialDialog::nextStepClicked()
{
	String id = String(current_step_ + 1);
	if (id.size() == 1) id = "0" + id;

	id = prefix_ + id + ".html";

	text_browser->setSource(id.c_str());
	next_button->setEnabled(false);

	current_step_ ++;

	if (demo_mode_) 
	{
		if (current_step_ == 18)
		{
			showTutorial();
			return;
		}

		nextStepDemo_();
	}
	else
	{
		if (current_step_ == 9)
		{
			next_button->setEnabled(true);
		}
		if (current_step_ == 10)
		{
			hide();
			HelpViewer* hv = HelpViewer::getInstance(0);
			if (hv == 0) return;
			hv->showHelp();
			hv->undock();
			hv->showMaximized();
		}
	}
}


void DemoTutorialDialog::nextStepDemo_()
{
	if (current_step_ == 18) // last page
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

	if (current_step_ < 9)
	{
		ModelType type = (ModelType) (current_step_ - 2);
		if (type >= MODEL_SA_SURFACE)
		{
			type = (ModelType)((Index)type + 1);
		}
		notify_(new CreateRepresentationMessage(composites_, type, COLORING_ELEMENT));
	}
	else if (current_step_ == 9)
	{
		getMainControl()->getMolecularControlSelection().clear();
		getMainControl()->getMolecularControlSelection().push_back(system_);
		ms->calculateHBonds();
   	notify_(new CreateRepresentationMessage(composites_, MODEL_STICK, COLORING_ELEMENT));
 		notify_(new CreateRepresentationMessage(composites_, MODEL_HBONDS, COLORING_ELEMENT));

	}
	else if (current_step_ == 10)
	{
		notify_(new CreateRepresentationMessage(composites_, MODEL_VDW, COLORING_TEMPERATURE_FACTOR));
	}
	else if (current_step_ == 11)
	{
		notify_(new CreateRepresentationMessage(composites_, MODEL_CARTOON, COLORING_SECONDARY_STRUCTURE));
	}
	else if (current_step_ == 12)
	{
		notify_(new CreateRepresentationMessage(composites_, MODEL_CARTOON, COLORING_RESIDUE_INDEX));
	}
	else if (current_step_ == 13)
	{
		notify_(new CreateRepresentationMessage(composites_, MODEL_STICK, COLORING_RESIDUE_NAME));
	}
	else if (current_step_ == 14)
	{
		getMainControl()->setMultithreading(0);
		notify_(new CreateRepresentationMessage(composites_, MODEL_STICK, COLORING_ELEMENT));
		getMainControl()->setMultithreading(1);

		List<Composite*> composites;
		composites.push_back(*getMainControl()->getCompositeManager().getComposites().begin());
		MolecularControl::getInstance(0)->highlight(composites);

		ms->chooseAmberFF();
		ms->getMDSimulationDialog().setTimeStep(0.001);
 		ms->getMDSimulationDialog().setNumberOfSteps(30);
		ms->MDSimulation(false);
	}
	else if (current_step_ == 15) //FDPB
	{
		ms->calculateFDPB();
		ms->getFDPBDialog()->okPressed();
		disable_button = false;
	}
	else if (current_step_ == 16) // SES colored 
	{
		getMainControl()->setMultithreading(false);
		notify_(new CreateRepresentationMessage(composites_, MODEL_SE_SURFACE, COLORING_ELEMENT));

		getMainControl()->setMultithreading(false);

		Representation* rep = *getMainControl()->getPrimitiveManager().begin();

		ModifySurfaceDialog* cdialog = ModifySurfaceDialog::getInstance(0);
		cdialog->setRepresentation(rep);
		cdialog->setGrid(grid_);
		cdialog->setMinValue(-0.7);
		cdialog->setMaxValue(3.0);
		cdialog->applyPressed();

		rep->setColorProcessor(0);

 		getMainControl()->sendMessage(*new SceneMessage(SceneMessage::REBUILD_DISPLAY_LISTS));

		disable_button = false;
	}
	else if (current_step_ == 17)
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

		// last entry: we are done
		disable_button = false;
	}

	next_button->setEnabled(!disable_button);
}

void DemoTutorialDialog::showTutorial()
{
	demo_mode_ = false;
	show();
}

void DemoTutorialDialog::showDemo()
{
	demo_mode_ = true;
	show();
}


void DemoTutorialDialog::onNotifyTutorial_(Message *message) throw()
{
	CompositeMessage* cmsg = RTTI::castTo<CompositeMessage>(*message);
	RepresentationMessage* rmsg = RTTI::castTo<RepresentationMessage>(*message);

	if (rmsg != 0 && rmsg->getRepresentation() == 0)
	{
		return;
	}

	switch (current_step_)
	{
		case 1: // "Building a peptide from a given sequence"
		{
			if (cmsg == 0 || cmsg->getType() != CompositeMessage::NEW_MOLECULE)
			{
				return;
			}
			break;
		}

		case 2: // "Hierarchy of molecules"
		{
			ControlSelectionMessage* msg = RTTI::castTo<ControlSelectionMessage>(*message);
			if (msg == 0 ||
					msg->getSelection().size() != 1 ||
					!RTTI::isKindOf<System>(**msg->getSelection().begin()))
			{
				return;
			}
			break;
		}

		case 3: // "Molecular Dynamics Simulation")
		{
			if (!RTTI::isKindOf<NewTrajectoryMessage>(*message))
			{
				return;
			}
			break;
		}

		case 4: // "Visualisation of trajectories")
		{
			if (cmsg != 0 && cmsg->getType() == CompositeMessage::CHANGED_COMPOSITE)
			{
				enableNextStep_();
			}
			break;
		}

		case 5:  // "Calculation of electrostatics"
		{
			if (!RTTI::isKindOf<RegularData3DMessage>(*message)) return;
			break;
		}

		case 6: // "Creating a Solvent Excluded Surface"
		{
			if (rmsg == 0 ||
					rmsg->getType() != RepresentationMessage::ADD_TO_GEOMETRIC_CONTROL ||
					rmsg->getRepresentation()->getModelType() != MODEL_SE_SURFACE)
			{
				return;
			}
			break;
		}

		case 7: // "Coloring a SES by electrostatics"
		{
			if (rmsg == 0 || 
					rmsg->getType() != RepresentationMessage::UPDATE &&
					rmsg->getRepresentation()->getModelType() != MODEL_SE_SURFACE)
			{
				return;
			}
			break;
		}

		case 8: // "Creating a isocontour surface"
		{
			if (rmsg == 0 ||
					rmsg->getRepresentation()->getModelType() != MODEL_CONTOUR_SURFACE)
			{
				return;
			}
			break;
		}		

		default:
			BALLVIEW_DEBUG;
			Log.error() << "Current step: " << current_step_ << std::endl;
			return;
	}

	enableNextStep_();
}

} } // namespaces
