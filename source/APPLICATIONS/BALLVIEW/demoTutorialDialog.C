// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include "demoTutorialDialog.h"
#include "mainframe.h"

#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/VIEW/PRIMITIVES/mesh.h>
#include <BALL/VIEW/KERNEL/message.h>

#include <BALL/VIEW/DIALOGS/displayProperties.h>
#include <BALL/VIEW/DIALOGS/FDPBDialog.h>
#include <BALL/VIEW/DIALOGS/modifyRepresentationDialog.h>
#include <BALL/VIEW/DIALOGS/molecularFileDialog.h>

#include <BALL/VIEW/DATATYPE/standardDatasets.h>

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

#include <QtGui/QPushButton>
#include <QtGui/QMessageBox>
#include <QtGui/QTextBrowser>

namespace BALL
{
	namespace VIEW
	{

enum TutorialSteps
{
	TUTORIAL_PEPTIDE = 1,
	TUTORIAL_ROTATE,
	TUTORIAL_HIERARCHY,
	TUTORIAL_MDS,
	TUTORIAL_TRAJECTORY,
	TUTORIAL_ES,
	TUTORIAL_SES,
	TUTORIAL_SES_COLORING,
	TUTORIAL_CS
};


DemoTutorialDialog::DemoTutorialDialog(QWidget* parent, const char* name)
	:	QDialog(parent),
		Ui_DemoTutorialDialogData(),
		ModularWidget(name),
		surface_(0),
		demo_action_(0),
		tutorial_action_(0)
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "new DemoTutorialDialog " << this << std::endl;
#endif

	setupUi(this);
	setObjectName(name);

	// register the widget with the MainControl
 	ModularWidget::registerWidget(this);
	hide();
	connect(next_button, SIGNAL(clicked()), this, SLOT(nextStepClicked()));
	connect(cancel_button, SIGNAL(clicked()), this, SLOT(hide()));
}

DemoTutorialDialog::~DemoTutorialDialog()
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "deleting DemoTutorialDialog " << this << std::endl;
#endif

	delete surface_;
}

void DemoTutorialDialog::initDemo_()
{
	setWindowTitle(tr("BALLView Demo"));

	prefix_ = getBaseDir_() + "demo";

	next_button->setEnabled(true);
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
	Path p;
	String dir = p.find(   String("..")
	                     + FileSystem::PATH_SEPARATOR
											 + "doc"
											 + FileSystem::PATH_SEPARATOR
											 + "internal" 
											 + FileSystem::PATH_SEPARATOR );

	return dir;
}

void DemoTutorialDialog::initTutorial_()
{
	setWindowTitle(tr("BALLView Tutorial"));
	
	prefix_ = getBaseDir_() + "tutorial";

	next_button->setEnabled(false);

	((Mainframe*)getMainControl())->reset();

	Scene::getInstance(0)->show();
	MolecularControl::getInstance(0)->show();
	MolecularControl::getInstance(0)->setFloating(false);
	MolecularControl::getInstance(0)->applyPreferences();
 	DatasetControl::getInstance(0)->show();
 	DatasetControl::getInstance(0)->applyPreferences();
 	DatasetControl::getInstance(0)->setFloating(false);
	GeometricControl::getInstance(0)->show();
	GeometricControl::getInstance(0)->applyPreferences();
	GeometricControl::getInstance(0)->setFloating(false);

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
		int result = QMessageBox::question(this, tr("Warning"),
				tr("To start the tutorial, all loaded structures and molecules will be deleted."),
				QMessageBox::Ok| QMessageBox::Cancel, QMessageBox::Ok);
		if (result != QMessageBox::Ok) return;

		initTutorial_();
	}

	QUrl qurl = QUrl::fromLocalFile((prefix_ + "01.html").c_str());
	text_browser->setSource(qurl);

	QDialog::show();
	resize(350, 650);
	raise();
}

void DemoTutorialDialog::onNotify(Message* message)
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
{
	RepresentationMessage* rmsg = RTTI::castTo<RepresentationMessage>(*message);

	if (current_step_ == 13 ||
			current_step_ == 14)
	{
		if (!RTTI::isKindOf<FinishedSimulationMessage>(*message)) return;
	}
	else if (current_step_ == 15)
	{
		DatasetMessage* msg = RTTI::castTo<DatasetMessage>(*message);
		if (msg == 0) return;

		if (msg->getDataset() == 0)
		{
			BALLVIEW_DEBUG
			return;
		}

		RegularData3DDataset* set = dynamic_cast<RegularData3DDataset*>(msg->getDataset());
		if (set->getType() != RegularData3DController::type) return;

		grid_ = (RegularData3D*) set->getData();
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

	QUrl qurl = QUrl::fromLocalFile(id.c_str());
	text_browser->setSource(qurl);
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
			HelpViewer* hv = HelpViewer::getInstance(1);
			if (hv == 0) return;
			hv->showHelp();
			hv->setFloating(true);
			hv->showMaximized();
		}
	}
}


