// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: geometricControl.C,v 1.8 2003/09/18 09:17:48 amoll Exp $

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

GeometricControl::SelectableListViewItem::SelectableListViewItem(QListView* parent, const QString& text, const QString& type, Representation* representation, GeometricControl& control)
	throw()
	: QCheckListItem(parent, text, QCheckListItem::CheckBox),
		representation_(representation),
		control_reference_(control)
{
	setText(1, type);
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
			colorMeshDlg_(0)
{
	listview->addColumn("[visible] Type");
	listview->addColumn("Properties");
	listview->setColumnWidth(0, 60);
	listview->setColumnWidth(1, 60);

	connect(listview, SIGNAL(selectionChanged()), this, SLOT(updateSelection()));
}

GeometricControl::~GeometricControl()
	throw()
{
  #ifdef BALL_VIEW_DEBUG
	  Log.error() << "Destructing object " << (void *)this << " of class " 
								<< RTTI::getName<GeometricControl>() << endl;
  #endif 

	if (colorMeshDlg_) delete colorMeshDlg_;
}

void GeometricControl::addRepresentation(Representation& rep)
	throw()
{
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
	
	QString properties = rep.getProperties().c_str();
	if (item->text(1) == properties) return;

	item->setText(0, getRepresentationName_(rep).c_str());
	item->setText(1, properties);

	listview->triggerUpdate();

	return;
}

void GeometricControl::onNotify(Message *message)
	throw()
{
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
	insertContextMenuEntry("Delete", this, SLOT(deleteRepresentation_()));
	insertContextMenuEntry("Properties", this, SLOT(modifyRepresentation_()));	

	// This is used to provide the coloring for meshes...
	if ((rep.getModelType() == MODEL_SE_SURFACE || rep.getModelType() == MODEL_SA_SURFACE)	
			&& rep.getGeometricObjects().size() > 0)
	{
		if (!colorMeshDlg_)
		{
			colorMeshDlg_ = new ColorMeshDialog(this);
		}
		colorMeshDlg_->setMesh(*(Mesh*)*(rep.getGeometricObjects().begin()));
		colorMeshDlg_->setRepresentation(rep);
		insertContextMenuEntry("Color Surface", colorMeshDlg_, SLOT(show()));	
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
		new SelectableListViewItem(listview, getRepresentationName_(rep).c_str(), properties, &rep, *this);

	CHECK_PTR(new_item);
	new_item->setOn(true);

	representation_to_item_[&rep] = new_item;
}


void GeometricControl::deleteRepresentation_()
{
	if (context_representation_ == 0)  return; 

	SceneMessage *scene_message = new SceneMessage;
	scene_message->setType(SceneMessage::REDRAW);
	scene_message->setDeletable(true);
	if (context_representation_->hasProperty(Representation::PROPERTY__IS_COORDINATE_SYSTEM))
	{
		scene_message->setType(SceneMessage::REMOVE_COORDINATE_SYSTEM);
	}
		
	RepresentationMessage* message = new RepresentationMessage;
	message->setType(RepresentationMessage::REMOVE);
	message->setRepresentation(context_representation_);
	message->setDeletable(true);
	notify_(message);

	getMainControl()->getPrimitiveManager().remove(*context_representation_);
	removeRepresentation(*context_representation_);

	notify_(scene_message);
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

	SceneMessage *scene_message = new SceneMessage;
	scene_message->setType(SceneMessage::REDRAW);
	scene_message->setDeletable(true);
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
		context_menu_.exec(point);
	}
}

void GeometricControl::modifyRepresentation_()
	throw()
{
	if (context_representation_ == 0) return;
	ShowDisplayPropertiesMessage* message = new ShowDisplayPropertiesMessage;
	message->setDeletable(true);
	notify_(message);
}

void GeometricControl::updateSelection()
{
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
		setStatusbarText("");
		return;
	}

	Representation* rep = getRepresentation(*item);

	RepresentationMessage* message = new RepresentationMessage;
	message->setType(RepresentationMessage::SELECTED);
	message->setRepresentation(rep);
	message->setDeletable(true);
	notify_(message);

	if (rep->getComposites().size() == 0) 
	{
		setStatusbarText("");
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
	if (colorMeshDlg_ && rep->getModelName() == "Surface" && rep->getGeometricObjects().size() > 0)
	{
		colorMeshDlg_->setMesh(*(Mesh*)*(rep->getGeometricObjects().begin()));
		colorMeshDlg_->setRepresentation(*rep);
	}
}

} } // namespaces
