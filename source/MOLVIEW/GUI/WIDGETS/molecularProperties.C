// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: molecularProperties.C,v 1.7.2.1 2003/01/07 13:21:34 anker Exp $

#include <BALL/MOLVIEW/GUI/WIDGETS/molecularProperties.h>
#include <BALL/MOLVIEW/KERNEL/molecularMessage.h>
#include <BALL/MOLVIEW/FUNCTOR/objectSelector.h>
#include <BALL/STRUCTURE/residueChecker.h>
#include <BALL/STRUCTURE/geometricProperties.h>

#include <qmenubar.h>

using namespace std;

namespace BALL
{
	namespace MOLVIEW
	{

  	MolecularProperties::MolecularProperties(QWidget* parent, const char* name)
			throw()
			:	QWidget(parent, name),
				ModularWidget(name),
				view_direction_(2),
				view_distance_(25)
    {
			registerWidget(this);
			hide();

			MainControl& main_control = *MainControl::getMainControl(this);

			center_camera_id_ = main_control.insertMenuEntry(MainControl::DISPLAY, "Focus C&amera", this, 
																											 	SLOT(centerCamera()), CTRL+Key_A);
			build_bonds_id_ = main_control.insertMenuEntry(MainControl::BUILD, "&Build Bonds", this, 
																										 		SLOT(buildBonds()), CTRL+Key_B);
			add_hydrogens_id_ = main_control.insertMenuEntry(MainControl::BUILD, "Add &Hydrogens", this, 
																											 	SLOT(addHydrogens()), CTRL+Key_H);
			check_structure_id_ = main_control.insertMenuEntry(MainControl::BUILD, "Check St&ructure", this, 
																												SLOT(checkResidue()), CTRL+Key_R);
			select_id_ = main_control.insertMenuEntry(MainControl::EDIT, "&Select", this, SLOT(select()), CTRL+Key_S);   
			deselect_id_ = main_control.insertMenuEntry(MainControl::EDIT, "&Deselect", this, SLOT(deselect()), CTRL+Key_D);   
    }

  	MolecularProperties::~MolecularProperties()
			throw()
    {
			MainControl& main_control = *MainControl::getMainControl(this);
			main_control.removeMenuEntry(MainControl::EDIT, "&Select", this, SLOT(select()), CTRL+Key_S);   
			main_control.removeMenuEntry(MainControl::EDIT, "&Deselect", this, SLOT(deselect()), CTRL+Key_D);   
			main_control.removeMenuEntry(MainControl::DISPLAY, "Focus C&amera", this, SLOT(centerCamera()), CTRL+Key_A);
			main_control.removeMenuEntry(MainControl::BUILD, "&Build Bonds", this, SLOT(buildBonds()), CTRL+Key_B);
			main_control.removeMenuEntry(MainControl::BUILD, "Add &Hydrogens", this, SLOT(addHydrogens()), CTRL+Key_H);
		}

