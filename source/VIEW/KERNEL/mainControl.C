// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: mainControl.C,v 1.25 2003/12/09 16:40:35 amoll Exp $
//

#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/geometricObject.h>
#include <BALL/VIEW/KERNEL/modularWidget.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/DIALOGS/mainControlPreferences.h>
#include <BALL/VIEW/DIALOGS/preferences.h>

#include <BALL/VIEW/WIDGETS/pyWidget.h>
#include <BALL/VIEW/WIDGETS/logView.h>
#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/WIDGETS/geometricControl.h>


#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/MATHS/analyticalGeometry.h>
#include <BALL/MATHS/common.h>

#include <qapplication.h>
#include <qmenubar.h>
#include <qpopupmenu.h>
#include <qstatusbar.h>
#include <qlabel.h>
#include <qtooltip.h>

#include <algorithm> // sort


using std::istream;
using std::ostream;
using std::endl;

namespace BALL
{
	namespace VIEW
	{

MainControl::MainControl(QWidget* parent, const char* name, String inifile)
	throw()
	:	QMainWindow(parent, name),
		ConnectionObject(),
		Embeddable("BALL::VIEW::MainControl"),
		selection_(),
		message_label_(new QLabel("" , statusBar())),
		main_control_preferences_(0),
		preferences_dialog_(0),
		preferences_id_(-1),
		composites_muteable_(true),
		timer_(new StatusbarTimer(this))
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "new MainControl " << this << std::endl;
#endif
	// read the preferences
	preferences_.setFilename(inifile);
	preferences_.read();

	statusBar()->addWidget(message_label_, 20);

	timer_->setInterval(5000);
	timer_->setLabel(message_label_);

	initPopupMenu(EDIT);

	connect(qApp,	SIGNAL(aboutToQuit()), this, SLOT(aboutToExit()));
	connect(menuBar(),	SIGNAL(highlighted(int)), this, SLOT(menuItemHighlighted(int)));

