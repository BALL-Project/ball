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
		preferences_dialog_(0),
		minimization_dialog_(0),
		label_properties_(0),
		open_hin_file_(0),
		open_pdb_file_(0),
		molecular_properties_(0),
		server_(0),
		geometric_convertor_(0),
		GL_object_collector_(),
		object_processor_(),
		fragment_db_(),
		hor_splitter_(0),
		vert_splitter_(0),
		logview_(0),
		vboxlayout_(0),
		popup_menus_(),
		selection_(),
		copy_list_(),
		server_icon_(0),
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

	preferences_dialog_ = new DlgPreferences(this);
	CHECK_PTR(preferences_dialog_);

	minimization_dialog_ = new DlgAmberMinimization(this);
	CHECK_PTR(minimization_dialog_);

	label_properties_ = new LabelProperties(this);
	CHECK_PTR(label_properties_);

	open_hin_file_ = new OpenHINFile(this);
	CHECK_PTR(open_hin_file_);

	open_pdb_file_ = new OpenPDBFile(this);
	CHECK_PTR(open_pdb_file_);

	molecular_properties_ = new MolecularProperties();
	CHECK_PTR(molecular_properties_);

	server_ = new Server();
	CHECK_PTR(server_);

	geometric_convertor_ = new GeometricConvertor();
	CHECK_PTR(geometric_convertor_);

	logview_ = new LogView(vert_splitter_);
	CHECK_PTR(logview_);

	#ifdef BALL_PYTHON_SUPPORT
		PyWidget* py_widget = new PyWidget(vert_splitter_);
		CHECK_PTR(py_widget);
	#endif

	QLabel* message_label = new QLabel(tr("Ready."), statusBar());
	statusBar()->addWidget(message_label, 20);

	server_icon_ = new QLabel(statusBar());
	statusBar()->addWidget(server_icon_, 1, TRUE );
	QToolTip::add(server_icon_, "VIEW server status");
	QPixmap icon(mini_ray_xpm);

	server_icon_->setPixmap(icon);
	server_icon_->show();
 
	// ---------------------
	// read preferences ----
	// ---------------------
	preferences_.setFilename(".molview.ini");
	preferences_.read();
	getPreferences(preferences_);	

	// ---------------------
	// Scene setup ---------
	// ---------------------

	scene_->registerGLObjectCollector(GL_object_collector_);

	// ---------------------
	// Dialogs setup ---------
	// ---------------------

	display_properties_->registerObjectProcessor(object_processor_);
	display_properties_->getPreferences(preferences_);
	preferences_dialog_->getPreferences(preferences_);
	minimization_dialog_->getPreferences(preferences_);
	label_properties_->getPreferences(preferences_);

	// ---------------------
	// Control setup -------
	// ---------------------

	control_->addColumn("Name");
	control_->addColumn("Type");
	control_->setColumnWidth(0, 80);
	control_->setColumnWidth(1, 60);

 	// ---------------------
	// molecularProperties setup -------
	// ---------------------

	molecular_properties_->registerFragmentDB(fragment_db_);

	// ---------------------
	// LogView setup ------
	// ---------------------
	
	QFont f("Courier", 12, QFont::DemiBold, false);
	logview_->setFont(f);
	Log.info() << "Welcome to MolVIEW." << endl;


	// ---------------------
	// PyWidget setup
	// ---------------------
	#ifdef BALL_PYTHON_SUPPORT
		py_widget->setFont(f);
		py_widget->startInterpreter();
	#endif
	

	// ---------------------
	// Menu ----------------
	// ---------------------

	// File-Menu -------------------------------------------------------------------

	insertMenuEntry(MainControl::FILE, "E&xit", qApp, SLOT(quit()), CTRL+Key_X);

	// Edit-Menu -------------------------------------------------------------------
	
	insertMenuEntry(MainControl::EDIT, "&Select", this, SLOT(select()), CTRL+Key_S, MENU__EDIT_SELECT);
	insertMenuEntry(MainControl::EDIT, "&Deselect", this, SLOT(deselect()), CTRL+Key_D, MENU__EDIT_DESELECT);

	// Build Menu -------------------------------------------------------------------

	insertMenuEntry(MainControl::BUILD, "Check St&ructure", this, SLOT(checkResidue()), CTRL+Key_R, MENU__BUILD_CHECK_RESIDUE);
	insertMenuEntry(MainControl::BUILD, "&Build Bonds", this, SLOT(buildBonds()), CTRL+Key_B, MENU__BUILD_BUILD_BONDS);
	insertMenuEntry(MainControl::BUILD, "Add &Hydrogens", this, SLOT(addHydrogens()), CTRL+Key_H, MENU__BUILD_ADD_HYDROGENS);
	insertMenuEntry(MainControl::BUILD, "Assign &Charges", this, SLOT(assignCharges()), CTRL+Key_H, MENU__BUILD_ASSIGN_CHARGES);
	insertMenuEntry(MainControl::BUILD, "Calculate AMBER &Energy", this, SLOT(calculateAmberEnergy()), CTRL+Key_U, MENU__BUILD_AMBER_ENERGY);
	insertMenuEntry(MainControl::BUILD, "Perform Energy &Minimization", this, SLOT(amberMinimization()), CTRL+Key_W, MENU__BUILD_AMBER_MINIMIZATION);
			

  // Display Menu -------------------------------------------------------------------
	insertMenuEntry(MainControl::DISPLAY, "&Preferences", this, 
									SLOT(openPreferencesDialog()), CTRL+Key_I, MENU__DISPLAY_OPEN_PREFERENCES_DIALOG);
	insertMenuEntry(MainControl::DISPLAY, "Focus C&amera", this, SLOT(centerCamera()), CTRL+Key_A, MENU__DISPLAY_CENTER_CAMERA);

	// Help-Menu -------------------------------------------------------------------

	insertMenuEntry(MainControl::HELP, "&About", this, SLOT(about()), CTRL+Key_A, MENU__HELP_ABOUT);

	// Menu ------------------------------------------------------------------------

	menuBar()->setSeparator(QMenuBar::InWindowsStyle);

	// ---------------------
	// Connectivity --------
	// ---------------------

	connect(initPopupMenu(MainControl::DISPLAY),
					SIGNAL(aboutToShow()),
					this,
					SLOT(checkMenuEntries()));
	connect(initPopupMenu(MainControl::BUILD),
					SIGNAL(aboutToShow()),
					this,
					SLOT(checkMenuEntries()));
	connect(initPopupMenu(MainControl::EDIT),
					SIGNAL(aboutToShow()),
					this,
					SLOT(checkMenuEntries()));
	connect(preferences_dialog_,
					SIGNAL(applyButtonPressed()),
					this,
					SLOT(applyPreferencesDialog()));

	// check the active menu entries
	checkMenuEntries();

	//--------------------------------
	// setup the VIEW server
	//--------------------------------

  // registering object generator
  MoleculeObjectCreator* object_creator = new MoleculeObjectCreator;
  server_->registerObjectCreator(*object_creator);

	// check whether we should start the server
	// and on which port
	checkServer();

	// building internal connection ------------------------------------------------
	registerConnectionObject(*molecular_properties_);
	registerConnectionObject(*server_);
	registerConnectionObject(*geometric_convertor_);
}

