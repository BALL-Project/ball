// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/DIALOGS/molecularDynamicsDialog.h>
#include <BALL/VIEW/DIALOGS/amberConfigurationDialog.h>
#include <BALL/VIEW/DIALOGS/charmmConfigurationDialog.h>
#include <BALL/SYSTEM/path.h>

#include <qfiledialog.h>
#include <qlineedit.h>
#include <qradiobutton.h>
#include <qbuttongroup.h>
#include <qcheckbox.h>
#include <qlabel.h>

namespace BALL
{
	namespace VIEW
	{

MolecularDynamicsDialog::MolecularDynamicsDialog(QWidget* parent, const char* name)
	:	MolecularDynamicsDialogData( parent, name ),
		amber_dialog_(0),
		charmm_dialog_(0)
{
	setINIFileSectionName("MDSIMULATION");

	registerObject_(temperature_lineedit);
	registerObject_(timestep_linedit);
	registerObject_(steps_lineedit);
 	registerObject_(dielectric_group_2);
}

MolecularDynamicsDialog::~MolecularDynamicsDialog()
{
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

void MolecularDynamicsDialog::enableDCDFileSelected()
{
	dcd_file_edit->setEnabled(enable_dcd->isChecked());
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
	if(useAmberRadioButton->isChecked())
	{
		if (amber_dialog_ != 0) amber_dialog_->exec();
	}
	else
	{
		if (charmm_dialog_ != 0) charmm_dialog_->exec();
	}
}

void MolecularDynamicsDialog::setAmberDialog(AmberConfigurationDialog* dialog)
{
	amber_dialog_ = dialog;
}

void MolecularDynamicsDialog::setCharmmDialog(CharmmConfigurationDialog* dialog)
{
	charmm_dialog_ = dialog;
}

void MolecularDynamicsDialog::useAmberFF()
{
	useAmberRadioButton->setChecked(true);
	useCharmmRadioButton->setChecked(false);
}

void MolecularDynamicsDialog::useCharmmFF()
{
	useCharmmRadioButton->setChecked(true);
	useAmberRadioButton->setChecked(false);
}

bool MolecularDynamicsDialog::getUseAmber()
{
	return useAmberRadioButton->isChecked();
}

void MolecularDynamicsDialog::chooseDCDFile()
{
	QString result = QFileDialog::getSaveFileName("", "*.dcd", 0, "Choose a DCDFile");
	if (result != "") dcd_file_edit->setText(result);
}

}} //namespaces
