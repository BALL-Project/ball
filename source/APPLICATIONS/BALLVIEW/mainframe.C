// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include "mainframe.h"
#include "aboutDialog.h"   // Phase 999.43: hand-coded About dialog (replaces aboutDialog.ui)
#include "demoTutorialDialog.h"
#ifdef BALL_UI_V2
#	include "iconBrowser.h"  // Phase 999.43: dev-only icon browser
#endif

#include <BALL/VIEW/KERNEL/theme/iconRegistry.h>  // Phase 999.42: Icons::get façade
#include <BALL/VIEW/KERNEL/theme/tokens.h>  // Phase 999.41: kIconToolbar
#include <BALL/VIEW/WIDGETS/molecularStructure.h>
#include <BALL/VIEW/WIDGETS/molecularControl.h>
#include <BALL/VIEW/WIDGETS/geometricControl.h>
#include <BALL/VIEW/WIDGETS/logView.h>
#include <BALL/VIEW/WIDGETS/datasetControl.h>
#include <BALL/VIEW/WIDGETS/fileObserver.h>
#include <BALL/VIEW/DIALOGS/pubchemDialog.h>
#include <BALL/VIEW/DIALOGS/undoManagerDialog.h>
#include <BALL/VIEW/DIALOGS/downloadPDBFile.h>
#include <BALL/VIEW/DIALOGS/downloadElectronDensity.h>
#include <BALL/VIEW/DIALOGS/labelDialog.h>
#include <BALL/VIEW/DIALOGS/displayProperties.h>
#include <BALL/VIEW/DIALOGS/molecularFileDialog.h>
#include <BALL/VIEW/DATATYPE/standardDatasets.h>
#ifdef BALL_PYTHON_SUPPORT
#	include <BALL/PYTHON/pyInterpreter.h>
#	include <BALL/VIEW/WIDGETS/testFramework.h>
#endif

#include <BALL/SYSTEM/path.h>
#include <BALL/KERNEL/forEach.h>

#include <BALL/CONCEPT/moleculeObjectCreator.h>

// Phase 999.43: ui_aboutDialog.h removed — AboutDialog is hand-coded
// (see aboutDialog.h included at the top of this file).

#include <BALL/PLUGIN/pluginManager.h>
#include <BALL/VIEW/PLUGIN/inputDevPluginHandler.h>
#include <BALL/VIEW/PLUGIN/modularWidgetPluginHandler.h>
#include <BALL/VIEW/PLUGIN/VIEWPlugin.h>
#include <BALL/VIEW/DIALOGS/pluginDialog.h>
#include <BALL/VIEW/DIALOGS/preferences.h>

using namespace std;
//#define BALL_VIEW_DEBUG

namespace BALL
{
	using namespace std;
	using namespace BALL::VIEW;

	Mainframe::Mainframe(QWidget* parent, const char* name)
		:	MainControl(parent, name, ".BALLView"),
			scene_(0),
			save_project_action_(0),
			qload_action_(0),
			qsave_action_(0)
	{
		// Fixes a major problem with Qt WebEngine 5.5 when being used in a DockWidget
		qApp->setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);

		registerThis();

		#ifdef BALL_VIEW_DEBUG
		Log.error() << "new Mainframe " << this << std::endl;
		#endif

		// ---------------------
		// setup main window
		// ---------------------
		setWindowTitle(tr("BALLView"));
		setWindowIcon(VIEW::Icons::get("ball-app"));
		// make sure submenus are the first 
		initPopupMenu(FILE_OPEN);
		initPopupMenu(EDIT);
		initPopupMenu(BUILD);
		initPopupMenu(DISPLAY);
		initPopupMenu(MOLECULARMECHANICS);
		initPopupMenu(TOOLS);
	#ifdef BALL_PYTHON_SUPPORT
		initPopupMenu(TOOLS_PYTHON);
		initPopupMenu(MainControl::USER);
	#endif
		initPopupMenu(WINDOWS);
		initPopupMenu(MACRO);

		// ---------------------
		// Logstream setup -----
		// ---------------------
//   		Log.remove(std::cout);
//   		Log.remove(std::cerr);
		setLoggingFilename("BALLView.log");
		
		// Display Menu
		String description = "Shortcut|Display|Toggle_Fullscreen";
		fullscreen_action_ = insertMenuEntry(MainControl::DISPLAY, (String)tr("Toggle Fullscreen"), this,
		                                     SLOT(toggleFullScreen()), description, QKeySequence("Alt+X"),
																				 UIOperationMode::MODE_ADVANCED);

