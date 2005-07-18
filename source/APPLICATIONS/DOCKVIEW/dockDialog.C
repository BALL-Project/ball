// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: dockDialog.C,v 1.1.2.14.2.49 2005/07/18 13:40:12 leonhardt Exp $
//

#include "dockDialog.h"
#include "geometricFitDialog.h"
#include "dockingController.h"

#include <qpushbutton.h>
#include <qcombobox.h>
#include <qstringlist.h>
#include <qmessagebox.h>
#include <qcheckbox.h>
#include <qlineedit.h>
#include <qradiobutton.h>
#include <qfiledialog.h>
#include <qtabwidget.h>
#include <qbuttongroup.h>
#include <qlistbox.h>

#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/VIEW/WIDGETS/molecularStructure.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/STRUCTURE/DOCKING/geometricFit.h>

namespace BALL
{
	namespace VIEW
	{
		// Default constructor
		DockDialog::DockDialog(QWidget* parent,  const char* name, bool modal, WFlags fl)
			throw()
			: DockDialogData(parent, name, modal, fl),
				PreferencesEntry(),
				docking_partner1_(0),
				docking_partner2_(0)
		{
		#ifdef BALL_VIEW_DEBUG
			Log.error() << "new DockDialog " << this << std::endl;
		#endif
			
			// register QWidgets of Dialog with PreferenceEntry
			// entries of them in the INIFile will be generated
			setINIFileSectionName("DOCKING");
			registerObject_(algorithms);
			registerObject_(scoring_functions);
			registerObject_(best_num);
			registerObject_(verbosity);
			registerObject_(phi_min);
			registerObject_(psi_min);
			registerObject_(theta_min);
			registerObject_(phi_max);
			registerObject_(psi_max);
			registerObject_(theta_max);
			registerObject_(delta_phi);
			registerObject_(delta_psi);
			registerObject_(delta_theta);
			registerObject_(radii_data_lineedit);
			registerObject_(radii_rules_lineedit);
			registerObject_(charges_data_lineedit);
			registerObject_(charges_rules_lineedit);
			registerObject_(normalize_names);
			registerObject_(assign_charges);
			registerObject_(assign_radii);
			registerObject_(build_bonds);
			registerObject_(add_hydrogens);
			
			//set flag
			is_redock_ = false;
			
			hide(); 
		}
		
		// Copy constructor.
		DockDialog::DockDialog(const DockDialog& dock_dialog)
			throw()
			: //DockDialogData(dock_dialog),
				PreferencesEntry(dock_dialog),
				is_redock_(dock_dialog.is_redock_),
				has_changed_(dock_dialog.has_changed_),
				algorithm_dialogs_(dock_dialog.algorithm_dialogs_),
				scoring_dialogs_(dock_dialog.scoring_dialogs_),
				allowed_sf_(dock_dialog.allowed_sf_),
				docking_partner1_(dock_dialog.docking_partner1_),
				docking_partner2_(dock_dialog.docking_partner2_),
				algorithm_opt_(dock_dialog.algorithm_opt_),
				scoring_opt_(dock_dialog.scoring_opt_),
				backup_(dock_dialog.backup_),
				radius_rule_processor_(dock_dialog.radius_rule_processor_),
				charge_rule_processor_(dock_dialog.charge_rule_processor_),
				radius_processor_(dock_dialog.radius_processor_),
				charge_processor_(dock_dialog.charge_processor_)
		{}
		
		// Destructor
		DockDialog::~DockDialog()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << this << " of class DockDialog" << std::endl;
			#endif 
			
			// remark: QDialogs in HashMaps are deleted automatically because their parent is DockDialog
			// remark: Systems are deleted by Maincontrol
		}
		
