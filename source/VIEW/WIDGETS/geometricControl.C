/* TRANSLATOR  BALL::QSAR

		Necessary for lupdate.
*/

// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/WIDGETS/geometricControl.h>
#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/KERNEL/representation.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/VIEW/KERNEL/clippingPlane.h>

#include <BALL/VIEW/DIALOGS/displayProperties.h>
#include <BALL/VIEW/DIALOGS/modifyRepresentationDialog.h>
#include <BALL/VIEW/DIALOGS/clippingDialog.h>
#include <BALL/VIEW/DIALOGS/setClippingPlane.h>

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QToolTip>
#include <QtWidgets/QInputDialog>

#include <BALL/MATHS/matrix44.h>
#include <BALL/MATHS/analyticalGeometry.h>
#include <BALL/VIEW/PRIMITIVES/gridVisualisation.h>

using std::endl;

namespace BALL
{
	namespace VIEW
	{

		GeometricControl::GeometricControl(QWidget* parent, const char* name)
			:	GenericControl(parent, name),
				context_menu_(this),
				clipping_plane_context_menu_(this),
				context_representation_(0),
				modify_rep_dialog_(new ModifyRepresentationDialog(this, ((String)tr("ModifyRepresentationDialog")).c_str())),
				creating_representations_(false),
				ignore_change_(false)
		{
		#ifdef BALL_VIEW_DEBUG
			Log.error() << "new GeometricControl " << this << std::endl;
		#endif

			listview->setObjectName("MolecularControlList");
			listview->headerItem()->setText(0, tr("[visible] Index"));
			listview->headerItem()->setText(1, tr("Model"));
			listview->headerItem()->setText(2, tr("Built from"));
			listview->headerItem()->setText(3, tr("Color"));
      listview->headerItem()->setText(4, tr("Name"));
			listview->headerItem()->setText(5, tr("Properties"));
			listview->setSortingEnabled(false);

			String txt = String(tr("List of the representations")) + ": \n" +
									"1." + (String)tr("column") + ": " + (String)tr("visible flag") + "\n" +
									"2." + (String)tr("column") + ": " + (String)tr("model type") + "\n" +
									"3." + (String)tr("column") + ": " + (String)tr("name of the molecular entity the model was created from") + "\n" +
									"4." + (String)tr("column") + ": " + (String)tr("used coloring method") + "\n" +
									"5." + (String)tr("column") + ": " + (String)tr("name of the representation") + "\n" +
									"6." + (String)tr("column") + ": " + (String)tr("number of used molecular entities, number of geometric objects") + ". ";

			listview->setToolTip(txt.c_str());

			registerWidget(this);

			clipping_plane_context_menu_.addAction(tr("Hide/Show"), this, SLOT(hideShowClippingPlane()));
			clipping_plane_context_menu_.addAction(tr("Flip"), this, SLOT(flipClippingPlane()));
			clipping_plane_context_menu_.addAction(tr("Set to position..."), this, SLOT(setClippingPosition()));
			clipping_plane_context_menu_.addAction(tr("Set to x axis"), this, SLOT(setClippingPlaneX()));
			clipping_plane_context_menu_.addAction(tr("Set to y axis"), this, SLOT(setClippingPlaneY()));
			clipping_plane_context_menu_.addAction(tr("Set to z axis"), this, SLOT(setClippingPlaneZ()));
			clipping_plane_context_menu_.addAction(tr("Clip <-> Cap"), this, SLOT(flipClippingCapping()));
			clipping_plane_context_menu_.addAction(tr("Select Representations"), this, SLOT(selectClipRepresentations()));
		}

		GeometricControl::~GeometricControl()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "Destructing object " << this << " of class GeometricControl" << std::endl;
			#endif
		}

		void GeometricControl::addRepresentation(Representation& rep)
		{
			if (representation_to_item_.has(&rep))
			{
				Log.error() << (String)tr("Tried to add an already inserted Representation in ")
										<< __FILE__ << " " << __LINE__ << std::endl;
				return;
			}
			generateListViewItem_(rep);
		}

