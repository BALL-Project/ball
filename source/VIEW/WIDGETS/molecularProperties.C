// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: molecularProperties.C,v 1.3 2003/09/02 16:06:23 amoll Exp $

#include <BALL/VIEW/WIDGETS/molecularProperties.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/STRUCTURE/residueChecker.h>
#include <BALL/STRUCTURE/geometricProperties.h>
#include <BALL/STRUCTURE/fragmentDB.h>

#include <qmenubar.h>

#define BALL_VIEW_DEBUG
using namespace std;

namespace BALL
{
	namespace VIEW
	{

  	MolecularProperties::MolecularProperties(QWidget* parent, const char* name)
			throw()
			:	QWidget(parent, name),
				ModularWidget(name),
				view_distance_(25)
    {
			registerWidget(this);
			hide();

			// cant use ModularWidget::getMainControl() here, no idea why
			MainControl& main_control = *MainControl::getMainControl(this);

			center_camera_id_ = main_control.insertMenuEntry(MainControl::DISPLAY, "&Focus Camera", this, 
																											 	SLOT(centerCamera()), CTRL+Key_F);
			build_bonds_id_ = main_control.insertMenuEntry(MainControl::BUILD, "&Build Bonds", this, 
																										 		SLOT(buildBonds()), CTRL+Key_B);
			add_hydrogens_id_ = main_control.insertMenuEntry(MainControl::BUILD, "Add &Hydrogens", this, 
																											 	SLOT(addHydrogens()), CTRL+Key_H);
			check_structure_id_ = main_control.insertMenuEntry(MainControl::BUILD, "Chec&k Structure", this, 
																												SLOT(checkResidue()), CTRL+Key_K);
			select_id_ = main_control.insertMenuEntry(MainControl::EDIT, "&Select", this, 
																												SLOT(select()), ALT+Key_S);   
			deselect_id_ = main_control.insertMenuEntry(MainControl::EDIT, "&Deselect", this, 
																												SLOT(deselect()), ALT+Key_D);   
    }

  	MolecularProperties::~MolecularProperties()
			throw()
    {
			MainControl& main_control = *getMainControl();
			main_control.removeMenuEntry(MainControl::DISPLAY, "&Focus Camera", this, 
																											SLOT(centerCamera()), CTRL+Key_F);
			main_control.removeMenuEntry(MainControl::BUILD, "&Build Bonds", this, 
																											SLOT(buildBonds()), CTRL+Key_B);
			main_control.removeMenuEntry(MainControl::BUILD, "Add &Hydrogens", this, 
																											SLOT(addHydrogens()), CTRL+Key_H);
			main_control.removeMenuEntry(MainControl::BUILD, "Chec&k Structure", this, 
																											SLOT(checkResidue()), CTRL+Key_K);
			main_control.removeMenuEntry(MainControl::EDIT, "&Select", this, SLOT(select()), ALT+Key_S);   
			main_control.removeMenuEntry(MainControl::EDIT, "&Deselect", this, SLOT(deselect()), ALT+Key_D);   
		}

