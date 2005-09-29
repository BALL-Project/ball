// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: amberConfigurationDialog.C,v 1.13.4.2 2005/09/29 14:01:23 amoll Exp $
//

#include <BALL/VIEW/DIALOGS/amberConfigurationDialog.h>
#include <BALL/MOLMEC/AMBER/amber.h>
#include <BALL/SYSTEM/path.h>

#include <qfiledialog.h>
#include <qlineedit.h>
#include <qradiobutton.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <qbuttongroup.h>

namespace BALL
{
	namespace VIEW
	{

		AmberConfigurationDialog::AmberConfigurationDialog(QWidget* parent, const char* name)
			:	AmberConfigurationDialogData(parent, name),
				amber_(0)
		{
			setINIFileSectionName("AMBER");

			registerObject_(nonbonded_cutoff_line_edit);
			registerObject_(vdw_cutoff_line_edit);
			registerObject_(vdw_cuton_line_edit);
			registerObject_(electrostatic_cutoff_line_edit);
			registerObject_(electrostatic_cuton_line_edit);
			registerObject_(scaling_electrostatic_1_4_line_edit);
			registerObject_(scaling_vdw_1_4_line_edit);

			registerObject_(dielectric_group);
			registerObject_(assign_charges_checkBox);
			registerObject_(assign_typenames_checkBox);
			registerObject_(assign_types_checkBox);
			registerObject_(overwrite_charges_checkBox);
			registerObject_(overwrite_typenames_checkBox);

			registerObject_(parameter_file_edit);
			registerObject_(max_unassigned_atoms);
		}

		AmberConfigurationDialog::~AmberConfigurationDialog()
		{
		}

		void AmberConfigurationDialog::browseParameterFiles()
		{
			// look up the full path of the parameter file
			Path p;
			String filename = p.find(parameter_file_edit->text().ascii());

			if (filename == "")
			{
				filename = parameter_file_edit->text().ascii();
			}
			QString tmp = filename.c_str();
			QString result = QFileDialog::getOpenFileName(tmp, "*.ini", 0, "Select an AMBER parameter file");
			if (!result.isEmpty())
			{
				// store the new filename in the lineedit field

				parameter_file_edit->setText(filename.c_str());
			}
		}

		void AmberConfigurationDialog::reject()
		{
			hide();
			PreferencesEntry::restoreValues();
		}

		void AmberConfigurationDialog::accept()
		{
			if (amber_ != 0) applyTo(*amber_);
			hide();
			PreferencesEntry::storeValues();
		}


		String AmberConfigurationDialog::getValue_(const QCheckBox* box) const
		{
			if (box->isChecked()) return true;
			else 									return false;
		}

		float AmberConfigurationDialog::getValue_(const QLineEdit* edit) const
			throw(Exception::InvalidFormat)
		{
			return String(edit->text().ascii()).toFloat();
		}

		void AmberConfigurationDialog::applyTo(AmberFF& amber)
			throw()
		{
			try
			{
				amber.options[AmberFF::Option::ASSIGN_TYPES] = getValue_(assign_types_checkBox);
				amber.options[AmberFF::Option::ASSIGN_CHARGES] = getValue_(assign_charges_checkBox);
				amber.options[AmberFF::Option::ASSIGN_TYPENAMES] = getValue_(assign_types_checkBox);
				amber.options[AmberFF::Option::OVERWRITE_CHARGES] = getValue_(overwrite_charges_checkBox);
				amber.options[AmberFF::Option::OVERWRITE_TYPENAMES] = getValue_(overwrite_typenames_checkBox);

				bool value = distance_button->isChecked();
				amber.options[AmberFF::Option::DISTANCE_DEPENDENT_DIELECTRIC] = value ? "true" : "false";

				amber.options[AmberFF::Option::NONBONDED_CUTOFF] = getValue_(nonbonded_cutoff_line_edit);
				amber.options[AmberFF::Option::VDW_CUTOFF] = getValue_(vdw_cutoff_line_edit);
				amber.options[AmberFF::Option::VDW_CUTON] = getValue_(vdw_cuton_line_edit);
				amber.options[AmberFF::Option::ELECTROSTATIC_CUTOFF] = getValue_(electrostatic_cutoff_line_edit);
				amber.options[AmberFF::Option::ELECTROSTATIC_CUTON] = getValue_(electrostatic_cuton_line_edit);
				amber.options[AmberFF::Option::SCALING_ELECTROSTATIC_1_4] = getValue_(scaling_electrostatic_1_4_line_edit);
				amber.options[AmberFF::Option::SCALING_VDW_1_4] = getValue_(scaling_vdw_1_4_line_edit);

				amber.options[AmberFF::Option::FILENAME] = String(parameter_file_edit->text().ascii());
				
				bool error = false;
				if (String(max_unassigned_atoms->text().ascii()).toUnsignedInt() == 0) 
				{
					error = true;
				}

				try
				{
					amber.setMaximumNumberOfErrors(String(max_unassigned_atoms->text().ascii()).toUnsignedInt());
				}
				catch(...)
				{
					error = true;
				}

				if (error)
				{
					max_unassigned_atoms->setText("10");
					amber.setMaximumNumberOfErrors(10);
					Log.error() << "Invalid value for max number of unassigned atoms, using default value of 10" << std::endl;
				}

				/*
				if (boundary_box->isChecked())
				{
					amber.options[PeriodicBoundary::Option::PERIODIC_BOX_ENABLED] = "true";
					try
					{
						amber.options[PeriodicBoundary::Option::PERIODIC_BOX_DISTANCE] = 
							String(box_size_edit->text().ascii()).toFloat();
					}
					catch(...)
					{
						Log.error() << "Invalid distance for Periodic Boundary choosen." << std::endl;
					}
					if (add_solvent_box->isChecked())
					{
						amber.options[PeriodicBoundary::Option::PERIODIC_BOX_ADD_SOLVENT] = "true";

						if (solvent_file_edit->text() != "")
						{
							amber.options[PeriodicBoundary::Option::PERIODIC_BOX_SOLVENT_FILE] = 
								solvent_file_edit->text().ascii();
						}
					}
				}
				else
				{
					amber.options[PeriodicBoundary::Option::PERIODIC_BOX_ENABLED] = "false";
					amber.options[PeriodicBoundary::Option::PERIODIC_BOX_ADD_SOLVENT] = "false";
				}
	*/
			}
			catch(...)
			{
			}
		}

		void AmberConfigurationDialog::setAmberFF(AmberFF& amber)
			throw()
		{
			amber_ = &amber;
		}
	
		void AmberConfigurationDialog::chooseSolventFile()
			throw()
		{
			/*
			QString result = QFileDialog::getOpenFileName(
					solvent_file_edit->text().ascii(), "", 0, "Select a solvent file");
			if (!result.isEmpty())
			{
				solvent_file_edit->setText(result);
			}
			*/
		}

		void AmberConfigurationDialog::periodicBoundaryClicked()
			throw()
		{
			/*
			if (boundary_box->isChecked())
			{
				add_solvent_box->setEnabled(true);
				solvent_file_edit->setEnabled(true);
				box_size_edit->setEnabled(true);
				browse_solvent_file->setEnabled(true);
			}
			else
			{
				add_solvent_box->setEnabled(false);
				solvent_file_edit->setEnabled(false);
				box_size_edit->setEnabled(false);
				add_solvent_box->setChecked(false);
				browse_solvent_file->setEnabled(false);
			}
			*/
		}
	
		void AmberConfigurationDialog::resetOptions()
		{
			PreferencesEntry::restoreDefaultValues();
		}

	}//namespace VIEW
}//namespace BALL
