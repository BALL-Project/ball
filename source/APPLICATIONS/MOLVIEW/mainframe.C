// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: mainframe.C,v 1.85 2003/10/05 15:27:40 amoll Exp $
//

#include "mainframe.h"
#include "icons.h"

#include <BALL/MOLMEC/AMBER/amber.h>
#include <BALL/MOLMEC/MINIMIZATION/conjugateGradient.h>
#include <BALL/MOLMEC/MDSIMULATION/microCanonicalMD.h>
#include <BALL/MOLMEC/MINIMIZATION/steepestDescent.h>
#include <BALL/MOLMEC/MDSIMULATION/canonicalMD.h>
#include <BALL/MOLMEC/MDSIMULATION/microCanonicalMD.h>
#include <BALL/MOLMEC/MDSIMULATION/molecularDynamics.h>
#include <BALL/STRUCTURE/HBondProcessor.h>

#include <BALL/VIEW/DIALOGS/FDPBDialog.h>
#include <BALL/VIEW/PRIMITIVES/mesh.h>
#include <BALL/VIEW/KERNEL/moleculeObjectCreator.h>
#include <BALL/VIEW/DIALOGS/peptideDialog.h>
#include <BALL/DATATYPE/regularData3D.h>
#include <BALL/DATATYPE/contourSurface.h>
#include <BALL/STRUCTURE/residueChecker.h>
#include <BALL/STRUCTURE/geometricProperties.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/FORMAT/DCDFile.h>

#include "aboutDialog.h"

#ifdef BALL_PYTHON_SUPPORT
#	include <BALL/VIEW/WIDGETS/pyWidget.h>
#endif

#include <qlabel.h>
#include <qmenubar.h>
#include <qdockwindow.h>
#include <qstatusbar.h>
#include <qlabel.h>
#include <qaccel.h> 
#include <qsettings.h>

#ifdef QT_THREAD_SUPPORT
#	include "threads.h"
#endif

// testing
#include <BALL/VIEW/DIALOGS/parsedFunctionDialog.h>

namespace BALL
{
	using namespace std;
	using namespace BALL::VIEW;

