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
	assign_charges = true;
	assign_typenames = true;
	assign_types = true;
	overwrite_charges = true;
	overwrite_typenames = false;
	ini = "Amber/amber96.ini";
	nonbonded_cutoff = 20.0;
	vdw_cutoff = 15.0;
	vdw_cuton = 13.0;
	electrostatic_cutoff =15.0;
	electrostatic_cuton = 13.0;
	scaling_electrostatic_1_4 = 2.0;
	scaling_vdw_1_4 = 2.0;
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

//EXPERIMENTAL
	inifile.insertValue("MINIMIZATION","NONBONDED_CUTOFF",nonbonded_cutoff);
	inifile.insertValue("MINIMIZATION","VDW_CUTOFF",vdw_cutoff);
	inifile.insertValue("MINIMIZATION","VDW_CUTON",vdw_cuton);
	inifile.insertValue("MINIMIZATION","ELECTROSTATIC_CUTOFF",electrostatic_cutoff);
	inifile.insertValue("MINIMIZATION","ELECTROSTATIC_CUTON",electrostatic_cuton);
	inifile.insertValue("MINIMIZATION","SCALING_ELECTROSTATIC_1_4",scaling_electrostatic_1_4);
	inifile.insertValue("MINIMIZATION","SCALING_VDW_1_4",scaling_vdw_1_4);
	
	inifile.insertValue("MINIMIZATION","ASSIGN_CHARGES",assign_charges);
	inifile.insertValue("MINIMIZATION","ASSIGN_TYPENAMES",assign_typenames);
	inifile.insertValue("MINIMIZATION","ASSIGN_TYPES",assign_types);
	inifile.insertValue("MINIMIZATION","OVERWRITE_CHARGES",overwrite_charges);
	inifile.insertValue("MINIMIZATION","OVERWRITE_TYPENAMES",overwrite_typenames);
//EXPERIMENTAL ende

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

//EXPERIMENTAL
	if(inifile.hasEntry("MINIMIZATION","NONBONDED_CUTOFF"))
	{
		nonbonded_cutoff = inifile.getValue("MINIMIZATION","NONBONDED_CUTOFF").toFloat();
	}
	
	if(inifile.hasEntry("MINIMIZATION","VDW_CUTOFF"))
	{
		vdw_cutoff = inifile.getValue("MINIMIZATION","VDW_CUTOFF").toFloat();
	}
	
	if(inifile.hasEntry("MINIMIZATION","VDW_CUTON"))
	{
		vdw_cuton = inifile.getValue("MINIMIZATION","VDW_CUTON").toFloat();
	}
	
	if(inifile.hasEntry("MINIMIZATION","ELECTROSTATIC_CUTOFF"))
	{
		electrostatic_cutoff = inifile.getValue("MINIMIZATION","ELECTROSTATIC_CUTOFF").toFloat();
	}
	
	if(inifile.hasEntry("MINIMIZATION","ELECTROSTATIC_CUTON"))
	{
		electrostatic_cuton = inifile.getValue("MINIMIZATION","ELECTROSTATIC_CUTON").toFloat();
	}
	if(inifile.hasEntry("MINIMIZATION","SCALING_ELECTROSTATIC_1_4"))
	{
		scaling_electrostatic_1_4 = inifile.getValue("MINIMIZATION","SCALING_ELECTROSTATIC_1_4").toFloat();
	}
	
	if(inifile.hasEntry("MINIMIZATION","SCALING_VDW_1_4"))
	{
		scaling_vdw_1_4 = inifile.getValue("MINIMIZATION","SCALING_VDW_1_4").toFloat();
	}
	
	if(inifile.hasEntry("MINIMIZATION","ASSIGN_CHARGES"))
	{
		assign_charges = inifile.getValue("MINIMIZATION","ASSIGN_CHARGES").toUnsignedInt();
	}
	
	if(inifile.hasEntry("MINIMIZATION","ASSIGN_TYPENAMES"))
	{
		assign_typenames = inifile.getValue("MINIMIZATION","ASSIGN_TYPENAMES").toUnsignedInt();
	}
	
	if(inifile.hasEntry("MINIMIZATION","ASSIGN_TYPES"))
	{
		assign_types = inifile.getValue("MINIMIZATION","ASSIGN_TYPES").toUnsignedInt();
	}
	
	if(inifile.hasEntry("MINIMIZATION","OVERWRITE_CHARGES"))
	{
		overwrite_charges = inifile.getValue("MINIMIZATION","OVERWRITE_CHARGES").toUnsignedInt();
	}
	
	if(inifile.hasEntry("MINIMIZATION","OVERWRITE_TYPENAMES"))
	{
		overwrite_typenames = inifile.getValue("MINIMIZATION","OVERWRITE_TYPENAMES").toUnsignedInt();
	}
	
