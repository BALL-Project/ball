#include "mainframe.h"
#include "icons.h"
#include "DIALOGS/DlgAbout.h"
#include "DIALOGS/DlgSelector.h"
#include "DIALOGS/DlgDisplayProperties.h"
#include <qcolordialog.h>
#include <qfiledialog.h>
#include <qlabel.h>
#include <qtooltip.h>
#include <qfile.h>

#include <BALL/MOLMEC/AMBER/amber.h>
#include <BALL/MOLMEC/MINIMIZATION/conjugateGradient.h>
#include <BALL/MOLMEC/MINIMIZATION/steepestDescent.h>
#include <BALL/MOLVIEW/OPENGL/KERNEL/moleculeObjectCreator.h>

using namespace std;

Mainframe::Mainframe
  (QWidget* parent, const char* name)
	:	scene_(0),
		object_processor_(),
		GL_object_collector_(),
		edit_menu_(0),
		hor_splitter_(0),
		vert_splitter_(0),
		control_(0),
		logview_(0),
		vboxlayout_(0),
		display_properties_dialog_(this),
		preferences_dialog_(this),

		popup_menus_(),
		preferences_(".molview.ini"),
		fragment_db_(),
		selection_(),
		copy_list_(),
		server_icon_(0),
		tool_box_(0),
		server_()
{
	// ---------------------
	// setup main window
	// ---------------------
	setCaption("MolVIEW");
	setIcon(*new QPixmap(bucky_64x64_xpm));

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

	control_ = new Control(hor_splitter_);
	CHECK_PTR(control_);

	scene_ = new Scene(hor_splitter_);
	CHECK_PTR(scene_);

	logview_ = new LogView(vert_splitter_);
	CHECK_PTR(logview_);

	QLabel* message_label = new QLabel(tr("Ready."), statusBar());
	statusBar()->addWidget(message_label, 20);


	server_icon_ = new QLabel(statusBar());
	statusBar()->addWidget(server_icon_, 1, TRUE );
	QToolTip::add(server_icon_, "VIEW server status");
	QPixmap icon(mini_ray_xpm);

	server_icon_->setPixmap(icon);
	server_icon_->show();
 
	// ---------------------
	// Scene setup ---------
	// ---------------------

	scene_->setGLObjectCollector(GL_object_collector_);

	// ---------------------
	// Control setup -------
	// ---------------------
	
	control_->addColumn("Name");
	control_->addColumn("Type");
	control_->addColumn("Display");
	control_->setColumnWidth(0, 80);
	control_->setColumnWidth(1, 60);

	// ---------------------
	// LogView setup ------
	// ---------------------
	
	QFont f("Courier", 12, QFont::DemiBold, false);
	logview_->setFont(f);

	Log.info() << "Welcome to MolVIEW." << endl;


	// ---------------------
	// Menu ----------------
	// ---------------------

	// File-Menu -------------------------------------------------------------------

	QPopupMenu* files_menu = new QPopupMenu(this);
	CHECK_PTR(files_menu);
	popup_menus_.push_back(files_menu);

	QPopupMenu* file_menu = new QPopupMenu(this);
	CHECK_PTR(file_menu);
	popup_menus_.push_back(file_menu);

	QPopupMenu* export_menu = new QPopupMenu(this);
	CHECK_PTR(export_menu);
	popup_menus_.push_back(export_menu);
	
	export_menu->insertItem("&PovRay", this, SLOT(exportPovray()), CTRL+Key_P, MENU__EXPORT_POVRAY);
	export_menu->setItemEnabled(MENU__EXPORT_POVRAY, false);

	file_menu->insertItem("&Import File", files_menu, CTRL+Key_I);
	file_menu->insertItem("&Export File", export_menu, CTRL+Key_E);
	file_menu->insertSeparator();
	file_menu->insertItem("E&xit", qApp, SLOT(quit()), CTRL+Key_X);

	files_menu->insertItem("&PDB File", this, SLOT(importPDB()), CTRL+Key_P, MENU__OPEN_FILE_PDB);
	files_menu->insertItem("&HIN File", this, SLOT(importHIN()), CTRL+Key_H, MENU__OPEN_FILE_HIN);
	files_menu->insertItem("&MOL2 File", MENU__OPEN_FILE_MOL2);
	files_menu->setItemEnabled(MENU__OPEN_FILE_MOL2, false);


	// Edit-Menu -------------------------------------------------------------------
	
	QPopupMenu* edit_menu_ = new QPopupMenu(this);
	CHECK_PTR(edit_menu_);
	popup_menus_.push_back(edit_menu_);

	edit_menu_->insertItem("&Cut", this, SLOT(cut()), CTRL+Key_C, MENU__EDIT_CUT);
	edit_menu_->insertItem("C&opy", this, SLOT(copy()), CTRL+Key_O, MENU__EDIT_COPY);
	edit_menu_->insertItem("&Paste", this, SLOT(paste()), CTRL+Key_P, MENU__EDIT_PASTE);
	edit_menu_->insertItem("&Erase", this, SLOT(erase()), CTRL+Key_P, MENU__EDIT_DELETE);
	edit_menu_->insertSeparator();
	edit_menu_->insertItem("&Select", this, SLOT(select()), CTRL+Key_S, MENU__EDIT_SELECT);
	edit_menu_->insertItem("&Deselect", this, SLOT(deselect()), CTRL+Key_D, MENU__EDIT_DESELECT);
	edit_menu_->insertSeparator();
	edit_menu_->insertItem("Cl&ear Clipboard", this, SLOT(clearClipboard()), CTRL+Key_E, MENU__EDIT_CLEAR_CLIPBOARD);

	edit_menu_->setItemEnabled(MENU__EDIT_CUT, FALSE);
	edit_menu_->setItemEnabled(MENU__EDIT_COPY, FALSE);
	edit_menu_->setItemEnabled(MENU__EDIT_PASTE, FALSE);
	edit_menu_->setItemEnabled(MENU__EDIT_DELETE, FALSE);
	edit_menu_->setItemEnabled(MENU__EDIT_SELECT, FALSE);
	edit_menu_->setItemEnabled(MENU__EDIT_DESELECT, FALSE);
	edit_menu_->setItemEnabled(MENU__EDIT_CLEAR_CLIPBOARD, FALSE);

	QPopupMenu* build_menu = new QPopupMenu(this);
	CHECK_PTR(build_menu);
	popup_menus_.push_back(build_menu);

	build_menu->insertItem("Check St&ructure", this, 
												 SLOT(checkResidue()), CTRL+Key_R, MENU__BUILD_CHECK_RESIDUE);
	build_menu->insertItem("&Build Bonds", this, 
												 SLOT(buildBonds()), CTRL+Key_B, MENU__BUILD_BUILD_BONDS);
	build_menu->insertItem("Add &Hydrogens", this, 
												 SLOT(addHydrogens()), CTRL+Key_H, MENU__BUILD_ADD_HYDROGENS);
	build_menu->insertItem("Assign &Charges", this, 
												 SLOT(assignCharges()), CTRL+Key_H, MENU__BUILD_ASSIGN_CHARGES);
	build_menu->insertSeparator();
	build_menu->insertItem("Calculate AMBER &Energy", this, 
												 SLOT(calculateAmberEnergy()), CTRL+Key_U, MENU__BUILD_AMBER_ENERGY);
	build_menu->insertItem("Perform Energy &Minimization", this, 
												 SLOT(amberMinimization()), CTRL+Key_W, MENU__BUILD_AMBER_MINIMIZATION);
	

	// Display Menu -------------------------------------------------------------------
	
	QPopupMenu* display_menu = new QPopupMenu(this);
	CHECK_PTR(display_menu);
	popup_menus_.push_back(display_menu);
	display_menu->setCheckable(TRUE);

	display_menu->insertItem("D&isplay Properties", this, 
													 SLOT(openDisplayPropertiesDialog()),
													 CTRL+Key_I, MENU__DISPLAY_OPEN_DISPLAY_PROPERTIES_DIALOG);
	display_menu->insertItem("&Preferences", this, 
													 SLOT(openPreferencesDialog()),
													 CTRL+Key_I, MENU__DISPLAY_OPEN_PREFERENCES_DIALOG);
	display_menu->insertSeparator();
	display_menu->insertItem("Focus C&amera", this, SLOT(centerCamera()), CTRL+Key_A, MENU__DISPLAY_CENTER_CAMERA);
	display_menu->setItemEnabled(MENU__DISPLAY_CENTER_CAMERA, FALSE);

	// Help-Menu -------------------------------------------------------------------

	QPopupMenu* help_menu = new QPopupMenu(this);
	CHECK_PTR(help_menu);
	popup_menus_.push_back(help_menu);
	
	help_menu->insertItem("&About", this, SLOT(about()), CTRL+Key_A, MENU__HELP_ABOUT);

	// Menu ------------------------------------------------------------------------

	menuBar()->insertItem("&File", file_menu);
	menuBar()->insertItem("&Edit", edit_menu_);
	menuBar()->insertItem("&Build", build_menu);
	menuBar()->insertItem("&Display", display_menu);
	menuBar()->insertSeparator();
	menuBar()->insertItem("&Help", help_menu);
	menuBar()->setSeparator(QMenuBar::InWindowsStyle);
	

	// ---------------------
	// Connectivity --------
	// ---------------------

	connect(display_menu,
					SIGNAL(aboutToShow()),
					this,
					SLOT(checkMenuEntries()));
	connect(build_menu,
					SIGNAL(aboutToShow()),
					this,
					SLOT(checkMenuEntries()));
	connect(edit_menu_,
					SIGNAL(aboutToShow()),
					this,
					SLOT(checkMenuEntries()));
	connect(&display_properties_dialog_,
					SIGNAL(apply()),
					this,
					SLOT(applyDisplayPropertiesDialog()));
	connect(&preferences_dialog_,
					SIGNAL(applyButtonPressed()),
					this,
					SLOT(applyPreferencesDialog()));

	// ---------------------
	// read preferences ----
	// ---------------------
	preferences_.setFilename(".molview.ini");
	preferences_.read();
	getPreferences(preferences_);	

	//--------------------------------
	// setup display properties dialog
	//--------------------------------

	display_properties_dialog_.setObjectProcessor(object_processor_);
	display_properties_dialog_.getPreferences(preferences_);

	//--------------------------------
	// setup preferences dialog
	//--------------------------------
	preferences_dialog_.getPreferences(preferences_);


	//--------------------------------
	// setup the VIEW server
	//--------------------------------
  // connecting server with mainframe
  NotificationRegister(server_, *this);

  // registering scene for creation
  server_.registerCreationScene(getScene());

  // registering object generator
  MoleculeObjectCreator object_creator;
  server_.registerObjectCreator(object_creator);

  // registering object processor
  server_.registerObjectProcessor(getObjectProcessor());

	// check whether we should start the server
	// and on which port
	checkServer();
}

