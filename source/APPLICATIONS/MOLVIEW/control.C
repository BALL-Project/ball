// $Id: control.C,v 1.3 2000/01/09 17:43:39 hekl Exp $

#include "control.h"

Control::Control
  (QWidget* parent__pQWidget, 
	 const char* name__pc)
		:	QListView(parent__pQWidget, name__pc),
			selected__mpComposite_(0),
			selected__mpQListViewItem_(0),
			selected_name__mQString_("unkown"),
			selected_root_name__mQString_("unkown"),
			selected_type__mQString_("unkown"),
			selected_root_type__mQString_("unkown"),
			copied__mpComposite_(0),
			__mDisplayProperties_(0)
{
	setRootIsDecorated(TRUE);
	//	setMultiSelection(TRUE);

	connect(this, 
					SIGNAL(rightButtonClicked(QListViewItem *, const QPoint &, int)),
					SLOT(ContextMenu(QListViewItem *, const QPoint &, int)));

	connect(this,
					SIGNAL(selectionChanged(QListViewItem *)),
					SLOT(objectSelected(QListViewItem *)));

	connect(&__mDisplayProperties_,
					SIGNAL(apply()),
					this,
					SLOT(applyDisplayProperties()));
}

Control::~Control
  (void)
{
}

void 
Control::updateContent
  (void)
{
}

bool
Control::addComposite
  (Composite *__pComposite,
	 QString *name__pQString)
{
	if (__pComposite == 0)
	{
		return false;
	}

	// if the own name is empty use name__pString as name
	QString name__QString = getName(__pComposite);

	if (name__QString == "UNKOWN")
	{
		if (name__pQString != 0)
		{
			name__QString = *name__pQString;
		}
	}

	// status text
	Log.info() << "> checking scene integrity ... ";

	// test, if composite is already inserted into scene
	CompositeManager *__pCompositeManager 
		= 	__mpScene_->getCompositeManager();

	// is there already a descriptor with the same name 
	CompositeDescriptor *__pCompositeDescriptor 
		= __pCompositeManager->getDescriptor(name__QString.ascii());

	if (__pCompositeDescriptor != 0)
	{
		// is the type the same
		if (getType(__pComposite) == getType(__pCompositeDescriptor->getComposite()))
		{
			// ask if the old composite should be replaced by the new one
			QString a__QString, b__QString;
			a__QString.sprintf("%s '%s' is already inserted in scene.\n", 
												 getTypeName(__pComposite).ascii(), name__QString.ascii());

			b__QString.sprintf("%s\n\nWould you like to replace it ?", a__QString.ascii());

			int button__i	= QMessageBox::information
				(this, "Load information", b__QString, "Yes", "No");

			if (button__i == 0) // yes
			{
				// removes the subtree belonging to the composite
				delete _findListViewItem(__pCompositeDescriptor->getComposite());

				// removes the descritor from the scene
				__pCompositeManager->remove(*__pCompositeDescriptor);
			}
			else // no
			{
				return true;
			}
		}
	}

	// status text
	Log.info() << "done." << endl;

	// status text
	Log.info() << "> generating new scene object ... ";

	// insert the composite into scene
	// create a tempory CompositeDescriptor
	CompositeDescriptor __CompositeDescriptor;
	__CompositeDescriptor.setName(name__QString.ascii());
	__CompositeDescriptor.setComposite(*__pComposite);

	// insert the desciptor into the compositemanager
	// and create a new deep copy of the compositedescriptor
	CompositeDescriptor *new__pCompositeDescriptor
		= __pCompositeManager->insert(__CompositeDescriptor, true);

	// if the descriptor is 0 => something very bad has happened
	if (new__pCompositeDescriptor == 0)
	{
		QMessageBox::about(this, "Load object error", "descriptor deep clone failed !");
		return false;
	}

	// get the new cloned composite
	Composite *new__pComposite = new__pCompositeDescriptor->getComposite();

	// status text
	Log.info() << "done." << endl;

	// status text
	Log.info() << "> generating graphical representation ... ";

	// use specified object processor for building the visible object
	// and setting the properties
	__mpMoleculeObjectProcessor_->applyOn(*new__pComposite);
	
	// set the center of the composite
	new__pCompositeDescriptor->setCenter(__mpMoleculeObjectProcessor_->getViewCenter());

	// status text
	Log.info() << "done." << endl;

	// status text
	Log.info() << "> generating tree representation ... ";

	// generate ListViewItem
	_genListViewItem(0, new__pComposite, &name__QString);

	// status text
	Log.info() << "done." << endl;

	// status text
	Log.info() << "> setting up scene ... ";

	// set the camera on the the new composite
	__mpScene_->camera.
		set(__mpMoleculeObjectProcessor_->getViewCenter(), 
				(Camera::ViewDirection)__mpMoleculeObjectProcessor_->getViewDirection(),
				__mpMoleculeObjectProcessor_->getViewDistance());

	// update the scene
	__mpScene_->update();

	// status text
	Log.info() << "done." << endl;

	return true;
}

bool 
Control::removeComposite
  (Composite *__pComposite)
{
	return true;
}

QString
Control::getTypeName
  (Composite *__pComposite)
{
	QString temp__QString;

	switch(getType(__pComposite))
	{
	  case TYPE__PROTEIN:
			temp__QString = "Protein";
			break;

	  case TYPE__MOLECULE:
			temp__QString = "Molecule";
			break;

	  case TYPE__SYSTEM:
			temp__QString = "System";
			break;

	  case TYPE__FRAGMENT:
			temp__QString = "Fragment";
			break;

	  case TYPE__CHAIN:
			temp__QString = "Chain";
			break;

	  case TYPE__RESIDUE:
			temp__QString = "Residue";
			break;

	  case TYPE__SECONDARY_STRUCTURE:
			temp__QString = "SecondaryStructure";
			break;

	  case TYPE__ATOM:
			temp__QString = "Atom";
			break;

	  default:
			temp__QString = "Unkown";
			break;
	}

	return temp__QString;
}

