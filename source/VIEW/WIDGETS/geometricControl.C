// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: geometricControl.C,v 1.77.2.25 2006/11/13 18:05:59 amoll Exp $
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
#include <QtGui/QFileDialog>

#include <QtGui/qmenubar.h>
#include <QtGui/qtooltip.h> 

#include <BALL/MATHS/matrix44.h>
#include <BALL/MATHS/analyticalGeometry.h>
#include <BALL/VIEW/PRIMITIVES/gridVisualisation.h>

using std::endl;

namespace BALL
{
	namespace VIEW
	{

		GeometricControl::GeometricControl(QWidget* parent, const char* name)
			throw()
			:	GenericControl(parent, name),
				context_menu_(this),
				clipping_plane_context_menu_(this),
				context_representation_(0),
				modify_rep_dialog_(new ModifyRepresentationDialog(this, "ModifyRepresentationDialog")),
				creating_representations_(false),
				ignore_change_(false)
		{
		#ifdef BALL_VIEW_DEBUG
			Log.error() << "new GeometricControl " << this << std::endl;
		#endif
			
			listview->setObjectName("MolecularControlList");
			listview->headerItem()->setText(0, "[visible] Model");
			listview->headerItem()->setText(1, "Color");
			listview->headerItem()->setText(2, "Properties");
			
			String txt = String("List of the representations: \n") +
									"1.column: model type and name of the molecular entity, the model was created from\n" +
									"2.column: used coloring method\n" +
									"3.column: number of used molecular entities, number of geometric objects. ";

			listview->setToolTip(txt.c_str());
			
			registerWidget(this);

			clipping_plane_context_menu_.addAction("Hide/Show", this, SLOT(hideShowClippingPlane()));
			clipping_plane_context_menu_.addAction("Flip", this, SLOT(flipClippingPlane()));	
			clipping_plane_context_menu_.addAction("Set to x axis", this, SLOT(setClippingPlaneX()));	
			clipping_plane_context_menu_.addAction("Set to y axis", this, SLOT(setClippingPlaneY()));	
			clipping_plane_context_menu_.addAction("Set to z axis", this, SLOT(setClippingPlaneZ()));	
			clipping_plane_context_menu_.addAction("Clip <-> Cap", this, SLOT(flipClippingCapping()));	
			clipping_plane_context_menu_.addAction("Select Representations", this, SLOT(selectClipRepresentations()));	
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
		}

		void GeometricControl::removeRepresentation(Representation& rep)
			throw()
		{
			if (!representation_to_item_.has(&rep)) return;

			delete representation_to_item_[&rep];

			item_to_representation_.erase(representation_to_item_[&rep]);
			representation_to_item_.erase(&rep);		
		}

