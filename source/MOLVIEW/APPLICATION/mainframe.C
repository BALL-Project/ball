#include <BALL/MOLVIEW/APPLICATION/mainframe.h>

Mainframe::Mainframe
  (QWidget *parent__pQWidget,
	 const char *name__pc)
		:
		__mLogStream_(),
		__mScene_(this),
		__mMoleculeObjectProcessor_(),
		__mMoleculeGLObjectCollector_(),
		__mQMenuBar_(this),
		__mpQVBoxLayout_(0),
		__mpQHBoxLayout_(0),
		__mControl_(this),
		__mpQTextView_(0),
		popup_menus__mList_()
{
	setCaption("MolVIEW");
		
	// ---------------------
	// Logstream setup -----
	// ---------------------

	__mLogStream_.insert(cout);
	
	// ---------------------
	// Menu ----------------
	// ---------------------

	QPopupMenu *files__pQPopupMenu = new QPopupMenu(this);
	CHECK_PTR(files__pQPopupMenu);
	popup_menus__mList_.push_back(files__pQPopupMenu);

	files__pQPopupMenu->insertItem("&PDB File", this, SLOT(importPDB()), CTRL+Key_P);
	files__pQPopupMenu->insertItem("&HIN File", this, SLOT(importHIN()), CTRL+Key_H);

	QPopupMenu *file__pQPopupMenu = new QPopupMenu(this);
	CHECK_PTR(file__pQPopupMenu);
	popup_menus__mList_.push_back(file__pQPopupMenu);

	file__pQPopupMenu->insertItem("&Import File", files__pQPopupMenu, CTRL+Key_I);
	file__pQPopupMenu->insertSeparator();
	file__pQPopupMenu->insertItem("E&xit", qApp, SLOT(quit()), CTRL+Key_X);

	QPopupMenu *help__pQPopupMenu = new QPopupMenu(this);
	CHECK_PTR(help__pQPopupMenu);
	popup_menus__mList_.push_back(help__pQPopupMenu);

	help__pQPopupMenu->insertItem("&About", this, SLOT(about()), CTRL+Key_A);

	__mQMenuBar_.insertItem("&File", file__pQPopupMenu);
	//	__mQMenuBar_.insertItem("&Edit", edit__pQPopupMenu);
	//	__mQMenuBar_.insertItem("&Options", options__pQPopupMenu);
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

	__mpQTextView_ = new QTextView(this);
	CHECK_PTR(__mpQTextView_);

	QString __QString("Welcome to MolVIEW Server.");
	__mpQTextView_->setText(__QString);
	
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
	__mpQVBoxLayout_->addWidget(__mpQTextView_ ,1);
}

Mainframe::~Mainframe
  (void)
{
	/*
	if (__mpQVBoxLayout_ != 0)
	{
		delete __mpQVBoxLayout_;
		__mpQVBoxLayout_ = 0;
	}

	if (__mpQHBoxLayout_ != 0)
	{
		delete __mpQHBoxLayout_;
		__mpQHBoxLayout_ = 0;
	}

	if (__mpQTextView_ != 0)
	{
		delete __mpQTextView_;
		__mpQTextView_ = 0;
	}
	*/
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
  (void)
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
			PDBFile __PDBFile(filename__QString.ascii());

			System __System;
			__PDBFile >> __System;
			/*
      FragmentDB db("Fragments.db");
      __System.apply(*db.buildBonds);
			*/
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
  (void)
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

			HINFile __HINFile(filename__QString.ascii());

			System __System;
			__HINFile >> __System;
			/*
      FragmentDB db("Fragments.db");
      __System.apply(*db.buildBonds);
			*/
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

void Mainframe::about
  (void)
{
	/*
	list<int> __list = __mLogStream_.getLineIndices(150,150);
	list<int>::iterator iterator__list = __list.begin();

	string __string;

	for (; iterator__list != __list.end(); ++iterator__list)
	{
		__string += __mLogStream_.getLineText(*iterator__list);
	}

	cout << __string << endl;
	*/
	//	__mpQTextView_->setText(QString(__string));
	
	
	QMessageBox::about(this, "About Molview", "Version 0.0.1");
}

void Mainframe::resizeEvent
  (QResizeEvent * /*__pQResizeEvent */)
{
}


#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/APPLICATION/mainframe.iC>
#		endif



