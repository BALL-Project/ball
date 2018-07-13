// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include "demoTutorialDialog.h"
#include "mainframe.h"

#include <BALL/VIEW/KERNEL/message.h>

#include <BALL/VIEW/DIALOGS/displayProperties.h>
#include <BALL/VIEW/DIALOGS/FDPBDialog.h>
#include <BALL/VIEW/DIALOGS/modifyRepresentationDialog.h>
#include <BALL/VIEW/DIALOGS/molecularFileDialog.h>
#include <BALL/VIEW/DIALOGS/lightSettings.h>
#include <BALL/VIEW/DIALOGS/stageSettings.h>

#include <BALL/VIEW/DATATYPE/standardDatasets.h>

#include <BALL/VIEW/WIDGETS/molecularStructure.h>
#include <BALL/VIEW/WIDGETS/logView.h>
#include <BALL/VIEW/WIDGETS/datasetControl.h>
#include <BALL/VIEW/WIDGETS/molecularControl.h>
#include <BALL/VIEW/WIDGETS/geometricControl.h>
#include <BALL/SYSTEM/path.h>

#include <QtWidgets/QPushButton>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QTextBrowser>

using namespace std;

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
	: QDialog(parent),
	  Ui_DemoTutorialDialogData(),
	  ModularWidget(name),
	  tutorial_type_(DEMO),
	  composites_(),
	  grid_(nullptr),
	  system_(nullptr),
	  prefix_(""),
	  current_step_(0),
	  surface_(nullptr),
	  demo_action_(nullptr),
	  tutorial_action_(nullptr)
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
	if (LogView::getInstance(0) != 0) LogView::getInstance(0)->hide();
 	if (DatasetControl::getInstance(0) != 0) DatasetControl::getInstance(0)->hide();
}

String DemoTutorialDialog::getBaseDir_()
{
	Path p;
	return p.find(String("BALLView") + FileSystem::PATH_SEPARATOR + "html" + FileSystem::PATH_SEPARATOR);
}

void DemoTutorialDialog::initTutorials_()
{
	if (tutorial_type_ == TUTORIAL)
	{
		setWindowTitle(tr("BALLView Tutorial"));
		prefix_ = getBaseDir_() + "tutorial";
	}

	next_button->setEnabled(false);

	if (tutorial_type_ == TUTORIAL)
	{
		DatasetControl::getInstance(0)->show();
 		DatasetControl::getInstance(0)->applyPreferences();
 		DatasetControl::getInstance(0)->setFloating(false);
	}
	LogView::getInstance(0)->hide();
}