	  void MolecularProperties::onNotify(Message *message)
			throw()
    {
			if (RTTI::isKindOf<NewCompositeMessage>(*message))
			{
				NewCompositeMessage* new_message = RTTI::castTo<NewCompositeMessage>(*message);
				addComposite_(*new_message->getComposite(), new_message->getCompositeName());
			}
			else if (RTTI::isKindOf<GeometricObjectSelectionMessage>(*message))
			{
				const List<GeometricObject*>& geometric_selection = 
					(RTTI::castTo<GeometricObjectSelectionMessage>(*message))->getSelection();

				// geometric selection is not empty
				if (!geometric_selection.empty())
				{
					List<Composite*> selection;
					
					// new collection not empty
					if (!selection.empty())
					{
						// create a molecular selection message and sent molecular objects
						SelectionMessage *molecular_selection = new SelectionMessage;
						molecular_selection->setDeletable(true);
						molecular_selection->setSelection(selection);

						notify_(molecular_selection);
					}
				}
			}
			else if(RTTI::isKindOf<CompositeSelectedMessage>(*message))
			{
				SceneMessage* scene_message = new SceneMessage;
				scene_message->setType(SceneMessage::REBUILD_DISPLAY_LISTS);
				notify_(scene_message);
				getMainControl()->printSelectionInfos();
			}
			else if (RTTI::isKindOf<MolecularTaskMessage>(*message))
			{
				switch (((RTTI::castTo<MolecularTaskMessage>(*message)))->getType())
				{
					case MolecularTaskMessage::BUILD_BONDS:
						buildBonds();
						return;
					case MolecularTaskMessage::CHECK_RESIDUE:
						checkResidue();
						return;
					case MolecularTaskMessage::ADD_HYDROGENS:
						addHydrogens();
						return;
					default:
						Log.error() << "Unknown type of MolecularTaskMessage in " 
												<< __FILE__ << "  " << __LINE__ << std::endl;
				}
			}
			else if (RTTI::isKindOf<CenterCameraMessage>(*message))
			{
				centerCamera(((CenterCameraMessage*) message)->getComposite());
			}
		}	


		bool MolecularProperties::checkResidue()
		{
			List<Composite*>& selection = getMainControl()->getControlSelection();
			if (selection.size() == 0) return false;

			setStatusbarText("checking " + String(selection.size()) + " objects...");

			ResidueChecker res_check(getFragmentDB());

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
					Log.error() << "ResidueChecker: cannot apply to a " << typeid(**it).name() 
											<< " object" << std::endl;
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
			if (getMainControl()->getControlSelection().size() == 0)
			{
				return;
			}

			// notify the main window
			setStatusbarText("adding hydrogens ...");

			// copy the selection_, it can change after a changemessage event
			List<Composite*> temp_selection_ = getMainControl()->getControlSelection();
			List<Composite*>::ConstIterator it = temp_selection_.begin();	

			ChangedCompositeMessage *change_message = new ChangedCompositeMessage;
			change_message->setDeletable(false);
			change_message->setUpdateControl(true);

			Size number_of_hydrogens = 0;

			for (; it != temp_selection_.end(); ++it)
			{	
				(*it)->apply(getFragmentDB().add_hydrogens);
				number_of_hydrogens += getFragmentDB().add_hydrogens.getNumberOfInsertedAtoms();
				(*it)->apply(getFragmentDB().build_bonds);

				change_message->setComposite((*it));
				notify_(change_message);
			}

			Log.info() << "added " <<  number_of_hydrogens << " hydrogen atoms." << std::endl;

			setStatusbarText("");
		}


		void MolecularProperties::buildBonds()
		{
			if (getMainControl()->getControlSelection().size() == 0)
			{
				return;
			}

			// notify the main window
			setStatusbarText("building bonds ...");

			// copy the selection_, it can change after a changemessage event
			List<Composite*> temp_selection_ = getMainControl()->getControlSelection();
			List<Composite*>::ConstIterator it = temp_selection_.begin();	
			
			ChangedCompositeMessage *change_message = new ChangedCompositeMessage;
			change_message->setDeletable(false);
			change_message->setUpdateControl(true);
			
			Size number_of_bonds = 0;
			for (; it != temp_selection_.end(); ++it)
			{	
				(*it)->apply(getFragmentDB().build_bonds);
				number_of_bonds += getFragmentDB().build_bonds.getNumberOfBondsBuilt();

				change_message->setComposite((*it));
				notify_(change_message);
			}

			setStatusbarText("");

			Log.info() << "Added " << number_of_bonds << " bonds." << std::endl;
		}

		
		void MolecularProperties::centerCamera(Composite* composite)
		{
			Composite* to_center_on = composite;
			
			if (to_center_on == 0)
			{
				if (getMainControl()->getControlSelection().size() == 0)
				{
					return;
				}

				to_center_on = *getMainControl()->getControlSelection().begin();
			}

			// use specified object processor for calculating the center
			calculateCenter_(*to_center_on);

			Vector3 view_point = view_center_vector_;

			// update scene
			SceneMessage *scene_message = new SceneMessage;
			scene_message->getCamera().setLookAtPosition(view_point);

			view_point.z += view_distance_;
			scene_message->getCamera().setViewPoint(view_point);
			scene_message->setDeletable(true);
			scene_message->setType(SceneMessage::UPDATE_CAMERA);
			notify_(scene_message);
		}