Control::Type 
Control::getType
  (Composite *__pComposite)
{
	Control::Type __Type = TYPE__UNKOWN;

	if (__pComposite == 0)
	{
		return __Type;
	}

	if (RTTI::isKindOf<System>(*__pComposite))
	{
		__Type = TYPE__SYSTEM;
	}	
	else if (RTTI::isKindOf<Protein>(*__pComposite))
	{
		__Type = TYPE__PROTEIN;
	}
	else if (RTTI::isKindOf<Molecule>(*__pComposite))
	{
		__Type = TYPE__MOLECULE;
	}
	else if (RTTI::isKindOf<Chain>(*__pComposite))
	{
		__Type = TYPE__CHAIN;
	}	
	else if (RTTI::isKindOf<SecondaryStructure>(*__pComposite))
	{
		__Type = TYPE__SECONDARY_STRUCTURE;
	}	
	else if (RTTI::isKindOf<Residue>(*__pComposite))
	{
		__Type = TYPE__RESIDUE;
	}	
	else if (RTTI::isKindOf<Fragment>(*__pComposite))
	{
		__Type = TYPE__FRAGMENT;
	}	
	else if (RTTI::isKindOf<Atom>(*__pComposite))
	{
		__Type = TYPE__ATOM;
	}	

	return __Type;
}

QString 
Control::getName
  (Composite *__pComposite)
{
	QString __QString = "UNKOWN";

	switch(getType(__pComposite))
	{
	  case TYPE__SYSTEM:
			{
				System *system_ptr = RTTI::castTo<System>(*__pComposite);
				__QString = system_ptr->getName().c_str(); 			
			}
			break;

	  case TYPE__MOLECULE:
			{
				Molecule *molecule_ptr = RTTI::castTo<Molecule>(*__pComposite);
				__QString = molecule_ptr->getName().c_str(); 			
			}
			break;

	  case TYPE__PROTEIN:
			{
				Protein *protein_ptr = RTTI::castTo<Protein>(*__pComposite);
				__QString = protein_ptr->getName().c_str();
			}
			break;

	  case TYPE__CHAIN:
			{
				Chain *chain_ptr = RTTI::castTo<Chain>(*__pComposite);
				__QString = chain_ptr->getName().c_str();
			}
			break;

	  case TYPE__FRAGMENT:
			{
				Fragment *fragment_ptr = RTTI::castTo<Fragment>(*__pComposite);
				__QString = fragment_ptr->getName().c_str();
			}
			break;

	  case TYPE__RESIDUE:
			{
				Residue *residue_ptr = RTTI::castTo<Residue>(*__pComposite);
				__QString = (residue_ptr->getName() + " " + residue_ptr->getID()).c_str();
			}
			break;

	  case TYPE__SECONDARY_STRUCTURE:
			{
				SecondaryStructure *secondarystructure_ptr 
					= RTTI::castTo<SecondaryStructure>(*__pComposite);
				__QString = secondarystructure_ptr->getName().c_str();
			}
			break;

	  case TYPE__ATOM:
			{
				Atom *atom_ptr 
					= RTTI::castTo<Atom>(*__pComposite);
				__QString = atom_ptr->getName().c_str();
			}
			break;

	  default:
		  break;
	}

	__QString = __QString.stripWhiteSpace();

	// empty string
	if (__QString.isEmpty()
			|| __QString.isNull())
	{
		__QString = "UNKOWN";
	}

	return __QString;
}

