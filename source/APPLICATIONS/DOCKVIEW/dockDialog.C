// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: dockDialog.C,v 1.1.2.14.2.25 2005/05/05 13:22:15 haid Exp $
//

#include "dockDialog.h"
#include "geometricFitDialog.h"

#include <qpushbutton.h>
#include <qcombobox.h>
#include <qstringlist.h>
#include <qmessagebox.h>
#include <qcheckbox.h>
#include <qlineedit.h>
#include <qradiobutton.h>
#include <qfiledialog.h>
#include <qtabwidget.h>
#include <qtable.h>

#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/STRUCTURE/DOCKING/dockingAlgorithm.h>
#include <BALL/STRUCTURE/DOCKING/geometricFit.h>
#include <BALL/VIEW/KERNEL/threads.h>
#include <BALL/STRUCTURE/DOCKING/energeticEvaluation.h>
#include <BALL/STRUCTURE/DOCKING/amberEvaluation.h>
#include <BALL/STRUCTURE/DOCKING/randomEvaluation.h>
#include <BALL/VIEW/DIALOGS/amberConfigurationDialog.h>
#include <BALL/VIEW/WIDGETS/molecularStructure.h>

namespace BALL
{
	namespace VIEW
	{
		//Default constructor
		DockDialog::DockDialog(QWidget* parent,  const char* name, bool modal, WFlags fl)
			throw()
			: DockDialogData(parent, name, modal, fl),
				ModularWidget(name),
				PreferencesEntry(),
				docking_partner1_(0),
				docking_partner2_(0),
				dock_alg_(0),
				progress_dialog_(0)
				//amber_(0)
		{
		#ifdef BALL_VIEW_DEBUG
			Log.error() << "new DockDialog " << this << std::endl;
		#endif
			
			// register the widget with the MainControl
			ModularWidget::registerWidget(this);
			
			// register QWidgets of Dialog with PreferenceEntry
			// entries of them in the INIFile will be generated
			setINIFileSectionName("DOCKING");
			registerObject_(systems1);
			registerObject_(systems2);
			registerObject_(algorithms);
			registerObject_(scoring_functions);
			registerObject_(best_num);
			registerObject_(verbosity);
			registerObject_(radii_data_lineedit);
			registerObject_(radii_rules_lineedit);
			registerObject_(charges_data_lineedit);
			registerObject_(charges_rules_lineedit);
			registerObject_(normalize_names);
			registerObject_(assign_charges);
			registerObject_(assign_radii);
			registerObject_(build_bonds);
			registerObject_(add_hydrogens);

			//build HashMap for algorithm advanced option dialogs
			//make sure the order of added algorithms is consistent to the enum order
			//because the algorithm with enum value i should be at position i in the combobox
			//otherwise you get the wrong option dialog for an algorithm
			GeometricFitDialog* geo_fit = new GeometricFitDialog(this);
			addAlgorithm("Geometric Fit", GEOMETRIC_FIT, geo_fit);
			
			//build HashMap for scoring function advanced option dialogs
			//make sure the order of added scoring functions is consistent to the enum order
			//because the scoring function with enum value i should be at position i in the Combobox
			//otherwise you get the wrong option dialog for a scoring function
			addScoringFunction("Default", DEFAULT);
			addScoringFunction("Amber Force Field", AMBER_FF, &(MolecularStructure::getInstance(0)->getAmberConfigurationDialog()));
			addScoringFunction("Random", RANDOM);
			
			//own_amber = false;
			
			hide(); 
		}

		//Destructor
		DockDialog::~DockDialog()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << this << " of class DockDialog" << std::endl;
			#endif 
		}
		
		// Assignment operator
		const DockDialog& DockDialog::operator =(const DockDialog& dock_dialog)
			throw()
		{
			if (&dock_dialog != this)
			{
				algorithm_dialogs_ = dock_dialog.algorithm_dialogs_;
				scoring_dialogs_ = dock_dialog.scoring_dialogs_;
				docking_partner1_ = dock_dialog.docking_partner1_;
				docking_partner2_ = dock_dialog.docking_partner2_;
				options_ = dock_dialog.options_;
				id_ = dock_dialog.id_;
				radius_rule_processor_ = dock_dialog.radius_rule_processor_;
				charge_rule_processor_ = dock_dialog.charge_rule_processor_;
				radius_processor_ = dock_dialog.radius_processor_;
				charge_processor_ = dock_dialog.charge_processor_;
			}
			return *this;
		}

