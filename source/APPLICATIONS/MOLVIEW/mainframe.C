// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include "mainframe.h"
#include "icons.h"
#include "DIALOGS/DlgAbout.h"

#include <BALL/MOLMEC/AMBER/amber.h>
#include <BALL/MOLMEC/MINIMIZATION/conjugateGradient.h>
#include <BALL/MOLMEC/MINIMIZATION/steepestDescent.h>
#include <BALL/MOLMEC/MDSIMULATION/canonicalMD.h>
#include <BALL/MOLMEC/MDSIMULATION/molecularDynamics.h>

#include <BALL/VIEW/GUI/DIALOGS/fileDialog.h>
#include <BALL/MOLVIEW/GUI/KERNEL/moleculeObjectCreator.h>
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


using namespace std;


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
		tool_box_(0)
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

	control_ = new MolecularControl(hor_splitter_);
	CHECK_PTR(control_);

	scene_ = new Scene(hor_splitter_);
	CHECK_PTR(scene_);

	display_properties_ = new DisplayProperties(this);
	CHECK_PTR(display_properties_);

	minimization_dialog_ = new DlgAmberMinimization(this);
	CHECK_PTR(minimization_dialog_);

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

	// Build Menu -------------------------------------------------------------------
	insertMenuEntry(MainControl::BUILD, "Assign &Charges", this, SLOT(assignCharges()), 
									CTRL+Key_H, MENU__BUILD_ASSIGN_CHARGES);
	insertMenuEntry(MainControl::BUILD, "Calculate AMBER &Energy", this, SLOT(calculateAmberEnergy()), 
									CTRL+Key_U, MENU__BUILD_AMBER_ENERGY);
	insertMenuEntry(MainControl::BUILD, "Perform Energy &Minimization", this, SLOT(amberMinimization()), 
									CTRL+Key_W, MENU__BUILD_AMBER_MINIMIZATION);
	insertMenuEntry(MainControl::BUILD, "Perform MD &Simulation", this, SLOT(amberMDSimulation()), 
									CTRL+Key_S, MENU__BUILD_AMBER_MDSIMULATION);
  insertMenuEntry(MainControl::DISPLAY, "Contour Surface", this,  SLOT(computeSurface()), 
									CTRL+Key_S,MENU__DISPLAY_OPEN_SURFACE_DIALOG);
			
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

	bool all_systems = (number_of_selected_objects > 0);

	menuBar()->setItemEnabled(MENU__BUILD_ASSIGN_CHARGES, selected);

	// AMBER methods are available only for single systems
	menuBar()->setItemEnabled(MENU__BUILD_AMBER_ENERGY, 
														(all_systems && (number_of_selected_objects == 1)));

	menuBar()->setItemEnabled(MENU__BUILD_AMBER_MINIMIZATION, 
														(all_systems && (number_of_selected_objects == 1)));

	menuBar()->setItemEnabled(MENU__BUILD_AMBER_MDSIMULATION, 
														(all_systems && (number_of_selected_objects == 1)));
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
	cs.createContourSurface(rd); 
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
	// retrieve the system from the selection
	System* system = getSelectedSystem();
	if (system == 0) return;

	// execute the minimization dialog
	// and abort if cancel is clicked
	int result = minimization_dialog_->exec();
	if (result == 0)
	{
		return;
	}
	
	// set up the AMBER force field
	setStatusbarText("setting up force field...");

	AmberFF amber;
  amber.options[AmberFF::Option::ASSIGN_TYPES] = "true";
  amber.options[AmberFF::Option::ASSIGN_CHARGES] = "true";
  amber.options[AmberFF::Option::ASSIGN_TYPENAMES] = "true";
  amber.options[AmberFF::Option::OVERWRITE_CHARGES] = "false";
  amber.options[AmberFF::Option::OVERWRITE_TYPENAMES] = "true";
	amber.options[AmberFF::Option::DISTANCE_DEPENDENT_DIELECTRIC] = String(minimization_dialog_->getUseDistanceDependentDC());
	amber.options[AmberFF::Option::FILENAME] = String(minimization_dialog_->getFilename());

 	if (!amber.setup(*system))
	{
		Log.error() << "Setup of AMBER force field failed." << endl;
		return;
	}

	// calculate the energy
	setStatusbarText("starting minimization...");

	amber.updateEnergy();

	EnergyMinimizer* minimizer;
	if (minimization_dialog_->getUseConjugateGradient())
	{
		// create a new CG minimizer object
		minimizer = new ConjugateGradientMinimizer;
	} 
	else 
	{
		// create a new SD minimizer object
		minimizer = new SteepestDescentMinimizer;
	}
	
	// set the minimizer options
	minimizer->options[EnergyMinimizer::Option::MAXIMAL_NUMBER_OF_ITERATIONS] = minimization_dialog_->getMaxIterations();
	minimizer->options[EnergyMinimizer::Option::MAX_GRADIENT] = minimization_dialog_->getMaxGradient();
	minimizer->options[EnergyMinimizer::Option::ENERGY_DIFFERENCE_BOUND] = minimization_dialog_->getEnergyDifference();
	minimizer->options[EnergyMinimizer::Option::ENERGY_OUTPUT_FREQUENCY] = minimization_dialog_->getRefresh();
	minimizer->setup(amber);
	
	
	// perform an initial step (no restart step)
	minimizer->minimize(1, false);

	// we update everything every so and so steps
	Size steps = minimization_dialog_->getRefresh();

	// iterate until done and refresh the screen every "steps" iterations
	while (!minimizer->minimize(steps, true))
	{
    MainControl::update(system->getRoot());

		// update scene
		SceneMessage scene_message;
		scene_message.updateOnly();
		notify_(scene_message);

		QString message;
		message.sprintf("Iteration %d: energy = %f kJ/mol, RMS gradient = %f kJ/mol A", 
										minimizer->getNumberOfIteration(),
										amber.getEnergy(),
										amber.getRMSGradient());

		setStatusbarText(String(message));
 	}

	Log.info() << endl << "minimization terminated." << endl << endl;

	// print the result
	Log.info() << "AMBER Energy:" << endl;
	Log.info() << " - electrostatic     : " << amber.getESEnergy() << " kJ/mol" << endl;
	Log.info() << " - van der Waals     : " << amber.getVdWEnergy() << " kJ/mol" << endl;
	Log.info() << " - bond stretch      : " << amber.getStretchEnergy() << " kJ/mol" << endl;
	Log.info() << " - angle bend        : " << amber.getBendEnergy() << " kJ/mol" << endl;
	Log.info() << " - torsion           : " << amber.getTorsionEnergy() << " kJ/mol" << endl;
	Log.info() << "---------------------------------------" << endl;
	Log.info() << "  total energy       : " << amber.getEnergy() << " kJ/mol" << endl;
	Log.info() << endl;
	Log.info() << "final RMS gadient    : " << amber.getRMSGradient() << " kJ/(mol A)   after " 
		<< minimizer->getNumberOfIteration() << " iterations" << endl;
	
	// clean up
	delete minimizer;

	setStatusbarText("Total AMBER energy: %f kJ/mol." + String(amber.getEnergy()));
}

