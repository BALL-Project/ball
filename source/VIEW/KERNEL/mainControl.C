// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: mainControl.C,v 1.166 2005/02/27 17:55:45 amoll Exp $
//

#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/geometricObject.h>
#include <BALL/VIEW/KERNEL/modularWidget.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/DIALOGS/mainControlPreferences.h>
#include <BALL/VIEW/DIALOGS/preferences.h>

#include <BALL/VIEW/WIDGETS/logView.h>
#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/WIDGETS/geometricControl.h>
#include <BALL/VIEW/WIDGETS/molecularStructure.h>
#include <BALL/VIEW/DIALOGS/displayProperties.h>

#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/MATHS/analyticalGeometry.h>
#include <BALL/MATHS/common.h>

#include <BALL/STRUCTURE/geometricTransformations.h>
#include <BALL/STRUCTURE/geometricProperties.h>

#include <BALL/SYSTEM/directory.h>
#include <BALL/CONCEPT/textPersistenceManager.h>
#include <BALL/SYSTEM/timer.h>

#ifdef BALL_QT_HAS_THREADS
#	include <BALL/VIEW/KERNEL/threads.h>
#endif

#include <qpopupmenu.h>	 // menus
#include <qstatusbar.h>  // statusbar
#include <qtooltip.h>
#include <qpushbutton.h> // needed for preferences
#include <qcursor.h>     // wait cursor
#include <qmessagebox.h> 
#include <qfiledialog.h> 

#include <algorithm> // sort

#include <qtimer.h>

//       #define BALL_BENCHMARKING

using std::istream;
using std::ostream;
using std::endl;

namespace BALL
{
	namespace VIEW
	{

		const char* MainControl::simulation_running_xpm_[] =
		{
				"16 14 4 1",
				"   c None",
				".  c black",
				"X  c red",
				"o  c gray50",
				"     .........  ",
				"     .XXXXXX.o  ",
				"    .XXXXXX.o   ",
				"    .XXXXX.o    ",
				"   .XXXXX.o     ",
				"   .XXXX.....   ",
				"  .XXXXXXXX.o   ",
				"  .....XXX.o    ",
				"   oo.XXX.o     ",
				"     .XX.o      ",
				"    .XX.o       ",
				"    .X.o        ",
				"   .X.o         ",
				"   ..o          "
		};        
			

		MainControl::MainControl(QWidget* parent, const char* name, String inifile)
			throw()
			:	QMainWindow(parent, name),
				ConnectionObject(),
				Embeddable("BALL::VIEW::MainControl"),
				fragment_db_(),
				selection_(),
				control_selection_(),
				message_label_(new QLabel("" , statusBar())),
				primitive_manager_(this),
				composite_manager_(),
				main_control_preferences_(0),
				preferences_dialog_(new Preferences(this, "BALLView Preferences")),
				preferences_id_(-1),
				delete_id_(0),
				preferences_(),
				composites_locked_(false),
				locking_widget_(0),
				stop_simulation_(false),
				simulation_thread_(0),
				modular_widgets_(),
				menu_entries_hints_(),
				simulation_icon_(0),
				working_dir_(),
				logging_file_name_("VIEW.log"),
				logging_to_file_(false),
				about_to_quit_(false),
				important_text_in_statusbar_(false)
		{
		#ifdef BALL_VIEW_DEBUG
			Log.error() << "new MainControl " << this << std::endl;
		#endif

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

			preferences_.setFilename(inifile);
			setup_();
		}

		void MainControl::setup_()
			throw()
		{
			// copy the environment variable BALLVIEW_DATA_PATH to BALL_DATA_PATH
			// this has to be done here also, if it was done in main.C, no idea why!
			char*	BALLView_data_path = getenv("BALLVIEW_DATA_PATH");
			if (BALLView_data_path != 0)
			{
				putenv(const_cast<char*>((String("BALL_DATA_PATH=") + String(BALLView_data_path)).c_str()));
			}

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
			}
			catch(Exception::GeneralException e)
			{
				char*	BALLView_data_path = getenv("BALLVIEW_DATA_PATH");
				char*	BALL_data_path = getenv("BALL_DATA_PATH");
				String vdp, bdp;
				if (BALLView_data_path!= 0) vdp = String(BALLView_data_path);
				if (BALL_data_path!= 0) bdp = String(BALL_data_path);

				QMessageBox::critical(0, "Critical error",
						QString("Could not read the FragmentDB data!\n") + 
						"Please check, that the BALL_DATA_PATH or BALLVIEW_DATA_PATH\n" + 
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
						QMessageBox::Abort,  QMessageBox::NoButton);
				Log.error() << e << std::endl;

				exit(-1);
			}

			preferences_.read();

			statusBar()->setMinimumSize(2, 25);
			statusBar()->addWidget(message_label_, 20);

			QFont font(message_label_->font());
			font.setBold(true);
			message_label_->setFont(font); 
			message_label_->setFrameShape(QLabel::NoFrame);

			connect(qApp,	SIGNAL(aboutToQuit()), this, SLOT(aboutToExit()));
			connect(menuBar(), SIGNAL(highlighted(int)), this, SLOT(menuItemHighlighted(int)));

			QToolTip::setWakeUpDelay(500);
			QToolTip::setGloballyEnabled(true);

			simulation_icon_ = new QLabel(statusBar());
			simulation_icon_->setMaximumSize(14,16);
			statusBar()->addWidget(simulation_icon_, 1, TRUE );
			QToolTip::add(simulation_icon_, "simulation status");
			QPixmap icon(simulation_running_xpm_);

			simulation_icon_->setPixmap(icon);
			simulation_icon_->hide();
			simulation_icon_->setFrameShape(QLabel::NoFrame);
			connect(&timer_, SIGNAL(timeout()), SLOT(clearStatusBarText_()));
		}


		MainControl::MainControl(const MainControl& main_control)
			throw()
			:	QMainWindow(0, ""),
				ConnectionObject(main_control),
				Embeddable(main_control),
				selection_(),
				main_control_preferences_(0),
				preferences_dialog_(new Preferences(this, "BALLView Preferences")),
				preferences_id_(-1),
				delete_id_(0),
				composites_locked_(false),
				locking_widget_(0),
				about_to_quit_(false)
		{
			setup_();
		}

		MainControl::~MainControl()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << (void *)this << " of class MainControl" << endl;
			#endif 

