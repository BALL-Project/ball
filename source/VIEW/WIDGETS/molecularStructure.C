// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/WIDGETS/molecularStructure.h>
#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/DIALOGS/peptideDialog.h>
#include <BALL/VIEW/DIALOGS/generateCrystalDialog.h>
#include <BALL/VIEW/DIALOGS/FDPBDialog.h>

#include <BALL/VIEW/DATATYPE/standardDatasets.h>

#include <BALL/STRUCTURE/residueChecker.h>
#include <BALL/STRUCTURE/geometricProperties.h>
#include <BALL/STRUCTURE/peptideCapProcessor.h>
#include <BALL/STRUCTURE/secondaryStructureProcessor.h>
#include <BALL/STRUCTURE/structureMapper.h>
#include <BALL/STRUCTURE/HBondProcessor.h>

#include <BALL/FORMAT/trajectoryFileFactory.h>
#include <BALL/FORMAT/trajectoryFile.h>
#include <BALL/FORMAT/DCDFile.h>

#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/SYSTEM/directory.h>

#include <BALL/MOLMEC/MINIMIZATION/conjugateGradient.h>
#include <BALL/MOLMEC/MINIMIZATION/steepestDescent.h>
#include <BALL/MOLMEC/MINIMIZATION/strangLBFGS.h>
#include <BALL/MOLMEC/MINIMIZATION/shiftedLVMM.h>
#include <BALL/MOLMEC/MDSIMULATION/microCanonicalMD.h>
#include <BALL/MOLMEC/MDSIMULATION/canonicalMD.h>
#include <BALL/MOLMEC/COMMON/snapShotManager.h>

#include <BALL/VIEW/KERNEL/threads.h>

#include <QtCore/QDir>
#include <QtWidgets/QMenuBar>

