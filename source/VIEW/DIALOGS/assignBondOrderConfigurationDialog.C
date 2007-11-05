// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/DIALOGS/assignBondOrderConfigurationDialog.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/SYSTEM/path.h>

#include <BALL/STRUCTURE/assignBondOrderProcessor.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qradiobutton.h>
#include <QtGui/qcheckbox.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/QFileDialog>

namespace BALL
{
	namespace VIEW
	{

		AssignBondOrderConfigurationDialog::AssignBondOrderConfigurationDialog(QWidget* parent, const char* name)
			:	QDialog(parent),
				Ui_AssignBondOrderConfigurationDialogData(),
				PreferencesEntry()
		{
			setupUi(this);

			setINIFileSectionName("ILP"); // TODO naming
			setObjectName(name);
			registerWidgets_();

			// signals and slots connections
			connect( close_button, SIGNAL( clicked() ), this, SLOT( accept() ) );
			connect( cancel_button, SIGNAL( clicked() ), this, SLOT( reject() ) );
			connect( reset_button, SIGNAL( clicked() ), this, SLOT( resetOptions() ) );
			connect( browse_button, SIGNAL( clicked() ), this, SLOT( browseParameterFiles() ) );
		}

		AssignBondOrderConfigurationDialog::~AssignBondOrderConfigurationDialog()
		{
		}

		void AssignBondOrderConfigurationDialog::browseParameterFiles()
		{
			// look up the full path of the parameter file
			Path p;
			String filename = p.find(ascii(parameter_file_edit->text()));

			if (filename == "")
			{
				filename = ascii(parameter_file_edit->text());
			}
			QString tmp = filename.c_str();
			QString result = QFileDialog::getExistingDirectory(0, "Select a folder with the ILP parameter files", tmp); // TODO nameing
			if (!result.isEmpty())
			{
				// store the new filename in the lineedit field
				parameter_file_edit->setText(result);
			}
		}

		void AssignBondOrderConfigurationDialog::reject()
		{
			hide();
			PreferencesEntry::restoreValues();
		}

		void AssignBondOrderConfigurationDialog::accept()
		{
			hide();
			PreferencesEntry::storeValues();
		}


		String AssignBondOrderConfigurationDialog::getValue_(const QCheckBox* box) const
		{
			if (box->isChecked()) return "true";
			else 									return "false";
		}

		float AssignBondOrderConfigurationDialog::getValue_(const QLineEdit* edit) const
			throw(Exception::InvalidFormat)
		{
			return ascii(edit->text()).toFloat();
		}

		/*
	//	void AssignBondOrderConfigurationDialog::applyTo(AssignBondOrderProcessor& assign_bond_proc)
		void readOptions(AssignBondOrderProcessor& assign_bond_proc)
			throw()
		{
			try
			{
				assign_bond_proc.options[AssignBondOrderProcessor::Option::OVERWRITE_UNKNOWN_BOND_ORDERS] = getValue_(overwrite_unknownBO_box);
				assign_bond_proc.options[AssignBondOrderProcessor::Option::OVERWRITE_SINGLE_BOND_ORDERS] = getValue_(overwrite_singleBO_box);
				assign_bond_proc.options[AssignBondOrderProcessor::Option::OVERWRITE_DOUBLE_BOND_ORDERS] = getValue_(overwrite_doubleBO_box);
				assign_bond_proc.options[AssignBondOrderProcessor::Option::OVERWRITE_TRIPLE_BOND_ORDERS] = getValue_(overwrite_tripleBO_box);
				assign_bond_proc.options[AssignBondOrderProcessor::Option::OVERWRITE_QUADRUPLE_BOND_ORDERS] = getValue_(overwrite_quadrupleBO_box);
				assign_bond_proc.options[AssignBondOrderProcessor::Option::OVERWRITE_AROMATIC_BOND_ORDERS] = getValue_(overwrite_aromaticBO_box);
				assign_bond_proc.options[AssignBondOrderProcessor::Option::ASSIGN_CHARGES] = getValue_(assign_charges_checkBox);
				assign_bond_proc.options[AssignBondOrderProcessor::Option::OVERWRITE_CHARGES] = getValue_(overwrite_charges_checkBox);

				// get the parameter folder
				assign_bond_proc.options[MMFF94::Option::FOLDER] = ascii(parameter_file_edit->text());

			}
			catch(...)
			{
				Log.error() << "Invalid values for ILP" << std::endl; // TODO: nameing
			}
		}*/
/*
		void AssignBondOrderConfigurationDialog::setProc(AssignBondOrderProcessor& assign_bond_proc)
			throw()
		{
			//mmff_ = &mmff;
		}*/

	
		void AssignBondOrderConfigurationDialog::resetOptions()
		{
			PreferencesEntry::restoreDefaultValues();
		}

	}//namespace VIEW
}//namespace BALL
