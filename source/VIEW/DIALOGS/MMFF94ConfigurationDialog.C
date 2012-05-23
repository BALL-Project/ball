// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/DIALOGS/MMFF94ConfigurationDialog.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/MOLMEC/MMFF94/MMFF94.h>
#include <BALL/MOLMEC/MMFF94/MMFF94NonBonded.h>
#include <BALL/MOLMEC/MMFF94/MMFF94StretchBend.h>
#include <BALL/MOLMEC/MMFF94/MMFF94Torsion.h>
#include <BALL/MOLMEC/MMFF94/MMFF94OutOfPlaneBend.h>
#include <BALL/MOLMEC/MMFF94/MMFF94Torsion.h>
#include <BALL/SYSTEM/path.h>

#include <QtWidgets/QLineEdit>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QFileDialog>

namespace BALL
{
	namespace VIEW
	{

		MMFF94ConfigurationDialog::MMFF94ConfigurationDialog(QWidget* parent, const char* name)
			:	QDialog(parent),
				Ui_MMFF94ConfigurationDialogData(),
				PreferencesEntry(),
				mmff_(0)
		{
			setupUi(this);

			setINIFileSectionName("MMFF94");
			setObjectName(name);
			registerWidgets_();

			// signals and slots connections
			connect( close_button, SIGNAL( clicked() ), this, SLOT( accept() ) );
			connect( cancel_button, SIGNAL( clicked() ), this, SLOT( reject() ) );
			connect( reset_button, SIGNAL( clicked() ), this, SLOT( resetOptions() ) );
			connect( browse_button, SIGNAL( clicked() ), this, SLOT( browseParameterFiles() ) );
		}

		MMFF94ConfigurationDialog::~MMFF94ConfigurationDialog()
		{
		}

		void MMFF94ConfigurationDialog::browseParameterFiles()
		{
			// look up the full path of the parameter file
			Path p;
			String filename = p.find(ascii(parameter_file_edit->text()));

			if (filename == "")
			{
				filename = ascii(parameter_file_edit->text());
			}
			QString tmp = filename.c_str();
			QString result = QFileDialog::getExistingDirectory(0, "Select a folder with the MMFF94 parameter files", tmp);
			if (!result.isEmpty())
			{
				// store the new filename in the lineedit field
				parameter_file_edit->setText(result);
			}
		}

		void MMFF94ConfigurationDialog::reject()
		{
			hide();
			PreferencesEntry::restoreValues();
		}

		void MMFF94ConfigurationDialog::accept()
		{
			if (mmff_ != 0) 
			{
				applyTo(*mmff_);
			}
			hide();
			PreferencesEntry::storeValues();
		}


		String MMFF94ConfigurationDialog::getValue_(const QCheckBox* box) const
		{
			if (box->isChecked()) return "true";
			else 									return "false";
		}

		float MMFF94ConfigurationDialog::getValue_(const QLineEdit* edit) const
			throw(Exception::InvalidFormat)
		{
			return ascii(edit->text()).toFloat();
		}

		void MMFF94ConfigurationDialog::applyTo(MMFF94& mmff)
		{
			mmff.options[MMFF94_STRETCHES_ENABLED] = getValue_(stretch_box);
			mmff.options[MMFF94_BENDS_ENABLED] = getValue_(bends_box);
			mmff.options[MMFF94_STRETCHBENDS_ENABLED] = getValue_(stretch_bends_box);
			mmff.options[MMFF94_TORSIONS_ENABLED] = getValue_(torsions_box);
			mmff.options[MMFF94_OUTOFPLANE_ENABLED] = getValue_(plane_box);
			mmff.options[MMFF94_VDW_ENABLED] = getValue_(vdw_box);
			mmff.options[MMFF94_ES_ENABLED] = getValue_(es_box);

			try
			{
				mmff.options[MMFF94::Option::ASSIGN_TYPES] = getValue_(assign_types_checkBox);
				mmff.options[MMFF94::Option::ASSIGN_CHARGES] = getValue_(assign_charges_checkBox);
				mmff.options[MMFF94::Option::ASSIGN_TYPENAMES] = getValue_(assign_types_checkBox);
				mmff.options[MMFF94::Option::OVERWRITE_CHARGES] = getValue_(overwrite_charges_checkBox);
				mmff.options[MMFF94::Option::OVERWRITE_TYPENAMES] = getValue_(overwrite_typenames_checkBox);

 				bool value = distance_button->isChecked();
 				mmff.options[MMFF94::Option::DISTANCE_DEPENDENT_DIELECTRIC] = value ? "true" : "false";

				mmff.options[MMFF94::Option::FOLDER] = ascii(parameter_file_edit->text());

				// cut offs and cut ons...
				mmff.options[MMFF94::Option::VDW_CUTOFF] = getValue_(vdw_cutoff);
				mmff.options[MMFF94::Option::VDW_CUTON] = getValue_(vdw_cuton);
				mmff.options[MMFF94::Option::ELECTROSTATIC_CUTOFF] = 
					getValue_(electrostatic_cutoff);
				mmff.options[MMFF94::Option::ELECTROSTATIC_CUTON] = 
					getValue_(electrostatic_cuton);
				
				bool error = false;
				if (ascii(max_unassigned_atoms->text()).toUnsignedInt() == 0) 
				{
					error = true;
				}

				try
				{
					mmff.setMaximumNumberOfErrors(ascii(max_unassigned_atoms->text()).toUnsignedInt());
				}
				catch(...)
				{
					error = true;
				}

				if (error)
				{
					max_unassigned_atoms->setText("10");
					mmff.setMaximumNumberOfErrors(10);
					Log.error() << "Invalid value for max number of unassigned atoms, using default value of 10" << std::endl;
				}
			}
			catch(...)
			{
				Log.error() << "Invalid values for MMFF94" << std::endl;
			}
		}

		void MMFF94ConfigurationDialog::setMMFF94(MMFF94& mmff)
		{
			mmff_ = &mmff;
		}
	
		void MMFF94ConfigurationDialog::resetOptions()
		{
			PreferencesEntry::restoreDefaultValues();
		}

	}//namespace VIEW
}//namespace BALL