		// Assignment operator
		const DockDialog& DockDialog::operator =(const DockDialog& dock_dialog)
			throw()
		{
			if (&dock_dialog != this)
			{
				is_redock_ = dock_dialog.is_redock_;
				has_changed_ = dock_dialog.has_changed_;
				// ??? should we delete the dialogs in the HashMaps ???
				algorithm_dialogs_ = dock_dialog.algorithm_dialogs_;
				scoring_dialogs_ = dock_dialog.scoring_dialogs_;
				allowed_sf_ = dock_dialog.allowed_sf_;
				docking_partner1_ = dock_dialog.docking_partner1_;
				docking_partner2_ = dock_dialog.docking_partner2_;
				algorithm_opt_ = dock_dialog.algorithm_opt_;
				scoring_opt_ = dock_dialog.scoring_opt_;
				backup_ = dock_dialog.backup_;
				radius_rule_processor_ = dock_dialog.radius_rule_processor_;
				charge_rule_processor_ = dock_dialog.charge_rule_processor_;
				radius_processor_ = dock_dialog.radius_processor_;
				charge_processor_ = dock_dialog.charge_processor_;
			}
			return *this;
		}

		// Sets two systems as docking partners.
		void DockDialog::setSystems(System* system1, System* system2)
			throw() 
		{
			docking_partner1_ = system1;
			docking_partner2_ = system2;
		}
		
		System* DockDialog::getSystem1()
			throw()
		{
			return docking_partner1_;
		}
					
		System* DockDialog::getSystem2()
			throw()
		{
		 	return docking_partner2_;
		}
		
		Options& DockDialog::getAlgorithmOptions()
			throw()
		{
			return algorithm_opt_;
		}
					
		Options& DockDialog::getScoringOptions()
			throw()
		{
			return scoring_opt_;
		}
		
		// Sets the flags 'is_redock_' and 'has_changed_'
		void DockDialog::setFlag(bool is_redock)
			throw()
		{
			if (is_redock_ == is_redock)
			{
			 	has_changed_ = false;
			}
			else
			{
				has_changed_ = true;
			 	is_redock_ = is_redock;
			}
		}
		
		// Adds docking algorithm to Combobox and its advanced option dialogs to HashMap.
		void DockDialog::addAlgorithm(const QString& name, const int algorithm, QDialog* dialog)
			throw()
		{
			if (dialog)
			{
				// add dialog to HashMap
				algorithm_dialogs_[algorithm] = dialog;
			}
			// add to ComboBox
			algorithms->insertItem(name, algorithm);
		}
		
		// Adds scoring function to Combobox and its advanced option dialogs to HashMap, if it has such an dialog.
		void DockDialog::addScoringFunction(const QString& name, const int score_func, QDialog* dialog)
			throw()
		{
			if (dialog)
			{
				// add scoring function to HashMap for scoring option dialogs
				scoring_dialogs_[score_func] = dialog;
			}
			// add to ComboBox
			scoring_functions->insertItem(name, score_func);
		}
		
		// Initializes the popup menu Molecular Mechanics with its checkable submenu Docking.
		void DockDialog::initializeWidget()
			throw()
		{
			//build HashMap for algorithm advanced option dialogs
			//make sure the order of added algorithms is consistent to the enum order
			//because the algorithm with enum value i should be at position i in the combobox
			//otherwise you get the wrong option dialog for an algorithm
			GeometricFitDialog* geo_fit = new GeometricFitDialog(this);
			addAlgorithm("Geometric Fit", DockingController::GEOMETRIC_FIT, geo_fit);
			
			//build HashMap for scoring function advanced option dialogs
			//make sure the order of added scoring functions is consistent to the enum order
			//because the scoring function with enum value i should be at position i in the Combobox
			//otherwise you get the wrong option dialog for a scoring function
			addScoringFunction("Default", DockingController::DEFAULT);
			addScoringFunction("Amber Force Field", DockingController::AMBER_FF, &(MolecularStructure::getInstance(0)->getAmberConfigurationDialog()));
			addScoringFunction("Random", DockingController::RANDOM);
			
			vector<int> sf;
			sf.push_back(DockingController::DEFAULT);
			sf.push_back(DockingController::AMBER_FF);
			allowed_sf_[DockingController::GEOMETRIC_FIT] = sf;
		}
		  
		// Read the preferences from the INIFile.
		void DockDialog::fetchPreferences(INIFile& file)
			throw()
		{
			PreferencesEntry::readPreferenceEntries(file);
			
			fetchPreferences_(file, "redock_entry_0", "<select>");
			fetchPreferences_(file, "redock_entry_1", "Default");
			fetchPreferences_(file, "redock_entry_2", "100");
			fetchPreferences_(file, "redock_entry_3", "1");
			
			// call this function to check which algorithm / scoring function is the current item in the combobox
			// and set advanced button enabled if necessary
			algorithmChosen();
			scoringFuncChosen();
			
			HashMap<int, QDialog*>::Iterator it = algorithm_dialogs_.begin();
			for (; +it; ++it)
			{
				GeometricFitDialog* dialog = dynamic_cast<GeometricFitDialog*>(it->second);
				if(dialog)
				{
					dialog->fetchPreferences(file);
				}
			}
		}
		 
