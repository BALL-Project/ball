// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: geometricControl.C,v 1.27 2004/01/15 13:11:42 amoll Exp $

#include <BALL/VIEW/WIDGETS/geometricControl.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/KERNEL/representation.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/DIALOGS/colorMeshDialog.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/atomContainer.h>
#include <BALL/VIEW/DIALOGS/displayProperties.h>
#include <qpopupmenu.h>
#include <qmenubar.h>

using std::endl;

namespace BALL
{
	namespace VIEW
	{

GeometricControl::SelectableListViewItem::SelectableListViewItem(
		QListView* parent, const QString& text,
		Representation* representation, GeometricControl& control)
	throw()
	: QCheckListItem(parent, text, QCheckListItem::CheckBox),
		representation_(representation),
		control_reference_(control)
{
	setText(0, text);
	setText(1, representation->getColoringName().c_str());
	setText(2, representation->getProperties().c_str());
}

void GeometricControl::SelectableListViewItem::stateChange(bool)
	throw()
{
	control_reference_.selectedRepresentation(*representation_, isOn());
}


GeometricControl::GeometricControl(QWidget* parent, const char* name)
	throw()
		:	GenericControl(parent, name),
			context_menu_(),
			context_representation_(0),
			colorMeshDlg_(new ColorMeshDialog(this, "ColorMeshDialog"))
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "new GeometricControl " << this << std::endl;
#endif
	listview->addColumn("[visible] Model");
	listview->addColumn("Coloring");
	listview->addColumn("Properties");
	listview->setColumnWidth(0, 60);
	listview->setColumnWidth(1, 60);
	listview->setColumnWidth(2, 60);

	connect(listview, SIGNAL(selectionChanged()), this, SLOT(updateSelection()));
}

GeometricControl::~GeometricControl()
	throw()
{
  #ifdef BALL_VIEW_DEBUG
	  Log.error() << "Destructing object " << this << " of class GeometricControl" << std::endl;
  #endif 
}

void GeometricControl::addRepresentation(Representation& rep)
	throw()
{
	if (representation_to_item_.has(&rep)) 
	{
		Log.error() << "Tried to add an already inserted Representation in " << __FILE__ << __LINE__ << std::endl;
		return;
	}
	generateListViewItem_(rep);

	// update the view
	listview->triggerUpdate();
}

void GeometricControl::removeRepresentation(Representation& rep)
	throw()
{
	QListViewItem* item = representation_to_item_[&rep]; 
	if (item != 0)
	{
		delete item;
		representation_to_item_.erase(&rep);		
		listview->triggerUpdate();
		return;
	}

	return;
}

void GeometricControl::updateRepresentation(Representation& rep)
	throw()
{
	QListViewItem* item = representation_to_item_[&rep]; 
	if (item == 0) return;
	
	item->setText(0, getRepresentationName_(rep).c_str());
	item->setText(1, rep.getColoringName().c_str());
	item->setText(2, rep.getProperties().c_str());
	listview->triggerUpdate();

	RepresentationMessage* message = new RepresentationMessage(rep, RepresentationMessage::SELECTED);
	notify_(message);
	return;
}

void GeometricControl::onNotify(Message *message)
	throw()
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "GeometricControl " << this << " onNotify " << message << std::endl;
#endif

	GenericControl::onNotify(message);

	if (!RTTI::isKindOf<RepresentationMessage> (*message))
	{
		return;
	}
	
	Representation* rep =	(RTTI::castTo<RepresentationMessage> (*message))->getRepresentation();
	if (rep == 0) return;

	switch ((RTTI::castTo<RepresentationMessage> (*message))->getType())
	{
		case RepresentationMessage::ADD:
			addRepresentation(*rep);
			return;
	
		case RepresentationMessage::REMOVE:
			removeRepresentation(*rep);
			return;

		case RepresentationMessage::UPDATE:
			updateRepresentation(*rep);
			return;

		default:
			Log.error() << "Unknown Type of Representation in " << __FILE__ << __LINE__ << std::endl;
	}
}

void GeometricControl::buildContextMenu(Representation& rep)
	throw()
{
	insertContextMenuEntry("Delete", this, SLOT(deleteRepresentation_()), 10);
	insertContextMenuEntry("Modify Model", this, SLOT(modifyRepresentation_()), 20);	
	if (getSelectedItems().size() > 1 ||
			rep.getModelType() >= MODEL_LABEL)
	{
		context_menu_.setItemEnabled(20, false); 
	}

	context_menu_.insertSeparator();

	// This is used to provide the coloring for meshes...
	insertContextMenuEntry("Color Surface", colorMeshDlg_, SLOT(show()), 30);	
	if (isSurfaceModel(rep.getModelType())
			&& rep.getGeometricObjects().size() > 0)
	{
		colorMeshDlg_->setMesh((Mesh*)*(rep.getGeometricObjects().begin()), &rep);
	}
	else
	{
		context_menu_.setItemEnabled(30, false);
	}
}

void GeometricControl::insertContextMenuEntry(const String& name, const QObject* receiver, const char* slot, int entry_ID, int accel)
	throw()
{
	context_menu_.insertItem(name.c_str(), receiver, slot, accel, entry_ID);
}

Representation* GeometricControl::getRepresentation(QListViewItem& item)
	throw()
{
	return ((SelectableListViewItem*) &item)->getRepresentation();
}

