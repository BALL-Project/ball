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
	usedddc = false;
	ini = "Amber/amber96.ini";
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

	// the AMBER options
	if (inifile.hasEntry("AMBER", "Filename"))
	{
		setFilename(inifile.getValue("AMBER", "Filename"));
	}
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

bool MolecularDynamicsDialog::getUseDistanceDependentDC() const
{
	return usedddc;
}

void MolecularDynamicsDialog::setUseDistanceDependentDC(bool use_dddc)
{
	usedddc=use_dddc;
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
	advancedOptionsDialog* dialog = new advancedOptionsDialog();
	if( dialog->exec() == QDialog::Accepted )
	{
		// set inifile to chosen file
		String filename = (dialog->parameter_file_edit)->text().ascii();
		ini = filename;
		// show chosen amber-ini-file in line edit
		parameter_file_edit->setText(filename);
		// set use_dddc
		usedddc = dialog->getUseDistanceDependentDC();
	}
	delete dialog;
}

}} //namespaces
