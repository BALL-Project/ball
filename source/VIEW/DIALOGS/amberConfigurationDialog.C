// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: amberConfigurationDialog.C,v 1.9 2004/04/30 11:45:18 amoll Exp $
//

#include <BALL/VIEW/DIALOGS/amberConfigurationDialog.h>
#include <BALL/MOLMEC/AMBER/amber.h>

#include <qfiledialog.h>
#include <qlineedit.h>
#include <qradiobutton.h>
#include <qcheckbox.h>
#include <qpushbutton.h>

namespace BALL
{
	namespace VIEW
	{

		AmberConfigurationDialog::AmberConfigurationDialog(QWidget* parent, const char* name)
			:	AmberConfigurationDialogData(parent, name),
				amber_(0)
		{
		}

		AmberConfigurationDialog::~AmberConfigurationDialog()
		{
		}

		void AmberConfigurationDialog::browseParameterFiles()
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

		void AmberConfigurationDialog::resetOptions()
		{
			nonbonded_cutoff_line_edit->setText("20.000000");
			vdw_cutoff_line_edit->setText("15.000000");
			vdw_cuton_line_edit->setText("13.000000");
			electrostatic_cutoff_line_edit->setText("15.000000");
			electrostatic_cuton_line_edit->setText("13.000000");
			scaling_electrostatic_1_4_line_edit->setText("2.000000");
			scaling_vdw_1_4_line_edit->setText("2.000000");

			distance_button->setChecked(false);
			constant_button->setChecked(true);
			assign_charges_checkBox->setChecked(true);
			assign_typenames_checkBox->setChecked(true);
			assign_types_checkBox->setChecked(true);
			overwrite_charges_checkBox->setChecked(true);
			overwrite_typenames_checkBox->setChecked(true);

			boundary_box->setChecked(false);
			add_solvent_box->setChecked(false);
			box_size_edit->setText("40.0000");
			solvent_file_edit->setText("");
		}

		const String& AmberConfigurationDialog::getFilename() const
		{
			static String filename;
			filename = parameter_file_edit->text().ascii();
			return filename;
		}
		
		void AmberConfigurationDialog::setFilename(const String& filename)
		{
			parameter_file_edit->setText(filename.c_str());
		}
		

		void AmberConfigurationDialog::writePreferences(INIFile& inifile) const
			throw()
		{
			// the AMBER options
			if (!inifile.hasSection("AMBER")) inifile.appendSection("AMBER");
			inifile.insertValue("AMBER", "Filename", getFilename());
			inifile.insertValue("AMBER", "NONBONDED_CUTOFF",nonbonded_cutoff_);
			inifile.insertValue("AMBER", "VDW_CUTOFF",vdw_cutoff_);
			inifile.insertValue("AMBER", "VDW_CUTON",vdw_cuton_);
			inifile.insertValue("AMBER", "ELECTROSTATIC_CUTOFF",electrostatic_cutoff_);
			inifile.insertValue("AMBER", "ELECTROSTATIC_CUTON",electrostatic_cuton_);
			inifile.insertValue("AMBER", "SCALING_ELECTROSTATIC_1_4",scaling_electrostatic_1_4_);
			inifile.insertValue("AMBER", "SCALING_VDW_1_4",scaling_vdw_1_4_);
			
			inifile.insertValue("AMBER", "ASSIGN_CHARGES",assign_charges_);
			inifile.insertValue("AMBER", "ASSIGN_TYPENAMES",assign_typenames_);
			inifile.insertValue("AMBER", "ASSIGN_TYPES",assign_types_);
			inifile.insertValue("AMBER", "OVERWRITE_CHARGES",overwrite_charges_);
			inifile.insertValue("AMBER", "OVERWRITE_TYPENAMES",overwrite_typenames_);
		}


