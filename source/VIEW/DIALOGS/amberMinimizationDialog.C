// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/DIALOGS/amberMinimizationDialog.h>
#include <BALL/VIEW/DIALOGS/advancedOptionsDialog.h>
#include <qfiledialog.h>
#include <qlineedit.h>
#include <qradiobutton.h>

namespace BALL
{
	namespace VIEW
	{

AmberMinimizationDialog::AmberMinimizationDialog(QWidget* parent, const char* name)
	:	AmberMinimizationDialogData( parent, name )
{
	use_dddc = false;
	ini = "Amber/amber96.ini";
}

AmberMinimizationDialog::~AmberMinimizationDialog()
{
}

void AmberMinimizationDialog::browseParameterFiles()
{
	// look up the full path of the parameter file
	Path p;
	String filename = p.find(getFilename());
	if (filename == "")
	{
		filename = getFilename();
	}
	QString tmp = filename.c_str();
	QString result = QFileDialog::getOpenFileName(tmp, "*.ini", 0, "Select an AMBER parameter file");
	if (!result.isEmpty())
	{
		// store the new filename in the lineedit field
		setFilename(String(result.ascii()));
	}
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
	inifile.insertValue("MINIMIZATION", "DistanceDependentDC", getUseDistanceDependentDC());

	// the AMBER options
	if (!inifile.hasSection("AMBER")) inifile.appendSection("AMBER");
	inifile.insertValue("AMBER", "Filename", getFilename());
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
	if (inifile.hasEntry("MINIMIZATION", "DistanceDependentDC"))
	{
		setUseDistanceDependentDC(inifile.getValue("MINIMIZATION", "DistanceDependentDC").toUnsignedInt() == 1);
	}

	// the AMBER options
	if (inifile.hasEntry("AMBER", "Filename"))
	{
		setFilename(inifile.getValue("AMBER", "Filename"));
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

String AmberMinimizationDialog::getFilename() const
{ 
	if (!parameter_file_edit->isEnabled()) return "";
	String filename;
	filename = parameter_file_edit->text().ascii();
	return filename;
}

void AmberMinimizationDialog::setFilename(const String& filename)
{
	ini = filename;
}

bool AmberMinimizationDialog::getUseDistanceDependentDC() const
{
	return use_dddc;
}

void AmberMinimizationDialog::setUseDistanceDependentDC(bool usedddc)
{
	use_dddc=usedddc;
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
	advancedOptionsDialog* dialog = new advancedOptionsDialog();
	if(dialog->exec() == QDialog::Accepted )
	{
		// set inifile to chosen file
		String filename = (dialog->parameter_file_edit)->text().ascii();
		ini = filename;
		// show chosen amber-ini-file in line edit
		parameter_file_edit->setText(filename);
		// set use_dddc
		use_dddc = dialog->getUseDistanceDependentDC();
	}
	delete dialog;
}

}} //namespaces