#include <sstream>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

		MolecularStructure::MolecularStructure(QWidget* parent, const char* name)
			:	QWidget(parent),
				ModularWidget(name),
				amber_(),
				charmm_(),
				amber_dialog_(parent),
				charmm_dialog_(parent),
				minimization_dialog_(parent),
				md_dialog_(parent),
				fdpb_dialog_(0),
				bond_order_dialog_(parent),
				bond_order_results_dialog_(parent),
				crystal_dialog_(0)
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "New MolecularStructure " << this << std::endl;
			#endif
			setObjectName(name);
			registerWidget(this);
			hide();
		}


		void MolecularStructure::initializeWidget(MainControl&)
		{
			// cant use ModularWidget::getMainControl() here, no idea why
			String description = "Shortcut|Display|Viewpoint|Focus_Camera";
			center_camera_id_  = insertMenuEntry(MainControl::DISPLAY_VIEWPOINT, tr("&Focus Camera"), this, 
																					 SLOT(centerCamera()), description, QKeySequence("Ctrl+F"),
			                                     tr("Focus the camera on one or multiple objects."), 
																					 UIOperationMode::MODE_ADVANCED);
			setIcon(center_camera_id_, "actions/object-focus", true);

			// Build Menu -------------------------------------------------------------------
	// 		hint = "To assign charges, one System has to be selected.";
	// 		assign_charges_id_ = insertMenuEntry(MainControl::BUILD, "Assign Char&ges", this, SLOT(assignCharges()),
	// 										CTRL+Key_G, -1 , hint);

			description = "Shortcut|Build|Build_Peptide";
			build_peptide_id_ = insertMenuEntry(MainControl::BUILD, tr("B&uild Peptide"), this, 
			                                    SLOT(buildPeptide()), description, QKeySequence("Ctrl+Alt+U"),
																					tr("Build a peptide from selected amino acids."), 
																					UIOperationMode::MODE_ADVANCED);

			description = "Shortcut|Build|Generate_Crystal";
			build_bonds_id_ = insertMenuEntry(MainControl::BUILD, tr("&Generate Crystal"), this, 
																				SLOT(generateCrystal()), description, QKeySequence("Ctrl+Alt+G"), 
																				tr("Generate a crystal packing."), 
																				UIOperationMode::MODE_ADVANCED);
			
			description = "Shortcut|Build|Build_Bonds";
			build_bonds_id_ = insertMenuEntry(MainControl::BUILD, tr("&Build Bonds"), this, 
																				SLOT(buildBonds()), description, QKeySequence("Ctrl+B"), 
																				tr("Add missing bonds to a selected structure."), 
																				UIOperationMode::MODE_ADVANCED);

			description = "Shortcut|Build|Add_End_Caps";
			build_endcaps_id_ = insertMenuEntry(MainControl::BUILD, tr("Add Peptide Endcaps"), this,
			                                    SLOT(buildEndcaps()), description, QKeySequence(), 
                                          tr("Add neutral end caps to an amino acid chain"),
                                          UIOperationMode::MODE_ADVANCED);

			description = "Shortcut|Build|Assign_Bond_Orders";
			assign_bond_orders_id_ = insertMenuEntry(MainControl::BUILD, tr("&Assign Bond Orders"), this, 
																							 SLOT(runBondOrderAssignment()), description, QKeySequence(),
																							 tr("Assign bond orders to a selected structure."), 
																							 UIOperationMode::MODE_ADVANCED); 
			setIcon(assign_bond_orders_id_, "actions/molecule-assign-bond-orders", false);

			description = "Shortcut|Build|Check_Structure";
			check_structure_id_ = insertMenuEntry(MainControl::BUILD, tr("Chec&k Structure against FragmentDB"), this, 
																						SLOT(checkResidue()), description, QKeySequence("Ctrl+K"),
																						tr("Check a structure against the fragment database."), 
																						UIOperationMode::MODE_ADVANCED);

			description = "Shortcut|Build|Add_Hydrogens";
			add_hydrogens_id_ = insertMenuEntry(MainControl::BUILD, tr("Add &Hydrogens"), this, 
																					SLOT(addHydrogens()), description, QKeySequence("Ctrl+H"),
																					tr("Add missing atoms to a selected structure by using the fragment database."),
																					UIOperationMode::MODE_ADVANCED);

			// MOLECULARMECHANICS Menu -------------------------------------------------------------------

			description = "Shortcut|MolecularMechanics|Single_Point_Calculation";
			energy_id_ = insertMenuEntry(MainControl::MOLECULARMECHANICS, tr("Single Point Calculation"), this, 
																	 SLOT(calculateForceFieldEnergy()), description, QKeySequence("Ctrl+A"),
			                             tr("Calculate the energy of a System with the selected force field."),
																	 UIOperationMode::MODE_ADVANCED);
			setMenuHelp(energy_id_, "mm.html#single_point");

			description = "Shortcut|MolecularMechanics|Energy_Minimization";
			minimization_id_ = insertMenuEntry(MainControl::MOLECULARMECHANICS, tr("&Energy Minimization"), this, 
																				 SLOT(runMinimization()), description, QKeySequence(),
																				 tr("To perform an Energy Minimization, first select the molecular structures."),
																				 UIOperationMode::MODE_ADVANCED);
			setMenuHelp(minimization_id_, "mm.html#mini");

			description = "Shortcut|MolecularMechanics|Molecular_Dynamics";
			mdsimulation_id_ = insertMenuEntry(MainControl::MOLECULARMECHANICS, tr("Molecular &Dynamics"), this, 
																				 SLOT(MDSimulation()), description, QKeySequence("Ctrl+M"),
																				 tr("To perform a MD simulation , first select the molecular structures."),
																				 UIOperationMode::MODE_ADVANCED);
			setMenuHelp(mdsimulation_id_, "mm.html#md");

			getMainControl()->insertPopupMenuSeparator(MainControl::MOLECULARMECHANICS, UIOperationMode::MODE_ADVANCED);

			description = "Shortcut|MolecularMechanics|Choose_ForceField|Amber";
			amber_ff_id_ = insertMenuEntry(MainControl::CHOOSE_FF, tr("Amber"), this, 
																	   SLOT(chooseAmberFF()), description, QKeySequence(),
																		 tr("Use Amber Force Field"),
																		 UIOperationMode::MODE_ADVANCED);
			if (amber_ff_id_)
			{
				amber_ff_id_->setCheckable(true);
				setMenuHelp(amber_ff_id_, "mm.html");
			}

			description = "Shortcut|MolecularMechanics|Choose_ForceField|Charmm";
			charmm_ff_id_ = insertMenuEntry(MainControl::CHOOSE_FF, tr("Charmm"), this, 
																			SLOT(chooseCharmmFF()), description, QKeySequence(),
			                                tr("Use Charmm Force Field"),
																			UIOperationMode::MODE_ADVANCED);
			if (charmm_ff_id_)
			{
				charmm_ff_id_->setCheckable(true);
				setMenuHelp(charmm_ff_id_, "mm.html");
			}

			description = "Shortcut|MolecularMechanics|Choose_ForceField|MMFF94";
			mmff94_id_ = insertMenuEntry(MainControl::CHOOSE_FF, tr("MMFF94"), this, 
																	 SLOT(chooseMMFF94()), description, QKeySequence(),
																	 tr("Use MMFF94 Force Field"),
																	 UIOperationMode::MODE_ADVANCED);
			if (mmff94_id_)
			{
				mmff94_id_->setCheckable(true);
				setMenuHelp(mmff94_id_, "mm.html");
			}

			description = "Shortcut|MolecularMechanics|Options";
			setup_ff_ = insertMenuEntry(MainControl::MOLECULARMECHANICS, tr("Force Field Options"), this,
																	SLOT(setupForceField()), description, QKeySequence(), 
																	tr("Configure the force field"),
																	UIOperationMode::MODE_ADVANCED);
			setMenuHelp(setup_ff_, "mm.html");

			// Tools Menu -------------------------------------------------------------------
			getMainControl()->insertPopupMenuSeparator(MainControl::TOOLS, UIOperationMode::MODE_ADVANCED);
//			hint = " Map two proteins.";
// 			map_proteins_id_ = insertMenuEntry(MainControl::TOOLS, "&Map two Proteins", this, SLOT(mapProteins()), 0, -1, hint);

			description = "Shortcut|Tools|Calculate_RMSD";
			calculate_RMSD_id_ = insertMenuEntry(MainControl::TOOLS, tr("&Calculate RMSD"), this, 
																					 SLOT(calculateRMSD()), description, QKeySequence(),
																					 tr("Highlight two (partial) structures to calculate their RMSD value"),
																					 UIOperationMode::MODE_ADVANCED);
			
			description = "Shortcut|Tools|Calculate_SecondaryStructure";
			calculate_ss_id_ = insertMenuEntry(MainControl::TOOLS, tr("Calculate Sec&ondary Structure"), this,
																				 SLOT(calculateSecondaryStructure()), description, QKeySequence("Alt+O"),
																				 tr("Recalculate the secondary structure for a structure"),
																				 UIOperationMode::MODE_ADVANCED);

//   			calculate_ramachandran_ = insertMenuEntry(MainControl::TOOLS, "Ramachandran Plot", this,
//   																				 SLOT(calculateRamachandranPlot()));
//   			setMenuHint("Calculate a Ramachandran Plot for a Protein.");
			
			description = "Shortcut|Tools|Calculate_HBonds";
			calculate_hbonds_id_ = insertMenuEntry(MainControl::TOOLS, tr("Calculate H-B&onds"), this, 
			                                       SLOT(calculateHBonds()), description, QKeySequence("Alt+N"),
			                                       tr("To assign H-bonds, one System has to be selected."),
																						 UIOperationMode::MODE_ADVANCED);

			description = "Shortcut|Tools|FDPB";
			menu_FPDB_ = insertMenuEntry(MainControl::TOOLS, tr("FDPB Electrostatics"), this, 
																	 SLOT(calculateFDPB()), description, QKeySequence(),
																	 tr("Calculate the Electrostatics with FDPB (for one highlighted System)"),
																	 UIOperationMode::MODE_ADVANCED);
				
			description = "Shortcut|Tools|Grid|Create_Distance_Grid";
			create_distance_grid_id_ = insertMenuEntry(MainControl::TOOLS_GRID, tr("Create &Distance Grid"), this, 
																								 SLOT(createGridFromDistance()), description, QKeySequence(),
			                                           tr("Create a grid with the distance to the geometric center of a structure"),
																								 UIOperationMode::MODE_ADVANCED);
			setMenuHelp(create_distance_grid_id_, "tips.html#distance_grids");

			description = "Shortcut|Tools|Grid|Create_Distance_From_Camera_Grid";
			create_distance_grid_id2_ = insertMenuEntry(MainControl::TOOLS_GRID, tr("Create &Distance from Camera Grid"), this, 
																									SLOT(createGridFromCameraDistance()), description, QKeySequence(),
																									tr("Create a grid with the distance to the view point"),
																									UIOperationMode::MODE_ADVANCED);
			setMenuHelp(create_distance_grid_id2_, "tips.html#distance_grids");

			minimization_dialog_.setAmberDialog(&amber_dialog_);
			minimization_dialog_.setCharmmDialog(&charmm_dialog_);
			minimization_dialog_.setMMFF94Dialog(&mmff94_dialog_);
			md_dialog_.setAmberDialog(&amber_dialog_);
			md_dialog_.setCharmmDialog(&charmm_dialog_);
			md_dialog_.setMMFF94Dialog(&mmff94_dialog_);

			// Assign the settings of the configuration dialogs to the 
			// force fields.
			amber_dialog_.setAmberFF(amber_);
			charmm_dialog_.setCharmmFF(charmm_);
			mmff94_dialog_.setMMFF94(mmff_);

			applyForceFieldSettings_();
			// use amber force field by default
			chooseAmberFF();
		}

		MolecularStructure::~MolecularStructure()
		{
		}


		MolecularStructure::MolecularStructure(const MolecularStructure& m)
		 : QWidget(),
			 ModularWidget()
		{
			// prevent warnings
			m.isValid();
		}


		void MolecularStructure::onNotify(Message *message)
		{
	#ifdef BALL_VIEW_DEBUG
			Log.error() << "MolecularStructure " << this  << "onNotify " << message << std::endl;
	#endif
            if (RTTI::isKindOf<ControlSelectionMessage>(message))
			{
				checkMenu(*getMainControl());
			}
            else if (RTTI::isKindOf<CompositeMessage>(message))
			{
				CompositeMessage* cmessage = RTTI::castTo<CompositeMessage>(*message);
				switch (cmessage->getType())
				{
					case CompositeMessage::NEW_COMPOSITE:
						addComposite_(*cmessage->getComposite(), cmessage->getCompositeName(), cmessage->data().empty() ? true : boost::any_cast<bool>(cmessage->data()));
						return;
					case CompositeMessage::CENTER_CAMERA:
						centerCamera(cmessage->getComposite());
						return;

					default:
						return;
				}
			}
            else if (RTTI::isKindOf<MolecularTaskMessage>(message))
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
						Log.error() << (String)tr("Unknown type of MolecularTaskMessage in") << " "
							          << __FILE__ << "  " << __LINE__ << std::endl;
				}
			}
		}


		bool MolecularStructure::checkResidue()
		{
			// selection can change
			list<Composite*> selection = getMainControl()->getMolecularControlSelection();
			if (selection.size() == 0)
			{
				return false;
			}

			setStatusbarText((String)tr("checking") + " " + String(selection.size()) + " " +  (String)tr("objects") + "...", true);

			ResidueChecker res_check(getFragmentDB());
			res_check.disable(ResidueChecker::OVERLAPPING_ATOMS);
			res_check.enableSelection();

			bool okay = true;

			HashSet<Composite*> changed_roots;
			list<Composite*>::const_iterator it = selection.begin();
			for (; it != selection.end(); ++it)
			{
                if (!RTTI::isKindOf<AtomContainer>(*it))
				{
					Log.error() << (String)tr("ResidueChecker: cannot apply to a") << " " << typeid(**it).name()
						          << " " << (String)tr("object") << std::endl;
					continue;
				}

				(*it)->apply(res_check);
				okay &= res_check.getStatus();
				if (!res_check.getStatus())
				{
					changed_roots.insert(&(**it).getRoot());
					if ((**it).isSelected())
					{
						CompositeMessage* msg = new CompositeMessage(**it, CompositeMessage::SELECTED_COMPOSITE);
						msg->setUpdateRepresentations(false);
						notify_(msg);
					}
					else
					{
						AtomIterator ait = (*(AtomContainer*)*it).beginAtom();
						for (; +ait; ait++)
						{
							if (!(*ait).isSelected()) continue;

							CompositeMessage* msg = new CompositeMessage(*ait, CompositeMessage::SELECTED_COMPOSITE);
							msg->setUpdateRepresentations(false);
							notify_(msg);
						}
					}
				}
			}

			if (okay)
			{
				setStatusbarText((String)tr("ResidueChecker: no errors found."), true);
			}
			else
			{
				setStatusbarText((String)tr("Errors found in molecule, the problematic atoms are now selected and colored! See also logs"), true);
				HashSet<Composite*>::Iterator it = changed_roots.begin();
				for (; it != changed_roots.end(); it++)
				{
					notify_(new CompositeMessage(**it, CompositeMessage::CHANGED_COMPOSITE));
				}
			}

			return okay;
		}


		void MolecularStructure::addHydrogens()
		{
			if (getMainControl()->getMolecularControlSelection().size() == 0)
			{
				return;
			}

			setStatusbarText((String)tr("  > adding hydrogens ..."), true);

			// copy the selection_, it can change after a changemessage event
			list<Composite*> temp_selection_ = getMainControl()->getMolecularControlSelection();
			list<Composite*>::const_iterator it = temp_selection_.begin();

			Size number_of_hydrogens = 0;

			bool hydrogen_ok = true;
			bool bond_ok = true;

			for (; it != temp_selection_.end(); ++it)
			{
				hydrogen_ok &= (*it)->apply(getFragmentDB().add_hydrogens);
				number_of_hydrogens += getFragmentDB().add_hydrogens.getNumberOfInsertedAtoms();

				if (getFragmentDB().add_hydrogens.getNumberOfInsertedAtoms() == 0) continue;

				bond_ok &= (*it)->apply(getFragmentDB().build_bonds);

				CompositeMessage *change_message =
					new CompositeMessage(**it, CompositeMessage::CHANGED_COMPOSITE_HIERARCHY);
				notify_(change_message);
			}

			String result = String(tr("added ")) + String(number_of_hydrogens) + " " + (String)tr("hydrogen atoms.");

			if (!bond_ok)
			{
				result += " " + (String)tr("An error occured, while adding the bonds. Too many bonds for one atom?");
			}

			if (!hydrogen_ok)
			{
				result += " " + (String)tr("An error occured, while adding the hydrogens. Too many bonds for one atom?");
			}

			setStatusbarText(result, true);
		}


		void MolecularStructure::buildBonds()
		{
			if (getMainControl()->getMolecularControlSelection().size() == 0)
			{
				return;
			}

			setStatusbarText((String)tr("  > building bonds ..."), true);

			// copy the selection_, it can change after a changemessage event
			list<Composite*> temp_selection_ = getMainControl()->getMolecularControlSelection();
			list<Composite*>::const_iterator it = temp_selection_.begin();

			Size old_number_of_bonds = 0;

			HashSet<System*> roots;

			for (; it != temp_selection_.end(); ++it)
			{
				if (!roots.has((System*)&(**it).getRoot()))
				{
					old_number_of_bonds += (((AtomContainer*)&(**it).getRoot()))->countBonds();
					roots.insert((System*)&(**it).getRoot());
				}
			}

			bool ok = true;

			for (it = temp_selection_.begin(); it != temp_selection_.end(); ++it)
			{
				ok &= (*it)->apply(getFragmentDB().build_bonds);

				CompositeMessage *change_message =
					new CompositeMessage(**it, CompositeMessage::CHANGED_COMPOSITE_HIERARCHY);
				notify_(change_message);
			}

			Size new_number_of_bonds = 0;
			for (HashSet<System*>::iterator sit = roots.begin(); sit != roots.end(); sit++)
			{
				new_number_of_bonds += ((**sit).countBonds());
			}

			String result = (String)tr("  > added ") + String(new_number_of_bonds - old_number_of_bonds) +
				(String)tr(" bonds") + " (" + (String)tr("total") + " " + String(new_number_of_bonds) + ").";

			if (!ok) result +=  " " + (String)tr("An error occured. Too many bonds for one atom?");
			setStatusbarText(result, true);
		}


		void MolecularStructure::buildEndcaps()
		{
			if (getMainControl()->getMolecularControlSelection().size() == 0)
			{
				setStatusbarText((String)tr("Please highlight a peptide!"), true);

				return;
			}

			//setStatusbarText((String)tr("adding neutral end caps..."), true);

			// copy the selection_, it can change after a changemessage event
			list<Composite*> temp_selection_ = getMainControl()->getMolecularControlSelection();
			list<Composite*>::const_iterator it = temp_selection_.begin();

			bool ok = true;
			PeptideCapProcessor pcp;

			for (; it != temp_selection_.end(); ++it)
			{
				ok &= (*it)->apply(pcp);

				CompositeMessage *change_message =
					new CompositeMessage(**it, CompositeMessage::CHANGED_COMPOSITE_HIERARCHY);
				notify_(change_message);
			}

			String result = (String)tr("  > added end caps");

			if (!ok) result +=  " " + (String)tr("An error occured.");

			setStatusbarText(result, true);
		}


		void MolecularStructure::runBondOrderAssignment(bool show_dialog)
		{
			// Make sure we run one instance of a assignment at a time only.
			if (getMainControl()->isBusy())
			{
				Log.error() << (String)tr("Assignment already running or still rendering!") << std::endl;
				return;
			}

			if (getMainControl()->getMolecularControlSelection().size() == 0)
			{
				setStatusbarText((String)tr("Please highlight exactly one AtomContainer!"), true);
				return;
			}

			setStatusbarText("  > " + (String)tr("  > assigning bond orders") + " ...", true);

			// Retrieve the selected atom container and abort if nothing is selected.
			list<AtomContainer*> containers;
			list<Composite*> highl = getMainControl()->getMolecularControlSelection();
			list<Composite*>::iterator lit = highl.begin();
			for (; lit != highl.end(); ++lit)
			{
				AtomContainer* ac = dynamic_cast<AtomContainer*>(*lit);
				if (ac != 0) containers.push_back(ac);
			}

			if (containers.size() != 1)
			{
				setStatusbarText((String)tr("Please highlight exactly one AtomContainer!"), true);
				return;
			}

			if (show_dialog)
			{
				// Execute the assign bond order dialog
				// and abort if cancel is clicked or nonsense arguments are given
				if (!bond_order_dialog_.exec())
				{
					return;
				}
			}

			AssignBondOrderProcessor abop;

			// read the options
			abop.options[AssignBondOrderProcessor::Option::OVERWRITE_SINGLE_BOND_ORDERS] = bond_order_dialog_.overwrite_singleBO_box->isChecked();
			abop.options[AssignBondOrderProcessor::Option::OVERWRITE_DOUBLE_BOND_ORDERS] = bond_order_dialog_.overwrite_doubleBO_box->isChecked();
			abop.options[AssignBondOrderProcessor::Option::OVERWRITE_TRIPLE_BOND_ORDERS] = bond_order_dialog_.overwrite_tripleBO_box->isChecked();
			abop.options[AssignBondOrderProcessor::Option::OVERWRITE_SELECTED_BONDS]     = bond_order_dialog_.overwrite_selected_bonds_box->isChecked();
			abop.options[AssignBondOrderProcessor::Option::KEKULIZE_RINGS]               = bond_order_dialog_.kekulizeBonds_button->isChecked();
			abop.options[AssignBondOrderProcessor::Option::ADD_HYDROGENS]                = bond_order_dialog_.add_hydrogens_checkBox->isChecked();
			abop.options[AssignBondOrderProcessor::Option::BOND_LENGTH_WEIGHTING]        = (bond_order_dialog_.penalty_balance_slider->value()/100.);

			if (bond_order_dialog_.ILP_button->isChecked())
			{
				abop.options[AssignBondOrderProcessor::Option::ALGORITHM] = AssignBondOrderProcessor::Algorithm::ILP;
			}
			else if (bond_order_dialog_.ASTAR_button->isChecked())
			{
				abop.options[AssignBondOrderProcessor::Option::ALGORITHM] = AssignBondOrderProcessor::Algorithm::A_STAR;
			}
			else
			{
				abop.options[AssignBondOrderProcessor::Option::ALGORITHM] = AssignBondOrderProcessor::Algorithm::FPT;
			}

			// automatically applying a solution might confuse the user --> set to false
			abop.options.setBool(AssignBondOrderProcessor::Option::APPLY_FIRST_SOLUTION, false);

			// get the parameter folder
			//
			// does the given INIFile exist?
			String param_edit_value = ascii(bond_order_dialog_.parameter_file_edit->text());

			Directory param_dir(FileSystem::path(param_edit_value));
			if (param_dir.isValid() && param_dir.has(FileSystem::baseName(param_edit_value)))
			{
				abop.options[AssignBondOrderProcessor::Option::INIFile] = param_edit_value;
			}
			else
			{
				setStatusbarText((String)tr("The given parameter file does not exist! Using default!"), true);
			}

			// check for valid input
			if (bond_order_dialog_.max_n_opt_solutions->text().toInt() < 1)
				bond_order_dialog_.max_n_opt_solutions->setText(String(1).c_str());

			if (bond_order_dialog_.max_n_all_solutions->text().toInt() < 1)
				bond_order_dialog_.max_n_all_solutions->setText(String(1).c_str());

			// get the limitations for number of bond order assignments
			if (bond_order_dialog_.single_solution_button->isChecked())
			{
				abop.options[AssignBondOrderProcessor::Option::MAX_NUMBER_OF_SOLUTIONS]          = 1;
				abop.options[AssignBondOrderProcessor::Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS]= false;
			}
			else if (bond_order_dialog_.all_optimal_solutions_button->isChecked())
			{
				abop.options[AssignBondOrderProcessor::Option::MAX_NUMBER_OF_SOLUTIONS]           = 0;
				abop.options[AssignBondOrderProcessor::Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS]= false;
			}
			else if (bond_order_dialog_.n_opt_solutions_button->isChecked())
			{
				abop.options[AssignBondOrderProcessor::Option::MAX_NUMBER_OF_SOLUTIONS]           = bond_order_dialog_.max_n_opt_solutions->text().toInt();
				abop.options[AssignBondOrderProcessor::Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS]= false;
			}
			else if (bond_order_dialog_.n_all_solutions_button->isChecked())
			{
				abop.options[AssignBondOrderProcessor::Option::MAX_NUMBER_OF_SOLUTIONS]           = bond_order_dialog_.max_n_all_solutions->text().toInt();
				abop.options[AssignBondOrderProcessor::Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS]= true;
			}


			// apply
			containers.front()->apply(abop);

			// give a message
			if (abop.getNumberOfComputedSolutions() == 0)
			{
				setStatusbarText(String(tr("Could not find a valid bond order assignment.")), true);
			}
			else
			{
				String nr = abop.getNumberOfComputedSolutions();
				setStatusbarText(String(tr("Found")) + " " + nr + " " +  (String)tr("bond order assignments."), true);

				Log.info()<< "  > " + (String)tr("Result AssignBondOrderProcessor: ") << endl;

				for (Size i = 0; i < abop.getNumberOfComputedSolutions(); i++)
				{
					ostringstream stream_description;
					stream_description.setf(std::ios_base::fixed);
					stream_description.precision(2);

					stream_description << "      " + (String)tr("Solution") << " " << i
														 << " "  + (String)tr("has penalty") << " " << abop.getTotalPenalty(i)
														 << ", " + (String)tr("charge") << " " << abop.getTotalCharge(i)
														 << ", " <<  abop.getNumberOfAddedHydrogens(i) << " " << (String)tr("added hydrogens.");

					String description = stream_description.str();

					Log.info() << description << endl;
				}

				showBondOrderAssignmentResults(abop);
			}

			getMainControl()->update(*containers.front(), true);
		}


		void MolecularStructure::showBondOrderAssignmentResults(AssignBondOrderProcessor& bop)
		{
			bond_order_results_dialog_.setProcessor(&bop);
			bond_order_results_dialog_.createEntries();

			// Execute the assign bond order dialog
			// and abort if cancel is clicked or nonsense arguments are given
			if (!bond_order_results_dialog_.exec())
			{
				return;
			}
		}

		void MolecularStructure::centerCamera(Composite* composite)
		{
			VIEW::focusCamera(composite);
		}


		void MolecularStructure::checkMenu(MainControl& main_control)
		{
			Size number_of_selected_objects = main_control.getMolecularControlSelection().size();

			bool composites_muteable = !main_control.isBusy();
			bool one_system = composites_muteable && (main_control.getSelectedSystem() != 0);

	// 		menuBar()->setItemEnabled(assign_charges_id_, one_system && composites_muteable);

			// AMBER methods are available only for single systems
			// disable calculation entries, if a simulation is running
			if (energy_id_)
				energy_id_->setEnabled( one_system && composites_muteable);
			if (minimization_id_)
				minimization_id_->setEnabled( one_system && composites_muteable);
			if (mdsimulation_id_)
				mdsimulation_id_->setEnabled( one_system && composites_muteable);

			if (assign_bond_orders_id_)
				assign_bond_orders_id_->setEnabled( one_system && composites_muteable);
			if (calculate_hbonds_id_)
				calculate_hbonds_id_->setEnabled( one_system && composites_muteable);

			// prevent changes to forcefields, if simulation is running
			QMenu* menu = getMainControl()->initPopupMenu(MainControl::CHOOSE_FF, UIOperationMode::MODE_ADVANCED);
			if (menu)
				menu->setEnabled(composites_muteable);

			if (build_peptide_id_)
				build_peptide_id_->setEnabled(composites_muteable);

			bool allow = composites_muteable && one_system;
			if (add_hydrogens_id_)
				add_hydrogens_id_->setEnabled(allow);
			if (build_bonds_id_)
				build_bonds_id_->setEnabled(allow);
			if (build_endcaps_id_)
				build_endcaps_id_->setEnabled(allow);
			if (check_structure_id_)
				check_structure_id_->setEnabled(allow);
			if (calculate_ss_id_)
				calculate_ss_id_->setEnabled(allow);
			if (center_camera_id_)
				center_camera_id_->setEnabled(allow);
			if (create_distance_grid_id_)
				create_distance_grid_id_->setEnabled(allow);
			if (menu_FPDB_)
				menu_FPDB_->setEnabled(allow);
			if (setup_ff_)
				setup_ff_->setEnabled(composites_muteable);

//			menuBar()->setItemEnabled( map_proteins_id_, (number_of_selected_objects == 2) && 
// 																									 composites_muteable);

			if (calculate_RMSD_id_)
				calculate_RMSD_id_->setEnabled( (number_of_selected_objects == 2) && composites_muteable);

			menu = getMainControl()->initPopupMenu(MainControl::TOOLS_GRID, UIOperationMode::MODE_ADVANCED);
			if (menu)
				menu->setEnabled(composites_muteable);

//   			calculate_ramachandran_->setEnabled((number_of_selected_objects == 1) &&
//   							RTTI::isKindOf<Protein>(*getMainControl()->getMolecularControlSelection().begin()));
		}



		void MolecularStructure::addComposite_(Composite& composite, const String& name, bool normalize)
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "starting applying molecular properties" << std::endl;
			#endif

			// properties will be used only for atom containers
            if (!RTTI::isKindOf<AtomContainer>(&composite)) return;

			Log.info() << "> " + (String)tr("applying molecular properties") + " ... " << endl;

			AtomContainer& atom_container = *RTTI::castTo<AtomContainer>(composite);

			if (normalize)
			{
				try
				{
					atom_container.apply(getFragmentDB().normalize_names);
				}
				catch (Exception::GeneralException& e)
				{
					Log.error() << " > " + (String)tr("normalize names failed: ") << endl;
					Log.error() << e << endl;
				}
				catch (std::exception& e)
				{
					Log.error() << "  > " + (String)tr("normalized names failed.") << endl;
					Log.error() << e.what() << std::endl;
					return;
				}

				Log.info() << "  > " + (String)tr("normalized names") << endl;

				try
				{
					atom_container.apply(getFragmentDB().build_bonds);
				}
				catch (Exception::GeneralException& e)
				{
					Log.error() << " > " + (String)tr("generate missing bonds - failed: ") << endl;
					Log.error() << e << endl;
				}
				catch (...)
				{
					Log.error() << "  > " + (String)tr("generate missing bonds - failed.") << endl;
					return;
				}

				Log.info() << "  > " + (String)tr("generated missing bonds") << endl;
			}

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

			RegularData3D* regdat = new RegularData3D(RegularData3D::IndexType(64),
																								bs.getLower()-Vector3(20,20,20),
																								bs.getUpper()-bs.getLower()+Vector3(40,40,40));

			for (Size i=0; i < regdat->size(); i++)
			{
				float distance = (regdat->getCoordinates(i) - v).getLength();
				(*regdat)[i] = distance;
			}

			RegularData3DDataset* set = new RegularData3DDataset;
			set->setData(regdat);
			set->setName(S.getName() + "_distance");
			set->setType(RegularData3DController::type);
			set->setComposite(&S);
			notify_(new DatasetMessage(set, DatasetMessage::ADD));

			setStatusbarText((String)tr("Calculated grid"), true);
		}

		void MolecularStructure::createGridFromCameraDistance()
		{
			if (!getMainControl()->getSelectedSystem()) return;
			System& S = *(System*) getMainControl()->getSelectedSystem();
			
			Vector3 v(0,0,0);
			AtomIterator atit = S.beginAtom();

			BoundingBoxProcessor bs;

			S.apply(bs);

			RegularData3D* regdat = new RegularData3D(RegularData3D::IndexType(64), 
																								bs.getLower()-Vector3(20,20,20), 
																								bs.getUpper()-bs.getLower()+Vector3(40,40,40));

			Vector3 vp;
			Scene* scene = Scene::getInstance(0);
			if (scene != 0) vp = scene->getStage()->getCamera().getViewPoint();

			for (Size i=0; i < regdat->size(); i++)
			{
				float distance = (regdat->getCoordinates(i) - vp).getLength();
				(*regdat)[i] = distance;
			}

			RegularData3DDataset* set = new RegularData3DDataset;
			set->setData(regdat);
			set->setName(S.getName() + "_camera_distance");
			set->setType(RegularData3DController::type);
			set->setComposite(&S);
			notify_(new DatasetMessage(set, DatasetMessage::ADD));

			setStatusbarText((String)tr("Calculated grid"), true);
		}


		void MolecularStructure::calculateSecondaryStructure()
		{
			if (getMainControl()->getSelectedSystem() == 0) return;
			System& s = *getMainControl()->getSelectedSystem();
			ChainIterator cit = s.beginChain();
 			for (; +cit; ++cit)
			{
				SecondaryStructureProcessor ssp;
				(*cit).apply(ssp);
			}

			notify_(new CompositeMessage(s, CompositeMessage::CHANGED_COMPOSITE_HIERARCHY));

			Size nr = s.countSecondaryStructures();

			setStatusbarText(String(tr("Calculated ")) + String(nr) + " " + (String)tr("secondary structures."), true);
		}


		void MolecularStructure::calculateRMSD()
		{
			if (getMainControl()->getMolecularControlSelection().size() != 2)
			{
				setStatusbarText((String)tr("Please highlight exactly 2 AtomContainers."), true);
				return;
			}

			list<Composite*>::iterator sit = getMainControl()->getMolecularControlSelection().begin();

			AtomContainer* a1 = dynamic_cast<AtomContainer*>(*sit);
			sit++;
			AtomContainer* a2 = dynamic_cast<AtomContainer*>(*sit);
		
			if (!a1 || !a2)
			{
				setStatusbarText((String)tr("Exact two AtomContainers have to be selected"), true);
				return;
			}

			if (a1->isRelatedWith(*a2))
			{
				setStatusbarText((String)tr("The two Proteins must not be descendet/ancestor of eachother."), true);
				return;
			}

			if (!a1->apply(getFragmentDB().normalize_names) ||
					!a2->apply(getFragmentDB().normalize_names)		) 
			{
				setStatusbarText((String)tr("Could not apply normalize names, so cant calulate RMSD"), true);
				return;
			}

			AtomBijection ab;
			ab.assignByName(*a1, *a2);
			float rmsd = ab.calculateRMSD();

			String rmsd_text((String)tr("Calcuted RMSD") + ": " + String(rmsd) + " A.");

			a1->deselect();
			a2->deselect();

			Size max_atoms = BALL_MAX(a1->countAtoms(), a2->countAtoms());

			if (ab.size() == max_atoms)
			{
				setStatusbarText(rmsd_text + ". " + (String)tr("All atoms could be matched."), true);
				return;
			}

			rmsd_text += "  " + (String)tr("WARNING") + ": " + String(max_atoms - ab.size()) + " " 
				        + (String)tr("atoms were not mapped and are now selected");
			setStatusbarText(rmsd_text, true);

			HashSet<Atom*> atom_set;
			AtomIterator ait = a1->beginAtom();
			for (; +ait; ++ait)
			{
				atom_set.insert(&*ait);
			}

			ait = a2->beginAtom();
			for (; +ait; ++ait)
			{
				atom_set.insert(&*ait);
			}

			AtomBijection::PairVector::iterator pit = ab.begin();
			for(; pit != ab.end(); pit++)
			{
				atom_set.erase(pit->first);
				atom_set.erase(pit->second);
			}

			HashSet<Atom*>::Iterator hit = atom_set.begin();
			for (; +hit; ++hit)
			{
				(**hit).select();
			}

			getMainControl()->updateRepresentationsOf(*a1, true);
			getMainControl()->updateRepresentationsOf(*a2, true);

			NewSelectionMessage* new_message = new NewSelectionMessage;
			notify_(new_message);
		}

		void MolecularStructure::mapProteins()
		{
			if (getMainControl()->getMolecularControlSelection().size() != 2)
			{
				return;
			}

			Protein* a1 = 0;
			Protein* a2 = 0;

			list<Composite*>::iterator it = getMainControl()->getMolecularControlSelection().begin();
			
            if (!RTTI::isKindOf<Protein>(*it))
			{
				setStatusbarText((String)tr("Exact two Proteins have to be selected"), true);
				return;
			}
			
			a1 = (Protein*) *it;
			it++;

            if (!RTTI::isKindOf<Protein>(*it))
			{
				setStatusbarText((String)tr("Exact two Proteins have to be selected"), true);
				return;
			}

			a2 = (Protein*) *it;

			if (a1->isRelatedWith(*a2))
			{
				setStatusbarText((String)tr("The two Proteins must not be descendet/ancestor of eachother."), true);
				return;
			}

			if (!a1->apply(getFragmentDB().normalize_names) ||
					!a2->apply(getFragmentDB().normalize_names)		) 
			{
				setStatusbarText((String)tr("Could not apply normalize names, so cant calulate RMSD"), true);
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

			Log.info() << (String)tr("Calcuted RMSD") + ": " << rmsd << std::endl;
			Log.info() << (String)tr("Calcuted Transformation") + ": " << std::endl << m<< std::endl;
			Log.info() << (String)tr("Number of not matched CA atoms") + ": " << not_matched_ca << std::endl << std::endl;
			setStatusbarText((String)tr("Calcuted RMSD") + ": " + String(rmsd), true);
		}

		AmberFF& MolecularStructure::getAmberFF()
		{
			return amber_;
		}

		AmberConfigurationDialog& MolecularStructure::getAmberConfigurationDialog()
		{
			return amber_dialog_;
		}

		CharmmConfigurationDialog& MolecularStructure::getCharmmConfigurationDialog()
		{
			return charmm_dialog_;
		}

		MMFF94ConfigurationDialog& MolecularStructure::getMMFF94ConfigurationDialog()
		{
			return mmff94_dialog_;
		}


		CharmmFF& MolecularStructure::getCharmmFF()
		{
			return charmm_;
		}
	
		MMFF94& MolecularStructure::getMMFF94() 
		{
			return mmff_;
		}

		ForceField& MolecularStructure::getForceField()
		{
			if (force_field_id_ == 0) return reinterpret_cast<ForceField&>(amber_);
			if (force_field_id_ == 1) return reinterpret_cast<ForceField&>(charmm_);
			if (force_field_id_ == 2) return reinterpret_cast<ForceField&>(mmff_);

			return reinterpret_cast<ForceField&>(amber_);
		}

		void MolecularStructure::fetchPreferences(INIFile& inifile)
		{
			minimization_dialog_.readPreferenceEntries(inifile);
		//	bond_order_dialog_.readPreferenceEntries(inifile); // TODO:
		//  bond_order_results_dialog_.readPreferenceEntries(inifile); // TODO:
			md_dialog_.readPreferenceEntries(inifile);
			amber_dialog_.readPreferenceEntries(inifile);
			charmm_dialog_.readPreferenceEntries(inifile);
			mmff94_dialog_.readPreferenceEntries(inifile);
			applyForceFieldSettings_();
			if (inifile.hasEntry("FORCEFIELD", "selected"))
			{
				try
				{
					force_field_id_ = inifile.getValue("FORCEFIELD", "selected").toUnsignedShort();
					chooseForceField(force_field_id_);
				}
				catch(...)
				{
				}
			}
			else
			{
				chooseForceField(AMBER_FF);
			}
		}
    
    
    Position MolecularStructure::getForceFieldID()
    {
      return force_field_id_;
    }


		void MolecularStructure::writePreferences(INIFile& inifile)
		{
			minimization_dialog_.writePreferenceEntries(inifile);
			//bond_order_dialog_.writePreferenceEntries(inifile); // TODO
			//bond_order_results_dialog_.writePreferenceEntries(inifile); // TODO
			md_dialog_.writePreferenceEntries(inifile);
			amber_dialog_.writePreferenceEntries(inifile);
			charmm_dialog_.writePreferenceEntries(inifile);
			mmff94_dialog_.writePreferenceEntries(inifile);
			inifile.appendSection("FORCEFIELD");
			inifile.insertValue("FORCEFIELD", "selected", force_field_id_);
		}


	// 	void MolecularStructure::assignCharges()
	// 	{
	// 	}

		void MolecularStructure::calculateHBonds()
		{
			System* system = getMainControl()->getSelectedSystem();

			if (system == 0)
			{
				setStatusbarText((String)tr("To calculate H-bonds, one system has to be selected"), true);
				return;
			}

			HBondProcessor proc;
			system->apply(proc);

			getMainControl()->update(*system);

			AtomBondIterator bit;
			AtomIterator ait;
			Size hbonds = 0;

			BALL_FOREACH_BOND(*system, ait, bit)
			{
				Bond& bond = *bit;
				if (bond.getType() == Bond::TYPE__HYDROGEN) hbonds++; 
			}
			
			setStatusbarText(String(tr("Calculated")) + " " + String(hbonds) + (String)tr(" H-bonds"), true);
		}

		void MolecularStructure::applyForceFieldSettings_()
		{
			charmm_dialog_.accept();
			amber_dialog_.accept();
			mmff94_dialog_.accept();
		}

		void MolecularStructure::calculateForceFieldEnergy()
		{
			System* system = getMainControl()->getSelectedSystem();
			if (system == 0)
			{
				setStatusbarText((String)tr("To calculate ForceField energies, one system has to be selected"), true);
				return;
			}

			if (!setupForceField_(system)) return;

			// Compute the single point energy and print the result to Log and the status bar.
			ForceField& ff = getForceField();
			ff.updateForces();
			// workaround for MSVC: need to create an string, than log it!
			String results = ff.getResults();
			Log.info() << results << std::endl;
			setStatusbarText((String)tr("Total energy") + ": " + String(ff.getEnergy()) + " " + (String)tr("kJ/mol."), true);
			getMainControl()->update(*system);
		}

		void MolecularStructure::runMinimization(bool show_dialog)
		{
			// Make sure we run one instance of a simulation at a time only.
			if (getMainControl()->isBusy())
			{
				Log.error() << (String)tr("Simulation already running or still rendering!") << std::endl;
				return;
			}

			// Retrieve the system from the selection and abort if nothing is selected.
			System* system = getMainControl()->getSelectedSystem();
			if (system == 0) return;

			if (show_dialog)
			{
				// Execute the minimization dialog
				// and abort if cancel is clicked or nonsense arguments are given
				if (!minimization_dialog_.exec() ||
						(minimization_dialog_.getMaxGradient() == 0.0) ||
						(minimization_dialog_.getEnergyDifference() == 0.0))
				{
					return;
				}
			}

			chooseForceField(minimization_dialog_.selectedForceField());
			if (!setupForceField_(system, true)) return;
			
			ForceField& ff = getForceField();
			// Print some stats on the force field
			Log.info() << (String)tr("Set up the force field for ") 
								 << ff.getAtoms().size() << " " << (String)tr("atoms with parameters from") << " "
								 << ff.getParameters().getFilename() << "." << std::endl;

			// Start the simulation. Make sure that potential selections
			// are now honored.
			ff.enableSelection();
			setStatusbarText((String)tr("Starting minimization..."), false);
			ff.updateEnergy();

			EnergyMinimizer* minimizer;
			if (minimization_dialog_.getUseConjugateGradient())
			{
				minimizer = new ConjugateGradientMinimizer;
			}
			else if (minimization_dialog_.getUseStrangLBFGS())
			{
				minimizer = new StrangLBFGSMinimizer;
			}
			else if (minimization_dialog_.getUseShiftedLVMM())
			{
				minimizer = new ShiftedLVMMMinimizer;
			}
			else
			{
				minimizer = new SteepestDescentMinimizer;
			}

			// set the minimizer options
			minimizer->options[EnergyMinimizer::Option::MAXIMAL_NUMBER_OF_ITERATIONS] = 
					minimization_dialog_.getMaxIterations();
			minimizer->options[EnergyMinimizer::Option::MAX_GRADIENT] = 
					minimization_dialog_.getMaxGradient();
			minimizer->options[EnergyMinimizer::Option::ENERGY_DIFFERENCE_BOUND] = 
					minimization_dialog_.getEnergyDifference();
			minimizer->options[EnergyMinimizer::Option::ENERGY_OUTPUT_FREQUENCY] = 0;
			minimizer->setMaxNumberOfIterations(minimization_dialog_.getMaxIterations());

			// Setup the minimizer.
			if (!minimizer->setup(ff))
			{
				setStatusbarText((String)tr("Setup of minimizer failed. See log for details."), true);
				delete minimizer;
				return;
			}

			// Print the number of moveable atoms.
			Log.info() << (String)tr("Minimizing ") << ff.getNumberOfMovableAtoms() 
								 << " " << (String)tr("of") << " " << system->countAtoms() <<  " " << (String)tr("atoms") << "." << std::endl;
			
			try
			{
				// perform an initial step (no restart step)
				minimizer->minimize(1, false);

				// ============================= WITH MULTITHREADING ==================
		#ifdef BALL_QT_HAS_THREADS
				EnergyMinimizerThread* thread = new EnergyMinimizerThread;
				getMainControl()->setSimulationThread(thread);

				thread->setEnergyMinimizer(minimizer);
				thread->setNumberOfStepsBetweenUpdates(minimization_dialog_.getRefresh());
				thread->setComposite(system);

				thread->start(QThread::LowPriority);
				return;
				
   		#else
				// ============================= WITHOUT MULTITHREADING ===============
				// iterate until done and refresh the screen every "steps" iterations
				bool ok = true;
				bool converged = false;
				while (ok && !converged && minimizer->getNumberOfIterations() < minimizer->getMaxNumberOfIterations())
				{
					converged = minimizer->minimize(minimization_dialog_.getRefresh(), true);
					getMainControl()->update(*system);

					setStatusbarText(String(tr("Iteration"))  + " "   + String(minimizer->getNumberOfIterations())
													 + ": " + (String)tr("E") + " = " + String(ff.getEnergy()) + " " 
													 + (String)tr("kJ/mol")   + ", "  + (String)tr("RMS grad") + " = "
													 + String(ff.getRMSGradient()) + " " + (String)tr("kJ/(mol A)"), true);
					ok = !minimizer->wasAborted();
				}

				if (!ok)
				{
					setStatusbarText((String)tr("Aborted EnergyMinimizer because of strange energy values."), true);
				}
				else
				{
					// Print the final results.
					Log.info() << endl << (String)tr("minimization terminated.") << endl << endl;
					Log.info() << ff.getResults();
					Log.info() << (String)tr("final RMS gradient")   << "    : " << ff.getRMSGradient() 
						         << " " << (String)tr("kJ/(mol A)")    << "   "    << (String)tr("after") << " " 
										 << minimizer->getNumberOfIterations() << " "      << (String)tr("iterations") << endl << endl;
					setStatusbarText((String)tr("Final energy") + ": " + String(ff.getEnergy()) + " " + (String)tr("kJ/mol."), true);
				}
		#endif
			}
			catch(Exception::GeneralException& e)
			{
				String txt = (String)tr("Calculation aborted because of unexpected exception") + ": ";
				setStatusbarText(txt + ". " + (String)tr("See Log for details."), true);
				Log.error() << txt << ":" << std::endl;
				Log.error() << e << std::endl;
			}

			delete minimizer;
		}

		void MolecularStructure::MDSimulation(bool show_dialog)
		{
			// Make sure we run just one instance at a time.
			if (getMainControl()->isBusy())
			{
				Log.error() << (String)tr("Simulation already running or still rendering!") << std::endl;
				return;
			}
			
			// retrieve the system from the selection
			System* system = getMainControl()->getSelectedSystem();

			// Execute the MD simulation dialog
			// and abort if cancel is clicked or nonsense arguments given
			if (system == 0 
					|| md_dialog_.getSimulationTime() == 0.0
					|| md_dialog_.getTemperature() == 0.0)
			{
				Log.error() << (String)tr("No system or invalid settings for MD Simulation") << std::endl;
				return;
			}

			if (show_dialog && !md_dialog_.exec()) return;

			chooseForceField(md_dialog_.selectedForceField());
			ForceField& ff = getForceField();

			if (!setupForceField_(system, true)) return;

			// Create an instance of the molecular dynamics simulation.
			MolecularDynamics* mds = 0;
			if (md_dialog_.useMicroCanonical())
			{
				mds = new MicroCanonicalMD;
			}
			else
			{
				mds = new CanonicalMD;
			}
			
			// set the options for the MDS	
			Options options;
			options[MolecularDynamics::Option::ENERGY_OUTPUT_FREQUENCY] = 99999999;
			options[MolecularDynamics::Option::TIME_STEP] = md_dialog_.getTimeStep();
			options[MolecularDynamics::Option::REFERENCE_TEMPERATURE] = md_dialog_.getTemperature();

			try
			{
				// setup the simulation
				if (!mds->setup(ff, 0, options))
				{
					Log.error() << (String)tr("Setup for MD simulation failed!") << std::endl;
					delete mds;
					return;
				}
				
				// perform an initial step (no restart step)
				ff.enableSelection();
				mds->simulateIterations(1, false);

				// We update everything every so and so many steps.
				Size steps = md_dialog_.getStepsBetweenRefreshs();

				TrajectoryFile* trajectory_file = 0;
				if (md_dialog_.getDCDFile().size() != 0)
				{
					// use an absolute filename
					String name;
					if(QDir::isRelativePath(md_dialog_.getDCDFile())) {
						name = String(QDir::current().absoluteFilePath(md_dialog_.getDCDFile()));
					} else {
						name = String(md_dialog_.getDCDFile());
					}

					trajectory_file = TrajectoryFileFactory::open(name, std::ios::out, "dcd");
					static_cast<DCDFile*>(trajectory_file)->enableVelocityStorage();
				}
				// ============================= WITH MULTITHREADING ===================
			#ifdef BALL_QT_HAS_THREADS
				MDSimulationThread* thread = new MDSimulationThread;
				if (!getMainControl()->setSimulationThread(thread))
				{
					delete thread;
					delete mds;
					if (trajectory_file != 0) delete trajectory_file;
					setStatusbarText((String)tr("Could not lock Composites, is an other thread running?"), true);
					return;
				}

				thread->setMolecularDynamics(mds);
				thread->setNumberOfSteps(md_dialog_.getNumberOfSteps());
				thread->setNumberOfStepsBetweenUpdates(steps);
				thread->setTrajectoryFile(trajectory_file);
				thread->setComposite(system);
				thread->start(QThread::LowPriority);

			#else
				// ============================= WITHOUT MULTITHREADING =================
				// iterate until done and refresh the screen every "steps" iterations
				SnapShotManager manager(system, &getForceField(), trajectory_file);
				manager.setFlushToDiskFrequency(10);
				
				bool ok = true;
				while (mds->getNumberOfIterations() < md_dialog_.getNumberOfSteps() && ok)
				{
					ok = mds->simulateIterations(steps, true);
					getMainControl()->update(*system);
					
					if (trajectory_file != 0) 
					{
						manager.takeSnapShot();
					}


					setStatusbarText(String(tr("Iteration"))  + " "   + String(mds->getNumberOfIterations())
													 + ": " + (String)tr("E") + " = " + String(ff.getEnergy()) + " " + (String)tr("kJ/mol") + ", " 
													 + (String)tr("RMS grad") + " = "
													 + String(ff.getRMSGradient())    + " "  + (String)tr("kJ/(mol A)"), true);
				}

				if (trajectory_file) manager.flushToDisk();

				if (!ok)
				{
					setStatusbarText((String)tr("Aborted MDSimulation because of strange energy values."), true);
					return;
				}

				Log.info() << std::endl << (String)tr("Simulation terminated.") << std::endl << endl;
				Log.info() << ff.getResults();
				Log.info() << (String)tr("final RMS gradient") << "    : " << ff.getRMSGradient() << " " 
				           << (String)tr("kJ/(mol A)")         << "   "    << (String)tr("after") << " " 
									 << mds->getNumberOfIterations()     << " "      << (String)tr("iterations") << endl << endl;
				setStatusbarText((String)tr("Final energy") + ": " + String(ff.getEnergy()) + " " + (String)tr("kJ/mol."), true);

				// clean up
				delete mds;

				if (trajectory_file != 0)
				{
					trajectory_file->close();
					delete trajectory_file;
					Directory d;

					// use an absolute filename
					String name = d.getPath() + FileSystem::PATH_SEPARATOR + md_dialog_.getDCDFile();
					trajectory_file = TrajectoryFileFactory::open(name, File::MODE_IN, "dcd");

					NewTrajectoryMessage* message = new NewTrajectoryMessage;
					message->setComposite(*system);
					message->setTrajectoryFile(*trajectory_file);
					notify_(message);
				}
			#endif
			}
			catch(Exception::GeneralException& e)
			{
				String txt = (String)tr("Calculation aborted because of an unexpected exception");
				setStatusbarText(txt + ". " + (String)tr("See Logs"), true);
				Log.error() << txt << ":" << std::endl;
				Log.error() << e << std::endl;
			}
		}

		void MolecularStructure::buildPeptide()
		{
			PeptideDialog dialog(this, ((String)tr("PeptideDialog")).c_str());
			dialog.setFragmentDB(&getFragmentDB());
			dialog.exec();

			Protein* protein = dialog.getProtein();
			if (protein == 0) return;

			System* system = new System;
			system->insert(*protein);
			system->setName(dialog.getSequence());
			getMainControl()->insert(*system, dialog.getSequence());
			setStatusbarText((String)tr("  > build peptide ") + Peptides::GetSequence(*protein), true);
		}
		
		bool MolecularStructure::generateCrystal(bool show)
		{
			if (crystal_dialog_ == 0)
			{
				crystal_dialog_ = new GenerateCrystalDialog(getMainControl(), ((String)tr("GenerateCrystalDialog")).c_str());
				//crystal_dialog_->fetchPreferences(getMainControl()->getINIFile());
			}

			if (show)
			{
				return crystal_dialog_->exec();
			}
			
			return crystal_dialog_->generate();
		}
		
		void MolecularStructure::showAmberForceFieldOptions()
		{
			amber_dialog_.raise();
			if(amber_dialog_.exec() == QDialog::Accepted)
			{
				chooseAmberFF();
			}
		}

		void MolecularStructure::showCharmmForceFieldOptions()
		{
			charmm_dialog_.raise();
			if (charmm_dialog_.exec() == QDialog::Accepted)
			{
				chooseCharmmFF();
			}
		}

		void MolecularStructure::showMMFF94ForceFieldOptions()
		{
			mmff94_dialog_.raise();
			if (mmff94_dialog_.exec() == QDialog::Accepted)
			{
				chooseMMFF94();
			}
		}

		void MolecularStructure::chooseAmberFF()
		{
			chooseForceField(AMBER_FF);
		}
		
		void MolecularStructure::chooseCharmmFF()
		{
			chooseForceField(CHARMM_FF);
		}

		void MolecularStructure::chooseMMFF94()
		{
			chooseForceField(MMFF94_FF);
		}

		void MolecularStructure::chooseForceField(Position nr)
		{
			if (nr > 2)
			{
				Log.error() << (String)tr("Selected invalid force field!") << std::endl;
				return;
			}

			force_field_id_ = nr;
			
			if (UIOperationMode::instance().getMode() <= UIOperationMode::MODE_ADVANCED)
			{
				amber_ff_id_->setChecked(false);
				charmm_ff_id_->setChecked(false);
				mmff94_id_->setChecked(false);

				if (nr == 0) amber_ff_id_->setChecked(true);
				else if (nr == 1) charmm_ff_id_->setChecked(true);
				else if (nr == 2) mmff94_id_->setChecked(true);
			}

			md_dialog_.selectForceField(force_field_id_);
			minimization_dialog_.selectForceField(force_field_id_);
		}

		void MolecularStructure::setupForceField()
		{
			if (force_field_id_ == AMBER_FF) showAmberForceFieldOptions();
			else if (force_field_id_ == CHARMM_FF) showCharmmForceFieldOptions();
			else if (force_field_id_ == MMFF94_FF) showMMFF94ForceFieldOptions();
		}


		void MolecularStructure::calculateRamachandranPlot()
		{
			/*
			List<Composite*> selection = getMainControl()->getMolecularControlSelection();
			if (!selection.size() != 1 ||
                    !RTTI::isKindOf<Protein>(*selection.begin()))
			{
				setStatusbarText("Exactly one Protein has to be selected for a Ramachandran Plot!");
			}

			Protein* protein = ((Protein*) *selection.begin());
			RegularData2D* rd = new RegularData2D(Vector2(-180,-180), Vector2(361, 361), Vector2(1,1));

			// now, iterate over all residues
			Size unassigned_residues = 0;
			Size   assigned_residues = 0;
			ResidueIterator res_it = protein->beginResidue();
			for (;+res_it; ++res_it)
			{
				// torsion angle phi does not exist for N terminal residues
				// torsion angle psi does not exist for C terminal residues
				if (res_it->hasTorsionPhi() &&
				    res_it->hasTorsionPsi())
				{
					Angle phi = res_it->getTorsionPhi();
					Angle psi = res_it->getTorsionPsi();
					(*rd).getClosestValue(Vector2(phi.toDegree(), psi.toDegree())) ++;
					assigned_residues++;
				}
				else
				{ 
					unassigned_residues++;
				}
			}

			String text = String(  assigned_residues) + " residues assigned, " +
										String(unassigned_residues) + " residues unassigned.";
			setStatusbarText(text, true);

			RegularData2DMessage* msg = new RegularData2DMessage(RegularData2DMessage::NEW);
			msg->setData(*rd);
			msg->setCompositeName(protein->getName() + " Ramachandran Plot");
			notify_(msg);
			*/
		}

		bool MolecularStructure::calculateFDPB(bool show)
		{
			if (fdpb_dialog_ == 0)
			{
				fdpb_dialog_ = new FDPBDialog(getMainControl(), ((String)tr("FDPBDialog")).c_str());
				fdpb_dialog_->fetchPreferences(getMainControl()->getINIFile());
			}

			if (show)
			{
				return fdpb_dialog_->exec();
			}
			
			return fdpb_dialog_->calculate();
		}

		void MolecularStructure::selectUnassignedForceFieldAtoms_()
		{
			HashSet<Composite*>::Iterator it = getMainControl()->getCompositeManager().begin();
			for (; it != getMainControl()->getCompositeManager().end(); it++)
			{
				(*it)->deselect();
			}

			HashSet<const Atom*>::ConstIterator ait = getForceField().getUnassignedAtoms().begin();
			for (; +ait; ait++)
			{
				(const_cast<Atom*>(*ait))->select();
	
				CompositeMessage* msg = new CompositeMessage(**ait, CompositeMessage::SELECTED_COMPOSITE);
				msg->setUpdateRepresentations(false);
				msg->setShowSelectionInfos(false);
				notify_(msg);
			}

			getMainControl()->update(*getForceField().getSystem(), true);

			setStatusbarText((String)tr("Setup of the force field failed for selected atoms."), true);
		}

		bool MolecularStructure::setupForceField_(System* system,
																							bool disable_selection)
		{
			// Get the force field.
			applyForceFieldSettings_();

			// set up the force field
			setStatusbarText((String)tr("setting up force field") + "...", false);
		
			// Setup the force field.
			ForceField& ff = getForceField();
			if (disable_selection) ff.disableSelection();
			else 									 ff.enableSelection();

			bool ok = false;
			try
			{
				if (ff.setup(*system))
				{
					ok = true;
				}

				if (ff.getUnassignedAtoms().size() > 0)
				{
					Log.error() << (String)tr("Warning: some atoms could not be assigned/initialized!")
											<< std::endl
											<< "<a href=mm.html#ff_problems>" << (String)tr("For more informations, click Here.") + "</a>" << std::endl;
				}
			}
			catch(...)
			{
			}
			
			ff.updateEnergy();

			// CHARMM setup may delete atoms (converted to united atoms!),
			// so we have to make sure the rest of the world realizes something might have changed.
			if (force_field_id_ == CHARMM_FF)
			{
				getMainControl()->update(*system, true);
			}

			if (!ok)
			{
				setStatusbarText((String)tr("Force field setup failed."), true);
				selectUnassignedForceFieldAtoms_();
			}
			else
			{
				ff.updateEnergy();
			}

			return ok;
		}

	} // namespace VIEW
} // namespace BALL
