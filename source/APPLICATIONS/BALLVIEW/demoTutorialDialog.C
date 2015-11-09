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
#include <BALL/VIEW/DIALOGS/lightSettings.h>
#include <BALL/VIEW/DIALOGS/stageSettings.h>

#include <BALL/VIEW/DATATYPE/standardDatasets.h>
#include <BALL/DATATYPE/contourSurface.h>

#include <BALL/VIEW/WIDGETS/molecularStructure.h>
#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/WIDGETS/logView.h>
#include <BALL/VIEW/WIDGETS/pyWidget.h>
#include <BALL/VIEW/WIDGETS/datasetControl.h>
#include <BALL/VIEW/WIDGETS/molecularControl.h>
#include <BALL/VIEW/WIDGETS/geometricControl.h>
#include <BALL/VIEW/WIDGETS/helpViewer.h>
#include <BALL/STRUCTURE/geometricProperties.h>
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
	:	QDialog(parent),
		Ui_DemoTutorialDialogData(),
		ModularWidget(name),
		surface_(0),
		demo_action_(0),
		tutorial_action_(0),
		raytracing_tutorial_action_(0)
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
	
#ifdef	BALL_HAS_RTFACT
	// Set the background to black
	ColorRGBA color(0, 0, 0, 255);
	Stage* stage = Scene::getInstance(0)->getStage();
		
	stage->setBackgroundColor(color);
	StageSettings* stage_settings = Scene::getInstance(0)->getStageSettings();
	stage_settings->updateFromStage();

	//TODO: get rid of this hack. 
	//TODO: BALLView should come up with good light settings by itself!
	// set a good light source	
	stage->clearLightSources();

	LightSource ls;
		
	ls.setPosition(Vector3(1, -2, -15));
	ls.setAttenuation(Vector3(0., 0., 0.3));
	ls.setType(LightSource::POSITIONAL);
	ls.setColor(ColorRGBA(255, 255, 255, 255));
	ls.setIntensity(500./100);
	stage->addLightSource(ls);
	LightSettings::getInstance(0)->updateFromStage();
	// apply everything to the scene...
	Scene::getInstance(0)->applyPreferences();
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

void DemoTutorialDialog::initTutorials_()
{
	if (tutorial_type_ == TUTORIAL)
	{
		setWindowTitle(tr("BALLView Tutorial"));
		prefix_ = getBaseDir_() + "tutorial";
	}
	else if (tutorial_type_ == RAYTRACING_TUTORIAL)
	{
		setWindowTitle(tr("Ray tracing Tutorial"));
		prefix_ = getBaseDir_() + "raytracing_tutorial";
	}
	
	next_button->setEnabled(false);

#ifdef BALL_HAS_RTFACT
	// Set the defaults
	((Mainframe*)getMainControl())->reset();

	Scene::getInstance(0)->show();
	MolecularControl::getInstance(0)->show();
	MolecularControl::getInstance(0)->setFloating(false);
	MolecularControl::getInstance(0)->applyPreferences();
 	GeometricControl::getInstance(0)->show();
	GeometricControl::getInstance(0)->applyPreferences();
	GeometricControl::getInstance(0)->setFloating(false);

	// Set the background to black
	ColorRGBA color(0, 0, 0, 255);
	Stage* stage = Scene::getInstance(0)->getStage();
		
	stage->setBackgroundColor(color);

	StageSettings* stage_settings = Scene::getInstance(0)->getStageSettings();
	stage_settings->updateFromStage();

	// get one useable light source
	stage->clearLightSources();

	LightSource ls;
		
	ls.setPosition(Vector3(1, -2, -15));
	ls.setAttenuation(Vector3(0., 0., 0.7));
	ls.setType(LightSource::POSITIONAL);
	ls.setColor(ColorRGBA(255, 255, 255, 255));
	ls.setIntensity(500./100);

	stage->addLightSource(ls);
	LightSettings::getInstance(0)->updateFromStage();
#endif
	if (tutorial_type_ == TUTORIAL)
	{
		DatasetControl::getInstance(0)->show();
 		DatasetControl::getInstance(0)->applyPreferences();
 		DatasetControl::getInstance(0)->setFloating(false);
	}
	else if  (tutorial_type_ == RAYTRACING_TUTORIAL)
	{ 
#ifdef BALL_HAS_RTFACT
		Stage::Material& rt_material = stage->getMaterial();

		rt_material.ambient_color = ColorRGBA(255, 255, 255, 255);
		rt_material.ambient_intensity = 0.;

		rt_material.specular_color = ColorRGBA(255, 255, 255, 255);
		rt_material.specular_intensity = 1.;

		rt_material.reflective_color = ColorRGBA(255, 255, 255, 255);
		rt_material.reflective_intensity = 0.;

		rt_material.shininess = 75.031;
		rt_material.transparency = 0;

		Scene::getInstance(0)->getMaterialSettings()->updateDefaultMaterialsFromStage();

		// set ball and stick as next model
		DisplayProperties::getInstance(0)->selectModel(MODEL_BALL_AND_STICK);

		// apply everything to the scene...
		Scene::getInstance(0)->applyPreferences();

		next_button->setEnabled(true);
#else
		Log.info() << "DemoTutorialDialog: no RTFact available! Close the dialog!" << __FILE__ << " " << __LINE__ << endl;
		return;
#endif
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
		case RAYTRACING_TUTORIAL:
			onNotifyRaytracingTutorial_(message);
			break;
	}
}

