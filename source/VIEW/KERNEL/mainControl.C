// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: mainControl.C,v 1.174.2.11 2006/03/17 20:39:21 amoll Exp $
//

#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/geometricObject.h>
#include <BALL/VIEW/KERNEL/modularWidget.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/KERNEL/clippingPlane.h>
#include <BALL/VIEW/DIALOGS/mainControlPreferences.h>
#include <BALL/VIEW/DIALOGS/networkPreferences.h>
#include <BALL/VIEW/DIALOGS/preferences.h>

#include <BALL/VIEW/WIDGETS/genericControl.h>
#include <BALL/VIEW/WIDGETS/molecularStructure.h>
#include <BALL/VIEW/WIDGETS/scene.h>
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
#include <BALL/VIEW/KERNEL/threads.h>

#include <QtGui/qstatusbar.h>  // statusbar
#include <QtGui/qtooltip.h>
#include <QtGui/qpushbutton.h> // needed for preferences
#include <QtGui/qcursor.h>     // wait cursor
#include <QtGui/qmessagebox.h> 
#include <QtGui/QMenuBar>
#include <QtGui/QLabel>
#include <QtGui/QAction>
#include <QtCore/qtimer.h>
#include <QtCore/QEvent>
#include <QtCore/QCustomEvent>

#include <algorithm> // sort

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
			:	QMainWindow(parent),
				ConnectionObject(),
				Embeddable("BALL::VIEW::MainControl"),
				fragment_db_(),
				selection_(),
				control_selection_(),
				message_label_(new QLabel("" , statusBar())),
				primitive_manager_(this),
				composite_manager_(),
				main_control_preferences_(0),
				network_preferences_(0),
				preferences_dialog_(new Preferences(this, "BALLView Preferences")),
				preferences_file_(),
				composites_locked_(false),
				locking_widget_(0),
				stop_simulation_(false),
				simulation_thread_(0),
 				multi_threading_mode_(true),
				modular_widgets_(),
				simulation_icon_(0),
				working_dir_(),
				logging_file_name_("VIEW.log"),
				logging_to_file_(false),
				about_to_quit_(false),
				important_text_in_statusbar_(false),
				stop_simulation_action_(0),
				complement_selection_action_(0),
				open_action_(0),
				save_project_action_(0),
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
						QMessageBox::Abort,  Qt::NoButton);
				Log.error() << e << std::endl;

				exit(-1);
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

//   			QToolTip::setWakeUpDelay(500);
//   			QToolTip::setGloballyEnabled(true);

			font.setPointSize(14);
			rep_label_ = new QLabel(statusBar());
			rep_label_->setFrameShape(QLabel::NoFrame);
			rep_label_->setFrameShadow(QLabel::Plain);
			rep_label_->setFont(font); 
			rep_label_->setMaximumSize(20,20);
			rep_label_->setMinimumSize(20,20);
			rep_label_->setAlignment(Qt::AlignCenter);
			rep_label_->hide();
			statusBar()->addPermanentWidget(rep_label_, false );
			rep_label_nr_ = 0;

			render_timer_.start(200);

			simulation_icon_ = new QLabel(statusBar());
			simulation_icon_->setMaximumSize(14,20);
			simulation_icon_->setMinimumSize(14,20);
			statusBar()->addPermanentWidget(simulation_icon_, false );
