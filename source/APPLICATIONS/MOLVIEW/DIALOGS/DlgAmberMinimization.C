/**********************************************************************

	--- Qt Architect generated file ---

	File: DlgAmberMinimization.C
	Last generated: Fri Jan 14 22:20:31 2000

 *********************************************************************/



#include "DlgAmberMinimization.h"
#include <qfiledialog.h>

#define Inherited DlgAmberMinimizationData

DlgAmberMinimization::DlgAmberMinimization
	(QWidget* parent, const char* name)
	:	Inherited( parent, name )
{
}

DlgAmberMinimization::~DlgAmberMinimization()
{
}

void DlgAmberMinimization::browseParameterFiles()
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

void DlgAmberMinimization::setPreferences(INIFile& inifile) const
{
	//	
	// the minimizer options
	//
	inifile.setValue("MINIMIZATION", "Iterations::Max", getMaxIterations());
	inifile.setValue("MINIMIZATION", "Iterations::Refresh", getRefresh());
	inifile.setValue("MINIMIZATION", "Convergence::MaxGrad", getMaxGradient());
	inifile.setValue("MINIMIZATION", "Convergence::EnergyDifference", getEnergyDifference());
	inifile.setValue("MINIMIZATION", "UseCGMinimizer", getUseConjugateGradient());

	//
	// the AMBER options
	//
	inifile.setValue("AMBER", "Filename", getFilename());
	inifile.setValue("AMBER", "DistanceDependentDC", getUseDistanceDependentDC());
}


void DlgAmberMinimization::getPreferences(const INIFile& inifile)
{
	// 
	// the minimizer options
	//
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
		 setEnergyDifference((float)inifile.getValue("MINIMIZATION", "Convergence::MaxGrad").toFloat());
	}
	if (inifile.hasEntry("MINIMIZATION", "UseCGMinimizer"))
	{
		setUseConjugateGradient(inifile.getValue("AMBER", "UseCGMinimizer") == 1);
	}

	//
	// the AMBER options
	//
	if (inifile.hasEntry("AMBER", "Filename"))
	{
		setFilename(inifile.getValue("AMBER", "Filename"));
	}
	if (inifile.hasEntry("AMBER", "DistanceDependentDC"))
	{
		setUseDistanceDependentDC(inifile.getValue("AMBER", "DistanceDependentDC") == 1);
	}
}


Size DlgAmberMinimization::getMaxIterations() const	
{
	return (Size)String(max_iterations_lineedit_->text().ascii()).toUnsignedInt();
}

void DlgAmberMinimization::setMaxIterations(Size n)
{
	max_iterations_lineedit_->setText(QString(String(n).c_str()));
}

Size DlgAmberMinimization::getRefresh() const
{
	return (Size)String(refresh_iterations_lineedit_->text().ascii()).toUnsignedInt();
}

void DlgAmberMinimization::setRefresh(Size n)
{
	refresh_iterations_lineedit_->setText(QString(String(n).c_str()));
}

double DlgAmberMinimization::getMaxGradient() const
{
	return (double)String(max_grad_lineedit_->text().ascii()).toFloat();
}

void DlgAmberMinimization::setMaxGradient(double max_gradient)
{
	max_grad_lineedit_->setText(QString(String(max_gradient).c_str()));
}

double DlgAmberMinimization::getEnergyDifference() const
{
	return (double)String(energy_difference_lineedit_->text().ascii()).toFloat();
}

void DlgAmberMinimization::setEnergyDifference(double energy_difference)
{
	energy_difference_lineedit_->setText(QString(String(energy_difference).c_str()));
}

const String& DlgAmberMinimization::getFilename() const
{
	static String filename;
	filename = parameter_file_edit_->text().ascii();
	return filename;
}

void DlgAmberMinimization::setFilename(const String& filename)
{
	parameter_file_edit_->setText(filename.c_str());
	parameter_file_edit_->update();
}

bool DlgAmberMinimization::getUseDistanceDependentDC() const
{
	return (distance_dependent_button_->isChecked() == true);
}

void DlgAmberMinimization::setUseDistanceDependentDC(bool use_dddc)
{
	distance_dependent_button_->setChecked(use_dddc);
}

bool DlgAmberMinimization::getUseConjugateGradient() const
{
	return (cg_minimizer_button_->isChecked() == true);
}

void DlgAmberMinimization::setUseConjugateGradient(bool use_CG)
{
	cg_minimizer_button_->setChecked(use_CG);
}