String GeometricControl::getRepresentationName_(const Representation& rep)
	throw()
{
	String name;
	if (!rep.hasProperty(Representation::PROPERTY__IS_COORDINATE_SYSTEM))
	{
		name = rep.getModelName().c_str();
	}
	else
	{
		name = "Coordinate System";
	}

	if (rep.getComposites().size() > 0)
	{
		const Composite* c_ptr = *rep.getComposites().begin();
		while (rep.getComposites().has(c_ptr->getParent()))
		{
			c_ptr = c_ptr->getParent();
		}
		
		String composite_name;
		if (RTTI::isKindOf<Atom>(*c_ptr))
		{
			if (c_ptr->getParent() != 0)
			{
				((Composite*)c_ptr->getParent())->host(information_);
				composite_name = information_.getName();
				composite_name += ":";
				composite_name += ((const Atom*) c_ptr)->getName();
			}
			else
			{
				composite_name = ((const Atom*) c_ptr)->getFullName();
			}
		}
		else
		{
			((Composite*)c_ptr)->host(information_);
			composite_name = information_.getName();
		}

		name = name + "  " + composite_name;

		if (rep.getComposites().size() > 1) name += "...";
	}

	return name;
}

void GeometricControl::generateListViewItem_(Representation& rep)
	throw()
{
	QString properties = rep.getProperties().c_str();
	// create a new list item
	SelectableListViewItem* new_item = 
		new SelectableListViewItem(listview, 
					getRepresentationName_(rep).c_str(), 
					&rep, *this);

	CHECK_PTR(new_item);
	new_item->setOn(true);

	representation_to_item_[&rep] = new_item;
}


void GeometricControl::deleteRepresentation_()
{
	if (context_representation_ == 0)  return; 
	ItemList selected = getSelectedItems();
	for (ItemList::Iterator it = selected.begin(); it != selected.end(); ++it)
	{
		Representation* rep = ((SelectableListViewItem*) *it)->getRepresentation();
		if (context_representation_->hasProperty(Representation::PROPERTY__IS_COORDINATE_SYSTEM))
		{
			SceneMessage *scene_message = new SceneMessage(SceneMessage::REMOVE_COORDINATE_SYSTEM);
			notify_(scene_message);
		}
			
		RepresentationMessage* message = new RepresentationMessage(*context_representation_, 
																															 RepresentationMessage::REMOVE);
		notify_(message);

		getMainControl()->getPrimitiveManager().remove(*rep);
		removeRepresentation(*rep);
	}

	setStatusbarText("Deleted representation.");
}

void GeometricControl::selectedRepresentation(Representation& representation, bool state)
{
	if (state != representation.hasProperty(Representation::PROPERTY__HIDDEN)) return;

	if (!state)
	{
		setStatusbarText("Hide representation.");
	}
	else
	{
		setStatusbarText("Show representation.");
	}
		
	representation.toggleProperty(Representation::PROPERTY__HIDDEN);

// 	SceneMessage *scene_message = new SceneMessage(SceneMessage::REDRAW);
	SceneMessage *scene_message = new SceneMessage(SceneMessage::REBUILD_DISPLAY_LISTS);
	notify_(scene_message);
}


void GeometricControl::onContextMenu_(QListViewItem* item,  const QPoint& point, int /* column */)
{
	if (item == 0) return;

	// clear the context menu
	context_menu_.clear();

	// get Representation address
	Representation* rep = getRepresentation(*item);
	if (rep == 0) return;
	// create the context menu
	context_representation_ = rep;
	context_item_ = item;

	buildContextMenu(*rep);

	// is the context menu not empty
	if (context_menu_.count())
	{
		context_menu_.popup(point);
	}
}

void GeometricControl::modifyRepresentation_()
	throw()
{
	if (context_representation_ == 0) return;
	ShowDisplayPropertiesMessage* message = new ShowDisplayPropertiesMessage;
	notify_(message);
}

void GeometricControl::updateSelection()
{
	GenericControl::updateSelection();

	QListViewItem* item = 0;
	QListViewItemIterator it(listview);
	for (; it.current(); ++it)
	{
		if (it.current()->isSelected())
		{
			if (item == 0)
			{
				item = it.current();
			}
			else
			{
				return;
			}
		}
	}

	if (item == 0) 
	{
		colorMeshDlg_->setMesh(0,0);
		return;
	}

	Representation* rep = getRepresentation(*item);

	RepresentationMessage* message = new RepresentationMessage(*rep, RepresentationMessage::SELECTED);
	notify_(message);

	if (rep->getComposites().size() == 0) 
	{
		return;
	}

	String name;
	const Composite* c_ptr = *rep->getComposites().begin();

	while (!c_ptr->isRoot())
	{
		if (RTTI::isKindOf<AtomContainer> (*c_ptr))
		{
			name = ((const AtomContainer*) c_ptr)->getName() + "->" + name;
		}
		else if (RTTI::isKindOf<Atom> (*c_ptr))
		{
			name = ((const Atom*) c_ptr)->getName() + "->" + name;
		}

		c_ptr = c_ptr->getParent();
	}

	if (RTTI::isKindOf<AtomContainer>(*c_ptr))
	{
		name = ((const AtomContainer*)c_ptr)->getProperty("FROM_FILE").getString() + "->" + name;
	}
	
	name.trimRight("->");

	if (rep->getComposites().size() > 1) name += "...";

	setStatusbarText("Representation from " + name);

	// update ColorMeshDialog if representation is a surface
	if ((rep->getModelType() == MODEL_SE_SURFACE || rep->getModelType() == MODEL_SA_SURFACE) &&
			rep->getGeometricObjects().size() > 0)
	{
		colorMeshDlg_->setMesh((Mesh*)*(rep->getGeometricObjects().begin()), rep);
	}
}

} } // namespaces
