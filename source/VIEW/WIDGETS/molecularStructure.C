// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: molecularStructure.C,v 1.10 2004/02/10 15:51:36 amoll Exp $

#include <BALL/VIEW/WIDGETS/molecularStructure.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/DIALOGS/peptideDialog.h>

#include <BALL/STRUCTURE/residueChecker.h>
#include <BALL/STRUCTURE/geometricProperties.h>
#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/STRUCTURE/secondaryStructureProcessor.h>
#include <BALL/STRUCTURE/structureMapper.h>
#include <BALL/STRUCTURE/HBondProcessor.h>

#include <BALL/FORMAT/DCDFile.h>
#include <BALL/KERNEL/system.h>

#include <BALL/MOLMEC/CHARMM/charmm.h>
#include <BALL/MOLMEC/AMBER/amber.h>

#include <BALL/MOLMEC/MINIMIZATION/conjugateGradient.h>
#include <BALL/MOLMEC/MINIMIZATION/steepestDescent.h>

#include <BALL/MOLMEC/MDSIMULATION/microCanonicalMD.h>
#include <BALL/MOLMEC/MDSIMULATION/canonicalMD.h>
#include <BALL/MOLMEC/MDSIMULATION/microCanonicalMD.h>
#include <BALL/MOLMEC/MDSIMULATION/molecularDynamics.h>

#ifdef BALL_QT_HAS_THREADS
#	include <BALL/VIEW/KERNEL/threads.h>
#endif

