#include "mainframe.h"
#include "icons.h"
#include <qcolordialog.h>
#include <qfiledialog.h>
#include <qlabel.h>
#include <qtooltip.h>
#include <qfile.h>
#include "DIALOGS/DlgAbout.h"

#include <BALL/MOLMEC/AMBER/amber.h>
#include <BALL/MOLMEC/MINIMIZATION/conjugateGradient.h>
#include <BALL/MOLMEC/MINIMIZATION/steepestDescent.h>
#include <BALL/MOLMEC/MDSIMULATION/canonicalMD.h>
#include <BALL/MOLMEC/MDSIMULATION/molecularDynamics.h>
#include <BALL/MOLVIEW/GUI/DIALOGS/openHINFile.h>
#include <BALL/MOLVIEW/GUI/DIALOGS/openMOL2File.h>
#include <BALL/MOLVIEW/GUI/DIALOGS/openPDBFile.h>

#include <BALL/MOLVIEW/GUI/KERNEL/moleculeObjectCreator.h>

#ifdef BALL_PYTHON_SUPPORT
#	include <BALL/VIEW/GUI/WIDGETS/pyWidget.h>
#endif

using namespace std;


Mainframe::Mainframe
	(QWidget* parent, const char* name)
	:	MainControl(parent, name, ".options"),
		scene_(0),
		control_(0),
		display_properties_(0),
		minimization_dialog_(0),
		label_properties_(0),
		molecular_properties_(0),
		server_(0),
		GL_object_collector_(),
		fragment_db_(),
		hor_splitter_(0),
		vert_splitter_(0),
		logview_(0),
		vboxlayout_(0),
		popup_menus_(),
		selection_(),
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
  // ???
	//Log.remove(std::cout);

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

	label_properties_ = new LabelProperties(this);
	CHECK_PTR(label_properties_);

	CHECK_PTR(new OpenHINFile(this));
	CHECK_PTR(new OpenMOL2File(this));
	CHECK_PTR(new OpenPDBFile(this));

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

	QLabel* message_label = new QLabel(tr("Ready."), statusBar());
	statusBar()->addWidget(message_label, 20);

	// ---------------------
	// Scene setup ---------
	// ---------------------

	scene_->registerGLObjectCollector(GL_object_collector_);

	// ---------------------
<<<<<<< mainframe.C
	// PyWidget setup
	// ---------------------
	#ifdef BALL_PYTHON_SUPPORT
	//	py_widget->setFont(f);
	//	py_widget->startInterpreter();
	#endif
	

	// ---------------------
=======
>>>>>>> 1.39
	// Menus ---------------
	// ---------------------

	// Build Menu -------------------------------------------------------------------
	insertMenuEntry(MainControl::BUILD, "Check St&ructure", this, SLOT(checkResidue()), CTRL+Key_R, MENU__BUILD_CHECK_RESIDUE);


	insertMenuEntry(MainControl::BUILD, "Assign &Charges", this, SLOT(assignCharges()), CTRL+Key_H, MENU__BUILD_ASSIGN_CHARGES);
	insertMenuEntry(MainControl::BUILD, "Calculate AMBER &Energy", this, SLOT(calculateAmberEnergy()), CTRL+Key_U, MENU__BUILD_AMBER_ENERGY);
	insertMenuEntry(MainControl::BUILD, "Perform Energy &Minimization", this, SLOT(amberMinimization()), CTRL+Key_W, MENU__BUILD_AMBER_MINIMIZATION);
	insertMenuEntry(MainControl::BUILD, "Perform MD &Simulation", this, SLOT(amberMDSimulation()), CTRL+Key_S, MENU__BUILD_AMBER_MDSIMULATION);
			
	// Help-Menu -------------------------------------------------------------------

	insertMenuEntry(MainControl::HELP, "&About", this, SLOT(about()), CTRL+Key_A, MENU__HELP_ABOUT);

	// Menu ------------------------------------------------------------------------

	menuBar()->setSeparator(QMenuBar::InWindowsStyle);

	// ---------------------
	// Connectivity --------
	// ---------------------

	connect(initPopupMenu(MainControl::BUILD),
					SIGNAL(aboutToShow()),
					this,
					SLOT(checkMenuEntries()));

	// check the active menu entries
	checkMenuEntries();

	//--------------------------------
	// setup the VIEW server
	//--------------------------------

  // registering object generator
  MoleculeObjectCreator* object_creator = new MoleculeObjectCreator;
  server_->registerObjectCreator(*object_creator);
}