	QToolTip::setWakeUpDelay(500);
	QToolTip::setGloballyEnabled(true);
}

MainControl::MainControl(const MainControl& main_control)
	throw()
	:	QMainWindow(0, ""),
		ConnectionObject(main_control),
		Embeddable(main_control),
		selection_(),
		main_control_preferences_(0),
		preferences_dialog_(0),
		preferences_id_(-1),
		composites_muteable_(main_control.composites_muteable_)
{
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
	if ((item == 0) || (item->popup() == 0))
	{
		menu = new QPopupMenu(this);
		CHECK_PTR(menu);

		connect(menu, SIGNAL(aboutToShow()), this, SLOT(checkMenus()));

		#ifdef BALL_VIEW_DEBUG
			Log.info() << "new menu entry: " << ID << endl;	
		#endif
		int max_id = menuBar()->count();
		switch (ID)
		{
			case FILE:
				menuBar()->insertItem("&File", menu, FILE, 0);
				break;
			case FILE_OPEN:
				initPopupMenu(MainControl::FILE)->insertItem("&Open", menu, FILE_OPEN);
				break;
			case FILE_IMPORT:
				initPopupMenu(MainControl::FILE)->insertItem("&Import", menu, FILE_IMPORT);
				break;
			case FILE_EXPORT:
				initPopupMenu(MainControl::FILE)->insertItem("&Export", menu, FILE_EXPORT);
				break;
			case EDIT:
				menuBar()->insertItem("&Edit", menu, EDIT, (1 <= max_id) ? 1 : -1);
				break;
			case BUILD:
				menuBar()->insertItem("&Build", menu, BUILD, (2 <= max_id) ? 2 : -1);
				break;
			case DISPLAY:
				menuBar()->insertItem("&Display", menu, DISPLAY, (3 <= max_id) ? 3 : -1);
				break;
			case DISPLAY_VIEWPOINT:
				initPopupMenu(MainControl::DISPLAY)->insertItem("&Viewpoint", menu, DISPLAY_VIEWPOINT);
				break;
			case TOOLS:
				menuBar()->insertItem("&Tools", menu, TOOLS, (4 <= max_id) ? 4 : -1);
				break;
			case TOOLS_CREATE_GRID:
				initPopupMenu(MainControl::TOOLS)->insertItem("&Create Grid", menu, TOOLS_CREATE_GRID);
				break;
			case TOOLS_PYTHON:
				initPopupMenu(MainControl::TOOLS)->insertItem("&Python", menu, TOOLS_PYTHON);
				break;
			case WINDOWS:
				menuBar()->insertItem("&Windows", menu, WINDOWS, (5 <= max_id) ? 5 : -1);
				break;
			case USER:
				menuBar()->insertItem("&User", menu, USER, (6 <= max_id) ? 6 : -1);
				break;
			case HELP:
				menuBar()->insertSeparator();
				menuBar()->insertItem("&Help", menu, HELP, -1);
				break;
			default:
				delete menu;
				menu = 0;
		}
	}
	else 
	{
		// return the existing popup menu
		menu = item->popup();
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

	// create own preferences dialog
	preferences_dialog_ = new Preferences(this, "Molview Preferences", 455, 352);

	if (preferences_dialog_ == 0)
	{
		throw Exception::GeneralException(__FILE__, __LINE__, 
				"memory allocation failed for preferences dialog.", "");
	}

	// establish connection 
	connect(preferences_dialog_, SIGNAL(applyButtonPressed()), this, SLOT(applyPreferencesTab()));
	connect(preferences_dialog_, SIGNAL(cancelButtonPressed()), this, SLOT(cancelPreferencesTab()));
	connect(preferences_dialog_, SIGNAL(defaultButtonPressed()), this, SLOT(defaultPreferencesTab()));

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
	applyPreferences(*preferences_dialog_);

	// check menu entries, fetch and apply preferences
	for (it = modular_widgets_.begin(); it != modular_widgets_.end(); ++it)
	{
		(*it)->checkMenu(*this);
		(*it)->fetchPreferences(preferences_);
		(*it)->applyPreferences(*preferences_dialog_);
	}

	// own menu entries
	insertPopupMenuSeparator(MainControl::FILE);
	insertMenuEntry(MainControl::FILE, "&Quit", qApp, SLOT(quit()), CTRL+Key_Q);	
	insertMenuEntry(MainControl::HELP, "Whats this?", this, SLOT(whatsThis()));	
	
	// if the preferences dialog has any tabs then show it
	if (preferences_dialog_->hasTabs())
	{
		initPopupMenu(MainControl::DISPLAY)->setCheckable(true);
		
		insertPopupMenuSeparator(MainControl::EDIT);
		preferences_id_ = insertMenuEntry(MainControl::EDIT, 
																			"Preferences", 
																			preferences_dialog_, 
																			SLOT(show()), CTRL+Key_Z);
	}

	restoreWindows();
	QMainWindow::show();
}

void MainControl::checkMenus()
{
	// preferences dialog not empty
	if (preferences_dialog_->hasTabs())
	{
		menuBar()->setItemChecked(preferences_id_, preferences_dialog_->isVisible());			
	}

	// checks all modular widgets 
	List<ModularWidget*>::Iterator it = modular_widgets_.begin(); 
	for (it = modular_widgets_.begin(); it != modular_widgets_.end(); ++it)
	{
		(*it)->checkMenu(*this);
	}
}

void MainControl::applyPreferencesTab()
{
	// apply on own preferences tab
	applyPreferences(*preferences_dialog_);

	// checks all modular widgets 
	List<ModularWidget*>::Iterator it = modular_widgets_.begin(); 
	for (it = modular_widgets_.begin(); it != modular_widgets_.end(); ++it)
	{
		(*it)->applyPreferences(*preferences_dialog_);
	}
}


void MainControl::cancelPreferencesTab()
	throw()
{
	// checks all modular widgets 
	List<ModularWidget*>::Iterator it = modular_widgets_.begin(); 
	for (it = modular_widgets_.begin(); it != modular_widgets_.end(); ++it)
	{
		(*it)->cancelPreferences(*preferences_dialog_);
	}
}

void MainControl::defaultPreferencesTab()
	throw()
{
	// checks all modular widgets if one of their preferences is the current page
	List<ModularWidget*>::Iterator it = modular_widgets_.begin(); 
	for (it = modular_widgets_.begin(); it != modular_widgets_.end(); ++it)
	{
		(*it)->defaultPreferences(*preferences_dialog_);
	}
}

void MainControl::aboutToExit()
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "MainControl::aboutToExit()" << std::endl;
#endif
	preferences_.clear();
	preferences_.appendSection("WINDOWS");

	// finalizes all modular widgets
	List<ModularWidget*>::Iterator it = modular_widgets_.begin(); 
	for (it = modular_widgets_.begin(); it != modular_widgets_.end(); ++it)
	{
		(*it)->writePreferences(preferences_);
		(*it)->finalizePreferencesTab(*preferences_dialog_);
		(*it)->finalizeWidget(*this);
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

bool MainControl::remove_(Composite& composite)
	throw()
{
	if (!composite_manager_.has(composite)) return false;
	
	Composite* root = 0;
	if (composite.getRoot() != composite) root = &composite.getRoot();
	composite_manager_.remove(composite);

	// delete all representations containing the composite
	List<Representation*> removed_representations;
	removed_representations = primitive_manager_.removedComposite(composite);
	List<Representation*>::Iterator reps_it = removed_representations.begin();
	// notify GeometricControl of removed representations
	for (; reps_it != removed_representations.end(); reps_it++)
	{
		RepresentationMessage* rr_message = new RepresentationMessage(*reps_it, RepresentationMessage::REMOVE);
		notify_(rr_message);
	}

	if (root != 0) updateRepresentationsOf(*root, true, true);

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
		if (rep->getModelType() == MODEL_SE_SURFACE ||
				rep->getModelType() == MODEL_SA_SURFACE ||
				rep->getModelType() == MODEL_BACKBONE 	||
				rep->getModelType() == MODEL_CARTOON    ||
				force)
		{
			rep->update(rebuild);
		}
		RepresentationMessage* ur_message = new RepresentationMessage(rep, RepresentationMessage::UPDATE);
		notify_(ur_message);
	}

	SceneMessage *scene_message = new SceneMessage(SceneMessage::REDRAW);
	notify_(scene_message);

	return true;
}


void MainControl::updateAllRepresentations(bool rebuild_display_lists)
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
				remove_(*cmessage->getComposite());
				return;
			case CompositeMessage::CHANGED_COMPOSITE_AND_UPDATE_MOLECULAR_CONTROL:
			case CompositeMessage::CHANGED_COMPOSITE:
				update(cmessage->getComposite()->getRoot());
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

				updateRepresentationsOf(*cmessage->getComposite(), false);

				NewSelectionMessage* nws_message = new NewSelectionMessage;					
				notify_(nws_message); // send to MolecularControl
			}
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

int MainControl::insertMenuEntry(int ID, const String& name, const QObject* receiver, const char* slot, 
																 int accel, int entry_ID, String hint)
	throw()
{
	QMenuBar* menu_bar = menuBar();
	if (menu_bar == 0) return -1;
	
	// enable the corresponding popup menu
	menu_bar->setItemEnabled(ID, true);
	QPopupMenu* popup = initPopupMenu(ID);
	if (popup == 0)
	{
		Log.error() << "MainControl::insertMenuEntry: cannot find popup menu for ID " << ID << endl;
		return -1;
	}

	if (entry_ID == -1) entry_ID = getNextID_();
	popup->insertItem(name.c_str(), receiver, slot, accel, entry_ID);

	setMenuHint(entry_ID, hint);

	return entry_ID;
}


void MainControl::removeMenuEntry
	(int /* ID */, const String& /* name */, 
	 const QObject* /* receiver */, const char* /* slot */, 
	 int /* accel */, int /* entry_ID */)
	throw()
{
	// ?????
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

	preferences.insertTab(main_control_preferences_, "General");
}

void MainControl::finalizePreferencesTab(Preferences &preferences)
	throw()
{
	if (main_control_preferences_ != 0)
	{
		preferences.removeTab(main_control_preferences_);

		delete main_control_preferences_;
		main_control_preferences_ = 0;
	}
}

void MainControl::applyPreferences(Preferences & /* preferences */)
	throw()
{
	if (main_control_preferences_ != 0)
	{
		QApplication::setStyle(main_control_preferences_->getStyle());
		BALL_VIEW_DOCKWINDOWS_SHOW_LABELS = main_control_preferences_->showLabelsEnabled();
		QWidget::update();
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
	setGeometry(x_pos, y_pos, w, h);
	
	// the default preferences tab (if existent)
	if (main_control_preferences_ != 0)
	{
		main_control_preferences_->fetchPreferences(inifile);
	}
}

void MainControl::writePreferences(INIFile &inifile)
	throw()
{
	// the main window position
	inifile.insertValue("WINDOWS", "Main::x", String(x()));
	inifile.insertValue("WINDOWS", "Main::y", String(y()));
	inifile.insertValue("WINDOWS", "Main::width", String(width()));
	inifile.insertValue("WINDOWS", "Main::height", String(height()));

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
if (RTTI::isKindOf<Atom>(*composite))

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
			setStatusbarText("Properties of atom " + atom.getFullName() + "  Type: " + 
											 String(atom.getType()) + "  Typename: " + 
											 String(atom.getTypeName()) + ":  Position: (" + 
											 String(atom.getPosition().x) + "|" +
											 String(atom.getPosition().y) + "|" +
											 String(atom.getPosition().z) + ")" + "  Charge: " + 
											 String(atom.getCharge()));
			break;
		}
		case 2:
		{
			// if two atoms were picked, show their distance
			setStatusbarText("Distance between atom " + 
												atoms[0]->getFullName() + " and " + 
												atoms[1]->getFullName() + ": " + 
												String(GetDistance(atoms[0]->getPosition(), atoms[1]->getPosition())));
			break;
		}
		case 3:
		{
			Vector3 vector1(ordered_atoms[1]->getPosition() - ordered_atoms[2]->getPosition());
			Vector3 vector2(ordered_atoms[1]->getPosition() - ordered_atoms[0]->getPosition());
			Angle result;
			GetAngle(vector1, vector2, result);
			setStatusbarText("Angle between atoms " + 
												atoms[0]->getFullName() + ", " + 
												atoms[1]->getFullName() + ", " +
												atoms[2]->getFullName() + ": " +
												String(result.toDegree())); 
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

			setStatusbarText("Torsion angle between atoms " + 
												ordered_atoms[0]->getFullName() + ", " + 
												ordered_atoms[1]->getFullName() + ", " +
												ordered_atoms[2]->getFullName() + ", " +
												ordered_atoms[3]->getFullName() + ": " +
												String(result.toDegree()));
			break;
		}
	}
}


