// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include "mainframe.h"
#include "aboutDialog.h"   // Phase 999.43: hand-coded About dialog (replaces aboutDialog.ui)
#include "demoTutorialDialog.h"
#	include "iconBrowser.h"  // Phase 999.43: dev-only icon browser

#include <BALL/VIEW/KERNEL/theme/iconRegistry.h>  // Phase 999.42: Icons::get façade
#include <BALL/VIEW/KERNEL/theme/tokens.h>  // Phase 999.41: kIconToolbar
#include <BALL/VIEW/WIDGETS/molecularStructure.h>
#include <BALL/VIEW/WIDGETS/molecularControl.h>
#include <BALL/VIEW/WIDGETS/geometricControl.h>
#include <BALL/VIEW/WIDGETS/logView.h>
#include <BALL/VIEW/WIDGETS/datasetControl.h>
#include <BALL/VIEW/WIDGETS/fileObserver.h>
// Phase 999.45 — BALLView Refresh: Workspace consolidation.
#	include <BALL/VIEW/KERNEL/workspaceManager.h>
#	include <BALL/VIEW/WIDGETS/projectDock.h>
#	include <BALL/VIEW/WIDGETS/bottomDrawer.h>
// Phase 999.44 Plan 02 — Unified Inspector mainframe wiring.
#	include <BALL/VIEW/WIDGETS/inspector/inspectorDock.h>
#	include <BALL/VIEW/WIDGETS/inspector/inspectorView.h>
#	include <BALL/VIEW/WIDGETS/inspector/selectionAdapter.h>
#	include <BALL/VIEW/WIDGETS/inspector/representationAdapter.h>
#	include <BALL/VIEW/KERNEL/legacySettingsHelper.h>
// Phase 999.46 — CommandPalette (Cmd/Ctrl+K) + CommandRegistry.
#	include <BALL/VIEW/WIDGETS/commandPalette.h>
#	include <BALL/VIEW/KERNEL/commandRegistry.h>
// Phase 999.47 (Handover §7) — WelcomeScreen + ballview:// scheme.
#	include <BALL/VIEW/WIDGETS/welcomeScreen.h>
#	include <QtCore/QFileInfo>
#	include <QtCore/QRegularExpression>
#	include <QtCore/QSet>
#	include <QtCore/QStandardPaths>
#	include <QtCore/QUrlQuery>
#	include <QtGui/QDesktopServices>
#	include <QtGui/QShortcut>
#	include <QtCore/QSettings>
#	include <QtCore/QDir>
#	include <QtWidgets/QStatusBar>
#	include <QtWidgets/QMenu>
#	include <QtWidgets/QMessageBox>
#	include <QtWidgets/QInputDialog>
#	include <QtWidgets/QLineEdit>
#	include <QtGui/QAction>
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
			, inspector_dock_(0)
			, hide_inspector_action_(0)
			, selection_adapter_(0)
			, representation_adapter_(0)
			, welcome_screen_(0)                  // Phase 999.47 §7.1
			, whats_new_shown_this_launch_(false) // Phase 999.47 §7.6
	{
		// Fixes a major problem with Qt WebEngine 5.x when being used in
		// a DockWidget (issue from the 999.40-era investigation; still
		// relevant — Qt WebEngine remains a runtime dependency via
		// VIEW/WIDGETS/HTMLPage + HTMLView and the EXTENSIONS plugins
		// PresentaBALL / Jupyter / BALLaxy).
		//
		// macOS caveat (Qt 6 / Apple Silicon): Qt::AA_DontCreateNativeWidgetSiblings
		// is known to interact poorly with QOpenGLWidget reparenting on
		// Qt 6 — the GL drawable can flicker (drop briefly) during dock
		// reflow or central-widget swap because the native NSView for the
		// GL surface is recreated rather than the sibling QWidget being
		// promoted to native. The flicker is cosmetic, brief, and only
		// fires on layout changes that move the Scene across docks
		// (rare in normal use). Removing the attribute would re-trigger
		// the WebEngine-in-DockWidget rendering corruption, so we accept
		// the GL flicker as the lesser evil for v1.7. Revisit after the
		// Phase 4b Qt 6 / QtWebEngineCore audit.
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

		// v1.7.0-rc2 UFG-08 — opt into the macOS unified
		// title-and-toolbar chrome HERE in the constructor, BEFORE any
		// setCentralWidget() call (Scene → WelcomeScreen flip later in
		// this ctor). Qt's docs require this attribute be set early so
		// the NSWindow style mask + content-layout machinery is in the
		// final shape before the central widget's first paint. The
		// original UFG-02 placement inside Mainframe::show() runs AFTER
		// the central widget was already set to WelcomeScreen and
		// re-laid the window out from under the central widget — the
		// resulting paint-timing mismatch is the root cause of UFG-08
		// (blank central area). UFG-02's intent (native NSToolbar look)
		// is preserved because the toolbar added later in show() picks
		// up the unified attribute automatically. No-op on non-mac.
		#ifdef Q_OS_MACOS
			setUnifiedTitleAndToolBarOnMac(true);
		#endif
		// make sure submenus are the first.
		//
		// Phase 999.46 — under BALL_UI_V2 the top-level order is:
		//   File · Edit · Select · View · Compute · Scripts · Window · Help
		// Insertion order into the menubar determines left-to-right
		// rendering; the legacy enum IDs are re-titled by initPopupMenu's
		// v2 branch (DISPLAY → View, MOLECULARMECHANICS → Compute,
		// TOOLS → Scripts, WINDOWS → Window). SELECT is the new
		// top-level menu.
		initPopupMenu(FILE_OPEN);
		initPopupMenu(EDIT);
		initPopupMenu(MainControl::SELECT);
		initPopupMenu(BUILD);  // v2: routes into Edit › Structure (no top-level item)
		initPopupMenu(DISPLAY);  // v2: titled "View"
		initPopupMenu(MOLECULARMECHANICS);  // v2: titled "Compute"
		initPopupMenu(TOOLS);  // v2: titled "Scripts"
	#ifdef BALL_PYTHON_SUPPORT
		initPopupMenu(TOOLS_PYTHON);
		initPopupMenu(MainControl::USER);
	#endif
		initPopupMenu(WINDOWS);  // v2: titled "Window"
		initPopupMenu(MACRO);    // v2: routes into Scripts › Macros

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

		// Phase 999.44 Plan 02 — View ▸ Hide Inspector (checkable, Ctrl+\).
		// "Hide" semantics: checked == hidden, unchecked == visible. We
		// flip into setChecked(false) on first show so the action label
		// reflects "Hide Inspector" while the dock is visible.
		{
			String desc = "Shortcut|Display|Hide_Inspector";
			hide_inspector_action_ = insertMenuEntry(
				MainControl::DISPLAY,
				(String)tr("Hide Inspector"),
				/*receiver*/ 0, /*slot*/ 0,
				desc,
				QKeySequence(QStringLiteral("Ctrl+\\")),
				UIOperationMode::MODE_ADVANCED);
			if (hide_inspector_action_ && inspector_dock_)
			{
				hide_inspector_action_->setCheckable(true);
				hide_inspector_action_->setChecked(false);  // dock visible by default
				connect(hide_inspector_action_, &QAction::toggled,
				        this, [this](bool hidden) {
					if (inspector_dock_) inspector_dock_->setVisible(!hidden);
				});
			}
		}

		insertPopupMenuSeparator(DISPLAY, UIOperationMode::MODE_ADVANCED);
		initPopupMenu(DISPLAY_VIEWPOINT);

		new MolecularFileDialog    (this, "MolecularFileDialog");
		new DownloadPDBFile        (this, "DownloadPDBFile", false);
		new DownloadElectronDensity(this, "DownloadElectronDensity", false);
		new PubChemDialog          (this, "PubChemDialog");
		new UndoManagerDialog      (this, "UndoManagerDialog");

		// Phase 999.45 — Workspace consolidation. The 3 project-related
		// controls are tabified into a single left-rail ProjectDock
		// group (Handover §5.1). (Phase 999.48 removed the BALL_UI_V2
		// build-flag gating; Phase 999.49 retired the named legacy
		// 5-dock "Classic" preset — the new Default workspace is the
		// only built-in tabified layout.)
		MolecularControl* mol_ctrl = new MolecularControl(this, ((String)tr("Structures")).c_str());
		GeometricControl* geom_ctrl = new GeometricControl(this, ((String)tr("Representations")).c_str());
		DatasetControl* dataset_ctrl = new DatasetControl(this, ((String)tr("Datasets")).c_str());

		// All 3 docks need to be in the same dock-area (left) before
		// tabifyDockWidget can group them. ProjectDock::tabify moves
		// DatasetControl off the top area into the left tab group.
		addDockWidget(Qt::LeftDockWidgetArea, mol_ctrl);
		addDockWidget(Qt::LeftDockWidgetArea, geom_ctrl);
		addDockWidget(Qt::LeftDockWidgetArea, dataset_ctrl);
		VIEW::ProjectDock::tabify(this, mol_ctrl, geom_ctrl, dataset_ctrl);

		DatasetControl* dc = DatasetControl::getInstance(0);
		dc->registerController(new RegularData3DController());
		dc->registerController(new TrajectoryController());
		dc->registerController(new VectorGridController());
		dc->registerController(new DockResultController());

//  NOTE: raytraceable grids have been deferred until 1.4/2.0
//		dc->registerController(new RaytraceableGridController());


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

		// Phase 999.44 Plan 02 — Unified Inspector dock (right rail).
		// Sub-PR 4.1 finish: registers the dock InspectorView builds in
		// 999.44 Plan 01. WorkspaceManager presets address it by the
		// inspectorDock objectName the dock sets in its constructor.
		inspector_dock_ = new VIEW::InspectorDock(this, tr("Inspector"));
		addDockWidget(Qt::RightDockWidgetArea, inspector_dock_);

		// Phase 999.44 Plan 03 — Selection tab wiring (sub-PR 4.2).
		// 1. Build the 3 Selection-tab sections (kept hidden behind
		//    the empty state until the first non-empty selection).
		// 2. Spawn the SelectionInspectorAdapter as a child
		//    ConnectionObject of this MainControl so it receives
		//    ControlSelectionMessage / NewSelectionMessage and
		//    forwards the live selection into the Inspector.
		inspector_dock_->view()->attachSelectionTab(this);
		selection_adapter_ = new VIEW::SelectionInspectorAdapter(
			this, inspector_dock_->view());

		// Phase 999.44 Plan 04 — Representation tab wiring (sub-PRs
		// 4.3 + 4.4). Lazy-attach the Representation tab; spawn the
		// RepresentationInspectorAdapter so each RepresentationMessage
		// triggers a refresh of the rep list / active Representation.
		inspector_dock_->view()->attachRepresentationTab();
		representation_adapter_ = new VIEW::RepresentationInspectorAdapter(
			this, inspector_dock_->view());
		// Phase 999.44 Plan 05 — Scene tab is wired post-scene_
		// construction (further down) because the Stage lives on the
		// Scene widget which doesn't exist yet at this point.

		setupPluginHandlers_();
		Scene::stereoBufferSupportTest();
		scene_ = new Scene(this, ((String)tr("3D View")).c_str());
		setCentralWidget(scene_);
		setAcceptDrops(true);

		// Phase 999.44 Plan 05 — Scene tab completion (sub-PR 4.5).
		// Now that the Scene + its Stage exist, attach the Scene-tab
		// sections (Camera / Lights / Stage / Stereo / Background)
		// with read-only mirror Controllers pointed at the live
		// Stage/Scene.
		if (inspector_dock_ != 0 && inspector_dock_->view() != 0)
			inspector_dock_->view()->attachSceneTab(scene_->getStage(), scene_);

		// Phase 999.47 (Handover §7.1) — WelcomeScreen panel. Constructed
		// lazily but parented to `this` so QObject ownership disposes it
		// at shutdown. Initially mounted because composite_manager_ is
		// empty at startup; swapped back to scene_ by checkMenus() once a
		// composite is loaded.
		welcome_screen_ = new VIEW::WelcomeScreen(this);
		connect(welcome_screen_, &VIEW::WelcomeScreen::openFileRequested,
		        this, &Mainframe::onWelcomeOpenFileRequested_);
		connect(welcome_screen_, &VIEW::WelcomeScreen::openFromPdbRequested,
		        this, &Mainframe::onWelcomeOpenFromPdbRequested_);
		connect(welcome_screen_, &VIEW::WelcomeScreen::recentFileRequested,
		        this, &Mainframe::onWelcomeRecentFileRequested_);
		connect(welcome_screen_, &VIEW::WelcomeScreen::sampleRequested,
		        this, &Mainframe::onWelcomeSampleRequested_);
		connect(welcome_screen_, &VIEW::WelcomeScreen::skipOnStartupToggled,
		        this, &Mainframe::onWelcomeSkipToggled_);

		loadRecentFiles_();
		installBallviewUrlHandler_();

		// Check the "skip on startup" preference. Default: surface
		// the WelcomeScreen on first launch, hidden when skip is set.
		{
			QSettings s;
			const bool skip = s.value(QStringLiteral("Onboarding/skipOnStartup"),
			                          false).toBool();
			if (!skip)
				showWelcomeScreen_();
		}

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
		{
			setMenuHint(action, (String)tr("Show informations on this version of BALLView"));
			// Phase 999.46 Task 4 (Handover §06 macOS notes + §09
			// cross-platform). Apply unconditionally; Qt is a no-op
			// on Linux/Windows and a positive relocation on macOS
			// where AboutRole moves the action into the BALLView
			// application menu (per Maintainer Q1: keep inline menubar
			// + DO use per-action standard menu roles).
			action->setMenuRole(QAction::AboutRole);
		}

#ifndef NDEBUG
		// Phase 999.43: Tools › Icon Browser. Dev-only menu entry so
		// the design team can spot-check the theme.qrc bundle without
		// firing up a debugger. Production release builds (NDEBUG)
		// don't get this menu — only the menu wiring is conditional;
		// the IconBrowser class itself always compiles so a debug
		// rebuild can flip the menu on without touching the build system.
		// (BALL_UI_V2 condition removed by 999.48 flag-removal pass.)
		description = "Shortcut|Tools|IconBrowser";
		action = insertMenuEntry(MainControl::TOOLS, (String)tr("Icon Browser (dev)"),
		                         this, SLOT(openIconBrowser()), description,
		                         QKeySequence(),
		                         UIOperationMode::MODE_ADVANCED);
		if (action)
			setMenuHint(action, (String)tr("Phase 999.43: browse theme.qrc icons (debug builds only)"));
#endif

		// Phase 999.44 Plan 02 — Tools › Legacy Settings submenu.
		// Sub-PR 4.6 finish: surfaces the 12 inventoried legacy
		// preferences-stack pages (Display / Lighting / Models / ...)
		// during the BALLView Refresh migration window. Each entry
		// opens the Preferences dialog at the matching stack page.
		// Phase 999.48 §8.8 removes the entire submenu when the legacy
		// dialogs are deleted.
		{
			QMenu* tools_menu = initPopupMenu(MainControl::TOOLS, UIOperationMode::MODE_ADVANCED);
			if (tools_menu)
			{
				tools_menu->addSeparator();
				QMenu* legacy_menu = tools_menu->addMenu(tr("Legacy Settings"));
				legacy_menu->setObjectName("legacySettingsMenu");
				const QStringList names = VIEW::LegacySettingsHelper::legacyStackNames();
				for (const QString& stack_name : names)
				{
					QAction* a = legacy_menu->addAction(
						VIEW::LegacySettingsHelper::displayName(stack_name));
					const QString captured = stack_name;
					connect(a, &QAction::triggered, this, [this, captured]() {
						openLegacySetting(captured);
					});
				}
			}
		}

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
		
		
		// Phase 999.46 — Window menu lists the built-in workspace presets
		// from 999.45 (Default / Focused — Classic retired in 999.49).
		// Selecting a preset calls WorkspaceManager::apply(...) on the
		// running Mainframe; WorkspaceManager emits presetChanged so any
		// other UI (status label, palette) stays in sync.
		{
			QMenu* window_menu = initPopupMenu(MainControl::WINDOWS, UIOperationMode::MODE_ADVANCED);
			if (window_menu)
			{
				window_menu->addSeparator();
				QMenu* presets_menu = window_menu->addMenu(tr("Workspace Preset"));
				presets_menu->setObjectName("workspacePresetsMenu");
				const auto add_preset = [this, presets_menu](VIEW::WorkspaceManager::Preset p, const QString& label) {
					QAction* a = presets_menu->addAction(label);
					a->setObjectName(QString("workspace-preset-") + label.toLower());
					connect(a, &QAction::triggered, this, [this, p]() {
						VIEW::WorkspaceManager::instance().apply(p, this);
					});
				};
				add_preset(VIEW::WorkspaceManager::Default, tr("Default"));
				add_preset(VIEW::WorkspaceManager::Focused, tr("Focused"));
			}
		}

		// Phase 999.46 §06 — Invert/Clear Selection live in the
		// top-level Select menu. (Pre-999.48 this was gated by
		// BALL_UI_V2 with an Edit-menu fallback for "Classic-preset
		// muscle memory"; 999.48 retired the flag and 999.49 retired
		// the Classic preset entirely.)
		const MainControl::PopUpID kSelectionMenu = MainControl::SELECT;

		description = "Shortcut|Edit|Invert_Selection";
		complement_selection_action_ = insertMenuEntry(kSelectionMenu, (String)tr("Invert Selection"), this,
																									 SLOT(complementSelection()), description, QKeySequence(),
																									 UIOperationMode::MODE_ADVANCED);

		description = "Shortcut|Edit|Clear_Selection";
		clear_selection_action_ = insertMenuEntry(kSelectionMenu, (String)tr("Clear Selection"), this,
																							SLOT(clearSelection()), description, QKeySequence(),
																							UIOperationMode::MODE_ADVANCED);

		// Phase 999.46 Task 5 (Handover §6.3) — CommandPalette
		// floating sheet. Construct after all menu entries are
		// registered with the CommandRegistry so the first open()
		// already shows the full command surface.
		//
		// Shortcut: Ctrl+K on Linux/Windows; Qt::CTRL translates
		// to ⌘K on macOS automatically. We use QShortcut with
		// Qt::ApplicationShortcut so the palette opens regardless
		// of which widget has focus.
		{
			auto* palette = new VIEW::CommandPalette(this);
			palette->setObjectName("commandPalette");
			auto* sc = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_K), this);
			sc->setContext(Qt::ApplicationShortcut);
			connect(sc, &QShortcut::activated, palette, &VIEW::CommandPalette::open);
		}

 		qApp->installEventFilter(this);

		setStatusbarText((String)tr("Ready."));
	}

	Mainframe::~Mainframe()
	{
		// Phase 999.44 Plan 03/04 — destroy the bus adapters before
		// the Inspector view they point at goes away (the view is
		// owned by inspector_dock_ which Qt deletes via the QWidget
		// tree).
		delete representation_adapter_;
		representation_adapter_ = 0;
		delete selection_adapter_;
		selection_adapter_ = 0;
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

		// Phase 999.47 §7.1 — surface or dismiss the WelcomeScreen
		// based on the current composite count. This is the natural
		// hook because checkMenus() runs after every composite
		// insert/remove via MainControl::checkMenus() plumbing.
		if (welcome_screen_ != 0)
		{
			if (composite_manager_.getNumberOfComposites() == 0)
			{
				QSettings s;
				const bool skip = s.value(QStringLiteral("Onboarding/skipOnStartup"),
				                          false).toBool();
				if (!skip)
					showWelcomeScreen_();
			}
			else
			{
				hideWelcomeScreen_();
			}
		}
	}

	void Mainframe::showWelcomeScreen_()
	{
		if (welcome_screen_ == 0) return;
		if (centralWidget() == welcome_screen_) return;

		// Refresh recent-files + maybe show what's new each time we
		// surface — cheap.
		welcome_screen_->setRecentFiles(recent_files_);
		maybeShowWhatsNew_();

		// Re-parent Scene off the central slot but DON'T destroy it
		// (it owns OpenGL context state). takeCentralWidget() returns
		// the previous widget without deleting it.
		QWidget* prev = takeCentralWidget();
		if (prev != welcome_screen_ && prev != 0)
		{
			// v1.7.0-rc2 UFG-08 — explicitly hide the prior central
			// widget (typically the Scene's QOpenGLWidget host). On
			// macOS the QPA composer keeps detached OpenGL surfaces
			// alive long enough to leak through the new central widget
			// on first paint; hiding pulls the surface from the
			// composition tree. Re-parenting alone is not enough.
			prev->setParent(this);
			prev->hide();
		}
		setCentralWidget(welcome_screen_);
		// v1.7.0-rc2 UFG-08 — explicit show()+raise() to force Qt to
		// schedule a paint immediately after the swap. Without this,
		// on macOS the first paint can be deferred past the next event
		// loop turn and the central area stays blank.
		welcome_screen_->show();
		welcome_screen_->raise();
		welcome_screen_->update();
	}

	void Mainframe::hideWelcomeScreen_()
	{
		if (welcome_screen_ == 0) return;
		if (centralWidget() != welcome_screen_) return;
		if (scene_ == 0) return;

		QWidget* prev = takeCentralWidget();
		if (prev != 0 && prev != scene_)
		{
			// v1.7.0-rc2 UFG-08 — hide the previous central widget
			// (the WelcomeScreen). Symmetric counterpart to the
			// hide+raise pattern in showWelcomeScreen_().
			prev->setParent(this);
			prev->hide();
		}
		setCentralWidget(scene_);
		scene_->show();
		scene_->raise();
		scene_->update();
	}

	void Mainframe::loadRecentFiles_()
	{
		QSettings s;
		recent_files_ = s.value(QStringLiteral("Onboarding/recentFiles"))
		                .toStringList();
		// Drop any entries that no longer exist on disk.
		QStringList alive;
		for (const QString& p : recent_files_)
			if (QFileInfo::exists(p))
				alive << p;
		recent_files_ = alive.mid(0, 10);
		if (welcome_screen_ != 0)
			welcome_screen_->setRecentFiles(recent_files_);
	}

	void Mainframe::rememberRecentFile_(const QString& path)
	{
		if (path.isEmpty()) return;
		recent_files_.removeAll(path);
		recent_files_.prepend(path);
		while (recent_files_.size() > 10) recent_files_.removeLast();
		QSettings s;
		s.setValue(QStringLiteral("Onboarding/recentFiles"), recent_files_);
		if (welcome_screen_ != 0)
			welcome_screen_->setRecentFiles(recent_files_);
	}

	void Mainframe::maybeShowWhatsNew_()
	{
		if (whats_new_shown_this_launch_) return;
		whats_new_shown_this_launch_ = true;

		// BALL_RELEASE_STRING is provided by build/include/BALL/CONFIG/config.h
		// (generated from cmake/config.h.in). Macro string-literal at preproc
		// time so QStringLiteral can consume it.
		const QString current = QString::fromLatin1(BALL_RELEASE_STRING);
		QSettings s;
		const QString last = s.value(
			QStringLiteral("Onboarding/lastVersion")).toString();
		if (last == current) return;

		// Persist eagerly so a crash during render still counts as
		// "shown" — better than re-showing every launch on a flaky box.
		s.setValue(QStringLiteral("Onboarding/lastVersion"), current);

		// Resolve the bundled what's-new markdown.
		try
		{
			BALL::Path p;
			BALL::String resolved = p.find("BALLView/help/whatsnew/1.7.md");
			if (!resolved.isEmpty() && welcome_screen_ != 0)
				welcome_screen_->showWhatsNew(
					QString::fromStdString(std::string(resolved)));
		}
		catch (...) { /* swallow — what's-new is best-effort */ }
	}

	void Mainframe::installBallviewUrlHandler_()
	{
		// Phase 999.47 §7.3 — in-app ballview:// scheme handler.
		// macOS LSEnvironment / CFBundleURLSchemes plumbing lives in
		// MacOSXBundleInfo.plist.in; this is the in-process side that
		// dispatches once Qt receives the URL via QFileOpenEvent /
		// QDesktopServices::openUrl.
		QDesktopServices::setUrlHandler(
			QStringLiteral("ballview"),
			this,
			"onBallviewUrlInvoked_");
	}

	void Mainframe::onWelcomeOpenFileRequested_()
	{
		MolecularFileDialog* dlg = MolecularFileDialog::getInstance(0);
		// readFiles() surfaces the QFileDialog; the openFile(String)
		// overload requires a pre-resolved filename and is the path
		// used by recent-file + sample clicks below.
		if (dlg) dlg->readFiles();
	}

	void Mainframe::onWelcomeOpenFromPdbRequested_()
	{
		// Surface the PubChem download dialog if available — the
		// PDB downloader is wired separately via downloadPDBFile.
		PubChemDialog* dlg = PubChemDialog::getInstance(0);
		if (dlg) dlg->show();
	}

	void Mainframe::onWelcomeRecentFileRequested_(const QString& path)
	{
		if (path.isEmpty()) return;
		MolecularFileDialog* dlg = MolecularFileDialog::getInstance(0);
		if (dlg && dlg->openFile(String(path.toStdString())))
			rememberRecentFile_(path);
	}

	void Mainframe::onWelcomeSampleRequested_(const QString& absolutePath)
	{
		if (absolutePath.isEmpty()) return;
		MolecularFileDialog* dlg = MolecularFileDialog::getInstance(0);
		if (dlg && dlg->openFile(String(absolutePath.toStdString())))
			rememberRecentFile_(absolutePath);
	}

	void Mainframe::onWelcomeSkipToggled_(bool skip)
	{
		QSettings s;
		s.setValue(QStringLiteral("Onboarding/skipOnStartup"), skip);
	}

	void Mainframe::onBallviewUrlInvoked_(const QUrl& url)
	{
		// ballview://open?pdb=1ubq → download via PubChem dialog.
		// ballview://open?file=... → molecular file open (validated allow-list).
		// ballview://command/<id>  → CommandRegistry dispatch (allow-list).
		//
		// v1.7 RC1 security hardening (C-1/C-2/C-3): all three branches
		// validate their input before acting. The scheme is reachable from
		// untrusted callers (browser link, mailto attachment, foreign-app
		// QDesktopServices), so every payload is treated as adversarial.
		// On rejection: log a warning + return silently (no error dialog —
		// silent failure is the right security UX for a URL-scheme handler).
		const QString host = url.host();
		const QString path = url.path();

		if (host == QStringLiteral("open"))
		{
			QUrlQuery q(url);
			const QString pdb = q.queryItemValue(QStringLiteral("pdb"));
			if (!pdb.isEmpty())
			{
				// C-3 — PDB id grammar check. RCSB ids are exactly four
				// alphanumeric chars. Reject anything else silently to
				// prevent the pre-fill hook (DownloadPDBFile::setPdbId)
				// from inheriting an attacker-controlled payload.
				static const QRegularExpression kPdbIdRe(QStringLiteral("^[A-Za-z0-9]{4}$"));
				if (!kPdbIdRe.match(pdb).hasMatch())
				{
					Log.warn() << "ballview://open?pdb= rejected: not a 4-char PDB id" << std::endl;
					return;
				}
				// Surface the PDB downloader; user confirms the fetch.
				// v1.7 RC patch (#999.47-followup) — DownloadPDBFile now
				// exposes setPdbId so the ID arrives pre-filled.
				DownloadPDBFile* dlg = DownloadPDBFile::getInstance(0);
				if (dlg)
				{
					dlg->setPdbId(String(pdb.toStdString()));
					dlg->show();
				}
				return;
			}
			const QString file = q.queryItemValue(QStringLiteral("file"));
			if (!file.isEmpty())
			{
				// C-2 — file path validation. The handler receives the raw
				// query string from an untrusted caller; validate before
				// passing through to MolecularFileDialog::openFile().
				//
				// 1. Reject UNC paths up front (Windows network shares —
				//    a UNC like \\evil-host\share\foo.pdb would cause an
				//    SMB fetch with the user's creds attached).
				if (file.startsWith(QStringLiteral("\\\\")) || file.startsWith(QStringLiteral("//")))
				{
					Log.warn() << "ballview://open?file= rejected: UNC path" << std::endl;
					return;
				}
				// 2. Canonicalize. canonicalFilePath returns empty for
				//    nonexistent paths and broken symlinks, which collapses
				//    most directory-traversal attacks.
				const QFileInfo fi(file);
				const QString canonical = fi.canonicalFilePath();
				if (canonical.isEmpty())
				{
					Log.warn() << "ballview://open?file= rejected: path does not resolve" << std::endl;
					return;
				}
				// 3. Defence-in-depth: after canonicalization the path
				//    should NOT still contain `..`. (canonicalFilePath
				//    collapses these; this catches the pathological case
				//    where a `..` literal appears in a real filename.)
				if (canonical.contains(QStringLiteral("..")))
				{
					Log.warn() << "ballview://open?file= rejected: '..' in canonical path" << std::endl;
					return;
				}
				// 4. Extension allow-list — molecular files only.
				static const QSet<QString> kMolExts = {
					QStringLiteral("pdb"),
					QStringLiteral("mol2"),
					QStringLiteral("mol"),
					QStringLiteral("sdf"),
					QStringLiteral("xyz"),
					QStringLiteral("cif"),
					QStringLiteral("mmcif"),
					QStringLiteral("hin")
				};
				const QString ext = QFileInfo(canonical).suffix().toLower();
				if (!kMolExts.contains(ext))
				{
					Log.warn() << "ballview://open?file= rejected: extension '"
					           << ext.toStdString() << "' not in molecular allow-list" << std::endl;
					return;
				}
				// 5. Readability check.
				if (!QFileInfo(canonical).isReadable())
				{
					Log.warn() << "ballview://open?file= rejected: file not readable" << std::endl;
					return;
				}
				MolecularFileDialog* dlg = MolecularFileDialog::getInstance(0);
				if (dlg) dlg->openFile(String(canonical.toStdString()));
				return;
			}
		}
		else if (host == QStringLiteral("command"))
		{
			// C-1 — Command id allow-list. The CommandRegistry is fully
			// populated with EVERY menu action of the app (Phase 999.46);
			// invoking arbitrary ids from a URL scheme is a privilege
			// escalation. We whitelist only the small set of ids that
			// make sense for a deep-link entry point (open file, recent
			// files, switch workspace, surface the palette).
			//
			// To add a new id: add it to kCommandAllowList AND verify
			// the Command has no destructive side-effect that an
			// untrusted caller could weaponize (e.g. NOT file.save,
			// NOT edit.delete, NOT preferences.reset).
			static const QSet<QString> kCommandAllowList = {
				QStringLiteral("file.open"),
				QStringLiteral("file.openRecent"),
				QStringLiteral("view.workspace.default"),
				QStringLiteral("view.workspace.focused"),
				QStringLiteral("view.showCommandPalette")
			};
			// Phase 999.46 CommandRegistry dispatch — look up the
			// Command by id and invoke its trigger; mirrors the
			// CommandPalette dispatch path at commandPalette.C:375.
			const QString id = path.startsWith('/') ? path.mid(1) : path;
			if (id.isEmpty())
			{
				return;
			}
			if (!kCommandAllowList.contains(id))
			{
				Log.warn() << "ballview://command/ rejected: id '"
				           << id.toStdString() << "' not in allow-list" << std::endl;
				return;
			}
			auto& reg = VIEW::CommandRegistry::instance();
			const QList<VIEW::Command> all = reg.all();
			for (const VIEW::Command& c : all)
			{
				if (c.id == id)
				{
					if (c.trigger)
					{
						c.trigger();
						reg.noteTriggered(c.id);
					}
					return;
				}
			}
		}
		// Unhandled — silent (caller likely external; logging would noise).
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
			tb->setIconSize(QSize(BALL::VIEW::Theme::kIconToolbar,
			                      BALL::VIEW::Theme::kIconToolbar));
			addToolBar(Qt::TopToolBarArea, tb);

			// UFG-02 / UFG-08: setUnifiedTitleAndToolBarOnMac(true) is
			// now called once in the Mainframe constructor, BEFORE the
			// first setCentralWidget(). Calling it here in show() (as
			// UFG-02's original rc2 fix did) ran after the WelcomeScreen
			// was already installed as the central widget and triggered
			// a layout invalidation that left the central area blank on
			// first paint (UFG-08). The toolbar added above still picks
			// up the unified look from the early attribute.
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
			QAction* focused_act  = ws_menu->addAction(wm.presetDisplayName(VIEW::WorkspaceManager::Focused));
			connect(default_act, &QAction::triggered, this, [this]() {
				VIEW::WorkspaceManager::instance().apply(VIEW::WorkspaceManager::Default, this);
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

		// Phase 999.44 Plan 02 — first-run BALLView Refresh migration
		// notice (Display/Model/Material moved to the Inspector). Runs
		// AFTER WorkspaceManager has settled the dock layout so the
		// user sees the Inspector dock behind the message box.
		showInspectorMigrationNoticeIfNeeded_();
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

	void Mainframe::openIconBrowser()
	{
		// Phase 999.43: open the dev-only IconBrowser dialog. Slot is
		// always present when BALL_UI_V2 is ON; the menu wiring above
		// gates the user-visible entry on !NDEBUG.
		VIEW::IconBrowser* dlg = new VIEW::IconBrowser(this);
		dlg->setAttribute(Qt::WA_DeleteOnClose, true);
		dlg->show();
	}

	void Mainframe::openLegacySetting(const QString& stackName)
	{
		// Phase 999.44 Plan 02 — Tools › Legacy Settings handler.
		// Resolves stackName → matching PreferencesEntry stack page via
		// Preferences::showStackByName, then shows the dialog. The
		// resolution is implemented inside Preferences because its
		// entries_ map is protected.
		VIEW::Preferences* prefs = getPreferences();
		if (prefs == 0)
		{
			Log.warn() << "[Mainframe::openLegacySetting] Preferences dialog not available." << std::endl;
			return;
		}

		prefs->show();
		const String target_name(stackName.toUtf8().constData());
		if (!prefs->showStackByName(target_name))
		{
			Log.warn()
				<< "[Mainframe::openLegacySetting] No PreferencesEntry stack page named '"
				<< stackName.toUtf8().constData()
				<< "' is registered; opening the default page instead." << std::endl;
		}
	}

	void Mainframe::showInspectorMigrationNoticeIfNeeded_()
	{
		// Phase 999.44 Plan 02 — one-shot migration notice on first
		// BALL_UI_V2 launch. Storage: [Inspector] firstRunMigrationNoticeShown
		// in ~/.BALLView (shared with InspectorView's INI persistence).
		QSettings s(QDir::homePath() + QStringLiteral("/.BALLView"),
		            QSettings::IniFormat);
		s.beginGroup(QStringLiteral("Inspector"));
		const bool shown = s.value(QStringLiteral("firstRunMigrationNoticeShown"),
		                           false).toBool();
		if (!shown)
		{
			QMessageBox::information(this,
				VIEW::LegacySettingsHelper::migrationNoticeTitle(),
				VIEW::LegacySettingsHelper::migrationNoticeBody());
			s.setValue(QStringLiteral("firstRunMigrationNoticeShown"), true);
		}
		s.endGroup();
		s.sync();
	}

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