		// function to read the redocking values from INIFile into vector backup_
		// if INIFile has not yet a section REDOCKING, fill backup_ vector with default values
		void DockDialog::fetchPreferences_(INIFile& file, const String& entry, const QString& default_value)
			throw()
		{
			if (!file.hasEntry("REDOCKING", entry))
			{
			 	backup_.push_back(default_value);
			}
			else
			{
				backup_.push_back(QString(file.getValue("REDOCKING", entry).c_str()));
			}
		}
		
		// Write the preferences to the INIFile.
		void DockDialog::writePreferences(INIFile& file)
			throw()
		{
			if (is_redock_)
			{
				swapValues_();
			}
			PreferencesEntry::writePreferenceEntries(file);
			
			file.appendSection("REDOCKING");
			for (unsigned int i = 0; i < backup_.size(); i++)
			{
				String entry = String("redock_entry_") + String(i);
				file.insertValue("REDOCKING", entry, backup_[i].ascii());
			}
			
			HashMap<int, QDialog*>::Iterator it = algorithm_dialogs_.begin();
			for (; +it; ++it)
			{
				GeometricFitDialog* dialog = dynamic_cast<GeometricFitDialog*>(it->second);
				if(dialog)
				{
					dialog->writePreferences(file);
				}
			}
		}
		
		/// Reset the dialog to the standard values
		void DockDialog::reset()
			throw()
		{
			if (tab_pages->currentPageIndex() == 0)
			{
				// comboboxes
				algorithms->setCurrentItem(0);
				scoring_functions->setCurrentItem(0);
				
				// buttons
				alg_advanced_button->setEnabled(false);
				scoring_advanced_button->setEnabled(false);
				
				// options
				best_num->setText("100");
				verbosity->setText("1");
				
				if(is_redock_)
				{
					// euler angles
				  phi_min->setText("-15");
					phi_max->setText("15");
					delta_phi->setText("3");
					
					psi_min->setText("-15");
					psi_max->setText("15");
					delta_psi->setText("3");
					
					theta_min->setText("-15");
					theta_max->setText("15");
					delta_theta->setText("3");
				}
				else
				{
					// system comboboxes
					systems1->setCurrentItem(0);
					systems2->setCurrentItem(0);
				}
			}
			
			if (tab_pages->currentPageIndex() == 1)
			{
				// radii / charges config files
				radii_data_lineedit->setText("radii/PARSE.siz");
				radii_rules_lineedit->setText("solvation/PARSE.rul");
				charges_data_lineedit->setText("charges/PARSE.crg");
				charges_rules_lineedit->setText("solvation/PARSE.rul");
			
				// processors
				normalize_names->setChecked(false);
				assign_charges->setChecked(true);
				assign_radii->setChecked(true);
				build_bonds->setChecked(false);
				add_hydrogens->setChecked(false);
			}
		}
		