			clear();
		}

		QPopupMenu* MainControl::initPopupMenu(int ID)
			throw()
		{
			QPopupMenu* menu = 0;
			QMenuItem* item = menuBar()->findItem(ID);
			if ((item != 0) && (item->popup() != 0))
			{
				return item->popup();
			}

			menu = new QPopupMenu(this);
			CHECK_PTR(menu);

			connect(menu, SIGNAL(aboutToShow()), this, SLOT(checkMenus()));

			#ifdef BALL_VIEW_DEBUG
				Log.info() << "new menu entry: " << ID << endl;	
			#endif
			switch (ID)
			{
				case FILE:
					menuBar()->insertItem("&File", menu, FILE, FILE);
					break;
				case FILE_OPEN:
					initPopupMenu(FILE)->insertItem("&Open", menu, FILE_OPEN);
					break;
				case FILE_IMPORT:
					initPopupMenu(FILE)->insertItem("&Import", menu, FILE_IMPORT);
					break;
				case FILE_EXPORT:
					initPopupMenu(FILE)->insertItem("&Export", menu, FILE_EXPORT);
					break;
				case EDIT:
					menuBar()->insertItem("&Edit", menu, EDIT, EDIT);
					break;
				case BUILD:
					menuBar()->insertItem("&Build", menu, BUILD, BUILD);
					break;
				case DISPLAY:
					menuBar()->insertItem("&Display", menu, DISPLAY, DISPLAY);
					break;
				case DISPLAY_VIEWPOINT:
					initPopupMenu(DISPLAY)->insertItem("&Viewpoint", menu, DISPLAY_VIEWPOINT);
					break;
				case DISPLAY_STEREO:
					initPopupMenu(DISPLAY)->insertItem("&Stereo", menu, DISPLAY_STEREO);
					break;
				case DISPLAY_ANIMATION:
					initPopupMenu(DISPLAY)->insertItem("&Animation", menu, DISPLAY_ANIMATION);
					menu->setCheckable(true);
					break;
				case MOLECULARMECHANICS:
					menuBar()->insertItem("&Molecular Mechanics", menu, MOLECULARMECHANICS, -1);
					break;
				case CHOOSE_FF:
					initPopupMenu(MOLECULARMECHANICS)->insertItem("Force Field", menu, CHOOSE_FF);
					break;
				case TOOLS:
					menuBar()->insertItem("&Tools", menu, TOOLS, TOOLS);
					break;
				case TOOLS_PYTHON:
					initPopupMenu(TOOLS)->insertItem("&Python", menu, TOOLS_PYTHON);
					break;
				case WINDOWS:
					menuBar()->insertItem("&Windows", menu, WINDOWS, WINDOWS);
					break;
				case USER:
					menuBar()->insertItem("&User", menu, USER, USER);
					break;
				case HELP:
					menuBar()->insertSeparator();
					menuBar()->insertItem("&Help", menu, HELP, HELP);
					break;
				default:
					delete menu;
					menu = 0;
			}

			return menu;
		}

		void MainControl::clear()
			throw()
		{
			selection_.clear();
			primitive_manager_.clear();
			composite_manager_.clear();

			return;
			#ifdef BALL_QT_HAS_THREADS
			if (simulation_thread_ != 0)
			{
				if (simulation_thread_->running())
				{
					simulation_thread_->wait();
				}

				delete simulation_thread_;
				simulation_thread_ = 0;
			}
			#endif
		}
			
		void MainControl::show()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "MainControl::show()  list.size() = " << modular_widgets_.size() << endl;
			#endif

			// prevent multiple inserting of menu entries, by calls of showFullScreen(), ...
			if (preferences_id_ != -1) 
			{
				QMainWindow::show();
				return;
			}


			#ifdef BALL_QT_HAS_THREADS
				String hint = "Abort a running simulation thread";
				insertMenuEntry(MainControl::MOLECULARMECHANICS, "Abort Calculation", this, SLOT(stopSimulation()),
						ALT+Key_C, MENU_STOPSIMULATION, hint);
			#endif

			insertMenuEntry(MainControl::EDIT, "Toggle Selection", this, 
										SLOT(complementSelection()), 0, MainControl::MENU_COMPLEMENT_SELECTION);

			// establish connection 
			connect(preferences_dialog_->ok_button, SIGNAL(clicked()), 
					this, SLOT(applyPreferencesTab()));
			
			connect(preferences_dialog_->cancel_button, SIGNAL(clicked()), 
					this, SLOT(cancelPreferencesTab()));
			
			connect(preferences_dialog_->defaults_button, SIGNAL(clicked()), 
					this, SLOT(defaultPreferencesTab()));

			// initialize own preferences tab
			initializePreferencesTab(*preferences_dialog_);

			// initialize all modular widgets 
			List<ModularWidget*>::Iterator it = modular_widgets_.begin(); 
			for (; it != modular_widgets_.end(); ++it)
			{
				(*it)->initializeWidget(*this);
				(*it)->initializePreferencesTab(*preferences_dialog_);
			}

			// check own preferences 
			preferences_dialog_->fetchPreferences(preferences_);

			// fetch own preferences tab
			fetchPreferences(preferences_);

			// apply on own preferences tab
			applyPreferences();

			// check menu entries, fetch and apply preferences
			for (it = modular_widgets_.begin(); it != modular_widgets_.end(); ++it)
			{
				(*it)->checkMenu(*this);
				(*it)->fetchPreferences(preferences_);
				(*it)->applyPreferences();
			}

			// own menu entries
			insertPopupMenuSeparator(MainControl::FILE);
			insertMenuEntry(MainControl::FILE, "&Quit", qApp, SLOT(quit()), CTRL+Key_Q);	
			//insertMenuEntry(MainControl::HELP, "Whats this?", this, SLOT(whatsThis()));	

			// if the preferences dialog has any tabs then show it
			if (preferences_dialog_->hasPages())
			{
				initPopupMenu(MainControl::DISPLAY)->setCheckable(true);
				
				insertPopupMenuSeparator(MainControl::EDIT);
				preferences_id_ = insertMenuEntry(MainControl::EDIT, 
																					"Preferences", 
																					preferences_dialog_, 
																					SLOT(show()), CTRL+Key_Z);
			}

			restoreWindows(preferences_);
			QMainWindow::show();
		}

		void MainControl::checkMenus()
		{
			// preferences dialog not empty
			if (preferences_dialog_ != 0 &&
					preferences_dialog_->hasPages())
			{
				menuBar()->setItemChecked(preferences_id_, preferences_dialog_->isVisible());			
			}

			// overridden in Controls
			if (delete_id_ != 0) menuBar()->setItemEnabled(delete_id_, false);

			// checks all modular widgets 
			List<ModularWidget*>::Iterator it = modular_widgets_.begin(); 
			for (; it != modular_widgets_.end(); ++it)
			{
				(*it)->checkMenu(*this);
			}

			menuBar()->setItemEnabled(MENU_STOPSIMULATION, simulation_thread_ != 0);
			menuBar()->setItemEnabled(MENU_COMPLEMENT_SELECTION, !composites_locked_);
		}

		void MainControl::applyPreferencesTab()
		{
			// apply on own preferences tab
			applyPreferences();
			setPreferencesEnabled_(false);

			// checks all modular widgets 
			List<ModularWidget*>::Iterator it = modular_widgets_.begin(); 
			for (; it != modular_widgets_.end(); ++it)
			{
				(*it)->applyPreferences();
			}

			if (!updateOfRepresentationRunning())
			{
				setPreferencesEnabled_(true);
			}
		}


		void MainControl::cancelPreferencesTab()
			throw()
		{
			// checks all modular widgets 
			List<ModularWidget*>::Iterator it = modular_widgets_.begin(); 
			for (; it != modular_widgets_.end(); ++it)
			{
				(*it)->cancelPreferences();
			}
		}

		void MainControl::defaultPreferencesTab()
			throw()
		{
			// checks all modular widgets if one of their preferences is the current page
			List<ModularWidget*>::Iterator it = modular_widgets_.begin(); 
			for (; it != modular_widgets_.end(); ++it)
			{
				(*it)->defaultPreferences();
			}
		}


		void MainControl::aboutToExit()
		{
		#ifdef BALL_VIEW_DEBUG
			Log.error() << "MainControl::aboutToExit()" << std::endl;
		#endif
			about_to_quit_ = true;
			
			stopSimulation();

			// write the preferences
			preferences_.clear();
			preferences_.appendSection("WINDOWS");
			preferences_dialog_->writePreferences(preferences_);

			// finalizes all modular widgets
			List<ModularWidget*>::Iterator it = modular_widgets_.begin(); 
			for (; it != modular_widgets_.end(); ++it)
			{
				(*it)->writePreferences(preferences_);
			}

			writePreferences(preferences_);

			it = modular_widgets_.begin(); 
			for (; it != modular_widgets_.end(); ++it)
			{
				(*it)->finalizePreferencesTab(*preferences_dialog_);
				(*it)->finalizeWidget(*this);
			}


			for (it = modular_widgets_.begin(); it != modular_widgets_.end(); ++it)
			{
				unregisterConnectionObject(**it);
			}

			#ifdef BALL_QT_HAS_THREADS
				if (simulation_thread_ != 0)
				{
					if (simulation_thread_->running()) 
					{
						simulation_thread_->terminate();
						simulation_thread_->wait();
					}
					delete simulation_thread_;
					simulation_thread_ = 0;
				}
			#endif

			modular_widgets_.clear();

			// finalize own preferences tab
			finalizePreferencesTab(*preferences_dialog_);

			delete preferences_dialog_;
			preferences_dialog_ = 0;
		#ifdef BALL_VIEW_DEBUG
			Log.error() << "MainControl::aboutToExit() finished" << std::endl;
		#endif
		}


		bool MainControl::remove_(Composite& composite, bool update_representations_of_parent, bool to_delete)
			throw()
		{
			if (!composite_manager_.has(composite)) return false;
			// delete all representations containing the composite
			primitive_manager_.removedComposite(composite);

			Composite* root = 0;
			if (composite.isRoot()) root = &composite.getRoot();

			// delete the Composite
			composite_manager_.remove(composite, to_delete);

			// update all Representations
			if (root != 0 && update_representations_of_parent) 
			{
				updateRepresentationsOf(*root, true, true);
			}

			return true;
		}

		// e.g. is called for root of items from picking, or for MolecularControl Selection
		bool MainControl::updateRepresentationsOf(const Composite& composite, bool rebuild, bool force)
			throw()
		{
			if (!composite_manager_.has(composite)) return false;
			
			// update all representations containing the composite
			List<Representation*> changed_representations = primitive_manager_.getRepresentationsOf(composite);
			List<Representation*>::Iterator reps_it = changed_representations.begin();
			// notify GeometricControl of changed representations
			for (; reps_it != changed_representations.end(); reps_it++)
			{
				Representation* rep = *reps_it;

				if (rep->getModelBuildTime() < Atom::getAttributesModificationTime())
				{
					rep->update(true);
				}
				else 
				{
					if (modelMustBeRebuild(rep->getModelType()) || force)
					{
						rep->update(rebuild);
					}
					else
					{
						rep->update(false);
					}
				}
				
			#ifndef BALL_QT_HAS_THREADS
				// if build multithreaded, the Representation will send the message itself
				RepresentationMessage* ur_message = new RepresentationMessage(*rep, RepresentationMessage::UPDATE);
				notify_(ur_message);
			#endif
			}

			return true;
		}


		void MainControl::redrawAllRepresentations(bool rebuild_display_lists)
			throw()
		{
			// update scene
			SceneMessage *scene_message = new SceneMessage;
			if (rebuild_display_lists)
			{
				scene_message->setType(SceneMessage::REBUILD_DISPLAY_LISTS);
			}
			else
			{
				scene_message->setType(SceneMessage::REDRAW);
			}

			notify_(scene_message); 
		}


		void MainControl::onNotify(Message *message)
			throw()
		{
  		#ifdef BALL_VIEW_DEBUG
				Log.error() << "MainControl " << this << " onNotify " << message << std::endl;
   		#endif

			if (RTTI::isKindOf<CompositeMessage>(*message))
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
						/*
						if (cmessage->showSelectionInfos())
						{
							printSelectionInfos();
						}
						*/

						if (cmessage->updateRepresentations())
						{
							updateRepresentationsOf(*cmessage->getComposite(), false);
						}

						NewSelectionMessage* nws_message = new NewSelectionMessage;					
						notify_(nws_message); // send to MolecularControl
					}

					default:
						return;
				}
			}
			else if (RTTI::isKindOf<ControlSelectionMessage> (*message))
			{
				ControlSelectionMessage* selection_message = RTTI::castTo<ControlSelectionMessage>(*message);
				control_selection_ = selection_message->getSelection();
			}
			else if (RTTI::isKindOf<GeometricObjectSelectionMessage>(*message))
			{
				GeometricObjectSelectionMessage* selection_message = 
					RTTI::castTo<GeometricObjectSelectionMessage>(*message);
				selectComposites_(*selection_message);
			}
			else if (RTTI::isKindOf<TransformationMessage> (*message))
			{
				if (compositesAreLocked()) return;
				moveItems((dynamic_cast<TransformationMessage*>(message))->getMatrix());
			}
			else if (RTTI::isKindOf<RepresentationMessage>(*message))
			{
				RepresentationMessage* msg = RTTI::castTo<RepresentationMessage>(*message);
				switch (msg->getType())
				{
					case RepresentationMessage::ADD:
						primitive_manager_.insert(*msg->getRepresentation(), false);
						break;

					case RepresentationMessage::REMOVE:
						primitive_manager_.remove(*msg->getRepresentation(), false);
						break;

					default:
						break;
				}	
			}
		}


		// VIEW automatic module registration
		MainControl* MainControl::getMainControl(const QObject* object)
			throw()
		{
			QObject* parent = object->parent();
			while ((parent != 0) && (parent->parent() != 0))
			{
				parent = parent->parent();
			}

			MainControl* mc = 0;
			if (parent != 0)
			{
				// check whether the top-level widget
				// is a MainControl
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

		int MainControl::current_id_ = 15000;

		int MainControl::insertMenuEntry(int parent_id, const String& name, const QObject* receiver, const char* slot, 
																		 int accel, int entry_ID, String hint)
			throw()
		{
			QMenuBar* menu_bar = menuBar();
			if (menu_bar == 0) return -1;
			
			// enable the corresponding popup menu
			menu_bar->setItemEnabled(parent_id, true);
			QPopupMenu* popup = initPopupMenu(parent_id);
			if (popup == 0)
			{
				Log.error() << "MainControl::insertMenuEntry: cannot find popup menu for ID " << parent_id << endl;
				return -1;
			}

			if (entry_ID == -1) entry_ID = getNextID_();
			popup->insertItem(name.c_str(), receiver, slot, accel, entry_ID);

			setMenuHint(entry_ID, hint);

			return entry_ID;
		}


		void MainControl::removeMenuEntry
			(int parent_id, const String& /* name */, 
			 const QObject* /* receiver */, const char* /* slot */, 
			 int /* accel */, int entry_ID)
			throw()
		{
			if (about_to_quit_) return;

			QMenuBar* menu_bar = menuBar();
			if (menu_bar == 0) return;
			
			QMenuItem* item = menuBar()->findItem(parent_id);
			if ((item == 0) || (item->popup() == 0) || entry_ID == -1) return;
			QPopupMenu* popup = item->popup();

			popup->removeItem(entry_ID);
		}


		void MainControl::insertPopupMenuSeparator(int ID)
			throw()
		{
			QMenuBar* menu_bar = menuBar();
			if (menu_bar != 0)
			{
				// enable the corresponding popup menu
				menu_bar->setItemEnabled(ID, true);
				//
				QPopupMenu* popup = initPopupMenu(ID);
				if (popup == 0)
				{
					Log.error() << "MainControl::insertMenuEntry: cannot find popup menu for ID " << ID << endl;
				}
				else
				{
					popup->insertSeparator();
				}
			}
		}


		void MainControl::initializePreferencesTab(Preferences &preferences)
			throw()
		{
			main_control_preferences_ = new MainControlPreferences();

			preferences.insertEntry(main_control_preferences_);
			preferences.showEntry(main_control_preferences_);

			main_control_preferences_->enableLoggingToFile(logging_to_file_);
		}

		void MainControl::finalizePreferencesTab(Preferences &preferences)
			throw()
		{
			if (main_control_preferences_ != 0)
			{
				preferences.removeEntry(main_control_preferences_);
				main_control_preferences_ = 0;
			}
		}

		void MainControl::applyPreferences()
			throw()
		{
			if (main_control_preferences_ != 0)
			{
				QApplication::setStyle(main_control_preferences_->getStyle());
				BALL_VIEW_DOCKWINDOWS_SHOW_LABELS = main_control_preferences_->showLabelsEnabled();
				QWidget::update();

				if (!main_control_preferences_->loggingToFileEnabled())
					disableLoggingToFile();
				else 	
					enableLoggingToFile();
			}
		}

		void MainControl::fetchPreferences(INIFile &inifile)
			throw()
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

			if (inifile.hasEntry("WINDOWS", "File::working_dir"))
			{
				setWorkingDir(inifile.getValue("WINDOWS", "File::working_dir"));
			}

			if (inifile.hasEntry("WINDOWS", "File::enable_logging_file"))
			{
				enableLoggingToFile();
			}

			resize(QSize(w,h));
			move(QPoint(x_pos, y_pos));

			restoreWindows(inifile);
		}

		void MainControl::writePreferences(INIFile &inifile)
			throw()
		{
			// the main window position
			inifile.insertValue("WINDOWS", "Main::x", String(x()));
			inifile.insertValue("WINDOWS", "Main::y", String(y()));
			inifile.insertValue("WINDOWS", "Main::width", String(width()));
	 		inifile.insertValue("WINDOWS", "Main::height", String(height()));
			inifile.insertValue("WINDOWS", "File::working_dir", working_dir_);

			if (logging_to_file_) 
			{
				inifile.insertValue("WINDOWS", "File::enable_logging_file", 1);
			}

			QString s;
			QTextStream stream( &s, IO_ReadWrite);
			stream.setEncoding(QTextStream::Latin1);
			stream << *this;
			String mys(s.data());
			for (Position p = 0; p < mys.size(); p++)
			{
				if (mys[p] == '\n') 
				{
					mys[p] = '*';
				}
			}
			inifile.insertValue("WINDOWS", "Main::dockwidgets", mys);

			inifile.write();
		}

		void MainControl::addModularWidget(ModularWidget* widget)
			throw()
		{
			#ifdef BALL_DEBUG_VIEW
				Log.info() << "MainControl::addModularWidget(" << widget << ")" << endl;
			#endif
			widget->registerThis();
			List<ModularWidget*>::Iterator it = modular_widgets_.begin();
			for (; it != modular_widgets_.end(); it++)
			{
				if (*it == widget) return;
			}
			modular_widgets_.push_back(widget);
			registerConnectionObject(*widget);
		}

		void MainControl::removeModularWidget(ModularWidget* widget)
			throw()
		{
			#ifdef BALL_DEBUG_VIEW
				Log.info() << "MainControl::removeModularWidget(" << widget << ")" << endl;
			#endif
			modular_widgets_.remove(widget);
			unregisterConnectionObject(*widget);
		}


		void MainControl::selectComposites_(GeometricObjectSelectionMessage& message)
			throw()
		{
			HashSet<Composite*> roots;

			Size nr = 0;
			List<GeometricObject*>& objects = const_cast<List<GeometricObject*>&>(message.getSelection());
			List<GeometricObject*>::Iterator it_objects = objects.begin();

			if (message.isSelected())
			{
				for (; it_objects != objects.end(); it_objects++)
				{
					Composite* composite = const_cast<Composite*>((**it_objects).getComposite());

					if (composite != 0  && !composite->isSelected())
					{	
						nr++;
						roots.insert(&composite->getRoot());
						if (RTTI::isKindOf<Bond>(*composite))
						{
							composite->select();
						}
						else
						{
							selectCompositeRecursive(composite , true);
						}
					}
				}
			}
			else
			{
				for (; it_objects != objects.end(); it_objects++)
				{
					Composite* composite = const_cast<Composite*>((**it_objects).getComposite());

					if (composite != 0  && composite->isSelected())
					{	
						nr++;
						roots.insert(&composite->getRoot());
						if (RTTI::isKindOf<Bond>(*composite))
						{
							composite->deselect();
						}
						else
						{
							deselectCompositeRecursive(composite , true);
						}
					}
				}
			}

			printSelectionInfos();

			HashSet<Composite*>::Iterator it = roots.begin();
			for(; it != roots.end(); it++)
			{
				updateRepresentationsOf(**it, false);
			}

			#ifdef BALL_DEBUG_VIEW
				Log.info() << "Selected " + String(nr) + " items."<< std::endl;
			#endif

			NewSelectionMessage* new_message = new NewSelectionMessage;
			notify_(new_message);
		}


		void MainControl::printSelectionInfos()
			throw()
		{
			if (selection_.size() > 4)
			{
				setStatusbarText(String(selection_.size()) + " objects selected.", true);
				return;
			}

			vector<PreciseTime> times;
			Atom* atoms[4];
			Size nr_of_atoms = 0;
			HashSet<Composite*>::Iterator it = selection_.begin();
			while (it != selection_.end() && 
						 RTTI::isKindOf<Atom>(**it) && 
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
					setStatusbarText("0 objects selected.", true);
					return;
				}
				case 1:
				{
					// if one atom was picked, show its properties
					Atom& atom = *atoms[0];
					String text = String("Properties of atom ") + atom.getFullName() + "  Type: " + 
													 String(atom.getType()) + "  Typename: " + 
													 String(atom.getTypeName()) + ":  Position: (" + 
													 String(atom.getPosition().x) + "|" +
													 String(atom.getPosition().y) + "|" +
													 String(atom.getPosition().z) + ")" + "  Charge: " + 
													 String(atom.getCharge());
					setStatusbarText(text, true);
					break;
				}
				case 2:
				{
					// if two atoms were picked, show their distance
					String text = String("Distance between atom ") + 
														atoms[0]->getFullName() + " and " + 
														atoms[1]->getFullName() + ": " + 
														String(GetDistance(atoms[0]->getPosition(), atoms[1]->getPosition()));
					setStatusbarText(text, true);
					break;
				}
				case 3:
				{
					Vector3 vector1(ordered_atoms[1]->getPosition() - ordered_atoms[2]->getPosition());
					Vector3 vector2(ordered_atoms[1]->getPosition() - ordered_atoms[0]->getPosition());
					Angle result;
					GetAngle(vector1, vector2, result);
					String text = String("Angle between atoms ") + 
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

					String text = String("Torsion angle between atoms ") + 
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
			throw()
		{
			if (control_selection_.size() != 1)
			{
				return 0;
			}
			
			return dynamic_cast<System*>(*control_selection_.begin());
		}


		void MainControl::selectCompositeRecursive(Composite* composite, bool first_call)
			throw()
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
				Composite* uppest = composite;
				Composite* parent = composite->getParent();
				while (parent != 0 && !selection_.has(parent))
				{
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
				selection_.erase(child);
				reduceSelection_(child);
				child = child->getSibling(1);
			}
		}


		void MainControl::deselectCompositeRecursive(Composite* composite, bool first_call)
			throw()
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
					selection_.erase(parent);
					parent = parent->getParent();
				}
			}
		}


		void MainControl::setStatusbarText(const String& text, bool important, bool beep)
			throw()
		{
#ifdef BALL_VIEW_DEBUG
			Log.error() << text << std::endl;
#endif
			if (message_label_->text().ascii() == text.c_str()) return;

			if (beep) 
			{
				QApplication::beep();
			}

			if (!important && important_text_in_statusbar_)
			{
				return;
			}

			important_text_in_statusbar_ = important;

			message_label_->setText(text.c_str());
			if (important)
			{
				message_label_->setPaletteForegroundColor( QColor(255,0,0) );
				Log.info() << text << std::endl;
			}
			else
			{
				message_label_->setPaletteForegroundColor( QColor(0,0,0) );
			}

			timer_.start(6000);
		}

		void MainControl::clearStatusBarText_()
		{
			if (important_text_in_statusbar_)
			{
				message_label_->setPaletteForegroundColor( QColor(0,0,0) );
				important_text_in_statusbar_ = false;
				timer_.start(6000);
			}

			message_label_->setText("");
		}

		void MainControl::restoreWindows(const INIFile& inifile)
			throw()
		{
			if (!inifile.hasEntry("WINDOWS", "Main::dockwidgets")) return;

			String mys(inifile.getValue("WINDOWS", "Main::dockwidgets"));
			for (Position p = 0; p < mys.size(); p++)
			{
				if (mys[p] == '*') mys[p] = '\n';
			}
			QString s = mys.c_str();
			QTextStream stream( &s, IO_ReadWrite);
			stream >> *this;
		}

		void MainControl::menuItemHighlighted(int id)
			throw()
		{
			if (menu_entries_hints_.has(id)) 
			{
				setStatusbarText(menu_entries_hints_[id]);
			}
			else
			{
				setStatusbarText("");
			}
		}

		void MainControl::dump(ostream& s, Size depth) const
			throw()
		{
			BALL_DUMP_STREAM_PREFIX(s);
			BALL_DUMP_DEPTH(s, depth);
				ConnectionObject::dump(s, depth);
				Embeddable::dump(s, depth);
			BALL_DUMP_STREAM_SUFFIX(s);     
		}

		bool MainControl::update(Composite& composite, bool changed_hierarchy)
			throw()
		{
			if (!composite_manager_.has(composite)) return false;

			CompositeMessage* cm = new CompositeMessage(composite, 
					CompositeMessage::CHANGED_COMPOSITE_HIERARCHY);
			if (!changed_hierarchy) cm->setType(CompositeMessage::CHANGED_COMPOSITE);

			notify_(cm);
			updateRepresentationsOf(composite.getRoot(), true, changed_hierarchy);

			return true;
		}

		bool MainControl::insert(Composite& composite, String name)
			throw()
		{
			if (composite_manager_.has(composite)) return false;

			composite_manager_.insert(composite);
			CompositeMessage* cm; 
		
			if (MolecularStructure::getInstance(0) != 0)
			{
				cm = new CompositeMessage(composite, CompositeMessage::NEW_COMPOSITE);
			}
			else
			{
				cm = new CompositeMessage(composite, CompositeMessage::NEW_MOLECULE);
			}

			cm->setCompositeName(name);
			notify_(cm);
			
			return true;
		}

		bool MainControl::remove(Composite& composite, bool to_delete)
			throw()
		{
			if (!composite_manager_.has(composite)) 
			{
				primitive_manager_.removedComposite(composite);
				return false;
			}

			control_selection_.clear();

			CompositeMessage* cm = new CompositeMessage(composite, 
					CompositeMessage::REMOVED_COMPOSITE);
			notify_(cm);
			remove_(composite, true, to_delete);

			return true;
		}

		bool MainControl::insert(Representation& rep)
			throw()
		{
			return primitive_manager_.insert(rep);
		}

		bool MainControl::update(Representation& rep)
			throw()
		{
			if (!primitive_manager_.has(rep)) return false;

			rep.update(true);

			RepresentationMessage* rm = new RepresentationMessage(rep, RepresentationMessage::UPDATE);
			notify_(rm);

			return true;
		}

		bool MainControl::remove(Representation& rep)
			throw()
		{
			if (rep.hasProperty(Representation::PROPERTY__IS_COORDINATE_SYSTEM))
			{
				SceneMessage *scene_message = new SceneMessage(SceneMessage::REMOVE_COORDINATE_SYSTEM);
				notify_(scene_message);
			}
			else if (rep.hasProperty("AX"))
			{
				SceneMessage *scene_message = new SceneMessage(SceneMessage::REBUILD_DISPLAY_LISTS);
				notify_(scene_message);
			}
					
			RepresentationMessage* message = new RepresentationMessage(rep, RepresentationMessage::REMOVE);
			notify_(message);

			return primitive_manager_.remove(rep);
		}

		void MainControl::sendMessage(Message& message)
			throw()
		{
			message.setDeletable(false);
			onNotify(&message);
			notify_(&message);
		}

		void MainControl::clearSelection()
			throw()
		{
			CompositeManager::CompositeIterator it = getCompositeManager().begin();
			for (; it != getCompositeManager().end(); it++)
			{
				deselectCompositeRecursive(*it);
			 	updateRepresentationsOf(**it, false);
			}

			getSelection().clear();

			redrawAllRepresentations(true);

			NewSelectionMessage* nm = new NewSelectionMessage;
			sendMessage(*nm);
		}

		void MainControl::deleteClicked()
		{
			List<ModularWidget*>::Iterator it = modular_widgets_.begin(); 
			for (; it != modular_widgets_.end(); ++it)
			{
				if (RTTI::isKindOf<GenericControl>(**it))
				{
					(dynamic_cast<GenericControl*>(*it))->deleteCurrentItems();
				}
			}
		}

		void MainControl::insertDeleteEntry()
			throw()
		{
			if (delete_id_ == 0) 
			{
				delete_id_ = insertMenuEntry(MainControl::EDIT, "Delete", this, SLOT(deleteClicked()));	
			}
		}

		void MainControl::setDeleteEntryEnabled(bool state)
			throw()
		{
			menuBar()->setItemEnabled(delete_id_, state);
		}

		void MainControl::setBusyMode_(bool state) 
		{
			checkMenus();

			if (state)
			{
				if (composites_locked_) 
				{
					return;
				}
				QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
				simulation_icon_->show();
			}
			else
			{
				QApplication::restoreOverrideCursor();
				simulation_icon_->hide();
			}
		}

		void MainControl::stopSimulation() 
		{
		#ifdef BALL_QT_HAS_THREADS
			if (simulation_thread_ == 0) return;

			stop_simulation_ = true;
			setStatusbarText("Terminating calculation ...", true);
			#endif
		}

		// is called when the SimulationThread has finished
		void MainControl::stopedSimulation_()
		{
			#ifdef BALL_QT_HAS_THREADS
			if (simulation_thread_ != 0)
			{
				DCDFile* file = simulation_thread_->getDCDFile();
				if (file != 0)
				{
					file->close();
					String filename = file->getName();
					delete file;
					file = new DCDFile(filename, File::IN);
					NewTrajectoryMessage* message = new NewTrajectoryMessage;
					message->setComposite(*simulation_thread_->getComposite());
					message->setTrajectoryFile(*file);
					notify_(message);
				}
				
				simulation_thread_->wait();
				delete simulation_thread_;
				simulation_thread_ = 0;
			}

			if (stop_simulation_)
			{
				setStatusbarText("Calculation terminated.", true);
			}
			stop_simulation_ = false;
			unlockCompositesFor(locking_widget_);
			#endif
		}

		void MainControl::customEvent(QCustomEvent* e)
		{
		#ifdef BALL_QT_HAS_THREADS
			if (e->type() == (QEvent::Type)FINISHED_REPRESENTATION_UPDATE_EVENT)
			{
				qApp->processEvents(200);
				primitive_manager_.finishedUpdate_();
				return;
			}

			if (e->type() == (QEvent::Type)SIMULATION_THREAD_FINISHED_EVENT)
			{
				stopedSimulation_();
				return;
			}
			
			if (e->type() == (QEvent::Type)LOG_EVENT)
			{
				LogEvent* so = dynamic_cast<LogEvent*>(e);
				if (so->showOnlyInLogView())
				{
					Log.info() << so->getMessage() << std::endl;
					return;
				}
 				setStatusbarText(so->getMessage(), so->isImportant());
				return;
			}
			
			if (e->type() == (QEvent::Type)UPDATE_COMPOSITE_EVENT)
			{
				UpdateCompositeEvent* so = dynamic_cast<UpdateCompositeEvent*>(e);
				if (so->getComposite() == 0) 
				{
					Log.warn() << "Could not update visualisation in " << __FILE__ << __LINE__ << std::endl;
					return;
				}

				updateRepresentationsOf(* const_cast<Composite*>(so->getComposite()), true);

				// prevent deadlock if no representation has to be updated:
				if (!primitive_manager_.getUpdateThread().running())
				{
					getPrimitiveManager().setUpdatePending(false);
				}
			}
		#else
			e->type(); // prevent warning for single thread build
		#endif
		}

		bool MainControl::setSimulationThread(SimulationThread* thread)
			throw()
		{
			#ifdef BALL_QT_HAS_THREADS
				if (!lockCompositesFor(0)) return false;

				simulation_thread_ = thread;
				if (thread != 0) 
				{
					thread->setMainControl(this);
				}

				checkMenus();
				setBusyMode_(thread != 0);

			#else
				// prevent warning
				if (thread != 0) return true;
			#endif

			return true;
		}

		SimulationThread* MainControl::getSimulationThread()
			throw()
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

			NewSelectionMessage* new_message = new NewSelectionMessage;
			notify_(new_message);
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

		void MainControl::enableLoggingToFile()
			throw()
		{
			if (logging_to_file_) return;
			if (logging_file_name_ == "")
			{
				Log.error() << "Empty logging file name" << std::endl;
				return;
			}

			logging_to_file_ = true;
			logging_file_.open(logging_file_name_, std::ios::out);
			Log.insert(logging_file_);

			main_control_preferences_->enableLoggingToFile(true);
		}
		
		void MainControl::disableLoggingToFile()
			throw()
		{
			if (!logging_to_file_) return;
			logging_to_file_ = false;
			Log.remove(logging_file_);
			logging_file_.close();
		}

		void MainControl::setLoggingFilename(const String& string)
			throw()
		{
			logging_file_name_ = string;
			if (logging_to_file_) 
			{
				disableLoggingToFile();
				enableLoggingToFile();
			}
		}

		void MainControl::setWorkingDir(const String& dir)
			throw() 
		{ 
			Directory directory(dir);
			if (directory.setCurrent())
			{
				working_dir_ = dir;
			}
		}


		void MainControl::moveItems(const Matrix4x4& m)
			throw()
		{
			if (selection_.size() == 0) return;

			// copy list, because selection could change
			HashSet<Composite*> selection = selection_;
			HashSet<Composite*>::Iterator it = selection.begin();
			HashSet<Composite*> roots;

			// rotation
			if ((m.m14 == 0.0) && (m.m24 == 0.0) && (m.m34 == 0.0))
			{
				GeometricCenterProcessor center_processor;
				Vector3 center;
				for(; it != selection.end(); it++)
				{
					(*it)->apply(center_processor);
					center += center_processor.getCenter();
				}
				
				center /= (float) selection.size();

				Matrix4x4 mym1, mym2;
				mym1.setTranslation(center * -1);
				mym2.setTranslation(center);

				TransformationProcessor tp1(mym1);
				TransformationProcessor tp2(m);
				TransformationProcessor tp3(mym2);

				for (it = selection.begin(); it != selection.end(); it++)
				{
					(*it)->apply(tp1);
					(*it)->apply(tp2);
					(*it)->apply(tp3) ;
					roots.insert(&(**it).getRoot());
				}
			}
			// translation
			else
			{
				TransformationProcessor tp(m);
				for(; it != selection.end(); it++)
				{
					(*it)->apply(tp);
					roots.insert(&(**it).getRoot());
				}
			}

			HashSet<Composite*>::Iterator rit = roots.begin();
			for(; rit != roots.end(); rit++)
			{
				updateRepresentationsOf(**rit, true, false);
			}
		}

		bool MainControl::lockCompositesFor(ModularWidget* widget)
			throw()
		{
#ifdef BALL_QT_HAS_THREADS
			if (!composites_locked_mutex_.tryLock()) return false;
#endif

			locking_widget_ = widget;
			composites_locked_ = true;
			setBusyMode_(true);
			return true;
		}

		bool MainControl::unlockCompositesFor(ModularWidget* widget)
			throw()
		{
			if (locking_widget_ != widget) return false;
#ifdef BALL_QT_HAS_THREADS
			composites_locked_mutex_.unlock();
			composites_locked_wait_condition_.wakeAll();
#endif
			composites_locked_ = false;
			setBusyMode_(false);
			return true;
		}


	void MainControl::saveBALLViewProjectFile(const String& filename)
	{
		String temp;
		File::createTemporaryFilename(temp);
		INIFile out(temp);
		out.appendSection("WINDOWS");
		out.appendSection("BALLVIEW_PROJECT");

		// check menu entries, fetch and apply preferences
		List<ModularWidget*>::Iterator mit = modular_widgets_.begin(); 
		for (; mit != modular_widgets_.end(); ++mit)
		{
			(*mit)->writePreferences(out);
		}

		if (Scene::getInstance(0) != 0)
		{
			out.insertValue("BALLVIEW_PROJECT", "Camera", Scene::getInstance(0)->getStage()->getCamera().toString());
		}

		Position nr_of_representations = 0;
		PrimitiveManager::RepresentationsConstIterator it = getPrimitiveManager().begin();
		for (; it != getPrimitiveManager().end(); it++)
		{
			if ((**it).begin() == (**it).end()) 
			{
				Log.error() << "Error while writing Project File in " << __FILE__ << " " << __LINE__ << std::endl;
				continue;
			}

			bool ok = true;

			Representation::CompositesIterator cit = (**it).begin();
			const Composite* root = &(**cit).getRoot();
			for (; cit != (**it).end(); cit++)
			{
				if ((**cit).getRoot() != *root)
				{
					ok = false;
					break;
				}
			}

			if (!ok) 
			{	
				Log.error() << "Error while writing Project File in " << __FILE__ << " " << __LINE__ << std::endl;
				continue;
			}

			Index system_nr = -1;
			CompositeManager::CompositeIterator cit2 = getCompositeManager().begin();
			for (Position nr = 0; cit2 != getCompositeManager().end(); cit2++)
			{
				if (root == *cit2) system_nr = nr;

				nr++;
			}

			if (system_nr == -1) continue;

			out.insertValue("BALLVIEW_PROJECT", 
											String("Representation") + String(nr_of_representations),  
											String(system_nr) + String(";") + (**it).toString());
			nr_of_representations++;
		}
				
		writePreferences(out);
		INIFile::LineIterator lit = out.getLine(0);
		File result(filename, std::ios::out);
		result << out.getNumberOfLines() << std::endl;
		result << getCompositeManager().getNumberOfComposites() << std::endl;
		for (; +lit; ++lit)
		{
			result << *lit << std::endl;
		}

		CompositeManager::CompositeIterator cit = getCompositeManager().begin();
		for (; cit != getCompositeManager().end(); cit++)
		{
			if (!RTTI::isKindOf<System>(**cit)) continue;

			// hack to prevent ambiguous problem with overloaded cstr
			TextPersistenceManager pm(result.getFileStream(), result.getFileStream());
			(*dynamic_cast<System*>(*cit)) >> pm;
		}

		result.close();
		File::remove(temp);
	} 

	void MainControl::loadBALLViewProjectFile(const String& filename)
		throw()
	{
		File file;
		try
		{
			file.open(filename, std::ios::in);
		}
		catch(...)
		{
			setStatusbarText("Could not open project file, it doesnt seem to exist.", true);
			return;
		}

		Size nr_lines, nr_composites;
		file >> nr_lines;
		file >> nr_composites;
 		INIFile in;
		char buffer[2000];
		for (Position p = 0; p <= nr_lines; p++)
		{
			if (!file.getline(&(buffer[0]), 2000))
			{
				setStatusbarText("Error while reading project file, could not read INIFile", true);
				return;
			}

			if (!in.appendLine(buffer)) 
			{
				setStatusbarText("Error while reading project file, could not read INIFile", true);
				return;
			}
		}

		// check menu entries, fetch and apply preferences
		List<ModularWidget*>::Iterator it = modular_widgets_.begin(); 
		for (; it != modular_widgets_.end(); ++it)
		{
			(*it)->fetchPreferences(in);
			(*it)->applyPreferences();
		}

		if (DisplayProperties::getInstance(0) != 0)
		{
			DisplayProperties::getInstance(0)->enableCreationForNewMolecules(false);
		}

		List<Composite*> selection;
		Position current_composite = 0;
		while (file.good() && !file.eof() && current_composite < nr_composites)
		{
			TextPersistenceManager pm(file, file);
			PersistentObject* po = pm.readObject();
			if (!RTTI::isKindOf<System>(*po))
			{
				setStatusbarText("Error while reading project file, could not read molecule", true);
				return;
			}

			insert(*dynamic_cast<System*>(po));
			current_composite++;
		}

		file.close();
		DisplayProperties::getInstance(0)->enableCreationForNewMolecules(true);

		try
		{
			for (Position p = 0; p < 9999999; p++)
			{
				if (!in.hasEntry("BALLVIEW_PROJECT", "Representation" + String(p))) break;

				String data_string = in.getValue("BALLVIEW_PROJECT", "Representation" + String(p));

				vector<String> string_vector;
				Size split_size;

				// Representation0=1;3 2 2 6.500000 0 0 [2]|Color|H
				// 								 ^ 																	System Number
				// 								         ^            							Model Settings
				// 								         							 ^            Composites numbers
				// 								         							     ^        Custom Color
				// 								         							     			^   Hidden Flag

				if (data_string.hasPrefix("CP:")) 
				{
					data_string = data_string.after("CP:");
					// we have a clipping plane
					split_size = data_string.split(string_vector);
					if (split_size != 4) continue;

					Representation* rep = new Representation();
					rep->setModelType(MODEL_CLIPPING_PLANE);
					rep->setProperty("AX", string_vector[0].toFloat());
					rep->setProperty("BY", string_vector[1].toFloat());
					rep->setProperty("CZ", string_vector[2].toFloat());
					rep->setProperty("D", string_vector[3].toFloat());

					insert(*rep);
					continue;
				}

				// split off information of system number
				split_size = data_string.split(string_vector, ";");
				Position system_pos = string_vector[0].toUnsignedInt();

				// split off between representation settings and composite numbers
				data_string = string_vector[1];
				vector<String> string_vector2;
				data_string.split(string_vector2, "[]");
				data_string = string_vector2[0];
				if (DisplayProperties::getInstance(0) != 0)
				{
					DisplayProperties::getInstance(0)->getSettingsFromString(data_string);
				}

				// Composite positions
				data_string = string_vector2[1];
				data_string.split(string_vector2, ",");
				HashSet<Position> hash_set;
				for (Position p = 0; p < string_vector2.size(); p++)
				{
					hash_set.insert(string_vector2[p].toUnsignedInt());
				}

				Position pos = getCompositeManager().getNumberOfComposites() - 1;
				CompositeManager::CompositeIterator cit2 = getCompositeManager().begin();
				for (; cit2 != getCompositeManager().end() && system_pos != pos; cit2++)
				{
					pos--;
				}

				if (cit2 == getCompositeManager().end())  
				{
					setStatusbarText("Error while reading project file! Aborting...", true);
					continue;
				}

				data_string = string_vector[1];
				if (data_string.has('|'))
				{
					data_string.split(string_vector2, "|");
					ColorRGBA color;
					color = string_vector2[1];
					if (DisplayProperties::getInstance(0) != 0)
					{
						DisplayProperties::getInstance(0)->setCustomColor(color);
					}
				}

				ControlSelectionMessage* msg = new ControlSelectionMessage();
				Position current = 0;

				Composite::CompositeIterator ccit = (*cit2)->beginComposite();
				for (; +ccit; ++ccit)
				{
					if (hash_set.has(current)) msg->getSelection().push_back((Composite*)&*ccit);
					current++;
				}

				notify_(msg);
			
				if (DisplayProperties::getInstance(0) != 0)
				{
					DisplayProperties::getInstance(0)->apply();
				}	

				if (string_vector2.size() == 3 && string_vector2[2].has('H'))
				{
					Representation* rep = 0;
					PrimitiveManager::RepresentationsIterator pit = getPrimitiveManager().begin();
					for (; pit != getPrimitiveManager().end(); pit++)
					{
						rep = *pit;
					}

					rep->setHidden(true);
					rep->update(false);

#ifndef BALL_QT_HAS_THREADS
			 		RepresentationMessage* msg = new RepresentationMessage(*rep, RepresentationMessage::UPDATE);
 					notify_(msg);
#endif
				}
			}
		}
		catch(Exception::InvalidFormat e)
		{
			setStatusbarText("Error while reading project file, could not read Representation.");
			Log.error() << e << std::endl;
			return;
		}

		getSelection().clear();
		NewSelectionMessage* msg = new NewSelectionMessage();
		notify_(msg);
 	
		fetchPreferences(in);

		if (in.hasEntry("BALLVIEW_PROJECT", "Camera"))
		{
			Stage stage;
			Camera c;
			if (!c.readFromString(in.getValue("BALLVIEW_PROJECT", "Camera")))
			{
				setStatusbarText("Could not read Camera position from project");
				Log.error() << "Could not read Camera position from project" << std::endl;
				return;
			}
			stage.setCamera(c);
			SceneMessage* msg = new SceneMessage(SceneMessage::UPDATE_CAMERA);
			msg->setStage(stage);
			notify_(msg);
		}

		if (DisplayProperties::getInstance(0) != 0)
		{
			DisplayProperties::getInstance(0)->enableCreationForNewMolecules(true);
		}

		Scene::getInstance(0)->fetchPreferences(in);
		Scene::getInstance(0)->applyPreferences();
	}

	void MainControl::setPreferencesEnabled_(bool state)
	{
		preferences_dialog_->ok_button->setEnabled(state);
	}

#	ifdef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/VIEW/KERNEL/mainControl.iC>
#	endif

		} // namespace VIEW
} // namespace BALL
