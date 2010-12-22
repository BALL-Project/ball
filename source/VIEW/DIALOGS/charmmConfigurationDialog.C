// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/DIALOGS/charmmConfigurationDialog.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/MOLMEC/CHARMM/charmm.h>
#include <BALL/MOLMEC/CHARMM/charmmBend.h>
#include <BALL/MOLMEC/CHARMM/charmmStretch.h>
#include <BALL/MOLMEC/CHARMM/charmmTorsion.h>
#include <BALL/MOLMEC/CHARMM/charmmImproperTorsion.h>
#include <BALL/MOLMEC/CHARMM/charmmNonBonded.h>
#include <BALL/SYSTEM/path.h>

#include <QtGui/QFileDialog>
#include <QtGui/QLineEdit>
#include <QtGui/QRadioButton>
#include <QtGui/QCheckBox>
#include <QtGui/QPushButton>

namespace BALL
{
	namespace VIEW
	{

		CharmmConfigurationDialog::CharmmConfigurationDialog(QWidget* parent, const char* name)
			:	QDialog(parent),
				Ui_CharmmConfigurationDialogData(),
				PreferencesEntry(),
				charmm_(0)
		{
			setupUi(this);

			// signals and slots connections
			connect( browse_button, SIGNAL( clicked() ), this, SLOT( browseParameterFiles() ) );
			connect( cancel_button, SIGNAL( clicked() ), this, SLOT( reject() ) );
			connect( close_button, SIGNAL( clicked() ), this, SLOT( accept() ) );
			connect( reset_button, SIGNAL( clicked() ), this, SLOT( resetOptions() ) );

			setINIFileSectionName("CHARMM");
			setObjectName(name);
			registerWidgets_();
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
			QString result = QFileDialog::getOpenFileName(0, tr("Select a Charmm parameter file"), tmp, "*.ini", 0);
			if (!result.isEmpty())
			{
				// store the new filename in the lineedit field
				parameter_file_edit->setText(result);
			}
		}

		const String& CharmmConfigurationDialog::getFilename() const
		{
			static String filename;
			filename = ascii(parameter_file_edit->text());
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
		{
			try
			{
				charmm.options[CharmmFF::Option::ASSIGN_TYPES] = getValue_(assign_types_checkBox);
				charmm.options[CharmmFF::Option::ASSIGN_CHARGES] = getValue_(assign_charges_checkBox);
				charmm.options[CharmmFF::Option::ASSIGN_TYPENAMES] = getValue_(assign_typenames_checkBox);
				charmm.options[CharmmFF::Option::OVERWRITE_CHARGES] = getValue_(overwrite_charges_checkBox);
				charmm.options[CharmmFF::Option::OVERWRITE_TYPENAMES] = getValue_(overwrite_typenames_checkBox);

				charmm.options[CHARMM_BENDS_ENABLED] = getValue_(bends_box);
				charmm.options[CHARMM_STRETCHES_ENABLED] = getValue_(stretches_box);
				charmm.options[CHARMM_TORSIONS_ENABLED] = getValue_(torsions_box);
				charmm.options[CHARMM_IMPROPER_TORSIONS_ENABLED] = getValue_(itorsions_box);
				charmm.options[CHARMM_NB_ENABLED] = getValue_(NB_box);

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
					if (ascii(max_unassigned_atoms->text()).toUnsignedInt() == 0) error = true;
					charmm.setMaximumNumberOfErrors(ascii(max_unassigned_atoms->text()).toUnsignedInt());
				}
				catch(...)
				{
					error = true;
				}

				if (error)
				{
					max_unassigned_atoms->setText("10");
					charmm.setMaximumNumberOfErrors(10);
					Log.error() << (String)tr("Invalid value for max number of unassigned atoms, using default value of 10") << std::endl;
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
		{
			charmm_ = &charmm;
		}

		void CharmmConfigurationDialog::chooseSolventFile()
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
			if (box->isChecked()) return "true";
			else 									return "false";
		}

		float CharmmConfigurationDialog::getValue_(const QLineEdit* edit) const
			throw(Exception::InvalidFormat)
		{
			return ascii(edit->text()).toFloat();
		}

		void CharmmConfigurationDialog::resetOptions()
		{
			PreferencesEntry::restoreDefaultValues();
		}

	}//namespace VIEW
}//namespace BALL