void DemoTutorialDialog::onNotifyDemo_(Message *message)
{
	RepresentationMessage* rmsg = RTTI::castTo<RepresentationMessage>(*message);

	if (current_step_ == 13 ||
			current_step_ == 14)
	{
        if (!RTTI::isKindOf<FinishedSimulationMessage>(message)) return;
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
		
		#ifdef BALL_HAS_RTFACT
		// we do not want to show energy minimization and MD simulation
		if (current_step_ == 11)
		{
			current_step_ +=2;	
			next_button->setEnabled(true);
		}
		#endif

	}
	else if (tutorial_type_ == TUTORIAL) 
	{
		if (current_step_ == 8)
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
	else if (tutorial_type_ == RAYTRACING_TUTORIAL) 
	{
		switch (current_step_)
		{
			case 2:
			{
				// prepare the background for the next step
				ColorRGBA color(255, 255, 255, 255); // white
				Scene::getInstance(0)->getStage()->setBackgroundColor(color);
				StageSettings* stage_settings = Scene::getInstance(0)->getStageSettings();
				stage_settings->updateFromStage();
				Scene::getInstance(0)->applyPreferences();
				break;
			}
			
			case 4: // preparing downgraded light settings 
			{
				// There should be just a single light source!
				// first manipulate the light
				LightSource& ls = Scene::getInstance(0)->getStage()->getLightSource(0);
				ls.setPosition(Vector3(1, -8, -45));        //(Vector3(1, -2, -15));
				ls.setAttenuation(Vector3(0., 0., 0.2));    //0.7
				ls.setType(LightSource::POSITIONAL);
				ls.setColor(ColorRGBA(255, 255, 255, 255)); //ColorRGBA(255, 245, 208, 255));
				ls.setIntensity(250./100);
				LightSettings::getInstance(0)->updateFromStage();

				// then change the camera position
				Camera& camera = Scene::getInstance(0)->getStage()->getCamera();
				camera.setViewPoint(camera.getViewPoint()+Vector3(20,20,20));

				// update everything
				Scene::getInstance(0)->applyPreferences();
				break;
			}
			case 5: //preparing bad materials
			{
				// Add a plane to be used as a mirror
				if (getMainControl()->getCompositeManager().getComposites().size() == 0)
				{
					Log.info() << "DemoTutorialDialog: no system available! " << __FILE__ << " " << __LINE__ << endl;
				 	return;	
				}
				
				addPlane_('x', 5, 5);
				break;
			}
			case 7:
			{
				// offer the Documentation
				hide();
				HelpViewer* hv = HelpViewer::getInstance(1);
				if (hv == 0) return;
				hv->showHelp();
				hv->setFloating(true);
				hv->showMaximized();	
				break;
			}

			default: // nothing to see here...
				break;
		}
	}
 
	current_step_++;		
}

void  DemoTutorialDialog::addPlane_(char plane_specifier, int height, int boundary, bool bottom)
{
	HashSet<Composite*> composites = MainControl::getInstance(0)->getCompositeManager().getComposites();
	HashSet<Composite*>::Iterator sit = composites.begin();

	BoundingBoxProcessor bbp;
	Vector3 v_low(0., 0., 0.);
	Vector3 v_upp(0., 0., 0.);

	System* system = NULL;
	system = dynamic_cast<System*>(*sit);
	if (system != 0) 
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
		if (system != 0) 
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

	Vector3 v_low_left (0., 0., 0.);
	Vector3 v_low_right(0., 0., 0.);
	Vector3 v_upp_right(0., 0., 0.);
	Vector3 v_upp_left (0., 0., 0.);

	Vector3 normal(0., 0., 0.);

	
	if (plane_specifier == 'x')
	{
		v_low       = v_low - Vector3(height, boundary, boundary);
		v_upp       = v_upp + Vector3(height, boundary, boundary);
		v_low_left  = Vector3(v_low.x, v_low.y, v_low.z);
		v_low_right = Vector3(v_low.x, v_upp.y, v_low.z);
		v_upp_right = Vector3(v_low.x, v_upp.y, v_upp.z);
		v_upp_left  = Vector3(v_low.x, v_low.y, v_upp.z);
		normal      = Vector3(1., 0., 0.);
	}
	else if (plane_specifier == 'y')
	{ 
		v_low       = v_low - Vector3(boundary, height, boundary);
    v_upp       = v_upp + Vector3(boundary, height, boundary);
    v_low_left  = Vector3(v_low.x, v_low.y, v_low.z);
    v_low_right = Vector3(v_low.x, v_low.y, v_upp.z);
    v_upp_right = Vector3(v_upp.x, v_low.y, v_upp.z);
    v_upp_left  = Vector3(v_upp.x, v_low.y, v_low.z);
    normal      = Vector3(0., 1., 0.);
	}
	else if (plane_specifier == 'z')
	{
		v_low 			= v_low - Vector3(boundary, boundary, height);
    v_upp 			= v_upp + Vector3(boundary, boundary, height);
    v_low_left  = Vector3(v_low.x, v_low.y, v_low.z);
    v_low_right = Vector3(v_low.x, v_upp.y, v_low.z);
    v_upp_right = Vector3(v_upp.x, v_upp.y, v_low.z);
    v_upp_left  = Vector3(v_upp.x, v_low.y, v_low.z);
    normal 			= Vector3(0., 0., 1.);
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

	if (surface_ == 0 && nr == 1 && current_step_ == 6)
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
#ifndef BALL_HAS_RTFACT
 		notify_(new CreateRepresentationMessage(composites_, MODEL_HBONDS, COLORING_ELEMENT));
#endif
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
	else if (current_step_ == 12 ||
					 current_step_ == 13)
	{
		getMainControl()->setMultithreading(0);
		notify_(new CreateRepresentationMessage(composites_, MODEL_STICK, COLORING_ELEMENT));
		getMainControl()->setMultithreading(1);

		list<Composite*> composites;
		composites.push_back(*getMainControl()->getCompositeManager().getComposites().begin());
 		MolecularControl::getInstance(0)->highlight(composites);

		if (current_step_ == 12)
		{
			ms->getAmberConfigurationDialog().resetOptions();
			ms->chooseAmberFF();
			ms->getMinimizationDialog().setMaxGradient(1.);
			ms->getMinimizationDialog().setMaxIterations(20);
#ifdef BALL_HAS_RTFACT
			ms->getMinimizationDialog().setRefresh(10);
#else
			ms->getMinimizationDialog().setRefresh(5);
#endif
			ms->runMinimization(false);
		}
		else
		{
			ms->getMDSimulationDialog().setTimeStep(0.002);
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
		cdialog->setMinValue(-0.7);
		cdialog->setMaxValue(0.7);
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
 		rcon.computeIsoContourSurface(*grids[0], ColorRGBA(255,0,0), -0.1);
 		rcon.computeIsoContourSurface(*grids[0], ColorRGBA(0,0,255), 0.1);

		// last entry: we are done
	}
}

void DemoTutorialDialog::showRaytracingTutorial()
{
	tutorial_type_ = RAYTRACING_TUTORIAL;
	show();
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
            if (!RTTI::isKindOf<DatasetMessage>(message)) return;
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

void DemoTutorialDialog::onNotifyRaytracingTutorial_(Message *message)
{
	CompositeMessage* cmsg = RTTI::castTo<CompositeMessage>(*message);
	RepresentationMessage* rmsg = RTTI::castTo<RepresentationMessage>(*message);

	if (rmsg != 0 && rmsg->getRepresentation() == 0) return;

	switch (current_step_)
	{
		case 1:
		{
			// nothing to be checked
			break;
		}
		case 2: // "Building a peptide from a given sequence"
		{
			if (cmsg == 0 || cmsg->getType() != CompositeMessage::NEW_MOLECULE) return;
			break;
		}
		case 3: // "Set the background color"
		{
			if (Scene::getInstance(0)->getStage()->getBackgroundColor() != ColorRGBA(0, 0, 0, 255)) return;
			break;
		}	
		case 4: // "Rotating"
		{
            if (!RTTI::isKindOf<SceneMessage>(message)) return;
			break;
		}	
		case 5: // "Setting light sources"
		{		
			//if (Scene::getInstance(0)->getStage()->getLightSource(0).getColor() != ColorRGBA(255, 245, 208, 255)) return;
			if (Scene::getInstance(0)->getStage()->getLightSources().size() != 2) return;
			break;
		}
		case 6: // "Setting the materials"
		{	
			// check, if we got an SES 
			if (rmsg == 0 ||
					rmsg->getType() != RepresentationMessage::ADD_TO_GEOMETRIC_CONTROL ||
					rmsg->getRepresentation()->getModelType() != MODEL_SE_SURFACE)
			{
				return;
			}
			break;
		}
		case 7:  // "downsampling/PNGs"
		{	
			//TODO find a checker!!
			if (cmsg != 0) 
				cout <<  "*7*" << cmsg->getType() << endl;
			break; 
		}	
	
		/* Put into a later step
			if (tutorial_type_ == RAYTRACING_TUTORIAL)
			{
				// set the camera non-optimal
				Camera& camera = Scene::getInstance(0)->getStage()->getCamera();
				Vector3 absolute_offset = 	camera.getRightVector() * 100
																	+	camera.getLookUpVector() * 100
																	+ camera.getViewVector() * 100;

				camera.setViewPoint(camera.getViewPoint()+absolute_offset);
				camera.setLookAtPosition(camera.getLookAtPosition()+absolute_offset);		
				Scene::getInstance(0)->rotateClockwise(90);
				Scene::getInstance(0)->update();
				Scene::getInstance(0)->updateGL();
			}
			*/

		default:
			BALLVIEW_DEBUG;
			Log.error() << "Current step: " << current_step_ << std::endl;
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
																 tr("Show a demonstration of BALLView's features"), UIOperationMode::MODE_ADVANCED);

	description = "Shortcut|Help|Tutorial";
	tutorial_action_ = insertMenuEntry(MainControl::HELP, tr("Tutorial"), this, SLOT(showTutorial()), 
	                                   description, QKeySequence(), tr("Perform a step-by-step tutorial"),
																		 UIOperationMode::MODE_ADVANCED);

#ifdef BALL_HAS_RTFACT
	description = "Shortcut|Help|RaytracingTutorial";
	raytracing_tutorial_action_ = insertMenuEntry(MainControl::HELP, tr("Ray tracing Tutorial"), this, 
	                                              SLOT(showRaytracingTutorial()), description, QKeySequence(),
																								tr("Learn how to use RTFact"), UIOperationMode::MODE_ADVANCED);
#endif

	getMainControl()->insertPopupMenuSeparator(MainControl::HELP);

}

void DemoTutorialDialog::checkMenu(MainControl& main_control)
{
	bool busy = main_control.isBusy();
	if (demo_action_)
		demo_action_->setEnabled(!busy);
	if (tutorial_action_)
		tutorial_action_->setEnabled(!busy);
	if (raytracing_tutorial_action_)
		raytracing_tutorial_action_->setEnabled(!busy);
}
						
} } // namespaces