	  void MolecularProperties::onNotify(Message *message)
			throw()
    {
			if (RTTI::isKindOf<NewCompositeMessage>(*message))
			{
				NewCompositeMessage *composite_message = RTTI::castTo<NewCompositeMessage>(*message);
				
				// properties will be used only for atom containers
				if (!RTTI::isKindOf<AtomContainer>(*(composite_message->getComposite())))
				{
					return;
				}
				
				Log.info() << "> applying molecular properties ... " << endl;
				
				AtomContainer* atom_container = RTTI::castTo<AtomContainer>(*(composite_message->getComposite()));
				
				try
				{
					atom_container->apply(fragment_db_.normalize_names);
				}
				catch (Exception::GeneralException e)
				{
					Log.error() << " > normalize named failed: " << e << endl;
				}
				catch (...)
				{
					Log.error() << "  > normalized names failed." << endl;
					return;
				}
				
				Log.info() << "  > normalized names" << endl;
				
				try
				{
					atom_container->apply(fragment_db_.build_bonds);
				}
				catch (Exception::GeneralException e)
				{
					Log.error() << " > generate missing bonds - failed: " << e << endl;
				}
				catch (...)
				{
					Log.error() << "  > generate missing bonds - failed." << endl;
					return;
				}
				
				Log.info() << "  > generated missing bonds" << endl;
				
				if (atom_container->getName() == "")
				{
					atom_container->setName(composite_message->getCompositeName());
				}
				
				// continue with molecular message
				NewMolecularMessage* mol_message = new NewMolecularMessage;
				mol_message->setComposite(composite_message->getComposite());
				mol_message->setDeletable(true);
				
				notify_(mol_message);
			}
			else if (RTTI::isKindOf<GeometricObjectSelectionMessage>(*message))
			{
				GeometricObjectSelectionMessage* geometric_selection = RTTI::castTo<GeometricObjectSelectionMessage>(*message);

				// geometric selection is not empty
				if (!geometric_selection->getSelection().empty())
				{
					List<Composite*> selection;
					List<Composite*>::ConstIterator it = geometric_selection->getSelection().begin();
					
					for (; it != geometric_selection->getSelection().end(); ++it)
					{
						Atom* atom = (**it).getAncestor(RTTI::getDefault<Atom>());
						AtomContainer* fragment = (**it).getAncestor(RTTI::getDefault<AtomContainer>());

						if (atom != 0)
						{
							selection.push_back(atom);
						}
						else if (fragment != 0)
						{
							selection.push_back(fragment);
						}
					}

					// new collection not empty
					if (!selection.empty())
					{
						// create a molecular selection message and sent molecular objects
						MolecularSelectionMessage *molecular_selection = new MolecularSelectionMessage;
						molecular_selection->setDeletable(true);
						molecular_selection->setSelection(selection);

						notify_(molecular_selection);
					}
				}
			}
			else if(RTTI::isKindOf<CompositeSelectedMessage>(*message))
			{
				// Information from Control: 1 Composite selected or deselected.
				CompositeSelectedMessage* selection_message = RTTI::castTo<CompositeSelectedMessage>(*message);
				if (selection_message->selected_) 
				{
					// select the geometric objects of the composite
					ObjectSelector selector;
					selection_message->composite_->apply(selector);
				}
				else
				{
					// deselect the geometric objects of the composite
					ObjectDeselector deselector;
					selection_message->composite_->apply(deselector);
				}

				// Inform the Scene of the changes
				MainControl::getMainControl(this)->update(selection_message->composite_->getRoot());

				SceneMessage* scene_message = new SceneMessage;
				scene_message->updateOnly();
				notify_(scene_message);
			}
			else if (RTTI::isKindOf<BuildBondsMessage>(*message))
			{
				buildBonds();
			}
			else if (RTTI::isKindOf<CenterCameraMessage>(*message))
			{
				centerCamera(((CenterCameraMessage*) message)->getComposite());
			}
			else if (RTTI::isKindOf<SelectMessage>(*message))
			{
				SelectMessage* smessage = RTTI::castTo<SelectMessage>(*message);
				if (smessage->select_) select();
				else									 deselect();
			}
			else if (RTTI::isKindOf<CheckResidueMessage>(*message))
			{
				checkResidue();
			}
		}	


		bool MolecularProperties::checkResidue()
		{
			List<Composite*>& selection = MainControl::getMainControl(this)->getControlSelection();
			if (selection.size() == 0) return false;

			setStatusbarText("checking" + String(selection.size()) + " objects...");

			ResidueChecker res_check(fragment_db_);

			bool okay = true;

			List<Composite*>::ConstIterator it = selection.begin();	
			for (; it != selection.end(); ++it)
			{	
				if (RTTI::isKindOf<AtomContainer>(**it))
				{
					(*it)->apply(res_check);
					okay = okay && res_check.getStatus();	
				}
				else
				{
					Log.error() << "ResidueChecker: cannot apply to a " << typeid(**it).name() << " object" << std::endl;
				}
			}

			if (okay)
			{
				Log.info() << "ResidueChecker: no errors found." << endl;
				setStatusbarText("no errors.");
			} 
			else 
			{
				setStatusbarText("errors found!");
			}

			return okay;
		}


