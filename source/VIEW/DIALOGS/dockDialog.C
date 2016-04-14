// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <QtWidgets/QPushButton>
#include <QtWidgets/QComboBox>
#include <QtCore/QStringList>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QButtonGroup>

#include <BALL/VIEW/DIALOGS/dockDialog.h>
#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/VIEW/WIDGETS/molecularStructure.h>
#include <BALL/VIEW/WIDGETS/dockingController.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/FORMAT/INIFile.h>
#include <BALL/KERNEL/system.h>
#include <BALL/DATATYPE/options.h>

#ifdef BALL_HAS_FFTW
#include <BALL/DOCKING/geometricFit.h>
#include <BALL/VIEW/DIALOGS/geometricFitDialog.h>
#endif


//#define BALL_VIEW_DEBUG
namespace BALL
{
	namespace VIEW
	{
		// Default constructor
		DockDialog::DockDialog(QWidget* parent,  const char* name)
			: QDialog(parent),
				Ui_DockDialogData(),
				PreferencesEntry(),
				docking_partner1_(0),
				docking_partner2_(0)
		{
		#ifdef BALL_VIEW_DEBUG
			Log.info() << "new DockDialog " << this << std::endl;
		#endif

			setupUi(this);
			setObjectName(name);
			
			// register QWidgets of Dialog with PreferenceEntry
			// entries of them in the INIFile will be generated
			setINIFileSectionName("DOCKING");
			registerWidgets_();
			unregisterObject_(systems1);
			unregisterObject_(systems2);
			
			// set flag
			is_redock_ = false;
			
			hide(); 

			connect( cancel_button, SIGNAL( clicked() ), this, SLOT( cancelPressed() ) );
			connect( ok_button, SIGNAL( clicked() ), this, SLOT( okPressed() ) );
			connect( reset_button, SIGNAL( clicked() ), this, SLOT( resetPressed() ) );
			connect( alg_advanced_button, SIGNAL( clicked() ), this, SLOT( algAdvancedPressed() ) );
			connect( scoring_advanced_button, SIGNAL( clicked() ), this, SLOT( scoringAdvancedPressed() ) );
			connect( systems1, SIGNAL( activated(const QString&) ), this, SLOT( partner1Chosen() ) );
			connect( systems2, SIGNAL( activated(const QString&) ), this, SLOT( partner2Chosen() ) );
			connect( scoring_functions, SIGNAL( activated(const QString&) ), this, SLOT( scoringFuncChosen() ) );
			connect( charges_data_browse, SIGNAL( clicked() ), this, SLOT( browseChargesData() ) );
			connect( charges_rules_browse, SIGNAL( clicked() ), this, SLOT( browseChargesRules() ) );
			connect( radii_data_browse, SIGNAL( clicked() ), this, SLOT( browseRadiiData() ) );
			connect( radii_rules_browse, SIGNAL( clicked() ), this, SLOT( browseRadiiRules() ) );
			connect( algorithms, SIGNAL( activated(const QString&) ), this, SLOT( algorithmChosen() ) );
		}
		
		// Copy constructor.
		DockDialog::DockDialog(const DockDialog& dock_dialog)
			: QDialog(),
				Ui_DockDialogData(),
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
		{
			setupUi(this);
		}
		
		// Destructor
		DockDialog::~DockDialog()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << this << " of class DockDialog" << std::endl;
			#endif 
			
			// remark: QDialogs in HashMaps are deleted automatically because their parent is DockDialog
			// remark: Systems are deleted by Maincontrol
		}
		