const HashSet<Composite*>& MainControl::getSelection() const
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
	if (selection_.has(composite)) return;

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
	if (!selection_.has(composite)) return;

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


void MainControl::setStatusbarText(const String& text)
	throw()
{
	message_label_->setText(text.c_str());
	timer_->stopTimer();
	timer_->startTimer();
	QWidget::update();
}

void MainControl::restoreWindows()
	throw()
{
	if (!getINIFile().hasEntry("WINDOWS", "Main::dockwidgets")) return;

	String mys(getINIFile().getValue("WINDOWS", "Main::dockwidgets"));
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

bool MainControl::insert(Composite& composite)
	throw()
{
	if (composite_manager_.has(composite)) return false;

	composite_manager_.insert(composite);
	CompositeMessage* cm = new CompositeMessage(composite, 
			CompositeMessage::NEW_COMPOSITE);
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

	RepresentationMessage* rm = new RepresentationMessage(&rep, RepresentationMessage::ADD);
	notify_(rm);

	return true;
}

bool MainControl::update(Representation& rep)
	throw()
{
	if (!primitive_manager_.has(rep)) return false;

	RepresentationMessage* rm = new RepresentationMessage(&rep, RepresentationMessage::UPDATE);
	notify_(rm);

	SceneMessage *scene_message = new SceneMessage(SceneMessage::REDRAW);
	notify_(scene_message);

	return true;
}

bool MainControl::remove(Representation& rep)
	throw()
{
	if (!primitive_manager_.has(rep)) return false;

	RepresentationMessage* rm = new RepresentationMessage(&rep, RepresentationMessage::REMOVE);
	notify_(rm);
	primitive_manager_.remove(rep);
	return true;
}
// ======================= StatusbarTimer =========================
StatusbarTimer::StatusbarTimer(QObject* parent)
	throw()
	: QTTimer(parent),
		label_(0)
{}

void StatusbarTimer::timer()
	throw()
{
	if (!label_) return;
	label_->setText("");
	stopTimer();
}

void StatusbarTimer::setLabel(QLabel* label)
	throw()
{
	label_ = label;
}

#	ifdef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/VIEW/KERNEL/mainControl.iC>
#	endif

} } // namespaces
