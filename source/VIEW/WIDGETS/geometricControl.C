// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: geometricControl.C,v 1.39 2004/05/17 11:24:59 amoll Exp $

#include <BALL/VIEW/WIDGETS/geometricControl.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/KERNEL/representation.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/DIALOGS/colorMeshDialog.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/atomContainer.h>
#include <BALL/VIEW/DIALOGS/displayProperties.h>
#include <BALL/STRUCTURE/geometricProperties.h>
#include <BALL/VIEW/DATATYPE/vertex2.h>
#include <BALL/VIEW/DATATYPE/vertex1.h>
#include <BALL/VIEW/PRIMITIVES/sphere.h>
#include <BALL/VIEW/PRIMITIVES/mesh.h>
#include <BALL/VIEW/PRIMITIVES/disc.h>
#include <BALL/VIEW/PRIMITIVES/point.h>
#include <BALL/VIEW/PRIMITIVES/twoColoredLine.h>
#include <BALL/VIEW/PRIMITIVES/twoColoredTube.h>
#include <BALL/VIEW/PRIMITIVES/tube.h>
#include <BALL/MATHS/simpleBox3.h>

#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qtooltip.h> 

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
		colorMeshDlg_(new ColorMeshDialog(this, "ColorMeshDialog")),
		creating_representations_(false)
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "new GeometricControl " << this << std::endl;
#endif
	listview->addColumn("[visible] Model");
	listview->addColumn("Color");
	listview->addColumn("Properties");
	listview->setColumnWidth(0, 60);
	listview->setColumnWidth(1, 60);
	listview->setColumnWidth(2, 60);
	String txt = String("List of the representations: \n") +
							"1.column: model type and name of the molecular entity, the model was created from\n" +
							"2.column: used coloring method\n" +
							"3.column: number of used molecular entities, number of geometric objects. ";

	QToolTip::add(listview, txt.c_str());
	connect(listview, SIGNAL(selectionChanged()), this, SLOT(updateSelection()));
	registerWidget(this);
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

	// prevent flickering in GeometricControl, e.g. while a simulation is running
	bool changed_content = false;
	String new_text;

	new_text = getRepresentationName_(rep);
	changed_content = item->text(0).ascii() != new_text;
	if (changed_content) item->setText(0, new_text.c_str());

	new_text = rep.getColoringName();
	changed_content |= item->text(1).ascii() != new_text;
	if (changed_content) item->setText(1, new_text.c_str());

	new_text = rep.getProperties();
	changed_content |= item->text(2).ascii() != new_text;
	if (changed_content) item->setText(2, new_text.c_str());

	if (changed_content) listview->triggerUpdate();

	// ????? testing, if I can remove these lines: 13.03.04
	/*
	RepresentationMessage* message = new RepresentationMessage(rep, RepresentationMessage::SELECTED);
	notify_(message);
	*/
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
		case RepresentationMessage::STARTED_UPDATE:
			creating_representations_ = true;
			return;

		case RepresentationMessage::FINISHED_UPDATE:
			creating_representations_ = false;
			return;

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
	insertContextMenuEntry("Focus", this, SLOT(focusRepresentation()), 5);
	insertContextMenuEntry("Delete", this, SLOT(deleteRepresentation_()), 10);
	insertContextMenuEntry("Modify Model", this, SLOT(modifyRepresentation_()), 20);	
	if (getSelectedItems().size() != 1)
	{
		context_menu_.setItemEnabled(5, false); 
	}

	if (getSelectedItems().size() > 1 ||
			rep.getModelType() >= MODEL_LABEL)
	{
		context_menu_.setItemEnabled(20, false); 
	}

	context_menu_.insertSeparator();

	// This is used to provide the coloring for meshes...
	insertContextMenuEntry("Color Surface", colorMeshDlg_, SLOT(show()), 30);	
	if (!isSurfaceModel(rep.getModelType()))
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
	if (creating_representations_) 
	{
		setStatusbarText("Can not modify representations, while creating a new one!");
		return;
	}

 	ItemList selected = getSelectedItems();
	for (ItemList::Iterator it = selected.begin(); it != selected.end(); ++it)
	{
		Representation* rep = ((SelectableListViewItem*) *it)->getRepresentation();
		if (rep->hasProperty(Representation::PROPERTY__IS_COORDINATE_SYSTEM))
		{
			SceneMessage *scene_message = new SceneMessage(SceneMessage::REMOVE_COORDINATE_SYSTEM);
			notify_(scene_message);
		}
			
		RepresentationMessage* message = new RepresentationMessage(*rep, 
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

	if (representation.needsUpdate()) 
	{
		representation.update(true);
		RepresentationMessage* msg = 
			new RepresentationMessage(representation, RepresentationMessage::UPDATE);
		notify_(msg);
	}
}


void GeometricControl::onContextMenu_(QListViewItem* item,  const QPoint& point, int /* column */)
{
	if (item == 0) return;

	if (!getMainControl()->compositesAreMuteable() ||
			creating_representations_)
	{
		setStatusbarText("No changes to representations allowed, while simulation is running or creating new representations!");
		return;
	}

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
		return;
	}

	Representation* rep = getRepresentation(*item);

	RepresentationMessage* message = new RepresentationMessage(*rep, RepresentationMessage::SELECTED);
	notify_(message);

	if (rep->getComposites().size() > 0) 
	{
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
	}

	// update ColorMeshDialog if representation is a surface
	if (isSurfaceModel(rep->getModelType()) &&
			rep->getGeometricObjects().size() > 0)
	{
		colorMeshDlg_->setMesh((Mesh*)*(rep->getGeometricObjects().begin()), rep);
	}
}

