// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include "mainframe.h"
#include "icons.h"
#include "DIALOGS/DlgAbout.h"

#include <BALL/MOLMEC/AMBER/amber.h>
#include <BALL/MOLMEC/MINIMIZATION/conjugateGradient.h>
#include <BALL/MOLMEC/MDSIMULATION/microCanonicalMD.h>
#include <BALL/MOLMEC/MINIMIZATION/steepestDescent.h>
#include <BALL/MOLMEC/MDSIMULATION/canonicalMD.h>
#include <BALL/MOLMEC/MDSIMULATION/molecularDynamics.h>

#include <BALL/VIEW/GUI/DIALOGS/fileDialog.h>
#include <BALL/MOLVIEW/GUI/KERNEL/moleculeObjectCreator.h>
#include <BALL/MOLVIEW/GUI/DIALOGS/peptideDialog.h>
#include <BALL/DATATYPE/regularData3D.h>
#include <BALL/DATATYPE/contourSurface.h>
#include <BALL/VIEW/GUI/PRIMITIV/glmesh.h>
#include <BALL/STRUCTURE/residueChecker.h>
#include <BALL/STRUCTURE/geometricProperties.h>

#ifdef BALL_PYTHON_SUPPORT
#	include <BALL/VIEW/GUI/WIDGETS/pyWidget.h>
#endif

#include <qlabel.h>
#include <qmenubar.h>
#include <qsplitter.h>
#include <qstatusbar.h>
#include <qlabel.h>

#ifdef QT_THREAD_SUPPORT
	 #include "threads.h"
#endif

using namespace std;

