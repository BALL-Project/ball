#include "mainframe.h"
#include "DlgAbout.h"
#include "DlgSelector.h"
#include "DlgDisplayProperties.h"
#include <qcolordialog.h>
#include <qfiledialog.h>
#include <qfile.h>

using namespace std;

Mainframe::Mainframe
  (QWidget *parent__pQWidget, const char *name__pc)
		:
		scene_(0),
		__mMoleculeObjectProcessor_(),
		__mMoleculeGLObjectCollector_(),
		menubar_(0),
		edit__mpQPopupMenu_(0),
		hor_splitter_(0),
		vert_splitter_(0),
		control_(0),
		logview_(0),
		statusbar_(0),
		vboxlayout_(0),
		popup_menus__mList_(),
		preferences_(".molview.ini")
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
	control_->resize
		((int)(0.15 * (float)width()),
		 (int)(0.8 * (float)height()));
	CHECK_PTR(control_);

	scene_ = new Scene(hor_splitter_);
	CHECK_PTR(scene_);

	logview_ = new LogView(vert_splitter_);
	CHECK_PTR(logview_);

	statusbar_ = new QStatusBar(this);
	CHECK_PTR(statusbar_);

	// ---------------------
	// Scene setup ---------
	// ---------------------

	scene_->setGLObjectCollector(__mMoleculeGLObjectCollector_);

	// ---------------------
	// Control setup -------
	// ---------------------
	
	control_->addColumn("Name");
	control_->addColumn("Type");
	control_->addColumn("Display");
	control_->setColumnWidth(0, 80);
	control_->setColumnWidth(1, 60);

	control_->setScene(*scene_);
	control_->setMoleculeObjectProcessor(__mMoleculeObjectProcessor_);

	// ---------------------
	// TextView setup ------
	// ---------------------
	
	QFont f("Courier", 12, QFont::DemiBold, false);
	logview_->setFont(f);
	logview_->setReadOnly(TRUE);
	logview_->setAutoUpdate(TRUE);

	Log.info() << "Welcome to MolVIEW." << endl;


	// ---------------------
	// Menu ----------------
	// ---------------------

	// File-Menu -------------------------------------------------------------------

	QPopupMenu *files__pQPopupMenu = new QPopupMenu(this);
	CHECK_PTR(files__pQPopupMenu);
	popup_menus__mList_.push_back(files__pQPopupMenu);

	QPopupMenu *file__pQPopupMenu = new QPopupMenu(this);
	CHECK_PTR(file__pQPopupMenu);
	popup_menus__mList_.push_back(file__pQPopupMenu);

	QPopupMenu *export__pQPopupMenu = new QPopupMenu(this);
	CHECK_PTR(export__pQPopupMenu);
	popup_menus__mList_.push_back(export__pQPopupMenu);
	
	export__pQPopupMenu->insertItem("&Povray", this, SLOT(exportPovray()), CTRL+Key_P, 4);
	export__pQPopupMenu->setItemEnabled(4, false);

	file__pQPopupMenu->insertItem("&Import File", files__pQPopupMenu, CTRL+Key_I);
	file__pQPopupMenu->insertItem("&Export File", export__pQPopupMenu, CTRL+Key_E);
	file__pQPopupMenu->insertSeparator();
	file__pQPopupMenu->insertItem("E&xit", qApp, SLOT(quit()), CTRL+Key_X);

	files__pQPopupMenu->insertItem("&PDB File", this, SLOT(importPDB()), CTRL+Key_P, 1);
	files__pQPopupMenu->insertItem("&HIN File", this, SLOT(importHIN()), CTRL+Key_H, 2);
	files__pQPopupMenu->insertItem("&MOL2 File", 3);
	files__pQPopupMenu->setItemEnabled(3, false);


	// Edit-Menu -------------------------------------------------------------------
	
	QPopupMenu *edit__pQPopupMenu = new QPopupMenu(this);
	CHECK_PTR(edit__pQPopupMenu);
	popup_menus__mList_.push_back(edit__pQPopupMenu);

	edit__mpQPopupMenu_ = edit__pQPopupMenu;

	edit__pQPopupMenu->insertItem("&cut", control_, 
																SLOT(cut()), CTRL+Key_C, MENU__CUT);
	edit__pQPopupMenu->insertItem("c&opy", control_, 
																SLOT(copy()), CTRL+Key_O, MENU__COPY);
	edit__pQPopupMenu->insertItem("&paste", control_, 
																SLOT(paste()), CTRL+Key_P, MENU__PASTE);
	edit__pQPopupMenu->insertSeparator();
	edit__pQPopupMenu->insertItem("&select", control_, 
																SLOT(select()), CTRL+Key_S, MENU__SELECT);
	edit__pQPopupMenu->insertItem("&deselect", control_, 
																SLOT(deselect()), CTRL+Key_D, MENU__DESELECT);
	edit__pQPopupMenu->insertSeparator();
	edit__pQPopupMenu->insertItem("focus c&amera", control_, 
																SLOT(centerCamera()), CTRL+Key_A, MENU__CENTER_CAMERA);
	edit__pQPopupMenu->insertSeparator();
	edit__pQPopupMenu->insertItem("cl&ear Clipboard", control_, 
																SLOT(clearClipboard()), CTRL+Key_E, MENU__CLEAR_CLIPBOARD);

	edit__mpQPopupMenu_->setItemEnabled(MENU__CUT, FALSE);
	edit__mpQPopupMenu_->setItemEnabled(MENU__COPY, FALSE);
	edit__mpQPopupMenu_->setItemEnabled(MENU__PASTE, FALSE);
	edit__mpQPopupMenu_->setItemEnabled(MENU__SELECT, FALSE);
	edit__mpQPopupMenu_->setItemEnabled(MENU__DESELECT, FALSE);
	edit__mpQPopupMenu_->setItemEnabled(MENU__CENTER_CAMERA, FALSE);
	edit__mpQPopupMenu_->setItemEnabled(MENU__CLEAR_CLIPBOARD, FALSE);

	QPopupMenu* build_menu = new QPopupMenu(this);
	CHECK_PTR(build_menu);
	popup_menus__mList_.push_back(build_menu);

	build_menu->setItemEnabled(MENU__CHECK_RESIDUE, FALSE);
	build_menu->setItemEnabled(MENU__BUILD_BONDS, FALSE);
	build_menu->setItemEnabled(MENU__REMOVE_BONDS, FALSE);
	build_menu->insertItem("check st&ructure", control_, 
												 SLOT(checkResidue()), CTRL+Key_R, MENU__CHECK_RESIDUE);
	build_menu->insertItem("&build bonds", control_, 
												 SLOT(buildBonds()), CTRL+Key_B, MENU__BUILD_BONDS);
	build_menu->insertItem("remove bo&nds", control_, 
												 SLOT(removeBonds()), CTRL+Key_N, MENU__REMOVE_BONDS);
	build_menu->insertItem("add &hydrogens", control_, 
												 SLOT(removeBonds()), CTRL+Key_H, MENU__REMOVE_BONDS);
	


	// Windows-Menu -------------------------------------------------------------------
	
	QPopupMenu *windows__pQPopupMenu = new QPopupMenu(this);
	CHECK_PTR(windows__pQPopupMenu);
	popup_menus__mList_.push_back(windows__pQPopupMenu);

	windows__pQPopupMenu->insertItem("d&isplay properties", control_, 
																	 SLOT(openDisplay()), CTRL+Key_I, MENU__OPEN_DISPLAY);


	// Help-Menu -------------------------------------------------------------------

	QPopupMenu *help__pQPopupMenu = new QPopupMenu(this);
	CHECK_PTR(help__pQPopupMenu);
	popup_menus__mList_.push_back(help__pQPopupMenu);
	
	help__pQPopupMenu->insertItem("&About", this, SLOT(about()), CTRL+Key_A);

	// Menu ------------------------------------------------------------------------

	menubar_->insertItem("&File", file__pQPopupMenu);
	menubar_->insertItem("&Edit", edit__pQPopupMenu);
	menubar_->insertItem("&Build", build_menu);
	menubar_->insertItem("&Windows", windows__pQPopupMenu);
	menubar_->insertSeparator();
	menubar_->insertItem("&Help", help__pQPopupMenu);
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

	connect(control_,
					SIGNAL(itemSelected(bool)),
					this,
					SLOT(updateEditMenuFromSelection(bool)));

	connect(control_,
					SIGNAL(itemCutOrCopied(bool)),
					this,
					SLOT(updateEditMenuFromCutOrCopy(bool)));

	// ---------------------
	// read preferences ----
	// ---------------------
	preferences_.setFilename(".molview.ini");
	preferences_.read();
	getPreferences(preferences_);	
}