void DemoTutorialDialog::show()
{
	current_step_ = 1;

	if (tutorial_type_ == DEMO)
	{
		initDemo_();
	}
	else  
	{
		int result = QMessageBox::question(this, tr("Warning"),
				tr("To start the tutorial, all loaded structures and molecules will be deleted."),
				QMessageBox::Ok| QMessageBox::Cancel, QMessageBox::Ok);
		if (result != QMessageBox::Ok) return;
		
		initTutorials_();
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

	switch (tutorial_type_)
	{
		case DEMO: 
			onNotifyDemo_(message);
			break;
		case TUTORIAL: 
			onNotifyTutorial_(message);
			break;
	}
}

void DemoTutorialDialog::onNotifyDemo_(Message *message)
{
	RepresentationMessage* rmsg = RTTI::castTo<RepresentationMessage>(*message);

	if (current_step_ == 13 || current_step_ == 14)
	{
		if (!RTTI::isKindOf<FinishedSimulationMessage>(message)) return;
	}
	else if (current_step_ == 15)
	{
		DatasetMessage* msg = RTTI::castTo<DatasetMessage>(*message);
		if (!msg) return;

		if (!msg->getDataset())
		{
			BALLVIEW_DEBUG
			return;
		}

		RegularData3DDataset* set = dynamic_cast<RegularData3DDataset*>(msg->getDataset());
		if (set->getType() != RegularData3DController::type) return;

		grid_ = set->getData();
	}
	else if (current_step_ == 16)
	{
		SceneMessage* msg = RTTI::castTo<SceneMessage>(*message);
		if (!msg || msg->getType() != SceneMessage::REBUILD_DISPLAY_LISTS)
		{
			return;
		}
	}
	else if (!rmsg || rmsg->getType() != RepresentationMessage::UPDATE)
	{
		return;
	}

	enableNextStep_();
}


void DemoTutorialDialog::enableNextStep_()
{
	next_button->setEnabled(true);
}

// TODO: split into several functions...
void DemoTutorialDialog::nextStepClicked()
{
	String id = String(current_step_ + 1); //TODO WHY??
	if (id.size() == 1) id = "0" + id;

	id = prefix_ + id + ".html";

	QUrl qurl = QUrl::fromLocalFile(id.c_str());
	text_browser->setSource(qurl);
	next_button->setEnabled(false);

	if (tutorial_type_ == DEMO) 
	{
		if (current_step_ == 17)
		{
			showTutorial();
			return;
		}

		nextStepDemo_();
		
	}
	else if (tutorial_type_ == TUTORIAL) 
	{
		if (current_step_ == 8)
		{
			next_button->setEnabled(true);
		}
	}

	current_step_++;		
}

void  DemoTutorialDialog::addPlane_(char plane_specifier, int height, int boundary, bool bottom)
{
	HashSet<Composite*> composites = MainControl::getInstance(0)->getCompositeManager().getComposites();
	HashSet<Composite*>::Iterator sit = composites.begin();

	BoundingBoxProcessor bbp;
	Vector3 v_low(0, 0, 0);
	Vector3 v_upp(0, 0, 0);

	System* system = dynamic_cast<System*>(*sit);
	if (system)
	{
		system->apply(bbp);
		v_low = Vector3(bbp.getLower().x, bbp.getLower().y, bbp.getLower().z);
		v_upp = Vector3(bbp.getUpper().x, bbp.getUpper().y, bbp.getUpper().z);
	}
	else
	{
		Log.info() << "DemoTutorialDialog.addPlane(): No system given! "<< __FILE__ << " " << __LINE__ << endl;
		return;
	}

	++sit;
	for (; +sit; ++sit)
	{
		system = dynamic_cast<System*>(*sit);
		if (system)
		{
			system->apply(bbp);
			Vector3 low = Vector3(bbp.getLower().x, bbp.getLower().y, bbp.getLower().z);
			Vector3 upp = Vector3(bbp.getUpper().x, bbp.getUpper().y, bbp.getUpper().z);

			// find the boundaries over all systems	
			if (v_low.x > low.x) v_low.x = low.x;
			if (v_low.y > low.y) v_low.y = low.y; 
			if (v_low.z > low.z) v_low.z = low.z;

			if (v_upp.x < upp.x) v_upp.x = upp.x; 
			if (v_upp.y < upp.y) v_upp.y = upp.y;
			if (v_upp.z < upp.z) v_upp.z = upp.z;

		}
	}

 	if (! bottom)
	{ 
		Vector3 v_tmp = v_low;
		v_low = v_upp;
		v_upp = v_tmp;
		height = height*(-1);
		boundary = boundary*(-1);
	}

	Vector3 v_low_left (0, 0, 0);
	Vector3 v_low_right(0, 0, 0);
	Vector3 v_upp_right(0, 0, 0);
	Vector3 v_upp_left (0, 0, 0);

	Vector3 normal(0, 0, 0);

	
	if (plane_specifier == 'x')
	{
		v_low       = v_low - Vector3(height, boundary, boundary);
		v_upp       = v_upp + Vector3(height, boundary, boundary);
		v_low_left  = Vector3(v_low.x, v_low.y, v_low.z);
		v_low_right = Vector3(v_low.x, v_upp.y, v_low.z);
		v_upp_right = Vector3(v_low.x, v_upp.y, v_upp.z);
		v_upp_left  = Vector3(v_low.x, v_low.y, v_upp.z);
		normal      = Vector3(1, 0, 0);
	}
	else if (plane_specifier == 'y')
	{ 
		v_low       = v_low - Vector3(boundary, height, boundary);
		v_upp       = v_upp + Vector3(boundary, height, boundary);
		v_low_left  = Vector3(v_low.x, v_low.y, v_low.z);
		v_low_right = Vector3(v_low.x, v_low.y, v_upp.z);
		v_upp_right = Vector3(v_upp.x, v_low.y, v_upp.z);
		v_upp_left  = Vector3(v_upp.x, v_low.y, v_low.z);
		normal      = Vector3(0, 1, 0);
	}
	else if (plane_specifier == 'z')
	{
		v_low 			= v_low - Vector3(boundary, boundary, height);
		v_upp 			= v_upp + Vector3(boundary, boundary, height);
		v_low_left  = Vector3(v_low.x, v_low.y, v_low.z);
		v_low_right = Vector3(v_low.x, v_upp.y, v_low.z);
		v_upp_right = Vector3(v_upp.x, v_upp.y, v_low.z);
		v_upp_left  = Vector3(v_upp.x, v_low.y, v_low.z);
		normal 			= Vector3(0, 0, 1);
	}
	else
	{
		Log.info() << "DemoTutorialDialog.addPlane(): unknown plane_specifier! "<< __FILE__ << " " << __LINE__ << endl;
		return;
	}

	// create the plane
	Mesh* plane = new Mesh();

	// the vertices
	plane->vertex.push_back(v_low_left);
	plane->vertex.push_back(v_low_right);
	plane->vertex.push_back(v_upp_right);
	plane->vertex.push_back(v_upp_left);

	// the first triangle of the plane
	Mesh::Triangle t1;
	t1.v1 = 0; // v_low_left
	t1.v2 = 1; // v_low_right
	t1.v3 = 2; // v_upp_right

	plane->triangle.push_back(t1);

	// the second triangle of the plane
	Mesh::Triangle t2;
	t2.v1 = 2; // v_upp_right
	t2.v2 = 3; // v_upp_left
	t2.v3 = 0; // v_low_left

	plane->triangle.push_back(t2);

	// the normals
	for (int i=0; i<4; i++)
		plane->normal.push_back(normal);

	// color
	ColorRGBA color(0,0,0,1);
	plane->setColor(color);

	// a representation
	Representation* rep = getMainControl()->getRepresentationManager().createRepresentation();
	rep->setName("Mirror Plane");

	// insert 
	rep->insert(*plane);

	// and commit
	getMainControl()->insert(*rep);
	getMainControl()->update(*rep);			
	notify_(new RepresentationMessage(*rep, RepresentationMessage::ADD_TO_GEOMETRIC_CONTROL));
}



void DemoTutorialDialog::nextStepDemo_()
{
	// initialisation for first real step
	if (current_step_ == 1)
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
			if (!dialog) return;

			dp->enableCreationForNewMolecules(false);
			system_ = dialog->openMolecularFile(file_name);
			dp->enableCreationForNewMolecules(true);

		  if (!system_)
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
	
	if (current_step_ == 17) // last page
	{
		hide();
		return;
	}
	MolecularStructure* ms = MolecularStructure::getInstance(0);

	next_button->setEnabled(current_step_ >= 14);

	// remove representations
	RepresentationManager& pm = getMainControl()->getRepresentationManager();
	Size nr = pm.getNumberOfRepresentations();
	std::list<Representation*> reps = pm.getRepresentations();

	if (!surface_ && nr == 1 && current_step_ == 6)
	{
		GeometricObject* go = *(**reps.begin()).getGeometricObjects().begin();
		Mesh* mesh = dynamic_cast<Mesh*>(go);
		if (mesh)
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

	if (current_step_ < 6)
	{
		ModelType type = (ModelType) (current_step_ - 1);
		if (type >= MODEL_SA_SURFACE)
		{
			type = (ModelType)((Index)type + 1);
		}
		notify_(new CreateRepresentationMessage(composites_, type, COLORING_ELEMENT));
	}
	else if (current_step_ < 8)
	{	
		ModelType type = (ModelType) (current_step_ - 1);
		if (type >= MODEL_SA_SURFACE)
		{
			type = (ModelType)((Index)type + 1);
		}
		notify_(new CreateRepresentationMessage(composites_, type, COLORING_MOLECULE));
	}
	else if (current_step_ == 8)
	{
		getMainControl()->getMolecularControlSelection().clear();
		getMainControl()->getMolecularControlSelection().push_back(system_);
		ms->calculateHBonds();
   	notify_(new CreateRepresentationMessage(composites_, MODEL_STICK, COLORING_ELEMENT));
	}
	else if (current_step_ == 9)
	{
		notify_(new CreateRepresentationMessage(composites_, MODEL_VDW, COLORING_TEMPERATURE_FACTOR));
	}
	else if (current_step_ == 10)
	{
		notify_(new CreateRepresentationMessage(composites_, MODEL_CARTOON, COLORING_SECONDARY_STRUCTURE));
	}
	else if (current_step_ == 11)
	{
		notify_(new CreateRepresentationMessage(composites_, MODEL_CARTOON, COLORING_RESIDUE_INDEX));
	}
	else if (current_step_ == 12 || current_step_ == 13)
	{
		getMainControl()->setMultithreading(false);
		notify_(new CreateRepresentationMessage(composites_, MODEL_STICK, COLORING_ELEMENT));
		getMainControl()->setMultithreading(true);

		list<Composite*> composites;
		composites.push_back(*getMainControl()->getCompositeManager().getComposites().begin());
 		MolecularControl::getInstance(0)->highlight(composites);

		if (current_step_ == 12)
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
			ms->getMDSimulationDialog().setTimeStep(0.002f);
			ms->getMDSimulationDialog().setNumberOfSteps(30);
			ms->MDSimulation(false);
		}
	}
	else if (current_step_ == 14) //FDPB
	{
		getMainControl()->setMultithreading(0);
		if (!ms->calculateFDPB(false))
		{
			BALLVIEW_DEBUG;
		}
		getMainControl()->setMultithreading(1);
	}
	else if (current_step_ == 15) // SES colored 
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
		cdialog->setMinValue(-0.7f);
		cdialog->setMaxValue(0.7f);
		cdialog->accept();

		getMainControl()->update(*rep);
	}
	else if (current_step_ == 16)
	{
		getMainControl()->setMultithreading(0);
		notify_(new CreateRepresentationMessage(composites_, MODEL_STICK, COLORING_ELEMENT));
		getMainControl()->setMultithreading(1);

		notify_(new CreateRepresentationMessage(composites_, MODEL_STICK, COLORING_ELEMENT));

		DatasetController* dc = DatasetControl::getInstance(0)->getController(RegularData3DController::type);
		RegularData3DController& rcon = *(RegularData3DController*) dc;
		vector<Dataset*> grids = rcon.getDatasets();
		if (grids.empty()) return;
		rcon.computeIsoContourSurface(*grids[0], ColorRGBA(255,0,0), -0.1f);
		rcon.computeIsoContourSurface(*grids[0], ColorRGBA(0,0,255), 0.1f);

		// last entry: we are done
	}
}

