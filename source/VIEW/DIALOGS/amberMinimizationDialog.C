// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: amberMinimizationDialog.C,v 1.9 2004/02/18 11:45:11 bender Exp $
//

#include <BALL/VIEW/DIALOGS/amberMinimizationDialog.h>
#include <BALL/VIEW/DIALOGS/amberConfigurationDialog.h>
#include <BALL/VIEW/DIALOGS/charmmConfigurationDialog.h>
#include <qfiledialog.h>
#include <qlineedit.h>
#include <qradiobutton.h>

namespace BALL
{
	namespace VIEW
	{

		AmberMinimizationDialog::AmberMinimizationDialog(QWidget* parent, const char* name)
			:	AmberMinimizationDialogData( parent, name ),
				amber_dialog_(0),
				charmm_dialog_(0)
		{
		}

		AmberMinimizationDialog::~AmberMinimizationDialog()
		{
		}

		void AmberMinimizationDialog::writePreferences(INIFile& inifile) const
		{
			// the minimizer options
			if (!inifile.hasSection("MINIMIZATION")) inifile.appendSection("MINIMIZATION");
			inifile.insertValue("MINIMIZATION", "Iterations::Max", getMaxIterations());
			inifile.insertValue("MINIMIZATION", "Iterations::Refresh", getRefresh());
			inifile.insertValue("MINIMIZATION", "Convergence::MaxGrad", getMaxGradient());
			inifile.insertValue("MINIMIZATION", "Convergence::EnergyDifference", getEnergyDifference());
			inifile.insertValue("MINIMIZATION", "UseCGMinimizer", getUseConjugateGradient());
		}

		void AmberMinimizationDialog::readPreferences(const INIFile& inifile)
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

		Size AmberMinimizationDialog::getMaxIterations() const	
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

		void AmberMinimizationDialog::setMaxIterations(Size n)
		{
			max_iterations_lineedit->setText(QString(String(n).c_str()));
		}

		Size AmberMinimizationDialog::getRefresh() const
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

		void AmberMinimizationDialog::setRefresh(Size n)
		{
			refresh_iterations_lineedit->setText(QString(String(n).c_str()));
		}

		double AmberMinimizationDialog::getMaxGradient() const
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

		void AmberMinimizationDialog::setMaxGradient(double max_gradient)
		{
			max_grad_lineedit->setText(QString(String(max_gradient).c_str()));
		}

		double AmberMinimizationDialog::getEnergyDifference() const
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

		void AmberMinimizationDialog::setEnergyDifference(double energy_difference)
		{
			energy_difference_lineedit->setText(QString(String(energy_difference).c_str()));
		}

		bool AmberMinimizationDialog::getUseConjugateGradient() const
		{
			return conjugate_button->isChecked();
		}

		void AmberMinimizationDialog::setUseConjugateGradient(bool use_CG)
		{
			conjugate_button->setChecked(use_CG);
		}

		void AmberMinimizationDialog::advancedOptions()
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

		void AmberMinimizationDialog::setAmberDialog(AmberConfigurationDialog* dialog)
		{
			amber_dialog_ = dialog;
		}

		void AmberMinimizationDialog::setCharmmDialog(CharmmConfigurationDialog* dialog)
		{
			charmm_dialog_ = dialog;
		}

		void AmberMinimizationDialog::setForceField(bool sel)
		{	
			if(sel == true)
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

		void AmberMinimizationDialog::useAmberFF()
		{
			useAmberRadioButton->setChecked(true);
			useCharmmRadioButton->setChecked(false);
		}

		void AmberMinimizationDialog::useCharmmFF()
		{
			useCharmmRadioButton->setChecked(true);
			useAmberRadioButton->setChecked(false);
		}

		bool AmberMinimizationDialog::getUseAmber()
		{
			return useAmberRadioButton->isChecked();
		}

	} // namespace VIEW
} //namespace BALL