Mainframe::~Mainframe()
{
	//
	// extract preferences 
	// from the current settings
	//
	setPreferences(preferences_);
	display_properties_dialog_.setPreferences(preferences_);
	preferences_dialog_.setPreferences(preferences_);

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

void Mainframe::importPDB()
{
	QStringList string_list;
	string_list = "PDB files (*.pdb)";
	string_list += "PDB files (*.brk)";
	string_list += "PDB files (*.ent)";
	string_list += "All files (*.*)";

	QFileDialog file_dialog(this, 0, TRUE);
	file_dialog.setFilters(string_list);
	file_dialog.setCaption("Import PDB file");

	if (file_dialog.exec())
	{
		QString filename = file_dialog.selectedFile();

		if (!filename.isNull())
		{
			// test, if file is existent
			// if the file does not exist => exit from function
			if (!QFile::exists(filename))
			{
				return;
			}

			// PDB file laden
			statusBar()->message("reading PDB-File...");

			PDBFile pdb_file(filename.ascii());

			System system;
			pdb_file >> system;
			pdb_file.close();

			Log.info() << "> read " << system.countAtoms() << " atoms from PDB file \"" 
								 << filename.ascii() << "\"" << endl;

			statusBar()->message("normalizing names...");
	    system.apply(*fragment_db_.normalizeNames);
			Log.info() << "> normalized names" << endl;

			statusBar()->message("generating missing bonds...");
	    system.apply(*fragment_db_.buildBonds);
			Log.info() << "> generated missing bonds" << endl;
  
			// construct a name (the filename without the dir path)
			filename.remove(0, file_dialog.dirPath().length() + 1);

			if (filename.find('.') != -1)
			{
				filename = filename.left(filename.find('.'));
			}

			addComposite(&system, &filename);
			statusBar()->clear();
		}
	}
}

void Mainframe::importHIN()
{
	QStringList string_list;
	string_list = "HIN files (*.hin)";
	string_list += "All files (*.*)";

	QFileDialog file_dialog(this, 0, TRUE);
	file_dialog.setFilters(string_list);
	file_dialog.setCaption("Import HIN file");

	if (file_dialog.exec())
	{
		QString filename = file_dialog.selectedFile();

		if (!filename.isNull())
		{
			// test, if file is existent
			// if the file does not exist => exit from function
			if (!QFile::exists(filename))
			{
				return;
			}

			statusBar()->message("reading HIN file...");

			HINFile hin_file(filename.ascii());

			System system;
			hin_file >> system;
			hin_file.close();

			// construct a name (the filename without the dir path)
			filename.remove(0, file_dialog.dirPath().length() + 1);

			if (filename.find('.') != -1)
			{
				filename = filename.left(filename.find('.'));
			}

			Log.info() << "> read " << system.countAtoms() << " atoms from HIN-File \"" << filename.ascii() << "\"" << endl;

			addComposite(&system, &filename);
			
			statusBar()->clear();
		}
	}
}

void Mainframe::exportPovray()
{
	QMessageBox::about(this, "export Povray", "available in Version 2.0");
}

void Mainframe::about()
{
	DlgAbout about_box;
	about_box.exec();
	statusBar()->message("MolVIEW V 0.9 alpha", 1500);
}

void Mainframe::checkMenuEntries()
{
	// extract the selection of the tree
	const List<Composite*>& selection = control_->getSelection();
	bool selected = (selection.size() > 0);

	// enable for multiple selection
	menuBar()->setItemEnabled(MENU__EDIT_SELECT, selected);
	menuBar()->setItemEnabled(MENU__EDIT_DESELECT, selected);
	menuBar()->setItemEnabled(MENU__BUILD_ADD_HYDROGENS, selected);
	menuBar()->setItemEnabled(MENU__BUILD_ASSIGN_CHARGES, selected);
	menuBar()->setItemEnabled(MENU__BUILD_CHECK_RESIDUE, selected);
	menuBar()->setItemEnabled(MENU__BUILD_BUILD_BONDS, selected);
	
	// these menu points for single items only
	menuBar()->setItemEnabled(MENU__DISPLAY_CENTER_CAMERA, selected && (selection.size() == 1));

	// set the checkboxes for the non-modal dialogs
	menuBar()->setItemChecked(MENU__DISPLAY_OPEN_DISPLAY_PROPERTIES_DIALOG, 
													 display_properties_dialog_.isVisible());
	menuBar()->setItemChecked(MENU__DISPLAY_OPEN_PREFERENCES_DIALOG, 
													 preferences_dialog_.isVisible());

	// check for paste-slot: enable only if copy_list_ not empty
	menuBar()->setItemEnabled
		(MENU__EDIT_PASTE, 
		 (bool)(copy_list_.size() > 0));

	// check for clearClipboard-slot: enable only if copy_list_ not empty
	menuBar()->setItemEnabled
		(MENU__EDIT_CLEAR_CLIPBOARD, 
		 (bool)(copy_list_.size() > 0));

	// check for erase-slot: enable erase-slot only if all selected composites
	// are systems
	bool all_systems = (selection.size() > 0);

	List<Composite*>::ConstIterator list_it = selection.begin();	
	for (; list_it != selection.end(); ++list_it)
	{
		if (!RTTI::isKindOf<System>(**list_it))
		{
			all_systems = false;
		}
	}

	// cut, copy, and paste will are only available for 
	// top level selections
	menuBar()->setItemEnabled(MENU__EDIT_CUT, all_systems);
	menuBar()->setItemEnabled(MENU__EDIT_COPY, all_systems);
	menuBar()->setItemEnabled(MENU__EDIT_DELETE, all_systems);
	
	// AMBER methods are available only for single systems
	menuBar()->setItemEnabled(MENU__BUILD_AMBER_ENERGY, (all_systems && (selection.size() == 1)));
	menuBar()->setItemEnabled(MENU__BUILD_AMBER_MINIMIZATION, (all_systems && (selection.size() == 1)));
}

void Mainframe::cut()
{
	const List<Composite*>& selection = control_->getSelection();
	if (selection.size() == 0)
	{
		return;
	}

	QString message;
	message.sprintf("cutting %d systems...", 
													selection.size());
	statusBar()->message(message);

	// delete old composites
	if (copy_list_.size() > 0)
	{
		List<Composite*>::ConstIterator list_it = copy_list_.begin();	
		for (; list_it != copy_list_.end(); ++list_it)
		{
			delete *list_it;
		}

		copy_list_.clear();
	}

	// remove all system composites from the tree and from the scene
	// but do not delete them from memory
	List<Composite*>::ConstIterator list_it = selection.begin();	
	for (; list_it != selection.end(); ++list_it)
	{
		// insert deep clone of the composite into the cut list
		copy_list_.push_back((Composite*)(*list_it)->create());

		removeComposite(*list_it);
	}

	scene_->update();

	control_->invalidateSelection();

	statusBar()->clear();
}

void Mainframe::copy()
{
	const List<Composite*>& selection = control_->getSelection();
	if (selection.size() == 0)
	{
		return;
	}

	QString message;
	message.sprintf("copying %d systems...", 
													selection.size());
	statusBar()->message(message);

	// delete old cutted composites
	if (copy_list_.size() > 0)
	{
		List<Composite*>::ConstIterator list_it = copy_list_.begin();	
		for (; list_it != copy_list_.end(); ++list_it)
		{
			delete *list_it;
		}

		copy_list_.clear();
	}

	// copy the selected composites into the copy_list_
	List<Composite*>::ConstIterator list_it = selection.begin();	
	for (; list_it != selection.end(); ++list_it)
	{
		// insert deep clone of the composite into the cut list
		copy_list_.push_back((Composite*)(*list_it)->create());
	}

	statusBar()->clear();
}

void Mainframe::paste()
{
	if (copy_list_.size() == 0)
	{
		return;
	}

	QString message;
	message.sprintf("pasting %d systems...", 
													copy_list_.size());
	statusBar()->message(message);

	// copying composites
	List<Composite*>::ConstIterator list_it = copy_list_.begin();	
	for (; list_it != copy_list_.end(); ++list_it)
	{
		addComposite(*list_it, 0);
	}

	control_->invalidateSelection();

	statusBar()->clear();
}

void Mainframe::erase()
{
	const List<Composite*>& selection = control_->getSelection();
	if (selection.size() == 0)
	{
		return;
	}

	QString message;
	message.sprintf("erasing %d systems...", 
													selection.size());
	statusBar()->message(message);

	// erase all system composites
	List<Composite*>::ConstIterator list_it = selection.begin();	
	for (; list_it != selection.end(); ++list_it)
	{
		removeComposite(*list_it);
	}

	control_->invalidateSelection();

	scene_->update();
	statusBar()->clear();
}

void Mainframe::clearClipboard()
{
	statusBar()->message("clearing clipboard...");

	// delete old composites
	if (copy_list_.size() > 0)
	{
		List<Composite*>::ConstIterator list_it = copy_list_.begin();	
		for (; list_it != copy_list_.end(); ++list_it)
		{
			delete *list_it;
		}

		copy_list_.clear();
	}

	statusBar()->clear();
}

void Mainframe::select()
{
	const List<Composite*>& selection = control_->getSelection();
	if (selection.size() == 0)
	{
		return;
	}

	QString message;
	message.sprintf("selecting %d objects...", 
													selection.size());
	statusBar()->message(message);

	int value_static = object_processor_.getValue(ADDRESS__STATIC_MODEL);
	int value_dynamic = object_processor_.getValue(ADDRESS__DYNAMIC_MODEL);

	object_processor_.setValue(ADDRESS__STATIC_MODEL, VALUE__SELECT);
	object_processor_.setValue(ADDRESS__DYNAMIC_MODEL, VALUE__SELECT);
        
	List<Composite*>::ConstIterator list_it = selection.begin();	
	for (; list_it != selection.end(); ++list_it)
	{
		object_processor_.applyOn(**list_it);
		scene_->getCompositeManager()->update((**list_it).getRoot());
	}

	// restore old values
	object_processor_.setValue(ADDRESS__STATIC_MODEL, value_static);
	object_processor_.setValue(ADDRESS__DYNAMIC_MODEL, value_dynamic);

	scene_->update();
	statusBar()->clear();
}

void Mainframe::deselect()
{
	const List<Composite*>& selection = control_->getSelection();
	if (selection.size() == 0)
	{
		return;
	}

	QString message;
	message.sprintf("selecting %d objects...", selection.size());
	statusBar()->message(message);

	int value_static = object_processor_.getValue(ADDRESS__STATIC_MODEL);
	int value_dynamic = object_processor_.getValue(ADDRESS__DYNAMIC_MODEL);

	object_processor_.setValue(ADDRESS__STATIC_MODEL, VALUE__DESELECT);
	object_processor_.setValue(ADDRESS__DYNAMIC_MODEL, VALUE__DESELECT);
        
	List<Composite*>::ConstIterator list_it = selection.begin();	
	for (; list_it != selection.end(); ++list_it)
	{
		object_processor_.applyOn(**list_it);
		scene_->getCompositeManager()->update((**list_it).getRoot());
	}

	// restore old values
	object_processor_.setValue(ADDRESS__STATIC_MODEL, value_static);
	object_processor_.setValue(ADDRESS__DYNAMIC_MODEL, value_dynamic);

	scene_->update();
	statusBar()->clear();
}

void Mainframe::checkResidue()
{
	const list<Composite*>& selection = control_->getSelection();
	if (selection.size() == 0)
	{
		return;
	}

	QString message;
	message.sprintf("selecting %d objects...", selection.size());
	statusBar()->message(message);

	ResidueChecker res_check(fragment_db_);
	bool okay = true;
	List<Composite*>::ConstIterator list_it = selection.begin();	
	for (; list_it != selection.end(); ++list_it)
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
}

void Mainframe::centerCamera()
{
	const List<Composite*>& selection = control_->getSelection();
	if (selection.size() != 1)
	{
		return;
	}

  // use specified object processor for calculating the center
  object_processor_.calculateCenter(*selection.front());

  // set the camera on the the new composite
  scene_->camera.set
		(object_processor_.getViewCenter(),
		 (Camera::ViewDirection)object_processor_.getViewDirection(),
		 object_processor_.getViewDistance());

  scene_->update(); 
}

void Mainframe::buildBonds()
{
	const list<Composite*>& selection = control_->getSelection();
	if (selection.size() == 0)
	{
		return;
	}

	Size number_of_bonds = 0;

	statusBar()->message("building bonds...");
	List<Composite*>::ConstIterator list_it = selection.begin();	
	for (; list_it != selection.end(); ++list_it)
	{	
		(*list_it)->apply(*fragment_db_.buildBonds);
		number_of_bonds += fragment_db_.buildBonds->getNumberOfBondsBuilt();
		object_processor_.applyOn(**list_it);
		scene_->getCompositeManager()->update((*list_it)->getRoot());
	}

	scene_->update();
	statusBar()->clear();
	Log.info() << "Added " << number_of_bonds << " bonds." << endl;
}

void Mainframe::addHydrogens()
{
	const list<Composite*>& selection = control_->getSelection();
	if (selection.size() == 0)
	{
		return;
	}

	Size number_of_hydrogens = 0;

	statusBar()->message("adding hydrogens...");
	List<Composite*>::ConstIterator list_it = selection.begin();	
	for (; list_it != selection.end(); ++list_it)
	{	
		(*list_it)->apply(*fragment_db_.addHydrogens);
		number_of_hydrogens += fragment_db_.addHydrogens->getNumberOfInsertedH();
		(*list_it)->apply(*fragment_db_.buildBonds);
		object_processor_.applyOn(**list_it);
		scene_->getCompositeManager()->update((*list_it)->getRoot());
	}

	Log.info() << "added " << number_of_hydrogens << " hydrogen atoms." << endl;
	statusBar()->clear();

	// update the marked representations
	scene_->update();
}

void Mainframe::assignCharges()
{
	
}

void Mainframe::applyDisplayPropertiesDialog()
{
	const list<Composite*>& selection = control_->getSelection();
	if (selection.size() == 0)
	{
		return;
	}

	QString message;
	message.sprintf("setting display properties for %d objects...", 
													selection.size());
	statusBar()->message(message);

	List<Composite*>::ConstIterator list_it = selection.begin();	
	for (; list_it != selection.end(); ++list_it)
	{
		object_processor_.applyOn(**list_it);
		scene_->getCompositeManager()->update((**list_it).getRoot());
	}
	scene_->update();
	statusBar()->clear();
}

void Mainframe::applyPreferencesDialog()
{
	if (preferences_dialog_.isTabEnabled(preferences_dialog_.getGeneralTab()))
	{
		QApplication::setStyle(preferences_dialog_.getGeneralTab()->getStyle());
		update();
	}
	
	if (preferences_dialog_.isTabEnabled(preferences_dialog_.getNetworkTab()))
	{
		checkServer();
	}
	
	if (preferences_dialog_.isTabEnabled(preferences_dialog_.getDisplayTab()))
	{
		if (preferences_dialog_.getDisplayTab()->isDepthCueingEnabled())
		{
		}

		if (preferences_dialog_.getDisplayTab()->isCoordinateAxesEnabled())
		{
		}
	}	
}

void Mainframe::openDisplayPropertiesDialog()
{
	display_properties_dialog_.show();
	display_properties_dialog_.raise();
}

void Mainframe::openPreferencesDialog()
{
	preferences_dialog_.show();
	preferences_dialog_.raise();
}

void Mainframe::resizeEvent
  (QResizeEvent* /* event */)
{
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

void Mainframe::addComposite(Composite* composite, QString* name)
{
  if (composite == 0)
  {
    return;
	}

	// insert into the scene
	statusBar()->message("creating graphical representation...");

	// create a new composite descriptor
  CompositeDescriptor composite_descriptor;
	String descriptor_name;
	if (name != 0)
	{
		descriptor_name = name->ascii();
	}
  composite_descriptor.setName(descriptor_name);
  composite_descriptor.setComposite(*composite);
 
  CompositeDescriptor* new_composite_descriptor
    = scene_->getCompositeManager()->insert(composite_descriptor, true);

  // if the descriptor is 0 => something very bad has happened
  if (new_composite_descriptor == 0)
  {
    QMessageBox::about(this, "Load object error", "descriptor deep clone failed!");
    return;
	}

	// retrieve the cloned composite
  Composite* new_composite = new_composite_descriptor->getComposite();

  // use specified object processor for building the visible object
  // and setting the properties
  object_processor_.applyOn(*new_composite);

  // set the center of the composite
  new_composite_descriptor->setCenter(object_processor_.getViewCenter());
 
	// set the camera on the the new composite
  scene_->camera.
    set(object_processor_.getViewCenter(),
				(Camera::ViewDirection)object_processor_.getViewDirection(),
        object_processor_.getViewDistance());

  // update the scene
  scene_->update();
 

  // generate the tree representation
  statusBar()->message("> generating tree representation... ");
	statusBar()->update();

  // generate ListViewItem
  control_->addComposite(new_composite, name);	
}

void Mainframe::removeComposite(Composite* composite)
{
	// remove the tree from the control
	control_->removeComposite(composite);

	// removes the composite from the scene
	scene_->getCompositeManager()->remove(*composite);
}

bool Mainframe::onNotify(Server& server)
{
	addComposite((Composite *)server.getReceivedComposite(), 0);

	return true;
}

void Mainframe::startServer()
{
	// retrieve the port number
	int port = preferences_dialog_.getNetworkTab()->getPort();
	
	// set the port and active the server
	server_.setPort(port);
	server_.activate();
	
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
	server_.deactivate();

	// hide the icon
	server_icon_->hide();
	QToolTip::add(server_icon_, "VIEW Server disabled");
	statusBar()->update();
}

void Mainframe::checkServer()
{
	// retrieve the settings of the preferences
	// dialog box
	bool start_server = preferences_dialog_.getNetworkTab()->getServerStatus();
	
	if (start_server)
	{
		startServer();
	} else {
		stopServer();
	}
}

void Mainframe::calculateAmberEnergy()
{
	const List<Composite*> selection = control_->getSelection();
	if (selection.size() != 1)
	{
		return;
	}

	if (!RTTI::isKindOf<System>(*selection.front()))
	{
		return;
	}

	// retrieve the system from the selection
	System& system = *RTTI::castTo<System>(*selection.front());

	// set up the AMBER force field
	statusBar()->message("setting up force field...");
	AmberFF amber;
	amber.options[AmberFF::Option::ASSIGN_TYPES] = "true";
	amber.options[AmberFF::Option::ASSIGN_CHARGES] = "true";
	amber.options[AmberFF::Option::ASSIGN_TYPENAMES] = "true";
	amber.options[AmberFF::Option::OVERWRITE_CHARGES] = "true";
	amber.options[AmberFF::Option::OVERWRITE_TYPENAMES] = "true";
	amber.setup(system);

	// calculate the energy
	statusBar()->message("calculating energy...");
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
}

void Mainframe::amberMinimization()
{
	const List<Composite*> selection = control_->getSelection();
	if (selection.size() == 0)
	{
		return;
	}
	// retrieve the system from the selection
	System& system = *RTTI::castTo<System>(*selection.front());

	// set up the AMBER force field
	statusBar()->message("setting up force field...");
	statusBar()->update();
	update();
	AmberFF amber;
  amber.options[AmberFF::Option::ASSIGN_TYPES] = "true";
  amber.options[AmberFF::Option::ASSIGN_CHARGES] = "true";
  amber.options[AmberFF::Option::ASSIGN_TYPENAMES] = "true";
  amber.options[AmberFF::Option::OVERWRITE_CHARGES] = "true";
  amber.options[AmberFF::Option::OVERWRITE_TYPENAMES] = "true";
 	amber.setup(system);

	// calculate the energy
	statusBar()->message("starting minimization...");
	statusBar()->update();
	amber.updateEnergy();

	ConjugateGradientMinimizer minimizer(amber);
	while (!minimizer.minimize(2, true))
	{
    scene_->getCompositeManager()->update(system.getRoot());
		QString message;
		message.sprintf("Iteration %d: energy = %f kJ/mol, RMS gradient = %f kJ/mol A", 
										minimizer.getNumberOfIteration(),
										amber.getEnergy(),
										amber.getRMSGradient());
		statusBar()->message(message);
		statusBar()->update();
		scene_->update();
		update();
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

	QString message;
	message.sprintf("Total AMBER energy: %f kJ/mol.", amber.getEnergy());
	statusBar()->message(message, 5000);
}

#ifdef BALL_NO_INLINE_FUNCTIONS
#	include "mainframe.iC"
#endif