Mainframe::~Mainframe()
	throw()
{
	//
	// clean up
	//
	List<QPopupMenu *>::Iterator list_it;

	for (list_it = popup_menus_.begin();
			 list_it != popup_menus_.end();
			 ++list_it)
	{
		delete *list_it;
	}

	popup_menus_.clear();
}

void Mainframe::onNotify(Message *message)
	throw()
{
	if (message == 0)
	{
		return;
	}

	// selection => store last selection for later processing
	if (RTTI::isKindOf<SelectionMessage>(*message))
	{
		SelectionMessage *selection = RTTI::castTo<SelectionMessage>(*message);

		selection_ = selection->getSelection();
	}
 
	MainControl::onNotify(message);
}

void Mainframe::checkMenuEntries()
{
	bool selected;
	int number_of_selected_objects = 0;

	if (selection_.empty())
	{
		selected = false;
	}
	else
	{
		number_of_selected_objects = selection_.size();
		selected = (number_of_selected_objects > 0);
	}

	bool all_systems = (number_of_selected_objects > 0);

	menuBar()->setItemEnabled(MENU__BUILD_CHECK_RESIDUE, selected);
	menuBar()->setItemEnabled(MENU__BUILD_ASSIGN_CHARGES, selected);

	// AMBER methods are available only for single systems
	menuBar()->setItemEnabled(MENU__BUILD_AMBER_ENERGY, 
														(all_systems && (number_of_selected_objects == 1)));

	menuBar()->setItemEnabled(MENU__BUILD_AMBER_MINIMIZATION, 
														(all_systems && (number_of_selected_objects == 1)));

	menuBar()->setItemEnabled(MENU__BUILD_AMBER_MDSIMULATION, 
														(all_systems && (number_of_selected_objects == 1)));
}

void Mainframe::checkResidue()
{
	if (selection_.size() == 0)
	{
		return;
	}

	QString message;
	message.sprintf("selecting %d objects...", selection_.size());
	statusBar()->message(message);
	QWidget::update();

	ResidueChecker res_check(fragment_db_);
	bool okay = true;
	List<Composite*>::ConstIterator list_it = selection_.begin();	
	for (; list_it != selection_.end(); ++list_it)
	{	
		(*list_it)->apply(res_check);
		okay = okay && res_check.getStatus();	
	}

	if (okay)
	{
		Log.info() << "ResidueChecker: no errors found." << endl;
		statusBar()->message("no errors.");
	} 
	else 
	{
		statusBar()->message("errors found!");
	}

	QWidget::update();
}

void Mainframe::assignCharges()
{
}

void Mainframe::calculateAmberEnergy()
{
	if (selection_.size() != 1)
	{
		return;
	}

	if (!RTTI::isKindOf<System>(*selection_.front()))
	{
		return;
	}

	// retrieve the system from the selection
	System& system = *RTTI::castTo<System>(*selection_.front());

	// set up the AMBER force field
	statusBar()->message("setting up force field...");
	QWidget::update();

	AmberFF amber;
	amber.options[AmberFF::Option::ASSIGN_TYPES] = "true";
	amber.options[AmberFF::Option::ASSIGN_CHARGES] = "true";
	amber.options[AmberFF::Option::ASSIGN_TYPENAMES] = "true";
	amber.options[AmberFF::Option::OVERWRITE_CHARGES] = "true";
	amber.options[AmberFF::Option::OVERWRITE_TYPENAMES] = "true";
  amber.options[AmberFF::Option::DISTANCE_DEPENDENT_DIELECTRIC] = String(minimization_dialog_->getUseDistanceDependentDC());
  amber.options[AmberFF::Option::FILENAME] = String(minimization_dialog_->getFilename());

	if (!amber.setup(system))
	{
		Log.error() << "Force field setup failed." << std::endl;
		return;
	}

	// calculate the energy
	statusBar()->message("calculating energy...");
	QWidget::update();

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
	QString message;
	message.sprintf("Total AMBER energy: %f kJ/mol.", amber.getEnergy());

	statusBar()->message(message, 5000);
	QWidget::update();
}

