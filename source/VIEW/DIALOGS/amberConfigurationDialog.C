// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/DIALOGS/amberConfigurationDialog.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/MOLMEC/AMBER/amber.h>
#include <BALL/MOLMEC/AMBER/amberBend.h>
#include <BALL/MOLMEC/AMBER/amberStretch.h>
#include <BALL/MOLMEC/AMBER/amberTorsion.h>
#include <BALL/MOLMEC/AMBER/amberNonBonded.h>
#include <BALL/SYSTEM/path.h>

#include <QtGui/QLineEdit>
#include <QtGui/QRadioButton>
#include <QtGui/QCheckBox>
#include <QtGui/QPushButton>
#include <QtGui/QFileDialog>

namespace BALL
{
	namespace VIEW
	{

		AmberConfigurationDialog::AmberConfigurationDialog(QWidget* parent, const char* name)
			:	QDialog(parent),
				Ui_AmberConfigurationDialogData(),
				PreferencesEntry(),
				amber_(0)
		{
			setupUi(this);

			// signals and slots connections
			connect( close_button, SIGNAL( clicked() ), this, SLOT( accept() ) );
			connect( cancel_button, SIGNAL( clicked() ), this, SLOT( reject() ) );
			connect( reset_button, SIGNAL( clicked() ), this, SLOT( resetOptions() ) );
			connect( browse_button, SIGNAL( clicked() ), this, SLOT( browseParameterFiles() ) );

			setINIFileSectionName("AMBER");
			setObjectName(name);
			registerWidgets_();
		}

		AmberConfigurationDialog::~AmberConfigurationDialog()
		{
		}

		void AmberConfigurationDialog::browseParameterFiles()
		{
			// look up the full path of the parameter file
			Path p;
			String filename = p.find(ascii(parameter_file_edit->text()));

			if (filename == "")
			{
				filename = ascii(parameter_file_edit->text());
			}
			QString tmp = filename.c_str();
			QString result = QFileDialog::getOpenFileName(0, "Select an AMBER parameter file", tmp, "*.ini", 0);
			if (!result.isEmpty())
			{
				// store the new filename in the lineedit field

				parameter_file_edit->setText(result);
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
			if (box->isChecked()) return "true";
			else 									return "false";
		}

		float AmberConfigurationDialog::getValue_(const QLineEdit* edit) const
			throw(Exception::InvalidFormat)
		{
			return ascii(edit->text()).toFloat();
		}

		void AmberConfigurationDialog::applyTo(AmberFF& amber)
		{
			try
			{
				amber.options[AmberFF::Option::ASSIGN_TYPES] = getValue_(assign_types_checkBox);
				amber.options[AmberFF::Option::ASSIGN_CHARGES] = getValue_(assign_charges_checkBox);
				amber.options[AmberFF::Option::ASSIGN_TYPENAMES] = getValue_(assign_typenames_checkBox);
				amber.options[AmberFF::Option::OVERWRITE_CHARGES] = getValue_(overwrite_charges_checkBox);
				amber.options[AmberFF::Option::OVERWRITE_TYPENAMES] = getValue_(overwrite_typenames_checkBox);
				amber.options[AMBER_BEND_ENABLED] = getValue_(bends_box);
				amber.options[AMBER_STRETCH_ENABLED] = getValue_(stretches_box);
				amber.options[AMBER_TORSIONS_ENABLED] = getValue_(torsions_box);
				amber.options[AMBER_NB_ENABLED] = getValue_(NB_box);

				bool value = distance_button->isChecked();
				amber.options[AmberFF::Option::DISTANCE_DEPENDENT_DIELECTRIC] = value ? "true" : "false";

				amber.options[AmberFF::Option::NONBONDED_CUTOFF] = getValue_(nonbonded_cutoff_line_edit);
				amber.options[AmberFF::Option::VDW_CUTOFF] = getValue_(vdw_cutoff_line_edit);
				amber.options[AmberFF::Option::VDW_CUTON] = getValue_(vdw_cuton_line_edit);
				amber.options[AmberFF::Option::ELECTROSTATIC_CUTOFF] = getValue_(electrostatic_cutoff_line_edit);
				amber.options[AmberFF::Option::ELECTROSTATIC_CUTON] = getValue_(electrostatic_cuton_line_edit);
				amber.options[AmberFF::Option::SCALING_ELECTROSTATIC_1_4] = getValue_(scaling_electrostatic_1_4_line_edit);
				amber.options[AmberFF::Option::SCALING_VDW_1_4] = getValue_(scaling_vdw_1_4_line_edit);

				amber.options[AmberFF::Option::FILENAME] = ascii(parameter_file_edit->text());
				
				bool error = false;
				if (ascii(max_unassigned_atoms->text()).toUnsignedInt() == 0) 
				{
					error = true;
				}

				try
				{
					amber.setMaximumNumberOfErrors(ascii(max_unassigned_atoms->text()).toUnsignedInt());
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
		{
			amber_ = &amber;
		}
	
		void AmberConfigurationDialog::chooseSolventFile()
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