void DemoTutorialDialog::showTutorial()
{
	tutorial_type_ = TUTORIAL;
	show();
}

void DemoTutorialDialog::showDemo()
{
	tutorial_type_ = DEMO;
	show();
}

void DemoTutorialDialog::onNotifyTutorial_(Message *message)
{
	CompositeMessage* cmsg = RTTI::castTo<CompositeMessage>(*message);
	RepresentationMessage* rmsg = RTTI::castTo<RepresentationMessage>(*message);

	if (rmsg && !rmsg->getRepresentation()) return;

	switch (current_step_)
	{
		case TUTORIAL_PEPTIDE: // "Building a peptide from a given sequence"
		{
			if (!cmsg || cmsg->getType() != CompositeMessage::NEW_MOLECULE) return;
			break;
		}

		case TUTORIAL_ROTATE: // "Rotating"
		{
            if (!RTTI::isKindOf<SceneMessage>(message)) return;
			break;
		}

		case TUTORIAL_HIERARCHY: // "Hierarchy of molecules"
		{
			break;
		}

		case TUTORIAL_MDS: // "Molecular Dynamics Simulation")
		{
			if (!RTTI::isKindOf<DatasetMessage>(message)) return;
			DatasetMessage* msg = dynamic_cast<DatasetMessage*>(message);
			if (!msg->getDataset())
			{
				BALLVIEW_DEBUG
				return;
			}

			if (msg->getDataset()->getType() != TrajectoryController::type || msg->getType() != DatasetMessage::ADD)
			{
				return;
			}

			break;
		}

		case TUTORIAL_TRAJECTORY: // "Visualisation of trajectories")
		{
			if (cmsg && cmsg->getType() == CompositeMessage::CHANGED_COMPOSITE)
			{
				enableNextStep_();
			}
			break;
		}

		case TUTORIAL_ES:  // "Calculation of electrostatics"
		{
            if (!RTTI::isKindOf<DatasetMessage>(message)) return;
			DatasetMessage* msg = dynamic_cast<DatasetMessage*>(message);
			if (!msg->getDataset())
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
			if (!rmsg ||
					rmsg->getType() != RepresentationMessage::ADD_TO_GEOMETRIC_CONTROL ||
					rmsg->getRepresentation()->getModelType() != MODEL_SE_SURFACE)
			{
				return;
			}
			break;
		}

		case TUTORIAL_SES_COLORING: // "Coloring a SES by electrostatics"
		{
			if (!rmsg ||
					(rmsg->getType() != RepresentationMessage::UPDATE &&
					rmsg->getRepresentation()->getModelType() != MODEL_SE_SURFACE))
			{
				return;
			}
			break;
		}

		case TUTORIAL_CS: // "Creating a isocontour surface"
		{
			if (!rmsg || rmsg->getRepresentation()->getModelType() != MODEL_CONTOUR_SURFACE)
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
	demo_action_ = insertMenuEntry(MainControl::HELP, tr("Demo"), this, SLOT(showDemo()), 
	                               description, QKeySequence(),
	                               tr("Show a demonstration of BALLView's features"),
	                               UIOperationMode::MODE_ADVANCED);

	description = "Shortcut|Help|Tutorial";
	tutorial_action_ = insertMenuEntry(MainControl::HELP, tr("Tutorial"), this, SLOT(showTutorial()), 
	                                   description, QKeySequence(), tr("Perform a step-by-step tutorial"),
	                                   UIOperationMode::MODE_ADVANCED);

	getMainControl()->insertPopupMenuSeparator(MainControl::HELP);

}

void DemoTutorialDialog::checkMenu(MainControl& main_control)
{
	bool busy = main_control.isBusy();
	if (demo_action_)
		demo_action_->setEnabled(!busy);
	if (tutorial_action_)
		tutorial_action_->setEnabled(!busy);
}

} } // namespaces