	Mainframe::Mainframe(QWidget* parent, const char* name)
		:	MainControl(parent, name, ".molview"),
			scene_(0),
			control_(0),
			dataset_control_(0),
			geometric_control_(0),
			display_properties_(0),
			minimization_dialog_(0),
			label_dialog_(0),
			molecular_properties_(0),
			file_dialog_(0),
			server_(0),
			logview_(0),
			simulation_thread_(0),
			fullscreen_(false),
			stop_simulation_(false)
	{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "new Mainframe " << this << std::endl;
#endif
		// ---------------------
		// setup main window
		// ---------------------
		setCaption("MolVIEW");
		setIcon(*new QPixmap(bucky_64x64_xpm));
		resize(640,400);
		// make sure the open submenu is the first in file
		initPopupMenu(FILE_OPEN);

		// ---------------------
		// Logstream setup -----
		// ---------------------
		Log.remove(std::cout);

		control_ = new MolecularControl(this, "Structures");
		CHECK_PTR(control_);
	
	  geometric_control_ = new GeometricControl(this, "Representations");
		CHECK_PTR(geometric_control_);

		dataset_control_ = new DatasetControl(this, "Datasets");
		CHECK_PTR(dataset_control_);

		scene_ = new Scene(this, "3D View");
		CHECK_PTR(scene_);
		scene_->setMinimumSize(10, 10);
		setCentralWidget(scene_);

		display_properties_ = new DisplayProperties(this, "DisplayProperties");
		CHECK_PTR(display_properties_);

		minimization_dialog_ = new AmberMinimizationDialog(this, "AmberMinimizationDialog");
		CHECK_PTR(minimization_dialog_);

		md_dialog_ = new MolecularDynamicsDialog(this, "MolecularDynamicsDialog");
		CHECK_PTR(md_dialog_);

		surface_dialog_ = new ContourSurfaceDialog(this, "ContourSurfaceDialog");
		CHECK_PTR(surface_dialog_);

		label_dialog_ = new LabelDialog(this, "LabelDialog");
		CHECK_PTR(label_dialog_);
		
		file_dialog_ = new MolecularFileDialog(this, "MolecularFileDialog");
		CHECK_PTR(file_dialog_);

		molecular_properties_ = new MolecularProperties(this, "MolecularProperties");
		CHECK_PTR(molecular_properties_);

		server_ = new Server(this);
		CHECK_PTR(server_);

		logview_ = new LogView(this, "Logs");
		CHECK_PTR(logview_);
		logview_->setMinimumSize(10, 10);

		FDPB_dialog_ = new FDPBDialog(this);
		CHECK_PTR(FDPB_dialog_);

		#ifdef BALL_PYTHON_SUPPORT
			PyWidget* pywidget = new PyWidget(this, "Python Interpreter");
			CHECK_PTR(pywidget);
			pywidget->startInterpreter();
		#endif

		// ---------------------
		// Menus ---------------
		// ---------------------
		String hint;

		// File Menu
		insertMenuEntry(MainControl::FILE_EXPORT, "Export POVRa&y file", this, SLOT(exportPOVRay()), 
										CTRL+Key_Y, MENU_EXPORT_POVRAYFILE);

		// Display Menu
		insertMenuEntry(MainControl::DISPLAY, "Toggle Fullscreen", this, SLOT(toggleFullScreen()),
										ALT+Key_X, MENU_FULLSCREEN);
		// Build Menu -------------------------------------------------------------------
		hint = "To assign charges, one System has to be selected.";
		insertMenuEntry(MainControl::BUILD, "Assign Char&ges", this, SLOT(assignCharges()), 
										CTRL+Key_G, MENU_ASSIGN_CHARGES, hint);
		hint = "To assign H-bonds, one System has to be selected.";
		insertMenuEntry(MainControl::BUILD, "Calculate H-Bonds", this, SLOT(calculateHBonds()), 
										CTRL+Key_9, MENU_CALCULATE_HBONDS, hint);
		insertMenuEntry(MainControl::BUILD, "Build Peptide", this, SLOT(buildPeptide()), ALT+Key_P, MENU_PEPTIDE);

		// Tools Menu -------------------------------------------------------------------
		hint = "Calculate the energy of a System with the AMBER force field.";
		insertMenuEntry(MainControl::TOOLS, "Single Point Energy", this, SLOT(calculateAmberEnergy()), 
										CTRL+Key_A, MENU_AMBER_ENERGY, hint);
		hint = "To perform an Energy Minimization, first select the molecular structures.";
		insertMenuEntry(MainControl::TOOLS, "&Energy Minimization", this, SLOT(amberMinimization()), 
										CTRL+Key_E, MENU_AMBER_MINIMIZATION, hint);
		hint = "To perform a MD simulation , first select the molecular structures.";
		insertMenuEntry(MainControl::TOOLS, "Molecular &Dynamics", this, SLOT(amberMDSimulation()), 
										CTRL+Key_D, MENU_AMBER_MDSIMULATION, hint);
		hint = "Calculate an isocontour surface from a 3D grid";
		insertMenuEntry(MainControl::TOOLS, "Contour S&urface", this,  SLOT(computeSurface()), 
										CTRL+Key_U,MENU_OPEN_SURFACE_DIALOG, hint);

	#ifdef QT_THREAD_SUPPORT
		hint = "Abort a running simulation thread";
		insertMenuEntry(MainControl::TOOLS, "Abort Calculation", this, SLOT(stopSimulation()),
				ALT+Key_C, MENU_STOPSIMULATION, hint);
	#endif
		hint = "Calculate the Electrostatics with FDPB, if one System selected.";
		insertMenuEntry(MainControl::TOOLS, "Electrostatics", FDPB_dialog_, SLOT(show()), -1,
				MENU_FDPB, hint);
				
		// Help-Menu -------------------------------------------------------------------
		insertMenuEntry(MainControl::HELP, "About", this, SLOT(about()), CTRL+Key_9, MENU__HELP_ABOUT);

		// Menu ------------------------------------------------------------------------
		menuBar()->setSeparator(QMenuBar::InWindowsStyle);

		// ---------------------
		// Connectivity --------
		// ---------------------

		connect(initPopupMenu(MainControl::BUILD), SIGNAL(aboutToShow()), this, SLOT(checkMenuEntries()));
		connect(initPopupMenu(MainControl::FILE), SIGNAL(aboutToShow()), this, SLOT(checkMenuEntries()));
		connect(initPopupMenu(MainControl::TOOLS), SIGNAL(aboutToShow()), this, SLOT(checkMenuEntries()));

		// check the active menu entries
		checkMenuEntries();

		//--------------------------------
		// setup the VIEW server
		//--------------------------------

		// registering object generator
	//  MoleculeObjectCreator* object_creator = new MoleculeObjectCreator;
	//  server_->registerObjectCreator(*object_creator);

		setStatusbarText("Ready.");
	}