void DemoTutorialDialog::nextStepDemo_()
{
	// initialisation for first real step
	if (current_step_ == 2)
	{
		DisplayProperties* dp = DisplayProperties::getInstance(0);
		dp->setDrawingPrecision(DRAWING_PRECISION_HIGH);

		((Mainframe*)getMainControl())->reset();

		// open bpti 
		try
		{
			Path path;
			String file_name = path.find("structures/bpti.pdb");

			MolecularFileDialog* dialog = MolecularFileDialog::getInstance(0);
			if (dialog == 0) return;

			dp->enableCreationForNewMolecules(false);
			system_ = dialog->openMolecularFile(file_name);
			dp->enableCreationForNewMolecules(true);

			if (system_ == 0)
			{
				String msg((String)tr("Could not open bpti.pdb. Maybe the file was deleted?")+"\n");
				msg += (String)tr("It should be found in") + " " + file_name;

				QMessageBox::critical(0, tr("Error while starting BALLView Demo"), msg.c_str(),
						QMessageBox::Ok, Qt::NoButton, Qt::NoButton);
				return;
			}
			
			system_->apply(getFragmentDB().add_hydrogens);
			system_->apply(getFragmentDB().build_bonds);
			getMainControl()->update(*system_, true);
		}
		catch(Exception::FileNotFound e)
		{
			Log.error() << (String)tr("Could not open") << " " << e.getFilename() << std::endl;
			return;
		}

		composites_.clear();
		composites_.push_back(system_);
	}

	if (current_step_ == 18) // last page
	{
		hide();
		return;
	}

	MolecularStructure* ms = MolecularStructure::getInstance(0);

	next_button->setEnabled(current_step_ >= 15);

	// remove representations
	RepresentationManager& pm = getMainControl()->getRepresentationManager();
	Size nr = pm.getNumberOfRepresentations();
	std::list<Representation*> reps = pm.getRepresentations();

	if (surface_ == 0 && nr == 1 && current_step_ == 7)
	{
		GeometricObject* go = *(**reps.begin()).getGeometricObjects().begin();
		Mesh* mesh = dynamic_cast<Mesh*>(go);
		if (mesh != 0)
		{
			surface_ = new Mesh(*mesh);
		}
		else
		{
			// should not happen
			BALLVIEW_DEBUG
			surface_ = new Mesh();
		}
	}

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
	else if (current_step_ == 13 ||
					 current_step_ == 14)
	{
		getMainControl()->setMultithreading(0);
		notify_(new CreateRepresentationMessage(composites_, MODEL_STICK, COLORING_ELEMENT));
		getMainControl()->setMultithreading(1);

		list<Composite*> composites;
		composites.push_back(*getMainControl()->getCompositeManager().getComposites().begin());
 		MolecularControl::getInstance(0)->highlight(composites);

		if (current_step_ == 13)
		{
			ms->getAmberConfigurationDialog().resetOptions();
			ms->chooseAmberFF();
			ms->getMinimizationDialog().setMaxGradient(1.);
			ms->getMinimizationDialog().setMaxIterations(20);
			ms->getMinimizationDialog().setRefresh(5);
			ms->runMinimization(false);
		}
		else
		{
			ms->getMDSimulationDialog().setTimeStep(0.002);
			ms->getMDSimulationDialog().setNumberOfSteps(30);
			ms->MDSimulation(false);
		}
	}
	else if (current_step_ == 15) //FDPB
	{
		getMainControl()->setMultithreading(0);
		if (!ms->calculateFDPB(false))
		{
			BALLVIEW_DEBUG;
		}
		getMainControl()->setMultithreading(1);
	}
	else if (current_step_ == 16) // SES colored 
	{
		// Create a new representation containing the contour surface.
		Representation* rep = getMainControl()->getRepresentationManager().createRepresentation();
		rep->setModelType(MODEL_SE_SURFACE); 
		rep->insert(*new Mesh(*surface_));
		getMainControl()->insert(*rep);

		ModifyRepresentationDialog* cdialog = ModifyRepresentationDialog::getInstance(0);
		cdialog->setMode(0);
		cdialog->setRepresentation(rep);
		cdialog->setGrid(grid_);
		cdialog->setMinValue(-0.7);
		cdialog->setMaxValue(0.7);
		cdialog->accept();

		getMainControl()->update(*rep);
	}
	else if (current_step_ == 17)
	{
		getMainControl()->setMultithreading(0);
		notify_(new CreateRepresentationMessage(composites_, MODEL_STICK, COLORING_ELEMENT));
		getMainControl()->setMultithreading(1);

   	notify_(new CreateRepresentationMessage(composites_, MODEL_STICK, COLORING_ELEMENT));

		DatasetController* dc = DatasetControl::getInstance(0)->getController(RegularData3DController::type);
		RegularData3DController& rcon = *(RegularData3DController*) dc;
		vector<Dataset*> grids = rcon.getDatasets();
		if (grids.size() == 0) return;
 		rcon.computeIsoContourSurface(*grids[0], ColorRGBA(255,0,0), -0.1);
 		rcon.computeIsoContourSurface(*grids[0], ColorRGBA(0,0,255), 0.1);

		// last entry: we are done
	}
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

void DemoTutorialDialog::onNotifyTutorial_(Message *message)
{
	CompositeMessage* cmsg = RTTI::castTo<CompositeMessage>(*message);
	RepresentationMessage* rmsg = RTTI::castTo<RepresentationMessage>(*message);

	if (rmsg != 0 && rmsg->getRepresentation() == 0) return;

	switch (current_step_)
	{
		case TUTORIAL_PEPTIDE: // "Building a peptide from a given sequence"
		{
			if (cmsg == 0 || cmsg->getType() != CompositeMessage::NEW_MOLECULE) return;
			break;
		}

		case TUTORIAL_ROTATE: // "Rotating"
		{
			if (!RTTI::isKindOf<SceneMessage>(*message)) return;
			break;
		}

		case TUTORIAL_HIERARCHY: // "Hierarchy of molecules"
		{
			break;
		}

		case TUTORIAL_MDS: // "Molecular Dynamics Simulation")
		{
			if (!RTTI::isKindOf<DatasetMessage>(*message)) return;
			DatasetMessage* msg = dynamic_cast<DatasetMessage*>(message);
			if (msg->getDataset() == 0)
			{
				BALLVIEW_DEBUG
				return;
			}

			if (msg->getDataset()->getType() != TrajectoryController::type ||
					msg->getType() != DatasetMessage::ADD)
			{
				return;
			}

			break;
		}

		case TUTORIAL_TRAJECTORY: // "Visualisation of trajectories")
		{
			if (cmsg != 0 && cmsg->getType() == CompositeMessage::CHANGED_COMPOSITE)
			{
				enableNextStep_();
			}
			break;
		}

		case TUTORIAL_ES:  // "Calculation of electrostatics"
		{
			if (!RTTI::isKindOf<DatasetMessage>(*message)) return;
			DatasetMessage* msg = dynamic_cast<DatasetMessage*>(message);
			if (msg->getDataset() == 0)
			{
				BALLVIEW_DEBUG
				return;
			}

			if (msg->getDataset()->getType() != RegularData3DController::type ||
					msg->getType() != DatasetMessage::ADD)
			{
				return;
			}

			break;
		}

		case TUTORIAL_SES: // "Creating a Solvent Excluded Surface"
		{
			if (rmsg == 0 ||
					rmsg->getType() != RepresentationMessage::ADD_TO_GEOMETRIC_CONTROL ||
					rmsg->getRepresentation()->getModelType() != MODEL_SE_SURFACE)
			{
				return;
			}
			break;
		}

		case TUTORIAL_SES_COLORING: // "Coloring a SES by electrostatics"
		{
			if (rmsg == 0 || 
					(rmsg->getType() != RepresentationMessage::UPDATE &&
					rmsg->getRepresentation()->getModelType() != MODEL_SE_SURFACE))
			{
				return;
			}
			break;
		}

		case TUTORIAL_CS: // "Creating a isocontour surface"
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
			Log.error() << (String)tr("Current step") << ": " << current_step_ << std::endl;
			return;
	}

	enableNextStep_();
}

void DemoTutorialDialog::initializeWidget(MainControl&)
{
	getMainControl()->insertPopupMenuSeparator(MainControl::HELP);
	
	String description = "Shortcut|Help|Demo";
	demo_action_ = insertMenuEntry(MainControl::HELP, (String)tr("Demo"), this, SLOT(showDemo()), description);
	setMenuHint((String)tr("Show a demonstration of BALLView's features"));

	description = "Shortcut|Help|Tutorial";
	tutorial_action_ = insertMenuEntry(MainControl::HELP, (String)tr("Tutorial"), this, SLOT(showTutorial()), description);
	setMenuHint((String)tr("Perform a step-by-step tutorial"));
	getMainControl()->insertPopupMenuSeparator(MainControl::HELP);
}

void DemoTutorialDialog::checkMenu(MainControl& main_control)
{
	bool busy = main_control.isBusy();
	demo_action_->setEnabled(!busy);
	tutorial_action_->setEnabled(!busy);
}
						
} } // namespaces
