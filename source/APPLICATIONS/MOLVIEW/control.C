// $Id: control.C,v 1.11 2000/01/15 19:08:49 oliver Exp $

#include "control.h"

using namespace std;

Control::Control
  (QWidget* parent, const char* name)
		:	QListView(parent, name),
			selected_(),
			selection_changed_(true)
{
	// appearance
	setRootIsDecorated(TRUE);
	setMultiSelection(TRUE);
	setSorting(-1);

	// if the selection of any item changed,
	// mark the complete selection as invalid
	// it is then re-determined by getSelection()
	connect(this,
					SIGNAL(selectionChanged()),
					this,
					SLOT(invalidateSelection()));
}

Control::~Control()
{
}

bool Control::addComposite
  (Composite* composite, QString* own_name)
{
	if (composite == 0)
	{
		return false;
	}

	// if the own name is empty use name as name
	QString name = getName(composite);

	if ((name[0] == '<') && (own_name != 0))
	{
		name = *own_name;
	}

	// status text
	Log.info() << "> generating tree representation ... ";

	// generate ListViewItem and insert it into the ListView
	genListViewItem_(0, composite, &name);

	// update the view
  selection_changed_ = true;
	updateContents();

	return true;
}

bool Control::removeComposite(Composite* composite)
{
	QListViewItem* item = findListViewItem_(composite);
	if (item != 0)
	{
		delete item;
		selection_changed_ = true;
		updateContents();
	}

	return true;
}

QString Control::getTypeName
  (Composite* composite)
{
	QString temp;

	switch(getType(composite))
	{
	  case TYPE__PROTEIN:
			temp = "Protein";
			break;

	  case TYPE__MOLECULE:
			temp = "Molecule";
			break;

	  case TYPE__SYSTEM:
			temp = "System";
			break;

	  case TYPE__FRAGMENT:
			temp = "Fragment";
			break;

	  case TYPE__CHAIN:
			temp = "Chain";
			break;

	  case TYPE__RESIDUE:
			temp = "Residue";
			break;

	  case TYPE__SECONDARY_STRUCTURE:
			temp = "SecondaryStructure";
			break;

	  case TYPE__ATOM:
			temp = "Atom";
			break;

	  default:
			temp = "Unknown";
			break;
	}

	return temp;
}

Control::Type Control::getType
  (Composite* composite)
{
	Control::Type type = TYPE__UNKNOWN;

	if (composite == 0)
	{
		return type;
	}

	if (RTTI::isKindOf<System>(*composite))
	{
		type = TYPE__SYSTEM;
	}	
	else if (RTTI::isKindOf<Protein>(*composite))
	{
		type = TYPE__PROTEIN;
	}
	else if (RTTI::isKindOf<Molecule>(*composite))
	{
		type = TYPE__MOLECULE;
	}
	else if (RTTI::isKindOf<Chain>(*composite))
	{
		type = TYPE__CHAIN;
	}	
	else if (RTTI::isKindOf<SecondaryStructure>(*composite))
	{
		type = TYPE__SECONDARY_STRUCTURE;
	}	
	else if (RTTI::isKindOf<Residue>(*composite))
	{
		type = TYPE__RESIDUE;
	}	
	else if (RTTI::isKindOf<Fragment>(*composite))
	{
		type = TYPE__FRAGMENT;
	}	
	else if (RTTI::isKindOf<Atom>(*composite))
	{
		type = TYPE__ATOM;
	}	

	return type;
}

QString Control::getName
  (Composite* composite)
{
	QString temp = "UNKNOWN";

	switch(getType(composite))
	{
	  case TYPE__SYSTEM:
			{
				System* system = RTTI::castTo<System>(*composite);
				temp = system->getName().c_str(); 			
			}
			break;

		case TYPE__RESIDUE:
			{
				Residue* residue = RTTI::castTo<Residue>(*composite);
				temp = residue->getName().c_str();
				temp += " ";
				temp += residue->getID().c_str();
			}
			break;

	  case TYPE__MOLECULE:
		case TYPE__PROTEIN:
		case TYPE__CHAIN:
		case TYPE__FRAGMENT:
	  case TYPE__SECONDARY_STRUCTURE:
			{
				BaseFragment* base_fragment = RTTI::castTo<BaseFragment>(*composite);
				temp = base_fragment->getName().c_str();
			}
			break;

	  case TYPE__ATOM:
			{
				Atom* atom = RTTI::castTo<Atom>(*composite);
				temp = atom->getName().c_str();
			}
			break;

	  default:
		  break;
	}

	temp = temp.stripWhiteSpace();

	// empty string
	if (temp.isEmpty() || temp.isNull())
	{
		temp = "<";
		temp += getTypeName(composite);
		temp += ">";		
	}

	return temp;
}

void Control::ContextMenu
  (QListViewItem* item,  const QPoint& point, int column)
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

	if (item == 0)
	{
		return;
	}

