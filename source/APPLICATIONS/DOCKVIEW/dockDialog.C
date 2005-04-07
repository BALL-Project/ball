// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: dockDialog.C,v 1.1.2.14.2.19 2005/04/07 17:02:06 leonhardt Exp $
//

#include "dockDialog.h"
#include "geometricFitDialog.h"
#include "dockProgressDialog.h"

#include <qpushbutton.h>
#include <qcombobox.h>
#include <qstringlist.h>
#include <qmessagebox.h>
#include <qcheckbox.h>
#include <qlineedit.h>
#include <qtextedit.h>
#include <qradiobutton.h>
#include <qfiledialog.h>
#include <qtabwidget.h>
#include <qtable.h>

#include <BALL/KERNEL/system.h>
#include <BALL/FORMAT/INIFile.h>
#include <BALL/STRUCTURE/DOCKING/energeticEvaluation.h>
#include <BALL/STRUCTURE/DOCKING/amberEvaluation.h>
#include <BALL/STRUCTURE/DOCKING/randomEvaluation.h>
#include <BALL/VIEW/DIALOGS/amberConfigurationDialog.h>
#include <BALL/VIEW/WIDGETS/molecularStructure.h>


namespace BALL
{
	namespace VIEW
	{
		
		DockDialog::DockDialog(QWidget* parent,  const char* name, bool modal, WFlags fl)
			throw()
			: DockDialogData(parent, name, modal, fl),
				ModularWidget(name),
				PreferencesEntry()
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
			GeometricFitDialog* geo_fit = new GeometricFitDialog(this);
			addAlgorithm("Geometric Fit", GEOMETRIC_FIT, geo_fit);
			
			//build HashMap for scoring function advanced option dialogs
			addScoringFunction("Default", DEFAULT);
			AmberConfigurationDialog* amber = new AmberConfigurationDialog(this); 
			addScoringFunction("Amber Force Field", AMBER_FF, amber);
			addScoringFunction("Random", RANDOM);
			
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
		
		/** Assignment operator
		*/
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
		
		// ------------------------- helper functions -------------------------------------
		// --------------------------------------------------------------------------------

		// add docking algorithm to HashMap and ComboBox
		void DockDialog::addAlgorithm(const QString& name, const int algorithm, QDialog* dialog)
			throw()
		{
			algorithm_dialogs_[algorithm] = dialog;
			algorithms->insertItem(name, algorithm);
		}
		
		// add scoring function to HashMap and ComboBox
		void DockDialog::addScoringFunction(const QString& name, const int score_func, QDialog* dialog)
			throw()
		{
			if(dialog)
			{
				scoring_dialogs_[score_func] = dialog;
			}
			scoring_functions->insertItem(name, score_func);
		}
		
		// Set the systems for docking
		void DockDialog::setSystem(System* system1, System* system2)
			throw() 
		{
			docking_partner1_ = system1;
			docking_partner2_ = system2;
		}
		
		//
		void DockDialog::initializeWidget(MainControl& main_control)
			throw()
		{
			main_control.initPopupMenu(MainControl::MOLECULARMECHANICS)->setCheckable(true);
			
			String hint = "Dock two systems.";
			id_ = main_control.insertMenuEntry(MainControl::MOLECULARMECHANICS, "&Docking", this,
																				 SLOT(show()), CTRL+Key_D, -1, hint);
		}
		
		void DockDialog::finalizeWidget(MainControl& main_control)
			throw()
		{
			main_control.removeMenuEntry(MainControl::DISPLAY, "&Docking", this,
																	 SLOT(show()), CTRL+Key_D);
		}   
		
		/// Read the preferences from a INIFile
		void DockDialog::fetchPreferences(INIFile& file)
			throw()
		{
			PreferencesEntry::readPreferenceEntries(file);
		}
		
		/// Write the preferences to a INIFile
		void DockDialog::writePreferences(INIFile& file)
			throw()
		{
			PreferencesEntry::writePreferenceEntries(file);
		}
		
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
		