		void GeometricControl::removeRepresentation(Representation& rep)
		{
			if (!representation_to_item_.has(&rep)) return;

			delete representation_to_item_[&rep];

			item_to_representation_.erase(representation_to_item_[&rep]);
			representation_to_item_.erase(&rep);
		}

		void GeometricControl::updateRepresentation(Representation& rep, bool force)
		{
			if (!force && getMainControl()->isBusy()) return;

			const HashMap<Representation*, QTreeWidgetItem*>::Iterator to_find =
				representation_to_item_.find(&rep);

			if (to_find == representation_to_item_.end()) return;

			QTreeWidgetItem* const item = to_find->second;

			// prevent flickering in GeometricControl, e.g. while a simulation is running
			// so only update if something changed
			const ModelInformation& mi = getMainControl()->getModelInformation();

			String new_text1 = mi.getModelName(rep.getModelType());
			bool changed_content = ascii(item->text(1)) != new_text1;

			String new_text2 = rep.getCompositeName();
			changed_content |= ascii(item->text(2)) != new_text2;

			String new_text3 = mi.getColoringName(rep.getColoringMethod());
			changed_content |= ascii(item->text(3)) != new_text3;

      String new_text4 = rep.getName();
			changed_content |= ascii(item->text(4)) != new_text4;

			String new_text5 = rep.getProperties();
			changed_content |= ascii(item->text(5)) != new_text5;

			if (changed_content)
			{
				ignore_change_ = true;
				item->setText(1, new_text1.c_str());
				item->setText(2, new_text2.c_str());
				item->setText(3, new_text3.c_str());
				item->setText(4, new_text4.c_str());
				item->setText(5, new_text5.c_str());
			}

			if (rep.isHidden() == (item->checkState(0) == Qt::Checked))
			{
				ignore_change_ = true;
				if (rep.isHidden()) item->setCheckState(0, Qt::Unchecked);
				else 							  item->setCheckState(0, Qt::Checked);
			}

			ignore_change_ = false;
		}