/*
void 
Control::ContextMenu
  (QListViewItem *__pQListViewItem, 
	 const QPoint &__rQPoint,
	 int column__i)
{
	enum
	{
		REMOVE__SYSTEM               = 0,
		REMOVE__PROTEIN              = 1,
		REMOVE__MOLECULE             = 2,
		REMOVE__FRAGMENT             = 3,
		REMOVE__CHAIN                = 4,
		REMOVE__RESIDUE              = 5,
		REMOVE__SECONDARY_STRUCTURE  = 6,
		CAMERA__CENTER_SYSTEM        = 10,
		DISPLAY__REMOVE_MODEL        = 20,
		DISPLAY__BALL_AND_STICK      = 21,
		DISPLAY__STICK               = 22,
		DISPLAY__VAN_DER_WAALS       = 23,
		DISPLAY__LINES               = 24,
		SELECT                       = 30,
		DESELECT                     = 31
	};

	QPopupMenu __QPopupMenu;
	QPopupMenu *camera__pQPopupMenu = new QPopupMenu();
	CHECK_PTR(camera__pQPopupMenu);
	camera__pQPopupMenu->insertItem("center Camera", CAMERA__CENTER_SYSTEM);

	QPopupMenu *display__pQPopupMenu = new QPopupMenu();
	CHECK_PTR(display__pQPopupMenu);
	display__pQPopupMenu->insertItem("remove Model", DISPLAY__REMOVE_MODEL);
	display__pQPopupMenu->insertSeparator();
	display__pQPopupMenu->insertItem("generate Wireframe model", DISPLAY__LINES);
	display__pQPopupMenu->insertItem("generate Stick model", DISPLAY__STICK);
	display__pQPopupMenu->insertItem("generate Ball and Stick model", DISPLAY__BALL_AND_STICK);
	display__pQPopupMenu->insertItem("generate Van der Waals model", DISPLAY__VAN_DER_WAALS);

	// get address from the attached composite
	QString address__QString = __pQListViewItem->text(6);

	// convert address string to the real composite address
	Composite *__pComposite;
	__pComposite = (Composite *)(atoi(address__QString.ascii()));

	// get name and type
	QString name__QString = __pQListViewItem->text(0);
	QString type__QString = __pQListViewItem->text(1);

	// build the context menu
	switch (getType(__pComposite))
	{
	  case TYPE__SYSTEM:
			__QPopupMenu.insertItem("remove System", REMOVE__SYSTEM);
			__QPopupMenu.insertItem("Camera", camera__pQPopupMenu);
			__QPopupMenu.insertItem("Display", display__pQPopupMenu);
			__QPopupMenu.insertSeparator();
			__QPopupMenu.insertItem("select", SELECT);
			__QPopupMenu.insertItem("deselect", DESELECT);
			break;

	  case TYPE__PROTEIN:
	  case TYPE__MOLECULE:
	  case TYPE__CHAIN:
	  case TYPE__RESIDUE:
	  case TYPE__SECONDARY_STRUCTURE:
	  case TYPE__FRAGMENT:
			__QPopupMenu.insertItem("Camera", camera__pQPopupMenu);
			__QPopupMenu.insertItem("Display", display__pQPopupMenu);
			__QPopupMenu.insertSeparator();
			__QPopupMenu.insertItem("select", SELECT);
			__QPopupMenu.insertItem("deselect", DESELECT);
			break;

	  default:
			break;
	}

	__QPopupMenu.insertItem("Test 1   ", 2);
	__QPopupMenu.insertItem("Test 2   ", 3);
	__QPopupMenu.insertSeparator();
	__QPopupMenu.insertItem("Test 3   ", 4);
	
	// status text
	outputStatus(QString("preforming action ..."));

	// execute the action
	switch (__QPopupMenu.exec(__rQPoint))
	{
	  case REMOVE__SYSTEM:
		{
			QString remove__QString;
			remove__QString.sprintf("Do you really want to remove %s '%s'.\n\n", 
															type__QString.ascii(), name__QString.ascii());

			int button__i	= QMessageBox::information
				(this, "Remove information", remove__QString, "Yes", "No");

			if (button__i == 0) // yes
			{
				// removes the subtree belonging to the composite
				delete __pQListViewItem;

				// removes the composite from the scene
				__mpScene_->getCompositeManager()->remove(*__pComposite);
			}
		}
		break;

  	case CAMERA__CENTER_SYSTEM:
		{
			// use specified object processor for calculating the center
			__mpMoleculeObjectProcessor_->calculateCenter(*__pComposite);

			// set the camera on the the new composite
			__mpScene_->camera.
				set(__mpMoleculeObjectProcessor_->getViewCenter(), 
						(Camera::ViewDirection)__mpMoleculeObjectProcessor_->getViewDirection(),
						__mpMoleculeObjectProcessor_->getViewDistance());

			__mpScene_->update();
		}
		break;

  	case DISPLAY__REMOVE_MODEL:
		{
			__mpMoleculeObjectProcessor_->setValue(ADDRESS__STATIC_MODEL, VALUE__MODEL_REMOVE);
			__mpMoleculeObjectProcessor_->setValue(ADDRESS__DYNAMIC_MODEL, VALUE__MODEL_REMOVE);

			__mpMoleculeObjectProcessor_->applyOn(*__pComposite);

			__mpScene_->getCompositeManager()->update(__pComposite->getRoot());
			__mpScene_->update();
		}
		break;

  	case DISPLAY__BALL_AND_STICK:
		{
			// remove Model first
			__mpMoleculeObjectProcessor_->setValue(ADDRESS__STATIC_MODEL, VALUE__MODEL_REMOVE);
			__mpMoleculeObjectProcessor_->setValue(ADDRESS__DYNAMIC_MODEL, VALUE__MODEL_REMOVE);
			__mpMoleculeObjectProcessor_->applyOn(*__pComposite);

			__mpMoleculeObjectProcessor_->setValue(ADDRESS__STATIC_MODEL, VALUE__MODEL_BALL_AND_STICK);
			__mpMoleculeObjectProcessor_->setValue(ADDRESS__DYNAMIC_MODEL, VALUE__MODEL_LINES);

			__mpMoleculeObjectProcessor_->applyOn(*__pComposite);

			__mpScene_->getCompositeManager()->update(__pComposite->getRoot());
			__mpScene_->update();
		}
		break;

  	case DISPLAY__STICK:
		{
			// remove Model first
			__mpMoleculeObjectProcessor_->setValue(ADDRESS__STATIC_MODEL, VALUE__MODEL_REMOVE);
			__mpMoleculeObjectProcessor_->setValue(ADDRESS__DYNAMIC_MODEL, VALUE__MODEL_REMOVE);
			__mpMoleculeObjectProcessor_->applyOn(*__pComposite);

			__mpMoleculeObjectProcessor_->setValue(ADDRESS__STATIC_MODEL, VALUE__MODEL_STICK);
			__mpMoleculeObjectProcessor_->setValue(ADDRESS__DYNAMIC_MODEL, VALUE__MODEL_LINES);

			__mpMoleculeObjectProcessor_->applyOn(*__pComposite);

			__mpScene_->getCompositeManager()->update(__pComposite->getRoot());
			__mpScene_->update();
		}
		break;

  	case DISPLAY__VAN_DER_WAALS:
		{
			// remove Model first
			__mpMoleculeObjectProcessor_->setValue(ADDRESS__STATIC_MODEL, VALUE__MODEL_REMOVE);
			__mpMoleculeObjectProcessor_->setValue(ADDRESS__DYNAMIC_MODEL, VALUE__MODEL_REMOVE);
			__mpMoleculeObjectProcessor_->applyOn(*__pComposite);

			__mpMoleculeObjectProcessor_->setValue(ADDRESS__STATIC_MODEL, VALUE__MODEL_VAN_DER_WAALS);
			__mpMoleculeObjectProcessor_->setValue(ADDRESS__DYNAMIC_MODEL, VALUE__MODEL_LINES);

			__mpMoleculeObjectProcessor_->applyOn(*__pComposite);

			__mpScene_->getCompositeManager()->update(__pComposite->getRoot());
			__mpScene_->update();
		}
		break;

  	case DISPLAY__LINES:
		{
			// remove Model first
			__mpMoleculeObjectProcessor_->setValue(ADDRESS__STATIC_MODEL, VALUE__MODEL_REMOVE);
			__mpMoleculeObjectProcessor_->setValue(ADDRESS__DYNAMIC_MODEL, VALUE__MODEL_REMOVE);
			__mpMoleculeObjectProcessor_->applyOn(*__pComposite);

			__mpMoleculeObjectProcessor_->setValue(ADDRESS__STATIC_MODEL, VALUE__MODEL_LINES);
			__mpMoleculeObjectProcessor_->setValue(ADDRESS__DYNAMIC_MODEL, VALUE__MODEL_LINES);

			__mpMoleculeObjectProcessor_->applyOn(*__pComposite);

			__mpScene_->getCompositeManager()->update(__pComposite->getRoot());
			__mpScene_->update();
		}
		break;

  	case SELECT:
		{
			__mpMoleculeObjectProcessor_->setValue(ADDRESS__STATIC_MODEL, VALUE__SELECT);
			__mpMoleculeObjectProcessor_->setValue(ADDRESS__DYNAMIC_MODEL, VALUE__SELECT);
			__mpMoleculeObjectProcessor_->applyOn(*__pComposite);

			__mpScene_->getCompositeManager()->update(__pComposite->getRoot());
			__mpScene_->update();
		}
		break;

  	case DESELECT:
		{
			__mpMoleculeObjectProcessor_->setValue(ADDRESS__STATIC_MODEL, VALUE__DESELECT);
			__mpMoleculeObjectProcessor_->setValue(ADDRESS__DYNAMIC_MODEL, VALUE__DESELECT);
			__mpMoleculeObjectProcessor_->applyOn(*__pComposite);

			__mpScene_->getCompositeManager()->update(__pComposite->getRoot());
			__mpScene_->update();
		}
		break;

    	default:
			break;
	}

	// clear
	delete camera__pQPopupMenu;
	delete display__pQPopupMenu;

	// status text
	outputStatus(QString(" done.\n"), false);
}
*/

