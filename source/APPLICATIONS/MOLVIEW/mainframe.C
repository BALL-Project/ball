#include "mainframe.h"
#include "DlgAbout.h"
#include "DlgSelector.h"
#include "DlgDisplayProperties.h"
#include <qcolordialog.h>

using namespace std;

Mainframe::Mainframe
  (QWidget *parent__pQWidget,
	 const char *name__pc)
		:
		__mLogStream_(),
		__mScene_(this),
		__mMoleculeObjectProcessor_(),
		__mMoleculeGLObjectCollector_(),
		__mQMenuBar_(this),
		edit__mpQPopupMenu_(0),
		__mpQVBoxLayout_(0),
		__mpQHBoxLayout_(0),
		__mControl_(this),
		__mTimerTextView_(this),
		popup_menus__mList_()
{
	setCaption("MolVIEW");
		
	// ---------------------
	// Logstream setup -----
	// ---------------------

	Log.remove(std::cout);

	// ---------------------
	// Connectiviy ---------
	// ---------------------

	connect(&__mControl_,
					SIGNAL(itemSelected(bool, bool)),
					this,
					SLOT(updateEditMenuFromSelection(bool, bool)));

	connect(&__mControl_,
					SIGNAL(itemCutOrCopied(bool)),
					this,
					SLOT(updateEditMenuFromCutOrCopy(bool)));

	// ---------------------
	// Menu ----------------
	// ---------------------

	// File-Menu -------------------------------------------------------------------

	QPopupMenu *files__pQPopupMenu = new QPopupMenu(this);
	CHECK_PTR(files__pQPopupMenu);
	popup_menus__mList_.push_back(files__pQPopupMenu);

	files__pQPopupMenu->insertItem("&PDB File", this, SLOT(importPDB()), CTRL+Key_P, 1);
	files__pQPopupMenu->insertItem("&HIN File", this, SLOT(importHIN()), CTRL+Key_H, 2);
	files__pQPopupMenu->insertItem("&MOL2 File", 3);
	files__pQPopupMenu->setItemEnabled(3, false);

	QPopupMenu *export__pQPopupMenu = new QPopupMenu(this);
	CHECK_PTR(export__pQPopupMenu);
	popup_menus__mList_.push_back(export__pQPopupMenu);
	
	export__pQPopupMenu->insertItem("&Povray", this, SLOT(exportPovray()), CTRL+Key_P, 4);
	export__pQPopupMenu->setItemEnabled(4, false);

	QPopupMenu *file__pQPopupMenu = new QPopupMenu(this);
	CHECK_PTR(file__pQPopupMenu);
	popup_menus__mList_.push_back(file__pQPopupMenu);

	file__pQPopupMenu->insertItem("&Import File", files__pQPopupMenu, CTRL+Key_I);
	file__pQPopupMenu->insertItem("&Export File", export__pQPopupMenu, CTRL+Key_E);
	file__pQPopupMenu->insertSeparator();
	file__pQPopupMenu->insertItem("E&xit", qApp, SLOT(quit()), CTRL+Key_X);

	// Edit-Menu -------------------------------------------------------------------
	
	QPopupMenu *edit__pQPopupMenu = new QPopupMenu(this);
	CHECK_PTR(edit__pQPopupMenu);
	popup_menus__mList_.push_back(edit__pQPopupMenu);

	edit__mpQPopupMenu_ = edit__pQPopupMenu;

	edit__pQPopupMenu->insertItem("check &Residue", &__mControl_, 
																SLOT(checkResidue()), CTRL+Key_R, MENU__CHECK_RESIDUE);
	edit__pQPopupMenu->insertSeparator();
	edit__pQPopupMenu->insertItem("&cut", &__mControl_, 
																SLOT(cut()), CTRL+Key_C, MENU__CUT);
	edit__pQPopupMenu->insertItem("c&opy", &__mControl_, 
																SLOT(copy()), CTRL+Key_O, MENU__COPY);
	edit__pQPopupMenu->insertItem("&paste", &__mControl_, 
																SLOT(paste()), CTRL+Key_P, MENU__PASTE);
 	edit__pQPopupMenu->insertSeparator();
	edit__pQPopupMenu->insertItem("build &Bonds", &__mControl_, 
																SLOT(buildBonds()), CTRL+Key_B, MENU__BUILD_BONDS);
	edit__pQPopupMenu->insertItem("remove Bo&nds", &__mControl_, 
																SLOT(removeBonds()), CTRL+Key_N, MENU__REMOVE_BONDS);
	edit__pQPopupMenu->insertSeparator();
	edit__pQPopupMenu->insertItem("&select", &__mControl_, 
																SLOT(select()), CTRL+Key_S, MENU__SELECT);
	edit__pQPopupMenu->insertItem("&deselect", &__mControl_, 
																SLOT(deselect()), CTRL+Key_D, MENU__DESELECT);
	edit__pQPopupMenu->insertSeparator();
	edit__pQPopupMenu->insertItem("focus c&amera", &__mControl_, 
																SLOT(centerCamera()), CTRL+Key_A, MENU__CENTER_CAMERA);
	edit__pQPopupMenu->insertSeparator();
	edit__pQPopupMenu->insertItem("cl&ear Clipboard", &__mControl_, 
																SLOT(clearClipboard()), CTRL+Key_E, MENU__CLEAR_CLIPBOARD);

	edit__mpQPopupMenu_->setItemEnabled(MENU__CHECK_RESIDUE, FALSE);
	edit__mpQPopupMenu_->setItemEnabled(MENU__CUT, FALSE);
	edit__mpQPopupMenu_->setItemEnabled(MENU__COPY, FALSE);
	edit__mpQPopupMenu_->setItemEnabled(MENU__PASTE, FALSE);
	edit__mpQPopupMenu_->setItemEnabled(MENU__BUILD_BONDS, FALSE);
	edit__mpQPopupMenu_->setItemEnabled(MENU__REMOVE_BONDS, FALSE);
	edit__mpQPopupMenu_->setItemEnabled(MENU__SELECT, FALSE);
	edit__mpQPopupMenu_->setItemEnabled(MENU__DESELECT, FALSE);
	edit__mpQPopupMenu_->setItemEnabled(MENU__CENTER_CAMERA, FALSE);
	edit__mpQPopupMenu_->setItemEnabled(MENU__CLEAR_CLIPBOARD, FALSE);
	
	// Windows-Menu -------------------------------------------------------------------
	
	QPopupMenu *windows__pQPopupMenu = new QPopupMenu(this);
	CHECK_PTR(windows__pQPopupMenu);
	popup_menus__mList_.push_back(windows__pQPopupMenu);

	windows__pQPopupMenu->insertItem("open D&isplay window", &__mControl_, 
																	 SLOT(openDisplay()), CTRL+Key_I, MENU__OPEN_DISPLAY);
	
	// Help-Menu -------------------------------------------------------------------

	QPopupMenu *help__pQPopupMenu = new QPopupMenu(this);
	CHECK_PTR(help__pQPopupMenu);
	popup_menus__mList_.push_back(help__pQPopupMenu);

	help__pQPopupMenu->insertItem("&About", this, SLOT(about()), CTRL+Key_A);

	// Menu ------------------------------------------------------------------------

	__mQMenuBar_.insertItem("&File", file__pQPopupMenu);
	__mQMenuBar_.insertItem("&Edit", edit__pQPopupMenu);
	__mQMenuBar_.insertItem("&Windows", windows__pQPopupMenu);
	__mQMenuBar_.insertSeparator();
	__mQMenuBar_.insertItem("&Help", help__pQPopupMenu);
	__mQMenuBar_.setSeparator(QMenuBar::InWindowsStyle);
	
	// ---------------------
	// Scene setup ---------
	// ---------------------

	__mScene_.setGLObjectCollector(__mMoleculeGLObjectCollector_);

	// ---------------------
	// Control setup -------
	// ---------------------
	
	__mControl_.addColumn("Name");
	__mControl_.addColumn("Type");
	__mControl_.addColumn("Display");
	__mControl_.setColumnWidth(0, 100);
	__mControl_.setColumnWidth(1, 100);

	__mControl_.setScene(__mScene_);
	__mControl_.setMoleculeObjectProcessor(__mMoleculeObjectProcessor_);

	// ---------------------
	// TextView setup ------
	// ---------------------
	
	QFont f("Courier", 12, QFont::Bold, false, QFont::ISO_8859_1);
	__mTimerTextView_.setFont(f);
	__mTimerTextView_.setReadOnly(TRUE);
	__mTimerTextView_.setAutoUpdate(TRUE);

	Log.info() << "Welcome to MolVIEW." << endl;

	// ---------------------
	// Layout --------------
	// ---------------------

	__mpQVBoxLayout_ = new QVBoxLayout(this);
	CHECK_PTR(__mpQVBoxLayout_);

	__mpQHBoxLayout_ = new QHBoxLayout();
	CHECK_PTR(__mpQHBoxLayout_);
	
	// horizontal Layout
	// first column: tree control
	// second column: Scene
 	__mpQHBoxLayout_->addWidget(&__mControl_, 1); 
  __mpQHBoxLayout_->addWidget(&__mScene_, 3);
	
	// vertical Layout (2 rows)
	// contains in the first row a horizontal layout with two columns
	__mpQVBoxLayout_->setMenuBar(&__mQMenuBar_);
	__mpQVBoxLayout_->addLayout(__mpQHBoxLayout_, 4);
	__mpQVBoxLayout_->addWidget(&__mTimerTextView_ ,1);
}

