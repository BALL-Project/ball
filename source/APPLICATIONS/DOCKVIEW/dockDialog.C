// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: dockDialog.C,v 1.1.2.4 2005/01/11 15:22:50 haid Exp $
//

#include "dockDialog.h"
#include <qpushbutton.h>
#include <qcombobox.h>
#include <qstringlist.h>
#include <qmessagebox.h>
#include <qcheckbox.h>

#ifndef BALL_KERNEL_SYSTEM_H
# include <BALL/KERNEL/system.h>
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
		
		void DockDialog::show()
		{
			MainControl* main_control = getMainControl();

			//get the composites
			CompositeManager& composite_manager = main_control->getCompositeManager();
			
			//iterate over all composites; add systems to list
			HashSet<Composite*>::iterator composite_it = composite_manager.begin();

			assign_charges->setChecked( TRUE );
			
			//selection lists for systems should be empty
			systems1->clear();
			systems2->clear();
			//pointer to selected systems should be NULL
			selected_system1_ = NULL;
			selected_system2_ = NULL;
			
			QStringList current_system_list;
			
			for(; +composite_it; ++composite_it)
			{
				if(RTTI::isKindOf<System>(*(*composite_it)))
				{
					System* system = RTTI::castTo<System>(*(*composite_it));
					current_system_list.append(system->getName());
					
					//check if user has already selected two systems 
					if (system->isSelected())
					{
						if(selected_system1_ == NULL)
						{
							selected_system1_ = system;
						}
						else
						{
							if(selected_system2_ == NULL)
							{
								selected_system2_ = system;
							}
							else
							{
								//if more than 2 systems are selected => Error message!
								#ifdef BALL_VIEW_DEBUG
									Log.error() << "DockDialog " << " More than two systems selected! " << std::endl;
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
		
			if(selected_system1_ != NULL)
			{
				systems1->setCurrentText(selected_system1_->getName());
				//Log.error() << "current item: " << systems1->currentItem() << std::endl;
			}
			if(selected_system2_ != NULL)
			{
				systems2->setCurrentText(selected_system2_->getName());
				//Log.error() << "current item1: " << systems1->currentItem() << std::endl;
				//Log.error() << "current item2: " << systems2->currentItem() << std::endl;
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
		
			// processors
			normalize_names->setChecked(false);
			assign_charges->setChecked(true);
			assign_radii->setChecked(true);
			build_bonds->setChecked(false);
			add_hydrogens->setChecked(false);
		}
		
		/// Calculate...
		bool DockDialog::calculate()
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
		
		///
		void DockDialog::cancelPressed()
		{
			hide();
		}
		///
		void DockDialog::okPressed()
		{
			hide();
			calculate();
		}

		///
		void DockDialog::resetPressed()
		{
			reset();
		}
		
	} // namespace VIEW
} // namespace BALL