namespace BALL
{

using BALL::MOLVIEW::PeptideDialog;

Mainframe::Mainframe(QWidget* parent, const char* name)
	:	MainControl(parent, name, ".options"),
		scene_(0),
		control_(0),
		display_properties_(0),
		minimization_dialog_(0),
		label_properties_(0),
		molecular_properties_(0),
		file_dialog_(0),
		server_(0),
		GL_object_collector_(),
		fragment_db_(),
		hor_splitter_(0),
		vert_splitter_(0),
		logview_(0),
		vboxlayout_(0),
		popup_menus_(),
		tool_box_(0),
		simulation_thread_(0),
		fullscreen_(false),
		stop_simulation_(false)
{
	// ---------------------
	// setup main window
	// ---------------------
	setCaption("MolVIEW");
	setIcon(*new QPixmap(bucky_64x64_xpm));

	resize(640,400);

	// ---------------------
	// Logstream setup -----
	// ---------------------
	Log.remove(std::cout);

	// ---------------------
	// create widgets ------
	// ---------------------
	vert_splitter_ = new QSplitter(this, "VertSplitter");
	CHECK_PTR(vert_splitter_);
	vert_splitter_->setOrientation(Vertical);
	setCentralWidget(vert_splitter_);

	hor_splitter_ = new QSplitter(vert_splitter_, "HorSplitter");
	CHECK_PTR(hor_splitter_);

	FDPB_dialog_ = new FDPBDialog(this);
	CHECK_PTR(FDPB_dialog_);

	control_ = new MolecularControl(hor_splitter_);
	CHECK_PTR(control_);

	scene_ = new Scene(hor_splitter_);
	CHECK_PTR(scene_);

	display_properties_ = new DisplayProperties(this);
	CHECK_PTR(display_properties_);

	minimization_dialog_ = new AmberMinimizationDialog(this);
	CHECK_PTR(minimization_dialog_);

	md_dialog_ = new MolecularDynamicsDialog(this);
	CHECK_PTR(md_dialog_);

	surface_dialog_ = new ContourSurfaceDialog(this);
	CHECK_PTR(surface_dialog_);

	label_properties_ = new LabelProperties(this);
	CHECK_PTR(label_properties_);
	
	file_dialog_ = new MolecularFileDialog(this);
	CHECK_PTR(file_dialog_);

	molecular_properties_ = new MolecularProperties(this);
	CHECK_PTR(molecular_properties_);

	server_ = new Server(this);
	CHECK_PTR(server_);

	logview_ = new LogView(vert_splitter_);
	CHECK_PTR(logview_);

	#ifdef BALL_PYTHON_SUPPORT
		PyWidget* py_widget = new PyWidget(vert_splitter_);
		CHECK_PTR(py_widget);
		py_widget->startInterpreter();
	#endif

	// ---------------------
	// Scene setup ---------
	// ---------------------

	scene_->registerGLObjectCollector(GL_object_collector_);

	// ---------------------
	// Menus ---------------
	// ---------------------

	// File Menu
	insertMenuEntry(MainControl::FILE, "Export POVRay &file", this, SLOT(exportPOVRay()), 
									CTRL+Key_F, MENU__FILE_EXPORT_POVRAYFILE);

	// DISPLAY Menu
	insertMenuEntry(MainControl::DISPLAY, "Toggle Fullscreen", this, SLOT(toggleFullScreen()),
									ALT+Key_X, MENU__DISPLAY_FULLSCREEN);
	// Build Menu -------------------------------------------------------------------
	insertMenuEntry(MainControl::BUILD, "Assign &Charges", this, SLOT(assignCharges()), 
									CTRL+Key_H, MENU__BUILD_ASSIGN_CHARGES);
	insertMenuEntry(MainControl::BUILD, "Calculate AMBER &Energy", this, SLOT(calculateAmberEnergy()), 
									CTRL+Key_U, MENU__BUILD_AMBER_ENERGY);
	insertMenuEntry(MainControl::BUILD, "Perform Energy &Minimization", this, SLOT(amberMinimization()), 
									CTRL+Key_W, MENU__BUILD_AMBER_MINIMIZATION);
	insertMenuEntry(MainControl::BUILD, "Perform MD &Simulation", this, SLOT(amberMDSimulation()), 
									CTRL+Key_S, MENU__BUILD_AMBER_MDSIMULATION);

#ifdef QT_THREAD_SUPPORT
	insertMenuEntry(MainControl::BUILD, "Stop simulation", this, SLOT(stopSimulation()),ALT+Key_C, MENU__BUILD_STOPSIMULATION);
#endif
  insertMenuEntry(MainControl::DISPLAY, "Contour Surface", this,  SLOT(computeSurface()), 
									CTRL+Key_S,MENU__DISPLAY_OPEN_SURFACE_DIALOG);
	insertMenuEntry(MainControl::BUILD, "Build Peptide", this, SLOT(buildPeptide()));
	// Help-Menu -------------------------------------------------------------------
	insertMenuEntry(MainControl::HELP, "&About", this, SLOT(about()), CTRL+Key_A, MENU__HELP_ABOUT);

	// Menu ------------------------------------------------------------------------
	menuBar()->setSeparator(QMenuBar::InWindowsStyle);

	// ---------------------
	// Connectivity --------
	// ---------------------

	connect(initPopupMenu(MainControl::BUILD), SIGNAL(aboutToShow()), this, SLOT(checkMenuEntries()));

	// check the active menu entries
	checkMenuEntries();

	//--------------------------------
	// setup the VIEW server
	//--------------------------------

  // registering object generator
  MoleculeObjectCreator* object_creator = new MoleculeObjectCreator;
  server_->registerObjectCreator(*object_creator);

	setStatusbarText("Ready.");
}

Mainframe::~Mainframe()
	throw()
{
	List<QPopupMenu *>::Iterator list_it = popup_menus_.begin();

	for (; list_it != popup_menus_.end(); ++list_it)
	{
		delete *list_it;
	}

	popup_menus_.clear();
}

void Mainframe::onNotify(Message *message)
	throw()
{
	MainControl::onNotify(message);
}

void Mainframe::checkMenuEntries()
{
	setCompositesMuteable(!simulation_thread_);

	bool selected;
	int number_of_selected_objects = 0;

	if (control_selection_.size() == 0)
	{
		selected = false;
	}
	else
	{
		number_of_selected_objects = control_selection_.size();
		selected = (number_of_selected_objects > 0);
	}

	bool all_systems = (number_of_selected_objects > 0 && composites_muteable_);

	menuBar()->setItemEnabled(MENU__BUILD_ASSIGN_CHARGES, selected);

	// AMBER methods are available only for single systems
	menuBar()->setItemEnabled(MENU__BUILD_AMBER_ENERGY, 
														(all_systems && (number_of_selected_objects == 1)));

	menuBar()->setItemEnabled(MENU__BUILD_AMBER_MINIMIZATION, 
														(all_systems && (number_of_selected_objects == 1)));

	menuBar()->setItemEnabled(MENU__BUILD_AMBER_MDSIMULATION, 
														(all_systems && (number_of_selected_objects == 1)));

	// enable stopSimulation if simulation is running
	menuBar()->setItemEnabled(MENU__BUILD_STOPSIMULATION, !composites_muteable_);
}

void Mainframe::exportPOVRay()
{
	FileDialog pov("Export POVRay File", QFileDialog::AnyFile, this);
	if (pov.exec())
	{
		POVRenderer pr(pov.getFileName());
		scene_->exportScene(pr);
	}
	removeModularWidget(&pov);	
}
	

void Mainframe::assignCharges()
{
}

void Mainframe::calculateAmberEnergy()
{
	// retrieve the system from the selection
	System* system = getSelectedSystem();
	if (system == 0)
	{
		setStatusbarText("to calculate AMBER energies, one system has to be selected");
		return;
	}

	// set up the AMBER force field
	setStatusbarText("setting up force field...");

	AmberFF amber;
	amber.options[AmberFF::Option::ASSIGN_TYPES] = "true";
	amber.options[AmberFF::Option::ASSIGN_CHARGES] = "true";
	amber.options[AmberFF::Option::ASSIGN_TYPENAMES] = "true";
	amber.options[AmberFF::Option::OVERWRITE_CHARGES] = "true";
	amber.options[AmberFF::Option::OVERWRITE_TYPENAMES] = "true";
  amber.options[AmberFF::Option::DISTANCE_DEPENDENT_DIELECTRIC] 
		= String(minimization_dialog_->getUseDistanceDependentDC());
  amber.options[AmberFF::Option::FILENAME] = String(minimization_dialog_->getFilename());

	if (!amber.setup(*system))
	{
		Log.error() << "Force field setup failed." << std::endl;
		return;
	}

	// calculate the energy
	setStatusbarText("calculating energy...");

	amber.updateEnergy();

	// print the result
	Log.info() << "AMBER Energy:" << endl;
	Log.info() << " - electrostatic     : " << amber.getESEnergy() << " kJ/mol" << endl;
	Log.info() << " - van der Waals     : " << amber.getVdWEnergy() << " kJ/mol" << endl;
	Log.info() << " - bond stretch      : " << amber.getStretchEnergy() << " kJ/mol" << endl;
	Log.info() << " - angle bend        : " << amber.getBendEnergy() << " kJ/mol" << endl;
	Log.info() << " - torsion           : " << amber.getTorsionEnergy() << " kJ/mol" << endl;
	Log.info() << "---------------------------------------" << endl;
	Log.info() << "  total energy       : " << amber.getEnergy() << " kJ/mol" << endl;

	setStatusbarText("Total AMBER energy: %f kJ/mol." + String(amber.getEnergy()));
}

void Mainframe::computeSurface()
{
	System *system = new System();

	// execute the surface dialog
	// and abort if cancel is clicked
	int result = surface_dialog_->exec();
	if (result == 0)
	{
		return;
	}
	RegularData3D rd;
	File f(surface_dialog_->getLoadName());
	f>>rd;
  Log.info()<<surface_dialog_->threshold_->text().toFloat()<<endl;
	ContourSurface cs(surface_dialog_->threshold_->text().toFloat());
  Log.info()<<surface_dialog_->threshold_->text().toFloat()<<endl;
	// Compute contour surface.
	cs << rd; 
	GLMesh *mesh = new GLMesh();
	*static_cast<Surface*>(mesh) = (Surface) cs;
	//mesh->PropertyManager::set(*this);
	mesh->setName(String("TestSurface"));
	system->getRoot().appendChild(*mesh);

	insert(system);

	NewCompositeMessage ncm;
	ncm.setComposite(system);
	notify_(ncm);

	// notify tree of the changes
	ChangedMolecularMessage changed_message; 
	changed_message.setComposite(system);

	notify_(changed_message);
	// calculate center of the new composite
	SceneMessage *scene_message = new SceneMessage;
	GeometricCenterProcessor center;
	system->apply(center);        

	scene_message->setCameraLookAt(center.getCenter());

	Vector3 view_point = center.getCenter();
	view_point.z = view_point.z + 5;
	scene_message->setCameraViewPoint(view_point);

	// notify scene to perform an update and set the camera to the new object
	notify_(scene_message);

	SceneMessage sc;
	sc.updateOnly();
	notify_(sc);

	WindowMessage wm;
	notify_(wm);

	MainControl::update(*system);
	QWidget::update();

}

void Mainframe::amberMinimization()
{
	if (simulation_thread_ != 0)
	{
		Log.error() << "Simulation already running!" << std::endl;
		return;
	}
	// retrieve the system from the selection
	System* system = getSelectedSystem();
	if (system == 0) return;

	// execute the minimization dialog
	// and abort if cancel is clicked
	if (!minimization_dialog_->exec() ||
	    minimization_dialog_->getMaxGradient() == 0 ||
			minimization_dialog_->getEnergyDifference() == 0)
	{
		return;
	}
	
	String filename(minimization_dialog_->getFilename());
	Path path;
	filename = path.find(filename);
	if (filename == "")
	{
		Log.error() << "Invalid filename for amber options" << std::endl;
		return;
	}

	// set up the AMBER force field
	setStatusbarText("setting up force field...");

	AmberFF* amber = new AmberFF;
  amber->options[AmberFF::Option::ASSIGN_TYPES] = "true";
  amber->options[AmberFF::Option::ASSIGN_CHARGES] = "true";
  amber->options[AmberFF::Option::ASSIGN_TYPENAMES] = "true";
  amber->options[AmberFF::Option::OVERWRITE_CHARGES] = "false";
  amber->options[AmberFF::Option::OVERWRITE_TYPENAMES] = "true";
	amber->options[AmberFF::Option::DISTANCE_DEPENDENT_DIELECTRIC] = String(minimization_dialog_->getUseDistanceDependentDC());
	amber->options[AmberFF::Option::FILENAME] = filename;

	if (!amber->setup(*system))
	{
		Log.error() << "Setup of AMBER force field failed." << endl;
		return;
	}

	// calculate the energy
	setStatusbarText("starting minimization...");

	amber->updateEnergy();

	EnergyMinimizer* minimizer;
	if (minimization_dialog_->getUseConjugateGradient())	 minimizer = new ConjugateGradientMinimizer;
	else 																									 minimizer = new SteepestDescentMinimizer;
	
	// set the minimizer options
	minimizer->options[EnergyMinimizer::Option::MAXIMAL_NUMBER_OF_ITERATIONS] = minimization_dialog_->getMaxIterations();
	minimizer->options[EnergyMinimizer::Option::MAX_GRADIENT] = minimization_dialog_->getMaxGradient();
	minimizer->options[EnergyMinimizer::Option::ENERGY_DIFFERENCE_BOUND] = minimization_dialog_->getEnergyDifference();
	minimizer->options[EnergyMinimizer::Option::ENERGY_OUTPUT_FREQUENCY] = 999999999;
	minimizer->setup(*amber);
	minimizer->setMaxNumberOfIterations(minimization_dialog_->getMaxIterations());

	// perform an initial step (no restart step)
	minimizer->minimize(1, false);

	// ============================= WITH MULTITHREADING ==============================================
#ifdef QT_THREAD_SUPPORT
	EnergyMinimizerThread* thread = new EnergyMinimizerThread;
	simulation_thread_ = thread;
	checkMenuEntries();

	thread->setEnergyMinimizer(minimizer);
	thread->setNumberOfStepsBetweenUpdates(minimization_dialog_->getRefresh());
	thread->setMainframe(this);
	thread->start();

#else
	// ============================= WITHOUT MULTITHREADING ===========================================
	// iterate until done and refresh the screen every "steps" iterations
	while (!minimizer->minimize(minimization_dialog_->getRefresh(), true) &&
					minimizer->getNumberOfIterations() < minimizer->getMaxNumberOfIterations())
	{
    MainControl::update(system->getRoot());

		QString message;
		message.sprintf("Iteration %d: energy = %f kJ/mol, RMS gradient = %f kJ/mol A", 
										minimizer->getNumberOfIterations(), amber->getEnergy(), amber->getRMSGradient());
		setStatusbarText(String(message.ascii()));
 	}

	Log.info() << endl << "minimization terminated." << endl << endl;
	printAmberResults(*amber);
	Log.info() << "final RMS gadient    : " << amber->getRMSGradient() << " kJ/(mol A)   after " 
						 << minimizer->getNumberOfIterations() << " iterations" << endl << endl;
	setStatusbarText("Total AMBER energy: " + String(amber->getEnergy()) + " kJ/mol.");
	
	// clean up
	delete minimizer;
	delete amber;
#endif
}

void Mainframe::amberMDSimulation()
{
	if (simulation_thread_ != 0)
	{
		Log.error() << "Simulation already running!" << std::endl;
		return;
	}
	// retrieve the system from the selection
	System* system = getSelectedSystem();

	// execute the MD simulation dialog
	// and abort if cancel is clicked
	if (system == 0 || 
			!md_dialog_->exec() ||
			md_dialog_->getSimulationTime() == 0 ||
			md_dialog_->getTemperature() == 0)
	{
		return;
	}

	String filename(md_dialog_->getFilename());
	Path path;
	filename = path.find(filename);
	if (filename == "")
	{
		Log.error() << "Invalid filename for amber options" << std::endl;
		return;
	}

	// set up the AMBER force field
	setStatusbarText("setting up force field...");

	AmberFF* amber = new AmberFF;
  amber->options[AmberFF::Option::ASSIGN_TYPES] = "true";
  amber->options[AmberFF::Option::ASSIGN_CHARGES] = "true";
  amber->options[AmberFF::Option::ASSIGN_TYPENAMES] = "true";
  amber->options[AmberFF::Option::OVERWRITE_CHARGES] = "false";
  amber->options[AmberFF::Option::OVERWRITE_TYPENAMES] = "true";
	amber->options[AmberFF::Option::DISTANCE_DEPENDENT_DIELECTRIC] = String(md_dialog_->getUseDistanceDependentDC());
	amber->options[AmberFF::Option::FILENAME] = filename;

 	if (!amber->setup(*system))
	{
		Log.error() << "Setup of AMBER force field failed." << endl;
		return;
	}

	// calculate the energy
	setStatusbarText("starting simulation...");

	amber->updateEnergy();

	MolecularDynamics* mds = 0;
	if (md_dialog_->useMicroCanonical()) mds = new CanonicalMD;
	else 																 mds = new MicroCanonicalMD;
	
	// set the options for the MDS	
	Options options;
	options[MolecularDynamics::Option::ENERGY_OUTPUT_FREQUENCY] = 99999999;
	options[MolecularDynamics::Option::TIME_STEP] = md_dialog_->getTimeStep();

	// setup the simulation
	mds->setup(*amber, 0, options);
	if (!mds->isValid())
	{
		Log.error() << "Setup for MD simulation failed!" << std::endl;
		return;
	}
	
	// perform an initial step (no restart step)
	mds->simulateIterations(1, false);

	// we update everything every so and so steps
	Size steps = 2;

	// ============================= WITH MULTITHREADING ==============================================
#ifdef QT_THREAD_SUPPORT
	MDSimulationThread* thread = new MDSimulationThread;
	simulation_thread_ = thread;
	checkMenuEntries();

	thread->setMolecularDynamics(mds);
	thread->setNumberOfSteps(md_dialog_->getNumberOfSteps());
	thread->setNumberOfStepsBetweenUpdates(steps);
	thread->setMainframe(this);
	thread->setSaveImages(md_dialog_->saveImages());
	thread->start();

#else
	// ============================= WITHOUT MULTITHREADING ===========================================
	// iterate until done and refresh the screen every "steps" iterations
	while (mds->getNumberOfIterations() < md_dialog_->getNumberOfSteps())
	{
		mds->simulateIterations(steps, true);
    MainControl::update(system->getRoot());
		if (md_dialog_->saveImages()) 
		{
			Scene* scene= (Scene*) Scene::getInstance(0);
			scene->exportPNG();
		}

		QString message;
		message.sprintf("Iteration %d: energy = %f kJ/mol, RMS gradient = %f kJ/mol A", 
										mds->getNumberOfIterations(), amber->getEnergy(), amber->getRMSGradient());
		setStatusbarText(String(message.ascii()));
 	}

	Log.info() << std::endl << "simulation terminated." << std::endl << endl;
	printAmberResults(*amber);
	Log.info() << "final RMS gadient    : " << amber->getRMSGradient() << " kJ/(mol A)   after " 
						 << mds->getNumberOfIterations() << " iterations" << endl << endl;
	setStatusbarText("Total AMBER energy: " + String(amber->getEnergy()) + " kJ/mol.");

	// clean up
	delete mds;
	delete amber;
#endif
}


void Mainframe::about()
{
	DlgAbout about_box;
	about_box.exec();

	setStatusbarText("MolVIEW V1.0");
}

void Mainframe::fetchPreferences(INIFile& inifile)
	throw()
{
	// 
	// the splitter positions
	//
	QValueList<int> value_list;
	String value_string;
	if (inifile.hasEntry("WINDOWS", "Main::hor_splitter"))
	{
		value_string = inifile.getValue("WINDOWS", "Main::hor_splitter");
		for (Size i = 0; i < value_string.countFields(); i++)
		{
			value_list.append(value_string.getField(i).toInt());
		}
		hor_splitter_->setSizes(value_list);
		value_list.clear();
	}
	if (inifile.hasEntry("WINDOWS", "Main::vert_splitter"))
	{
		value_string = inifile.getValue("WINDOWS", "Main::vert_splitter");
		for (Size i = 0; i < value_string.countFields(); i++)
		{
			value_list.append(value_string.getField(i).toInt());
		}
		vert_splitter_->setSizes(value_list);
	}

	MainControl::fetchPreferences(inifile);
}

void Mainframe::writePreferences(INIFile& inifile)
	throw()
{
	// the splitter positions
	QValueList<int> size_list = hor_splitter_->sizes();
	String value_string = "";
	QValueListConstIterator<int> list_it = size_list.begin();
	for (; list_it != size_list.end(); ++list_it)
	{
		value_string += String(*list_it) + " ";
	}
	inifile.insertValue("WINDOWS", "Main::hor_splitter", value_string);

	value_string = "";
	size_list = vert_splitter_->sizes();
	list_it = size_list.begin();
	for (; list_it != size_list.end(); ++list_it)
	{
		value_string += String(*list_it) + " ";
	}
	inifile.insertValue("WINDOWS", "Main::vert_splitter", value_string);

	minimization_dialog_->writePreferences(inifile);

	MainControl::writePreferences(inifile);
}

void Mainframe::buildPeptide()
{
	PeptideDialog* dialog = new PeptideDialog;
	dialog->exec();

	Protein* protein = dialog->getProtein();
	if (protein == 0) return;

	System* system = new System;
	system->insert(*protein);
	insert(*system);

	// repaint of the scene and the composites needed
	for (Position p = 0; ; p++)
	{
		Scene* scene= (Scene*) Scene::getInstance(p);
		if (scene == 0) return;
		scene->update(true);
	}
}

void Mainframe::toggleFullScreen()
{
	if (!fullscreen_)
	{
		// This call is needed because showFullScreen won't work
		// correctly if the widget already considers itself to be fullscreen.
		showNormal();	
		showFullScreen();
		setGeometry(qApp->desktop()->screenGeometry());

		logview_->hide();

		List<ModularWidget*>::Iterator it = modular_widgets_.begin();
		for( ; it != modular_widgets_.end(); it++)
		{
			if (!RTTI::isKindOf<QWidget>(**it))
			{
				continue;
			}

			dynamic_cast<QWidget*>(*it)->hide();
		}
		scene_->show();
	
		fullscreen_ = true;
		return;
	}

	showNormal();

	List<ModularWidget*>::Iterator it = modular_widgets_.begin();
	for( ; it != modular_widgets_.end(); it++)
	{
		if (!RTTI::isKindOf<Scene>(**it) &&
				 RTTI::isKindOf<QWidget>(**it) &&
				!RTTI::isKindOf<QDialog>(**it))
		{	
			dynamic_cast<QWidget*>(*it)->show();
		}
	}
	
	logview_->show();
	fullscreen_ = false;
}


void Mainframe::stopSimulation() 
{
#ifdef QT_THREAD_SUPPORT
	stop_simulation_ = true;
	if (simulation_thread_ != 0)
	{
		if (simulation_thread_->running()) simulation_thread_->wait();
		delete simulation_thread_;
		simulation_thread_ = 0;
	}

	Log.insert(std::cout);
	Log.insert(std::cerr);
//	setStatusbarText("Simulation finished.");
	stop_simulation_ = false;
	checkMenuEntries();
#endif
}

void Mainframe::printAmberResults(const AmberFF& amber)
	throw()
{
	Log.info() << endl;
	Log.info() << "AMBER Energy:" << endl;
	Log.info() << " - electrostatic     : " << amber.getESEnergy() << " kJ/mol" << endl;
	Log.info() << " - van der Waals     : " << amber.getVdWEnergy() << " kJ/mol" << endl;
	Log.info() << " - bond stretch      : " << amber.getStretchEnergy() << " kJ/mol" << endl;
	Log.info() << " - angle bend        : " << amber.getBendEnergy() << " kJ/mol" << endl;
	Log.info() << " - torsion           : " << amber.getTorsionEnergy() << " kJ/mol" << endl;
	Log.info() << "---------------------------------------" << endl;
	Log.info() << "  total energy       : " << amber.getEnergy() << " kJ/mol" << endl;
}

void Mainframe::customEvent( QCustomEvent * e )
{
	if ( e->type() == 65431 )   // It must be a SimulationThreadFinished
	{
		stopSimulation();
		return;
	}
	if ( e->type() == 65430 )   // It must be a SimulationThreadFinished
	{
		SimulationOutput* so = (SimulationOutput*) e;
		Log.info() << so->getMessage() << std::endl;
		return;
	}
}

void Mainframe::openFile(const String& file)
	throw()
{
	file_dialog_->openFile(file);
}


#ifdef BALL_NO_INLINE_FUNCTIONS
#	include "mainframe.iC"
#endif

} // namespace