		/// Calculate...
		bool DockDialog::calculate()
			throw()
		{
			// get options user has chosen
			applyValues_();
			
			// before docking, apply processors, e.g. add hydrogens
			applyProcessors_();
			
			//check which algorithm is chosen
			DockingAlgorithm* dock_alg = 0;
			int index = algorithms->currentItem();
			switch(index)
			{
				case GEOMETRIC_FIT:
					dock_alg =  new GeometricFit();
					break;
			}
			
			// keep the larger protein in System A and the smaller one in System B
			if (docking_partner1_->countAtoms() < docking_partner2_->countAtoms())
			{
				dock_alg->setup(*docking_partner2_, *docking_partner1_, options_);
			}
			else
			{
				dock_alg->setup(*docking_partner1_, *docking_partner2_, options_);
			}
			
			DockProgressDialog progress;
			QString s = "Docking partner 1: ";
			progress.options->append(s.append(docking_partner1_->getName()));
			s = "Docking partner 2: ";
			progress.options->append(s.append(docking_partner2_->getName()));
			s = "Algorithm: ";
			progress.options->append(s.append(algorithms->currentText()));
			progress.options->append("\n*** Options ***");
			
			Options::Iterator it = options_.begin();
			for(; +it; ++it)
			{
				s = it->first;
				s.append(" : ");
				progress.options->append(s.append(it->second));
			}
			progress.exec();
//   			progress.show(); // to use with multithreading later
			
			// start docking
			Log.error() << "starting docking" << std::endl;
			dock_alg->start();
			Log.error() << "finished docking" << std::endl;
			
			///////////////// BEST_NUM ist Option von DockingAlgorithm!!!!!!!!!!!!!!!!
			ConformationSet conformation_set = dock_alg->getConformationSet(options_.getInteger(GeometricFit::Option::BEST_NUM));
			
			// create scoring function object
			EnergeticEvaluation* scoring = 0;
			//check which scoring function is chosen
			index = scoring_functions->currentItem();
			switch(index)
			{
				case DEFAULT:
					scoring = new EnergeticEvaluation();
					break;
				case AMBER_FF:
				{
					AmberFF& ff = MolecularStructure::getInstance(0)->getAmberFF();
					AmberConfigurationDialog* dialog = RTTI::castTo<AmberConfigurationDialog>(*(scoring_dialogs_[index]));
					dialog->applyTo(ff);
					Log.info() << "in DockDialog:: Option of Amber FF:" << std::endl;
					Options::Iterator it = ff.options.begin();
					for(; +it; ++it)
					{
						Log.info() << it->first << " : " << it->second << std::endl;
					}
					scoring = new AmberEvaluation(ff);
					break;
				}
				case RANDOM:
					scoring = new RandomEvaluation();
					break;
			}
		
			// apply scoring function; set new scores in the conformation set
			std::vector<ConformationSet::Conformation> ranked_conformations = (*scoring)(conformation_set);
			conformation_set.setScoring(ranked_conformations);

			// store docking information and result in a DockResult object
			QString docking_alg = algorithms->currentText();
			vector<float> scores;
			for(unsigned int i = 0; i < ranked_conformations.size(); i++)
			{
				scores.push_back(ranked_conformations[i].second);
			}

			DockResult* dock_res = new DockResult(docking_alg,
																							conformation_set,
																							options_);
																							
			dock_res->addScoring(scoring_functions->currentText(), options_, scores);

			// add docked system to BALLView structures 
			SnapShot best_result = conformation_set[0];
			
			System* docked_system = new System(conformation_set.getSystem());
			best_result.applySnapShot(*docked_system);
			//docked_system->deselect();
			if(docked_system->isSelected())
			{
				Log.info() << "system is selcted" << std::endl;
			}	
			getMainControl()->insert(*docked_system);
			
			// send a DockResultMessage
			NewDockResultMessage* dock_res_m = new NewDockResultMessage();
			dock_res_m->setDockResult(*dock_res);
			dock_res_m->setComposite(*docked_system);
			notify_(dock_res_m);
			
			
  		delete dock_alg;
  		delete scoring;
			
			Log.info() << "End of calculate" << std::endl;
			return true;
		}
		
		// set options_ with values user has chosen 
		void DockDialog::applyValues_()
			throw()
		{
			// options for all docking algorithms
			//options_[DockingAlgorithm::Option::BEST_NUM] = String(best_num->text().ascii()).toInt();
			options_[GeometricFit::Option::BEST_NUM] = String(best_num->text().ascii()).toInt();
			options_[GeometricFit::Option::VERBOSITY] = String(verbosity->text().ascii()).toInt();
			
			// options for chosen algorithm
			int index = algorithms->currentItem();
		
			switch(index)
			{
				case GEOMETRIC_FIT:
					GeometricFitDialog* dialog = RTTI::castTo<GeometricFitDialog>(*(algorithm_dialogs_[index]));
					dialog->getOptions(options_);
					break;
			}
			
			Options::Iterator it = options_.begin();
			for(; +it; ++it)
			{
				Log.info() << "Options:" << std::endl;
				Log.info() << it->first << " : " << it->second << std::endl;
			}
		}
		
		// apply the processors to the systems
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
		
		// -------------------------------- SLOTS ------------------------------------------------
		// ---------------------------------------------------------------------------------------

		// if the user has selected one or two systems,  
		// they should be the current items in the comboboxes
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
			//put <select> in list as the first element
			current_system_list.prepend("<select>");
			
			//set selection lists of dialog
			systems1->insertStringList(current_system_list);
			systems2->insertStringList(current_system_list);
		
			if(docking_partner1_ != NULL)
			{
				systems1->setCurrentText(docking_partner1_->getName());
			}
			if(docking_partner2_ != NULL)
			{
				systems2->setCurrentText(docking_partner2_->getName());
			}
			
			tab_pages->setCurrentPage(0);
			
			//show dialog to user
			DockDialogData::show();
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
		
		//
		void DockDialog::okPressed()
		{
			//if less than 2/ more than 2 equal systems are chosen => Error message!
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
		void DockDialog::partner1Chosen()
		{
			docking_partner1_ = partnerChosen_(systems1->currentText());
		}
		
		//
		void DockDialog::partner2Chosen()
		{
			docking_partner2_ = partnerChosen_(systems2->currentText());
		}
		
		//
		void DockDialog::algorithmChosen()
		{
		
		}
		
		//
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
		
	} // namespace VIEW
} // namespace BALL