		// Sets two systems as docking partners.
		void DockDialog::setSystem(System* system1, System* system2)
			throw() 
		{
			docking_partner1_ = system1;
			docking_partner2_ = system2;
		}
		
		// Adds docking algorithm to Combobox and its advanced option dialogs to HashMap.
		void DockDialog::addAlgorithm(const QString& name, const int algorithm, QDialog* dialog)
			throw()
		{
			if(dialog)
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
			if(dialog)
			{
				// add to ComboBox
				scoring_dialogs_[score_func] = dialog;
			}
			// add to ComboBox
			scoring_functions->insertItem(name, score_func);
		}
		
		// Message handling method.
		void DockDialog::onNotify(Message *message)
			throw()
		{
			if (RTTI::isKindOf<DockingFinishedMessage>(*message))
			{
				unlockComposites();
				DockingFinishedMessage* dfm = RTTI::castTo<DockingFinishedMessage>(*message);
				if(dfm->wasAborted())
				{
					QMessageBox request_message(0,0);
					if( request_message.question(0,"Request","Do you want to see the current Result?", 
																			 "Yes", "No", QString::null, 0, 1))
					{
						return;
					}
					//setStatusbarText("Warning: Docking was aborted; DockResult may be incorrect!", true);
				}
				continueCalculate_((ConformationSet*)(dfm->getConformationSet()));
			}
		}
		
		// Initializes the popup menu Molecular Mechanics with its checkable submenu Docking.
		void DockDialog::initializeWidget(MainControl& main_control)
			throw()
		{
			main_control.initPopupMenu(MainControl::MOLECULARMECHANICS)->setCheckable(true);
			
			String hint = "Dock two systems.";
			id_ = main_control.insertMenuEntry(MainControl::MOLECULARMECHANICS, "&Docking", this,
																				 SLOT(show()), CTRL+Key_D, -1, hint);
		}
		
		//Removes the checkable submenu Docking from the popup menu Molecular Mechanics.
		void DockDialog::finalizeWidget(MainControl& main_control)
			throw()
		{
			main_control.removeMenuEntry(MainControl::DISPLAY, "&Docking", this,
																	 SLOT(show()), CTRL+Key_D);
		}   
		
		// Read the preferences from the INIFile.
		void DockDialog::fetchPreferences(INIFile& file)
			throw()
		{
			PreferencesEntry::readPreferenceEntries(file);
			// call this function to check which algorithm / scoring function is the current item in the combobox
			// and set advanced button enabled if necessary
			algorithmChosen();
			scoringFuncChosen();
		}
		
		// Write the preferences to the INIFile.
		void DockDialog::writePreferences(INIFile& file)
			throw()
		{
			PreferencesEntry::writePreferenceEntries(file);
		}
		
		// Updates the state of menu entry Docking in the popup menu Molecular Mechanics.
		void DockDialog::checkMenu (MainControl& main_control)
			throw()
		{
			CompositeManager& composite_manager = main_control.getCompositeManager();
			
			//iterate over all composites; get to know if there are systems
			HashSet<Composite*>::Iterator composite_it = composite_manager.begin();

			Size num_systems = 0;
			for (; +composite_it; ++composite_it)
			{
				if (RTTI::isKindOf<System>(**composite_it))
				{
					num_systems++;
				}
			}

			//if no or only one system loaded, disable menu entry "Docking"
			if(num_systems > 1)
			{
				menuBar()->setItemEnabled(id_, true);
			}
			else
			{
				menuBar()->setItemEnabled(id_, false);
			}
		}
	