Mainframe::~Mainframe()
{
	//
	// extract preferences 
	// from the current settings
	//
	setPreferences(preferences_);

	//
	// write the preferences
	//
	preferences_.write();

	//
	// clean up
	//
	List<QPopupMenu *>::Iterator iterator__List;

	for (iterator__List = popup_menus__mList_.begin();
			 iterator__List != popup_menus__mList_.end();
			 ++iterator__List)
	{
		delete *iterator__List;
	}

	popup_menus__mList_.clear();
}

void Mainframe::importPDB()
{
	QStringList __QStringList;
	__QStringList = "PDB files (*.pdb)";
	__QStringList += "PDB files (*.brk)";
	__QStringList += "PDB files (*.ent)";
	__QStringList += "All files (*.*)";

	QFileDialog __QFileDialog(this, 0, TRUE);
	__QFileDialog.setFilters(__QStringList);
	__QFileDialog.setCaption("Import PDB file");

	if (__QFileDialog.exec())
	{
		QString filename__QString = __QFileDialog.selectedFile();

		if (!filename__QString.isNull())
		{
			// test, if file is existent
			// if the file does not exist => exit from function
			if (!QFile::exists(filename__QString))
			{
				return;
			}

			// PDB file laden
			statusbar_->message("reading PDB-File...");

			PDBFile __PDBFile(filename__QString.ascii());

			System __System;
			__PDBFile >> __System;
			__PDBFile.close();

			Log.info() << "> read " << __System.countAtoms() << " atoms from PDB file \"" 
								 << filename__QString.ascii() << "\"" << endl;

			statusbar_->message("normalizing names...");
	    __System.apply(*(__mMoleculeObjectProcessor_.fragmentdb.normalizeNames));
			Log.info() << "> normalized names" << endl;

			statusbar_->message("generating missing bonds...");
	    __System.apply(*(__mMoleculeObjectProcessor_.fragmentdb.buildBonds));
			Log.info() << "> generated missing bonds" << endl;
  
			// construct a name (the filename without the dir path)
			filename__QString.remove(0, __QFileDialog.dirPath().length() + 1);

			if (filename__QString.find('.') != -1)
			{
				filename__QString = filename__QString.left(filename__QString.find('.'));
			}

			control_->addComposite(&__System, &filename__QString);
			statusbar_->clear();
		}
	}
}

