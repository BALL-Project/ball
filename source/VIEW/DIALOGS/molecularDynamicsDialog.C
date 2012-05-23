// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/DIALOGS/molecularDynamicsDialog.h>
#include <BALL/VIEW/DIALOGS/amberConfigurationDialog.h>
#include <BALL/VIEW/DIALOGS/charmmConfigurationDialog.h>
#include <BALL/VIEW/DIALOGS/MMFF94ConfigurationDialog.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/SYSTEM/path.h>

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMessageBox>

namespace BALL
{
	namespace VIEW
	{

MolecularDynamicsDialog::MolecularDynamicsDialog(QWidget* parent, const char* name)
	:	QDialog(parent),
		Ui_MolecularDynamicsDialogData(),
		PreferencesEntry(),
		amber_dialog_(0),
		charmm_dialog_(0),
		mmff_dialog_(0)
{
	setINIFileSectionName("MDSIMULATION");

	setupUi(this);
	setObjectName(name);
	registerWidgets_();
	
	// signals and slots connections
	connect( start_button, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect( cancel_button, SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect( steps_lineedit, SIGNAL( textChanged(const QString&) ), this, SLOT( timeChanged() ) );
	connect( timestep_linedit, SIGNAL( textChanged(const QString&) ), this, SLOT( timeChanged() ) );
	connect( enable_dcd, SIGNAL( stateChanged(int) ), this, SLOT( enableDCDFileSelected() ) );
	connect( advanced_button, SIGNAL( clicked() ), this, SLOT( advancedOptions() ) );
	connect( browse_button, SIGNAL( clicked() ), this, SLOT( chooseDCDFile() ) );
}

MolecularDynamicsDialog::~MolecularDynamicsDialog()
{
}

float MolecularDynamicsDialog::getSimulationTime() const
{
	try
	{
		return ascii(time_lineedit->text()).toFloat();
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
		return ascii(timestep_linedit->text()).toFloat();
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
		return (Size)ascii(steps_lineedit->text()).toUnsignedInt();
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
		return (Size)ascii(temperature_lineedit->text()).toFloat();
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

QString MolecularDynamicsDialog::getDCDFile() const
{
	if (!dcd_file_edit->isEnabled()) return "";
	return dcd_file_edit->text();
}

Size MolecularDynamicsDialog::getStepsBetweenRefreshs() const
{
	return ascii(refresh_lineedit->text()).toUnsignedInt();
}

void MolecularDynamicsDialog::advancedOptions()
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

void MolecularDynamicsDialog::setAmberDialog(AmberConfigurationDialog* dialog)
{
	amber_dialog_ = dialog;
}

void MolecularDynamicsDialog::setCharmmDialog(CharmmConfigurationDialog* dialog)
{
	charmm_dialog_ = dialog;
}

void MolecularDynamicsDialog::setMMFF94Dialog(MMFF94ConfigurationDialog* dialog)
{
	mmff_dialog_ = dialog;
}

void MolecularDynamicsDialog::chooseDCDFile()
{
	QString result = QFileDialog::getSaveFileName(0, tr("Choose a DCDFile"),
																	getMainControl()->getWorkingDir().c_str(), "*.dcd");
	if (result != "") dcd_file_edit->setText(result);
}

void MolecularDynamicsDialog::selectForceField(Position nr)
{
	if 			(nr == 0) useAmberRadioButton->setChecked(true);
	else if (nr == 1) useCharmmRadioButton->setChecked(true);
	else if (nr == 2) useMMFF94RadioButton->setChecked(true);
	else
	{
		BALLVIEW_DEBUG
	}
}

Position MolecularDynamicsDialog::selectedForceField() const
{
	if 			(useAmberRadioButton->isChecked())  return 0;
	else if (useCharmmRadioButton->isChecked()) return 1;
	else if (useMMFF94RadioButton->isChecked()) return 2;

	return 0;
}
	
void MolecularDynamicsDialog::accept()
{
	QString error;
	if (getNumberOfSteps() == 0)
	{
		error = tr("number of steps.");
	}

	if (getTemperature() == 0)
	{
		error = tr("temperature.");
	}

	if (getTimeStep() == 0.0)
	{
		error = tr("time step.");
	}

	if (error == "")
	{
		QDialog::accept();
		return;
	}

	QMessageBox::critical(this, tr("Invalid values"), tr("Please apply correct settings") + " (> 0) " + tr("for the") + " " + error,
			QMessageBox::Ok| QMessageBox::Default);
}

void MolecularDynamicsDialog::setDCDFile(const String& name)
{
	if (name == "")
	{
		dcd_file_edit->setText("");
		enable_dcd->setEnabled(false);
	}
	else
	{
		dcd_file_edit->setText(name.c_str());
		enable_dcd->setEnabled(true);
	}
}


}} //namespaces
