#include "mainframe.h"
#include "DlgAbout.h"
#include "DlgSelector.h"
#include "DlgDisplayProperties.h"
#include <qcolordialog.h>
#include <qfiledialog.h>
#include <qlabel.h>
#include <qfile.h>

using namespace std;

Mainframe::Mainframe
  (QWidget* parent, const char* name)
	:	scene_(0),
		object_processor_(),
		GL_object_collector_(),
		menubar_(0),
		edit_menu_(0),
		hor_splitter_(0),
		vert_splitter_(0),
		control_(0),
		logview_(0),
		statusbar_(0),
		vboxlayout_(0),
		display_properties_dialog_(this),
		preferences_dialog_(this),

		popup_menus_(),
		preferences_(".molview.ini"),
		fragment_db_(),
		selection_(),
		copy_list_(),
		cut_list_(),
		server_icon_(0),
		tool_box_(0)
{
	// ---------------------
	// Logstream setup -----
	// ---------------------

	Log.remove(std::cout);

	// ---------------------
	// create widgets ------
	// ---------------------
	setCaption("MolVIEW");
	menubar_ = new QMenuBar(this);
	CHECK_PTR(menubar_);

	vert_splitter_ = new QSplitter(this);
	CHECK_PTR(vert_splitter_);
	vert_splitter_->setOrientation(Vertical);

	hor_splitter_ = new QSplitter(vert_splitter_);
	CHECK_PTR(hor_splitter_);

	control_ = new Control(hor_splitter_);
	CHECK_PTR(control_);

	scene_ = new Scene(hor_splitter_);
	CHECK_PTR(scene_);

	logview_ = new LogView(vert_splitter_);
	CHECK_PTR(logview_);

	statusbar_ = new QStatusBar(this);
	CHECK_PTR(statusbar_);
	QLabel* message_label = new QLabel(tr("Ready."), statusbar_);
	statusbar_->addWidget(message_label, 20);


	server_icon_ = new QLabel(statusbar_);
	statusbar_->addWidget(server_icon_, 1, TRUE );

	static const char * mini_ray_xpm[] = 
	{
		"16 14 4 1",
		" 	c None",
		".	c black",
		"X	c yellow",
		"o	c gray50",
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

	QPixmap icon(mini_ray_xpm);

	server_icon_->setPixmap(icon);
	server_icon_->hide();
 
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

	build_menu->setItemEnabled(MENU__BUILD_CHECK_RESIDUE, FALSE);
	build_menu->setItemEnabled(MENU__BUILD_BUILD_BONDS, FALSE);
	build_menu->insertItem("Check St&ructure", this, 
												 SLOT(checkResidue()), CTRL+Key_R, MENU__BUILD_CHECK_RESIDUE);
	build_menu->insertItem("&Build Bonds", this, 
												 SLOT(buildBonds()), CTRL+Key_B, MENU__BUILD_BUILD_BONDS);
	build_menu->insertItem("Add &Hydrogens", this, 
												 SLOT(addHydrogens()), CTRL+Key_H, MENU__BUILD_ADD_HYDROGENS);
	

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

	menubar_->insertItem("&File", file_menu);
	menubar_->insertItem("&Edit", edit_menu_);
	menubar_->insertItem("&Build", build_menu);
	menubar_->insertItem("&Display", display_menu);
	menubar_->insertSeparator();
	menubar_->insertItem("&Help", help_menu);
	menubar_->setSeparator(QMenuBar::InWindowsStyle);
	

	// ---------------------
	// Layout --------------
	// ---------------------

	vboxlayout_ = new QVBoxLayout(this);
	CHECK_PTR(vboxlayout_);

  vboxlayout_->setMenuBar(menubar_);
	vboxlayout_->addWidget(vert_splitter_);
	vboxlayout_->addWidget(statusbar_);

	// ---------------------
	// Connectivity --------
	// ---------------------

	connect(display_menu,
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
					SIGNAL(apply()),
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
			statusbar_->message("reading PDB-File...");

			PDBFile pdb_file(filename.ascii());

			System system;
			pdb_file >> system;
			pdb_file.close();

			Log.info() << "> read " << system.countAtoms() << " atoms from PDB file \"" 
								 << filename.ascii() << "\"" << endl;

			statusbar_->message("normalizing names...");
	    system.apply(*fragment_db_.normalizeNames);
			Log.info() << "> normalized names" << endl;

			statusbar_->message("generating missing bonds...");
	    system.apply(*fragment_db_.buildBonds);
			Log.info() << "> generated missing bonds" << endl;
  
			// construct a name (the filename without the dir path)
			filename.remove(0, file_dialog.dirPath().length() + 1);

			if (filename.find('.') != -1)
			{
				filename = filename.left(filename.find('.'));
			}

			addComposite(&system, &filename);
			statusbar_->clear();
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

			statusbar_->message("reading HIN file...");

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
			
			statusbar_->clear();
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
	statusbar_->message("MolVIEW V 0.9 alpha");
}

void Mainframe::checkMenuEntries()
{
	// extract the selection of the tree
	const List<Composite*>& selection = control_->getSelection();
	bool selected = (selection.size() > 0);

	// enable for multiple selection
	menubar_->setItemEnabled(MENU__EDIT_SELECT, selected);
	menubar_->setItemEnabled(MENU__EDIT_DESELECT, selected);
	
	// these menu points for single items only
	menubar_->setItemEnabled(MENU__DISPLAY_CENTER_CAMERA, selected && (selection.size() == 1));

	// set the checkboxes for the non-modal dialogs
	menubar_->setItemChecked(MENU__DISPLAY_OPEN_DISPLAY_PROPERTIES_DIALOG, 
													 display_properties_dialog_.isVisible());
	menubar_->setItemChecked(MENU__DISPLAY_OPEN_PREFERENCES_DIALOG, 
													 preferences_dialog_.isVisible());

	// check for paste-slot: enable only if copy_list_ not empty
	menubar_
		->setItemEnabled(MENU__EDIT_PASTE, 
										 (bool)(copy_list_.size() > 0) 
										 || (bool)(cut_list_.size() > 0));

	// check for clearClipboard-slot: enable only if copy_list_ not empty
	menubar_
		->setItemEnabled(MENU__EDIT_CLEAR_CLIPBOARD, 
										 (bool)(copy_list_.size() > 0) 
										 || (bool)(cut_list_.size() > 0));

	// check for erase-slot: enable erase-slot only if all selected composites
	// are systems
	if (selection.size() > 0)
	{
		bool all_systems = true;

		List<Composite*>::ConstIterator list_it = selection.begin();	
		for (; list_it != selection.end(); ++list_it)
		{
			if (!RTTI::isKindOf<System>(**list_it))
			{
				all_systems = false;
			}
		}

		menubar_->setItemEnabled(MENU__EDIT_CUT, all_systems);
		menubar_->setItemEnabled(MENU__EDIT_COPY, all_systems);
		menubar_->setItemEnabled(MENU__EDIT_DELETE, all_systems);
	}
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
	statusbar_->message(message);

	// clear copy list
	copy_list_.clear();

	// delete old composites
	if (cut_list_.size() > 0)
	{
		List<Composite*>::ConstIterator list_it = cut_list_.begin();	
		for (; list_it != cut_list_.end(); ++list_it)
		{
			delete *list_it;
		}

		cut_list_.clear();
	}

	// remove all system composites from the tree and from the scene
	// but do not delete them from memory
	List<Composite*>::ConstIterator list_it = selection.begin();	
	for (; list_it != selection.end(); ++list_it)
	{
		// insert deep clone of the composite into the cut list
		cut_list_.push_back((Composite*)(*list_it)->create());

		removeComposite(*list_it, false);
	}

	scene_->update();

	control_->invalidateSelection();

	statusbar_->clear();
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
	statusbar_->message(message);

	// delete old cutted composites
	if (cut_list_.size() > 0)
	{
		List<Composite*>::ConstIterator list_it = cut_list_.begin();	
		for (; list_it != cut_list_.end(); ++list_it)
		{
			delete *list_it;
		}

		cut_list_.clear();
	}

	// copy the selected composites into the copy_list_
	copy_list_ = const_cast<List<Composite*>&>(selection);

	statusbar_->clear();
}

void Mainframe::paste()
{
	if (copy_list_.size() == 0
			&& cut_list_.size() == 0)
	{
		return;
	}

	QString message;
	message.sprintf("pasting %d systems...", 
													copy_list_.size());
	statusbar_->message(message);

	// copying composites
	List<Composite*>::ConstIterator list_it = copy_list_.begin();	
	for (; list_it != copy_list_.end(); ++list_it)
	{
		addComposite(*list_it, 0);
	}

	// adding cutted composites
	List<Composite*>::ConstIterator cut_list_it = cut_list_.begin();	
	for (; cut_list_it != cut_list_.end(); ++cut_list_it)
	{
		addComposite(*cut_list_it, 0);
	}

	control_->invalidateSelection();

	statusbar_->clear();
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
	statusbar_->message(message);

	// erase all system composites
	List<Composite*>::ConstIterator list_it = selection.begin();	
	for (; list_it != selection.end(); ++list_it)
	{
		removeComposite(*list_it);
	}

	control_->invalidateSelection();

	scene_->update();
	statusbar_->clear();
}

void Mainframe::clearClipboard()
{
	statusbar_->message("clearing clipboard...");

	copy_list_.clear();
	cut_list_.clear();

	statusbar_->clear();
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
	statusbar_->message(message);

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
	statusbar_->clear();
}

void Mainframe::deselect()
{
	const List<Composite*>& selection = control_->getSelection();
	if (selection.size() == 0)
	{
		return;
	}

	QString message;
	message.sprintf("selecting %d objects...", 
													selection.size());
	statusbar_->message(message);

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
	statusbar_->clear();
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
	statusbar_->message(message);

	ResidueChecker res_check(fragment_db_);
	bool okay = true;
	List<Composite*>::ConstIterator list_it = selection.begin();	
	for (; list_it != selection.end(); ++list_it)
	{	
		Residue* residue = dynamic_cast<Residue*>(*list_it);
		if (residue != 0)
		{
			residue->apply(res_check);
			okay = okay && res_check.getStatus();	
		} else {
			Composite::SubcompositeIterator it = (*list_it)->beginSubcomposite();
			for (; +it; ++it)
			{
				residue = dynamic_cast<Residue*>(&*it);
				if (residue != 0)
				{
					residue->apply(res_check);
					okay = okay && res_check.getStatus();	
				}	
			}
		}
	}

	if (okay)
	{
		Log.info() << "ResidueChecker: no errors found." << endl;
		statusbar_->message("no errors.");
	} else {
		statusbar_->message("errors found!");
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

	statusbar_->message("building bonds...");
	List<Composite*>::ConstIterator list_it = selection.begin();	
	for (; list_it != selection.end(); ++list_it)
	{	
		Fragment* frag = dynamic_cast<Fragment*>(*list_it);
		if (frag != 0)
		{
			frag->apply(*fragment_db_.buildBonds);
			number_of_bonds += fragment_db_.buildBonds->getNumberOfBondsBuilt();
      object_processor_.applyOn(*frag);
      scene_->getCompositeManager()->update(frag->getRoot());
 		} else {
			System* system = dynamic_cast<System*>(*list_it);
			if (system != 0)
			{
				system->apply(*fragment_db_.buildBonds);
				number_of_bonds += fragment_db_.buildBonds->getNumberOfBondsBuilt();
				object_processor_.applyOn(*system);
				scene_->getCompositeManager()->update(system->getRoot());
			} else {
				Molecule* molecule = dynamic_cast<Molecule*>(*list_it);
				if (molecule != 0)
				{
					molecule->apply(*fragment_db_.buildBonds);
					number_of_bonds += fragment_db_.buildBonds->getNumberOfBondsBuilt();
					object_processor_.applyOn(*molecule);
					scene_->getCompositeManager()->update(molecule->getRoot());
				} else {
					Chain* chain = dynamic_cast<Chain*>(*list_it);
					if (chain != 0)
					{
						chain->apply(*fragment_db_.buildBonds);
						number_of_bonds += fragment_db_.buildBonds->getNumberOfBondsBuilt();
						object_processor_.applyOn(*chain);
						scene_->getCompositeManager()->update(chain->getRoot());
					} else {
						SecondaryStructure* sec_struc = dynamic_cast<SecondaryStructure*>(*list_it);
						if (sec_struc != 0)
						{
							sec_struc->apply(*fragment_db_.buildBonds);
							number_of_bonds += fragment_db_.buildBonds->getNumberOfBondsBuilt();
							object_processor_.applyOn(*sec_struc);
							scene_->getCompositeManager()->update(sec_struc->getRoot());
						} else {
							Composite::SubcompositeIterator it = (*list_it)->beginSubcomposite();
							for (; +it; ++it)
							{
								frag = dynamic_cast<Fragment*>(&*it);
								if (frag != 0)
								{
									frag->apply(*fragment_db_.buildBonds);
									number_of_bonds += fragment_db_.buildBonds->getNumberOfBondsBuilt();
									object_processor_.applyOn(*frag);
									scene_->getCompositeManager()->update(frag->getRoot());
								}	
							}
						}
					}
				}
			}
		}
	}

	
	scene_->update();
	statusbar_->clear();
	QString message;
	message.sprintf("Added %d bonds.", number_of_bonds);
	statusbar_->message(message, 2000);
}

void Mainframe::addHydrogens()
{
	const list<Composite*>& selection = control_->getSelection();
	if (selection.size() == 0)
	{
		return;
	}

	Size number_of_hydrogens = 0;

	statusbar_->message("adding hydrogens...");
	List<Composite*>::ConstIterator list_it = selection.begin();	
	for (; list_it != selection.end(); ++list_it)
	{	
		Residue* residue = dynamic_cast<Residue*>(*list_it);
		if (residue != 0)
		{
			residue->apply(*fragment_db_.addHydrogens);
			number_of_hydrogens += fragment_db_.addHydrogens->getNumberOfInsertedH();
			residue->apply(*fragment_db_.buildBonds);
			object_processor_.applyOn(*residue);
			scene_->getCompositeManager()->update(residue->getRoot());
		} else {
			Composite::SubcompositeIterator it = (*list_it)->beginSubcomposite();
			for (; +it; ++it)
			{
				residue = dynamic_cast<Residue*>(&*it);
				if (residue != 0)
				{
					residue->apply(*fragment_db_.addHydrogens);
					number_of_hydrogens += fragment_db_.addHydrogens->getNumberOfInsertedH();
					residue->apply(*fragment_db_.buildBonds);
					object_processor_.applyOn(*residue);
					scene_->getCompositeManager()->update(residue->getRoot());
				}	
			}
		}
	}

	Log.info() << "added " << number_of_hydrogens << " hydrogen atoms." << endl;
	statusbar_->clear();
	QString message;
	message.sprintf("Added %d hydrogen atoms.", number_of_hydrogens);
	statusbar_->message(message, 2000);

	// update the marked representations
	scene_->update();
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
	statusbar_->message(message);

	List<Composite*>::ConstIterator list_it = selection.begin();	
	for (; list_it != selection.end(); ++list_it)
	{
		object_processor_.applyOn(**list_it);
		scene_->getCompositeManager()->update((**list_it).getRoot());
	}
	scene_->update();
	statusbar_->clear();
}

void Mainframe::applyPreferencesDialog()
{
}

void Mainframe::openDisplayPropertiesDialog()
{
	display_properties_dialog_.show();
	display_properties_dialog_.raise();
	startServer();
}

void Mainframe::openPreferencesDialog()
{
	preferences_dialog_.show();
	preferences_dialog_.raise();
	stopServer();
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
	String value_string;
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
	statusbar_->message("creating graphical representation...");

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
  statusbar_->message("> generating tree representation... ");

  // generate ListViewItem
  control_->addComposite(new_composite, name);	
}

void Mainframe::removeComposite(Composite* composite, bool delete_composite)
{
	// remove the tree from the control
	control_->removeComposite(composite);

	// removes the composite from the scene
	scene_->getCompositeManager()->remove(*composite);

	if (delete_composite)
	{
		delete composite;
	}
}

bool Mainframe::onNotify(Server& server)
{
	addComposite((Composite *)server.getReceivedComposite(), 0);

	return true;
}

void Mainframe::toggleServer()
{
	if (server_icon_->isVisible())
	{
		stopServer();
	} else {
		startServer();
	}
}
void Mainframe::startServer()
{
	server_icon_->show();
	statusbar_->update();
}

void Mainframe::stopServer()
{
	server_icon_->hide();
	statusbar_->update();
}

#ifdef BALL_NO_INLINE_FUNCTIONS
#	include "mainframe.iC"
#endif