void Mainframe::amberMDSimulation()
{
	// retrieve the system from the selection
	System* system = getSelectedSystem();
	if (system == 0) return;

	// execute the MD simulation dialog
	// and abort if cancel is clicked
	//int result = minimization_dialog_->exec();
	//if (result == 0)
	//{
	//	return;
	//}
	
	// set up the AMBER force field
	setStatusbarText("setting up force field...");

	AmberFF amber;
  amber.options[AmberFF::Option::ASSIGN_TYPES] = "true";
  amber.options[AmberFF::Option::ASSIGN_CHARGES] = "true";
  amber.options[AmberFF::Option::ASSIGN_TYPENAMES] = "true";
  amber.options[AmberFF::Option::OVERWRITE_CHARGES] = "false";
  amber.options[AmberFF::Option::OVERWRITE_TYPENAMES] = "true";
	// amber.options[AmberFF::Option::DISTANCE_DEPENDENT_DIELECTRIC] = String(minimization_dialog_->getUseDistanceDependentDC());
	// amber.options[AmberFF::Option::FILENAME] = String(minimization_dialog_->getFilename());

 	if (!amber.setup(*system))
	{
		Log.error() << "Setup of AMBER force field failed." << endl;
		return;
	}

	// calculate the energy
	setStatusbarText("starting simulation...");

	amber.updateEnergy();

	MolecularDynamics* mds = new CanonicalMD;
	
	// set the options for the MDS	
	Options options;
	options[MolecularDynamics::Option::ENERGY_OUTPUT_FREQUENCY] = 100;
	options[MolecularDynamics::Option::TIME_STEP] = 0.001;

	// setup the simulation
	mds->setup(amber, 0, options);
	if (!mds->isValid())
	{
		Log.error() << "Setup for MD simulation failed!" << std::endl;
		return;
	}
	
	// perform an initial step (no restart step)
	mds->simulateIterations(1, false);

	// we update everything every so and so steps
	Size steps = 2;

	//
	// iterate until done and refresh the screen every "steps" iterations
	// 
	while (mds->getNumberOfIteration() < 500)
	{
		mds->simulateIterations(steps, true);
    MainControl::update(system->getRoot());

		// update scene
		SceneMessage scene_message;
		scene_message.updateOnly();
		notify_(scene_message);

		QString message;
		message.sprintf("Iteration %d: energy = %f kJ/mol, RMS gradient = %f kJ/mol A", 
										mds->getNumberOfIteration(),
										amber.getEnergy(),
										amber.getRMSGradient());
		setStatusbarText(String(message));
 	}
	Log.info() << std::endl << "simulation terminated." << std::endl << endl;

	// print the result
	Log.info() << "AMBER Energy:" << endl;
	Log.info() << " - electrostatic     : " << amber.getESEnergy() << " kJ/mol" << endl;
	Log.info() << " - van der Waals     : " << amber.getVdWEnergy() << " kJ/mol" << endl;
	Log.info() << " - bond stretch      : " << amber.getStretchEnergy() << " kJ/mol" << endl;
	Log.info() << " - angle bend        : " << amber.getBendEnergy() << " kJ/mol" << endl;
	Log.info() << " - torsion           : " << amber.getTorsionEnergy() << " kJ/mol" << endl;
	Log.info() << "---------------------------------------" << endl;
	Log.info() << "  total energy       : " << amber.getEnergy() << " kJ/mol" << endl;
	Log.info() << endl;
	Log.info() << "final RMS gadient    : " << amber.getRMSGradient() << " kJ/(mol A)   after " 
						 << mds->getNumberOfIteration() << " iterations" << endl;

	// clean up
	delete mds;

	setStatusbarText("Total AMBER energy: " + String(amber.getEnergy()) + " kj/mol.");
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


#ifdef BALL_NO_INLINE_FUNCTIONS
#	include "mainframe.iC"
#endif