Mainframe::~Mainframe()
{
	//
	// extract preferences 
	// from the current settings
	//
	setPreferences(preferences_);
	display_properties_->setPreferences(preferences_);
	preferences_dialog_->setPreferences(preferences_);
	minimization_dialog_->setPreferences(preferences_);
	label_properties_->setPreferences(preferences_);

	//
	// write the preferences
	//
	preferences_.write();

	//
	// clean up
	//
	List<QPopupMenu *>::Iterator iterator__List;

	for (iterator__List = popup_menus_.begin();
			 iterator__List != popup_menus_.end();
			 ++iterator__List)
	{
		delete *iterator__List;
	}

	popup_menus_.clear();
}

void Mainframe::onNotify(Message *message)
{
	if (message == 0)
	{
		return;
	}

	if (RTTI::isKindOf<WindowMessage>(*message))
	{
		WindowMessage *window_message = RTTI::castTo<WindowMessage>(*message);

		statusBar()->message(window_message->getStatusBar().c_str());

		QWidget::update();
	}
	// selection => store last selection for later processing
	else if (RTTI::isKindOf<SelectionMessage>(*message))
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

	// enable for multiple selection
	menuBar()->setItemEnabled(MENU__EDIT_SELECT, selected);
	menuBar()->setItemEnabled(MENU__EDIT_DESELECT, selected);
	menuBar()->setItemEnabled(MENU__BUILD_ADD_HYDROGENS, selected);
	menuBar()->setItemEnabled(MENU__BUILD_ASSIGN_CHARGES, selected);
	menuBar()->setItemEnabled(MENU__BUILD_CHECK_RESIDUE, selected);
	menuBar()->setItemEnabled(MENU__BUILD_BUILD_BONDS, selected);
	
	// these menu points for single items only
	menuBar()->setItemEnabled(MENU__DISPLAY_CENTER_CAMERA, selected && (number_of_selected_objects == 1));

	// set the checkboxes for the non-modal dialogs
	menuBar()->setItemChecked(MENU__DISPLAY_OPEN_PREFERENCES_DIALOG, 
													 preferences_dialog_->isVisible());

	// AMBER methods are available only for single systems
	menuBar()->setItemEnabled(MENU__BUILD_AMBER_ENERGY, 
														(all_systems && (number_of_selected_objects == 1)));

	menuBar()->setItemEnabled(MENU__BUILD_AMBER_MINIMIZATION, 
														(all_systems && (number_of_selected_objects == 1)));
}

