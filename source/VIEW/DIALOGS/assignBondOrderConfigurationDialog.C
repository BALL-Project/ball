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
			QDialog::accept();
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

	
		void AssignBondOrderConfigurationDialog::resetOptions()
		{
			PreferencesEntry::restoreDefaultValues();
		}

	}//namespace VIEW
}//namespace BALL