Mainframe::~Mainframe
  ()
{
	List<QPopupMenu *>::Iterator iterator__List;

	for (iterator__List = popup_menus__mList_.begin();
			 iterator__List != popup_menus__mList_.end();
			 ++iterator__List)
	{
		delete *iterator__List;
	}

	popup_menus__mList_.clear();
}

void Mainframe::importPDB
  ()
{
	QStringList __QStringList;
	__QStringList = "PDB files (*.pdb)";
	__QStringList += "PDB files (*.brk)";
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
			Log.info() << "> opening PDB-File." << endl;

			PDBFile __PDBFile(filename__QString.ascii());

			System __System;
			__PDBFile >> __System;
			/*
			if (!__PDBFile.isGood())
			{
				QMessageBox::about(this, "PDB-File Error", "Error reading PDB-File!\n\n");

				return;
			}
			*/

			Log.info() << "> generating bonds." << endl;
      __System.apply(*(__mMoleculeObjectProcessor_.fragmentdb.buildBonds));

			// construct a name (the filename without the dir path)
			filename__QString.remove(0, __QFileDialog.dirPath().length() + 1);

			if (filename__QString.find('.') != -1)
			{
				filename__QString = filename__QString.left(filename__QString.find('.'));
			}

			__mControl_.addComposite(&__System, &filename__QString);
		}
	}
}