void 
Control::ContextMenu
  (QListViewItem *__pQListViewItem, 
	 const QPoint &__rQPoint,
	 int column__i)
{
	enum
	{
		OBJECT__REMOVE               = 0,
		OBJECT__CUT                  = 1,
		OBJECT__COPY                 = 2,
		OBJECT__PASTE                = 3,
		CAMERA__CENTER               = 10,
		BONDS__BUILD                 = 20,
		BONDS__REMOVE                = 21,
		SELECT                       = 30,
		DESELECT                     = 31,
		RESIDUE__CHECK               = 40,
		DISPLAY__CHANGE              = 50
	};

	if (__pQListViewItem == 0)
	{
		return;
	}

	// select the current listviewitem
	setSelected(__pQListViewItem, TRUE);

	// get composite address
	Composite *__pComposite = _getCompositeAddress(__pQListViewItem);
	
	// storing ptr
	selected__mpComposite_ = __pComposite;
	selected__mpQListViewItem_ = __pQListViewItem;

	// get names and types
	selected_name__mQString_ = _getName(__pQListViewItem);
	selected_root_name__mQString_ = _getRootName(__pQListViewItem);
	selected_type__mQString_ = _getTypeName(__pQListViewItem);
	selected_root_type__mQString_ = _getRootTypeName(__pQListViewItem);

	QPopupMenu __QPopupMenu;

	// build the context menu
	switch (getType(__pComposite))
	{
	  case TYPE__RESIDUE:
			__QPopupMenu.insertItem("check Residue", RESIDUE__CHECK);
			__QPopupMenu.insertSeparator();
	  case TYPE__SYSTEM:
	  case TYPE__PROTEIN:
	  case TYPE__MOLECULE:
	  case TYPE__CHAIN:
	  case TYPE__SECONDARY_STRUCTURE:
	  case TYPE__FRAGMENT:
			{
				__QPopupMenu.insertItem("cut", OBJECT__CUT);
				__QPopupMenu.insertItem("copy", OBJECT__COPY);
				__QPopupMenu.insertItem("paste", OBJECT__PASTE);
				
				if (copied__mpComposite_ != 0)
				{
					__QPopupMenu.setItemEnabled(OBJECT__PASTE, TRUE);
				}
				else
				{
					__QPopupMenu.setItemEnabled(OBJECT__PASTE, FALSE);
				}
				
				__QPopupMenu.insertSeparator();
				__QPopupMenu.insertItem("build Bonds", BONDS__BUILD);
				__QPopupMenu.insertItem("remove Bonds", BONDS__REMOVE);
				__QPopupMenu.insertSeparator();
				
				QString __QString("remove ");
				__QString += getTypeName(__pComposite);

				__QPopupMenu.insertItem(__QString, OBJECT__REMOVE);
				__QPopupMenu.insertSeparator();
				__QPopupMenu.insertItem("select", SELECT);
				__QPopupMenu.insertItem("deselect", DESELECT);
	
//				if (__pComposite->isSelected())
//				{
//  				__QPopupMenu.setItemEnabled(DESELECT, TRUE);
//				}
//				else
//				{
//				__QPopupMenu.setItemEnabled(DESELECT, FALSE);
//			}
	
				__QPopupMenu.insertSeparator();
				__QPopupMenu.insertItem("center Camera", CAMERA__CENTER);
			}
			break;

	  default:
			break;
	}

	// execute the action
	switch (__QPopupMenu.exec(__rQPoint))
	{
	  case RESIDUE__CHECK:
			checkResidue();
			break;

	  case OBJECT__CUT:
			cut();
			break;

	  case OBJECT__COPY:
			copy();
			break;

	  case OBJECT__PASTE:
			paste();
			break;

	  case OBJECT__REMOVE:
			removeObject();
			break;

	  case BONDS__BUILD:
			buildBonds();
			break;

	  case BONDS__REMOVE:
			removeBonds();
			break;

	  case SELECT:
			select();
			break;

	  case DESELECT:
			deselect();
			break;

	  case CAMERA__CENTER:
			centerCamera();
			break;

	  default:
			break;
	}
}