		if (fullscreen_action_)
			fullscreen_action_->setIcon(VIEW::Icons::get("actions/view-fullscreen"));

		insertPopupMenuSeparator(DISPLAY, UIOperationMode::MODE_ADVANCED);
		initPopupMenu(DISPLAY_VIEWPOINT);

		new MolecularFileDialog    (this, "MolecularFileDialog");
		new DownloadPDBFile        (this, "DownloadPDBFile", false);
		new DownloadElectronDensity(this, "DownloadElectronDensity", false);
		new PubChemDialog          (this, "PubChemDialog");
		new UndoManagerDialog      (this, "UndoManagerDialog");

		addDockWidget(Qt::LeftDockWidgetArea, new MolecularControl(this, ((String)tr("Structures")).c_str()));
		addDockWidget(Qt::LeftDockWidgetArea, new GeometricControl(this, ((String)tr("Representations")).c_str()));
		addDockWidget(Qt::TopDockWidgetArea,  new DatasetControl(this, ((String)tr("Datasets")).c_str()));
		DatasetControl* dc = DatasetControl::getInstance(0);
		dc->registerController(new RegularData3DController());
		dc->registerController(new TrajectoryController());
		dc->registerController(new VectorGridController());
		dc->registerController(new DockResultController());

//  NOTE: raytraceable grids have been deferred until 1.4/2.0
//		dc->registerController(new RaytraceableGridController());
		
		DatasetControl::getInstance(0)->hide();

		// For Demo, Tutorial, and RayTracing 
		new DemoTutorialDialog(this, ((String)tr("BALLViewDemo")).c_str());

		Path path;

		new LabelDialog(        this, ((String)tr("LabelDialog")).c_str());
		new MolecularStructure(	this, ((String)tr("MolecularStructure")).c_str());
 		addDockWidget(Qt::BottomDockWidgetArea, new LogView(      this, ((String)tr("Logs")).c_str()));
		addDockWidget(Qt::BottomDockWidgetArea, new FileObserver( this, ((String)tr("FileObserver")).c_str()));

		setupPluginHandlers_();
		Scene::stereoBufferSupportTest();
		scene_ = new Scene(this, ((String)tr("3D View")).c_str());
		setCentralWidget(scene_);
		setAcceptDrops(true);

		new DisplayProperties(this, ((String)tr("DisplayProperties")).c_str());

		#ifdef BALL_PYTHON_SUPPORT
			PyInterpreter::initialize();
			new TestFramework(this, ((String)"Test Framework").c_str());
		#endif

		// ---------------------
		// Menus ---------------
		// ---------------------

		description = "Shortcut|File|Open|Project";
		insertMenuEntry(MainControl::FILE_OPEN, ((String)tr("Project")).c_str(), this, 
										SLOT(loadBALLViewProjectFile()), description, QKeySequence(),
										UIOperationMode::MODE_ADVANCED);
		
		description = "Shortcut|File|Save_Project";
		save_project_action_ = insertMenuEntry(MainControl::FILE, ((String)tr("Save Project")).c_str(), this, 
											 										 SLOT(saveBALLViewProjectFile()), description, QKeySequence(),
																					 UIOperationMode::MODE_ADVANCED);

		// Help-Menu -------------------------------------------------------------------
		QAction* action = 0;

		description = "Shortcut|Help|About";
		action = insertMenuEntry(MainControl::HELP, (String)tr("About"), this, SLOT(about()), description);
		if (action)
			setMenuHint(action, (String)tr("Show informations on this version of BALLView"));

#if defined(BALL_UI_V2) && !defined(NDEBUG)
		// Phase 999.43: Tools › Icon Browser. Dev-only menu entry so
		// the design team can spot-check the theme.qrc bundle without
		// firing up a debugger. Production release builds (NDEBUG)
		// don't get this menu — only the menu wiring is conditional;
		// the IconBrowser class itself compiles whenever BALL_UI_V2
		// is ON so a debug rebuild can flip the menu on without
		// touching the build system.
		description = "Shortcut|Tools|IconBrowser";
		action = insertMenuEntry(MainControl::TOOLS, (String)tr("Icon Browser (dev)"),
		                         this, SLOT(openIconBrowser()), description,
		                         QKeySequence(),
		                         UIOperationMode::MODE_ADVANCED);
		if (action)
			setMenuHint(action, (String)tr("Phase 999.43: browse theme.qrc icons (debug builds only)"));
#endif