void Mainframe::importHIN
  ()
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

			Log.info() << "> opening HIN-File." << endl;

			HINFile __HINFile(filename__QString.ascii());

			System __System;
			__HINFile >> __System;

			/*
			if (!__HINFile.isGood())
			{
				QMessageBox::about(this, "HIN-File Error", "Error reading HIN-File!\n\n");

				return;
			}
			*/

			Log.info() << "> generating bonds." << endl;
      __System.apply(*(__mMoleculeObjectProcessor_.fragmentdb.buildBonds));

			// construct a name (the filename without the dir path)
			filename__QString.remove(0, __QFileDialog.dirPath().length() + 1);

			if (filename__QString.find('.') != -1)
			{
				filename__QString = filename__QString.left(filename__QString.find('.'));
			}
			
			__mControl_.addComposite(&__System, &filename__QString);
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
	about_box.show();
}

void Mainframe::updateEditMenuFromSelection(bool selected__bool, bool residue__bool)
{
	if (residue__bool == true)
	{
		edit__mpQPopupMenu_->setItemEnabled(MENU__CHECK_RESIDUE, TRUE);
	}
	else
	{
		edit__mpQPopupMenu_->setItemEnabled(MENU__CHECK_RESIDUE, FALSE);
	}

	if (selected__bool == true)
	{
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


#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include "mainframe.iC"
#		endif