void Mainframe::amberMinimization()
{
	if (selection_.size() == 0)
	{
		return;
	}

	// retrieve the system from the selection
	System& system = *RTTI::castTo<System>(*selection_.front());


	// execute the minimization dialog
	// and abort if cancel is clicked
	int result = minimization_dialog_->exec();
	if (result == 0)
	{
		return;
	}
	
	// set up the AMBER force field
	statusBar()->message("setting up force field...");
	statusBar()->update();
	QWidget::update();
	AmberFF amber;
  amber.options[AmberFF::Option::ASSIGN_TYPES] = "true";
  amber.options[AmberFF::Option::ASSIGN_CHARGES] = "true";
  amber.options[AmberFF::Option::ASSIGN_TYPENAMES] = "true";
  amber.options[AmberFF::Option::OVERWRITE_CHARGES] = "false";
  amber.options[AmberFF::Option::OVERWRITE_TYPENAMES] = "true";
	amber.options[AmberFF::Option::DISTANCE_DEPENDENT_DIELECTRIC] = String(minimization_dialog_->getUseDistanceDependentDC());
	amber.options[AmberFF::Option::FILENAME] = String(minimization_dialog_->getFilename());

 	if (!amber.setup(system))
	{
		Log.error() << "Setup of AMBER force field failed." << endl;
		return;
	}

	// calculate the energy
	statusBar()->message("starting minimization...");
	statusBar()->update();
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

	//
	// iterate until done and refresh the screen every "steps" iterations
	// 
	while (!minimizer->minimize(steps, true))
	{
    MainControl::update(system.getRoot());

		QString message;
		message.sprintf("Iteration %d: energy = %f kJ/mol, RMS gradient = %f kJ/mol A", 
										minimizer->getNumberOfIteration(),
										amber.getEnergy(),
										amber.getRMSGradient());
		// update scene
		SceneMessage scene_message;
		scene_message.updateOnly();
		notify_(scene_message);
		QWidget::update();
		statusBar()->message(message);
		statusBar()->update();
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

	QString message;
	message.sprintf("Total AMBER energy: %f kJ/mol.", amber.getEnergy());
	statusBar()->message(message, 5000);
}

void Mainframe::amberMDSimulation()
{
	if (selection_.size() == 0)
	{
		return;
	}

	// retrieve the system from the selection
	System& system = *RTTI::castTo<System>(*selection_.front());


	// execute the MD simulation dialog
	// and abort if cancel is clicked
	//int result = minimization_dialog_->exec();
	//if (result == 0)
	//{
	//	return;
	//}
	
	// set up the AMBER force field
	statusBar()->message("setting up force field...");
	statusBar()->update();
	QWidget::update();
	AmberFF amber;
  amber.options[AmberFF::Option::ASSIGN_TYPES] = "true";
  amber.options[AmberFF::Option::ASSIGN_CHARGES] = "true";
  amber.options[AmberFF::Option::ASSIGN_TYPENAMES] = "true";
  amber.options[AmberFF::Option::OVERWRITE_CHARGES] = "false";
  amber.options[AmberFF::Option::OVERWRITE_TYPENAMES] = "true";
	// amber.options[AmberFF::Option::DISTANCE_DEPENDENT_DIELECTRIC] = String(minimization_dialog_->getUseDistanceDependentDC());
	// amber.options[AmberFF::Option::FILENAME] = String(minimization_dialog_->getFilename());

 	if (!amber.setup(system))
	{
		Log.error() << "Setup of AMBER force field failed." << endl;
		return;
	}

	// calculate the energy
	statusBar()->message("starting simulation...");
	statusBar()->update();
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
    MainControl::update(system.getRoot());

		QString message;
		message.sprintf("Iteration %d: energy = %f kJ/mol, RMS gradient = %f kJ/mol A", 
										mds->getNumberOfIteration(),
										amber.getEnergy(),
										amber.getRMSGradient());
		// update scene
		SceneMessage scene_message;
		scene_message.updateOnly();
		notify_(scene_message);
		QWidget::update();
		statusBar()->message(message);
		statusBar()->update();
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

	QString message;
	message.sprintf("Total AMBER energy: %f kJ/mol.", amber.getEnergy());
	statusBar()->message(message, 5000);
}

void Mainframe::about()
{
	DlgAbout about_box;
	about_box.exec();
	statusBar()->message("MolVIEW V1.0", 1500);
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
	// 
	QValueList<int> size_list = hor_splitter_->sizes();
	String value_string = "";
	QValueListConstIterator<int> list_it = size_list.begin();
	for (; list_it != size_list.end(); ++list_it)
	{
		value_string += String(*list_it) + " ";
	}
	inifile.setValue("WINDOWS", "Main::hor_splitter", value_string);

	value_string = "";
	size_list = vert_splitter_->sizes();
	list_it = size_list.begin();
	for (; list_it != size_list.end(); ++list_it)
	{
		value_string += String(*list_it) + " ";
	}
	inifile.setValue("WINDOWS", "Main::vert_splitter", value_string);

	MainControl::writePreferences(inifile);
}


#ifdef BALL_NO_INLINE_FUNCTIONS
#	include "mainframe.iC"
#endif