#include <qmenubar.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

	MolecularStructure::MolecularStructure(QWidget* parent, const char* name)
		throw()
		:	QWidget(parent, name),
			ModularWidget(name),
			view_distance_(25),
			amber_(0),
			charmm_(0),
			amber_dialog_(this),
			minimization_dialog_(this),
			md_dialog_(this)
	{
		registerWidget(this);
		hide();

		// cant use ModularWidget::getMainControl() here, no idea why
		MainControl& main_control = *MainControl::getMainControl(this);
		String hint;
		
		hint = "Focus the camera on one or multiple objects."; 
		center_camera_id_ = main_control.insertMenuEntry(MainControl::DISPLAY_VIEWPOINT, "&Focus Camera", this, 
																											SLOT(centerCamera()), CTRL+Key_F, -1, hint);

		minimization_dialog_.setAmberDialog(&amber_dialog_);
		md_dialog_.setAmberDialog(&amber_dialog_);

		// Build Menu -------------------------------------------------------------------
		hint = "To assign charges, one System has to be selected.";
		assign_charges_id_ = insertMenuEntry(MainControl::BUILD, "Assign Char&ges", this, SLOT(assignCharges()),
										CTRL+Key_G, -1 , hint);

		build_peptide_id_ = insertMenuEntry(MainControl::BUILD, "Build Peptide", this, SLOT(buildPeptide()), ALT+Key_P, 
										-1, "Build a peptide from selected amino acids.");

		hint = "Add missing bonds to a selected structure.";
		build_bonds_id_ = insertMenuEntry(MainControl::BUILD, "&Build Bonds", this, 
																											SLOT(buildBonds()), CTRL+Key_B, -1, hint);
		
		hint = "Add missing H-atoms to a selected structure."; 
		add_hydrogens_id_ = insertMenuEntry(MainControl::BUILD, "Add &Hydrogens", this, 
																											SLOT(addHydrogens()), CTRL+Key_H, -1, hint);
		
		hint = "Check a structure against the fragment database.";
		check_structure_id_ = insertMenuEntry(MainControl::BUILD, "Chec&k Structure", this, 
																											SLOT(checkResidue()), CTRL+Key_K, -1, hint);
		
	
		hint = "Select a molecular object to see its position in the scene or to mark it for a simulation";
		select_id_ = insertMenuEntry(MainControl::EDIT, "&Select", this, SLOT(select()), ALT+Key_S, -1, hint);   
		
		hint = "Deselect a molecular object.";
		deselect_id_ = insertMenuEntry(MainControl::EDIT, "&Deselect", this, SLOT(deselect()), ALT+Key_D, -1, hint);
		// MOLECULARMECHANICS Menu -------------------------------------------------------------------
		hint = "Calculate the energy of a System with the AMBER force field.";
		amber_energy_id_ = insertMenuEntry(MainControl::MOLECULARMECHANICS, "Single Point Calculation", this, 
																			 SLOT(calculateAmberEnergy()), CTRL+Key_A, MainControl::MOLECULARMECHANICS + 12, hint);
			
		hint = "To perform an Energy Minimization, first select the molecular structures.";
		amber_minimization_id_ = insertMenuEntry(MainControl::MOLECULARMECHANICS, "&Energy Minimization", this, 
															SLOT(amberMinimization()), CTRL+Key_E, MainControl::MOLECULARMECHANICS+ 10, hint);

		hint = "To perform a MD simulation , first select the molecular structures.";
		amber_mdsimulation_id_ = insertMenuEntry(MainControl::MOLECULARMECHANICS, "Molecular &Dynamics", this, 
															SLOT(amberMDSimulation()), CTRL+Key_D, MainControl::MOLECULARMECHANICS + 11, hint);

		hint = "Options for the used force fields.";
		force_field_options_id_ = insertMenuEntry(MainControl::MOLECULARMECHANICS, "Force Field &Options", this,
															SLOT(showForceFieldOptions()), 0, MainControl::MOLECULARMECHANICS +14, hint);

		// Tools Menu -------------------------------------------------------------------
		getMainControl()->insertPopupMenuSeparator(MainControl::TOOLS);
		hint = "Map two proteins.";
		map_proteins_id_ = insertMenuEntry(MainControl::TOOLS, "&Map two Proteins", this, SLOT(mapProteins()), 0, -1, hint);

		hint = "Calculate RMSD for two Molecules or Fragments of Molecules.";
		calculate_RMSD_id_ = insertMenuEntry(MainControl::TOOLS, "&Calculate RMSD", this, SLOT(calculateRMSD()), 0, -1, hint);

		getMainControl()->insertPopupMenuSeparator(MainControl::TOOLS);

		hint = "Recalculate the secondary structure for a structure.";
		calculate_ss_id_ = insertMenuEntry(MainControl::TOOLS, "Calculate Secondary Structure", this,
																								 SLOT(calculateSecondaryStructure()), ALT+Key_2, -1, hint);

		hint = "To assign H-bonds, one System has to be selected.";
		calculate_hbonds_id_ = insertMenuEntry(MainControl::TOOLS, "Calculate H-Bonds", this, SLOT(calculateHBonds()),
										CTRL+Key_9, -1, hint);

		getMainControl()->insertPopupMenuSeparator(MainControl::TOOLS);

		hint = "Create a grid with the distance to the geometric center of a structure.";
		create_distance_grid_id_ = insertMenuEntry(MainControl::TOOLS, 
																				"&Distance Grid", this, SLOT(createGridFromDistance()), 0, -1, hint);   
	}

	MolecularStructure::~MolecularStructure()
		throw()
	{
		MainControl& main_control = *getMainControl();
		main_control.removeMenuEntry(MainControl::DISPLAY_VIEWPOINT, "&Focus Camera", this, 
																										SLOT(centerCamera()), CTRL+Key_F);
		main_control.removeMenuEntry(MainControl::BUILD, "&Build Bonds", this, 
																										SLOT(buildBonds()), CTRL+Key_B);
		main_control.removeMenuEntry(MainControl::BUILD, "Add &Hydrogens", this, 
																										SLOT(addHydrogens()), CTRL+Key_H);
		main_control.removeMenuEntry(MainControl::BUILD, "Chec&k Structure", this, 
																										SLOT(checkResidue()), CTRL+Key_K);
		main_control.removeMenuEntry(MainControl::EDIT, "&Select", this, SLOT(select()), ALT+Key_S);   
		main_control.removeMenuEntry(MainControl::EDIT, "&Deselect", this, SLOT(deselect()), ALT+Key_D);   
		main_control.removeMenuEntry(MainControl::TOOLS_CREATE_GRID, "&Distance Grid", this, 
																										SLOT(createGridFromDistance()));   
		main_control.removeMenuEntry(MainControl::BUILD, "Calculate Secondary Structure", this,
																										SLOT(calculateSecondaryStructure()));

		main_control.removeMenuEntry(MainControl::BUILD, "Map two Proteins", this,
																										SLOT(mapProteins()));

		if (charmm_ != 0) delete charmm_;
		if (amber_  != 0) delete amber_;
	}

	void MolecularStructure::onNotify(Message *message)
		throw()
	{
#ifdef BALL_VIEW_DEBUG
		Log.error() << "MolecularStructure " << this  << "onNotify " << message << std::endl;
#endif
		if (RTTI::isKindOf<CompositeMessage>(*message))
		{
			CompositeMessage* cmessage = RTTI::castTo<CompositeMessage>(*message);
			switch (cmessage->getType())
			{
				case CompositeMessage::NEW_COMPOSITE:
					addComposite_(*cmessage->getComposite(), cmessage->getCompositeName());
					return;
				case CompositeMessage::CENTER_CAMERA:
					centerCamera(cmessage->getComposite());
					return;

				default:
					return;
			}
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
				case MolecularTaskMessage::CREATE_DISTANCE_GRID:
					createGridFromDistance();
					return;

				default:
					Log.error() << "Unknown type of MolecularTaskMessage in " 
											<< __FILE__ << "  " << __LINE__ << std::endl;
			}
		}
	}	


	bool MolecularStructure::checkResidue()
	{
		List<Composite*>& selection = getMainControl()->getMolecularControlSelection();
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


	void MolecularStructure::addHydrogens()
	{
		if (getMainControl()->getMolecularControlSelection().size() == 0) 
		{
			return;
		}

		setStatusbarText("adding hydrogens ...");

		// copy the selection_, it can change after a changemessage event
		List<Composite*> temp_selection_ = getMainControl()->getMolecularControlSelection();
		List<Composite*>::ConstIterator it = temp_selection_.begin();	

		Size number_of_hydrogens = 0;

		for (; it != temp_selection_.end(); ++it)
		{	
			(*it)->apply(getFragmentDB().add_hydrogens);
			number_of_hydrogens += getFragmentDB().add_hydrogens.getNumberOfInsertedAtoms();
			
			if (getFragmentDB().add_hydrogens.getNumberOfInsertedAtoms() == 0) continue;
			
			(*it)->apply(getFragmentDB().build_bonds);

			CompositeMessage *change_message = 
				new CompositeMessage(**it, CompositeMessage::CHANGED_COMPOSITE_AND_UPDATE_MOLECULAR_CONTROL);
			notify_(change_message);
		}

		String result = "added " +  String(number_of_hydrogens) + " hydrogen atoms."; 
		Log.info() << result << std::endl;   
		setStatusbarText(result);
	}


	void MolecularStructure::buildBonds()
	{
		if (getMainControl()->getMolecularControlSelection().size() == 0) 
		{
			return;
		}

		setStatusbarText("building bonds ...");

		// copy the selection_, it can change after a changemessage event
		List<Composite*> temp_selection_ = getMainControl()->getMolecularControlSelection();
		List<Composite*>::ConstIterator it = temp_selection_.begin();	
		
		Size number_of_bonds = 0;
		for (; it != temp_selection_.end(); ++it)
		{	
			(*it)->apply(getFragmentDB().build_bonds);
			number_of_bonds += getFragmentDB().build_bonds.getNumberOfBondsBuilt();

			CompositeMessage *change_message = 
				new CompositeMessage(**it, CompositeMessage::CHANGED_COMPOSITE_AND_UPDATE_MOLECULAR_CONTROL);
			notify_(change_message);
		}

		String result = "added " + String(number_of_bonds) + " bonds.";
		setStatusbarText(result);
		Log.info() << result << std::endl;
	}


	void MolecularStructure::centerCamera(Composite* composite)
	{
		Composite* to_center_on = composite;
		
		if (to_center_on == 0)
		{
			if (getMainControl()->getMolecularControlSelection().size() == 0)
			{
				return;
			}

			to_center_on = *getMainControl()->getMolecularControlSelection().begin();
		}

		// use specified object processor for calculating the center
		calculateCenter_(*to_center_on);

		Vector3 view_point = view_center_vector_;

		// update scene
		SceneMessage *scene_message = new SceneMessage(SceneMessage::UPDATE_CAMERA);
		scene_message->getCamera().setLookAtPosition(view_point);
		view_point.z += view_distance_;
		scene_message->getCamera().setViewPoint(view_point);
		notify_(scene_message);
	}


	void MolecularStructure::calculateCenter_(Composite &composite)
	{
		GeometricCenterProcessor center;
		composite.apply((UnaryProcessor<Atom>&) center);			
				
		view_center_vector_ = center.getCenter();

		BoundingBoxProcessor bbox;
		composite.apply(bbox);				
		view_distance_ = (bbox.getUpper() - bbox.getLower()).getLength() - center.getCenter().z + 3;
	}


	void MolecularStructure::checkMenu(MainControl& main_control)
		throw()
	{
		List<Composite*>& selection = main_control.getMolecularControlSelection();
		Size number_of_selected_objects = selection.size(); 

		bool one_item = (number_of_selected_objects == 1);
		bool composites_muteable = main_control.compositesAreMuteable();

		menuBar()->setItemEnabled(assign_charges_id_, one_item && composites_muteable);

		// AMBER methods are available only for single systems
		menuBar()->setItemEnabled(amber_energy_id_, one_item);
		// disable simulation entries, if a simulation is already running
		menuBar()->setItemEnabled(amber_minimization_id_, one_item && composites_muteable);
		menuBar()->setItemEnabled(amber_mdsimulation_id_, one_item && composites_muteable);
		menuBar()->setItemEnabled(build_peptide_id_, composites_muteable);


		bool selected = (number_of_selected_objects != 0);
		selected = selected && main_control.compositesAreMuteable();

		menuBar()->setItemEnabled(add_hydrogens_id_, selected);
		menuBar()->setItemEnabled(build_bonds_id_, selected);
		menuBar()->setItemEnabled(check_structure_id_, selected);

		menuBar()->setItemEnabled(calculate_ss_id_, main_control.getSelectedSystem() && composites_muteable);

		// these menu point for single items only
		menuBar() ->setItemEnabled(center_camera_id_, one_item);
		menuBar()->setItemEnabled(create_distance_grid_id_, one_item);

		menuBar()->setItemEnabled(map_proteins_id_, number_of_selected_objects == 2);
		menuBar()->setItemEnabled(calculate_RMSD_id_, number_of_selected_objects == 2);

		menuBar()->setItemEnabled(select_id_, selected);
		menuBar()->setItemEnabled(deselect_id_, selected);
	}


	void MolecularStructure::select()
	{
		List<Composite*>& selection = getMainControl()->getMolecularControlSelection();

		if (!selection.size()) return;

		// notify the main window
		setStatusbarText("selecting " + String(selection.size()) + " objects...");

		// copy list because the selection_ list can change after a changemessage event
		List<Composite*> temp_selection_ = selection;
					
		List<Composite*>::ConstIterator list_it = temp_selection_.begin();	

		for (; list_it != temp_selection_.end(); ++list_it)
		{
			CompositeMessage* cs_message = new CompositeMessage(**list_it, CompositeMessage::SELECTED_COMPOSITE);
			notify_(cs_message);
		}

		getMainControl()->printSelectionInfos();
	}


	void MolecularStructure::deselect()
	{
		List<Composite*>& selection = getMainControl()->getMolecularControlSelection();

		if (!selection.size()) return;

		setStatusbarText("deselecting " + String(selection.size()) + "objects...");

		// copy list because the selection_ list can change after a changemessage event
		List<Composite*> temp_selection_ = selection;

		List<Composite*>::ConstIterator list_it = temp_selection_.begin();	
		for (; list_it != temp_selection_.end(); ++list_it)
		{
			if (!(*list_it)->isSelected()) continue;
			// mark composite for update
			CompositeMessage* cs_message = new CompositeMessage(**list_it, CompositeMessage::DESELECTED_COMPOSITE);
			notify_(cs_message);
		}

		getMainControl()->printSelectionInfos();
	}


	void MolecularStructure::addComposite_(Composite& composite, const String& name)
		throw()
	{
		#ifdef BALL_VIEW_DEBUG
			Log.error() << "starting applying molecular properties" << std::endl;
		#endif

		// properties will be used only for atom containers
		if (!RTTI::isKindOf<AtomContainer>(composite)) return;
		
		Log.info() << "> applying molecular properties ... " << endl;
		
		AtomContainer& atom_container = *RTTI::castTo<AtomContainer>(composite);
		
		try
		{
			atom_container.apply(getFragmentDB().normalize_names);
		}
		catch (Exception::GeneralException e)
		{
			Log.error() << " > normalize named failed: " <<endl; 
			Log.error() << e << endl;
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
			Log.error() << " > generate missing bonds - failed: " <<endl; 
			Log.error() << e << endl;
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
		
		#ifdef BALL_VIEW_DEBUG
			Log.error() << "finished applying molecular properties" << std::endl;
		#endif
		// continue with molecular message
		CompositeMessage* mol_message = new CompositeMessage(composite, CompositeMessage::NEW_MOLECULE);
		notify_(mol_message);
	}


	void MolecularStructure::createGridFromDistance()
	{
		if (!getMainControl()->getSelectedSystem()) return;
		System& S = *(System*) getMainControl()->getSelectedSystem();
		
		Vector3 v(0,0,0);
		AtomIterator atit = S.beginAtom();

		for (; +atit; ++atit)
		{
			v+=atit->getPosition();
		}

		v /= S.countAtoms();

		BoundingBoxProcessor bs;

		S.apply(bs);

		RegularData3D* regdat = new RegularData3D(RegularData3D::IndexType(50), 
																							bs.getLower()-Vector3(2,2,2), 
																							bs.getUpper()-bs.getLower()+Vector3(4,4,4));

		for (Size i=0; i < regdat->size(); i++)
		{
			float distance = (regdat->getCoordinates(i) - v).getLength();
			(*regdat)[i] = distance;
		}

		RegularData3DMessage* message = new RegularData3DMessage(RegularData3DMessage::NEW);
		message->setComposite(S);
		message->setCompositeName(S.getName() + "_distance");
		message->setRegularData3D(*regdat);
		notify_(message);

		setStatusbarText("done");
	}

	void MolecularStructure::calculateSecondaryStructure()
	{
		if (!getMainControl()->getSelectedSystem()) return;
		System& s = *(System*) getMainControl()->getSelectedSystem();
		SecondaryStructureProcessor ssp;
		s.apply(ssp);

		CompositeMessage *change_message = 
			new CompositeMessage(s, CompositeMessage::CHANGED_COMPOSITE_AND_UPDATE_MOLECULAR_CONTROL);
		notify_(change_message);
		setStatusbarText("Calculated Secondary Structure");
	}


	void MolecularStructure::calculateRMSD()
	{
		if (getMainControl()->getMolecularControlSelection().size() != 2)
		{
			return;
		}

		AtomContainer * a1 = 0;
		AtomContainer * a2 = 0;

		List<Composite*>::Iterator it = getMainControl()->getMolecularControlSelection().begin();
		
		if (!RTTI::isKindOf<AtomContainer>(**it)) 
		{
			setStatusbarText("Exact two AtomContainers have to be selected");
			return;
		}

		a1 = (AtomContainer*) *it;
		it++;
		if (!RTTI::isKindOf<AtomContainer>(**it)) 
		{
			setStatusbarText("Exact two AtomContainers have to be selected");
			return;
		}
		
		a2 = (AtomContainer*) *it;

		if (a1->isRelatedWith(*a2))
		{
			setStatusbarText("The two Proteins must not be descendet/ancestor of eachother.");
			return;
		}

		if (!a1->apply(getFragmentDB().normalize_names) ||
				!a2->apply(getFragmentDB().normalize_names)		) 
		{
			setStatusbarText("Could not apply normalize names, so cant calulate RMSD");
			return;
		}

		StructureMapper sm(*a1, *a2);
		double	rmsd = sm.calculateRMSD();

		Log.info() << "Calcuted RMSD: " << rmsd << std::endl;
		String rmsd_text("Calcuted RMSD: " + String(rmsd));
		if (sm.getBijection().size() != a1->countAtoms())
		{
			Index not_matched = max(a1->countAtoms() - sm.getBijection().size(), a2->countAtoms() -sm.getBijection().size());
			String warning("WARNING: " + String(not_matched) + " atoms were not mapped.");
			Log.error() << warning << std::endl;
			rmsd_text += "  WARNING: not all atoms were mapped.";
		}

		setStatusbarText(rmsd_text);
	}

	void MolecularStructure::mapProteins()
	{
		if (getMainControl()->getMolecularControlSelection().size() != 2)
		{
			return;
		}

		Protein* a1 = 0;
		Protein* a2 = 0;

		List<Composite*>::Iterator it = getMainControl()->getMolecularControlSelection().begin();
		
		if (!RTTI::isKindOf<Protein>(**it)) 
		{
			setStatusbarText("Exact two Proteins have to be selected");
			return;
		}
		
		a1 = (Protein*) *it;
		it++;

		if (!RTTI::isKindOf<Protein>(**it)) 
		{
			setStatusbarText("Exact two Proteins have to be selected");
			return;
		}

		a2 = (Protein*) *it;

		if (a1->isRelatedWith(*a2))
		{
			setStatusbarText("The two Proteins must not be descendet/ancestor of eachother.");
			return;
		}

		if (!a1->apply(getFragmentDB().normalize_names) ||
				!a2->apply(getFragmentDB().normalize_names)		) 
		{
			setStatusbarText("Could not apply normalize names, so cant calulate RMSD");
			return;
		}

		StructureMapper sm(*a1, *a2);
		

		double					rmsd;
		
		map<String, Position> type_map;
		type_map["ALA"] = 0;
		type_map["GLY"] = 1;
		type_map["VAL"] = 2;
		type_map["LEU"] = 3;
		type_map["ILE"] = 4;
		type_map["SER"] = 5;
		type_map["CYS"] = 6;
		type_map["THR"] = 7;
		type_map["MET"] = 8;
		type_map["PHE"] = 9;
		type_map["TYR"] = 10;
		type_map["TRP"] = 11;
		type_map["PRO"] = 12;
		type_map["HIS"] = 13;
		type_map["LYS"] = 14;
		type_map["ARG"] = 15;
		type_map["ASP"] = 16;
		type_map["GLU"] = 17;
		type_map["ASN"] = 18;
		type_map["GLN"] = 19;

		double upper = 8.0;
		double lower = 4.0;
		double tolerance = 0.6;

		Size not_matched_ca;

		Matrix4x4	m;
		m = sm.mapProteins(*a1, *a2, type_map, not_matched_ca, rmsd, upper, lower, tolerance);
		sm.setTransformation(m);
		a1->apply(sm);

		CompositeMessage* cm =
				new CompositeMessage(*a1, CompositeMessage::CHANGED_COMPOSITE);
		notify_(cm);

		Log.info() << "Calcuted RMSD: " << rmsd << std::endl;
		Log.info() << "Calcuted Transformation: " << std::endl << m<< std::endl;
		Log.info() << "Number of not matched CA atoms: " << not_matched_ca << std::endl << std::endl;
		setStatusbarText("Calcuted RMSD: " + String(rmsd));
	}

	AmberFF& MolecularStructure::getAMBERFF()
		throw()
	{
		if (amber_ == 0) 
		{
			amber_ = new AmberFF;
			amber_dialog_.setAmberFF(*amber_);
			amber_dialog_.accept();
		}

		return *amber_;
	}

	AmberConfigurationDialog& MolecularStructure::getAmberConfigurationDialog()
		throw()
	{
		return amber_dialog_;
	}
		

	CharmmFF& MolecularStructure::getCHARMMFF()
		throw()
	{
		if (charmm_ == 0) 
		{
			charmm_ = new CharmmFF();

			charmm_->options[CharmmFF::Option::ASSIGN_TYPES] = "true";
			charmm_->options[CharmmFF::Option::ASSIGN_CHARGES] = "true";
			charmm_->options[CharmmFF::Option::ASSIGN_TYPENAMES] = "true";
			charmm_->options[CharmmFF::Option::OVERWRITE_CHARGES] = "true";
			charmm_->options[CharmmFF::Option::OVERWRITE_TYPENAMES] = "true";
			charmm_->options[CharmmFF::Option::DISTANCE_DEPENDENT_DIELECTRIC] = 1;
			charmm_->options[CharmmFF::Option::FILENAME] = "CHARMM/param22.ini";
		}

		return *charmm_;
	}

	void MolecularStructure::printAmberResults()
		throw()
	{
		Log.info() << endl;
		Log.info() << "AMBER Energy:" << endl;
		Log.info() << " - electrostatic     : " << getAMBERFF().getESEnergy() << " kJ/mol" << endl;
		Log.info() << " - van der Waals     : " << getAMBERFF().getVdWEnergy() << " kJ/mol" << endl;
		Log.info() << " - bond stretch      : " << getAMBERFF().getStretchEnergy() << " kJ/mol" << endl;
		Log.info() << " - angle bend        : " << getAMBERFF().getBendEnergy() << " kJ/mol" << endl;
		Log.info() << " - torsion           : " << getAMBERFF().getTorsionEnergy() << " kJ/mol" << endl;
		Log.info() << "---------------------------------------" << endl;
		Log.info() << "  total energy       : " << getAMBERFF().getEnergy() << " kJ/mol" << endl;
	}

	void MolecularStructure::fetchPreferences(INIFile& inifile)
		throw()
	{
		minimization_dialog_.readPreferences(inifile);
		md_dialog_.readPreferences(inifile);
		amber_dialog_.fetchPreferences(inifile);
	}


	void MolecularStructure::writePreferences(INIFile& inifile)
		throw()
	{
		minimization_dialog_.writePreferences(inifile);
		md_dialog_.writePreferences(inifile);
		amber_dialog_.writePreferences(inifile);
	}


	void MolecularStructure::assignCharges()
	{
	}

	void MolecularStructure::calculateHBonds()
	{
		if (!getMainControl()->getSelectedSystem())
		{
			setStatusbarText("to calculate H-bonds, one system has to be selected");
			return;
		}

		HBondProcessor proc;
		if (!getMainControl()->getSelectedSystem()) return;
		getMainControl()->getSelectedSystem()->apply(proc);
		getMainControl()->update(*getMainControl()->getSelectedSystem());
		setStatusbarText("calculated H-bonds");
	}

	void MolecularStructure::calculateAmberEnergy()
	{
		System* system = getMainControl()->getSelectedSystem();
		if (system == 0)
		{
			setStatusbarText("to calculate AMBER energies, one system has to be selected");
			return;
		}

		// set up the AMBER force field
		setStatusbarText("setting up force field...");

		AmberFF& amber = getAMBERFF();

		if (!amber.setup(*system))
		{
			Log.error() << "Force field setup failed." << std::endl;
			return;
		}

		// calculate the energy
		setStatusbarText("calculating energy...");

		amber.updateEnergy();

		// print the result
		printAmberResults();
		setStatusbarText("Total AMBER energy: " + String(amber.getEnergy()) + " kJ/mol.");
	}


	void MolecularStructure::amberMinimization()
	{
		if (!getMainControl()->compositesAreMuteable())
		{
			Log.error() << "Simulation already running or still rendering!" << std::endl;
			return;
		}
		// retrieve the system from the selection
		System* system = getMainControl()->getSelectedSystem();
		if (!system) return;

		// execute the minimization dialog
		// and abort if cancel is clicked or nonsense arguments are given
		if (!minimization_dialog_.exec() ||
				!minimization_dialog_.getMaxGradient() ||
				!minimization_dialog_.getEnergyDifference())
		{
			return;
		}
		
		// set up the AMBER force field
		setStatusbarText("setting up force field...");

		AmberFF& amber = getAMBERFF();

		if (!amber.setup(*system))
		{
			Log.error() << "Setup of AMBER force field failed." << endl;
			return;
		}

		// calculate the energy
		setStatusbarText("starting minimization...");

		amber.updateEnergy();

		EnergyMinimizer* minimizer;
		if (minimization_dialog_.getUseConjugateGradient())	minimizer = new ConjugateGradientMinimizer;
		else 																								minimizer = new SteepestDescentMinimizer;

		// set the minimizer options
		minimizer->options[EnergyMinimizer::Option::MAXIMAL_NUMBER_OF_ITERATIONS] = minimization_dialog_.getMaxIterations();
		minimizer->options[EnergyMinimizer::Option::MAX_GRADIENT] = minimization_dialog_.getMaxGradient();
		minimizer->options[EnergyMinimizer::Option::ENERGY_DIFFERENCE_BOUND] = minimization_dialog_.getEnergyDifference();
		minimizer->options[EnergyMinimizer::Option::ENERGY_OUTPUT_FREQUENCY] = 999999999;
		minimizer->setup(amber);
		minimizer->setMaxNumberOfIterations(minimization_dialog_.getMaxIterations());

		try
		{
			// perform an initial step (no restart step)
			minimizer->minimize(1, false);

			// ============================= WITH MULTITHREADING ====================================
	#ifdef BALL_QT_HAS_THREADS
			EnergyMinimizerThread* thread = new EnergyMinimizerThread;
			getMainControl()->setSimulationThread(thread);

			thread->setEnergyMinimizer(minimizer);
			thread->setNumberOfStepsBetweenUpdates(minimization_dialog_.getRefresh());
			thread->setComposite(system);

		#if BALL_QT_VERSION >=	0x030200
				thread->start(QThread::LowPriority);
		#else
				thread->start();
		#endif
			
	#else
			// ============================= WITHOUT MULTITHREADING =================================
			// iterate until done and refresh the screen every "steps" iterations
			while (!minimizer->minimize(minimization_dialog_.getRefresh(), true) &&
							minimizer->getNumberOfIterations() < minimizer->getMaxNumberOfIterations())
			{
				getMainControl()->update(*system);

				QString message;
				message.sprintf("Iteration %d: energy = %f kJ/mol, RMS gradient = %f kJ/mol A",
												minimizer->getNumberOfIterations(), amber.getEnergy(), amber.getRMSGradient());
				setStatusbarText(String(message.ascii()));
			}

			Log.info() << endl << "minimization terminated." << endl << endl;
			printAmberResults();
			Log.info() << "final RMS gadient    : " << amber.getRMSGradient() << " kJ/(mol A)   after "
								 << minimizer->getNumberOfIterations() << " iterations" << endl << endl;
			setStatusbarText("Total AMBER energy: " + String(amber.getEnergy()) + " kJ/mol.");

			// clean up
			delete minimizer;
	#endif
		}
		catch(Exception::GeneralException e)
		{
			String txt = "Calculation aborted because of throwed exception";
			setStatusbarText(txt + ". See Logs");
			Log.error() << txt << ":" << std::endl;
			Log.error() << e << std::endl;
		}
	}

	void MolecularStructure::amberMDSimulation()
	{
 		if (!getMainControl()->compositesAreMuteable())
		{
			Log.error() << "Simulation already running or still rendering!" << std::endl;
			return;
		}
		// retrieve the system from the selection
		System* system = getMainControl()->getSelectedSystem();

		// execute the MD simulation dialog
		// and abort if cancel is clicked or nonsense arguments given
		if (system == 0||
				!md_dialog_.exec() ||
				!md_dialog_.getSimulationTime() ||
				!md_dialog_.getTemperature())
		{
			return;
		}
		
		AmberFF& amber = getAMBERFF();

		// set up the AMBER force field
		setStatusbarText("setting up force field...");

	#ifdef BALL_VIEW_DEBUG
		amber.options.dump();
	#endif

		if (!amber.setup(*system))
		{
			Log.error() << "Setup of AMBER force field failed." << endl;
			return;
		}

		// calculate the energy
		setStatusbarText("starting simulation...");

		amber.updateEnergy();

		MolecularDynamics* mds = 0;
		if (md_dialog_.useMicroCanonical()) mds = new CanonicalMD;
		else 																mds = new MicroCanonicalMD;
		
		// set the options for the MDS	
		Options options;
		options[MolecularDynamics::Option::ENERGY_OUTPUT_FREQUENCY] = 99999999;
		options[MolecularDynamics::Option::TIME_STEP] = md_dialog_.getTimeStep();

		try
		{
			// setup the simulation
			mds->setup(amber, 0, options);
			if (!mds->isValid())
			{
				Log.error() << "Setup for MD simulation failed!" << std::endl;
				return;
			}
			
			// perform an initial step (no restart step)
			mds->simulateIterations(1, false);

			// we update everything every so and so steps
			Size steps = md_dialog_.getStepsBetweenRefreshs();

			DCDFile* dcd = 0;
			if (md_dialog_.getDCDFile().size() != 0) 
			{
				dcd = new DCDFile;
				dcd->open(md_dialog_.getDCDFile(), File::OUT);
				dcd->enableVelocityStorage();
			}
			// ============================= WITH MULTITHREADING ===================================
		#ifdef BALL_QT_HAS_THREADS
			MDSimulationThread* thread = new MDSimulationThread;
			getMainControl()->setSimulationThread(thread);

			thread->setMolecularDynamics(mds);
			thread->setNumberOfSteps(md_dialog_.getNumberOfSteps());
			thread->setNumberOfStepsBetweenUpdates(steps);
			thread->setSaveImages(md_dialog_.saveImages());
			thread->setDCDFile(dcd);
			thread->setComposite(system);

			#if BALL_QT_VERSION >=	0x030200
				thread->start(QThread::LowPriority);
			#else
				thread->start();
			#endif

		#else
			// ============================= WITHOUT MULTITHREADING ==============================
			// iterate until done and refresh the screen every "steps" iterations
			
			SnapShotManager manager(amber.getSystem(), &amber, dcd);
			manager.setFlushToDiskFrequency(10);
			while (mds->getNumberOfIterations() < md_dialog_.getNumberOfSteps())
			{
				mds->simulateIterations(steps, true);
				getMainControl()->update(*system);
				if (md_dialog_.saveImages()) 
				{
					SceneMessage* msg = new SceneMessage(SceneMessage::EXPORT_PNG);
					notify_(msg);
				}
				
				if (dcd != 0) 
				{
					manager.takeSnapShot();
				}

				QString message;
				message.sprintf("Iteration %d: energy = %f kJ/mol, RMS gradient = %f kJ/mol A", 
												mds->getNumberOfIterations(), amber.getEnergy(), amber.getRMSGradient());
				setStatusbarText(String(message.ascii()));
			}

			if (dcd) manager.flushToDisk();

			Log.info() << std::endl << "simulation terminated." << std::endl << endl;
			printAmberResults();
			Log.info() << "final RMS gadient    : " << amber.getRMSGradient() << " kJ/(mol A)   after " 
								 << mds->getNumberOfIterations() << " iterations" << endl << endl;
			setStatusbarText("Total AMBER energy: " + String(amber.getEnergy()) + " kJ/mol.");

			// clean up
			delete mds;

			if (dcd != 0)
			{
				dcd->close();
				delete dcd;
				dcd = new DCDFile(md_dialog_.getDCDFile(), File::IN);

				NewTrajectoryMessage* message = new NewTrajectoryMessage;
				message->setComposite(*amber.getSystem());
				message->setTrajectoryFile(*dcd);
				notify_(message);
			}
		#endif
		}
		catch(Exception::GeneralException e)
		{
			String txt = "Calculation aborted because of throwed exception";
			setStatusbarText(txt + ". See Logs");
			Log.error() << txt << ":" << std::endl;
			Log.error() << e << std::endl;
		}
	}


	void MolecularStructure::buildPeptide()
	{
		PeptideDialog* dialog = new PeptideDialog;
		dialog->exec();

		Protein* protein = dialog->getProtein();
		if (protein == 0) return;

		System* system = new System;
		system->insert(*protein);
		getMainControl()->insert(*system, dialog->getSequence());
	}

	void MolecularStructure::showForceFieldOptions()
	{
		amber_dialog_.raise();
		amber_dialog_.show();
	}

} } // namespaces