		void AmberConfigurationDialog::fetchPreferences(const INIFile& inifile)
			throw()
		{
			// the AMBER options
			if (inifile.hasEntry("AMBER", "DistanceDependentDC"))
			{
				use_dddc_ = inifile.getValue("AMBER", "DistanceDependentDC").toUnsignedInt() == 1;
			}

			if (inifile.hasEntry("AMBER", "NONBONDED_CUTOFF"))
			{
				nonbonded_cutoff_ = inifile.getValue("AMBER", "NONBONDED_CUTOFF").toFloat();
			}
			
			if (inifile.hasEntry("AMBER", "VDW_CUTOFF"))
			{
				vdw_cutoff_ = inifile.getValue("AMBER", "VDW_CUTOFF").toFloat();
			}
			
			if (inifile.hasEntry("AMBER", "VDW_CUTON"))
			{
				vdw_cuton_ = inifile.getValue("AMBER", "VDW_CUTON").toFloat();
			}
			
			if (inifile.hasEntry("AMBER", "ELECTROSTATIC_CUTOFF"))
			{
				electrostatic_cutoff_ = inifile.getValue("AMBER", "ELECTROSTATIC_CUTOFF").toFloat();
			}
			
			if (inifile.hasEntry("AMBER", "ELECTROSTATIC_CUTON"))
			{
				electrostatic_cuton_ = inifile.getValue("AMBER", "ELECTROSTATIC_CUTON").toFloat();
			}
			if (inifile.hasEntry("AMBER", "SCALING_ELECTROSTATIC_1_4"))
			{
				scaling_electrostatic_1_4_ = inifile.getValue("AMBER", "SCALING_ELECTROSTATIC_1_4").toFloat();
			}
			
			if (inifile.hasEntry("AMBER", "SCALING_VDW_1_4"))
			{
				scaling_vdw_1_4_ = inifile.getValue("AMBER", "SCALING_VDW_1_4").toFloat();
			}
			
			if (inifile.hasEntry("AMBER", "ASSIGN_CHARGES"))
			{
				assign_charges_ = inifile.getValue("AMBER", "ASSIGN_CHARGES").toUnsignedInt();
			}
			
			if (inifile.hasEntry("AMBER", "ASSIGN_TYPENAMES"))
			{
				assign_typenames_ = inifile.getValue("AMBER", "ASSIGN_TYPENAMES").toUnsignedInt();
			}
			
			if (inifile.hasEntry("AMBER", "ASSIGN_TYPES"))
			{
				assign_types_ = inifile.getValue("AMBER", "ASSIGN_TYPES").toUnsignedInt();
			}
			
			if (inifile.hasEntry("AMBER", "OVERWRITE_CHARGES"))
			{
				overwrite_charges_ = inifile.getValue("AMBER", "OVERWRITE_CHARGES").toUnsignedInt();
			}
			
			if (inifile.hasEntry("AMBER", "OVERWRITE_TYPENAMES"))
			{
				overwrite_typenames_ = inifile.getValue("AMBER", "OVERWRITE_TYPENAMES").toUnsignedInt();
			}

			if (inifile.hasEntry("AMBER", "Filename"))
			{
				setFilename(inifile.getValue("AMBER", "Filename"));
			}
		}

		float AmberConfigurationDialog::getNonbondedCutoff() const
		{
			return nonbonded_cutoff_;
		}

		float AmberConfigurationDialog::getVdwCutoff() const
		{
			return vdw_cutoff_;
		}

		float AmberConfigurationDialog::getVdwCuton() const
		{
			return vdw_cuton_;
		}

		float AmberConfigurationDialog::getElectrostaticCutoff() const
		{
			return electrostatic_cutoff_;
		}

		float AmberConfigurationDialog::getElectrostaticCuton() const
		{
			return electrostatic_cuton_;
		}

		float AmberConfigurationDialog::getScalingElectrostatic_1_4() const
		{
			return scaling_electrostatic_1_4_;
		}

		float AmberConfigurationDialog::getScalingVdw_1_4() const
		{
			return scaling_vdw_1_4_;
		}

		bool AmberConfigurationDialog::getAssignCharges() const
		{
			return assign_charges_;
		}

		bool AmberConfigurationDialog::getAssignTypenames() const
		{
			return assign_typenames_;
		}

		bool AmberConfigurationDialog::getAssignTypes() const
		{
			return assign_types_;
		}

		bool AmberConfigurationDialog::getOverwriteCharges() const
		{
			return overwrite_charges_;
		}

		bool AmberConfigurationDialog::getOverwriteTypenames() const
		{
			return overwrite_typenames_;
		}

		bool AmberConfigurationDialog::getUseDistanceDependentDC() const
		{
			return use_dddc_;
		}

		void AmberConfigurationDialog::reject()
		{
			hide();

			nonbonded_cutoff_line_edit->setText(String(nonbonded_cutoff_).c_str());
			vdw_cutoff_line_edit->setText(String(vdw_cutoff_).c_str());
			vdw_cuton_line_edit->setText(String(vdw_cuton_).c_str());
			electrostatic_cutoff_line_edit->setText(String(electrostatic_cutoff_).c_str());
			electrostatic_cuton_line_edit->setText(String(electrostatic_cuton_).c_str());
			scaling_electrostatic_1_4_line_edit->setText(String(scaling_electrostatic_1_4_).c_str());
			scaling_vdw_1_4_line_edit->setText(String(scaling_vdw_1_4_).c_str());

			distance_button->setChecked(use_dddc_);
			assign_charges_checkBox->setChecked(assign_charges_);
			assign_typenames_checkBox->setChecked(assign_typenames_);
			assign_types_checkBox->setChecked(assign_types_);
			overwrite_charges_checkBox->setChecked(overwrite_charges_);
			overwrite_typenames_checkBox->setChecked(overwrite_typenames_);
			setResult(0);
		}