void Mainframe::importHIN()
{
	QStringList __QStringList;
	__QStringList = "HIN files (*.hin)";
	__QStringList += "All files (*.*)";

	QFileDialog __QFileDialog(this, 0, TRUE);
	__QFileDialog.setFilters(__QStringList);
	__QFileDialog.setCaption("Import HIN file");

	if (__QFileDialog.exec())
	{
		QString filename__QString = __QFileDialog.selectedFile();

		if (!filename__QString.isNull())
		{
			// test, if file is existent
			// if the file does not exist => exit from function
			if (!QFile::exists(filename__QString))
			{
				return;
			}

			statusbar_->message("reading HIN file...");

			HINFile __HINFile(filename__QString.ascii());

			System __System;
			__HINFile >> __System;
			__HINFile.close();

			// construct a name (the filename without the dir path)
			filename__QString.remove(0, __QFileDialog.dirPath().length() + 1);

			if (filename__QString.find('.') != -1)
			{
				filename__QString = filename__QString.left(filename__QString.find('.'));
			}

			Log.info() << "> read " << __System.countAtoms() << " atoms from HIN-File \"" << filename__QString.ascii() << "\"" << endl;

			control_->addComposite(&__System, &filename__QString);
			
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

void Mainframe::updateEditMenuFromSelection(bool selected__bool)
{
	if (selected__bool == true)
	{
		edit__mpQPopupMenu_->setItemEnabled(MENU__CHECK_RESIDUE, TRUE);
		edit__mpQPopupMenu_->setItemEnabled(MENU__CUT, TRUE);
		edit__mpQPopupMenu_->setItemEnabled(MENU__COPY, TRUE);
		edit__mpQPopupMenu_->setItemEnabled(MENU__BUILD_BONDS, TRUE);
		edit__mpQPopupMenu_->setItemEnabled(MENU__REMOVE_BONDS, TRUE);
		edit__mpQPopupMenu_->setItemEnabled(MENU__SELECT, TRUE);
		edit__mpQPopupMenu_->setItemEnabled(MENU__DESELECT, TRUE);
		edit__mpQPopupMenu_->setItemEnabled(MENU__CENTER_CAMERA, TRUE);
	}
	else
	{
		edit__mpQPopupMenu_->setItemEnabled(MENU__CHECK_RESIDUE, FALSE);
		edit__mpQPopupMenu_->setItemEnabled(MENU__CUT, FALSE);
		edit__mpQPopupMenu_->setItemEnabled(MENU__COPY, FALSE);
		edit__mpQPopupMenu_->setItemEnabled(MENU__BUILD_BONDS, FALSE);
		edit__mpQPopupMenu_->setItemEnabled(MENU__REMOVE_BONDS, FALSE);
		edit__mpQPopupMenu_->setItemEnabled(MENU__SELECT, FALSE);
		edit__mpQPopupMenu_->setItemEnabled(MENU__DESELECT, FALSE);
		edit__mpQPopupMenu_->setItemEnabled(MENU__CENTER_CAMERA, FALSE);
	}
}

void Mainframe::updateEditMenuFromCutOrCopy(bool copied__bool)
{
	if (copied__bool == true)
	{
		edit__mpQPopupMenu_->setItemEnabled(MENU__PASTE, TRUE);
		edit__mpQPopupMenu_->setItemEnabled(MENU__CLEAR_CLIPBOARD, TRUE);
	}
	else
	{
		edit__mpQPopupMenu_->setItemEnabled(MENU__PASTE, FALSE);
		edit__mpQPopupMenu_->setItemEnabled(MENU__CLEAR_CLIPBOARD, FALSE);
	}
}

void Mainframe::resizeEvent
  (QResizeEvent * /*__pQResizeEvent */)
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
		("WINDOWS", "Main::y", String(x()));
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

#ifdef BALL_NO_INLINE_FUNCTIONS
#	include "mainframe.iC"
#endif