		// Assignment operator
		const DockDialog& DockDialog::operator =(const DockDialog& dock_dialog)
		{
			if (&dock_dialog != this)
			{
				is_redock_ = dock_dialog.is_redock_;
				has_changed_ = dock_dialog.has_changed_;
				// dialogs in hashmaps are dynamically allocated in method initializeWidget()
				HashMap<int, QDialog*>::Iterator it = algorithm_dialogs_.begin();
				for (; it != algorithm_dialogs_.end(); ++it)
				{
					delete it->second;
					algorithm_dialogs_.erase(it);
				}
				algorithm_dialogs_ = dock_dialog.algorithm_dialogs_;
				for (it = scoring_dialogs_.begin(); it != scoring_dialogs_.end(); ++it)
				{
					delete it->second;
					scoring_dialogs_.erase(it);
				}
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
		{
			docking_partner1_ = system1;
			docking_partner2_ = system2;
		}
		
		System* DockDialog::getSystem1()
		{
			return docking_partner1_;
		}
					
		System* DockDialog::getSystem2()
		{
		 	return docking_partner2_;
		}
		
		Options& DockDialog::getAlgorithmOptions()
		{
			return algorithm_opt_;
		}
					
		Options& DockDialog::getScoringOptions()
		{
			return scoring_opt_;
		}
		
		// Sets the flags 'is_redock_' and 'has_changed_'
		void DockDialog::isRedock(bool is_redock)
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
		{
			if (dialog)
			{
				// add dialog to HashMap
				algorithm_dialogs_[algorithm] = dialog;
			}
			// add to ComboBox
			algorithms->addItem(name, algorithm);
		}
		
		// Adds scoring function to Combobox and its advanced option dialogs to HashMap, if it has such an dialog.
		void DockDialog::addScoringFunction(const QString& name, const int score_func, QDialog* dialog)
		{
			if (dialog)
			{
				// add scoring function to HashMap for scoring option dialogs
				scoring_dialogs_[score_func] = dialog;
			}
			// add to ComboBox
			scoring_functions->addItem(name, score_func);
		}
		
		// is called by DockingController::initializeWidget()
		// HashMaps for algorithm advanced option dialogs and scoring function advanced option dialogs are built
		// also HashMap with the allowed scoring functions for the different algorithms
		void DockDialog::initializeWidget()
		{
			//build HashMap for algorithm advanced option dialogs
			//make sure the order of added algorithms is consistent to the enum order
			//because the algorithm with enum value i should be at position i in the combobox
			//otherwise you get the wrong option dialog for an algorithm
#ifdef BALL_HAS_FFTW
			GeometricFitDialog* geo_fit = new GeometricFitDialog(this);
			addAlgorithm("Geometric Fit", DockingController::GEOMETRIC_FIT, geo_fit);
#endif
			
			//build HashMap for scoring function advanced option dialogs
			//make sure the order of added scoring functions is consistent to the enum order
			//because the scoring function with enum value i should be at position i in the Combobox
			//otherwise you get the wrong option dialog for a scoring function
			addScoringFunction("Default", DockingController::DEFAULT);
			MolecularStructure* mol_struct = MolecularStructure::getInstance(0);
			if (!mol_struct)
			{
				Log.error() << "Error while building HashMap for scoring function advanced option dialogs! " 
										<< __FILE__ << " " << __LINE__ << std::endl;
				return;
			}
			addScoringFunction("Amber Force Field", DockingController::AMBER_FF, &(mol_struct->getAmberConfigurationDialog()));
			addScoringFunction("MMFF94 Force Field", DockingController::MMFF94_FF, &(mol_struct->getMMFF94ConfigurationDialog()));
			addScoringFunction("Selected Force Field", DockingController::SELECTED_FF, 0);
			
			vector<int> sf;
			sf.push_back(DockingController::DEFAULT);
			sf.push_back(DockingController::AMBER_FF);
			sf.push_back(DockingController::MMFF94_FF);
			sf.push_back(DockingController::SELECTED_FF);
			allowed_sf_[DockingController::GEOMETRIC_FIT] = sf;
		}
		  
		// Read the preferences from the INIFile.
		void DockDialog::fetchPreferences(INIFile& file)
		{
			// read preferences of INI-section docking into the QWidget of the dialog
			PreferencesEntry::readPreferenceEntries(file);
			// read the redocking values from INIFile into vector backup_
			// if INIFile has not yet a section REDOCKING, fill backup_ vector with default values 
			fetchPreferences_(file, "redock_entry_0", "<select>");
			fetchPreferences_(file, "redock_entry_1", "Default");
			fetchPreferences_(file, "redock_entry_2", "100");
			fetchPreferences_(file, "redock_entry_3", "1");
			
			// call this function to check which algorithm / scoring function is the current item in the combobox
			// and set advanced button enabled if necessary
			algorithmChosen();
			scoringFuncChosen();
			
			HashMap<int, QDialog*>::Iterator it = algorithm_dialogs_.begin();
			for (; it != algorithm_dialogs_.end(); ++it)
			{
#ifdef BALL_HAS_FFTW
				GeometricFitDialog* dialog = dynamic_cast<GeometricFitDialog*>(it->second);
				if(dialog)
				{
					dialog->fetchPreferences(file);
				}
#endif
			}
		}
		 
		// function to read the redocking values from INIFile into vector backup_
		// if INIFile has not yet a section REDOCKING, fill backup_ vector with default values
		void DockDialog::fetchPreferences_(INIFile& file, const String& entry, const QString& default_value)
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
		{
			// swap values if dialog is in redocking-modus, because for PreferencesEntry::writePreferenceEntries 
			// the dialog's widgets has to contain the docking values
			if (is_redock_)
			{
				swapValues_();
			}
			PreferencesEntry::writePreferenceEntries(file);
			
			file.appendSection("REDOCKING");
			for (Position i = 0; i < backup_.size(); i++)
			{
				String entry = String("redock_entry_") + String(i);
				file.insertValue("REDOCKING", entry, ascii(backup_[i]));
			}
			
			HashMap<int, QDialog*>::Iterator it = algorithm_dialogs_.begin();
			for (; it != algorithm_dialogs_.end();  ++it)
			{
#ifdef BALL_HAS_FFTW
				GeometricFitDialog* dialog = dynamic_cast<GeometricFitDialog*>(it->second);
				if(dialog)
				{
					dialog->writePreferences(file);
				}
#endif
			}
		}
		
		/// Reset the dialog to the standard values
		void DockDialog::reset()
		{
			if (tab_pages->currentIndex() == 0)
			{
				// comboboxes
				algorithms->setCurrentIndex(0);
				scoring_functions->setCurrentIndex(0);
				
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
					systems1->setCurrentIndex(0);
					systems2->setCurrentIndex(0);
				}
			}
			
			if (tab_pages->currentIndex() == 1)
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
		{
			algorithm_opt_.clear();
			scoring_opt_.clear();
			// options for all docking algorithms
			///////////////// TODO common options should be in class DockingAlgorithm ////////////////////////////////////////////
			//options_[DockingAlgorithm::Option::BEST_NUM] = String(best_num->text().ascii()).toInt();
			try
			{
#ifdef BALL_HAS_FFTW
				algorithm_opt_[GeometricFit::Option::BEST_NUM] = ascii(best_num->text()).toInt();
				algorithm_opt_[GeometricFit::Option::VERBOSITY] = ascii(verbosity->text()).toInt();
#endif
			}
			catch (Exception::InvalidFormat&)
			{
				Log.error() << "Conversion from String to int failed: invalid format! " << __FILE__ << " " << __LINE__<< std::endl;
				return;
			}
			// options for chosen algorithm; options are filled by the corresponding dialog
			Index index = algorithms->currentIndex();
			switch(index)
			{
				case DockingController::GEOMETRIC_FIT:
#ifdef BALL_HAS_FFTW
					GeometricFitDialog* dialog = RTTI::castTo<GeometricFitDialog>(*(algorithm_dialogs_[index]));
					dialog->getOptions(algorithm_opt_);
#endif
					break;
			}
			
			// options for redocking (euler angles)
			if(is_redock_)
			{
				try
					{
#ifdef BALL_HAS_FFTW	
						algorithm_opt_[GeometricFit::Option::PHI_MIN] = ascii(phi_min->text()).toFloat();
						algorithm_opt_[GeometricFit::Option::PHI_MAX] = ascii(phi_max->text()).toFloat();
						algorithm_opt_[GeometricFit::Option::DEG_PHI] = ascii(delta_phi->text()).toFloat();
						algorithm_opt_[GeometricFit::Option::PSI_MIN] = ascii(psi_min->text()).toFloat();
						algorithm_opt_[GeometricFit::Option::PSI_MAX] = ascii(psi_max->text()).toFloat();
						algorithm_opt_[GeometricFit::Option::DEG_PSI] = ascii(delta_psi->text()).toFloat();
						algorithm_opt_[GeometricFit::Option::THETA_MIN] = ascii(theta_min->text()).toFloat();
						algorithm_opt_[GeometricFit::Option::THETA_MAX] = ascii(theta_max->text()).toFloat();
						algorithm_opt_[GeometricFit::Option::DEG_THETA] = ascii(delta_theta->text()).toFloat();
#endif
					}
				catch(Exception::InvalidFormat&)
					{
						Log.error() << "Conversion from String to float failed: invalid format! " << __FILE__ << " " << __LINE__<< std::endl;
						return;
					}
			}
			else
				{
#ifdef BALL_HAS_FFTW
					algorithm_opt_[GeometricFit::Option::PHI_MIN] = GeometricFit::Default::PHI_MIN;
					algorithm_opt_[GeometricFit::Option::PHI_MAX] = GeometricFit::Default::PHI_MAX;
					algorithm_opt_[GeometricFit::Option::DEG_PHI] = (float) 
																				algorithm_opt_.getReal(GeometricFit::Option::DEGREE_INTERVAL);

					algorithm_opt_[GeometricFit::Option::PSI_MIN] = GeometricFit::Default::PSI_MIN;
					algorithm_opt_[GeometricFit::Option::PSI_MAX] = GeometricFit::Default::PSI_MAX;
					algorithm_opt_[GeometricFit::Option::DEG_PSI] = (float)
																				algorithm_opt_.getReal(GeometricFit::Option::DEGREE_INTERVAL);

					algorithm_opt_[GeometricFit::Option::THETA_MIN] = GeometricFit::Default::THETA_MIN;
					algorithm_opt_[GeometricFit::Option::THETA_MAX] = GeometricFit::Default::THETA_MAX;
					algorithm_opt_[GeometricFit::Option::DEG_THETA] = (float) 
																				algorithm_opt_.getReal(GeometricFit::Option::DEGREE_INTERVAL);
#endif
				}
			
			// options for chosen scoring function
			index = scoring_functions->currentIndex();
			switch(index)
			{
				case DockingController::AMBER_FF:
				{
					MolecularStructure* mol_struct = MolecularStructure::getInstance(0);
					if (!mol_struct)
					{
						Log.error() << "Error while applying options of AMBER_FF scoring function! " 
												<< __FILE__ << " " << __LINE__<< std::endl;
						return;
					}
					AmberFF& ff = mol_struct->getAmberFF();
					AmberConfigurationDialog* dialog = RTTI::castTo<AmberConfigurationDialog>(*(scoring_dialogs_[index]));
					
					// now the Amber force field gets its options
					dialog->applyTo(ff);
					scoring_opt_ = ff.options;
				}
			}
		}
		
		// apply processors to the systems
		bool DockDialog::applyProcessors_()
		{
			if ((docking_partner1_ == 0) || (docking_partner2_ == 0)) 
			{
				Log.error() << "No two systems given! Aborting... " << __FILE__ << " " << __LINE__<< std::endl;
				return false;
			}
			
			MainControl* main_control = MainControl::getInstance(0);
			if (!main_control)
			{
				Log.error() << "Error while adding hydrogens! " << __FILE__ << " " << __LINE__ << std::endl;
				return false;
			}
			FragmentDB& frag_db = const_cast<FragmentDB&>(main_control->getFragmentDB());
			
			// add hydrogens to systems and normalize names
			if (add_hydrogens->isChecked())
			{
				if (!docking_partner1_->apply(frag_db.add_hydrogens)) return false;
				if (!docking_partner2_->apply(frag_db.add_hydrogens)) return false;
				if (!docking_partner1_->apply(frag_db.normalize_names)) return false;
				if (!docking_partner2_->apply(frag_db.normalize_names)) return false;
			}
			else if (normalize_names->isChecked())
			{
				if (!docking_partner1_->apply(frag_db.normalize_names)) return false;
				if (!docking_partner2_->apply(frag_db.normalize_names)) return false;
			}
			
			// add bonds to systems
			if (build_bonds->isChecked())
			{
				if (!docking_partner1_->apply(frag_db.build_bonds)) return false;
				if (!docking_partner2_->apply(frag_db.build_bonds)) return false;
			}

			// assign charges and radii
			try
			{
				if (assign_charges->isChecked())
				{
					if (charges_data_button->isChecked())
					{
						charge_processor_.setFilename(ascii(charges_data_lineedit->text()));
						if (!docking_partner1_->apply(charge_processor_)) return false;
						if (!docking_partner2_->apply(charge_processor_)) return false;
					}
					else
					{
						INIFile inifile(ascii(charges_rules_lineedit->text()));
						charge_rule_processor_ = ChargeRuleProcessor(inifile);
						if (!docking_partner1_->apply(charge_rule_processor_)) return false;
						if (!docking_partner2_->apply(charge_rule_processor_)) return false;
					}
				}
				if (assign_radii->isChecked())
				{
					if (radii_data_button->isChecked())
					{
						radius_processor_.setFilename(ascii(radii_data_lineedit->text()));
						if (!docking_partner1_->apply(radius_processor_)) return false;
						if (!docking_partner2_->apply(radius_processor_)) return false;
					}
					else
					{
						INIFile inifile(ascii(radii_rules_lineedit->text()));
						radius_rule_processor_ = RadiusRuleProcessor(inifile);
						if (!docking_partner1_->apply(radius_rule_processor_)) return false;
						if (!docking_partner2_->apply(radius_rule_processor_)) return false;
					}
				}
			}
			catch (Exception::FileNotFound& e)
			{
				Log.error() << "Invalid file " << e.getFilename() << " " << __FILE__ << " " << __LINE__ << std::endl;
				return false;
			}
		
			main_control->setMultithreading(false);
			main_control->update(*docking_partner1_, true);
			main_control->update(*docking_partner2_, true);
			main_control->setMultithreading(true);
			return true;
		}
		
		// show chosen file in the dialog
		void DockDialog::selectFile_(QLineEdit& lineedit)
		{
			MainControl* main_control = MainControl::getInstance(0);
			if (!main_control)
			{
				Log.error() << "Error while selecting file! " << __FILE__ << " " << __LINE__ << std::endl;
				return;
			}

			QString s = QFileDialog::getOpenFileName(0, "Choose a file", 
																							 main_control->getWorkingDir().c_str());

			if (s == QString::null) return;
			main_control->setWorkingDir(ascii(s));
			lineedit.setText(s);
		}
		
		// Function to fill the system comboboxes.
		// If the user has already selected one or two systems, they are the current items in the comboboxes.
		void DockDialog::fillSystemComboboxes_()
		{
			// pointer to selected systems
			docking_partner1_ = NULL;
			docking_partner2_ = NULL;
			
			// vector of pointers to the loaded systems
			loaded_systems_.clear();

			QStringList current_system_list;
			// put <select> in list as the first element
			current_system_list.append("<select>");
			
			// get the composites
			MainControl* main_control = getMainControl();
			if (!main_control)
			{
				BALLVIEW_DEBUG
				return;
			}
			CompositeManager& composite_manager = main_control->getCompositeManager();
			
			// iterate over all composites; add systems to list
			HashSet<Composite*>::Iterator composite_it = composite_manager.begin();
			
			// fill current system list and check if user has already selected two systems;
			// vector of pointers to all loaded systems is also filled (same order as current system list)
			for (; +composite_it; ++composite_it)
			{
				System* system = dynamic_cast<System*>(*composite_it);
				if (system == 0) continue;

				current_system_list << system->getName().c_str();
				loaded_systems_.push_back(system);			
			}

			std::list<Composite*> cl = main_control->getMolecularControlSelection();
			if (cl.size() == 2)
			{
				list<Composite*>::iterator lit = cl.begin();
				System* system1 = dynamic_cast<System*>(*lit);
				lit++;
				System* system2 = dynamic_cast<System*>(*lit);

				if (system1 && system2)
				{
					docking_partner1_ = system1;
					docking_partner2_ = system2;
				}
			}

			// set selection lists of dialog
			systems1->clear();
			systems1->addItems(current_system_list);
			systems2->clear();
			systems2->addItems(current_system_list);
			
			// If the user has selected one or two systems, they are the current items in the comboboxes.
			if (docking_partner1_ != NULL)
			{
				systems1->setCurrentIndex(systems1->findText(docking_partner1_->getName().c_str()));
			}
			if (docking_partner2_ != NULL)
			{
				systems2->setCurrentIndex(systems2->findText(docking_partner2_->getName().c_str()));
			}
		}
		
		// Swaps the option values between vector backup_ and dialog
		// Is called in show() if has_changed_ is true
		// and in writePreferences if is_redock_ is true
		void DockDialog::swapValues_()
		{
			QString temp = algorithms->currentText();
			algorithms->setCurrentIndex(algorithms->findText(backup_[0]));
			backup_[0] = temp;

			temp = scoring_functions->currentText();
			scoring_functions->setCurrentIndex(scoring_functions->findText(backup_[1]));
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
		// In case of docking, the comboboxes for the docking partners are filled with the loaded systems in BALLView.
		int DockDialog::exec()
		{
			if (is_redock_)
			{
				setWindowTitle("Redocking Options");
				tab_pages->setTabEnabled(1, false);
				systems_group->setHidden(true);
				euler_group->setHidden(false);
			}
			else
			{
				setWindowTitle("Docking Options");
				tab_pages->setTabEnabled(1, true);
				euler_group->setHidden(true);
				systems_group->setHidden(false);
				fillSystemComboboxes_();
			}
			
			if (has_changed_)
			{
				swapValues_();
			}
			
			tab_pages->adjustSize();
			adjustSize();
			
			// always show the first tab page
			tab_pages->setCurrentIndex(0);
			
			// show dialog to user
			return QDialog::exec();
		}
		
		// Indicates the OK button was pressed.
		// If we are not doing redocking it checks if two different systems are chosen.
		// Hides the dialog and calls applyValues_ and applyProcessors_.
		void DockDialog::okPressed()
		{
			if (!is_redock_)
			{
				// if less than 2 or 2 equal systems are chosen => Error message!
				if ((systems1->currentText() == "<select>") || 
						(systems2->currentText() == "<select>") || 
						(systems1->currentText() == systems2->currentText()))
				{
					QMessageBox error_message("Error","Please select two different docking partners!", 
																		QMessageBox::Critical,
																		QMessageBox::Ok,
																		QMessageBox::NoButton,
																		QMessageBox::NoButton);
					error_message.exec();
					return;
				}
			}
			// if no algorithm is chosen => Error message!
			if (algorithms->currentText() == "<select>")
			{
				QMessageBox error_message("Error", "Please select docking algorithm!",
																	QMessageBox::Critical,
																	QMessageBox::Ok,
																	QMessageBox::NoButton,
																	QMessageBox::NoButton);
				error_message.exec();
				return;
			}
			hide();
			// set options user has chosen
			applyValues_();
			// apply processors for first docking, e.g. add hydrogens
			if (!is_redock_)
			{
				applyProcessors_();
			}
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
			storeValues();
		}
		
		void DockDialog::cancelPressed()
		{
			restoreValues();
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
			Index index = algorithms->currentIndex();
			if (algorithm_dialogs_.has(index))
			{
				switch(index)
				{
					case DockingController::GEOMETRIC_FIT:
#ifdef BALL_HAS_FFTW
						GeometricFitDialog* gfd = dynamic_cast<GeometricFitDialog*> (algorithm_dialogs_[index]);
						gfd->isRedock(is_redock_);
						gfd->exec();
#endif
						break;
				}
			}
		}
			
		// advanced button for scoring function options pressed
		void DockDialog::scoringAdvancedPressed()
		{
			// show corresponding options dialog
			Index index = scoring_functions->currentIndex();
			if (scoring_dialogs_.has(index))
			{
				scoring_dialogs_[index]->exec();
			}
		}
		
		// Indicates a system in the combobox was chosen as docking partner 1.
		void DockDialog::partner1Chosen()
		{
			int chosen_system = systems1->currentIndex();
			// if item 0 (<select>) is chosen, do nothing
			if (chosen_system)
			{
				docking_partner1_ = loaded_systems_[chosen_system - 1];
		 	}
		}
		
		// Indicates a system in the combobox was chosen as docking partner 2.
		void DockDialog::partner2Chosen()
		{
			int chosen_system = systems2->currentIndex();
			// if item 0 (<select>) is chosen, do nothing
			if (chosen_system)
			{
				docking_partner2_ = loaded_systems_[chosen_system - 1];
		 	}
		}
		
		// Indicates that a scoring function in the combobox was chosen.
		void DockDialog::scoringFuncChosen()
		{
			// if chosen scoring function has advanced options, enable advanced_button
			Index index = scoring_functions->currentIndex();
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
			Index index = algorithms->currentIndex();
			if (algorithm_dialogs_.has(index))
			{
				alg_advanced_button->setEnabled(true);
				// disable scoring functions which aren't allowed for chosen algorithm
				for (Index i = 0; i < scoring_functions->count(); i++)
				{
					bool found = false;
					for (Position j = 0; j < allowed_sf_[index].size(); j++)
					{
						if (allowed_sf_[index][j] == i)
						{
						 	found = true;
							break;
						}
					}
//   					scoring_functions->listBox()->item(i)->setSelectable(found); ????
				}
			}
			else
			{
				// current item is <select>
				alg_advanced_button->setEnabled(false);
				// enable all scoring functions
				for (int i = 0; i < scoring_functions->count(); i++)
				{
//   					scoring_functions->listBox()->item(i)->setSelectable(true); ????
				}
			}
			
			// set default scoring function as current item if the current item isn't an allowed scoring function
//   			if(!scoring_functions->listBox()->item(scoring_functions->currentIndex())->isSelectable())
//   			{
//   				scoring_functions->setCurrentIndex(0);
//   			}
			// ?????????
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