		/// Reset the dialog to the standard values
		void DockDialog::reset()
			throw()
		{
			if (tab_pages->currentPageIndex() == 0)
			{
				// comboboxes
				systems1->setCurrentItem(0);
				systems2->setCurrentItem(0);
				algorithms->setCurrentItem(0);
				scoring_functions->setCurrentItem(0);
				
				// buttons
				scoring_advanced_button->setEnabled(false);
				
				//options
				best_num->setText("100");
				verbosity->setText("1");
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
		
		// Docks the two systems.
		void DockDialog::calculate()
			throw()
		{
			// get options user has chosen
			applyValues_();
			
			// before docking, apply processors, e.g. add hydrogens
			applyProcessors_();
			
			// check which algorithm is chosen and create an DockingAlgorithm object
			int index = algorithms->currentItem();
			switch(index)
			{
				case GEOMETRIC_FIT:
					dock_alg_ =  new GeometricFit();
					break;
			}
			
			// Set up the docking algorithm
			setStatusbarText("setting up docking algorithm...", true);
			// keep the larger protein in System A and the smaller one in System B
			// and setup the algorithm
			if (docking_partner1_->countAtoms() < docking_partner2_->countAtoms())
			{
				dock_alg_->setup(*docking_partner2_, *docking_partner1_, options_);
			}
			else
			{
				dock_alg_->setup(*docking_partner1_, *docking_partner2_, options_);
			}
			
			Options scoring_options;  /////TODO options sind immer leer; als private? applyValues erweitern? ///////////////////////////////
			
			// ============================= WITH MULTITHREADING ====================================
			if (!(getMainControl()->lockCompositesFor(this))) return;
			Log.info() << "vor thread" << std::endl;
			#ifdef BALL_QT_HAS_THREADS
				DockingThread* thread = new DockingThread;
				thread->setDockingAlgorithm(dock_alg_);
				thread->setMainControl(getMainControl());
				
				progress_dialog_ = new DockProgressDialog(this);
				progress_dialog_->fillDialog(systems1->currentText(),
																		systems2->currentText(),
																		algorithms->currentText(),
																		scoring_functions->currentText(),
																		options_,
																		scoring_options);
				progress_dialog_->setDockingAlgorithm(dock_alg_);
				
				
				thread->start();
				progress_dialog_->show();
			// ============================= WITHOUT MULTITHREADING =================================
			#else
				// start docking
				setStatusbarText("starting docking...", true);
				dock_alg_->start();
				setStatusbarText("Docking finished.", true);
				continueCalculate_(dock_alg->getConformationSet());
				// delete instance 
				if (dock_alg_ != NULL)
				{
					delete dock_alg_;
					dock_alg_ = NULL;
				}
			#endif
			
		}
		
		void DockDialog::continueCalculate_(ConformationSet* conformation_set)
			throw()
		{
			Log.error() << "in DockDialog::continueCalculate_" << std::endl;
		
		 	// create scoring function object
			EnergeticEvaluation* scoring = 0;
			//check which scoring function is chosen
			int index = scoring_functions->currentItem();
			
			Options scoring_options;
			switch(index)
			{
				case DEFAULT:
					Log.error() << "in continueCalculate_::DEFAULT" << std::endl;
					scoring = new EnergeticEvaluation();
					break;

				case AMBER_FF:
				{
					Log.info() << "in DockDialog:: Option of Amber FF:" << std::endl;
					AmberFF& ff = MolecularStructure::getInstance(0)->getAmberFF();
					AmberConfigurationDialog* dialog = RTTI::castTo<AmberConfigurationDialog>(*(scoring_dialogs_[index]));
					Log.info() << "blubb" << std::endl;
					//now the Amber force field gets its options
					dialog->applyTo(ff);
					Log.info() << "bla" << std::endl;
					
					Options::Iterator it = ff.options.begin();
					for(; +it; ++it)
					{
						Log.info() << it->first << " : " << it->second << std::endl;
					}
					scoring_options = ff.options;
					//the force field is given to the AmberEvaluation (scoring function) object
					scoring = new AmberEvaluation(ff);
					break;
				}
				case RANDOM:
					Log.error() << "in continueCalculate_::RANDOM" << std::endl;
					scoring = new RandomEvaluation();
					break;
			}
		
			// apply scoring function; set new scores in the conformation set
	   	std::vector<ConformationSet::Conformation> ranked_conformations((*scoring)(*conformation_set));
			conformation_set->setScoring(ranked_conformations);

			// add a new scoring to dock_res_; we need the name, options and score vector of the scoring function
			vector<float> scores;
			for(unsigned int i = 0; i < ranked_conformations.size(); i++)
			{
				scores.push_back(ranked_conformations[i].second);
			}

			DockResult* dock_res = new DockResult(String(algorithms->currentText().ascii()),
																						conformation_set,
																						options_); 
																							
			dock_res->addScoring(String(scoring_functions->currentText().ascii()), scoring_options, scores);

			// add docked system to BALLView structures 
			SnapShot best_result = (*conformation_set)[0];
			
			System* docked_system = new System(conformation_set->getSystem());
			best_result.applySnapShot(*docked_system);
			getMainControl()->deselectCompositeRecursive(docked_system, true);
			getMainControl()->insert(*docked_system);
			
			// send a DockResultMessage
			NewDockResultMessage* dock_res_m = new NewDockResultMessage();
			dock_res_m->setDockResult(*dock_res);
			dock_res_m->setComposite(*docked_system);
			notify_(dock_res_m);

			// delete instance 
			if (scoring != NULL)
			{
				delete scoring;
				scoring = NULL;
			}
			
			Log.info() << "End of calculate" << std::endl;
		}
		
		// set options_ with values user has chosen 
		void DockDialog::applyValues_()
			throw()
		{
			// options for all docking algorithms
			/////////////////////////////////////// TODO allgemeine Options ////////////////////////////////////////////
			//options_[DockingAlgorithm::Option::BEST_NUM] = String(best_num->text().ascii()).toInt();
			options_[GeometricFit::Option::BEST_NUM] = String(best_num->text().ascii()).toInt();
			options_[GeometricFit::Option::VERBOSITY] = String(verbosity->text().ascii()).toInt();
			
			// options for chosen algorithm; options are filled by the corresponding dialog
			int index = algorithms->currentItem();
			switch(index)
			{
				case GEOMETRIC_FIT:
					GeometricFitDialog* dialog = RTTI::castTo<GeometricFitDialog>(*(algorithm_dialogs_[index]));
					dialog->getOptions(options_);
					break;
			}
		}
		
		// apply processors to the systems
		bool DockDialog::applyProcessors_()
			throw()
		{
			Log.error() << "starting apply processors" << std::endl;

			if ((docking_partner1_ == 0) || (docking_partner2_ == 0)) 
			{
				Log.error() << "No two systems given! Aborting..." << std::endl;
				return false;
			}
			
			//add hydrogens to systems and normalize names
			if (add_hydrogens->isChecked())
			{
				if (!docking_partner1_->apply(getFragmentDB().add_hydrogens)) return false;
				if (!docking_partner2_->apply(getFragmentDB().add_hydrogens)) return false;
				if (!docking_partner1_->apply(getFragmentDB().normalize_names)) return false;
				if (!docking_partner2_->apply(getFragmentDB().normalize_names)) return false;
			}
			else if (normalize_names->isChecked())
			{
				if (!docking_partner1_->apply(getFragmentDB().normalize_names)) return false;
				if (!docking_partner2_->apply(getFragmentDB().normalize_names)) return false;
			}
			
			//add bonds to systems
			if (build_bonds->isChecked())
			{
				if (!docking_partner1_->apply(getFragmentDB().build_bonds)) return false;
				if (!docking_partner2_->apply(getFragmentDB().build_bonds)) return false;
			}

			//assign charges and radii
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
			
			getMainControl()->getPrimitiveManager().setMultithreadingMode(false);
			getMainControl()->update(*docking_partner1_, true);
			getMainControl()->update(*docking_partner2_, true);
			getMainControl()->getPrimitiveManager().setMultithreadingMode(true);
			Log.error() << "End of applyProcessors" << std::endl;
			
			return true;
		}
		
		// show chosen file in the dialog
		void DockDialog::selectFile_(QLineEdit& lineedit)
			throw()
		{
			QString s = QFileDialog::getOpenFileName(getWorkingDir().c_str(), "", getMainControl(), "", "Choose a file");

			if (s == QString::null) return;
			setWorkingDirFromFilename_(s.ascii());
			lineedit.setText(s);
		}
		
		// get system which the user has chosen in the dialog as docking partner
		System* DockDialog::partnerChosen_(QString qstr)
			throw()
		{
			//iterate over all composites; find chosen system
			HashSet<Composite*>::iterator composite_it = getMainControl()->getCompositeManager().begin();
				
			for(; +composite_it; ++composite_it)
			{
				System* system = dynamic_cast<System*>(*composite_it);
				if (system != 0 && system->getName() == qstr.ascii())
				{
					return system;
				}
			}
			return 0;
		}
		
		// -------------------------------- SLOTS ------------------------------------------------
		// ---------------------------------------------------------------------------------------

		// Shows and raises the dialog.
		// The comboboxes for the docking partners are filled with the loaded systems in BALLView.
		// If the user has selected one or two systems, they are the current items in the comboboxes.
		void DockDialog::show()
		{
			MainControl* main_control = getMainControl();

			//get the composites
			CompositeManager& composite_manager = main_control->getCompositeManager();
			
			//iterate over all composites; add systems to list
			HashSet<Composite*>::Iterator composite_it = composite_manager.begin();
			
			//selection lists for systems should be empty
			systems1->clear();
			systems2->clear();
			
			//pointer to selected systems
			docking_partner1_ = NULL;
			docking_partner2_ = NULL;
			
			QStringList current_system_list;
			//put <select> in list as the first element
			current_system_list.append("<select>");
			
			//fill current system list and check if user has already selected two systems
			for(; +composite_it; ++composite_it)
			{
				System* system = dynamic_cast<System*>(*composite_it);
				if (system == 0) continue;

				current_system_list.append(system->getName());
				
				//test if the user has selected one or two systems
				//more than 2 selected systems -> error
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
							//if more than 2 systems are selected => Error message!
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
			
			//set selection lists of dialog
			systems1->insertStringList(current_system_list);
			systems2->insertStringList(current_system_list);
		
			// If the user has selected one or two systems, they are the current items in the comboboxes.
			if(docking_partner1_ != NULL)
			{
				systems1->setCurrentText(docking_partner1_->getName());
			}
			if(docking_partner2_ != NULL)
			{
				systems2->setCurrentText(docking_partner2_->getName());
			}
			
			// always show the first tab page
			tab_pages->setCurrentPage(0);
			
			//show dialog to user
			DockDialogData::show();
		}
		
		//
		void DockDialog::okPressed()
		{
			//if less than 2 / more than 2 equal systems are chosen => Error message!
			if ((systems1->currentText() == "<select>") || 
					(systems2->currentText() == "<select>") || 
					(systems1->currentText() == systems2->currentText()))
			{
				#ifdef BALL_VIEW_DEBUG
					Log.error() << "DockDialog: " << "Please select two different docking partners!" << std::endl;
				#endif
					
				QMessageBox error_message(0,0);
				error_message.warning(0,"Error","Please select two different docking partners!", QMessageBox::Ok, QMessageBox::NoButton);
			}
			else
			{
				//if no algorithm is chosen => Error message!
				if(algorithms->currentText() == "<select>")
				{
					#ifdef BALL_VIEW_DEBUG
					Log.error() << "DockDialog: " << "Please select docking algorithm!" << std::endl;
					#endif
					
					QMessageBox error_message(0,0);
					error_message.warning(0,"Error","Please select docking algorithm!", QMessageBox::Ok, QMessageBox::NoButton);
				}
				else
				{
					hide();
					calculate();
				}
			}
		}
		
		//
		void DockDialog::cancelPressed()
		{
			hide();
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
			if(index)
			{
				algorithm_dialogs_[index]->exec();
			}
		}
			
		// advanced button for scoring function options pressed
		void DockDialog::scoringAdvancedPressed()
		{
			// show corresponding options dialog
			int index = scoring_functions->currentItem();
			if(index)
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
		
		// Indicates a scoring function in the combobox was chosen.
		void DockDialog::scoringFuncChosen()
		{
			// if chosen scoring function has advanced options, enable advanced_button
			int index = scoring_functions->currentItem();
			if(scoring_dialogs_.has(index))
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
			// if chosen scoring function has advanced options, enable advanced_button
			int index = algorithms->currentItem();
			if(algorithm_dialogs_.has(index))
			{
				alg_advanced_button->setEnabled(true);
			}
			else
			{
				alg_advanced_button->setEnabled(false);
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
