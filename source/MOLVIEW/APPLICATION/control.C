// $Id: control.C,v 1.3 1999/12/30 18:05:36 oliver Exp $

#include <BALL/MOLVIEW/APPLICATION/control.h>

Control::Control
  (QWidget* parent__pQWidget, 
	 const char* name__pc)
		:	QListView(parent__pQWidget, name__pc)
{
	setRootIsDecorated(TRUE);

	connect(this, 
					SIGNAL(rightButtonClicked(QListViewItem *, const QPoint &, int)),
					SLOT(ContextMenu(QListViewItem *, const QPoint &, int)));
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

	// use specified object processor for building the visible object
	// and setting the properties
	__mpMoleculeObjectProcessor_->apply(*new__pComposite);
	
	// set the center of the composite
	new__pCompositeDescriptor->setCenter(__mpMoleculeObjectProcessor_->getViewCenter());

	// generate ListViewItem
	_genListViewItem(0, new__pComposite, &name__QString);

	// set the camera on the the new composite
	__mpScene_->camera.
		set(__mpMoleculeObjectProcessor_->getViewCenter(), 
				(Camera::ViewDirection)__mpMoleculeObjectProcessor_->getViewDirection(),
				__mpMoleculeObjectProcessor_->getViewDistance());

	// update the scene
	__mpScene_->update();

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
				__QString = (residue_ptr->getName() + ": " + residue_ptr->getID()).c_str();
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
		DISPLAY__LINES               = 24
	};

	QPopupMenu __QPopupMenu;
	QPopupMenu *camera__pQPopupMenu = new QPopupMenu();
	CHECK_PTR(camera__pQPopupMenu);

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
		  __QPopupMenu.insertSeparator();
//			__QPopupMenu.insertItem("Camera", camera__pQPopupMenu);
//			camera__pQPopupMenu->insertItem("center System", CAMERA__CENTER_SYSTEM);
//			__QPopupMenu.insertItem("Display", display__pQPopupMenu);
		  __QPopupMenu.insertItem("remove Model", DISPLAY__REMOVE_MODEL);
		  __QPopupMenu.insertItem("generate Wireframe model", DISPLAY__LINES);
		  __QPopupMenu.insertItem("generate Stick model", DISPLAY__STICK);
		  __QPopupMenu.insertItem("generate Ball and Stick model", DISPLAY__BALL_AND_STICK);
		  __QPopupMenu.insertItem("generate Van der Waals model", DISPLAY__VAN_DER_WAALS);
		  __QPopupMenu.insertSeparator();
		  __QPopupMenu.insertItem("center System", CAMERA__CENTER_SYSTEM);
			break;

	  case TYPE__PROTEIN:
	  case TYPE__MOLECULE:
	  case TYPE__CHAIN:
	  case TYPE__RESIDUE:
	  case TYPE__SECONDARY_STRUCTURE:
	  case TYPE__FRAGMENT:
