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
#ifdef BALL_UI_V2
// Phase 999.45 — BALLView Refresh: Workspace consolidation.
#	include <BALL/VIEW/KERNEL/workspaceManager.h>
#	include <BALL/VIEW/WIDGETS/projectDock.h>
#	include <BALL/VIEW/WIDGETS/bottomDrawer.h>
#	include <QtWidgets/QStatusBar>
#	include <QtWidgets/QMenu>
#	include <QtWidgets/QInputDialog>
#	include <QtWidgets/QLineEdit>
#	include <QtGui/QAction>
#endif
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

		// Phase 999.45 — Workspace consolidation. Under BALL_UI_V2 the
		// 3 project-related controls are tabified into a single
		// left-rail ProjectDock group (Handover §5.1). Without the
		// flag (legacy / Classic path) keep the original 5-dock
		// layout — this IS the Classic preset by definition.
		MolecularControl* mol_ctrl = new MolecularControl(this, ((String)tr("Structures")).c_str());
		GeometricControl* geom_ctrl = new GeometricControl(this, ((String)tr("Representations")).c_str());
		DatasetControl* dataset_ctrl = new DatasetControl(this, ((String)tr("Datasets")).c_str());

#ifdef BALL_UI_V2
		// All 3 docks need to be in the same dock-area (left) before
		// tabifyDockWidget can group them. ProjectDock::tabify moves
		// DatasetControl off the top area into the left tab group.
		addDockWidget(Qt::LeftDockWidgetArea, mol_ctrl);
		addDockWidget(Qt::LeftDockWidgetArea, geom_ctrl);
		addDockWidget(Qt::LeftDockWidgetArea, dataset_ctrl);
		VIEW::ProjectDock::tabify(this, mol_ctrl, geom_ctrl, dataset_ctrl);
#else
		addDockWidget(Qt::LeftDockWidgetArea, mol_ctrl);
		addDockWidget(Qt::LeftDockWidgetArea, geom_ctrl);
		addDockWidget(Qt::TopDockWidgetArea,  dataset_ctrl);
#endif

		DatasetControl* dc = DatasetControl::getInstance(0);
		dc->registerController(new RegularData3DController());
		dc->registerController(new TrajectoryController());
		dc->registerController(new VectorGridController());
		dc->registerController(new DockResultController());

//  NOTE: raytraceable grids have been deferred until 1.4/2.0
//		dc->registerController(new RaytraceableGridController());

#ifndef BALL_UI_V2
		// Classic-only: DatasetControl ships hidden by default in the
		// legacy layout. Under BALL_UI_V2 it lives as a tab inside
		// ProjectDock and is always-visible-as-a-tab.
		DatasetControl::getInstance(0)->hide();
#endif

		// For Demo, Tutorial, and RayTracing 
		new DemoTutorialDialog(this, ((String)tr("BALLViewDemo")).c_str());

		Path path;

		new LabelDialog(        this, ((String)tr("LabelDialog")).c_str());
		new MolecularStructure(	this, ((String)tr("MolecularStructure")).c_str());

		// Phase 999.45 — Workspace consolidation. The bottom area:
		//   BALL_UI_V2 OFF: legacy 5-dock layout — LogView +
		//                   FileObserver as separate bottom docks.
		//   BALL_UI_V2 ON : both are reused as the two tabs of the
		//                   BottomDrawer (collapsible 24px↔240px).
		//                   We still construct LogView + FileObserver
		//                   first so their objectName + ModularWidget
		//                   message-bus registration matches the legacy
		//                   path; BottomDrawer extracts their inner
		//                   widgets and hides the outer dock chrome.
		LogView* log_view = new LogView(this, ((String)tr("Logs")).c_str());
		FileObserver* file_obs = new FileObserver(this, ((String)tr("FileObserver")).c_str());

#ifdef BALL_UI_V2
		// objectName-s for WorkspaceManager preset addressing. Set
		// before the BottomDrawer constructor so the outer docks have
		// stable identities when WorkspaceManager hides them.
		log_view->setObjectName("LogView");
		file_obs->setObjectName("FileObserver");
		addDockWidget(Qt::BottomDockWidgetArea, log_view);
		addDockWidget(Qt::BottomDockWidgetArea, file_obs);
		// BottomDrawer takes ownership of presenting Logs + Files; the
		// two legacy docks remain registered ModularWidgets but their
		// outer QDockWidget shells are hidden (BottomDrawer::buildBody
		// calls hide() on them).
		BottomDrawer* drawer = new BottomDrawer(log_view, file_obs, this);
		addDockWidget(Qt::BottomDockWidgetArea, drawer);