		void MolecularProperties::addHydrogens()
		{
			if (MainControl::getMainControl(this)->getControlSelection().size() == 0)
			{
				return;
			}

			// notify the main window
			setStatusbarText("adding hydrogens ...");

			// copy the selection_, it can change after a changemessage event
			List<Composite*> temp_selection_ = MainControl::getMainControl(this)->getControlSelection();
			List<Composite*>::ConstIterator it = temp_selection_.begin();	

			ChangedCompositeMessage *change_message = new ChangedCompositeMessage;
			change_message->setDeletable(false);

			DrawMessage* draw_message = new DrawMessage;
			draw_message->setDeletable(false);

			Size number_of_hydrogens = 0;

			for (; it != temp_selection_.end(); ++it)
			{	
				(*it)->apply(fragment_db_.add_hydrogens);
				number_of_hydrogens += fragment_db_.add_hydrogens.getNumberOfInsertedAtoms();
				(*it)->apply(fragment_db_.build_bonds);

				draw_message->setComposite((*it));
				notify_(draw_message);
				change_message->setComposite((*it));
				notify_(change_message);
			}

			Log.info() << "added " <<  number_of_hydrogens << " hydrogen atoms." << std::endl;

			// update scene
			SceneMessage *scene_message = new SceneMessage;
			scene_message->updateOnly();
			scene_message->setDeletable(true);
			notify_(scene_message);

			setStatusbarText("");
		}


		void MolecularProperties::buildBonds()
		{
			if (MainControl::getMainControl(this)->getControlSelection().size() == 0)
			{
				return;
			}

			// notify the main window
			setStatusbarText("building bonds ...");

			// copy the selection_, it can change after a changemessage event
			List<Composite*> temp_selection_ = MainControl::getMainControl(this)->getControlSelection();
			List<Composite*>::ConstIterator it = temp_selection_.begin();	
			
			ChangedCompositeMessage *change_message = new ChangedCompositeMessage;
			change_message->setDeletable(false);
			
			DrawMessage* draw_message = new DrawMessage;
			draw_message->setDeletable(false);

			Size number_of_bonds = 0;
			for (; it != temp_selection_.end(); ++it)
			{	
				(*it)->apply(fragment_db_.build_bonds);
				number_of_bonds += fragment_db_.build_bonds.getNumberOfBondsBuilt();

				draw_message->setComposite((*it));
				notify_(draw_message);
				change_message->setComposite((*it));
				notify_(change_message);
			}

			// update scene
			SceneMessage *scene_message = new SceneMessage;
			scene_message->updateOnly();
			scene_message->setDeletable(true);
			notify_(scene_message);

			setStatusbarText("");

			Log.info() << "Added " << number_of_bonds << " bonds." << std::endl;
		}

		
		void MolecularProperties::centerCamera(Composite* composite)
		{
			Composite* to_center_on = composite;
			
			if (to_center_on == 0)
			{
				if (MainControl::getMainControl(this)->getControlSelection().size() == 0)
				{
					return;
				}

				to_center_on = *MainControl::getMainControl(this)->getControlSelection().begin();
			}

			// use specified object processor for calculating the center
			calculateCenter_(*to_center_on);

			Vector3 view_point = view_center_vector_;

			// update scene
			SceneMessage *scene_message = new SceneMessage;
			scene_message->setCameraLookAt(view_point);

			view_point.z += view_distance_;
			scene_message->setCameraViewPoint(view_point);
			scene_message->setDeletable(true);
			notify_(scene_message);
		}


