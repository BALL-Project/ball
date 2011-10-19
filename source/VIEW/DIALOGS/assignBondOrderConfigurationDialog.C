// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/DIALOGS/assignBondOrderConfigurationDialog.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/SYSTEM/path.h>

#include <BALL/STRUCTURE/assignBondOrderProcessor.h>
#include <QtGui/QLineEdit>
#include <QtGui/QRadioButton>
#include <QtGui/QCheckBox>
#include <QtGui/QPushButton>
#include <QtGui/QFileDialog>

namespace BALL
{
	namespace VIEW
	{

		AssignBondOrderConfigurationDialog::AssignBondOrderConfigurationDialog(QWidget* parent, const char* name)
			:	QDialog(parent),
				Ui_AssignBondOrderConfigurationDialogData(),
				ModularWidget(name),
				PreferencesEntry()
		{
			setupUi(this);

			setINIFileSectionName("BONDORDERASSIGNER");
			setObjectName(name);
			// for preferences	
			registerWidgets_();
			// for actions, icons...
			ModularWidget::registerWidget(this);
			hide();

			// signals and slots connections
			connect( browse_button, SIGNAL( clicked() ), this, SLOT( browseParameterFiles_() ) );
			connect( penalty_balance_slider, SIGNAL( valueChanged(int) ), this, SLOT( balanceParameterChanged_() ) );

			connect(overwrite_singleBO_box, SIGNAL(stateChanged(int)), this, SLOT(validateBOBoxes_()));
			connect(overwrite_doubleBO_box, SIGNAL(stateChanged(int)), this, SLOT(validateBOBoxes_()));
			connect(overwrite_tripleBO_box, SIGNAL(stateChanged(int)), this, SLOT(validateBOBoxes_()));
			connect(overwrite_selected_bonds_box, SIGNAL(stateChanged(int)), this, SLOT(validateBOBoxes_()));

			connect(ASTAR_button, SIGNAL(clicked()), this, SLOT(validateStrategies_()));
			connect(ILP_button,   SIGNAL(clicked()), this, SLOT(validateStrategies_()));
			connect(FPT_button,   SIGNAL(clicked()), this, SLOT(validateStrategies_()));
			connect(all_optimal_solutions_button, SIGNAL(clicked()), this, SLOT(validateStrategies_()));

			validateBOBoxes_();
			validateStrategies_();

#ifndef BALL_HAS_LPSOLVE
			ILP_button->setEnabled(false);
			ILP_button->setChecked(false);
			ASTAR_button->setChecked(true);
#endif
		}

		AssignBondOrderConfigurationDialog::~AssignBondOrderConfigurationDialog()
		{
		}

		void AssignBondOrderConfigurationDialog::balanceParameterChanged_()
		{
			atom_type_penalty_label->setText(String((int)(100 - penalty_balance_slider->value())).c_str());
			bond_length_penalty_label ->setText(String(penalty_balance_slider->value()).c_str());
		}

		void AssignBondOrderConfigurationDialog::validateBOBoxes_()
		{
			// if one of the bond orders is checked, the "selected" box must be
			// de-activated and vice versa
			bool    selected_checked = overwrite_selected_bonds_box->isChecked();

			if (selected_checked)
			{
				overwrite_singleBO_box->setChecked(!selected_checked);
				overwrite_doubleBO_box->setChecked(!selected_checked);
				overwrite_tripleBO_box->setChecked(!selected_checked);
			}
			overwrite_singleBO_box->setDisabled(selected_checked);
			overwrite_doubleBO_box->setDisabled(selected_checked);
			overwrite_tripleBO_box->setDisabled(selected_checked);
		}

		void AssignBondOrderConfigurationDialog::validateStrategies_()
		{
			// if the FPT strategy is selected we have to disable some functionality
			bool FPT_clicked = FPT_button->isChecked();
			bool ILP_clicked = ILP_button->isChecked();
			bool touched = FPT_clicked || ILP_clicked;

			if (touched)
			{
				add_hydrogens_checkBox->setChecked(!touched);
				penalty_balance_slider->setValue(0);

			}
			hydrogen_check_options->setDisabled(touched);
			bond_length_groupBox->setDisabled(touched);
			ASTAR_groupBox->setDisabled(touched);

			if (FPT_clicked)
			{
				all_optimal_solutions_button->setChecked(!FPT_clicked);
				n_opt_solutions_button->setChecked(FPT_clicked);

				overwrite_singleBO_box->setChecked(FPT_clicked);
				overwrite_doubleBO_box->setChecked(FPT_clicked);
				overwrite_tripleBO_box->setChecked(FPT_clicked);
				overwrite_selected_bonds_box->setChecked(!FPT_clicked);
			}

			all_optimal_solutions_button->setDisabled(FPT_clicked);
			restrictions_groupBox->setDisabled(FPT_clicked);
		}

		void AssignBondOrderConfigurationDialog::browseParameterFiles_()
		{
			// look up the full path of the parameter file
			Path p;
			String filename = p.find(ascii(parameter_file_edit->text()));

			if (filename == "")
			{
				filename = ascii(parameter_file_edit->text());
			}

			QString tmp = filename.c_str();
			QString result = QFileDialog::getOpenFileName(0, tr("Select the penalty parameter file"), tmp, "*.xml");
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
			if (box->isChecked())
				return "true";
			else
				return "false";
		}

		float AssignBondOrderConfigurationDialog::getValue_(const QLineEdit* edit) const
			throw(Exception::InvalidFormat)
		{
			return ascii(edit->text()).toFloat();
		}

		void  AssignBondOrderConfigurationDialog::initializeWidget(MainControl&)
		{
		}

		void AssignBondOrderConfigurationDialog::dialogButtonClicked_(QAbstractButton* button)
		{
			if(!button) {
				return;
			}

			switch(buttonBox->buttonRole(button)){
				case QDialogButtonBox::ResetRole:
					resetOptions();
					break;
				default:
					return;
			}
		}

		void AssignBondOrderConfigurationDialog::resetOptions()
		{
			PreferencesEntry::restoreDefaultValues();
		}

	}//namespace VIEW
}//namespace BALL
