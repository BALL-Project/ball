// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: minimizationDialog.C,v 1.2 2004/02/18 17:37:01 amoll Exp $
//

#include <BALL/VIEW/DIALOGS/minimizationDialog.h>
#include <BALL/VIEW/DIALOGS/amberConfigurationDialog.h>
#include <BALL/VIEW/DIALOGS/charmmConfigurationDialog.h>
#include <qfiledialog.h>
#include <qlineedit.h>
#include <qradiobutton.h>

namespace BALL
{
	namespace VIEW
	{

		MinimizationDialog::MinimizationDialog(QWidget* parent, const char* name)
			:	MinimizationDialogData( parent, name ),
				amber_dialog_(0),
				charmm_dialog_(0)
		{
		}

		MinimizationDialog::~MinimizationDialog()
		{
		}

		void MinimizationDialog::writePreferences(INIFile& inifile) const
		{
			// the minimizer options
			if (!inifile.hasSection("MINIMIZATION")) inifile.appendSection("MINIMIZATION");
			inifile.insertValue("MINIMIZATION", "Iterations::Max", getMaxIterations());
			inifile.insertValue("MINIMIZATION", "Iterations::Refresh", getRefresh());
			inifile.insertValue("MINIMIZATION", "Convergence::MaxGrad", getMaxGradient());
			inifile.insertValue("MINIMIZATION", "Convergence::EnergyDifference", getEnergyDifference());
			inifile.insertValue("MINIMIZATION", "UseCGMinimizer", getUseConjugateGradient());
		}

		void MinimizationDialog::readPreferences(const INIFile& inifile)
		{
			// the minimizer options
			if (inifile.hasEntry("MINIMIZATION", "Iterations::Max"))
			{
				setMaxIterations((Size)inifile.getValue("MINIMIZATION", "Iterations::Max").toUnsignedInt());
			}
			if (inifile.hasEntry("MINIMIZATION", "Iterations::Refresh"))
			{
				 setRefresh((Size)inifile.getValue("MINIMIZATION", "Iterations::Refresh").toUnsignedInt());
			}
			if (inifile.hasEntry("MINIMIZATION", "Convergence::MaxGrad"))
			{
				 setMaxGradient((float)inifile.getValue("MINIMIZATION", "Convergence::MaxGrad").toFloat());
			}
			if (inifile.hasEntry("MINIMIZATION", "Convergence::EnergyDifference"))
			{
				 setEnergyDifference((float)inifile.getValue("MINIMIZATION", "Convergence::EnergyDifference").toFloat());
			}
			if (inifile.hasEntry("MINIMIZATION", "UseCGMinimizer"))
			{
				setUseConjugateGradient(inifile.getValue("MINIMIZATION", "UseCGMinimizer").toUnsignedInt() == 1);
			}
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

		void MinimizationDialog::setForceField(bool amber)
		{	
			if (amber)
			{
				useAmberRadioButton->setChecked(true);
				useCharmmRadioButton->setChecked(false);
			}
			else
			{
				useCharmmRadioButton->setChecked(true);
				useAmberRadioButton->setChecked(false);
			}
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