//			__QPopupMenu.insertItem("Display", display__pQPopupMenu);
		  __QPopupMenu.insertItem("remove Model", DISPLAY__REMOVE_MODEL);
		  __QPopupMenu.insertItem("generate Wireframe model", DISPLAY__LINES);
		  __QPopupMenu.insertItem("generate Stick model", DISPLAY__STICK);
		  __QPopupMenu.insertItem("generate Ball and Stick model", DISPLAY__BALL_AND_STICK);
		  __QPopupMenu.insertItem("generate Van der Waals model", DISPLAY__VAN_DER_WAALS);
		  __QPopupMenu.insertSeparator();
		  __QPopupMenu.insertItem("center System", CAMERA__CENTER_SYSTEM);
			break;

	  default:
			break;
	}

	__QPopupMenu.insertItem("Test 1   ", 2);
	__QPopupMenu.insertItem("Test 2   ", 3);
	__QPopupMenu.insertSeparator();
	__QPopupMenu.insertItem("Test 3   ", 4);
	

	// execute the action
	int result = __QPopupMenu.exec(__rQPoint);
	Log.info() << "ACTION: " << result << std::endl;
	switch (result)
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
			__mpMoleculeObjectProcessor_->applyCenter(*__pComposite);
	
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
			Log.info() << "REMOVE_MODEL" << std::endl;

			__mpMoleculeObjectProcessor_->setValue(ADDRESS__STATIC_MODEL, VALUE__MODEL_REMOVE);
			__mpMoleculeObjectProcessor_->setValue(ADDRESS__DYNAMIC_MODEL, VALUE__MODEL_REMOVE);

			__mpMoleculeObjectProcessor_->apply(*__pComposite);

			__mpScene_->getCompositeManager()->update(__pComposite->getRoot());
			__mpScene_->update();
		}
		break;

  	case DISPLAY__BALL_AND_STICK:
		{
			// remove Model first
			__mpMoleculeObjectProcessor_->setValue(ADDRESS__STATIC_MODEL, VALUE__MODEL_REMOVE);
			__mpMoleculeObjectProcessor_->setValue(ADDRESS__DYNAMIC_MODEL, VALUE__MODEL_REMOVE);
			__mpMoleculeObjectProcessor_->apply(*__pComposite);

			__mpMoleculeObjectProcessor_->setValue(ADDRESS__STATIC_MODEL, VALUE__MODEL_BALL_AND_STICK);
			__mpMoleculeObjectProcessor_->setValue(ADDRESS__DYNAMIC_MODEL, VALUE__MODEL_LINES);

			__mpMoleculeObjectProcessor_->apply(*__pComposite);

			__mpScene_->getCompositeManager()->update(__pComposite->getRoot());
			__mpScene_->update();
		}
		break;

  	case DISPLAY__STICK:
		{
			// remove Model first
			__mpMoleculeObjectProcessor_->setValue(ADDRESS__STATIC_MODEL, VALUE__MODEL_REMOVE);
			__mpMoleculeObjectProcessor_->setValue(ADDRESS__DYNAMIC_MODEL, VALUE__MODEL_REMOVE);
			__mpMoleculeObjectProcessor_->apply(*__pComposite);

			__mpMoleculeObjectProcessor_->setValue(ADDRESS__STATIC_MODEL, VALUE__MODEL_STICK);
			__mpMoleculeObjectProcessor_->setValue(ADDRESS__DYNAMIC_MODEL, VALUE__MODEL_LINES);

			__mpMoleculeObjectProcessor_->apply(*__pComposite);

			__mpScene_->getCompositeManager()->update(__pComposite->getRoot());
			__mpScene_->update();
		}
		break;

  	case DISPLAY__VAN_DER_WAALS:
		{
			// remove Model first
			__mpMoleculeObjectProcessor_->setValue(ADDRESS__STATIC_MODEL, VALUE__MODEL_REMOVE);
			__mpMoleculeObjectProcessor_->setValue(ADDRESS__DYNAMIC_MODEL, VALUE__MODEL_REMOVE);
			__mpMoleculeObjectProcessor_->apply(*__pComposite);

			__mpMoleculeObjectProcessor_->setValue(ADDRESS__STATIC_MODEL, VALUE__MODEL_VAN_DER_WAALS);
			__mpMoleculeObjectProcessor_->setValue(ADDRESS__DYNAMIC_MODEL, VALUE__MODEL_LINES);

			__mpMoleculeObjectProcessor_->apply(*__pComposite);

			__mpScene_->getCompositeManager()->update(__pComposite->getRoot());
			__mpScene_->update();
		}
		break;

  	case DISPLAY__LINES:
		{
			// remove Model first
			__mpMoleculeObjectProcessor_->setValue(ADDRESS__STATIC_MODEL, VALUE__MODEL_REMOVE);
			__mpMoleculeObjectProcessor_->setValue(ADDRESS__DYNAMIC_MODEL, VALUE__MODEL_REMOVE);
			__mpMoleculeObjectProcessor_->apply(*__pComposite);

			__mpMoleculeObjectProcessor_->setValue(ADDRESS__STATIC_MODEL, VALUE__MODEL_LINES);
			__mpMoleculeObjectProcessor_->setValue(ADDRESS__DYNAMIC_MODEL, VALUE__MODEL_LINES);

			__mpMoleculeObjectProcessor_->apply(*__pComposite);

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
#			include <BALL/MOLVIEW/APPLICATION/control.iC>
#		endif