void 
Control::objectSelected
  (QListViewItem *__pQListViewItem)
{
	if (__pQListViewItem == 0)
	{
		selected__mpComposite_ = 0;
		selected__mpQListViewItem_ = 0;
		selected_name__mQString_ = "unkown";
		selected_root_name__mQString_ = "unkown";
		selected_type__mQString_ = "unkown";
		selected_root_type__mQString_ = "unkown";

		emit itemSelected(false, false);

		return;
	}

	// convert address string to the real composite address
	Composite *__pComposite = _getCompositeAddress(__pQListViewItem);

	// storing ptr
	selected__mpComposite_ = __pComposite;
	selected__mpQListViewItem_ = __pQListViewItem;

	// get names and types
	selected_name__mQString_ = _getName(__pQListViewItem);
	selected_root_name__mQString_ = _getRootName(__pQListViewItem);
	selected_type__mQString_ = _getTypeName(__pQListViewItem);
	selected_root_type__mQString_ = _getRootTypeName(__pQListViewItem);

	if (getType(selected__mpComposite_) == TYPE__RESIDUE)
	{
		emit itemSelected(true, true);
	}
	else
	{
		emit itemSelected(true, false);
	}
}

void 
Control::cut()
{
	QMessageBox::about(this, "CUT-DEMO", "cut object choosen.");

	emit itemCutOrCopied(true);
}

void 
Control::copy()
{
	QMessageBox::about(this, "COPY-DEMO", "copy object choosen.");

	emit itemCutOrCopied(true);
}

void 
Control::paste()
{
	QMessageBox::about(this, "PASTE-DEMO", "paste object choosen.");
}

void 
Control::buildBonds()
{
	QMessageBox::about(this, "BONDS-DEMO", "build bonds choosen.");
}

void 
Control::removeBonds()
{
	QMessageBox::about(this, "BONDS-DEMO", "remove bonds choosen.");
}

void 
Control::select()
{
	if (selected__mpComposite_ == 0)
	{
		return;
	}

	// status text
	Log.info() << selected_root_type__mQString_.ascii()
						 << " ´"
						 << selected_root_name__mQString_.ascii() 
						 << "´> selecting " 
						 << selected_type__mQString_.ascii()
						 << " ´"
						 << selected_name__mQString_.ascii()
						 << "´ ... ";

	// save old values
	int value_static__i = __mpMoleculeObjectProcessor_->getValue(ADDRESS__STATIC_MODEL);
	int value_dynamic__i = __mpMoleculeObjectProcessor_->getValue(ADDRESS__DYNAMIC_MODEL);

	__mpMoleculeObjectProcessor_->setValue(ADDRESS__STATIC_MODEL, VALUE__SELECT);
	__mpMoleculeObjectProcessor_->setValue(ADDRESS__DYNAMIC_MODEL, VALUE__SELECT);
	__mpMoleculeObjectProcessor_->applyOn(*selected__mpComposite_);
	
	// restore old values
	__mpMoleculeObjectProcessor_->setValue(ADDRESS__STATIC_MODEL, value_static__i);
	__mpMoleculeObjectProcessor_->setValue(ADDRESS__DYNAMIC_MODEL, value_dynamic__i);

	// update scene
	__mpScene_->getCompositeManager()->update(selected__mpComposite_->getRoot());
	__mpScene_->update();

	// status text
	Log.info() << "done." << endl;
}

void
Control::deselect()
{
	if (selected__mpComposite_ == 0)
	{
		return;
	}

	// status text
	Log.info() << selected_root_type__mQString_.ascii()
						 << " ´"
						 << selected_root_name__mQString_.ascii() 
						 << "´> deselecting " 
						 << selected_type__mQString_.ascii()
						 << " ´"
						 << selected_name__mQString_.ascii()
						 << "´ ... ";

	// save old values
	int value_static__i = __mpMoleculeObjectProcessor_->getValue(ADDRESS__STATIC_MODEL);
	int value_dynamic__i = __mpMoleculeObjectProcessor_->getValue(ADDRESS__DYNAMIC_MODEL);

	__mpMoleculeObjectProcessor_->setValue(ADDRESS__STATIC_MODEL, VALUE__DESELECT);
	__mpMoleculeObjectProcessor_->setValue(ADDRESS__DYNAMIC_MODEL, VALUE__DESELECT);
	__mpMoleculeObjectProcessor_->applyOn(*selected__mpComposite_);
	
	// restore old values
	__mpMoleculeObjectProcessor_->setValue(ADDRESS__STATIC_MODEL, value_static__i);
	__mpMoleculeObjectProcessor_->setValue(ADDRESS__DYNAMIC_MODEL, value_dynamic__i);

	// update scene
	__mpScene_->getCompositeManager()->update(selected__mpComposite_->getRoot());
	__mpScene_->update();

	// status text
	Log.info() << "done." << endl;
}

void 
Control::checkResidue()
{
	if (selected__mpComposite_ == 0)
	{
		return;
	}

	__mpMoleculeObjectProcessor_->checkResidue(*selected__mpComposite_);
}