		void MolecularProperties::calculateCenter_(Composite &composite)
		{
			GeometricCenterProcessor center;
			composite.apply((UnaryProcessor<Atom>&) center);			
					
			view_center_vector_ = center.getCenter();
			view_direction_ = 2;

			BoundingBoxProcessor bbox;
			composite.apply(bbox);				
			view_distance_ = (bbox.getUpper() - bbox.getLower()).getLength() - center.getCenter().z + 3;
		}


		void MolecularProperties::checkMenu(MainControl& main_control)
			throw()
		{
			List<Composite*>& selection = MainControl::getMainControl(this)->getControlSelection();
			Size number_of_selected_objects = selection.size(); 
			bool selected = (number_of_selected_objects != 0);

			(main_control.menuBar())->setItemEnabled(add_hydrogens_id_, selected);
			(main_control.menuBar())->setItemEnabled(build_bonds_id_, selected);
			(main_control.menuBar())->setItemEnabled(check_structure_id_, selected);

			// these menu points for single items only
			(main_control.menuBar()) ->setItemEnabled(center_camera_id_, number_of_selected_objects == 1);

			if (!selected)
			{
				(main_control.menuBar())->setItemEnabled(select_id_, false);
				(main_control.menuBar())->setItemEnabled(deselect_id_, false);
				return;
			}

			bool allow_select = true;
			bool allow_deselect = true;
			List<Composite*>::Iterator it = selection.begin();
			for (; it != selection.end(); it++)
			{
				if (!(**it).isSelected())
				{
					allow_deselect = false;
				}
				else
				{
					allow_select = false;
				}
			}
			
			(main_control.menuBar())->setItemEnabled(select_id_, allow_select);
			(main_control.menuBar())->setItemEnabled(deselect_id_, allow_deselect);
		}


		void MolecularProperties::select()
		{
			List<Composite*>& selection = MainControl::getMainControl(this)->getControlSelection();

			if (selection.size() == 0)
			{
				return;
			}

			// notify the main window
			setStatusbarText("selecting " + String(selection.size()) + " objects...");

			// copy list because the selection_ list can change after a changemessage event
			List<Composite*> temp_selection_ = selection;
						
			List<Composite*>::ConstIterator list_it = temp_selection_.begin();	
			CompositeSelectedMessage* cs_message = new CompositeSelectedMessage(0, true);
			cs_message->setDeletable(false);

			ObjectSelector selector;

			for (; list_it != temp_selection_.end(); ++list_it)
			{
				(*list_it)->apply(selector);
				MainControl::getMainControl(this)->update((*list_it)->getRoot());
				cs_message->composite_ = *list_it;
				notify_(cs_message);
			}

			// Inform the Scene of the changes
			SceneMessage* scene_message = new SceneMessage;
			scene_message->updateOnly();
			notify_(scene_message);

			setStatusbarText("");
		}


		void MolecularProperties::deselect()
		{
			List<Composite*>& selection = MainControl::getMainControl(this)->getControlSelection();

			if (selection.size() == 0)
			{
				return;
			}

			// notify the main window
			setStatusbarText("deselecting " + String(selection.size()) + "objects...");

			// copy list because the selection_ list can change after a changemessage event
			List<Composite*> temp_selection_ = selection;

			List<Composite*>::ConstIterator list_it = temp_selection_.begin();	
			CompositeSelectedMessage* cs_message = new CompositeSelectedMessage(0, false);
			cs_message->setDeletable(false);
			ObjectSelector selector;
			for (; list_it != temp_selection_.end(); ++list_it)
			{
				if (!(*list_it)->isSelected()) continue;
				(*list_it)->apply(selector);
				// mark composite for update
				MainControl::getMainControl(this)->update((*list_it)->getRoot());
				cs_message->composite_ = *list_it;
				notify_(cs_message);
			}

			SceneMessage* scene_message = new SceneMessage;
			scene_message->updateOnly();
			notify_(scene_message);

			setStatusbarText("");
		}

	} // namespace MOLVIEW

} // namespace BALL
