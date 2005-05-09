// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: geometricControl.C,v 1.73.4.7 2005/05/09 15:36:57 amoll Exp $
//

#include <BALL/VIEW/WIDGETS/geometricControl.h>
#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/KERNEL/representation.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/VIEW/KERNEL/clippingPlane.h>

#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/atomContainer.h>

#include <BALL/VIEW/DIALOGS/displayProperties.h>
#include <BALL/VIEW/DIALOGS/modifySurfaceDialog.h>
#include <BALL/VIEW/DIALOGS/clippingDialog.h>

#include <BALL/VIEW/DATATYPE/vertex2.h>
#include <BALL/VIEW/DATATYPE/vertex1.h>

#include <BALL/VIEW/PRIMITIVES/sphere.h>
#include <BALL/VIEW/PRIMITIVES/mesh.h>
#include <BALL/VIEW/PRIMITIVES/disc.h>
#include <BALL/VIEW/PRIMITIVES/box.h>
#include <BALL/VIEW/PRIMITIVES/label.h>

#include <BALL/VIEW/MODELS/standardColorProcessor.h>

#include <BALL/MATHS/simpleBox3.h>
#include <BALL/STRUCTURE/geometricProperties.h>

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
				control_reference_(control),
				ignore_change_(false)
		{
			setText(0, text);
			setText(1, representation->getColoringName().c_str());
			setText(2, representation->getProperties().c_str());
			setOn(!representation->isHidden());
		}

		void GeometricControl::SelectableListViewItem::stateChange(bool state)
			throw()
		{
			if (ignore_change_)
			{
				ignore_change_ = false;
				return;
			}

			if (control_reference_.getMainControl()->compositesAreLocked())
			{
				ignore_change_ = true;
				setOn(!state);
				VIEW::getMainControl()->setStatusbarText("Cannot switch on/off Representation now!", true);
				return;
			}
			control_reference_.selectedRepresentation(*representation_, isOn());
		}


		GeometricControl::GeometricControl(QWidget* parent, const char* name)
			throw()
			:	GenericControl(parent, name),
				context_menu_(this),
				clipping_plane_context_menu_(this),
				context_representation_(0),
				modify_surface_dialog_(new ModifySurfaceDialog(this, "ModifySurfaceDialog")),
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

			clipping_plane_context_menu_.insertItem("Move Clipping Plane", this, SLOT(moveClippingPlane()));
			clipping_plane_context_menu_.insertItem("Flip Clipping Plane", this, SLOT(flipClippingPlane()));	
			clipping_plane_context_menu_.insertItem("Set Clipping Plane to x axis", this, SLOT(setClippingPlaneX()));	
			clipping_plane_context_menu_.insertItem("Set Clipping Plane to y axis", this, SLOT(setClippingPlaneY()));	
			clipping_plane_context_menu_.insertItem("Set Clipping Plane to z axis", this, SLOT(setClippingPlaneZ()));	
			clipping_plane_context_menu_.insertItem("Select Representations to clip", this, SLOT(selectClipRepresentations()));	
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
				Log.error() << "Tried to add an already inserted Representation in " 
										<< __FILE__ << " " << __LINE__ << std::endl;
				return;
			}
			generateListViewItem_(rep);

			// update the view
			listview->triggerUpdate();
		}

		void GeometricControl::removeRepresentation(Representation& rep)
			throw()
		{
			if (!representation_to_item_.has(&rep)) return;

			removeItem_(representation_to_item_[&rep], true);
			representation_to_item_.erase(&rep);		
		}

		void GeometricControl::updateRepresentation(Representation& rep)
			throw()
		{
			const HashMap<Representation*, SelectableListViewItem*>::Iterator to_find = 
				representation_to_item_.find(&rep);

			if (to_find == representation_to_item_.end()) return;

			SelectableListViewItem* const item = to_find->second;

			// prevent flickering in GeometricControl, e.g. while a simulation is running
			// so only update if something changed
			String new_text = rep.getName();
			bool changed_content = item->text(0).ascii() != new_text;
			if (changed_content) item->setText(0, new_text.c_str());

			new_text = rep.getColoringName();
			changed_content |= item->text(1).ascii() != new_text;
			if (changed_content) item->setText(1, new_text.c_str());

			new_text = rep.getProperties();
			changed_content |= item->text(2).ascii() != new_text;
			if (changed_content) item->setText(2, new_text.c_str());

			if (rep.isHidden() == item->isOn())
			{
				changed_content = true;
				item->setOn(!rep.isHidden());
			}

			if (changed_content) listview->triggerUpdate();
		}


		void GeometricControl::onNotify(Message *message)
			throw()
		{
		#ifdef BALL_VIEW_DEBUG
			Log.error() << "GeometricControl " << this << " onNotify " << message << std::endl;
		#endif

			GenericControl::onNotify(message);

			if (RTTI::isKindOf<TransformationMessage> (*message))
			{
				moveItems(((TransformationMessage*)message)->getMatrix());
				return;
			}

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
				case RepresentationMessage::ADD_TO_GEOMETRIC_CONTROL:
					addRepresentation(*rep);
					return;
			
				case RepresentationMessage::REMOVE:
					removeRepresentation(*rep);
					return;

				case RepresentationMessage::UPDATE:
					updateRepresentation(*rep);
					return;

				default:
					Log.error() << "Unknown Type of RepresentationMessage in " << __FILE__ << __LINE__ << std::endl;
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

			insertContextMenuEntry("Select Atoms", this, SLOT(selectAtoms()), 25);

			context_menu_.insertSeparator();

			// This is used to provide the coloring for meshes...
			insertContextMenuEntry("Modifiy Surface", modify_surface_dialog_, SLOT(show()), 30);	
			if (!isSurfaceModel(rep.getModelType()))
			{
				context_menu_.setItemEnabled(30, false);
				context_menu_.setItemEnabled(34, false);
			}

			context_menu_.insertSeparator();
			context_menu_.insertItem("Clipping Plane", &clipping_plane_context_menu_, 40);
			if (rep.getModelType() == MODEL_CLIPPING_PLANE)
			{
				context_menu_.setItemEnabled(5, false);
				context_menu_.setItemEnabled(20, false);
			}
			else
			{
				context_menu_.setItemEnabled(40, false);
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

		void GeometricControl::generateListViewItem_(Representation& rep)
			throw()
		{
			if (!getMainControl()->getPrimitiveManager().has(rep)) return;

			// create a new list item
			SelectableListViewItem* new_item = new SelectableListViewItem(listview, 
																								rep.getName().c_str(), &rep, *this);

			CHECK_PTR(new_item);

			representation_to_item_[&rep] = new_item;
		}


		void GeometricControl::deleteRepresentation_()
		{
			if (getSelection().size() == 0) return;

			if (creating_representations_) 
			{
				setStatusbarText("Can not modify representations, while creating a new one!", true);
				return;
			}

			List<Representation*> reps = getSelection();
			List<Representation*>::Iterator it = reps.begin();
			for (; it != reps.end(); ++it)
			{
				getMainControl()->remove(**it);
			}

			setStatusbarText("Deleted representation.");
		}

		void GeometricControl::selectedRepresentation(Representation& representation, bool state)
		{
			if (state != representation.isHidden()) return;

			if (!state)
			{
				setStatusbarText("Hide representation.");
			}
			else
			{
				setStatusbarText("Show representation.");
			}
				
			representation.setHidden(!representation.isHidden());

			if (representation.needsUpdate()) 
			{
				representation.update(true);
			}
		}


		void GeometricControl::onContextMenu_(QListViewItem* item,  const QPoint& point, int /* column */)
		{
			if (item == 0) return;

			if (getMainControl()->compositesAreLocked() ||
					creating_representations_)
			{
				setStatusbarText("No changes to representations allowed, while simulation is running or creating new representations!", true);
				return;
			}

			// clear the context menu
			context_menu_.clear();

			// get Representation address
			Representation* const rep = getRepresentation(*item);
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
			modify_surface_dialog_->setRepresentation(rep);

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

				const AtomContainer* const ac = dynamic_cast<const AtomContainer*>(c_ptr);
				if (ac != 0)
				{
					name = ac->getProperty("FROM_FILE").getString() + "->" + name;
				}
				
				name.trimRight("->");

				if (rep->getComposites().size() > 1) name += "...";

				setStatusbarText("Representation from " + name);
			}
		}


		List<Representation*> GeometricControl::getSelection() const
			throw()
		{
			List<Representation*> selection;
			ItemList selected = ((GeometricControl*)this)->getSelectedItems();
			for (ItemList::Iterator it = selected.begin(); it != selected.end(); ++it)
			{
				selection.push_back(((GeometricControl*) this)->getRepresentation(**it));
			}
			return selection;
		}


		void GeometricControl::checkMenu(MainControl& main_control)
			throw()
		{
			ItemList item_list = getSelectedItems(); 
			if (item_list.size() > 0 && 
					!main_control.compositesAreLocked() &&
					!main_control.updateOfRepresentationRunning()
					) 
			{
				main_control.setDeleteEntryEnabled(true);
			}
		}

		void GeometricControl::focusRepresentation()
		{
			if (getSelectedItems().size() != 1) return;
			Representation* const rep = *getSelection().begin();
			
			GeometricCenterProcessor centerp;
			BoundingBoxProcessor bbox;
			centerp.start();
			bbox.start();

			Vector3 center;
			List<GeometricObject*>::Iterator it = rep->getGeometricObjects().begin();
			for (; it != rep->getGeometricObjects().end(); it++)
			{
				const GeometricObject& go = **it;

				// cant use Vertex or Vertex2 here, no idea why
				if (RTTI::isKindOf<Vertex2>(go))
				{
					const Vertex2& v = *dynamic_cast<const Vertex2*>(&go);
					center = (v.getVertex1() + (v.getVertex2() - v.getVertex1()) / 2.0);

					bbox.operator()(v.getVertex1());
					bbox.operator()(v.getVertex2());
				}
				else if (RTTI::isKindOf<Vertex>(go))
				{
					const Vertex& v = *dynamic_cast<const Vertex*>(&go);
					center = v.getVertex();

					bbox.operator()(v.getVertex());
				}
				else if (RTTI::isKindOf<SimpleBox3>(go))
				{
					const SimpleBox3& b = reinterpret_cast<const SimpleBox3&>(go);
					center = b.a + (b.b - b.a) /2;

					bbox.operator()(b.a);
					bbox.operator()(b.b);
				}
				else if (RTTI::isKindOf<Sphere>(go))
				{
					const Sphere& s = reinterpret_cast<const Sphere&>(go);
					center = s.getPosition();

					bbox.operator()(s.getPosition());
				}
				else if (RTTI::isKindOf<Disc>(go))
				{
					const Disc& d = reinterpret_cast<const Disc&>(go);
					center = d.getCircle().p;

					bbox.operator()(d.getCircle().p);
				}
				else if (RTTI::isKindOf<Mesh>(go))
				{
					const Mesh& mesh = reinterpret_cast<const Mesh&>(go);

					for (Size index = 0; index < mesh.vertex.size(); ++index)
					{
						centerp.operator()(mesh.vertex[index]);
						bbox.operator()(mesh.vertex[index]);
					}
					continue;
				}
				else if (RTTI::isKindOf<BALL::VIEW::Box>(go))
				{
					const BALL::VIEW::Box& box = reinterpret_cast<const BALL::VIEW::Box&>(go);
 					centerp.operator()(box.getPoint() + box.getHeightVector() * 0.5 + box.getRightVector() * 0.5);
					bbox.operator()(box.getPoint());
					bbox.operator()(box.getPoint() + box.getHeightVector());
					bbox.operator()(box.getPoint() + box.getRightVector());
					bbox.operator()(box.getPoint() + box.getDiagonalVector());
				}
				else if (RTTI::isKindOf<BALL::VIEW::Label>(go))
				{
					// do nothing
				}
				else
				{
					Log.error() << "Unknown geometric object: " << typeid(go).name() 
											<< "in " << __FILE__ << __LINE__ << std::endl;
					continue;
				}

				centerp.operator() (center);
			}

			centerp.finish();
			bbox.finish();
			
			Vector3 vwp = centerp.getCenter();
			const float view_distance = (bbox.getUpper() - bbox.getLower()).getLength() - vwp.z + 3;

			// update scene
			SceneMessage * const scene_message = new SceneMessage(SceneMessage::UPDATE_CAMERA);
			scene_message->getStage().getCamera().setLookAtPosition(vwp);
			vwp.z += view_distance;
			scene_message->getStage().getCamera().setViewPoint(vwp);
			notify_(scene_message);
		}

		void GeometricControl::initializeWidget(MainControl& main_control)
			throw()
		{
			GenericControl::initializeWidget(main_control);
		}

		void GeometricControl::finalizeWidget(MainControl& main_control)
			throw()
		{
			GenericControl::finalizeWidget(main_control);
		}

		void GeometricControl::moveClippingPlane()
		{
			getMainControl()->clearSelection();

			SceneMessage* msg = new SceneMessage(SceneMessage::ENTER_MOVE_MODE);
			notify_(msg);
		}

		void GeometricControl::flipClippingPlane()
		{
			if (!RTTI::isKindOf<ClippingPlane>(*context_representation_)) return;
			ClippingPlane& plane = *(ClippingPlane*) context_representation_;

			plane.setX(-plane.getX());
			plane.setY(-plane.getY());
			plane.setZ(-plane.getZ());
			plane.setD(-plane.getD());

			RepresentationMessage* msg = 
				new RepresentationMessage(*context_representation_, RepresentationMessage::UPDATE);
			notify_(msg);
		}

		void GeometricControl::setClippingPlaneX()
		{
			if (Scene::getInstance(0) == 0) return;
			setClippingPlane_(Scene::getInstance(0)->getStage()->getCamera().getLookUpVector());
		}

		void GeometricControl::setClippingPlaneY()
		{
			if (Scene::getInstance(0) == 0) return;
			setClippingPlane_(Scene::getInstance(0)->getStage()->getCamera().getRightVector());
		}

		void GeometricControl::setClippingPlaneZ()
		{
			if (Scene::getInstance(0) == 0) return;
			setClippingPlane_(Scene::getInstance(0)->getStage()->getCamera().getViewVector());
		}

		void GeometricControl::setClippingPlane_(const Vector3& n)
		{
			if (!RTTI::isKindOf<ClippingPlane>(*context_representation_)) return;
			ClippingPlane& plane = *(ClippingPlane*) context_representation_;

			plane.setX(n.x);
			plane.setY(n.y);
			plane.setZ(n.z);
			plane.setD(1);

			RepresentationMessage* msg = 
				new RepresentationMessage(*context_representation_, RepresentationMessage::UPDATE);
			notify_(msg);
		}


		void GeometricControl::moveItems(const Matrix4x4& m)
			throw()
		{
			List<Representation*> reps = getSelection();
			List<Representation*>::Iterator it = reps.begin();

			for (; it != reps.end(); it++)
			{
				if (!RTTI::isKindOf<ClippingPlane>(**it)) return;
				ClippingPlane& plane = *(ClippingPlane*) *it ;

				Vector3 n(plane.getX(),
									plane.getY(),
									plane.getZ());
				float d = plane.getD();

				if (m.m14 == 0 && m.m24 == 0 && m.m34 == 0)
				{
					if (n.getSquareLength() > 0)
					{
						n.normalize();
					}
					n = m * n;

					plane.setX(n.x);
					plane.setX(n.y);
					plane.setX(n.z);
				}
				else
				{
					Vector3 t(-m.m14, -m.m24, -m.m34);
					plane.setD(d + t * n);
				}

				RepresentationMessage* msg = 
					new RepresentationMessage((**it), RepresentationMessage::UPDATE);
				notify_(msg);
			}
		}

		void GeometricControl::selectAtoms()
		{
			if (context_representation_ == 0) return;

			Representation::CompositeSet::Iterator it = context_representation_->getComposites().begin();
			for (; +it; ++it)
			{
				getMainControl()->selectCompositeRecursive((Composite*)*it, false);
			}

			NewSelectionMessage* msg = new NewSelectionMessage();
			notify_(msg);

			context_representation_->update(false);
		}

		void GeometricControl::selectClipRepresentations()
		{
			if (context_representation_ == 0) return;
			ClippingPlane& plane = *(ClippingPlane*) context_representation_;

			ClippingDialog dialog;
			dialog.setClippingPlane(&plane);
			dialog.exec();

			SceneMessage* msg = new SceneMessage(SceneMessage::REDRAW);
			notify_(msg);
		}

			
	
	} // namespace VIEW
} // namespace BALL