void Mainframe::select()
{
	if (selection_.size() == 0)
	{
		return;
	}

	QString message;
	message.sprintf("selecting %d objects...", 
													selection_.size());
	statusBar()->message(message);
	QWidget::update();

	int value_static = object_processor_.getValue(ADDRESS__STATIC_MODEL);
	int value_dynamic = object_processor_.getValue(ADDRESS__DYNAMIC_MODEL);

	object_processor_.setValue(ADDRESS__STATIC_MODEL, VALUE__SELECT);
	object_processor_.setValue(ADDRESS__DYNAMIC_MODEL, VALUE__SELECT);
        
	List<Composite*>::ConstIterator list_it = selection_.begin();	
	for (; list_it != selection_.end(); ++list_it)
	{
		object_processor_.applyOn(**list_it);
		
		// update composite
		MainControl::update((*list_it)->getRoot());
	}

	// restore old values
	object_processor_.setValue(ADDRESS__STATIC_MODEL, value_static);
	object_processor_.setValue(ADDRESS__DYNAMIC_MODEL, value_dynamic);

	// update scene
	SceneMessage scene_message;
	scene_message.updateOnly();
	notify_(scene_message);

	statusBar()->clear();
	QWidget::update();
}

void Mainframe::deselect()
{
	if (selection_.size() == 0)
	{
		return;
	}

	QString message;
	message.sprintf("selecting %d objects...", selection_.size());
	statusBar()->message(message);
	QWidget::update();

	int value_static = object_processor_.getValue(ADDRESS__STATIC_MODEL);
	int value_dynamic = object_processor_.getValue(ADDRESS__DYNAMIC_MODEL);

	object_processor_.setValue(ADDRESS__STATIC_MODEL, VALUE__DESELECT);
	object_processor_.setValue(ADDRESS__DYNAMIC_MODEL, VALUE__DESELECT);
        
	List<Composite*>::ConstIterator list_it = selection_.begin();	
	for (; list_it != selection_.end(); ++list_it)
	{
		object_processor_.applyOn(**list_it);
		MainControl::update((**list_it).getRoot());
	}

	// restore old values
	object_processor_.setValue(ADDRESS__STATIC_MODEL, value_static);
	object_processor_.setValue(ADDRESS__DYNAMIC_MODEL, value_dynamic);

	// update scene
	SceneMessage scene_message;
	scene_message.updateOnly();
	notify_(scene_message);

	statusBar()->clear();
	QWidget::update();
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
	} else {
		statusBar()->message("errors found!");
	}

	QWidget::update();
}

void Mainframe::centerCamera()
{
	if (selection_.size() != 1)
	{
		return;
	}

  // use specified object processor for calculating the center
  object_processor_.calculateCenter(*selection_.front());

	Vector3 view_point = object_processor_.getViewCenter();

  // set the camera on the the new composite
	SceneMessage scene_message;
	scene_message.setCameraLookAt(view_point);

	view_point.z = view_point.z + object_processor_.getViewDistance();
	scene_message.setCameraViewPoint(view_point);

	// notify scene to perform an update and set the camera to the new object
	notify_(scene_message);
}

void Mainframe::buildBonds()
{
	if (selection_.size() == 0)
	{
		return;
	}

	Size number_of_bonds = 0;

	statusBar()->message("building bonds...");
	QWidget::update();

	List<Composite*>::ConstIterator list_it = selection_.begin();	
	for (; list_it != selection_.end(); ++list_it)
	{	
		(*list_it)->apply(fragment_db_.build_bonds);
		number_of_bonds += fragment_db_.build_bonds.getNumberOfBondsBuilt();
		object_processor_.applyOn(**list_it);

		MainControl::update((**list_it).getRoot());
	}

	// update scene
	SceneMessage scene_message;
	scene_message.updateOnly();
	notify_(scene_message);

	statusBar()->clear();
	QWidget::update();

	Log.info() << "Added " << number_of_bonds << " bonds." << endl;
}