	Mainframe::~Mainframe()
		throw()
	{
#ifdef QT_THREAD_SUPPORT
		stop_simulation_ = true;
		if (simulation_thread_ != 0)
		{
			if (simulation_thread_->running()) simulation_thread_->wait();
			delete simulation_thread_;
			simulation_thread_ = 0;
		}
#endif

		delete minimization_dialog_;
		delete md_dialog_;
	}

	void Mainframe::onNotify(Message *message)
		throw()
	{
		MainControl::onNotify(message);
	}

	void Mainframe::checkMenuEntries()
	{
		setCompositesMuteable(!simulation_thread_);

		bool one_item = (control_selection_.size() == 1);

		menuBar()->setItemEnabled(MENU_ASSIGN_CHARGES, one_item && composites_muteable_);

		// AMBER methods are available only for single systems
		menuBar()->setItemEnabled(MENU_AMBER_ENERGY, one_item);
		// disable simulation entries, if a simulation is already running
		menuBar()->setItemEnabled(MENU_AMBER_MINIMIZATION, one_item && composites_muteable_);
		menuBar()->setItemEnabled(MENU_AMBER_MDSIMULATION, one_item && composites_muteable_);
		menuBar()->setItemEnabled(MENU_PEPTIDE, composites_muteable_);
		// enable stopSimulation if simulation is running
		menuBar()->setItemEnabled(MENU_STOPSIMULATION, !composites_muteable_);

		menuBar()->setItemEnabled(MENU_FDPB, getSelectedSystem() && composites_muteable_);
	}

	void Mainframe::exportPOVRay()
	{
		QFileDialog *fd = new QFileDialog(this, "", true);
		fd->setMode(QFileDialog::AnyFile);
		fd->setCaption("Export POVRay File");
		fd->setViewMode(QFileDialog::Detail);

		if (!fd->exec()== QDialog::Accepted) return;

		String filename(fd->selectedFile().ascii());
		delete fd;

		POVRenderer pr(filename);
		pr.width  = scene_->width();
		pr.height = scene_->height(); 
		pr.init(*(scene_->getStage()));
		scene_->exportScene(pr);
		pr.finish();
	}
		

	void Mainframe::assignCharges()
	{
	}

	void Mainframe::calculateHBonds()
	{
		if (!getSelectedSystem())
		{
			setStatusbarText("to calculate H-bonds, one system has to be selected");
			return;
		}

		HBondProcessor proc;
		if (!getSelectedSystem()) return;
		getSelectedSystem()->apply(proc);
		MainControl::update(*getSelectedSystem());
		setStatusbarText("calculated H-bonds");
	}