List<Representation*> GeometricControl::getSelection() const
	throw()
{
	List<Representation*> selection;
	ItemList selected = ((GeometricControl*)this)->getSelectedItems();
	for (ItemList::Iterator it = selected.begin(); it != selected.end(); ++it)
	{
		Representation* rep = ((SelectableListViewItem*) *it)->getRepresentation();
		selection.push_back(rep);
	}
	return selection;
}


void GeometricControl::checkMenu(MainControl& main_control)
	throw()
{
	ItemList item_list = getSelectedItems(); 
	if (item_list.size() > 0) main_control.enableDeleteEntry();
}

void GeometricControl::focusRepresentation()
{
	if (getSelectedItems().size() != 1) return;
	Representation* rep = *getSelection().begin();
	
	GeometricCenterProcessor centerp;
	BoundingBoxProcessor bbox;
	centerp.start();
	bbox.start();

	List<GeometricObject*>::Iterator it = rep->getGeometricObjects().begin();
	for (; it != rep->getGeometricObjects().end(); it++)
	{
		GeometricObject& go = **it;
		Vector3 center;

		// cant use Vertex or Vertex2 here, no idea why
		if (RTTI::isKindOf<TwoColoredLine>(go))
		{
			TwoColoredLine& v = reinterpret_cast<TwoColoredLine&>(go);
 			center = (v.getVertex1() + (v.getVertex2() - v.getVertex1()) / 2.0);

			bbox.operator()(v.getVertex1());
			bbox.operator()(v.getVertex2());
		}
		else if (RTTI::isKindOf<TwoColoredTube>(go))
		{
			TwoColoredTube& v = reinterpret_cast<TwoColoredTube&>(go);
 			center = (v.getVertex1() + (v.getVertex2() - v.getVertex1()) / 2.0);

			bbox.operator()(v.getVertex1());
			bbox.operator()(v.getVertex2());
		}
		else if (RTTI::isKindOf<Tube>(go))
		{
			Tube& v = reinterpret_cast<Tube&>(go);
 			center = (v.getVertex1() + (v.getVertex2() - v.getVertex1()) / 2.0);

			bbox.operator()(v.getVertex1());
			bbox.operator()(v.getVertex2());
		}
		else if (RTTI::isKindOf<Point> (go))
		{
			Point& v = reinterpret_cast<Point&>(go);
			center = v.getVertex();
			bbox.operator()(v.getVertex());
		}
		else if (RTTI::isKindOf<SimpleBox3>(go))
		{
			SimpleBox3& b = reinterpret_cast<SimpleBox3&>(go);
			center = b.a + (b.b - b.a) /2;

			bbox.operator()(b.a);
			bbox.operator()(b.b);
		}
		else if (RTTI::isKindOf<Sphere>(go))
		{
			Sphere& s = reinterpret_cast<Sphere&>(go);
			center = s.getPosition();

			bbox.operator()(s.getPosition());
		}
		else if (RTTI::isKindOf<Disc>(go))
		{
			Disc& d = reinterpret_cast<Disc&>(go);
			center = d.getCircle().p;

			bbox.operator()(d.getCircle().p);
		}
		else if (RTTI::isKindOf<Mesh>(go))
		{
			Mesh& mesh = reinterpret_cast<Mesh&>(go);

			for (Size index = 0; index < mesh.vertex.size(); ++index)
			{
				centerp.operator()(mesh.vertex[index]);
				bbox.operator()(mesh.vertex[index]);
			}
			continue;
		}
		else
		{
			Log.error() << "Unknown geometric object: " << typeid(go).name() << std::endl;
			continue;
		}

		centerp.operator() (center);
	}

	centerp.finish();
	bbox.finish();
	
	Vector3 vwp = centerp.getCenter();
	float view_distance = (bbox.getUpper() - bbox.getLower()).getLength() - vwp.z + 3;

	// update scene
	SceneMessage *scene_message = new SceneMessage(SceneMessage::UPDATE_CAMERA);
	scene_message->getCamera().setLookAtPosition(vwp);
	vwp.z += view_distance;
	scene_message->getCamera().setViewPoint(vwp);
	notify_(scene_message);
}



} } // namespaces
