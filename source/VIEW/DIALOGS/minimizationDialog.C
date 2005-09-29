// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: minimizationDialog.C,v 1.3.8.1 2005/09/29 14:01:24 amoll Exp $
//

#include <BALL/VIEW/DIALOGS/minimizationDialog.h>
#include <BALL/VIEW/DIALOGS/amberConfigurationDialog.h>
#include <BALL/VIEW/DIALOGS/charmmConfigurationDialog.h>
#include <BALL/SYSTEM/path.h>

#include <qfiledialog.h>
#include <qlineedit.h>
#include <qradiobutton.h>
#include <qbuttongroup.h>

namespace BALL
{
	namespace VIEW
	{

		MinimizationDialog::MinimizationDialog(QWidget* parent, const char* name)
			:	MinimizationDialogData( parent, name ),
				amber_dialog_(0),
				charmm_dialog_(0)
		{
			setINIFileSectionName("MINIMIZATION");

			registerObject_(max_iterations_lineedit);
			registerObject_(energy_difference_lineedit);
			registerObject_(max_grad_lineedit);
			registerObject_(refresh_iterations_lineedit);
			registerObject_(minimization_group);
		}

		MinimizationDialog::~MinimizationDialog()
		{
		}

		Size MinimizationDialog::getMaxIterations() const	
		{
			try
			{
				return (Size)String(max_iterations_lineedit->text().ascii()).toUnsignedInt();
			}
			catch(...)
			{
				Log.error() << "Invalid max iterations!" << std::endl;
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
				return (Size)String(refresh_iterations_lineedit->text().ascii()).toUnsignedInt();
			}
			catch(...)
			{
				Log.error() << "Invalid refresh iterations!" << std::endl;
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
				return (double)String(max_grad_lineedit->text().ascii()).toFloat();
			}
			catch(...)
			{
				Log.error() << "Invalid max gradient!" << std::endl;
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
				return (double)String(energy_difference_lineedit->text().ascii()).toFloat();
			}
			catch(...)
			{
				Log.error() << "Invalid Energy Difference!" << std::endl;
				return 0;
			}
		}

		void MinimizationDialog::setEnergyDifference(double energy_difference)
		{
			energy_difference_lineedit->setText(QString(String(energy_difference).c_str()));
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
			if(useAmberRadioButton->isChecked())
			{
				if (amber_dialog_ != 0) amber_dialog_->exec();
			}
			else
			{
				if (charmm_dialog_ != 0) charmm_dialog_->exec();
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

		void MinimizationDialog::useAmberFF()
		{
			useAmberRadioButton->setChecked(true);
			useCharmmRadioButton->setChecked(false);
		}

		void MinimizationDialog::useCharmmFF()
		{
			useCharmmRadioButton->setChecked(true);
			useAmberRadioButton->setChecked(false);
		}

		bool MinimizationDialog::getUseAmber()
		{
			return useAmberRadioButton->isChecked();
		}

	} // namespace VIEW
} //namespace BALL
