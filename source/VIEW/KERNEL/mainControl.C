// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: mainControl.C,v 1.103 2004/08/31 14:34:03 amoll Exp $
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

#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/MATHS/analyticalGeometry.h>
#include <BALL/MATHS/common.h>

#include <BALL/SYSTEM/directory.h>

#ifdef BALL_QT_HAS_THREADS
#	include <BALL/VIEW/KERNEL/threads.h>
#endif

#include <qapplication.h>
#include <qmenubar.h>    // menus
#include <qpopupmenu.h>	 // menus
#include <qstatusbar.h>  // statusbar
#include <qlabel.h>			 // statusbar
#include <qtooltip.h>
#include <qpushbutton.h> // needed for preferences
#include <qcursor.h>     // wait cursor
#include <qmessagebox.h> 

#include <algorithm> // sort

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
				selection_(),
				message_label_(new QLabel("" , statusBar())),
				main_control_preferences_(0),
				preferences_dialog_(new Preferences(this, "BALLView Preferences")),
				preferences_id_(-1),
				delete_id_(0),
				composites_muteable_(true),
				stop_simulation_(false),
				simulation_thread_(0),
				timer_(new StatusbarTimer(this)),
				logging_file_name_("VIEW.log"),
				logging_to_file_(false),
				about_to_quit_(false)
		{
		#ifdef BALL_VIEW_DEBUG
			Log.error() << "new MainControl " << this << std::endl;
		#endif

			// store and load the INIFile from the Users homedir
			// default for UNIX/LINUX
			char* home_dir = getenv("HOME");
			if (home_dir == 0) 
			{
				// windows
				home_dir = getenv("HOMEPATH");
			}

			if (home_dir != 0)
			{
				inifile = String(home_dir) + String(FileSystem::PATH_SEPARATOR) + inifile;
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
				putenv((char*)((BALL::String("BALL_DATA_PATH=") + BALL::String(BALLView_data_path)).c_str()));
			}

			try
			{
				fragment_db_ = FragmentDB("fragments/Fragments.db");
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

			timer_->setInterval(2000);
			timer_->setLabel(message_label_);

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
				composites_muteable_(main_control.composites_muteable_),
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

			#ifdef BALL_QT_HAS_THREADS
				stop_simulation_ = true;
				if (simulation_thread_ != 0)
				{
					if (simulation_thread_->running()) simulation_thread_->wait();
					delete simulation_thread_;
					simulation_thread_ = 0;
				}
			#endif
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

			insertMenuEntry(MainControl::EDIT, "Toggle Selection", this, SLOT(complementSelection()));

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

			setCompositesMuteable(simulation_thread_ == 0);

			// checks all modular widgets 
			List<ModularWidget*>::Iterator it = modular_widgets_.begin(); 
			for (; it != modular_widgets_.end(); ++it)
			{
				(*it)->checkMenu(*this);
			}

			if (composites_muteable_) simulation_icon_->hide();
			else 											simulation_icon_->show();

			menuBar()->setItemEnabled(MENU_STOPSIMULATION, !composites_muteable_);
		}

		void MainControl::applyPreferencesTab()
		{
			// apply on own preferences tab
			applyPreferences();

			// checks all modular widgets 
			List<ModularWidget*>::Iterator it = modular_widgets_.begin(); 
			for (; it != modular_widgets_.end(); ++it)
			{
				(*it)->applyPreferences();
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

			preferences_.clear();
			preferences_.appendSection("WINDOWS");

			// finalizes all modular widgets
			List<ModularWidget*>::Iterator it = modular_widgets_.begin(); 
			for (; it != modular_widgets_.end(); ++it)
			{
				(*it)->writePreferences(preferences_);
				(*it)->finalizePreferencesTab(*preferences_dialog_);
				(*it)->finalizeWidget(*this);
			}

			for (it = modular_widgets_.begin(); it != modular_widgets_.end(); ++it)
			{
				unregisterConnectionObject(**it);
			}

			modular_widgets_.clear();

			//
			// write the preferences
			//
			preferences_dialog_->writePreferences(preferences_);

			// write default preferences 
			writePreferences(preferences_);

			// finalize own preferences tab
			finalizePreferencesTab(*preferences_dialog_);

			delete preferences_dialog_;
			preferences_dialog_ = 0;
		#ifdef BALL_VIEW_DEBUG
			Log.error() << "MainControl::aboutToExit() finished" << std::endl;
		#endif
		}


		bool MainControl::remove_(Composite& composite, bool update_representations_of_parent)
			throw()
		{
			if (!composite_manager_.has(composite)) return false;
			
			Composite* root = 0;
			if (composite.getRoot() != composite) root = &composite.getRoot();

			// delete all representations containing the composite
			List<Representation*> removed_representations;
			removed_representations = primitive_manager_.removedComposite(composite);
			List<Representation*>::Iterator reps_it = removed_representations.begin();
			// notify GeometricControl of removed representations
			for (; reps_it != removed_representations.end(); reps_it++)
			{
				RepresentationMessage* rr_message = new RepresentationMessage(**reps_it, RepresentationMessage::REMOVE);
				notify_(rr_message);
			}

			composite_manager_.remove(composite);

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
						remove_(*cmessage->getComposite(), cmessage->updateRepresentations());
						return;
					case CompositeMessage::CHANGED_COMPOSITE_AND_UPDATE_MOLECULAR_CONTROL:
					case CompositeMessage::CHANGED_COMPOSITE:
						if (cmessage->updateRepresentations())
						{
							updateRepresentationsOf(cmessage->getComposite()->getRoot(), true, true);
						}
						return;
					case CompositeMessage::SELECTED_COMPOSITE:
					case CompositeMessage::DESELECTED_COMPOSITE:
					{
						bool selected = (cmessage->getType() == CompositeMessage::SELECTED_COMPOSITE);
						if (selected == selection_.has(cmessage->getComposite())) return;
						if (selected)
						{
							selectCompositeRecursive(cmessage->getComposite(), true);
						}
						else
						{
							deselectCompositeRecursive(cmessage->getComposite(), true);
						}
						printSelectionInfos();

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
			CHECK_PTR(main_control_preferences_);

			preferences.insertPage(main_control_preferences_, "General");
			preferences.showPage(0);

			main_control_preferences_->enableLoggingToFile(logging_to_file_);
		}

		void MainControl::finalizePreferencesTab(Preferences &preferences)
			throw()
		{
			if (main_control_preferences_ != 0)
			{
				preferences.removePage(main_control_preferences_);

				delete main_control_preferences_;
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

		#ifdef BALL_PLATFORM_WINDOWS
			// workaround for strange microsoft windows behaviour
// 			x_pos += 4;
// 			y_pos += 23;
		#endif
			
			resize(QSize(w,h));
			move(QPoint(x_pos, y_pos));
			
			// the default preferences tab (if existent)
			if (main_control_preferences_ != 0)
			{
				main_control_preferences_->fetchPreferences(inifile);
			}

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

			// the default preferences tab (if existent)
			if (main_control_preferences_ != 0)
			{
				main_control_preferences_->writePreferences(inifile);
			}
			
			inifile.write();
		}

		void MainControl::addModularWidget(ModularWidget* widget)
			throw()
		{
			#ifdef BALL_DEBUG_VIEW
				Log.info() << "MainControl::addModularWidget(" << widget << ")" << endl;
			#endif
			modular_widgets_.push_back(widget);
			widget->registerThis();
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
			List<GeometricObject*>& objects = const_cast<List<GeometricObject*>&>(message.getSelection());
			List<GeometricObject*>::Iterator it_objects = objects.begin();
			HashSet<Composite*> roots;

			Size nr = 0;
			for (; it_objects != objects.end(); it_objects++)
			{
				Composite* composite = (Composite*)(*it_objects)->getComposite();

				if (composite != 0  && (selection_.has(composite) != message.isSelected()))
				{	
					if (message.isSelected())
					{
						selectCompositeRecursive(composite , true);
					}
					else
					{
						deselectCompositeRecursive(composite, true);
					}
					nr++;

					if (!roots.has(&composite->getRoot()))
					{
						roots.insert(&composite->getRoot());
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
				setStatusbarText(String(selection_.size()) + " objects selected.");
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
				atoms[nr_of_atoms] = (Atom*) *it;
				times.push_back(((Atom*) *it)->getSelectionTime());
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
					setStatusbarText("0 objects selected.");
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
					setStatusbarText(text);
					Log.info() << text << std::endl;
					break;
				}
				case 2:
				{
					// if two atoms were picked, show their distance
					String text = String("Distance between atom ") + 
														atoms[0]->getFullName() + " and " + 
														atoms[1]->getFullName() + ": " + 
														String(GetDistance(atoms[0]->getPosition(), atoms[1]->getPosition()));
					setStatusbarText(text);
					Log.info() << text << std::endl;
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
					setStatusbarText(text);
					Log.info() << text << std::endl;
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
					setStatusbarText(text);
					Log.info() << text << std::endl;
					break;
				}
			}
		}


		const HashSet<Composite*>& MainControl::getSelection() const
			throw()
		{
			return selection_;
		}

		HashSet<Composite*>& MainControl::getSelection() 
			throw()
		{
			return selection_;
		}

		System* MainControl::getSelectedSystem()
			throw()
		{
			if (control_selection_.size() != 1 || 
					!RTTI::isKindOf<System>(**control_selection_.begin()))
			{
				return 0;
			}
			
			return (System*) *control_selection_.begin();
		}


		void MainControl::selectCompositeRecursive(Composite* composite, bool first_call)
			throw()
		{
			composite->select();
			if (RTTI::isKindOf<Bond>(*composite)) return;
			selection_.insert(composite);

			if (RTTI::isKindOf<Atom> (*composite))
			{
				Atom *atom = (Atom*) composite;
				AtomBondIterator bi;		
				BALL_FOREACH_ATOM_BOND(*atom, bi)
				{
					if (selection_.has(bi->getPartner(*atom)))
					{
						bi->select();			
					}				
				}				
			}		
			else if (RTTI::isKindOf<AtomContainer> (*composite))
			{
				for (Size i=0; i< composite->getDegree();i++)
				{
					selectCompositeRecursive(composite->getChild(i), false);
				}
			}		

			if (first_call)
			{
				Composite* parent = composite->getParent();
				while (parent != 0 && !selection_.has(parent))
				{
					for (Size i=0; i < parent->getDegree();i++)
					{
						if (!selection_.has(parent->getChild(i)))
						{
							return;
						}
					}
					
					selection_.insert(parent);
					parent->select();
					parent = parent->getParent();
				}
			}
		}


		void MainControl::deselectCompositeRecursive(Composite* composite, bool first_call)
			throw()
		{
			composite->deselect();
			if (RTTI::isKindOf<Bond>(*composite)) return;
			selection_.erase(composite);

			if (RTTI::isKindOf<Atom> (*composite))
			{
				AtomBondIterator bi;		
				BALL_FOREACH_ATOM_BOND(*(Atom*) composite, bi)
				{
					bi->deselect();			
				}				
			}		
			else if (RTTI::isKindOf<AtomContainer> (*composite))
			{
				for (Size i=0; i< composite->getDegree();i++)
				{
					deselectCompositeRecursive(composite->getChild(i), false);
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


		void MainControl::setStatusbarText(const String& text, bool beep)
			throw()
		{
			if (beep) QApplication::beep();
			message_label_->setText(text.c_str());
			message_label_->setPaletteForegroundColor( QColor(255, 0, 0) );
			timer_->stopTimer();
			timer_->startTimer();
			QWidget::update();
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

		void MainControl::setMenuHint(Index id, const String& string)
			throw()
		{
			menu_entries_hints_[id] = string;
			menuBar()->setWhatsThis(id, string.c_str());
		}

		const String& MainControl::getMenuHint(Index id) const
			throw() 
		{
			return menu_entries_hints_[id];
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

		bool MainControl::update(Composite& composite)
			throw()
		{
			if (!composite_manager_.has(composite)) return false;

			CompositeMessage* cm = new CompositeMessage(composite, 
					CompositeMessage::CHANGED_COMPOSITE_AND_UPDATE_MOLECULAR_CONTROL);
			notify_(cm);
			updateRepresentationsOf(composite.getRoot(), true, true);

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
				cm = new CompositeMessage(composite, 
						CompositeMessage::NEW_COMPOSITE);
			}
			else
			{
				cm = new CompositeMessage(composite, 
						CompositeMessage::NEW_MOLECULE);
			}

			cm->setCompositeName(name);
			notify_(cm);
			
			return true;
		}

		bool MainControl::remove(Composite& composite)
			throw()
		{
			if (!composite_manager_.has(composite)) return false;

			CompositeMessage* cm = new CompositeMessage(composite, 
					CompositeMessage::REMOVED_COMPOSITE);
			notify_(cm);
			remove_(composite);

			return true;
		}

		bool MainControl::insert(Representation& rep)
			throw()
		{
			if (primitive_manager_.has(rep)) return false;
			primitive_manager_.insert(rep);

			RepresentationMessage* rm = new RepresentationMessage(rep, RepresentationMessage::ADD);
			notify_(rm);

			return true;
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
			if (!primitive_manager_.has(rep)) return false;

			RepresentationMessage* rm = new RepresentationMessage(rep, RepresentationMessage::REMOVE);
			notify_(rm);
			primitive_manager_.remove(rep);

			return true;
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
					((GenericControl*) (*it))->deleteCurrentItems();
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

		void MainControl::setCompositesMuteable(bool state) 
		{
			if (state == composites_muteable_) return;

			composites_muteable_ = state;
			if (state)
			{
				QApplication::restoreOverrideCursor();
				simulation_icon_->hide();
			}
			else
			{
				QApplication::setOverrideCursor( QCursor(Qt::WaitCursor) );
				simulation_icon_->show();
			}
		}

		void MainControl::stopSimulation() 
		{
		#ifdef BALL_QT_HAS_THREADS
			stop_simulation_ = true;
			if (simulation_thread_ != 0)
			{
				if (simulation_thread_->running()) 
				{
					simulation_thread_->wait();
				}

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
				
				delete simulation_thread_;
				simulation_thread_ = 0;
			}

			setStatusbarText("Calculation terminated.");
			stop_simulation_ = false;
			checkMenus();
		#endif
		}

		void MainControl::customEvent( QCustomEvent * e )
		{
			e->type(); // prevent warning for single thread build

		#ifdef BALL_QT_HAS_THREADS
			if (e->type() == (QEvent::Type)SIMULATION_THREAD_FINISHED_EVENT)
			{
				stopSimulation();
				return;
			}
			if (e->type() == (QEvent::Type)SIMULATION_OUTPUT_EVENT)
			{
				SimulationOutput* so = (SimulationOutput*) e;
				Log.info() << so->getMessage() << std::endl;
				return;
			}
			if ( e->type() == (QEvent::Type)UPDATE_COMPOSITE_EVENT)
			{
				UpdateCompositeEvent* so = (UpdateCompositeEvent*) e;
				if (so->getComposite() == 0) 
				{
					Log.warn() << "Could not update visualisation in " << __FILE__ << __LINE__ << std::endl;
					return;
				}

				qApp->wakeUpGuiThread();
				qApp->processEvents();
				if (simulation_thread_ == 0 ||
						stop_simulation_) 
				{
					return;
				}

				updateRepresentationsOf(*(Composite*)so->getComposite(), true);

				qApp->wakeUpGuiThread();
				qApp->processEvents();
				if (simulation_thread_ == 0 || 
						stop_simulation_) 
				{
					return;
				}

				// ok, continue simulation
				simulation_thread_->setUpdateRunning(false);
				return;
			}
		#endif
		}

		bool MainControl::setSimulationThread(SimulationThread* thread)
			throw()
		{
			#ifdef BALL_QT_HAS_THREADS
				if (simulation_thread_ != 0) 
				{
					return false;
				}
				simulation_thread_ = thread;
				if (thread != 0) 
				{
					thread->setMainControl(this);
				}
				checkMenus();
			#else
				simulation_thread_ = thread;
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

		const String& MainControl::getLoggingFilename() const
			throw()
		{
			return logging_file_name_;
		}

		// ======================= StatusbarTimer =========================
		StatusbarTimer::StatusbarTimer(QObject* parent)
			throw()
			: QTTimer(parent),
				label_(0),
				seconds_(0)
		{}

		void StatusbarTimer::timer()
			throw()
		{
			stopTimer();

			if (!label_) return;
			if (label_->text() == "") 
			{
				seconds_ = 0;
				return;
			}

			label_->setPaletteForegroundColor( QColor(0,0,0) );

			startTimer();
			seconds_ ++;

			if (seconds_ < 9) return;

			seconds_ = 0;
			label_->setText("");
		}

		void StatusbarTimer::setLabel(QLabel* label)
			throw()
		{
			label_ = label;
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


#	ifdef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/VIEW/KERNEL/mainControl.iC>
#	endif

		} // namespace VIEW
} // namespace BALL
