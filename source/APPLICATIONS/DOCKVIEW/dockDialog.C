// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: dockDialog.C,v 1.1.2.6 2005/01/19 09:41:47 leonhardt Exp $
//

#include "dockDialog.h"
#include <qpushbutton.h>
#include <qcombobox.h>
#include <qstringlist.h>
#include <qmessagebox.h>
#include <qcheckbox.h>
#include <qlineedit.h>
#include <qradiobutton.h>
#include <qfiledialog.h>

#ifndef BALL_KERNEL_SYSTEM_H
# include <BALL/KERNEL/system.h>
#endif
#ifndef BALL_FORMAT_INIFILE_H
#include <BALL/FORMAT/INIFile.h>
#endif

namespace BALL
{
	namespace VIEW
	{
		
		//Constructor
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
			registerObject_(generators);
			registerObject_(eval_functions);
			registerObject_(rank_functions);
			registerObject_(radii_data_lineedit);
			registerObject_(radii_rules_lineedit);
			registerObject_(charges_data_lineedit);
			registerObject_(charges_rules_lineedit);
			//registerObject_(radii_rules_button);
			//registerObject_(charges_rules_button);
			registerObject_(normalize_names);
			registerObject_(assign_charges);
			registerObject_(assign_radii);
			registerObject_(build_bonds);
			registerObject_(add_hydrogens);
		
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

		// ------------------------- helper functions -------------------------------------
		// --------------------------------------------------------------------------------
		
		void DockDialog::initializeWidget(MainControl& main_control)
			throw()
		{
			main_control.initPopupMenu(MainControl::MOLECULARMECHANICS)->setCheckable(true);
			
			String hint = "Dock two systems";
			id_ = main_control.insertMenuEntry(MainControl::MOLECULARMECHANICS, "&Docking", this,
																				 SLOT(show()), CTRL+Key_D, -1, hint);
		}
																				 																		 
		void DockDialog::finalizeWidget(MainControl& main_control)
			throw()
		{
			main_control.removeMenuEntry(MainControl::DISPLAY, "&Docking", this,
																	 SLOT(show()), CTRL+Key_D);
		}   
		
		void DockDialog::checkMenu (MainControl& main_control)
			throw()
		{
			CompositeManager& composite_manager = main_control.getCompositeManager();
			
			//iterate over all composites; get to know if there are systems
			HashSet<Composite*>::iterator composite_it = composite_manager.begin();

			int num_systems = 0;
			for(; +composite_it; ++composite_it)
			{
				if(RTTI::isKindOf<System>(*(*composite_it)))
				{
					num_systems++;
				}
			}
			//no systems loaded, disable menu entry "Docking"
			menuBar()->setItemEnabled(id_, num_systems);
		}
		