//   			QToolTip::add(simulation_icon_, "simulation status");
			QPixmap icon(simulation_running_xpm_);

			simulation_icon_->setPixmap(icon);
			simulation_icon_->hide();
			simulation_icon_->setFrameShape(QLabel::NoFrame);
			simulation_icon_->setFrameShadow(QLabel::Plain);

			connect(&timer_, SIGNAL(timeout()), SLOT(clearStatusBarText_()));
			connect(&render_timer_, SIGNAL(timeout()), SLOT(updateRepLabel_()));
		}


		MainControl::MainControl(const MainControl& main_control)
			throw()
			:	QMainWindow(0),
				ConnectionObject(main_control),
				Embeddable(main_control),
				selection_(),
				main_control_preferences_(0),
				network_preferences_(0),
				preferences_dialog_(new Preferences(this, "BALLView Preferences")),
				composites_locked_(false),
				locking_widget_(0),
				about_to_quit_(false),
				stop_simulation_action_(0),
				complement_selection_action_(0),
				open_action_(0),
				save_project_action_(0),
				preferences_action_(0),
				delete_action_(0)
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

		QMenu* MainControl::initPopupMenu(int ID)
			throw()
		{
			if (id_to_menu_.has(ID)) return id_to_menu_[ID];

			QMenu* menu = 0;

			#ifdef BALL_VIEW_DEBUG
				Log.info() << "new menu entry: " << ID << endl;	
			#endif
			switch (ID)
			{
				case FILE:
					menu = menuBar()->addMenu("&File");
					break;
				case FILE_OPEN:
					menu = initPopupMenu(FILE)->addMenu("&Open");
					break;
				case FILE_IMPORT:
					menu = initPopupMenu(FILE)->addMenu("&Import");
					break;
				case FILE_EXPORT:
					menu = initPopupMenu(FILE)->addMenu("&Export Image");
					break;
				case EDIT:
					menu = menuBar()->addMenu("&Edit");
					break;
				case BUILD:
					menu = menuBar()->addMenu("&Build");
					break;
				case DISPLAY:
					menu = menuBar()->addMenu("&Display");
					break;
				case DISPLAY_VIEWPOINT:
					menu = initPopupMenu(DISPLAY)->addMenu("&Viewpoint");
					break;
				case DISPLAY_STEREO:
					menu = initPopupMenu(DISPLAY)->addMenu("&Stereo");
					break;
				case DISPLAY_ANIMATION:
					menu = initPopupMenu(DISPLAY)->addMenu("&Animation");
					break;
				case MOLECULARMECHANICS:
					menu = menuBar()->addMenu("&Molecular Mechanics");
					break;
				case CHOOSE_FF:
					menu = initPopupMenu(MOLECULARMECHANICS)->addMenu("Force Field");
					break;
				case TOOLS:
					menu = menuBar()->addMenu("&Tools");
					break;
				case TOOLS_PYTHON:
					menu = initPopupMenu(TOOLS)->addMenu("&Python");
					break;
				case WINDOWS:
					menu = menuBar()->addMenu("&Windows");
					break;
				case USER:
					menu = menuBar()->addMenu("&User");
					break;
				case HELP:
					menu = menuBar()->addMenu("&Help");
					break;
				default:
					return 0;
			}

			connect(menu, SIGNAL(aboutToShow()), this, SLOT(checkMenus()));
			id_to_menu_[ID] = menu;

			return menu;
		}

		void MainControl::clear()
			throw()
		{
			selection_.clear();
			primitive_manager_.clear();
			composite_manager_.clear();

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
			// connect apply button in Preferences dialog to slot
			connect(preferences_dialog_->ok_button, SIGNAL(clicked()), this, SLOT(applyPreferencesClicked_()));

			// initialize own preferences tab
			initializePreferencesTab_();

			// initialize all modular widgets 
			List<ModularWidget*>::Iterator it = modular_widgets_.begin(); 
			for (; it != modular_widgets_.end(); ++it)
			{
				(*it)->initializeWidget(*this);
				(*it)->initializePreferencesTab(*preferences_dialog_);
			}

			preferences_dialog_->showEntry(main_control_preferences_);

			// own menu entries
			insertPopupMenuSeparator(MainControl::FILE);
			insertMenuEntry(MainControl::FILE, "&Quit", qApp, SLOT(quit()), Qt::CTRL+Qt::Key_Q);	

			// if the preferences dialog has any tabs then show it
			if (preferences_dialog_->hasPages())
			{
				insertPopupMenuSeparator(MainControl::EDIT);
				preferences_action_ = insertMenuEntry(MainControl::EDIT,
																					"Preferences", 
																					preferences_dialog_, 
																					SLOT(show()), Qt::CTRL+Qt::Key_Z);
			}

			fetchPreferences(preferences_file_);
			applyPreferences();
		}

		void MainControl::checkMenus()
		{
			// preferences dialog not empty
			if (preferences_dialog_ != 0 &&
					preferences_dialog_->hasPages())
			{
				preferences_action_->setChecked(preferences_dialog_->isVisible());			
			}

			// overridden in Controls
			if (delete_action_ != 0) delete_action_->setEnabled(false);

			// checks all modular widgets 
			List<ModularWidget*>::Iterator it = modular_widgets_.begin(); 
			for (; it != modular_widgets_.end(); ++it)
			{
				(*it)->checkMenu(*this);
			}

			stop_simulation_action_->setEnabled(simulation_thread_ != 0);
			complement_selection_action_->setEnabled(!composites_locked_);

			initPopupMenu(FILE_OPEN)->setEnabled(!composites_locked_);
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

			List<ModularWidget*>::Iterator it = modular_widgets_.begin(); 
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
			throw()
		{
			// delete all representations containing the composite
			primitive_manager_.removedComposite(composite, update_representations_of_parent);

			getSelection().erase(&composite);
			
			// remove childs of composite from selection 
			List<Composite*> to_remove;
			HashSet<Composite*>::Iterator cit = getSelection().begin();
			for (; +cit; ++cit)
			{
				if ((**cit).isDescendantOf(composite))
				{
					to_remove.push_back(*cit);
				}
			}

			List<Composite*>::iterator lit = to_remove.begin();
			for (; lit != to_remove.end(); ++lit)
			{
				getSelection().erase(*lit);
			}

			// delete the Composite
			composite_manager_.remove(composite, to_delete);

			return true;
		}

		// e.g. is called for root of items from picking, or for MolecularControl Selection
		bool MainControl::updateRepresentationsOf(const Composite& composite, bool rebuild, bool force)
			throw()
		{
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
			}

			return changed_representations.size() != 0;
		}


		void MainControl::redrawAllRepresentations(bool rebuild_display_lists)
			throw()
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

		QAction* MainControl::insertMenuEntry(Position parent_id, const String& name, const QObject* receiver, 
																		 const char* slot, QKeySequence accel)
			throw()
		{
			QMenu* popup = initPopupMenu(parent_id);
			if (popup == 0)
			{
				Log.error() << "MainControl::insertMenuEntry: cannot find popup menu for ID " << parent_id << endl;
				return 0;
			}

			QAction* action = popup->addAction(name.c_str(), receiver, slot, accel);

			return action;
		}

		void MainControl::insertPopupMenuSeparator(int ID)
			throw()
		{
			QMenu* popup = initPopupMenu(ID);
			if (popup == 0)
			{
				Log.error() << "MainControl::insertMenuEntry: cannot find popup menu for ID " << ID << endl;
				return;
			}
			popup->addSeparator();
		}

		void MainControl::initializePreferencesTab_()
			throw()
		{
			main_control_preferences_ = new MainControlPreferences();

			preferences_dialog_->insertEntry(main_control_preferences_);

			main_control_preferences_->enableLoggingToFile(logging_to_file_);

			network_preferences_ = new NetworkPreferences();
			preferences_dialog_->insertEntry(network_preferences_);
		}

		void MainControl::applyPreferencesClicked_()
		{
			preferences_dialog_->close();
			setPreferencesEnabled_(false);
			applyPreferences();

			if (!updateOfRepresentationRunning())
			{
				setPreferencesEnabled_(true);
			}
		}

		void MainControl::applyPreferences()
			throw()
		{
			// MainControls own preferences
			
			if (main_control_preferences_ != 0)
			{
				QApplication::setStyle(main_control_preferences_->getStyle());
				QWidget::update();

				if (!main_control_preferences_->loggingToFileEnabled()) 
				{
					disableLoggingToFile();
				}
				else 	
				{
					enableLoggingToFile();
				}
			}

			if (network_preferences_ != 0)
			{
				network_preferences_->applySettings();
			}

			// all other preferences
			List<ModularWidget*>::Iterator it = modular_widgets_.begin(); 
			for (; it != modular_widgets_.end(); ++it)
			{
				(*it)->applyPreferences();
				(*it)->checkMenu(*this);
			}

			preferences_dialog_->applyPreferences();
		}

		void MainControl::fetchPreferences(INIFile &inifile)
			throw()
		{
			if (inifile.hasEntry("WINDOWS", "File::working_dir"))
			{
				setWorkingDir(inifile.getValue("WINDOWS", "File::working_dir"));
			}

			restoreWindows(inifile);
			
			preferences_dialog_->fetchPreferences(inifile);

			// check menu entries, fetch and apply preferences
			List<ModularWidget*>::Iterator it = modular_widgets_.begin(); 
			for (; it != modular_widgets_.end(); ++it)
			{
				(*it)->fetchPreferences(inifile);
			}
		}

		void MainControl::writePreferences(INIFile &inifile)
			throw()
		{
			// the main window position
			inifile.appendSection("WINDOWS");
			inifile.insertValue("WINDOWS", "Main::x", String(x()));
			inifile.insertValue("WINDOWS", "Main::y", String(y()));
			inifile.insertValue("WINDOWS", "Main::width", String(width()));
	 		inifile.insertValue("WINDOWS", "Main::height", String(height()));
			inifile.insertValue("WINDOWS", "File::working_dir", working_dir_);

			String mys;
			QByteArray ba = saveState();
			for (Position p = 0; p < (Position) ba.size(); p++)
			{
				mys += String((short) ba.at(p));
				mys += "|";
			}

			inifile.insertValue("WINDOWS", "Main::dockwidgets", mys);

			// finalizes all modular widgets
			List<ModularWidget*>::Iterator it = modular_widgets_.begin(); 
			for (; it != modular_widgets_.end(); ++it)
			{
				(*it)->writePreferences(inifile);
			}

			preferences_dialog_->writePreferences(inifile);

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

			List<GeometricObject*>& objects = const_cast<List<GeometricObject*>&>(message.getSelection());
			List<GeometricObject*>::Iterator it_objects = objects.begin();
			
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
						Atom* second = (Atom*) bond->getSecondAtom();
						
						roots.insert(&first->getRoot());
						
						(this->*func_ptr)(first, true);
						(this->*func_ptr)(second, true);
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
			if (ascii(message_label_->text()) == text) return;

			if (beep) 
			{
				QApplication::beep();
			}

			if (!important && important_text_in_statusbar_)
			{
				return;
			}

			important_text_in_statusbar_ = important;

			QPalette pal(message_label_->palette());

			if (important)
			{
				pal.setColor(message_label_->foregroundRole(), QColor(255,0,0));
				Log.info() << text << std::endl;
			}
			else
			{
				pal.setColor(message_label_->foregroundRole(), QColor(0,0,0));
			}

			message_label_->setPalette(pal);
			message_label_->setText(text.c_str());
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

		void MainControl::restoreWindows(const INIFile& inifile)
			throw()
		{
			try
			{
				// the geometry of the main window
				int x_pos = x();
				int y_pos = y();
				int w = 1024;
				int h = 800;
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

				resize(w,h);
				move(QPoint(x_pos, y_pos));

				// now the dockwidgets
				if (!inifile.hasEntry("WINDOWS", "Main::dockwidgets")) return;

				String mys(inifile.getValue("WINDOWS", "Main::dockwidgets"));
				vector<String> sv;
				mys.split(sv, "|");

				QByteArray s;
				s.reserve(sv.size());
				for (Position p = 0; p < sv.size(); p++)
				{
					s.append((char) sv[p].toShort());
				}

				restoreState(s);
			}
			catch(...)
			{
				Log.error() << "Could not restore settings of main windows" << std::endl;
			}
		}

		void MainControl::menuItemHighlighted(QAction* action)
			throw()
		{
			last_highlighted_menu_entry_ = action;

			setStatusbarText(ascii(action->toolTip()));
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

		void MainControl::update(Composite& composite, bool changed_hierarchy)
			throw()
		{
			CompositeMessage* cm = new CompositeMessage(composite, 
					CompositeMessage::CHANGED_COMPOSITE_HIERARCHY);
			if (!changed_hierarchy) cm->setType(CompositeMessage::CHANGED_COMPOSITE);

			notify_(cm);
			updateRepresentationsOf(composite.getRoot(), true, changed_hierarchy);
		}

		bool MainControl::insert(Composite& composite, String name)
			throw()
		{
			if (!composite_manager_.insert(composite)) return false;
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

		bool MainControl::remove(Composite& composite, bool to_delete, bool update)
			throw()
		{
			control_selection_.clear();

			notify_(new CompositeMessage(composite, CompositeMessage::REMOVED_COMPOSITE));
			remove_(composite, update, to_delete);

			return true;
		}

		bool MainControl::insert(Representation& rep)
			throw()
		{
			return primitive_manager_.insert(rep, true);
		}

		bool MainControl::update(Representation& rep)
			throw()
		{
			if (!primitive_manager_.has(rep)) return false;

			rep.update(true);

			notify_(new RepresentationMessage(rep, RepresentationMessage::UPDATE));

			return true;
		}

		bool MainControl::remove(Representation& rep)
			throw()
		{
			if (getRepresentationManager().updateRunning())
			{
				setStatusbarText("Could not delete Representation while update is running!", true);
				return false;
			}

			if (rep.hasProperty(Representation::PROPERTY__IS_COORDINATE_SYSTEM))
			{
				notify_(new SceneMessage(SceneMessage::REMOVE_COORDINATE_SYSTEM));
			}
			else if (rep.hasProperty("AX"))
			{
				notify_(new SceneMessage(SceneMessage::REBUILD_DISPLAY_LISTS));
			}
					
			notify_(new RepresentationMessage(rep, RepresentationMessage::REMOVE));

			return primitive_manager_.remove(rep);
		}

		void MainControl::sendMessage(Message& message)
			throw()
		{
			onNotify(&message);
			notify_(&message);
		}

		void MainControl::clearSelection()
			throw()
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
			if (delete_action_ == 0) 
			{
				delete_action_ = insertMenuEntry(MainControl::EDIT, "Delete", this, SLOT(deleteClicked()));	
			}
		}

		void MainControl::setDeleteEntryEnabled(bool state)
			throw()
		{
			delete_action_->setEnabled(state);
		}

		void MainControl::setBusyMode_(bool state) 
		{
			checkMenus();

			if (state)
			{
				QApplication::setOverrideCursor(QCursor(Qt::BusyCursor));
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
			if (simulation_thread_ == 0) return;

			stop_simulation_ = true;
			setStatusbarText("Terminating calculation ...", true);
		}

		// is called when the SimulationThread has finished
		void MainControl::stopedSimulation_()
		{
			if (simulation_thread_ != 0)
			{
				DCDFile* file = simulation_thread_->getDCDFile();
				if (file != 0)
				{
					file->close();
					String filename = file->getName();
					delete file;
					file = new DCDFile(filename, std::ios::in);
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

			notify_(new FinishedSimulationMessage);
		}

		bool MainControl::event(QEvent* e)
		{
			if (e->type() < QEvent::User)
			{
				return QMainWindow::event(e);
			}

			if (e->type() == (QEvent::Type)FINISHED_REPRESENTATION_UPDATE_EVENT)
			{
				FinishedRepresentionUpdateEvent* fe = dynamic_cast<FinishedRepresentionUpdateEvent*> (e);
				primitive_manager_.finishedUpdate_(fe->getRepresentation());
				return true;
			}

			if (e->type() == (QEvent::Type)SIMULATION_THREAD_FINISHED_EVENT)
			{
				stopedSimulation_();
				return true;
			}
			
			if (e->type() == (QEvent::Type)LOG_EVENT)
			{
				LogEvent* so = dynamic_cast<LogEvent*>(e);
				if (so->showOnlyInLogView())
				{
					Log.info() << so->getMessage() << std::endl;
					return true;
				}
 				setStatusbarText(so->getMessage(), so->isImportant());
				return true;
			}
			
			if (e->type() == (QEvent::Type)UPDATE_COMPOSITE_EVENT)
			{
				UpdateCompositeEvent* so = dynamic_cast<UpdateCompositeEvent*>(e);
				if (so->getComposite() == 0) 
				{
					Log.warn() << "Could not update visualisation in " << __FILE__ << __LINE__ << std::endl;
					return true;
				}

				updateRepresentationsOf(* const_cast<Composite*>(so->getComposite()), true);
			}

			if (e->type() == (QEvent::Type)DOCKING_FINISHED_EVENT)
			{
				DockingFinishedEvent* dock_event = dynamic_cast<DockingFinishedEvent*>(e);
				if (dock_event->getConformationSet() == 0)
				{
					Log.warn() << "Could not send docking finished message in " << __FILE__ << " " << __LINE__ << std::endl;
					return true;
				}
				// send a DockingFinishedMessage
				DockingFinishedMessage* dock_fin_m = new DockingFinishedMessage(dock_event->wasAborted());
				dock_fin_m->setConformationSet(dock_event->getConformationSet());
				notify_(dock_fin_m);
				return true;
			}

			return false;
		}

		bool MainControl::setSimulationThread(SimulationThread* thread)
			throw()
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
			String dir2 = dir;
			// QT will return Paths on windows with "/" as delimiter!
#ifdef BALL_PLATFORM_WINDOWS
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
			throw()
		{
			if (!composites_locked_mutex_.tryLock()) return false;

			locking_widget_ = widget;
			composites_locked_ = true;
			setBusyMode_(true);
			return true;
		}

		bool MainControl::unlockCompositesFor(ModularWidget* widget)
			throw()
		{
			if (locking_widget_ != widget) return false;
			composites_locked_mutex_.unlock();
			composites_locked_wait_condition_.wakeAll();
			composites_locked_ = false;
			setBusyMode_(false);
			return true;
		}


	void MainControl::saveBALLViewProjectFile(const String& filename)
	{
		String temp;
		File::createTemporaryFilename(temp);
		INIFile out(temp);
		out.appendSection("BALLVIEW_PROJECT");

		getRepresentationManager().storeRepresentations(out);

		// write turning point of scene
		Scene* scene = Scene::getInstance(0);
		if (scene != 0)
		{
			String s = vector3ToString(scene->getTurnPoint());
			out.insertValue("BALLVIEW_PROJECT", "TurnPoint", s);
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
		if (isBusy())
		{
			setStatusbarText("Could not load project file, while update is running!", true);
			return; 
		}

		File file;
		try
		{
			file.open(filename, std::ios::in);
		}
		catch(...)
		{
			setStatusbarText("Could not open the project file \"" + filename + "\", it doesnt seem to exist.", true);
			return;
		}

		Size nr_lines, nr_composites;
		file >> nr_lines;
		file >> nr_composites;
 		INIFile in;
		char buffer[50000];
		for (Position p = 0; p <= nr_lines; p++)
		{
			if (!file.getline(&(buffer[0]), 50000))
			{
				setStatusbarText("Error while reading project file, could not read INIFile", true);
				BALLVIEW_DEBUG
				return;
			}

			if (!in.appendLine(buffer)) 
			{
				setStatusbarText("Error while reading project file, could not read INIFile", true);
				BALLVIEW_DEBUG
				return;
			}
		}

		fetchPreferences(in);
		applyPreferences();

		bool has_dp = (DisplayProperties::getInstance(0) != 0);

		if (has_dp)
		{
			DisplayProperties::getInstance(0)->enableCreationForNewMolecules(false);
		}

		vector<const Composite*> new_systems;
		Position current_composite = 0;
		while (file.good() && !file.eof() && current_composite < nr_composites)
		{
			TextPersistenceManager pm(file, file);
			PersistentObject* po = pm.readObject();
			if (!RTTI::isKindOf<System>(*po))
			{
				setStatusbarText("Error while reading project file, could not read molecule", true);
				if (has_dp)	DisplayProperties::getInstance(0)->enableCreationForNewMolecules(true);
				return;
			}

			System* system = dynamic_cast<System*>(po);
			if (system == 0) continue;

			insert(*system);
			new_systems.push_back(system);
			current_composite++;
		}

		file.close();
		if (has_dp) DisplayProperties::getInstance(0)->enableCreationForNewMolecules(true);

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
				setStatusbarText("Could not read Camera position from project");
				Log.error() << "Could not read Camera position from project" << std::endl;
				return;
			}
			stage.setCamera(c);
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
	}

	void MainControl::setPreferencesEnabled_(bool state)
	{
		preferences_dialog_->ok_button->setEnabled(state);
	}


	void MainControl::setProxy(const String& host, Position port)
	{
		proxy_ = host;
		proxy_port_ = port;

		if (network_preferences_ != 0)
		{
			network_preferences_->getSettings();
		}
	}

	void MainControl::resize(int w, int h)
	{
		QMainWindow::resize(w, h);
	}

	bool MainControl::useMultithreading()
		throw() 
	{ 
		return multi_threading_mode_;
	}

	void MainControl::processEvents(Size ms)
	{
		qApp->processEvents(QEventLoop::AllEvents, ms);
	}
				
	void MainControl::quickSave() 
	{
		setStatusbarText("quick saving...", true);
		processEvents(500);

		String file = Directory::getUserHomeDir();
		file += FileSystem::PATH_SEPARATOR;
		file += "quick.bvp";
		saveBALLViewProjectFile(file);

		setStatusbarText("quick saving finished", true);
	}

	void MainControl::quickLoad()
	{
		setStatusbarText("quick loading...", true);
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
		setStatusbarText("quick loading finished", true);
	}

	void MainControl::updateRepLabel_()
	{
		if (!primitive_manager_.updateRunning()) 
		{
			if (!composites_locked_)
			{
				QCursor* cursor = QApplication::overrideCursor();
				if (cursor != 0 &&
						cursor->shape() == Qt::BusyCursor)
				{
					QApplication::restoreOverrideCursor();
				}
			}
			rep_label_->setText("");
			return;
		}

		rep_label_nr_ ++;
		rep_label_->show();
		char c = '-';
		if 		   (rep_label_nr_ == 2) c = '\\';
		else  if (rep_label_nr_ == 3) c = '|';
		else  if (rep_label_nr_ == 4) c = '/';
		else  if (rep_label_nr_ == 5) 
		{
			c = '-';
			rep_label_nr_ = 1;
		}

		QApplication::setOverrideCursor(QCursor(Qt::BusyCursor));

		rep_label_->setText(QString(c));
	}

	bool MainControl::isBusy() const
	{
		if (composites_locked_ || primitive_manager_.updateRunning()) return true;

		return false;
	}
		

#	ifdef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/VIEW/KERNEL/mainControl.iC>
#	endif

		} // namespace VIEW
} // namespace BALL