void 
Control::removeObject()
{
	if (selected__mpComposite_ == 0)
	{
		return;
	}

	// status text
	Log.info() << selected_root_type__mQString_.ascii()
						 << " ´"
						 << selected_root_name__mQString_.ascii() 
						 << "´> removing " 
						 << selected_type__mQString_.ascii()
						 << " ´"
						 << selected_name__mQString_.ascii()
						 << "´ ... " << endl;

	if (RTTI::isKindOf<System>(*selected__mpComposite_))
	{
			QString remove__QString;
			remove__QString.sprintf("Do you really want to remove %s '%s'.\n\n", 
															selected_type__mQString_.ascii(), selected_name__mQString_.ascii());

			int button__i	= QMessageBox::information
				(this, "Remove information", remove__QString, "Yes", "No");

			if (button__i == 0) // yes
			{
				// removes the composite from the scene
				__mpScene_->getCompositeManager()->remove(*selected__mpComposite_);
				__mpScene_->update();

				// removes the subtree belonging to the composite
				delete selected__mpQListViewItem_;

			}

			selected__mpComposite_ = 0;
			selected__mpQListViewItem_ = 0;
	}
	else
	{
		QString __QString = "remove ";
		__QString += selected_type__mQString_;

		QMessageBox::about(this, __QString, "not yet supported!");
	}

	// status text
	Log.info() << "done." << endl;
}

void 
Control::centerCamera()
{
	if (selected__mpComposite_ == 0)
	{
		return;
	}

	// status text
	Log.info() << selected_root_type__mQString_.ascii()
						 << " ´"
						 << selected_root_name__mQString_.ascii() 
						 << "´> set camera at " 
						 << selected_type__mQString_.ascii()
						 << " ´"
						 << selected_name__mQString_.ascii()
						 << "´ ... ";

	// use specified object processor for calculating the center
	__mpMoleculeObjectProcessor_->calculateCenter(*selected__mpComposite_);
	
	// set the camera on the the new composite
	__mpScene_->camera.
		set(__mpMoleculeObjectProcessor_->getViewCenter(), 
				(Camera::ViewDirection)__mpMoleculeObjectProcessor_->getViewDirection(),
				__mpMoleculeObjectProcessor_->getViewDistance());
	
	__mpScene_->update();

	// status text
	Log.info() << "done." << endl;
}

void 
Control::openDisplay()
{
	__mDisplayProperties_.show();
}

void 
Control::clearClipboard()
{
	QMessageBox::about(this, "CLIPBOARD-DEMO", "clear clipboard choosen.");

	emit itemCutOrCopied(false);	
}

void 
Control::applyDisplayProperties()
{
	if (selected__mpComposite_ == 0)
	{
		return;
	}

	Log.info() << selected_root_type__mQString_.ascii()
						 << " ´"
						 << selected_root_name__mQString_.ascii() 
						 << "´> applying display properties on " 
						 << selected_type__mQString_.ascii()
						 << " ´"
						 << selected_name__mQString_.ascii()
						 << "´: ";

	__mpMoleculeObjectProcessor_->applyOn(*selected__mpComposite_);
	
	__mpScene_->getCompositeManager()->update(selected__mpComposite_->getRoot());

	__mpScene_->update();
}

QListViewItem *
Control::_getRoot(QListViewItem *__pQListViewItem)
{
	QListViewItem *parent__pQListViewItem = __pQListViewItem;

	while (parent__pQListViewItem->parent() != 0)
	{
		parent__pQListViewItem = parent__pQListViewItem->parent();
	}

	return parent__pQListViewItem;
}

QString 
Control::_getName(QListViewItem *__pQListViewItem)
{
	if (__pQListViewItem == 0)
	{
		return QString("unkown");
	}

	return __pQListViewItem->text(COLUMN_ID__NAME);
}

QString 
Control::_getRootName(QListViewItem *__pQListViewItem)
{
	return _getName(_getRoot(__pQListViewItem));
}

QString 
Control::_getTypeName(QListViewItem *__pQListViewItem)
{
	if (__pQListViewItem == 0)
	{
		return QString("unkown");
	}

	return __pQListViewItem->text(COLUMN_ID__TYPE);
}

QString 
Control::_getRootTypeName(QListViewItem *__pQListViewItem)
{
	return _getTypeName(_getRoot(__pQListViewItem));
}

Composite *
Control::_getCompositeAddress(QListViewItem *__pQListViewItem)
{
	if (__pQListViewItem == 0)
	{
		return (Composite *)0;
	}

	// get address from the attached composite
	QString address__QString = __pQListViewItem->text(COLUMN_ID__ADDRESS);

	// convert address string to the real composite address
	Composite *__pComposite;
	__pComposite = (Composite *)(atoi(address__QString.ascii()));

	return __pComposite;
}