		// if the user has selected one or two systems,  
		// they should be the current items in the comboboxes
		void DockDialog::show()
		{
			MainControl* main_control = getMainControl();

			//get the composites
			CompositeManager& composite_manager = main_control->getCompositeManager();
			
			//iterate over all composites; add systems to list
			HashSet<Composite*>::iterator composite_it = composite_manager.begin();
			
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
				if(RTTI::isKindOf<System>(*(*composite_it)))
				{
					System* system = RTTI::castTo<System>(*(*composite_it));
					current_system_list.append(system->getName());
					
					//test if the user has selected one or two systems
					//three selected systems -> error
					if (system->isSelected())
					{
						if(docking_partner1_ == NULL)
						{
							docking_partner1_ = system;
						}
						else
						{
							if(docking_partner2_ == NULL)
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
			//show dialog to user
			DockDialogData::show();
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
		
		/// Reset the dialog to the standard values
		void DockDialog::reset()
			throw()
		{
			// comboboxes
			systems1->setCurrentText("<select>");
			systems2->setCurrentText("<select>");
			generators->setCurrentText("<select>");
			eval_functions->setCurrentText("<select>");
			rank_functions->setCurrentText("<select>");
		
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
		
		/// show chosen file in the dialog
		void DockDialog::selectFile_(QLineEdit& lineedit)
			throw()
		{
			QString s = QFileDialog::getOpenFileName(
										getWorkingDir().c_str(),
										"",
										getMainControl(),
										"",
										"Choose a file" );

			if (s == QString::null) return;
			setWorkingDirFromFilename_(s.ascii());
			lineedit.setText(s);

			QWidget::update();
		}
		
		/// Calculate...
		bool DockDialog::calculate()
			throw()
		{
			// get options user has chosen
			applyValues_();
			// before docking, apply processors, e.g. add hydrogens
			applyProcessors_();
			//setup docking
			docking_.setup(*docking_partner1_,*docking_partner2_,options_)
			Log.info() << "End of calculate" << std::endl;
			return true;
		}
		
		//set options_ with values user has chosen 
		void DockDialog::applyValues_()
			throw()
		{
		
		}
		/// apply the processors to the systems
		bool DockDialog::applyProcessors_()
			throw()
		{
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
			
			//send messages that systems were changed
			CompositeMessage* message = new CompositeMessage;
			message->setComposite(*docking_partner1_);
			message->setType(CompositeMessage::CHANGED_COMPOSITE);
			notify_(message);
			
			CompositeMessage* message2 = new CompositeMessage;
			message2->setComposite(*docking_partner2_);
			message2->setType(CompositeMessage::CHANGED_COMPOSITE);
			notify_(message2);

			return true;
		}
		
		/// merge two systems into one
		void DockDialog::mergeSystems()
			throw()
		{
		
		}
		
		// ------------------------- SLOTS ------------------------------------------------
		// --------------------------------------------------------------------------------

		///
		void DockDialog::genAdvancedPressed()
		{
		
		}
		//
		void DockDialog::evalAdvancedPressed()
		{
		
		}
		
		//
		void DockDialog::rankAdvancedPressed()
		{
		
		}
		
		void DockDialog::browseChargesData()
		{
			selectFile_(*charges_data_lineedit);
		}

		void DockDialog::browseChargesRules()
		{
			selectFile_(*charges_rules_lineedit);
		}

		void DockDialog::browseRadiiData()
		{
			selectFile_(*radii_data_lineedit);
		}

		void DockDialog::browseRadiiRules()
		{
			selectFile_(*radii_rules_lineedit);
		}
		
		///
		void DockDialog::cancelPressed()
		{
			hide();
		}
		
		///
		void DockDialog::resetPressed()
		{
			reset();
		}
		
		///
		void DockDialog::okPressed()
		{
			//if less than 2/ 2 equal systems are selected => Error message!
			if ((systems1->currentText() == "<select>") || (systems2->currentText() == "<select>") 
					|| (systems1->currentText() == systems2->currentText()))
			{
				#ifdef BALL_VIEW_DEBUG
					Log.error() << "DockDialog: " << "Please select two different docking partners!" << std::endl;
				#endif
					
				QMessageBox error_message(0,0);
				error_message.warning(0,"Error","Please select two different docking partners!", QMessageBox::Ok, QMessageBox::NoButton);
			}
			else
			{
				hide();
				calculate();
			}
		}
		
		///
		void DockDialog::partner1Chosen()
		{
			docking_partner1_ = partnerChosen_(systems1->currentText());
		}
		
		///
		void DockDialog::partner2Chosen()
		{
			docking_partner2_ = partnerChosen_(systems2->currentText());
		}
		
		// find the system user has chosen in the dialog as docking partner
		System* DockDialog::partnerChosen_(QString qstr)
			throw()
		{
			//iterate over all composites; find chosen system
			MainControl* main_control = getMainControl();
			CompositeManager& composite_manager = main_control->getCompositeManager();
			HashSet<Composite*>::iterator composite_it = composite_manager.begin();
				
			System* system;
			for(; +composite_it; ++composite_it)
			{
				if(RTTI::isKindOf<System>(*(*composite_it)))
				{
					system = RTTI::castTo<System>(*(*composite_it));
					if(system->getName() == (const char*)qstr)
					{
						break;
					}
				}
			}
			return system;
		}
		
		
	} // namespace VIEW
} // namespace BALL
