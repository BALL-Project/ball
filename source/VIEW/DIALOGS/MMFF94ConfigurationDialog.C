// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94ConfigurationDialog.C,v 1.1.2.1 2006/06/14 13:09:20 amoll Exp $
//

#include <BALL/VIEW/DIALOGS/MMFF94ConfigurationDialog.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/MOLMEC/MMFF94/MMFF94.h>
#include <BALL/SYSTEM/path.h>

#include <QtGui/qlineedit.h>
#include <QtGui/qradiobutton.h>
#include <QtGui/qcheckbox.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/QFileDialog>

namespace BALL
{
	namespace VIEW
	{

		MMFF94ConfigurationDialog::MMFF94ConfigurationDialog(QWidget* parent, const char* name)
			:	QDialog(parent),
				Ui_MMFF94ConfigurationDialogData(),
				mmff_(0)
		{
			setupUi(this);

			// signals and slots connections
			connect( close_button, SIGNAL( clicked() ), this, SLOT( accept() ) );
			connect( cancel_button, SIGNAL( clicked() ), this, SLOT( reject() ) );
			connect( reset_button, SIGNAL( clicked() ), this, SLOT( resetOptions() ) );
			connect( browse_button, SIGNAL( clicked() ), this, SLOT( browseParameterFiles() ) );

			setINIFileSectionName("MMFF94");
			setObjectName(name);

			registerObject_(dielectric_group);
			registerObject_(assign_charges_checkBox);
			registerObject_(assign_typenames_checkBox);
			registerObject_(assign_types_checkBox);
			registerObject_(overwrite_charges_checkBox);
			registerObject_(overwrite_typenames_checkBox);

			registerObject_(parameter_file_edit);
			registerObject_(max_unassigned_atoms);
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
Log.error() << "#~~#   2 "             << " "  << __FILE__ << "  " << __LINE__<< std::endl;
			if (mmff_ != 0) 
			{
Log.error() << "#~~#   1 "             << " "  << __FILE__ << "  " << __LINE__<< std::endl;
				applyTo(*mmff_);
			}
			hide();
			PreferencesEntry::storeValues();
		}


		String MMFF94ConfigurationDialog::getValue_(const QCheckBox* box) const
		{
			if (box->isChecked()) return true;
			else 									return false;
		}

		float MMFF94ConfigurationDialog::getValue_(const QLineEdit* edit) const
			throw(Exception::InvalidFormat)
		{
			return ascii(edit->text()).toFloat();
		}

		void MMFF94ConfigurationDialog::applyTo(MMFF94& mmff)
			throw()
		{
			mmff.options[MMFF94::Option::STRETCHES_ENABLED] = getValue_(stretch_box);
			mmff.options[MMFF94::Option::BENDS_ENABLED] = getValue_(bends_box);
			mmff.options[MMFF94::Option::STRETCHBENDS_ENABLED] = getValue_(stretch_bends_box);
			mmff.options[MMFF94::Option::TORSIONS_ENABLED] = getValue_(torsions_box);
			mmff.options[MMFF94::Option::OUTOFPLANE_ENABLED] = getValue_(plane_box);
			mmff.options[MMFF94::Option::VDW_ENABLED] = getValue_(vdw_box);
			mmff.options[MMFF94::Option::ES_ENABLED] = getValue_(es_box);

			try
			{
				mmff.options[MMFF94::Option::ASSIGN_TYPES] = getValue_(assign_types_checkBox);
				mmff.options[MMFF94::Option::ASSIGN_CHARGES] = getValue_(assign_charges_checkBox);
				mmff.options[MMFF94::Option::ASSIGN_TYPENAMES] = getValue_(assign_types_checkBox);
				mmff.options[MMFF94::Option::OVERWRITE_CHARGES] = getValue_(overwrite_charges_checkBox);
				mmff.options[MMFF94::Option::OVERWRITE_TYPENAMES] = getValue_(overwrite_typenames_checkBox);

//   				bool value = distance_button->isChecked();
//   				MMFF94.options[MMFF94::Option::DISTANCE_DEPENDENT_DIELECTRIC] = value ? "true" : "false";

				mmff.options[MMFF94::Option::FOLDER] = ascii(parameter_file_edit->text());
				
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
			throw()
		{
			mmff_ = &mmff;
		}
	
		void MMFF94ConfigurationDialog::resetOptions()
		{
			PreferencesEntry::restoreDefaultValues();
		}

	}//namespace VIEW
}//namespace BALL
