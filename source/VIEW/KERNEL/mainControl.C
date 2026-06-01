/* TRANSLATOR BALL::VIEW::MainControl

		Necessary for lupdate.
*/

// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/stageMutation.h>
#include <BALL/VIEW/KERNEL/modelInformation.h>
#include <BALL/VIEW/KERNEL/geometricObject.h>
#include <BALL/VIEW/KERNEL/theme/iconRegistry.h>  // Phase 999.42: Icons::get façade
// Phase 999.46 Task 2 — auto-register every menu QAction with the
// CommandRegistry so the Cmd/Ctrl+K palette can find them all.
#	include <BALL/VIEW/KERNEL/commandRegistry.h>
#include <BALL/VIEW/KERNEL/modularWidget.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/KERNEL/clippingPlane.h>
#include <BALL/VIEW/KERNEL/UIOperationMode.h>
#include <BALL/VIEW/DIALOGS/mainControlPreferences.h>
#include <BALL/VIEW/DIALOGS/networkPreferences.h>
#include <BALL/VIEW/DIALOGS/preferences.h>
#include <BALL/VIEW/DIALOGS/openSavePreferences.h>
#include <BALL/VIEW/DIALOGS/shortcutDialog.h>

#ifdef BALL_HAS_QTWEBENGINE
#	include <BALL/VIEW/WIDGETS/HTMLView.h>
#endif

#ifdef BALL_HAS_XDR
#	include <BALL/CONCEPT/XDRPersistenceManager.h>
#endif

#include <BALL/VIEW/WIDGETS/genericControl.h>
#include <BALL/VIEW/WIDGETS/molecularStructure.h>
#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/WIDGETS/logView.h>
#include <BALL/VIEW/KERNEL/representationCreator.h>

#include <BALL/VIEW/DATATYPE/dataset.h>
#include <BALL/VIEW/DATATYPE/standardDatasets.h>

#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/MATHS/analyticalGeometry.h>
#include <BALL/MATHS/common.h>
#include <BALL/SYSTEM/path.h>

#include <BALL/STRUCTURE/geometricTransformations.h>
#include <BALL/STRUCTURE/geometricProperties.h>

#include <BALL/SYSTEM/directory.h>
#include <BALL/CONCEPT/textPersistenceManager.h>
#include <BALL/SYSTEM/timer.h>
#include <BALL/VIEW/KERNEL/threads.h>

#include <QtWidgets/QStatusBar>  // statusbar
#include <QtWidgets/QToolTip>
#include <QtWidgets/QPushButton> // needed for preferences
#include <QtGui/QCursor>     // wait cursor
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QLabel>
#include <QtGui/QAction>
#include <QtCore/QTimer>
#include <QtCore/QEvent>

#include <algorithm> // sort
#include <chrono> // chrono::milliseconds
#include <cstdlib>
#include <thread> // this_thread::sleep_for

//       #define BALL_BENCHMARKING

using std::istream;
using std::ostream;
using std::endl;

//#define BALL_VIEW_DEBUG
#undef BALL_VIEW_DEBUG

namespace BALL
{
	namespace VIEW
	{

		// Phase 999.42: simulation_running_xpm_ array deleted — the status-bar
		// "simulation running" indicator now loads VIEW::Icons::get("actions/simulation-running")
		// (theme.qrc → lucide/activity.svg). See setup_() below.

		MainControl::MainControl(QWidget* parent, const char* name, String inifile)
			:	QMainWindow(parent),
				ConnectionObject(),
				Embeddable("MainControl"),
				about_to_quit_(false),
 				multi_threading_mode_(true),
				fragment_db_(),
				model_information_(new ModelInformation()),
				selection_(),
				control_selection_(),
				message_label_(new QLabel("" , statusBar())),
				primitive_manager_(this),
				composite_manager_(),
				shortcut_registry_(),
				main_control_preferences_(0),
				network_preferences_(0),
				preferences_dialog_(new Preferences(this, ((String)tr("BALLView Preferences")).c_str())),
				preferences_file_(),
				composites_locked_(false),
				locking_widget_(0),
				stop_simulation_(false),
				simulation_thread_(0),
				modular_widgets_(),
				simulation_icon_(0),
				working_dir_(),
				logging_file_name_("VIEW.log"),
				logging_to_file_(false),
				important_text_in_statusbar_(false),
				stop_simulation_action_(0),
				complement_selection_action_(0),
				clear_selection_action_(0),
				open_action_(0),
				preferences_action_(0),
				delete_action_(0)
		{
		#ifdef BALL_VIEW_DEBUG
			Log.error() << "new MainControl " << this << std::endl;
		#endif
			setObjectName(name);

			// store and load the INIFile from the Users homedir
			// default for UNIX/LINUX
			String home_dir;

			if (getenv("HOME") != 0)
			{
				home_dir = getenv("HOME");
			}
			else
			{
				// windows
				if (getenv("HOMEDRIVE") != 0)
				{
					home_dir += getenv("HOMEDRIVE");
				}
				if (getenv("HOMEPATH") != 0)
				{
					home_dir +=	getenv("HOMEPATH");
				}
			}

			if (home_dir != "")
			{
				inifile = home_dir + String(FileSystem::PATH_SEPARATOR) + inifile;
			}

			preferences_file_.setFilename(inifile);
			setup_();
		}

		void MainControl::setup_()
		{
			// copy the environment variable BALLVIEW_DATA_PATH to BALL_DATA_PATH
			// this has to be done here also, if it was done in main.C, no idea why!
			char*	BALLView_data_path = getenv("BALLVIEW_DATA_PATH");
			if (BALLView_data_path != 0)
			{
#ifdef BALL_OS_WINDOWS
				// Phase 5.1 carry-forward: use *A suffix explicitly. Under UNICODE
				// (which Qt 6 enables on MSVC), SetEnvironmentVariable remaps to
				// SetEnvironmentVariableW which expects LPCWSTR; our literals are
				// char*. SetEnvironmentVariableA accepts char* unconditionally.
				SetEnvironmentVariableA("BALL_DATA_PATH", BALLView_data_path);
#else
				char* key = const_cast<char*>("BALL_DATA_PATH=");
				setenv(key,BALLView_data_path,true);
#endif
			}

			bool ball_data_path_works = false;

			while (!ball_data_path_works)
			{
				try
				{
#ifdef BALL_BENCHMARKING
					Timer t;
					t.start();
#endif
					fragment_db_.setFilename("fragments/Fragments.db");
					fragment_db_.init();
#ifdef BALL_BENCHMARKING
					t.stop();
					Log.error() << "Building FragmentDB time: " << t.getClockTime() << std::endl;
#endif
					ball_data_path_works = true;
				}
				catch(Exception::GeneralException& e)
				{
					char*	BALLView_data_path = getenv("BALLVIEW_DATA_PATH");
					char*	BALL_data_path = getenv("BALL_DATA_PATH");
					String vdp, bdp;
					if (BALLView_data_path!= 0) vdp = String(BALLView_data_path);
					if (BALL_data_path!= 0) bdp = String(BALL_data_path);

					Log.error() << e << std::endl;

					QMessageBox* mbox = new QMessageBox(QMessageBox::Critical,
						tr("Critical error"),
						QString(tr("Could not read the FragmentDB data!\n")) +
						"Please check that the BALL_DATA_PATH or BALLVIEW_DATA_PATH\n" +
						"environment variable is set to the directory containing the\n" +
						"BALL or BALLView data directory (e.g. to C:\\BALL\\data).\n"+
						"Currently:\n" +
						"BALLVIEW_DATA_PATH = " + vdp.c_str() + "\n"+
						"BALL_DATA_PATH = "     + bdp.c_str() + "\n"+
						"If the problem persists, start the application with the\n"+
						"-l flag to enable logging and read the file "+
						logging_file_name_.c_str() + "." +
						"This file is created in either your home directory or\n"+
						"in the directory with this executeable.",
						QMessageBox::Close | QMessageBox::Open, this);

					mbox->button(QMessageBox::Open)->setText(tr("Choose datapath ..."));

					if(mbox->exec() == QMessageBox::Open) {
						QString new_dir = QFileDialog::getExistingDirectory(0, tr("Choose the BALL data directory"), "");
						Path p;
						p.addDataPath(ascii(new_dir));

						delete mbox;
					} else {
						throw Exception::GeneralException(__FILE__, __LINE__, ((String)tr("Datapath Error")).c_str(), ((String)tr("Could not read the FragmentDB")).c_str());	
					}
				}
			}

			preferences_file_.read();

			statusBar()->setMinimumSize(2, 25);
			statusBar()->addPermanentWidget(message_label_, 20);

			QFont font(message_label_->font());
			font.setBold(true);
			message_label_->setFont(font); 
			message_label_->setFrameShape(QLabel::NoFrame);
			message_label_->setFrameShadow(QLabel::Plain);

			connect(qApp,	SIGNAL(aboutToQuit()), this, SLOT(aboutToExit()));
			connect(menuBar(), SIGNAL(hovered(QAction*)), this, SLOT(menuItemHighlighted(QAction*)));
			menuBar()->setObjectName("MainMenuBar");

			font.setPointSize(14);
			rep_label_ = new QLabel(statusBar());
			rep_label_->setFrameShape(QLabel::NoFrame);
			rep_label_->setFrameShadow(QLabel::Plain);
			rep_label_->setFont(font); 
			rep_label_->setMaximumSize(20,20);
			rep_label_->setMinimumSize(20,20);
			rep_label_->setAlignment(Qt::AlignCenter);
			rep_label_->hide();
			rep_label_->setToolTip(tr("update of model running"));
			rep_label_->setText("M");
			statusBar()->addPermanentWidget(rep_label_, false );
			rep_label_nr_ = 0;
			rep_label_delta_ = 32;
			was_not_busy_ = false;

			render_timer_.start(300);

			simulation_icon_ = new QLabel(statusBar());
			simulation_icon_->setMaximumSize(14,20);
			simulation_icon_->setMinimumSize(14,20);
			simulation_icon_->setToolTip(tr("simulation running"));
			statusBar()->addPermanentWidget(simulation_icon_, false );
			// Phase 999.42: load the icon via the SVG façade instead of the
			// removed simulation_running_xpm_ array. The status-bar widget is
			// 14x20 logical px (fixed by the surrounding setMaximumSize call),
			// so we pixmap the icon at the matching small size.
			simulation_icon_->setPixmap(Icons::get("actions/simulation-running").pixmap(14, 14));
			simulation_icon_->hide();
			simulation_icon_->setFrameShape(QLabel::NoFrame);
			simulation_icon_->setFrameShadow(QLabel::Plain);

			connect(&timer_, SIGNAL(timeout()), SLOT(clearStatusBarText_()));
			connect(&render_timer_, SIGNAL(timeout()), SLOT(updateRepLabel_()));
		}