#else
 		addDockWidget(Qt::BottomDockWidgetArea, log_view);
		addDockWidget(Qt::BottomDockWidgetArea, file_obs);
#endif

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

#ifdef BALL_UI_V2
		// Phase 999.45 — Workspace consolidation.
		//
		// Build the View > Workspace submenu (Task 9): 3 built-in
		// presets + Save current as... + dynamically populated
		// user-defined presets (rebuilt on aboutToShow).
		QMenu* windows_menu = initPopupMenu(MainControl::WINDOWS, UIOperationMode::MODE_ADVANCED);
		if (windows_menu != 0)
		{
			windows_menu->addSeparator();
			QMenu* ws_menu = windows_menu->addMenu(tr("Workspace"));
			ws_menu->setObjectName("workspaceMenu");

			VIEW::WorkspaceManager& wm = VIEW::WorkspaceManager::instance();
			QAction* default_act  = ws_menu->addAction(wm.presetDisplayName(VIEW::WorkspaceManager::Default));
			QAction* classic_act  = ws_menu->addAction(wm.presetDisplayName(VIEW::WorkspaceManager::Classic));
			QAction* focused_act  = ws_menu->addAction(wm.presetDisplayName(VIEW::WorkspaceManager::Focused));
			connect(default_act, &QAction::triggered, this, [this]() {
				VIEW::WorkspaceManager::instance().apply(VIEW::WorkspaceManager::Default, this);
			});
			connect(classic_act, &QAction::triggered, this, [this]() {
				VIEW::WorkspaceManager::instance().apply(VIEW::WorkspaceManager::Classic, this);
			});
			connect(focused_act, &QAction::triggered, this, [this]() {
				VIEW::WorkspaceManager::instance().apply(VIEW::WorkspaceManager::Focused, this);
			});

			ws_menu->addSeparator();
			QAction* save_act = ws_menu->addAction(tr("Save current as…"));
			connect(save_act, &QAction::triggered, this, [this]() {
				bool ok = false;
				const QString name = QInputDialog::getText(
				    this, tr("Save Workspace"), tr("Preset name:"),
				    QLineEdit::Normal, QString(), &ok);
				if (ok && !name.isEmpty())
				{
					VIEW::WorkspaceManager::instance().save(this, name);
				}
			});

			// Rebuild user-defined entries each time the submenu opens
			// — the set is dynamic (users can save while the app is
			// running). Strategy: remove every action AFTER save_act
			// and rebuild from WorkspaceManager::userPresets().
			connect(ws_menu, &QMenu::aboutToShow, this, [ws_menu, save_act]() {
				// Drop everything after save_act.
				const QList<QAction*> all = ws_menu->actions();
				bool past_save = false;
				for (QAction* a : all)
				{
					if (past_save) ws_menu->removeAction(a);
					if (a == save_act) past_save = true;
				}
				const QStringList user_names = VIEW::WorkspaceManager::instance().userPresets();
				if (!user_names.isEmpty()) ws_menu->addSeparator();
				for (const QString& name : user_names)
				{
					QAction* a = ws_menu->addAction(name);
					Mainframe* self = static_cast<Mainframe*>(ws_menu->parent()->parent());
					QObject::connect(a, &QAction::triggered, [self, name]() {
						VIEW::WorkspaceManager::instance().load(self, name);
					});
				}
			});
		}

		// Phase 999.45 Task 8 — status-bar workspace label (right end).
		VIEW::WorkspaceStatusLabel* ws_label = new VIEW::WorkspaceStatusLabel(this);
		statusBar()->addPermanentWidget(ws_label->label());

		// Phase 999.45 Task 5 — apply Default preset on first show.
		// (User-stored preference overrides this in main.C after the
		// migration prompt; see the [Workspace] currentPreset key.)
		VIEW::WorkspaceManager::instance().apply(VIEW::WorkspaceManager::Default, this);
#endif
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
