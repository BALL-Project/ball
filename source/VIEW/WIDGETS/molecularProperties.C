// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: molecularProperties.C,v 1.21 2004/01/20 15:08:35 amoll Exp $

#include <BALL/VIEW/WIDGETS/molecularProperties.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/STRUCTURE/residueChecker.h>
#include <BALL/STRUCTURE/geometricProperties.h>
#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/STRUCTURE/secondaryStructureProcessor.h>
#include <BALL/KERNEL/system.h>
#include <BALL/STRUCTURE/structureMapper.h>
#include <BALL/MOLMEC/CHARMM/charmm.h>
#include <BALL/MOLMEC/AMBER/amber.h>

#include <qmenubar.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

MolecularProperties::MolecularProperties(QWidget* parent, const char* name)
	throw()
	:	QWidget(parent, name),
		ModularWidget(name),
		view_distance_(25),
		amber_(0),
		charmm_(0),
		amber_dialog_(this)
{
	registerWidget(this);
	hide();

	// cant use ModularWidget::getMainControl() here, no idea why
	MainControl& main_control = *MainControl::getMainControl(this);
	String hint;
	
	hint = "Focus the camera on one or multiple objects."; 
	center_camera_id_ = main_control.insertMenuEntry(MainControl::DISPLAY_VIEWPOINT, "&Focus Camera", this, 
																										SLOT(centerCamera()), CTRL+Key_F, -1, hint);
	
	hint = "Add missing bonds to a selected structure.";
	build_bonds_id_ = main_control.insertMenuEntry(MainControl::BUILD, "&Build Bonds", this, 
																										SLOT(buildBonds()), CTRL+Key_B, -1, hint);
	
	hint = "Add missing H-atoms to a selected structure."; 
	add_hydrogens_id_ = main_control.insertMenuEntry(MainControl::BUILD, "Add &Hydrogens", this, 
																										SLOT(addHydrogens()), CTRL+Key_H, -1, hint);
	
 	hint = "Check a structure against the fragment database.";
	check_structure_id_ = main_control.insertMenuEntry(MainControl::BUILD, "Chec&k Structure", this, 
																										SLOT(checkResidue()), CTRL+Key_K, -1, hint);
	
	hint = "Recalculate the secondary structure for a structure.";
	calculate_ss_ = main_control.insertMenuEntry(MainControl::BUILD, "Calculate Secondary Structure", this,
																							 SLOT(calculateSecondaryStructure()), ALT+Key_2, -1, hint);
	
	hint = "Select a molecular object to see its position in the scene or to mark it for a simulation";
	select_id_ = main_control.insertMenuEntry(MainControl::EDIT, "&Select", this, 
																										SLOT(select()), ALT+Key_S, -1, hint);   
	
	hint = "Deselect a molecular object.";
	deselect_id_ = main_control.insertMenuEntry(MainControl::EDIT, "&Deselect", this, 
																										SLOT(deselect()), ALT+Key_D, -1, hint);
	
	hint = "Create a grid with the distance to the geometric center of a structure.";
	create_distance_grid_id_ = main_control.insertMenuEntry(MainControl::TOOLS_CREATE_GRID, 
																			"&Distance Grid", this, SLOT(createGridFromDistance()), 
																			0, -1, hint);   

	hint = "Map two proteins.";
	map_proteins_ = main_control.insertMenuEntry(MainControl::TOOLS,
																			"&Map two Proteins", this, SLOT(mapProteins()), 0, -1, hint);
}

MolecularProperties::~MolecularProperties()
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

void MolecularProperties::onNotify(Message *message)
	throw()
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "MolecularProperties " << this  << "onNotify " << message << std::endl;
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


bool MolecularProperties::checkResidue()
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


void MolecularProperties::addHydrogens()
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


void MolecularProperties::buildBonds()
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


void MolecularProperties::centerCamera(Composite* composite)
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
	List<Composite*>& selection = getMainControl()->getMolecularControlSelection();
	Size number_of_selected_objects = selection.size(); 
	bool selected = (number_of_selected_objects != 0);
	selected = selected && getMainControl()->compositesAreMuteable();

	(main_control.menuBar())->setItemEnabled(add_hydrogens_id_, selected);
	(main_control.menuBar())->setItemEnabled(build_bonds_id_, selected);
	(main_control.menuBar())->setItemEnabled(check_structure_id_, selected);

	(main_control.menuBar())->setItemEnabled(calculate_ss_, getMainControl()->getSelectedSystem() && 
																					 								getMainControl()->compositesAreMuteable());

	// these menu point for single items only
	(main_control.menuBar()) ->setItemEnabled(center_camera_id_, number_of_selected_objects == 1);

	(main_control.menuBar()) ->setItemEnabled(map_proteins_, number_of_selected_objects == 2);

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
	
	main_control.menuBar()->setItemEnabled(select_id_, allow_select);
	main_control.menuBar()->setItemEnabled(deselect_id_, allow_deselect);
	main_control.menuBar()->setItemEnabled(create_distance_grid_id_, main_control.getSelectedSystem());
}


void MolecularProperties::select()
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


void MolecularProperties::deselect()
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


void MolecularProperties::addComposite_(Composite& composite, const String& name)
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
	
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "finished applying molecular properties" << std::endl;
	#endif
	// continue with molecular message
	CompositeMessage* mol_message = new CompositeMessage(composite, CompositeMessage::NEW_MOLECULE);
	notify_(mol_message);
}


void MolecularProperties::createGridFromDistance()
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
}

void MolecularProperties::calculateSecondaryStructure()
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


void MolecularProperties::mapProteins()
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
	a2->apply(sm);

	CompositeMessage* cm =
			new CompositeMessage(*a1, CompositeMessage::CHANGED_COMPOSITE);
	notify_(cm);

	Log.info() << "Calcuted RMSD: " << rmsd << std::endl;
	Log.info() << "Calcuted Transformation: " << m<< std::endl;
	Log.info() << "Not matched CA: " << not_matched_ca << std::endl << std::endl;
	setStatusbarText("Calcuted RMSD: " + String(rmsd));
}

AmberFF& MolecularProperties::getAMBERFF()
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

AmberConfigurationDialog& MolecularProperties::getAmberConfigurationDialog()
	throw()
{
	return amber_dialog_;
}
	

CharmmFF& MolecularProperties::getCHARMMFF()
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

void MolecularProperties::printAmberResults()
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

void MolecularProperties::fetchPreferences(INIFile& inifile)
	throw()
{
	amber_dialog_.fetchPreferences(inifile);
}


void MolecularProperties::writePreferences(INIFile& inifile)
	throw()
{
	amber_dialog_.writePreferences(inifile);
}

} } // namespaces