		MainControl::MainControl(const MainControl& main_control)
			:	QMainWindow(0),
				ConnectionObject(main_control),
				Embeddable(main_control),
				about_to_quit_(false),
				model_information_(new ModelInformation()),
				selection_(),
				main_control_preferences_(0),
				network_preferences_(0),
				preferences_dialog_(new Preferences(this, ascii(tr("BALLView Preferences")).c_str())),
				composites_locked_(false),
				locking_widget_(0),
				stop_simulation_action_(0),
				complement_selection_action_(0),
				clear_selection_action_(0),
				open_action_(0),
				preferences_action_(0),
				delete_action_(0)
		{
			setup_();
		}

		MainControl::~MainControl()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << (void *)this << " of class MainControl" << endl;
			#endif 

			delete model_information_;

			clear();
		}

		QMenu* MainControl::initPopupMenu(int ID, UIOperationMode::OperationMode mode)
		{
			if (UIOperationMode::instance().getMode() > mode) return 0;
			
			if (id_to_menu_.has(ID)) return id_to_menu_[ID];

			QMenu* menu = 0;

			#ifdef BALL_VIEW_DEBUG
				Log.info() << "new menu entry: " << ID << endl;	
			#endif
			switch (ID)
			{
				case FILE:
					menu = addMenu(tr("&File"), UIOperationMode::MODE_ADVANCED);
					break;
				case FILE_OPEN:
					menu = initPopupMenu(FILE, UIOperationMode::MODE_ADVANCED);
					if (menu)
						menu = menu->addMenu(tr("&Open"));
					break;
				case FILE_OPEN_GRID:
					menu = initPopupMenu(FILE_OPEN, UIOperationMode::MODE_ADVANCED);
					if (menu)
						menu = menu->addMenu(tr("&Grids"));
					break;
				case FILE_IMPORT:
					menu = initPopupMenu(FILE, UIOperationMode::MODE_ADVANCED);
					if (menu)
						menu = menu->addMenu(tr("&Import"));
					break;
				case FILE_EXPORT:
					menu = initPopupMenu(FILE, UIOperationMode::MODE_ADVANCED);
					if (menu)
						menu = menu->addMenu(tr("&Export"));
					break;
				case FILE_MONITOR:
					menu = initPopupMenu(FILE, UIOperationMode::MODE_ADVANCED);
					if (menu)
						menu = menu->addMenu(tr("&Monitor File"));
					break;
				case EDIT:
					menu = addMenu(tr("&Edit"), UIOperationMode::MODE_ADVANCED);
					break;
				case BUILD:
					// Phase 999.46 §06-phase-menus.md — Build › Add Hydrogens
					// / Generate Peptide / Generate Crystal move under
					// Edit › Structure. Reuse the same BUILD id so existing
					// callers don't change; redirect at construction.
					menu = initPopupMenu(EDIT, UIOperationMode::MODE_ADVANCED);
					if (menu)
						menu = menu->addMenu(tr("&Structure"));
					break;
				case DISPLAY:
					// Phase 999.46 — Display renames to View (Handover §06).
					// All existing DISPLAY callers land in View.
					menu = addMenu(tr("&View"), UIOperationMode::MODE_ADVANCED);
					break;
				case DISPLAY_VIEWPOINT:
					menu = initPopupMenu(DISPLAY, UIOperationMode::MODE_ADVANCED);
					if (menu)
						menu = menu->addMenu(tr("&Viewpoint"));
					break;
				case DISPLAY_CREATE:
					menu = initPopupMenu(DISPLAY, UIOperationMode::MODE_ADVANCED);
					if (menu)
						menu = menu->addMenu(tr("&Create"));
					break;
				case DISPLAY_STEREO:
					menu = initPopupMenu(DISPLAY, UIOperationMode::MODE_ADVANCED);
					if (menu)
						menu = menu->addMenu(tr("&Stereo"));
					break;
				case DISPLAY_ANIMATION:
					menu = initPopupMenu(DISPLAY, UIOperationMode::MODE_ADVANCED);
					if (menu)
						menu = menu->addMenu(tr("&Animation"));
					break;
#ifdef BALL_HAS_RTFACT
				case DISPLAY_CONTINUOUSLOOP:
					menu = initPopupMenu(DISPLAY, UIOperationMode::MODE_ADVANCED);
					if (menu)
						menu = menu->addMenu("&Continuous Loop");
					break;
#endif
				case MOLECULARMECHANICS:
					// Phase 999.46 — Molecular Mechanics top-level renames
					// to Compute (Handover §06). Minimize / Dynamics / Abort
					// land in Compute via the existing MOLECULARMECHANICS
					// callers.
					menu = addMenu(tr("&Compute"), UIOperationMode::MODE_ADVANCED);
					break;
				case CHOOSE_FF:
					menu = initPopupMenu(MOLECULARMECHANICS, UIOperationMode::MODE_ADVANCED);
					if (menu)
						menu = menu->addMenu(tr("Force Field"));
					break;
				case TOOLS:
					// Phase 999.46 — Tools top-level renames to Scripts
					// (Handover §06). All existing TOOLS callers land in
					// Scripts. The Tools › Legacy Settings submenu wired
					// in 999.44 Plan 02 also re-roots under Scripts;
					// rename to "Legacy Settings" stays correct.
					menu = addMenu(tr("&Scripts"), UIOperationMode::MODE_ADVANCED);
					break;
				case TOOLS_GRID:
					menu = NULL;
					if (UIOperationMode::instance().getMode() <= UIOperationMode::MODE_ADVANCED)
					{
						menu = initPopupMenu(TOOLS, UIOperationMode::MODE_ADVANCED);
						if (menu)
							menu = menu->addMenu(tr("&Grids"));
					}
					break;
				case WINDOWS:
					// Phase 999.46 — Windows → Window (single word) per
					// Handover §06.
					menu = addMenu(tr("&Window"), UIOperationMode::MODE_ADVANCED);
					break;
				case MACRO:
					// Phase 999.46 — Macros top-level retargets to
					// Scripts › Macros (Handover §06). Existing MACRO
					// callers (testFramework.C: macro record/run/abort)
					// land in the submenu without per-callsite edits.
					menu = initPopupMenu(TOOLS, UIOperationMode::MODE_ADVANCED);
					if (menu)
						menu = menu->addMenu(tr("Macros"));
					break;
				case HELP:
					menu = addMenu(tr("&Help"), UIOperationMode::MODE_KIOSK);
					break;
				// Phase 999.46 — new top-level Select menu (Handover §06,
				// 8-menu remap). Holds Invert / Clear / By-Expression.
				case SELECT:
					menu = addMenu(tr("&Select"), UIOperationMode::MODE_ADVANCED);
					break;
				// Phase 999.46 — explicit submenu IDs for callers that
				// want to target them directly (otherwise the convenience
				// re-routing via BUILD / etc. covers most use).
				case EDIT_STRUCTURE:
					menu = initPopupMenu(EDIT, UIOperationMode::MODE_ADVANCED);
					if (menu)
						menu = menu->addMenu(tr("&Structure"));
					break;
				case VIEW_WORKSPACE:
					menu = initPopupMenu(DISPLAY, UIOperationMode::MODE_ADVANCED);
					if (menu)
						menu = menu->addMenu(tr("&Workspace"));
					break;
				case COMPUTE_ENERGY:
					menu = initPopupMenu(MOLECULARMECHANICS, UIOperationMode::MODE_ADVANCED);
					if (menu)
						menu = menu->addMenu(tr("&Energy"));
					break;
				case SCRIPTS_MACROS:
					menu = initPopupMenu(TOOLS, UIOperationMode::MODE_ADVANCED);
					if (menu)
						menu = menu->addMenu(tr("Macros"));
					break;
				default:
					return 0;
			}

			if (menu)
			{
                if (!RTTI::isKindOf<QMenu>(menu->parent()))
				{
					connect(menu, SIGNAL(aboutToShow()), this, SLOT(checkMenus()));
				}

				id_to_menu_[ID] = menu;
				menu->setObjectName(menu->title());
			}

			return menu;
		}

		void MainControl::clearData()
		{
			if (simulation_thread_ != 0)
			{
				if (simulation_thread_->isRunning())
				{
					simulation_thread_->terminate();
					simulation_thread_->wait();
				}

				delete simulation_thread_;
				simulation_thread_ = 0;
			}

			selection_.clear();

			while (getRepresentationManager().getRepresentations().size() > 0)
			{
				remove(**getRepresentationManager().begin());
			}

			while (getCompositeManager().getComposites().size() > 0)
			{
				remove(**getCompositeManager().begin());
			}
		}