		// TODO: why is this done here and not, e.g., in mainControl()???
		description = "Shortcut|MolecularMechanics|Abort_Calculation";
		stop_simulation_action_ = insertMenuEntry(MainControl::MOLECULARMECHANICS, (String)tr("Abort Calculation"), this, 
																							SLOT(stopSimulation()), description, QKeySequence("Alt+C"), 
																							UIOperationMode::MODE_ADVANCED);
		if (stop_simulation_action_)
		{
			stop_simulation_action_->setEnabled(false);
			setMenuHint(stop_simulation_action_, (String)tr("Abort a running simulation"));
			insertPopupMenuSeparator(MainControl::MOLECULARMECHANICS);

			stop_simulation_action_->setIcon(VIEW::Icons::get("actions/process-stop"));
		}
		
		
		description = "Shortcut|Edit|Invert_Selection";
		complement_selection_action_ = insertMenuEntry(MainControl::EDIT, (String)tr("Invert Selection"), this, 
																									 SLOT(complementSelection()), description, QKeySequence(), 
																									 UIOperationMode::MODE_ADVANCED);

		description = "Shortcut|Edit|Clear_Selection";
		clear_selection_action_ = insertMenuEntry(MainControl::EDIT, (String)tr("Clear Selection"), this, 
																							SLOT(clearSelection()), description, QKeySequence(),
																							UIOperationMode::MODE_ADVANCED);


 		qApp->installEventFilter(this);