void 
Control::_genListViewItem
  (QListViewItem *__pQListViewItem,
	 Composite *__pComposite,
	 QString *name__pQString)
{
	if (__pComposite == 0)
	{
		return;
	}

	QString type__QString;
	QString address__QString;

	// if getName returns ´UNKOWN´ and name__pString contains a valid string
	// use it instead of the string ´UNKOWN´
	QString name__QString = getName(__pComposite);

	if (name__QString == "UNKOWN")
	{
		if (name__pQString != 0)
		{
			name__QString = *name__pQString;
		}
	}

	switch (getType(__pComposite))
	{
	  case TYPE__MOLECULE:
			{
				QListViewItem *new__pQListViewItem = 0;
				type__QString = "Molecule";

				address__QString.sprintf("%ld", (((unsigned long)((void *)__pComposite))));
				
				// is it the first listviewitem (use this as parent)
				if (__pQListViewItem == 0)
				{
					new__pQListViewItem 
						= new QListViewItem(this,	name__QString, type__QString,
																QString::null, QString::null, QString::null, QString::null,
																address__QString);
					
					CHECK_PTR(new__pQListViewItem);
				}
				else
				{
					new__pQListViewItem 
						= new QListViewItem(__pQListViewItem, name__QString, type__QString,
																QString::null, QString::null,	QString::null, QString::null,
																address__QString);
					
					CHECK_PTR(new__pQListViewItem);
				}

				// recursiv all included fragments
				Molecule *__pMolecule = RTTI::castTo<Molecule>(*__pComposite);
				FragmentIterator __FragmentIterator;
				int index__i = 0;
				
				for(__FragmentIterator = __pMolecule->beginFragment();
						__FragmentIterator != __pMolecule->endFragment();
						__FragmentIterator++)
				{
					QString fragname__QString;
					fragname__QString.sprintf("Fragment_%d", ++index__i);

					_genListViewItem(new__pQListViewItem, 
													 (Composite *)&(*__FragmentIterator),
													 &fragname__QString);
				}

				// all Atoms
				AtomIterator __AtomIterator;
				index__i = 0;
				
				for(__AtomIterator = __pMolecule->beginAtom();
						__AtomIterator != __pMolecule->endAtom();
						__AtomIterator++)
				{
					QString atom__QString;
					atom__QString.sprintf("Atom_%d", ++index__i);

					_genListViewItem(new__pQListViewItem, 
													 (Composite *)&(*__AtomIterator),
													 &atom__QString);
				}
			}
			break;

	  case TYPE__PROTEIN:
			{
				QListViewItem *new__pQListViewItem = 0; 
				type__QString = "Protein";

				address__QString.sprintf("%ld", (((unsigned long)((void *)__pComposite))));

				// is it the first listviewitem (use this as parent)
				if (__pQListViewItem == 0)
				{
					new__pQListViewItem 
						= new QListViewItem(this,	name__QString, type__QString,
																QString::null, QString::null, QString::null, QString::null,
																address__QString);
					
					CHECK_PTR(new__pQListViewItem);
				}
				else
				{
					new__pQListViewItem 
						= new QListViewItem(__pQListViewItem, name__QString, type__QString,
																QString::null, QString::null,	QString::null, QString::null,
																address__QString);
					
					CHECK_PTR(new__pQListViewItem);
				}

				// recursiv all included chains
				Protein *__pProtein = RTTI::castTo<Protein>(*__pComposite);
				ChainIterator __ChainIterator;
				int index__i = 0;
				
				for(__ChainIterator = __pProtein->beginChain();
						__ChainIterator != __pProtein->endChain();
						__ChainIterator++)
				{
					QString chain__QString;
					chain__QString.sprintf("Chain_%d", ++index__i);

					_genListViewItem(new__pQListViewItem, 
													 (Composite *)&(*__ChainIterator),
													 &chain__QString);
				}
			}
			break;

  	case TYPE__SYSTEM:
			{
				QListViewItem *new__pQListViewItem = 0;
				type__QString = "System";
				
				address__QString.sprintf("%ld", (((unsigned long)((void *)__pComposite))));

				// is it the first listviewitem (use this as parent)
				if (__pQListViewItem == 0)
				{
					new__pQListViewItem 
						= new QListViewItem(this,	name__QString, type__QString,
																QString::null, QString::null, QString::null, QString::null,
																address__QString);
					
					CHECK_PTR(new__pQListViewItem);
				}
				else
				{
					new__pQListViewItem 
						= new QListViewItem(__pQListViewItem, name__QString, type__QString,
																QString::null, QString::null,	QString::null, QString::null,
																address__QString);
					
					CHECK_PTR(new__pQListViewItem);
				}

				// recursiv all included molecules			
				System *__pSystem = RTTI::castTo<System>(*__pComposite);
				MoleculeIterator __MoleculeIterator;
				int index__i = 0;
				
				for(__MoleculeIterator = __pSystem->beginMolecule();
						__MoleculeIterator != __pSystem->endMolecule();
						__MoleculeIterator++)
				{
					Composite *__pComposite;
					QString molprotname__QString;

					// is the molecule a protein ?
					if (RTTI::isKindOf<Protein>(*__MoleculeIterator))
					{
						__pComposite = (Composite *)(RTTI::castTo<Protein>(*__MoleculeIterator));
						molprotname__QString.sprintf("Protein_%d", ++index__i);
					}
					else
					{
						__pComposite = (Composite *)&(*__MoleculeIterator);
						molprotname__QString.sprintf("Molecule_%d", ++index__i);
					}

					_genListViewItem(new__pQListViewItem, 
													 __pComposite,
													 &molprotname__QString);
				}
			}	
			break;

  	case TYPE__CHAIN:
			{
				QListViewItem *new__pQListViewItem = 0;
				type__QString = "Chain";
				
				address__QString.sprintf("%ld", (((unsigned long)((void *)__pComposite))));
				
				// is it the first listviewitem (use this as parent)
				if (__pQListViewItem == 0)
				{
					new__pQListViewItem 
						= new QListViewItem(this,	name__QString, type__QString,
																QString::null, QString::null, QString::null, QString::null,
																address__QString);
					
					CHECK_PTR(new__pQListViewItem);
				}
				else
				{
					new__pQListViewItem 
						= new QListViewItem(__pQListViewItem, name__QString, type__QString,
																QString::null, QString::null,	QString::null, QString::null,
																address__QString);
					
					CHECK_PTR(new__pQListViewItem);
				}

				Chain *__pChain = RTTI::castTo<Chain>(*__pComposite);
				SecondaryStructureIterator __SecondaryStructureIterator;
				int index__i = 0;
				
				for(__SecondaryStructureIterator = __pChain->beginSecondaryStructure();
						__SecondaryStructureIterator != __pChain->endSecondaryStructure();
						__SecondaryStructureIterator++)
				{
					QString secondaryStructure__QString;
					secondaryStructure__QString.sprintf("SecondaryStructure_%d", ++index__i);

					_genListViewItem(new__pQListViewItem, 
													 (Composite *)&(*__SecondaryStructureIterator),
													 &secondaryStructure__QString);
				}

				// all Residues
				ResidueIterator __ResidueIterator;
				index__i = 0;
				
				for(__ResidueIterator = __pChain->beginResidue();
						__ResidueIterator != __pChain->endResidue();
						__ResidueIterator++)
				{
					QString residue__QString;
					residue__QString.sprintf("Residue_%d", ++index__i);

					_genListViewItem(new__pQListViewItem, 
													 (Composite *)&(*__ResidueIterator),
													 &residue__QString);
				}
			}	
			break;

  	case TYPE__SECONDARY_STRUCTURE:
			{
				QListViewItem *new__pQListViewItem = 0;
				type__QString = "SecondaryStructure";
				
				address__QString.sprintf("%ld", (((unsigned long)((void *)__pComposite))));
				
				// is it the first listviewitem (use this as parent)
				if (__pQListViewItem == 0)
				{
					new__pQListViewItem 
						= new QListViewItem(this,	name__QString, type__QString,
																QString::null, QString::null, QString::null, QString::null,
																address__QString);
					
					CHECK_PTR(new__pQListViewItem);
				}
				else
				{
					new__pQListViewItem 
						= new QListViewItem(__pQListViewItem, name__QString, type__QString,
																QString::null, QString::null,	QString::null, QString::null,
																address__QString);
					
					CHECK_PTR(new__pQListViewItem);
				}

				// all Residues
				SecondaryStructure *__pSecondaryStructure = RTTI::castTo<SecondaryStructure>(*__pComposite);
				ResidueIterator __ResidueIterator;
				int index__i = 0;
				
				for(__ResidueIterator = __pSecondaryStructure->beginResidue();
						__ResidueIterator != __pSecondaryStructure->endResidue();
						__ResidueIterator++)
				{
					QString residue__QString;
					residue__QString.sprintf("Residue_%d", ++index__i);

					_genListViewItem(new__pQListViewItem, 
													 (Composite *)&(*__ResidueIterator),
													 &residue__QString);
				}
			}	
			break;

  	case TYPE__RESIDUE:
			{
				QListViewItem *new__pQListViewItem = 0;
				type__QString = "Residue";
				
				address__QString.sprintf("%ld", (((unsigned long)((void *)__pComposite))));
				
				// is it the first listviewitem (use this as parent)
				if (__pQListViewItem == 0)
				{
					new__pQListViewItem 
						= new QListViewItem(this,	name__QString, type__QString,
																QString::null, QString::null, QString::null, QString::null,
																address__QString);
					
					CHECK_PTR(new__pQListViewItem);
				}
				else
				{
					new__pQListViewItem 
						= new QListViewItem(__pQListViewItem, name__QString, type__QString,
																QString::null, QString::null,	QString::null, QString::null,
																address__QString);
					
					CHECK_PTR(new__pQListViewItem);
				}

				// all Atoms
				Residue *__pResidue = RTTI::castTo<Residue>(*__pComposite);
				AtomIterator __AtomIterator;
				int index__i = 0;
				
				for(__AtomIterator = __pResidue->beginAtom();
						__AtomIterator != __pResidue->endAtom();
						__AtomIterator++)
				{
					QString atom__QString;
					atom__QString.sprintf("Atom_%d", ++index__i);

					_genListViewItem(new__pQListViewItem, 
													 (Composite *)&(*__AtomIterator),
													 &atom__QString);
				}
			}	
			break;

  	case TYPE__FRAGMENT:
			{
				QListViewItem *new__pQListViewItem = 0;
				type__QString = "Fragment";
				
				address__QString.sprintf("%ld", (((unsigned long)((void *)__pComposite))));
				
				// is it the first listviewitem (use this as parent)
				if (__pQListViewItem == 0)
				{
					new__pQListViewItem 
						= new QListViewItem(this,	name__QString, type__QString,
																QString::null, QString::null, QString::null, QString::null,
																address__QString);
					
					
					CHECK_PTR(new__pQListViewItem);
				}
				else
				{
					new__pQListViewItem 
						= new QListViewItem(__pQListViewItem, name__QString, type__QString,
																QString::null, QString::null,	QString::null, QString::null,
																address__QString);
					
					CHECK_PTR(new__pQListViewItem);
				}

				// all Atoms
				Fragment *__pFragment = RTTI::castTo<Fragment>(*__pComposite);
				AtomIterator __AtomIterator;
				int index__i = 0;
				
				for(__AtomIterator = __pFragment->beginAtom();
						__AtomIterator != __pFragment->endAtom();
						__AtomIterator++)
				{
					QString atom__QString;
					atom__QString.sprintf("Atom_%d", ++index__i);

					_genListViewItem(new__pQListViewItem, 
													 (Composite *)&(*__AtomIterator),
													 &atom__QString);
				}
			}	
			break;

  	case TYPE__ATOM:
			{
				type__QString = "Atom";
				
				address__QString.sprintf("%ld", (((unsigned long)((void *)__pComposite))));
				
				// is it the first listviewitem (use this as parent)
				if (__pQListViewItem == 0)
				{
					QListViewItem *new__pQListViewItem 
						= new QListViewItem(this,	name__QString, type__QString,
																QString::null, QString::null, QString::null, QString::null,
																address__QString);
					
					
					CHECK_PTR(new__pQListViewItem);
				}
				else
				{
					QListViewItem *new__pQListViewItem 
						= new QListViewItem(__pQListViewItem, name__QString, type__QString,
																QString::null, QString::null,	QString::null, QString::null,
																address__QString);
					
					CHECK_PTR(new__pQListViewItem);
				}
			}	
			break;

  	default:
			break;
	}
}

QListViewItem *
Control::_findListViewItem
  (Composite *__pComposite)
{
	QString address__QString;
	address__QString.sprintf("%ld", (((unsigned long)((void *)__pComposite))));

	QListViewItemIterator __QListViewItemIterator(this);

	// iterate through all items
	for (; __QListViewItemIterator.current(); ++__QListViewItemIterator)
	{
		// find the address of the composite
		if (__QListViewItemIterator.current()->text(6) == address__QString)
		{
			return __QListViewItemIterator.current();
		}
	}

	return (QListViewItem *)0;
}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include "control.iC"
#		endif