		// set options with values user has chosen 
		void DockDialog::applyValues_()
			throw()
		{
			algorithm_opt_.clear();
			scoring_opt_.clear();
			// options for all docking algorithms
			/////////////////////////////////////// TODO allgemeine Options ////////////////////////////////////////////
			//options_[DockingAlgorithm::Option::BEST_NUM] = String(best_num->text().ascii()).toInt();
			algorithm_opt_[GeometricFit::Option::BEST_NUM] = String(best_num->text().ascii()).toInt();
			algorithm_opt_[GeometricFit::Option::VERBOSITY] = String(verbosity->text().ascii()).toInt();
				
			// options for chosen algorithm; options are filled by the corresponding dialog
			int index = algorithms->currentItem();
			switch(index)
			{
				case DockingController::GEOMETRIC_FIT:
					GeometricFitDialog* dialog = RTTI::castTo<GeometricFitDialog>(*(algorithm_dialogs_[index]));
					dialog->getOptions(algorithm_opt_);
					break;
			}
			
			// options for redocking (euler angles)
			if(is_redock_)
			{
				algorithm_opt_[GeometricFit::Option::PHI_MIN] = String(phi_min->text().ascii()).toFloat();
				algorithm_opt_[GeometricFit::Option::PHI_MAX] = String(phi_max->text().ascii()).toFloat();
				algorithm_opt_[GeometricFit::Option::DEG_PHI] = String(delta_phi->text().ascii()).toFloat();
				algorithm_opt_[GeometricFit::Option::PSI_MIN] = String(psi_min->text().ascii()).toFloat();
				algorithm_opt_[GeometricFit::Option::PSI_MAX] = String(psi_max->text().ascii()).toFloat();
				algorithm_opt_[GeometricFit::Option::DEG_PSI] = String(delta_psi->text().ascii()).toFloat();
				algorithm_opt_[GeometricFit::Option::THETA_MIN] = String(theta_min->text().ascii()).toFloat();
				algorithm_opt_[GeometricFit::Option::THETA_MAX] = String(theta_max->text().ascii()).toFloat();
				algorithm_opt_[GeometricFit::Option::DEG_THETA] = String(delta_theta->text().ascii()).toFloat();
			}
		  else
			{
				algorithm_opt_[GeometricFit::Option::PHI_MIN] = GeometricFit::Default::PHI_MIN;
				algorithm_opt_[GeometricFit::Option::PHI_MAX] = GeometricFit::Default::PHI_MAX;
				algorithm_opt_[GeometricFit::Option::DEG_PHI] = (float) algorithm_opt_.getReal(GeometricFit::Option::DEGREE_INTERVAL);
				algorithm_opt_[GeometricFit::Option::PSI_MIN] = GeometricFit::Default::PSI_MIN;
				algorithm_opt_[GeometricFit::Option::PSI_MAX] = GeometricFit::Default::PSI_MAX;
				algorithm_opt_[GeometricFit::Option::DEG_PSI] = (float) algorithm_opt_.getReal(GeometricFit::Option::DEGREE_INTERVAL);
				algorithm_opt_[GeometricFit::Option::THETA_MIN] = GeometricFit::Default::THETA_MIN;
				algorithm_opt_[GeometricFit::Option::THETA_MAX] = GeometricFit::Default::THETA_MAX;
				algorithm_opt_[GeometricFit::Option::DEG_THETA] = (float) algorithm_opt_.getReal(GeometricFit::Option::DEGREE_INTERVAL);
			}
			
			// options for chosen scoring function
			index = scoring_functions->currentItem();
			switch(index)
			{
				case DockingController::AMBER_FF:
				{
					AmberFF& ff = MolecularStructure::getInstance(0)->getAmberFF();
					AmberConfigurationDialog* dialog = RTTI::castTo<AmberConfigurationDialog>(*(scoring_dialogs_[index]));
					
					// now the Amber force field gets its options
					dialog->applyTo(ff);
					scoring_opt_ = ff.options;
				}
			}
		}
		