void Mainframe::addHydrogens()
{
	if (selection_.size() == 0)
	{
		return;
	}

	Size number_of_hydrogens = 0;

	statusBar()->message("adding hydrogens...");
	QWidget::update();

	List<Composite*>::ConstIterator list_it = selection_.begin();	
	for (; list_it != selection_.end(); ++list_it)
	{	
		(*list_it)->apply(fragment_db_.add_hydrogens);
		number_of_hydrogens += fragment_db_.add_hydrogens.getNumberOfInsertedH();
		(*list_it)->apply(fragment_db_.build_bonds);
		object_processor_.applyOn(**list_it);

		MainControl::update((*list_it)->getRoot());
	}

	Log.info() << "added " << number_of_hydrogens << " hydrogen atoms." << endl;
	statusBar()->clear();
	QWidget::update();

	// update scene
	SceneMessage scene_message;
	scene_message.updateOnly();
	notify_(scene_message);
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
	amber.setup(system);

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

	QString message;
	message.sprintf("Total AMBER energy: %f kJ/mol.", amber.getEnergy());
	statusBar()->message(message, 5000);
}

void Mainframe::openPreferencesDialog()
{
	preferences_dialog_->show();
	preferences_dialog_->raise();
}

void Mainframe::about()
{
	DlgAbout about_box;
	about_box.exec();
	statusBar()->message("MolVIEW V 0.96a", 1500);
}

void Mainframe::applyPreferencesDialog()
{

	if (preferences_dialog_->isTabEnabled(preferences_dialog_->getGeneralTab()))
	{
		QApplication::setStyle(preferences_dialog_->getGeneralTab()->getStyle());
		QWidget::update();
	}
	
	if (preferences_dialog_->isTabEnabled(preferences_dialog_->getNetworkTab()))
	{
		checkServer();
	}
	
	if (preferences_dialog_->isTabEnabled(preferences_dialog_->getDisplayTab()))
	{
		if (preferences_dialog_->getDisplayTab()->isDepthCueingEnabled())
		{
		}

		if (preferences_dialog_->getDisplayTab()->isCoordinateAxesEnabled())
		{
		}
	}	
}

void Mainframe::startServer()
{
	// retrieve the port number
	int port = preferences_dialog_->getNetworkTab()->getPort();
	
	// set the port and active the server
	server_->setPort(port);
	server_->activate();
	
	// adjust the tool tip and update the server icon
	QString tip;
	tip.sprintf("VIEW Server listening on port %d", port); 
	QToolTip::add(server_icon_, tip);
	server_icon_->show();
	statusBar()->update();
}

void Mainframe::stopServer()
{
	// stop the server
	server_->deactivate();

	// hide the icon
	server_icon_->hide();
	QToolTip::add(server_icon_, "VIEW Server disabled");
	statusBar()->update();
}

void Mainframe::checkServer()
{
	// retrieve the settings of the preferences
	// dialog box
	bool start_server = preferences_dialog_->getNetworkTab()->getServerStatus();
	
	if (start_server)
	{
		startServer();
	} else {
		stopServer();
	}
}

void Mainframe::setPreferences(INIFile& inifile) const
{
	//	
	// the main window position
	//
	inifile.setValue
		("WINDOWS", "Main::x", String(x()));
	inifile.setValue
		("WINDOWS", "Main::y", String(y()));
	inifile.setValue
		("WINDOWS", "Main::width", String(width()));
	inifile.setValue
		("WINDOWS", "Main::height", String(height()));

	// 
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
}

void Mainframe::getPreferences(const INIFile& inifile)
{
	// 
	// the geometry of the main window
	//
	int x_pos = x();
	int y_pos = y();
	int w = 640;
	int h = 480;
	if (inifile.hasEntry("WINDOWS", "Main::x"))
	{
		x_pos = inifile.getValue("WINDOWS", "Main::x").toInt();
	}
	if (inifile.hasEntry("WINDOWS", "Main::y"))
	{
		y_pos = inifile.getValue("WINDOWS", "Main::y").toInt();
	}
	if (inifile.hasEntry("WINDOWS", "Main::height"))
	{
		h = inifile.getValue("WINDOWS", "Main::height").toInt();
	}
	if (inifile.hasEntry("WINDOWS", "Main::width"))
	{
		w = inifile.getValue("WINDOWS", "Main::width").toInt();
	}
	setGeometry(x_pos, y_pos, w, h);

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
}


#ifdef BALL_NO_INLINE_FUNCTIONS
#	include "mainframe.iC"
#endif