		void AmberConfigurationDialog::accept()
		{
			try
			{
				nonbonded_cutoff_ = String(nonbonded_cutoff_line_edit->text().ascii()).toFloat();
				vdw_cutoff_ = String(vdw_cutoff_line_edit->text().ascii()).toFloat();
				vdw_cuton_ = String(vdw_cuton_line_edit->text().ascii()).toFloat();
				electrostatic_cutoff_ = String(electrostatic_cutoff_line_edit->text().ascii()).toFloat();
				electrostatic_cuton_ = String(electrostatic_cuton_line_edit->text().ascii()).toFloat();
				scaling_electrostatic_1_4_ = String(scaling_electrostatic_1_4_line_edit->text().ascii()).toFloat();
				scaling_vdw_1_4_ = String(scaling_vdw_1_4_line_edit->text().ascii()).toFloat();
			}
			catch(Exception::GeneralException e)
			{
				Log.error() << "Invalid value: " << std::endl << e << std::endl;
				return;
			}

			use_dddc_ = distance_button->isChecked();
			assign_charges_ = assign_charges_checkBox->isChecked();
			assign_types_ = assign_types_checkBox->isChecked();
			assign_typenames_ = assign_typenames_checkBox->isChecked();
			overwrite_typenames_ = overwrite_typenames_checkBox->isChecked();
			overwrite_charges_ = overwrite_charges_checkBox->isChecked();

			if (amber_ != 0) applyTo(*amber_);
			setResult(1);
			hide();
		}


		void AmberConfigurationDialog::applyTo(AmberFF& amber)
			throw()
		{
			amber.options[AmberFF::Option::ASSIGN_TYPES] = (getAssignTypes() ? "true" : "false");
			amber.options[AmberFF::Option::ASSIGN_CHARGES] = (getAssignCharges()  ? "true" : "false");
			amber.options[AmberFF::Option::ASSIGN_TYPENAMES] = (getAssignTypenames() ? "true" : "false");
			amber.options[AmberFF::Option::OVERWRITE_CHARGES] = (getOverwriteCharges() ? "true" : "false");
			amber.options[AmberFF::Option::OVERWRITE_TYPENAMES] = (getOverwriteTypenames() ? "true" : "false");

			amber.options[AmberFF::Option::DISTANCE_DEPENDENT_DIELECTRIC] = getUseDistanceDependentDC();
			amber.options[AmberFF::Option::NONBONDED_CUTOFF] = getNonbondedCutoff();
			amber.options[AmberFF::Option::VDW_CUTOFF] = getVdwCutoff();
			amber.options[AmberFF::Option::VDW_CUTON] = getVdwCuton();
			amber.options[AmberFF::Option::ELECTROSTATIC_CUTOFF] = getElectrostaticCutoff();
			amber.options[AmberFF::Option::ELECTROSTATIC_CUTON] = getElectrostaticCuton();
			amber.options[AmberFF::Option::SCALING_ELECTROSTATIC_1_4] = getScalingElectrostatic_1_4();
			amber.options[AmberFF::Option::SCALING_VDW_1_4] = getScalingVdw_1_4();

			amber.options[AmberFF::Option::FILENAME] = getFilename();

			/*
			if (boundary_box->isChecked())
			{
				amber.options[PeriodicBoundary::Option::PERIODIC_BOX_ENABLED] = "true";
				try
				{
					amber.options[PeriodicBoundary::Option::PERIODIC_BOX_DISTANCE] = 
						String(box_size_edit->text().ascii()).toFloat();
				}
				catch(...)
				{
					Log.error() << "Invalid distance for Periodic Boundary choosen." << std::endl;
				}
				if (add_solvent_box->isChecked())
				{
					amber.options[PeriodicBoundary::Option::PERIODIC_BOX_ADD_SOLVENT] = "true";

					if (solvent_file_edit->text() != "")
					{
						amber.options[PeriodicBoundary::Option::PERIODIC_BOX_SOLVENT_FILE] = 
							solvent_file_edit->text().ascii();
					}
				}
			}
			else
			{
				amber.options[PeriodicBoundary::Option::PERIODIC_BOX_ENABLED] = "false";
				amber.options[PeriodicBoundary::Option::PERIODIC_BOX_ADD_SOLVENT] = "false";
			}
		}
*/
		void AmberConfigurationDialog::setAmberFF(AmberFF& amber)
			throw()
		{
			amber_ = &amber;
		}
	
		void AmberConfigurationDialog::chooseSolventFile()
			throw()
		{
			QString result = QFileDialog::getOpenFileName(
					solvent_file_edit->text().ascii(), "", 0, "Select a solvent file");
			if (!result.isEmpty())
			{
				solvent_file_edit->setText(result);
			}
		}

		void AmberConfigurationDialog::periodicBoundaryClicked()
			throw()
		{
			/*
			if (boundary_box->isChecked())
			{
				add_solvent_box->setEnabled(true);
				solvent_file_edit->setEnabled(true);
				box_size_edit->setEnabled(true);
				browse_solvent_file->setEnabled(true);
			}
			else
			{
				add_solvent_box->setEnabled(false);
				solvent_file_edit->setEnabled(false);
				box_size_edit->setEnabled(false);
				add_solvent_box->setChecked(false);
				browse_solvent_file->setEnabled(false);
			}
			*/
		}
	
	}//namespace VIEW
}//namespace BALL