		// apply processors to the systems
		bool DockDialog::applyProcessors_()
			throw()
		{
			if ((docking_partner1_ == 0) || (docking_partner2_ == 0)) 
			{
				Log.error() << "No two systems given! Aborting..." << std::endl;
				return false;
			}
			
			// add hydrogens to systems and normalize names
			if (add_hydrogens->isChecked())
			{
				if (!docking_partner1_->apply(*(const_cast<ReconstructFragmentProcessor*>(&MainControl::getInstance(0)->getFragmentDB().add_hydrogens)))) return false;
				if (!docking_partner2_->apply(*(const_cast<ReconstructFragmentProcessor*>(&MainControl::getInstance(0)->getFragmentDB().add_hydrogens)))) return false;
				if (!docking_partner1_->apply(*(const_cast<FragmentDB::NormalizeNamesProcessor*>(&MainControl::getInstance(0)->getFragmentDB().normalize_names)))) return false;
				if (!docking_partner2_->apply(*(const_cast<FragmentDB::NormalizeNamesProcessor*>(&MainControl::getInstance(0)->getFragmentDB().normalize_names)))) return false;
			}
			else if (normalize_names->isChecked())
			{
				if (!docking_partner1_->apply(*(const_cast<FragmentDB::NormalizeNamesProcessor*>(&MainControl::getInstance(0)->getFragmentDB().normalize_names)))) return false;
				if (!docking_partner2_->apply(*(const_cast<FragmentDB::NormalizeNamesProcessor*>(&MainControl::getInstance(0)->getFragmentDB().normalize_names)))) return false;
			}
			
			// add bonds to systems
			if (build_bonds->isChecked())
			{
				if (!docking_partner1_->apply(*(const_cast<FragmentDB::BuildBondsProcessor*>(&MainControl::getInstance(0)->getFragmentDB().build_bonds)))) return false;
				if (!docking_partner2_->apply(*(const_cast<FragmentDB::BuildBondsProcessor*>(&MainControl::getInstance(0)->getFragmentDB().build_bonds)))) return false;
			}

			// assign charges and radii
			try
			{
				if (assign_charges->isChecked())
				{
					if (charges_data_button->isChecked())
					{
						charge_processor_.setFilename(charges_data_lineedit->text().ascii());
						if (!docking_partner1_->apply(charge_processor_)) return false;
						if (!docking_partner2_->apply(charge_processor_)) return false;
					}
					else
					{
						INIFile inifile(String(charges_rules_lineedit->text().ascii()));
						charge_rule_processor_ = ChargeRuleProcessor(inifile);
						if (!docking_partner1_->apply(charge_rule_processor_)) return false;
						if (!docking_partner2_->apply(charge_rule_processor_)) return false;
					}
				}
				if (assign_radii->isChecked())
				{
					if (radii_data_button->isChecked())
					{
						radius_processor_.setFilename(radii_data_lineedit->text().ascii());
						if (!docking_partner1_->apply(radius_processor_)) return false;
						if (!docking_partner2_->apply(radius_processor_)) return false;
					}
					else
					{
						INIFile inifile(String(radii_rules_lineedit->text().ascii()));
						radius_rule_processor_ = RadiusRuleProcessor(inifile);
						if (!docking_partner1_->apply(radius_rule_processor_)) return false;
						if (!docking_partner2_->apply(radius_rule_processor_)) return false;
					}
				}
			}
			catch (Exception::FileNotFound e)
			{
				Log.error() << "Invalid file " << e.getFilename() << std::endl;
				return false;
			}
			MainControl::getInstance(0)->getPrimitiveManager().setMultithreadingMode(false);
			MainControl::getInstance(0)->update(*docking_partner1_, true);
			MainControl::getInstance(0)->update(*docking_partner2_, true);
			MainControl::getInstance(0)->getPrimitiveManager().setMultithreadingMode(true);
			
			return true;
		}
		
		// show chosen file in the dialog
		void DockDialog::selectFile_(QLineEdit& lineedit)
			throw()
		{
			QString s = QFileDialog::getOpenFileName(MainControl::getInstance(0)->getWorkingDir().c_str(), "", MainControl::getInstance(0), "", "Choose a file");

			if (s == QString::null) return;
			MainControl::getInstance(0)->setWorkingDir(s.ascii());
			lineedit.setText(s);
		}
		
		// get system which the user has chosen in the dialog as docking partner
		System* DockDialog::partnerChosen_(const QString& qstr)
			throw()
		{
			// iterate over all composites; find chosen system
			HashSet<Composite*>::iterator composite_it = MainControl::getInstance(0)->getCompositeManager().begin();
				
			for (; +composite_it; ++composite_it)
			{
				System* system = dynamic_cast<System*>(*composite_it);
				if (system != 0 && system->getName() == qstr.ascii())
				{
					return system;
				}
			}
			return 0;
		}
		
