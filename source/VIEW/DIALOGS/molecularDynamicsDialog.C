// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/DIALOGS/molecularDynamicsDialog.h>
#include <BALL/VIEW/DIALOGS/advancedOptionsDialog.h>

#include <qfiledialog.h>
#include <qlineedit.h>
#include <qradiobutton.h>
#include <qcheckbox.h>

namespace BALL
{
	namespace VIEW
	{

MolecularDynamicsDialog::MolecularDynamicsDialog(QWidget* parent, const char* name)
	:	MolecularDynamicsDialogData( parent, name )
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

MolecularDynamicsDialog::~MolecularDynamicsDialog()
{
}

void MolecularDynamicsDialog::browseParameterFiles()
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

void MolecularDynamicsDialog::writePreferences(INIFile& inifile) const
{
	// the minimizer options
	if (!inifile.hasSection("MDSIMULATION")) inifile.appendSection("MDSIMULATION");
	inifile.insertValue("MDSIMULATION", "MicroCanonical", useMicroCanonical());
	inifile.insertValue("MDSIMULATION", "DistanceDependentDC", getUseDistanceDependentDC());
	inifile.insertValue("MDSIMULATION", "NumberOfSteps", getNumberOfSteps());
	inifile.insertValue("MDSIMULATION", "Timestep", getTimeStep());
	inifile.insertValue("MDSIMULATION", "Temperature", getTemperature());
	
	inifile.insertValue("MDSIMULATION","NONBONDED_CUTOFF",nonbonded_cutoff);
	inifile.insertValue("MDSIMULATION","VDW_CUTOFF",vdw_cutoff);
	inifile.insertValue("MDSIMULATION","VDW_CUTON",vdw_cuton);
	inifile.insertValue("MDSIMULATION","ELECTROSTATIC_CUTOFF",electrostatic_cutoff);
	inifile.insertValue("MDSIMULATION","ELECTROSTATIC_CUTON",electrostatic_cuton);
	inifile.insertValue("MDSIMULATION","SCALING_ELECTROSTATIC_1_4",scaling_electrostatic_1_4);
	inifile.insertValue("MDSIMULATION","SCALING_VDW_1_4",scaling_vdw_1_4);
	
	inifile.insertValue("MDSIMULATION","ASSIGN_CHARGES",assign_charges);
	inifile.insertValue("MDSIMULATION","ASSIGN_TYPENAMES",assign_typenames);
	inifile.insertValue("MDSIMULATION","ASSIGN_TYPES",assign_types);
	inifile.insertValue("MDSIMULATION","OVERWRITE_CHARGES",overwrite_charges);
	inifile.insertValue("MDSIMULATION","OVERWRITE_TYPENAMES",overwrite_typenames);
	
	// the AMBER options
	if (!inifile.hasSection("AMBER")) inifile.appendSection("AMBER");
	inifile.insertValue("AMBER", "Filename", getFilename());
}


void MolecularDynamicsDialog::readPreferences(const INIFile& inifile)
{
	// the minimizer options
	if (inifile.hasEntry("MDSIMULATION", "MicroCanonical"))
	{
		setMicroCanonical((Size)inifile.getValue("MDSIMULATION", "MicroCanonical").toUnsignedInt());
	}
	if (inifile.hasEntry("MDSIMULATION", "DistanceDependentDC"))
	{
		setUseDistanceDependentDC(inifile.getValue("MDSIMULATION", "DistanceDependentDC").toUnsignedInt() == 1);
	}
	if (inifile.hasEntry("MDSIMULATION", "NumberOfSteps"))
	{
		 setNumberOfSteps(inifile.getValue("MDSIMULATION", "NumberOfSteps").toUnsignedInt());
	}
	if (inifile.hasEntry("MDSIMULATION", "Timestep"))
	{
		 setTimeStep(inifile.getValue("MDSIMULATION", "Timestep").toFloat());
	}
	if (inifile.hasEntry("MDSIMULATION", "Temperature"))
	{
		 setTemperature(inifile.getValue("MDSIMULATION", "Temperature").toFloat());
	}
	
	if(inifile.hasEntry("MDSIMULATION","NONBONDED_CUTOFF"))
	{
		nonbonded_cutoff = inifile.getValue("MDSIMULATION","NONBONDED_CUTOFF").toFloat();
	}
	
	if(inifile.hasEntry("MDSIMULATION","VDW_CUTOFF"))
	{
		vdw_cutoff = inifile.getValue("MDSIMULATION","VDW_CUTOFF").toFloat();
	}
	
	if(inifile.hasEntry("MDSIMULATION","VDW_CUTON"))
	{
		vdw_cuton = inifile.getValue("MDSIMULATION","VDW_CUTON").toFloat();
	}
	
	if(inifile.hasEntry("MDSIMULATION","ELECTROSTATIC_CUTOFF"))
	{
		electrostatic_cutoff = inifile.getValue("MDSIMULATION","ELECTROSTATIC_CUTOFF").toFloat();
	}
	
	if(inifile.hasEntry("MDSIMULATION","ELECTROSTATIC_CUTON"))
	{
		electrostatic_cuton = inifile.getValue("MDSIMULATION","ELECTROSTATIC_CUTON").toFloat();
	}
	if(inifile.hasEntry("MDSIMULATION","SCALING_ELECTROSTATIC_1_4"))
	{
		scaling_electrostatic_1_4 = inifile.getValue("MDSIMULATION","SCALING_ELECTROSTATIC_1_4").toFloat();
	}
	
	if(inifile.hasEntry("MDSIMULATION","SCALING_VDW_1_4"))
	{
		scaling_vdw_1_4 = inifile.getValue("MDSIMULATION","SCALING_VDW_1_4").toFloat();
	}
	
	if(inifile.hasEntry("MDSIMULATION","ASSIGN_CHARGES"))
	{
		assign_charges = inifile.getValue("MDSIMULATION","ASSIGN_CHARGES").toUnsignedInt() == 1;
	}
	
	if(inifile.hasEntry("MDSIMULATION","ASSIGN_TYPENAMES"))
	{
		assign_typenames = inifile.getValue("MDSIMULATION","ASSIGN_TYPENAMES").toUnsignedInt() == 1;
	}
	
	if(inifile.hasEntry("MDSIMULATION","ASSIGN_TYPES"))
	{
		assign_types = inifile.getValue("MDSIMULATION","ASSIGN_TYPES").toUnsignedInt() == 1;
	}
	
	if(inifile.hasEntry("MDSIMULATION","OVERWRITE_CHARGES"))
	{
		overwrite_charges = inifile.getValue("MDSIMULATION","OVERWRITE_CHARGES").toUnsignedInt() == 1;
	}
	
	if(inifile.hasEntry("MDSIMULATION","OVERWRITE_TYPENAMES"))
	{
		overwrite_typenames = inifile.getValue("MDSIMULATION","OVERWRITE_TYPENAMES").toUnsignedInt() == 1;
	}

	// the AMBER options
	if (inifile.hasEntry("AMBER", "Filename"))
	{
		setFilename(inifile.getValue("AMBER", "Filename"));
	}
}

float MolecularDynamicsDialog::getNonbondedCutoff() const
{
	return nonbonded_cutoff;
}

float MolecularDynamicsDialog::getVdwCutoff() const
{
	return vdw_cutoff;
}

float MolecularDynamicsDialog::getVdwCuton() const
{
	return vdw_cuton;
}

float MolecularDynamicsDialog::getElectrostaticCutoff() const
{
	return electrostatic_cutoff;
}

float MolecularDynamicsDialog::getElectrostaticCuton() const
{
	return electrostatic_cuton;
}

float MolecularDynamicsDialog::getScalingElectrostatic_1_4() const
{
	return scaling_electrostatic_1_4;;
}

float MolecularDynamicsDialog::getScalingVdw_1_4() const
{
	return scaling_vdw_1_4;;
}

bool MolecularDynamicsDialog::getAssignCharges() const
{
	return assign_charges;
}

bool MolecularDynamicsDialog::getAssignTypenames() const
{
	return assign_typenames;
}

bool MolecularDynamicsDialog::getAssignTypes() const
{
	return assign_types;
}

bool MolecularDynamicsDialog::getOverwriteCharges() const
{
	return overwrite_charges;
}

bool MolecularDynamicsDialog::getOverwriteTypenames() const
{
	return overwrite_typenames;
}

bool MolecularDynamicsDialog::getUseDistanceDependentDC() const
{
	return use_dddc;
}

void MolecularDynamicsDialog::setUseDistanceDependentDC(bool usedddc)
{
	use_dddc=usedddc;
}

float MolecularDynamicsDialog::getSimulationTime() const
{
	try
	{
		return String(time_lineedit->text().ascii()).toFloat();
	}
	catch(...)
	{
		return 0;
	}
}

void MolecularDynamicsDialog::setTimeStep(float time)
{
	timestep_linedit->setText(String(time).c_str());
}

float MolecularDynamicsDialog::getTimeStep() const
{
	try
	{
		return String(timestep_linedit->text().ascii()).toFloat();
	}
	catch(...)
	{
		return 0;
	}
}

Size MolecularDynamicsDialog::getNumberOfSteps() const
{
	try
	{
		return (Size)String(steps_lineedit->text().ascii()).toUnsignedInt();
	}
	catch(...)
	{
		return 0;
	}
}

void MolecularDynamicsDialog::setNumberOfSteps(Size steps)
{
	steps_lineedit->setText(String(steps).c_str());
}

const String& MolecularDynamicsDialog::getFilename() const
{
	static String filename;
	filename = ini;
	return filename;
}

void MolecularDynamicsDialog::setFilename(const String& filename)
{
	ini = filename;
}

bool MolecularDynamicsDialog::useMicroCanonical() const
{
	return microcanonical_button->isChecked();
}

void MolecularDynamicsDialog::setMicroCanonical(bool state)
{
	microcanonical_button->setChecked(state);
}

void MolecularDynamicsDialog::timeChanged()
{
	if (getTimeStep() == 0 || getNumberOfSteps() == 0)
	{
		time_lineedit->setText("");
		return;
	}

	float time = getTimeStep() * getNumberOfSteps();
	time_lineedit->setText(String(time).c_str());
}

float MolecularDynamicsDialog::getTemperature() const
{
	try
	{
		return (Size)String(temperature_lineedit->text().ascii()).toFloat();
	}
	catch(...)
	{
		return 0;
	}
}

void MolecularDynamicsDialog::setTemperature(float temperature)
{
	temperature_lineedit->setText(String(temperature).c_str());
}

bool MolecularDynamicsDialog::saveImages() const
{
	return export_images->isChecked();
}

void MolecularDynamicsDialog::enableDCDFileSelected()
{
	dcd_file_edit->setEnabled(true);
}

String MolecularDynamicsDialog::getDCDFile() const
{
	if (!dcd_file_edit->isEnabled()) return "";
	return String(dcd_file_edit->text().ascii());
}

Size MolecularDynamicsDialog::getStepsBetweenRefreshs() const
{
	return String(refresh_lineedit->text().ascii()).toUnsignedInt();
}

void MolecularDynamicsDialog::advancedOptions()
{
	AdvancedOptionsDialog* dialog = new AdvancedOptionsDialog();
	//restore previos changes  in dialog
	dialog->setOptions( nonbonded_cutoff, vdw_cutoff, vdw_cuton, electrostatic_cutoff, electrostatic_cuton,
				    scaling_electrostatic_1_4, scaling_vdw_1_4, use_dddc, assign_charges, assign_typenames,
				    assign_types, overwrite_charges, overwrite_typenames);
				    
	if( dialog->exec() == QDialog::Accepted )
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
