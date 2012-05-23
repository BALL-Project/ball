// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/DIALOGS/minimizationDialog.h>
#include <BALL/VIEW/DIALOGS/amberConfigurationDialog.h>
#include <BALL/VIEW/DIALOGS/charmmConfigurationDialog.h>
#include <BALL/VIEW/DIALOGS/MMFF94ConfigurationDialog.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/SYSTEM/path.h>

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMessageBox>

namespace BALL
{
	namespace VIEW
	{

		MinimizationDialog::MinimizationDialog(QWidget* parent, const char* name)
			:	QDialog(parent),
				Ui_MinimizationDialogData(),
				PreferencesEntry(),
				amber_dialog_(0),
				charmm_dialog_(0),
				mmff_dialog_(0)
		{
			setupUi(this);
			setObjectName(name);
			setINIFileSectionName("MINIMIZATION");
			registerWidgets_();
			
			// signals and slots connections
			connect( start_button, SIGNAL( clicked() ), this, SLOT( accept() ) );
			connect( cancel_button, SIGNAL( clicked() ), this, SLOT( reject() ) );
			connect( advanced_button, SIGNAL( clicked() ), this, SLOT( advancedOptions() ) );
		}

		MinimizationDialog::~MinimizationDialog()
		{
		}

		Size MinimizationDialog::getMaxIterations() const	
		{
			try
			{
				return (Size)ascii(max_iterations_lineedit->text()).toUnsignedInt();
			}
			catch(...)
			{
				Log.error() << (String)tr("Invalid max iterations!") << std::endl;
				return 0;
			}
		}

		void MinimizationDialog::setMaxIterations(Size n)
		{
			max_iterations_lineedit->setText(QString(String(n).c_str()));
		}

		Size MinimizationDialog::getRefresh() const
		{
			try
			{
				return (Size)ascii(refresh_iterations_lineedit->text()).toUnsignedInt();
			}
			catch(...)
			{
				Log.error() << (String)tr("Invalid refresh iterations!") << std::endl;
				return 0;
			}
		}

		void MinimizationDialog::setRefresh(Size n)
		{
			refresh_iterations_lineedit->setText(QString(String(n).c_str()));
		}

		double MinimizationDialog::getMaxGradient() const
		{
			try
			{
				return (double)ascii(max_grad_lineedit->text()).toFloat();
			}
			catch(...)
			{
				Log.error() << (String)tr("Invalid max gradient!") << std::endl;
				return 0;
			}
		}

		void MinimizationDialog::setMaxGradient(double max_gradient)
		{
			max_grad_lineedit->setText(QString(String(max_gradient).c_str()));
		}

		double MinimizationDialog::getEnergyDifference() const
		{
			try
			{
				return (double)ascii(energy_difference_lineedit->text()).toFloat();
			}
			catch(...)
			{
				Log.error() << (String)tr("Invalid Energy Difference!") << std::endl;
				return 0;
			}
		}

		void MinimizationDialog::setEnergyDifference(double energy_difference)
		{
			energy_difference_lineedit->setText(QString(String(energy_difference).c_str()));
		}

		bool MinimizationDialog::getUseStrangLBFGS() const
		{
			return SLBFGS_button->isChecked();
		}

		void MinimizationDialog::setUseStrangLBFGS(bool use_LBFGS)
		{
			SLBFGS_button->setChecked(use_LBFGS);
		}

		bool MinimizationDialog::getUseShiftedLVMM() const
		{
			return SLVMM_button->isChecked();
		}

		void MinimizationDialog::setUseShiftedLVMM(bool use_shifted)
		{
			SLVMM_button->setChecked(use_shifted);
		}

		bool MinimizationDialog::getUseConjugateGradient() const
		{
			return conjugate_button->isChecked();
		}

		void MinimizationDialog::setUseConjugateGradient(bool use_CG)
		{
			conjugate_button->setChecked(use_CG);
		}

		void MinimizationDialog::advancedOptions()
		{
			if (useAmberRadioButton->isChecked())
			{
				if (amber_dialog_ != 0) amber_dialog_->exec();
			}
			else if (useCharmmRadioButton->isChecked())
			{
				if (charmm_dialog_ != 0) charmm_dialog_->exec();
			}
			else if (useMMFF94RadioButton->isChecked())
			{
				if (mmff_dialog_ != 0) mmff_dialog_->exec();
			}
		}

		void MinimizationDialog::setAmberDialog(AmberConfigurationDialog* dialog)
		{
			amber_dialog_ = dialog;
		}

		void MinimizationDialog::setCharmmDialog(CharmmConfigurationDialog* dialog)
		{
			charmm_dialog_ = dialog;
		}

		void MinimizationDialog::setMMFF94Dialog(MMFF94ConfigurationDialog* dialog)
		{
			mmff_dialog_ = dialog;
		}
		void MinimizationDialog::selectForceField(Position nr)
		{
			if 			(nr == 0) useAmberRadioButton->setChecked(true);
			else if (nr == 1) useCharmmRadioButton->setChecked(true);
			else if (nr == 2) useMMFF94RadioButton->setChecked(true);
			else
			{
				BALLVIEW_DEBUG
			}
		}
		
		Position MinimizationDialog::selectedForceField() const
		{
			if 			(useAmberRadioButton->isChecked())  return 0;
			else if (useCharmmRadioButton->isChecked()) return 1;
			else if (useMMFF94RadioButton->isChecked()) return 2;

			return 0;
		}
	
			
		void MinimizationDialog::accept()
		{
			QString error;
			if (getMaxGradient() == 0)
			{
				error = tr("maximum gradient.");
			}

			if(getEnergyDifference() == 0.0)
			{
				error = tr("energy difference.");
			}

			if (error == "")
			{
				QDialog::accept();
				return;
			}

			QMessageBox::critical(this, tr("Invalid values"), tr("Please apply correct settings") + " (> 0) " + tr("for the") + " " + error,
					QMessageBox::Ok| QMessageBox::Default);
		}

	} // namespace VIEW
} //namespace BALL