		// Function to fill the system comboboxes.
		// If the user has already selected one or two systems, they are the current items in the comboboxes.
		void DockDialog::fillSystemComboxes_()
			throw()
		{
			// selection lists for systems should be empty
			systems1->clear();
			systems2->clear();
			
			// pointer to selected systems
			docking_partner1_ = NULL;
			docking_partner2_ = NULL;
			
			QStringList current_system_list;
			// put <select> in list as the first element
			current_system_list.append("<select>");
			
			// get the composites
			MainControl* main_control = MainControl::getInstance(0);
			CompositeManager& composite_manager = main_control->getCompositeManager();
			
			// iterate over all composites; add systems to list
			HashSet<Composite*>::Iterator composite_it = composite_manager.begin();
			
			// fill current system list and check if user has already selected two systems
			for (; +composite_it; ++composite_it)
			{
				System* system = dynamic_cast<System*>(*composite_it);
				if (system == 0) continue;

				current_system_list.append(system->getName());
				
				// test if the user has selected one or two systems
				// more than 2 selected systems -> error
				if (system->isSelected())
				{
					if (docking_partner1_ == NULL)
					{
						docking_partner1_ = system;
					}
					else
					{
						if (docking_partner2_ == NULL)
						{
							docking_partner2_ = system;
						}
						else
						{
							// if more than 2 systems are selected => Error message!
							#ifdef BALL_VIEW_DEBUG
								Log.error() << "DockDialog: " << " More than two systems selected! " << std::endl;
							#endif
												
							QMessageBox error_message(0,0);
							error_message.warning(0,"Error","More than two systems selected!", QMessageBox::Ok, QMessageBox::NoButton);
							return;
						}
					}
				} 	
			 }
			// set selection lists of dialog
			systems1->insertStringList(current_system_list);
			systems2->insertStringList(current_system_list);
		
			// If the user has selected one or two systems, they are the current items in the comboboxes.
			if (docking_partner1_ != NULL)
			{
				systems1->setCurrentText(docking_partner1_->getName());
			}
			if (docking_partner2_ != NULL)
			{
				systems2->setCurrentText(docking_partner2_->getName());
			}
		}
		
		// Swaps the option values between vector backup_ and dialog
		// Is called in show() if has_changed_ is true
		// and in writePreferences if is_redock_ is true
		void DockDialog::swapValues_()
			throw()
		{
			QString temp = algorithms->currentText();
			algorithms->setCurrentText(backup_[0]);
			backup_[0] = temp;

			temp = scoring_functions->currentText();
			scoring_functions->setCurrentText(backup_[1]);
			backup_[1] = temp;

			temp = best_num->text();
			best_num->setText(backup_[2]);
			backup_[2] = temp;

			temp = verbosity->text();
			verbosity->setText(backup_[3]);
			backup_[3] = temp;

			algorithmChosen();
			scoringFuncChosen();
		}
		
		
		// -------------------------------- SLOTS ------------------------------------------------
		// ---------------------------------------------------------------------------------------

		// Shows and raises the dialog.
		// The comboboxes for the docking partners are filled with the loaded systems in BALLView.
		// If the user has selected one or two systems, they are the current items in the comboboxes.
		void DockDialog::show()
		{
			if (is_redock_)
			{
				setCaption("Redocking Options");
				tab_pages->setTabEnabled(tab_pages->page(1), false);
				systems_group->setHidden(true);
				euler_group->setHidden(false);
			}
			else
			{
				setCaption("Docking Options");
				tab_pages->setTabEnabled(tab_pages->page(1), true);
				euler_group->setHidden(true);
				systems_group->setHidden(false);
				fillSystemComboxes_();
			}
			
			if (has_changed_)
			{
				swapValues_();
			}
			
			tab_pages->adjustSize();
			adjustSize();
			
			// always show the first tab page
			tab_pages->setCurrentPage(0);
			
			// show dialog to user
			DockDialogData::show();
		}
		
		// Indicates the OK button was pressed.
		// If we are not doing redocking it checks if two different systems are chosen.
		// Hides the dialog and calls \link DockDialog::applyValues_ applyValues_ \endlink and \link DockDialog::applyProcessors_ applyProcessors_ \endlink.
		void DockDialog::okPressed()
		{
			if (!is_redock_)
			{
				// if less than 2 or 2 equal systems are chosen => Error message!
				if ((systems1->currentText() == "<select>") || 
						(systems2->currentText() == "<select>") || 
						(systems1->currentText() == systems2->currentText()))
				{
					QMessageBox error_message(0,0);
					error_message.warning(0,"Error","Please select two different docking partners!", QMessageBox::Ok, QMessageBox::NoButton);
					return;
				}
			}
			// if no algorithm is chosen => Error message!
			if (algorithms->currentText() == "<select>")
			{
				QMessageBox error_message(0,0);
				error_message.warning(0,"Error","Please select docking algorithm!", QMessageBox::Ok, QMessageBox::NoButton);
				return;
			}
			hide();
			// set options user has chosen
			applyValues_();
			// apply processors, e.g. add hydrogens
			applyProcessors_();
			
			// set property for the two docking partners
			// is needed to identify these original partners for redocking
			AtomContainerIterator it;
			for (it = docking_partner1_->beginAtomContainer(); +it; ++it)
			{
				it->setProperty("DOCKING_PARTNER_1");
			}
			for (it = docking_partner2_->beginAtomContainer(); +it; ++it)
			{
				it->setProperty("DOCKING_PARTNER_2");
			}
			accept();
		}
		