		void GeometricControl::onNotify(Message *message)
		{
		#ifdef BALL_VIEW_DEBUG
			Log.error() << "GeometricControl " << this << " onNotify " << message << std::endl;
		#endif

			GenericControl::onNotify(message);

            if (RTTI::isKindOf<TransformationMessage>(message))
			{
				moveItems(*(TransformationMessage*)message);
				return;
			}

            if (RTTI::isKindOf<SyncClippingPlanesMessage>(message))
			{
				updateClippingPlanes();
				return;
			}

            if (!RTTI::isKindOf<RepresentationMessage>(message)) return;

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

				case RepresentationMessage::UPDATE_PROPERTIES:
					updateRepresentation(*rep, true);
					return;

				default:
					Log.error() << (String)tr("Unknown Type of RepresentationMessage in ") << __FILE__ << __LINE__ << std::endl;
			}
		}

		void GeometricControl::buildContextMenu()
		{
			QList<QTreeWidgetItem*> il = getSelectedItems();
			if (il.size() == 0)
			{
				context_menu_.clear();
				return;
			}

			Representation* rep = 0;
			ClippingPlane* plane = 0;

			if (item_to_representation_.has(context_item_)) rep = item_to_representation_[context_item_];
			else if (item_to_plane_.has(context_item_))     plane = item_to_plane_[context_item_];

			context_menu_.clear();
			context_menu_actions_.clear();
			context_menu_.setObjectName("ContextMenu");

			addItem_((String)tr("Delete"), SLOT(deleteCurrentItems()));
			addItem_((String)tr("Focus"), SLOT(focus()));
			addItem_((String)tr("Duplicate"), SLOT(duplicate()));
			addItem_((String)tr("Move"), SLOT(enterMoveMode()));

			if (rep && rep->getModelType() == MODEL_PROXIMITY_LIGHT)
			{
				addItem_((String)tr("Setup Proximity Light"), SLOT(setupProximityLight()));
			}
			else
			{
				Size all_pos = context_menu_actions_.size() - 1;
				context_menu_.addSeparator();
				addItem_((String)tr("Save Surface"), SLOT(saveSurface()));
				addItem_((String)tr("Select Atoms"), SLOT(selectAtoms()));
				addItem_((String)tr("Modify Model"), SLOT(modifyRepresentation_()));
				addItem_((String)tr("Modify Representation"), SLOT(show()), modify_rep_dialog_);
				addItem_((String)tr("Rename"), SLOT(renameRepresentation()));
				context_menu_.addSeparator();
				Size acs = context_menu_actions_.size() - 1;
				context_menu_actions_.push_back(context_menu_.addMenu(&clipping_plane_context_menu_));
				context_menu_actions_[acs + 1]->setText(tr("Clipping Plane"));

				// planes ->
				if (plane != 0)
				{
					context_menu_actions_[acs + 1]->setEnabled(true);

					for (Position p = all_pos + 1; p <= acs; p++)
					{
						context_menu_actions_[p]->setEnabled(false);
					}
					return;
				}

				// representations ->
				context_menu_actions_[4]->setEnabled(rep->getModelType() == MODEL_GRID_SLICE);
				const ModelInformation& mi = getMainControl()->getModelInformation();
				context_menu_actions_[all_pos + 1]->setEnabled(mi.isSurfaceModel(rep->getModelType()));
				context_menu_actions_[acs + 1]->setEnabled(false);

				if (getSelectedItems().size() != 1)
				{
					for (Position p = 1; p <= acs; p++)
					{
						context_menu_actions_[p]->setEnabled(false);
					}
				}

				// not modifyable
				if (rep && rep->getModelType() >= MODEL_LABEL)
				{
					context_menu_actions_[6]->setEnabled(false);
				}
			}

			modify_rep_dialog_->setRepresentation(rep);
		}

		void GeometricControl::addItem_(const String& text, const char* member, QWidget* widget)
		{
			if (widget == 0) widget = this;
			context_menu_actions_.push_back(
					context_menu_.addAction(text.c_str(), widget, member));
		}

		void GeometricControl::generateListViewItem_(Representation& rep)
		{
			if (!getMainControl()->getRepresentationManager().has(rep)) return;

			const ModelInformation& mi = getMainControl()->getModelInformation();
			QStringList sl;
			sl << ""
				 << mi.getModelName(rep.getModelType()).c_str()
				 << rep.getCompositeName().c_str()
				 << mi.getColoringName(rep.getColoringMethod()).c_str()
         << rep.getName().c_str()
				 << rep.getProperties().c_str();
			// create a new list item
			QTreeWidgetItem* new_item = new QTreeWidgetItem(listview, sl);

			representation_to_item_[&rep] = new_item;
			item_to_representation_[new_item] = &rep;

			new_item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
			ignore_change_ = true;
			new_item->setCheckState(0, Qt::Checked);
			ignore_change_ = true;
			if (rep.isHidden()) new_item->setCheckState(0, Qt::Unchecked);
			ignore_change_ = false;

			listview->setItemSelected(new_item, true);
 			deselectOtherControls_();
			updateSelection();
		}

		void GeometricControl::deleteCurrentItems()
		{
			if (getMainControl()->getRepresentationManager().updateRunning() ||
			    creating_representations_)
			{
				setStatusbarText((String)tr("Could not delete Representation while update is running!"), true);
				return;
			}

			ItemList items = getSelectedItems();
			ItemList::iterator it = items.begin();
			for (; it != items.end(); ++it)
			{
				QTreeWidgetItem* item = *it;
				if (item_to_representation_.has(item))
				{
					getMainControl()->remove(*item_to_representation_[item]);
					setStatusbarText((String)tr("Deleted representation."));
					continue;
				}

				ClippingPlane* plane = item_to_plane_[item];
				removeItem_(item);
				item_to_plane_.erase(item);
				plane_to_item_.erase(plane);
				getMainControl()->getRepresentationManager().removeClippingPlane(plane);
				setStatusbarText((String)tr("Deleted Clipping Plane."));
			}
		}

		void GeometricControl::setupProximityLight()
		{
			Log.info() << "I am sooooo sorry, but this does not exist yet!" << std::endl;
		}

		void GeometricControl::selectedRepresentation(Representation& representation, bool state)
		{
			if (state != representation.isHidden()) return;

			representation.setHidden(!state);
			if (!representation.isHidden())
			{
				representation.update(false);
			}
			else
			{
				notify_(new SceneMessage(SceneMessage::REDRAW));
			}
		}

		void GeometricControl::showGuestContextMenu(const QPoint& pos)
		{
			if (getMainControl()->isBusy())
			{
				setStatusbarText((String)tr("No changes to representations allowed, while simulation is running or creating new representations!"), true);
				return;
			}

			buildContextMenu();
			context_menu_.popup(mapToGlobal(pos));
		}


		void GeometricControl::modifyRepresentation_()
		{
			if (context_representation_ == 0) return;

			notify_(new ShowDisplayPropertiesMessage);
		}

		void GeometricControl::updateSelection()
		{
			GenericControl::updateSelection();

			getSelectedItems();

			if (context_item_ == 0)
			{
				context_representation_ = 0;
				context_plane_          = 0;
				return;
			}

			if (item_to_plane_.has(context_item_))
			{
				context_representation_ = 0;
				context_plane_ 					= item_to_plane_[context_item_];
			}
			else
			{
				context_representation_ = item_to_representation_[context_item_];
				context_plane_ 					= 0;
			}

			// less to type...
			Representation* rep = context_representation_;

			modify_rep_dialog_->setRepresentation(rep);
			notify_(new RepresentationMessage(*rep, RepresentationMessage::SELECTED));

			if (rep == 0 || !getMainControl()->getRepresentationManager().has(*rep))
			{
				return;
			}

			if (rep->getComposites().size() > 0)
			{
				String name;
				const Composite* c_ptr = *rep->getComposites().begin();

				while (!c_ptr->isRoot())
				{
                    if (RTTI::isKindOf<AtomContainer> (c_ptr))
					{
						name = ((const AtomContainer*) c_ptr)->getName() + "->" + name;
					}
                    else if (RTTI::isKindOf<Atom> (c_ptr))
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

				setStatusbarText((String)tr("Representation from ") + name);
			}
		}


		list<Representation*> GeometricControl::getHighlightedRepresentations() const
		{
			list<Representation*> selection;
			QList<QTreeWidgetItem*> selected_items = listview->selectedItems();

			QList<QTreeWidgetItem*>::const_iterator sel_it = selected_items.begin();

			for (; sel_it != selected_items.end(); ++sel_it)
			{
				if (item_to_representation_.has(*sel_it))
					selection.push_back(item_to_representation_[*sel_it]);
			}

			return selection;
		}


		void GeometricControl::checkMenu(MainControl& main_control)
		{
			bool busy = main_control.isBusy();
			if (!busy && getSelectedItems().size() > 0)
			{
				main_control.setDeleteEntryEnabled(true);
			}

			if (menu_clipping_plane_)
				menu_clipping_plane_->setEnabled(!busy);
			if (menu_load_surface_)
				menu_load_surface_->setEnabled(!busy);
			if (modify_surface_)
				modify_surface_->setEnabled(!busy && getSelectedItems().size());
		}

		void GeometricControl::focus()
		{
			list<Representation*> reps = getHighlightedRepresentations();
			if (reps.size() == 1)
			{
				getMainControl()->getRepresentationManager().focusRepresentation(**reps.begin());
				return;
			}

			if (context_plane_ != 0)
			{
				Vector3 look_at_point = context_plane_->getPoint();
				Vector3 view_vector = context_plane_->getNormal() * 25.;
				Vector3 up_vector = getNormal(view_vector);
				SceneMessage *scene_message = new SceneMessage(SceneMessage::UPDATE_CAMERA);
				scene_message->getStage().getCamera().setLookAtPosition(look_at_point);
				scene_message->getStage().getCamera().setViewPoint(look_at_point- view_vector);
				scene_message->getStage().getCamera().setLookUpVector(up_vector);
				notify_(*scene_message);
			}
		}

		void GeometricControl::initializeWidget(MainControl& main_control)
		{
			GenericControl::initializeWidget(main_control);

			menu_clipping_plane_ = insertMenuEntry(MainControl::DISPLAY_CREATE, tr("Clipping Plane"),
			                                       this, SLOT(createNewClippingPlane()), "Shortcut|Display|Create|ClippingPlane",
																						 QKeySequence(), tr("Add an OpenGL Clipping Plane to the Scene"),
																						 UIOperationMode::MODE_ADVANCED);
			setMenuHelp(menu_clipping_plane_, "geometricControl.html#clipping_planes");

			menu_load_surface_ = insertMenuEntry(MainControl::FILE_OPEN, tr("Surface"), this,
																					 SLOT(loadSurface()), "Shortcut|Display|Create|Surface",
																					 QKeySequence(), tr(""), UIOperationMode::MODE_ADVANCED);

			modify_surface_ = insertMenuEntry(MainControl::DISPLAY, tr("Modify Representation"), modify_rep_dialog_,
			                                  SLOT(show()), "Shortcut|Display|Modify Representation", QKeySequence(),
																				tr(""), UIOperationMode::MODE_ADVANCED);

			registerForHelpSystem(this, "geometricControl.html");
		}

		void GeometricControl::enterMoveMode()
		{
			getMainControl()->clearSelection();
			selectAtoms();

			notify_(new SceneMessage(SceneMessage::ENTER_MOVE_MODE));
		}

		void GeometricControl::flipClippingPlane()
		{
			if (context_plane_ == 0) return;

			context_plane_->flip();
			getMainControl()->redrawAllRepresentations();
		}

		void GeometricControl::setClippingPosition()
		{
			updateSelection();

			if (context_plane_ == 0) return;
			SetClippingPlane sp(this, ((String)tr("Set plane")).c_str(), context_plane_);
			sp.exec();

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
			Vector3 v = Scene::getInstance(0)->getStage()->getCamera().getViewVector();
			if (Maths::isZero(v.getSquareLength())) return;
			v.normalize();
			setClippingPlane_(v);
		}

		void GeometricControl::setClippingPlane_(const Vector3& n)
		{
			updateSelection();

			if (context_plane_ == 0) return;

			context_plane_->setNormal(n);
			getMainControl()->redrawAllRepresentations();
		}


		void GeometricControl::moveItems(const TransformationMessage& msg)
		{
			const Matrix4x4& m = msg.getMatrix();

			if (m.isIdentity()) return;

			ItemList items = getSelectedItems();
			ItemList::Iterator it = items.begin();

			Scene* scene = Scene::getInstance(0);
			if (scene == 0) return;

			bool changed = false;
			for (; it != items.end(); it++)
			{
				ClippingPlane* plane = 0;
				GridVisualisation* slice = 0;
				Vector3 n;

				if (item_to_plane_.has(*it)) plane = item_to_plane_[*it];

				if (plane == 0)
				{
					if (!item_to_representation_.has(*it)) continue;
					const Representation* rep = item_to_representation_[*it];
					if (rep->getModelType() != MODEL_GRID_SLICE) continue;
					slice = (GridVisualisation*) *rep->getGeometricObjects().begin();
				}

				if (plane != 0) n = plane->getNormal();
				else            n = slice->getNormal();

				if (m.m14 == 0 && m.m24 == 0 && m.m34 == 0)
				{
					Vector3 v = n;
					v = m * v;

					if (!Maths::isZero(v.getSquareLength()))
					{
						v.normalize();
					}

					if (plane != 0) plane->setNormal(v);
					else            slice->setNormal(v);
				}
				else
				{
					Vector3 t(-m.m14, -m.m24, -m.m34);
					if (plane != 0) plane->setPoint(plane->getPoint() - t);
					else            slice->setPoint(slice->getPoint() - t);
				}

				changed = true;
			}

			if (changed) getMainControl()->redrawAllRepresentations();
		}


		void GeometricControl::selectAtoms()
		{
			if (context_representation_ == 0) return;

			Representation& rep = *context_representation_;

			list<const Composite*>::const_iterator it = rep.getComposites().begin();
			for (; it != rep.getComposites().end(); ++it)
			{
				getMainControl()->selectCompositeRecursive((Composite*)*it, false);
			}

			notify_(new NewSelectionMessage);

		  rep.update(false);
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

			RepresentationManager& pm = mc->getRepresentationManager();
			RepresentationList::const_iterator it = pm.getRepresentations().begin();
			for (; it != pm.getRepresentations().end(); it++)
			{
				plane->getRepresentations().insert(*it);
			}
			getMainControl()->getRepresentationManager().insertClippingPlane(plane);
		}


		void GeometricControl::updateClippingPlanes()
		{
			// put the planes from the RepresentationManager into a hashset
			HashSet<ClippingPlane*> to_have_planes;

			const vector<ClippingPlane*>& vc = getMainControl()->getRepresentationManager().getClippingPlanes();
			vector<ClippingPlane*>::const_iterator plane_it = vc.begin();
			for (; plane_it != vc.end(); plane_it++)
			{
				to_have_planes.insert(*plane_it);
			}

			context_plane_ = 0;

			HashSet<ClippingPlane*>::Iterator cit = to_have_planes.begin();
			for (; +cit; ++cit)
			{
				ClippingPlane* plane = *cit;
				if (!plane_to_item_.has(plane))
				{
					QStringList sl;
					sl << "";
					if (plane->cappingEnabled())
					{
						sl << tr("CappingPlane");
					}
					else
					{
						sl << tr("ClippingPlane");
					}
					if (plane->isHidden()) sl << "[" << tr("hidden") << "]";
					sl << " ";
					QTreeWidgetItem* new_item = new QTreeWidgetItem(listview, sl);
					new_item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);

					ignore_change_ = true;
					new_item->setCheckState(0, Qt::Checked);

					if (!plane->isActive())
					{
						ignore_change_ = true;
						new_item->setCheckState(0, Qt::Unchecked);
					}

					item_to_plane_[new_item] = plane;
					plane_to_item_[plane] = new_item;
				}
			}

			HashMap<ClippingPlane*, QTreeWidgetItem*>::Iterator mit = plane_to_item_.begin();
			for (; mit != plane_to_item_.end(); ++mit)
			{
				if (!to_have_planes.has(mit->first)) removeItem_(mit->second);
			}
		}

		void GeometricControl::hideShowClippingPlane()
		{
			if (context_plane_ == 0) return;

			bool hidden = !context_plane_->isHidden();

			context_plane_->setHidden(hidden);
			VIEW::getMainControl()->redrawAllRepresentations();

			if (context_item_ == 0) return;

			context_item_->setText(2, hidden ? "[" + tr("hidden") + "]" : "");
		}

		void GeometricControl::duplicate()
		{
			if (context_representation_ != 0)
			{
				Representation* new_rep = (Representation*) context_representation_->create();
				if (new_rep == 0) return;
				new_rep->setNeedsUpdate(false);

				getMainControl()->insert(*new_rep);
				notify_(new RepresentationMessage(*new_rep, RepresentationMessage::UPDATE));
			}

			if (context_plane_ != 0)
			{
				ClippingPlane* plane = (ClippingPlane*) context_plane_->create();
				if (plane == 0) return;
				getMainControl()->getRepresentationManager().insertClippingPlane(plane);
			}
		}

		GeometricControl::GeometricControl(const GeometricControl& mc)
			: GenericControl(mc)
		{
		}

		void GeometricControl::onItemClicked(QTreeWidgetItem* item, int col)
		{
			if (col != 0) return;

			if (ignore_change_)
			{
				ignore_change_ = false;
				return;
			}

			bool checked = (item->checkState(0) == Qt::Checked);

			if (getMainControl()->isBusy())
			{
				ignore_change_ = true;

				if (checked) item->setCheckState(0, Qt::Unchecked);
				else 				 item->setCheckState(0, Qt::Checked);

				VIEW::getMainControl()->setStatusbarText((String)tr("Cannot switch Representation on/off now!"), true);
				return;
			}

			ClippingPlane* plane = 0;
			Representation* rep  = 0;

			if 			(item_to_plane_.has(item)) 					plane = item_to_plane_[item];
			else if (item_to_representation_.has(item))	rep   = item_to_representation_[item];

			if (rep != 0)
			{
				if (rep->isHidden() == checked)
				{
					selectedRepresentation(*rep, checked);
				}
				return;
			}

			if (plane->isActive() != checked)
			{
				plane->setActive(!plane->isActive());
				getMainControl()->redrawAllRepresentations();
			}
		}

		void GeometricControl::flipClippingCapping()
		{
			if (context_plane_ == 0) return;

			bool capping = !context_plane_->cappingEnabled();

			context_plane_->setCappingEnabled(capping);
			VIEW::getMainControl()->redrawAllRepresentations();

			if (context_item_ == 0) return;

			context_item_->setText(0, capping ? tr("CappingPlane") : tr("ClippingPlane"));
		}

		ModifyRepresentationDialog* GeometricControl::getModifySurfaceDialog()
		{
			return modify_rep_dialog_;
		}

		void GeometricControl::saveSurface()
		{
			if (context_representation_ == 0) return;
			if (context_representation_->getGeometricObjects().size() == 0) return;

			GeometricObject& go = **context_representation_->getGeometricObjects().begin();
			Mesh* mesh = dynamic_cast<Mesh*>(&go);
			if (mesh == 0) return;

			QString qresult = QFileDialog::getSaveFileName(
												0,
												tr("Export Surface"),
												(getWorkingDir() + String(FileSystem::PATH_SEPARATOR) +
												 "surface.dat").c_str(),
												"*.*");

			if (qresult == QString::null) return;

			String result = ascii(qresult);
			mesh->binaryWrite(result);
		}

		void GeometricControl::loadSurface()
		{
			QString qresult = QFileDialog::getOpenFileName(
												0,
												tr("Read Surface"),
												(getWorkingDir() + String(FileSystem::PATH_SEPARATOR)).c_str(),
												"*.*");

			if (qresult == QString::null) return;

			String result = ascii(qresult);
			Mesh* mesh = new Mesh();
			Representation* rep = new Representation();
			list<Composite*> cl = getMainControl()->getMolecularControlSelection();
			list<const Composite*> ccl;
			list<Composite*>::iterator cit = cl.begin();
			for (; cit != cl.end(); cit++)
			{
				ccl.push_back(*cit);
			}
//   			rep->setComposites(ccl);
			rep->insert(*mesh);
			rep->setModelType(MODEL_SE_SURFACE);
			mesh->binaryRead(result);
			getMainControl()->insert(*rep);
			getMainControl()->update(*rep);
		}

		void GeometricControl::renameRepresentation()
		{
			if (context_representation_ == 0) return;

			String name = context_representation_->getName();

			bool ok;
			QString text = QInputDialog::getText(this, tr("Rename a Representation"),
																						tr("Enter new name:"), QLineEdit::Normal,
																						name.c_str(), &ok);
			if (!ok) return;

			name = ascii(text);
			context_representation_->setName(name);
			updateRepresentation(*context_representation_);
		}


	} // namespace VIEW
} // namespace BALL