	void Mainframe::calculateAmberEnergy()
	{
		System* system = getSelectedSystem();
		if (system == 0)
		{
			setStatusbarText("to calculate AMBER energies, one system has to be selected");
			return;
		}

		// set up the AMBER force field
		setStatusbarText("setting up force field...");

		String filename(minimization_dialog_->getFilename());
		Path path;
		filename = path.find(filename);
		if (filename == "")
		{
			Log.error() << "Invalid filename for amber options" << std::endl;
			return;
		}

		AmberFF amber;
		amber.options[AmberFF::Option::ASSIGN_TYPES] = "true";
		amber.options[AmberFF::Option::ASSIGN_CHARGES] = "true";
		amber.options[AmberFF::Option::ASSIGN_TYPENAMES] = "true";
		amber.options[AmberFF::Option::OVERWRITE_CHARGES] = "true";
		amber.options[AmberFF::Option::OVERWRITE_TYPENAMES] = "true";
		amber.options[AmberFF::Option::DISTANCE_DEPENDENT_DIELECTRIC] 
			= String(minimization_dialog_->getUseDistanceDependentDC());
		amber.options[AmberFF::Option::FILENAME] = String(filename);

		if (!amber.setup(*system))
		{
			Log.error() << "Force field setup failed." << std::endl;
			return;
		}

		// calculate the energy
		setStatusbarText("calculating energy...");

		amber.updateEnergy();

		// print the result
		printAmberResults(amber);
		setStatusbarText("Total AMBER energy: " + String(amber.getEnergy()) + " kJ/mol.");
	}

	void Mainframe::computeSurface()
	{
		// execute the surface dialog and abort if cancel is clicked
		if (surface_dialog_->exec() == 0)
		{
			return;
		}
		RegularData3D rd;
		File f(surface_dialog_->getFileName());
		f>>rd;
		ContourSurface cs(rd, surface_dialog_->getThreshold());
		Mesh* mesh = new Mesh();
		*static_cast<Surface*>(mesh) = (Surface) cs;

		Representation* rep = getPrimitiveManager().createRepresentation();
		rep->insert(*mesh);
		rep->setModelType(MODEL_CONTOUR_SURFACE); // Setting Representation type to Surface

		RepresentationMessage* message = new RepresentationMessage(rep, RepresentationMessage::ADD);
		notify_(message);
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

		// ============================= WITH MULTITHREADING ====================================
	#ifdef QT_THREAD_SUPPORT
		EnergyMinimizerThread* thread = new EnergyMinimizerThread;
		simulation_thread_ = thread;
		checkMenuEntries();

		thread->setEnergyMinimizer(minimizer);
		thread->setNumberOfStepsBetweenUpdates(minimization_dialog_->getRefresh());
		thread->setMainframe(this);
		thread->start();
	#else
		// ============================= WITHOUT MULTITHREADING =================================
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
		Size steps = md_dialog_->getStepsBetweenRefreshs();

		DCDFile* dcd = 0;
		if (md_dialog_->getDCDFile().size()) 
		{
			dcd = new DCDFile;
			dcd->open(md_dialog_->getDCDFile(), File::OUT);
			dcd->enableVelocityStorage();
		}
		// ============================= WITH MULTITHREADING ===================================
	#ifdef QT_THREAD_SUPPORT
		MDSimulationThread* thread = new MDSimulationThread;
		simulation_thread_ = thread;
		checkMenuEntries();

		thread->setMolecularDynamics(mds);
		thread->setNumberOfSteps(md_dialog_->getNumberOfSteps());
		thread->setNumberOfStepsBetweenUpdates(steps);
		thread->setMainframe(this);
		thread->setSaveImages(md_dialog_->saveImages());
		thread->setDCDFile(dcd);
		thread->setComposite(system);
		thread->start();

	#else
		// ============================= WITHOUT MULTITHREADING ==============================
		// iterate until done and refresh the screen every "steps" iterations
		
		SnapShotManager manager(amber->getSystem(), amber, dcd);
		manager.setFlushToDiskFrequency(10);
		while (mds->getNumberOfIterations() < md_dialog_->getNumberOfSteps())
		{
			mds->simulateIterations(steps, true);
			MainControl::update(system->getRoot());
			if (md_dialog_->saveImages()) 
			{
				Scene* scene= (Scene*) Scene::getInstance(0);
				scene->exportPNG();
			}
			
			if (dcd) manager.takeSnapShot();

			QString message;
			message.sprintf("Iteration %d: energy = %f kJ/mol, RMS gradient = %f kJ/mol A", 
											mds->getNumberOfIterations(), amber->getEnergy(), amber->getRMSGradient());
			setStatusbarText(String(message.ascii()));
		}

		if (dcd) manager.flushToDisk();

		Log.info() << std::endl << "simulation terminated." << std::endl << endl;
		printAmberResults(*amber);
		Log.info() << "final RMS gadient    : " << amber->getRMSGradient() << " kJ/(mol A)   after " 
							 << mds->getNumberOfIterations() << " iterations" << endl << endl;
		setStatusbarText("Total AMBER energy: " + String(amber->getEnergy()) + " kJ/mol.");

		// clean up
		delete mds;

		if (dcd)
		{
			dcd->close();
			delete dcd;
			dcd = new DCDFile(md_dialog_->getDCDFile(), File::IN);

			NewTrajectoryMessage* message = new NewTrajectoryMessage;
			message->setComposite(amber->getSystem());
			message->setTrajectoryFile(dcd);
			notify_(message);
		}

		delete amber;
	#endif
	}