//EXPERIMENTAL ende

	// the AMBER options
	if (inifile.hasEntry("AMBER", "Filename"))
	{
		setFilename(inifile.getValue("AMBER", "Filename"));
	}
}

float AmberMinimizationDialog::getNonbondedCutoff() const
{
	return nonbonded_cutoff;
}

float AmberMinimizationDialog::getVdwCutoff() const
{
	return vdw_cutoff;
}

float AmberMinimizationDialog::getVdwCuton() const
{
	return vdw_cuton;
}

float AmberMinimizationDialog::getElectrostaticCutoff() const
{
	return electrostatic_cutoff;
}

float AmberMinimizationDialog::getElectrostaticCuton() const
{
	return electrostatic_cuton;
}

float AmberMinimizationDialog::getScalingElectrostatic_1_4() const
{
	return scaling_electrostatic_1_4;;
}

float AmberMinimizationDialog::getScalingVdw_1_4() const
{
	return scaling_vdw_1_4;;
}

bool AmberMinimizationDialog::getAssignCharges() const
{
	return assign_charges;
}

bool AmberMinimizationDialog::getAssignTypenames() const
{
	return assign_typenames;
}

bool AmberMinimizationDialog::getAssignTypes() const
{
	return assign_types;
}

bool AmberMinimizationDialog::getOverwriteCharges() const
{
	return overwrite_charges;
}

bool AmberMinimizationDialog::getOverwriteTypenames() const
{
	return overwrite_typenames;
}

bool AmberMinimizationDialog::getUseDistanceDependentDC() const
{
	return use_dddc;
}

void AmberMinimizationDialog::setUseDistanceDependentDC(bool usedddc)
{
	use_dddc=usedddc;
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
	
	//restore previos changes  in dialog
	dialog->setOptions( nonbonded_cutoff, vdw_cutoff, vdw_cuton, electrostatic_cutoff, electrostatic_cuton,
				    scaling_electrostatic_1_4, scaling_vdw_1_4, use_dddc, assign_charges, assign_typenames,
				    assign_types, overwrite_charges, overwrite_typenames);
				    
	if(dialog->exec() == QDialog::Accepted )
	{
		// set inifile to chosen file
		String filename = (dialog->parameter_file_edit)->text().ascii();
		ini = filename;
		// show chosen amber-ini-file in line edit
		parameter_file_edit->setText(filename);
		// set AmberFF Options
		use_dddc = dialog->getUseDistanceDependentDC();
		nonbonded_cutoff = dialog->getNonbondedCutoff();
		vdw_cutoff = dialog->getVdwCutoff();
		vdw_cuton = dialog->getVdwCuton();
		electrostatic_cutoff = dialog->getElectrostaticCutoff();
		electrostatic_cuton = dialog->getElectrostaticCuton();
		scaling_electrostatic_1_4 = dialog->getScalingElectrostatic_1_4();
		scaling_vdw_1_4 = dialog->getScalingVdw_1_4();
		
		assign_charges = dialog->getAssignCharges();
		assign_typenames = dialog->getAssignTypenames();
		assign_types = dialog->getAssignTypes();
		overwrite_charges = dialog->getOverwriteCharges();
		overwrite_typenames = dialog->getOverwriteTypenames();
		
	}
	delete dialog;
}

}} //namespaces