		setStatusbarText((String)tr("Ready."));
	}

	Mainframe::~Mainframe()
	{
		#ifdef BALL_PYTHON_SUPPORT
			PyInterpreter::finalize();
		#endif
	}


	bool Mainframe::eventFilter(QObject* sender, QEvent* event) 
	{
		if (event->type() != QEvent::KeyPress) return false;

		QKeyEvent* e = dynamic_cast<QKeyEvent*>(event);

		QPoint point = QCursor::pos();
		QWidget* widget = qApp->widgetAt(point);
		if (widget == scene_ &&
 				qApp->focusWidget() != scene_)
		{
 			scene_->keyPressEvent(e);
			return true;
		}

 		if (e->key() == Qt::Key_Delete &&
                RTTI::isKindOf<QTreeWidget>(sender))
 		{
 			deleteClicked();
			return true;
 		}

		if (e->key() == Qt::Key_Enter) 
		{
			if (composite_manager_.getNumberOfComposites() == 0) return false;

			if (getMolecularControlSelection().size() == 0)
			{
				control_selection_.push_back(*composite_manager_.begin());
			}
				
			MolecularStructure::getInstance(0)->centerCamera();
			return true;
		}

		// check all menu entries if Alt or CTRL is pressed to enable shortcuts
		if (e->key() == Qt::Key_Alt ||
				e->key() == Qt::Key_Control)				
		{
			checkMenus();
			return false;
		}

		return false;
	}

	
	void Mainframe::reset()
	{
		if (composites_locked_ || getRepresentationManager().updateRunning()) return;

		clearData();

		DisplayProperties* dp = DisplayProperties::getInstance(0);
		dp->setDrawingPrecision(DRAWING_PRECISION_HIGH);
		dp->selectModel(MODEL_STICK);
		dp->selectColoringMethod(COLORING_ELEMENT);
		dp->selectMode(DRAWING_MODE_SOLID);
		dp->setTransparency(0);
		dp->setSurfaceDrawingPrecision(6.5);
	}

	
	void Mainframe::checkMenus()
	{
		MainControl::checkMenus();

		save_project_action_->setEnabled(!composites_locked_);
		qload_action_->setEnabled(!composites_locked_);
		qsave_action_->setEnabled(!composites_locked_);
	}

	void Mainframe::show()
	{
		// prevent multiple inserting of menu entries, by calls of showFullScreen(), ...
		if (preferences_action_ != 0) 
		{
			MainControl::show();
			return;
		}

		QToolBar* tb = NULL;
		if (UIOperationMode::instance().getMode() <= UIOperationMode::MODE_ADVANCED)
		{
			tb = new QToolBar("Main Toolbar", this);
			tb->setObjectName("Main Toolbar");
			// Phase 999.41 — BALLView Refresh: 22→20 logical px under
			// BALL_UI_V2 (denser, modern; renderer handles HiDPI scaling).
			// Constant lives in source/VIEW/KERNEL/theme/tokens.h (999.40).
#ifdef BALL_UI_V2
			tb->setIconSize(QSize(BALL::VIEW::Theme::kIconToolbar,
			                      BALL::VIEW::Theme::kIconToolbar));
#else
			tb->setIconSize(QSize(22,22));
#endif
			addToolBar(Qt::TopToolBarArea, tb);
		}

		MainControl::show();

		QMenu *menu = initPopupMenu(MainControl::WINDOWS, UIOperationMode::MODE_ADVANCED);

		if (menu)
		{
			menu->addSeparator();
		  menu->addAction(tb->toggleViewAction());
		}

		// NOTE: this *has* to be run... a null pointer is unproblematic
		if (UIOperationMode::instance().getMode() <= UIOperationMode::MODE_ADVANCED)
		{
						MolecularFileDialog::getInstance(0)->addToolBarEntries(tb);
						DownloadPDBFile::getInstance(0)->addToolBarEntries(tb);
						DownloadElectronDensity::getInstance(0)->addToolBarEntries(tb);
						PubChemDialog::getInstance(0)->addToolBarEntries(tb);
						UndoManagerDialog::getInstance(0)->addToolBarEntries(tb);
						tb->addAction(fullscreen_action_);

						Path path;

						qload_action_ = new QAction(VIEW::Icons::get("actions/quickopen-file"), tr("quickload"), this);
						qload_action_->setObjectName("quickload");
						connect(qload_action_, SIGNAL(triggered()), this, SLOT(quickLoadConfirm()));
						tb->addAction(qload_action_);

						qsave_action_ = new QAction(VIEW::Icons::get("actions/quicksave"), tr("quicksave"), this);
						qsave_action_->setObjectName("quicksave");
						connect(qsave_action_, SIGNAL(triggered()), this, SLOT(quickSave()));
						tb->addAction(qsave_action_);

						tb->addSeparator();
						DisplayProperties::getInstance(0)->addToolBarEntries(tb);
						MolecularStructure::getInstance(0)->addToolBarEntries(tb);
		}

		scene_->addToolBarEntries(tb);
		if (UIOperationMode::instance().getMode() <= UIOperationMode::MODE_ADVANCED)
		{
		
						tb->addAction(stop_simulation_action_);
						tb->addAction(preferences_action_);
		}
		// we have changed the child widgets stored in the maincontrol (e.g. toolbars), so we have
		// to restore the window state again!
		restoreWindows();
	}

	void Mainframe::about()
	{
		// Phase 999.43: hand-coded AboutDialog (see aboutDialog.h). The
		// dialog composes its own layout from the shared widget set
		// (SectionHeader + FormRow) and loads the author list from
		// data/BALLView/about/authors.json, so this slot just constructs
		// and exec()s it.
		VIEW::AboutDialog dlg(this);
		dlg.exec();
	}

#ifdef BALL_UI_V2
	void Mainframe::openIconBrowser()
	{
		// Phase 999.43: open the dev-only IconBrowser dialog. Slot is
		// always present when BALL_UI_V2 is ON; the menu wiring above
		// gates the user-visible entry on !NDEBUG.
		VIEW::IconBrowser* dlg = new VIEW::IconBrowser(this);
		dlg->setAttribute(Qt::WA_DeleteOnClose, true);
		dlg->show();
	}
#endif

	void Mainframe::changeEvent(QEvent* evt)
	{
		if(evt->type() == QEvent::WindowStateChange) {
			if (isFullScreen())
			{
				if (fullscreen_action_ != 0)
				{
					fullscreen_action_->setIcon(VIEW::Icons::get("actions/view-restore"));
				}
			}
			else
			{
				if (fullscreen_action_ != 0)
				{
					fullscreen_action_->setIcon(VIEW::Icons::get("actions/view-fullscreen"));
				}
			}
		}
	}

	void Mainframe::setupPluginHandlers_()
	{
		PluginManager& man = PluginManager::instance();

		boost::shared_ptr<PluginHandler> handler(new InputDevPluginHandler());
		man.registerHandler(handler);

		handler.reset(new ModularWidgetPluginHandler(this));
		man.registerHandler(handler);
		man.registerHandler(new PluginDialog(getPreferences(), this));
	}
}