		void MolecularProperties::calculateCenter_(Composite &composite)
		{
			GeometricCenterProcessor center;
			composite.apply((UnaryProcessor<Atom>&) center);			
					
			view_center_vector_ = center.getCenter();

			BoundingBoxProcessor bbox;
			composite.apply(bbox);				
			view_distance_ = (bbox.getUpper() - bbox.getLower()).getLength() - center.getCenter().z + 3;
		}


		void MolecularProperties::checkMenu(MainControl& main_control)
			throw()
		{
			List<Composite*>& selection = getMainControl()->getControlSelection();
			Size number_of_selected_objects = selection.size(); 
			bool selected = (number_of_selected_objects != 0);
			selected = selected && getMainControl()->compositesAreMuteable();

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
			List<Composite*>& selection = getMainControl()->getControlSelection();

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

			for (; list_it != temp_selection_.end(); ++list_it)
			{
				cs_message->setComposite(*list_it);
				notify_(cs_message);
			}

			delete cs_message;

			getMainControl()->printSelectionInfos();

			// we have to send SceneMessage here, because it wont be send in onNotify 
			SceneMessage* scene_message = new SceneMessage;
			scene_message->setType(SceneMessage::REBUILD_DISPLAY_LISTS);
			notify_(scene_message);
		}


		void MolecularProperties::deselect()
		{
			List<Composite*>& selection = getMainControl()->getControlSelection();

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
			for (; list_it != temp_selection_.end(); ++list_it)
			{
				if (!(*list_it)->isSelected()) continue;
				// mark composite for update
				cs_message->setComposite(*list_it);
				notify_(cs_message);
			}

			delete cs_message;

			getMainControl()->printSelectionInfos();

			// we have to send SceneMessage here, because it wont be send in onNotify 
			SceneMessage* scene_message = new SceneMessage;
			scene_message->setType(SceneMessage::REBUILD_DISPLAY_LISTS);
			notify_(scene_message);
		}


		void MolecularProperties::addComposite_(Composite& composite, const String& name)
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "starting applying molecular properties" << std::endl;
			#endif

			// properties will be used only for atom containers
			if (!RTTI::isKindOf<AtomContainer>(composite))
			{
				return;
			}
			
			Log.info() << "> applying molecular properties ... " << endl;
			
			AtomContainer& atom_container = *RTTI::castTo<AtomContainer>(composite);
			
			try
			{
				atom_container.apply(getFragmentDB().normalize_names);
			}
			catch (Exception::GeneralException e)
			{
				Log.error() << " > normalize named failed: " <<endl; //<< e << endl;
			}
			catch (...)
			{
				Log.error() << "  > normalized names failed." << endl;
				return;
			}
			
			Log.info() << "  > normalized names" << endl;
			
			try
			{
				atom_container.apply(getFragmentDB().build_bonds);
			}
			catch (Exception::GeneralException e)
			{
				Log.error() << " > generate missing bonds - failed: " <<endl; //<< e << endl;
			}
			catch (...)
			{
				Log.error() << "  > generate missing bonds - failed." << endl;
				return;
			}
			
			Log.info() << "  > generated missing bonds" << endl;
			
			if (atom_container.getName() == "")
			{
				atom_container.setName(name);
			}
			
			// continue with molecular message
			NewMolecularMessage* mol_message = new NewMolecularMessage;
			mol_message->setComposite(composite);
			mol_message->setDeletable(true);
			
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "finished applying molecular properties" << std::endl;
			#endif
			notify_(mol_message);
		}
#undef BALL_VIEW_DEBUG
	} // namespace VIEW
} // namespace BALL