	void Mainframe::about()
	{
		/*
		ParsedFunctionDialog* pfd = new ParsedFunctionDialog();
		pfd->show();
		return;
		*/

		AboutDialog about;
		about.exec();
	}

	void Mainframe::fetchPreferences(INIFile& inifile)
		throw()
	{
		// preferences for the dialogs
		minimization_dialog_->readPreferences(inifile);
		md_dialog_->readPreferences(inifile);

		MainControl::fetchPreferences(inifile);
	}

	void Mainframe::writePreferences(INIFile& inifile)
		throw()
	{
		minimization_dialog_->writePreferences(inifile);
		md_dialog_->writePreferences(inifile);

		MainControl::writePreferences(inifile);
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

			fullscreen_ = true;
			return;
		}

		showNormal();
		fullscreen_ = false;
	}

	void Mainframe::openFile(const String& file)
		throw()
	{
		file_dialog_->openFile(file);
	}


	void Mainframe::buildPeptide()
	{
		PeptideDialog* dialog = new PeptideDialog;
		dialog->exec();

		Protein* protein = dialog->getProtein();
		if (protein == 0) return;

		System* system = new System;
		system->insert(*protein);
		composite_manager_.insert(*system);
		CompositeMessage* new_message = new CompositeMessage(*system, CompositeMessage::NEW_COMPOSITE);
		new_message->setCompositeName("Peptide");
		notify_(new_message);
	}

	void Mainframe::stopSimulation() 
	{
	#ifdef QT_THREAD_SUPPORT
		stop_simulation_ = true;
		if (simulation_thread_ != 0)
		{
			if (simulation_thread_->running()) simulation_thread_->wait();

			DCDFile* file = simulation_thread_->getDCDFile();
			if (file)
			{
				file->close();
				String filename = file->getName();
				delete file;
				file = new DCDFile(filename, File::IN);
				NewTrajectoryMessage* message = new NewTrajectoryMessage;
				message->setComposite(simulation_thread_->getComposite());
				message->setTrajectoryFile(file);
				notify_(message);
			}
			
			delete simulation_thread_;
			simulation_thread_ = 0;
		}

		setStatusbarText("Simulation terminated.");
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

} 