		void MainControl::clear()
		{
			selection_.clear();
			primitive_manager_.clear();
			composite_manager_.clear();
			shortcut_registry_.clear();

			if (simulation_thread_ != 0)
			{
				if (simulation_thread_->isRunning())
				{
					simulation_thread_->terminate();
					simulation_thread_->wait();
				}

				delete simulation_thread_;
				simulation_thread_ = 0;
			}
		}
			
		void MainControl::show()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "MainControl::show()  list.size() = " << modular_widgets_.size() << endl;
			#endif

			// prevent multiple inserting of menu entries, by calls of showFullScreen(), ...
			if (preferences_action_ != 0) 
			{
				QMainWindow::show();
				return;
			}

			init_();
			QMainWindow::show();
		}

		void MainControl::init_()
		{
			// connect ok button in Preferences dialog to slot
			connect(preferences_dialog_, SIGNAL(accepted()), this, SLOT(okPreferencesClicked_()));
			
			// connect apply button in Preferences dialog to slot
			connect(preferences_dialog_, SIGNAL(applied()), this, SLOT(applyPreferencesClicked_()));

			// initialize own preferences tab
			initializePreferencesTab_();

			// initialize all modular widgets 
			list<ModularWidget*>::iterator it = modular_widgets_.begin();
			for (; it != modular_widgets_.end(); ++it)
			{
#ifdef BALL_VIEW_DEBUG
				(*it)->dump();
#endif
				(*it)->initializeWidget(*this);
				(*it)->initializePreferencesTab(*preferences_dialog_);
			}

			preferences_dialog_->showEntry(main_control_preferences_);

			// own menu entries
			insertPopupMenuSeparator(MainControl::FILE, UIOperationMode::MODE_ADVANCED);

			String description = "Shortcut|File|Quit";
			QAction* quit_action = insertMenuEntry(MainControl::FILE, (String)tr("&Quit"), qApp,
											SLOT(quit()), description,
											QKeySequence("Ctrl+Q"), UIOperationMode::MODE_ADVANCED);
			// Phase 999.46 Task 4 (Handover §06 macOS notes + §09
			// cross-platform). Apply unconditionally — Qt is a
			// no-op on Linux/Windows and a positive relocation on
			// macOS where the standard role makes Qt move the
			// action into the BALLView application menu (per
			// Maintainer Q1: keep inline menubar but DO use
			// per-action standard roles for the 3 standard items).
			if (quit_action) quit_action->setMenuRole(QAction::QuitRole);

			// if the preferences dialog has any tabs then show it
			if (preferences_dialog_->hasPages())
			{
				insertPopupMenuSeparator(MainControl::EDIT, UIOperationMode::MODE_ADVANCED);

				String description = "Shortcut|Edit|Preferences";
				preferences_action_ = insertMenuEntry(MainControl::EDIT, (String)tr("Preferences"), preferences_dialog_,
																							SLOT(show()), description,
																							QKeySequence("Ctrl+Z"), UIOperationMode::MODE_ADVANCED);

				if (preferences_action_)
				{
					preferences_action_->setIcon(Icons::get("categories/preferences"));
					// Phase 999.46 Task 4 — see Quit comment above.
					preferences_action_->setMenuRole(QAction::PreferencesRole);
				}
			}

			fetchPreferences(preferences_file_);
			applyPreferences();
		}

		void MainControl::closeEvent(QCloseEvent* /*evt*/)
		{
			quit();
		}

		void MainControl::checkMenus()
		{
			if (preferences_action_ == 0) return;

			// preferences dialog not empty
			if (preferences_dialog_ != 0 &&
					preferences_dialog_->hasPages())
			{
				preferences_action_->setEnabled(!compositesAreLocked());
			}

			// overridden in Controls
			if (delete_action_ != 0) delete_action_->setEnabled(false);

			// checks all modular widgets 
			list<ModularWidget*>::iterator it = modular_widgets_.begin();
			for (; it != modular_widgets_.end(); ++it)
			{
				(*it)->checkMenu(*this);
			}

			if (stop_simulation_action_ != 0)
			{
				stop_simulation_action_->setEnabled(simulation_thread_ != 0);
			}

			if (complement_selection_action_ != 0)
			{
				complement_selection_action_->setEnabled(!composites_locked_);
			}

			if (clear_selection_action_ != 0)
			{
				clear_selection_action_->setEnabled(!composites_locked_);
			}

			if (id_to_menu_.has(FILE_OPEN))
			{
				initPopupMenu(FILE_OPEN)->setEnabled(!composites_locked_);
			}

			if (preferences_action_ == 0) return;
		}


		void MainControl::aboutToExit()
		{
		#ifdef BALL_VIEW_DEBUG
			Log.error() << "MainControl::aboutToExit()" << std::endl;
		#endif
			about_to_quit_ = true;

			stopSimulation();

			// write the preferences
			preferences_file_.clear();
			writePreferences(preferences_file_);

			list<ModularWidget*>::iterator it = modular_widgets_.begin();
			for (; it != modular_widgets_.end(); ++it)
			{
//   				(*it)->finalizePreferencesTab(*preferences_dialog_);
				(*it)->finalizeWidget(*this);
			}


			for (it = modular_widgets_.begin(); it != modular_widgets_.end(); ++it)
			{
				unregisterConnectionObject(**it);
			}

			if (simulation_thread_ != 0)
			{
				if (simulation_thread_->isRunning()) 
				{
					simulation_thread_->terminate();
					simulation_thread_->wait();
				}
				delete simulation_thread_;
				simulation_thread_ = 0;
			}

			modular_widgets_.clear();

			delete preferences_dialog_;
			preferences_dialog_ = 0;
		#ifdef BALL_VIEW_DEBUG
			Log.error() << "MainControl::aboutToExit() finished" << std::endl;
		#endif
		}


		bool MainControl::remove_(Composite& composite, bool update_representations_of_parent, bool to_delete)
		{
			// delete all representations containing the composite
			primitive_manager_.removedComposite(composite, update_representations_of_parent);

			getSelection().erase(&composite);

			// remove childs of composite from selection
			//
			// v1.7.x-12 (delete-crash fix) — this purge was previously gated
			// on `!composite.isSelected()`. That was safe only while selecting
			// a composite did NOT also select its descendants. Since the
			// v1.7.x-13 selection sync calls selectCompositeRecursive(), a
			// selected protein carries ALL its chains/residues/atoms in the
			// selection set too. With the old guard, deleting that protein
			// (composite.isSelected() == true) SKIPPED the descendant purge,
			// leaving every descendant as a DANGLING pointer in the selection
			// set once `delete &composite` freed them — a use-after-free that
			// crashed in CompositeManager::remove the next time the selection
			// was iterated. Purge descendants whenever any are selected,
			// regardless of whether the composite itself is selected.
			if (composite.containsSelection())
			{
				HashSet<Composite*>::Iterator cit = getSelection().begin();
				list<Composite*> to_remove;
				for (; +cit; ++cit)
				{
					if ((**cit).isDescendantOf(composite))
					{
						to_remove.push_back(*cit);
					}
				}

				list<Composite*>::iterator lit = to_remove.begin();
				for (; lit != to_remove.end(); ++lit)
				{
					getSelection().erase(*lit);
				}
			}

			// delete the Composite
			composite_manager_.remove(composite, to_delete);

			return true;
		}

		// e.g. is called for root of items from picking, or for MolecularControl Selection
		bool MainControl::updateRepresentationsOf(const Composite& composite, bool rebuild, bool force)
		{
			// update all representations containing the composite
			list<Representation*> changed_representations = primitive_manager_.getRepresentationsOf(composite);

			list<Representation*>::iterator reps_it = changed_representations.begin();
			// notify GeometricControl of changed representations
			for (; reps_it != changed_representations.end(); reps_it++)
			{
				Representation* rep = *reps_it;

				if (model_information_->modelMustBeRebuild(rep->getModelType()) || force)
				{
					rep->update(rebuild);
				}
				else
				{
					rep->update(false);
				}
			}

			return changed_representations.size() != 0;
		}


		void MainControl::redrawAllRepresentations(bool rebuild_display_lists)
		{
			// update scene
			if (rebuild_display_lists)
			{
				notify_(new SceneMessage(SceneMessage::REBUILD_DISPLAY_LISTS));
			}
			else
			{
				notify_(new SceneMessage(SceneMessage::REDRAW));
			}
		}


		void MainControl::onNotify(Message *message)
		{
  		#ifdef BALL_VIEW_DEBUG
				Log.error() << "MainControl " << this << " onNotify " << message << std::endl;
   		#endif

            if (RTTI::isKindOf<CompositeMessage>(message))
			{
				CompositeMessage* cmessage = RTTI::castTo<CompositeMessage>(*message);
				switch(cmessage->getType())
				{
					case CompositeMessage::NEW_COMPOSITE:
						composite_manager_.insert(*cmessage->getComposite());
						return;
					case CompositeMessage::REMOVED_COMPOSITE:
						remove_(*cmessage->getComposite(), cmessage->updateRepresentations(), true);
						return;
					case CompositeMessage::CHANGED_COMPOSITE_HIERARCHY:
					case CompositeMessage::CHANGED_COMPOSITE:
						if (cmessage->updateRepresentations())
						{
							const bool force = cmessage->getType() == CompositeMessage::CHANGED_COMPOSITE_HIERARCHY;
							updateRepresentationsOf(cmessage->getComposite()->getRoot(), true, force);
						}
						return;
					case CompositeMessage::SELECTED_COMPOSITE:
					case CompositeMessage::DESELECTED_COMPOSITE:
					{
						bool selected = (cmessage->getType() == CompositeMessage::SELECTED_COMPOSITE);
						if (selected)
						{
							selectCompositeRecursive(cmessage->getComposite(), true);
						}
						else
						{
							deselectCompositeRecursive(cmessage->getComposite(), true);
						}

						if (cmessage->updateRepresentations())
						{
							updateRepresentationsOf(*cmessage->getComposite(), false);
						}

						// send to MolecularControl
						notify_(new NewSelectionMessage);
					}

					default:
						return;
				}
			}
            else if (RTTI::isKindOf<ControlSelectionMessage> (message))
			{
				ControlSelectionMessage* selection_message = RTTI::castTo<ControlSelectionMessage>(*message);
				control_selection_ = selection_message->getSelection();
			}
            else if (RTTI::isKindOf<GeometricObjectSelectionMessage>(message))
			{
				GeometricObjectSelectionMessage* selection_message = 
					RTTI::castTo<GeometricObjectSelectionMessage>(*message);
				selectComposites_(*selection_message);
			}
            else if (RTTI::isKindOf<RepresentationMessage>(message))
			{
				RepresentationMessage* msg = RTTI::castTo<RepresentationMessage>(*message);
				Representation* rep = msg->getRepresentation();
				switch (msg->getType())
				{
					case RepresentationMessage::ADD:
						primitive_manager_.insert(*rep, false);
						break;

					case RepresentationMessage::REMOVE:
						primitive_manager_.remove(*rep, false);
						break;

					case RepresentationMessage::FINISHED_UPDATE:
						primitive_manager_.finishedUpdate_(rep);
						break;
			
					default:
						break;
				}	
			}
            else if (RTTI::isKindOf<FinishedSimulationMessage>(message))
			{
				stopedSimulation_();
			}

			// enable the buttons of our preferences dialog again if everything important was done
			if (!updateOfRepresentationRunning())
			{
				setPreferencesEnabled_(true);
			}
		}


		// VIEW automatic module registration
		MainControl* MainControl::getMainControl(const QObject* object)
		{
			QObject* parent = object->parent();
			while ((parent != 0) && (parent->parent() != 0))
			{
				parent = parent->parent();
			}

			MainControl* mc = 0;
			if (parent != 0)
			{
				// check whether the top-level widget is a MainControl
				mc = dynamic_cast<MainControl*>(parent);
			}
			else 
			{
				// try whether the widget itself is the main control
				mc = dynamic_cast<MainControl*>(const_cast<QObject*>(object));
			}

			#ifdef BALL_DEBUG_VIEW
				Log.info() << "Top level widget : mc = " << mc << endl;
			#endif

			return mc;
		}

		QAction* MainControl::insertMenuEntry(Position parent_id, const String& name,        const QObject* receiver, 
																		      const char* slot,   const String& description, QKeySequence accel,
																					UIOperationMode::OperationMode minimal_mode)
		{
			if (UIOperationMode::instance().getMode() > minimal_mode)
			{
				return 0;
			}

			QMenu* popup = initPopupMenu(parent_id);
			if (popup == 0)
			{
				Log.error() << "MainControl::insertMenuEntry: cannot find popup menu for ID " << parent_id << endl;
				Log.error() << "Requested menu name/description was: " << name << " / " << description << std::endl;
				return 0;
			}

			QAction* action;
			if(slot && receiver)
			{
				// Qt 6: addAction signature changed — shortcut moved from
				// last to second argument (text, shortcut, receiver, slot).
				action = popup->addAction(name.c_str(), accel, receiver, slot);
			}
			else
			{
				action = popup->addAction(name.c_str());
				action->setShortcut(accel);
			}

			action->setObjectName(name.c_str());

			if (description != "")
				shortcut_registry_.registerShortcut(description, action);

			// Phase 999.46 Task 2 — auto-register every menu QAction
			// with the CommandRegistry so the Cmd/Ctrl+K command
			// palette (Task 5) sees it without any per-callsite
			// changes. Category = parent-menu title with the Qt
			// mnemonic ampersand stripped. Plugins can re-bucket
			// post-registration via
			// CommandRegistry::overrideCategory(action, new_cat).
			QString category = popup->title();
			category.remove(QLatin1Char('&'));
			if (category.isEmpty()) category = QStringLiteral("Uncategorized");
			CommandRegistry::instance().registerFromAction(action, category);

			return action;
		}

		QMenu* MainControl::addMenu(const QString& title, UIOperationMode::OperationMode mode)
		{
			QMenu* menu = 0;
			if (UIOperationMode::instance().getMode() <= mode)
			{
				menu = menuBar()->addMenu(title);
			}

			return menu;
		}

		void MainControl::removeMenuEntry(Index parent_id, QAction* action)
		{
			QMenu* popup = initPopupMenu(parent_id);
			if (popup == 0) return;
			popup->removeAction(action);
		}

		void MainControl::insertPopupMenuSeparator(int ID, UIOperationMode::OperationMode mode)
		{
			if (UIOperationMode::instance().getMode() > mode)
				return;

			QMenu* popup = initPopupMenu(ID);
			if (popup == 0)
			{
				Log.error() << "MainControl::insertMenuEntry: cannot find popup menu for ID " << ID << endl;
				return;
			}
			popup->addSeparator();
		}

		void MainControl::initializePreferencesTab_()
		{
			main_control_preferences_ = new MainControlPreferences();

			// General
			preferences_dialog_->insertEntry(main_control_preferences_);
			main_control_preferences_->enableLoggingToFile(logging_to_file_);
			
			// Shortcuts
			preferences_dialog_->insertEntry(new ShortcutDialog(this));

			// Open/Save
			open_save_preferences_ = new OpenSavePreferences();
			preferences_dialog_->insertEntry(open_save_preferences_);

			// Network Preferences
			network_preferences_ = new NetworkPreferences();
			preferences_dialog_->insertEntry(network_preferences_);
		}

		void MainControl::okPreferencesClicked_()
		{	
			if (isBusy()) 
			{
				setStatusbarText((String)tr("Cant apply preferences while being busy!"), true);
				return;
			}

			preferences_dialog_->close();
			applyPreferencesClicked_();
		}


		void MainControl::applyPreferencesClicked_()
		{
			if (isBusy()) 
			{
				setStatusbarText((String)tr("Cant apply preferences while beeing busy!"), true);
				return;
			}

			setPreferencesEnabled_(false);
			applyPreferences();

			if (!updateOfRepresentationRunning())
			{
				setPreferencesEnabled_(true);
			}
		}

		void MainControl::applyPreferences()
		{
			// MainControls own preferences
			
			if (main_control_preferences_ != 0)
			{
				main_control_preferences_->setStyle();
				QApplication::setFont(main_control_preferences_->getFont());

				if(main_control_preferences_->getLanguageChanged()) {
					main_control_preferences_->resetLanguageChanged();
					QMessageBox::information(this, tr("Language Changed"),
					                               tr("Changing the language only takes effect after restarting BALLView."));
				}

				QWidget::update();

				if (!main_control_preferences_->loggingToFileEnabled()) 
				{
					disableLoggingToFile();
				}
				else 	
				{
					enableLoggingToFile();
				}

#ifdef BALL_HAS_QTWEBENGINE
				for (unsigned int i = 0; i < HTMLViewDock::countInstances(); ++i)
				{
					HTMLViewDock::getInstance(i)->resetHTMLView(main_control_preferences_->getSkipDriverChecks());
				}
#endif
			}

			if (network_preferences_ != 0)
			{
				network_preferences_->applySettings();
			}

			// all other preferences
			list<ModularWidget*>::iterator it = modular_widgets_.begin();
			for (; it != modular_widgets_.end(); ++it)
			{
				(*it)->applyPreferences();
				(*it)->checkMenu(*this);
			}

			preferences_dialog_->applyPreferences();
		}

		void MainControl::fetchPreferences(INIFile &inifile)
		{
			if (inifile.hasEntry("WINDOWS", "File::working_dir"))
			{
				setWorkingDir(inifile.getValue("WINDOWS", "File::working_dir"));
			}

			if (inifile.hasEntry("MAIN", "Font"))
			{
				QFont font;
				font.fromString(inifile.getValue("MAIN", "Font").c_str());
				if (main_control_preferences_ != 0)
				{
					main_control_preferences_->setFont(font);
				}
			}

			//restoreWindows(inifile);
			
			preferences_dialog_->fetchPreferences(inifile);

			// check menu entries, fetch and apply preferences
			list<ModularWidget*>::iterator it = modular_widgets_.begin();
			for (; it != modular_widgets_.end(); ++it)
			{
				(*it)->fetchPreferences(inifile);
			}
		}

		void MainControl::writePreferences(INIFile &inifile)
		{
			inifile.appendSection("MAIN");
			VersionInfo version;
			String version_string(version.getMajorRevision());
			version_string += "." + String(version.getMinorRevision());
			inifile.insertValue("MAIN", "Version", version_string);

			// the font 
			if (main_control_preferences_ != 0)
			{
				inifile.insertValue("MAIN", "Font", ascii(main_control_preferences_->getFont().toString()));
			}

			Index x = pos().x();
			Index y = pos().y();

			// the main window position
			inifile.appendSection("WINDOWS");
			inifile.insertValue("WINDOWS", "Main::x", String(x));
			inifile.insertValue("WINDOWS", "Main::y", String(y));
			inifile.insertValue("WINDOWS", "Main::width", String(size().width()));
	 		inifile.insertValue("WINDOWS", "Main::height", String(size().height()));
			inifile.insertValue("WINDOWS", "File::working_dir", working_dir_);

			QByteArray ba = saveState();
			ba = ba.toBase64();
			inifile.insertValue("WINDOWS", "Main::dockwidgets", ba.data());

			// finalizes all modular widgets
			list<ModularWidget*>::iterator it = modular_widgets_.begin();
			for (; it != modular_widgets_.end(); ++it)
			{
				(*it)->writePreferences(inifile);
			}

			preferences_dialog_->writePreferences(inifile);

			inifile.write();
		}

		void MainControl::addModularWidget(ModularWidget* widget)
		{
			#ifdef BALL_DEBUG_VIEW
				Log.info() << "MainControl::addModularWidget(" << widget << ")" << endl;
			#endif
			widget->registerThis();
			list<ModularWidget*>::iterator it = modular_widgets_.begin();
			for (; it != modular_widgets_.end(); it++)
			{
				if (*it == widget) return;
			}
			modular_widgets_.push_back(widget);
			registerConnectionObject(*widget);
		}

		void MainControl::removeModularWidget(ModularWidget* widget)
		{
			#ifdef BALL_DEBUG_VIEW
				Log.info() << "MainControl::removeModularWidget(" << widget << ")" << endl;
			#endif
			modular_widgets_.remove(widget);
			unregisterConnectionObject(*widget);
		}


		void MainControl::selectComposites_(GeometricObjectSelectionMessage& message)
		{
			HashSet<Composite*> roots;

			list<GeometricObject*>& objects = const_cast<list<GeometricObject*>&>(message.getSelection());
			list<GeometricObject*>::iterator it_objects = objects.begin();
			
			const bool to_select = message.isSelected();

			// use a function pointer to (de)selectCompositeRecursive
			void (MainControl::*func_ptr) (Composite*, bool);

			if (to_select) func_ptr = &MainControl::selectCompositeRecursive;
			else 					 func_ptr = &MainControl::deselectCompositeRecursive;

			for (; it_objects != objects.end(); it_objects++)
			{
				Composite* composite = const_cast<Composite*>((**it_objects).getComposite());

				if (composite != 0  && composite->isSelected() != to_select)
				{	
					Bond* const bond = dynamic_cast<Bond*>(composite);
					if (bond !=0)
					{
						Atom* first  = (Atom*) bond->getFirstAtom();
						roots.insert(&first->getRoot());
						
						/*
						// select the atoms only if exactly one bond was selected
 						if (objects.size() == 1)
 						{
 							Atom* second = (Atom*) bond->getSecondAtom();
 							(this->*func_ptr)(first, true);
 							(this->*func_ptr)(second, true);
 						}
						*/
					}
					else
					{
						roots.insert(&composite->getRoot());
						(this->*func_ptr)(composite, true);
					}
				}
			}

			printSelectionInfos();

			HashSet<Composite*>::Iterator it = roots.begin();
			for(; it != roots.end(); it++)
			{
 				updateRepresentationsOf(**it, false);
			}

			notify_(new NewSelectionMessage);
		}


		void MainControl::printSelectionInfos()
		{
			if (selection_.size() > 4)
			{
				setStatusbarText(String(selection_.size()) + (String)tr(" objects selected."), true);
				return;
			}

			vector<PreciseTime> times;
			Atom* atoms[4];
			Size nr_of_atoms = 0;
			HashSet<Composite*>::Iterator it = selection_.begin();
			while (it != selection_.end() && 
                         RTTI::isKindOf<Atom>(*it) &&
						 nr_of_atoms < 5)
			{
				atoms[nr_of_atoms] = dynamic_cast<Atom*>(*it);
				times.push_back((dynamic_cast<Atom*>(*it))->getSelectionTime());
				nr_of_atoms++;
				it++;
			}

			sort(times.begin(), times.end());
			vector<Atom*> ordered_atoms;

			for (Position o = 0; o < nr_of_atoms; o++)
			{
				for (Position p = 0; p < nr_of_atoms; p++)
				{
					if (atoms[p]->getSelectionTime() == times[o])
					{
						ordered_atoms.push_back(atoms[p]);
						break;
					}
				}
			}
			
			switch(nr_of_atoms)
			{
				case 0:
				{
					setStatusbarText(QString(), true);
					return;
				}
				case 1:
				{
					// if one atom was picked, show its properties
					Atom& atom = *atoms[0];
					String text = String(tr("Properties of atom")) + " " + atom.getFullName() + "  " + (String)tr("Type") + ": " + 
													 String(atom.getType()) + "  " + String(tr("Typename")) + ": " + 
													 String(atom.getTypeName()) + ":  " + String(tr("Position")) + ": (" + 
													 String(atom.getPosition().x) + "|" +
													 String(atom.getPosition().y) + "|" +
													 String(atom.getPosition().z) + ")" + "  " + String(tr("Charge")) + ": " + 
													 String(atom.getCharge());
					setStatusbarText(text, true);
					break;
				}
				case 2:
				{
					// if two atoms were picked, show their distance
					String text = String(tr("Distance between atom ")) + 
														atoms[0]->getFullName() + " " + String(tr("and")) + " " + 
														atoms[1]->getFullName() + ": " + 
														String(GetDistance(atoms[0]->getPosition(), atoms[1]->getPosition()));

					Bond* bond = atoms[0]->getBond(*atoms[1]);
					if (bond != 0)
					{
						text += String(", ") + String(tr("Bond order")) + ": ";
						if (bond->getOrder() != Bond::ORDER__AROMATIC) 
						{
							text += String(bond->getOrder());
						}
						else 
						{
							text += String(tr("aromatic"));
						}
					}
					setStatusbarText(text, true);
					break;
				}
				case 3:
				{
					Vector3 vector1(ordered_atoms[1]->getPosition() - ordered_atoms[2]->getPosition());
					Vector3 vector2(ordered_atoms[1]->getPosition() - ordered_atoms[0]->getPosition());
					Angle result;
					GetAngle(vector1, vector2, result);
					String text = String(tr("Angle between atoms ")) + 
														atoms[0]->getFullName() + ", " + 
														atoms[1]->getFullName() + ", " +
														atoms[2]->getFullName() + ": " +
														String(result.toDegree()); 
					setStatusbarText(text, true);
					break;
				}
				case 4:
				{
					// if tree atoms were picked, show their torsion angle
					Angle result = getTorsionAngle(
							ordered_atoms[0]->getPosition().x, ordered_atoms[0]->getPosition().y, ordered_atoms[0]->getPosition().z,
							ordered_atoms[1]->getPosition().x, ordered_atoms[1]->getPosition().y, ordered_atoms[1]->getPosition().z,
							ordered_atoms[2]->getPosition().x, ordered_atoms[2]->getPosition().y, ordered_atoms[2]->getPosition().z,
							ordered_atoms[3]->getPosition().x, ordered_atoms[3]->getPosition().y, ordered_atoms[3]->getPosition().z);

					String text = String(tr("Torsion angle between atoms ")) + 
														ordered_atoms[0]->getFullName() + ", " + 
														ordered_atoms[1]->getFullName() + ", " +
														ordered_atoms[2]->getFullName() + ", " +
														ordered_atoms[3]->getFullName() + ": " +
														String(result.toDegree());
					setStatusbarText(text, true);
					break;
				}
			}
		}

		System* MainControl::getSelectedSystem()
		{
			if (getCompositeManager().getComposites().size() == 0) return 0;

			if (getCompositeManager().getComposites().size() == 1) 
			{
				return dynamic_cast<System*>(*getCompositeManager().getComposites().begin());
			}

			if (control_selection_.size())
			{
				return dynamic_cast<System*>(&(**control_selection_.begin()).getRoot());
			}

			return 0;
		}


		void MainControl::selectCompositeRecursive(Composite* composite, bool first_call)
		{
			composite->select();
			selection_.insert(composite);

			Atom* const atom = dynamic_cast<Atom*>(composite);
			if (atom != 0)
			{
				AtomBondIterator bi;		
				BALL_FOREACH_ATOM_BOND(*atom, bi)
				{
					if (selection_.has(bi->getPartner(*atom)))
					{
						bi->select();			
					}				
				}				
			}		
			else 
			{
				Composite* child = composite->getFirstChild();
				while (child != 0)
				{
					selectCompositeRecursive(child, false);
					child = child->getSibling(1);
				}
			}		

			if (first_call)
			{
				// try to reduce the selection to the upmost selected entries:
				Composite* uppest = composite;
				Composite* parent = composite->getParent();
				while (parent != 0 && !selection_.has(parent))
				{
					// if one child of the parent is not selected:
					// abort on the current level
					Composite* child = parent->getFirstChild();
					while (child != 0)
					{
						if (!child->isSelected())
						{
							reduceSelection_(uppest);
							return;
						}
						child = child->getSibling(1);
					}
					
					selection_.insert(parent);
					parent->select();
					uppest = parent;
					parent = parent->getParent();
				}

				reduceSelection_(uppest);
			}
		}

		void MainControl::reduceSelection_(Composite* const composite)
		{
			Composite* child = composite->getFirstChild();
			while (child != 0)
			{
				if (selection_.erase(child))
				{
					reduceSelection_(child);
				}
				child = child->getSibling(1);
			}
		}


		void MainControl::deselectCompositeRecursive(Composite* composite, bool first_call)
		{
			composite->deselect();
			selection_.erase(composite);

			Atom* const atom = dynamic_cast<Atom*>(composite);
			if (atom != 0)
			{
				AtomBondIterator bi;		
				BALL_FOREACH_ATOM_BOND(*atom, bi)
				{
					bi->deselect();			
				}				
			}		
			else
			{
				Composite* child = composite->getFirstChild();
				while (child != 0)
				{
					deselectCompositeRecursive(child, false);
					child = child->getSibling(1);
				}
			}		

			if (first_call)
			{
				Composite* parent = composite->getParent();
				while (parent != 0)
				{
					// was the parent in the selection?
					if (!selection_.erase(parent)) return;

					// parent is no longer fully selected:
					// maybe we have to add some of its childs?
					Composite* child2 = parent->getFirstChild();
					while (child2 != 0)
					{
						if (child2->isSelected()) selection_.insert(child2);
						child2 = child2->getSibling(1);
					}

					parent = parent->getParent();
				}
			}
		}

		void MainControl::setStatusbarText(const String& text, bool important, bool beep)
		{
			setStatusbarText(QString(text.c_str()), important, beep);
		}

		void MainControl::setStatusbarText(const QString& text, bool important, bool beep)
		{
#ifdef BALL_VIEW_DEBUG
			Log.error() << text << std::endl;
#endif
			if (message_label_->text() == text) return;

			if (beep) QApplication::beep();

			if (!important && important_text_in_statusbar_)
			{
				return;
			}

			important_text_in_statusbar_ = important;

			if (important)
			{
				setTextColor(message_label_, ColorRGBA(255,0,0));
 				Log.info() << ascii(text) << std::endl;
			}
			else
			{
				setTextColor(message_label_, ColorRGBA(0,0,0));
			}

			message_label_->setText(text);
			timer_.start(6000);
		}

		void MainControl::clearStatusBarText_()
		{
			if (important_text_in_statusbar_)
			{
				QPalette pal(message_label_->palette());
				pal.setColor(message_label_->foregroundRole(), QColor(0,0,0));
				message_label_->setPalette(pal);
				important_text_in_statusbar_ = false;
				timer_.start(6000);
			}

			message_label_->setText("");
		}

		void MainControl::restoreWindows()
		{
			restoreWindows(preferences_file_);
		}

		void MainControl::restoreWindows(const INIFile& inifile)
		{
			try
			{
				// the geometry of the main window
				int x_pos = 0;
				int y_pos = 0;
				int w = 1024;
				int h = 800;
				if (inifile.hasEntry("WINDOWS", "Main::x")) 		 x_pos = inifile.getValue("WINDOWS", "Main::x").toInt();
				if (inifile.hasEntry("WINDOWS", "Main::y")) 		 y_pos = inifile.getValue("WINDOWS", "Main::y").toInt();
				if (inifile.hasEntry("WINDOWS", "Main::height")) h = inifile.getValue("WINDOWS", "Main::height").toInt();
				if (inifile.hasEntry("WINDOWS", "Main::width"))  w = inifile.getValue("WINDOWS", "Main::width").toInt();

				x_pos = std::max(-4, x_pos);
				y_pos = std::max(-4, y_pos);
				if (String(BALL_OS) == "Darwin") 
				{
					// Don't move the window under the menu bar per default on OSX.
					// The menu bar has a fixed height of 22 pixels. Yes, always.
					y_pos = std::max(22, y_pos);
				}

				resize(w,h);
				move(QPoint(x_pos, y_pos));

				// now the dockwidgets
				if (!inifile.hasEntry("WINDOWS", "Main::dockwidgets")) return;

				String mys(inifile.getValue("WINDOWS", "Main::dockwidgets"));
				QByteArray s(mys.c_str());
				s = s.fromBase64(s);
				restoreState(s);
			}
			catch(...)
			{
				Log.error() << (String)tr("Could not restore settings of main windows") << std::endl;
			}
		}

		void MainControl::menuItemHighlighted(QAction* action)
		{
			last_highlighted_menu_entry_ = action;
			setStatusbarText(ascii(action->toolTip()));
		}

		void MainControl::dump(ostream& s, Size depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			BALL_DUMP_DEPTH(s, depth);
				ConnectionObject::dump(s, depth);
				Embeddable::dump(s, depth);
			BALL_DUMP_STREAM_SUFFIX(s);     
		}

		void MainControl::update(Composite& composite, bool changed_hierarchy)
		{
			CompositeMessage* cm = new CompositeMessage(composite, 
					CompositeMessage::CHANGED_COMPOSITE_HIERARCHY);
			if (!changed_hierarchy) cm->setType(CompositeMessage::CHANGED_COMPOSITE);

			notify_(cm);
			updateRepresentationsOf(composite.getRoot(), true, changed_hierarchy);
		}

		bool MainControl::insert(Composite& composite, String name, bool normalize)
		{
			if (!composite_manager_.insert(composite)) return false;
			CompositeMessage* cm; 
		
			if (MolecularStructure::getInstance(0) != 0)
			{
				cm = new CompositeMessage(composite, CompositeMessage::NEW_COMPOSITE);
				cm->data() = boost::any(normalize);
			}
			else
			{
				cm = new CompositeMessage(composite, CompositeMessage::NEW_MOLECULE);
				cm->data() = boost::any(normalize);
			}

			cm->setCompositeName(name);
			notify_(cm);
			
			return true;
		}

		bool MainControl::remove(Composite& composite, bool to_delete, bool update)
		{
			wait();
			control_selection_.clear();

			if (update) notify_(new CompositeMessage(composite, CompositeMessage::REMOVED_COMPOSITE));
			remove_(composite, update, to_delete);

			return true;
		}

		bool MainControl::insert(Representation& rep)
		{
			return primitive_manager_.insert(rep, true);
		}

		bool MainControl::update(Representation& rep)
		{
			if (!primitive_manager_.has(rep)) return false;

			rep.update(true);

			notify_(new RepresentationMessage(rep, RepresentationMessage::UPDATE));

			return true;
		}

		bool MainControl::remove(Representation& rep)
		{
			if (getRepresentationManager().updateRunning())
			{
				setStatusbarText((String)tr("Could not delete Representation while update is running!"), true);
				getRepresentationManager().logUpdateState("Representation delete"); // v1.7.x-29 self-diagnosis
				return false;
			}

			if (rep.hasProperty(Representation::PROPERTY__IS_COORDINATE_SYSTEM))
			{
				notify_(new SceneMessage(SceneMessage::REMOVE_COORDINATE_SYSTEM));
			}

			notify_(new RepresentationMessage(rep, RepresentationMessage::REMOVE));

			return primitive_manager_.remove(rep);
		}

		void MainControl::sendMessage(Message& message)
		{
			onNotify(&message);
			notify_(&message);
		}

		void MainControl::clearSelection()
		{
			if (getSelection().size() == 0) return;

			CompositeManager::CompositeIterator it = getCompositeManager().begin();
			for (; it != getCompositeManager().end(); it++)
			{
				deselectCompositeRecursive(*it);
			 	updateRepresentationsOf(**it, false);
			}

			getSelection().clear();

			redrawAllRepresentations(true);

			sendMessage(*new NewSelectionMessage());
		}

		void MainControl::deleteClicked()
		{
			list<ModularWidget*>::iterator it = modular_widgets_.begin();
			for (; it != modular_widgets_.end(); ++it)
			{
                if (RTTI::isKindOf<GenericControl>(*it))
				{
					(dynamic_cast<GenericControl*>(*it))->deleteCurrentItems();
				}
			}
		}

		void MainControl::insertDeleteEntry(QWidget* owner)
		{
			if (delete_action_ == 0)
			{
				delete_action_ = insertMenuEntry(MainControl::EDIT, (String)tr("Delete"), this,
												 SLOT(deleteClicked()), "Shortcut|Edit|Delete", QKeySequence::Delete,
												 UIOperationMode::MODE_ADVANCED);

				// BUG-622 (#622): scope the destructive Delete shortcut so it only
				// fires when a structure-owning widget (a GenericControl such as the
				// molecular control, or the scene) has keyboard focus. Without this
				// the action defaults to Qt::WindowShortcut and Del fires
				// window-globally — deleting selected structures even while a text
				// field or the embedded web view is focused, and swallowing the
				// Delete key from those inputs.
				if (delete_action_ != 0)
				{
					delete_action_->setShortcutContext(Qt::WidgetWithChildrenShortcut);
				}
			}

			// BUG-622 (#622): register the calling widget (and its child subtree) as a
			// focus owner for the Delete shortcut. Each GenericControl that can delete
			// items adds the (shared) action to itself, so Del resolves only when that
			// control — or a child of it — holds focus.
			if (delete_action_ != 0 && owner != 0)
			{
				owner->addAction(delete_action_);
			}
		}

		void MainControl::setDeleteEntryEnabled(bool state)
		{
			delete_action_->setEnabled(state);
		}

		void MainControl::setBusyMode_(bool state) 
		{
			checkMenus();

			if (state)
			{
				simulation_icon_->show();
			}
			else
			{
				simulation_icon_->hide();
			}
		}

		void MainControl::stopSimulation() 
		{
			if (simulation_thread_ == 0) return;

			stop_simulation_ = true;
			setStatusbarText((String)tr("Terminating calculation ..."), true);
		}

		// is called when the SimulationThread has finished
		void MainControl::stopedSimulation_()
		{
			if (simulation_thread_ != 0)
			{
				simulation_thread_->wait();
				delete simulation_thread_;
				simulation_thread_ = 0;
			}

			if (stop_simulation_)
			{
				setStatusbarText((String)tr("Calculation terminated."), true);
			}
			stop_simulation_ = false;
			unlockCompositesFor(locking_widget_);
		}


		bool MainControl::event(QEvent* e)
		{
			if (e->type() == (QEvent::Type) MESSAGE_EVENT)
			{
				Message* msg = dynamic_cast<MessageEvent*>(e)->getMessage();
				sendMessage(*msg);
				return true;
			}

			return QMainWindow::event(e);
		}


		bool MainControl::setSimulationThread(SimulationThread* thread)
		{
			if (!lockCompositesFor(0)) return false;

			simulation_thread_ = thread;
			if (thread != 0) 
			{
				thread->setMainControl(this);
			}

			checkMenus();
			setBusyMode_(thread != 0);

			return true;
		}

		SimulationThread* MainControl::getSimulationThread()
		{
			return simulation_thread_;
		}


		void MainControl::complementSelection()
		{
			CompositeManager::iterator it = getCompositeManager().begin();
			for (; it != getCompositeManager().end(); it++)
			{
				complementSelectionHelper_(**it);
				updateRepresentationsOf(**it, false);
			}

			notify_(new NewSelectionMessage);
			printSelectionInfos();
		}

		void MainControl::complementSelectionHelper_(Composite& c)
		{
			if (c.isSelected())
			{
				deselectCompositeRecursive(&c, true);
			}
			else if (!c.containsSelection())
			{
				selectCompositeRecursive(&c, true);
			}
			else
			{
				for (Position i = 0; i < c.getDegree(); i++)
				{
					complementSelectionHelper_(*c.getChild(i));
				}
			}
		}

		void MainControl::expandSelectionToResidues()
		{
			// Snapshot the current selection — we mutate selection_ below.
			HashSet<Composite*> snapshot = selection_;

			// Collect the residues touched by the current selection.
			HashSet<Composite*> residues;
			for (HashSet<Composite*>::Iterator it = snapshot.begin(); +it; ++it)
			{
				Composite* c = *it;
				Residue* res = 0;
				Atom* atom = dynamic_cast<Atom*>(c);
				if (atom != 0)
				{
					res = atom->getResidue();
				}
				else
				{
					res = dynamic_cast<Residue*>(c);
				}
				if (res != 0) residues.insert(res);
			}

			if (residues.isEmpty()) return;

			// Select every atom of each touched residue.
			for (HashSet<Composite*>::Iterator rit = residues.begin(); +rit; ++rit)
			{
				selectCompositeRecursive(*rit, true);
			}

			// Refresh the representations of every loaded root once (mirrors
			// complementSelection's update pattern), then announce the change.
			CompositeManager::iterator cit = getCompositeManager().begin();
			for (; cit != getCompositeManager().end(); cit++)
			{
				updateRepresentationsOf(**cit, false);
			}

			notify_(new NewSelectionMessage);
			printSelectionInfos();
		}

		void MainControl::enableLoggingToFile()
		{
			if (logging_to_file_) return;
			if (logging_file_name_ == "")
			{
				Log.error() << (String)tr("Empty logging file name") << std::endl;
				return;
			}

			logging_to_file_ = true;
			logging_file_.open(logging_file_name_, std::ios::out);
			Log.insert(logging_file_);

			main_control_preferences_->enableLoggingToFile(true);
		}
		
		void MainControl::disableLoggingToFile()
		{
			if (!logging_to_file_) return;
			logging_to_file_ = false;
			Log.remove(logging_file_);
			logging_file_.close();
		}

		void MainControl::setLoggingFilename(const String& string)
		{
			logging_file_name_ = string;
			if (logging_to_file_) 
			{
				disableLoggingToFile();
				enableLoggingToFile();
			}
		}

		void MainControl::setWorkingDir(const String& dir)
		{ 
			String dir2 = dir;
			// QT will return Paths on windows with "/" as delimiter!
#ifdef BALL_OS_WINDOWS
			for (Position p = 0; p < dir2.size(); p++)
			{
				if (dir2[p] == '/') dir2[p] = '\\';
			}
#endif
			Directory directory(dir2);
			if (directory.isValid())
			{
				working_dir_ = dir2;
			}
		}


		bool MainControl::lockCompositesFor(ModularWidget* widget)
		{
			if (!composites_locked_mutex_.tryLock()) return false;

			locking_widget_ = widget;
			composites_locked_ = true;
			setBusyMode_(true);
			checkMenus();
			return true;
		}

		bool MainControl::unlockCompositesFor(ModularWidget* widget)
		{
			if (!composites_locked_) return true;

			if (locking_widget_ != widget) return false;
			composites_locked_mutex_.unlock();
			composites_locked_ = false;
			setBusyMode_(false);
			checkMenus();
			return true;
		}


	void MainControl::saveBALLViewProjectFile(const String& filename, bool binary)
	{
#ifndef BALL_HAS_XDR
		if(binary) {
			Log.warn() << "BALL has been compiled without XDR support! Binary-encoded project files cannot be written."
			           << " Falling back to plain-text encoding!" << std::endl;
			binary = false;
		}
#endif
		String temp;
		File::createTemporaryFilename(temp);
		INIFile out(temp);
		out.appendSection("BALLVIEW_PROJECT");

		out.insertValue("BALLVIEW_PROJECT", "PersistenceFormat", binary ? "XDR" : "TEXT");
		getRepresentationManager().storeRepresentations(out);

		// write turning point of scene
		Scene* scene = Scene::getInstance(0);
		if (scene != 0)
		{
			String s = vector3ToString(scene->getTurnPoint());
			out.insertValue("BALLVIEW_PROJECT", "TurnPoint", s);
		}

		// BUG-621 (#621): do NOT embed the user's GLOBAL preference set into project
		// files. Previously writePreferences(out) baked the entire user INI (font,
		// working dir, network/PDB-download settings, every ModularWidget's prefs)
		// into the project, which loadBALLViewProjectFile then applied over the
		// user's live settings on every open — the PresentaBALL "every open reloads
		// settings" symptom. Global preferences now live solely in the user's own INI.
		//
		// BUT the SCENE's visual state (background color, fog, lighting, clipping,
		// projection — everything Scene::writePreferences emits) is genuine PROJECT
		// state: a project should reopen looking as it was designed, the same way its
		// camera/turn point and representations do (user choice, 999.73-04 UAT). So we
		// write ONLY the Scene widget's preferences into the project — not the whole-
		// app prefs blob — and the load side restores only those. Global prefs stay
		// protected; scene visuals travel with the project. Older project files that
		// still carry a full [preferences] block are handled by the load side reading
		// back only the Scene section.
		if (scene != 0)
		{
			scene->writePreferences(out);
		}

		INIFile::LineIterator lit = out.getLine(0);
		File result;
		bool ok = false;
		try
		{
			ok = result.open(filename, (binary) ? std::ios::out|std::ios::binary : std::ios::out);
		}
		catch(...)
		{
		}

		if (!ok)
		{
			setStatusbarText(String(tr("Can not write projectfile")) + 
											 filename + "!", true);
			return;
		}

		result << out.getNumberOfLines() << std::endl;
		result << getCompositeManager().getNumberOfComposites() << std::endl;
		for (; +lit; ++lit)
		{
			result << *lit << std::endl;
		}

		CompositeManager::CompositeIterator cit = getCompositeManager().begin();
		for (; cit != getCompositeManager().end(); cit++)
		{
            if (!RTTI::isKindOf<System>(*cit)) continue;

			if (!binary)
			{
				// hack to prevent ambiguous problem with overloaded cstr
				TextPersistenceManager pm(result.getFileStream(), result.getFileStream());
				(*dynamic_cast<System*>(*cit)) >> pm;
			}
			else
			{
#ifdef BALL_HAS_XDR
				XDRPersistenceManager pm(result.getFileStream(), result.getFileStream());
				(*dynamic_cast<System*>(*cit)) >> pm;
#endif
			}
		}

		result.close();
		File::remove(temp);
	} 

	void MainControl::loadBALLViewProjectFile(const String& filename)
	{
		if (isBusy())
		{
			setStatusbarText((String)tr("Could not load project file, while update is running!"), true);
			return; 
		}

#ifndef BALL_HAS_XDR
		Log.error() << "BALL has been compiled without XDR support. Binary-encoded project files cannot be read!"
		            << std::endl;
		return;
#endif

		File file;
		try
		{
			file.open(filename, std::ios::in|std::ios::binary);
		}
		catch(...)
		{
			setStatusbarText((String)tr("Could not open the project file") 
					             + " \"" + filename + "\", " + (String)tr("it doesnt seem to exist."), true);
			return;
		}

		Size nr_lines, nr_composites;
		file >> nr_lines;
		file >> nr_composites;
 		INIFile in;
		String buffer;
		for (Position p = 0; p <= nr_lines; p++)
		{
			if (!buffer.getline(file))
			{
				setStatusbarText((String)tr("Error while reading project file, could not read INIFile."), true);
				BALLVIEW_DEBUG
				return;
			}

			if (!in.appendLine(buffer)) 
			{
				setStatusbarText((String)tr("Error while reading project file, could not read INIFile."), true);
				BALLVIEW_DEBUG
				return;
			}
		}

		// BUG-621 (#621): a project file embeds a full copy of the user's BALLView
		// INI (see saveBALLViewProjectFile, where writePreferences used to be
		// called). Previously this restore ran fetchPreferences(in) + applyPreferences()
		// here, overwriting the user's live settings (working dir, font, the
		// preferences dialog, the PDB download URL, and every ModularWidget's prefs)
		// with the project's — often stale — copy. That is exactly the PresentaBALL
		// "every project load reloads settings" symptom.
		//
		// A project file legitimately owns only PROJECT STATE — its representations,
		// scene turn point/camera, and serialized systems — all of which are restored
		// below from the BALLVIEW_PROJECT section and the system stream, independently
		// of the user preferences. So we no longer touch user preferences on load:
		// the user's settings live solely in their own INI (preferences_file_, fetched
		// at startup and written at exit) and are never clobbered by opening a project.
		// applyPreferences() here only existed to push the just-fetched project prefs,
		// so the whole-app restore is removed; restoreRepresentations below re-renders
		// the restored project state on its own.
		//
		// EXCEPTION (999.73-04 UAT, user choice): restore ONLY the Scene widget's
		// preferences — the scene-visual state (background color, fog, lighting,
		// clipping, projection) that saveBALLViewProjectFile writes via
		// scene->writePreferences(out). This is project look, not global settings, so
		// it should travel with the project like the camera does. We fetch + apply
		// just the Scene (never MainControl's global prefs or other preference pages),
		// so a project reopens looking as designed while font/language/PDB-URL/paths
		// stay the user's. Old projects that embedded a full prefs blob still carry a
		// Scene section, so their background travels too; their other (global) sections
		// are simply ignored here.
		{
			Scene* scene_prefs = Scene::getInstance(0);
			if (scene_prefs != 0)
			{
				scene_prefs->fetchPreferences(in);
				scene_prefs->applyPreferences();
			}
		}

		// Phase 999.67 Plan 03: the suppress-during-load guard is re-homed off
		// DisplayProperties onto the non-dialog RepresentationCreator. A project
		// file restores its OWN saved representations (restoreRepresentations
		// below), so NEW_MOLECULE auto-create must stay suppressed while the
		// project loads and be re-enabled afterwards.
		bool has_rc = (RepresentationCreator::getInstance(0) != 0);

		if (has_rc)
		{
			RepresentationCreator::getInstance(0)->enableCreationForNewMolecules(false);
		}

		vector<const Composite*> new_systems;
		Position current_composite = 0;
		PersistenceManager *pm;

		// if the persistence format is not explicitly given, or if it is not explicitly
		// set to XDR, we assume TextPersistence
		bool use_xdr = false;
		if (in.hasEntry("BALLVIEW_PROJECT", "PersistenceFormat"))
		{
			if (in.getValue("BALLVIEW_PROJECT", "PersistenceFormat") == "XDR")
				use_xdr = true;
		}

		if (use_xdr)
		{
#ifdef BALL_HAS_XDR
			pm = new XDRPersistenceManager(file, file);
			pm->initializeInputStream();
			((XDRPersistenceManager*)pm)->setHandleStreamsExternally(true);
#endif
		}
		else
		{
			pm = new TextPersistenceManager(file, file);
		}

		while (file.good() && !file.eof() && current_composite < nr_composites)
		{
			PersistentObject* po = pm->readObject();
            if (!RTTI::isKindOf<System>(po))
			{
				setStatusbarText((String)tr("Error while reading project file, could not read molecule."), true);
				if (has_rc)	RepresentationCreator::getInstance(0)->enableCreationForNewMolecules(true);
				return;
			}

			System* system = dynamic_cast<System*>(po);
			if (system == 0) continue;

			insert(*system, "", false);
			new_systems.push_back(system);
			current_composite++;
		}

		if (use_xdr)
		{
#ifdef BALL_HAS_XDR
			((XDRPersistenceManager*)pm)->setHandleStreamsExternally(false);
			pm->finalizeInputStream();
#endif
		}

		delete (pm);

		file.close();
		if (has_rc) RepresentationCreator::getInstance(0)->enableCreationForNewMolecules(true);

		getRepresentationManager().restoreRepresentations(in, new_systems);

		getSelection().clear();
		notify_(new NewSelectionMessage);
 	
		if (in.hasEntry("BALLVIEW_PROJECT", "Camera"))
		{
			Stage stage;

			Scene* scene = Scene::getInstance(0);
			if (scene != 0)
			{
				stage = *Scene::getInstance(0)->getStage();
			}

			Camera c;
			if (!c.readFromString(in.getValue("BALLVIEW_PROJECT", "Camera")))
			{
				setStatusbarText((String)tr("Could not read Camera position from project."), true);
				return;
			}
			// §3b — Stage mutation flows through the single StageMutation friend.
			StageMutation(stage).camera(c);
			SceneMessage* msg = new SceneMessage(SceneMessage::UPDATE_CAMERA);
			msg->setStage(stage);
			notify_(msg);

			// read in turning point of scene
			if (in.hasEntry("BALLVIEW_PROJECT", "TurnPoint"))
			{
				Vector3 v;
				stringToVector3(in.getValue("BALLVIEW_PROJECT", "TurnPoint"), v);
				if (scene != 0)
				{
					scene->setTurnPoint(v);
				}
			}
		}

		checkMenus();
	}

	void MainControl::saveBALLViewProjectFile()
	{
		QString qresult = QFileDialog::getSaveFileName(
											0,
											String(tr("Select a project file")).c_str(),
											getWorkingDir().c_str(), 
											"*.bvp");

	 	if (qresult == QString()) return;

		String result = ascii(qresult);
		if (result.isEmpty()) return;

		if (!result.hasSuffix(".bvp")) result += ".bvp";

 		saveBALLViewProjectFile(result, open_save_preferences_->binaryProjectsEnabled());
		setStatusbarText((String)tr("Saved project to ") + result);
	} 


	void MainControl::loadBALLViewProjectFile()
	{
		QString result = QFileDialog::getOpenFileName(
																		0,
																		String(tr("Select a project file")).c_str(),
																		getWorkingDir().c_str(), 
																		"*.bvp");
		if (result.isEmpty()) return;

 		loadBALLViewProjectFile(ascii(result));
	}


	void MainControl::quickLoadConfirm()
	{
		if (isBusy()) return;

		if (QMessageBox::question(this, tr("Quickload"), tr("Do you really want to quickload?"),
					QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes)
						== QMessageBox::Yes)
		{
			quickLoad();
		}
	}


	void MainControl::setPreferencesEnabled_(bool state)
	{
		preferences_dialog_->setApplyEnabled(state);
	}

	void MainControl::resize(int w, int h)
	{
		QMainWindow::resize(w, h);
	}

	bool MainControl::useMultithreading()
	{ 
		return multi_threading_mode_;
	}

	void MainControl::processEvents(Size ms)
	{
		qApp->processEvents(QEventLoop::AllEvents, ms);
	}
				
	void MainControl::quickSave() 
	{
		setStatusbarText((String)tr("quick saving..."), true);
		processEvents(500);

		String file = Directory::getUserHomeDir();
		file += FileSystem::PATH_SEPARATOR;
		file += "quick.bvp";
		saveBALLViewProjectFile(file);

		setStatusbarText((String)tr("quick saving finished"), true);
	}

	void MainControl::quickLoad()
	{
		setStatusbarText((String)tr("quick loading..."), true);
		processEvents(500);

		while (getCompositeManager().getComposites().size() > 0)
		{
			remove(**getCompositeManager().begin());
		}

		while (getRepresentationManager().getRepresentations().size() > 0)
		{
			remove((**getRepresentationManager().getRepresentations().begin()));
		}

		String file = Directory::getUserHomeDir();
		file += FileSystem::PATH_SEPARATOR;
		file += "quick.bvp";
		loadBALLViewProjectFile(file);
		setStatusbarText((String)tr("quick loading finished"), true);
	}

	void MainControl::updateRepLabel_()
	{
		bool busy = isBusy();
		if (was_not_busy_ && !busy) return;
		setPreferencesEnabled_(!busy);

		if (!busy)
		{
			QCursor* cursor = QApplication::overrideCursor();
			if (cursor != 0 &&
					cursor->shape() == Qt::BusyCursor)
			{
				QApplication::restoreOverrideCursor();
			}
			setTextColor(rep_label_, getColor(rep_label_));
			was_not_busy_ = true;
			return;
		}

		rep_label_->show();
		if (rep_label_nr_ + rep_label_delta_ > 255 || (Index)rep_label_nr_ + rep_label_delta_ < 0) 
		{
			rep_label_delta_ *= -1;
		}

		rep_label_nr_ += rep_label_delta_;

		setTextColor(rep_label_, ColorRGBA(Size(rep_label_nr_ / 3) ,rep_label_nr_ / 3,(Size)rep_label_nr_));

		QCursor* cursor = QApplication::overrideCursor();
		{
			if (cursor == 0) QApplication::setOverrideCursor(QCursor(Qt::BusyCursor));
		}
		was_not_busy_ = false;
	}

	bool MainControl::isBusy() const
	{
		return (composites_locked_ || primitive_manager_.updateRunning());
	}
		
	void MainControl::setContentSize(int w, int h)
	{
		Size height = menuBar()->height();
		height += statusBar()->height();
		resize(w, h + height);
	}

	void MainControl::wait()
 	{
		while (isBusy())
		{
			QApplication::processEvents();
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	}

	void MainControl::toggleFullScreen()
	{
		setWindowState(windowState() ^ Qt::WindowFullScreen);
	}

	void MainControl::openFile(const String& file) 
	{
		// drag and drop operations can result in addition emtpy file names,
		// ignore these:
		if (file == "") return;
		// workaround for drag and drop under windows: damn QT sometimes adds slashes!
		
		String filename(file);
#ifdef BALL_OS_WINDOWS
		if (filename[0] == '/') 
		{
			filename.trimLeft("/");
		}
#endif

		vector<String> fields;
		filename.split(fields, ". ");
		if (fields.size() < 2) 
		{
			Log.error() << (String)tr("Could not open file ") 
									<< file << ", " + (String)tr("because no file extension exists.") << std::endl;
			return;
		}

		if (composites_locked_) return;

		setStatusbarText((String)tr("Opening file ") + filename + "...");

		if (filename.hasSuffix(".bvp"))
		{
			loadBALLViewProjectFile(filename);
			return;
		}

		String ext = fields[fields.size() - 1];
		list<ModularWidget*>::iterator it = modular_widgets_.begin();
		for (; it != modular_widgets_.end(); ++it)
		{
			if ((**it).canHandle(ext))
			{
				(**it).openFile(filename);
				return;
			}
		}

		Log.error() << (String)tr("Dont know how to open ")
								<< file << (String)tr(". This extension is not supported.") << std::endl;
	}

	QMenu* MainControl::createPopupMenu()
	{
		if (UIOperationMode::instance().getMode() <= UIOperationMode::MODE_ADVANCED)
		{
			return QMainWindow::createPopupMenu();
		}

		return NULL;
	}

	void MainControl::setModelInformation(ModelInformation* mi)
	{
		if (mi == 0) return;
		delete model_information_;
		model_information_ = mi;
	}

	const ModelInformation& MainControl::getModelInformation() const
	{
		return *model_information_;
	}

#	ifdef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/VIEW/KERNEL/mainControl.iC>
#	endif

		} // namespace VIEW
} // namespace BALL