		void GeometricControl::updateRepresentation(Representation& rep)
			throw()
		{
			const HashMap<Representation*, QTreeWidgetItem*>::Iterator to_find = 
				representation_to_item_.find(&rep);

			if (to_find == representation_to_item_.end()) return;

			QTreeWidgetItem* const item = to_find->second;

			// prevent flickering in GeometricControl, e.g. while a simulation is running
			// so only update if something changed
			String new_text1 = rep.getName();
			bool changed_content = ascii(item->text(0)) != new_text1;

			String new_text2 = rep.getColoringName();
			changed_content |= ascii(item->text(1)) != new_text2;

			String new_text3 = rep.getProperties();
			changed_content |= ascii(item->text(2)) != new_text3;

			if (changed_content)
			{
				ignore_change_ = true;
				item->setText(0, new_text1.c_str());
				item->setText(1, new_text2.c_str());
				item->setText(2, new_text3.c_str());
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

			if (!RTTI::isKindOf<RepresentationMessage> (*message)) return;
			
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

		void GeometricControl::buildContextMenu()
			throw()
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

			addItem_("Delete", SLOT(deleteCurrentItems()));
			addItem_("Focus", SLOT(focusRepresentation()));
			addItem_("Duplicate", SLOT(duplicateRepresentation()));
			addItem_("Save Surface", SLOT(saveSurface()));
 			addItem_("Select Atoms", SLOT(selectAtoms()));
			addItem_("Modify Model", SLOT(modifyRepresentation_()));	
 			addItem_("Modify Representation", SLOT(show()), modify_rep_dialog_);	
			addItem_("Move", SLOT(enterMoveMode()));
			context_menu_.addSeparator();
			context_menu_actions_.push_back(context_menu_.addMenu(&clipping_plane_context_menu_));
			Size acs = context_menu_actions_.size() - 1;
			context_menu_actions_[acs]->setText("Clipping Plane");

			// planes ->
			if (plane != 0)
			{
				for (Position p = 1; p < 6; p++)
				{
					context_menu_actions_[p]->setEnabled(false); 
				}
				context_menu_actions_[2]->setEnabled(true);
				context_menu_actions_[acs-1]->setEnabled(true);
				context_menu_actions_[acs]->setEnabled(true);
				return;
			}

			// representations ->
			context_menu_actions_[acs-1]->setEnabled(rep->getModelType() == MODEL_GRID_SLICE);
			context_menu_actions_[acs]->setEnabled(false);

			if (getSelectedItems().size() != 1)
			{
				for (Position p = 1; p < 7; p++)
				{
					context_menu_actions_[p]->setEnabled(false); 
				}
			}

			// not modifyable
			if (rep->getModelType() >= MODEL_LABEL)
			{
				context_menu_actions_[5]->setEnabled(false);
			}

			if (!isSurfaceModel(rep->getModelType()))
			{
				context_menu_actions_[3]->setEnabled(false);
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
			throw()
		{
			if (!getMainControl()->getRepresentationManager().has(rep)) return;

			QStringList sl;
			sl << rep.getName().c_str()
				 << rep.getColoringName().c_str()
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
			throw()
		{
			if (getMainControl()->getRepresentationManager().updateRunning() ||
			    creating_representations_) 
			{
				setStatusbarText("Could not delete Representation while update is running!", true);
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
					setStatusbarText("Deleted representation.");
					continue;
				}

				ClippingPlane* plane = item_to_plane_[item];
				removeItem_(item);
				item_to_plane_.erase(item);
				plane_to_item_.erase(plane);
				getMainControl()->getRepresentationManager().removeClippingPlane(plane);
				setStatusbarText("Deleted Clipping Plane.");
			}
		}

		void GeometricControl::selectedRepresentation(Representation& representation, bool state)
		{
			if (state != representation.isHidden()) return;

			if (!state)  setStatusbarText("Hide representation.");
			else 				 setStatusbarText("Show representation.");
				
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
				setStatusbarText("No changes to representations allowed, while simulation is running or creating new representations!", true);
				return;
			}

			buildContextMenu();
			context_menu_.popup(mapToGlobal(pos));
		}
		

		void GeometricControl::modifyRepresentation_()
			throw()
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
 			HashMap<QTreeWidgetItem*, Representation*>::ConstIterator it = item_to_representation_.begin();
			for (; +it; ++it)
			{
				QTreeWidgetItem* item = (*it).first;
				if (listview->isItemSelected(item))
				{
					selection.push_back((*it).second);
				}
			}
			
			return selection;
		}


		void GeometricControl::checkMenu(MainControl& main_control)
			throw()
		{
			bool busy = main_control.isBusy();
			if (!busy && getSelectedItems().size() > 0)
			{
				main_control.setDeleteEntryEnabled(true);
			}

			menu_clipping_plane_->setEnabled(!busy);
			menu_load_surface_->setEnabled(!busy);
		}

		void GeometricControl::focusRepresentation()
		{
			List<Representation*> reps = getHighlightedRepresentations();
			if (reps.size() != 1) return;
			
			getMainControl()->getRepresentationManager().focusRepresentation(**reps.begin());
		}

		void GeometricControl::initializeWidget(MainControl& main_control)
			throw()
		{
			GenericControl::initializeWidget(main_control);

			menu_clipping_plane_ = insertMenuEntry(MainControl::DISPLAY, 
																		"New Clipping Plane", this, SLOT(createNewClippingPlane()));   
			setMenuHint("Add an OpenGL Clipping Plane to the Scene");
			setMenuHelp("geometricControl.html#clipping_planes");

			menu_load_surface_ = insertMenuEntry(MainControl::FILE_OPEN, "Surface", this,
																						SLOT(loadSurface()));

			registerForHelpSystem(this, "geometricControl.html");
		}

		void GeometricControl::enterMoveMode()
		{
			getMainControl()->clearSelection();

			notify_(new SceneMessage(SceneMessage::ENTER_MOVE_MODE));
		}

		void GeometricControl::flipClippingPlane()
		{
			if (context_plane_ == 0) return;

			context_plane_->flip();
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
			throw()
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

			List<const Composite*>::const_iterator it = rep.getComposites().begin();
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
			RepresentationManager::RepresentationList::ConstIterator it = pm.getRepresentations().begin();
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
					if (plane->cappingEnabled())
					{
						sl << "CappingPlane";
					}
					else
					{
						sl << "ClippingPlane";
					}
					if (plane->isHidden()) sl << "[hidden]";
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
			for (; +mit; ++mit)
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

			context_item_->setText(2, hidden ? "[hidden]" : "");
		}

		void GeometricControl::duplicateRepresentation()
		{
			if (context_representation_ == 0) return;

			Representation* new_rep = (Representation*) context_representation_->create();
			if (new_rep == 0) return;

			getMainControl()->insert(*new_rep);
		}

		GeometricControl::GeometricControl(const GeometricControl& mc)
			throw()
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

				VIEW::getMainControl()->setStatusbarText("Cannot switch Representation on/off now!", true);
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

			context_item_->setText(0, capping ? "CappingPlane" : "ClippingPlane");
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
												"Export Surface",
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
												"Read Surface",
												(getWorkingDir() + String(FileSystem::PATH_SEPARATOR)).c_str(),
												"*.*");

			if (qresult == QString::null) return;

			String result = ascii(qresult);
			Mesh* mesh = new Mesh();
			Representation* rep = new Representation();
			List<Composite*> cl = getMainControl()->getMolecularControlSelection();
			List<const Composite*> ccl;
			List<Composite*>::iterator cit = cl.begin();
			for (; cit != cl.end(); cit++);
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


	} // namespace VIEW
} // namespace BALL