		///////// TODO: take the values which were in the fields when dialog was opened
		void DockDialog::cancelPressed()
		{
			reject();
		}
		
		//
		void DockDialog::resetPressed()
		{
			reset();
		}
		
		// advanced button for algorithm options pressed
		void DockDialog::algAdvancedPressed()
		{
			// show corresponding options dialog
			int index = algorithms->currentItem();
			if (algorithm_dialogs_.has(index))
			{
				switch(index)
				{
					case DockingController::GEOMETRIC_FIT:
						GeometricFitDialog* gfd = dynamic_cast<GeometricFitDialog*> (algorithm_dialogs_[index]);
						gfd->setFlag(is_redock_);
						gfd->exec();
				}
			}
		}
			
		// advanced button for scoring function options pressed
		void DockDialog::scoringAdvancedPressed()
		{
			// show corresponding options dialog
			int index = scoring_functions->currentItem();
			if (scoring_dialogs_.has(index))
			{
				scoring_dialogs_[index]->exec();
			}
		}
		
		// Indicates a system in the combobox was chosen as docking partner 1.
		void DockDialog::partner1Chosen()
		{
			docking_partner1_ = partnerChosen_(systems1->currentText());
		}
		
		// Indicates a system in the combobox was chosen as docking partner 2.
		void DockDialog::partner2Chosen()
		{
			docking_partner2_ = partnerChosen_(systems2->currentText());
		}
		
		// Indicates that a scoring function in the combobox was chosen.
		void DockDialog::scoringFuncChosen()
		{
			// if chosen scoring function has advanced options, enable advanced_button
			int index = scoring_functions->currentItem();
			if (scoring_dialogs_.has(index))
			{
				scoring_advanced_button->setEnabled(true);
			}
			else
			{
				scoring_advanced_button->setEnabled(false);
			}
		}
		
		// Indicates an algorithm in the combobox was chosen.
		void DockDialog::algorithmChosen()
		{
			// if chosen algorithm has advanced options
			int index = algorithms->currentItem();
			if (algorithm_dialogs_.has(index))
			{
				alg_advanced_button->setEnabled(true);
				// disable scoring functions which aren't allowed for chosen algorithm
				for (int i = 0; i < scoring_functions->count(); i++)
				{
					bool found = false;
					for (unsigned int j = 0; j < allowed_sf_[index].size(); j++)
					{
						if (allowed_sf_[index][j] == i)
						{
						 	found = true;
							break;
						}
					}
					scoring_functions->listBox()->item(i)->setSelectable(found);
				}
			}
			else
			{
				alg_advanced_button->setEnabled(false);
				// enable all scoring functions
				for (int i = 0; i < scoring_functions->count(); i++)
				{
					scoring_functions->listBox()->item(i)->setSelectable(true);
				}
			}
			
			// set default scoring function as current item if the current item isn't an allowed scoring function
			if(!scoring_functions->listBox()->item(scoring_functions->currentItem())->isSelectable())
			{
				scoring_functions->setCurrentItem(0);
			}
		}
		
		//
		void DockDialog::browseChargesData()
		{
			selectFile_(*charges_data_lineedit);
		}

		//
		void DockDialog::browseChargesRules()
		{
			selectFile_(*charges_rules_lineedit);
		}

		//
		void DockDialog::browseRadiiData()
		{
			selectFile_(*radii_data_lineedit);
		}

		//
		void DockDialog::browseRadiiRules()
		{
			selectFile_(*radii_rules_lineedit);
		}
		
	} // namespace VIEW
} // namespace BALL