/*	
	bool no_context_available = false;
	// select the current listviewitem
	setSelected(item, TRUE);

	// get composite address
	Composite* composite = getCompositeAddress_(item);
	
	// storing ptr
	selectedcomposite_ = composite;
	selected__mpQListViewItem_ = item;

	// get names and types
	selected_name__mQString_ = _getName(item);
	selected_root_name__mQString_ = _getRootName(item);
	selected_type__mQString_ = _getTypeName(item);
	selected_root_type__mQString_ = _getRootTypeName(item);

	QPopupMenu __QPopupMenu;

	// build the context menu
	switch (getType(composite))
	{
	  case TYPE__RESIDUE:
	  case TYPE__SYSTEM:
	  case TYPE__PROTEIN:
	  case TYPE__MOLECULE:
	  case TYPE__CHAIN:
	  case TYPE__SECONDARY_STRUCTURE:
	  case TYPE__FRAGMENT:
			{
				__QPopupMenu.insertItem("check Residue", RESIDUE__CHECK);
				__QPopupMenu.insertSeparator();
				__QPopupMenu.insertItem("cut", OBJECT__CUT);
				__QPopupMenu.insertItem("copy", OBJECT__COPY);
				__QPopupMenu.insertItem("paste", OBJECT__PASTE);
				
				if (copiedcomposite_ != 0)
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
				__QString += getTypeName(composite);

				__QPopupMenu.insertItem(__QString, OBJECT__REMOVE);
				__QPopupMenu.insertSeparator();
				__QPopupMenu.insertItem("select", SELECT);
				__QPopupMenu.insertItem("deselect", DESELECT);
	
				if (composite->isSelected())
				{
  				__QPopupMenu.setItemEnabled(SELECT, FALSE);
  				__QPopupMenu.setItemEnabled(DESELECT, TRUE);
				}
				else
				{
					__QPopupMenu.setItemEnabled(SELECT, TRUE);
					__QPopupMenu.setItemEnabled(DESELECT, FALSE);
				}
	
				__QPopupMenu.insertSeparator();
				__QPopupMenu.insertItem("center Camera", CAMERA__CENTER);
			}
			break;

	  default:
			no_context_available__bool = true;
			break;
	}

	if (no_context_available__bool)
	{
		return;
	}

	// execute the action
	switch (__QPopupMenu.exec(point))
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
	*/
}

QListViewItem* Control::getRoot_(QListViewItem* item)
{
	QListViewItem* parent = item;

	while (parent->parent() != 0)
	{
		parent = parent->parent();
	}

	return parent;
}

QString Control::getName_(QListViewItem* item)
{
	if (item == 0)
	{
		return QString("unknown");
	}

	return item->text(COLUMN_ID__NAME);
}

QString Control::getRootName_(QListViewItem* item)
{
	return getName_(getRoot_(item));
}

QString Control::getTypeName_(QListViewItem* item)
{
	if (item == 0)
	{
		return QString("unknown");
	}

	return item->text(COLUMN_ID__TYPE);
}

QString Control::getRootTypeName_(QListViewItem* item)
{
	return getTypeName_(getRoot_(item));
}

Composite* Control::getCompositeAddress_(QListViewItem* item)
{
	if (item == 0)
	{
		return 0;
	}

	// get address from the attached composite
	QString address = item->text(COLUMN_ID__ADDRESS);

	// convert address string to the real composite address
	Composite* composite = 0;
	if (address != 0)
	{
		composite = (Composite*)(atoi(address.ascii()));
	}

	return composite;
}

void Control::genListViewItem_
  (QListViewItem* item, Composite* composite, QString* default_name)
{
	if (composite == 0)
	{
		return;
	}

	// if getName returns "<xxxx>"  and name contains a valid string
	// use it instead of the default name
	QString name = getName(composite);

	if (name[0] == '<')
	{
		if (default_name == 0)
		{
			name = "<";
			name += getTypeName(composite);
			name += ">";
		} else {
			name = *default_name;
		}
	}


	// create an entry for this composite
	QString type = getTypeName(composite);
	QString address_string;
	address_string.sprintf("%ld", (((unsigned long)((void*)composite))));

	// create a new list item
	QListViewItem* new_item = 0;

	// is this the first item?
	if (item == 0)
	{
		// yes, insert into the root node 
		new_item = new QListViewItem
							(this, 
							 name, type, QString::null, QString::null,
							 QString::null, QString::null, address_string);
	} else {
		// no, insert into the current item
		new_item = new QListViewItem
							(item, name, type, QString::null, QString::null,
							 QString::null, QString::null, address_string);
	}
	CHECK_PTR(new_item);

	// if the composite is anything but an atom,
	// we iterate over all children and recurse
	if (!RTTI::isKindOf<Atom>(*composite))
	{
		Composite::ChildCompositeReverseIterator it = composite->rbeginChildComposite();
		for (; it != composite->rendChildComposite(); ++it)
		{
			genListViewItem_(new_item, &*it);
		}
	}
}

QListViewItem* Control::findListViewItem_
  (Composite* composite)
{
	QString address;
	address.sprintf("%ld", (((unsigned long)((void *)composite))));

	QListViewItemIterator it(this);

	// iterate through all items
	for (; it.current(); ++it)
	{
		// find the address of the composite
		if (it.current()->text(6) == address)
		{
			return it.current();
		}
	}

	return 0;
}

const List<Composite*>& Control::getSelection()
{
	if (selection_changed_)
	{
		selected_.clear();

		QListViewItemIterator it(this);
		for (; it.current(); ++it)
		{
			if (it.current()->isSelected())
			{
				Composite* composite = getCompositeAddress_(it.current());
				if (composite != 0)
				{
					selected_.push_back(composite);
				}
			}
		}
		
		selection_changed_ = false;
	}
			
	return selected_;
}

void Control::invalidateSelection()
{
	selection_changed_ = true;
}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include "control.iC"
#		endif

