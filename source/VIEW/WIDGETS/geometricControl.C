// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: geometricControl.C,v 1.73.4.24 2005/06/12 17:38:47 amoll Exp $
//

#include <BALL/VIEW/WIDGETS/geometricControl.h>
#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/KERNEL/representation.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/VIEW/KERNEL/clippingPlane.h>

#include <BALL/VIEW/DIALOGS/displayProperties.h>
#include <BALL/VIEW/DIALOGS/modifySurfaceDialog.h>
#include <BALL/VIEW/DIALOGS/clippingDialog.h>

#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qtooltip.h> 

#include <BALL/MATHS/matrix44.h>
#include <BALL/MATHS/analyticalGeometry.h>

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
				clipping_plane_(0),
				control_reference_(control),
				ignore_change_(false)
		{
			setText(0, text);
			if (representation_ != 0)
			{
				setText(1, representation->getColoringName().c_str());
				setText(2, representation->getProperties().c_str());
				setOn(!representation->isHidden());
			}
		}

		void GeometricControl::SelectableListViewItem::stateChange(bool state)
			throw()
		{
			if (ignore_change_)
			{
				ignore_change_ = false;
				return;
			}

			if (getClippingPlane() != 0)
			{
				getClippingPlane()->setActive(!getClippingPlane()->isActive());
				VIEW::getMainControl()->redrawAllRepresentations();
				return;
			}

			if (representation_ == 0) return;

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
				moveItems(*(TransformationMessage*)message);
				return;
			}

			if (RTTI::isKindOf<SyncClippingPlanesMessage>(*message))
			{
				updateClippingPlanes();
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

		void GeometricControl::buildContextMenu(SelectableListViewItem* item)
			throw()
		{
			Representation* rep = item->getRepresentation();
			ClippingPlane* plane = item->getClippingPlane();

			insertContextMenuEntry("Focus", this, SLOT(focusRepresentation()), 5);
			insertContextMenuEntry("Delete", this, SLOT(deleteCurrentItems()), 10);
			insertContextMenuEntry("Select Atoms", this, SLOT(selectAtoms()), 25);
			insertContextMenuEntry("Modify Model", this, SLOT(modifyRepresentation_()), 20);	
			insertContextMenuEntry("Modifiy Surface", modify_surface_dialog_, SLOT(show()), 30);	
			context_menu_.insertSeparator();
			context_menu_.insertItem("Clipping Plane", &clipping_plane_context_menu_, 40);

			if (plane != 0)
			{
				context_menu_.setItemEnabled(5, false); 
				context_menu_.setItemEnabled(20, false); 
				context_menu_.setItemEnabled(25, false); 
				context_menu_.setItemEnabled(30, false); 
				context_menu_.setItemEnabled(40, true); 
				return;
			}

			context_menu_.setItemEnabled(40, false); 

			if (getSelectedItems().size() != 1)
			{
				context_menu_.setItemEnabled(5, false); 
			}

			if (getSelectedItems().size() > 1 ||
					rep->getModelType() >= MODEL_LABEL)
			{
				context_menu_.setItemEnabled(20, false); 
			}

			if (!isSurfaceModel(rep->getModelType()))
			{
				context_menu_.setItemEnabled(30, false);
			}
			else
			{
				modify_surface_dialog_->setRepresentation(rep);
			}
		}

		void GeometricControl::insertContextMenuEntry(const String& name, const QObject* receiver, const char* slot, int entry_ID, int accel)
			throw()
		{
			context_menu_.insertItem(name.c_str(), receiver, slot, accel, entry_ID);
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


		void GeometricControl::deleteCurrentItems()
			throw()
		{
			if (getMainControl()->getPrimitiveManager().updateRunning())
			{
				setStatusbarText("Could not delete Representation while update is running!", true);
				return;
			}

			ItemList items = getSelectedItems();
			if (items.size() == 0) return;

			if (creating_representations_) 
			{
				setStatusbarText("Can not modify representations, while creating a new one!", true);
				return;
			}

			ItemList::Iterator it = items.begin();
			for (; it != items.end(); ++it)
			{
				SelectableListViewItem* item = (SelectableListViewItem*) *it;
				Representation* rep = item->getRepresentation();
				if (rep != 0) 
				{
					getMainControl()->remove(*rep);
					continue;
				}

				ClippingPlane* plane = item->getClippingPlane();
				getMainControl()->getPrimitiveManager().removeClippingPlane(plane);
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

			// create the context menu
			context_representation_ = ((SelectableListViewItem*)item)->getRepresentation();
			context_plane_ = 					((SelectableListViewItem*)item)->getClippingPlane();
			context_item_ = item;

			buildContextMenu((SelectableListViewItem*)item);

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

			context_representation_ = 0;
			context_plane_ 					= 0;

			QListViewItem* item = 0;
			QListViewItemIterator it(listview);
			for (; it.current(); ++it)
			{
				if (it.current()->isSelected())
				{
					item = it.current();
					break;
				}
			}

			if (item == 0) return;

			Representation* rep = ((SelectableListViewItem*)item)->getRepresentation();
			modify_surface_dialog_->setRepresentation(rep);
			if (rep == 0) return; 

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


		List<Representation*> GeometricControl::getHighlightedRepresentations() const
			throw()
		{
			List<Representation*> selection;
			ItemList selected = ((GeometricControl*)this)->getSelectedItems();
			for (ItemList::Iterator it = selected.begin(); it != selected.end(); ++it)
			{
				Representation* rep = ((SelectableListViewItem*) *it)->getRepresentation();
				if (rep != 0) selection.push_back(rep);
			}
			return selection;
		}


		void GeometricControl::checkMenu(MainControl& main_control)
			throw()
		{
			if (!main_control.compositesAreLocked() &&
					!main_control.updateOfRepresentationRunning() &&
					getSelectedItems().size() > 0)
			{
				main_control.setDeleteEntryEnabled(true);
			}
		}

		void GeometricControl::focusRepresentation()
		{
			List<Representation*> reps = getHighlightedRepresentations();
			if (reps.size() != 1) return;
			
			getMainControl()->getPrimitiveManager().focusRepresentation(**reps.begin());
		}

		void GeometricControl::initializeWidget(MainControl& main_control)
			throw()
		{
			GenericControl::initializeWidget(main_control);

			menu_clipping_plane_id_ = insertMenuEntry(MainControl::DISPLAY, 
																		"New Clipping Plane", this, SLOT(createNewClippingPlane()));   
			setMenuHint("Add an OpenGL Clipping Plane to the Scene");
		}

		void GeometricControl::moveClippingPlane()
		{
			getMainControl()->clearSelection();

			SceneMessage* msg = new SceneMessage(SceneMessage::ENTER_MOVE_MODE);
			notify_(msg);
		}

		void GeometricControl::flipClippingPlane()
		{
			if (context_plane_ == 0) return;

			getMainControl()->redrawAllRepresentations();
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
			if (context_plane_ == 0) return;

			context_plane_->setNormal(n);
			getMainControl()->redrawAllRepresentations();
		}


		void GeometricControl::moveItems(const TransformationMessage& msg)
			throw()
		{
			const Matrix4x4& m = msg.getMatrix();

			if (m.isIdentity()) return;

			ItemList items = getSelectedItems();
			ItemList::Iterator it = items.begin();

			Scene* scene = Scene::getInstance(0);

			if (scene == 0) return;


			for (; it != items.end(); it++)
			{
				ClippingPlane* plane = ((SelectableListViewItem*) *it)->getClippingPlane();
				if (plane == 0) continue;

				Vector3 n(plane->getNormal());

				if (m.m14 == 0 && m.m24 == 0 && m.m34 == 0)
				{
					Vector3 v = n;
					v = m * v;

					if (!Maths::isZero(v.getSquareLength()))
					{
						v.normalize();
					}

					plane->setNormal(v);
				}
				else
				{
					Vector3 t(-m.m14, -m.m24, -m.m34);
					plane->setPoint(plane->getPoint() - t);
				}

				getMainControl()->redrawAllRepresentations();
			}
		}


		void GeometricControl::selectAtoms()
		{
			if (context_representation_ == 0) return;

			Representation::CompositeSet::ConstIterator it = context_representation_->getComposites().begin();
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
			if (context_plane_ == 0) return;

			ClippingDialog dialog;
			dialog.setClippingPlane(context_plane_);
			dialog.exec();
		}

		void GeometricControl::createNewClippingPlane()
		{
			MainControl* mc = getMainControl();
			Scene* scene = Scene::getInstance(0);
			if (mc == 0 || scene == 0) return;

			Camera& camera = scene->getStage()->getCamera();

			ClippingPlane* plane = new ClippingPlane();

			Vector3 vv = camera.getViewVector();
			if (!Maths::isZero(vv.getSquareLength())) vv.normalize();
	
			Vector3 n(vv + -camera.getRightVector() + camera.getLookUpVector());
			if (!Maths::isZero(n.getSquareLength())) n.normalize();
		
			plane->setNormal(n);
			plane->setPoint(camera.getLookAtPosition() + vv * 10);

			PrimitiveManager& pm = mc->getPrimitiveManager();
			PrimitiveManager::RepresentationList::ConstIterator it = pm.getRepresentations().begin();
			for (; it != pm.getRepresentations().end(); it++)
			{
				plane->getRepresentations().insert(*it);
			}
			getMainControl()->getPrimitiveManager().insertClippingPlane(plane);
		}


		void GeometricControl::updateClippingPlanes()
		{
			// put the planes from the PrimitiveManager into a hashset
			HashSet<ClippingPlane*> to_have_planes;

			const vector<ClippingPlane*>& vc = getMainControl()->getPrimitiveManager().getClippingPlanes();
			vector<ClippingPlane*>::const_iterator plane_it = vc.begin();
			for (; plane_it != vc.end(); plane_it++)
			{
				to_have_planes.insert(*plane_it);
			}

			// collect all planes, the GeometricControl currently has
			HashMap<ClippingPlane*, SelectableListViewItem*> plane_map;

			QListViewItemIterator it(listview);
			for (; it.current(); ++it)
			{
				SelectableListViewItem* item = (SelectableListViewItem*)it.current();
				if (item->getClippingPlane() != 0)
				{
					plane_map[item->getClippingPlane()] = item;
				}
			}

			HashSet<ClippingPlane*>::Iterator cit = to_have_planes.begin();
			for (; +cit; ++cit)
			{
				if (!plane_map.has(*cit))
				{
					SelectableListViewItem* new_item = new SelectableListViewItem(listview, "ClippingPlane", 0, *this);
					new_item->setOn((**cit).isActive());
					new_item->setClippingPlane(*cit);
				}
			}

			HashMap<ClippingPlane*, SelectableListViewItem*>::Iterator mit = plane_map.begin();
			for (; +mit; ++mit)
			{
				if (!to_have_planes.has(mit->first)) removeItem_(mit->second);
			}
		}
	
	} // namespace VIEW
} // namespace BALL
