// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: charmmConfigurationDialog.C,v 1.9.4.1 2005/09/29 14:01:23 amoll Exp $
//

#include <BALL/VIEW/DIALOGS/charmmConfigurationDialog.h>
#include <BALL/MOLMEC/CHARMM/charmm.h>
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

		CharmmConfigurationDialog::CharmmConfigurationDialog(QWidget* parent, const char* name)
			:	CharmmConfigurationDialogData(parent, name),
				charmm_(0)
		{
			setINIFileSectionName("CHARMM");

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
			registerObject_(use_eef1_checkBox);
			
			registerObject_(parameter_file_edit);
			registerObject_(max_unassigned_atoms);
		}

		CharmmConfigurationDialog::~CharmmConfigurationDialog()
		{
		}

		void CharmmConfigurationDialog::browseParameterFiles()
		{
			// look up the full path of the parameter file
			Path p;
			String filename = p.find(getFilename());
			if (filename == "")
			{
				filename = getFilename();
			}
			QString tmp = filename.c_str();
			QString result = QFileDialog::getOpenFileName(tmp, "*.ini", 0, "Select a Charmm parameter file");
			if (!result.isEmpty())
			{
				// store the new filename in the lineedit field
				parameter_file_edit->setText(result.ascii());
			}
		}

		const String& CharmmConfigurationDialog::getFilename() const
		{
			static String filename;
			filename = parameter_file_edit->text().ascii();
			return filename;
		}
		

		void CharmmConfigurationDialog::reject()
		{
			hide();
			PreferencesEntry::restoreValues();
		}

		void CharmmConfigurationDialog::accept()
		{
			if (charmm_ != 0) applyTo(*charmm_);
			hide();
			PreferencesEntry::storeValues();
		}


		void CharmmConfigurationDialog::applyTo(CharmmFF& charmm)
			throw()
		{
			try
			{
				charmm.options[CharmmFF::Option::ASSIGN_TYPES] = getValue_(assign_types_checkBox);
				charmm.options[CharmmFF::Option::ASSIGN_CHARGES] = getValue_(assign_charges_checkBox);
				charmm.options[CharmmFF::Option::ASSIGN_TYPENAMES] = getValue_(assign_typenames_checkBox);
				charmm.options[CharmmFF::Option::OVERWRITE_CHARGES] = getValue_(overwrite_charges_checkBox);
				charmm.options[CharmmFF::Option::OVERWRITE_TYPENAMES] = getValue_(overwrite_typenames_checkBox);

				bool value = distance_button->isChecked();
				charmm.options[CharmmFF::Option::DISTANCE_DEPENDENT_DIELECTRIC] = value ? "true" : "false";

				charmm.options[CharmmFF::Option::USE_EEF1] = getValue_(use_eef1_checkBox);
				charmm.options[CharmmFF::Option::NONBONDED_CUTOFF] = getValue_(nonbonded_cutoff_line_edit);
				charmm.options[CharmmFF::Option::VDW_CUTOFF] = getValue_(vdw_cutoff_line_edit);
				charmm.options[CharmmFF::Option::VDW_CUTON] = getValue_(vdw_cuton_line_edit);
				charmm.options[CharmmFF::Option::ELECTROSTATIC_CUTOFF] = getValue_(electrostatic_cutoff_line_edit);
				charmm.options[CharmmFF::Option::ELECTROSTATIC_CUTON] = getValue_(electrostatic_cuton_line_edit);
				charmm.options[CharmmFF::Option::SOLVATION_CUTOFF] = getValue_(solvation_cutoff_line_edit);
				charmm.options[CharmmFF::Option::SOLVATION_CUTON] = getValue_(solvation_cuton_line_edit);
				charmm.options[CharmmFF::Option::SCALING_ELECTROSTATIC_1_4] = getValue_(scaling_electrostatic_1_4_line_edit);
				charmm.options[CharmmFF::Option::SCALING_VDW_1_4] = getValue_(scaling_vdw_1_4_line_edit);

				charmm.options[CharmmFF::Option::FILENAME] = getFilename();

				bool error = false;
				try
				{
					if (String(max_unassigned_atoms->text().ascii()).toUnsignedInt() == 0) error = true;
					charmm.setMaximumNumberOfErrors(String(max_unassigned_atoms->text().ascii()).toUnsignedInt());
				}
				catch(...)
				{
					error = true;
				}

				if (error)
				{
					max_unassigned_atoms->setText("10");
					charmm.setMaximumNumberOfErrors(10);
					Log.error() << "Invalid value for max number of unassigned atoms, using default value of 10" << std::endl;
				}

				/*
				if (boundary_box->isChecked())
				{
					charmm.options[PeriodicBoundary::Option::PERIODIC_BOX_ENABLED] = "true";
					try
					{
						charmm.options[PeriodicBoundary::Option::PERIODIC_BOX_DISTANCE] = 
							String(box_size_edit->text().ascii()).toFloat();
					}
					catch(...)
					{
						Log.error() << "Invalid distance for Periodic Boundary choosen." << std::endl;
					}
					if (add_solvent_box->isChecked())
					{
						charmm.options[PeriodicBoundary::Option::PERIODIC_BOX_ADD_SOLVENT] = "true";

						if (solvent_file_edit->text() != "")
						{
							charmm.options[PeriodicBoundary::Option::PERIODIC_BOX_SOLVENT_FILE] = 
								solvent_file_edit->text().ascii();
						}
					}
				}
				else
				{
					charmm.options[PeriodicBoundary::Option::PERIODIC_BOX_ENABLED] = "false";
					charmm.options[PeriodicBoundary::Option::PERIODIC_BOX_ADD_SOLVENT] = "false";
				}
			*/
			}
			catch(...)
			{
			}
		}

		void CharmmConfigurationDialog::setCharmmFF(CharmmFF& charmm)
			throw()
		{
			charmm_ = &charmm;
		}

		void CharmmConfigurationDialog::chooseSolventFile()
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
	
		void CharmmConfigurationDialog::periodicBoundaryClicked()
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
	
		String CharmmConfigurationDialog::getValue_(const QCheckBox* box) const
		{
			if (box->isChecked()) return true;
			else 									return false;
		}

		float CharmmConfigurationDialog::getValue_(const QLineEdit* edit) const
			throw(Exception::InvalidFormat)
		{
			return String(edit->text().ascii()).toFloat();
		}

		void CharmmConfigurationDialog::resetOptions()
		{
			PreferencesEntry::restoreDefaultValues();
		}

	}//namespace VIEW
}//namespace BALL
